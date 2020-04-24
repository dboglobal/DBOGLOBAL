#include "stdafx.h"
#include "World.h"
#include "WorldZone.h"
#include "WorldCell.h"
#include "TableContainerManager.h"
#include "TriggerObject.h"
#include "SpawnTable.h"
#include "MobTable.h"
#include "MobServerTable.h"
#include "NpcServerTable.h"
#include "Monster.h"
#include "SummonPet.h"
#include "CPlayer.h"
#include "ScriptAlgoObjectManager.h"
#include "WpsAlgoObject.h"
#include "ServerConfigTable.h"
#include "NtlNavi.h"
#include "ItemDrop.h"
#include "ItemManager.h"
#include "GameServer.h"


CWorld::CWorld()
{
	Init();
}

CWorld::~CWorld()
{
	Destroy();
}


void CWorld::Init()
{
	m_worldID = INVALID_WORLDID;
	m_pTbldat = NULL;
	m_vStart.Reset();
	m_vEnd.Reset();
	m_vStartBoundary.Reset();
	m_vEndBoundary.Reset();
	m_vWorldSize.Reset();
	m_nEdY = 0;
	m_nEdX = 0;
	m_nStY = 0;
	m_nStX = 0;
	m_dwTotalCount = 0;
	m_dwCellVerticalCount = 0;
	m_dwCellHorizontalCount = 0;
	m_paWorldCell = NULL;
	m_mapZone.clear();
	m_hNaviInstance = NULL;

	m_hTriggerObjectOffset = 1;
}

int CWorld::Create(WORLDID worldID, sWORLD_TBLDAT* pTbldat, CWorldZoneTable* pWorldZoneTable)
{
	m_worldID = worldID;
	m_pTbldat = pTbldat;

	if (!CreateZoneMap(pWorldZoneTable))
		return 100001;

	if (GetNaviEngine()->IsPropDataLoaded(pTbldat->dwWorldResourceID))
	{
		m_hNaviInstance = GetNaviEngine()->CreateInstanceHandler(pTbldat->dwWorldResourceID);
		if (m_hNaviInstance == NULL)
		{
			printf("Navi: CreateInstanceHandler failed. World tblidx %u \n", GetIdx());
			return 100001;
		}
	}

	int nRes = Split(pTbldat);

	if (nRes != NTL_SUCCESS)
		return nRes;

	//Create world objects
	CObjectTable* pObj = g_pTableContainer->GetObjectTable(pTbldat->tblidx);
	if (pObj)
	{
		for (CTable::TABLEIT itObjSpawn = pObj->Begin(); itObjSpawn != pObj->End(); itObjSpawn++)
		{
			sOBJECT_TBLDAT* pObjTblData = (sOBJECT_TBLDAT*)itObjSpawn->second;

			if (pObjTblData)
			{
				CTriggerObject* pStaticObj = new CTriggerObject;

				pStaticObj->Create(MakeTriggerObjectHandle(m_hTriggerObjectOffset, pObjTblData->dwSequence));
				pStaticObj->SetToObjTable(pObjTblData);
				pStaticObj->SetInitialized(true);

				pStaticObj->OnEnterWorld(this);
				m_objectList.Add(pStaticObj->GetWorldObjectLinker());

				CWorldCell* pWorldCell = GetWorldCellWithLoc(pObjTblData->vLoc);
				if (pWorldCell)
				{
					pStaticObj->OnEnterWorldCell(pWorldCell);

					if (pWorldCell->GetWorldZone()) //because we do not have all world data, we have to check if world zone exist.
						pStaticObj->OnEnterWorldZone(pWorldCell->GetWorldZone());
				}

				m_map_TriggerObj.insert(std::make_pair(pStaticObj->GetID(), pStaticObj));
			}
		}
	}

	return NTL_SUCCESS;
}

void CWorld::Destroy()
{
	DestroyWorldCellArray();

	for (ZONEMAP::iterator it = m_mapZone.begin(); it != m_mapZone.end(); it++)
	{
		CWorldZone* pWorldZone = it->second;
		if (pWorldZone)
			delete pWorldZone;
		else
			ERR_LOG(LOG_SYSTEM, "(NULL == pWorldZone), iter->first = %u", it->first);
	}

	m_mapZone.clear();

	if (m_hNaviInstance)
	{
		GetNaviEngine()->DeleteInstanceHandler(m_hNaviInstance);
		m_hNaviInstance = NULL;
	}
}


void CWorld::TickProcess(DWORD dwTickDiff, float fMultiple)
{
	if (HasPlayer() == false && GetTbldat()->bDynamic == true)
		return;

	/*CWorldZone* pWorldZone = NULL;
	for (ZONEMAP::iterator it = m_mapZone.begin(); it != m_mapZone.end(); it++)
	{
		pWorldZone = it->second;
		if (pWorldZone)
		{
			pWorldZone->TickProcess(dwTickDiff, fMultiple);
		}
		else
			ERR_LOG(LOG_SYSTEM, "(NULL == pWorldZone), iter->first = %u", it->first);
	}*/

	//Update trigger objects
	for (std::map<HOBJECT, CTriggerObject*>::iterator it = m_map_TriggerObj.begin(); it != m_map_TriggerObj.end(); it++)
	{
		CTriggerObject* pTriggerObj = it->second;

		pTriggerObj->TickProcess(dwTickDiff);
	}
}


int CWorld::OnObjectMsg(CObjectMsg* pObjMsg)
{
	return 0;
}


