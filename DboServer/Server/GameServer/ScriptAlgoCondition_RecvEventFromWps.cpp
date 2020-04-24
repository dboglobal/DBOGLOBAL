#include "stdafx.h"
#include "ScriptAlgoCondition_RecvEventFromWps.h"
#include "WPSNodeCondition_RecvEventFromWps.h"
#include "ObjectMsg.h"


CWpsAlgoCondition_RecvEventFromWps::CWpsAlgoCondition_RecvEventFromWps(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_RECV_EVENT_FROM_WPS, "SCRIPTCONTROL_CONDITION_RECV_EVENT_FROM_WPS")
{
}


CWpsAlgoCondition_RecvEventFromWps::~CWpsAlgoCondition_RecvEventFromWps()
{
}


bool CWpsAlgoCondition_RecvEventFromWps::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_RecvEventFromWps* pCond = dynamic_cast<CWPSNodeCondition_RecvEventFromWps*>(pControlScriptNode);
	if (pCond)
	{
		m_wpsTblidx = pCond->m_wpsTblidx;
		m_eventId = pCond->m_eventId;

		if (m_eventId != -1)
		{
			GetOwner()->GetEventMap()->SetScriptEvent(m_eventId);
		}

		return true;
	}

	return false;
}

int CWpsAlgoCondition_RecvEventFromWps::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetOwner()->GetEventMap()->IsReceived(m_eventId))
		m_status = COMPLETED;

	return m_status;
}


int CWpsAlgoCondition_RecvEventFromWps::OnObjectMsg(CObjectMsg* pObjMsg)
{
	if (pObjMsg->GetID() == OBJMSG_SEND_AI_EVENT_TO_WPS)
	{
		CObjMsg_SendWpsEventToWps* pEvt = static_cast<CObjMsg_SendWpsEventToWps*>(pObjMsg);
		if (pEvt)
		{
			if (pEvt->wpsTblidx == m_wpsTblidx && pEvt->eventId == m_eventId)
				m_status = COMPLETED;
		}
	}

	return m_status;
}