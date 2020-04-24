#include "stdafx.h"

#include "NtlWE.h"
#include "NtlWEFrm.h"
#include "NtlWEDoc.h"

#include "NtlWorldBrush.h"
#include "NtlWorldBrush2DCircle.h"
#include "NtlWorldBrushCircle.h"
#include "NtlWorldBrushRectangle.h"
#include "NtlWorldBrushTile.h"
#include "NtlWorldBrushTileEx.h"
#include "NtlWorldBrushFrustum.h"
#include "NtlWorldBrushRoad.h"
#include "NtlWorldBrushController.h"

#include "NtlWorldPaletteDefine.h"

#include "NtlWorldFieldManager.h"

#include "NtlPLRenderState.h"

#include "NtlPLWorldEntity.h"

#include "PaletteTerrain.h"

#include "UpdateEventTerrainPalette.h"


void CNtlHistoryActionTerrain::TerrainDrop(CNtlHistoryData* pHistoryData)
{
	CNtlHistoryDataTerrainRaiseDrop* pHistoryDataTemp = (CNtlHistoryDataTerrainRaiseDrop*)pHistoryData;

	CNtlWorldFieldManager*			pMgr				= dGETMGR();
	int								MaxCurBrushTileNum	= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum * 4;
	sTARGET_VERT_SL_INFO			TargetVertSLInfo;
	vector<sTARGET_VERT_SL_INFO>	vecTargetVertSLInfo; vecTargetVertSLInfo.reserve(MaxCurBrushTileNum);

	RwBBox							bbox;
	bbox.inf.x = static_cast<RwReal>((static_cast<RwInt32>(pHistoryDataTemp->vCenter.x - pHistoryDataTemp->fRadius) / dGET_WORLD_PARAM()->WorldSectorSize - 1) * dGET_WORLD_PARAM()->WorldSectorSize);
	bbox.inf.z = static_cast<RwReal>((static_cast<RwInt32>(pHistoryDataTemp->vCenter.z - pHistoryDataTemp->fRadius) / dGET_WORLD_PARAM()->WorldSectorSize - 1) * dGET_WORLD_PARAM()->WorldSectorSize);
	bbox.sup.x = static_cast<RwReal>((static_cast<RwInt32>(pHistoryDataTemp->vCenter.x + pHistoryDataTemp->fRadius) / dGET_WORLD_PARAM()->WorldSectorSize + 1) * dGET_WORLD_PARAM()->WorldSectorSize);
	bbox.sup.z = static_cast<RwReal>((static_cast<RwInt32>(pHistoryDataTemp->vCenter.z + pHistoryDataTemp->fRadius) / dGET_WORLD_PARAM()->WorldSectorSize + 1) * dGET_WORLD_PARAM()->WorldSectorSize);


	for (RwReal z = bbox.inf.z; z < bbox.sup.z; z += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for (RwReal x = bbox.inf.x; x < bbox.sup.x; x += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			RwV3d		vPos; vPos.x = x; vPos.z = z;
			RwInt32		iIdx = pMgr->GetSectorIdx(vPos);

			if (iIdx != -1)
			{
				RpAtomic	*pCurAtomic = pMgr->m_pSectors[iIdx].m_pAtomic;
				if (pCurAtomic)
				{
					RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
					RwRGBA		*pPrelights = RpGeometryGetPreLightColors(pCurAtomic->geometry);
					RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);

					RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKVERTICES);
					{
						for(int i = 0; i < VertNum; ++i)
						{
							float Distance = sqrtf(	(pVList[i].x - pHistoryDataTemp->vCenter.x) * (pVList[i].x - pHistoryDataTemp->vCenter.x) +
								(pVList[i].z - pHistoryDataTemp->vCenter.z) * (pVList[i].z - pHistoryDataTemp->vCenter.z));

							if(Distance <= pHistoryDataTemp->fRadius)
							{
								pVList[i].y	-= pHistoryDataTemp->fSensitivity * sinf(D3DXToRadian((90.0f * (pHistoryDataTemp->fRadius - Distance) /  pHistoryDataTemp->fRadius)));

								// set slope lighting
								TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[iIdx];
								TargetVertSLInfo.IdxVert			= i;
								vecTargetVertSLInfo.push_back(TargetVertSLInfo);
							}
							
							else if (Distance <= pHistoryDataTemp->fRadius + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize * 2))
							{
								// set slope lighting
								TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[iIdx];
								TargetVertSLInfo.IdxVert			= i;
								vecTargetVertSLInfo.push_back(TargetVertSLInfo);
							}
						}
					}
					RpGeometryUnlock(pCurAtomic->geometry);

					RwSphere sphereBounding;
					RpMorphTargetCalcBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
					RpMorphTargetSetBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
				}
			}
		}
	}
	pMgr->OnSetSlopeLighting(vecTargetVertSLInfo);
}

void CNtlHistoryActionTerrain::TerrainRaise(CNtlHistoryData* pHistoryData)
{
	CNtlHistoryDataTerrainRaiseDrop* pHistoryDataTemp = (CNtlHistoryDataTerrainRaiseDrop*)pHistoryData;

	CNtlWorldFieldManager*			pMgr				= dGETMGR();
	int								MaxCurBrushTileNum	= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum * 4;
	sTARGET_VERT_SL_INFO			TargetVertSLInfo;
	vector<sTARGET_VERT_SL_INFO>	vecTargetVertSLInfo; vecTargetVertSLInfo.reserve(MaxCurBrushTileNum);

	RwBBox							bbox;
	bbox.inf.x = static_cast<RwReal>((static_cast<RwInt32>(pHistoryDataTemp->vCenter.x - pHistoryDataTemp->fRadius) / dGET_WORLD_PARAM()->WorldSectorSize - 1) * dGET_WORLD_PARAM()->WorldSectorSize);
	bbox.inf.z = static_cast<RwReal>((static_cast<RwInt32>(pHistoryDataTemp->vCenter.z - pHistoryDataTemp->fRadius) / dGET_WORLD_PARAM()->WorldSectorSize - 1) * dGET_WORLD_PARAM()->WorldSectorSize);
	bbox.sup.x = static_cast<RwReal>((static_cast<RwInt32>(pHistoryDataTemp->vCenter.x + pHistoryDataTemp->fRadius) / dGET_WORLD_PARAM()->WorldSectorSize + 1) * dGET_WORLD_PARAM()->WorldSectorSize);
	bbox.sup.z = static_cast<RwReal>((static_cast<RwInt32>(pHistoryDataTemp->vCenter.z + pHistoryDataTemp->fRadius) / dGET_WORLD_PARAM()->WorldSectorSize + 1) * dGET_WORLD_PARAM()->WorldSectorSize);

	for (RwReal z = bbox.inf.z; z < bbox.sup.z; z += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for (RwReal x = bbox.inf.x; x < bbox.sup.x; x += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			RwV3d		vPos; vPos.x = x; vPos.z = z;
			RwInt32		iIdx = pMgr->GetSectorIdx(vPos);

			if (iIdx != -1)
			{
				RpAtomic	*pCurAtomic = pMgr->m_pSectors[iIdx].m_pAtomic;
				if (pCurAtomic)
				{
					RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
					RwRGBA		*pPrelights = RpGeometryGetPreLightColors(pCurAtomic->geometry);
					RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);

					RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKVERTICES);
					{
						for(int i = 0; i < VertNum; ++i)
						{
							float Distance = sqrtf(	(pVList[i].x - pHistoryDataTemp->vCenter.x) * (pVList[i].x - pHistoryDataTemp->vCenter.x) +
								(pVList[i].z - pHistoryDataTemp->vCenter.z) * (pVList[i].z - pHistoryDataTemp->vCenter.z));

							if(Distance <= pHistoryDataTemp->fRadius)
							{
								pVList[i].y	+= pHistoryDataTemp->fSensitivity * sinf(D3DXToRadian((90.0f * (pHistoryDataTemp->fRadius - Distance) /  pHistoryDataTemp->fRadius)));

								// set slope lighting
								TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[iIdx];
								TargetVertSLInfo.IdxVert			= i;
								vecTargetVertSLInfo.push_back(TargetVertSLInfo);
							}
							else if (Distance <= pHistoryDataTemp->fRadius + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize * 2))
							{
								// set slope lighting
								TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[iIdx];
								TargetVertSLInfo.IdxVert			= i;
								vecTargetVertSLInfo.push_back(TargetVertSLInfo);
							}
						}
					}
					RpGeometryUnlock(pCurAtomic->geometry);

					RwSphere sphereBounding;
					RpMorphTargetCalcBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
					RpMorphTargetSetBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
				}
			}
		}
	}
	pMgr->OnSetSlopeLighting(vecTargetVertSLInfo);
}