int CWorld::Enter(CSpawnObject* pSpawnObject, bool bIsServerStart/* = false*/)
{
	if (pSpawnObject->GetWorldObjectLinker()->GetPrev() || pSpawnObject->GetWorldObjectLinker()->GetNext())
	{
		ERR_LOG(LOG_SYSTEM, "[Handle : %u] pPrevLinker, pNextLinker", pSpawnObject->GetID());

		if (pSpawnObject->GetWorldObjectLinker()->GetPrev() && !pSpawnObject->GetWorldObjectLinker()->GetNext())
		{
			ERR_LOG(LOG_SYSTEM, "[Handle : %u] pPrevLinker actually is not linked with the current linker.(NULL == pPrevLinker->GetNext())", pSpawnObject->GetID());
		}
		if (!pSpawnObject->GetWorldObjectLinker()->GetPrev() && pSpawnObject->GetWorldObjectLinker()->GetNext())
		{
			ERR_LOG(LOG_SYSTEM, "[Handle : %u] pNextLinker actually is not linked with the current linker.(NULL == pNextLinker->GetPrev())", pSpawnObject->GetID());
		}
	}

	if (pSpawnObject->GetCurWorld())
	{
		ERR_LOG(LOG_SYSTEM, "[Handle : %u] (NULL != pSpawnObject->GetCurWorld()), pSpawnObject->GetCurWorld()->GetID() = %u", pSpawnObject->GetID(), pSpawnObject->GetCurWorld()->GetID());
		return 100001;
	}

	if (IsInBoundary(pSpawnObject->GetCurLoc()))
	{
		CWorldCell* pWorldCell = GetWorldCellWithLoc(pSpawnObject->GetCurLoc());
		if (pWorldCell)
		{
			pSpawnObject->OnEnterWorld(this);
			if (pWorldCell->EnterObject(pSpawnObject, bIsServerStart) != NTL_SUCCESS)
			{
				ERR_LOG(LOG_SYSTEM, "[Handle : %u] (pWorldCell->EnterObject != NTL_SUCCESS), pSpawnObject->GetCurLoc() = (%f, %f, %f)", pSpawnObject->GetID(), pSpawnObject->GetCurLoc().x, pSpawnObject->GetCurLoc().y, pSpawnObject->GetCurLoc().z);
			//	CNtlMiniDump::Snapshot();
			}
			else
			{
				if (m_objectList.Add(pSpawnObject->GetWorldObjectLinker()))
				{
					pSpawnObject->OnEnterWorldComplete();

					return NTL_SUCCESS;
				}
				else
				{
					ERR_LOG(LOG_SYSTEM, "World ObjectList Add Error");
				}
			}
		}
		else
		{
			ERR_LOG(LOG_SYSTEM, "[Handle : %u] (NULL == pWorldCell), pSpawnObject->GetCurLoc() = (%f, %f, %f)", pSpawnObject->GetID(), pSpawnObject->GetCurLoc().x, pSpawnObject->GetCurLoc().y, pSpawnObject->GetCurLoc().z);
		}
	}
	else
	{
		ERR_LOG(LOG_SYSTEM, "[Handle : %u] (IsInBoundary == false), pSpawnObject->GetCurLoc() = (%f, %f, %f)", pSpawnObject->GetID(), pSpawnObject->GetCurLoc().x, pSpawnObject->GetCurLoc().y, pSpawnObject->GetCurLoc().z);
	}

	return 100001;
}

int CWorld::Leave(CSpawnObject* pSpawnObject)
{
	if (this == pSpawnObject->GetCurWorld())
	{
		CWorldCell* pWorldCell = pSpawnObject->GetCurWorldCell();
		if (pWorldCell && pWorldCell->LeaveObject(pSpawnObject) == NTL_SUCCESS)
		{
			if (m_objectList.Remove(pSpawnObject->GetWorldObjectLinker()))
			{
				if (pSpawnObject->GetWorldObjectLinker()->GetPrev() || pSpawnObject->GetWorldObjectLinker()->GetNext())
				{
					ERR_LOG(LOG_SYSTEM, "[Handle : %u] ASSERT1: pPrevLinker, pNextLinker", pSpawnObject->GetID());

					if (pSpawnObject->GetWorldObjectLinker()->GetPrev() && !pSpawnObject->GetWorldObjectLinker()->GetNext())
					{
						ERR_LOG(LOG_SYSTEM, "[Handle : %u] ASSERT1: pPrevLinker actually is not linked with the current linker.(NULL == pPrevLinker->GetNext())", pSpawnObject->GetID());
					}
					if (!pSpawnObject->GetWorldObjectLinker()->GetPrev() && pSpawnObject->GetWorldObjectLinker()->GetNext())
					{
						ERR_LOG(LOG_SYSTEM, "[Handle : %u] ASSERT1: pNextLinker actually is not linked with the current linker.(NULL == pNextLinker->GetPrev())", pSpawnObject->GetID());
					}
				}

				pSpawnObject->OnLeaveWorld(this);
				return NTL_SUCCESS;
			}
		}
		else
		{
			m_objectList.Remove(pSpawnObject->GetWorldObjectLinker());

			if (pSpawnObject->GetWorldObjectLinker()->GetPrev() || pSpawnObject->GetWorldObjectLinker()->GetNext())
			{
				ERR_LOG(LOG_SYSTEM, "[Handle : %u] ASSERT2: pPrevLinker, pNextLinker", pSpawnObject->GetID());

				if (pSpawnObject->GetWorldObjectLinker()->GetPrev() && !pSpawnObject->GetWorldObjectLinker()->GetNext())
				{
					ERR_LOG(LOG_SYSTEM, "[Handle : %u] ASSERT2: pPrevLinker actually is not linked with the current linker.(NULL == pPrevLinker->GetNext())", pSpawnObject->GetID());
				}
				if (!pSpawnObject->GetWorldObjectLinker()->GetPrev() && pSpawnObject->GetWorldObjectLinker()->GetNext())
				{
					ERR_LOG(LOG_SYSTEM, "[Handle : %u] ASSERT2: pNextLinker actually is not linked with the current linker.(NULL == pNextLinker->GetPrev())", pSpawnObject->GetID());
				}
			}

			return 100001;
		}
	}
	else
	{
		ERR_LOG(LOG_SYSTEM, "[Handle : %u] (this != pSpawnObject->GetCurWorld()), GetID() = %u, pSpawnObject->GetCurWorld()->GetID() = %u", pSpawnObject->GetID(), GetID(), pSpawnObject->GetCurWorld()->GetID());
	}

	return 100001;
}


