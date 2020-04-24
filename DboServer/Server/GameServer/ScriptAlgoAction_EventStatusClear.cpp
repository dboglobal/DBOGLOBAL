#include "stdafx.h"
#include "ScriptAlgoAction_EventStatusClear.h"
#include "WPSNodeAction_EventStatusClear.h"


CWpsAlgoAction_EventStatusClear::CWpsAlgoAction_EventStatusClear(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_EVENT_STATUS_CLEAR, "SCRIPTCONTROL_ACTION_EVENT_STATUS_CLEAR")
{
}


CWpsAlgoAction_EventStatusClear::~CWpsAlgoAction_EventStatusClear()
{
}


bool CWpsAlgoAction_EventStatusClear::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_EventStatusClear* pAction = dynamic_cast<CWPSNodeAction_EventStatusClear*>(pControlScriptNode);
	if (pAction)
	{
		m_byGroup = pAction->m_byGroup;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_EventStatusClear", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_EventStatusClear::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
