#include "precomp_ntlpresentation.h"
#include "NtlPLSceneManager.h"
#include "NtlPLPropertyContainer.h"

#include "NtlWorldField.h"
#include "NtlWorldFieldManager.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLObjectProperty.h"

#include "NtlWorldSectorInfo.h"

#include "NtlPLObject.h"
#include "NtlPLPlanet.h"
#include "NtlPLObjectType.h"

#include "SM_GetRuleOutDoodad.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

#define dSM_GETRULEOUTDOODAD_OFFSET_UPPER	512.0f
#define dSM_GETRULEOUTDOODAD_OFFSET_LOWER	0.0f

CSM_GetRuleOutDoodad_RetContainer::CSM_GetRuleOutDoodad_RetContainer()
{
}

CSM_GetRuleOutDoodad_RetContainer::~CSM_GetRuleOutDoodad_RetContainer()
{
	Clear();
}

void CSM_GetRuleOutDoodad_RetContainer::Clear()
{
	for (tRET_VEC_ITER it = m_vecRet.begin(); it != m_vecRet.end(); ++it)
	{
		SSM_GetRuleOutDoodad* pRuleOutDoodad = *it;
		if (pRuleOutDoodad)
		{
			NTL_DELETE(pRuleOutDoodad);
		}
	}

	m_vecRet.clear();
}

void CSM_GetRuleOutDoodad_RetContainer::Push_Back(SSM_GetRuleOutDoodad* pRuleOutDoodad)
{
	SSM_GetRuleOutDoodad* pRuleOutDoodadNew	= NTL_NEW SSM_GetRuleOutDoodad;
	CopyMemory(pRuleOutDoodadNew->acName, pRuleOutDoodad->acName, sizeof(RwChar) * 128);
	pRuleOutDoodadNew->eRuleOutType	= pRuleOutDoodad->eRuleOutType;
	pRuleOutDoodadNew->vTranslation	= pRuleOutDoodad->vTranslation;

	m_vecRet.push_back(pRuleOutDoodadNew);
}

void CSM_GetRuleOutDoodad_RetContainer::Push_Back(const RwUInt32 eRuleOutType, const RwChar* pName, const RwV3d* pvTranslation)
{
	SSM_GetRuleOutDoodad* pRuleOutDoodadNew	= NTL_NEW SSM_GetRuleOutDoodad;
	CopyMemory(pRuleOutDoodadNew->acName, pName, sizeof(RwChar) * 128);
	pRuleOutDoodadNew->eRuleOutType	= eRuleOutType;
	pRuleOutDoodadNew->vTranslation	= *pvTranslation;

	m_vecRet.push_back(pRuleOutDoodadNew);
}
												  

void CSM_GetRuleOutDoodad_RetContainer::Copy(CSM_GetRuleOutDoodad_RetContainer* pRuleOutDoodad_RetContainer)
{
	Clear();
	for (tRET_VEC_ITER it = pRuleOutDoodad_RetContainer->Begin(); it != pRuleOutDoodad_RetContainer->End(); ++it)
	{
		SSM_GetRuleOutDoodad* pRuleOutDoodad = static_cast<SSM_GetRuleOutDoodad*>(*it);
		Push_Back(pRuleOutDoodad);	
	}
}

CSM_GetRuleOutDoodad::CSM_GetRuleOutDoodad()
{
}

CSM_GetRuleOutDoodad::~CSM_GetRuleOutDoodad()
{
	DestroySelfRetMap();
}

RwBool CSM_GetRuleOutDoodad::DestroySelfRetMap()
{
	ISectorMethod::tSELF_RET_MAP_ITER The = m_SelfRetMap.begin();
	while(The != m_SelfRetMap.end())
	{
		CSM_GetRuleOutDoodad_RetContainer* pDel = static_cast<CSM_GetRuleOutDoodad_RetContainer*>((*The).second);
		NTL_DELETE(pDel);

		++The;
	}
	m_SelfRetMap.clear();

	return TRUE;
}

RwBool CSM_GetRuleOutDoodad::Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI)
{
	return TRUE;
}

VOID* CSM_GetRuleOutDoodad::GetAnalysis()
{
	CSM_GetRuleOutDoodad_RetContainer* _pRet = NTL_NEW CSM_GetRuleOutDoodad_RetContainer;
	
	_pRet->Copy(&m_RetContainer);
	
	return static_cast<VOID*>(_pRet);
}

