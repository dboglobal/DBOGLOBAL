#include "stdafx.h"
#include "ScriptAlgoAction_SendSPSEvent.h"
#include "WPSNodeAction_SendSPSEvent.h"
#include "Monster.h"


CWpsAlgoAction_SendSPSEvent::CWpsAlgoAction_SendSPSEvent(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_SEND_SPS_EVENT, "SCRIPTCONTROL_ACTION_SEND_SPS_EVENT")
{
}


CWpsAlgoAction_SendSPSEvent::~CWpsAlgoAction_SendSPSEvent()
{
}


bool CWpsAlgoAction_SendSPSEvent::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_SendSPSEvent* pAction = dynamic_cast<CWPSNodeAction_SendSPSEvent*>(pControlScriptNode);
	if (pAction)
	{
		m_eObjType = pAction->m_eObjType;
		m_targetTblidx = pAction->m_targetTblidx;
		m_eventID = pAction->m_eventID;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_SendSPSEvent", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_SendSPSEvent::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	switch (m_eObjType)
	{
		case OBJTYPE_NPC:
		{
			CNpc* pNpc = GetOwner()->FindNpc(m_targetTblidx);
			if (pNpc && pNpc->IsInitialized())
			{
				CObjMsg_SendWpsEventToSps pMsg;
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
