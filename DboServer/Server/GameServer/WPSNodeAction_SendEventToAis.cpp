#include "stdafx.h"
#include "WPSNodeAction_SendEventToAis.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_SendEventToAis::CWPSNodeAction_SendEventToAis(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_SEND_EVENT_TO_AIS, lpszName)
{
}

CWPSNodeAction_SendEventToAis::~CWPSNodeAction_SendEventToAis()
{

}

CWPSNodeAction_SendEventToAis::sPARAMETER_INFO* CWPSNodeAction_SendEventToAis::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_SendEventToAis::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeAction_SendEventToAis: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_SendEventToAis::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeAction_SendEventToAis: [%s] not exist \n", name);
	return false;
}