#include "stdafx.h"
#include "ObjectManager.h"
#include "CPlayer.h"
#include "Monster.h"
#include "MobServerTable.h"
#include "NpcServerTable.h"
#include "SummonPet.h"
#include "NtlPacketGU.h"
#include "TableContainerManager.h"
#include "GameServer.h"
#include "GameMain.h"


const DWORD TIME_DELETE_DELAY = 1000;


CObjectManager::CObjectManager()
{
	uiSerialId = MAX_GAME_OBJECT + 1;
	memset(m_objectArray, NULL, sizeof(m_objectArray));
	m_dwLpEpEventId = 100;
}

CObjectManager::~CObjectManager()
{
}

void CObjectManager::SpawnNpcAndMob()
{
	CGameServer* app = (CGameServer*)g_pApp;
	CWorldTable* pWorld = g_pTableContainer->GetWorldTable();

	//dont spawn anything on dojo channel
	if (app->IsDojoChannel())
		return;
	
	for (CTable::TABLEIT itpWorld = pWorld->Begin(); itpWorld != pWorld->End(); itpWorld++)
	{
		BYTE bySpawnFlag = 0;
		BIT_FLAG_SET(bySpawnFlag, SPAWN_FUNC_FLAG_RESPAWN);
		BIT_FLAG_SET(bySpawnFlag, SPAWN_FUNC_FLAG_NO_SPAWN_WAIT);

		sWORLD_TBLDAT* pWorldTblData = (sWORLD_TBLDAT*)itpWorld->second;
		CSpawnTable* pNPCSpawnTbl = g_pTableContainer->GetNpcSpawnTable(pWorldTblData->tblidx);
		CSpawnTable* pMOBSpawnTbl = g_pTableContainer->GetMobSpawnTable(pWorldTblData->tblidx);

		if (pWorldTblData->bDynamic == true) //only summon in normal worlds
			continue;

		if (app->GetGameMain()->GetWorldManager()->FindWorld(pWorldTblData->tblidx) == NULL)
			return;

		//spawn npc
		if (pNPCSpawnTbl != NULL)
		{
			for (CTable::TABLEIT itNPCSpawn = pNPCSpawnTbl->Begin(); itNPCSpawn != pNPCSpawnTbl->End(); itNPCSpawn++)
			{
				sSPAWN_TBLDAT* pNPCSpwnTblData = (sSPAWN_TBLDAT*)itNPCSpawn->second;

				if (pNPCSpwnTblData->spawnGroupId != INVALID_SPAWNGROUPID)
					continue;

				sNPC_TBLDAT* pNPCTblData = (sNPC_TBLDAT*)g_pTableContainer->GetNpcTable()->FindData(pNPCSpwnTblData->mob_Tblidx);
				if (pNPCTblData)
				{
					sNPC_SERVER_TBLDAT* pNpcSrv = (sNPC_SERVER_TBLDAT*)g_pTableContainer->GetNpcServerTable()->FindData(pNPCTblData->tblidx);
					if (pNpcSrv)
					{
						if (pNPCTblData->bValidity_Able == true && pNpcSrv->bValidity_Able == true)
						{
							CNpc* pNpc = (CNpc*)CreateCharacter(OBJTYPE_NPC);
							if (pNpc)
							{
								if (!pNpc->CreateDataAndSpawn(pWorldTblData->tblidx, pNPCTblData, pNPCSpwnTblData, true, bySpawnFlag))
								{
									ERR_LOG(LOG_SYSTEM, "Failed to create monster on server start. Tblidx %u World %u spawn location %f %f %f", pNPCTblData->tblidx, pWorldTblData->tblidx, pNPCSpwnTblData->vSpawn_Loc.x, pNPCSpwnTblData->vSpawn_Loc.y, pNPCSpwnTblData->vSpawn_Loc.z);
									m_map_CharByUID.erase(pNpc->GetID());
									m_objectArray[pNpc->GetID() - MAX_GAME_OBJECT] = NULL;
									delete pNpc;
								}
							}
						}
					}
				}
			}
		}

		//spawn mob
		if (pMOBSpawnTbl != NULL)
		{
			BYTE bySpawnFlag = 0;
			BIT_FLAG_SET(bySpawnFlag, SPAWN_FUNC_FLAG_RESPAWN);
			BIT_FLAG_SET(bySpawnFlag, SPAWN_FUNC_FLAG_NO_SPAWN_WAIT);

			for (CTable::TABLEIT itMOBSpawn = pMOBSpawnTbl->Begin(); itMOBSpawn != pMOBSpawnTbl->End(); itMOBSpawn++)
			{
				sSPAWN_TBLDAT* pMOBSpwnTblData = (sSPAWN_TBLDAT*)itMOBSpawn->second;

				if (pMOBSpwnTblData->spawnGroupId != INVALID_SPAWNGROUPID)
					continue;

				sMOB_TBLDAT* pMOBTblData = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(pMOBSpwnTblData->mob_Tblidx);
				if (pMOBTblData)
				{
					sMOB_SERVER_TBLDAT* pMobSrv = (sMOB_SERVER_TBLDAT*)g_pTableContainer->GetMobServerTable()->FindData(pMOBTblData->tblidx);
					if (pMobSrv)
					{
						if (pMOBTblData->bValidity_Able == true && pMobSrv->bValidity_Able == true)
						{
							CMonster* pMob = (CMonster*)CreateCharacter(OBJTYPE_MOB);
							if (pMob)
							{

								if (!pMob->CreateDataAndSpawn(pWorldTblData->tblidx, pMOBTblData, pMOBSpwnTblData, true, bySpawnFlag))
								{
									ERR_LOG(LOG_SYSTEM, "Failed to create monster on server start. Tblidx %u World %u spawn location %f %f %f", pMOBTblData->tblidx, pWorldTblData->tblidx, pMOBSpwnTblData->vSpawn_Loc.x, pMOBSpwnTblData->vSpawn_Loc.y, pMOBSpwnTblData->vSpawn_Loc.z);
										
									m_map_CharByUID.erase(pMob->GetID());
									m_objectArray[pMob->GetID() - MAX_GAME_OBJECT] = NULL;
									delete pMob;
								}
							}
						}
					}
				}
			}
		}

	}
}


