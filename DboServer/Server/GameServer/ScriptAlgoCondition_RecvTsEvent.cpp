#include "stdafx.h"
#include "ScriptAlgoCondition_RecvTsEvent.h"
#include "WPSNodeCondition_RecvTsEvent.h"
#include "ObjectMsg.h"


CWpsAlgoCondition_RecvTsEvent::CWpsAlgoCondition_RecvTsEvent(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_RECV_TS_EVENT, "SCRIPTCONTROL_CONDITION_RECV_TS_EVENT")
{
}


CWpsAlgoCondition_RecvTsEvent::~CWpsAlgoCondition_RecvTsEvent()
{
}


bool CWpsAlgoCondition_RecvTsEvent::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_RecvTsEvent* pCond = dynamic_cast<CWPSNodeCondition_RecvTsEvent*>(pControlScriptNode);
	if (pCond)
	{
		m_byTriggerType = pCond->m_byTriggerType;
		m_teid = pCond->m_teid;
		m_dwWaitTime = pCond->m_dwWaitTime;
		m_bSynchronize = pCond->m_bSynchronize;

		if (m_teid != -1)
		{
			GetOwner()->GetEventMap()->SetScriptEvent(m_teid);
		}

		return true;
	}

	return false;
}

int CWpsAlgoCondition_RecvTsEvent::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetOwner()->GetEventMap()->IsReceived((DWORD)m_teid))
		m_status = COMPLETED;

	return m_status;
}
