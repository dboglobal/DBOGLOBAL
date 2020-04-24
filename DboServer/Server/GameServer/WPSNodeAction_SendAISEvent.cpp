#include "stdafx.h"
#include "WPSNodeAction_SendAISEvent.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlObject.h"


CWPSNodeAction_SendAISEvent::CWPSNodeAction_SendAISEvent(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_SEND_AIS_EVENT, lpszName)
{
}

CWPSNodeAction_SendAISEvent::~CWPSNodeAction_SendAISEvent()
{

}

CWPSNodeAction_SendAISEvent::sPARAMETER_INFO* CWPSNodeAction_SendAISEvent::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_SendAISEvent::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeAction_SendAISEvent: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_SendAISEvent::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeAction_SendAISEvent: [%s] not exist \n", name);
	return false;
}