bool CObjectManager::Create()
{

	SpawnNpcAndMob();

	return true;
}


void CObjectManager::Destroy()
{
	for (auto it = m_map_CharByUID.begin(); it != m_map_CharByUID.end(); it++)
		SAFE_DELETE(it->second);

	m_map_CharByUID.clear();
	m_map_pkChrByPID.clear();
	m_map_DelayDelete.clear();
	m_map_Respawn.clear();
}


//--------------------------------------------------------------------------------------//
//		UPDATE
//--------------------------------------------------------------------------------------//
void CObjectManager::TickProcess(DWORD dwTickDiff, float fMultiple)
{
	for (auto it = m_map_CharByUID.begin(); it != m_map_CharByUID.end(); it++)
	{
		CGameObject* pGameObject = it->second;
		if (pGameObject)
		{
			pGameObject->TickProcess(dwTickDiff, fMultiple);
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "ERROR pGameObject is null hObject %u !", it->first);
		}
	}

	//Delayed deletion
	ProcessDelayDeleteMap();

	//BOT RESPAWN
	ProcessRespawn();
}


HOBJECT CObjectManager::CreateUID()
{
	HOBJECT hUniqueID = INVALID_HOBJECT;

	if (m_vSerialVec.size() > 0)
	{
		hUniqueID = m_vSerialVec.front();
		m_vSerialVec.erase(m_vSerialVec.begin());
	}
	else
		hUniqueID = uiSerialId++;

	return hUniqueID;
}

void CObjectManager::DeleteUID(HOBJECT hHandle)
{
	m_vSerialVec.push_back(hHandle);
}

CGameObject* CObjectManager::CreateCharacter(eOBJTYPE objtype, CHARACTERID CharID)
{
	HOBJECT hHandle = CreateUID();

	CGameObject* ch = NULL;

	switch (objtype)
	{
		case OBJTYPE_PC: {	ch = new CPlayer; } break;
		case OBJTYPE_NPC: {	ch = new CNpc;	} break;
		case OBJTYPE_MOB: {	ch = new CMonster;	} break;
		case OBJTYPE_SUMMON_PET: {	ch = new CSummonPet;	} break;
		case OBJTYPE_TOBJECT: {} break;
		case OBJTYPE_DYNAMIC: {} break;

		default:{ ERR_LOG(LOG_SYSTEM, "ERROR Object type %d not available", objtype); return NULL; }
	}

	UINT32 uiArrayID = UnsignedSafeDecrease<UINT32>(hHandle, MAX_GAME_OBJECT);
	if (uiArrayID == 0 || hHandle - MAX_GAME_OBJECT >= MAX_GAME_OBJECT)
	{
		ERR_LOG(LOG_GENERAL, "Create OBJ arrayID is null hHandle is %u. id inside obj array %u . Objtype %u \n", hHandle, hHandle - MAX_GAME_OBJECT, objtype);
		return NULL;
	}

	ch->SetID(hHandle);

	m_map_CharByUID.insert(std::make_pair(hHandle, ch));

	if (objtype == OBJTYPE_PC)
	{
		m_map_pkChrByPID.insert(std::make_pair(CharID, static_cast<CPlayer*>(ch)));

	//	ERR_LOG(LOG_GENERAL, "Create player hHandle %u. id inside obj array %u \n", hHandle, hHandle - MAX_GAME_OBJECT);
	}

	if (m_objectArray[uiArrayID])
	{
		ERR_LOG(LOG_GENERAL, "ERROR!!! The uiArrayID %u already exists in objectArray !!", uiArrayID);
		return NULL;
	}

	m_objectArray[uiArrayID] = ch;

	return ch;
}


