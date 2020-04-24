#include "stdafx.h"
#include "WpsScriptAlgoCondition_CCBD_check_run_state.h"


CWpsScriptAlgoCondition_CCBD_check_run_state::CWpsScriptAlgoCondition_CCBD_check_run_state(CWpsAlgoObject* pObject) :
	CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_CCBD_CHECK_RUN_STATE, "SCRIPTCONTROL_CONDITION_CCBD_CHECK_RUN_STATE")
{
}


CWpsScriptAlgoCondition_CCBD_check_run_state::~CWpsScriptAlgoCondition_CCBD_check_run_state()
{
}

int CWpsScriptAlgoCondition_CCBD_check_run_state::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
