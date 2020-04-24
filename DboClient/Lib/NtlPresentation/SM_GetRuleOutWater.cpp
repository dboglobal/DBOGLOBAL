#include "precomp_ntlpresentation.h"

#include "precomp_ntlpresentation.h"
#include "NtlPLSceneManager.h"
#include "NtlPLPropertyContainer.h"

#include "NtlWorldField.h"
#include "NtlWorldFieldManager.h"

#include "NtlWorldSectorInfo.h"

#include "NtlPLWorldEntity.h"

#include "SM_GetRuleOutWater.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

#define dSM_GETRULEOUTWATER_OFFSET_ALPHA	127

CSM_GetRuleOutWater::CSM_GetRuleOutWater()
{
}

CSM_GetRuleOutWater::~CSM_GetRuleOutWater()
{
	DestroySelfRetMap();
}

RwBool CSM_GetRuleOutWater::DestroySelfRetMap()
{
	ISectorMethod::tSELF_RET_MAP_ITER The = m_SelfRetMap.begin();
	while(The != m_SelfRetMap.end())
	{
		CSM_GetRuleOutWater_RetContainer* pDel = static_cast<CSM_GetRuleOutWater_RetContainer*>((*The).second);
		NTL_DELETE(pDel);

		++The;
	}
	m_SelfRetMap.clear();

	return TRUE;
}

RwBool CSM_GetRuleOutWater::Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI)
{
	return TRUE;
}

VOID* CSM_GetRuleOutWater::GetAnalysis()
{
	CSM_GetRuleOutWater_RetContainer* _pRet = NTL_NEW CSM_GetRuleOutWater_RetContainer;

	_pRet->eRuleOutType = m_RetContainer.eRuleOutType;
	_pRet->vTranslation = m_RetContainer.vTranslation;

	return static_cast<VOID*>(_pRet);
}

RwBool CSM_GetRuleOutWater::CheckSector(BYTE* _pFileMemPos)
{
	/*
	WorldFileFormat - SectorMethod
	*/

	RwInt32		iWorldSectorVertNum		= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;
	RwV3d*		pvHeightfield			= NTL_NEW RwV3d[iWorldSectorVertNum];
	RwBool		bRet					= FALSE;
	RwBool		bWaterExistence			= FALSE;

	_pFileMemPos = GetNtlWorldSectorInfo()->SectorMaterialSkipToFileMem(_pFileMemPos);
	_pFileMemPos = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMemGetVertex(_pFileMemPos, 0, iWorldSectorVertNum, pvHeightfield);
	_pFileMemPos = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(_pFileMemPos);
	_pFileMemPos = GetNtlWorldSectorInfo()->SectorObjectSkipToFileMem(_pFileMemPos, AW_HEGITHFIELD);

	CopyMemory(&bWaterExistence, _pFileMemPos, sizeof(RwBool));
	_pFileMemPos += (sizeof(RwBool));

	if (bWaterExistence)
	{
		RwReal fHeight;
		RwRGBA rgbaDiffuse;

		//////////////////////////////////////////////////////////////////////////
		// 필요한 정보를 복사해두고 파일포인터를 스킵해둔다.
		//////////////////////////////////////////////////////////////////////////
		// Use : Height
		CopyMemory(&fHeight, _pFileMemPos, sizeof(RwReal));
		_pFileMemPos += (sizeof(RwReal));

		// Use : Diffuse
		CopyMemory(&rgbaDiffuse, _pFileMemPos, sizeof(RwRGBA));
		_pFileMemPos += (sizeof(RwRGBA));

		// Skip : renderstate source : dst : rotate : scale
		_pFileMemPos += (sizeof(DWORD) * 2) + (sizeof(RwReal) * 2);

		// Skip : Depth Map Size
		RwInt32 Size = 0;
		CopyMemory(&Size, _pFileMemPos, sizeof(RwInt32));
		_pFileMemPos += sizeof(RwInt32);

		_pFileMemPos += (sizeof(RwReal) * 2) + sizeof(RwInt32);

		RwBool bDepthMapExistence;
		CopyMemory(&bDepthMapExistence, _pFileMemPos, sizeof(RwBool));
		_pFileMemPos += sizeof(RwBool);

		if(bDepthMapExistence)
		{
			_pFileMemPos += (sizeof(BYTE) * Size * Size);
		}

		//////////////////////////////////////////////////////////////////////////
		// 실질적인 검사를 한다.
		//////////////////////////////////////////////////////////////////////////
		// Water Check
		{
			// Ahpha Check
			m_RetContainer.eRuleOutType = CSM_GetRuleOutWater_RetContainer::eRULEOUT_TYPE_NONE;
			if (rgbaDiffuse.alpha <= dSM_GETRULEOUTWATER_OFFSET_ALPHA)
			{
				m_RetContainer.eRuleOutType |= CSM_GetRuleOutWater_RetContainer::eRULEOUT_TYPE_ALPHA;
			}

			// Heightfield Check
			RwBool bCheckHeightfield = FALSE;
			for (int i = 0; i < iWorldSectorVertNum; ++i)
			{
				if (pvHeightfield[i].y < fHeight)
				{	
					bCheckHeightfield = TRUE;
					break;
				}
			}
			
			if (!bCheckHeightfield)
			{				
				m_RetContainer.eRuleOutType |= CSM_GetRuleOutWater_RetContainer::eRULEOUT_TYPE_LOWER;
			}

			if (m_RetContainer.eRuleOutType)
			{
				m_RetContainer.vTranslation		= pvHeightfield[iWorldSectorVertNum / 2];
				m_RetContainer.vTranslation.y	= fHeight;
				bRet = TRUE;
			}
		}
	}

	NTL_ARRAY_DELETE(pvHeightfield);
	return bRet;
}

