#include "stdafx.h"
#include "ScriptAlgoAction_Default.h"


CWpsAlgoAction_Default::CWpsAlgoAction_Default(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_DEFAULT, "SCRIPTCONTROL_ACTION_DEFAULT")
{
}


CWpsAlgoAction_Default::~CWpsAlgoAction_Default()
{
}

int CWpsAlgoAction_Default::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
