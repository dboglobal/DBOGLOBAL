#include "stdafx.h"
#include "BotAiAction_SPS_SendEvent.h"
#include "SPSNodeAction_SendEvent.h"


CBotAiAction_SPS_SendEvent::CBotAiAction_SPS_SendEvent(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_SPS_SEND_EVENT, "BOTCONTROL_ACTION_SPS_SEND_EVENT")
{
	m_eEventType = CSPSNodeAction_SendEvent::INVALID_EVENTTYPE;
}


CBotAiAction_SPS_SendEvent::~CBotAiAction_SPS_SendEvent()
{
}


bool CBotAiAction_SPS_SendEvent::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_SendEvent* pAction = dynamic_cast<CSPSNodeAction_SendEvent*>(pControlScriptNode);
	if (pAction)
	{
		m_eEventType = pAction->m_eEventType;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_SendEvent", pControlScriptNode);
	return false;
}


void CBotAiAction_SPS_SendEvent::OnEnter()
{
}

void CBotAiAction_SPS_SendEvent::OnExit()
{
}


int CBotAiAction_SPS_SendEvent::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	ERR_LOG(LOG_BOTAI, "IMPORTANT-ERROR. TO DO");
	m_status = COMPLETED;
	return m_status;
}