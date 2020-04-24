#include "stdafx.h"
#include "ScriptAlgoAction_ClearEvent.h"
#include "WPSNodeAction_ClearEvent.h"


CWpsAlgoAction_ClearEvent::CWpsAlgoAction_ClearEvent(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CLEAR_EVENT, "SCRIPTCONTROL_ACTION_CLEAR_EVENT")
{
}


CWpsAlgoAction_ClearEvent::~CWpsAlgoAction_ClearEvent()
{
}


bool CWpsAlgoAction_ClearEvent::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_ClearEvent* pAction = dynamic_cast<CWPSNodeAction_ClearEvent*>(pControlScriptNode);
	if (pAction)
	{
		m_bClearAll = pAction->m_bClearAll;
		m_eventId = pAction->m_eventId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_ClearEvent", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_ClearEvent::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_bClearAll)
	{
		GetOwner()->GetEventMap()->Clear();
	}
	else
	{
		GetOwner()->GetEventMap()->ClearScriptEvent(m_eventId);
	}
	
	m_status = COMPLETED;
	return m_status;
}