CGameObject* CObjectManager::GetObjectA(HOBJECT hHandle)
{
	if (hHandle - MAX_GAME_OBJECT < MAX_GAME_OBJECT)
		return m_objectArray[hHandle - MAX_GAME_OBJECT];

	return NULL;
}

CCharacterObject* CObjectManager::GetCharacterObject(HOBJECT hHandle)
{
	return dynamic_cast<CCharacterObject*>(GetObjectA(hHandle));
}

CCharacter* CObjectManager::GetChar(HOBJECT hHandle)
{
	return dynamic_cast<CCharacter*>(GetObjectA(hHandle));
}

CCharacter* CObjectManager::GetChar(eOBJTYPE eObjType, HOBJECT hHandle)
{
	switch (eObjType)
	{
		case OBJTYPE_PC:
		{
			return GetPC(hHandle);
		}
		break;
		case OBJTYPE_NPC:
		{
			return GetNpc(hHandle);
		}
		break;
		case OBJTYPE_MOB:
		{
			return GetMob(hHandle);
		}
		break;
		case OBJTYPE_SUMMON_PET:
		{
			return GetSummonPet(hHandle);
		}
		break;

		default:
		{
			ERR_LOG(LOG_SYSTEM, "fail : eObjType(%d), handle(%d)", eObjType, hHandle);
		}
		break;
	}

	return NULL;
}

CPlayer* CObjectManager::GetPC(HOBJECT hHandle)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(GetObjectA(hHandle));

	if (pPlayer && pPlayer->IsLoggedOut()) //if player is logged out then return null.
		return NULL;

	return pPlayer;
}

CNpc* CObjectManager::GetNpc(HOBJECT hHandle)
{
	return dynamic_cast<CNpc*>(GetObjectA(hHandle));
}

CMonster* CObjectManager::GetMob(HOBJECT hHandle)
{
	return dynamic_cast<CMonster*>(GetObjectA(hHandle));
}

CSummonPet * CObjectManager::GetSummonPet(HOBJECT hHandle)
{
	return dynamic_cast<CSummonPet*>(GetObjectA(hHandle));
}

CPlayer* CObjectManager::FindByAccount(ACCOUNTID uiAccId)
{
	for (auto it = m_map_pkChrByPID.begin(); it != m_map_pkChrByPID.end(); it++)
	{
		CPlayer* found = it->second;
		if(found)
		{
			if (found->GetAccountID() == uiAccId && found->GetClientSession())
				return found;
		}
	}

	return NULL;
}

CPlayer* CObjectManager::FindByChar(CHARACTERID uiCharId)
{
	auto it = m_map_pkChrByPID.find(uiCharId);

	if (m_map_pkChrByPID.end() == it)
		return NULL;

	return it->second;
}

CPlayer* CObjectManager::FindByName(WCHAR* wszCharName)
{
	for (auto it = m_map_pkChrByPID.begin(); it != m_map_pkChrByPID.end(); it++)
	{
		CPlayer* found = it->second;
		if(found)
		{
			if (_wcsicmp(found->GetCharName(), wszCharName) == 0 && found->GetClientSession())
			{
				return found;
			}
		}
	}
	return NULL;
}
CPlayer* CObjectManager::FindByName(const WCHAR* wszCharName)
{
	for (auto it = m_map_pkChrByPID.begin(); it != m_map_pkChrByPID.end(); it++)
	{
		CPlayer* found = it->second;
		if(found && found->IsInitialized())
		{
			if (_wcsicmp(found->GetCharName(), wszCharName) == 0)
			{
				return found;
			}
		}
	}
	return NULL;
}

void CObjectManager::DestroyCharacter(CGameObject* ch)
{
	if(ch->IsPC())
	{
		CPlayer* plr = static_cast<CPlayer*>(ch);
		m_map_pkChrByPID.erase(plr->GetCharID());
		plr->LeaveGame();
	}

	m_objectArray[ch->GetID() - MAX_GAME_OBJECT] = NULL; //remove from array so we cant get it

	m_map_DelayDelete.insert(std::make_pair(ch, GetTickCount()));
}


void CObjectManager::AddRespawn(CNpc* pBot)
{
	m_map_Respawn.insert(std::make_pair(pBot, GetTickCount()));
}

