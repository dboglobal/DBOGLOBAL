#include "stdafx.h"
#include "ScriptAlgoCondition_WaitCondition.h"
#include "WPSNodeCondition_WaitCondition.h"


CWpsAlgoCondition_WaitCondition::CWpsAlgoCondition_WaitCondition(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_WAIT_CONDITION, "SCRIPTCONTROL_CONDITION_WAIT_CONDITION")
{
	m_nCondCount = 0;
	m_bSetCondCount = false;
}


CWpsAlgoCondition_WaitCondition::~CWpsAlgoCondition_WaitCondition()
{
}


bool CWpsAlgoCondition_WaitCondition::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_WaitCondition* pCond = dynamic_cast<CWPSNodeCondition_WaitCondition*>(pControlScriptNode);
	if (pCond)
	{
		m_bOperationAND = pCond->m_bOperationAND;

		return true;
	}

	return false;
}

void CWpsAlgoCondition_WaitCondition::OnEnter()
{
	if (!m_bSetCondCount)
	{
		m_nCondCount = m_subControlList.GetCount();
		m_bSetCondCount = true;
	}
}

int CWpsAlgoCondition_WaitCondition::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	UpdateSubControlList(dwTickDiff, fMultiple);

	if (m_bOperationAND)
	{
		if (m_subControlList.GetCount() == 0)
			m_status = COMPLETED;
	}
	else
	{
		if (m_subControlList.GetCount() != m_nCondCount)
			m_status = COMPLETED;
	}

	return m_status;
}
