#include "precomp_ntlpresentation.h"

#include "precomp_ntlpresentation.h"
#include "NtlPLSceneManager.h"
#include "NtlPLPropertyContainer.h"

#include "NtlWorldField.h"
#include "NtlWorldFieldManager.h"

#include "NtlWorldSectorInfo.h"

#include "NtlPLWorldEntity.h"

#include "SM_GetTileTransparency.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

CSM_GetTileTransparency::CSM_GetTileTransparency()
{
}

CSM_GetTileTransparency::~CSM_GetTileTransparency()
{
	DestroySelfRetMap();
}

RwBool CSM_GetTileTransparency::DestroySelfRetMap()
{
	ISectorMethod::tSELF_RET_MAP_ITER The = m_SelfRetMap.begin();
	while(The != m_SelfRetMap.end())
	{
		CSM_GetTileTransparency_RetContainer* pDel = static_cast<CSM_GetTileTransparency_RetContainer*>((*The).second);
		NTL_DELETE(pDel);

		++The;
	}
	m_SelfRetMap.clear();

	return TRUE;
}

RwBool CSM_GetTileTransparency::Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI)
{
	return TRUE;
}

VOID* CSM_GetTileTransparency::GetAnalysis()
{
	CSM_GetTileTransparency_RetContainer* _pRet = NTL_NEW CSM_GetTileTransparency_RetContainer;

	_pRet->iFieldIndex	= m_RetContainer.iFieldIndex;
	_pRet->vTranslation	= m_RetContainer.vTranslation;

	return static_cast<VOID*>(_pRet);
}

RwBool CSM_GetTileTransparency::CheckSector(BYTE* _pFileMemPos)
{
	/*
	WorldFileFormat - SectorMethod
	*/

	//RwInt32		iWorldSectorVertNum		= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;
	RwBool		bTileTransparency		= FALSE;

	_pFileMemPos = GetNtlWorldSectorInfo()->SectorMaterialSkipToFileMem(_pFileMemPos);
	_pFileMemPos = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMem(_pFileMemPos);
	_pFileMemPos = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(_pFileMemPos);
	_pFileMemPos = GetNtlWorldSectorInfo()->SectorObjectSkipToFileMem(_pFileMemPos, AW_HEGITHFIELD);
	_pFileMemPos = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(_pFileMemPos);
	_pFileMemPos = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(_pFileMemPos);
	_pFileMemPos = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(_pFileMemPos);
	_pFileMemPos = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(_pFileMemPos);
	_pFileMemPos = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(_pFileMemPos);

	CopyMemory(&bTileTransparency, _pFileMemPos, sizeof(RwBool));
	_pFileMemPos += (sizeof(RwBool));

	return bTileTransparency;
}

RwBool CSM_GetTileTransparency::Analyze(DWORD _SectorIdx)
{
	CNtlWorldFieldManager*	pMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwV3d					Pos;
	RwInt32					CurFieldIdx;
	CFileMem				CurFieldFileMem;
	RwInt32					SelSectorIdxCntInField;
	BYTE*					pPosCurFileMem;
	RwBool					bRet = FALSE;

	CNtlMath::MathRwV3dAssign(&Pos, pMgr->m_pSectors[_SectorIdx].DatumPoint.x, pMgr->m_pSectors[_SectorIdx].DatumPoint.y, pMgr->m_pSectors[_SectorIdx].DatumPoint.z);
	CurFieldIdx = pMgr->GetFieldIdx(Pos);

	// selected sector index count in the current field file
	SelSectorIdxCntInField = pMgr->GetSelSectorIdxCntInField(_SectorIdx, CurFieldIdx);

	// load a field file and check the sector
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", CurFieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	CurFieldFileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

	pPosCurFileMem = CurFieldFileMem.GetDat();
	//pPosCurFileMem = CNtlWorldFileMemAccessor::GetSPosOfMaterial(CurFieldFileMem.GetDat());

	for(RwInt32 i = 0; i < SelSectorIdxCntInField; ++i)
	{
		pPosCurFileMem = GetNtlWorldSectorInfo()->OutdoorSectorSkipToFileMem(pPosCurFileMem);
	}

	if(CheckSector(pPosCurFileMem))
	{
		m_RetContainer.iFieldIndex	= CurFieldIdx;
		m_RetContainer.vTranslation	= Pos;
		bRet = TRUE;
	}

	CurFieldFileMem.Free();
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return bRet;
}