void CWorld::OnPreCreate()
{
	CGameServer* app = (CGameServer*)g_pApp;

	//dont spawn anything on dojo channel
	if (app->IsDojoChannel())
		return;

	if (GetTbldat()->bDynamic == false && GetTbldat()->wpsLinkIndex != INVALID_TBLIDX) //check if not dungeon map and has WPS script linked
	{
		//Create wps script
		bool bStart;
		CWpsAlgoObject* pObj = g_pScriptAlgoManager->CreateWpsObject(GetTbldat()->wpsLinkIndex, bStart);
		if (pObj)
		{
			pObj->SetWorld(this);
			AddScript(pObj);

			pObj->Start();
		}
	}
}

void CWorld::OnCreate() //only used when creating dungeon world
{
	CGameServer* app = (CGameServer*)g_pApp;

	//dont spawn anything on dojo channel
	if (app->IsDojoChannel() && GetTbldat()->byWorldRuleType != GAMERULE_MINORMATCH && GetTbldat()->byWorldRuleType != GAMERULE_MINORMATCH &&GetTbldat()->byWorldRuleType != GAMERULE_MINORMATCH)
		return;

	SpawnNpcMob();

	if (GetTbldat()->wpsLinkIndex != INVALID_TBLIDX) //check if has WPS script linked
	{
		//Create wps script but do not start the script yet.
		bool bStart;
		CWpsAlgoObject* pObj = g_pScriptAlgoManager->CreateWpsObject(GetTbldat()->wpsLinkIndex, bStart);
		if (pObj)
		{
			pObj->SetWorld(this);
			AddScript(pObj);
		}
	}
}

void CWorld::OnDestroy()
{
	//printf("CWorld::OnDestroy() \n");
	//remove all scripts first before we despawn npcs and mobs. 
	RemAllScript();

	DespawnNpcMob();
	
	//reset object states
	for (std::map<HOBJECT, CTriggerObject*>::iterator it = m_map_TriggerObj.begin(); it != m_map_TriggerObj.end(); it++)
	{
		CTriggerObject* pTriggerObj = it->second;

		pTriggerObj->StopExecuting(true, false);
	}
}


void CWorld::SpawnNpcMob()
{
	sWORLD_TBLDAT* pWorldTblData = GetTbldat();

	CSpawnTable* pNPCSpawnTbl = g_pTableContainer->GetNpcSpawnTable(pWorldTblData->tblidx);
	CSpawnTable* pMOBSpawnTbl = g_pTableContainer->GetMobSpawnTable(pWorldTblData->tblidx);

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
						CNpc* pNpc = (CNpc*)g_pObjectManager->CreateCharacter(OBJTYPE_NPC);
						if (pNpc)
						{
							if (!pNpc->CreateDataAndSpawn(GetID(), pNPCTblData, pNPCSpwnTblData, false, SPAWN_FUNC_FLAG_NO_SPAWN_WAIT))
							{
								ERR_LOG(LOG_SYSTEM, "Failed to create NPC. Tblidx %u World %u spawn location %f %f %f", pNPCTblData->tblidx, pWorldTblData->tblidx, pNPCSpwnTblData->vSpawn_Loc.x, pNPCSpwnTblData->vSpawn_Loc.y, pNPCSpwnTblData->vSpawn_Loc.z);
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
						CMonster* pMob = (CMonster*)g_pObjectManager->CreateCharacter(OBJTYPE_MOB);
						if (pMob)
						{
							if (!pMob->CreateDataAndSpawn(GetID(), pMOBTblData, pMOBSpwnTblData, false, SPAWN_FUNC_FLAG_NO_SPAWN_WAIT))
							{
								ERR_LOG(LOG_SYSTEM, "Failed to create monster. Tblidx %u World %u spawn location %f %f %f", pMOBTblData->tblidx, pWorldTblData->tblidx, pMOBSpwnTblData->vSpawn_Loc.x, pMOBSpwnTblData->vSpawn_Loc.y, pMOBSpwnTblData->vSpawn_Loc.z);
								delete pMob;
							}
						}
					}
				}
			}
		}
	}
}


