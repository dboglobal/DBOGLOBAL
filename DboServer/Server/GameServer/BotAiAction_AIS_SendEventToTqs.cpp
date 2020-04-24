#include "stdafx.h"
#include "BotAiAction_AIS_SendEventToTqs.h"
#include "AISNodeAction_SendEventToTQS.h"


CBotAiAction_AIS_SendEventToTqs::CBotAiAction_AIS_SendEventToTqs(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_AIS_SEND_EVENT_TO_TQS, "BOTCONTROL_ACTION_AIS_SEND_EVENT_TO_TQS")
{
	m_eventId = INVALID_DWORD;
}


CBotAiAction_AIS_SendEventToTqs::~CBotAiAction_AIS_SendEventToTqs()
{
}


bool CBotAiAction_AIS_SendEventToTqs::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CAISNodeAction_SendEventToTQS* pAction = dynamic_cast<CAISNodeAction_SendEventToTQS*>(pControlScriptNode);
	if (pAction)
	{
		m_eventId = pAction->m_eventId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CAISNodeAction_SendEventToTQS", pControlScriptNode);
	return false;
}

void CBotAiAction_AIS_SendEventToTqs::CopyTo(CControlState* pTo)
{
}

void CBotAiAction_AIS_SendEventToTqs::OnEnter()
{
}

void CBotAiAction_AIS_SendEventToTqs::OnExit()
{
}


int CBotAiAction_AIS_SendEventToTqs::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	ERR_LOG(LOG_BOTAI, "IMPORTANT-ERROR TO DO");
	m_status = COMPLETED;
	return m_status;
}