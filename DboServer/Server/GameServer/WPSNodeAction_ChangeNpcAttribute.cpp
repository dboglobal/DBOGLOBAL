#include "stdafx.h"
#include "WPSNodeAction_ChangeNpcAttribute.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_ChangeNpcAttribute::CWPSNodeAction_ChangeNpcAttribute(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_CHANGE_NPC_ATTRIBUTE, lpszName)
{
}

CWPSNodeAction_ChangeNpcAttribute::~CWPSNodeAction_ChangeNpcAttribute()
{

}

CWPSNodeAction_ChangeNpcAttribute::sPARAMETER_INFO* CWPSNodeAction_ChangeNpcAttribute::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_ChangeNpcAttribute::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();



	printf("CWPSNodeAction_ChangeNpcAttribute: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_ChangeNpcAttribute::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();



	printf("CWPSNodeAction_ChangeNpcAttribute: [%s] not exist \n", name);
	return false;
}