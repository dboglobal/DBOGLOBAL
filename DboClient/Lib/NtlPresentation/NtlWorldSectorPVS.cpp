#include "precomp_ntlpresentation.h"
#include "NtlWorldSectorPVS.h"
#include "NtlDebug.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLSceneManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLObject.h"
#include "ntlworldinterface.h"
#include "NtlWorldBrush.h"
#include "NtlWorldBrushLine.h"
#include "NtlPLRenderState.h"
#include "NtlPLOccluder_Base.h"
#include "NtlPLOccluder_Quad.h"


RwImage*	CNtlWorldSectorPVS::m_pPVSAreaFilter	= NULL;
RwInt32		CNtlWorldSectorPVS::m_LockedPVSSector	= -999;


CNtlWorldSectorPVS::CNtlWorldSectorPVS(void)
{
	m_Visibility			= TRUE;
	m_pVisibilityNeighbor	= NULL;
	m_pPVSAreaFilter		= NULL;
}

CNtlWorldSectorPVS::~CNtlWorldSectorPVS(void)
{
	if(m_pPVSAreaFilter)
	{
		RwImageDestroy(m_pPVSAreaFilter);
		m_pPVSAreaFilter = NULL;
	}

	NTL_DELETE(m_pVisibilityNeighbor);
}

VOID CNtlWorldSectorPVS::SetVisibilityNeighbor(BYTE* _VisibilityNeighbor)
{
	NTL_DELETE(m_pVisibilityNeighbor);

	m_pVisibilityNeighbor = NTL_NEW BYTE [dPVS_TOT_CELL_CNT * dPVS_LAYER_CNT];

	CopyMemory(m_pVisibilityNeighbor, _VisibilityNeighbor, sizeof(BYTE) * dPVS_TOT_CELL_CNT * dPVS_LAYER_CNT);
}

VOID CNtlWorldSectorPVS::SetVisibility(RwInt32 _SectorIdx, RwBool _Visible)
{ 
	NTL_ASSERTE(_SectorIdx != -1);

	RwBool					bCulled	= !_Visible;

	CNtlWorldFieldManager*	pFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlPLObject*			pObjTmp;

	// doodads
	RwInt32 NumDoodads = static_cast<RwInt32>(pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityOrnament.size());

	for(RwInt32 j = 0; j < NumDoodads; ++j)
	{
		pObjTmp = static_cast<CNtlPLObject*>(pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityOrnament[j]);

		/*if(pObjTmp->GetFlags() & NTL_PLEFLAG_TRIGGER)
		{
			continue;
		}*/

		CNtlPLObjectProperty* pProperty = pObjTmp->GetProperty();
		if (!pProperty->GetApplyPVS())
		{
			continue;
		}

		//pObjTmp->SetVisible(_Visible);
		//pObjTmp->SetVisiblePVS(_Visible);
		pObjTmp->SetCullFlags(NTL_PLEFLAG_CULLED_PVS, bCulled);
	}

	// effects
	RwInt32 NumEffects = static_cast<RwInt32>(pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityEffect.size());

	for(RwInt32 j = 0; j < NumEffects; ++j)
	{
		//pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityEffect[j]->SetVisible(_Visible);
		//pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityEffect[j]->SetVisiblePVS(_Visible);
		pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityEffect[j]->SetCullFlags(NTL_PLEFLAG_CULLED_PVS, bCulled);
	}

	RwInt32 NumDecals = static_cast<RwInt32>(pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityDecal.size());

	// decals
	for(RwInt32 j = 0; j < NumDecals; ++j)
	{
		//pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityDecal[j]->SetVisible(_Visible);
		//pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityDecal[j]->SetVisiblePVS(_Visible);
		pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityDecal[j]->SetCullFlags(NTL_PLEFLAG_CULLED_PVS, bCulled);

	}

	// shoreline
	if(pFMgr->m_pSectors[_SectorIdx].m_pNtlPLEntityShoreLine)
	{
		//pFMgr->m_pSectors[_SectorIdx].m_pNtlPLEntityShoreLine->SetVisible(_Visible);
		//pFMgr->m_pSectors[_SectorIdx].m_pNtlPLEntityShoreLine->SetVisiblePVS(_Visible);
		pFMgr->m_pSectors[_SectorIdx].m_pNtlPLEntityShoreLine->SetCullFlags(NTL_PLEFLAG_CULLED_PVS, bCulled);
	}

	// terrain & water
	m_Visibility = _Visible;
}

