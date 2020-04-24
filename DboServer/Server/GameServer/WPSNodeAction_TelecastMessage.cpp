#include "stdafx.h"
#include "WPSNodeAction_TelecastMessage.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_TelecastMessage::CWPSNodeAction_TelecastMessage(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_TELECAST_MESSAGE, lpszName)
{
}

CWPSNodeAction_TelecastMessage::~CWPSNodeAction_TelecastMessage()
{

}

CWPSNodeAction_TelecastMessage::sPARAMETER_INFO* CWPSNodeAction_TelecastMessage::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_TelecastMessage::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeAction_TelecastMessage: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_TelecastMessage::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeAction_TelecastMessage: [%s] not exist \n", name);
	return false;
}