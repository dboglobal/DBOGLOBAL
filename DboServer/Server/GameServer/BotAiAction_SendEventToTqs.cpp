#include "stdafx.h"
#include "BotAiAction_SendEventToTqs.h"
#include "SPSNodeAction_SendEventToTqs.h"


CBotAiAction_SendEventToTqs::CBotAiAction_SendEventToTqs(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_SEND_EVENT_TO_TQS, "BOTCONTROL_ACTION_SEND_EVENT_TO_TQS")
{
	m_eventId = INVALID_DWORD;
}


CBotAiAction_SendEventToTqs::~CBotAiAction_SendEventToTqs()
{
}

bool CBotAiAction_SendEventToTqs::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_SendEventToTqs* pAction = dynamic_cast<CSPSNodeAction_SendEventToTqs*>(pControlScriptNode);
	if (pAction)
	{
		m_eventId = pAction->m_eventId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_SendEventToTqs", pControlScriptNode);
	return false;
}

void CBotAiAction_SendEventToTqs::OnEnter()
{
}

void CBotAiAction_SendEventToTqs::OnExit()
{
}

int CBotAiAction_SendEventToTqs::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	ERR_LOG(LOG_BOTAI, "IMPORTANT-ERROR TO DO");
	m_status = COMPLETED;
	return m_status;
}

