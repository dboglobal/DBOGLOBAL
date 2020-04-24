#include "stdafx.h"
#include "WorldManager.h"
#include "World.h"
#include "WorldFactory.h"
#include "TableContainerManager.h"
#include "char.h"
#include "NtlNavi.h"
#include "GameServer.h"


CWorldManager::CWorldManager()
{
	Init();
}

CWorldManager::~CWorldManager()
{
	Destroy();
}


int CWorldManager::Create(CWorldFactory *pWorldFactory, CWorldTable *pWorldTable, CWorldZoneTable *pWorldZoneTable)
{
	m_pWorldFactory = pWorldFactory;

	int nResult = NTL_SUCCESS;



	for (CTable::TABLEIT iterTable = pWorldTable->Begin(); iterTable != pWorldTable->End(); iterTable++)
	{
		sWORLD_TBLDAT* pWorldData = (sWORLD_TBLDAT*)(iterTable->second);

		if (pWorldData->tblidx == 430000) //dont bother creating the DBO_TEST world.
			continue;

		if (pWorldData->bDynamic)
		{
			if (pWorldData->tblidx < DYNAMIC_WORLD_SEPARATOR)
			{
				ERR_LOG(LOG_SYSTEM, "pTbldat->tblidx[%u] < DYNAMIC_WORLD_SEPARATOR[%u]", pWorldData->tblidx, DYNAMIC_WORLD_SEPARATOR);
				break;
			}
			nResult = CreateDynamicWorld(pWorldData, pWorldZoneTable);
		}
		else
		{
			nResult = CreateStaticWorld(pWorldData, pWorldZoneTable);
		}

		if (nResult > NTL_SUCCESS)
		{
			ERR_LOG(LOG_SYSTEM, "NTL_SUCCESS != nResult[%u]", nResult);
			break;
		}

		if (!m_pDefaultWorld)
		{
			m_pDefaultWorld = m_worldIdxList.GetFirst();
			if (m_pDefaultWorld)
			{
				m_defaultWorldID = m_pDefaultWorld->GetID();
				m_defaultWorldTblidx = m_pDefaultWorld->GetIdx();
				m_vDefaultWorldLoc.operator=(pWorldData->vDefaultLoc);
				m_vDefaultWorldDir.operator=(pWorldData->vDefaultDir);
			}
			else
			{
				ERR_LOG(LOG_SYSTEM, "Can't find Default World Tblidx[%u]", pWorldData->tblidx);
				break;
			}
		}
	}

	return nResult;
}


void CWorldManager::Destroy()
{
	for (CWorld* pWorld = m_worldList.GetFirst(); pWorld; pWorld = m_worldList.GetFirst())
	{
		m_worldList.RemoveWorld(pWorld);
		m_pWorldFactory->Destroy(pWorld);
	}

	m_defaultWorldID = INVALID_WORLDID;
	m_defaultWorldTblidx = INVALID_TBLIDX;
}


CWorld* CWorldManager::CreateWorld(WORLDID worldID, sWORLD_TBLDAT *pTbldat)
{
	CWorld* pWorld = FindWorld(worldID);
	if (pWorld && MakeWorld(pTbldat) != NULL)
		pWorld->OnCreate();
	else
	{
		ERR_LOG(LOG_SYSTEM, "pWorld == NULL. worldID[%u] worldidx[%u]", worldID, pTbldat->tblidx);
	}

	return pWorld;
}

CWorld* CWorldManager::CreateWorld(sWORLD_TBLDAT *pTbldat)
{
	CWorld* pWorld = MakeWorld(pTbldat);
	if (pWorld)
	{
		pWorld->OnCreate();
	}
	else
	{
		ERR_LOG(LOG_SYSTEM, "pWorld == NULL. worldidx[%u]", pTbldat->tblidx);
	}

	return pWorld;
}