RwBool CNtlHistoryActionTerrain::CheckExceptionTerrainRaiseDrop(CNtlHistoryData* pHistoryData)
{
	CNtlHistoryDataTerrainRaiseDrop* pHistoryDataTemp = (CNtlHistoryDataTerrainRaiseDrop*)pHistoryData;

	CNtlWorldFieldManager*			pMgr = dGETMGR();

	RwBBox							bbox;
	bbox.inf.x = static_cast<RwReal>((static_cast<RwInt32>(pHistoryDataTemp->vCenter.x - pHistoryDataTemp->fRadius) / dGET_WORLD_PARAM()->WorldSectorSize - 1) * dGET_WORLD_PARAM()->WorldSectorSize);
	bbox.inf.z = static_cast<RwReal>((static_cast<RwInt32>(pHistoryDataTemp->vCenter.z - pHistoryDataTemp->fRadius) / dGET_WORLD_PARAM()->WorldSectorSize - 1) * dGET_WORLD_PARAM()->WorldSectorSize);
	bbox.sup.x = static_cast<RwReal>((static_cast<RwInt32>(pHistoryDataTemp->vCenter.x + pHistoryDataTemp->fRadius) / dGET_WORLD_PARAM()->WorldSectorSize + 1) * dGET_WORLD_PARAM()->WorldSectorSize);
	bbox.sup.z = static_cast<RwReal>((static_cast<RwInt32>(pHistoryDataTemp->vCenter.z + pHistoryDataTemp->fRadius) / dGET_WORLD_PARAM()->WorldSectorSize + 1) * dGET_WORLD_PARAM()->WorldSectorSize);

	for (RwReal z = bbox.inf.z; z < bbox.sup.z; z += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for (RwReal x = bbox.inf.x; x < bbox.sup.x; x += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			RwV3d		vPos; vPos.x = x; vPos.z = z;
			RwInt32		iIdx = pMgr->GetSectorIdx(vPos);

			if (iIdx == -1 || !pMgr->IsSectorLoaded(iIdx))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNtlHistoryActionTerrainRaise::Undo(CNtlHistoryData* pHistoryData)
{
	TerrainDrop(pHistoryData);
}

void CNtlHistoryActionTerrainRaise::Redo(CNtlHistoryData* pHistoryData)
{
	TerrainRaise(pHistoryData);
}

RwBool CNtlHistoryActionTerrainRaise::CheckException(CNtlHistoryData* pHistoryData)
{
	return CheckExceptionTerrainRaiseDrop(pHistoryData);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNtlHistoryActionTerrainDrop::Undo(CNtlHistoryData* pHistoryData)
{
	TerrainRaise(pHistoryData);
}

void CNtlHistoryActionTerrainDrop::Redo(CNtlHistoryData* pHistoryData)
{
	TerrainDrop(pHistoryData);
}

RwBool CNtlHistoryActionTerrainDrop::CheckException(CNtlHistoryData* pHistoryData)
{
	return CheckExceptionTerrainRaiseDrop(pHistoryData);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CUpdateEventTerrainPalette::CUpdateEventTerrainPalette()
{
	m_pNtlHistoryCtrl	= NTL_NEW CNtlHistoryController;

	m_pBrushSelector	= NULL;

	::ZeroMemory(&m_CustomizedTerrainBrush, sizeof(sTERRAIN_CUSTOMIZE_BRUSH_INFO));
}

CUpdateEventTerrainPalette::~CUpdateEventTerrainPalette()
{
	NTL_DELETE(m_pNtlHistoryCtrl);

	NTL_DELETE(m_pBrushSelector);

	if (m_CustomizedTerrainBrush.m_pMultiSelection)
	{
		NTL_DELETE(m_CustomizedTerrainBrush.m_pMultiSelection);
		m_CustomizedTerrainBrush.m_vecMulti.clear();
	}
}

void CUpdateEventTerrainPalette::DeleteTerrainPaletteDummies()
{
	m_pNtlHistoryCtrl->DeleteHistroyActionAll();

	NTL_DELETE(m_pBrushSelector);
}

void CUpdateEventTerrainPalette::UpdateWorldSectorHeightInRectangle(RwReal Height)
{
	CNtlWorldFieldManager*	pMgr		= dGETMGR();
	int						RectAreaNum = m_CustomizedTerrainBrush.m_vecMulti.size();

	for(int i = 0; i < RectAreaNum; ++i)
	{
		pMgr->SetWorldSectorHeightInRectangle(m_CustomizedTerrainBrush.m_vecMulti[i].SPos, m_CustomizedTerrainBrush.m_vecMulti[i].EPos, Height);
	}
}

void CUpdateEventTerrainPalette::RenderTerrainPalette()
{
	if(dGET_CUR_PALETTE() == ePM_TERRAIN)
	{
		if(dGET_TERRAIN_MODE() == eMEM_CUSTOMIZE)
		{
			if(m_pBrushSelector)
			{
				m_pBrushSelector->Render();
			}

			if(m_CustomizedTerrainBrush.m_pMultiSelection)
			{
				for(int i = 0; i < (int)m_CustomizedTerrainBrush.m_vecMulti.size(); ++i)
				{
					CNtlWorldFieldManager* pMgr	= dGETMGR();
					CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_CustomizedTerrainBrush.m_pMultiSelection->GetController());

					pCurBrush->SetPosS(m_CustomizedTerrainBrush.m_vecMulti[i].SPos);
					pCurBrush->SetPosE(m_CustomizedTerrainBrush.m_vecMulti[i].EPos);
					pCurBrush->Render();
				}
			}
		}
	}
}

void CUpdateEventTerrainPalette::UpdateTerrainPalette()
{
	if (dGET_CUR_PALETTE() != ePM_TERRAIN)
	{
		return;
	}

	if(dKEY(DIK_LALT) && dGET_MOUSE()->GetBehavior(MB_LBDN))
	{				
		dGETFRM()->m_pTerrainPalette->SetCustomizeEnhancedData(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y);
		return;
	}

	if (dGET_TERRAIN_MODE() == eMEM_RAISE || dGET_TERRAIN_MODE() == eMEM_DROP)
	{
		if (dOKEY(DIK_LEFT))
		{
			m_pNtlHistoryCtrl->Undo();
		}
		else if (dOKEY(DIK_RIGHT))
		{
			m_pNtlHistoryCtrl->Redo();
		}
		else if (dKEY(DIK_LCONTROL) && dOKEY(DIK_Z))
		{
 			if (m_pNtlHistoryCtrl->GetHistoryActionCur() == m_pNtlHistoryCtrl->GetHistoryActionNum())
 			{
 				m_pNtlHistoryCtrl->Undo();
			}
 			else
 			{
				m_pNtlHistoryCtrl->Redo();
 			}
		}
	}
	else
	{
		m_pNtlHistoryCtrl->DeleteHistroyActionAll();
	}

	switch(dGET_TERRAIN_MODE())
	{
	case eMEM_RAISE:		UpdateTerrainPaletteRaise();		break;
	case eMEM_DROP:			UpdateTerrainPaletteDrop();			break;
	case eMEM_PLATEAU:		UpdateTerrainPalettePlateAU();		break;		
	case eMEM_JUST:			UpdateTerrainPaletteJust();			break;
	case eMEM_STANDARDIZE:	UpdateTerrainPaletteStandardize();	break;
	case eMEM_SMOOTH:		UpdateTerrainPaletteSmooth();		break;
	case eMEM_SMOOTH_RANGE:	UpdateTerrainPaletteRange();		break;
	case eMEM_ROAD:			UpdateTerrainPaletteRoad();			break;
	case eMEM_CUSTOMIZE:	UpdateTerrainPaletteCustomize();	break;
	case eMEM_CUSTOMIZE_E:	UpdateTerrainPaletteCustomizeE();	break;
	case eMEM_SLOPE:		UpdateTerrainPaletteSlope();		break;
	}
}

void CUpdateEventTerrainPalette::UpdateTerrainPaletteCustomize()
{
	if (dGET_TERRAIN_MODE() != eMEM_CUSTOMIZE)
	{
		return;
	}

	CNtlWorldFieldManager* pMgr = dGETMGR();
	if(dGET_MOUSE()->GetLBUp())
	{
		if(dKEY(DIK_LSHIFT) && m_pBrushSelector)
		{
			if(!m_CustomizedTerrainBrush.m_pMultiSelection)
			{
				m_CustomizedTerrainBrush.m_pMultiSelection = NTL_NEW CNtlWorldBrush;
				CNtlWorldBrushController *pController = new CNtlWB_Rectangle<CNtlWorldFieldManager>(eNBT_RECTANGLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
				m_CustomizedTerrainBrush.m_pMultiSelection->SetController(pController);
			}

			CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());

			sRECT rt;
			::CopyMemory(&rt.SPos, &pCurBrush->GetPosS(), sizeof(RwV3d));
			::CopyMemory(&rt.EPos, &pCurBrush->GetPosE(), sizeof(RwV3d));
			m_CustomizedTerrainBrush.m_vecMulti.push_back(rt);
		}
		else
		{
			m_CustomizedTerrainBrush.m_vecMulti.clear();
		}

		if(m_pBrushSelector)
		{
			NTL_DELETE(m_pBrushSelector);
		}			
	}

	if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
	{
		if(!m_pBrushSelector)
		{
			m_pBrushSelector = NTL_NEW CNtlWorldBrush;

			CNtlWorldBrushController *pController = new CNtlWB_Rectangle<CNtlWorldFieldManager>(eNBT_RECTANGLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
			m_pBrushSelector->SetController(pController);

			CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
			pCurBrush->SetPosS(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
		}

		CNtlWorldFieldManager *pMgr = dGETMGR();
		CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
		pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
	}
}

void CUpdateEventTerrainPalette::UpdateTerrainPaletteRaise()
{
	static CNtlHistoryAction* pHistoryAction = NULL;

	if (dGET_TERRAIN_MODE() != eMEM_RAISE)
	{
		return;
	}

	if (dGET_MOUSE()->GetBehavior(MB_LBDN))
	{
		if (!pHistoryAction)
		{
			pHistoryAction = NTL_NEW CNtlHistoryActionTerrainRaise();
		}

		CNtlHistoryDataTerrainRaiseDrop* pHistoryData = NTL_NEW CNtlHistoryDataTerrainRaiseDrop();

		pHistoryData->vCenter		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
		pHistoryData->fRadius		= dGET_WORLD_PARAM()->WorldSectorTileSize * dGET_WORLD_PARAM()->NumBrushRadiusInTile;
		pHistoryData->fSensitivity	= dGET_WORLD_PARAM()->ValHeightfieldSensitivity;		

		pHistoryAction->Action(pHistoryData);
		pHistoryAction->PushHistoryData(pHistoryData);		
	}
	else if (pHistoryAction)
	{
		pHistoryAction = m_pNtlHistoryCtrl->PushHistoryAction(pHistoryAction, dGET_CUR_PALETTE());
	}
}

void CUpdateEventTerrainPalette::UpdateTerrainPaletteDrop()
{
	static CNtlHistoryAction* pHistoryAction = NULL;

	if (dGET_TERRAIN_MODE() != eMEM_DROP)
	{
		return;
	}

	if (dGET_MOUSE()->GetBehavior(MB_LBDN))
	{
		if (!pHistoryAction)
		{
			pHistoryAction = NTL_NEW CNtlHistoryActionTerrainDrop();
		}

		CNtlHistoryDataTerrainRaiseDrop* pHistoryData = NTL_NEW CNtlHistoryDataTerrainRaiseDrop();

		pHistoryData->vCenter		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
		pHistoryData->fRadius		= dGET_WORLD_PARAM()->WorldSectorTileSize * dGET_WORLD_PARAM()->NumBrushRadiusInTile;
		pHistoryData->fSensitivity	= dGET_WORLD_PARAM()->ValHeightfieldSensitivity;		

		pHistoryAction->Action(pHistoryData);
		pHistoryAction->PushHistoryData(pHistoryData);		
	}
	else if (pHistoryAction)
	{
		pHistoryAction = m_pNtlHistoryCtrl->PushHistoryAction(pHistoryAction, dGET_CUR_PALETTE());
	}
}

void CUpdateEventTerrainPalette::UpdateTerrainPaletteRoad()
{
	if (dGET_TERRAIN_MODE() != eMEM_ROAD)
	{
		return;
	}

	CNtlWorldFieldManager* pMgr = dGETMGR();
	if(dGET_MOUSE()->GetLBUp())
	{
		if(dKEY(DIK_SPACE))
		{
			sTARGET_VERT_SL_INFO			TargetVertSLInfo;
			vector<sTARGET_VERT_SL_INFO>	vecTargetVertSLInfo;

			CNtlWB_Road<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Road<CNtlWorldFieldManager>*>(dGETAPP().GetNtlPLWorldEntity()->m_pNtlBrush->GetController());

			for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
			{
				RpAtomic	*pCurAtomic = pMgr->m_pSectors[g_vecVisibleSectors[j]].m_pAtomic;
				RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
				RwRGBA		*pPrelights = RpGeometryGetPreLightColors(pCurAtomic->geometry);
				RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);
				RwV3d		Center		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
				RwReal		MaxRadius	= dGET_WORLD_PARAM()->WorldSectorTileSize * dGET_WORLD_PARAM()->NumBrushRadiusInTile;
				RwReal		BrushSensitivity = dGET_WORLD_PARAM()->ValHeightfieldSensitivity;
				RwV3d		vCollLine[2], vLineDelta;
				RwReal		ResultDist;
				RwV3d		vResult;

				RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKVERTICES);
				{
					for(int i = 0; i < VertNum; ++i)
					{
						// collision
						CNtlMath::MathRwV3dAssign(&vCollLine[0], pVList[i].x, pVList[i].y, pVList[i].z);
						CNtlMath::MathRwV3dAssign(&vCollLine[1], pVList[i].x, pVList[i].y, pVList[i].z);
						vCollLine[0].y += dGET_WORLD_PARAM()->WorldValueMax;
						vCollLine[1].y += dGET_WORLD_PARAM()->WorldValueMin;
						RwV3dSubMacro(&vLineDelta, &vCollLine[1], &vCollLine[0]);

						if(RtIntersectionLineTriangle(	&vCollLine[0],
							&vLineDelta,
							&pCurBrush->m_OrhogonalOBB[0][0],
							&pCurBrush->m_OrhogonalOBB[0][1],
							&pCurBrush->m_OrhogonalOBB[0][2],
							&ResultDist))
						{
							RwV3dScale(&vResult, &vLineDelta, ResultDist);
							RwV3dAdd(&pVList[i], &vCollLine[0], &vResult);
							//CNtlMath::MathRwV3dAssign(&pVList[i], vResult.x, vResult.y, vResult.z);
						}
						else if(RtIntersectionLineTriangle(	&vCollLine[0],
							&vLineDelta,
							&pCurBrush->m_OrhogonalOBB[1][0],
							&pCurBrush->m_OrhogonalOBB[1][1],
							&pCurBrush->m_OrhogonalOBB[1][2],
							&ResultDist))
						{
							RwV3dScale(&vResult, &vLineDelta, ResultDist);
							RwV3dAdd(&pVList[i], &vCollLine[0], &vResult);
							//CNtlMath::MathRwV3dAssign(&pVList[i], vResult.x, vResult.y, vResult.z);
						}

						// stand by for the slope lighting
						TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
						TargetVertSLInfo.IdxVert			= i;
						vecTargetVertSLInfo.push_back(TargetVertSLInfo);
					}
				}
				RpGeometryUnlock(pCurAtomic->geometry);

				RwSphere sphereBounding;
				RpMorphTargetCalcBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
				RpMorphTargetSetBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
			}

			pMgr->OnSetSlopeLighting(vecTargetVertSLInfo);	
		}

		// set controller null
		dGETAPP().GetNtlPLWorldEntity()->m_pNtlBrush->SetController(NULL);
	}

	if(dGET_MOUSE()->GetLBOnce())
	{
		CNtlWorldBrushController *pController = new CNtlWB_Road<CNtlWorldFieldManager>(eNBT_LINE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
		dGETAPP().GetNtlPLWorldEntity()->m_pNtlBrush->SetController(pController);

		CNtlWB_Road<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Road<CNtlWorldFieldManager>*>(dGETAPP().GetNtlPLWorldEntity()->m_pNtlBrush->GetController());
		pCurBrush->SetPosS(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
	}
	else if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
	{
		CNtlWB_Road<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Road<CNtlWorldFieldManager>*>(dGETAPP().GetNtlPLWorldEntity()->m_pNtlBrush->GetController());
		pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
	}

	CNtlWB_Road<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Road<CNtlWorldFieldManager>*>(dGETAPP().GetNtlPLWorldEntity()->m_pNtlBrush->GetController());
	if(pCurBrush)
	{
		pCurBrush->SetLength(dGET_WORLD_PARAM()->NumBrushRadiusInTile * 2.0f);
	}
}

void CUpdateEventTerrainPalette::UpdateTerrainPalettePlateAU()
{
	if (dGET_TERRAIN_MODE() != eMEM_PLATEAU)
	{
		return;
	}

	if (dGET_MOUSE()->GetBehavior(MB_LBDN))
	{
		CNtlWorldFieldManager*			pMgr				= dGETMGR();
		int								MaxCurBrushTileNum	= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum * 4;
		sTARGET_VERT_SL_INFO			TargetVertSLInfo;
		vector<sTARGET_VERT_SL_INFO>	vecTargetVertSLInfo; vecTargetVertSLInfo.reserve(MaxCurBrushTileNum);
		vector<sTARGET_VERT_INFO>		vecTargetVertInfo; vecTargetVertInfo.reserve(MaxCurBrushTileNum);

		RwBool							First = true;
		RwReal							Min, MinTemp;

		for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
		{
			RpAtomic	*pCurAtomic			= pMgr->m_pSectors[g_vecVisibleSectors[j]].m_pAtomic;
			RwV3d		*pVList				= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
			RwInt32		VertNum				= RpGeometryGetNumVertices(pCurAtomic->geometry);
			RwV3d		Center				= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
			RwReal		MaxRadius			= dGET_WORLD_PARAM()->WorldSectorTileSize * dGET_WORLD_PARAM()->NumBrushRadiusInTile;		
			RwReal		BrushSensitivity	= dGET_WORLD_PARAM()->ValHeightfieldSensitivity;
			RwV3d		DatumPt, Result;

			for(int i = 0; i < VertNum; ++i)
			{
				float Distance = sqrtf(	(pVList[i].x - Center.x) * (pVList[i].x - Center.x) +
					(pVList[i].z - Center.z) * (pVList[i].z - Center.z));

				if(Distance <= MaxRadius)
				{
					DatumPt.x = pVList[i].x;
					DatumPt.z = pVList[i].z;
					pMgr->GetVertInMemory(DatumPt, Result);

					if(First)
					{
						Min = Result.y;
						First = false;
					}
					else
					{
						MinTemp = Result.y;
						if(MinTemp < Min)
							Min = MinTemp;
					}	

					TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
					TargetVertSLInfo.IdxVert			= i;
					vecTargetVertSLInfo.push_back(TargetVertSLInfo);
				}
				else if (Distance <= MaxRadius + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize * 2))
				{
					// set slope lighting
					TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
					TargetVertSLInfo.IdxVert			= i;
					vecTargetVertSLInfo.push_back(TargetVertSLInfo);
				}
			}
		}

		for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
		{
			RpAtomic	*pCurAtomic = pMgr->m_pSectors[g_vecVisibleSectors[j]].m_pAtomic;
			RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
			RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);
			RwV3d		Center		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
			RwReal		MaxRadius	= dGET_WORLD_PARAM()->WorldSectorTileSize * dGET_WORLD_PARAM()->NumBrushRadiusInTile;		
			RwReal		BrushSensitivity = dGET_WORLD_PARAM()->ValHeightfieldSensitivity;
			RwV3d		DatumPt, Result;

			for(int i = 0; i < VertNum; ++i)
			{
				float Distance = sqrtf(	(pVList[i].x - Center.x) * (pVList[i].x - Center.x) +
					(pVList[i].z - Center.z) * (pVList[i].z - Center.z));

				if(Distance <= MaxRadius)
				{
					DatumPt.x = pVList[i].x;
					DatumPt.z = pVList[i].z;
					pMgr->GetVertInMemory(DatumPt, Result);

					if(Result.y == Min)
						continue;								

					if((Result.y >= (Min - 4.0f)) && (Result.y <= (Min + 4.0f)))
					{
						sTARGET_VERT_INFO TargetVertInfo;
						TargetVertInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
						TargetVertInfo.IdxVert			= i;
						TargetVertInfo.ValHeight		= Min;
						TargetVertInfo.ValHeight2Append	= false;
						vecTargetVertInfo.push_back(TargetVertInfo);

						continue;
					}

					if((Result.y - 1.0f) > Min)
					{
						sTARGET_VERT_INFO TargetVertInfo;
						TargetVertInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
						TargetVertInfo.IdxVert			= i;
						TargetVertInfo.ValHeight		= -BrushSensitivity;
						TargetVertInfo.ValHeight2Append	= true;
						vecTargetVertInfo.push_back(TargetVertInfo);

						continue;
					}
				}				
			}				
		}

		for(int j = 0; j < (int)vecTargetVertInfo.size(); ++j)
		{
			sTARGET_VERT_INFO	CurTargetVertInfo	= vecTargetVertInfo[j];
			RpAtomic			*pCurAtomic			= CurTargetVertInfo.pNtlWorldSector->m_pAtomic;
			RwV3d				*pVList				= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
			RwInt32				VertNum				= RpGeometryGetNumVertices(pCurAtomic->geometry);

			RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKVERTICES);
			{
				if(CurTargetVertInfo.ValHeight2Append)
					pVList[CurTargetVertInfo.IdxVert].y += CurTargetVertInfo.ValHeight;
				else
					pVList[CurTargetVertInfo.IdxVert].y = CurTargetVertInfo.ValHeight;
			}
			RpGeometryUnlock(pCurAtomic->geometry);

			RwSphere sphereBounding;
			RpMorphTargetCalcBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
			RpMorphTargetSetBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
		}

		pMgr->OnSetSlopeLighting(vecTargetVertSLInfo);
	}
}

void CUpdateEventTerrainPalette::UpdateTerrainPaletteJust()
{
	if (dGET_TERRAIN_MODE() != eMEM_JUST)
	{
		return;
	}

	if (dGET_MOUSE()->GetBehavior(MB_LBDN))
	{
		if(!dGET_MOUSE()->GetYMovementWithRate(10.0f))
			return;

		CNtlWorldFieldManager*			pMgr				= dGETMGR();
		int								MaxCurBrushTileNum	= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum * 4;
		sTARGET_VERT_SL_INFO			TargetVertSLInfo;
		vector<sTARGET_VERT_SL_INFO>	vecTargetVertSLInfo; vecTargetVertSLInfo.reserve(MaxCurBrushTileNum);

		for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
		{
			RpAtomic	*pCurAtomic = pMgr->m_pSectors[g_vecVisibleSectors[j]].m_pAtomic;
			RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
			RwRGBA		*pPrelights = RpGeometryGetPreLightColors(pCurAtomic->geometry);
			RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);
			RwV3d		Center		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
			RwReal		MaxRadius	= dGET_WORLD_PARAM()->WorldSectorTileSize * dGET_WORLD_PARAM()->NumBrushRadiusInTile;
			RwReal		BrushSensitivity = dGET_WORLD_PARAM()->ValHeightfieldSensitivity;
			RwReal		HemisphereHeightRate;
			RwReal		Angle;
			RwReal		Value;

			RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKVERTICES);

			for(int i = 0; i < VertNum; ++i)
			{
				float Distance = sqrtf(	(pVList[i].x - Center.x) * (pVList[i].x - Center.x) +
					(pVList[i].z - Center.z) * (pVList[i].z - Center.z));

				if(Distance <= MaxRadius)
				{
					HemisphereHeightRate = (MaxRadius - Distance) / MaxRadius;
					Angle = 90.0f * HemisphereHeightRate;
					Value = BrushSensitivity * -dGET_MOUSE()->GetYMovementWithRate(10.0f) * sinf(D3DXToRadian(Angle));
					pVList[i].y	+= Value;

					// set slope lighting
					TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
					TargetVertSLInfo.IdxVert			= i;
					vecTargetVertSLInfo.push_back(TargetVertSLInfo);
				}
				else if (Distance <= MaxRadius + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize * 2))
				{
					// set slope lighting
					TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
					TargetVertSLInfo.IdxVert			= i;
					vecTargetVertSLInfo.push_back(TargetVertSLInfo);
				}
			}

			RpGeometryUnlock(pCurAtomic->geometry);

			RwSphere sphereBounding;
			RpMorphTargetCalcBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
			RpMorphTargetSetBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
		}

		pMgr->OnSetSlopeLighting(vecTargetVertSLInfo);
	}
}

