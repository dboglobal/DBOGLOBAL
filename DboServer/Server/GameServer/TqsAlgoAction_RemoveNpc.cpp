#include "stdafx.h"
#include "TqsAlgoAction_RemoveNpc.h"
#include "TQSNodeAction_RemoveNpc.h"
#include "ObjectManager.h"
#include "Npc.h"


CTqsAlgoAction_RemoveNpc::CTqsAlgoAction_RemoveNpc(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_REMOVE_NPC, "TQS_ALGOCONTROLID_ACTION_REMOVE_NPC")
{
}


CTqsAlgoAction_RemoveNpc::~CTqsAlgoAction_RemoveNpc()
{
}


bool CTqsAlgoAction_RemoveNpc::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_RemoveNpc* pAction = dynamic_cast<CTQSNodeAction_RemoveNpc*>(pControlScriptNode);
	if (pAction)
	{
		m_npcTblidx = pAction->m_npcTblidx;
		m_eSpawnRemoveType = pAction->m_eSpawnRemoveType;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CTQSNodeAction_RemoveNpc", pControlScriptNode);
	return false;
}

int CTqsAlgoAction_RemoveNpc::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CNpc* pNpc = GetOwner()->FindNpc(m_npcTblidx);

	if (pNpc == NULL && GetOwner()->GetWorld())
	{
		pNpc = GetOwner()->GetWorld()->FindNpc(m_npcTblidx);
	}

	GetOwner()->RemoveNpc(m_npcTblidx); //remove npc from script

	if (pNpc == NULL || !pNpc->IsInitialized()) //did we already remove the npc? 
	{
		m_status = COMPLETED;
		return m_status;
	}

	pNpc->SetSpawnFuncFlag(0);
	pNpc->RemAllScript();	//remove from all scripts

	switch (m_eSpawnRemoveType)
	{
		case SPAWN_REMOVE_TYPE_CLEAR:
		{
			pNpc->LeaveGame();
		}
		break;
		case SPAWN_REMOVE_TYPE_DESPAWN:
		{
			if (!pNpc->IsDespawning() && !pNpc->IsFainting())
				pNpc->SendCharStateDespawning(TELEPORT_TYPE_GAME_OUT);
		}
		break;
		case SPAWN_REMOVE_TYPE_FAINT:
		{
			pNpc->Faint(NULL, FAINT_REASON_REMOVE);
		}
		break;

		default: ERR_LOG(LOG_SCRIPT, "m_eSpawnRemoveType %u not valid", m_eSpawnRemoveType); break;
	}

	m_status = COMPLETED;
	return m_status;
}
