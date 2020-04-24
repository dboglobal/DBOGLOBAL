#include "precomp_ntlpresentation.h"
#include "SM_SplatTexMaxBlend.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLSceneManager.h"
#include "NtlWorldSector.h"
#include "NtlWorldSectorInfo.h"
#include "NtlPLResourceManager.h"
#include "NtlWorldField.h"
#include "NtlPLWorldEntity.h"
#include "NtlMath.h"
#include "FileMem.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

CSM_SplatTexMaxBlend::CSM_SplatTexMaxBlend(void)
{
}

CSM_SplatTexMaxBlend::~CSM_SplatTexMaxBlend(void)
{
	DestroySelfRetMap();
}

RwBool CSM_SplatTexMaxBlend::DestroySelfRetMap()
{
	ISectorMethod::tSELF_RET_MAP_ITER The = m_SelfRetMap.begin();
	while(The != m_SelfRetMap.end())
	{
		CSM_SplatTexMaxBlend_RetContainer* pDel = static_cast<CSM_SplatTexMaxBlend_RetContainer*>((*The).second);
		NTL_DELETE(pDel);

		++The;
	}
	m_SelfRetMap.clear();

	return TRUE;
}

VOID CSM_SplatTexMaxBlend::RemoveSplatTex(RwInt32 _FieldIdx, RwInt32 _SectorIdx, RwInt32 _StartIdx2Del)
{
	CNtlWorldFieldManager*	pMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pMgr->GetFields());
	CFileMem				CurFieldFileMem;
	RwInt32					SelSectorIdxCntInField;
	BYTE*					pPosCurFileMem;

	SelSectorIdxCntInField = pMgr->GetSelSectorIdxCntInField(_SectorIdx, _FieldIdx);

	// load a field file and check the sector
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", _FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	CurFieldFileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

	pPosCurFileMem = CurFieldFileMem.GetDat();
	//pPosCurFileMem = CNtlWorldFileMemAccessor::GetSPosOfMaterial(CurFieldFileMem.GetDat());

	FILE* pNewFieldFile;
	if(!fopen_s(&pNewFieldFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
	{
		for(RwInt32 i = 0; i < 4; ++i)
		{
			if(SelSectorIdxCntInField != i)
			{
				pPosCurFileMem = GetNtlWorldSectorInfo()->OutdoorSectorSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);				
			}
			else
			{
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorMaterialSaveIntoFileFromFileMemSplatTexMaxBlend(pNewFieldFile, pPosCurFileMem, _StartIdx2Del);

				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorHeightfieldSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorPrelightsSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);

				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorObjectSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem, AW_HEGITHFIELD);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorWaterSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorShadowSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorEffectSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorSoundEffectSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorCullSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorTileTransparencySaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorShoreLineSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorDecalSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorPlantSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorWorldLightSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorOccluderSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorLightObjectSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
				pPosCurFileMem = GetNtlWorldSectorInfo()->SectorDojoSaveIntoFileFromFileMem(pNewFieldFile, pPosCurFileMem);
			}
		}
	}
	fclose(pNewFieldFile);

	CurFieldFileMem.Free();
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
}

RwBool CSM_SplatTexMaxBlend::Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI)
{
	if(_SMETI != eSMETI_SECTOR)
	{
		return TRUE;
	}

	CNtlWorldFieldManager*	pMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwV3d					TmpPos;
	RwInt32					CurSectorIdx;
	RwInt32					CurFieldIdx;
	CFileMem				CurFieldFileMem;

	ISectorMethod::tSELF_RET_MAP_ITER The = m_SelfRetMap.begin();
	while(The != m_SelfRetMap.end())
	{
		// get current sector and field indices
		CurSectorIdx = (*The).first;
		
		CNtlMath::MathRwV3dAssign(&TmpPos, pMgr->m_pSectors[CurSectorIdx].DatumPoint.x, pMgr->m_pSectors[CurSectorIdx].DatumPoint.y, pMgr->m_pSectors[CurSectorIdx].DatumPoint.z);
		CurFieldIdx = pMgr->GetFieldIdx(TmpPos);

		CSM_SplatTexMaxBlend_RetContainer* pCurRetContainer = static_cast<CSM_SplatTexMaxBlend_RetContainer*>((*The).second);

		for(RwInt32 i = (dNTL_SPLAT_LAYER_NUM - 1); i > 0; --i)
		{
			if(pCurRetContainer->m_AverageAlphaDensity[i] > dMAX_BLEND_DENSITY)
			{
				RwInt32 StartIdx2Del = i - 1;
				RemoveSplatTex(CurFieldIdx, CurSectorIdx, StartIdx2Del);
				break;
			}
		}

		++The;
	}

	return TRUE;
}