void CUpdateEventTerrainPalette::UpdateTerrainPaletteSmooth()
{
	if (dGET_TERRAIN_MODE() != eMEM_SMOOTH)
	{
		return;
	}

	if (dGET_MOUSE()->GetBehavior(MB_LBDN))
	{
		CNtlWorldFieldManager*			pMgr				= dGETMGR();
		int								MaxCurBrushTileNum	= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum * 4;
		sTARGET_VERT_SL_INFO			TargetVertSLInfo;
		vector<sTARGET_VERT_SL_INFO>	vecTargetVertSLInfo; vecTargetVertSLInfo.reserve(MaxCurBrushTileNum);
		vector<sTARGET_VERT_INFO>		vecTargetVertInfo; vecTargetVertInfo.reserve(MaxCurBrushTileNum);

		for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
		{
			RpAtomic	*pCurAtomic = pMgr->m_pSectors[g_vecVisibleSectors[j]].m_pAtomic;
			RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
			RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);
			RwV3d		Center		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
			RwReal		MaxRadius	= dGET_WORLD_PARAM()->WorldSectorTileSize * dGET_WORLD_PARAM()->NumBrushRadiusInTile;
			RwReal		BrushSensitivity = dGET_WORLD_PARAM()->ValHeightfieldSensitivity;
			RwReal		Average;
			RwReal		Deviation;

			for(int i = 0; i < VertNum; ++i)
			{
				float Distance = sqrtf(	(pVList[i].x - Center.x) * (pVList[i].x - Center.x) +
					(pVList[i].z - Center.z) * (pVList[i].z - Center.z));

				if(Distance <= MaxRadius)
				{	
					//
					// 3---2---1
					// |       | 
					// 4  cur  0
					// |       |
					// 5---6---7
					// 
					RwV3d DatumPt, Result[8];

					// 0
					DatumPt.x = pVList[i].x - dGET_WORLD_PARAM()->WorldSectorTileSize;
					DatumPt.z = pVList[i].z;
					pMgr->GetVertInMemory(DatumPt, Result[0]);

					// 1
					DatumPt.x = pVList[i].x - dGET_WORLD_PARAM()->WorldSectorTileSize;
					DatumPt.z = pVList[i].z + dGET_WORLD_PARAM()->WorldSectorTileSize;
					pMgr->GetVertInMemory(DatumPt, Result[1]);

					// 2
					DatumPt.x = pVList[i].x;
					DatumPt.z = pVList[i].z + dGET_WORLD_PARAM()->WorldSectorTileSize;
					pMgr->GetVertInMemory(DatumPt, Result[2]);

					// 3
					DatumPt.x = pVList[i].x + dGET_WORLD_PARAM()->WorldSectorTileSize;
					DatumPt.z = pVList[i].z + dGET_WORLD_PARAM()->WorldSectorTileSize;
					pMgr->GetVertInMemory(DatumPt, Result[3]);

					// 4
					DatumPt.x = pVList[i].x + dGET_WORLD_PARAM()->WorldSectorTileSize;
					DatumPt.z = pVList[i].z;
					pMgr->GetVertInMemory(DatumPt, Result[4]);

					// 5
					DatumPt.x = pVList[i].x + dGET_WORLD_PARAM()->WorldSectorTileSize;
					DatumPt.z = pVList[i].z - dGET_WORLD_PARAM()->WorldSectorTileSize;
					pMgr->GetVertInMemory(DatumPt, Result[5]);

					// 6
					DatumPt.x = pVList[i].x;
					DatumPt.z = pVList[i].z - dGET_WORLD_PARAM()->WorldSectorTileSize;
					pMgr->GetVertInMemory(DatumPt, Result[6]);

					// 7
					DatumPt.x = pVList[i].x - dGET_WORLD_PARAM()->WorldSectorTileSize;
					DatumPt.z = pVList[i].z - dGET_WORLD_PARAM()->WorldSectorTileSize;
					pMgr->GetVertInMemory(DatumPt, Result[7]);

					Average = 0.0f;
					for(int v = 0; v < 8; ++v)
						Average += Result[v].y;
					Average /= 8.0f;					

					/*
					RwInt32 ValidCnt = 0;
					Average = 0.0f;
					for(int v = 0; v < 8; ++v)
					{
					float DistanceTmp = sqrtf(	(Result[v].x - Center.x) * (Result[v].x - Center.x) +
					(Result[v].z - Center.z) * (Result[v].z - Center.z));

					if(DistanceTmp <= MaxRadius)
					{
					ValidCnt++;
					Average += Result[v].y;
					}
					}
					Average /= static_cast<RwReal>(ValidCnt);
					*/

					if(Average > pVList[i].y)
					{
						Deviation = (Average - pVList[i].y);
						Average = (BrushSensitivity / 10.0f) * Deviation;
					}
					else if(Average < pVList[i].y)
					{
						Deviation = (pVList[i].y - Average);
						Average = (BrushSensitivity / 10.0f) * -Deviation;
					}
					else
						Average = 0.0f;

					sTARGET_VERT_INFO TargetVertInfo;
					TargetVertInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
					TargetVertInfo.ValHeight		= Average;
					TargetVertInfo.IdxVert			= i;
					vecTargetVertInfo.push_back(TargetVertInfo);

					// set slope lighting
					TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
					TargetVertSLInfo.IdxVert			= i;
					vecTargetVertSLInfo.push_back(TargetVertSLInfo);
				}
				else if (Distance <= MaxRadius + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize * 2))
				{
					// set slope lighting
					TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
					TargetVertSLInfo.IdxVert			= i;
					vecTargetVertSLInfo.push_back(TargetVertSLInfo);
				}
			}					
		}

		for(int j = 0; j < (int)vecTargetVertInfo.size(); ++j)
		{
			sTARGET_VERT_INFO	CurTargetVertInfo	= vecTargetVertInfo[j];
			RpAtomic			*pCurAtomic			= CurTargetVertInfo.pNtlWorldSector->m_pAtomic;
			RwV3d				*pVList				= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
			RwInt32				VertNum				= RpGeometryGetNumVertices(pCurAtomic->geometry);

			RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKVERTICES);
			{
				pVList[CurTargetVertInfo.IdxVert].y += CurTargetVertInfo.ValHeight;
			}
			RpGeometryUnlock(pCurAtomic->geometry);

			RwSphere sphereBounding;
			RpMorphTargetCalcBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
			RpMorphTargetSetBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
		}

		pMgr->OnSetSlopeLighting(vecTargetVertSLInfo);
	}
}

