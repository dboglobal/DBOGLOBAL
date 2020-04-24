#include "stdafx.h"
#include "TqsAlgoCondition_RecvSpsEvent.h"
#include "TQSNodeCondition_RecvSpsEvent.h"
#include "Npc.h"


CTqsAlgoCondition_RecvSpsEvent::CTqsAlgoCondition_RecvSpsEvent(CTqsAlgoObject* pObject) :
	CTqsAlgoCondition_Base(pObject, TQS_ALGOCONTROLID_CONDITION_RECV_SPS_EVENT, "TQS_ALGOCONTROLID_CONDITION_RECV_SPS_EVENT")
{
}


CTqsAlgoCondition_RecvSpsEvent::~CTqsAlgoCondition_RecvSpsEvent()
{
}


bool CTqsAlgoCondition_RecvSpsEvent::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeCondition_RecvSpsEvent* pCond = dynamic_cast<CTQSNodeCondition_RecvSpsEvent*>(pControlScriptNode);
	if (pCond)
	{
		m_targetTblidx = pCond->m_targetTblidx;
		m_eventID = pCond->m_eventID;
		m_dwTime = pCond->m_dwTime;

		if (m_eventID != -1)
		{
			GetOwner()->GetEventMap()->SetScriptEvent(m_eventID);
		}

		return true;
	}

	return false;
}

int CTqsAlgoCondition_RecvSpsEvent::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetOwner()->GetEventMap()->IsReceived(m_eventID))
		m_status = COMPLETED;

	return m_status;
}
