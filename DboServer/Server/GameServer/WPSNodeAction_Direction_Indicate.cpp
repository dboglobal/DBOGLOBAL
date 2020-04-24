#include "stdafx.h"
#include "WPSNodeAction_Direction_Indicate.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_Direction_Indicate::CWPSNodeAction_Direction_Indicate(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_DIRECTION_INDICATE, lpszName)
{
}

CWPSNodeAction_Direction_Indicate::~CWPSNodeAction_Direction_Indicate()
{

}

CWPSNodeAction_Direction_Indicate::sPARAMETER_INFO* CWPSNodeAction_Direction_Indicate::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_Direction_Indicate::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();


	printf("CWPSNodeAction_Direction_Indicate: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_Direction_Indicate::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();


	printf("CWPSNodeAction_Direction_Indicate: [%s] not exist \n", name);
	return false;
}