RwInt32 CNtlWorldSectorPVS::GetVisibilityNeighbor(RwInt32 _NeighborIdx, RwInt32 _iLayer)
{
#ifdef dNTL_WORLD_TOOL_MODE
	if (CNtlPLGlobal::m_iCurrentPVSLayer >= 0)
	{
		_iLayer = CNtlPLGlobal::m_iCurrentPVSLayer;
	}
#endif

	if(_NeighborIdx < 0 || _NeighborIdx > dPVS_TOT_CELL_CNT)
	{
		NTL_ASSERTE(_NeighborIdx > 0 && _NeighborIdx < dPVS_TOT_CELL_CNT);
	}	
	return m_pVisibilityNeighbor[_NeighborIdx + dPVS_TOT_CELL_CNT * _iLayer];
}

RwBool CNtlWorldSectorPVS::TestPVS(RwInt32 _SrcSecIdx, RwInt32 _DstSecIdx, RwInt32 _CurArrayIdx, RwReal fEpsilsonHeight)
{
	CNtlWorldFieldManager* pFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	RwV3d SrcSPos, DstSPos;
	RwV3d SrcMPos, DstMPos;
	CNtlMath::MathRwV3dAssign(&SrcMPos, pFMgr->m_pSectors[_SrcSecIdx].DatumPoint.x, pFMgr->m_pSectors[_SrcSecIdx].DatumPoint.y, pFMgr->m_pSectors[_SrcSecIdx].DatumPoint.z);
	CNtlMath::MathRwV3dAssign(&DstMPos, pFMgr->m_pSectors[_DstSecIdx].DatumPoint.x, pFMgr->m_pSectors[_DstSecIdx].DatumPoint.y, pFMgr->m_pSectors[_DstSecIdx].DatumPoint.z);
	SrcSPos.x = SrcMPos.x - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
	SrcSPos.z = SrcMPos.z - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
	DstSPos.x = DstMPos.x - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
	DstSPos.z = DstMPos.z - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;

	RwReal	PermittedEpsilon	= 0.1f;
	RwV3d	CurSrcVert;
	RwV3d	CurDstVert;
	RwV3d	CurDir;
	RwV3d   CurPosVert;
	RwReal	CurDist, FinDist;
	RwInt32	CurTestCnt;
	RwReal	CurSkipExtent = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize) * 8.0f;
	RwInt32	CurSrcSecIdx;
	RwInt32	CurDstSecIdx;
	RwInt32	CurPosSecIdx;

	RwReal	afLayerEpsilsonHeight[dPVS_LAYER_CNT];
	RwBool	abLayerTestResult[dPVS_LAYER_CNT];

	for (RwInt32 iLayerCnt = dPVS_LAYER_CNT - 1; iLayerCnt >= 0; --iLayerCnt)
	{
		afLayerEpsilsonHeight[iLayerCnt]	= fEpsilsonHeight + (iLayerCnt * 10);
		abLayerTestResult[iLayerCnt]		= TRUE;
	}

	// create source vertices and test with all of the destination ones
	for(RwReal CurSrcSPosZ = SrcSPos.z; CurSrcSPosZ < dGET_WORLD_PARAM()->WorldSectorSize + SrcSPos.z + PermittedEpsilon; CurSrcSPosZ += CurSkipExtent)
	{
		for(RwReal CurSrcSPosX = SrcSPos.x; CurSrcSPosX < dGET_WORLD_PARAM()->WorldSectorSize + SrcSPos.x + PermittedEpsilon; CurSrcSPosX += CurSkipExtent)
		{
			CurSrcVert.x = CurSrcSPosX; 		
			CurSrcVert.z = CurSrcSPosZ;
			pFMgr->GetHeight(CurSrcVert);
			CurSrcSecIdx = pFMgr->GetSectorIdx(CurSrcVert);

			// make sure that we'd better using the shorter line extent check 'cos of the accuracy of PVS test
			for(RwReal CurDstSPosZ = DstSPos.z; CurDstSPosZ < dGET_WORLD_PARAM()->WorldSectorSize + DstSPos.z + PermittedEpsilon; CurDstSPosZ += CurSkipExtent)
			{
				for(RwReal CurDstSPosX = DstSPos.x; CurDstSPosX < dGET_WORLD_PARAM()->WorldSectorSize + DstSPos.x + PermittedEpsilon; CurDstSPosX += CurSkipExtent)
				{
					CurDstVert.x = CurDstSPosX;
					CurDstVert.z = CurDstSPosZ;
					pFMgr->GetHeight(CurDstVert);
					CurDstSecIdx = pFMgr->GetSectorIdx(CurDstVert);

					RwV3dSubMacro(&CurDir, &CurDstVert, &CurSrcVert);
					RwV3dNormalize(&CurDir, &CurDir);

					CurTestCnt	= 1; 
					CurDist		= RwRealMINVAL;
					FinDist		= RwV3dLength(&(CurDstVert - CurSrcVert));


					RwBool abTestResult[dPVS_LAYER_CNT];
					for (RwInt32 iLayerCnt = 0; iLayerCnt < dPVS_LAYER_CNT; ++iLayerCnt)
					{
						abTestResult[iLayerCnt] = FALSE;
					}
					
					while(CurDist < FinDist)
					{
						CurPosVert = CurSrcVert;
						RwV3dIncrementScaled(&CurPosVert, &CurDir, static_cast<RwReal>(dPVS_LINE_TEST_EXTENT) * static_cast<RwReal>(CurTestCnt));

						CurDist	= RwV3dLength(&(CurPosVert - CurSrcVert));

						pFMgr->GetHeight(CurPosVert);
						CurPosSecIdx = pFMgr->GetSectorIdx(CurPosVert);

						// no need to create a occluder point
						if(CurPosSecIdx == CurSrcSecIdx || CurPosSecIdx == CurDstSecIdx)
						{
							++CurTestCnt;
							continue;							
						}

						RwBool	bLastTestPVS	= FALSE;
						RwBool	bAllClearPVS	= TRUE;
						for (RwInt32 iLayerCnt = 0; iLayerCnt < dPVS_LAYER_CNT; ++iLayerCnt)
						{
							if (abLayerTestResult[iLayerCnt] && !abTestResult[iLayerCnt])
							{
								if (bLastTestPVS)
								{
									abTestResult[iLayerCnt] = bLastTestPVS;
								}
								else
								{
									if(CurPosVert.y > CurDstVert.y + afLayerEpsilsonHeight[iLayerCnt] && CurPosVert.y > CurSrcVert.y + fEpsilsonHeight + afLayerEpsilsonHeight[iLayerCnt])
									{
										abTestResult[iLayerCnt] = bLastTestPVS = TRUE;
									}
									else
									{
										bAllClearPVS = FALSE;
									}
								}
							}
						}

						if (bAllClearPVS)
						{
							break;
						}

						++CurTestCnt;
					}

					RwBool	bLastTestOcc	= FALSE;
					for (RwInt32 iLayerCnt = 0; iLayerCnt < dPVS_LAYER_CNT; ++iLayerCnt)
					{
						if (bLastTestOcc)
						{
							abTestResult[iLayerCnt] = bLastTestOcc;
						}
						else
						{
							if (abLayerTestResult[iLayerCnt] && !abTestResult[iLayerCnt])
							{
								RwV3d CurSrcVertOcc = CurSrcVert; CurSrcVertOcc.y += afLayerEpsilsonHeight[iLayerCnt];
								RwV3d CurDstVertOcc = CurDstVert; CurDstVertOcc.y += afLayerEpsilsonHeight[iLayerCnt];

								for(RwInt32 i = -dPVS_SECTOR_EXTENT_CNT; i < dPVS_SECTOR_EXTENT_CNT + 1; ++i)
								{
									for(RwInt32 j = -dPVS_SECTOR_EXTENT_CNT; j < dPVS_SECTOR_EXTENT_CNT + 1; ++j)
									{
										RwV3d	vTemp;
										RwInt32	iIndex;

										vTemp.x	= SrcMPos.x + j * dGET_WORLD_PARAM()->WorldSectorSize;
										vTemp.z	= SrcMPos.z + i * dGET_WORLD_PARAM()->WorldSectorSize;
										iIndex	= pFMgr->GetSectorIdx(vTemp);

										if (iIndex == -1)
										{
											continue;
										}

										if(iIndex == CurSrcSecIdx || iIndex == CurDstSecIdx)
										{
											continue;
										}

										CNtlWorldSector* pSector = &pFMgr->m_pSectors[iIndex];
										for (int k = 0; k < (int)pSector->m_vecNtlPLEntityOccluder_Quad.size(); ++k)
										{
											CNtlPLOccluder_Base* pOccluder = (CNtlPLOccluder_Base*)pSector->m_vecNtlPLEntityOccluder_Quad.at(k);									

											if (pOccluder->IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS) && pOccluder->PVSTest(&CurSrcVertOcc, &CurDstVertOcc))
											{
												abTestResult[iLayerCnt] = bLastTestOcc = TRUE;
												break;
											}
										}

										if (bLastTestOcc)
										{
											break;
										}
									}
									if (bLastTestOcc)
									{
										break;
									}
								}
							}
						}
					}

					// fail & return once TestResult is FALSE
					RwBool bAllFailed = TRUE;
					for (RwInt32 iLayerCnt = 0; iLayerCnt < dPVS_LAYER_CNT; ++iLayerCnt)
					{
						if (!abTestResult[iLayerCnt])
						{
							abLayerTestResult[iLayerCnt] = FALSE;
						}
						else
						{
							bAllFailed = FALSE;
						}
					}
					if(bAllFailed)
					{
						return FALSE;
					}
				}
			}	
		}
	}

	for (RwInt32 iLayerCnt = 0; iLayerCnt < dPVS_LAYER_CNT; ++iLayerCnt)
	{
		if (abLayerTestResult[iLayerCnt])
		{
			m_pVisibilityNeighbor[_CurArrayIdx + (iLayerCnt * dPVS_TOT_CELL_CNT)] = 0;
		}
		else
		{
			m_pVisibilityNeighbor[_CurArrayIdx + (iLayerCnt * dPVS_TOT_CELL_CNT)] = 1;
		}		
	}

  	return TRUE;
}

