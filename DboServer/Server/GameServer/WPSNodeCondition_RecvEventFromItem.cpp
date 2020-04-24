#include "stdafx.h"
#include "WPSNodeCondition_RecvEventFromItem.h"
#include "ControlScriptNodeParam_Number.h"


CWPSNodeCondition_RecvEventFromItem::CWPSNodeCondition_RecvEventFromItem(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_RECV_EVENT_FROM_ITEM, lpszName)
{
}

CWPSNodeCondition_RecvEventFromItem::~CWPSNodeCondition_RecvEventFromItem()
{

}


bool CWPSNodeCondition_RecvEventFromItem::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeCondition_RecvEventFromItem: [%s] not exist \n", name);
	return false;
}