void CWorld::DespawnNpcMob()
{
	CNpc* pNextObject = NULL;
	CNpc* pExistObject = (CNpc*)m_objectList.GetFirst(OBJTYPE_NPC);
	int nObj = m_objectList.GetObjCount(OBJTYPE_NPC);

	for(int i = 0; i < nObj; i++)
	{
		pNextObject = (CNpc*)m_objectList.GetNext(pExistObject->GetWorldObjectLinker());

		pExistObject->SetSpawnFuncFlag(0);

		if (pExistObject->IsInitialized())
			pExistObject->LeaveGame();

		pExistObject = pNextObject;
	}

	pNextObject = NULL;

	pExistObject = (CNpc*)m_objectList.GetFirst(OBJTYPE_MOB);
	nObj = m_objectList.GetObjCount(OBJTYPE_MOB);

	for (int i = 0; i < nObj; i++)
	{
		pNextObject = (CNpc*)m_objectList.GetNext(pExistObject->GetWorldObjectLinker());

		pExistObject->SetSpawnFuncFlag(0);

		if(pExistObject->IsInitialized())
			pExistObject->LeaveGame();

		pExistObject = pNextObject;
	}

	CItemDrop* pDropNextObject = NULL;

	CItemDrop* pDropExistObject = (CItemDrop*)m_objectList.GetFirst(OBJTYPE_DROPMONEY);
	nObj = m_objectList.GetObjCount(OBJTYPE_DROPMONEY);

	for (int i = 0; i < nObj; i++)
	{
		pDropNextObject = (CItemDrop*)m_objectList.GetNext(pDropExistObject->GetWorldObjectLinker());

		if (pDropExistObject->IsInitialized())
		{
			g_pItemManager->DestroyItemDropOverTime(pDropExistObject);
		}

		pDropExistObject = pDropNextObject;
	}

	pDropNextObject = NULL;

	pDropExistObject = (CItemDrop*)m_objectList.GetFirst(OBJTYPE_DROPITEM);
	nObj = m_objectList.GetObjCount(OBJTYPE_DROPITEM);

	for (int i = 0; i < nObj; i++)
	{
		pDropNextObject = (CItemDrop*)m_objectList.GetNext(pDropExistObject->GetWorldObjectLinker());

		if (pDropExistObject->IsInitialized())
		{
			g_pItemManager->DestroyItemDropOverTime(pDropExistObject);
		}

		pDropExistObject = pDropNextObject;
	}

//	printf("CWorld::DespawnNpcMob(): m_objectList.GetObjCount(OBJTYPE_NPC) %u, Max %u \n", m_objectList.GetObjCount(OBJTYPE_NPC), m_objectList.GetObjMaxCount(OBJTYPE_NPC));
}


int CWorld::CopyToInfo(sWORLD_INFO* pWorldInfo)
{
	pWorldInfo->worldID = GetID();
	pWorldInfo->tblidx = GetIdx();
	pWorldInfo->hTriggerObjectOffset = m_hTriggerObjectOffset;
	pWorldInfo->sRuleInfo.byRuleType = GetTbldat()->byWorldRuleType;

	if (GetTbldat()->byWorldRuleType == GAMERULE_CCBATTLEDUNGEON)
	{
		sSERVERCONFIG_BATTLEDUNGEON* pCCBD = &g_pTableContainer->GetServerConfigTable()->GetServerConfigData()->sBattleDungeonData;

		pWorldInfo->sRuleInfo.sBattleDungeonRuleInfo.bEpRegen = pCCBD->bEpRegen;
		pWorldInfo->sRuleInfo.sBattleDungeonRuleInfo.bExpAdd = pCCBD->bExpAdd;
		pWorldInfo->sRuleInfo.sBattleDungeonRuleInfo.bItemDrop = pCCBD->bItemDrop;
		pWorldInfo->sRuleInfo.sBattleDungeonRuleInfo.bItemUse = pCCBD->bItemUse;
		pWorldInfo->sRuleInfo.sBattleDungeonRuleInfo.bLpRegen = pCCBD->bLpRegen;
	}
	

	return sizeof(sWORLD_INFO);
}


