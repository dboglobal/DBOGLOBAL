#include "precomp_ntlpresentation.h"

#include "NtlSpawnMergeManager.h"

CNtlSpawnMergeContainer::CNtlSpawnMergeContainer()
{
	m_eSpawnType	= -1;

	m_dwPartyIdx	= 0;
	m_iPartyColorIdx= 0;
}

CNtlSpawnMergeContainer::~CNtlSpawnMergeContainer()
{
}

void CNtlSpawnMergeContainer::Free()
{
	m_eSpawnType	= -1;

	m_dwPartyIdx	= 0;
	m_iPartyColorIdx= 0;

	for (SPAWN_VEC_ITER it = m_vecSpawn.begin(); it != m_vecSpawn.end(); ++it)
	{
		CMergeSpawn* pSpawn = *it;
		NTL_DELETE(pSpawn);
	}
	m_vecSpawn.clear();

	for (SPAWN_PARTY_ITER it = m_mapSpawnParty.begin(); it != m_mapSpawnParty.end(); ++it)
	{
		CMergeSpawnParty* pSpawnParty = it->second;
		NTL_DELETE(pSpawnParty);
	}
	m_mapSpawnParty.clear();
}

RwBool CNtlSpawnMergeContainer::Load(const char* pFilename)
{
	Free();

	FILE* pFile;
	if(::fopen_s(&pFile, pFilename, "rb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << pFilename << ")");
		return FALSE;
	}

	Load(pFile);

	fclose(pFile);

	return TRUE;
}

RwBool CNtlSpawnMergeContainer::Save(const char* pFilename)
{
	FILE* pFile;
	if(::fopen_s(&pFile, pFilename, "wb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << pFilename << ")");
		return FALSE;
	}

	Save(pFile);

	fclose(pFile);

	return TRUE;
}

RwBool CNtlSpawnMergeContainer::Load(FILE* pFile)
{
	LoadSpawnType(pFile);
	LoadSpawnParty(pFile);
	LoadSpawnObject(pFile);

	return TRUE;
}

RwBool CNtlSpawnMergeContainer::Save(FILE* pFile)
{
	SaveSpawnType(pFile);
	SaveSpawnParty(pFile);
	SaveSpawnObject(pFile);

	return TRUE;
}

RwBool CNtlSpawnMergeContainer::LoadSpawnType(FILE* pFile)
{
	fread(&m_eSpawnType, sizeof(RwInt32), 1, pFile);

	return TRUE;
}

RwBool CNtlSpawnMergeContainer::SaveSpawnType(FILE* pFile)
{
	fwrite(&m_eSpawnType, sizeof(RwInt32), 1, pFile);

	return TRUE;
}

RwBool CNtlSpawnMergeContainer::LoadSpawnParty(FILE* pFile)
{
	fread(&m_dwPartyIdx, sizeof(DWORD), 1, pFile);
	fread(&m_iPartyColorIdx, sizeof(RwInt32), 1, pFile);

	RwInt32	iNum = 0;
	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CMergeSpawnParty* pSpawnParty = NTL_NEW CMergeSpawnParty;
		pSpawnParty->Load(pFile);
		DBO_ASSERT(InsertSpawnParty(pSpawnParty), "merge spawn party insert failed.");

		// 혹시나 모르는 상황을 예외처리 하자.
		if (pSpawnParty->dwPartyIdx > m_dwPartyIdx)
		{
			m_dwPartyIdx = pSpawnParty->dwPartyIdx;
		}
	}

	return TRUE;
}

RwBool CNtlSpawnMergeContainer::SaveSpawnParty(FILE* pFile)
{
	fwrite(&m_dwPartyIdx, sizeof(DWORD), 1, pFile);
	fwrite(&m_iPartyColorIdx, sizeof(RwInt32), 1, pFile);

	RwInt32	iNum = m_mapSpawnParty.size();
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (SPAWN_PARTY_ITER it = m_mapSpawnParty.begin(); it != m_mapSpawnParty.end(); ++it)
	{
		CMergeSpawnParty* pSpawnParty = it->second;

		pSpawnParty->Save(pFile);
	}

	return TRUE;
}