RwBool CSM_GetRuleOutDoodad::CheckSector(BYTE* _pFileMemPos)
{
	/*
	WorldFileFormat - SectorMethod
	*/

	RwInt32		iWorldSectorVertNum		= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;
	RwV3d*		pvHeightfield			= NTL_NEW RwV3d[iWorldSectorVertNum];
	RwBool		bRet					= FALSE;
	RwInt32		iNumDoodads				= 0;

	_pFileMemPos = GetNtlWorldSectorInfo()->SectorMaterialSkipToFileMem(_pFileMemPos);
	_pFileMemPos = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMemGetVertex(_pFileMemPos, 0, iWorldSectorVertNum, pvHeightfield);
	_pFileMemPos = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(_pFileMemPos);

	CopyMemory(&iNumDoodads, _pFileMemPos, sizeof(RwInt32));
	_pFileMemPos += (sizeof(RwInt32));

	for (int i = 0; i < iNumDoodads; ++i)
	{
		RwChar	acName[128];
		RwV3d	vScaling;
		RwV3d	vRotation;
		RwV3d	vTranslation;

		//////////////////////////////////////////////////////////////////////////
		// 필요한 정보를 복사해두고 파일포인터를 스킵해둔다.
		//////////////////////////////////////////////////////////////////////////
		// Use : Name
		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
		{
			CopyMemory(acName, _pFileMemPos, sizeof(RwChar) * 128);
			_pFileMemPos += (sizeof(RwChar) * 128);

		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
		{
			RwUInt32	uiLength = 0;

			CopyMemory(&uiLength, _pFileMemPos, sizeof(RwUInt32));			
			_pFileMemPos  += sizeof(RwUInt32);

			CopyMemory(acName, _pFileMemPos , (sizeof(RwChar) * uiLength));
			_pFileMemPos  += (sizeof(RwChar) * uiLength);
			acName[uiLength] = '\0';
		}

		// Use : Scaling
		CopyMemory(&vScaling, _pFileMemPos, sizeof(RwV3d));
		_pFileMemPos += (sizeof(RwV3d));

		// Use : Rotation
		CopyMemory(&vRotation, _pFileMemPos, sizeof(RwV3d));
		_pFileMemPos += (sizeof(RwV3d));

		// Use : Translation
		CopyMemory(&vTranslation, _pFileMemPos, sizeof(RwV3d));
		_pFileMemPos += (sizeof(RwV3d));

		RwUInt32 uiObjectType;
		CopyMemory(&uiObjectType, _pFileMemPos, sizeof(RwUInt32));
		_pFileMemPos += sizeof(RwUInt32);

		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			_pFileMemPos = CNtlPLObjectTypeHasProperty::SkipToFileMem(_pFileMemPos);
		}

		// Skip : trigger obj serial id
		_pFileMemPos += (sizeof(RwUInt32));
		if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
		{
			// Skip : object serial id
			_pFileMemPos += sizeof(RwUInt32);
		}

		// Skip : Obj. name index : visibility distance		
		_pFileMemPos += (sizeof(DWORD)) + (sizeof(RwReal));

		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
		{
			// Skip : enable trigger : enable selection : enable alterable : enable portal : enable path engine collision : enable shadowprop
			_pFileMemPos += (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool));
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
		{
			// Skip : enable trigger : enable selection : enable alterable : enable portal : enable path engine collision : enable shadowprop
			_pFileMemPos += (sizeof(RwUInt32));
		}

		// Skip : Projection Shadow Obj
		_pFileMemPos = CNtlPLObject::SkipPSMapToFileMem(_pFileMemPos);

		// Skip : milepost
		RwBool EnableFlg;
		CopyMemory(&EnableFlg, _pFileMemPos, sizeof(RwBool));
		_pFileMemPos += sizeof(RwBool);

		if(EnableFlg)
		{
			_pFileMemPos += sizeof(RwUInt32);
		}

		// attach light object skip
		_pFileMemPos = CNtlPLObject::SkipLightObjectAttachDataToFileMem(_pFileMemPos);

		if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
		{
			_pFileMemPos = CNtlPLObject::SkipLightmapToFileMem(_pFileMemPos);
		}

		//////////////////////////////////////////////////////////////////////////
		// 실질적인 검사를 한다.
		//////////////////////////////////////////////////////////////////////////
		// Doodad Check
		{
			CNtlPLObjectProperty* pProperty = (CNtlPLObjectProperty*)CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_OBJECT, acName);

			/*if (pProperty)
			{
				RwBBox rwBoundBox;
				if (!GetSRTBBoxFromObjectProperty(&rwBoundBox, pProperty, &vScaling, &vRotation, &vTranslation))
				{
					continue;
				}

				RwV3d	vCheck;
				RwInt32 eRuleOutType = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE;

				vCheck.x = (rwBoundBox.sup.x + rwBoundBox.inf.x) * 0.5f;
				vCheck.z = (rwBoundBox.sup.z + rwBoundBox.inf.z) * 0.5f;
				if (CheckRuleOut(&eRuleOutType, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
				{
					if (eRuleOutType)
					{
						m_RetContainer.Push_Back(eRuleOutType, acName, &vTranslation);						
						bRet = TRUE;
					}
					continue;
				}
				else
				{
					vCheck.x = rwBoundBox.sup.x;
					vCheck.z = rwBoundBox.sup.z;
					if (CheckRuleOut(&eRuleOutType, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
					{
						if (eRuleOutType)
						{
							m_RetContainer.Push_Back(eRuleOutType, acName, &vTranslation);						
							bRet = TRUE;
						}
						continue;
					}

					vCheck.x = rwBoundBox.inf.x;
					vCheck.z = rwBoundBox.inf.z;
					if (CheckRuleOut(&eRuleOutType, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
					{
						if (eRuleOutType)
						{
							m_RetContainer.Push_Back(eRuleOutType, acName, &vTranslation);						
							bRet = TRUE;
						}
						continue;
					}

					vCheck.x = rwBoundBox.sup.x;
					vCheck.z = rwBoundBox.inf.z;
					if (CheckRuleOut(&eRuleOutType, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
					{
						if (eRuleOutType)
						{
							m_RetContainer.Push_Back(eRuleOutType, acName, &vTranslation);						
							bRet = TRUE;
						}
						continue;
					}

					vCheck.x = rwBoundBox.inf.x;
					vCheck.z = rwBoundBox.sup.z;
					if (CheckRuleOut(&eRuleOutType, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
					{
						if (eRuleOutType)
						{
							m_RetContainer.Push_Back(eRuleOutType, acName, &vTranslation);						
							bRet = TRUE;
						}
						continue;
					}
				}
			}*/
			if (pProperty)
			{
				RwBBox rwBoundBox;
				if (!GetSRTBBoxFromObjectProperty(&rwBoundBox, pProperty, &vScaling, &vRotation, &vTranslation))
				{
					continue;
				}

				RwV3d	vCheck;
				RwInt32 eRuleOutType = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE;

				vCheck.x = (rwBoundBox.sup.x + rwBoundBox.inf.x) * 0.5f;
				vCheck.z = (rwBoundBox.sup.z + rwBoundBox.inf.z) * 0.5f;
				if (!CheckRuleOut(&eRuleOutType, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
				{
					RwInt32 eRuleOutType1 = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE;
					RwInt32 eRuleOutType2 = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE;
					RwInt32 eRuleOutType3 = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE;
					RwInt32 eRuleOutType4 = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE;

					vCheck.x = rwBoundBox.sup.x;
					vCheck.z = rwBoundBox.sup.z;
					if (CheckRuleOut(&eRuleOutType1, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
					{
						if (eRuleOutType1 == CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE)
						{
							continue;
						}
					}

					vCheck.x = rwBoundBox.inf.x;
					vCheck.z = rwBoundBox.inf.z;
					if (CheckRuleOut(&eRuleOutType2, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
					{
						if (eRuleOutType2 == CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE)
						{
							continue;
						}
					}

					vCheck.x = rwBoundBox.sup.x;
					vCheck.z = rwBoundBox.inf.z;
					if (CheckRuleOut(&eRuleOutType3, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
					{
						if (eRuleOutType3 == CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE)
						{
							continue;
						}
					}

					vCheck.x = rwBoundBox.inf.x;
					vCheck.z = rwBoundBox.sup.z;
					if (CheckRuleOut(&eRuleOutType4, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
					{
						if (eRuleOutType4 == CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE)
						{
							continue;
						}
					}

					eRuleOutType = eRuleOutType1 | eRuleOutType2 | eRuleOutType3 | eRuleOutType4;
				}

				if (eRuleOutType != CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE)
				{
					m_RetContainer.Push_Back(eRuleOutType, acName, &vTranslation);						
					bRet = TRUE;
				}
			}
		}
	}
	
	NTL_ARRAY_DELETE(pvHeightfield);
	return bRet;
}

RwBool CSM_GetRuleOutDoodad::CheckRuleOut(RwInt32* peRuleOutType, RwV3d vPos, RwReal fUpper, RwReal fLower, const RwV3d* pvHeightfield)
{
	*peRuleOutType		= CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE;

	RwInt32 iIndexX		= static_cast<RwInt32>((vPos.x - pvHeightfield[dGET_WORLD_PARAM()->WorldSectorTileNum].x) / dGET_WORLD_PARAM()->WorldSectorTileSize);
	RwInt32 iIndexZ		= static_cast<RwInt32>((vPos.z - pvHeightfield[dGET_WORLD_PARAM()->WorldSectorTileNum].z) / dGET_WORLD_PARAM()->WorldSectorTileSize);

	iIndexX	= abs(iIndexX);
	iIndexZ	= abs(iIndexZ);

	// reverse order
	iIndexX				= dGET_WORLD_PARAM()->WorldSectorTileNum - iIndexX - 1;
	RwInt32 iIndex		= dGET_WORLD_PARAM()->WorldSectorVertNum * iIndexZ + iIndexX;

	if (iIndex >= 0 && iIndex < dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum)
	{
		if (pvHeightfield[iIndex].y + dSM_GETRULEOUTDOODAD_OFFSET_UPPER < fUpper)
		{
			*peRuleOutType = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_UPPER;
		}
		else if (pvHeightfield[iIndex].y - dSM_GETRULEOUTDOODAD_OFFSET_LOWER > fLower)
		{
			*peRuleOutType = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_LOWER;
		}
		return TRUE;
	}

	return FALSE;
}
 
RwBool CSM_GetRuleOutDoodad::GetSRTBBoxFromObjectProperty(RwBBox* pOutBox, CNtlPLObjectProperty* pProperty, const RwV3d* pScaling, const RwV3d* pRotation, const RwV3d* pTranslation)
{
	if (pProperty->GetBBox()->inf.x == 0 &&
		pProperty->GetBBox()->inf.y == 0 &&
		pProperty->GetBBox()->inf.z == 0 &&
		pProperty->GetBBox()->sup.x == 0 &&
		pProperty->GetBBox()->sup.y == 0 &&
		pProperty->GetBBox()->sup.z == 0)
	{
		return FALSE;
	}

	RwV3d vertsIn[8], vertsOut[8];

	vertsIn[0] = pProperty->GetBBox()->inf;
	vertsIn[1] = pProperty->GetBBox()->inf;
	vertsIn[1].z = pProperty->GetBBox()->sup.z;
	vertsIn[2] = vertsIn[1];
	vertsIn[2].x = pProperty->GetBBox()->sup.x;
	vertsIn[3] = pProperty->GetBBox()->inf;
	vertsIn[3].x = pProperty->GetBBox()->sup .x;

	for(int i = 4; i < 8; ++i)
	{
		vertsIn[i] = vertsIn[i - 4];
		vertsIn[i].y = pProperty->GetBBox()->sup.y;
	}

	RwMatrix matLtm;    
	RwMatrixSetIdentity(&matLtm);
	RwMatrixScale(&matLtm, pScaling, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&matLtm, &XAxis, pRotation->x, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&matLtm, &YAxis, pRotation->y, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&matLtm, &ZAxis, pRotation->z, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(&matLtm, pTranslation, rwCOMBINEPOSTCONCAT);

	RwV3dTransformPoints(vertsOut, vertsIn, 8, &matLtm);
	RwBBoxCalculate(pOutBox, vertsOut, 8);

	return TRUE;
}

RwBool CSM_GetRuleOutDoodad::Analyze(DWORD _SectorIdx)
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

	m_RetContainer.Clear();
	if(CheckSector(pPosCurFileMem))
	{
		bRet = TRUE;
	}

	CurFieldFileMem.Free();
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return bRet;
}

RwBool CSM_GetRuleOutDoodad::Report()
{
	CNtlWorldFieldManager*	pMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	FILE*					pFile;

	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	::_chdir("Tool");
	::_chdir("Saber");
	::_chdir("Report");
	::_chdir("Sector Method");

	if(!fopen_s(&pFile, "SM_GET_RULE_OUT_DOODAD.rpt", "wt"))
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
			CSM_GetRuleOutDoodad_RetContainer* pRetContainer = static_cast<CSM_GetRuleOutDoodad_RetContainer*>((*The).second);

			for (CSM_GetRuleOutDoodad_RetContainer::tRET_VEC_ITER it = pRetContainer->Begin(); it != pRetContainer->End(); ++it)
			{
				CSM_GetRuleOutDoodad_RetContainer::SSM_GetRuleOutDoodad* pRet = static_cast<CSM_GetRuleOutDoodad_RetContainer::SSM_GetRuleOutDoodad*>(*it);

				if (pRet->eRuleOutType & CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_UPPER)
				{
					fprintf(pFile, "(UPPER)");
				}
				if (pRet->eRuleOutType & CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_LOWER)
				{
					fprintf(pFile, "(LOWER)");
				}

				fprintf(pFile, " : NAME(%s) POS(%.1f, %.1f, %.1f)\n", pRet->acName, pRet->vTranslation.x, pRet->vTranslation.y, pRet->vTranslation.z);
			}
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

#define dSM_GETRULEOUTDOODAD_OFFSET_UPPER	512.0f
#define dSM_GETRULEOUTDOODAD_OFFSET_LOWER	0.0f

CSM_GetRuleOutDoodad_RetContainer::CSM_GetRuleOutDoodad_RetContainer()
{
}

CSM_GetRuleOutDoodad_RetContainer::~CSM_GetRuleOutDoodad_RetContainer()
{
	Clear();
}

void CSM_GetRuleOutDoodad_RetContainer::Clear()
{
	for (tRET_VEC_ITER it = m_vecRet.begin(); it != m_vecRet.end(); ++it)
	{
		SSM_GetRuleOutDoodad* pRuleOutDoodad = *it;
		if (pRuleOutDoodad)
		{
			NTL_DELETE(pRuleOutDoodad);
		}
	}

	m_vecRet.clear();
}

void CSM_GetRuleOutDoodad_RetContainer::Push_Back(SSM_GetRuleOutDoodad* pRuleOutDoodad)
{
	SSM_GetRuleOutDoodad* pRuleOutDoodadNew	= NTL_NEW SSM_GetRuleOutDoodad;
	CopyMemory(pRuleOutDoodadNew->acName, pRuleOutDoodad->acName, sizeof(RwChar) * 128);
	pRuleOutDoodadNew->eRuleOutType	= pRuleOutDoodad->eRuleOutType;
	pRuleOutDoodadNew->vTranslation	= pRuleOutDoodad->vTranslation;

	m_vecRet.push_back(pRuleOutDoodadNew);
}

void CSM_GetRuleOutDoodad_RetContainer::Push_Back(const RwUInt32 eRuleOutType, const RwChar* pName, const RwV3d* pvTranslation)
{
	SSM_GetRuleOutDoodad* pRuleOutDoodadNew	= NTL_NEW SSM_GetRuleOutDoodad;
	CopyMemory(pRuleOutDoodadNew->acName, pName, sizeof(RwChar) * 128);
	pRuleOutDoodadNew->eRuleOutType	= eRuleOutType;
	pRuleOutDoodadNew->vTranslation	= *pvTranslation;

	m_vecRet.push_back(pRuleOutDoodadNew);
}


void CSM_GetRuleOutDoodad_RetContainer::Copy(CSM_GetRuleOutDoodad_RetContainer* pRuleOutDoodad_RetContainer)
{
	Clear();
	for (tRET_VEC_ITER it = pRuleOutDoodad_RetContainer->Begin(); it != pRuleOutDoodad_RetContainer->End(); ++it)
	{
		SSM_GetRuleOutDoodad* pRuleOutDoodad = static_cast<SSM_GetRuleOutDoodad*>(*it);
		Push_Back(pRuleOutDoodad);	
	}
}

CSM_GetRuleOutDoodad::CSM_GetRuleOutDoodad()
{
}

CSM_GetRuleOutDoodad::~CSM_GetRuleOutDoodad()
{
	DestroySelfRetMap();
}

RwBool CSM_GetRuleOutDoodad::DestroySelfRetMap()
{
	ISectorMethod::tSELF_RET_MAP_ITER The = m_SelfRetMap.begin();
	while(The != m_SelfRetMap.end())
	{
		CSM_GetRuleOutDoodad_RetContainer* pDel = static_cast<CSM_GetRuleOutDoodad_RetContainer*>((*The).second);
		NTL_DELETE(pDel);

		++The;
	}
	m_SelfRetMap.clear();

	return TRUE;
}

RwBool CSM_GetRuleOutDoodad::Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI)
{
	return TRUE;
}

VOID* CSM_GetRuleOutDoodad::GetAnalysis()
{
	CSM_GetRuleOutDoodad_RetContainer* _pRet = NTL_NEW CSM_GetRuleOutDoodad_RetContainer;

	_pRet->Copy(&m_RetContainer);

	return static_cast<VOID*>(_pRet);
}

RwBool CSM_GetRuleOutDoodad::CheckSector(BYTE* _pFileMemPos)
{
	/*
	WorldFileFormat - SectorMethod
	*/

	RwInt32		iWorldSectorVertNum		= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;
	RwV3d*		pvHeightfield			= NTL_NEW RwV3d[iWorldSectorVertNum];
	RwBool		bRet					= FALSE;
	RwInt32		iNumDoodads				= 0;

	_pFileMemPos = Skip_Material(_pFileMemPos);

	CopyMemory(pvHeightfield, _pFileMemPos, sizeof(RwV3d) * iWorldSectorVertNum);
	_pFileMemPos += (sizeof(RwV3d) * iWorldSectorVertNum);

	_pFileMemPos = Skip_Diffuse(_pFileMemPos);

	CopyMemory(&iNumDoodads, _pFileMemPos, sizeof(RwInt32));
	_pFileMemPos += (sizeof(RwInt32));

	for (int i = 0; i < iNumDoodads; ++i)
	{
		RwChar	acName[128];
		RwV3d	vScaling;
		RwV3d	vRotation;
		RwV3d	vTranslation;

		//////////////////////////////////////////////////////////////////////////
		// 필요한 정보를 복사해두고 파일포인터를 스킵해둔다.
		//////////////////////////////////////////////////////////////////////////
		// Use : Name
		CopyMemory(acName, _pFileMemPos, sizeof(RwChar) * 128);
		_pFileMemPos += (sizeof(RwChar) * 128);

		// Use : Scaling
		CopyMemory(&vScaling, _pFileMemPos, sizeof(RwV3d));
		_pFileMemPos += (sizeof(RwV3d));

		// Use : Rotation
		CopyMemory(&vRotation, _pFileMemPos, sizeof(RwV3d));
		_pFileMemPos += (sizeof(RwV3d));

		// Use : Translation
		CopyMemory(&vTranslation, _pFileMemPos, sizeof(RwV3d));
		_pFileMemPos += (sizeof(RwV3d));

		RwUInt32 uiObjectType;
		CopyMemory(&uiObjectType, _pFileMemPos, sizeof(RwUInt32));
		_pFileMemPos += sizeof(RwUInt32);

		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
				(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
				sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
				(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

			_pFileMemPos += iPropertyRange;
		}

		// Skip : trigger obj serial id : object serial id : Obj. name index : visibility distance : enable trigger : 
		// enable selection : enable alterable : enable portal : enable path engine collision : enable shadowprop
		_pFileMemPos += (sizeof(RwUInt32)) +
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
			(sizeof(RwUInt32)) +
#endif
			(sizeof(DWORD)) + (sizeof(RwReal)) + (sizeof(RwBool)) +
			(sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool));

		// Skip : Projection Shadow Obj
		RwInt32 PSAtomicCnt = 0;
		CopyMemory(&PSAtomicCnt, _pFileMemPos, sizeof(RwInt32));
		_pFileMemPos += (sizeof(RwInt32));
		for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
		{
			// Map name
			_pFileMemPos += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

			// Frustum view window extent
			_pFileMemPos += (sizeof(RwReal));

			// Light matrices
			_pFileMemPos += (sizeof(RwMatrix));

			// Map size
			RwInt32 Size = 0;
			CopyMemory(&Size, _pFileMemPos, sizeof(RwInt32));
			_pFileMemPos += (sizeof(RwInt32));

			// Map
			_pFileMemPos += (sizeof(DWORD) * Size * Size);
		}

		// Skip : milepost
		RwBool EnableFlg;
		CopyMemory(&EnableFlg, _pFileMemPos, sizeof(RwBool));
		_pFileMemPos += sizeof(RwBool);

		if(EnableFlg)
		{
			_pFileMemPos += sizeof(RwUInt32);
		}

		// attach light object skip
		RwInt32 iLightObjectNum;
		CopyMemory(&iLightObjectNum, _pFileMemPos, sizeof(RwInt32));
		_pFileMemPos += sizeof(RwInt32);

		for (int j = 0; j < iLightObjectNum; ++j)
		{
			_pFileMemPos += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

			_pFileMemPos += sizeof(RpLightType);
			_pFileMemPos += sizeof(RwBool);

			_pFileMemPos += sizeof(RwV3d);

			_pFileMemPos += sizeof(RwV3d);
			_pFileMemPos += sizeof(RwRGBAReal);
			_pFileMemPos += sizeof(RwReal);
			_pFileMemPos += sizeof(RwReal);

			_pFileMemPos += sizeof(RwBool);
			_pFileMemPos += sizeof(RwBool);
		}

		//////////////////////////////////////////////////////////////////////////
		// 실질적인 검사를 한다.
		//////////////////////////////////////////////////////////////////////////
		// Doodad Check
		{
			CNtlPLObjectProperty* pProperty = (CNtlPLObjectProperty*)CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_OBJECT, acName);

			/*if (pProperty)
			{
			RwBBox rwBoundBox;
			if (!GetSRTBBoxFromObjectProperty(&rwBoundBox, pProperty, &vScaling, &vRotation, &vTranslation))
			{
			continue;
			}

			RwV3d	vCheck;
			RwInt32 eRuleOutType = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE;

			vCheck.x = (rwBoundBox.sup.x + rwBoundBox.inf.x) * 0.5f;
			vCheck.z = (rwBoundBox.sup.z + rwBoundBox.inf.z) * 0.5f;
			if (CheckRuleOut(&eRuleOutType, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
			{
			if (eRuleOutType)
			{
			m_RetContainer.Push_Back(eRuleOutType, acName, &vTranslation);						
			bRet = TRUE;
			}
			continue;
			}
			else
			{
			vCheck.x = rwBoundBox.sup.x;
			vCheck.z = rwBoundBox.sup.z;
			if (CheckRuleOut(&eRuleOutType, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
			{
			if (eRuleOutType)
			{
			m_RetContainer.Push_Back(eRuleOutType, acName, &vTranslation);						
			bRet = TRUE;
			}
			continue;
			}

			vCheck.x = rwBoundBox.inf.x;
			vCheck.z = rwBoundBox.inf.z;
			if (CheckRuleOut(&eRuleOutType, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
			{
			if (eRuleOutType)
			{
			m_RetContainer.Push_Back(eRuleOutType, acName, &vTranslation);						
			bRet = TRUE;
			}
			continue;
			}

			vCheck.x = rwBoundBox.sup.x;
			vCheck.z = rwBoundBox.inf.z;
			if (CheckRuleOut(&eRuleOutType, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
			{
			if (eRuleOutType)
			{
			m_RetContainer.Push_Back(eRuleOutType, acName, &vTranslation);						
			bRet = TRUE;
			}
			continue;
			}

			vCheck.x = rwBoundBox.inf.x;
			vCheck.z = rwBoundBox.sup.z;
			if (CheckRuleOut(&eRuleOutType, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
			{
			if (eRuleOutType)
			{
			m_RetContainer.Push_Back(eRuleOutType, acName, &vTranslation);						
			bRet = TRUE;
			}
			continue;
			}
			}
			}*/
			if (pProperty)
			{
				RwBBox rwBoundBox;
				if (!GetSRTBBoxFromObjectProperty(&rwBoundBox, pProperty, &vScaling, &vRotation, &vTranslation))
				{
					continue;
				}

				RwV3d	vCheck;
				RwInt32 eRuleOutType = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE;

				vCheck.x = (rwBoundBox.sup.x + rwBoundBox.inf.x) * 0.5f;
				vCheck.z = (rwBoundBox.sup.z + rwBoundBox.inf.z) * 0.5f;
				if (!CheckRuleOut(&eRuleOutType, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
				{
					RwInt32 eRuleOutType1 = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE;
					RwInt32 eRuleOutType2 = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE;
					RwInt32 eRuleOutType3 = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE;
					RwInt32 eRuleOutType4 = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE;

					vCheck.x = rwBoundBox.sup.x;
					vCheck.z = rwBoundBox.sup.z;
					if (CheckRuleOut(&eRuleOutType1, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
					{
						if (eRuleOutType1 == CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE)
						{
							continue;
						}
					}

					vCheck.x = rwBoundBox.inf.x;
					vCheck.z = rwBoundBox.inf.z;
					if (CheckRuleOut(&eRuleOutType2, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
					{
						if (eRuleOutType2 == CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE)
						{
							continue;
						}
					}

					vCheck.x = rwBoundBox.sup.x;
					vCheck.z = rwBoundBox.inf.z;
					if (CheckRuleOut(&eRuleOutType3, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
					{
						if (eRuleOutType3 == CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE)
						{
							continue;
						}
					}

					vCheck.x = rwBoundBox.inf.x;
					vCheck.z = rwBoundBox.sup.z;
					if (CheckRuleOut(&eRuleOutType4, vCheck, rwBoundBox.inf.y, rwBoundBox.sup.y, pvHeightfield))
					{
						if (eRuleOutType4 == CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE)
						{
							continue;
						}
					}

					eRuleOutType = eRuleOutType1 | eRuleOutType2 | eRuleOutType3 | eRuleOutType4;
				}

				if (eRuleOutType != CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE)
				{
					m_RetContainer.Push_Back(eRuleOutType, acName, &vTranslation);						
					bRet = TRUE;
				}
			}
		}
	}

	NTL_ARRAY_DELETE(pvHeightfield);
	return bRet;
}

RwBool CSM_GetRuleOutDoodad::CheckRuleOut(RwInt32* peRuleOutType, RwV3d vPos, RwReal fUpper, RwReal fLower, const RwV3d* pvHeightfield)
{
	*peRuleOutType		= CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_NONE;

	RwInt32 iIndexX		= static_cast<RwInt32>((vPos.x - pvHeightfield[dGET_WORLD_PARAM()->WorldSectorTileNum].x) / dGET_WORLD_PARAM()->WorldSectorTileSize);
	RwInt32 iIndexZ		= static_cast<RwInt32>((vPos.z - pvHeightfield[dGET_WORLD_PARAM()->WorldSectorTileNum].z) / dGET_WORLD_PARAM()->WorldSectorTileSize);

	iIndexX	= abs(iIndexX);
	iIndexZ	= abs(iIndexZ);

	// reverse order
	iIndexX				= dGET_WORLD_PARAM()->WorldSectorTileNum - iIndexX - 1;
	RwInt32 iIndex		= dGET_WORLD_PARAM()->WorldSectorVertNum * iIndexZ + iIndexX;

	if (iIndex >= 0 && iIndex < dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum)
	{
		if (pvHeightfield[iIndex].y + dSM_GETRULEOUTDOODAD_OFFSET_UPPER < fUpper)
		{
			*peRuleOutType = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_UPPER;
		}
		else if (pvHeightfield[iIndex].y - dSM_GETRULEOUTDOODAD_OFFSET_LOWER > fLower)
		{
			*peRuleOutType = CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_LOWER;
		}
		return TRUE;
	}

	return FALSE;
}

RwBool CSM_GetRuleOutDoodad::GetSRTBBoxFromObjectProperty(RwBBox* pOutBox, CNtlPLObjectProperty* pProperty, const RwV3d* pScaling, const RwV3d* pRotation, const RwV3d* pTranslation)
{
	if (pProperty->GetBBox()->inf.x == 0 &&
		pProperty->GetBBox()->inf.y == 0 &&
		pProperty->GetBBox()->inf.z == 0 &&
		pProperty->GetBBox()->sup.x == 0 &&
		pProperty->GetBBox()->sup.y == 0 &&
		pProperty->GetBBox()->sup.z == 0)
	{
		return FALSE;
	}

	RwV3d vertsIn[8], vertsOut[8];

	vertsIn[0] = pProperty->GetBBox()->inf;
	vertsIn[1] = pProperty->GetBBox()->inf;
	vertsIn[1].z = pProperty->GetBBox()->sup.z;
	vertsIn[2] = vertsIn[1];
	vertsIn[2].x = pProperty->GetBBox()->sup.x;
	vertsIn[3] = pProperty->GetBBox()->inf;
	vertsIn[3].x = pProperty->GetBBox()->sup .x;

	for(int i = 4; i < 8; ++i)
	{
		vertsIn[i] = vertsIn[i - 4];
		vertsIn[i].y = pProperty->GetBBox()->sup.y;
	}

	RwMatrix matLtm;    
	RwMatrixSetIdentity(&matLtm);
	RwMatrixScale(&matLtm, pScaling, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&matLtm, &XAxis, pRotation->x, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&matLtm, &YAxis, pRotation->y, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&matLtm, &ZAxis, pRotation->z, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(&matLtm, pTranslation, rwCOMBINEPOSTCONCAT);

	RwV3dTransformPoints(vertsOut, vertsIn, 8, &matLtm);
	RwBBoxCalculate(pOutBox, vertsOut, 8);

	return TRUE;
}

RwBool CSM_GetRuleOutDoodad::Analyze(DWORD _SectorIdx)
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

	m_RetContainer.Clear();
	if(CheckSector(pPosCurFileMem))
	{
		bRet = TRUE;
	}

	CurFieldFileMem.Free();
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return bRet;
}

RwBool CSM_GetRuleOutDoodad::Report()
{
	CNtlWorldFieldManager*	pMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	FILE*					pFile;

	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	::_chdir("Tool");
	::_chdir("Saber");
	::_chdir("Report");
	::_chdir("Sector Method");

	if(!fopen_s(&pFile, "SM_GET_RULE_OUT_DOODAD.rpt", "wt"))
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
			CSM_GetRuleOutDoodad_RetContainer* pRetContainer = static_cast<CSM_GetRuleOutDoodad_RetContainer*>((*The).second);

			for (CSM_GetRuleOutDoodad_RetContainer::tRET_VEC_ITER it = pRetContainer->Begin(); it != pRetContainer->End(); ++it)
			{
				CSM_GetRuleOutDoodad_RetContainer::SSM_GetRuleOutDoodad* pRet = static_cast<CSM_GetRuleOutDoodad_RetContainer::SSM_GetRuleOutDoodad*>(*it);

				if (pRet->eRuleOutType & CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_UPPER)
				{
					fprintf(pFile, "(UPPER)");
				}
				if (pRet->eRuleOutType & CSM_GetRuleOutDoodad_RetContainer::eRULEOUT_TYPE_LOWER)
				{
					fprintf(pFile, "(LOWER)");
				}

				fprintf(pFile, " : NAME(%s) POS(%.1f, %.1f, %.1f)\n", pRet->acName, pRet->vTranslation.x, pRet->vTranslation.y, pRet->vTranslation.z);
			}
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