void CUpdateEventTerrainPalette::UpdateTerrainPaletteStandardize()
{
	if (dGET_TERRAIN_MODE() != eMEM_STANDARDIZE)
	{
		return;
	}

	if (dGET_MOUSE()->GetBehavior(MB_LBDN))
	{
		CNtlWorldFieldManager*			pMgr				= dGETMGR();
		int								MaxCurBrushTileNum	= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum * 4;
		sTARGET_VERT_SL_INFO			TargetVertSLInfo;
		vector<sTARGET_VERT_SL_INFO>	vecTargetVertSLInfo; vecTargetVertSLInfo.reserve(MaxCurBrushTileNum);

		for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
		{
			RpAtomic	*pCurAtomic = pMgr->m_pSectors[g_vecVisibleSectors[j]].m_pAtomic;
			RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
			RwRGBA		*pPrelights = RpGeometryGetPreLightColors(pCurAtomic->geometry);
			RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);
			RwV3d		Center		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
			RwReal		MaxRadius	= dGET_WORLD_PARAM()->WorldSectorTileSize * dGET_WORLD_PARAM()->NumBrushRadiusInTile;
			RwReal		BrushSensitivity = dGET_WORLD_PARAM()->ValHeightfieldSensitivity;

			RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKVERTICES);

			for(int i = 0; i < VertNum; ++i)
			{
				float Distance = sqrtf(	(pVList[i].x - Center.x) * (pVList[i].x - Center.x) +
					(pVList[i].z - Center.z) * (pVList[i].z - Center.z));

				if(Distance <= MaxRadius)
				{
					float SeaLevelHeight = 0.0f;

					if(pVList[i].y > SeaLevelHeight)
					{
						if(-0.5f + pVList[i].y < SeaLevelHeight)
						{
							pVList[i].y	= SeaLevelHeight;
						}
						else
						{
							pVList[i].y	+= -BrushSensitivity;
						}
					}
					else if(pVList[i].y < SeaLevelHeight)
					{
						if(0.5f + pVList[i].y > SeaLevelHeight)
						{
							pVList[i].y	= SeaLevelHeight;
						}
						else
						{
							pVList[i].y	+= BrushSensitivity;
						}
					}

					// set slope lighting
					TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
					TargetVertSLInfo.IdxVert			= i;
					vecTargetVertSLInfo.push_back(TargetVertSLInfo);
				}
				else if (Distance <= MaxRadius + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize * 2))
				{
					// set slope lighting
					TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
					TargetVertSLInfo.IdxVert			= i;
					vecTargetVertSLInfo.push_back(TargetVertSLInfo);
				}
			}

			RpGeometryUnlock(pCurAtomic->geometry);

			RwSphere sphereBounding;
			RpMorphTargetCalcBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
			RpMorphTargetSetBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
		}

		pMgr->OnSetSlopeLighting(vecTargetVertSLInfo);
	}
}

