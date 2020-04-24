#include "stdafx.h"
#include "TqsAlgoCondition_CheckTime.h"
#include "TQSNodeCondition_CheckTime.h"



CTqsAlgoCondition_CheckTime::CTqsAlgoCondition_CheckTime(CTqsAlgoObject* pObject) :
	CTqsAlgoCondition_Base(pObject, TQS_ALGOCONTROLID_CONDITION_CHECKTIME, "TQS_ALGOCONTROLID_CONDITION_CHECKTIME")
{
	m_dwWaitTime = INVALID_DWORD;
}


CTqsAlgoCondition_CheckTime::~CTqsAlgoCondition_CheckTime()
{
}


bool CTqsAlgoCondition_CheckTime::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeCondition_CheckTime* pNode = dynamic_cast<CTQSNodeCondition_CheckTime*>(pControlScriptNode);
	if (pNode)
	{
		m_dwWaitTime = pNode->m_dwWaitTime;
		return true;
	}

	return false;
}


int CTqsAlgoCondition_CheckTime::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwWaitTime = UnsignedSafeDecrease<DWORD>(m_dwWaitTime, dwTickDiff);

	if (m_dwWaitTime == 0)
		m_status = COMPLETED;

	 return m_status;
}

