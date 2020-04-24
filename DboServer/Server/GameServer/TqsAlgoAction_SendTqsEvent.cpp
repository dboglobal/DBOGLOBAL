#include "stdafx.h"
#include "TqsAlgoAction_SendTqsEvent.h"
#include "TQSNodeAction_SendTqsEvent.h"
#include "Monster.h"


CTqsAlgoAction_SendTqsEvent::CTqsAlgoAction_SendTqsEvent(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_PROGRESS_MESSAGE, "TQS_ALGOCONTROLID_ACTION_PROGRESS_MESSAGE")
{
}


CTqsAlgoAction_SendTqsEvent::~CTqsAlgoAction_SendTqsEvent()
{
}


bool CTqsAlgoAction_SendTqsEvent::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_SendTqsEvent* pNode = dynamic_cast<CTQSNodeAction_SendTqsEvent*>(pControlScriptNode);
	if (pNode)
	{
		m_eObjType = pNode->m_eObjType;
		m_targetTblidx = pNode->m_targetTblidx;
		m_eventID = pNode->m_eventID;

		return true;
	}
	return false;
}

int CTqsAlgoAction_SendTqsEvent::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	switch (m_eObjType)
	{
		case OBJTYPE_NPC:
		{
			CNpc* pNpc = GetOwner()->FindNpc(m_targetTblidx);
			if (pNpc && pNpc->IsInitialized())
			{
				CObjMsg_TqsEvent pMsg;
				pMsg.eventID = m_eventID;
				pNpc->OnObjectMsg(&pMsg);

				m_status = COMPLETED;
			}
			else
				ERR_LOG(LOG_SCRIPT, "cant find npc %u", m_targetTblidx);
		}
		break;

		default: ERR_LOG(LOG_SCRIPT, "m_eObjType '%u' not set", m_eObjType); break;
	}

	return m_status;
}