void CUpdateEventTerrainPalette::UpdateTerrainPaletteRange()
{
	if (dGET_TERRAIN_MODE() != eMEM_SMOOTH_RANGE)
	{
		return;
	}

	if (dGET_MOUSE()->GetBehavior(MB_LBDN))
	{
		CNtlWorldFieldManager*			pMgr				= dGETMGR();
		int								MaxCurBrushTileNum	= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum * 4;
		sTARGET_VERT_SL_INFO			TargetVertSLInfo;
		vector<sTARGET_VERT_SL_INFO>	vecTargetVertSLInfo; vecTargetVertSLInfo.reserve(MaxCurBrushTileNum);
		vector<sTARGET_VERT_INFO>		vecTargetVertInfo; vecTargetVertInfo.reserve(MaxCurBrushTileNum);

		for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
		{
			RpAtomic	*pCurAtomic = pMgr->m_pSectors[g_vecVisibleSectors[j]].m_pAtomic;
			RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
			RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);
			RwV3d		Center		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
			RwReal		MaxRadius	= dGET_WORLD_PARAM()->WorldSectorTileSize * dGET_WORLD_PARAM()->NumBrushRadiusInTile;
			RwReal		BrushSensitivity = dGET_WORLD_PARAM()->ValHeightfieldSensitivity;
			RwReal		Average;
			RwReal		Deviation;

			for(int i = 0; i < VertNum; ++i)
			{
				float Distance = sqrtf(	(pVList[i].x - Center.x) * (pVList[i].x - Center.x) +
					(pVList[i].z - Center.z) * (pVList[i].z - Center.z));

				if(Distance <= MaxRadius)
				{	
					//
					// 3---2---1
					// |       | 
					// 4  cur  0
					// |       |
					// 5---6---7
					// 
					RwV3d DatumPt, Result[8];

					// 0
					DatumPt.x = pVList[i].x - dGET_WORLD_PARAM()->WorldSectorTileSize;
					DatumPt.z = pVList[i].z;
					pMgr->GetVertInMemory(DatumPt, Result[0]);

					// 1
					DatumPt.x = pVList[i].x - dGET_WORLD_PARAM()->WorldSectorTileSize;
					DatumPt.z = pVList[i].z + dGET_WORLD_PARAM()->WorldSectorTileSize;
					pMgr->GetVertInMemory(DatumPt, Result[1]);

					// 2
					DatumPt.x = pVList[i].x;
					DatumPt.z = pVList[i].z + dGET_WORLD_PARAM()->WorldSectorTileSize;
					pMgr->GetVertInMemory(DatumPt, Result[2]);

					// 3
					DatumPt.x = pVList[i].x + dGET_WORLD_PARAM()->WorldSectorTileSize;
					DatumPt.z = pVList[i].z + dGET_WORLD_PARAM()->WorldSectorTileSize;
					pMgr->GetVertInMemory(DatumPt, Result[3]);

					// 4
					DatumPt.x = pVList[i].x + dGET_WORLD_PARAM()->WorldSectorTileSize;
					DatumPt.z = pVList[i].z;
					pMgr->GetVertInMemory(DatumPt, Result[4]);

					// 5
					DatumPt.x = pVList[i].x + dGET_WORLD_PARAM()->WorldSectorTileSize;
					DatumPt.z = pVList[i].z - dGET_WORLD_PARAM()->WorldSectorTileSize;
					pMgr->GetVertInMemory(DatumPt, Result[5]);

					// 6
					DatumPt.x = pVList[i].x;
					DatumPt.z = pVList[i].z - dGET_WORLD_PARAM()->WorldSectorTileSize;
					pMgr->GetVertInMemory(DatumPt, Result[6]);

					// 7
					DatumPt.x = pVList[i].x - dGET_WORLD_PARAM()->WorldSectorTileSize;
					DatumPt.z = pVList[i].z - dGET_WORLD_PARAM()->WorldSectorTileSize;
					pMgr->GetVertInMemory(DatumPt, Result[7]);

					RwInt32 ValidCnt = 0;
					Average = 0.0f;
					for(int v = 0; v < 8; ++v)
					{
						float DistanceTmp = sqrtf(	(Result[v].x - Center.x) * (Result[v].x - Center.x) +
							(Result[v].z - Center.z) * (Result[v].z - Center.z));

						if(DistanceTmp <= MaxRadius)
						{
							ValidCnt++;
							Average += Result[v].y;
						}
					}
					Average /= static_cast<RwReal>(ValidCnt);

					if(Average > pVList[i].y)
					{
						Deviation = (Average - pVList[i].y);
						Average = (BrushSensitivity / 10.0f) * Deviation;
					}
					else if(Average < pVList[i].y)
					{
						Deviation = (pVList[i].y - Average);
						Average = (BrushSensitivity / 10.0f) * -Deviation;
					}
					else
						Average = 0.0f;

					sTARGET_VERT_INFO TargetVertInfo;
					TargetVertInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
					TargetVertInfo.ValHeight		= Average;
					TargetVertInfo.IdxVert			= i;
					vecTargetVertInfo.push_back(TargetVertInfo);

					// set slope lighting
					TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
					TargetVertSLInfo.IdxVert			= i;
					vecTargetVertSLInfo.push_back(TargetVertSLInfo);
				}
				else if (Distance <= MaxRadius + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize * 2))
				{
					// set slope lighting
					TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
					TargetVertSLInfo.IdxVert			= i;
					vecTargetVertSLInfo.push_back(TargetVertSLInfo);
				}
			}					
		}

		for(int j = 0; j < (int)vecTargetVertInfo.size(); ++j)
		{
			sTARGET_VERT_INFO	CurTargetVertInfo	= vecTargetVertInfo[j];
			RpAtomic			*pCurAtomic			= CurTargetVertInfo.pNtlWorldSector->m_pAtomic;
			RwV3d				*pVList				= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
			RwInt32				VertNum				= RpGeometryGetNumVertices(pCurAtomic->geometry);

			RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKVERTICES);
			{
				pVList[CurTargetVertInfo.IdxVert].y += CurTargetVertInfo.ValHeight;
			}
			RpGeometryUnlock(pCurAtomic->geometry);

			RwSphere sphereBounding;
			RpMorphTargetCalcBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
			RpMorphTargetSetBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
		}

		pMgr->OnSetSlopeLighting(vecTargetVertSLInfo);
	}
}

