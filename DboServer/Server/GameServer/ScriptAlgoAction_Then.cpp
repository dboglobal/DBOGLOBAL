#include "stdafx.h"
#include "ScriptAlgoAction_Then.h"


CWpsAlgoAction_Then::CWpsAlgoAction_Then(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_THEN, "SCRIPTCONTROL_ACTION_THEN")
{
}


CWpsAlgoAction_Then::~CWpsAlgoAction_Then()
{
}



int CWpsAlgoAction_Then::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_status = UpdateSubControlQueue(dwTickDiff, fMultiple); //UPDATE ACTIONS

	return m_status;
}
