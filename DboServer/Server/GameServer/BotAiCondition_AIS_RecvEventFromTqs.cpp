#include "stdafx.h"
#include "BotAiCondition_AIS_RecvEventFromTqs.h"
#include "AISNodeCondition_RecvEventFromTQS.h"


CBotAiCondition_AIS_RecvEventFromTqs::CBotAiCondition_AIS_RecvEventFromTqs(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_AIS_RECV_EVENT_FROM_TQS, "BOTCONTROL_CONDITION_AIS_RECV_EVENT_FROM_TQS")
{
	m_eventId = INVALID_DWORD;
	m_scriptnodeID = INVALID_DWORD;
}

CBotAiCondition_AIS_RecvEventFromTqs::~CBotAiCondition_AIS_RecvEventFromTqs()
{
}

bool CBotAiCondition_AIS_RecvEventFromTqs::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CAISNodeCondition_RecvEventFromTQS* pCond = dynamic_cast<CAISNodeCondition_RecvEventFromTQS*>(pControlScriptNode);
	if (pCond)
	{
		m_eventId = pCond->m_eventId;
		m_scriptnodeID = pControlScriptNode->GetNodeIndex();

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CAISNodeCondition_RecvEventFromTQS", pControlScriptNode);
	return false;
}

void CBotAiCondition_AIS_RecvEventFromTqs::CopyTo(CControlState* pTo)
{
}

void CBotAiCondition_AIS_RecvEventFromTqs::OnEnter()
{
}

void CBotAiCondition_AIS_RecvEventFromTqs::OnExit()
{
}

int CBotAiCondition_AIS_RecvEventFromTqs::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	ERR_LOG(LOG_BOTAI, "ERROR TO DO");
	return m_status;
}

int CBotAiCondition_AIS_RecvEventFromTqs::OnObjectMsg(CObjectMsg* pObjMsg)
{
	ERR_LOG(LOG_BOTAI, "ERROR TO DO");
	return m_status;
}