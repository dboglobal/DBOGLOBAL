#include "stdafx.h"
#include "WPSNodeAction_EventStatusClear.h"
#include "ControlScriptNodeParam_Number.h"


CWPSNodeAction_EventStatusClear::CWPSNodeAction_EventStatusClear(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_EVENTSTATUS_CLEAR, lpszName)
{
}

CWPSNodeAction_EventStatusClear::~CWPSNodeAction_EventStatusClear()
{

}

CWPSNodeAction_EventStatusClear::sPARAMETER_INFO* CWPSNodeAction_EventStatusClear::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_EventStatusClear::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();


	printf("CWPSNodeAction_EventStatusClear: [%s] not exist \n", name);
	return false;
}
