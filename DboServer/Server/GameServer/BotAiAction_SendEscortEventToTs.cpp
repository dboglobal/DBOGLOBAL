#include "stdafx.h"
#include "BotAiAction_SendEscortEventToTs.h"
#include "SPSNodeAction_SendEscortEventToTs.h"



CBotAiAction_SendEscortEventToTs::CBotAiAction_SendEscortEventToTs(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_SEND_ESCORT_EVENT_TO_TS, "BOTCONTROL_ACTION_SEND_ESCORT_EVENT_TO_TS")
{
	m_teId = -1;
}


CBotAiAction_SendEscortEventToTs::~CBotAiAction_SendEscortEventToTs()
{
}

bool CBotAiAction_SendEscortEventToTs::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_SendEscortEventToTs* pAction = dynamic_cast<CSPSNodeAction_SendEscortEventToTs*>(pControlScriptNode);
	if (pAction)
	{
		m_teId = pAction->m_teId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_SendEscortEventToTs", pControlScriptNode);
	return false;
}

void CBotAiAction_SendEscortEventToTs::OnEnter()
{
}

void CBotAiAction_SendEscortEventToTs::OnExit()
{
}

int CBotAiAction_SendEscortEventToTs::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	ERR_LOG(LOG_BOTAI, "IMPORTANT-ERROR TO DO");
	m_status = COMPLETED;
	return m_status;
}