VOID CNtlWorldSectorPVS::CreatePVS(RwInt32 _SectorIdx, RwReal fEpsilsonHeight)
{
	CNtlWorldFieldManager* pFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	DestroyPVS(_SectorIdx);

	// initialize; all of the neighbors are visible
	m_Visibility			= TRUE;
	m_pVisibilityNeighbor	= NTL_NEW BYTE [dPVS_TOT_CELL_CNT * dPVS_LAYER_CNT];

	RwInt32 TmpCnt = dPVS_TOT_CELL_CNT * dPVS_LAYER_CNT;
	while(TmpCnt--)
	{
		m_pVisibilityNeighbor[TmpCnt] = 1;
	}

	// self index check first and PVS test
	RwV3d Pos;
	CNtlMath::MathRwV3dAssign(&Pos, pFMgr->m_pSectors[_SectorIdx].DatumPoint.x, pFMgr->m_pSectors[_SectorIdx].DatumPoint.y, pFMgr->m_pSectors[_SectorIdx].DatumPoint.z);

	RwInt32 CellCnt = dPVS_SECTOR_EXTENT_CNT + dPVS_SECTOR_EXTENT_CNT + 1;
	RwInt32 SelfIdx	= static_cast<RwInt32>(static_cast<RwReal>(dPVS_TOT_CELL_CNT) / 2.0f);

	RwV3d SPos;
	SPos.x = Pos.x - (dPVS_SECTOR_EXTENT_CNT * dGET_WORLD_PARAM()->WorldSectorSize);
	SPos.z = Pos.z - (dPVS_SECTOR_EXTENT_CNT * dGET_WORLD_PARAM()->WorldSectorSize);

	RwInt32 CurSectorIdx;
	RwV3d	CurSectorPos;
	RwInt32	CurArrayIdx;
	RwBool	CurTestResult = FALSE;
	RwV3d	CurTmp;
	RwReal	CurNeighborSecDist;

	// The dist. would be bigger than the real one
	RwReal	CurDistSec2SecAtMiddle = (dGET_WORLD_PARAM()->WorldSectorSize / 2.0f) * sqrtf(2.0f) + (dGET_WORLD_PARAM()->WorldSectorSize / 2.0f) * sqrtf(2.0f);

	for(RwInt32 i = 0; i < CellCnt; ++i)
	{
		for(RwInt32 j = 0; j < CellCnt; ++j)
		{
			CurSectorPos.x	= SPos.x + j * dGET_WORLD_PARAM()->WorldSectorSize;
			CurSectorPos.z	= SPos.z + i * dGET_WORLD_PARAM()->WorldSectorSize;
			CurSectorIdx	= pFMgr->GetSectorIdx(CurSectorPos);
			CurArrayIdx		= i * CellCnt + j;

			// exception handling
			if(CurSectorIdx == -1)
			{
				for (RwInt32 iLayerCnt = 0; iLayerCnt < dPVS_LAYER_CNT; ++iLayerCnt)
				{
					m_pVisibilityNeighbor[CurArrayIdx + (iLayerCnt* dPVS_TOT_CELL_CNT)] = 1;
				}
				continue;
			}

			// self process
			if(CurSectorIdx == _SectorIdx)
			{
				for (RwInt32 iLayerCnt = 0; iLayerCnt < dPVS_LAYER_CNT; ++iLayerCnt)
				{
					m_pVisibilityNeighbor[CurArrayIdx + (iLayerCnt* dPVS_TOT_CELL_CNT)] = 1;
				}
				continue;
			}

			// the closest neighbors which don't need to be analyzed
			CNtlMath::MathRwV3dAssign(&CurTmp, pFMgr->m_pSectors[CurSectorIdx].DatumPoint.x, pFMgr->m_pSectors[CurSectorIdx].DatumPoint.y, pFMgr->m_pSectors[CurSectorIdx].DatumPoint.z);
			CurNeighborSecDist = RwV3dLength(&(Pos - CurTmp));
			
			if(CurNeighborSecDist < CurDistSec2SecAtMiddle)
			{
				for (RwInt32 iLayerCnt = 0; iLayerCnt < dPVS_LAYER_CNT; ++iLayerCnt)
				{
					m_pVisibilityNeighbor[CurArrayIdx + (iLayerCnt* dPVS_TOT_CELL_CNT)] = 1;
				}
				continue;
			}

			// PVS test and result
			TestPVS(_SectorIdx, CurSectorIdx, CurArrayIdx, fEpsilsonHeight);			
		}
	}
}