RwBool CNtlSpawnMergeContainer::LoadSpawnObject(FILE* pFile)
{
	RwInt32 iNum;
	fread(&iNum, sizeof(RwInt32), 1, pFile);

	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CMergeSpawn* pSpawn = NTL_NEW CMergeSpawn;
		pSpawn->Load(pFile);

		InsertSpawn(pSpawn);

		if (pSpawn->iPartyIndex != -1)
		{
			CMergeSpawnParty* pSpawnParty = GetSpawnParty(pSpawn->iPartyIndex);
			if (pSpawnParty)
			{
				pSpawnParty->vecSpawn.push_back(pSpawn);
			}
		}
	}

	return TRUE;
}

RwBool CNtlSpawnMergeContainer::SaveSpawnObject(FILE* pFile)
{
	RwInt32 iNum = m_vecSpawn.size();
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);

	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CMergeSpawn* pSpawn = m_vecSpawn.at(i);
		
		pSpawn->Save(pFile);
	}

	return TRUE;
}

CMergeSpawnParty* CNtlSpawnMergeContainer::GetSpawnParty(DWORD dwIdx)
{
	SPAWN_PARTY_ITER it = m_mapSpawnParty.find(dwIdx);
	if (it != m_mapSpawnParty.end())
	{
		return it->second;
	}

	return NULL;
}

RwBool CNtlSpawnMergeContainer::InsertSpawnParty(CMergeSpawnParty* pSpawnParty)
{
	SPAWN_PARTY_ITER it = m_mapSpawnParty.find(pSpawnParty->dwPartyIdx);
	if (it != m_mapSpawnParty.end())
	{
		return FALSE;
	}

	m_mapSpawnParty.insert(SPAWN_PARTY_VALUE(pSpawnParty->dwPartyIdx, pSpawnParty));

	return TRUE;
}

RwBool CNtlSpawnMergeContainer::RemoveSpawnParty(CMergeSpawnParty* pSpawnParty)
{
	SPAWN_PARTY_ITER it = m_mapSpawnParty.find(pSpawnParty->dwPartyIdx);
	if (it != m_mapSpawnParty.end())
	{
		m_mapSpawnParty.erase(it);

		return TRUE;
	}

	return FALSE;
}

RwBool CNtlSpawnMergeContainer::InsertSpawn(CMergeSpawn* pSpawn)
{
	m_vecSpawn.push_back(pSpawn);

	return TRUE;
}

RwBool CNtlSpawnMergeContainer::RemoveSpawn(CMergeSpawn* pSpawn)
{
	SPAWN_VEC_ITER it = find(m_vecSpawn.begin(), m_vecSpawn.end(), pSpawn);
	if (it != m_vecSpawn.end())
	{
		m_vecSpawn.erase(it);

		return TRUE;
	}

	return FALSE;
}

DWORD CNtlSpawnMergeContainer::IndexingSpawnParty(DWORD dwStartIdx)
{
	SPAWN_PARTY_MAP mapSpawnParty = m_mapSpawnParty;

	m_mapSpawnParty.clear();

	
	for (SPAWN_PARTY_ITER it = mapSpawnParty.begin(); it != mapSpawnParty.end(); ++it)
	{
		CMergeSpawnParty* pSpawnParty = it->second;

		pSpawnParty->dwPartyIdx = dwStartIdx++;

		for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
		{
			CMergeSpawn* pSpawn = *itSpawn;

			pSpawn->iPartyIndex = pSpawnParty->dwPartyIdx;
		}

		InsertSpawnParty(pSpawnParty);
	}
	
	return m_dwPartyIdx = dwStartIdx;
}

