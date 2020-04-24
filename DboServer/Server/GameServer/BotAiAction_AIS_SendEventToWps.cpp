#include "stdafx.h"
#include "BotAiAction_AIS_SendEventToWps.h"
#include "AISNodeAction_SendEventToWPS.h"


CBotAiAction_AIS_SendEventToWps::CBotAiAction_AIS_SendEventToWps(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_AIS_SEND_EVENT_TO_WPS, "BOTCONTROL_ACTION_AIS_SEND_EVENT_TO_WPS")
{
	m_wpsTblidx = INVALID_TBLIDX;
	m_eventId = INVALID_DWORD;
}


CBotAiAction_AIS_SendEventToWps::~CBotAiAction_AIS_SendEventToWps()
{
}


bool CBotAiAction_AIS_SendEventToWps::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CAISNodeAction_SendEventToWPS* pAction = dynamic_cast<CAISNodeAction_SendEventToWPS*>(pControlScriptNode);
	if (pAction)
	{
		m_wpsTblidx = pAction->m_wpsTblidx;
		m_eventId = pAction->m_eventId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CAISNodeAction_SendEventToWPS", pControlScriptNode);
	return false;
}

void CBotAiAction_AIS_SendEventToWps::CopyTo(CControlState* pTo)
{
}

void CBotAiAction_AIS_SendEventToWps::OnEnter()
{
}

void CBotAiAction_AIS_SendEventToWps::OnExit()
{
}


int CBotAiAction_AIS_SendEventToWps::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	ERR_LOG(LOG_BOTAI, "IMPORTANT-ERROR: ERROR TO DO");
	m_status = COMPLETED;
	return m_status;
}