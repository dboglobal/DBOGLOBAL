#include "stdafx.h"
#include "TqsAlgoAction_RegisterCondition.h"


CTqsAlgoAction_RegisterCondition::CTqsAlgoAction_RegisterCondition(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_REGISTER_CONDITION, "TQS_ALGOCONTROLID_ACTION_REGISTER_CONDITION")
{

}


CTqsAlgoAction_RegisterCondition::~CTqsAlgoAction_RegisterCondition()
{
}


int CTqsAlgoAction_RegisterCondition::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	//add condition to stage
	for (CControlState* j = (CControlState *)m_subControlList.GetFirst();
		j;
		j = (CControlState *)m_subControlList.GetFirst())
	{
		m_subControlList.Remove(j);

		GetParent()->AddSubControlList(j, false);
	}

	m_status = COMPLETED;
	return m_status;
}
