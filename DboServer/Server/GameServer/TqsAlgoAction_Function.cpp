#include "stdafx.h"
#include "TqsAlgoAction_Function.h"


CTqsAlgoAction_Function::CTqsAlgoAction_Function(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_FUNCTION, "TQS_ALGOCONTROLID_ACTION_FUNCTION")
{
}


CTqsAlgoAction_Function::~CTqsAlgoAction_Function()
{
}


int CTqsAlgoAction_Function::OnUpdate(DWORD dwTickDiff, float fMultiple)
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