bool CWorld::CanDoBattle(CSpawnObject* pSubject, CSpawnObject* pTarget)
{
	if (pSubject && pTarget)
	{
		//if data not loaded, then return true..
		if (GetNaviEngine()->IsPropDataLoaded(GetTbldat()->dwWorldResourceID) == false)
		{
			//printf("worldidx %u, resourceid %u not loaded.. \n", GetIdx(), GetTbldat()->dwWorldResourceID);
			return true;
		}

		if (pSubject->GetObjType() != OBJTYPE_TOBJECT && pTarget->GetObjType() != OBJTYPE_TOBJECT)
		{
			CSpawnObject* pSub = pSubject;

			if (pSubject->GetObjType() == OBJTYPE_SUMMON_PET)
			{
				CSummonPet* pPet = (CSummonPet*)pSubject;

				pSub = pPet->GetOwner();
			}
			if (pTarget->GetObjType() == OBJTYPE_SUMMON_PET)
			{
				CSummonPet* pPet = (CSummonPet*)pSubject;

				pSub = pPet->GetOwner();
			}

			if (pSub == NULL)
				return false;

			if (GetNaviEngine()->IsBasicAttributeSet(GetNaviInstanceHandle(), pSub->GetCurLoc().x, pSub->GetCurLoc().z, DBO_WORLD_ATTR_BASIC_FREE_PVP_ZONE))
			{
				if (GetNaviEngine()->IsBasicAttributeSet(GetNaviInstanceHandle(), pTarget->GetCurLoc().x, pTarget->GetCurLoc().z, DBO_WORLD_ATTR_BASIC_FREE_PVP_ZONE))
				{
					return true;
				}
			}
		}

		if (pTarget->GetCurWorld() == NULL)
			return false;
		if (IsInBoundary(pSubject->GetCurLoc()) == false)
			return false;
		if (pTarget->GetCurWorld()->IsInBoundary(pTarget->GetCurLoc()) == false)
			return false;

		if (pSubject->GetObjType() > OBJTYPE_PC)
		{
			if (pSubject->GetObjType() == OBJTYPE_SUMMON_PET && (pTarget->GetObjType() == OBJTYPE_PC || pTarget->GetObjType() == OBJTYPE_SUMMON_PET))
			{
				if (GetNaviEngine()->IsBasicAttributeSet(GetNaviInstanceHandle(), pTarget->GetCurLoc().x, pTarget->GetCurLoc().z, DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE))
					return false;

				if (GetNaviEngine()->IsBasicAttributeSet(GetNaviInstanceHandle(), pSubject->GetCurLoc().x, pSubject->GetCurLoc().z, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE))
					return false;
			}
		}
		else
		{
			if (GetNaviEngine()->IsBasicAttributeSet(GetNaviInstanceHandle(), pSubject->GetCurLoc().x, pSubject->GetCurLoc().z, DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE))
				return false;


			if (pTarget->GetObjType() == OBJTYPE_PC || pTarget->GetObjType() == OBJTYPE_SUMMON_PET)
			{
				if (GetNaviEngine()->IsBasicAttributeSet(GetNaviInstanceHandle(), pSubject->GetCurLoc().x, pSubject->GetCurLoc().z, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE))
					return false;

				if (GetNaviEngine()->IsBasicAttributeSet(GetNaviInstanceHandle(), pTarget->GetCurLoc().x, pTarget->GetCurLoc().z, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE))
					return false;
			}
		}

		return true;
	}

	return false;
}

bool CWorld::CanMoveTo(CNtlVector& rLoc, CSpawnObject* pObject)
{
	if (IsInBoundary(pObject->GetCurLoc()))
	{
		return true;
	}

	return false;
}

bool CWorld::IsSwimmingArea(CNtlVector& rLoc)
{
	if (IsInBoundary(rLoc))
	{
		return GetNaviEngine()->IsBasicAttributeSet(GetNaviInstanceHandle(), rLoc.x, rLoc.z, DBO_WORLD_ATTR_BASIC_ENABLE_SWIM);
	}

	return false;
}

bool CWorld::CanFly()
{
	return BIT_FLAG_TEST(GetTbldat()->dwProhibition_Bit_Flag, eWORLD_PROPHITED_AIR_FLAG) == false;
}


int CWorld::Split(sWORLD_TBLDAT* pTbldat)
{
	CNtlVector vBoundaryAreaOffset(512.0f, 0.0f, 512.0f);
	m_vStart = (pTbldat->vStart - vBoundaryAreaOffset) + pTbldat->fSplitSize;
	m_vEnd = (pTbldat->vEnd + vBoundaryAreaOffset) - pTbldat->fSplitSize;
	//NTL_PRINT(PRINT_APP, "Split WorldIdx %u Start(%f, %f, %f) End(%f, %f, %f)", pTbldat->tblidx, m_vStart.x, m_vStart.y, m_vStart.z, m_vEnd.x, m_vEnd.y, m_vEnd.z);

	CNtlVector vOffset(1.0f, 1.0f, 1.0f);
	m_vWorldSize = (m_vStart - m_vEnd) + vOffset;
	//NTL_PRINT(PRINT_APP, "m_vWorldSize(%f, %f, %f)", m_vWorldSize.x, m_vWorldSize.y, m_vWorldSize.z);
	
	if (pTbldat->fSplitSize * 3.0f < m_vWorldSize.x && pTbldat->fSplitSize * 3.0f < m_vWorldSize.z)
	{
		m_dwCellHorizontalCount = (DWORD) floor(m_vWorldSize.x / pTbldat->fSplitSize);
		m_dwCellVerticalCount = (DWORD) floor(m_vWorldSize.z / pTbldat->fSplitSize);
		m_dwTotalCount = m_dwCellVerticalCount * m_dwCellHorizontalCount;

		m_nStY = 1;
		m_nStX = 1;
		m_nEdX = m_dwCellHorizontalCount - 2;
		m_nEdY = m_dwCellVerticalCount - 2;

		CreateWorldCellArray(m_dwTotalCount);
		if (m_paWorldCell)
		{
			CNtlVector pvStart;
			CNtlVector pvEnd;

			for (CELLID cellID = 0; cellID < m_dwTotalCount; cellID++)
			{
				CWorldCell* pWorldCell = GetWorldCell(cellID);
				if (!pWorldCell)
					return 100003;

				GetCellStartPos(cellID, &pvStart);
				GetCellEndPos(cellID, &pvEnd);

				int rc = pWorldCell->Create(this, cellID, pvStart, pvEnd);
				if (rc != NTL_SUCCESS)
					return rc;
			}

			CWorldCell* pStartCell = GetWorldCell(m_dwCellHorizontalCount + 1);
			if (pStartCell)
			{
				m_vStartBoundary = pStartCell->GetStartLoc();
				m_vStartBoundary.x = m_vStartBoundary.x - 10.0f; //


				CELLID endId = m_dwCellHorizontalCount + (m_dwCellVerticalCount - 2) * m_dwCellHorizontalCount - 2;

				CWorldCell* pEndCell = GetWorldCell(endId);
				if (pEndCell)
				{
					m_vEndBoundary = pEndCell->GetEndLoc();
				}
				else 
				{
					ERR_LOG(LOG_SYSTEM, "NULL == pEndCell. worldId[%u]", m_worldID);
					return 100001;
				}
			}
			else
			{
				ERR_LOG(LOG_SYSTEM, "NULL == pStartCell. worldId[%u]", m_worldID);
				return 100001;
			}
		}
		else
		{
			return 100003;
		}
	}
	else
	{
		return 100001;
	}

	return NTL_SUCCESS;
}


