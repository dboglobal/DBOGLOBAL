#include "stdafx.h"
#include "ScriptAlgoCondition_RecvEventFromSps.h"
#include "WPSNodeCondition_RecvEventFromSps.h"
#include "ObjectMsg.h"


CWpsAlgoCondition_RecvEventFromSps::CWpsAlgoCondition_RecvEventFromSps(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_RECV_EVENT_FROM_SPS, "SCRIPTCONTROL_CONDITION_RECV_EVENT_FROM_SPS")
{
}


CWpsAlgoCondition_RecvEventFromSps::~CWpsAlgoCondition_RecvEventFromSps()
{
}


bool CWpsAlgoCondition_RecvEventFromSps::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_RecvEventFromSps* pCond = dynamic_cast<CWPSNodeCondition_RecvEventFromSps*>(pControlScriptNode);
	if (pCond)
	{
		m_eObjType = pCond->m_eObjType;
		m_objectTblidx = pCond->m_objectTblidx;
		m_eventId = pCond->m_eventId;

		if (m_eventId != -1)
		{
			GetOwner()->GetEventMap()->SetScriptEvent(m_eventId);
		}

		return true;
	}

	return false;
}

int CWpsAlgoCondition_RecvEventFromSps::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetOwner()->GetEventMap()->IsReceived(m_eventId))
	{
		//printf("CWpsAlgoCondition_RecvEventFromSps: received event %u \n", m_eventId);
		m_status = COMPLETED;
	}

	return m_status;
}


int CWpsAlgoCondition_RecvEventFromSps::OnObjectMsg(CObjectMsg* pObjMsg)
{
	if (pObjMsg->GetID() == OBJMSG_SEND_SPS_EVENT_TO_WPS)
	{
		CObjMsg_SendSpsEventToWps* pEvt = static_cast<CObjMsg_SendSpsEventToWps*>(pObjMsg);
		if (pEvt)
		{
			if (pEvt->eObjType == m_eObjType && pEvt->objectTblidx == m_objectTblidx && pEvt->eventId == m_eventId)
				m_status = COMPLETED;
		}
	}

	return m_status;
}