#include "stdafx.h"
#include "ScriptAlgoAction_Wait.h"
#include "WPSNodeAction_Wait.h"



CWpsAlgoAction_Wait::CWpsAlgoAction_Wait(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_WAIT, "SCRIPTCONTROL_ACTION_WAIT")
{
	m_nCondCount = 100000;
}


CWpsAlgoAction_Wait::~CWpsAlgoAction_Wait()
{
}


bool CWpsAlgoAction_Wait::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_Wait* pNode = dynamic_cast<CWPSNodeAction_Wait*>(pControlScriptNode);
	if (pNode)
	{
		m_bOperationAND = pNode->m_bOperationAND;
		return true;
	}
	return false;
}

void CWpsAlgoAction_Wait::OnEnter()
{
	if(m_nCondCount == 100000)
		m_nCondCount = m_subControlList.GetCount();
}

int CWpsAlgoAction_Wait::OnUpdate(DWORD dwTickDiff, float fMultiple)
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