RwBool CSM_SplatTexMaxBlend::CheckSector(BYTE* _pFileMemPos)
{
	/*
	WorldFileFormat - SectorMethod
	*/

	RwBool	BoolFlag;
	BYTE*	pAlphaValueArray = NTL_NEW BYTE [dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum];
	RwBool	Ret = FALSE;

	_pFileMemPos += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
	_pFileMemPos += (sizeof(D3DXMATRIX));
	_pFileMemPos += (sizeof(D3DXMATRIX));
	_pFileMemPos += (sizeof(RwReal));
	_pFileMemPos += (sizeof(RwUInt8));

	for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		CopyMemory(&BoolFlag, _pFileMemPos, sizeof(RwBool));
		_pFileMemPos += (sizeof(RwBool));

		if(BoolFlag)
		{
			_pFileMemPos += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

			CopyMemory(&BoolFlag, _pFileMemPos, sizeof(RwBool));
			_pFileMemPos += (sizeof(RwBool));

			if(BoolFlag)
			{
				_pFileMemPos += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

				CopyMemory(pAlphaValueArray, _pFileMemPos, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
				_pFileMemPos += (sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);

				m_RetContainer.m_AverageAlphaDensity[i] = CNtlPLResourceManager::GetInstance()->GetAlphaAverageDensity(pAlphaValueArray, dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
				if(m_RetContainer.m_AverageAlphaDensity[i] > dMAX_BLEND_DENSITY && i)
				{
					Ret = TRUE;
				}
			}

			_pFileMemPos += (sizeof(D3DXMATRIX));
			_pFileMemPos += (sizeof(D3DXMATRIX));
			_pFileMemPos += (sizeof(RwReal));
			_pFileMemPos += (sizeof(RwUInt8));
		}
		else
		{
			m_RetContainer.m_AverageAlphaDensity[i] = -999.0f;
		}
	}

	NTL_ARRAY_DELETE(pAlphaValueArray);

	return Ret;
}

VOID* CSM_SplatTexMaxBlend::GetAnalysis()
{
	CSM_SplatTexMaxBlend_RetContainer* _pRet = NTL_NEW CSM_SplatTexMaxBlend_RetContainer;

	for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		_pRet->m_AverageAlphaDensity[i] = m_RetContainer.m_AverageAlphaDensity[i];
	}

	return static_cast<VOID*>(_pRet);
}

RwBool CSM_SplatTexMaxBlend::Analyze(DWORD _SectorIdx)
{
	CNtlWorldFieldManager*	pMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwV3d					Pos;
	RwInt32					CurFieldIdx;
	CFileMem				CurFieldFileMem;
	RwInt32					SelSectorIdxCntInField;
	BYTE*					pPosCurFileMem;
	RwBool					Ret = FALSE;

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
		Ret = TRUE;
	}

	CurFieldFileMem.Free();
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return Ret;
}