RwBool CNtlSpawnMergeContainer::MergePathUnlinkedSpawn(RwBBox* pBBoxDst, RwBBox* pBBoxSrc, CNtlSpawnMergeContainer* pContainerSrc, FILE* pLogFile)
{
	if (m_eSpawnType != pContainerSrc->m_eSpawnType)
	{
		return FALSE;
	}
	
	// 삭제
	{
		// Party Delete
		SPAWN_PARTY_ITER itSpawnParty = m_mapSpawnParty.begin();
		while (itSpawnParty != m_mapSpawnParty.end())
		{
			CMergeSpawnParty* pSpawnParty = itSpawnParty->second;

			RwInt32	iInnerCnt	= 0;
			RwBool	bInner		= TRUE;
			RwBool	bPath		= FALSE;
			for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
			{
				CMergeSpawn* pSpawn = *itSpawn;

				if (pSpawn->vT.x < pBBoxDst->inf.x || pSpawn->vT.x >= pBBoxDst->sup.x ||
					pSpawn->vT.z < pBBoxDst->inf.z || pSpawn->vT.z >= pBBoxDst->sup.z)
				{
					bInner = FALSE;
				}
				else
				{
					++iInnerCnt;
				}
				if (pSpawn->iNestPathIndex != -999)
				{
					bPath = TRUE;
				}
			}

			if (bInner && !bPath)
			{			
				for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
				{
					CMergeSpawn* pSpawn = *itSpawn;

					// 속도를 위해 여기서 삭제 하지 않고 NonParty를 처리 할때 지우자.
					pSpawn->uiMergeTemp = dSPAWN_MERGE_TEMP_FLAG_SPAWN_POST_DELETE;
				}

				itSpawnParty = m_mapSpawnParty.erase(itSpawnParty);
				NTL_DELETE(pSpawnParty);
			}			
			else
			{
				// 로그 출력
				if (iInnerCnt > 0 && !bPath)
				{
					for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
					{
						CMergeSpawn* pSpawn = *itSpawn;

						fprintf_s(pLogFile, "[can not party merge-delete] Mob_Tblidx(%u) Spawn_Loc(%.2f, %.2f, %.2f)\n",
							pSpawn->dwMdlGUID, pSpawn->vT.x, pSpawn->vT.y, pSpawn->vT.z);
					}
				}
				else if (iInnerCnt > 0 && bPath)
				{
					for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
					{
						CMergeSpawn* pSpawn = *itSpawn;

						fprintf_s(pLogFile, "[can not party merge-delete] Mob_Tblidx(%u) Spawn_Loc(%.2f, %.2f, %.2f) Path_Index(%d) Spawn_Move_Type(%d)\n",
							pSpawn->dwMdlGUID, pSpawn->vT.x, pSpawn->vT.y, pSpawn->vT.z, pSpawn->iNestPathIndex, pSpawn->SpawnMacro.m_MoveType);
					}
				}
				++itSpawnParty;
			}
		}

		// Non Party Delete
		SPAWN_VEC_ITER itSpawn = m_vecSpawn.begin();
		while (itSpawn != m_vecSpawn.end())
		{
			CMergeSpawn* pSpawn = *itSpawn;

			// Party Check 시 삭제된 Spawn
			if (pSpawn->uiMergeTemp == dSPAWN_MERGE_TEMP_FLAG_SPAWN_POST_DELETE)
			{
				itSpawn = m_vecSpawn.erase(itSpawn);
				NTL_DELETE(pSpawn);
			}
			else if (pSpawn->iPartyIndex != -1)
			{
				++itSpawn;
			}
			else
			{
				RwBool bInner	= TRUE;
				RwBool bPath	= FALSE;
				if (pSpawn->vT.x < pBBoxDst->inf.x || pSpawn->vT.x >= pBBoxDst->sup.x ||
					pSpawn->vT.z < pBBoxDst->inf.z || pSpawn->vT.z >= pBBoxDst->sup.z)
				{
					bInner = FALSE;
				}
				if (pSpawn->iNestPathIndex != -999)
				{
					bPath = TRUE;
				}

				if (bInner && !bPath)
				{
					itSpawn = m_vecSpawn.erase(itSpawn);
					NTL_DELETE(pSpawn);
				}
				else
				{
					++itSpawn;
				}
			}				
		}
	}


	// 추가 : Src의 Data를 Dst로 옮긴다. 복사할 필요가 있다면, 나중에 복사로 변경하자.
	{
		// Party Insert
		SPAWN_PARTY_ITER itSpawnParty = pContainerSrc->m_mapSpawnParty.begin();
		while (itSpawnParty != pContainerSrc->m_mapSpawnParty.end())
		{
			CMergeSpawnParty* pSpawnParty = itSpawnParty->second;

			RwInt32	iInnerCnt	= 0;
			RwBool	bInner		= TRUE;
			RwBool	bPath		= FALSE;
			for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
			{
				CMergeSpawn* pSpawn = *itSpawn;

				if (pSpawn->vT.x < pBBoxSrc->inf.x || pSpawn->vT.x >= pBBoxSrc->sup.x ||
					pSpawn->vT.z < pBBoxSrc->inf.z || pSpawn->vT.z >= pBBoxSrc->sup.z)
				{
					bInner = FALSE;
				}
				else
				{
					++iInnerCnt;
				}
				if (pSpawn->iNestPathIndex != -999)
				{
					bPath = TRUE;
				}
			}

			if (bInner && !bPath)
			{			
				DWORD dwPartyIdxOld = pSpawnParty->dwPartyIdx;

				pSpawnParty->dwCurPartyIdx = pSpawnParty->dwPartyIdx = ++m_dwPartyIdx;

				// 로그 출력
				fprintf_s(pLogFile, "[path is not linked spawn-insert] Party_Old_Index(%u) Party_New_Index(%u)\n", dwPartyIdxOld, pSpawnParty->dwPartyIdx);
				
				for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
				{
					CMergeSpawn* pSpawn = *itSpawn;

					pSpawn->iPartyIndex = pSpawnParty->dwPartyIdx;

					// 속도를 위해 여기서 삽입 하지 않고 NonParty를 처리 할때 삽입 하자.
					pSpawn->uiMergeTemp = dSPAWN_MERGE_TEMP_FLAG_SPAWN_POST_INSERT;
				}

				itSpawnParty = pContainerSrc->m_mapSpawnParty.erase(itSpawnParty);
				m_mapSpawnParty.insert(SPAWN_PARTY_VALUE(pSpawnParty->dwPartyIdx, pSpawnParty));

			}			
			else
			{
				// 로그 출력
				if (iInnerCnt > 0 && !bPath)
				{
					for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
					{
						CMergeSpawn* pSpawn = *itSpawn;

						fprintf_s(pLogFile, "[can not party merge-insert] Mob_Tblidx(%u) Spawn_Loc(%.2f, %.2f, %.2f)\n", pSpawn->dwMdlGUID, pSpawn->vT.x, pSpawn->vT.y, pSpawn->vT.z);
					}
				}
				else if (iInnerCnt > 0 && bPath)
				{
					for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
					{
						CMergeSpawn* pSpawn = *itSpawn;

						fprintf_s(pLogFile, "[can not party merge-insert] Mob_Tblidx(%u) Spawn_Loc(%.2f, %.2f, %.2f) Path_Index(%d) Spawn_Move_Type(%d)\n",
							pSpawn->dwMdlGUID, pSpawn->vT.x, pSpawn->vT.y, pSpawn->vT.z, pSpawn->iNestPathIndex, pSpawn->SpawnMacro.m_MoveType);
					}
				}

				++itSpawnParty;
			}
		}

		// Non Party Insert
		SPAWN_VEC_ITER itSpawn = pContainerSrc->m_vecSpawn.begin();
		while (itSpawn != pContainerSrc->m_vecSpawn.end())
		{
			CMergeSpawn* pSpawn = *itSpawn;

			// Party Check 시 삭제된 Spawn
			if (pSpawn->uiMergeTemp == dSPAWN_MERGE_TEMP_FLAG_SPAWN_POST_INSERT)
			{
				itSpawn = pContainerSrc->m_vecSpawn.erase(itSpawn);
				m_vecSpawn.push_back(pSpawn);
			}
			else if (pSpawn->iPartyIndex != -1)
			{
				++itSpawn;
			}
			else
			{
				RwBool bInner	= TRUE;
				RwBool bPath	= FALSE;
				if (pSpawn->vT.x < pBBoxSrc->inf.x || pSpawn->vT.x >= pBBoxSrc->sup.x ||
					pSpawn->vT.z < pBBoxSrc->inf.z || pSpawn->vT.z >= pBBoxSrc->sup.z)
				{
					bInner = FALSE;
				}
				if (pSpawn->iNestPathIndex != -999)
				{
					bPath = TRUE;
				}

				if (bInner && !bPath)
				{
					itSpawn = pContainerSrc->m_vecSpawn.erase(itSpawn);
					m_vecSpawn.push_back(pSpawn);

				}
				else
				{
					++itSpawn;
				}
			}				
		}
	}

	return TRUE;
}