void CWorld::AdjustLocation(CNtlVector& rLocation)
{
	if (rLocation.x <= this->m_vStartBoundary.x)
	{
		if (this->m_vEndBoundary.x >= rLocation.x)
			rLocation.x = this->m_vEndBoundary.x;
	}
	else
	{
		rLocation.x = this->m_vStartBoundary.x;
	}

	if (rLocation.z <= this->m_vStartBoundary.z)
	{
		if (this->m_vEndBoundary.z >= rLocation.z)
			rLocation.z = this->m_vEndBoundary.z;
	}
	else
	{
		rLocation.z = this->m_vStartBoundary.z;
	}
}


bool CWorld::IsInBoundary(CNtlVector& rLoc)
{
	//printf("%f %f %f | %f %f %f | %f %f %f\n", rLoc.x, rLoc.y, rLoc.z, m_vStartBoundary.x, m_vStartBoundary.y, m_vStartBoundary.z, m_vEndBoundary.x, m_vEndBoundary.y, m_vEndBoundary.z);
	if (rLoc.x < this->m_vStartBoundary.x)
	{
		if (this->m_vEndBoundary.x < rLoc.x)
		{
			if (rLoc.z < this->m_vStartBoundary.z)
				return this->m_vEndBoundary.z < rLoc.z;
		}
	}

	return false;
}


void CWorld::Broadcast(CNtlPacket* pPacket, CSpawnObject* pSubject, float fRange)
{
	BroadcastPacket(pPacket, pSubject, fRange);
}

void CWorld::Broadcast(CNtlPacket* pPacket, CSpawnObject* pExcept/* = NULL */)
{
	BroadcastPacket(pPacket, pExcept);
}

void CWorld::AddEventIdToAllPlayers(DWORD dwEventID)
{
	CPlayer* pExistObject = (CPlayer*)m_objectList.GetFirst(OBJTYPE_PC);
	int nCount = m_objectList.GetObjCount(OBJTYPE_PC);

	for (int i = 0; i < nCount; i++) //use for instead of "while" to avoid endless loop ;)
	{
		pExistObject->GetQuests()->GetEventMap()->OnEventReceived(dwEventID);

		pExistObject = (CPlayer*)m_objectList.GetNext(pExistObject->GetWorldObjectLinker());
	}
}

void CWorld::AddEventIdToAllPlayers(DWORD dwEventID, CSpawnObject * pSubject, float fRange)
{
	int nObjCount = m_objectList.GetObjCount(OBJTYPE_PC);
	CPlayer* pExistObject = (CPlayer*)m_objectList.GetFirst(OBJTYPE_PC);

	int n = 0;
	for (int i = 0; i < nObjCount; i++) //use for instead of "while" to avoid endless loop ;)
	{
		if (!pSubject || (pExistObject && pSubject->IsInRange(pExistObject->GetCurLoc(), fRange)))
		{
			pExistObject->GetQuests()->GetEventMap()->OnEventReceived(dwEventID);
		}

		if (++n >= MAX_PC)
		{
			ERR_LOG(LOG_GENERAL, "ERROR !! ENDLESS LOOP.");
			break;
		}

		pExistObject = (CPlayer*)m_objectList.GetNext(pExistObject->GetWorldObjectLinker());
	}
}


bool CWorld::HasPlayer()
{
	return GetObjectList()->GetObjCount(OBJTYPE_PC) != 0;
}


WORLDID CWorld::GetID()
{
	return m_worldID;
}


TBLIDX CWorld::GetIdx()
{
	if (m_pTbldat)
	{
		return m_pTbldat->tblidx;
	}
	else
	{
		ERR_LOG(LOG_SYSTEM, "World[%u] Tbldat NULL", GetID());
	}

	return INVALID_TBLIDX;
}


const char* CWorld::GetName()
{
	if (m_pTbldat)
	{
		return m_pTbldat->szName;
	}
	else
	{
		ERR_LOG(LOG_SYSTEM, "World[%u] Tbldat NULL", GetID());
	}

	return "UNKNOWN";
}


eGAMERULE_TYPE CWorld::GetRuleType()
{
	if (m_pTbldat)
	{
		return (eGAMERULE_TYPE)m_pTbldat->byWorldRuleType;
	}
	else
	{
		ERR_LOG(LOG_SYSTEM, "World[%u] Tbldat NULL", GetID());
	}

	return INVALID_GAMERULE;
}


CWorldCell* CWorld::GetWorldCell(CELLID cellID)
{
	if (m_paWorldCell)
	{
		if (cellID < m_dwTotalCount)
			return &m_paWorldCell[cellID];
	}

	return NULL;
}

CWorldCell* CWorld::GetWorldCellWithLoc(CNtlVector& rhs)
{
	CELLID cellID = GetCellID(rhs);
	if (cellID != INVALID_CELLID)
		return GetWorldCell(cellID);
	
	return NULL;
}

