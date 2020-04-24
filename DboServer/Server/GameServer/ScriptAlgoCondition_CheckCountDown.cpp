#include "stdafx.h"
#include "ScriptAlgoCondition_CheckCountDown.h"
#include "WPSNodeCondition_CheckCountDown.h"


CWpsAlgoCondition_CheckCountDown::CWpsAlgoCondition_CheckCountDown(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_CHECK_COUNTDOWN, "SCRIPTCONTROL_CONDITION_CHECK_COUNTDOWN")
{
}


CWpsAlgoCondition_CheckCountDown::~CWpsAlgoCondition_CheckCountDown()
{
}


bool CWpsAlgoCondition_CheckCountDown::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_CheckCountDown* pCond = dynamic_cast<CWPSNodeCondition_CheckCountDown*>(pControlScriptNode);
	if (pCond)
	{
		m_dwWaitTime = pCond->m_dwWaitTime;

		return true;
	}

	return false;
}

int CWpsAlgoCondition_CheckCountDown::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
