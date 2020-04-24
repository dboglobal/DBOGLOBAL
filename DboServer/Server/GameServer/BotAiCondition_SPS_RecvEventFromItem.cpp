#include "stdafx.h"
#include "BotAiCondition_SPS_RecvEventFromItem.h"
#include "SPSNodeCondition_RecvEventFromItem.h"


CBotAiCondition_SPS_RecvEventFromItem::CBotAiCondition_SPS_RecvEventFromItem(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_SPS_RECV_EVENT_FROM_ITEM, "BOTCONTROL_CONDITION_SPS_RECV_EVENT_FROM_ITEM")
{
	m_itemTblidx = INVALID_TBLIDX;
}

CBotAiCondition_SPS_RecvEventFromItem::~CBotAiCondition_SPS_RecvEventFromItem()
{
}


bool CBotAiCondition_SPS_RecvEventFromItem::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeCondition_RecvEventFromItem* pCond = dynamic_cast<CSPSNodeCondition_RecvEventFromItem*>(pControlScriptNode);
	if (pCond)
	{
		m_itemTblidx = pCond->m_itemTblidx;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeCondition_RecvEventFromItem", pControlScriptNode);
	return false;
}

void CBotAiCondition_SPS_RecvEventFromItem::OnEnter()
{
}

void CBotAiCondition_SPS_RecvEventFromItem::OnExit()
{
}

int CBotAiCondition_SPS_RecvEventFromItem::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}

int CBotAiCondition_SPS_RecvEventFromItem::OnObjectMsg(CObjectMsg* pObjMsg)
{
	if (pObjMsg->GetID() != OBJMSG_SEND_ITEM_EVENT_TO_SPS)
		return m_status;

	CObjMsg_SendItemEventToSps* pMsg = static_cast<CObjMsg_SendItemEventToSps*>(pObjMsg);

	if (pMsg == NULL)
	{
		ERR_LOG(LOG_BOTAI, "fail : NULL == msg");
	}
	else
	{
		if (pMsg->itemTblidx == m_itemTblidx)
			m_status = COMPLETED;
	}

	return m_status;
}