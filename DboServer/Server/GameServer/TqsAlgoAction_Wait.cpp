#include "stdafx.h"
#include "TqsAlgoAction_Wait.h"
#include "TQSNodeAction_Wait.h"



CTqsAlgoAction_Wait::CTqsAlgoAction_Wait(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_WAIT, "TQS_ALGOCONTROLID_ACTION_WAIT")
{
	m_nCondCount = 100000;
}


CTqsAlgoAction_Wait::~CTqsAlgoAction_Wait()
{
}


bool CTqsAlgoAction_Wait::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_Wait* pNode = dynamic_cast<CTQSNodeAction_Wait*>(pControlScriptNode);
	if (pNode)
	{
		m_bOperationAND = pNode->m_bOperationAND;
		return true;
	}
	return false;
}

void CTqsAlgoAction_Wait::OnEnter()
{
	if(m_nCondCount == 100000)
		m_nCondCount = m_subControlList.GetCount();
}

int CTqsAlgoAction_Wait::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	///info: m_bOperationAND | (and)true = COMPLETE when all conditions done | (or)false = when at least one finished

	int nStatus = UpdateSubControlList(dwTickDiff, fMultiple); //UPDATE CONDITION

	if (m_bOperationAND)
	{
		if (m_subControlList.GetCount() == 0)
		{
			m_status = COMPLETED;
		}
	}
	else if (m_nCondCount != m_subControlList.GetCount())
	{
		m_status = COMPLETED;
	}

	return m_status;
}