VOID CNtlWorldSectorPVS::DestroyPVS(RwInt32 _SectorIdx)
{
	CNtlWorldFieldManager* pFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	m_Visibility = TRUE;

	NTL_DELETE(m_pVisibilityNeighbor);
}

VOID CNtlWorldSectorPVS::SetPVSAreaFilter(RwImage* _pPVSAreaFilter)
{
	if(m_pPVSAreaFilter)
	{
		RwImageDestroy(m_pPVSAreaFilter);
		m_pPVSAreaFilter = NULL;
	}

	if(_pPVSAreaFilter)
	{
		m_pPVSAreaFilter = _pPVSAreaFilter;
	}
}

RwInt32	CNtlWorldSectorPVS::GetPVSAreaTotNum()
{
	if(!m_pPVSAreaFilter)
	{
		return -1;
	}

	RwRGBA	RGBA;
	DWORD*	pDest;
	DWORD	Texel;
	RwInt32 TexelCnt		= m_pPVSAreaFilter->width;
	DWORD*	pBits			= (DWORD*)RwImageGetPixels(m_pPVSAreaFilter);
	RwInt32	TotPVSAreaCnt	= 0;

	for(RwInt32 i = 0; i < TexelCnt * TexelCnt; ++i)
	{
		pDest		= pBits + i;
		Texel		= (*pDest);
		RGBA.alpha	= (RwUInt8)((Texel & 0xff000000) >> 24);
		RGBA.blue	= (RwUInt8)((Texel & 0x00ff0000) >> 16);
		RGBA.green	= (RwUInt8)((Texel & 0x0000ff00) >> 8);
		RGBA.red	= (RwUInt8)((Texel & 0x000000ff) >> 0);

		if(RGBA.red || RGBA.green || RGBA.blue)
		{
			continue;
		}
		else
		{
			++TotPVSAreaCnt;
		}
	}

	return TotPVSAreaCnt;
}