CELLID CWorld::GetCellID(CNtlVector& rhs)
{
	if (m_pTbldat)
	{
		DWORD dwX = (DWORD) floor(float(m_vStart.x - rhs.x) / m_pTbldat->fSplitSize);
		if (m_dwCellHorizontalCount > dwX)
		{
			DWORD dwZ = (DWORD)((DWORD) (m_vStart.z - rhs.z) / m_pTbldat->fSplitSize);
			if (m_dwCellVerticalCount > dwX)
				return dwX + m_dwCellHorizontalCount * dwZ;
		}
	}

	return INVALID_CELLID;
}

void CWorld::GetCellStartPos(CELLID cellID, CNtlVector* pvStart)
{
	if (m_pTbldat)
	{
		if (pvStart)
		{
			pvStart->x = m_vStart.x - (float(cellID % m_dwCellHorizontalCount) * m_pTbldat->fSplitSize);
			pvStart->z = m_vStart.z - (float(cellID / m_dwCellHorizontalCount) * m_pTbldat->fSplitSize);
		}
	}
}

void CWorld::GetCellEndPos(CELLID cellID, CNtlVector* pvEnd)
{
	if (m_pTbldat)
	{
		if (pvEnd)
		{
			pvEnd->x = (m_vStart.x - (float(cellID % m_dwCellHorizontalCount) * m_pTbldat->fSplitSize)) - m_pTbldat->fSplitSize;
			pvEnd->z = (m_vStart.z - (float(cellID / m_dwCellHorizontalCount) * m_pTbldat->fSplitSize)) - m_pTbldat->fSplitSize;
		}
	}
}

DWORD CWorld::GetCellHorizontalCount()
{
	return m_dwCellHorizontalCount;
}

DWORD CWorld::GetCellVerticalCount()
{
	return m_dwCellVerticalCount;
}

DWORD CWorld::GetCellTotalCount()
{
	return m_dwTotalCount;
}


float CWorld::GetSplitSize()
{
	return m_pTbldat->fSplitSize;
}


CWorldObjectList* CWorld::GetObjectList()
{
	return &m_objectList;
}


sWORLD_TBLDAT* CWorld::GetTbldat()
{
	return m_pTbldat;
}


CWorldZone* CWorld::FindWorldZone(ZONEID zoneId)
{
	ZONEMAP::iterator it = m_mapZone.find(zoneId);
	if (it != m_mapZone.end())
	{
		return it->second;
	}

	return NULL;
}


void* CWorld::GetNaviInstanceHandle()
{
	return m_hNaviInstance;
}


DWORD CWorld::GetZoneTotalCount()
{
	return (DWORD)m_mapZone.size();
}


CTriggerObject* CWorld::FindStaticObject(HOBJECT hObject)
{
	std::map<HOBJECT, CTriggerObject*>::iterator it = m_map_TriggerObj.find(hObject);
	if (m_map_TriggerObj.end() == it)
		return NULL;

	return it->second;
}

CTriggerObject* CWorld::FindStaticObjectByIdx(TBLIDX tblidx)
{
	for (std::map<HOBJECT, CTriggerObject*>::iterator it = m_map_TriggerObj.begin(); it != m_map_TriggerObj.end(); it++)
	{
		CTriggerObject* pObj = it->second;
		if (pObj->GetTblidx() == tblidx)
			return pObj;
	}

	return NULL;
}


