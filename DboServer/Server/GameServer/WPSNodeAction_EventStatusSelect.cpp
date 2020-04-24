#include "stdafx.h"
#include "WPSNodeAction_EventStatusSelect.h"
#include "ControlScriptNodeParam_Number.h"


CWPSNodeAction_EventStatusSelect::CWPSNodeAction_EventStatusSelect(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_EVENTSTATUS_SELECT, lpszName)
{
}

CWPSNodeAction_EventStatusSelect::~CWPSNodeAction_EventStatusSelect()
{

}

CWPSNodeAction_EventStatusSelect::sPARAMETER_INFO* CWPSNodeAction_EventStatusSelect::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_EventStatusSelect::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();


	printf("CWPSNodeAction_EventStatusSelect: [%s] not exist \n", name);
	return false;
}