RwInt32 CNtlWorldSectorPVS::GetPVSAppliedNeighborSectorNum(RwInt32 _CurSectorIdx, RwInt32 _iLayer)
{
#ifdef dNTL_WORLD_TOOL_MODE
	if (CNtlPLGlobal::m_iCurrentPVSLayer >= 0)
	{
		_iLayer = CNtlPLGlobal::m_iCurrentPVSLayer;
	}
#endif

	if(_CurSectorIdx == -1 || !m_pVisibilityNeighbor)
	{
		return 0;
	}

	RwInt32 RetCnt = 0;
	RwInt32 LoopCnt = dPVS_TOT_CELL_CNT;
	while(LoopCnt)
	{
		if(!m_pVisibilityNeighbor[LoopCnt + (_iLayer * dPVS_TOT_CELL_CNT)])
		{
			++RetCnt;
		}

		--LoopCnt;
	}

	return RetCnt;
}

RwBool CNtlWorldSectorPVS::GetPVSAreaSectorArray(vector<RwInt32>& _vecPVSAreaSectorArray)
{
	if(!m_pPVSAreaFilter)
	{
		return FALSE;
	}

	_vecPVSAreaSectorArray.clear();

	RwRGBA					RGBA;
	DWORD*					pDest;
	DWORD					Texel;
	RwInt32					TexelCnt	= m_pPVSAreaFilter->width;
	DWORD*					pBits		= (DWORD*)RwImageGetPixels(m_pPVSAreaFilter);
	CNtlWorldFieldManager*	pFieldMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
	RwInt32					CurFieldSecIdx[4];

	for(RwInt32 i = 0; i < TexelCnt * TexelCnt; ++i)
	{
		pDest		= pBits + i;
		Texel		= (*pDest);
		RGBA.alpha	= (RwUInt8)((Texel & 0xff000000) >> 24);
		RGBA.blue	= (RwUInt8)((Texel & 0x00ff0000) >> 16);
		RGBA.green	= (RwUInt8)((Texel & 0x0000ff00) >> 8);
		RGBA.red	= (RwUInt8)((Texel & 0x000000ff) >> 0);

		if(RGBA.red || RGBA.green || RGBA.blue)
		{
			continue;
		}
		else
		{
			pFieldMgr->GetFieldSectorIndices(TexelCnt * TexelCnt - 1 - i, CurFieldSecIdx);
			_vecPVSAreaSectorArray.push_back(CurFieldSecIdx[0]);
			_vecPVSAreaSectorArray.push_back(CurFieldSecIdx[1]);
			_vecPVSAreaSectorArray.push_back(CurFieldSecIdx[2]);
			_vecPVSAreaSectorArray.push_back(CurFieldSecIdx[3]);
		}
	}

	return TRUE;
}

