#include "stdafx.h"
#include "TqsAlgoCondition_Child.h"


CTqsAlgoCondition_Child::CTqsAlgoCondition_Child(CTqsAlgoObject* pObject) :
	CTqsAlgoCondition_Base(pObject, TQS_ALGOCONTROLID_CONDITION_CHILD, "TQS_ALGOCONTROLID_CONDITION_CHILD")
{
}


CTqsAlgoCondition_Child::~CTqsAlgoCondition_Child()
{
}


int CTqsAlgoCondition_Child::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_status = UpdateSubControlQueue(dwTickDiff, fMultiple); //update actions

	return m_status;
}
