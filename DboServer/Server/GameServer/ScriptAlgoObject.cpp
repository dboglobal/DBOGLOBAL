#include "stdafx.h"
#include "ScriptAlgoObjectManager.h" // #include "ScriptAlgoObject.h"
#include "Monster.h"
#include "CPlayer.h"



CScriptAlgoObject::CScriptAlgoObject(TBLIDX tblidx, DWORD dwHandle, eSSD_SCRIPT_TYPE scripType)
{
	m_ScriptID = (DWORD)tblidx;

	m_scriptAlgoController.SetOwner(this);

	m_dwHandle = dwHandle;

	m_eScriptType = scripType;

	m_bStartedByQuest = false;

	Init();
}

CScriptAlgoObject::~CScriptAlgoObject()
{
	GetEventMap()->Clear();
}


void CScriptAlgoObject::Init()
{
	m_bPaused = true;
	m_bIsStopped = false;
	m_pWorld = NULL;
}


void CScriptAlgoObject::Start()
{
	SetPaused(false);
}

void CScriptAlgoObject::Stop(bool bEnterFailStage/* = false*/, bool bRemoveFromWorld/* = true*/)
{
	UNREFERENCED_PARAMETER(bEnterFailStage);
	ERR_LOG(LOG_SYSTEM, "Stop Script Tblidx %u, handle = %u \n", m_ScriptID, m_dwHandle);

	SetPaused(true);
	SetStopped(true);

	//remove all npc's
	for (std::map<TBLIDX, HOBJECT>::iterator it = m_map_Npc.begin(); it != m_map_Npc.end(); )
	{
		CNpc* pNpc = g_pObjectManager->GetNpc(it->second);
		if (pNpc && pNpc->IsInitialized())
		{
			if(pNpc->GetTbldat())
				ERR_LOG(LOG_SYSTEM, "Remove NPC(%i) %u(%u,%u), bStandAlone:%i from Script %u \n", pNpc->GetObjType(), it->first, pNpc->GetTblidx(), pNpc->GetTbldat()->tblidx, pNpc->GetStandAlone(), m_ScriptID);
			else
				ERR_LOG(LOG_SYSTEM, "Remove NPC(%i) %u(%u), bStandAlone:%i from Script %u \n", pNpc->GetObjType(), it->first, pNpc->GetTblidx(), pNpc->GetStandAlone(), m_ScriptID);

			if (!pNpc->GetStandAlone())	//check if not stand alone
			{
				pNpc->SetSpawnFuncFlag(0);
				pNpc->LeaveGame();	//remove from game && remove scripts. Here we also remove npc from m_map_Npc

				/*INFO: if npc does not have this script registered then infinite loop will be here because, we go back to .begin()*/
				it = m_map_Npc.begin();
				continue;
			}
			else
			{
				pNpc->RemScript(GetScriptID()); //remove script from npc
			}
		}

		it = m_map_Npc.erase(it);
	}
	m_map_Npc.clear();

	//remove all mobs
	for (SPAWNGROUP_IT it = m_spawnGroup.begin(); it != m_spawnGroup.end();)
	{
		CMonster* pMob = g_pObjectManager->GetMob(it->second);
		if (pMob && pMob->IsInitialized())
		{
			bool bHasScript = pMob->GetScriptCount() > 0;

			pMob->SetSpawnFuncFlag(0);
			pMob->LeaveGame(); //remove from game && remove scripts. Here we also remove mob from m_spawnGroup.

			if (bHasScript)
			{
				/*INFO: if mob does not have this script registered then infinite loop will be here because, we go back to .begin()*/
				it = m_spawnGroup.begin();
				continue;
			}
		}

		it = m_spawnGroup.erase(it);
	}
	m_spawnGroup.clear();

	//remove script from all players
	for (PLAYERSET_IT it = m_setPlayers.begin(); it != m_setPlayers.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(*it);
		if (pPlayer && pPlayer->IsInitialized())
		{
			pPlayer->RemScript(GetScriptID()); //remove script from player
		}
	}
	m_setPlayers.clear();


	m_scriptAlgoController.DestroyAllControlStage();


	if (GetWorld() && bRemoveFromWorld)
		GetWorld()->RemScript(GetScriptID()); //remove script from world


	g_pScriptAlgoManager->DestroyScript(this); //start destroy script <async>
}

int CScriptAlgoObject::OnUpdate(DWORD dwTickDiff)
{
	return m_scriptAlgoController.OnUpdate(dwTickDiff);
}

int CScriptAlgoObject::OnObjectMsg(CObjectMsg * pObjMsg)
{
	return m_scriptAlgoController.OnObjectMsg(pObjMsg);
}


CNpc* CScriptAlgoObject::FindNpc(TBLIDX tblidx)
{
	std::map<TBLIDX, HOBJECT>::iterator it = m_map_Npc.find(tblidx);
	if (it == m_map_Npc.end())
		return NULL;

	return g_pObjectManager->GetNpc(it->second);
}


void CScriptAlgoObject::AddNpc(CNpc* pNpc)
{
	m_map_Npc.insert({ pNpc->GetTblidx(), pNpc->GetID() });
}


void CScriptAlgoObject::RemoveNpc(TBLIDX npcTblidx)
{
	m_map_Npc.erase(npcTblidx);
}


