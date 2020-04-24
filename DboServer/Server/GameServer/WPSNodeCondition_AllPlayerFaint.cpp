#include "stdafx.h"
#include "WPSNodeCondition_AllPlayerFaint.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeCondition_AllPlayerFaint::CWPSNodeCondition_AllPlayerFaint(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_ALL_PLAYER_FAINT, lpszName)
{
	m_bWpsFail = false;
}

CWPSNodeCondition_AllPlayerFaint::~CWPSNodeCondition_AllPlayerFaint()
{

}


bool CWPSNodeCondition_AllPlayerFaint::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();


	printf("CWPSNodeCondition_AllPlayerFaint: [%s] not exist \n", name);
	return false;
}