void CWorldManager::DestroyWorld(CWorld *pWorld)
{
	if (!pWorld)
	{
		ERR_LOG(LOG_SYSTEM, "pWorld == NULL");
	}

	pWorld->OnDestroy();
	m_worldIdxList.RemoveWorld(pWorld);
	m_worldList.RemoveWorld(pWorld);
	m_worldPool.PushWorld(pWorld->GetIdx(), pWorld);
}


void CWorldManager::TickProcess(DWORD dwTickDiff, float fMultiple)
{
	for (CWorld* pWorld = m_worldList.GetFirst(); pWorld; pWorld = m_worldList.GetNext())
		pWorld->TickProcess(dwTickDiff, fMultiple);
}


int CWorldManager::EnterObject(CSpawnObject *pSpawnObject, WORLDID worldID, bool bIsServerStart/* = false*/)
{
	CWorld* pWorld = FindWorld(worldID);
	if (pWorld)
	{
		return pWorld->Enter(pSpawnObject, bIsServerStart);
	}

	return 100001;
}

int CWorldManager::LeaveObject(CSpawnObject *pSpawnObject)
{
	CWorld* pWorld = pSpawnObject->GetCurWorld();
	if (pWorld)
		return pWorld->Leave(pSpawnObject);

	return 100001;
}

int CWorldManager::ChangeWorld(CSpawnObject *pSpawnObject, WORLDID destID)
{
	CWorld* pWorld = pSpawnObject->GetCurWorld();
	if (pWorld)
	{
		if (LeaveObject(pSpawnObject) == NTL_SUCCESS)
		{
			return EnterObject(pSpawnObject, destID);
		}
	}

	return 100001;
}


bool CWorldManager::IsInBoundary(WORLDID worldId, CNtlVector& rLoc)
{
	CWorld* pWorld = FindWorld(worldId);
	if (pWorld)
	{
		return pWorld->IsInBoundary(rLoc);
	}

	return false;
}


CWorld* CWorldManager::FindWorld(WORLDID worldId)
{
	return m_worldList.FindWorld(worldId);
}

CWorld* CWorldManager::GetFirst()
{
	return m_worldList.GetFirst();
}

CWorld* CWorldManager::GetNext()
{
	return m_worldList.GetNext();
}


int CWorldManager::GetAvailableCount(TBLIDX tblidx)
{
	return m_worldPool.GetCount(tblidx);
}


int CWorldManager::GetTotalWorldCount()
{
	return m_worldPool.GetCount() + m_worldList.GetCount();
}


void CWorldManager::GetZoneAndCellTotalCount(DWORD *dwZoneCount, DWORD *dwCellCount)
{
	m_worldPool.GetZoneAndCellTotalCount(dwZoneCount, dwCellCount);
}


CWorld* CWorldManager::GetDefaultWorld()
{
	if (m_defaultWorldID == INVALID_WORLDID)
		return NULL;

	return m_pDefaultWorld;
}


CWorld* CWorldManager::MakeWorld(sWORLD_TBLDAT *pTbldat)
{
	CWorld* pWorld = m_worldPool.PopWorld(pTbldat->tblidx);
	if (pWorld)
	{
	//	printf("PopWorld tblidx %u success. m_worldPool GetCount = %u \n", pTbldat->tblidx, m_worldPool.GetCount(pTbldat->tblidx));
		if (m_worldList.AddWorld(pWorld))
		{
			if (m_worldIdxList.AddWorld(pWorld))
				return pWorld;
		}
		else
		{
			m_worldPool.PushWorld(pTbldat->tblidx, pWorld);
		}
	}

	return NULL;
}

CWorld* CWorldManager::MakeWorld(sWORLD_TBLDAT *pTbldat, WORLDID worldID)
{
	CWorld* pWorld = m_worldPool.PopWorld(pTbldat->tblidx, worldID);
	if (pWorld)
	{
		if (m_worldList.AddWorld(pWorld))
		{
			if (m_worldIdxList.AddWorld(pWorld))
				return pWorld;
		}
		else
		{
			m_worldPool.PushWorld(pTbldat->tblidx, pWorld);
		}
	}

	return NULL;
}