void CScriptAlgoObject::AddSpawnGroupMob(SPAWNGROUPID spawnGroupId, HOBJECT hMonster)
{
	m_spawnGroup.insert(SPAWNGROUP::value_type(spawnGroupId, hMonster));
}

HOBJECT CScriptAlgoObject::GetSpawnGroupFirst(SPAWNGROUPID spawnGroupId)
{
	m_spawnGroupItBeg = m_spawnGroup.lower_bound(spawnGroupId);
	m_spawnGroupItEnd = m_spawnGroup.upper_bound(spawnGroupId);

	if (m_spawnGroupItBeg == m_spawnGroupItEnd)
	{
		return INVALID_HOBJECT;
	}

	return m_spawnGroupItBeg->second;
}

HOBJECT CScriptAlgoObject::GetSpawnGroupNext(SPAWNGROUPID spawnGroupId)
{
	if (m_spawnGroupItBeg == m_spawnGroupItEnd)
	{
		return INVALID_HOBJECT;
	}

	if (++m_spawnGroupItBeg == m_spawnGroupItEnd)
	{
		return INVALID_HOBJECT;
	}


	return m_spawnGroupItBeg->second;
}


DWORD CScriptAlgoObject::GetSpawnGroupCount(SPAWNGROUPID spawnGroupId)
{
	return (DWORD)m_spawnGroup.count(spawnGroupId);
}


DWORD CScriptAlgoObject::GetRemovedSpawnGroupCount(SPAWNGROUPID spawnGroupId)
{
	return (DWORD)std::count(m_vecRemovedSpawnGroup.begin(), m_vecRemovedSpawnGroup.end(), spawnGroupId);
}


void CScriptAlgoObject::RemoveMob(CMonster* pMob)
{
	SPAWNGROUP_IT beg = m_spawnGroup.lower_bound(pMob->GetSpawnGroupID());
	SPAWNGROUP_IT end = m_spawnGroup.upper_bound(pMob->GetSpawnGroupID());

	for (; beg != end; beg++)
	{
		if (beg->second == pMob->GetID())
		{
			m_spawnGroup.erase(beg);
			break;
		}
	}

	//reset iterators
	GetSpawnGroupFirst(pMob->GetSpawnGroupID());

	//add to removed spawn group
	m_vecRemovedSpawnGroup.push_back(pMob->GetSpawnGroupID());
}


void CScriptAlgoObject::AddPlayer(HOBJECT hPlayer)
{
	m_setPlayers.insert(hPlayer);
}


CPlayer* CScriptAlgoObject::GetPlayersFirst()
{
	m_setPlayersItBeg = m_setPlayers.begin();
	m_setPlayersItEnd = m_setPlayers.end();

	if (m_setPlayersItBeg == m_setPlayersItEnd)
	{
		return NULL;
	}

	return g_pObjectManager->GetPC(*m_setPlayersItBeg);
}

CPlayer* CScriptAlgoObject::GetPlayersNext()
{
	if (m_setPlayersItBeg == m_setPlayersItEnd)
	{
		return NULL;
	}

	if (++m_setPlayersItBeg == m_setPlayersItEnd)
	{
		return NULL;
	}


	return g_pObjectManager->GetPC(*m_setPlayersItBeg);
}


DWORD CScriptAlgoObject::GetPlayersCount()
{
	return (DWORD)m_setPlayers.size();
}

void CScriptAlgoObject::Broadcast(CNtlPacket * pPacket)
{
	for (PLAYERSET_IT it = m_setPlayers.begin(); it != m_setPlayers.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(*it);
		if (pPlayer && pPlayer->IsInitialized())
		{
			pPlayer->SendPacket(pPacket);
		}
	}
}


void CScriptAlgoObject::SendPartyLeader(CNtlPacket * pPacket)
{
	bool bSent = false;
	for (PLAYERSET_IT it = m_setPlayers.begin(); it != m_setPlayers.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(*it);
		if (pPlayer && pPlayer->IsInitialized())
		{
			if (pPlayer->GetParty() && pPlayer->GetParty()->GetPartyLeaderID() == pPlayer->GetID())
			{
				pPlayer->SendPacket(pPacket);
				bSent = true;
			}
		}
	}

	if (bSent == false)
	{
		ERR_LOG(LOG_SCRIPT, "ERROR: No party leader was found! Could not send packet");
	}
}

void CScriptAlgoObject::QuestFailed()
{
	if (m_bStartedByQuest == false)
		return;

	for (PLAYERSET_IT it = m_setPlayers.begin(); it != m_setPlayers.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(*it);
		if (pPlayer && pPlayer->IsInitialized())
		{
			pPlayer->GetQuests()->WpsFailed(GetScriptID());
		}
	}
}


bool CScriptAlgoObject::NotifyPlayerLeave(HOBJECT hPlayer, bool bRemoveScriptFromPlayer/* = true*/)
{
	//printf("CScriptAlgoObject::NotifyPlayerLeave(HOBJECT hPlayer) %u scriptid %u \n", hPlayer, GetScriptID());
	PLAYERSET_IT it = m_setPlayers.find(hPlayer);
	if (it == m_setPlayers.end())
	{
		return false;
	}

	if (bRemoveScriptFromPlayer)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(*it);
		if (pPlayer)
		{
			pPlayer->RemScript(GetScriptID());
		}
	}

	m_setPlayers.erase(hPlayer);
	
	return true;
}

