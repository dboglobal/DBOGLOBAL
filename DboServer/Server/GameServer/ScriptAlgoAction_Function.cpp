#include "stdafx.h"
#include "ScriptAlgoAction_Function.h"


CWpsAlgoAction_Function::CWpsAlgoAction_Function(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_FUNCTION, "SCRIPTCONTROL_ACTION_FUNCTION")
{
}


CWpsAlgoAction_Function::~CWpsAlgoAction_Function()
{
}


int CWpsAlgoAction_Function::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	int nCondStatus = UpdateSubControlList(dwTickDiff, fMultiple); //UPDATE CONDITION

	if (nCondStatus != FAILED && nCondStatus != CHANGED)
	{
		m_status = UpdateSubControlQueue(dwTickDiff, fMultiple); //update action
	}
	else
	{
		m_status = nCondStatus;
	}

	return m_status;
}