void CWorldManager::Init()
{
	this->m_pWorldFactory = NULL;
	this->m_pDefaultWorld = NULL;
	this->m_defaultWorldID = INVALID_WORLDID;
	this->m_defaultWorldTblidx = INVALID_TBLIDX;
}


int CWorldManager::CreateStaticWorld(sWORLD_TBLDAT *pTbldat, CWorldZoneTable *pWorldZoneTable)
{
	int nResult = NTL_SUCCESS;

	CWorld* pWorld = m_pWorldFactory->Create();
	if (pWorld)
	{
		nResult = pWorld->Create(pTbldat->tblidx, pTbldat, pWorldZoneTable);
		if (nResult != NTL_SUCCESS)
			return nResult;

		if (m_worldList.AddWorld(pWorld))
		{
			if (m_worldIdxList.AddWorld(pWorld))
			{
				pWorld->OnPreCreate();
				return NTL_SUCCESS;
			}
			else nResult = 100001;
		}
		else
		{
			nResult = 100001;
		}
	}
	else nResult = 100003;

	return nResult;
}


int CWorldManager::CreateDynamicWorld(sWORLD_TBLDAT *pTbldat, CWorldZoneTable *pWorldZoneTable)
{
	int nResult = NTL_SUCCESS;

	if (pTbldat->nCreateCount <= MAX_DYNAMIC_WORLD_CREATE_COUNT)
	{
		for (int i = 0; i < pTbldat->nCreateCount; i++)
		{
			if (i > 50)
				break;

			CWorld* pWorld = m_pWorldFactory->Create();
			if (!pWorld)
			{
				ERR_LOG(LOG_SYSTEM, "CWorld Memory Alloc Fail");
				return 100003;
			}

			WORLDID worldID = i + pTbldat->tblidx;
			nResult = pWorld->Create(worldID, pTbldat, pWorldZoneTable);
			if (nResult != NTL_SUCCESS)
			{
				printf("Fail : pWorld->Create( worldID[%u], pTbldat[%u] )", worldID, pTbldat->tblidx);
				m_pWorldFactory->Destroy(pWorld);
				return nResult;
			}

			if (!m_worldPool.PushWorld(pTbldat->tblidx, pWorld))
			{
				printf("Fail : m_worldPool.PushWorld( pTbldat->tblidx[%u], pWorld[%u] )", pTbldat->tblidx, worldID);
				nResult = 100001;
				m_pWorldFactory->Destroy(pWorld);
				break;
			}

			pWorld->OnPreCreate();
		//	break;
		}
	}
	else
	{
		printf("World[%u] Table Dynamic CreateCount[%u] > MAX_DYNAMIC_WORLD_CREATE_COUNT[%u]", pTbldat->tblidx, pTbldat->nCreateCount, MAX_DYNAMIC_WORLD_CREATE_COUNT);
		nResult = 100001;
	}

	return nResult;
}

float CWorldManager::GetAdjustedHeight(WORLDID worldId, float fX, float fY, float fZ, int nVertRange)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (app->m_config.m_bEnableNavigator)
	{
		CWorld* pWorld = FindWorld(worldId);
		if (pWorld == NULL)
			return fY;

		if (GetNaviEngine()->IsPathDataLoaded(pWorld->GetTbldat()->dwWorldResourceID))
		{
			float fNewY;
			long lHeight = GetNaviEngine()->GetFastHeight(pWorld->GetNaviInstanceHandle(), fX, fY, fZ, fNewY, nVertRange);

			if (lHeight == 0xFFFFFFFF)
			{
				ERR_LOG(LOG_USER, "Any object can't spawn at this point., worldId = %u, fX = %f, fY = %f, fZ = %f", worldId, fX, fY, fZ);
				return fY;
			}

			return fNewY;
		}
	}

	return fY;
}