RwBool CSM_GetTileTransparency::Report()
{
	CNtlWorldFieldManager*	pMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	FILE*					pFile;

	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	::_chdir("Tool");
	::_chdir("Saber");
	::_chdir("Report");
	::_chdir("Sector Method");

	if(!fopen_s(&pFile, "SM_GET_TILE_TRANSPARENCY.rpt", "wt"))
	{
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		RwChar szBuffer[256];
		RwInt32 nYear   = (RwInt32)systemTime.wYear;
		RwInt32 nMonth  = (RwInt32)systemTime.wMonth;
		RwInt32 nDay	= (RwInt32)systemTime.wDay;
		RwInt32 nHour	= (RwInt32)systemTime.wHour;
		RwInt32 nSecond = (RwInt32)systemTime.wSecond;
		RwInt32 nMinute = (RwInt32)systemTime.wMinute;
		RwReal fMilliseconds = (RwReal)systemTime.wMilliseconds / 1000.f;
		sprintf_s(szBuffer, "%d년 %d월 %d일 %d시 %d분 %d초", nYear, nMonth, nDay, nHour, nMinute, nSecond);

		fprintf(pFile, "========== %s ==========\n", szBuffer);

		ISectorMethod::tSELF_RET_MAP_ITER The = m_SelfRetMap.begin();
		while(The != m_SelfRetMap.end())
		{
			CSM_GetTileTransparency_RetContainer* pRetContainer = static_cast<CSM_GetTileTransparency_RetContainer*>((*The).second);

			fprintf(pFile, " : FIELD_INDEX(%d) : SECTOR_POS(%.1f, %.1f, %.1f)\n", pRetContainer->iFieldIndex,
																				  pRetContainer->vTranslation.x,
																				  pRetContainer->vTranslation.y,
																				  pRetContainer->vTranslation.z);				
			++The;
		}

		fclose(pFile);
	}

	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

CSM_GetTileTransparency::CSM_GetTileTransparency()
{
}

CSM_GetTileTransparency::~CSM_GetTileTransparency()
{
	DestroySelfRetMap();
}

RwBool CSM_GetTileTransparency::DestroySelfRetMap()
{
	ISectorMethod::tSELF_RET_MAP_ITER The = m_SelfRetMap.begin();
	while(The != m_SelfRetMap.end())
	{
		CSM_GetTileTransparency_RetContainer* pDel = static_cast<CSM_GetTileTransparency_RetContainer*>((*The).second);
		NTL_DELETE(pDel);

		++The;
	}
	m_SelfRetMap.clear();

	return TRUE;
}

RwBool CSM_GetTileTransparency::Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI)
{
	return TRUE;
}

VOID* CSM_GetTileTransparency::GetAnalysis()
{
	CSM_GetTileTransparency_RetContainer* _pRet = NTL_NEW CSM_GetTileTransparency_RetContainer;

	_pRet->iFieldIndex	= m_RetContainer.iFieldIndex;
	_pRet->vTranslation	= m_RetContainer.vTranslation;

	return static_cast<VOID*>(_pRet);
}

RwBool CSM_GetTileTransparency::CheckSector(BYTE* _pFileMemPos)
{
	/*
	WorldFileFormat - SectorMethod
	*/

	//RwInt32		iWorldSectorVertNum		= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;
	RwBool		bTileTransparency		= FALSE;

	_pFileMemPos = Skip_Material(_pFileMemPos);
	_pFileMemPos = Skip_Heightfield(_pFileMemPos);
	_pFileMemPos = Skip_Diffuse(_pFileMemPos);
	_pFileMemPos = Skip_Doodads(_pFileMemPos);
	_pFileMemPos = Skip_Water(_pFileMemPos);
	_pFileMemPos = Skip_Shadow(_pFileMemPos);
	_pFileMemPos = Skip_Effects(_pFileMemPos);
	_pFileMemPos = Skip_SE(_pFileMemPos);
	// CZ-SS
	//_pFileMemPos = Skip_SS(_pFileMemPos);
	_pFileMemPos = Skip_SectorCull(_pFileMemPos);

	CopyMemory(&bTileTransparency, _pFileMemPos, sizeof(RwBool));
	_pFileMemPos += (sizeof(RwBool));

	return bTileTransparency;
}

