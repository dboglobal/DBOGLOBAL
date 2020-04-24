#include "stdafx.h"
#include "ScriptAlgoCondition_RecvEventFromAis.h"
#include "WPSNodeCondition_RecvEventFromAis.h"
#include "ObjectMsg.h"


CWpsAlgoCondition_RecvEventFromAis::CWpsAlgoCondition_RecvEventFromAis(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_RECV_EVENT_FROM_AIS, "SCRIPTCONTROL_CONDITION_RECV_EVENT_FROM_AIS")
{
}


CWpsAlgoCondition_RecvEventFromAis::~CWpsAlgoCondition_RecvEventFromAis()
{
}


bool CWpsAlgoCondition_RecvEventFromAis::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_RecvEventFromAis* pCond = dynamic_cast<CWPSNodeCondition_RecvEventFromAis*>(pControlScriptNode);
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

int CWpsAlgoCondition_RecvEventFromAis::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetOwner()->GetEventMap()->IsReceived(m_eventId))
		m_status = COMPLETED;

	return m_status;
}

int CWpsAlgoCondition_RecvEventFromAis::OnObjectMsg(CObjectMsg* pObjMsg)
{
	if (pObjMsg->GetID() == OBJMSG_SEND_AI_EVENT_TO_WPS)
	{
		CObjMsg_SendAisEventToWps* pEvt = static_cast<CObjMsg_SendAisEventToWps*>(pObjMsg);
		if (pEvt)
		{
			if (pEvt->eObjType == m_eObjType && pEvt->objectTblidx == m_objectTblidx && pEvt->eventId == m_eventId)
				m_status = COMPLETED;
		}
	}

	return m_status;
}