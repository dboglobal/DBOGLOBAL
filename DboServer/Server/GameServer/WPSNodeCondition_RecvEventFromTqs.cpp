#include "stdafx.h"
#include "WPSNodeCondition_RecvEventFromTqs.h"
#include "ControlScriptNodeParam_Number.h"


CWPSNodeCondition_RecvEventFromTqs::CWPSNodeCondition_RecvEventFromTqs(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_RECV_EVENT_FROM_TQS, lpszName)
{
}

CWPSNodeCondition_RecvEventFromTqs::~CWPSNodeCondition_RecvEventFromTqs()
{

}


bool CWPSNodeCondition_RecvEventFromTqs::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();


	printf("CWPSNodeCondition_RecvEventFromTqs: [%s] not exist \n", name);
	return false;
}