RwBool CSM_GetTileTransparency::Analyze(DWORD _SectorIdx)
{
	CNtlWorldFieldManager*	pMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwV3d					Pos;
	RwInt32					CurFieldIdx;
	CFileMem				CurFieldFileMem;
	RwInt32					SelSectorIdxCntInField;
	BYTE*					pPosCurFileMem;
	RwBool					bRet = FALSE;

	CNtlMath::MathRwV3dAssign(&Pos, pMgr->m_pSectors[_SectorIdx].DatumPoint.x, pMgr->m_pSectors[_SectorIdx].DatumPoint.y, pMgr->m_pSectors[_SectorIdx].DatumPoint.z);
	CurFieldIdx = pMgr->GetFieldIdx(Pos);

	// selected sector index count in the current field file
	SelSectorIdxCntInField = pMgr->GetSelSectorIdxCntInField(_SectorIdx, CurFieldIdx);

	// load a field file and check the sector
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", CurFieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	CurFieldFileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

	pPosCurFileMem = CNtlWorldFileMemAccessor::GetSPosOfMaterial(CurFieldFileMem.GetDat());

	for(RwInt32 i = 0; i < SelSectorIdxCntInField; ++i)
	{
		pPosCurFileMem = CNtlWorldFileMemAccessor::Skip_Sector(pPosCurFileMem);
	}

	if(CheckSector(pPosCurFileMem))
	{
		m_RetContainer.iFieldIndex	= CurFieldIdx;
		m_RetContainer.vTranslation	= Pos;
		bRet = TRUE;
	}

	CurFieldFileMem.Free();
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return bRet;
}

RwBool CSM_GetTileTransparency::Report()
{
	CNtlWorldFieldManager*	pMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	FILE*					pFile;

	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	::_chdir("Tool");
	::_chdir("Saber");
	::_chdir("Report");
	::_chdir("Sector Method");

	if(!fopen_s(&pFile, "SM_GET_TILE_TRANSPARENCY.rpt", "wt"))
	{
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		RwChar szBuffer[256];
		RwInt32 nYear   = (RwInt32)systemTime.wYear;
		RwInt32 nMonth  = (RwInt32)systemTime.wMonth;
		RwInt32 nDay	= (RwInt32)systemTime.wDay;
		RwInt32 nHour	= (RwInt32)systemTime.wHour;
		RwInt32 nSecond = (RwInt32)systemTime.wSecond;
		RwInt32 nMinute = (RwInt32)systemTime.wMinute;
		RwReal fMilliseconds = (RwReal)systemTime.wMilliseconds / 1000.f;
		sprintf_s(szBuffer, "%d년 %d월 %d일 %d시 %d분 %d초", nYear, nMonth, nDay, nHour, nMinute, nSecond);

		fprintf(pFile, "========== %s ==========\n", szBuffer);

		ISectorMethod::tSELF_RET_MAP_ITER The = m_SelfRetMap.begin();
		while(The != m_SelfRetMap.end())
		{
			CSM_GetTileTransparency_RetContainer* pRetContainer = static_cast<CSM_GetTileTransparency_RetContainer*>((*The).second);

			fprintf(pFile, " : FIELD_INDEX(%d) : SECTOR_POS(%.1f, %.1f, %.1f)\n", pRetContainer->iFieldIndex,
				pRetContainer->vTranslation.x,
				pRetContainer->vTranslation.y,
				pRetContainer->vTranslation.z);				
			++The;
		}

		fclose(pFile);
	}

	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif