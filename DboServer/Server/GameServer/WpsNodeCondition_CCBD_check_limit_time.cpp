#include "stdafx.h"
#include "WpsNodeCondition_CCBD_check_limit_time.h"
#include "ControlScriptNodeParam_String.h"


CWpsNodeCondition_CCBD_check_limit_time::CWpsNodeCondition_CCBD_check_limit_time(const char* lpszName)
	:CControlScriptNodeCondition(WPS_NODE_CONDITION_CCBD_CHECK_LIMIT_TIME, lpszName)
{

}

CWpsNodeCondition_CCBD_check_limit_time::~CWpsNodeCondition_CCBD_check_limit_time()
{

}


bool CWpsNodeCondition_CCBD_check_limit_time::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();


	printf("CWpsNodeAction_CCBD_check_limit_time: [%s] not exist \n", name);
	return false;
}