#include "stdafx.h"
#include "SPSNodeCondition_RecvEventFromItem.h"
#include "ControlScriptNodeParam_Number.h"



CSPSNodeCondition_RecvEventFromItem::CSPSNodeCondition_RecvEventFromItem(const char* lpszName)
:CControlScriptNodeCondition(SPS_NODE_CONDITION_RECV_EVENT_FROM_ITEM, lpszName)
{
	m_itemTblidx = INVALID_TBLIDX;
}

CSPSNodeCondition_RecvEventFromItem::~CSPSNodeCondition_RecvEventFromItem()
{

}

bool CSPSNodeCondition_RecvEventFromItem::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "item index") == 0)
	{
		m_itemTblidx = (TBLIDX)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeCondition_RecvEventFromItem: [%s] not exist \n", name);
	return false;
}
