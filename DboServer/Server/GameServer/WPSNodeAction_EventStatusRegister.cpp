#include "stdafx.h"
#include "WPSNodeAction_EventStatusRegister.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_EventStatusRegister::CWPSNodeAction_EventStatusRegister(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_EVENTSTATUS_REGISTER, lpszName)
{
}

CWPSNodeAction_EventStatusRegister::~CWPSNodeAction_EventStatusRegister()
{

}

CWPSNodeAction_EventStatusRegister::sPARAMETER_INFO* CWPSNodeAction_EventStatusRegister::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_EventStatusRegister::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();


	printf("CWPSNodeAction_EventStatusRegister: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_EventStatusRegister::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();


	printf("CWPSNodeAction_EventStatusRegister: [%s] not exist \n", name);
	return false;
}