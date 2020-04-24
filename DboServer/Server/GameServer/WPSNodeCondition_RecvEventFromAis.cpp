#include "stdafx.h"
#include "WPSNodeCondition_RecvEventFromAis.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlObject.h"


CWPSNodeCondition_RecvEventFromAis::CWPSNodeCondition_RecvEventFromAis(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_RECV_EVENT_FROM_AIS, lpszName)
{
}

CWPSNodeCondition_RecvEventFromAis::~CWPSNodeCondition_RecvEventFromAis()
{

}

bool CWPSNodeCondition_RecvEventFromAis::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeCondition_RecvEventFromAis: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeCondition_RecvEventFromAis::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeCondition_RecvEventFromAis: [%s] not exist \n", name);
	return false;
}