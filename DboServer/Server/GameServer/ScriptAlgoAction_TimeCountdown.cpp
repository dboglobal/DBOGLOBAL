#include "stdafx.h"
#include "ScriptAlgoAction_TimeCountdown.h"
#include "WPSNodeAction_TimeCountdown.h"


CWpsAlgoAction_TimeCountdown::CWpsAlgoAction_TimeCountdown(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_TIME_COUNTDOWN, "SCRIPTCONTROL_ACTION_TIME_COUNTDOWN")
{
}


CWpsAlgoAction_TimeCountdown::~CWpsAlgoAction_TimeCountdown()
{
}


bool CWpsAlgoAction_TimeCountdown::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_TimeCountdown* pAction = dynamic_cast<CWPSNodeAction_TimeCountdown*>(pControlScriptNode);
	if (pAction)
	{
		m_bCountdown = pAction->m_bCountdown;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_TimeCountdown", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_TimeCountdown::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_status = COMPLETED;
	return m_status;
}
