#include "stdafx.h"
#include "WPSNodeAction_EventHandler.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_EventHandler::CWPSNodeAction_EventHandler(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_EVENT_HANDLER, lpszName)
{
}

CWPSNodeAction_EventHandler::~CWPSNodeAction_EventHandler()
{

}

CWPSNodeAction_EventHandler::sPARAMETER_INFO* CWPSNodeAction_EventHandler::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_EventHandler::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();


	printf("CWPSNodeAction_EventHandler: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_EventHandler::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();


	printf("CWPSNodeAction_EventHandler: [%s] not exist \n", name);
	return false;
}
