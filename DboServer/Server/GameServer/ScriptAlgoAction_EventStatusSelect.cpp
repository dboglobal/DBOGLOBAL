#include "stdafx.h"
#include "ScriptAlgoAction_EventStatusSelect.h"
#include "WPSNodeAction_EventStatusSelect.h"


CWpsAlgoAction_EventStatusSelect::CWpsAlgoAction_EventStatusSelect(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_EVENT_STATUS_SELECT, "SCRIPTCONTROL_ACTION_EVENT_STATUS_SELECT")
{
}


CWpsAlgoAction_EventStatusSelect::~CWpsAlgoAction_EventStatusSelect()
{
}


bool CWpsAlgoAction_EventStatusSelect::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_EventStatusSelect* pAction = dynamic_cast<CWPSNodeAction_EventStatusSelect*>(pControlScriptNode);
	if (pAction)
	{
		m_byGroup = pAction->m_byGroup;
		m_byIndex = pAction->m_byIndex;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_EventStatusSelect", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_EventStatusSelect::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
