#include "stdafx.h"
#include "ScriptAlgoAction_SendEventToSps.h"
#include "WPSNodeAction_SendEventToSps.h"
#include "Monster.h"


CWpsAlgoAction_SendEventToSps::CWpsAlgoAction_SendEventToSps(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_SEND_EVENT_TO_SPS, "SCRIPTCONTROL_ACTION_SEND_EVENT_TO_SPS")
{
}


CWpsAlgoAction_SendEventToSps::~CWpsAlgoAction_SendEventToSps()
{
}


bool CWpsAlgoAction_SendEventToSps::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_SendEventToSps* pAction = dynamic_cast<CWPSNodeAction_SendEventToSps*>(pControlScriptNode);
	if (pAction)
	{
		m_eTargetType = pAction->m_eTargetType;
		m_targetTblidx = pAction->m_targetTblidx;
		m_eventId = pAction->m_eventId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_SendEventToSps", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_SendEventToSps::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_eTargetType == OBJTYPE_NPC)
	{
	//	ERR_LOG(LOG_SCRIPT, "NPC: WPSIDX = %u, m_targetTblidx = %u, m_eventId = %u", GetOwner()->GetScriptID(), m_targetTblidx, m_eventId);
		CNpc* pNpc = GetOwner()->FindNpc(m_targetTblidx);

		if (pNpc == NULL && GetOwner()->GetWorld())
		{
			pNpc = GetOwner()->GetWorld()->FindNpc(m_targetTblidx);
		}

		if (pNpc == NULL || !pNpc->IsInitialized()) //did we already remove the npc? 
		{
			m_status = COMPLETED;
			return m_status;
		}

		CObjMsg_SendWpsEventToSps pMsg;
		pMsg.eventID = m_eventId;
		pNpc->OnObjectMsg(&pMsg);
	}
	else if (m_eTargetType == OBJTYPE_MOB)
	{
	//	ERR_LOG(LOG_SCRIPT, "MOB: WPSIDX = %u, m_targetTblidx = %u, m_eventId = %u", GetOwner()->GetScriptID(), m_targetTblidx, m_eventId);

		DWORD dwCount = GetOwner()->GetSpawnGroupCount(m_targetTblidx);
		HOBJECT hMonster = GetOwner()->GetSpawnGroupFirst(m_targetTblidx);

		for (DWORD i = 0; i < dwCount; i++)
		{
			CMonster* pMob = g_pObjectManager->GetMob(hMonster);
			if (pMob && pMob->IsInitialized() && !pMob->IsFainting())
			{
				CObjMsg_SendWpsEventToSps pMsg;
				pMsg.eventID = m_eventId;
				pMob->OnObjectMsg(&pMsg);

				break;
			}

			hMonster = GetOwner()->GetSpawnGroupNext(m_targetTblidx);
		}
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "ERROR: Cant send event to sps. Invalid targettype %u, m_targetTblidx %u, m_eventId %u", m_eTargetType, m_targetTblidx, m_eventId);
	}

//	printf("CWpsAlgoAction_SendEventToSps: send event %u \n", m_eventId);
	m_status = COMPLETED;
	return m_status;
}