CNpc* CWorld::FindNpc(TBLIDX tblidx)
{
	CNpc* pExistObject = (CNpc*)m_objectList.GetFirst(OBJTYPE_NPC);
	int nCount = m_objectList.GetObjCount(OBJTYPE_NPC);

	for(int i = 0; i < nCount; i++) //use for instead of "while" to avoid endless loop ;)
	{
		if ( pExistObject->GetTblidx() == tblidx)
			return pExistObject;

		pExistObject = (CNpc*)m_objectList.GetNext(pExistObject->GetWorldObjectLinker());
	}

	return NULL;
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
bool CWorld::AddScript(CScriptAlgoObject* pScript)
{
	if (pScript == NULL)
		return false;

	if (m_mapScript.find(pScript->GetScriptID()) != m_mapScript.end())
		return false;

	m_mapScript.insert({ pScript->GetScriptID(), pScript });

	return true;
}

//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CWorld::RemScript(DWORD dwScriptID)
{
	m_mapScript.erase(dwScriptID);
}

//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
CScriptAlgoObject* CWorld::GetScript(DWORD dwScriptID)
{
	std::map<DWORD, CScriptAlgoObject*>::iterator it = m_mapScript.find(dwScriptID);
	if (it != m_mapScript.end())
		return it->second;

	ERR_LOG(LOG_SCRIPT, "World %u does not have script %u", GetID(), dwScriptID);
	return NULL;
}

CScriptAlgoObject * CWorld::GetFirstScript(eSSD_SCRIPT_TYPE scripType)
{
	for (std::map<DWORD, CScriptAlgoObject*>::iterator it = m_mapScript.begin(); it != m_mapScript.end(); it++)
	{
		CScriptAlgoObject* pScript = it->second;
		if (pScript)
		{
			if (pScript->GetScriptType() == scripType)
				return pScript;
		}
	}

	return NULL;
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CWorld::AddScriptToPlayer(CPlayer* pPlayer)
{
	for (std::map<DWORD, CScriptAlgoObject*>::iterator it = m_mapScript.begin(); it != m_mapScript.end(); it++)
	{
		CScriptAlgoObject* pObj = it->second;
		if (pObj && !pObj->IsPaused())
		{
			pObj->AddPlayer(pPlayer->GetID());
			pPlayer->AddScript(pObj);
		}
	}
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CWorld::RemAllScript()
{
	if (m_mapScript.size() > 0)
	{
		ERR_LOG(LOG_GENERAL, "Remove all script from WorldIdx %u, scriptcount %I64u", GetID(), m_mapScript.size());
		for (std::map<DWORD, CScriptAlgoObject*>::iterator it = m_mapScript.begin(); it != m_mapScript.end(); it++)
		{
			ERR_LOG(LOG_GENERAL, "Script Idx %u", it->first);
			CScriptAlgoObject* pScript = it->second;
			if (pScript)
			{
				pScript->Stop(false, false); //1. false so we dont enter fail stage. 2. false so we dont remove script from m_mapScript inside function
			}
		}

		m_mapScript.clear();
	}
}


bool CWorld::CreateZoneMap(CWorldZoneTable *pWorldZoneTable)
{
	for (CTable::TABLEIT iterTable = pWorldZoneTable->Begin(); iterTable != pWorldZoneTable->End(); iterTable++)
	{
		sWORLD_ZONE_TBLDAT* pWorldZoneData = (sWORLD_ZONE_TBLDAT*)(iterTable->second);

		if (GetIdx() == pWorldZoneData->worldTblidx)
		{
			CWorldZone* pWorldZone = CreateWorldZone(pWorldZoneData, this);
			if (!pWorldZone)
			{
				ERR_LOG(LOG_SYSTEM, "CreateWorldZone() failed.(NULL == pWorldZone)");
				return false;
			}

			if (m_mapZone.insert(ZONEVAL(pWorldZone->GetZoneId(), pWorldZone)).second == false)
			{
				ERR_LOG(LOG_SYSTEM, "Duplicated CWorldZone created. pWorldZone->GetZoneId() = %u", pWorldZone->GetZoneId());
				delete pWorldZone;
				return false;
			}
		//	printf("create zone %u world %u \n", pWorldZone->GetZoneId(), GetIdx());
		}
	}

	//ERR_LOG(LOG_SYSTEM, "Create Zone for worldidx %u failed ", GetIdx());
	return true;
}


void CWorld::CreateWorldCellArray(DWORD dwTotalCount)
{
	m_paWorldCell = new CWorldCell[dwTotalCount];
}

void CWorld::DestroyWorldCellArray()
{
	SAFE_DELETE_ARRAY(m_paWorldCell);
}


CWorldZone* CWorld::CreateWorldZone(sWORLD_ZONE_TBLDAT *pWorldZoneData, CWorld *pWorldRef)
{
	CWorldZone* pWorldZone = new CWorldZone;
	if (pWorldZone->Create(pWorldZoneData, pWorldRef))
		return pWorldZone;

	ERR_LOG(LOG_SYSTEM, "WorldZone creation fialed.(false == pWorldZone->Create(pWorldZoneData, pWorldRef)), zoneId = %u", pWorldZoneData->tblidx);

	return NULL;
}


bool CWorld::BroadcastPacket(CNtlPacket *pPacket, CSpawnObject *pSubject, float fRange)
{
	bool bIsPacketSent = false;
	int nObjCount = m_objectList.GetObjCount(OBJTYPE_PC);
	CSpawnObject* pExistObject = m_objectList.GetFirst(OBJTYPE_PC);

	int n = 0;
	for(int i = 0; i < nObjCount; i++) //use for instead of "while" to avoid endless loop ;)
	{
		if (!pSubject || (pExistObject && pSubject->IsInRange(pExistObject->GetCurLoc(), fRange)) )
		{
			pExistObject->SendPacket(pPacket);
			bIsPacketSent = true;
		}

		if (++n >= MAX_PC)
		{
			ERR_LOG(LOG_GENERAL, "ERROR !! ENDLESS LOOP.");
			break;
		}

		pExistObject = m_objectList.GetNext(pExistObject->GetWorldObjectLinker());
	}

	return bIsPacketSent;
}


bool CWorld::BroadcastPacket(CNtlPacket *pPacket, CSpawnObject *pExcept)
{
	bool bIsPacketSent = false;
	int nObjCount = m_objectList.GetObjCount(OBJTYPE_PC);
	CSpawnObject* pExistObject = m_objectList.GetFirst(OBJTYPE_PC);
	CSpawnObject* pNextObj = NULL;

	int n = 0;
	for (int i = 0; i < nObjCount; i++) //use for instead of "while" to avoid endless loop ;)
	{
		if (pExistObject == NULL/* || !pExistObject->IsInitialized()*/) //todo : find out how it can happen that this is NULL or not initialized.
		{
			ERR_LOG(LOG_GENERAL, "Cant continue sending packet because Player object is NULL. WorldTblidx %u, WorldID %u, Playercount %u, CurLoop %i", GetIdx(), GetID(), nObjCount, i);
			break;
		}

		pNextObj = m_objectList.GetNext(pExistObject->GetWorldObjectLinker());

		if (pExistObject && pExistObject != pExcept)
		{
			pExistObject->SendPacket(pPacket);
			bIsPacketSent = true;
		}

		if (++n >= MAX_PC)
		{
			ERR_LOG(LOG_GENERAL, "ERROR !! ENDLESS LOOP.");
			break;
		}

		pExistObject = pNextObj;
	}

	return bIsPacketSent;
}

