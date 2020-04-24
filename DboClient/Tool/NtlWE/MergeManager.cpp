#include "StdAfx.h"
#include "NtlWe.h"
#include "MergeManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"
#include "ntlworldsectorinfo.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

CMergeManager::CMergeManager(void)
{
}

CMergeManager::~CMergeManager(void)
{
}

CMergeManager* CMergeManager::GetInstance(void)
{
	static CMergeManager MergeManager;
	return &MergeManager;
}

FILE* CMergeManager::SetFilePosAtSector(RwV3d _WorldPos, eFILE_POS_AT_SECTOR _FilePosAtSector)
{
	CNtlWorldFieldManager*	pMgr		= dGETMGR();
	CNtlWorldField*			pField		= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwInt32					FieldIdx	= pMgr->GetFieldIdx(_WorldPos);
	RwInt32					SectorIdx	= pMgr->GetSectorIdx(_WorldPos);

	// outside world
	if(FieldIdx >= dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum || FieldIdx < 0)
	{
		return NULL;
	}

	// create a file pointer
	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("fields");

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	::_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		NTL_RETURN(FALSE);
	}

	// find the starting file position of the sector
	RwInt32	l, m;
	RwInt32	TmpSectorIdx;
	RwV3d	SectorSPos;
	int		CntLoad			= 0;
	RwV3d	SPos			= pField[FieldIdx].GetSPos();
	RwInt32 Cnt				= 0;

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x	= (RwReal)m;
			SectorSPos.z	= (RwReal)l;
			TmpSectorIdx	= pMgr->GetSectorIdx(SectorSPos);

			if(SectorIdx == TmpSectorIdx)
			{
				switch(_FilePosAtSector)
				{
				case eFPAS_HEIGHTFIELD:
					{
						GetNtlWorldSectorInfo()->SectorMaterialSkipToFile(pFile);

						::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

						return pFile;
					}
				case eFPAS_DOODADS:
					{
						GetNtlWorldSectorInfo()->SectorMeshSkipToFile(pFile);

						::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

						return pFile;
					}
				}
			}
			else
			{
				// skip the file position 'till the proper sector will be found
				GetNtlWorldSectorInfo()->OutdoorSectorSkipToFile(pFile);
			}
		}
	}

	::fclose(pFile);
	pFile = NULL;

	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

CMergeManager::CMergeManager(void)
{
}

CMergeManager::~CMergeManager(void)
{
}

CMergeManager* CMergeManager::GetInstance(void)
{
	static CMergeManager MergeManager;
	return &MergeManager;
}

FILE* CMergeManager::SetFilePosAtSector(RwV3d _WorldPos, eFILE_POS_AT_SECTOR _FilePosAtSector)
{
	CNtlWorldFieldManager*	pMgr		= dGETMGR();
	CNtlWorldField*			pField		= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwInt32					FieldIdx	= pMgr->GetFieldIdx(_WorldPos);
	RwInt32					SectorIdx	= pMgr->GetSectorIdx(_WorldPos);

	// outside world
	if(FieldIdx >= dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum || FieldIdx < 0)
	{
		return NULL;
	}

	// create a file pointer
	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("fields");

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	::_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		NTL_RETURN(FALSE);
	}

	// find the starting file position of the sector
	RwInt32	l, m;
	RwInt32	TmpSectorIdx;
	RwV3d	SectorSPos;
	int		CntLoad			= 0;
	RwV3d	SPos			= pField[FieldIdx].GetSPos();
	RwInt32 Cnt				= 0;

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x	= (RwReal)m;
			SectorSPos.z	= (RwReal)l;
			TmpSectorIdx	= pMgr->GetSectorIdx(SectorSPos);

			if(SectorIdx == TmpSectorIdx)
			{
				switch(_FilePosAtSector)
				{
				case eFPAS_HEIGHTFIELD:
					{
						::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

						return pFile;
					}
				case eFPAS_DOODADS:
					{
						_RpNtlWorldGetFilePosDoodads(pFile);

						::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

						return pFile;
					}
				}
			}
			else
			{
				// skip the file position 'till the proper sector will be found
				_RpNtlWorldSectorReadDummy(pFile);
			}
		}
	}

	::fclose(pFile);
	pFile = NULL;

	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif