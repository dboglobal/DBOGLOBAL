#include "stdafx.h"
#include "ScriptAlgoAction_Loop.h"


CWpsAlgoAction_Loop::CWpsAlgoAction_Loop(CWpsAlgoObject* pObject) : 
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_LOOP, "SCRIPTCONTROL_ACTION_LOOP")
{
	
}


CWpsAlgoAction_Loop::~CWpsAlgoAction_Loop()
{
}


int CWpsAlgoAction_Loop::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	int status = UpdateSubControlList(dwTickDiff, fMultiple); //Update CONDITIONS. returns completed if EMPTY

	if (status == COMPLETED)
	{
		if (UpdateSubControlQueue(dwTickDiff, fMultiple) == COMPLETED) //Update ACTIONS. returns completed if EMPTY
		{
		//	ERR_LOG(LOG_SCRIPT, "'LOOP' IS COMPLETED");
			m_status = COMPLETED;
		}
	}

	return m_status;
}
