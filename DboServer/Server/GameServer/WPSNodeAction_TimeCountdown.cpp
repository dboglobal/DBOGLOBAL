#include "stdafx.h"
#include "WPSNodeAction_TimeCountdown.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_TimeCountdown::CWPSNodeAction_TimeCountdown(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_TIME_COUNTDOWN, lpszName)
{
}

CWPSNodeAction_TimeCountdown::~CWPSNodeAction_TimeCountdown()
{

}

CWPSNodeAction_TimeCountdown::sPARAMETER_INFO* CWPSNodeAction_TimeCountdown::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_TimeCountdown::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeAction_TimeCountdown: [%s] not exist \n", name);
	return false;
}