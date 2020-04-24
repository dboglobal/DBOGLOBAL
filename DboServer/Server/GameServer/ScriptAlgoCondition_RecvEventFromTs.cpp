#include "stdafx.h"
#include "ScriptAlgoCondition_RecvEventFromTs.h"
#include "WPSNodeCondition_RecvEventFromTs.h"
#include "ObjectMsg.h"


CWpsAlgoCondition_RecvEventFromTs::CWpsAlgoCondition_RecvEventFromTs(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_RECV_EVENT_FROM_TS, "SCRIPTCONTROL_CONDITION_RECV_EVENT_FROM_TS")
{
}


CWpsAlgoCondition_RecvEventFromTs::~CWpsAlgoCondition_RecvEventFromTs()
{
}


bool CWpsAlgoCondition_RecvEventFromTs::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_RecvEventFromTs* pCond = dynamic_cast<CWPSNodeCondition_RecvEventFromTs*>(pControlScriptNode);
	if (pCond)
	{
		m_byTriggerType = pCond->m_byTriggerType;
		m_teId = pCond->m_teId;
		m_dwWaitTime = pCond->m_dwWaitTime;
		m_bSynchronize = pCond->m_bSynchronize;

		if (m_teId != -1)
		{
			GetOwner()->GetEventMap()->SetScriptEvent(m_teId);
		}

		return true;
	}

	return false;
}

int CWpsAlgoCondition_RecvEventFromTs::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetOwner()->GetEventMap()->IsReceived((DWORD)m_teId))
	{
		m_status = COMPLETED;
	}

	return m_status;
}

int CWpsAlgoCondition_RecvEventFromTs::OnObjectMsg(CObjectMsg* pObjMsg)
{
	if (pObjMsg->GetID() == OBJMSG_SEND_TS_EVENT_TO_WPS)
	{
		CObjMsg_SendTsEventToWps* pEvt = static_cast<CObjMsg_SendTsEventToWps*>(pObjMsg);
		if (pEvt)
		{
			if (pEvt->byTriggerType == m_byTriggerType && pEvt->teId == m_teId)
			{
				printf("received wps event from ts m_teId %u\n", m_teId);
				m_status = COMPLETED;
			}
		}
	}

	return m_status;
}