bool CWorldManager::GetDestLocAfterCollision(CWorld * pCurWorld, CCharacter * pChar, CNtlVector & rvShift, CNtlVector & rvDestLoc, CNtlVector & rvNewDir)
{
	if (pCurWorld && pChar)
	{
		if (rvShift.x != 0.0f && rvShift.z != 0.0f)
		{
			CNtlVector vDestLoc(pChar->GetCurLoc() + rvShift);

			if (GetNaviEngine()->IsPathDataLoaded(pCurWorld->GetTbldat()->dwWorldResourceID))
			{
				vDestLoc.y = GetAdjustedHeight(pCurWorld->GetID(), vDestLoc.x, vDestLoc.y, vDestLoc.z, 5000);

				sNAVI_POS sNaviCurLoc(pChar->GetCurLoc().x, pChar->GetCurLoc().y, pChar->GetCurLoc().z);
				sNAVI_POS sNaviDestLoc(vDestLoc.x, vDestLoc.y, vDestLoc.z);
				sNAVI_POS sFirstCollisionPos;

				float fAgentRadius = Dbo_ConvertToAgentRadius(pChar->GetObjectRadius());

			//	CSimplePerfomanceChecker::CSimplePerfomanceChecker(&v44);
			//	CSimplePerfomanceChecker::Start(&v44)

				eCOL_TEST_RESULT eResult = GetNaviEngine()->FastFirstCollisionTest(pCurWorld->GetNaviInstanceHandle(), fAgentRadius, sNaviCurLoc, sNaviDestLoc, sFirstCollisionPos);

				switch (eResult)
				{
					case eCOL_TEST_RESULT_COL:
					case eCOL_TEST_RESULT_NO_COL:
					{
						if (eResult == eCOL_TEST_RESULT_COL)
						{
							rvDestLoc.CopyFrom(sFirstCollisionPos.x, sFirstCollisionPos.y, sFirstCollisionPos.z);
						}
						else
						{
							float fNewY = 0.0f;
							long lHeight = GetNaviEngine()->GetFastHeight(pCurWorld->GetNaviInstanceHandle(), vDestLoc.x, vDestLoc.x, vDestLoc.z, fNewY, 5000);

							if (lHeight == 0xFFFFFFFF)
							{
								return false;
							}

							rvDestLoc.x = vDestLoc.x;
							rvDestLoc.y = fNewY;
							rvDestLoc.z = vDestLoc.z;
						}

						rvNewDir = rvShift;

					//	rvNewDir -= v49; //TODO

						if (rvNewDir.SafeNormalize() == false)
						{
							rvNewDir = pChar->GetCurDir();
						}

						return true;
					}
					break;

					default: return false;
				}
			}
			else
			{
				rvDestLoc.operator=(vDestLoc);
				rvNewDir.operator=(rvShift);

				if (!rvNewDir.SafeNormalize())
					rvNewDir.operator=(pChar->GetCurDir());
			}
		}
		else
		{
			rvDestLoc.operator=(pChar->GetCurLoc());
			rvNewDir.operator=(pChar->GetCurDir());
		}

		return true;
	}
	else
		ERR_LOG(LOG_GENERAL, "pCurWorld = %016x, pChar = %016x", pCurWorld, pChar);

	return false;
}

void CWorldManager::GetTextMemoryUseInfo(char * buff, int nBuffSize)
{
	/*int totalNpc;
	int nSuPetCount;
	int nPetSize;
	int nNpcCount;
	int nSuPetSize;
	int totalMob;
	int nPcCount;
	int totalSuPet;
	int nNpcSize;
	int totalPet;
	int nMobCount;
	int nPcSize;
	int totalPc;
	int nPetCount;
	int nMobSize;

	sprintf_s(buff, nBuffSize, "WORLD,%d,%d,%I64d,ZONE,%I64d,%d,%I64d,CELL,%I64d,%d,%I64d", v17);*/
}