VOID CNtlWorldSectorPVS::RenderLockingGuideLine()
{
	CNtlWorldFieldManager* pFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PVS_LOCKING) && GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)		
	{
		sLINE3D			BrushLine[2];
		D3DXMATRIXA16	mIdentity;
		RwV3d			CurPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

		BeginD3DLineState();

		RwD3D9SetFVF(D3DFVF_LINE3D);

		D3DXMatrixIdentity(&mIdentity);
		RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

		D3DXVECTOR3 MPos = pFMgr->m_pSectors[m_LockedPVSSector].DatumPoint;

		RwV3d MPosRW; 
		RwV3d Temp;
		CNtlMath::MathRwV3dAssign(&MPosRW, pFMgr->m_pSectors[m_LockedPVSSector].DatumPoint.x, pFMgr->m_pSectors[m_LockedPVSSector].DatumPoint.y, pFMgr->m_pSectors[m_LockedPVSSector].DatumPoint.z);
		MPos.y = pFMgr->GetWorldSectorHeight(MPosRW);

		/*
		BrushLine[0].Col = 0xff0000ff;
		BrushLine[1].Col = 0xff0000ff;
		BrushLine[0].Pos = MPos;
		BrushLine[0].Pos.x -= dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		BrushLine[0].Pos.z -= dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		Temp.x = BrushLine[0].Pos.x;
		Temp.z = BrushLine[0].Pos.z;
		BrushLine[0].Pos.y = pFMgr->GetWorldSectorHeight(Temp);
		BrushLine[1].Pos = MPos;
		BrushLine[1].Pos.y += dGET_WORLD_PARAM()->WorldFarPlane / 2.0f;
		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

		BrushLine[0].Pos = MPos;
		BrushLine[0].Pos.x += dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		BrushLine[0].Pos.z -= dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		Temp.x = BrushLine[0].Pos.x;
		Temp.z = BrushLine[0].Pos.z;
		BrushLine[0].Pos.y = pFMgr->GetWorldSectorHeight(Temp);
		BrushLine[1].Pos = MPos;
		BrushLine[1].Pos.y += dGET_WORLD_PARAM()->WorldFarPlane / 2.0f;
		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

		BrushLine[0].Pos = MPos;
		BrushLine[0].Pos.x += dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		BrushLine[0].Pos.z += dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		Temp.x = BrushLine[0].Pos.x;
		Temp.z = BrushLine[0].Pos.z;
		BrushLine[0].Pos.y = pFMgr->GetWorldSectorHeight(Temp);
		BrushLine[1].Pos = MPos;
		BrushLine[1].Pos.y += dGET_WORLD_PARAM()->WorldFarPlane / 2.0f;
		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

		BrushLine[0].Pos = MPos;
		BrushLine[0].Pos.x -= dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		BrushLine[0].Pos.z += dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		Temp.x = BrushLine[0].Pos.x;
		Temp.z = BrushLine[0].Pos.z;
		BrushLine[0].Pos.y = pFMgr->GetWorldSectorHeight(Temp);
		BrushLine[1].Pos = MPos;
		BrushLine[1].Pos.y += dGET_WORLD_PARAM()->WorldFarPlane / 2.0f;
		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

		BrushLine[0].Col = 0xffff0000;
		BrushLine[1].Col = 0xffff0000;
		BrushLine[0].Pos = MPos;
		BrushLine[1].Pos = MPos;
		BrushLine[1].Pos.y += dGET_WORLD_PARAM()->WorldFarPlane / 2.0f;
		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));
*/
		BrushLine[0].Col = 0xff0000ff;
		BrushLine[1].Col = 0xff0000ff;
		BrushLine[0].Pos = MPos;
		BrushLine[0].Pos.x -= dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		BrushLine[0].Pos.z -= dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		Temp.x = BrushLine[0].Pos.x;
		Temp.z = BrushLine[0].Pos.z;
		BrushLine[0].Pos.y = pFMgr->GetWorldSectorHeight(Temp);
		BrushLine[1].Pos = BrushLine[0].Pos;
		BrushLine[1].Pos.y += dGET_WORLD_PARAM()->WorldFarPlane;
		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

		BrushLine[0].Pos = MPos;
		BrushLine[0].Pos.x += dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		BrushLine[0].Pos.z -= dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		Temp.x = BrushLine[0].Pos.x;
		Temp.z = BrushLine[0].Pos.z;
		BrushLine[0].Pos.y = pFMgr->GetWorldSectorHeight(Temp);
		BrushLine[1].Pos = BrushLine[0].Pos;
		BrushLine[1].Pos.y += dGET_WORLD_PARAM()->WorldFarPlane;
		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

		BrushLine[0].Pos = MPos;
		BrushLine[0].Pos.x += dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		BrushLine[0].Pos.z += dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		Temp.x = BrushLine[0].Pos.x;
		Temp.z = BrushLine[0].Pos.z;
		BrushLine[0].Pos.y = pFMgr->GetWorldSectorHeight(Temp);
		BrushLine[1].Pos = BrushLine[0].Pos;
		BrushLine[1].Pos.y += dGET_WORLD_PARAM()->WorldFarPlane;
		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

		BrushLine[0].Pos = MPos;
		BrushLine[0].Pos.x -= dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		BrushLine[0].Pos.z += dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
		Temp.x = BrushLine[0].Pos.x;
		Temp.z = BrushLine[0].Pos.z;
		BrushLine[0].Pos.y = pFMgr->GetWorldSectorHeight(Temp);
		BrushLine[1].Pos = BrushLine[0].Pos;
		BrushLine[1].Pos.y += dGET_WORLD_PARAM()->WorldFarPlane;
		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

		BrushLine[0].Col = 0xffff0000;
		BrushLine[1].Col = 0xffff0000;
		BrushLine[0].Pos = MPos;
		BrushLine[1].Pos = BrushLine[0].Pos;
		BrushLine[1].Pos.y += dGET_WORLD_PARAM()->WorldFarPlane;
		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

		EndD3DLineState();
	}
}