RwBool CSM_GetRuleOutWater::Analyze(DWORD _SectorIdx)
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
		bRet = TRUE;
	}

	CurFieldFileMem.Free();
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return bRet;
}

RwBool CSM_GetRuleOutWater::Report()
{
	CNtlWorldFieldManager*	pMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	FILE*					pFile;

	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	::_chdir("Tool");
	::_chdir("Saber");
	::_chdir("Report");
	::_chdir("Sector Method");

	if(!fopen_s(&pFile, "SM_GET_RULE_OUT_WATER.rpt", "wt"))
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
			CSM_GetRuleOutWater_RetContainer* pRetContainer = static_cast<CSM_GetRuleOutWater_RetContainer*>((*The).second);

			if (pRetContainer->eRuleOutType & CSM_GetRuleOutWater_RetContainer::eRULEOUT_TYPE_ALPHA)
			{
				fprintf(pFile, "(ALPHA)");
			}
			if (pRetContainer->eRuleOutType & CSM_GetRuleOutWater_RetContainer::eRULEOUT_TYPE_LOWER)
			{
				fprintf(pFile, "(LOWER)");
			}

			fprintf(pFile, " : POS(%.1f, %.1f, %.1f)\n", pRetContainer->vTranslation.x,
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

#define dSM_GETRULEOUTWATER_OFFSET_ALPHA	127

CSM_GetRuleOutWater::CSM_GetRuleOutWater()
{
}

CSM_GetRuleOutWater::~CSM_GetRuleOutWater()
{
	DestroySelfRetMap();
}

RwBool CSM_GetRuleOutWater::DestroySelfRetMap()
{
	ISectorMethod::tSELF_RET_MAP_ITER The = m_SelfRetMap.begin();
	while(The != m_SelfRetMap.end())
	{
		CSM_GetRuleOutWater_RetContainer* pDel = static_cast<CSM_GetRuleOutWater_RetContainer*>((*The).second);
		NTL_DELETE(pDel);

		++The;
	}
	m_SelfRetMap.clear();

	return TRUE;
}

RwBool CSM_GetRuleOutWater::Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI)
{
	return TRUE;
}

VOID* CSM_GetRuleOutWater::GetAnalysis()
{
	CSM_GetRuleOutWater_RetContainer* _pRet = NTL_NEW CSM_GetRuleOutWater_RetContainer;

	_pRet->eRuleOutType = m_RetContainer.eRuleOutType;
	_pRet->vTranslation = m_RetContainer.vTranslation;

	return static_cast<VOID*>(_pRet);
}

RwBool CSM_GetRuleOutWater::CheckSector(BYTE* _pFileMemPos)
{
	/*
	WorldFileFormat - SectorMethod
	*/

	RwInt32		iWorldSectorVertNum		= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;
	RwV3d*		pvHeightfield			= NTL_NEW RwV3d[iWorldSectorVertNum];
	RwBool		bRet					= FALSE;
	RwBool		bWaterExistence			= FALSE;

	_pFileMemPos = Skip_Material(_pFileMemPos);

	CopyMemory(pvHeightfield, _pFileMemPos, sizeof(RwV3d) * iWorldSectorVertNum);
	_pFileMemPos += (sizeof(RwV3d) * iWorldSectorVertNum);

	_pFileMemPos = Skip_Diffuse(_pFileMemPos);
	_pFileMemPos = Skip_Doodads(_pFileMemPos);

	CopyMemory(&bWaterExistence, _pFileMemPos, sizeof(RwBool));
	_pFileMemPos += (sizeof(RwBool));

	if (bWaterExistence)
	{
		RwReal fHeight;
		RwRGBA rgbaDiffuse;

		//////////////////////////////////////////////////////////////////////////
		// 필요한 정보를 복사해두고 파일포인터를 스킵해둔다.
		//////////////////////////////////////////////////////////////////////////
		// Use : Height
		CopyMemory(&fHeight, _pFileMemPos, sizeof(RwReal));
		_pFileMemPos += (sizeof(RwReal));

		// Use : Diffuse
		CopyMemory(&rgbaDiffuse, _pFileMemPos, sizeof(RwRGBA));
		_pFileMemPos += (sizeof(RwRGBA));

		// Skip : renderstate source : dst : rotate : scale
		_pFileMemPos += (sizeof(DWORD) * 2) + (sizeof(RwReal) * 2);

		// Skip : Depth Map Size
		RwInt32 Size = 0;
		CopyMemory(&Size, _pFileMemPos, sizeof(RwInt32));
		_pFileMemPos += sizeof(RwInt32);

		_pFileMemPos += (sizeof(RwReal) * 2) + sizeof(RwInt32);

		RwBool bDepthMapExistence;
		CopyMemory(&bDepthMapExistence, _pFileMemPos, sizeof(RwBool));
		_pFileMemPos += sizeof(RwBool);

		if(bDepthMapExistence)
		{
			_pFileMemPos += (sizeof(BYTE) * Size * Size);
		}

		//////////////////////////////////////////////////////////////////////////
		// 실질적인 검사를 한다.
		//////////////////////////////////////////////////////////////////////////
		// Water Check
		{
			// Ahpha Check
			m_RetContainer.eRuleOutType = CSM_GetRuleOutWater_RetContainer::eRULEOUT_TYPE_NONE;
			if (rgbaDiffuse.alpha <= dSM_GETRULEOUTWATER_OFFSET_ALPHA)
			{
				m_RetContainer.eRuleOutType |= CSM_GetRuleOutWater_RetContainer::eRULEOUT_TYPE_ALPHA;
			}

			// Heightfield Check
			RwBool bCheckHeightfield = FALSE;
			for (int i = 0; i < iWorldSectorVertNum; ++i)
			{
				if (pvHeightfield[i].y < fHeight)
				{	
					bCheckHeightfield = TRUE;
					break;
				}
			}

			if (!bCheckHeightfield)
			{				
				m_RetContainer.eRuleOutType |= CSM_GetRuleOutWater_RetContainer::eRULEOUT_TYPE_LOWER;
			}

			if (m_RetContainer.eRuleOutType)
			{
				m_RetContainer.vTranslation		= pvHeightfield[iWorldSectorVertNum / 2];
				m_RetContainer.vTranslation.y	= fHeight;
				bRet = TRUE;
			}
		}
	}

	NTL_ARRAY_DELETE(pvHeightfield);
	return bRet;
}

RwBool CSM_GetRuleOutWater::Analyze(DWORD _SectorIdx)
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
		bRet = TRUE;
	}

	CurFieldFileMem.Free();
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return bRet;
}

RwBool CSM_GetRuleOutWater::Report()
{
	CNtlWorldFieldManager*	pMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	FILE*					pFile;

	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	::_chdir("Tool");
	::_chdir("Saber");
	::_chdir("Report");
	::_chdir("Sector Method");

	if(!fopen_s(&pFile, "SM_GET_RULE_OUT_WATER.rpt", "wt"))
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
			CSM_GetRuleOutWater_RetContainer* pRetContainer = static_cast<CSM_GetRuleOutWater_RetContainer*>((*The).second);

			if (pRetContainer->eRuleOutType & CSM_GetRuleOutWater_RetContainer::eRULEOUT_TYPE_ALPHA)
			{
				fprintf(pFile, "(ALPHA)");
			}
			if (pRetContainer->eRuleOutType & CSM_GetRuleOutWater_RetContainer::eRULEOUT_TYPE_LOWER)
			{
				fprintf(pFile, "(LOWER)");
			}

			fprintf(pFile, " : POS(%.1f, %.1f, %.1f)\n", pRetContainer->vTranslation.x,
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