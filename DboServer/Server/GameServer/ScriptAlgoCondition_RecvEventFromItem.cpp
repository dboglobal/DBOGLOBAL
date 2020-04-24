#include "stdafx.h"
#include "ScriptAlgoCondition_RecvEventFromItem.h"
#include "WPSNodeCondition_RecvEventFromItem.h"


CWpsAlgoCondition_RecvEventFromItem::CWpsAlgoCondition_RecvEventFromItem(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_RECV_EVENT_FROM_ITEM, "SCRIPTCONTROL_CONDITION_RECV_EVENT_FROM_ITEM")
{
}


CWpsAlgoCondition_RecvEventFromItem::~CWpsAlgoCondition_RecvEventFromItem()
{
}


bool CWpsAlgoCondition_RecvEventFromItem::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_RecvEventFromItem* pCond = dynamic_cast<CWPSNodeCondition_RecvEventFromItem*>(pControlScriptNode);
	if (pCond)
	{
		m_itemTblidx = pCond->m_itemTblidx;

		if (m_itemTblidx != -1)
		{
			GetOwner()->GetEventMap()->SetScriptEvent(m_itemTblidx);
		}

		return true;
	}

	return false;
}

int CWpsAlgoCondition_RecvEventFromItem::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetOwner()->GetEventMap()->IsReceived(m_itemTblidx))
		m_status = COMPLETED;

	return m_status;
}
