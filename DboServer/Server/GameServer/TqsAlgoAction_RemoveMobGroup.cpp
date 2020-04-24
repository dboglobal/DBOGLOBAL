#include "stdafx.h"
#include "TqsAlgoAction_RemoveMobGroup.h"
#include "TQSNodeAction_RemoveMobGroup.h"
#include "Monster.h"


CTqsAlgoAction_RemoveMobGroup::CTqsAlgoAction_RemoveMobGroup(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_REMOVE_MOB_GROUP, "TQS_ALGOCONTROLID_ACTION_REMOVE_MOB_GROUP")
{

}


CTqsAlgoAction_RemoveMobGroup::~CTqsAlgoAction_RemoveMobGroup()
{
}


bool CTqsAlgoAction_RemoveMobGroup::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_RemoveMobGroup* pAction = dynamic_cast<CTQSNodeAction_RemoveMobGroup*>(pControlScriptNode);
	if (pAction)
	{
		m_spawnGroupId = pAction->m_spawnGroupId;
		m_eSpawnRemoveType = pAction->m_eSpawnRemoveType;


		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CTQSNodeAction_RemoveMobGroup", pControlScriptNode);
	return false;
}

int CTqsAlgoAction_RemoveMobGroup::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	DWORD dwCount = GetOwner()->GetSpawnGroupCount(m_spawnGroupId);
	HOBJECT hMob = GetOwner()->GetSpawnGroupFirst(m_spawnGroupId);

	for (DWORD i = 0; i < dwCount; i++)
	{
		if (hMob == INVALID_HOBJECT)
			continue;

		CMonster* pMob = g_pObjectManager->GetMob(hMob);
		if (pMob)
		{
			GetOwner()->RemoveMob(pMob);

			if (pMob->GetSpawnFuncFlag() == 0 && (pMob->IsInitialized() == false || pMob->IsDespawning() || pMob->IsFainting()))
				continue;

			pMob->SetSpawnFuncFlag(0);
			pMob->RemAllScript();	//clear scripts map.

			switch (m_eSpawnRemoveType)
			{
				case SPAWN_REMOVE_TYPE_CLEAR:
				{
					pMob->LeaveGame();
				}
				break;
				case SPAWN_REMOVE_TYPE_DESPAWN:
				{
					pMob->SendCharStateDespawning(TELEPORT_TYPE_GAME_OUT);
				}
				break;
				case SPAWN_REMOVE_TYPE_FAINT:
				{
					pMob->Faint(NULL, FAINT_REASON_REMOVE);
				}
				break;

				default: ERR_LOG(LOG_SCRIPT, "m_eSpawnRemoveType %u not valid", m_eSpawnRemoveType); break;
			}

			hMob = GetOwner()->GetSpawnGroupFirst(m_spawnGroupId); //get first because mob gets removed from group at "remallscript"
		}
		else
		{
			ERR_LOG(LOG_SCRIPT, "IMPORTANT-ERROR: MOB IS NULL. Group Count %u, Handle %u, SpawnGroup %u !", dwCount, hMob, m_spawnGroupId);
			hMob = GetOwner()->GetSpawnGroupNext(m_spawnGroupId); //get next because mob gets not removed from group
		}

	}

	m_status = COMPLETED;
	return m_status;
}