RwBool CSM_SplatTexMaxBlend::Report()
{
	CNtlWorldFieldManager*	pMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwInt32					CurSectorIdx;
	FILE*					pFile;

	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	::_chdir("Tool");
	::_chdir("Saber");
	::_chdir("Report");
	::_chdir("Sector Method");

	if(!fopen_s(&pFile, "SM_SPLAT_TEX_MAX_BLEND.rpt", "wt"))
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
			CurSectorIdx = static_cast<RwInt32>((*The).first);

			RwV3d TmpPos;
			CNtlMath::MathRwV3dAssign(&TmpPos, pMgr->m_pSectors[CurSectorIdx].DatumPoint.x, pMgr->m_pSectors[CurSectorIdx].DatumPoint.y, pMgr->m_pSectors[CurSectorIdx].DatumPoint.z);
			fprintf(pFile, "%d's sector(%d's field)\n", CurSectorIdx, pMgr->GetFieldIdx(TmpPos));
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

CSM_SplatTexMaxBlend::CSM_SplatTexMaxBlend(void)
{
}

CSM_SplatTexMaxBlend::~CSM_SplatTexMaxBlend(void)
{
	DestroySelfRetMap();
}

RwBool CSM_SplatTexMaxBlend::DestroySelfRetMap()
{
	ISectorMethod::tSELF_RET_MAP_ITER The = m_SelfRetMap.begin();
	while(The != m_SelfRetMap.end())
	{
		CSM_SplatTexMaxBlend_RetContainer* pDel = static_cast<CSM_SplatTexMaxBlend_RetContainer*>((*The).second);
		NTL_DELETE(pDel);

		++The;
	}
	m_SelfRetMap.clear();

	return TRUE;
}

VOID CSM_SplatTexMaxBlend::RemoveSplatTex(RwInt32 _FieldIdx, RwInt32 _SectorIdx, RwInt32 _StartIdx2Del)
{
	CNtlWorldFieldManager*	pMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pMgr->GetFields());
	CFileMem				CurFieldFileMem;
	RwInt32					SelSectorIdxCntInField;
	BYTE*					pPosCurFileMem;

	SelSectorIdxCntInField = pMgr->GetSelSectorIdxCntInField(_SectorIdx, _FieldIdx);

	// load a field file and check the sector
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", _FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	CurFieldFileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
	pPosCurFileMem = CNtlWorldFileMemAccessor::GetSPosOfMaterial(CurFieldFileMem.GetDat());

	FILE* pNewFieldFile;
	if(!fopen_s(&pNewFieldFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
	{
		for(RwInt32 i = 0; i < 4; ++i)
		{
			if(SelSectorIdxCntInField != i)
			{
				pPosCurFileMem = Save_Sector(pPosCurFileMem, pNewFieldFile);
			}
			else
			{
				pPosCurFileMem = OnSM_SplatTexMaxBlend(pPosCurFileMem, pNewFieldFile, _StartIdx2Del);
				pPosCurFileMem = Save_Heightfield(pPosCurFileMem, pNewFieldFile);
				pPosCurFileMem = Save_Diffuse(pPosCurFileMem, pNewFieldFile);
				pPosCurFileMem = Save_Doodads(pPosCurFileMem, pNewFieldFile);
				pPosCurFileMem = Save_Water(pPosCurFileMem, pNewFieldFile);
				pPosCurFileMem = Save_Shadow(pPosCurFileMem, pNewFieldFile);
				pPosCurFileMem = Save_Effects(pPosCurFileMem, pNewFieldFile);
				pPosCurFileMem = Save_SE(pPosCurFileMem, pNewFieldFile);
				// CZ-SS
				//pPosCurFileMem = Save_SS(pPosCurFileMem, pNewFieldFile);
				pPosCurFileMem = Save_SectorCull(pPosCurFileMem, pNewFieldFile);
				pPosCurFileMem = Save_TileTransparency(pPosCurFileMem, pNewFieldFile);
				pPosCurFileMem = Save_Shoreline(pPosCurFileMem, pNewFieldFile);
				pPosCurFileMem = Save_Decal(pPosCurFileMem, pNewFieldFile);
				pPosCurFileMem = Save_Plant(pPosCurFileMem, pNewFieldFile);
				pPosCurFileMem = Save_WorldLight(pPosCurFileMem, pNewFieldFile);				
				pPosCurFileMem = Save_Occluder(pPosCurFileMem, pNewFieldFile);				
				pPosCurFileMem = Save_HeatHazeObject(pPosCurFileMem, pNewFieldFile);		
				pPosCurFileMem = Save_LightObject(pPosCurFileMem, pNewFieldFile);		
				pPosCurFileMem = Save_Dojo(pPosCurFileMem, pNewFieldFile);		

			}
		}
	}
	fclose(pNewFieldFile);

	CurFieldFileMem.Free();
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
}

RwBool CSM_SplatTexMaxBlend::Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI)
{
	if(_SMETI != eSMETI_SECTOR)
	{
		return TRUE;
	}

	CNtlWorldFieldManager*	pMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwV3d					TmpPos;
	RwInt32					CurSectorIdx;
	RwInt32					CurFieldIdx;
	CFileMem				CurFieldFileMem;

	ISectorMethod::tSELF_RET_MAP_ITER The = m_SelfRetMap.begin();
	while(The != m_SelfRetMap.end())
	{
		// get current sector and field indices
		CurSectorIdx = (*The).first;

		CNtlMath::MathRwV3dAssign(&TmpPos, pMgr->m_pSectors[CurSectorIdx].DatumPoint.x, pMgr->m_pSectors[CurSectorIdx].DatumPoint.y, pMgr->m_pSectors[CurSectorIdx].DatumPoint.z);
		CurFieldIdx = pMgr->GetFieldIdx(TmpPos);

		CSM_SplatTexMaxBlend_RetContainer* pCurRetContainer = static_cast<CSM_SplatTexMaxBlend_RetContainer*>((*The).second);

		for(RwInt32 i = (dNTL_SPLAT_LAYER_NUM - 1); i > 0; --i)
		{
			if(pCurRetContainer->m_AverageAlphaDensity[i] > dMAX_BLEND_DENSITY)
			{
				RwInt32 StartIdx2Del = i - 1;
				RemoveSplatTex(CurFieldIdx, CurSectorIdx, StartIdx2Del);
				break;
			}
		}

		++The;
	}

	return TRUE;
}

RwBool CSM_SplatTexMaxBlend::CheckSector(BYTE* _pFileMemPos)
{
	/*
	WorldFileFormat - SectorMethod
	*/

	RwBool	BoolFlag;
	BYTE*	pAlphaValueArray = NTL_NEW BYTE [dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum];
	RwBool	Ret = FALSE;

	_pFileMemPos += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
	_pFileMemPos += (sizeof(D3DXMATRIX));
	_pFileMemPos += (sizeof(D3DXMATRIX));
	_pFileMemPos += (sizeof(RwReal));
	_pFileMemPos += (sizeof(RwUInt8));

	for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		CopyMemory(&BoolFlag, _pFileMemPos, sizeof(RwBool));
		_pFileMemPos += (sizeof(RwBool));

		if(BoolFlag)
		{
			_pFileMemPos += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

			CopyMemory(&BoolFlag, _pFileMemPos, sizeof(RwBool));
			_pFileMemPos += (sizeof(RwBool));

			if(BoolFlag)
			{
				_pFileMemPos += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

				CopyMemory(pAlphaValueArray, _pFileMemPos, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
				_pFileMemPos += (sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);

				m_RetContainer.m_AverageAlphaDensity[i] = CNtlPLResourceManager::GetInstance()->GetAlphaAverageDensity(pAlphaValueArray, dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
				if(m_RetContainer.m_AverageAlphaDensity[i] > dMAX_BLEND_DENSITY && i)
				{
					Ret = TRUE;
				}
			}

			_pFileMemPos += (sizeof(D3DXMATRIX));
			_pFileMemPos += (sizeof(D3DXMATRIX));
			_pFileMemPos += (sizeof(RwReal));
			_pFileMemPos += (sizeof(RwUInt8));
		}
		else
		{
			m_RetContainer.m_AverageAlphaDensity[i] = -999.0f;
		}
	}

	NTL_ARRAY_DELETE(pAlphaValueArray);

	return Ret;
}

VOID* CSM_SplatTexMaxBlend::GetAnalysis()
{
	CSM_SplatTexMaxBlend_RetContainer* _pRet = NTL_NEW CSM_SplatTexMaxBlend_RetContainer;

	for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		_pRet->m_AverageAlphaDensity[i] = m_RetContainer.m_AverageAlphaDensity[i];
	}

	return static_cast<VOID*>(_pRet);
}

RwBool CSM_SplatTexMaxBlend::Analyze(DWORD _SectorIdx)
{
	CNtlWorldFieldManager*	pMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwV3d					Pos;
	RwInt32					CurFieldIdx;
	CFileMem				CurFieldFileMem;
	RwInt32					SelSectorIdxCntInField;
	BYTE*					pPosCurFileMem;
	RwBool					Ret = FALSE;

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
		Ret = TRUE;
	}

	CurFieldFileMem.Free();
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return Ret;
}

RwBool CSM_SplatTexMaxBlend::Report()
{
	CNtlWorldFieldManager*	pMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwInt32					CurSectorIdx;
	FILE*					pFile;

	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	::_chdir("Tool");
	::_chdir("Saber");
	::_chdir("Report");
	::_chdir("Sector Method");

	if(!fopen_s(&pFile, "SM_SPLAT_TEX_MAX_BLEND.rpt", "wt"))
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
			CurSectorIdx = static_cast<RwInt32>((*The).first);

			RwV3d TmpPos;
			CNtlMath::MathRwV3dAssign(&TmpPos, pMgr->m_pSectors[CurSectorIdx].DatumPoint.x, pMgr->m_pSectors[CurSectorIdx].DatumPoint.y, pMgr->m_pSectors[CurSectorIdx].DatumPoint.z);
			fprintf(pFile, "%d's sector(%d's field)\n", CurSectorIdx, pMgr->GetFieldIdx(TmpPos));
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