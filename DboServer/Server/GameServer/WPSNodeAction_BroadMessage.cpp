#include "stdafx.h"
#include "WPSNodeAction_BroadMessage.h"
#include "ControlScriptNodeParam_String.h"
#include "ControlScriptNodeParam_Number.h"


CWPSNodeAction_BroadMessage::CWPSNodeAction_BroadMessage(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_BROAD_MESSAGE, lpszName)
{
}

CWPSNodeAction_BroadMessage::~CWPSNodeAction_BroadMessage()
{

}

CWPSNodeAction_BroadMessage::sPARAMETER_INFO* CWPSNodeAction_BroadMessage::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_BroadMessage::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeAction_BroadMessage: [%s] not exist \n", name);
	return false;

	return CControlScriptNode::AddParam(pNode);
}

bool CWPSNodeAction_BroadMessage::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeAction_BroadMessage: [%s] not exist \n", name);
	return false;

	return CControlScriptNode::AddParam(pNode);
}