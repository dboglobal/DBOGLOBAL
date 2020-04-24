#include "stdafx.h"
#include "WpsNodeCondition_CCBD_check_run_state.h"
#include "ControlScriptNodeParam_String.h"


CWpsNodeCondition_CCBD_check_run_state::CWpsNodeCondition_CCBD_check_run_state(const char* lpszName)
	:CControlScriptNodeCondition(WPS_NODE_CONDITION_CCBD_CHECK_RUN_STATE, lpszName)
{
	
}

CWpsNodeCondition_CCBD_check_run_state::~CWpsNodeCondition_CCBD_check_run_state()
{

}


bool CWpsNodeCondition_CCBD_check_run_state::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();


	printf("CWpsNodeCondition_CCBD_check_run_state: [%s] not exist \n", name);
	return false;
}