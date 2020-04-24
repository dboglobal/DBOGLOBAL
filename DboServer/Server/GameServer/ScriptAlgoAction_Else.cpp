#include "stdafx.h"
#include "ScriptAlgoAction_Else.h"


CWpsAlgoAction_Else::CWpsAlgoAction_Else(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_ELSE, "SCRIPTCONTROL_ACTION_ELSE")
{
}


CWpsAlgoAction_Else::~CWpsAlgoAction_Else()
{
}

int CWpsAlgoAction_Else::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_status = UpdateSubControlQueue(dwTickDiff, fMultiple); //UPDATE ACTIONS

	return m_status;
}
