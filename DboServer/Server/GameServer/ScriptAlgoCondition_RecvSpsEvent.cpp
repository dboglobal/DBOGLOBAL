#include "stdafx.h"
#include "ScriptAlgoCondition_RecvSpsEvent.h"
#include "WPSNodeCondition_RecvSpsEvent.h"
#include "Npc.h"


CWpsAlgoCondition_RecvSpsEvent::CWpsAlgoCondition_RecvSpsEvent(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_RECV_SPS_EVENT, "SCRIPTCONTROL_CONDITION_RECV_SPS_EVENT")
{
}


CWpsAlgoCondition_RecvSpsEvent::~CWpsAlgoCondition_RecvSpsEvent()
{
}


bool CWpsAlgoCondition_RecvSpsEvent::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_RecvSpsEvent* pCond = dynamic_cast<CWPSNodeCondition_RecvSpsEvent*>(pControlScriptNode);
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

int CWpsAlgoCondition_RecvSpsEvent::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetOwner()->GetEventMap()->IsReceived(m_eventID))
		m_status = COMPLETED;

	return m_status;
}

int CWpsAlgoCondition_RecvSpsEvent::OnObjectMsg(CObjectMsg* pObjMsg)
{
	if (pObjMsg->GetID() == OBJMSG_SEND_SPS_EVENT)
	{
		CObjMsg_SendSpsEvent* pEvt = static_cast<CObjMsg_SendSpsEvent*>(pObjMsg);
		if (pEvt)
		{
			if (pEvt->targetTblidx == m_targetTblidx && pEvt->eventId == m_eventID)
			{
				printf("2 received sps event. ID %u\n", m_eventID);
				m_status = COMPLETED;
			}
		}
	}

	return m_status;
}