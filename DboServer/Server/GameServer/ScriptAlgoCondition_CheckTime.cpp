#include "stdafx.h"
#include "ScriptAlgoCondition_CheckTime.h"
#include "WPSNodeCondition_CheckTime.h"



CWpsAlgoCondition_CheckTime::CWpsAlgoCondition_CheckTime(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_CHECKTIME, "SCRIPTCONTROL_CONDITION_CHECKTIME")
{
	m_dwWaitTime = INVALID_DWORD;
}


CWpsAlgoCondition_CheckTime::~CWpsAlgoCondition_CheckTime()
{
}


bool CWpsAlgoCondition_CheckTime::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_CheckTime* pNode = dynamic_cast<CWPSNodeCondition_CheckTime*>(pControlScriptNode);
	if (pNode)
	{
		m_dwWaitTime = pNode->m_dwWaitTime;
		return true;
	}

	return false;
}


int CWpsAlgoCondition_CheckTime::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwWaitTime = UnsignedSafeDecrease<DWORD>(m_dwWaitTime, dwTickDiff);

	if (m_dwWaitTime == 0)
		m_status = COMPLETED;

	 return m_status;
}

