#include "stdafx.h"
#include "BotAiAction_SPS_SendEventToWps.h"
#include "SPSNodeAction_SendEventToWps.h"
#include "WpsAlgoObject.h"


CBotAiAction_SPS_SendEventToWps::CBotAiAction_SPS_SendEventToWps(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_SPS_SEND_EVENT_TO_WPS, "BOTCONTROL_ACTION_SPS_SEND_EVENT_TO_WPS")
{
	m_wpsTblidx = INVALID_TBLIDX;
	m_eventId = INVALID_DWORD;
}


CBotAiAction_SPS_SendEventToWps::~CBotAiAction_SPS_SendEventToWps()
{
}


bool CBotAiAction_SPS_SendEventToWps::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_SendEventToWps* pAction = dynamic_cast<CSPSNodeAction_SendEventToWps*>(pControlScriptNode);
	if (pAction)
	{
		m_wpsTblidx = pAction->m_wpsTblidx;
		m_eventId = pAction->m_eventId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_SendEventToWps", pControlScriptNode);
	return false;
}


void CBotAiAction_SPS_SendEventToWps::OnEnter()
{
}

void CBotAiAction_SPS_SendEventToWps::OnExit()
{
}


int CBotAiAction_SPS_SendEventToWps::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	/*
		Some WPS script are hosted by world so it can be that BOT dont have the script. So try get the script from world.
		Do we need some further checks here?
	*/

	CScriptAlgoObject* pScript = GetBot()->GetScript(m_wpsTblidx);
	if (pScript == NULL)	//check if bot dont have the script
	{
		pScript = GetBot()->GetCurWorld()->GetScript(m_wpsTblidx);	//get wps script from world
	}
	//ERR_LOG(LOG_SCRIPT, "m_wpsTblidx = %u, m_eventId = %u", m_wpsTblidx, m_eventId);
	if (pScript)
	{
		CObjMsg_SendSpsEventToWps cMsg;
		cMsg.eventId = m_eventId;
		pScript->OnObjectMsg(&cMsg);
	//	printf("CBotAiAction_SPS_SendEventToWps: send event %u to wps %u \n", m_eventId, m_wpsTblidx);
	}

	m_status = COMPLETED;
	return m_status;
}