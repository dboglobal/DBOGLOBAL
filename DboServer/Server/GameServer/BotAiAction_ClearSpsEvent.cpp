#include "stdafx.h"
#include "BotAiAction_ClearSpsEvent.h"
#include "SPSNodeAction_ClearEvent.h"


CBotAiAction_ClearSpsEvent::CBotAiAction_ClearSpsEvent(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_CLEAR_SPS_EVENT, "BOTCONTROL_ACTION_CLEAR_SPS_EVENT")
{
	m_bClearAll = false;
	m_eventId = INVALID_DWORD;
}


CBotAiAction_ClearSpsEvent::~CBotAiAction_ClearSpsEvent()
{
}


bool CBotAiAction_ClearSpsEvent::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_ClearEvent* pAction = dynamic_cast<CSPSNodeAction_ClearEvent*>(pControlScriptNode);
	if (pAction)
	{
		m_bClearAll = pAction->m_bClearAll;
		m_eventId = pAction->m_eventId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_ClearEvent", pControlScriptNode);
	return false;
}

void CBotAiAction_ClearSpsEvent::CopyTo(CControlState* pTo)
{
	CBotAiAction_ClearSpsEvent* pAction = dynamic_cast<CBotAiAction_ClearSpsEvent*>(pTo);
	if (pAction)
	{
		pAction->m_bClearAll = m_bClearAll;
		pAction->m_eventId = m_eventId;
	}
}

void CBotAiAction_ClearSpsEvent::OnEnter()
{
}

void CBotAiAction_ClearSpsEvent::OnExit()
{
}


int CBotAiAction_ClearSpsEvent::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	GetBot()->GetSpsEventMap()->ClearScriptEvent(m_eventId);
	m_status = COMPLETED;
	return m_status;
}