void CUpdateEventTerrainPalette::UpdateTerrainPaletteCustomizeE()
{
	if (dGET_TERRAIN_MODE() != eMEM_CUSTOMIZE_E)
	{
		return;
	}

	if (dGET_MOUSE()->GetBehavior(MB_LBDN))
	{
		CNtlWorldFieldManager*			pMgr				= dGETMGR();
		int								MaxCurBrushTileNum	= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum * 4;
		sTARGET_VERT_SL_INFO			TargetVertSLInfo;
		vector<sTARGET_VERT_SL_INFO>	vecTargetVertSLInfo; vecTargetVertSLInfo.reserve(MaxCurBrushTileNum);

		for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
		{
			RpAtomic	*pCurAtomic = pMgr->m_pSectors[g_vecVisibleSectors[j]].m_pAtomic;
			RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
			RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);
			RwV3d		Center		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
			RwReal		MaxRadius	= dGET_WORLD_PARAM()->WorldSectorTileSize * dGET_WORLD_PARAM()->NumBrushRadiusInTile;

			RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKVERTICES);
			{
				for(int i = 0; i < VertNum; ++i)
				{
					float Distance = sqrtf(	(pVList[i].x - Center.x) * (pVList[i].x - Center.x) +
						(pVList[i].z - Center.z) * (pVList[i].z - Center.z));

					if(Distance <= MaxRadius)
					{
						pVList[i].y	= dGETFRM()->m_pTerrainPalette->GetCustomizeEnhancedData();

						// set slope lighting
						TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
						TargetVertSLInfo.IdxVert			= i;
						vecTargetVertSLInfo.push_back(TargetVertSLInfo);
					}
					else if(Distance <= MaxRadius + (dGET_WORLD_PARAM()->WorldSectorTileSize * 2))
					{
						// set slope lighting
						TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
						TargetVertSLInfo.IdxVert			= i;
						vecTargetVertSLInfo.push_back(TargetVertSLInfo);
					}
				}
			}
			RpGeometryUnlock(pCurAtomic->geometry);

			RwSphere sphereBounding;
			RpMorphTargetCalcBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
			RpMorphTargetSetBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
		}

		pMgr->OnSetSlopeLighting(vecTargetVertSLInfo);
	}
}

