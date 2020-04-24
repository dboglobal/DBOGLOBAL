#include "stdafx.h"
#include "WpsScriptAlgoCondition_CCBD_check_limit_time.h"


CWpsScriptAlgoCondition_CCBD_check_limit_time::CWpsScriptAlgoCondition_CCBD_check_limit_time(CWpsAlgoObject* pObject) :
	CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_CCBD_CHECK_LIMIT_TIME, "SCRIPTCONTROL_CONDITION_CCBD_CHECK_LIMIT_TIME")
{
}


CWpsScriptAlgoCondition_CCBD_check_limit_time::~CWpsScriptAlgoCondition_CCBD_check_limit_time()
{
}

int CWpsScriptAlgoCondition_CCBD_check_limit_time::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_status = COMPLETED;
	return m_status;
}
