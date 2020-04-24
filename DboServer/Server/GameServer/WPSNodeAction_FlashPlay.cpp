#include "stdafx.h"
#include "WPSNodeAction_FlashPlay.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_FlashPlay::CWPSNodeAction_FlashPlay(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_ADD_FLASH_PLAY, lpszName)
{
}

CWPSNodeAction_FlashPlay::~CWPSNodeAction_FlashPlay()
{

}

CWPSNodeAction_FlashPlay::sPARAMETER_INFO* CWPSNodeAction_FlashPlay::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_FlashPlay::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeAction_FlashPlay: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_FlashPlay::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeAction_FlashPlay: [%s] not exist \n", name);
	return false;
}