RwBool CNtlSpawnMergeContainer::MergePathLinkedSpawn(std::vector<RwInt32>* pvecDeletePathIdxList, std::vector<RwInt32>* pvecInsertPathIdxList, CNtlSpawnMergeContainer* pContainerSrc, FILE* pLogFile)
{
	if (m_eSpawnType != pContainerSrc->m_eSpawnType)
	{
		return FALSE;
	}

	// 삭제
	{
		// Party Delete
		SPAWN_PARTY_ITER itSpawnParty = m_mapSpawnParty.begin();
		while (itSpawnParty != m_mapSpawnParty.end())
		{
			CMergeSpawnParty* pSpawnParty = itSpawnParty->second;

			RwBool bDeleteParty	= FALSE;
			for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
			{
				CMergeSpawn* pSpawn = *itSpawn;

				if (pSpawn->iNestPathIndex != -999 && pvecDeletePathIdxList->end() != find(pvecDeletePathIdxList->begin(), pvecDeletePathIdxList->end(), pSpawn->iNestPathIndex))
				{
					bDeleteParty = TRUE;
					break;
				}
			}

			if (bDeleteParty)
			{
				for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
				{
					CMergeSpawn* pSpawn = *itSpawn;

					// 속도를 위해 여기서 삭제 하지 않고 NonParty를 처리 할때 지우자.
					pSpawn->uiMergeTemp = dSPAWN_MERGE_TEMP_FLAG_SPAWN_POST_DELETE;
				}

				itSpawnParty = m_mapSpawnParty.erase(itSpawnParty);
				NTL_DELETE(pSpawnParty);
			}
			else
			{
				++itSpawnParty;
			}
		}

		// Non Party Delete
		SPAWN_VEC_ITER itSpawn = m_vecSpawn.begin();
		while (itSpawn != m_vecSpawn.end())
		{
			CMergeSpawn* pSpawn = *itSpawn;

			// Party Check 시 삭제된 Spawn
			if (pSpawn->uiMergeTemp == dSPAWN_MERGE_TEMP_FLAG_SPAWN_POST_DELETE)
			{
				itSpawn = m_vecSpawn.erase(itSpawn);
				NTL_DELETE(pSpawn);
			}
			else if (pSpawn->iPartyIndex != -1)
			{
				++itSpawn;
			}
			else
			{
				if (pSpawn->iNestPathIndex != -999 && pvecDeletePathIdxList->end() != find(pvecDeletePathIdxList->begin(), pvecDeletePathIdxList->end(), pSpawn->iNestPathIndex))
				{
					itSpawn = m_vecSpawn.erase(itSpawn);
					NTL_DELETE(pSpawn);
				}
				else
				{
					++itSpawn;
				}
			}				
		}
	}


	// 추가 : Src의 Data를 Dst로 옮긴다. 복사할 필요가 있다면, 나중에 복사로 변경하자.
	{
		// Party Insert
		SPAWN_PARTY_ITER itSpawnParty = pContainerSrc->m_mapSpawnParty.begin();
		while (itSpawnParty != pContainerSrc->m_mapSpawnParty.end())
		{
			CMergeSpawnParty* pSpawnParty = itSpawnParty->second;

			RwBool bInsertParty	= FALSE;
			for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
			{
				CMergeSpawn* pSpawn = *itSpawn;

				if (pvecInsertPathIdxList->end() != find(pvecInsertPathIdxList->begin(), pvecInsertPathIdxList->end(), pSpawn->iNestPathIndex))
				{
					bInsertParty = TRUE;
					break;
				}
			}

			if (bInsertParty)
			{
				DWORD dwPartyIdxOld = pSpawnParty->dwPartyIdx;
				
				pSpawnParty->dwCurPartyIdx = pSpawnParty->dwPartyIdx = ++m_dwPartyIdx;

				// 로그 출력
				fprintf_s(pLogFile, "[path is not linked spawn-insert] Party_Old_Index(%u) Party_New_Index(%u)\n", dwPartyIdxOld, pSpawnParty->dwPartyIdx);

				for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
				{
					CMergeSpawn* pSpawn = *itSpawn;

					pSpawn->iPartyIndex = pSpawnParty->dwPartyIdx;

					// 속도를 위해 여기서 삽입 하지 않고 NonParty를 처리 할때 삽입 하자.
					pSpawn->uiMergeTemp = dSPAWN_MERGE_TEMP_FLAG_SPAWN_POST_INSERT;
				}

				itSpawnParty = pContainerSrc->m_mapSpawnParty.erase(itSpawnParty);
				m_mapSpawnParty.insert(SPAWN_PARTY_VALUE(pSpawnParty->dwPartyIdx, pSpawnParty));

			}
			else
			{
				++itSpawnParty;
			}
		}

		// Non Party Insert
		SPAWN_VEC_ITER itSpawn = pContainerSrc->m_vecSpawn.begin();
		while (itSpawn != pContainerSrc->m_vecSpawn.end())
		{
			CMergeSpawn* pSpawn = *itSpawn;

			// Party Check 시 삭제된 Spawn
			if (pSpawn->uiMergeTemp == dSPAWN_MERGE_TEMP_FLAG_SPAWN_POST_INSERT)
			{
				itSpawn = pContainerSrc->m_vecSpawn.erase(itSpawn);
				m_vecSpawn.push_back(pSpawn);
			}
			else if (pSpawn->iPartyIndex != -1)
			{
				++itSpawn;
			}
			else
			{
				if (pvecInsertPathIdxList->end() != find(pvecInsertPathIdxList->begin(), pvecInsertPathIdxList->end(), pSpawn->iNestPathIndex))
				{
					itSpawn = pContainerSrc->m_vecSpawn.erase(itSpawn);
					m_vecSpawn.push_back(pSpawn);
				}
				else
				{
					++itSpawn;
				}
			}				
		}
	}

	return TRUE;
}

