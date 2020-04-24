#include "stdafx.h"
#include "WPSNodeCondition_CheckCountDown.h"
#include "ControlScriptNodeParam_Number.h"


CWPSNodeCondition_CheckCountDown::CWPSNodeCondition_CheckCountDown(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_CHECK_COUNTDOWN, lpszName)
{
}

CWPSNodeCondition_CheckCountDown::~CWPSNodeCondition_CheckCountDown()
{

}

bool CWPSNodeCondition_CheckCountDown::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();


	printf("CWPSNodeCondition_CheckCountDown: [%s] not exist \n", name);
	return false;
}