void CObjectManager::RemoveSpawn(HOBJECT hHandle)
{
	for (auto it = m_map_Respawn.begin(); it != m_map_Respawn.end(); it++)
	{
		CNpc* pBot = it->first;
		if (pBot)
		{
			if (pBot->GetID() == hHandle)
			{
				m_map_Respawn.erase(it);
				ERR_LOG(LOG_GENERAL, "Handle %u successfully removed", hHandle);
				return;
			}
		}
	}
}

//--------------------------------------------------------------------------------------//
//	respawn process
//--------------------------------------------------------------------------------------//
void CObjectManager::ProcessRespawn()
{
	if (m_map_Respawn.size() > 0)
	{
		DWORD dwCurTick = GetTickCount();
		for (auto it = m_map_Respawn.begin(); it != m_map_Respawn.end(); )
		{
			CNpc* pBot = it->first;
			if (pBot)
			{
				if (dwCurTick > it->second + (pBot->GetSpawnTime() * 1000))
				{
					if (BIT_FLAG_TEST(pBot->GetSpawnFuncFlag(), SPAWN_FUNC_FLAG_RESPAWN)) //Check if can spawn
						pBot->Spawn(false);
					else
					{
						ERR_LOG(LOG_SYSTEM, "INFO: Obj without respawn flag inside respawn map.");
						DestroyCharacter(pBot);
					}

					it = m_map_Respawn.erase(it);
					continue;
				}
				++it;
			}
			else
			{
				ERR_LOG(LOG_SYSTEM, "BOT does not exist anymore.");
				it = m_map_Respawn.erase(it);
			}
		}
	}
}

//--------------------------------------------------------------------------------------//
//	delayed deletion
//--------------------------------------------------------------------------------------//
void CObjectManager::ProcessDelayDeleteMap()
{
	if (m_map_DelayDelete.size() > 0)
	{
		DWORD dwCurTick = GetTickCount();
		for (auto it = m_map_DelayDelete.begin(); it != m_map_DelayDelete.end(); )
		{
			if (dwCurTick > it->second + TIME_DELETE_DELAY)
			{
				CGameObject* ch = it->first;

				if (ch->IsNPC() || ch->IsMonster())
				{
					CNpc* pNpc = (CNpc*)ch;

					if (pNpc->IsInRespawn())
					{
						ERR_LOG(LOG_GENERAL, "ObjType %u, Handle %u, Tblidx %u is in respawn progress. Removing from respawn map..", pNpc->GetObjType(), pNpc->GetID(), pNpc->GetTblidx());
						RemoveSpawn(pNpc->GetID());
					}
					else if (pNpc->IsInitialized())
					{
						ERR_LOG(LOG_GENERAL, "ERROR: ObjType %u, Handle %u, Tblidx %u is respawned while in delete map !!!", pNpc->GetObjType(), pNpc->GetID(), pNpc->GetTblidx());
						it = m_map_DelayDelete.erase(it);
						pNpc->SetSpawnFuncFlag(0);
						pNpc->LeaveGame();
						continue;
					}
				}

			//	ERR_LOG(LOG_GENERAL, "Delete object type %u id %u", ch->GetObjType(), ch->GetID());
				m_map_CharByUID.erase(ch->GetID());
				DeleteUID(ch->GetID());

				it = m_map_DelayDelete.erase(it);
				SAFE_DELETE(ch);
				continue;
			}

			++it;
		}
	}
}

//--------------------------------------------------------------------------------------//
//		SEND PACKET TO ALL PLAYERS
//--------------------------------------------------------------------------------------//
void CObjectManager::SendPacketToAll(CNtlPacket * pPacket)
{
	for (auto it = m_map_pkChrByPID.begin(); it != m_map_pkChrByPID.end(); it++)
	{
		CPlayer* ch = it->second;
		if (ch && ch->IsInitialized())
			ch->SendPacket(pPacket);
	}
}

//--------------------------------------------------------------------------------------//
//		remove all players
//--------------------------------------------------------------------------------------//
bool CObjectManager::DisconnectAll()
{
	CPlayer* ch = NULL;
	for (auto it = m_map_pkChrByPID.begin(); it != m_map_pkChrByPID.end(); it++)
	{
		ch = it->second;
		if (ch->GetClientSession())
			ch->GetClientSession()->Disconnect(false);
	}

	return true;
}

//--------------------------------------------------------------------------------------//
//		SEND PACKET TO ALL PLAYERS IN ZONE
//--------------------------------------------------------------------------------------//
void CObjectManager::SendToAllInZone(ZONEID zoneId, CNtlPacket * pPacket)
{
	for (auto it = m_map_pkChrByPID.begin(); it != m_map_pkChrByPID.end(); it++)
	{
		CPlayer* ch = it->second;
		if(ch && ch->IsInitialized())
		{
			if (ch->GetMapNameTblidx() / 1000 == zoneId)
				ch->SendPacket(pPacket);
		}
	}
}