RwBool CNtlSpawnMergeContainer::GetPathIdxListFromPathLinkedSpawnParty(std::vector<RwInt32>* pvecPathIdxList, FILE* pLogFile)
{
	RwBool bResult = TRUE;

	// Party 에서 pvecPathIdxList와 동일한 Path를 가진 Spawn들을 골라낸다.
	SPAWN_PARTY_ITER itSpawnParty = m_mapSpawnParty.begin();
	while (itSpawnParty != m_mapSpawnParty.end())
	{
		CMergeSpawnParty* pSpawnParty = itSpawnParty->second;

		RwBool bPathTest = FALSE;
		for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
		{
			CMergeSpawn* pSpawn = *itSpawn;

			if (pSpawn->iNestPathIndex != -999)
			{
				if (pvecPathIdxList->end() != find(pvecPathIdxList->begin(), pvecPathIdxList->end(), pSpawn->iNestPathIndex))
				{
					bPathTest = TRUE;
					break;
				}
			}
		}

		if (bPathTest)
		{			
			for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
			{
				CMergeSpawn* pSpawn = *itSpawn;

				if (pSpawn->iNestPathIndex != -999)
				{
					if (pvecPathIdxList->end() == find(pvecPathIdxList->begin(), pvecPathIdxList->end(), pSpawn->iNestPathIndex))
					{
						pvecPathIdxList->push_back(pSpawn->iNestPathIndex);
						bResult = FALSE;
					}
				}
			}

			// 로그 출력
			if (!bResult)
			{
				for (SPAWN_VEC_ITER itSpawn = pSpawnParty->vecSpawn.begin(); itSpawn != pSpawnParty->vecSpawn.end(); ++itSpawn)
				{
					CMergeSpawn* pSpawn = *itSpawn;

					fprintf_s(pLogFile, "[recursive path spawn merge] Mob_Tblidx(%u) Spawn_Loc(%.2f, %.2f, %.2f) Path_Index(%d) Party_Leader_Able(%d) Spawn_Move_Type(%d)", 
						pSpawn->dwMdlGUID, pSpawn->vT.x, pSpawn->vT.y, pSpawn->vT.z, pSpawn->iNestPathIndex, (pSpawn->bPartyLeader ? 1 : 0), pSpawn->SpawnMacro.m_MoveType);
				}
			}
		}
		++itSpawnParty;
	}

	return bResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPathListMergeContainer::CNtlPathListMergeContainer()
{
}


CNtlPathListMergeContainer::~CNtlPathListMergeContainer()
{
}

void CNtlPathListMergeContainer::Free()
{
	for (PATH_LIST_ITER it = m_mapPathList.begin(); it != m_mapPathList.end(); ++it)
	{
		CPathList* pPathList = it->second;
		NTL_DELETE(pPathList);
	}
	m_mapPathList.clear();
}

RwBool CNtlPathListMergeContainer::Load(const char* pFilename)
{
	Free();

	FILE* pFile;
	if(::fopen_s(&pFile, pFilename, "rb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << pFilename << ")");
		return FALSE;
	}

	Load(pFile);

	fclose(pFile);

	return TRUE;
}

RwBool CNtlPathListMergeContainer::Save(const char* pFilename)
{
	FILE* pFile;
	if(::fopen_s(&pFile, pFilename, "wb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << pFilename << ")");
		return FALSE;
	}

	Save(pFile);

	fclose(pFile);

	return TRUE;
}

RwBool CNtlPathListMergeContainer::Load(FILE* pFile)
{
	unsigned int uiNum;
	fread(&uiNum, sizeof(unsigned int), 1, pFile);

	for(unsigned int i = 0; i < uiNum; ++i)
	{
		TBLIDX		tblIdx;
		float		fRadius;
		BYTE		byPointCnt;
		RwChar		acMemo[255];

		fread(&tblIdx, sizeof(TBLIDX), 1, pFile);
		fread(&fRadius, sizeof(float), 1, pFile);
		fread(&byPointCnt, sizeof(BYTE), 1, pFile);
		fread(acMemo, sizeof(RwChar) * 255, 1, pFile);

		CMergePathList* pPathList = NTL_NEW CMergePathList(tblIdx, fRadius, acMemo);
		pPathList->Load(pFile, byPointCnt);
		DBO_ASSERT(Insert(pPathList), "merge path insert failed.");
	}

	return TRUE;
}

RwBool CNtlPathListMergeContainer::Save(FILE* pFile)
{
	unsigned int uiNum = m_mapPathList.size();
	fwrite(&uiNum, sizeof(unsigned int), 1, pFile);

	for(PATH_LIST_ITER it = m_mapPathList.begin(); it != m_mapPathList.end(); ++it)
	{
		CMergePathList* pPathList = it->second;
		
		pPathList->Save(pFile, TRUE);
	}

	return TRUE;
}

RwBool CNtlPathListMergeContainer::Insert(CMergePathList* pPathList)
{
	PATH_LIST_ITER it = m_mapPathList.find(pPathList->GetUID());
	if (it != m_mapPathList.end())
	{
		return FALSE;
	}

	m_mapPathList.insert(PATH_LIST_VALUE(pPathList->GetUID(), pPathList));

	return TRUE;
}

RwBool CNtlPathListMergeContainer::Remove(CMergePathList* pPathList)
{
	PATH_LIST_ITER it = m_mapPathList.find(pPathList->GetUID());
	if (it != m_mapPathList.end())
	{
		m_mapPathList.erase(it);

		return TRUE;
	}

	return FALSE;
}

CMergePathList* CNtlPathListMergeContainer::GetPathList(RwInt32 iIdx)
{
	PATH_LIST_ITER it = m_mapPathList.find(iIdx);
	if (it != m_mapPathList.end())
	{
		return it->second;
	}

	return NULL;
}

RwBool CNtlPathListMergeContainer::MergePathList(std::vector<RwInt32>* pvecDeletePathIdxList, std::vector<RwInt32>* pvecInsertPathIdxList, CNtlPathListMergeContainer* pContainerSrc)
{
	// delete path
	{
		std::vector<RwInt32>::iterator itPathIdx = pvecDeletePathIdxList->begin();
		while (itPathIdx != pvecDeletePathIdxList->end())
		{
			RwInt32 iPathIdx = *itPathIdx;
			PATH_LIST_ITER itPathList = m_mapPathList.find(iPathIdx);
			if (itPathList != m_mapPathList.end())
			{
				CMergePathList* pPathList = itPathList->second;
				m_mapPathList.erase(itPathList);

				NTL_DELETE(pPathList);
			}
			++itPathIdx;
		}
	}

	// insert path
	{
		std::vector<RwInt32>::iterator itPathIdx = pvecInsertPathIdxList->begin();
		while (itPathIdx != pvecInsertPathIdxList->end())
		{
			RwInt32 iPathIdx = *itPathIdx;
			PATH_LIST_ITER itPathList = pContainerSrc->m_mapPathList.find(iPathIdx);
			if (itPathList != pContainerSrc->m_mapPathList.end())
			{
				CMergePathList* pPathList = itPathList->second;

				pContainerSrc->m_mapPathList.erase(itPathList);
				m_mapPathList.insert(PATH_LIST_VALUE(pPathList->GetUID(), pPathList));
			}
			++itPathIdx;
		}
	}

	return TRUE;
}