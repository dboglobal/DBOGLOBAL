#include "stdafx.h"
#include "TqsAlgoCondition_RecvTsEvent.h"
#include "TQSNodeCondition_RecvTsEvent.h"
#include "ObjectMsg.h"
#include "TimeQuest.h"


CTqsAlgoCondition_RecvTsEvent::CTqsAlgoCondition_RecvTsEvent(CTqsAlgoObject* pObject) :
	CTqsAlgoCondition_Base(pObject, TQS_ALGOCONTROLID_CONDITION_RECV_TS_EVENT, "TQS_ALGOCONTROLID_CONDITION_RECV_TS_EVENT")
{
}


CTqsAlgoCondition_RecvTsEvent::~CTqsAlgoCondition_RecvTsEvent()
{
}


bool CTqsAlgoCondition_RecvTsEvent::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeCondition_RecvTsEvent* pCond = dynamic_cast<CTQSNodeCondition_RecvTsEvent*>(pControlScriptNode);
	if (pCond)
	{
		m_byTriggerType = pCond->m_byTriggerType;
		m_teid = pCond->m_teid;
		m_dwWaitTime = pCond->m_dwWaitTime;
		m_bTmqFail = pCond->m_bTmqFail;
		m_bSynchronize = pCond->m_bSynchronize;

		if (m_teid != -1)
		{
			GetOwner()->GetEventMap()->SetScriptEvent(m_teid);
		}

		return true;
	}

	return false;
}

int CTqsAlgoCondition_RecvTsEvent::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_dwWaitTime > 0)
	{
		m_dwWaitTime = UnsignedSafeDecrease<DWORD>(m_dwWaitTime, dwTickDiff);
		if (m_dwWaitTime == 0)
		{
			if (m_bTmqFail)
			{
				GetOwner()->GetTimeQuest()->UpdateTmqFail();
				m_status = COMPLETED;
				return m_status;
			}
		}
	}

	if (m_bSynchronize) // check if received from everyone
	{
		if (GetOwner()->GetEventMap()->IsReceived((DWORD)m_teid, (WORD)GetOwner()->GetPlayersCount()))
		{
			m_status = COMPLETED;
		}
	}
	else //check if only one received
	{
		if (GetOwner()->GetEventMap()->IsReceived((DWORD)m_teid))
		{
			m_status = COMPLETED;
		}
	}

	return m_status;
}