void CUpdateEventTerrainPalette::UpdateTerrainPaletteSlope()
{
	if (dGET_TERRAIN_MODE() != eMEM_SLOPE)
	{
		return;
	}

	if (dGET_MOUSE()->GetBehavior(MB_LBDN))
	{
		CNtlWorldFieldManager*			pMgr				= dGETMGR();
		int								MaxCurBrushTileNum	= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum * 4;
		sTARGET_VERT_SL_INFO			TargetVertSLInfo;
		vector<sTARGET_VERT_SL_INFO>	vecTargetVertSLInfo; vecTargetVertSLInfo.reserve(MaxCurBrushTileNum);

		RwReal							Value				= dGETFRM()->m_pTerrainPalette->GetCustomizeEnhancedData();
		RwReal							BrushSensitivity	= dGET_WORLD_PARAM()->ValHeightfieldSensitivity;

		for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
		{
			RpAtomic	*pCurAtomic = pMgr->m_pSectors[g_vecVisibleSectors[j]].m_pAtomic;
			RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
			RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);
			RwV3d		Center		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
			RwReal		MaxRadius	= dGET_WORLD_PARAM()->WorldSectorTileSize * dGET_WORLD_PARAM()->NumBrushRadiusInTile;

			RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKVERTICES);
			{
				for(int i = 0; i < VertNum; ++i)
				{
					float Distance = sqrtf(	(pVList[i].x - Center.x) * (pVList[i].x - Center.x) +
						(pVList[i].z - Center.z) * (pVList[i].z - Center.z));

					if(Distance <= MaxRadius)
					{	
						RwReal SrcHeight = dGET_POSITIVE_VALUE(pVList[i].y);
						RwReal DstHeight = dGET_POSITIVE_VALUE(Value);

						if(SrcHeight < DstHeight)
						{
							if(SrcHeight + BrushSensitivity > DstHeight)
							{
								SrcHeight = DstHeight;
							}
							else
							{
								SrcHeight += BrushSensitivity;
							}
						}
						else if(SrcHeight > DstHeight)
						{
							if(SrcHeight- BrushSensitivity < DstHeight)
							{
								SrcHeight = DstHeight;
							}
							else
							{
								SrcHeight -= BrushSensitivity;
							}
						}

						pVList[i].y = dGET_NEGATIVE_VALUE(SrcHeight);

						// set slope lighting
						TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
						TargetVertSLInfo.IdxVert			= i;
						vecTargetVertSLInfo.push_back(TargetVertSLInfo);
					}
					else if(Distance <= MaxRadius + (dGET_WORLD_PARAM()->WorldSectorTileSize * 2))
					{
						// set slope lighting
						TargetVertSLInfo.pNtlWorldSector	= &pMgr->m_pSectors[g_vecVisibleSectors[j]];
						TargetVertSLInfo.IdxVert			= i;
						vecTargetVertSLInfo.push_back(TargetVertSLInfo);
					}
				}
			}
			RpGeometryUnlock(pCurAtomic->geometry);

			RwSphere sphereBounding;
			RpMorphTargetCalcBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
			RpMorphTargetSetBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
		}

		pMgr->OnSetSlopeLighting(vecTargetVertSLInfo);
	}
}