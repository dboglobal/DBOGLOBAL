#include "stdafx.h"
#include "ScriptAlgoCondition_RecvEventFromTqs.h"
#include "WPSNodeCondition_RecvEventFromTqs.h"


CWpsAlgoCondition_RecvEventFromTqs::CWpsAlgoCondition_RecvEventFromTqs(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_RECV_EVENT_FROM_TQS, "SCRIPTCONTROL_CONDITION_RECV_EVENT_FROM_TQS")
{
}


CWpsAlgoCondition_RecvEventFromTqs::~CWpsAlgoCondition_RecvEventFromTqs()
{
}


bool CWpsAlgoCondition_RecvEventFromTqs::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_RecvEventFromTqs* pCond = dynamic_cast<CWPSNodeCondition_RecvEventFromTqs*>(pControlScriptNode);
	if (pCond)
	{
		m_eventId = pCond->m_eventId;

		if (m_eventId != -1)
		{
			GetOwner()->GetEventMap()->SetScriptEvent(m_eventId);
		}

		return true;
	}

	return false;
}

int CWpsAlgoCondition_RecvEventFromTqs::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetOwner()->GetEventMap()->IsReceived(m_eventId))
		m_status = COMPLETED;

	return m_status;
}
