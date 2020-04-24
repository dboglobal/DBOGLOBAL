#include "stdafx.h"
#include "WPSNodeAction_SendEventToTqs.h"
#include "ControlScriptNodeParam_Number.h"



CWPSNodeAction_SendEventToTqs::CWPSNodeAction_SendEventToTqs(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_SEND_EVENT_TO_TQS, lpszName)
{
}

CWPSNodeAction_SendEventToTqs::~CWPSNodeAction_SendEventToTqs()
{

}

CWPSNodeAction_SendEventToTqs::sPARAMETER_INFO* CWPSNodeAction_SendEventToTqs::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_SendEventToTqs::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();


	printf("CWPSNodeAction_SendEventToTqs: [%s] not exist \n", name);
	return false;
}
