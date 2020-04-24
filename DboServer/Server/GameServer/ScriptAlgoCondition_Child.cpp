#include "stdafx.h"
#include "ScriptAlgoCondition_Child.h"


CWpsAlgoCondition_Child::CWpsAlgoCondition_Child(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_CHILD, "SCRIPTCONTROL_CONDITION_CHILD")
{
}


CWpsAlgoCondition_Child::~CWpsAlgoCondition_Child()
{
}


int CWpsAlgoCondition_Child::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_status = UpdateSubControlQueue(dwTickDiff, fMultiple); //update actions

	return m_status;
}
