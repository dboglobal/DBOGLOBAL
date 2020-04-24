#include "StdAfx.h"

#include "RtLTMap.h"

#include "NtlWE.h"
#include "NtlWEFrm.h"
#include "NtlWEDoc.h"

#include "AppConfigManager.h"
#include "TextAllTable.h"

#include "NtlApplication.h"
#include "NtlProfiler.h"
#include "NtlText.h"
#include "NtlCoreUtil.h"
#include "NtlSharedType.h"

#include "NtlSLApi.h"

#include "gui_StaticBox.h"
#include "gui_define.h"
#include "gui_guimanager.h"

#include "NtlSoundManager.h"

#include "NtlWorldCommon.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlWorldInterface.h"

#include "NtlPLWorldState.h"

#include "NtlWorldMouse.h"
#include "NtlWorldBrush.h"
#include "NtlWorldBrush2DCircle.h"
#include "NtlWorldBrushCircle.h"
#include "NtlWorldBrushRectangle.h"
#include "NtlWorldBrushTile.h"
#include "NtlWorldBrushTileEx.h"
#include "NtlWorldBrushFrustum.h"
#include "NtlWorldBrushRoad.h"
#include "NtlWorldBrushController.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RWWorld.h"
#include "NtlWorldPathEngineManager.h"
#include "NtlWorldSLManger.h"
#include "NtlWorldShadowManager.h"
#include "NtlWorldRpLightManager.h"
#include "NtlWorldLTManager.h"

#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"
#include "NtlPLResourceManager.h"
#include "NtlPLGuiManager.h"

#include "NtlWorldField.h"
#include "NtlWorldSector.h"
#include "NtlWorldShadow.h"
#include "NtlWorldSectorPVS.h"

#include "TableContainer.h"
#include "PathHandler.h"
#include "ProgressWnd.h"

#include "NtlPLEvent.h"
#include "NtlPLRenderState.h"
#include "NtlPLRenderGeometry.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLEntity.h"
#include "NtlPLObject.h"
#include "NtlPLWater.h"
#include "NtlPLWaterProp.h"
#include "NtlPLSound.h"
#include "NtlPLSoftSpot.h"
#include "NtlPLHeatHaze.h"

#include "SpawnManager.h"
#include "FieldUIManager.h"
#include "MergeUIManager.h"

#include "NtlWorldEntityPreview.h"
#include "ObjPropPopup.h"
#include "EffectPropPopup.h"
#include "CameraAttrDlg.h"
#include "PeHeightPopup.h"
#include "WorldViewDlg.h"

#include "PaletteOrnament.h"
#include "PaletteTerrain.h"
#include "PaletteWater.h"
#include "PaletteShadow.h"
#include "PaletteNMProp.h"
#include "PaletteSE.h"
#include "PaletteSoftSpot.h"
#include "PalettePerformanceSectorCull.h"
#include "PalettePlantDlg.h"
#include "PaletteWorldLight.h"
#include "PalettePerformanceOcclusion.h"
#include "PaletteHeatHazeDlg.h"
#include "PaletteLightAndShadow.h"
//#include "PaletteLightObject.h"
#include "PaletteLightMode.h"
#include "PaletteGameProperty.h"
#include "PaletteDecalVolumeDlg.h"
#include "PaletteEffect.h"
#include "ntlplemblemmaker.h"

#include "UpdateEvent.h"

CWorldEditorEventHander::CWorldEditorEventHander(RwInt32 WindowWith, RwInt32 WindowHeight)
{
	// CZ-SS
	//m_SelectedSS		= NULL;
	m_MultipleSelect	= false;
	m_IsSomeCaptured	= false;

	m_pBrushSelector		= NULL;
	m_pBrushSelectorFrsutum	= NULL;
	m_pBrushSelectorPE		= NULL;
	m_pBrushSelection		= NULL;
	m_pBrushSelectionSub	= NULL;
	m_pBrush2D				= NULL;

	// 민석선배 요청으로 기본값을 0.5f -> 1.0f 로 변경.

	//m_fObjMovementSensitivity	= 0.5f;
	m_fObjMovementSensitivity	= 1.0f;
	m_ScalingValue				= 1.0f;
	m_RotationTabValue			= 1.0f;

	//m_EffectTransform[0]		= 0.5f;
	m_EffectTransform[0]		= 1.0f;
	m_EffectTransform[1]		= 1.0f;
	m_EffectTransform[2]		= 1.0f;

	m_pFieldUIManager = NTL_NEW CFieldUIManager;
	m_pMergeUIManager = NTL_NEW CMergeUIManager;

	m_IdxCopyWater = -1;

	dGETDOC()->CreateWorldData();

	m_vecSelectedEffect.reserve(1);

	m_vecSelectedWorldLight.reserve(1);

	//m_vecSelectedLightObject.reserve(1);

	m_vecSelectedSE.reserve(1);

	m_pTileAttr			= NULL;
	m_pTileAttrSelector	= NULL;

	m_pSelectedOccluder	= NULL;
	m_pSelectedHeatHaze = NULL;

	m_iAttachBoneIdx	= -1;
	m_pAttachBoneObject	= NULL;

	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY, 0);
	LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY, 0);	
	LinkMsg(NPEI_IS_MAPNAME_CHANGE, 0);

	m_vecTileAttr.reserve(9 * dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldSectorTileNum);

	// test gui
	CRectangle Rt;
	Rt.SetRectWH(0, WindowHeight - 30, 512, 30);
	m_pFieldName = NTL_NEW gui::CStaticBox(Rt, GetNtlGuiManager()->GetGuiManager(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pFieldName->CreateFontStd("MSSanserif", 150, 0);
	m_pFieldName->SetAlpha(100);
	m_pFieldName->SetText("");
	m_pFieldName->Show(TRUE);

	// Object counts info.
	m_CntDoodad[0]	= 0;
	m_CntDoodad[1]	= 0;
	m_CntEffect[0]	= 0;
	m_CntEffect[1]	= 0;
	m_CntWater[0]	= 0;
	m_CntWater[1]	= 0;

	m_bboxSelectWater.sup.x = 0.0f; m_bboxSelectWater.sup.y = 0.0f; m_bboxSelectWater.sup.z = 0.0f;
	m_bboxSelectWater.inf.x = 0.0f; m_bboxSelectWater.inf.y = 0.0f; m_bboxSelectWater.inf.z = 0.0f;
}

CWorldEditorEventHander::~CWorldEditorEventHander()
{
	NTL_DELETE(m_pFieldName);

	DeleteAllPaletteDummies();

	// delete animatible doodad mark
	if(m_vecAnimatibleDoodadDummy.size())
	{
		for(unsigned int p = 0; p < m_vecAnimatibleDoodadDummy.size(); ++p)
		{
			GetSceneManager()->DeleteEntity(m_vecAnimatibleDoodadDummy[p]);
		}
		m_vecAnimatibleDoodadDummy.clear();
	}

	NTL_DELETE(m_pBrushSelector);
	NTL_DELETE(m_pBrushSelectorFrsutum);
	NTL_DELETE(m_pBrushSelection);
	NTL_DELETE(m_pBrush2D);

	NTL_DELETE(m_pFieldUIManager);
	NTL_DELETE(m_pMergeUIManager);

	RwTextureDestroy(m_pTileAttr);
	RwTextureDestroy(m_pTileAttrSelector);

	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY);
	UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY);
	UnLinkMsg(NPEI_IS_MAPNAME_CHANGE);

	NTL_DELETE(m_pPreview);
}

RwBool CWorldEditorEventHander::LoadConfig()
{
	NTL_FUNCTION("CNtlWorldFieldManager::LoadConfig")

		CAppConfigManager::GetInstance().Load();

	NTL_RETURN(TRUE);
}

RwBool CWorldEditorEventHander::SaveConfig()
{
	NTL_FUNCTION("CNtlWorldFieldManager::SaveConfig")

		CAppConfigManager::GetInstance().Save();

	NTL_RETURN(TRUE);
}

RwBool CWorldEditorEventHander::SaveAll()
{
	NTL_FUNCTION("CWorldEditorEventHander::SaveAll");

	if(::AfxMessageBox("Are you sure?", MB_YESNO) == IDYES)
	{
		DBO_TRACE(FALSE, "CWorldEditorEventHander::SaveAll() Start...");

		CNtlWorldFieldManager*	pMgr		= dGETMGR();
		CNtlWorldField*			pField		= const_cast<CNtlWorldField*>(pMgr->GetFields());
		char					Text[40]	= {0,};
		RwInt32					TotFieldNum = dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum;

		CProgressWnd WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Save All Fields");
		WndProgress.GoModal();
		WndProgress.SetRange(0, TotFieldNum);

		for(RwInt32 f = 0; f < TotFieldNum; ++f)
		{
			::sprintf_s(Text, 40, "%d / %d field is now saving...", f, TotFieldNum);
			WndProgress.SetText(Text);

			pMgr->SaveField(f);

			WndProgress.StepIt();
			WndProgress.PeekAndPump();
		}

		if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
		{
			((CNtlWorldFieldManager4RWWorld*)(pMgr))->SaveProperty();

			CNtlWorldLTManager::GetInstance()->SaveBSP();
		}

		WndProgress.SetText("Done.");	

		DBO_TRACE(FALSE, "CWorldEditorEventHander::SaveAll() End...");
	}

	NTL_RETURN(TRUE);
}

void CWorldEditorEventHander::Restore()
{
	// thumbnail
	RestoreThumbnail();

	// compass
	RestoreCompass();

	// Field UI manager
	m_pFieldUIManager->Restore();
	m_pMergeUIManager->Restore();
}

void CWorldEditorEventHander::Create()
{
	// thumbnail
	CreateThumbnail();

	// compass
	CreateCompass();

	switch (GetSceneManager()->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		{
			m_pFieldUIManager->Create(dGET_WORLD_PARAM()->WorldFieldNum, dGET_WORLD_PARAM()->WorldFieldSize);
		}
		break;
	case AW_RWWORLD:
		{
			CNtlWorldBlockMananger* pWBManager = ((CNtlWorldFieldManager4RWWorld*)dGETMGR())->GetWorldBlockManager();
			m_pFieldUIManager->Create(dGET_WORLD_PARAM()->WorldBlockSize, dGET_WORLD_PARAM()->WorldBlockNum);
		}
		break;
	}	
	m_pMergeUIManager->Create();

	// attr
	m_pTileAttr			= CNtlPLResourceManager::GetInstance()->LoadTexture("TileRectangleProp", "Tool\\SABER\\Texture\\UI\\");
	m_pTileAttrSelector = CNtlPLResourceManager::GetInstance()->LoadTexture("TileRectangleSelection", "Tool\\SABER\\Texture\\UI\\");
	DBO_ASSERT(m_pTileAttr, "Texture load failed.");
	DBO_ASSERT(m_pTileAttrSelector, "Texture load failed.");

	// Preview
	m_pPreview = NTL_NEW CNtlWorldEntityPreview;
}

VOID CWorldEditorEventHander::SetScaling(RwInt32 MenuIdx)
{
	if(!MenuIdx)
	{
		// Scaling using keyboard
		for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
		{
			CNtlPLObject* pNtlPLCurObj = static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
			RwV3d *pSclCur = const_cast<RwV3d*>(pNtlPLCurObj->GetScale());
			pSclCur->x = m_ScalingValue;
			pSclCur->y = m_ScalingValue;
			pSclCur->z = m_ScalingValue;
			CLAMP(pSclCur->x, 0.0f, 10.0f);
			CLAMP(pSclCur->y, 0.0f, 10.0f);
			CLAMP(pSclCur->z, 0.0f, 10.0f);
			pNtlPLCurObj->SetScale(pSclCur);
		}
	}	
	else if(MenuIdx == 1)
	{
		RwReal ScaleRate = m_EffectTransform[0];
		CLAMP(ScaleRate, 0.0f, 10.0f);
		if (m_vecSelectedEffect.size())
		{
			m_vecSelectedEffect[0]->SetScale(ScaleRate);
		}
	}
}

void CWorldEditorEventHander::DeleteAllPaletteDummies()
{
	CNtlPLGameProperty::SetSelectGameProperty(NULL);
	CNtlPLLightObject::SetToolUI(NULL);
	CNtlPLDecalVolume::SetToolUI(NULL);
	// CZ-SS
	//CNtlPLSpot::SetToolUI(NULL);

	// Soft edged spot light
	// 	if (m_SelectedSS)
	// 	{
	// 		m_SelectedSS = NULL;
	// 	}

	// Occluder
	if (m_pSelectedOccluder)
	{
		m_pSelectedOccluder->SetToolUI(FALSE);
		m_pSelectedOccluder = NULL;
	}

	if (m_pSelectedHeatHaze)
	{
		m_pSelectedHeatHaze->SetToolUI(FALSE);
		m_pSelectedHeatHaze = NULL;
	}

	// delete spawn; current spawn entities
	GetSpawnInstance()->DestroyEntitiesAtCurFields();

	// delete water mark
	if(m_vecSelectedWaterObj.size())
	{
		for(unsigned int p = 0; p < m_vecSelectedWaterObj.size(); ++p)
		{
			GetSceneManager()->DeleteEntity(m_vecSelectedWaterObj[p]);
		}
		m_vecSelectedWaterObj.clear();
	}

	m_bboxSelectWater.sup.x = 0.0f; m_bboxSelectWater.sup.y = 0.0f; m_bboxSelectWater.sup.z = 0.0f;
	m_bboxSelectWater.inf.x = 0.0f; m_bboxSelectWater.inf.y = 0.0f; m_bboxSelectWater.inf.z = 0.0f;

	// delete shadow mark
	if(m_vecSelectedShadowObj.size())
	{
		for(unsigned int p = 0; p < m_vecSelectedShadowObj.size(); ++p)
		{
			GetSceneManager()->DeleteEntity(m_vecSelectedShadowObj[p]);
		}
		m_vecSelectedShadowObj.clear();
	}

	// delete Sector Cull mark
	if(m_vecSelectedSectorCullObj.size())
	{
		for(unsigned int p = 0; p < m_vecSelectedSectorCullObj.size(); ++p)
		{
			GetSceneManager()->DeleteEntity(m_vecSelectedSectorCullObj[p]);
		}
		m_vecSelectedSectorCullObj.clear();
	}

	// delete effect mark
	m_vecSelectedEffect.clear();

	// delete World Light
	SetWLVision(FALSE);
	m_vecSelectedWorldLight.clear();

	// delete Light Object
	DeleteLightObjectPaletteDummies();
	//m_vecSelectedLightObject.clear();

	// delete dojo
	DeleteDojoPaletteDummies();


	// delete volume decal
	m_vecSelectedDecalVolume.clear();

	// delete normal prop. tile
	m_vecTileAttr.clear();

	// delete SE
	SetSEVision(FALSE);
	m_vecSelectedSE.clear();

	// map obj
	m_vecSelectedObj.clear();

	// delete shoreline ui
	m_ShoreLineIOHandler.Delete();

	// delete decal ui
	m_DecalIOHandler.Delete();

	// delete planet handler
	m_PlanetIOHandler.DestroyAll();

	m_pAttachBoneObject		= NULL;
	m_iAttachBoneIdx		= -1;

	m_IdxCopyWater			= -1;

	// brush entity
	if(dGET_BRUSH_ENTITY())
	{
		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
		dGET_BRUSH_ENTITY() = NULL;
		dGET_MAPOBJ_MODE()	= eOEM_IDLE;
		dGET_SPAWN_MODE()	= eSEM_IDLE;
	}

	// brush
	NTL_DELETE(m_pBrushSelector);
	NTL_DELETE(m_pBrushSelectorFrsutum);	
	NTL_DELETE(m_pBrushSelectorPE);
	NTL_DELETE(m_pBrushSelection);
	NTL_DELETE(m_pBrushSelectionSub);
	NTL_DELETE(m_pBrush2D);

	DeleteTerrainPaletteDummies();
}

void CWorldEditorEventHander::OnSectorCullProc(RwInt32 _Menu)
{
	CNtlWorldFieldManager*			pMgr		= dGETMGR();
	CPalettePerformanceSectorCull*	pSectorCull	= dGETFRM()->m_pPerformanceSectorCull;
	CNtlPLObject*					pNtlPLObject;
	RwInt32							IdxSector;
	RwV3d							PosSectorMid;

	switch(_Menu)
	{
	case 0:
		{
			for(unsigned int i = 0; i < m_vecSelectedSectorCullObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedSectorCullObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnSectorCullProc, invalid sector index.");					
				}
				else
				{
					pMgr->m_pSectors[IdxSector].m_IsCulled = TRUE;
				}
			}
		}
		break;

	case 1:
		{
			for(unsigned int i = 0; i < m_vecSelectedSectorCullObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedSectorCullObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnSectorCullProc, invalid sector index.");
				}
				else
				{
					pMgr->m_pSectors[IdxSector].m_IsCulled = FALSE;
				}
			}
		}
		break;
	}
}

RwBool CWorldEditorEventHander::AreThereObjPSMapSet(CNtlWorldSector* _pSector)
{
	RwInt32 Num = static_cast<RwInt32>(_pSector->m_vecNtlPLEntityOrnament.size());

	for(RwInt32 i = 0; i < Num; ++i)
	{
		if((static_cast<CNtlPLObject*>(_pSector->m_vecNtlPLEntityOrnament[i]))->AreThereObjPSMap())
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CWorldEditorEventHander::OnShadowProc(RwInt32 _MenuIdx, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam)
{
	CNtlWorldFieldManager*	pMgr			= dGETMGR();
	CNtlPLWater*			pWater			= pMgr->GetNtlPLWater();
	CPaletteShadow*			pPalette		= dGETFRM()->m_pShadowPalette;
	CNtlPLObject*			pNtlPLObject;
	RwInt32					IdxSector;
	RwV3d					PosSectorMid;


	switch(_MenuIdx)
	{
	case 0:
		{
			for(unsigned int i = 0; i < m_vecSelectedShadowObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedShadowObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnShadowProc, invalid sector index.");
				}
				else
				{
					CNtlWorldShadowManager::GetInst()->Create(PosSectorMid, _NtlWorldShadowParam, pMgr);
				}
			}
		}
		break;

	case 1:
		{
			for(unsigned int i = 0; i < m_vecSelectedShadowObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedShadowObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnShadowProc, invalid sector index.");
				}
				else
				{
					if(AreThereObjPSMapSet(&pMgr->m_pSectors[IdxSector]))
					{
						RwChar Temp[1024];
						sprintf(Temp, "can't delete terrain shadow with object shadow set at %d's sector.", IdxSector);
						AfxMessageBox(Temp);
					}
					else
					{
						CNtlWorldShadowManager::GetInst()->Delete(PosSectorMid, _NtlWorldShadowParam, pMgr);
					}
				}
			}
		}
		break;

	case 2:
		{
			for(unsigned int i = 0; i < m_vecSelectedShadowObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedShadowObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnShadowProc, invalid sector index.");
				}
				else
				{
					if(!pMgr->m_pSectors[IdxSector].m_pNtlWorldShadow->m_pTexShadow)
					{
						RwChar Temp[1024];
						sprintf(Temp, "There's no terrain shadow at %d's Sector.", IdxSector);
						AfxMessageBox(Temp);
					}
					else
					{
						CNtlWorldShadowManager::GetInst()->CreateDoodadShadow(PosSectorMid, _NtlWorldShadowParam, pMgr);
					}
				}
			}
		}
		break;

	case 3:
		{
			for(unsigned int i = 0; i < m_vecSelectedShadowObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedShadowObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnShadowProc, invalid sector index.");
				}
				else
				{
					CNtlWorldShadowManager::GetInst()->DeleteDoodadShadow(PosSectorMid, _NtlWorldShadowParam, pMgr);
				}
			}
		}
		break;

	case 4:
		{
			for(unsigned int i = 0; i < m_vecSelectedShadowObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedShadowObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnShadowProc, invalid sector index.");
				}
				else
				{
					CNtlWorldShadowManager::GetInst()->CreateShadowProp(PosSectorMid, _NtlWorldShadowParam, pMgr);
				}
			}			
		}
		break;

	case 5:
		{
			for(unsigned int i = 0; i < m_vecSelectedShadowObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedShadowObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnShadowProc, invalid sector index.");
				}
				else
				{
					CNtlWorldShadowManager::GetInst()->DeleteShadowProp(PosSectorMid, _NtlWorldShadowParam, pMgr);
				}
			}			
		}
		break;
	case 6:
		{
			for(unsigned int i = 0; i < m_vecSelectedShadowObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedShadowObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnShadowProc, invalid sector index.");
				}
				else
				{
					CNtlWorldShadowManager::GetInst()->UpdateParam(PosSectorMid, _NtlWorldShadowParam, pMgr);
				}
			}
		}
		break;
	}
}

void CWorldEditorEventHander::OnWaterProc(RwInt32 IdxMenu)
{
	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD &&
		((CNtlWEFrm*)AfxGetMainWnd())->GetWorldView()->GetBlockMode() != EBLOCK_MODE_SECTOR)
	{
		AfxMessageBox("change block mode!!!");
		return;
	}

	CNtlWorldFieldManager*	pMgr			= dGETMGR();
	CNtlPLWater*			pWater			= pMgr->GetNtlPLWater();
	CPaletteWater*			pPalette		= dGETFRM()->m_pWaterPalette;
	CNtlPLObject*			pNtlPLObject;
	RwInt32					IdxSector;
	RwV3d					PosSectorMid;


	switch(IdxMenu)
	{
	case 0:
		{
			for(unsigned int i = 0; i < m_vecSelectedWaterObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnWaterProc, invalid sector index.");
				}
				else
				{
					if(pMgr->m_pSectors[IdxSector].m_pWater)
					{
						CString MsgWarning;
						MsgWarning.Format("[%d] sector has got already one. Would you like to overwrite?", IdxSector);
						if(::AfxMessageBox(MsgWarning.GetBuffer(0), MB_YESNO) == IDYES)
						{
							pMgr->OnWaterProc(IdxMenu, IdxSector, pPalette->m_SectorWaterAttr);
						}
					}
					else
					{
						pMgr->OnWaterProc(IdxMenu, IdxSector, pPalette->m_SectorWaterAttr);
					}
				}

				if(!i)
				{
					dGETFRM()->m_pWaterPalette->SetWaterAttr(*pMgr->m_pSectors[IdxSector].m_pWater);
				}
			}
		}
		break;

	case 1:
		{
			for(unsigned int i = 0; i < m_vecSelectedWaterObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnWaterProc, invalid sector index.");
				}
				else
				{
					if(!pMgr->m_pSectors[IdxSector].m_pWater)
					{
						CString MsgWarning;
						MsgWarning.Format("[%d] sector hasn't got water.", IdxSector);
						if(::AfxMessageBox(MsgWarning.GetBuffer(0), MB_OK))
						{
							continue;
						}
					}
					else
					{
						pMgr->OnWaterProc(IdxMenu, IdxSector, pPalette->m_SectorWaterAttr);
					}
				}

				if(!i)
				{
					dGETFRM()->m_pWaterPalette->SetWaterAttr(*pMgr->m_pSectors[IdxSector].m_pWater);
				}
			}
		}
		break;

	case 2:
		{
			for(unsigned int i = 0; i < m_vecSelectedWaterObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnWaterProc, invalid sector index.");
				}
				else
				{
					if(!pMgr->m_pSectors[IdxSector].m_pWater)
					{
						CString MsgWarning;
						MsgWarning.Format("There's no water obj. at %d's sector. Plz create one.", IdxSector);
						if(::AfxMessageBox(MsgWarning.GetBuffer(0), MB_OK))
						{
							continue;
						}
					}
					else
					{
						pMgr->OnWaterProc(IdxMenu, IdxSector, pPalette->m_SectorWaterAttr);
					}
				}
			}

			if(m_vecSelectedWaterObj.size())
			{
				for(unsigned int p = 0; p < m_vecSelectedWaterObj.size(); ++p)
				{
					GetSceneManager()->DeleteEntity(m_vecSelectedWaterObj[p]);
				}
				m_vecSelectedWaterObj.clear();

				if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
				{
					CWorldViewDlg* pWorldView = ((CNtlWEFrm*)AfxGetMainWnd())->GetWorldView();

					m_bboxSelectWater.sup.x = 0.0f; m_bboxSelectWater.sup.y = 0.0f; m_bboxSelectWater.sup.z = 0.0f;
					m_bboxSelectWater.inf.x = 0.0f; m_bboxSelectWater.inf.y = 0.0f; m_bboxSelectWater.inf.z = 0.0f;

					pWorldView->ResetSelectBlock();
				}
			}
		}
		break;

	case 3:
		{
			for(unsigned int i = 0; i < m_vecSelectedWaterObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnWaterProc, invalid sector index.");
				}
				else
				{
					if(!pMgr->m_pSectors[IdxSector].m_pWater)
					{
						CString MsgWarning;
						MsgWarning.Format("There's no water obj. at [%d]'s sector. Plz create one.", IdxSector);
						if(::AfxMessageBox(MsgWarning.GetBuffer(0), IdxSector, MB_OK))
						{
							continue;
						}
					}
					else
					{
						pMgr->OnWaterProc(IdxMenu, IdxSector, pPalette->m_SectorWaterAttr);
					}
				}

				if(!i)
				{
					dGETFRM()->m_pWaterPalette->SetWaterAttr(*pMgr->m_pSectors[IdxSector].m_pWater);
				}
			}
		}
		break;

	case 4:
		{
			for(unsigned int i = 0; i < m_vecSelectedWaterObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnWaterProc, invalid sector index.");					
				}
				else
				{
					if(!pMgr->m_pSectors[IdxSector].m_pWater)
					{
						if(::AfxMessageBox("There's no water to delete.", IdxSector, MB_OK))
						{
							continue;
						}
					}
					else
					{
						pMgr->OnWaterProc(IdxMenu, IdxSector, pPalette->m_SectorWaterAttr);
					}
				}

				if(!i)
				{
					dGETFRM()->m_pWaterPalette->SetWaterAttr(*pMgr->m_pSectors[IdxSector].m_pWater);
				}
			}
		}
		break;

	case 5:
		{
			pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[0]);
			PosSectorMid	= pNtlPLObject->GetPosition();
			IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

			if(!pMgr->m_pSectors[IdxSector].m_pWater)
			{
				if(::AfxMessageBox("Some sectors are not available to unify heights. Plz retry.", IdxSector, MB_OK))
				{
					return;
				}
			}

			// get the highest one
			RwReal CurHeight = 0.0f;

			if(m_vecSelectedWaterObj.size())
			{				
				// be careful with null data water
				for(unsigned int i = 0; i < m_vecSelectedWaterObj.size(); ++i)
				{
					pNtlPLObject		= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[i]);
					PosSectorMid		= pNtlPLObject->GetPosition();
					IdxSector			= pMgr->GetSectorIdx(PosSectorMid);

					if(pMgr->m_pSectors[IdxSector].m_pWater)
					{
						CurHeight = pMgr->m_pSectors[IdxSector].m_pWater->_Height;
						break;
					}
				}

				// get the highest one to link
				for(unsigned int i = 0; i < m_vecSelectedWaterObj.size(); ++i)
				{
					pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[i]);
					PosSectorMid	= pNtlPLObject->GetPosition();
					IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

					if(pMgr->m_pSectors[IdxSector].m_pWater)
					{
						if(CurHeight < pMgr->m_pSectors[IdxSector].m_pWater->_Height)
						{
							CurHeight = pMgr->m_pSectors[IdxSector].m_pWater->_Height;
						}
					}
				}
			}

			for(unsigned int i = 0; i < m_vecSelectedWaterObj.size(); ++i)
			{
				pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[i]);
				PosSectorMid	= pNtlPLObject->GetPosition();
				IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(IdxSector == -1)
				{
					DBO_TRACE(FALSE, "CWorldEditorEventHander::OnWaterProc, invalid sector index.");
				}
				else
				{
					if(!pMgr->m_pSectors[IdxSector].m_pWater)
					{
						CString MsgWarning;
						MsgWarning.Format("There's no water obj. at [%d]'s sector.", IdxSector);
						if(::AfxMessageBox(MsgWarning.GetBuffer(0), MB_OK))
						{
							continue;
						}
					}
					else
					{
						pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[i]);
						PosSectorMid	= pNtlPLObject->GetPosition();
						IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

						pMgr->m_pSectors[IdxSector].m_pWater->_Height = CurHeight;
						pMgr->GetNtlPLWater()->OnMove(&pMgr->m_pSectors[IdxSector], CurHeight, TRUE);
					}
				}
			}
		}
		break;
	}
}

VOID CWorldEditorEventHander::SetEffectsDummy(RwBool _Flag)
{
	if(_Flag)
	{
		CNtlWorldFieldManager*	pMgr	= dGETMGR();
		CNtlWorldField*			pField	= const_cast<CNtlWorldField*>(pMgr->GetFields());

		for(int j = 0; j < 36; ++j)
		{
			int		SectorIdx;
			RwV3d	SectorSPos;
			int		IdxField = pMgr->m_Fields6x6[1][j];

			if(IdxField == -1)
			{
				DBO_TRACE(FALSE, "CWorldEditorEventHander::SetEffectsDummy, invalid field index.");
				continue;
			}

			RwV3d	SPos = pField[IdxField].GetSPos();

			for(int l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				for(int m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
				{
					SectorSPos.x = (RwReal)m;
					SectorSPos.z = (RwReal)l;
					SectorIdx = pMgr->GetSectorIdx(SectorSPos);

					if(SectorIdx == -1)
					{
						continue;
					}

					int NumDoodads	= pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntityEffect.size();
					for(int k = 0; k < NumDoodads; ++k)
					{
						(static_cast<CNtlInstanceEffect*>(pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntityEffect[k]))->SetVisibleBoundingVolume(TRUE);
					}
				}
			}
		}
	}
	else
	{
		CNtlWorldFieldManager*	pMgr	= dGETMGR();
		CNtlWorldField*			pField	= const_cast<CNtlWorldField*>(pMgr->GetFields());

		for(int j = 0; j < 36; ++j)
		{
			int		SectorIdx;
			RwV3d	SectorSPos;
			int		IdxField	= pMgr->m_Fields6x6[1][j];

			if(IdxField == -1)
			{
				continue;
			}

			RwV3d	SPos		= pField[IdxField].GetSPos();

			for(int l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				for(int m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
				{
					SectorSPos.x = (RwReal)m;
					SectorSPos.z = (RwReal)l;
					SectorIdx = pMgr->GetSectorIdx(SectorSPos);

					if(SectorIdx != -1)
					{
						int NumDoodads	= pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntityEffect.size();
						for(int k = 0; k < NumDoodads; ++k)
						{
							(static_cast<CNtlInstanceEffect*>(pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntityEffect[k]))->SetVisibleBoundingVolume(FALSE);
						}
					}
				}
			}
		}
	}
}

void CWorldEditorEventHander::UpdateShadow()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	// control
	if(m_vecSelectedShadowObj.size() == 1)
	{
		if(dOKEY(DIK_P))
		{
			RwV3d			PosCurField = m_vecSelectedShadowObj[0]->GetPosition();
			CNtlWorldField*	pFields		= const_cast<CNtlWorldField*>(pMgr->GetFields());
			RwInt32			IdxShadowSec;

			IdxShadowSec = pMgr->GetSectorIdx(PosCurField);
			if(IdxShadowSec == -1)
			{
				CNtlWorldObjVisionAnalyzer::GetInstance()->SetCheck(eNWOVA_PSTEX, FALSE);
				DBO_TRACE(FALSE, "CWorldEditorEventHander::UpdateShadow, invalid sector index.");
			}
			else
			{
				if(pMgr->CNtlWorldSectorManager::m_pSectors[IdxShadowSec].m_pNtlWorldShadow->m_pTexShadow)
				{
					CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_PSTEX);

					if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PSTEX))
					{
						SetCursor(NULL);
					}
					else
					{
						SetCursor(LoadCursor(NULL,IDC_ARROW)); 
					}
				}
				else
				{
					CNtlWorldObjVisionAnalyzer::GetInstance()->SetCheck(eNWOVA_PSTEX, FALSE);
					AfxMessageBox("Shadow isn't created at the sector.", MB_OK);
				}
			}
		}
	}
	else
	{
		CNtlWorldObjVisionAnalyzer::GetInstance()->SetCheck(eNWOVA_PSTEX, FALSE);
	}

	if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PSTEX))
	{
		CNtlWorldShadowManager::GetInst()->UpdateSectorPSTex(m_vecSelectedShadowObj, pMgr);

		return;
	}

	// clear all shadow sector's display obj.
	if(dOKEY(DIK_SPACE))
	{
		if(m_vecSelectedShadowObj.size())
		{
			for(unsigned int p = 0; p < m_vecSelectedShadowObj.size(); ++p)
			{
				GetSceneManager()->DeleteEntity(m_vecSelectedShadowObj[p]);
			}
			m_vecSelectedShadowObj.clear();
		}
	}

	if(dGET_MOUSE()->GetLBUp())
	{
		if(m_pBrushSelector)
		{
			CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());

			RwV3d OldPosS = pCurBrush->GetPosS();
			RwV3d PosS = pCurBrush->GetPosS();
			RwV3d PosE = pCurBrush->GetPosE();
			RwV3d FPosS;
			RwV3d FPosE;

			if(PosS.x <= PosE.x)
			{
				FPosS.x = PosS.x;
				FPosE.x = PosE.x;
			}
			else
			{
				FPosS.x = PosE.x;
				FPosE.x = PosS.x;
			}

			if(PosS.z <= PosE.z)
			{
				FPosS.z = PosS.z;
				FPosE.z = PosE.z;
			}
			else
			{
				FPosS.z = PosE.z;
				FPosE.z = PosS.z;
			}

			PosS = FPosS;
			PosE = FPosE;

			vector<RwInt32> vecTmpSectorIdx;

			for(RwReal i = PosS.z; i < PosE.z; i += dGET_WORLD_PARAM()->WorldSectorTileSize)
			{
				for(RwReal j = PosS.x; j < PosE.x; j += dGET_WORLD_PARAM()->WorldSectorTileSize)
				{
					RwV3d TmpPos;
					TmpPos.x = j;
					TmpPos.z = i;
					TmpPos.y = pMgr->GetWorldSectorHeight(TmpPos);
					unsigned int Idx = static_cast<unsigned int>(pMgr->GetSectorIdx(TmpPos));

					RwBool Flag = FALSE;
					for(unsigned int b = 0; b < vecTmpSectorIdx.size(); ++b)
					{
						if(vecTmpSectorIdx[b] == Idx)
						{
							Flag = TRUE;
							break;
						}
					}

					if(!Flag)
					{
						vecTmpSectorIdx.push_back(Idx);
					}						
				}
			}

			for(unsigned int k = 0; k < vecTmpSectorIdx.size(); ++k)
			{
				RwV3d TmpPos;
				TmpPos.x = pMgr->CNtlWorldSectorManager::m_pSectors[vecTmpSectorIdx[k]].DatumPoint.x;
				TmpPos.z = pMgr->CNtlWorldSectorManager::m_pSectors[vecTmpSectorIdx[k]].DatumPoint.z;
				TmpPos.y = pMgr->GetWorldSectorHeight(TmpPos);

				SPLEntityCreateParam PLEntityCreateParam;
				PLEntityCreateParam.pPos = &TmpPos;

				CNtlPLObject *pNtlPLObject = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "TEXT_SHADOW", &PLEntityCreateParam));						
				DBO_ASSERT(pNtlPLObject, "Entity create failed.");

				pNtlPLObject->SetFadeEnable(FALSE);
				m_vecSelectedShadowObj.push_back(pNtlPLObject);
			}

			// display
			if(m_vecSelectedShadowObj.size())
			{
				if(pMgr->m_pSectors[pMgr->GetSectorIdx(pCurBrush->GetPosS())].m_pNtlWorldShadow)
				{	
					dGETFRM()->m_pShadowPalette->SetShadowAttr(pMgr->m_pSectors[pMgr->GetSectorIdx(OldPosS)].m_pNtlWorldShadow);
				}
			}

			vecTmpSectorIdx.clear();

			NTL_DELETE(m_pBrushSelector);
		}
	}

	if(dGET_MOUSE()->GetLBOnce())
	{
		if(!m_pBrushSelector)
		{
			for(unsigned int p = 0; p < m_vecSelectedShadowObj.size(); ++p)
			{
				GetSceneManager()->DeleteEntity(m_vecSelectedShadowObj[p]);
			}
			m_vecSelectedShadowObj.clear();

			m_pBrushSelector = NTL_NEW CNtlWorldBrush;

			CNtlWorldBrushController *pController = new CNtlWB_Rectangle<CNtlWorldFieldManager>(eNBT_RECTANGLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
			m_pBrushSelector->SetController(pController);

			CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
			pCurBrush->SetPosS(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
		}
	}

	if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
	{
		CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
		pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
	}

	// copy and paste
	if(m_vecSelectedShadowObj.size() == 1)
	{
		if(dKEY(DIK_LCONTROL) && dOKEY(DIK_C))
		{
			CNtlPLObject*	pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedShadowObj[0]);
			RwV3d			PosSectorMid	= pNtlPLObject->GetPosition();

			RwInt32 IdxTmp = pMgr->GetSectorIdx(PosSectorMid);
			if(IdxTmp != -1 && pMgr->m_pSectors[IdxTmp].m_pNtlWorldShadow->m_pTexShadow)
			{
				m_IdxCopyShadow = pMgr->GetSectorIdx(PosSectorMid);
			}
		}
	}

	if(m_vecSelectedShadowObj.size())
	{
		if(dKEY(DIK_LCONTROL) && dOKEY(DIK_V))
		{
			if(m_vecSelectedShadowObj.size() && m_IdxCopyShadow != -1)
			{
				for(unsigned int p = 0; p < m_vecSelectedShadowObj.size(); ++p)
				{
					CNtlPLObject*	pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedShadowObj[p]);
					RwV3d			PosSectorMid	= pNtlPLObject->GetPosition();
					RwInt32			IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

					if(IdxSector != m_IdxCopyShadow)
					{
						pMgr->m_pSectors[IdxSector].m_pNtlWorldShadow->Delete();

						sNTL_WORLD_SHADOW_PARAM sNtlWorldShadowParam;
						::CopyMemory(&sNtlWorldShadowParam, &pMgr->m_pSectors[m_IdxCopyShadow].m_pNtlWorldShadow->m_NtlWorldShadowParam, sizeof(sNTL_WORLD_SHADOW_PARAM));

						CNtlWorldShadowManager::GetInst()->Create(PosSectorMid, sNtlWorldShadowParam, pMgr);
					}			
				}
			}
		}
	}
}

RpCollisionTriangle* CWorldEditorEventHander::IntersectionWaterTriCB(RpIntersection* _pIntersection, RpCollisionTriangle* _pCollTriangle, RwReal _Distance, void* _pData)
{
	sNtlWorldCollisionInfo*	pNtlWorldCollisionInfo = static_cast<sNtlWorldCollisionInfo*>(_pData);

	if(_Distance < pNtlWorldCollisionInfo->fDist)
	{
		pNtlWorldCollisionInfo->fDist				= _Distance;
		pNtlWorldCollisionInfo->IsCollidedAtSectors = TRUE;
		pNtlWorldCollisionInfo->RayIntersectionPt4Terrain.x	= _pIntersection->t.line.start.x + _Distance * (_pIntersection->t.line.end.x - _pIntersection->t.line.start.x);
		pNtlWorldCollisionInfo->RayIntersectionPt4Terrain.y	= _pIntersection->t.line.start.y + _Distance * (_pIntersection->t.line.end.y - _pIntersection->t.line.start.y);
		pNtlWorldCollisionInfo->RayIntersectionPt4Terrain.z	= _pIntersection->t.line.start.z + _Distance * (_pIntersection->t.line.end.z - _pIntersection->t.line.start.z);
	}

	return _pCollTriangle;
}

VOID CWorldEditorEventHander::SetWaterBrushRadius(RwReal _Radius)
{
	if(m_pBrush2D)
	{
		CNtlWB_Circle2D<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Circle2D<CNtlWorldFieldManager>*>(m_pBrush2D->GetController());
		pCurBrush->SetRadius(_Radius);
	}
}

VOID CWorldEditorEventHander::UpdateDepthMapAlpha()
{
	CNtlWorldFieldManager*					pMgr		= dGETMGR();
	CNtlWB_Circle2D<CNtlWorldFieldManager>* pCurBrush	= reinterpret_cast<CNtlWB_Circle2D<CNtlWorldFieldManager>*>(m_pBrush2D->GetController());

	if(!pCurBrush->GetEnable())
	{
		return;
	}

	if(!dGET_MOUSE()->GetBehavior(MB_LBDN))
	{
		return;
	}

	RwV3d	CurPos			= pCurBrush->GetPos();
	RwInt32	CurSectorIdx	= pMgr->GetSectorIdx(CurPos);
	RwReal	CurRadius		= dGETFRM()->m_pWaterPalette->GetCurRadius();

	vector<RwInt32> vecNSec;
	pMgr->GetNeighborSectors(CurSectorIdx, vecNSec);
	vecNSec.push_back(CurSectorIdx);

	for(RwUInt32 i = 0; i < vecNSec.size(); ++i)
	{
		if(vecNSec[i] == -1)
		{
			continue;
		}

		RwTexture* pDepthMap = NULL;
		if(pMgr->m_pSectors[vecNSec[i]].m_pWater)
		{
			pDepthMap = pMgr->m_pSectors[vecNSec[i]].m_pWater->_pDepthMap;
		}

		if(!pDepthMap)
		{
			continue;
		}

		RwV2d	PosSector;
		RwV2d	TexPt;
		RwInt32	AlphaVariation	= static_cast<RwInt32>(static_cast<RwReal>(dGETFRM()->m_pWaterPalette->GetCurOpa()) / 10.0f);
		RwInt32	DepthMapSize	= RwRasterGetWidth(pDepthMap->raster);
		RwInt32 CurSIdx			= vecNSec[i];
		RwReal	RelativeSize;
		float	Distance;

		for(int i = 0; i < DepthMapSize; ++i)
		{
			for(int j = 0; j < DepthMapSize; ++j)
			{
				PosSector.x		= pMgr->m_pSectors[CurSIdx].DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
				PosSector.y		= pMgr->m_pSectors[CurSIdx].DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
				RelativeSize	= (RwReal)dGET_WORLD_PARAM()->WorldSectorSize / (RwReal)DepthMapSize;
				TexPt.x			= static_cast<RwReal>(j + 1) * RelativeSize - (RelativeSize / 2.0f) + PosSector.x;
				TexPt.y			= static_cast<RwReal>(i + 1) * RelativeSize - (RelativeSize / 2.0f) + PosSector.y;
				Distance		= sqrtf((CurPos.x - TexPt.x) * (CurPos.x - TexPt.x) + (CurPos.z - TexPt.y) * (CurPos.z - TexPt.y));

				if(Distance <= CurRadius)
				{
					TexPt.x	= static_cast<RwReal>(DepthMapSize) - ((fabs(TexPt.x - PosSector.x) / (float)dGET_WORLD_PARAM()->WorldSectorSize) * static_cast<RwReal>(DepthMapSize));
					TexPt.y	= static_cast<RwReal>(DepthMapSize) - ((fabs(TexPt.y - PosSector.y) / (float)dGET_WORLD_PARAM()->WorldSectorSize) * static_cast<RwReal>(DepthMapSize));
					TexPt.x	= static_cast<RwReal>(::abs(TexPt.x));
					TexPt.y	= static_cast<RwReal>(::abs(TexPt.y));
					CLAMP(TexPt.x, 0, DepthMapSize - 1);
					CLAMP(TexPt.y, 0, DepthMapSize - 1);

					if(dKEY(DIK_LCONTROL))
					{
						CNtlPLResourceManager::GetInstance()->SetAlphaValueInc(pDepthMap, AlphaVariation, TexPt);
					}
					else
					{		
						CNtlPLResourceManager::GetInstance()->SetAlphaValueDec(pDepthMap, AlphaVariation, TexPt);
					}
				}
			}
		}
	}
}

void CWorldEditorEventHander::UpdateDepthMap()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	// delete the existing "水" ui entities
	if(m_vecSelectedWaterObj.size())
	{
		for(unsigned int p = 0; p < m_vecSelectedWaterObj.size(); ++p)
		{
			GetSceneManager()->DeleteEntity(m_vecSelectedWaterObj[p]);
		}
		m_vecSelectedWaterObj.clear();

		if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
		{
			CWorldViewDlg* pWorldView = ((CNtlWEFrm*)AfxGetMainWnd())->GetWorldView();

			m_bboxSelectWater.sup.x = 0.0f; m_bboxSelectWater.sup.y = 0.0f; m_bboxSelectWater.sup.z = 0.0f;
			m_bboxSelectWater.inf.x = 0.0f; m_bboxSelectWater.inf.y = 0.0f; m_bboxSelectWater.inf.z = 0.0f;

			pWorldView->ResetSelectBlock();
		}
	}

	// update depthmap brush
	if(!m_pBrush2D)
	{
		RwReal Radius = dGETFRM()->m_pWaterPalette->GetCurRadius();

		m_pBrush2D								= NTL_NEW CNtlWorldBrush;
		CNtlWorldBrushController *pController	= new CNtlWB_Circle2D<CNtlWorldFieldManager>(eNBT_2D_CIRCLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight, Radius);
		m_pBrush2D->SetController(pController);
	}

	// size radius
	if(dGET_MOUSE()->GetWheelMovement())
	{
		RwReal Value = static_cast<RwReal>(dGET_MOUSE()->GetWheelMovement() / ::fabsf(dGET_MOUSE()->GetWheelMovement()));

		CNtlWB_Circle2D<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Circle2D<CNtlWorldFieldManager>*>(m_pBrush2D->GetController());
		pCurBrush->SetRadius(Value, TRUE);

		dGETFRM()->m_pWaterPalette->Refresh(pCurBrush->GetRadius());
	}

	// pick water get height and position
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwV3d					SPos;
	RwInt32					l, m;
	RwV3d					SectorSPos;
	RwInt32					SectorIdx;

	// collision param. definition
	sNtlWorldCollisionInfo	NtlWorldCollisionInfo;
	NtlWorldCollisionInfo.IsCollidedAtSectors		= FALSE;
	NtlWorldCollisionInfo.fDist						= RwRealMAXVAL;

	RwV2d ScreenPos;
	ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
	ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

	RwLine cameraRay;
	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

	for(RwInt32 i = 0; i < 36; ++i)
	{
		RwInt32 CurFieldIdx = pMgr->m_Fields6x6[1][i];
		if(CurFieldIdx == -1)
		{
			continue;
		}

		SPos = pFields[CurFieldIdx].GetSPos();

		for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x	= (RwReal)m;
				SectorSPos.z	= (RwReal)l;
				SectorIdx		= pMgr->GetSectorIdx(SectorSPos);

				if(SectorIdx == -1)
				{
					continue;
				}

				RpAtomic* pCurAtomic = pMgr->m_pSectors[SectorIdx].m_pWater ? pMgr->m_pSectors[SectorIdx].m_pWater->_pAtom : NULL;
				if(pCurAtomic)
				{
					RpIntersection TempIS;
					TempIS.t.line.start = cameraRay.start;
					TempIS.t.line.end	= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
					TempIS.type			= rpINTERSECTLINE;

					// scale
					RwV3d vRayDir;
					RwV3dSubMacro(&vRayDir, &TempIS.t.line.end, &TempIS.t.line.start);
					RwV3dNormalize(&vRayDir, &vRayDir);

					RwV3dIncrementScaled(&TempIS.t.line.end, &vRayDir, 1000.0f);

					RpAtomicForAllIntersections(pCurAtomic, &TempIS, IntersectionWaterTriCB, &NtlWorldCollisionInfo);
				}
			}
		}
	}

	if(NtlWorldCollisionInfo.IsCollidedAtSectors)
	{
		//RwV3d	CamPt = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
		//RwReal	Cam2T = CNtlMath::MathRwV3dSquaredLength(&(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain - CamPt));
		//RwReal	Cam2W = CNtlMath::MathRwV3dSquaredLength(&(NtlWorldCollisionInfo.RayIntersectionPt4Terrain - CamPt));

		RwV3d	PickPos;
		//if(Cam2W < Cam2T)
		{
			PickPos = NtlWorldCollisionInfo.RayIntersectionPt4Terrain;

			CNtlWB_Circle2D<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Circle2D<CNtlWorldFieldManager>*>(m_pBrush2D->GetController());
			pCurBrush->SetPos(PickPos);

			if(pMgr->m_pSectors[pMgr->GetSectorIdx(PickPos)].m_pWater)
			{
				if(pMgr->m_pSectors[pMgr->GetSectorIdx(PickPos)].m_pWater->_pDepthMap)
				{
					pCurBrush->SetEnable(TRUE);
				}
				else
				{
					pCurBrush->SetEnable(FALSE);
				}
			}
			else
			{
				pCurBrush->SetEnable(FALSE);
			}
		}
		/*
		else
		{
		PickPos = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;

		CNtlWB_Circle2D<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Circle2D<CNtlWorldFieldManager>*>(m_pBrush2D->GetController());
		pCurBrush->SetPos(PickPos);
		pCurBrush->SetEnable(FALSE);
		}*/

	}
	else
	{
		CNtlWB_Circle2D<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Circle2D<CNtlWorldFieldManager>*>(m_pBrush2D->GetController());
		pCurBrush->SetEnable(FALSE);
	}

	// brush current including depthmap
	UpdateDepthMapAlpha();
}

void CWorldEditorEventHander::UpdateWater()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	if(m_pBrush2D)
	{
		NTL_DELETE(m_pBrush2D);
		m_pBrush2D = NULL;
	}

	if(dOKEY(DIK_DELETE))
	{
		if(::AfxMessageBox("Are you sure?", MB_YESNO) == IDYES)
		{
			OnWaterProc(2);
		}
	}

	// up and down
	if(dKEY(DIK_LSHIFT))
	{
		if(dOKEY(DIK_PGUP))
		{
			for(unsigned int i = 0; i < m_vecSelectedWaterObj.size(); ++i)
			{
				CNtlPLObject*	pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[i]);
				RwV3d			PosSectorMid	= pNtlPLObject->GetPosition();
				RwInt32			IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(!pMgr->m_pSectors[IdxSector].m_pWater)
				{
					if(::AfxMessageBox("Some Sectors are not available to move, plz retry"))
					{
						return;
					}
				}

				pMgr->GetNtlPLWater()->OnMove(&pMgr->m_pSectors[IdxSector], +dWATER_MOVE_HEIGHT_TAB);

				if(!i)
				{
					dGETFRM()->m_pWaterPalette->SetWaterAttr(*pMgr->m_pSectors[IdxSector].m_pWater);
				}
			}
		}

		if(dOKEY(DIK_PGDN))
		{
			for(unsigned int i = 0; i < m_vecSelectedWaterObj.size(); ++i)
			{
				CNtlPLObject*	pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[i]);
				RwV3d			PosSectorMid	= pNtlPLObject->GetPosition();
				RwInt32			IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(!pMgr->m_pSectors[IdxSector].m_pWater)
				{
					if(::AfxMessageBox("Some Sectors are not available to move, plz retry"))
					{
						return;
					}
				}

				pMgr->GetNtlPLWater()->OnMove(&pMgr->m_pSectors[IdxSector], -dWATER_MOVE_HEIGHT_TAB);

				if(!i)
				{
					dGETFRM()->m_pWaterPalette->SetWaterAttr(*pMgr->m_pSectors[IdxSector].m_pWater);
				}
			}
		}
	}
	else
	{
		if(dKEY(DIK_PGUP))
		{
			for(unsigned int i = 0; i < m_vecSelectedWaterObj.size(); ++i)
			{
				CNtlPLObject*	pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[i]);
				RwV3d			PosSectorMid	= pNtlPLObject->GetPosition();
				RwInt32			IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(!pMgr->m_pSectors[IdxSector].m_pWater)
				{
					if(::AfxMessageBox("Some Sectors are not available to move, plz retry"))
					{
						return;
					}
				}

				pMgr->GetNtlPLWater()->OnMove(&pMgr->m_pSectors[IdxSector], +dWATER_MOVE_HEIGHT_MPS);

				if(!i)
				{
					dGETFRM()->m_pWaterPalette->SetWaterAttr(*pMgr->m_pSectors[IdxSector].m_pWater);
				}
			}
		}

		if(dKEY(DIK_PGDN))
		{
			for(unsigned int i = 0; i < m_vecSelectedWaterObj.size(); ++i)
			{
				CNtlPLObject*	pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[i]);
				RwV3d			PosSectorMid	= pNtlPLObject->GetPosition();
				RwInt32			IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

				if(!pMgr->m_pSectors[IdxSector].m_pWater)
				{
					if(::AfxMessageBox("Some Sectors are not available to move, plz retry"))
					{
						return;
					}
				}

				pMgr->GetNtlPLWater()->OnMove(&pMgr->m_pSectors[IdxSector], -dWATER_MOVE_HEIGHT_MPS);

				if(!i)
				{
					dGETFRM()->m_pWaterPalette->SetWaterAttr(*pMgr->m_pSectors[IdxSector].m_pWater);
				}
			}
		}
	}

	// copy and paste
	if(m_vecSelectedWaterObj.size() == 1)
	{
		if(dKEY(DIK_LCONTROL) && dOKEY(DIK_C))
		{
			CNtlPLObject*	pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[0]);
			RwV3d			PosSectorMid	= pNtlPLObject->GetPosition();

			RwInt32 IdxTmp = pMgr->GetSectorIdx(PosSectorMid);
			if(IdxTmp != -1 && pMgr->m_pSectors[IdxTmp].m_pWater)
			{
				m_IdxCopyWater = pMgr->GetSectorIdx(PosSectorMid);
			}
		}
	}

	if(m_vecSelectedWaterObj.size())
	{
		if(dKEY(DIK_LCONTROL) && dOKEY(DIK_V))
		{
			if(m_vecSelectedWaterObj.size() && m_IdxCopyWater != -1)
			{
				if (pMgr->m_pSectors[m_IdxCopyWater].m_pWater)
				{
					for(unsigned int p = 0; p < m_vecSelectedWaterObj.size(); ++p)
					{
						CNtlPLObject*	pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedWaterObj[p]);
						RwV3d			PosSectorMid	= pNtlPLObject->GetPosition();
						RwInt32			IdxSector		= pMgr->GetSectorIdx(PosSectorMid);

						if(IdxSector != m_IdxCopyWater)
						{

							pMgr->GetNtlPLWater()->OnCreate(&pMgr->m_pSectors[IdxSector], *pMgr->m_pSectors[m_IdxCopyWater].m_pWater);

							if(pMgr->m_pSectors[m_IdxCopyWater].m_pWater->_pDepthMap)
							{
								pMgr->OnWaterProc(3, IdxSector, *pMgr->m_pSectors[IdxSector].m_pWater);
							}
						}			
					}
				}
				else
				{
					AfxMessageBox("Invalid CopyData.");
				}
			}
		}
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		switch(dGET_WATER_MODE())
		{
		case eWEM_IDLE:
			{
				if(dOKEY(DIK_SPACE))
				{
					if(m_vecSelectedWaterObj.size())
					{
						for(unsigned int p = 0; p < m_vecSelectedWaterObj.size(); ++p)
						{
							GetSceneManager()->DeleteEntity(m_vecSelectedWaterObj[p]);
						}
						m_vecSelectedWaterObj.clear();

						if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
						{
							CWorldViewDlg* pWorldView = ((CNtlWEFrm*)AfxGetMainWnd())->GetWorldView();

							m_bboxSelectWater.sup.x = 0.0f; m_bboxSelectWater.sup.y = 0.0f; m_bboxSelectWater.sup.z = 0.0f;
							m_bboxSelectWater.inf.x = 0.0f; m_bboxSelectWater.inf.y = 0.0f; m_bboxSelectWater.inf.z = 0.0f;

							pWorldView->ResetSelectBlock();
						}
					}
				}

				if(dGET_MOUSE()->GetLBUp())
				{
					if(m_pBrushSelector)
					{
						CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());

						RwV3d OldPosS = pCurBrush->GetPosS();
						RwV3d PosS = pCurBrush->GetPosS();
						RwV3d PosE = pCurBrush->GetPosE();
						RwV3d FPosS;
						RwV3d FPosE;

						if(PosS.x <= PosE.x)
						{
							FPosS.x = PosS.x;
							FPosE.x = PosE.x;
						}
						else
						{
							FPosS.x = PosE.x;
							FPosE.x = PosS.x;
						}

						if(PosS.z <= PosE.z)
						{
							FPosS.z = PosS.z;
							FPosE.z = PosE.z;
						}
						else
						{
							FPosS.z = PosE.z;
							FPosE.z = PosS.z;
						}

						PosS = FPosS;
						PosE = FPosE;

						vector<RwInt32> vecTmpSectorIdx;

						for(RwReal i = PosS.z; i < PosE.z; i += dGET_WORLD_PARAM()->WorldSectorTileSize)
						{
							for(RwReal j = PosS.x; j < PosE.x; j += dGET_WORLD_PARAM()->WorldSectorTileSize)
							{
								RwV3d TmpPos;
								TmpPos.x = j;
								TmpPos.z = i;
								TmpPos.y = pMgr->GetWorldSectorHeight(TmpPos);
								unsigned int Idx = static_cast<unsigned int>(pMgr->GetSectorIdx(TmpPos));

								RwBool Flag = FALSE;
								for(unsigned int b = 0; b < vecTmpSectorIdx.size(); ++b)
								{
									if(vecTmpSectorIdx[b] == Idx)
									{
										Flag = TRUE;
										break;
									}
								}

								if(!Flag)
								{
									vecTmpSectorIdx.push_back(Idx);
								}						
							}
						}

						for(unsigned int k = 0; k < vecTmpSectorIdx.size(); ++k)
						{
							RwV3d TmpPos;
							TmpPos.x = pMgr->CNtlWorldSectorManager::m_pSectors[vecTmpSectorIdx[k]].DatumPoint.x;
							TmpPos.z = pMgr->CNtlWorldSectorManager::m_pSectors[vecTmpSectorIdx[k]].DatumPoint.z;
							TmpPos.y = pMgr->GetWorldSectorHeight(TmpPos);

							SPLEntityCreateParam PLEntityCreateParam;
							PLEntityCreateParam.pPos = &TmpPos;

							CNtlPLObject *pNtlPLObject = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "TEXT_WATER", &PLEntityCreateParam));						
							DBO_ASSERT(pNtlPLObject, "Entity create failed.");

							pNtlPLObject->SetFadeEnable(FALSE);
							m_vecSelectedWaterObj.push_back(pNtlPLObject);
						}

						// display
						if(m_vecSelectedWaterObj.size())
						{
							if(pMgr->m_pSectors[pMgr->GetSectorIdx(pCurBrush->GetPosS())].m_pWater)
							{	
								dGETFRM()->m_pWaterPalette->SetWaterAttr(*pMgr->m_pSectors[pMgr->GetSectorIdx(OldPosS)].m_pWater);
							}
						}

						vecTmpSectorIdx.clear();

						NTL_DELETE(m_pBrushSelector);
					}
				}

				if(dGET_MOUSE()->GetLBOnce())
				{
					if(!m_pBrushSelector)
					{
						for(unsigned int p = 0; p < m_vecSelectedWaterObj.size(); ++p)
						{
							GetSceneManager()->DeleteEntity(m_vecSelectedWaterObj[p]);
						}
						m_vecSelectedWaterObj.clear();

						m_pBrushSelector = NTL_NEW CNtlWorldBrush;

						CNtlWorldBrushController *pController = new CNtlWB_Rectangle<CNtlWorldFieldManager>(eNBT_RECTANGLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
						m_pBrushSelector->SetController(pController);

						CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
						pCurBrush->SetPosS(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
					}
				}

				if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
				{
					if(m_pBrushSelector)
					{
						CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
						pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
					}
				}
			}
			break;
		}
	}
	else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		CWorldViewDlg* pWorldView = ((CNtlWEFrm*)AfxGetMainWnd())->GetWorldView();
		if ((dGET_WATER_MODE() == eWEM_IDLE && dOKEY(DIK_SPACE) ||
			(pWorldView->GetBlockMode() != EBLOCK_MODE_SECTOR)))
		{
			for(unsigned int p = 0; p < m_vecSelectedWaterObj.size(); ++p)
			{
				GetSceneManager()->DeleteEntity(m_vecSelectedWaterObj[p]);
			}
			m_vecSelectedWaterObj.clear();

			m_bboxSelectWater.sup.x = 0.0f; m_bboxSelectWater.sup.y = 0.0f; m_bboxSelectWater.sup.z = 0.0f;
			m_bboxSelectWater.inf.x = 0.0f; m_bboxSelectWater.inf.y = 0.0f; m_bboxSelectWater.inf.z = 0.0f;

			pWorldView->ResetSelectBlock();
		}
		else if ((dGET_WATER_MODE() == eWEM_IDLE) && (pWorldView->GetBlockMode() == EBLOCK_MODE_SECTOR))
		{
			RwV3d vInfCur, vSupCur;

			if (pWorldView->GetSelectBlockPos(&vSupCur, &vInfCur))
			{
				if (vSupCur != m_bboxSelectWater.sup || vInfCur != m_bboxSelectWater.inf)
				{
					for(unsigned int p = 0; p < m_vecSelectedWaterObj.size(); ++p)
					{
						GetSceneManager()->DeleteEntity(m_vecSelectedWaterObj[p]);
					}
					m_vecSelectedWaterObj.clear();

					m_bboxSelectWater.sup = vSupCur;
					m_bboxSelectWater.inf = vInfCur;

					for (RwReal z = vInfCur.z; z < vSupCur.z; z += pWorldView->GetBlockSize())
					{
						for (RwReal x = vInfCur.x; x < vSupCur.x; x += pWorldView->GetBlockSize())
						{
							RwV3d vTemp;
							vTemp.x = x;
							vTemp.z = z;
							vTemp.y = RpWorldGetBBox(CNtlPLGlobal::m_pRpWorld)->sup.y;

							RwInt32 iIdx = pMgr->GetSectorIdx(vTemp);
							if (iIdx != -1)
							{
								vTemp.x = pMgr->CNtlWorldSectorManager::m_pSectors[iIdx].DatumPoint.x;
								vTemp.z = pMgr->CNtlWorldSectorManager::m_pSectors[iIdx].DatumPoint.z;

								SPLEntityCreateParam PLEntityCreateParam;
								PLEntityCreateParam.pPos = &vTemp;

								CNtlPLObject *pNtlPLObject = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "TEXT_WATER", &PLEntityCreateParam));						
								DBO_ASSERT(pNtlPLObject, "Entity create failed.");

								pNtlPLObject->SetFadeEnable(FALSE);
								m_vecSelectedWaterObj.push_back(pNtlPLObject);
							}
						}
					}
					// display
					if(m_vecSelectedWaterObj.size())
					{
						if(pMgr->m_pSectors[pMgr->GetSectorIdx(pWorldView->GetSelectBlockMidPos())].m_pWater)
						{	
							dGETFRM()->m_pWaterPalette->SetWaterAttr(*pMgr->m_pSectors[pMgr->GetSectorIdx(pWorldView->GetSelectBlockMidPos())].m_pWater);
						}
					}
				}
			}
			else
			{
				for(unsigned int p = 0; p < m_vecSelectedWaterObj.size(); ++p)
				{
					GetSceneManager()->DeleteEntity(m_vecSelectedWaterObj[p]);
				}
				m_vecSelectedWaterObj.clear();

				if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
				{
					CWorldViewDlg* pWorldView = ((CNtlWEFrm*)AfxGetMainWnd())->GetWorldView();

					m_bboxSelectWater.sup.x = 0.0f; m_bboxSelectWater.sup.y = 0.0f; m_bboxSelectWater.sup.z = 0.0f;
					m_bboxSelectWater.inf.x = 0.0f; m_bboxSelectWater.inf.y = 0.0f; m_bboxSelectWater.inf.z = 0.0f;

					pWorldView->ResetSelectBlock();
				}
			}
		}
	}
}

void CWorldEditorEventHander::UpdateFieldUI()
{
	UpdateSkyFloating();

	m_pFieldUIManager->Update();
	m_pMergeUIManager->Update(m_pFieldUIManager);
}

void CWorldEditorEventHander::UpdateSkyFloating()
{
	m_PlanetIOHandler.Update(g_GetElapsedTime());
}

RpAtomic* WorldIntersection4EffectCB_CTLine2Sphere(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData)
{
	if((static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic))->GetClassType() != PLENTITY_EFFECT))
	{
		return pAtomic;
	}

	sNPE_COLLISION_PARAM*	pNPECollisionParam	= static_cast<sNPE_COLLISION_PARAM*>(pData);
	CNtlPLEntity*			pNtlPLEntityCur		= static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic));
	RwReal					DistSphere2LineSPos;

	RwSphere*	pSphere = RpAtomicGetBoundingSphere(pAtomic);
	RwV3d		SphereC;
	RwV3d		Src2Dst;

	// dummy scaling
	RwV3dTransformPoints(&SphereC, &pSphere->center, 1, RwFrameGetLTM(RpAtomicGetFrame(pAtomic)));

	for(unsigned int i = 0; i < pNPECollisionParam->_vecNPECollisionParam.size(); ++i)
	{
		if(pAtomic == pNPECollisionParam->_vecNPECollisionParam[i]._pAtomic)
		{
			RwV3dSubMacro(&Src2Dst, &SphereC, &pIntersection->t.sphere.center);
			DistSphere2LineSPos = ::sqrtf(CNtlMath::MathRwV3dSquaredLength(&Src2Dst));

			if(static_cast<RwReal>(::fabs(pNPECollisionParam->_vecNPECollisionParam[i]._Dist)) > DistSphere2LineSPos)
			{
				pNPECollisionParam->_vecNPECollisionParam[i]._Dist = fDistance * DistSphere2LineSPos;
			}

			return pAtomic;
		}
	}

	sCOLLISION_PARAM sCollisionParam;
	RwV3dSubMacro(&Src2Dst, &SphereC, &pIntersection->t.sphere.center);
	DistSphere2LineSPos = ::sqrtf(CNtlMath::MathRwV3dSquaredLength(&Src2Dst));
	sCollisionParam._Dist			= fDistance * DistSphere2LineSPos;
	sCollisionParam._pAtomic		= pAtomic;
	sCollisionParam._pNtlPLEntity	= static_cast<CNtlPLEntity*>(pNtlPLEntityCur);
	pNPECollisionParam->_vecNPECollisionParam.push_back(sCollisionParam);

	return pAtomic;
}

RpAtomic* WorldIntersection4SECB_CTLine2Sphere(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData)
{
	if((static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic))->GetClassType() != PLENTITY_SOUND) &&
		(static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic))->GetClassType() != PLENTITY_SOUND_BGM))
	{
		return pAtomic;
	}

	sNPE_COLLISION_PARAM*	pNPECollisionParam	= static_cast<sNPE_COLLISION_PARAM*>(pData);
	CNtlPLEntity*			pNtlPLEntityCur		= static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic));
	RwReal					DistSphere2LineSPos;

	RwSphere*	pSphere = RpAtomicGetBoundingSphere(pAtomic);
	RwV3d		SphereC;
	RwV3d		Src2Dst;
	RwV3dTransformPoints(&SphereC, &pSphere->center, 1, RwFrameGetLTM(RpAtomicGetFrame(pAtomic)));

	for(unsigned int i = 0; i < pNPECollisionParam->_vecNPECollisionParam.size(); ++i)
	{
		if(pAtomic == pNPECollisionParam->_vecNPECollisionParam[i]._pAtomic)
		{
			RwV3dSubMacro(&Src2Dst, &SphereC, &pIntersection->t.sphere.center);
			DistSphere2LineSPos = ::sqrtf(CNtlMath::MathRwV3dSquaredLength(&Src2Dst));

			if(static_cast<RwReal>(::fabs(pNPECollisionParam->_vecNPECollisionParam[i]._Dist)) > DistSphere2LineSPos)
			{
				pNPECollisionParam->_vecNPECollisionParam[i]._Dist = fDistance * DistSphere2LineSPos;
			}

			return pAtomic;
		}
	}

	sCOLLISION_PARAM sCollisionParam;
	RwV3dSubMacro(&Src2Dst, &SphereC, &pIntersection->t.sphere.center);
	DistSphere2LineSPos = ::sqrtf(CNtlMath::MathRwV3dSquaredLength(&Src2Dst));
	sCollisionParam._Dist			= fDistance * DistSphere2LineSPos;
	sCollisionParam._pAtomic		= pAtomic;
	sCollisionParam._pNtlPLEntity	= static_cast<CNtlPLEntity*>(pNtlPLEntityCur);
	pNPECollisionParam->_vecNPECollisionParam.push_back(sCollisionParam);

	return pAtomic;
}

RpAtomic* WorldIntersection4WLCB_CTLine2Sphere(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData)
{
	if((static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic))->GetClassType() != PLENTITY_WORLD_LIGHT))
	{
		return pAtomic;
	}

	sNPE_COLLISION_PARAM*	pNPECollisionParam	= static_cast<sNPE_COLLISION_PARAM*>(pData);
	CNtlPLEntity*			pNtlPLEntityCur		= static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic));
	RwReal					DistSphere2LineSPos;

	RwSphere*	pSphere = RpAtomicGetBoundingSphere(pAtomic);
	RwV3d		SphereC;
	RwV3d		Src2Dst;
	RwV3dTransformPoints(&SphereC, &pSphere->center, 1, RwFrameGetLTM(RpAtomicGetFrame(pAtomic)));

	for(unsigned int i = 0; i < pNPECollisionParam->_vecNPECollisionParam.size(); ++i)
	{
		if(pAtomic == pNPECollisionParam->_vecNPECollisionParam[i]._pAtomic)
		{
			RwV3dSubMacro(&Src2Dst, &SphereC, &pIntersection->t.sphere.center);
			DistSphere2LineSPos = ::sqrtf(CNtlMath::MathRwV3dSquaredLength(&Src2Dst));

			if(static_cast<RwReal>(::fabs(pNPECollisionParam->_vecNPECollisionParam[i]._Dist)) > DistSphere2LineSPos)
			{
				pNPECollisionParam->_vecNPECollisionParam[i]._Dist = fDistance * DistSphere2LineSPos;
			}

			return pAtomic;
		}
	}

	sCOLLISION_PARAM sCollisionParam;
	RwV3dSubMacro(&Src2Dst, &SphereC, &pIntersection->t.sphere.center);
	DistSphere2LineSPos = ::sqrtf(CNtlMath::MathRwV3dSquaredLength(&Src2Dst));
	sCollisionParam._Dist			= fDistance * DistSphere2LineSPos;
	sCollisionParam._pAtomic		= pAtomic;
	sCollisionParam._pNtlPLEntity	= static_cast<CNtlPLEntity*>(pNtlPLEntityCur);
	pNPECollisionParam->_vecNPECollisionParam.push_back(sCollisionParam);

	return pAtomic;
}

void CWorldEditorEventHander::UpdateSE()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	// placing mode
	if(dGET_BRUSH_ENTITY())
	{
		m_vecSelectedSE.clear();

		// wanna cancel the effect attached to current mouse brush and use effects editing mode
		if(dOKEY(DIK_SPACE))
		{
			if(dGET_BRUSH_ENTITY())
			{
				GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
				dGET_BRUSH_ENTITY() = NULL;
				return;
			}
		}

		// object moving with picking variation
		sNPE_COLLISION_PARAM sNPECollisionParam;
		sNPECollisionParam._pNtlPLEntity = dGET_BRUSH_ENTITY();

		RwV3d vDist;
		vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
		vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
		vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;
		dGET_BRUSH_ENTITY()->SetPosition(&vDist);

		// place effects
		if(dGET_MOUSE()->GetLBOnce())
		{
			if(dGET_BRUSH_ENTITY())
			{
				RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vDist);
				if(CurSectorIdx != -1)
				{
					SPLEntityCreateParam PLEntityCreateParam;
					PLEntityCreateParam.pPos = &vDist;

					RwChar FileName[32];
					RwBool Ret = dGETFRM()->m_pSEPalette->GetCurBGMFileName(FileName);
					if(!Ret)
					{
						AfxMessageBox("Tree item should be selected", MB_OK);
						return;
					}

					RwChar CategoryName[32];
					dGETFRM()->m_pSEPalette->GetCurCategoryName(CategoryName);

					CNtlPLEntity *pNtlPLEntity = NULL;
					if (strcmp(CategoryName, "bgm"))
					{
						pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_SOUND, dGET_BRUSH_ENTITY()->GetName(), &PLEntityCreateParam);
						DBO_ASSERT(pNtlPLEntity, "Entity create failed.");
					}
					else
					{
						pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_SOUND_BGM, dGET_BRUSH_ENTITY()->GetName(), &PLEntityCreateParam);
						DBO_ASSERT(pNtlPLEntity, "Entity create failed.");
					}

					if (pNtlPLEntity && SOUNDRESULT_OK == static_cast<CNtlPLSound*>(pNtlPLEntity)->PlaySound(FileName))
					{
						pMgr->m_pSectors[CurSectorIdx].InsertIn2World(pNtlPLEntity, 2);
					}
					else
					{
						GetSceneManager()->DeleteEntity(pNtlPLEntity);
						AfxMessageBox("sound failed. confirm resource directory.");
					}
				}
			}
		}
	}
	// capture mode; picking
	else if(!dGET_BRUSH_ENTITY())
	{
		if(dGET_MOUSE()->GetLBOnce() && !dKEY(DIK_LSHIFT) && !dKEY(DIK_RSHIFT) && !dKEY(DIK_SPACE))
		{
			RwV2d ScreenPos;
			ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
			ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

			RwLine cameraRay;
			RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

			// pick map objects, collision
			sNPE_COLLISION_PARAM NPECollisionParam;

			RpIntersection Intersection;
			Intersection.type = rpINTERSECTLINE;
			::CopyMemory(&Intersection.t.line, &cameraRay, sizeof(RwLine));

			RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &Intersection, WorldIntersection4SECB_CTLine2Sphere, &NPECollisionParam);

			RwInt32 Num = NPECollisionParam._vecNPECollisionParam.size();
			if(Num)
			{
				RwInt32 Idx		= 0;
				RwReal	Dist	= NPECollisionParam._vecNPECollisionParam[0]._Dist;

				for(int i = 1; i < Num; ++i)
				{
					if(NPECollisionParam._vecNPECollisionParam[i]._Dist < Dist)
					{
						Idx = i;
					}
				}

				m_vecSelectedSE.clear();
				m_vecSelectedSE.push_back(NPECollisionParam._vecNPECollisionParam[Idx]._pNtlPLEntity);

				if(!m_pBrushSelection)
				{
					m_pBrushSelection						= NTL_NEW CNtlWorldBrush;
					CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
					m_pBrushSelection->SetController(pController);
				}

				if(!m_pBrushSelectionSub)
				{
					m_pBrushSelectionSub					= NTL_NEW CNtlWorldBrush;
					CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
					m_pBrushSelectionSub->SetController(pController);
				}
			}
			else
			{
				m_vecSelectedSE.clear();
			}
		}

		// editing mode
		if(m_vecSelectedSE.size())
		{
			if(dOKEY(DIK_DELETE))
			{
				RwBool BreakFlag = FALSE;

				for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
				{
					RwInt32 NumObj = 0;
					NumObj = static_cast<RwInt32>(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntitySound.size());
					for(int k = 0; k < NumObj; ++k)
					{
						if(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntitySound[k] == m_vecSelectedSE[0])
						{
							pMgr->m_pSectors[g_vecVisibleSectors[j]].DeleteEntity(m_vecSelectedSE[0], 2);
							GetSceneManager()->DeleteEntity(m_vecSelectedSE[0]);
							m_vecSelectedSE.clear();
							BreakFlag = TRUE;
							break;
						}
					}

					NumObj = static_cast<RwInt32>(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntitySoundBGM.size());
					for(int k = 0; k < NumObj; ++k)
					{
						if(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntitySoundBGM[k] == m_vecSelectedSE[0])
						{
							pMgr->m_pSectors[g_vecVisibleSectors[j]].DeleteEntity(m_vecSelectedSE[0], 2);
							GetSceneManager()->DeleteEntity(m_vecSelectedSE[0]);
							m_vecSelectedSE.clear();
							BreakFlag = TRUE;
							break;
						}
					}

					if(BreakFlag)
					{
						break;
					}
				}
			}

			// transform
			if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
			{
				// translate
				if(dKEY(DIK_LSHIFT))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

					if(dKEY(DIK_Z))
					{
						RwV3d PosCur = m_vecSelectedSE[0]->GetPosition();
						PosCur.y -= DistY;
						m_vecSelectedSE[0]->SetPosition(&PosCur);
					}
					else
					{
						if(m_vecSelectedSE.size())
						{
							RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
							RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
							RwV3d *pRight			= RwMatrixGetRight(pM);
							RwV3d *pUp				= RwMatrixGetUp(pM);
							RwV3d *pAt				= RwMatrixGetAt(pM);
							RwV3d vFront, vRight;
							RwReal TerrainHeight, TotalHeight, ObjHeight;

							CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
							CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
							RwV3dNormalize(&vFront, &vFront);
							RwV3dNormalize(&vRight, &vRight);

							RwV3d PosCur = m_vecSelectedSE[0]->GetPosition();

							// delete from the previous field
							RwInt32 CurSectorIdx = pMgr->GetSectorIdx(PosCur);
							if(CurSectorIdx != -1)
							{
								pMgr->m_pSectors[CurSectorIdx].DeleteEntity(m_vecSelectedSE[0], 2);
							}

							TotalHeight		= dGET_POSITIVE_VALUE(PosCur.y);
							TerrainHeight	= dGET_POSITIVE_VALUE(pMgr->GetWorldSectorHeight(PosCur));
							ObjHeight		= TotalHeight - TerrainHeight;

							// transform
							RwV3dIncrementScaled(&PosCur, &vFront, -DistY);
							RwV3dIncrementScaled(&PosCur, &vRight, -DistX);

							PosCur.y = pMgr->GetWorldSectorHeight(PosCur) + ObjHeight;
							m_vecSelectedSE[0]->SetPosition(&PosCur);

							// create into the new field
							CurSectorIdx = pMgr->GetSectorIdx(PosCur);
							if(CurSectorIdx != -1)
							{
								pMgr->m_pSectors[CurSectorIdx].InsertIn2World(m_vecSelectedSE[0], 2);
							}
						}
					}
				}
			}

			// Volume
			if(dOKEY(DIK_LBRACKET))
			{
				static_cast<CNtlPLSound*>(m_vecSelectedSE[0])->ControlVolume(-0.1f);
			}
			else if(dOKEY(DIK_RBRACKET))
			{
				static_cast<CNtlPLSound*>(m_vecSelectedSE[0])->ControlVolume(0.1f);
			}

			// radius
			if(dGET_MOUSE()->GetWheelMovement() && m_vecSelectedSE.size())
			{
				int Value = 0;

				if(dGET_MOUSE()->GetWheelMovement())
				{
					Value = static_cast<int>(dGET_MOUSE()->GetWheelMovement() / ::fabsf(dGET_MOUSE()->GetWheelMovement()));

					if(dKEY(DIK_LCONTROL))
					{
						Value *= 10;
					}
				}

				if(dKEY(DIK_SPACE))
				{
					static_cast<CNtlPLSound*>(m_vecSelectedSE[0])->ControlRadiusMin(static_cast<RwReal>(Value));
				}
				else
				{
					static_cast<CNtlPLSound*>(m_vecSelectedSE[0])->ControlRadiusMax(static_cast<RwReal>(Value));
				}
			}
		}
	}
}

void CWorldEditorEventHander::UpdateGameProperty()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	// placing mode
	if(dGET_BRUSH_ENTITY())
	{
		m_vecSelectedWorldLight.clear();

		// wanna cancel the effect attached to current mouse brush and use effects editing mode
		if(dOKEY(DIK_SPACE))
		{
			if(dGET_BRUSH_ENTITY())
			{
				GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
				dGET_BRUSH_ENTITY() = NULL;
				dGETFRM()->m_pGamePropertyPalette->UpdatePropertyData(NULL);
				return;
			}
		}

		// object moving with picking variation
		sNPE_COLLISION_PARAM sNPECollisionParam;
		sNPECollisionParam._pNtlPLEntity = dGET_BRUSH_ENTITY();

		RwV3d vDist;
		vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
		vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
		vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;
		dGET_BRUSH_ENTITY()->SetPosition(&vDist);

		// place effects
		if(dGET_MOUSE()->GetLBOnce())
		{
			if(dGET_BRUSH_ENTITY())
			{
				RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vDist);
				if(CurSectorIdx != -1)
				{
					dGETFRM()->m_pGamePropertyPalette->UpdatePropertyData(dGET_BRUSH_ENTITY());		
					if (dGETFRM()->m_pGamePropertyPalette->GetCreateParam())
					{
						CNtlPLEntity *pNtlPLEntity	= GetSceneManager()->CreateEntity(PLENTITY_GAME_PROPERTY, NULL, dGETFRM()->m_pGamePropertyPalette->GetCreateParam());
						DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

						pMgr->m_pSectors[CurSectorIdx].InsertIn2World(pNtlPLEntity, 9);						 
						return;
					}					
				}
			}
		}
	}
	// capture mode; picking
	else if(!dGET_BRUSH_ENTITY())
	{
		if(dGET_MOUSE()->GetLBOnce() && !dKEY(DIK_LSHIFT) && !dKEY(DIK_RSHIFT) && !dKEY(DIK_SPACE))
		{
			RwInt32					CurFieldIdx;
			RwInt32					CurSectorIdx;
			CNtlWorldFieldManager*	pMgr	= dGETMGR();
			CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
			RwV3d					SectorPos;
			RwReal					ResultDist = RwRealMAXVAL;	
			RwReal					CurDist;
			CNtlPLGameProperty*		pPickedEntity = NULL;
			RwV3d					SPos;

			for(RwUInt32 i = 0; i < 36; ++i)
			{
				CurFieldIdx = pMgr->m_Fields6x6[1][i];
				SPos		= pFields[CurFieldIdx].GetSPos();

				for(int l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
				{
					for(int m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
					{
						SectorPos.x = (RwReal)m;
						SectorPos.z = (RwReal)l;
						CurSectorIdx = pMgr->CNtlWorldFieldManager::GetSectorIdx(SectorPos);

						if(CurSectorIdx == -1)
							continue;

						CNtlWorldSector* pCurSector = &pMgr->CNtlWorldSectorManager::m_pSectors[CurSectorIdx];

						for(RwUInt32 z = 0; z < pCurSector->m_vecNtlPLEntityGameProperty.size(); ++z)
						{
							CNtlPLGameProperty* pEntity = static_cast<CNtlPLGameProperty*>(pCurSector->m_vecNtlPLEntityGameProperty[z]);
							CurDist = RwRealMAXVAL;
							if(pEntity->Pick(CurDist))
							{
								if(CurDist < ResultDist)
								{
									ResultDist		= CurDist;
									pPickedEntity	= pEntity;
								}
							}
						}
					}
				}
			}

			// if picked,
			if(pPickedEntity)
			{
				CNtlPLGameProperty::SetSelectGameProperty(pPickedEntity);
				dGETFRM()->m_pGamePropertyPalette->UpdatePropertyData(pPickedEntity);
			}
			else
			{
				CNtlPLGameProperty::SetSelectGameProperty(NULL);
				dGETFRM()->m_pGamePropertyPalette->UpdatePropertyData(NULL);				
			}
		}

		// editing mode
		if(CNtlPLGameProperty::GetSelectGameProperty())
		{			
			if(dOKEY(DIK_DELETE))
			{
				RwBool BreakFlag = FALSE;

				for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
				{
					RwInt32 NumObj = static_cast<RwInt32>(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityGameProperty.size());
					for(int k = 0; k < NumObj; ++k)
					{
						if(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityGameProperty[k] == CNtlPLGameProperty::GetSelectGameProperty())
						{
							pMgr->m_pSectors[g_vecVisibleSectors[j]].DeleteEntity(CNtlPLGameProperty::GetSelectGameProperty(), 9);
							GetSceneManager()->DeleteEntity(CNtlPLGameProperty::GetSelectGameProperty());
							CNtlPLGameProperty::SetSelectGameProperty(NULL);
							dGETFRM()->m_pGamePropertyPalette->UpdatePropertyData(NULL);
							return;
						}
					}					
				}
			}

			if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

					if(dKEY(DIK_Z))
					{
						RwV3d vPos = CNtlPLGameProperty::GetSelectGameProperty()->GetPosition();
						vPos.y -= DistY;
						CNtlPLGameProperty::GetSelectGameProperty()->SetPosition(&vPos);
						dGETFRM()->m_pGamePropertyPalette->UpdatePropertyData(CNtlPLGameProperty::GetSelectGameProperty());
					}
					else
					{
						RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
						RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
						RwV3d *pRight			= RwMatrixGetRight(pM);
						RwV3d *pUp				= RwMatrixGetUp(pM);
						RwV3d *pAt				= RwMatrixGetAt(pM);
						RwV3d vFront, vRight;
						RwReal TerrainHeight, TotalHeight, ObjHeight;

						CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
						CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
						RwV3dNormalize(&vFront, &vFront);
						RwV3dNormalize(&vRight, &vRight);

						RwV3d vPos = CNtlPLGameProperty::GetSelectGameProperty()->GetPosition();
						// delete from the previous field
						RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
						if(CurSectorIdx != -1)
						{
							pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(CNtlPLGameProperty::GetSelectGameProperty()), 9);
						}

						TotalHeight		= dGET_POSITIVE_VALUE(vPos.y);
						TerrainHeight	= dGET_POSITIVE_VALUE(pMgr->GetWorldSectorHeight(vPos));
						ObjHeight		= TotalHeight - TerrainHeight;

						// transform
						RwV3dIncrementScaled(&vPos, &vFront, -DistY);
						RwV3dIncrementScaled(&vPos, &vRight, -DistX);

						vPos.y = pMgr->GetWorldSectorHeight(vPos) + ObjHeight;
						CNtlPLGameProperty::GetSelectGameProperty()->SetPosition(&vPos);
						dGETFRM()->m_pGamePropertyPalette->UpdatePropertyData(CNtlPLGameProperty::GetSelectGameProperty());

						// create into the new field
						CurSectorIdx = pMgr->GetSectorIdx(vPos);
						if(CurSectorIdx != -1)
						{
							pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(CNtlPLGameProperty::GetSelectGameProperty()), 9);
						}
					}
				}
				else if(dKEY(DIK_RSHIFT))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

					RwV3d vSize = CNtlPLGameProperty::GetSelectGameProperty()->GetSize();
					vSize.x -= DistY;
					vSize.y -= DistY;
					vSize.z -= DistY;
					CLAMP(vSize.x, 0.5f, dGET_WORLD_PARAM()->WorldBlockSize / 2);
					CLAMP(vSize.y, 0.0f, dGET_WORLD_PARAM()->WorldBlockSize / 2);
					CLAMP(vSize.z, 0.5f, dGET_WORLD_PARAM()->WorldBlockSize / 2);

					CNtlPLGameProperty::GetSelectGameProperty()->SetSize(&vSize);
					dGETFRM()->m_pGamePropertyPalette->UpdatePropertyData(CNtlPLGameProperty::GetSelectGameProperty());
				}
			}

			// translation keyboard
			if(dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					if(dOKEY(DIK_UPARROW))
					{
						RwV3d vPos = CNtlPLGameProperty::GetSelectGameProperty()->GetPosition();
						vPos.y += m_fObjMovementSensitivity;
						CNtlPLGameProperty::GetSelectGameProperty()->SetPosition(&vPos);
						dGETFRM()->m_pGamePropertyPalette->UpdatePropertyData(CNtlPLGameProperty::GetSelectGameProperty());
					}
					else if(dOKEY(DIK_DOWNARROW))
					{
						RwV3d vPos = CNtlPLGameProperty::GetSelectGameProperty()->GetPosition();
						vPos.y -= m_fObjMovementSensitivity;
						CNtlPLGameProperty::GetSelectGameProperty()->SetPosition(&vPos);
						dGETFRM()->m_pGamePropertyPalette->UpdatePropertyData(CNtlPLGameProperty::GetSelectGameProperty());
					}
				}
				else
				{
					if(dOKEY(DIK_UPARROW))
					{
						RwV3d vPos = CNtlPLGameProperty::GetSelectGameProperty()->GetPosition();
						vPos.y += m_fObjMovementSensitivity;
						CNtlPLGameProperty::GetSelectGameProperty()->SetPosition(&vPos);
						dGETFRM()->m_pGamePropertyPalette->UpdatePropertyData(CNtlPLGameProperty::GetSelectGameProperty());
					}
					else if(dOKEY(DIK_DOWNARROW))
					{
						RwV3d vPos = CNtlPLGameProperty::GetSelectGameProperty()->GetPosition();
						vPos.y -= m_fObjMovementSensitivity;
						CNtlPLGameProperty::GetSelectGameProperty()->SetPosition(&vPos);
						dGETFRM()->m_pGamePropertyPalette->UpdatePropertyData(CNtlPLGameProperty::GetSelectGameProperty());
					}
				}
			}

			if(!dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LEFTARROW) || dKEY(DIK_RIGHTARROW) || dKEY(DIK_UPARROW) || dKEY(DIK_DOWNARROW))
				{
					RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
					RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
					RwV3d *pRight			= RwMatrixGetRight(pM);
					RwV3d *pUp				= RwMatrixGetUp(pM);
					RwV3d *pAt				= RwMatrixGetAt(pM);
					RwV3d vFront, vRight;

					CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
					CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
					RwV3dNormalize(&vFront, &vFront);
					RwV3dNormalize(&vRight, &vRight);

					RwV3d vPos = CNtlPLGameProperty::GetSelectGameProperty()->GetPosition();

					// delete from the previous field
					RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
					if(CurSectorIdx != -1)
					{
						pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(CNtlPLGameProperty::GetSelectGameProperty()), 9);
					}

					if(!dKEY(DIK_LSHIFT))
					{
						if(dKEY(DIK_LEFTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, m_fObjMovementSensitivity);
						}
						else if(dKEY(DIK_RIGHTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, -m_fObjMovementSensitivity);
						}
						else if(dKEY(DIK_UPARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, m_fObjMovementSensitivity);
						}
						else if(dKEY(DIK_DOWNARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, -m_fObjMovementSensitivity);
						}
					}
					else
					{
						if(dOKEY(DIK_LEFTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, m_fObjMovementSensitivity);
						}
						else if(dOKEY(DIK_RIGHTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, -m_fObjMovementSensitivity);
						}
						else if(dOKEY(DIK_UPARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, m_fObjMovementSensitivity);
						}
						else if(dOKEY(DIK_DOWNARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, -m_fObjMovementSensitivity);
						}
					}

					CNtlPLGameProperty::GetSelectGameProperty()->SetPosition(&vPos);
					dGETFRM()->m_pGamePropertyPalette->UpdatePropertyData(CNtlPLGameProperty::GetSelectGameProperty());

					// create into the new field
					CurSectorIdx = pMgr->GetSectorIdx(vPos);
					if(CurSectorIdx != -1)
					{
						pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(CNtlPLGameProperty::GetSelectGameProperty()), 9);
					}
				}
			}
		}
	}
}

void CWorldEditorEventHander::UpdateWorldLight()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	// placing mode
	if(dGET_BRUSH_ENTITY())
	{
		m_vecSelectedWorldLight.clear();

		// wanna cancel the effect attached to current mouse brush and use effects editing mode
		if(dOKEY(DIK_SPACE))
		{
			if(dGET_BRUSH_ENTITY())
			{
				GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
				dGET_BRUSH_ENTITY() = NULL;
				dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(NULL);
				return;
			}
		}

		// object moving with picking variation
		sNPE_COLLISION_PARAM sNPECollisionParam;
		sNPECollisionParam._pNtlPLEntity = dGET_BRUSH_ENTITY();

		RwV3d vDist;
		vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
		vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
		vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;
		dGET_BRUSH_ENTITY()->SetPosition(&vDist);

		// place effects
		if(dGET_MOUSE()->GetLBOnce())
		{
			if(dGET_BRUSH_ENTITY())
			{
				RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vDist);
				if(CurSectorIdx != -1)
				{
					dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(dGET_BRUSH_ENTITY());		
					if (dGETFRM()->m_pWorldLightPalette->GetCreateParam())
					{
						CNtlPLEntity *pNtlPLEntity	= GetSceneManager()->CreateEntity(PLENTITY_WORLD_LIGHT, NULL, dGETFRM()->m_pWorldLightPalette->GetCreateParam());
						DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

						pMgr->m_pSectors[CurSectorIdx].InsertIn2World(pNtlPLEntity, 5);						 
						return;
					}					
				}
			}
		}
	}
	// capture mode; picking
	else if(!dGET_BRUSH_ENTITY())
	{
		if(dGET_MOUSE()->GetLBOnce() && !dKEY(DIK_LSHIFT) && !dKEY(DIK_RSHIFT) && !dKEY(DIK_SPACE))
		{
			RwV2d ScreenPos;
			ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
			ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

			RwLine cameraRay;
			RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

			// pick map objects, collision
			sNPE_COLLISION_PARAM NPECollisionParam;

			RpIntersection Intersection;
			Intersection.type = rpINTERSECTLINE;
			::CopyMemory(&Intersection.t.line, &cameraRay, sizeof(RwLine));

			RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &Intersection, WorldIntersection4WLCB_CTLine2Sphere, &NPECollisionParam);

			RwInt32 Num = NPECollisionParam._vecNPECollisionParam.size();
			if(Num)
			{
				RwInt32 Idx		= 0;
				RwReal	Dist	= NPECollisionParam._vecNPECollisionParam[0]._Dist;

				for(int i = 1; i < Num; ++i)
				{
					if(NPECollisionParam._vecNPECollisionParam[i]._Dist < Dist)
					{
						Idx = i;
					}
				}
				m_vecSelectedWorldLight.clear();
				m_vecSelectedWorldLight.push_back(NPECollisionParam._vecNPECollisionParam[Idx]._pNtlPLEntity);
				dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(NPECollisionParam._vecNPECollisionParam[Idx]._pNtlPLEntity);

				if(!m_pBrushSelection)
				{
					m_pBrushSelection						= NTL_NEW CNtlWorldBrush;
					CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
					m_pBrushSelection->SetController(pController);
				}
			}
			else
			{
				m_vecSelectedWorldLight.clear();
				dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(NULL);
			}
		}

		// editing mode
		if(m_vecSelectedWorldLight.size())
		{
			CNtlPLWorldLight_Base*		pWorldLight			= (CNtlPLWorldLight_Base*)m_vecSelectedWorldLight[0];

			CNtlPLWorldLight_Sphere*	pWorldLightSphere	= NULL;
			CNtlPLWorldLight_Box*		pWorldLightBox		= NULL;

			switch (pWorldLight->GetWorldLightType())
			{
			case EPLWORLDLIGHT_SPHERE:
				pWorldLightSphere = (CNtlPLWorldLight_Sphere*)pWorldLight;
				break;
			case EPLWORLDLIGHT_BOX:
				pWorldLightBox = (CNtlPLWorldLight_Box*)pWorldLight;
				break;
			}

			if(dOKEY(DIK_DELETE))
			{
				RwBool BreakFlag = FALSE;

				for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
				{
					if (pWorldLightSphere)
					{
						RwInt32 NumObj = static_cast<RwInt32>(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityWorldLight_Sphere.size());
						for(int k = 0; k < NumObj; ++k)
						{
							if(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityWorldLight_Sphere[k] == pWorldLightSphere)
							{
								pMgr->m_pSectors[g_vecVisibleSectors[j]].DeleteEntity(pWorldLightSphere, 5);
								GetSceneManager()->DeleteEntity(pWorldLightSphere);
								m_vecSelectedWorldLight.clear();
								dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(NULL);
								return;
							}
						}	
					}
					else if (pWorldLightBox)
					{
						RwInt32 NumObj = static_cast<RwInt32>(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityWorldLight_Box.size());
						for(int k = 0; k < NumObj; ++k)
						{
							if(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityWorldLight_Box[k] == pWorldLightBox)
							{
								pMgr->m_pSectors[g_vecVisibleSectors[j]].DeleteEntity(pWorldLightBox, 5);
								GetSceneManager()->DeleteEntity(pWorldLightBox);
								m_vecSelectedWorldLight.clear();
								dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(NULL);
								return;
							}
						}
					}
				}
			}

			if(dKEY(DIK_LCONTROL) && dGET_MOUSE()->GetLBOnce() && m_vecSelectedWorldLight.size() == 1)
			{
				if(dGET_BRUSH_ENTITY())
				{
					GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
					dGET_BRUSH_ENTITY() = NULL;
				}

				if (dGETFRM()->m_pWorldLightPalette->GetCreateParam(m_vecSelectedWorldLight.at(0)))
				{
					dGET_BRUSH_ENTITY() = GetSceneManager()->CreateEntity(PLENTITY_WORLD_LIGHT, NULL, dGETFRM()->m_pWorldLightPalette->GetCreateParam(m_vecSelectedWorldLight.at(0)));
					DBO_ASSERT(dGET_BRUSH_ENTITY(), "Entity create failed.");

					dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(dGET_BRUSH_ENTITY());

					m_vecSelectedWorldLight.clear();
					return;
				}					
			}

			if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

					if(dKEY(DIK_Z))
					{
						if (pWorldLight)
						{
							RwV3d vPos = pWorldLight->GetPosition();
							vPos.y -= DistY;
							pWorldLight->SetPosition(&vPos);
							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);
						}
					}
					else
					{
						if (pWorldLight)
						{
							RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
							RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
							RwV3d *pRight			= RwMatrixGetRight(pM);
							RwV3d *pUp				= RwMatrixGetUp(pM);
							RwV3d *pAt				= RwMatrixGetAt(pM);
							RwV3d vFront, vRight;
							RwReal TerrainHeight, TotalHeight, ObjHeight;

							CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
							CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
							RwV3dNormalize(&vFront, &vFront);
							RwV3dNormalize(&vRight, &vRight);

							RwV3d vPos = pWorldLight->GetPosition();
							// delete from the previous field
							RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
							if(CurSectorIdx != -1)
							{
								pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pWorldLight), 5);
							}

							TotalHeight		= dGET_POSITIVE_VALUE(vPos.y);
							TerrainHeight	= dGET_POSITIVE_VALUE(pMgr->GetWorldSectorHeight(vPos));
							ObjHeight		= TotalHeight - TerrainHeight;

							// transform
							RwV3dIncrementScaled(&vPos, &vFront, -DistY);
							RwV3dIncrementScaled(&vPos, &vRight, -DistX);

							vPos.y = pMgr->GetWorldSectorHeight(vPos) + ObjHeight;
							pWorldLight->SetPosition(&vPos);
							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);

							// create into the new field
							CurSectorIdx = pMgr->GetSectorIdx(vPos);
							if(CurSectorIdx != -1)
							{
								pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pWorldLight), 5);
							}
						}
					}
				}
				// Rot
				else if(dKEY(DIK_SPACE))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(1.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(1.0f);

					if(dKEY(DIK_V))
					{
						if (pWorldLightBox)
						{
							RwV3d vRot = *pWorldLightBox->GetRotate();
							vRot.x -= DistX;
							pWorldLightBox->SetRotate(&vRot);
							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);
						}
					}
					else if(dKEY(DIK_N))
					{
						if (pWorldLightBox)
						{
							RwV3d vRot = *pWorldLightBox->GetRotate();
							vRot.z -= DistX;
							pWorldLightBox->SetRotate(&vRot);
							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);
						}
					}
					else
					{
						if (pWorldLightBox)
						{
							RwV3d vRot = *pWorldLightBox->GetRotate();
							vRot.y -= DistX;
							pWorldLightBox->SetRotate(&vRot);
							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);
						}
					}
				}
				// Scale
				else if(dKEY(DIK_RSHIFT))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

					if (pWorldLightSphere)
					{
						RwReal fRadius = pWorldLightSphere->GetRadius();
						fRadius -= DistY;
						CLAMP(fRadius, 0.5f, dGET_WORLD_PARAM()->WorldSectorSize / 2);
						pWorldLightSphere->SetRadius(fRadius);
						dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);

					}
					else if (pWorldLightBox)
					{
						RwV3d vScale = *pWorldLightBox->GetScaleVector();
						vScale.x -= DistY;
						vScale.y -= DistY;
						vScale.z -= DistY;
						CLAMP(vScale.x, 1.0f, dGET_WORLD_PARAM()->WorldSectorSize);
						CLAMP(vScale.y, 1.0f, dGET_WORLD_PARAM()->WorldSectorSize);
						CLAMP(vScale.z, 1.0f, dGET_WORLD_PARAM()->WorldSectorSize);
						pWorldLightBox->SetScaleVector(&vScale);
						dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);
					}
				}
			}

			// Rotation using Keyboard
			if(dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					if(dOKEY(DIK_LEFTARROW))
					{
						if (pWorldLightBox)
						{
							RwV3d vRot = *pWorldLightBox->GetRotate();
							vRot.y -= m_RotationTabValue;
							pWorldLightBox->SetRotate(&vRot);
							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);
						}
					}
					else if(dOKEY(DIK_RIGHTARROW))
					{
						if (pWorldLightBox)
						{
							RwV3d vRot = *pWorldLightBox->GetRotate();
							vRot.y += m_RotationTabValue;
							pWorldLightBox->SetRotate(&vRot);
							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);
						}
					}
				}
				else
				{
					if(dKEY(DIK_LEFTARROW))
					{
						if (pWorldLightBox)
						{
							RwV3d vRot = *pWorldLightBox->GetRotate();
							vRot.y -= m_RotationTabValue;
							pWorldLightBox->SetRotate(&vRot);
							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);
						}
					}
					else if(dKEY(DIK_RIGHTARROW))
					{
						if (pWorldLightBox)
						{
							RwV3d vRot = *pWorldLightBox->GetRotate();
							vRot.y += m_RotationTabValue;
							pWorldLightBox->SetRotate(&vRot);
							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);
						}
					}
				}
			}

			// translation keyboard
			if(dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					if(dOKEY(DIK_UPARROW))
					{
						if (pWorldLightBox)
						{
							RwV3d vPos = pWorldLightBox->GetPosition();
							vPos.y += m_fObjMovementSensitivity;
							pWorldLightBox->SetPosition(&vPos);
							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);
						}
					}
					else if(dOKEY(DIK_DOWNARROW))
					{
						if (pWorldLightBox)
						{
							RwV3d vPos = pWorldLightBox->GetPosition();
							vPos.y -= m_fObjMovementSensitivity;
							pWorldLightBox->SetPosition(&vPos);
							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);
						}
					}
				}
				else
				{
					if(dKEY(DIK_UPARROW))
					{
						if (pWorldLightBox)
						{
							RwV3d vPos = pWorldLightBox->GetPosition();
							vPos.y += m_fObjMovementSensitivity;
							pWorldLightBox->SetPosition(&vPos);
							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);
						}
					}
					else if(dKEY(DIK_DOWNARROW))
					{
						if (pWorldLightBox)
						{
							RwV3d vPos = pWorldLightBox->GetPosition();
							vPos.y -= m_fObjMovementSensitivity;
							pWorldLightBox->SetPosition(&vPos);
							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);
						}
					}
				}
			}

			if(!dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LEFTARROW) || dKEY(DIK_RIGHTARROW) || dKEY(DIK_UPARROW) || dKEY(DIK_DOWNARROW))
				{
					if (pWorldLight)
					{
						RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
						RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
						RwV3d *pRight			= RwMatrixGetRight(pM);
						RwV3d *pUp				= RwMatrixGetUp(pM);
						RwV3d *pAt				= RwMatrixGetAt(pM);
						RwV3d vFront, vRight;

						CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
						CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
						RwV3dNormalize(&vFront, &vFront);
						RwV3dNormalize(&vRight, &vRight);

						RwV3d vPos = pWorldLight->GetPosition();

						// delete from the previous field
						RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
						if(CurSectorIdx != -1)
						{
							pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pWorldLight), 5);
						}

						if(!dKEY(DIK_LSHIFT))
						{
							if(dKEY(DIK_LEFTARROW))
							{
								RwV3dIncrementScaled(&vPos, &vRight, m_fObjMovementSensitivity);
							}
							else if(dKEY(DIK_RIGHTARROW))
							{
								RwV3dIncrementScaled(&vPos, &vRight, -m_fObjMovementSensitivity);
							}
							else if(dKEY(DIK_UPARROW))
							{
								RwV3dIncrementScaled(&vPos, &vFront, m_fObjMovementSensitivity);
							}
							else if(dKEY(DIK_DOWNARROW))
							{
								RwV3dIncrementScaled(&vPos, &vFront, -m_fObjMovementSensitivity);
							}
						}
						else
						{
							if(dOKEY(DIK_LEFTARROW))
							{
								RwV3dIncrementScaled(&vPos, &vRight, m_fObjMovementSensitivity);
							}
							else if(dOKEY(DIK_RIGHTARROW))
							{
								RwV3dIncrementScaled(&vPos, &vRight, -m_fObjMovementSensitivity);
							}
							else if(dOKEY(DIK_UPARROW))
							{
								RwV3dIncrementScaled(&vPos, &vFront, m_fObjMovementSensitivity);
							}
							else if(dOKEY(DIK_DOWNARROW))
							{
								RwV3dIncrementScaled(&vPos, &vFront, -m_fObjMovementSensitivity);
							}
						}

						pWorldLight->SetPosition(&vPos);
						dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pWorldLight);

						// create into the new field
						CurSectorIdx = pMgr->GetSectorIdx(vPos);
						if(CurSectorIdx != -1)
						{
							pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pWorldLight), 5);
						}
					}
				}
			}
		}
	}
}

void CWorldEditorEventHander::UpdatePlant()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	// placing mode
	RwV3d vDist;
	vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
	vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
	vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;

	if(!m_pBrushSelection)
	{
		m_pBrushSelection						= NTL_NEW CNtlWorldBrush;
		CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
		m_pBrushSelection->SetController(pController);
	}

	if(dGET_MOUSE()->GetLBOnce())
	{
		if (dKEY(DIK_LALT))
		{
			// Delete
			dGETFRM()->m_pPlantPalette->OnWork(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, dGET_WORLD_PARAM()->NumBrushRadiusInTile, FALSE);
		}
		else
		{
			// Create
			dGETFRM()->m_pPlantPalette->OnWork(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, dGET_WORLD_PARAM()->NumBrushRadiusInTile, TRUE);
		}
	}	
	else if(dGET_MOUSE()->GetWheelMovement())
	{
		int iValue = static_cast<int>(dGET_MOUSE()->GetWheelMovement() / ::fabsf(dGET_MOUSE()->GetWheelMovement()));
		dGETFRM()->m_pPlantPalette->SetBrushSize(static_cast<RwReal>(dGET_WORLD_PARAM()->NumBrushRadiusInTile + iValue));
	}
}

void CWorldEditorEventHander::UpdateNMProp()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	if(dOKEY(DIK_SPACE))
	{
		if(m_pBrushSelector)
		{
			NTL_DELETE(m_pBrushSelector);
		}			
	}

	if(dGET_MOUSE()->GetLBOnce())
	{
		if(!m_pBrushSelector)
		{
			m_pBrushSelector = NTL_NEW CNtlWorldBrush;

			CNtlWorldBrushController *pController = new CNtlWorldBrushTile<CNtlWorldFieldManager>(eNBT_TILE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
			m_pBrushSelector->SetController(pController);

			UpdateNMPropBrushLimit();
		}

		CNtlWorldBrushTile<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTile<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
		pCurBrush->SetPosS(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
	}

	if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE) && m_pBrushSelector)
	{
		CNtlWorldBrushTile<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTile<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());

		if(dKEY(DIK_LSHIFT))
		{
			pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, TRUE);
		}
		else
		{
			pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
		}
	}
}

void CWorldEditorEventHander::UpdateNMPropBrushLimit()
{
	CNtlWorldFieldManager*	pMgr		= dGETMGR();
	RwV3d					Pos			= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwInt32					IdxCamField = pMgr->GetFieldIdx(Pos);

	if(IdxCamField == -1)
	{
		DBO_TRACE(FALSE, "CWorldEditorEventHander::UpdateNMPropBrushLimit, invalid field index.");
		return;
	}

	RwInt32			LenField	= dGET_WORLD_PARAM()->WorldFieldSize;
	RwInt32			LenTile		= dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32			LenHalf		= dGET_WORLD_PARAM()->WorldSizeHalf;
	CNtlWorldField*	pFields		= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwV3d			PosMidField = pFields[IdxCamField].GetSPos();

	RwInt32 SLmtX	= static_cast<RwInt32>(((PosMidField.x - LenField) + LenHalf) / LenTile);
	RwInt32 ELmtX	= static_cast<RwInt32>(((PosMidField.x + LenField + LenField) + LenHalf) / LenTile);
	RwInt32 SLmtZ	= static_cast<RwInt32>(((PosMidField.z - LenField) + LenHalf) / LenTile);
	RwInt32 ELmtZ	= static_cast<RwInt32>(((PosMidField.z + LenField + LenField) + LenHalf) / LenTile);

	CNtlWorldBrushTile<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTile<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
	pCurBrush->Update(SLmtX, ELmtX, SLmtZ, ELmtZ);
}


void CWorldEditorEventHander::UpdatePathEngineBrushLimit()
{
	CNtlWorldFieldManager*	pMgr		= dGETMGR();
	RwV3d					Pos			= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwInt32					IdxCamField = pMgr->GetFieldIdx(Pos);

	if(IdxCamField == -1)
	{
		DBO_TRACE(FALSE, "CWorldEditorEventHander::UpdatePathEngineBrushLimit, invalid field index.");
		return;
	}

	RwInt32			LenField	= dGET_WORLD_PARAM()->WorldFieldSize;
	RwInt32			LenTile		= CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
	RwInt32			LenHalf		= dGET_WORLD_PARAM()->WorldSizeHalf;
	CNtlWorldField*	pFields		= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwV3d			PosMidField = pFields[IdxCamField].GetSPos();

	RwInt32 SLmtX	= static_cast<RwInt32>(((PosMidField.x - LenField) + LenHalf) / LenTile);
	RwInt32 ELmtX	= static_cast<RwInt32>(((PosMidField.x + LenField + LenField) + LenHalf) / LenTile);
	RwInt32 SLmtZ	= static_cast<RwInt32>(((PosMidField.z - LenField) + LenHalf) / LenTile);
	RwInt32 ELmtZ	= static_cast<RwInt32>(((PosMidField.z + LenField + LenField) + LenHalf) / LenTile);

	CNtlWorldPathEngineManager::GetInstance()->RenderLmt(SLmtX, ELmtX, SLmtZ, ELmtZ);

	CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>*>(m_pBrushSelectorPE->GetController());
	pCurBrush->Update(SLmtX, ELmtX, SLmtZ, ELmtZ);
}


VOID CWorldEditorEventHander::SetTransparentTiles(RwBool _IsTileTransparency)
{
	RwInt32										CntX;
	RwInt32										CntZ;
	RwV3d										PosTile;
	CNtlWorldBrushTile<CNtlWorldFieldManager>*	pCurBrush			= reinterpret_cast<CNtlWorldBrushTile<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
	vector<RwInt32>								vecTileContainer	= pCurBrush->GetTileContainer();
	RwInt32										NumFieldTile		= dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
	RwInt32										NumWorldTile		= dGET_WORLD_PARAM()->WorldSize / dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32										LenTile				= dGET_WORLD_PARAM()->WorldSectorTileSize;
	CNtlWorldFieldManager*						pMgr				= dGETMGR();

	for(RwUInt32 i = 0; i < vecTileContainer.size(); ++i)
	{
		CntX		= vecTileContainer[i] - ((vecTileContainer[i] / NumWorldTile) * NumWorldTile);
		CntZ		= vecTileContainer[i] / NumWorldTile;
		PosTile.x	= static_cast<RwReal>(CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
		PosTile.z	= static_cast<RwReal>(CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);

		RwInt32 CurSectorIdx = pMgr->GetSectorIdx(PosTile);
		if(CurSectorIdx != -1)
		{
			pMgr->m_pSectors[CurSectorIdx].SetTransparentTile(_IsTileTransparency, PosTile);
		}
	}
}

void CWorldEditorEventHander::UpdateNMPropTile()
{
	m_vecTileAttr.clear();

	RwBool* pNMMapProp = dGETFRM()->m_pNMPropPalette->GetNMapProp();

	CNtlWorldFieldManager*	pMgr		= dGETMGR();
	RwV3d					Pos			= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwInt32					IdxCamField = pMgr->GetFieldIdx(Pos);

	if(IdxCamField == -1)
	{
		DBO_TRACE(FALSE, "CWorldEditorEventHander::UpdateNMPropTile, invalid field index.");
		return;
	}

	RwInt32			LenField	= dGET_WORLD_PARAM()->WorldFieldSize;
	RwInt32			LenTile		= dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32			LenHalf		= dGET_WORLD_PARAM()->WorldSizeHalf;
	CNtlWorldField*	pFields		= const_cast<CNtlWorldField*>(pMgr->GetFields());

	// center
	RwInt32 i, j;
	RwV3d	PosS;
	RwInt32 IdxTileSX, IdxTileSZ;
	DWORD*	pBitProp;
	RwInt32	NumFieldTile = dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
	RwInt32 NumWorldTile = dGET_WORLD_PARAM()->WorldSize / dGET_WORLD_PARAM()->WorldSectorTileSize;
	DWORD	ArgBitProp;

	PosS		= pFields[IdxCamField].GetSPos();
	IdxTileSX	= static_cast<RwInt32>((PosS.x + dGET_WORLD_PARAM()->WorldSizeHalf) / dGET_WORLD_PARAM()->WorldSectorTileSize);
	IdxTileSZ	= static_cast<RwInt32>((PosS.z + dGET_WORLD_PARAM()->WorldSizeHalf) / dGET_WORLD_PARAM()->WorldSectorTileSize);
	pBitProp	= pFields[IdxCamField].GetProp()->_pBitProp;

	for(i = 0; i < NumFieldTile; ++i)
	{
		for(j = 0; j < NumFieldTile; ++j)
		{
			ArgBitProp = pBitProp[j + i * NumFieldTile];
			if(dGETFRM()->m_pNMPropPalette->IsMatched(ArgBitProp))
			{
				m_vecTileAttr.push_back((IdxTileSX + j) + ((IdxTileSZ + i) * NumWorldTile));
			}
		}
	}
}

void CWorldEditorEventHander::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY || pMsg.Id == NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY)
	{
		if(dGET_CUR_PALETTE() == ePM_NM_PROP)
		{
			if(m_pBrushSelector)
			{
				UpdateNMPropBrushLimit();
			}

			if(dGETFRM()->m_pNMPropPalette->GetNMapPropActivated())
			{
				UpdateNMPropTile();
			}
		}	
		else// if (dGET_CUR_PALETTE() == ePM_DEFAULT)
		{
			if (CNtlWorldPathEngineManager::GetInstance()->GetPathEngineMode())
			{
				if(m_pBrushSelectorPE)
				{
					UpdatePathEngineBrushLimit();
				}
			}
		}

		RefreshPerformanceTextInfo();
	}
	else if(pMsg.Id == NPEI_IS_MAPNAME_CHANGE)
	{
		TBLIDX*		pIdx		= static_cast<TBLIDX*>(pMsg.pData);
		CTextTable*	pTextMobTbl	= static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl());

		m_pFieldName->SetText(pTextMobTbl->GetText(*pIdx).c_str());
	}
}

void CWorldEditorEventHander::UpdateEffect()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	// placing mode
	if(dGET_BRUSH_ENTITY())
	{
		m_vecSelectedEffect.clear();

		// wanna cancel the effect attached to current mouse brush and use effects editing mode
		if(dOKEY(DIK_SPACE))
		{
			if(dGET_BRUSH_ENTITY())
			{
				GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
				dGET_BRUSH_ENTITY() = NULL;
				dGETFRM()->m_pEffectPalette->UpdatePropertyData(NULL);
				return;
			}
		}

		// object moving with picking variation
		sNPE_COLLISION_PARAM sNPECollisionParam;
		sNPECollisionParam._pNtlPLEntity = dGET_BRUSH_ENTITY();

		RwV3d vDist;
		vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
		vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
		vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;
		dGET_BRUSH_ENTITY()->SetPosition(&vDist);

		// place effects
		if(dGET_MOUSE()->GetLBOnce())
		{
			if(dGET_BRUSH_ENTITY())
			{
				RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vDist);
				if(CurSectorIdx != -1)
				{
					SPLEntityCreateParam PLEntityCreateParam;
					PLEntityCreateParam.pPos = &vDist;

					CNtlPLEntity *pNtlPLEntity	= GetSceneManager()->CreateEntity(PLENTITY_EFFECT, dGET_BRUSH_ENTITY()->GetName(), &PLEntityCreateParam);
					DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

					(static_cast<CNtlInstanceEffect*>(pNtlPLEntity))->SetVisibleBoundingVolume(TRUE);

					pNtlPLEntity->SetScale(dGET_BRUSH_ENTITY()->GetScale());
					pNtlPLEntity->SetRotate(dGET_BRUSH_ENTITY()->GetRotate()->y, dGET_BRUSH_ENTITY()->GetRotate()->x, dGET_BRUSH_ENTITY()->GetRotate()->z);
					pNtlPLEntity->SetPosition(&dGET_BRUSH_ENTITY()->GetPosition());
					pNtlPLEntity->SetVisibleCullingDistance(dGET_BRUSH_ENTITY()->GetVisibleCullingDistance());

					pMgr->m_pSectors[CurSectorIdx].InsertIn2World(pNtlPLEntity, 1);
				}
			}
		}
	}
	// capture mode; picking
	else if(!dGET_BRUSH_ENTITY())
	{
		if(dGET_MOUSE()->GetLBOnce() && !dKEY(DIK_LSHIFT) && !dKEY(DIK_RSHIFT) && !dKEY(DIK_SPACE))
		{
			RwV2d ScreenPos;
			ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
			ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

			RwLine cameraRay;
			RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

			// pick map objects, collision
			sNPE_COLLISION_PARAM NPECollisionParam;

			RpIntersection Intersection;
			Intersection.type = rpINTERSECTLINE;
			::CopyMemory(&Intersection.t.line, &cameraRay, sizeof(RwLine));

			RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &Intersection, WorldIntersection4EffectCB_CTLine2Sphere, &NPECollisionParam);

			RwInt32 Num = NPECollisionParam._vecNPECollisionParam.size();
			if(Num)
			{
				RwInt32 Idx		= 0;
				RwReal	Dist	= NPECollisionParam._vecNPECollisionParam[0]._Dist;

				for(int i = 1; i < Num; ++i)
				{
					if(NPECollisionParam._vecNPECollisionParam[i]._Dist < Dist)
					{
						Idx = i;
					}
				}

				m_vecSelectedEffect.clear();
				m_vecSelectedEffect.push_back(NPECollisionParam._vecNPECollisionParam[Idx]._pNtlPLEntity);
				dGETFRM()->m_pEffectPalette->UpdatePropertyData(GetCurEffect());

				if(!m_pBrushSelection)
				{
					m_pBrushSelection						= NTL_NEW CNtlWorldBrush;
					CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
					m_pBrushSelection->SetController(pController);
				}
			}
			else
			{
				m_vecSelectedEffect.clear();
				dGETFRM()->m_pEffectPalette->UpdatePropertyData(NULL);				
			}
		}

		if(m_vecSelectedEffect.size() == 1)
		{
			if(dGET_MOUSE()->GetLBDC())
			{
				CEffectPropPopup Dlg;
				Dlg.DoModal();
			}
		}

		// editing mode
		if(m_vecSelectedEffect.size())
		{
			if(dOKEY(DIK_DELETE))
			{
				RwBool BreakFlag = FALSE;

				for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
				{
					RwInt32 NumObj = static_cast<RwInt32>(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityEffect.size());
					for(int k = 0; k < NumObj; ++k)
					{
						if(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityEffect[k] == m_vecSelectedEffect[0])
						{
							pMgr->m_pSectors[g_vecVisibleSectors[j]].DeleteEntity(m_vecSelectedEffect[0], 1);
							GetSceneManager()->DeleteEntity(m_vecSelectedEffect[0]);
							dGETFRM()->m_pEffectPalette->UpdatePropertyData(NULL);
							m_vecSelectedEffect.clear();
							BreakFlag = TRUE;
							break;
						}
					}	

					if(BreakFlag)
					{
						break;
					}
				}
			}

			if(dKEY(DIK_LCONTROL) && dGET_MOUSE()->GetLBOnce() && m_vecSelectedEffect.size() == 1)
			{
				if(dGET_BRUSH_ENTITY())
				{
					GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());					
					dGET_BRUSH_ENTITY() = NULL;
				}

				CNtlInstanceEffect* pEffect = (CNtlInstanceEffect*)m_vecSelectedEffect.at(0);
				if (pEffect)
				{
					RwInt32 CurSectorIdx = pMgr->GetSectorIdx(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
					if (CurSectorIdx != -1)
					{
						SPLEntityCreateParam PLEntityCreateParam;
						PLEntityCreateParam.pPos = &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;

						dGET_BRUSH_ENTITY()	= GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pEffect->GetName(), &PLEntityCreateParam);
						DBO_ASSERT(dGET_BRUSH_ENTITY(), "Entity create failed.");

						(static_cast<CNtlInstanceEffect*>(dGET_BRUSH_ENTITY()))->SetVisibleBoundingVolume(TRUE);

						dGET_BRUSH_ENTITY()->SetScale(pEffect->GetScale());
						dGET_BRUSH_ENTITY()->SetRotate(pEffect->GetRotate()->y, pEffect->GetRotate()->x, pEffect->GetRotate()->z);
						dGET_BRUSH_ENTITY()->SetPosition(&pEffect->GetPosition());
						dGET_BRUSH_ENTITY()->SetMinimapLayer(pEffect->GetMinimapLayer());
						dGET_BRUSH_ENTITY()->SetVisibleCullingDistance(pEffect->GetVisibleCullingDistance());

						m_vecSelectedEffect.clear();

						return;
					}					
				}					
			}

			// transform
			if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
			{
				// translate
				if(dKEY(DIK_LSHIFT))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

					if(dKEY(DIK_Z))
					{
						RwV3d PosCur = m_vecSelectedEffect[0]->GetPosition();
						PosCur.y -= DistY;
						m_vecSelectedEffect[0]->SetPosition(&PosCur);
					}
					else
					{
						RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
						RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
						RwV3d *pRight			= RwMatrixGetRight(pM);
						RwV3d *pUp				= RwMatrixGetUp(pM);
						RwV3d *pAt				= RwMatrixGetAt(pM);
						RwV3d vFront, vRight;
						RwReal TerrainHeight, TotalHeight, ObjHeight;

						CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
						CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
						RwV3dNormalize(&vFront, &vFront);
						RwV3dNormalize(&vRight, &vRight);

						RwV3d PosCur = m_vecSelectedEffect[0]->GetPosition();

						// delete from the previous field
						RwInt32 CurSectorIdx = pMgr->GetSectorIdx(PosCur);
						if(CurSectorIdx != -1)
						{
							pMgr->m_pSectors[CurSectorIdx].DeleteEntity(m_vecSelectedEffect[0], 1);
						}

						TotalHeight		= dGET_POSITIVE_VALUE(PosCur.y);
						TerrainHeight	= dGET_POSITIVE_VALUE(pMgr->GetWorldSectorHeight(PosCur));
						ObjHeight		= TotalHeight - TerrainHeight;

						// transform
						RwV3dIncrementScaled(&PosCur, &vFront, -DistY);
						RwV3dIncrementScaled(&PosCur, &vRight, -DistX);

						PosCur.y = pMgr->GetWorldSectorHeight(PosCur) + ObjHeight;
						m_vecSelectedEffect[0]->SetPosition(&PosCur);

						// create into the new field
						CurSectorIdx = pMgr->GetSectorIdx(PosCur);
						if(CurSectorIdx != -1)
						{
							pMgr->m_pSectors[CurSectorIdx].InsertIn2World(m_vecSelectedEffect[0], 1);
						}
					}
				}
				// rotate
				else if(dKEY(DIK_SPACE))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(1.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(1.0f);

					if(dKEY(DIK_V))
					{
						const RwV3d *pPosCur = m_vecSelectedEffect[0]->GetRotate();
						m_vecSelectedEffect[0]->SetRotate(pPosCur->x - DistX, pPosCur->y, pPosCur->z);
					}
					else if(dKEY(DIK_N))
					{
						const RwV3d *pPosCur = m_vecSelectedEffect[0]->GetRotate();
						m_vecSelectedEffect[0]->SetRotate(pPosCur->x, pPosCur->y, pPosCur->z - DistX);
					}
					else
					{
						const RwV3d *pPosCur = m_vecSelectedEffect[0]->GetRotate();
						m_vecSelectedEffect[0]->SetRotate(pPosCur->x, pPosCur->y - DistX, pPosCur->z);
					}
				}
				// scale
				else if(dKEY(DIK_RSHIFT))
				{
					float	DistX		= dGET_MOUSE()->GetXMovementWithRate(20.0f);
					float	DistY		= dGET_MOUSE()->GetYMovementWithRate(20.0f);
					RwReal	ScaleRate	= m_vecSelectedEffect[0]->GetScale();

					ScaleRate -= DistY;
					CLAMP(ScaleRate, 0.0f, 10.0f);
					m_vecSelectedEffect[0]->SetScale(ScaleRate);
				}
			}

			// Rotation using Keyboard
			if(dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					if(dOKEY(DIK_LEFTARROW))
					{
						const RwV3d *pPosCur = m_vecSelectedEffect[0]->GetRotate();
						m_vecSelectedEffect[0]->SetRotate(pPosCur->x, pPosCur->y - m_EffectTransform[1], pPosCur->z);
					}
					else if(dOKEY(DIK_RIGHTARROW))
					{
						const RwV3d *pPosCur = m_vecSelectedEffect[0]->GetRotate();
						m_vecSelectedEffect[0]->SetRotate(pPosCur->x, pPosCur->y + m_EffectTransform[1], pPosCur->z);
					}
				}
				else
				{
					if(dKEY(DIK_LEFTARROW))
					{
						const RwV3d *pPosCur = m_vecSelectedEffect[0]->GetRotate();
						m_vecSelectedEffect[0]->SetRotate(pPosCur->x, pPosCur->y - m_EffectTransform[1], pPosCur->z);
					}
					else if(dKEY(DIK_RIGHTARROW))
					{
						const RwV3d *pPosCur = m_vecSelectedEffect[0]->GetRotate();
						m_vecSelectedEffect[0]->SetRotate(pPosCur->x, pPosCur->y + m_EffectTransform[1], pPosCur->z);
					}
				}
			}

			// translation using keyboard
			if(dKEY(DIK_UPARROW) | dKEY(DIK_DOWNARROW))
			{
				if(dKEY(DIK_SPACE))
				{
					if(!dKEY(DIK_LSHIFT))
					{
						RwV3d PosCur = m_vecSelectedEffect[0]->GetPosition();

						if(dKEY(DIK_UPARROW))
						{
							PosCur.y += m_EffectTransform[2];
						}
						else if(dKEY(DIK_DOWNARROW))
						{
							PosCur.y -= m_EffectTransform[2];
						}

						m_vecSelectedEffect[0]->SetPosition(&PosCur);
					}
					else
					{
						RwV3d PosCur = m_vecSelectedEffect[0]->GetPosition();

						if(dOKEY(DIK_UPARROW))
						{
							PosCur.y += m_EffectTransform[2];
						}
						else if(dOKEY(DIK_DOWNARROW))
						{
							PosCur.y -= m_EffectTransform[2];
						}

						m_vecSelectedEffect[0]->SetPosition(&PosCur);
					}
				}
			}

			if(dKEY(DIK_LEFTARROW) || dKEY(DIK_RIGHTARROW) || dKEY(DIK_UPARROW) || dKEY(DIK_DOWNARROW))
			{			
				if(!dKEY(DIK_SPACE))
				{
					RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
					RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
					RwV3d *pRight			= RwMatrixGetRight(pM);
					RwV3d *pUp				= RwMatrixGetUp(pM);
					RwV3d *pAt				= RwMatrixGetAt(pM);
					RwV3d vFront, vRight;

					CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
					CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
					RwV3dNormalize(&vFront, &vFront);
					RwV3dNormalize(&vRight, &vRight);

					RwV3d PosCur = m_vecSelectedEffect[0]->GetPosition();

					// delete from the previous field
					RwInt32 CurSectorIdx = pMgr->GetSectorIdx(PosCur);
					if(CurSectorIdx != -1)
					{
						pMgr->m_pSectors[CurSectorIdx].DeleteEntity(m_vecSelectedEffect[0], 1);
					}

					if(!dKEY(DIK_LSHIFT))
					{
						if(dKEY(DIK_LEFTARROW))
						{
							RwV3dIncrementScaled(&PosCur, &vRight, m_EffectTransform[2]);
						}
						else if(dKEY(DIK_RIGHTARROW))
						{
							RwV3dIncrementScaled(&PosCur, &vRight, -m_EffectTransform[2]);
						}
						else if(dKEY(DIK_UPARROW))
						{
							RwV3dIncrementScaled(&PosCur, &vFront, m_EffectTransform[2]);
						}
						else if(dKEY(DIK_DOWNARROW))
						{
							RwV3dIncrementScaled(&PosCur, &vFront, -m_EffectTransform[2]);
						}
					}
					else
					{
						if(dOKEY(DIK_LEFTARROW))
						{
							RwV3dIncrementScaled(&PosCur, &vRight, m_EffectTransform[2]);
						}
						else if(dOKEY(DIK_RIGHTARROW))
						{
							RwV3dIncrementScaled(&PosCur, &vRight, -m_EffectTransform[2]);
						}
						else if(dOKEY(DIK_UPARROW))
						{
							RwV3dIncrementScaled(&PosCur, &vFront, m_EffectTransform[2]);
						}
						else if(dOKEY(DIK_DOWNARROW))
						{
							RwV3dIncrementScaled(&PosCur, &vFront, -m_EffectTransform[2]);
						}
					}

					m_vecSelectedEffect[0]->SetPosition(&PosCur);

					// create into the new field
					CurSectorIdx = pMgr->GetSectorIdx(PosCur);
					if(CurSectorIdx != -1)
					{
						pMgr->m_pSectors[CurSectorIdx].InsertIn2World(m_vecSelectedEffect[0], 1);
					}
				}
			}

			// backup transform
			if(dOKEY(DIK_INSERT) || dOKEY(DIK_HOME) || dOKEY(DIK_PGUP))
			{
				if(dOKEY(DIK_INSERT))
				{
					m_vecSelectedEffect[0]->SetScale(1.0f);
				}

				if(dOKEY(DIK_HOME))
				{
					m_vecSelectedEffect[0]->SetRotate(0.0f, 0.0f, 0.0f);
				}

				if(dOKEY(DIK_PGUP))
				{
					RwV3d vTransUnit;
					vTransUnit = m_vecSelectedEffect[0]->GetPosition();
					vTransUnit.y = pMgr->GetWorldSectorHeight(vTransUnit);
					m_vecSelectedEffect[0]->SetPosition(&vTransUnit);
				}
			}

		}
	}
}

// CZ-SS
// RwBool CWorldEditorEventHander::IsValidSoftSpot(RwV3d& _Pos, RwBool _MovingFlag/* = FALSE*/)
// {
// 	CNtlWorldFieldManager*	pMgr			= dGETMGR();
// 	RwInt32					CurSectorIdx	= pMgr->GetSectorIdx(_Pos);
// 	RwInt32					SecotorIdxGrp[9];
// 	RwV3d					NeiborPos;
// 	RwInt32					CurNeiborIdx;
// 
// 	if(CurSectorIdx == -1)
// 	{
// 		return FALSE;
// 	}
// 
// 	//8 7 6
// 	//5 4 3
// 	//2 1 0
// 
// 	// 0
// 	NeiborPos		= _Pos;
// 	NeiborPos.x		-= dGET_WORLD_PARAM()->WorldSectorSize;
// 	NeiborPos.z		-= dGET_WORLD_PARAM()->WorldSectorSize;
// 	CurNeiborIdx	= pMgr->GetSectorIdx(NeiborPos);
// 	if(CurNeiborIdx != -1)
// 	{
// 		SecotorIdxGrp[0] = CurNeiborIdx;
// 	}
// 	else
// 	{
// 		SecotorIdxGrp[0] = -1;
// 	}
// 
// 	// 1
// 	NeiborPos		= _Pos;
// 	NeiborPos.z		-= dGET_WORLD_PARAM()->WorldSectorSize;
// 	CurNeiborIdx	= pMgr->GetSectorIdx(NeiborPos);
// 	if(CurNeiborIdx != -1)
// 	{
// 		SecotorIdxGrp[1] = CurNeiborIdx;
// 	}
// 	else
// 	{
// 		SecotorIdxGrp[1] = -1;
// 	}
// 
// 	// 2
// 	NeiborPos		= _Pos;
// 	NeiborPos.x		+= dGET_WORLD_PARAM()->WorldSectorSize;
// 	NeiborPos.z		-= dGET_WORLD_PARAM()->WorldSectorSize;
// 	CurNeiborIdx	= pMgr->GetSectorIdx(NeiborPos);
// 	if(CurNeiborIdx != -1)
// 	{
// 		SecotorIdxGrp[2] = CurNeiborIdx;
// 	}
// 	else
// 	{
// 		SecotorIdxGrp[2] = -1;
// 	}
// 
// 	// 3
// 	NeiborPos		= _Pos;
// 	NeiborPos.x		-= dGET_WORLD_PARAM()->WorldSectorSize;
// 	CurNeiborIdx	= pMgr->GetSectorIdx(NeiborPos);
// 	if(CurNeiborIdx != -1)
// 	{
// 		SecotorIdxGrp[3] = CurNeiborIdx;
// 	}
// 	else
// 	{
// 		SecotorIdxGrp[3] = -1;
// 	}
// 
// 	// 4
// 	SecotorIdxGrp[4] = CurSectorIdx;
// 
// 	// 5
// 	NeiborPos		= _Pos;
// 	NeiborPos.x		+= dGET_WORLD_PARAM()->WorldSectorSize;
// 	CurNeiborIdx	= pMgr->GetSectorIdx(NeiborPos);
// 	if(CurNeiborIdx != -1)
// 	{
// 		SecotorIdxGrp[5] = CurNeiborIdx;
// 	}
// 	else
// 	{
// 		SecotorIdxGrp[5] = -1;
// 	}
// 
// 	// 6
// 	NeiborPos		= _Pos;
// 	NeiborPos.x		-= dGET_WORLD_PARAM()->WorldSectorSize;
// 	NeiborPos.z		+= dGET_WORLD_PARAM()->WorldSectorSize;
// 	CurNeiborIdx	= pMgr->GetSectorIdx(NeiborPos);
// 	if(CurNeiborIdx != -1)
// 	{
// 		SecotorIdxGrp[6] = CurNeiborIdx;
// 	}
// 	else
// 	{
// 		SecotorIdxGrp[6] = -1;
// 	}
// 
// 	// 7
// 	NeiborPos		= _Pos;
// 	NeiborPos.z		+= dGET_WORLD_PARAM()->WorldSectorSize;
// 	CurNeiborIdx	= pMgr->GetSectorIdx(NeiborPos);
// 	if(CurNeiborIdx != -1)
// 	{
// 		SecotorIdxGrp[7] = CurNeiborIdx;
// 	}
// 	else
// 	{
// 		SecotorIdxGrp[7] = -1;
// 	}
// 
// 	// 8
// 	NeiborPos		= _Pos;
// 	NeiborPos.x		+= dGET_WORLD_PARAM()->WorldSectorSize;
// 	NeiborPos.z		+= dGET_WORLD_PARAM()->WorldSectorSize;
// 	CurNeiborIdx	= pMgr->GetSectorIdx(NeiborPos);
// 	if(CurNeiborIdx != -1)
// 	{
// 		SecotorIdxGrp[8] = CurNeiborIdx;
// 	}
// 	else
// 	{
// 		SecotorIdxGrp[8] = -1;
// 	}
// 
// 	RwInt32 TotSSCnt = 0;
// 	for(RwInt32 i = 0; i < 9; ++i)
// 	{
// 		RwInt32 CurSectorIdxIfGrp = SecotorIdxGrp[i];
// 		if(CurSectorIdxIfGrp == -1)
// 		{
// 			continue;
// 		}
// 
// 		TotSSCnt += static_cast<RwInt32>(pMgr->m_pSectors[CurSectorIdxIfGrp].m_vecNtlPLEntitySS.size());
// 	}
// 
// 	if(_MovingFlag)
// 	{	
// 		if(TotSSCnt <= 5)
// 		{
// 			return TRUE;
// 		}
// 		else
// 		{
// 			return FALSE;
// 		}
// 	}
// 	else
// 	{
// 		if(TotSSCnt >= 5)
// 		{
// 			return FALSE;
// 		}
// 	}
// 
// 	return TRUE;
// }

VOID CWorldEditorEventHander::UpdateLightMode()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	if(dGETFRM()->m_pLightModePalette->GetCurLightData())
	{
		if(dOKEY(DIK_SPACE))
		{
			dGETFRM()->m_pLightModePalette->UpdatePropertyData(NULL);
			dGETFRM()->m_pLightModePalette->SetCurLightData(NULL);
			return;
		}

		// move entity
		RwV3d vDist;
		vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
		vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
		vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;

		dGETFRM()->m_pLightModePalette->GetCurLightData()->vPosision = vDist;

		if (dGET_COLLISION_INFO()->IsCollidedAtSectors)
		{
			if(dGET_MOUSE()->GetLBOnce())
			{
				dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetCurLightData());
				dGETFRM()->m_pLightModePalette->SetCurLightData(NULL);
				return;
			}
		}
	}
	else
	{
		// picking
		if(dGET_MOUSE()->GetLBOnce() && !dKEY(DIK_LSHIFT) && !dKEY(DIK_RSHIFT) && !dKEY(DIK_SPACE))
		{
			CNtlPLLightObject*		pNtlPLEntity	= NULL;
			RwReal					fResultDist		= RwRealMAXVAL;	

			RwV3d vCur;
			if (GetSceneManager()->GetWorld()->GetPlayerPosition(vCur))
			{
				dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->Pick());
			}
		}

		if (dGETFRM()->m_pLightModePalette->GetSelLightData())
		{
			CNtlWorldFieldManager*	pMgr				= dGETMGR();

			if(dOKEY(DIK_DELETE))
			{
				dGETFRM()->m_pLightModePalette->GetSelLightData()->rgbafColor.red	= 0.0f;
				dGETFRM()->m_pLightModePalette->GetSelLightData()->rgbafColor.green = 0.0f;
				dGETFRM()->m_pLightModePalette->GetSelLightData()->rgbafColor.blue	= 0.0f;
				dGETFRM()->m_pLightModePalette->UpdateRpLight(dGETFRM()->m_pLightModePalette->GetSelLightData());
				dGETFRM()->m_pLightModePalette->UpdatePropertyData(NULL);
				return;
			}

			if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

					if(dKEY(DIK_Z))
					{
						RwV3d vPos = dGETFRM()->m_pLightModePalette->GetSelLightData()->vPosision;
						vPos.y -= DistY;
						dGETFRM()->m_pLightModePalette->GetSelLightData()->vPosision = vPos;
						dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetSelLightData());
					}
					else
					{
						RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
						RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
						RwV3d *pRight			= RwMatrixGetRight(pM);
						RwV3d *pUp				= RwMatrixGetUp(pM);
						RwV3d *pAt				= RwMatrixGetAt(pM);
						RwV3d vFront, vRight;
						RwReal TerrainHeight, TotalHeight, ObjHeight;

						CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
						CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
						RwV3dNormalize(&vFront, &vFront);
						RwV3dNormalize(&vRight, &vRight);

						RwV3d vPos = dGETFRM()->m_pLightModePalette->GetSelLightData()->vPosision;

						TotalHeight		= dGET_POSITIVE_VALUE(vPos.y);
						TerrainHeight	= dGET_POSITIVE_VALUE(pMgr->GetWorldSectorHeight(vPos));
						ObjHeight		= TotalHeight - TerrainHeight;

						// transform
						RwV3dIncrementScaled(&vPos, &vFront, -DistY);
						RwV3dIncrementScaled(&vPos, &vRight, -DistX);

						vPos.y = pMgr->GetWorldSectorHeight(vPos) + ObjHeight;
						dGETFRM()->m_pLightModePalette->GetSelLightData()->vPosision = vPos;
						dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetSelLightData());
					}
				}
				// Rot
				else if(dKEY(DIK_SPACE))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(1.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(1.0f);

					if(dKEY(DIK_V))
					{
						RwV3d vRot = dGETFRM()->m_pLightModePalette->GetSelLightData()->vRotation;
						vRot.x -= DistX;
						if (vRot.x < 0.0f)
						{
							vRot.x = 360.0f;
						}
						if (vRot.x > 360.0f)
						{
							vRot.x = 0.0f;
						}						
						dGETFRM()->m_pLightModePalette->GetSelLightData()->vRotation = vRot;
						dGETFRM()->m_pLightModePalette->UpdateRpLight(dGETFRM()->m_pLightModePalette->GetSelLightData());
						dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetSelLightData());
					}
					/*else if(dKEY(DIK_N))
					{
					RwV3d vRot = dGETFRM()->m_pLightModePalette->GetSelLightData()->vRotation;
					vRot.z -= DistX;
					CLAMP(vRot.x, 0.0f, 359.0f);
					CLAMP(vRot.y, 0.0f, 359.0f);
					CLAMP(vRot.z, 0.0f, 359.0f);
					dGETFRM()->m_pLightModePalette->GetSelLightData()->vRotation = vRot;
					dGETFRM()->m_pLightModePalette->UpdateRpLight(dGETFRM()->m_pLightModePalette->GetSelLightData());
					dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetSelLightData());
					}*/
					else
					{
						RwV3d vRot = dGETFRM()->m_pLightModePalette->GetSelLightData()->vRotation;
						vRot.y -= DistX;
						if (vRot.y < 0.0f)
						{
							vRot.y = 360.0f;
						}
						if (vRot.y > 360.0f)
						{
							vRot.y = 0.0f;
						}
						dGETFRM()->m_pLightModePalette->GetSelLightData()->vRotation = vRot;
						dGETFRM()->m_pLightModePalette->UpdateRpLight(dGETFRM()->m_pLightModePalette->GetSelLightData());
						dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetSelLightData());
					}
				}
			}

			// Rotation using Keyboard
			if(dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					if(dOKEY(DIK_LEFTARROW))
					{
						RwV3d vRot = dGETFRM()->m_pLightModePalette->GetSelLightData()->vRotation;
						vRot.y -= m_RotationTabValue;
						if (vRot.y < 0.0f)
						{
							vRot.y = 360.0f;
						}
						if (vRot.y > 360.0f)
						{
							vRot.y = 0.0f;
						}
						dGETFRM()->m_pLightModePalette->GetSelLightData()->vRotation = vRot;
						dGETFRM()->m_pLightModePalette->UpdateRpLight(dGETFRM()->m_pLightModePalette->GetSelLightData());
						dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetSelLightData());
					}
					else if(dOKEY(DIK_RIGHTARROW))
					{
						RwV3d vRot = dGETFRM()->m_pLightModePalette->GetSelLightData()->vRotation;
						vRot.y += m_RotationTabValue;
						if (vRot.y < 0.0f)
						{
							vRot.y = 360.0f;
						}
						if (vRot.y > 360.0f)
						{
							vRot.y = 0.0f;
						}
						dGETFRM()->m_pLightModePalette->GetSelLightData()->vRotation = vRot;
						dGETFRM()->m_pLightModePalette->UpdateRpLight(dGETFRM()->m_pLightModePalette->GetSelLightData());
						dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetSelLightData());
					}
				}
				else
				{
					if(dOKEY(DIK_LEFTARROW))
					{
						RwV3d vRot = dGETFRM()->m_pLightModePalette->GetSelLightData()->vRotation;
						vRot.y -= m_RotationTabValue;
						if (vRot.y < 0.0f)
						{
							vRot.y = 360.0f;
						}
						if (vRot.y > 360.0f)
						{
							vRot.y = 0.0f;
						}
						dGETFRM()->m_pLightModePalette->GetSelLightData()->vRotation = vRot;
						dGETFRM()->m_pLightModePalette->UpdateRpLight(dGETFRM()->m_pLightModePalette->GetSelLightData());
						dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetSelLightData());
					}
					else if(dOKEY(DIK_RIGHTARROW))
					{
						RwV3d vRot = dGETFRM()->m_pLightModePalette->GetSelLightData()->vRotation;
						vRot.y += m_RotationTabValue;
						if (vRot.y < 0.0f)
						{
							vRot.y = 360.0f;
						}
						if (vRot.y > 360.0f)
						{
							vRot.y = 0.0f;
						}
						dGETFRM()->m_pLightModePalette->GetSelLightData()->vRotation = vRot;
						dGETFRM()->m_pLightModePalette->UpdateRpLight(dGETFRM()->m_pLightModePalette->GetSelLightData());
						dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetSelLightData());
					}
				}
			}

			// translation keyboard
			if(dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					if(dOKEY(DIK_UPARROW))
					{
						RwV3d vPos = dGETFRM()->m_pLightModePalette->GetSelLightData()->vPosision;
						vPos.y += m_fObjMovementSensitivity;
						dGETFRM()->m_pLightModePalette->GetSelLightData()->vPosision = vPos;
						dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetSelLightData());
					}
					else if(dOKEY(DIK_DOWNARROW))
					{
						RwV3d vPos = dGETFRM()->m_pLightModePalette->GetSelLightData()->vPosision;
						vPos.y -= m_fObjMovementSensitivity;
						dGETFRM()->m_pLightModePalette->GetSelLightData()->vPosision = vPos;
						dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetSelLightData());
					}
				}
				else
				{
					if(dOKEY(DIK_UPARROW))
					{
						RwV3d vPos = dGETFRM()->m_pLightModePalette->GetSelLightData()->vPosision;
						vPos.y += m_fObjMovementSensitivity;
						dGETFRM()->m_pLightModePalette->GetSelLightData()->vPosision = vPos;
						dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetSelLightData());
					}
					else if(dOKEY(DIK_DOWNARROW))
					{
						RwV3d vPos = dGETFRM()->m_pLightModePalette->GetSelLightData()->vPosision;
						vPos.y -= m_fObjMovementSensitivity;
						dGETFRM()->m_pLightModePalette->GetSelLightData()->vPosision = vPos;
						dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetSelLightData());
					}
				}
			}

			if(!dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LEFTARROW) || dKEY(DIK_RIGHTARROW) || dKEY(DIK_UPARROW) || dKEY(DIK_DOWNARROW))
				{
					RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
					RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
					RwV3d *pRight			= RwMatrixGetRight(pM);
					RwV3d *pUp				= RwMatrixGetUp(pM);
					RwV3d *pAt				= RwMatrixGetAt(pM);
					RwV3d vFront, vRight;

					CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
					CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
					RwV3dNormalize(&vFront, &vFront);
					RwV3dNormalize(&vRight, &vRight);

					RwV3d vPos = dGETFRM()->m_pLightModePalette->GetSelLightData()->vPosision;

					if(!dKEY(DIK_LSHIFT))
					{
						if(dKEY(DIK_LEFTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, m_fObjMovementSensitivity);
						}
						else if(dKEY(DIK_RIGHTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, -m_fObjMovementSensitivity);
						}
						else if(dKEY(DIK_UPARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, m_fObjMovementSensitivity);
						}
						else if(dKEY(DIK_DOWNARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, -m_fObjMovementSensitivity);
						}
					}
					else
					{
						if(dOKEY(DIK_LEFTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, m_fObjMovementSensitivity);
						}
						else if(dOKEY(DIK_RIGHTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, -m_fObjMovementSensitivity);
						}
						else if(dOKEY(DIK_UPARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, m_fObjMovementSensitivity);
						}
						else if(dOKEY(DIK_DOWNARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, -m_fObjMovementSensitivity);
						}
					}

					dGETFRM()->m_pLightModePalette->GetSelLightData()->vPosision = vPos;
					dGETFRM()->m_pLightModePalette->UpdatePropertyData(dGETFRM()->m_pLightModePalette->GetSelLightData());
				}
			}
		}
	}
}


// VOID CWorldEditorEventHander::UpdateLightObject()
// {
// 	CNtlWorldFieldManager *pMgr = dGETMGR();
// 
// 	if(dGET_BRUSH_ENTITY())
// 	{
// 		if(dOKEY(DIK_SPACE))
// 		{
// 			if(dGET_BRUSH_ENTITY())
// 			{
// 				dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(NULL);
// 				GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
// 				dGET_BRUSH_ENTITY() = NULL;
// 				return;
// 			}
// 		}
// 
// 		// move entity
// 		sNPE_COLLISION_PARAM sNPECollisionParam;
// 		sNPECollisionParam._pNtlPLEntity = dGET_BRUSH_ENTITY();
// 
// 		RwV3d vDist;
// 		vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x - dGET_COLLISION_INFO()->RayDir.x * 10.0f;
// 		vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y - dGET_COLLISION_INFO()->RayDir.y * 10.0f;
// 		vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z - dGET_COLLISION_INFO()->RayDir.z * 10.0f;
// 
// 		dGET_BRUSH_ENTITY()->SetPosition(&vDist);
// 		if (dGET_COLLISION_INFO()->IsCollidedAtSectors)
// 		{
// 			// place effects
// 			if(dGET_MOUSE()->GetLBOnce())
// 			{
// 				if(dGET_BRUSH_ENTITY())
// 				{
// 					RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vDist);
// 					if(CurSectorIdx != -1)
// 					{
// 						if (dGETFRM()->m_pLightObjectPalette->GetCreateParam(dGET_BRUSH_ENTITY()))
// 						{
// 							CNtlPLEntity *pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, NULL, dGETFRM()->m_pLightObjectPalette->GetCreateParam(dGET_BRUSH_ENTITY()));
// 							DBO_ASSERT(pNtlPLEntity, "Entity create failed.");
// 							
// 							pMgr->m_pSectors[CurSectorIdx].InsertIn2World(pNtlPLEntity, 8);
// 							return;
// 						}					
// 					}
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		// picking
// 		if(dGET_MOUSE()->GetLBOnce() && !dKEY(DIK_LSHIFT) && !dKEY(DIK_RSHIFT) && !dKEY(DIK_SPACE))
// 		{
// 			RwReal					fCurDist;
// 			CNtlPLLightObject*		pNtlPLEntity	= NULL;
// 			RwReal					fResultDist		= RwRealMAXVAL;	
// 
// 			RwV3d vCur;
// 			if (GetSceneManager()->GetWorld()->GetPlayerPosition(vCur))
// 			{
// 				CNtlWorldFieldManager*	pWFMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
// 				RwInt32					iSectorIdx	= pWFMgr->GetSectorIdx(vCur);
// 
// 				if (iSectorIdx)
// 				{
// 					vector<RwInt32> vecNeighborSectorIndices;
// 					pWFMgr->GetNeighborSectors(iSectorIdx, vecNeighborSectorIndices);
// 					vecNeighborSectorIndices.push_back(iSectorIdx);
// 
// 					for (int i = 0; i < (int)vecNeighborSectorIndices.size(); ++i)
// 					{
// 						RwInt32 iIdx = vecNeighborSectorIndices.at(i);
// 						if (iIdx == -1)
// 						{
// 							continue;
// 						}
// 
// 						CNtlWorldSector* pNtlWorldSector = &pWFMgr->m_pSectors[vecNeighborSectorIndices.at(i)];
// 						for (int j = 0; j < (int)pNtlWorldSector->m_vecNtlPLEntityLightObject.size(); ++j)
// 						{
// 							CNtlPLLightObject* pNtlPLEntityTemp = ((CNtlPLLightObject*)pNtlWorldSector->m_vecNtlPLEntityLightObject.at(j));
// 							fCurDist = RwRealMAXVAL;
// 							if(pNtlPLEntityTemp->Pick(fCurDist))
// 							{
// 								if(fCurDist < fResultDist)
// 								{
// 									fResultDist		= fCurDist;
// 									pNtlPLEntity	= pNtlPLEntityTemp;
// 								}
// 							}
// 						}
// 
// 						for (int j = 0; j < (int)pNtlWorldSector->m_vecNtlPLEntityLightObjectDummy.size(); ++j)
// 						{
// 							CNtlPLLightObject* pNtlPLEntityTemp = ((CNtlPLLightObject*)pNtlWorldSector->m_vecNtlPLEntityLightObjectDummy.at(j));
// 							fCurDist = RwRealMAXVAL;
// 							if(pNtlPLEntityTemp->Pick(fCurDist))
// 							{
// 								if(fCurDist < fResultDist)
// 								{
// 									fResultDist		= fCurDist;
// 									pNtlPLEntity	= pNtlPLEntityTemp;
// 								}
// 							}
// 						}
// 					}
// 				}
// 			}
// 
// 			m_vecSelectedLightObject.clear();
// 			if (pNtlPLEntity)
// 			{
// 				m_vecSelectedLightObject.push_back(pNtlPLEntity);
// 			}
// 			dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pNtlPLEntity);
// 		}
// 
// 		if (m_vecSelectedLightObject.size())
// 		{
// 			CNtlWorldFieldManager*	pMgr				= dGETMGR();
// 			CNtlPLLightObject*		pPLEntitySelected	= (CNtlPLLightObject*)m_vecSelectedLightObject.front();
// 
// 			if (dOKEY(DIK_RETURN) && GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
// 			{
// 				dGETFRM()->m_pLightAndShadowPalette->CreateWorldLightByLightObject(pPLEntitySelected);
// 				return;
// 			}
// 
// 			if(dOKEY(DIK_DELETE))
// 			{
// 				//				if (pPLEntitySelected->IsRealLight())
// 				{
// 					RwInt32 iSectorIdx = pMgr->GetSectorIdx(pPLEntitySelected->GetPosition());
// 					if (iSectorIdx != -1)
// 					{
// 						pMgr->m_pSectors[iSectorIdx].DeleteEntity(pPLEntitySelected, 8);
// 						m_vecSelectedLightObject.clear();
// 						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(NULL);
// 					}
// 					GetSceneManager()->DeleteEntity(pPLEntitySelected);
// 					return;
// 				}
// 				// 				else
// 				// 				{
// 				// 					GetSceneManager()->DeleteEntity(pPLEntitySelected);
// 				// 					return;
// 				// 				}
// 			}
// 
// 			if(dKEY(DIK_LCONTROL) && dGET_MOUSE()->GetLBOnce() && m_vecSelectedLightObject.size() == 1)
// 			{
// 				if(dGET_BRUSH_ENTITY())
// 				{
// 					GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
// 					dGET_BRUSH_ENTITY() = NULL;
// 				}
// 
// 				if (dGETFRM()->m_pLightObjectPalette->GetCreateParam(m_vecSelectedLightObject.at(0)))
// 				{
// 					dGET_BRUSH_ENTITY() = GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, NULL, dGETFRM()->m_pLightObjectPalette->GetCreateParam(m_vecSelectedLightObject.at(0)));
// 					DBO_ASSERT(dGET_BRUSH_ENTITY(), "Entity create failed.");
// 
// 					dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(dGET_BRUSH_ENTITY());
// 
// 					m_vecSelectedLightObject.clear();
// 					return;
// 				}					
// 			}
// 
// 			if(dGET_MOUSE()->GetWheelMovement())
// 			{
// 				int iValue = static_cast<int>(dGET_MOUSE()->GetWheelMovement() / ::fabsf(dGET_MOUSE()->GetWheelMovement()));
// 
// 				if(dKEY(DIK_LSHIFT))
// 				{
// 					RwReal fRadius = pPLEntitySelected->GetRadius();
// 					fRadius += static_cast<RwReal>(iValue);
// 					CLAMP(fRadius, 0.5f, static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize));
// 					pPLEntitySelected->SetRadius(fRadius);
// 					dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 
// 				}
// 				else if (pPLEntitySelected->GetLightType() != rpLIGHTPOINT)
// 				{
// 					RwReal fAngle = pPLEntitySelected->GetAngle();
// 					fAngle += static_cast<RwReal>(iValue);
// 					CLAMP(fAngle, 0.0f, 90.0f);
// 					pPLEntitySelected->SetAngle(fAngle);
// 					dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 				}
// 			}
// 
// 			if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
// 			{
// 				if(dKEY(DIK_LSHIFT))
// 				{
// 					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
// 					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);
// 
// 					if(dKEY(DIK_Z))
// 					{
// 						RwV3d vPos = pPLEntitySelected->GetPosition();
// 						vPos.y -= DistY;
// 						pPLEntitySelected->SetPosition(&vPos);
// 						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 					}
// 					else
// 					{
// 						RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
// 						RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
// 						RwV3d *pRight			= RwMatrixGetRight(pM);
// 						RwV3d *pUp				= RwMatrixGetUp(pM);
// 						RwV3d *pAt				= RwMatrixGetAt(pM);
// 						RwV3d vFront, vRight;
// 						RwReal /*TerrainHeight, TotalHeight, */ObjHeight;
// 
// 						CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
// 						CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
// 						RwV3dNormalize(&vFront, &vFront);
// 						RwV3dNormalize(&vRight, &vRight);
// 
// 						RwV3d vPos = pPLEntitySelected->GetPosition();
// 						// delete from the previous field
// 						RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
// 						if(CurSectorIdx != -1)
// 						{
// 							pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pPLEntitySelected), 8);
// 						}
// 
// 						//TotalHeight		= dGET_POSITIVE_VALUE(vPos.y);
// 						//TerrainHeight	= dGET_POSITIVE_VALUE(pMgr->GetWorldSectorHeight(vPos));
// 						//ObjHeight		= TotalHeight - TerrainHeight;
// 						ObjHeight		= vPos.y;
// 
// 						// transform
// 						RwV3dIncrementScaled(&vPos, &vFront, -DistY);
// 						RwV3dIncrementScaled(&vPos, &vRight, -DistX);
// 
// 						//vPos.y = pMgr->GetWorldSectorHeight(vPos) + ObjHeight;
// 						vPos.y = ObjHeight;
// 
// 						pPLEntitySelected->SetPosition(&vPos);
// 						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 
// 						// create into the new field
// 						CurSectorIdx = pMgr->GetSectorIdx(vPos);
// 						if(CurSectorIdx != -1)
// 						{
// 							//CNtlWorldRpLightManager::GetInstance()->InsertIn2World(pPLEntitySelected);
// 							pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pPLEntitySelected), 8);
// 						}
// 					}
// 				}
// 				// Rot
// 				else if(dKEY(DIK_SPACE) && pPLEntitySelected->GetLightType() != rpLIGHTPOINT)
// 				{
// 					float DistX = dGET_MOUSE()->GetXMovementWithRate(1.0f);
// 					float DistY = dGET_MOUSE()->GetYMovementWithRate(1.0f);
// 
// 					if(dKEY(DIK_V))
// 					{
// 						RwV3d vRot = *pPLEntitySelected->GetRotate();
// 						vRot.x -= DistX;
// 						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
// 						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 					}
// 					else if(dKEY(DIK_N))
// 					{
// 						RwV3d vRot = *pPLEntitySelected->GetRotate();
// 						vRot.z -= DistX;
// 						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
// 						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 					}
// 					else
// 					{
// 						RwV3d vRot = *pPLEntitySelected->GetRotate();
// 						vRot.y -= DistX;
// 						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
// 						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 					}
// 				}
// 				// Scale
// 				else if(dKEY(DIK_RSHIFT))
// 				{
// 					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
// 					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);
// 
// 					RwReal fRadius = pPLEntitySelected->GetRadius();
// 					fRadius -= DistY;
// 					CLAMP(fRadius, 0.5f, dGET_WORLD_PARAM()->WorldSectorSize);
// 					pPLEntitySelected->SetRadius(fRadius);
// 					dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 				}
// 			}
// 
// 			// Rotation using Keyboard
// 			if(dKEY(DIK_SPACE))
// 			{
// 				if(dKEY(DIK_LSHIFT))
// 				{
// 					if(dOKEY(DIK_LEFTARROW))
// 					{
// 						RwV3d vRot = *pPLEntitySelected->GetRotate();
// 						vRot.y -= m_RotationTabValue;
// 						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
// 						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 					}
// 					else if(dOKEY(DIK_RIGHTARROW))
// 					{
// 						RwV3d vRot = *pPLEntitySelected->GetRotate();
// 						vRot.y += m_RotationTabValue;
// 						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
// 						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 					}
// 				}
// 				else
// 				{
// 					if(dKEY(DIK_LEFTARROW))
// 					{
// 						RwV3d vRot = *pPLEntitySelected->GetRotate();
// 						vRot.y -= m_RotationTabValue;
// 						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
// 						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 					}
// 					else if(dKEY(DIK_RIGHTARROW))
// 					{
// 						RwV3d vRot = *pPLEntitySelected->GetRotate();
// 						vRot.y += m_RotationTabValue;
// 						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
// 						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 					}
// 				}
// 			}
// 
// 			// translation keyboard
// 			if(dKEY(DIK_SPACE))
// 			{
// 				if(dKEY(DIK_LSHIFT))
// 				{
// 					if(dOKEY(DIK_UPARROW))
// 					{
// 						RwV3d vPos = pPLEntitySelected->GetPosition();
// 						vPos.y += m_fObjMovementSensitivity;
// 						pPLEntitySelected->SetPosition(&vPos);
// 						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 					}
// 					else if(dOKEY(DIK_DOWNARROW))
// 					{
// 						RwV3d vPos = pPLEntitySelected->GetPosition();
// 						vPos.y -= m_fObjMovementSensitivity;
// 						pPLEntitySelected->SetPosition(&vPos);
// 						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 					}
// 				}
// 				else
// 				{
// 					if(dKEY(DIK_UPARROW))
// 					{
// 						RwV3d vPos = pPLEntitySelected->GetPosition();
// 						vPos.y += m_fObjMovementSensitivity;
// 						pPLEntitySelected->SetPosition(&vPos);
// 						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 					}
// 					else if(dKEY(DIK_DOWNARROW))
// 					{
// 						RwV3d vPos = pPLEntitySelected->GetPosition();
// 						vPos.y -= m_fObjMovementSensitivity;
// 						pPLEntitySelected->SetPosition(&vPos);
// 						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 					}
// 				}
// 			}
// 
// 			if(!dKEY(DIK_SPACE))
// 			{
// 				if(dKEY(DIK_LEFTARROW) || dKEY(DIK_RIGHTARROW) || dKEY(DIK_UPARROW) || dKEY(DIK_DOWNARROW))
// 				{
// 					RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
// 					RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
// 					RwV3d *pRight			= RwMatrixGetRight(pM);
// 					RwV3d *pUp				= RwMatrixGetUp(pM);
// 					RwV3d *pAt				= RwMatrixGetAt(pM);
// 					RwV3d vFront, vRight;
// 
// 					CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
// 					CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
// 					RwV3dNormalize(&vFront, &vFront);
// 					RwV3dNormalize(&vRight, &vRight);
// 
// 					RwV3d vPos = pPLEntitySelected->GetPosition();
// 
// 					// delete from the previous field
// 					RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
// 					if(CurSectorIdx != -1)
// 					{
// 						pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pPLEntitySelected), 8);
// 					}
// 
// 					if(!dKEY(DIK_LSHIFT))
// 					{
// 						if(dKEY(DIK_LEFTARROW))
// 						{
// 							RwV3dIncrementScaled(&vPos, &vRight, m_fObjMovementSensitivity);
// 						}
// 						else if(dKEY(DIK_RIGHTARROW))
// 						{
// 							RwV3dIncrementScaled(&vPos, &vRight, -m_fObjMovementSensitivity);
// 						}
// 						else if(dKEY(DIK_UPARROW))
// 						{
// 							RwV3dIncrementScaled(&vPos, &vFront, m_fObjMovementSensitivity);
// 						}
// 						else if(dKEY(DIK_DOWNARROW))
// 						{
// 							RwV3dIncrementScaled(&vPos, &vFront, -m_fObjMovementSensitivity);
// 						}
// 					}
// 					else
// 					{
// 						if(dOKEY(DIK_LEFTARROW))
// 						{
// 							RwV3dIncrementScaled(&vPos, &vRight, m_fObjMovementSensitivity);
// 						}
// 						else if(dOKEY(DIK_RIGHTARROW))
// 						{
// 							RwV3dIncrementScaled(&vPos, &vRight, -m_fObjMovementSensitivity);
// 						}
// 						else if(dOKEY(DIK_UPARROW))
// 						{
// 							RwV3dIncrementScaled(&vPos, &vFront, m_fObjMovementSensitivity);
// 						}
// 						else if(dOKEY(DIK_DOWNARROW))
// 						{
// 							RwV3dIncrementScaled(&vPos, &vFront, -m_fObjMovementSensitivity);
// 						}
// 					}
// 
// 					pPLEntitySelected->SetPosition(&vPos);
// 					dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
// 
// 					// create into the new field
// 					CurSectorIdx = pMgr->GetSectorIdx(vPos);
// 					if(CurSectorIdx != -1)
// 					{
// 						//CNtlWorldRpLightManager::GetInstance()->InsertIn2World(pPLEntitySelected);
// 						pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pPLEntitySelected), 8);
// 					}
// 				}
// 			}
// 		}
// 	}
// }

VOID CWorldEditorEventHander::UpdateDecalVolume()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	if(dGET_BRUSH_ENTITY())
	{
		if(dOKEY(DIK_SPACE))
		{
			if(dGET_BRUSH_ENTITY())
			{
				dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(NULL);
				GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
				dGET_BRUSH_ENTITY() = NULL;
				return;
			}
		}

		// move entity
		sNPE_COLLISION_PARAM sNPECollisionParam;
		sNPECollisionParam._pNtlPLEntity = dGET_BRUSH_ENTITY();

		RwV3d vDist;
		vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
		vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
		vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;

		dGET_BRUSH_ENTITY()->SetPosition(&vDist);

		(static_cast<CNtlPLDecalVolume*>dGET_BRUSH_ENTITY())->RotatePickPlane();
		if (dGET_COLLISION_INFO()->IsCollidedAtSectors)
		{
			// place effects
			if(dGET_MOUSE()->GetLBOnce())
			{
				if(dGET_BRUSH_ENTITY())
				{
					RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vDist);
					if(CurSectorIdx != -1)
					{
						if (dGETFRM()->m_pDecalVolumePalette->GetCreateParam(dGET_BRUSH_ENTITY()))
						{
							CNtlPLEntity *pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_DECAL_VOLUME, NULL, dGETFRM()->m_pDecalVolumePalette->GetCreateParam(dGET_BRUSH_ENTITY()));
							DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

							pMgr->m_pSectors[CurSectorIdx].InsertIn2World(pNtlPLEntity, 10);
							return;
						}					
					}
				}
			}
		}
	}
	else
	{
		// picking
		if(dGET_MOUSE()->GetLBOnce() && !dKEY(DIK_LSHIFT) && !dKEY(DIK_RSHIFT) && !dKEY(DIK_SPACE))
		{
			RwReal					fCurDist;
			CNtlPLDecalVolume*		pNtlPLEntity	= NULL;
			RwReal					fResultDist		= RwRealMAXVAL;	

			RwV3d vCur;
			if (GetSceneManager()->GetWorld()->GetPlayerPosition(vCur))
			{
				CNtlWorldFieldManager*	pWFMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
				RwInt32					iSectorIdx	= pWFMgr->GetSectorIdx(vCur);

				if (iSectorIdx)
				{
					vector<RwInt32> vecNeighborSectorIndices;
					pWFMgr->GetNeighborSectors(iSectorIdx, vecNeighborSectorIndices);
					vecNeighborSectorIndices.push_back(iSectorIdx);

					for (int i = 0; i < (int)vecNeighborSectorIndices.size(); ++i)
					{
						RwInt32 iIdx = vecNeighborSectorIndices.at(i);
						if (iIdx == -1)
						{
							continue;
						}

						CNtlWorldSector* pNtlWorldSector = &pWFMgr->m_pSectors[vecNeighborSectorIndices.at(i)];
						for (int j = 0; j < (int)pNtlWorldSector->m_vecNtlPLEntityDecalVolume.size(); ++j)
						{
							CNtlPLDecalVolume* pNtlPLEntityTemp = ((CNtlPLDecalVolume*)pNtlWorldSector->m_vecNtlPLEntityDecalVolume.at(j));
							fCurDist = RwRealMAXVAL;
							if(pNtlPLEntityTemp->Pick(fCurDist))
							{
								if(fCurDist < fResultDist)
								{
									fResultDist		= fCurDist;
									pNtlPLEntity	= pNtlPLEntityTemp;
								}
							}
						}
					}
				}
			}

			dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pNtlPLEntity);
			m_vecSelectedDecalVolume.clear();
			if (pNtlPLEntity)
			{
				m_vecSelectedDecalVolume.push_back(pNtlPLEntity);
			}
		}

		if (m_vecSelectedDecalVolume.size())
		{
			CNtlWorldFieldManager*	pMgr				= dGETMGR();
			CNtlPLDecalVolume*		pPLEntitySelected	= (CNtlPLDecalVolume*)m_vecSelectedDecalVolume.front();

			if(dOKEY(DIK_DELETE))
			{
				RwInt32 iSectorIdx = pMgr->GetSectorIdx(pPLEntitySelected->GetPosition());
				if (iSectorIdx != -1)
				{
					pMgr->m_pSectors[iSectorIdx].DeleteEntity(pPLEntitySelected, 10);
					m_vecSelectedDecalVolume.clear();
					dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(NULL);
				}
				GetSceneManager()->DeleteEntity(pPLEntitySelected);
				return;
			}

			if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

					if(dKEY(DIK_Z))
					{
						RwV3d vPos = pPLEntitySelected->GetPosition();
						vPos.y -= DistY;
						pPLEntitySelected->SetPosition(&vPos);
						dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);
					}
					else
					{
						RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
						RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
						RwV3d *pRight			= RwMatrixGetRight(pM);
						RwV3d *pUp				= RwMatrixGetUp(pM);
						RwV3d *pAt				= RwMatrixGetAt(pM);
						RwV3d vFront, vRight;
						RwReal TerrainHeight, TotalHeight, ObjHeight;

						CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
						CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
						RwV3dNormalize(&vFront, &vFront);
						RwV3dNormalize(&vRight, &vRight);

						RwV3d vPos = pPLEntitySelected->GetPosition();
						// delete from the previous field
						RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
						if(CurSectorIdx != -1)
						{
							pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pPLEntitySelected), 10);
						}

						TotalHeight		= dGET_POSITIVE_VALUE(vPos.y);
						TerrainHeight	= dGET_POSITIVE_VALUE(pMgr->GetWorldSectorHeight(vPos));
						ObjHeight		= TotalHeight - TerrainHeight;

						// transform
						RwV3dIncrementScaled(&vPos, &vFront, -DistY);
						RwV3dIncrementScaled(&vPos, &vRight, -DistX);

						vPos.y = pMgr->GetWorldSectorHeight(vPos) + ObjHeight;
						pPLEntitySelected->SetPosition(&vPos);
						dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);

						// create into the new field
						CurSectorIdx = pMgr->GetSectorIdx(vPos);
						if(CurSectorIdx != -1)
						{
							pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pPLEntitySelected), 10);
						}
					}
				}
				// Rot
				else if(dKEY(DIK_SPACE))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(1.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(1.0f);

					if(dKEY(DIK_V))
					{
						RwV3d vRot = *pPLEntitySelected->GetRotate();
						vRot.x -= DistX;
						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);
					}
					else if(dKEY(DIK_N))
					{
						RwV3d vRot = *pPLEntitySelected->GetRotate();
						vRot.z -= DistX;
						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);
					}
					else
					{
						RwV3d vRot = *pPLEntitySelected->GetRotate();
						vRot.y -= DistX;
						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);
					}
				}
				// Scale
				else if(dKEY(DIK_RSHIFT))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

					RwV3d vScale = *pPLEntitySelected->GetScale();
					vScale.x -= DistY; vScale.y -= DistY; vScale.z -= DistY;
					pPLEntitySelected->SetScale(&vScale);
					dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);
				}
			}

			// Rotation using Keyboard
			if(dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					if(dOKEY(DIK_LEFTARROW))
					{
						RwV3d vRot = *pPLEntitySelected->GetRotate();
						vRot.y -= m_RotationTabValue;
						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);
					}
					else if(dOKEY(DIK_RIGHTARROW))
					{
						RwV3d vRot = *pPLEntitySelected->GetRotate();
						vRot.y += m_RotationTabValue;
						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);
					}
				}
				else
				{
					if(dKEY(DIK_LEFTARROW))
					{
						RwV3d vRot = *pPLEntitySelected->GetRotate();
						vRot.y -= m_RotationTabValue;
						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);
					}
					else if(dKEY(DIK_RIGHTARROW))
					{
						RwV3d vRot = *pPLEntitySelected->GetRotate();
						vRot.y += m_RotationTabValue;
						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);
					}
				}
			}

			// translation keyboard
			if(dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					if(dOKEY(DIK_UPARROW))
					{
						RwV3d vPos = pPLEntitySelected->GetPosition();
						vPos.y += m_fObjMovementSensitivity;
						pPLEntitySelected->SetPosition(&vPos);
						dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);
					}
					else if(dOKEY(DIK_DOWNARROW))
					{
						RwV3d vPos = pPLEntitySelected->GetPosition();
						vPos.y -= m_fObjMovementSensitivity;
						pPLEntitySelected->SetPosition(&vPos);
						dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);
					}
				}
				else
				{
					if(dKEY(DIK_UPARROW))
					{
						RwV3d vPos = pPLEntitySelected->GetPosition();
						vPos.y += m_fObjMovementSensitivity;
						pPLEntitySelected->SetPosition(&vPos);
						dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);
					}
					else if(dKEY(DIK_DOWNARROW))
					{
						RwV3d vPos = pPLEntitySelected->GetPosition();
						vPos.y -= m_fObjMovementSensitivity;
						pPLEntitySelected->SetPosition(&vPos);
						dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);
					}
				}
			}

			if(!dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LEFTARROW) || dKEY(DIK_RIGHTARROW) || dKEY(DIK_UPARROW) || dKEY(DIK_DOWNARROW))
				{
					RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
					RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
					RwV3d *pRight			= RwMatrixGetRight(pM);
					RwV3d *pUp				= RwMatrixGetUp(pM);
					RwV3d *pAt				= RwMatrixGetAt(pM);
					RwV3d vFront, vRight;

					CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
					CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
					RwV3dNormalize(&vFront, &vFront);
					RwV3dNormalize(&vRight, &vRight);

					RwV3d vPos = pPLEntitySelected->GetPosition();

					// delete from the previous field
					RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
					if(CurSectorIdx != -1)
					{
						pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pPLEntitySelected), 10);
					}

					if(!dKEY(DIK_LSHIFT))
					{
						if(dKEY(DIK_LEFTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, m_fObjMovementSensitivity);
						}
						else if(dKEY(DIK_RIGHTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, -m_fObjMovementSensitivity);
						}
						else if(dKEY(DIK_UPARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, m_fObjMovementSensitivity);
						}
						else if(dKEY(DIK_DOWNARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, -m_fObjMovementSensitivity);
						}
					}
					else
					{
						if(dOKEY(DIK_LEFTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, m_fObjMovementSensitivity);
						}
						else if(dOKEY(DIK_RIGHTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, -m_fObjMovementSensitivity);
						}
						else if(dOKEY(DIK_UPARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, m_fObjMovementSensitivity);
						}
						else if(dOKEY(DIK_DOWNARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, -m_fObjMovementSensitivity);
						}
					}

					pPLEntitySelected->SetPosition(&vPos);
					dGETFRM()->m_pDecalVolumePalette->UpdatePropertyData(pPLEntitySelected);

					// create into the new field
					CurSectorIdx = pMgr->GetSectorIdx(vPos);
					if(CurSectorIdx != -1)
					{
						pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pPLEntitySelected), 10);
					}
				}
			}
		}
	}
}

// CZ-SS
// VOID CWorldEditorEventHander::UpdateSoftSpot()
// {
// 	CNtlWorldFieldManager *pMgr = dGETMGR();
// 
// 	if(dGET_BRUSH_ENTITY())
// 	{
// 		m_SelectedSS = NULL;
// 
// 		// cancel entity
// 		if(dOKEY(DIK_SPACE))
// 		{
// 			if(dGET_BRUSH_ENTITY())
// 			{
// 				GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
// 				dGET_BRUSH_ENTITY() = NULL;
// 				return;
// 			}
// 		}
// 
// 		// move entity
// 		sNPE_COLLISION_PARAM sNPECollisionParam;
// 		sNPECollisionParam._pNtlPLEntity = dGET_BRUSH_ENTITY();
// 
// 		RwV3d vDist;
// 		vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
// 		vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y + dDUMMY_HEIGHT;
// 		vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;
// 		dGET_BRUSH_ENTITY()->SetPosition(&vDist);
// 
// 		// place entity
// 		if(dGET_MOUSE()->GetLBOnce())
// 		{
// 			if(dGET_BRUSH_ENTITY())
// 			{
// 				RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vDist);
// 				if(CurSectorIdx != -1)
// 				{
// 					SPLEntityCreateParam CreateParam;
// 					CreateParam.pPos = &vDist;
// 
// 					CNtlPLEntity*	pNtlPLEntity	= GetSceneManager()->CreateEntity(PLENTITY_SPOT_LIGHT, dGET_BRUSH_ENTITY()->GetName(), &CreateParam);
// 					DBO_ASSERT(pNtlPLEntity, "Entity create failed.");
// 
// 					CNtlPLSpot*		pSpotEntity		= static_cast<CNtlPLSpot*>(pNtlPLEntity);
// 					CNtlPLSpot*		pSpotEntityM	= static_cast<CNtlPLSpot*>(dGET_BRUSH_ENTITY());
// 					pSpotEntity->SetColor(pSpotEntityM->GetColor());
// 					pSpotEntity->SetAngle(pSpotEntityM->GetAngle());
// 					pSpotEntity->SetRadius(pSpotEntityM->GetRadius());
// 
// 					pMgr->m_pSectors[CurSectorIdx].InsertIn2World(pNtlPLEntity, 3);
// 				}
// 			}
// 		}
// 
// 		// adjust angle & radius only for mouse entity
// 		int Value = 0;
// 
// 		if(dGET_MOUSE()->GetWheelMovement())
// 		{
// 			Value = static_cast<int>(dGET_MOUSE()->GetWheelMovement() / ::fabsf(dGET_MOUSE()->GetWheelMovement()));
// 
// 			if(dKEY(DIK_LSHIFT))
// 			{
// 				CNtlPLSpot* pPLSpot = static_cast<CNtlPLSpot*>(dGET_BRUSH_ENTITY());
// 				pPLSpot->SetRadius(pPLSpot->GetRadius() + static_cast<RwReal>(Value));
// 			}
// 			else
// 			{
// 				CNtlPLSpot* pPLSpot = static_cast<CNtlPLSpot*>(dGET_BRUSH_ENTITY());
// 				pPLSpot->SetAngle(pPLSpot->GetAngle() + static_cast<RwReal>(Value));
// 			}
// 
// 			dGETFRM()->m_pSoftSpotPalette->RefreshProp(static_cast<CNtlPLSpot*>(dGET_BRUSH_ENTITY()));
// 		}
// 	}
// 	else
// 	{
// 		// picking
// 		if(dGET_MOUSE()->GetLBOnce() && !dKEY(DIK_LSHIFT) && !dKEY(DIK_RSHIFT) && !dKEY(DIK_SPACE))
// 		{
// 			RwInt32					CurFieldIdx;
// 			RwInt32					CurSectorIdx;
// 			CNtlWorldFieldManager*	pMgr	= dGETMGR();
// 			CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
// 			RwV3d					SectorPos;
// 			RwReal					ResultDist = RwRealMAXVAL;	
// 			RwReal					CurDist;
// 			CNtlPLSpot*				pPickedEntity = NULL;
// 			RwV3d					SPos;
// 
// 			for(RwUInt32 i = 0; i < 36; ++i)
// 			{
// 				CurFieldIdx = pMgr->m_Fields6x6[1][i];
// 				SPos		= pFields[CurFieldIdx].GetSPos();
// 
// 				for(int l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
// 				{
// 					for(int m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
// 					{
// 						SectorPos.x = (RwReal)m;
// 						SectorPos.z = (RwReal)l;
// 						CurSectorIdx = pMgr->CNtlWorldFieldManager::GetSectorIdx(SectorPos);
// 
// 						if(CurSectorIdx == -1)
// 							continue;
// 
// 						CNtlWorldSector* pCurSector = &pMgr->CNtlWorldSectorManager::m_pSectors[CurSectorIdx];
// 
// 						for(RwUInt32 z = 0; z < pCurSector->m_vecNtlPLEntitySS.size(); ++z)
// 						{
// 							CNtlPLSpot* pEntity = static_cast<CNtlPLSpot*>(pCurSector->m_vecNtlPLEntitySS[z]);
// 							if(pEntity->Pick(CurDist))
// 							{
// 								if(CurDist < ResultDist)
// 								{
// 									ResultDist		= CurDist;
// 									pPickedEntity	= pEntity;
// 								}
// 							}
// 						}
// 					}
// 				}
// 			}
// 
// 			// if picked,
// 			if(pPickedEntity)
// 			{
// 				m_SelectedSS = pPickedEntity;
// 				dGETFRM()->m_pSoftSpotPalette->RefreshProp(m_SelectedSS);
// 				CNtlPLSpot::SetToolUI(m_SelectedSS);
// 			}
// 			else
// 			{
// 				m_SelectedSS = NULL;
// 				CNtlPLSpot::SetToolUI(NULL);
// 			}
// 		}
// 
// 		// editing mode
// 		if(m_SelectedSS)
// 		{
// 			if(dOKEY(DIK_DELETE))
// 			{
// 				RwInt32					CurFieldIdx;
// 				RwInt32					CurSectorIdx;
// 				CNtlWorldFieldManager*	pMgr	= dGETMGR();
// 				CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
// 				RwV3d					SectorPos;
// 				RwReal					ResultDist = RwRealMAXVAL;
// 				CNtlPLSpot*				pPickedEntity = NULL;
// 				RwV3d					SPos;
// 				RwBool					BreakFlag = FALSE;
// 
// 				for(RwUInt32 i = 0; i < 36; ++i)
// 				{
// 					CurFieldIdx = pMgr->m_Fields6x6[1][i];
// 					SPos		= pFields[CurFieldIdx].GetSPos();
// 
// 					for(int l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
// 					{
// 						for(int m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
// 						{
// 							SectorPos.x = (RwReal)m;
// 							SectorPos.z = (RwReal)l;
// 							CurSectorIdx = pMgr->CNtlWorldFieldManager::GetSectorIdx(SectorPos);
// 
// 							if(CurSectorIdx == -1)
// 								continue;
// 
// 							CNtlWorldSector* pCurSector = &pMgr->CNtlWorldSectorManager::m_pSectors[CurSectorIdx];
// 
// 							for(RwUInt32 z = 0; z < pCurSector->m_vecNtlPLEntitySS.size(); ++z)
// 							{
// 								CNtlPLSpot* pEntity = static_cast<CNtlPLSpot*>(pCurSector->m_vecNtlPLEntitySS[z]);
// 								if(pEntity == m_SelectedSS)
// 								{
// 									pMgr->m_pSectors[CurSectorIdx].DeleteEntity(m_SelectedSS, 3);
// 									GetSceneManager()->DeleteEntity(static_cast<CNtlPLEntity*>(m_SelectedSS));
// 									m_SelectedSS = NULL;
// 									BreakFlag = TRUE;
// 									break;
// 								}
// 							}
// 						}
// 
// 						if(BreakFlag)
// 						{
// 							break;
// 						}
// 					}
// 
// 					if(BreakFlag)
// 					{
// 						break;
// 					}
// 				}
// 			}
// 
// 			// adjust angle & radius
// 			int Value = 0;
// 
// 			if(dGET_MOUSE()->GetWheelMovement())
// 			{
// 				Value = static_cast<int>(dGET_MOUSE()->GetWheelMovement() / ::fabsf(dGET_MOUSE()->GetWheelMovement()));
// 
// 				if(dKEY(DIK_LSHIFT))
// 				{
// 					m_SelectedSS->SetRadius(m_SelectedSS->GetRadius() + static_cast<RwReal>(Value));
// 				}
// 				else
// 				{
// 					m_SelectedSS->SetAngle(m_SelectedSS->GetAngle() + static_cast<RwReal>(Value));
// 				}
// 
// 				dGETFRM()->m_pSoftSpotPalette->RefreshProp(m_SelectedSS);
// 			}
// 
// 			// transform
// 			if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
// 			{
// 				// translate
// 				if(dKEY(DIK_LSHIFT))
// 				{
// 					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
// 					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);
// 
// 					if(dKEY(DIK_Z))
// 					{
// 						RwV3d PosCur = m_SelectedSS->GetPosition();
// 						PosCur.y -= DistY;
// 						m_SelectedSS->SetPosition(&PosCur);
// 					}
// 					else
// 					{
// 						RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
// 						RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
// 						RwV3d *pRight			= RwMatrixGetRight(pM);
// 						RwV3d *pUp				= RwMatrixGetUp(pM);
// 						RwV3d *pAt				= RwMatrixGetAt(pM);
// 						RwV3d vFront, vRight;
// 						RwReal TerrainHeight, TotalHeight, ObjHeight;
// 
// 						CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
// 						CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
// 						RwV3dNormalize(&vFront, &vFront);
// 						RwV3dNormalize(&vRight, &vRight);
// 
// 						RwV3d PosCur = m_SelectedSS->GetPosition();
// 
// 						// check valid soft spot sector
// 						RwV3d PosCheck = PosCur;
// 						RwV3dIncrementScaled(&PosCheck, &vFront, -DistY);
// 						RwV3dIncrementScaled(&PosCheck, &vRight, -DistX);
// 						if(!IsValidSoftSpot(PosCheck, TRUE))
// 						{
// 							return;
// 						}
// 
// 						// delete from the previous field
// 						RwInt32 CurSectorIdx = pMgr->GetSectorIdx(PosCur);
// 						if(CurSectorIdx != -1)
// 						{
// 							pMgr->m_pSectors[CurSectorIdx].DeleteEntity(m_SelectedSS, 3);
// 						}
// 
// 						TotalHeight		= dGET_POSITIVE_VALUE(PosCur.y);
// 						TerrainHeight	= dGET_POSITIVE_VALUE(pMgr->GetWorldSectorHeight(PosCur));
// 						ObjHeight		= TotalHeight - TerrainHeight;
// 
// 						// transform
// 						RwV3dIncrementScaled(&PosCur, &vFront, -DistY);
// 						RwV3dIncrementScaled(&PosCur, &vRight, -DistX);
// 
// 						PosCur.y = pMgr->GetWorldSectorHeight(PosCur) + ObjHeight;
// 						m_SelectedSS->SetPosition(&PosCur);
// 
// 						// create into the new field
// 						CurSectorIdx = pMgr->GetSectorIdx(PosCur);
// 						if(CurSectorIdx != -1)
// 						{
// 							pMgr->m_pSectors[CurSectorIdx].InsertIn2World(m_SelectedSS, 3);
// 						}
// 					}
// 
// 					dGETFRM()->m_pSoftSpotPalette->RefreshProp(m_SelectedSS);
// 				}
// 
// 				// rotate
// 				if(dKEY(DIK_SPACE))
// 				{
// 					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
// 					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);
// 
// 					if(dKEY(DIK_V))
// 					{
// 						const RwV3d *pRot = m_SelectedSS->GetRotate();
// 						m_SelectedSS->SetRotate(pRot->x - DistX, pRot->y, pRot->z);
// 					}
// 					else if(dKEY(DIK_N))
// 					{
// 						const RwV3d *pRot = m_SelectedSS->GetRotate();
// 						m_SelectedSS->SetRotate(pRot->x, pRot->y, pRot->z - DistX);
// 					}
// 					else
// 					{
// 						const RwV3d *pRot = m_SelectedSS->GetRotate();
// 						m_SelectedSS->SetRotate(pRot->x, pRot->y - DistX, pRot->z);
// 					}
// 
// 					dGETFRM()->m_pSoftSpotPalette->RefreshProp(m_SelectedSS);
// 				}
// 			}
// 		}
// 	}
// }

VOID CWorldEditorEventHander::UpdatePerformaceSectorCull()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	// clear all shadow sector's display obj.
	if(dOKEY(DIK_SPACE))
	{
		if(m_vecSelectedSectorCullObj.size())
		{
			for(unsigned int p = 0; p < m_vecSelectedSectorCullObj.size(); ++p)
			{
				GetSceneManager()->DeleteEntity(m_vecSelectedSectorCullObj[p]);
			}
			m_vecSelectedSectorCullObj.clear();
		}
	}

	if(dGET_MOUSE()->GetLBUp())
	{
		if(m_pBrushSelector)
		{
			CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());

			RwV3d OldPosS = pCurBrush->GetPosS();
			RwV3d PosS = pCurBrush->GetPosS();
			RwV3d PosE = pCurBrush->GetPosE();
			RwV3d FPosS;
			RwV3d FPosE;

			if(PosS.x <= PosE.x)
			{
				FPosS.x = PosS.x;
				FPosE.x = PosE.x;
			}
			else
			{
				FPosS.x = PosE.x;
				FPosE.x = PosS.x;
			}

			if(PosS.z <= PosE.z)
			{
				FPosS.z = PosS.z;
				FPosE.z = PosE.z;
			}
			else
			{
				FPosS.z = PosE.z;
				FPosE.z = PosS.z;
			}

			PosS = FPosS;
			PosE = FPosE;

			vector<RwInt32> vecTmpSectorIdx;

			for(RwReal i = PosS.z; i < PosE.z; i += dGET_WORLD_PARAM()->WorldSectorTileSize)
			{
				for(RwReal j = PosS.x; j < PosE.x; j += dGET_WORLD_PARAM()->WorldSectorTileSize)
				{
					RwV3d TmpPos;
					TmpPos.x = j;
					TmpPos.z = i;
					TmpPos.y = pMgr->GetWorldSectorHeight(TmpPos);
					unsigned int Idx = static_cast<unsigned int>(pMgr->GetSectorIdx(TmpPos));

					RwBool Flag = FALSE;
					for(unsigned int b = 0; b < vecTmpSectorIdx.size(); ++b)
					{
						if(vecTmpSectorIdx[b] == Idx)
						{
							Flag = TRUE;
							break;
						}
					}

					if(!Flag)
					{
						vecTmpSectorIdx.push_back(Idx);
					}						
				}
			}

			for(unsigned int k = 0; k < vecTmpSectorIdx.size(); ++k)
			{
				RwV3d TmpPos;
				TmpPos.x = pMgr->CNtlWorldSectorManager::m_pSectors[vecTmpSectorIdx[k]].DatumPoint.x;
				TmpPos.z = pMgr->CNtlWorldSectorManager::m_pSectors[vecTmpSectorIdx[k]].DatumPoint.z;
				TmpPos.y = pMgr->GetWorldSectorHeight(TmpPos);

				SPLEntityCreateParam PLEntityCreateParam;
				PLEntityCreateParam.pPos = &TmpPos;

				CNtlPLObject *pNtlPLObject = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "TEXT_ZERO", &PLEntityCreateParam));						
				DBO_ASSERT(pNtlPLObject, "Entity create failed.");

				pNtlPLObject->SetFadeEnable(FALSE);
				m_vecSelectedSectorCullObj.push_back(pNtlPLObject);
			}

			// display
			dGETFRM()->m_pPerformanceSectorCull->Refresh();

			vecTmpSectorIdx.clear();

			NTL_DELETE(m_pBrushSelector);
		}
	}

	if(dGET_MOUSE()->GetLBOnce())
	{
		if(!m_pBrushSelector)
		{
			for(unsigned int p = 0; p < m_vecSelectedSectorCullObj.size(); ++p)
			{
				GetSceneManager()->DeleteEntity(m_vecSelectedSectorCullObj[p]);
			}
			m_vecSelectedSectorCullObj.clear();

			m_pBrushSelector = NTL_NEW CNtlWorldBrush;

			CNtlWorldBrushController *pController = new CNtlWB_Rectangle<CNtlWorldFieldManager>(eNBT_RECTANGLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
			m_pBrushSelector->SetController(pController);

			CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
			pCurBrush->SetPosS(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
		}
	}

	if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
	{
		CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
		pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
	}
}

VOID CWorldEditorEventHander::UpdatePerformaceOcclusion()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	if(dGET_BRUSH_ENTITY())
	{
		m_pSelectedOccluder = NULL;

		// cancel entity
		if(dOKEY(DIK_SPACE))
		{
			if(dGET_BRUSH_ENTITY())
			{
				GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
				dGET_BRUSH_ENTITY() = NULL;
				dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(NULL);
				return;
			}
		}

		RwV3d vDist;
		vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
		vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
		vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;
		dGET_BRUSH_ENTITY()->SetPosition(&vDist);

		// place effects
		if(dGET_MOUSE()->GetLBOnce())
		{
			if(dGET_BRUSH_ENTITY())
			{
				RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vDist);
				if(CurSectorIdx != -1)
				{
					dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(dGET_BRUSH_ENTITY());		
					if (dGETFRM()->m_pPerformanceOcclusion->GetCreateParam())
					{
						CNtlPLEntity *pNtlPLEntity	= GetSceneManager()->CreateEntity(PLENTITY_OCCLUDER, NULL, dGETFRM()->m_pPerformanceOcclusion->GetCreateParam());
						DBO_ASSERT(pNtlPLEntity, "Entity create failed.");
						
						pMgr->m_pSectors[CurSectorIdx].InsertIn2World(pNtlPLEntity, 6);
						return;
					}					
				}
			}
		}
	}
	// capture mode; picking
	else if(!dGET_BRUSH_ENTITY())
	{
		RwReal					fDistResult		= RwRealMAXVAL;
		RwInt32					iIndexResult	= -1;
		CNtlPLOccluder_Base*	pPickedOccluder	= NULL;

		if(dGET_MOUSE()->GetLBOnce() && !dKEY(DIK_LSHIFT) && !dKEY(DIK_RSHIFT) && !dKEY(DIK_SPACE))
		{
			for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
			{
				RwInt32 NumObj = static_cast<RwInt32>(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityOccluder_Quad.size());
				for(int k = 0; k < NumObj; ++k)
				{
					RwReal	fDist;
					RwInt32	iIndex;
					CNtlPLOccluder_Base* pOccluderCurrent = (CNtlPLOccluder_Base*)pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityOccluder_Quad[k];
					if(pOccluderCurrent->Pick(&fDist, &iIndex))
					{
						if(fDist < fDistResult)
						{
							fDistResult		= fDist;
							iIndexResult	= iIndex;
							pPickedOccluder	= pOccluderCurrent;
						}
					}
				}	
			}
			// if picked,
			if(pPickedOccluder)
			{
				if(m_pSelectedOccluder)
				{
					m_pSelectedOccluder->SetToolUI(FALSE);
					m_pSelectedOccluder = NULL;
				}

				m_pSelectedOccluder = pPickedOccluder;
				m_pSelectedOccluder->SetToolUI(TRUE, iIndexResult);
				dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(m_pSelectedOccluder);
			}
			else
			{
				if(m_pSelectedOccluder)
				{
					m_pSelectedOccluder->SetToolUI(FALSE, iIndexResult);
					m_pSelectedOccluder = NULL;
					dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(NULL);
				}
			}
		}

		// editing mode
		if(m_pSelectedOccluder)
		{
			CNtlPLOccluder_Quad* pOccluderQuad = NULL;

			switch (m_pSelectedOccluder->GetOccluderType())
			{
			case EPLOCCLUDER_QUAD:
				pOccluderQuad = (CNtlPLOccluder_Quad*)m_pSelectedOccluder;
				break;
			}

			if(dOKEY(DIK_DELETE))
			{
				RwBool BreakFlag = FALSE;

				for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
				{
					if (pOccluderQuad)
					{
						RwInt32 NumObj = static_cast<RwInt32>(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityOccluder_Quad.size());
						for(int k = 0; k < NumObj; ++k)
						{
							if(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityOccluder_Quad[k] == pOccluderQuad)
							{
								pMgr->m_pSectors[g_vecVisibleSectors[j]].DeleteEntity(pOccluderQuad, 6);
								GetSceneManager()->DeleteEntity(pOccluderQuad);

								m_pSelectedOccluder = NULL;
								dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(NULL);
								return;
							}
						}
					}
				}
			}

			if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

					if (pOccluderQuad && pOccluderQuad->m_iIndexPicked >= 0)
					{
						RwFrame*	pfrmCamera	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
						RwMatrix*	pmatCamera	= RwFrameGetMatrix(pfrmCamera);

						RwV3d*		pvUp		= RwMatrixGetUp(pmatCamera);
						RwV3d*		pvRight		= RwMatrixGetRight(pmatCamera);

						pOccluderQuad->m_sOccluderSwap.vTrans[pOccluderQuad->m_iIndexPicked].x += RwV3dLength(pvRight) * -DistX * 0.1f;
						pOccluderQuad->m_sOccluderSwap.vTrans[pOccluderQuad->m_iIndexPicked].y += RwV3dLength(pvUp)* -DistY * 0.1f;

						pOccluderQuad->CalcOccluderData();
					}
					else if(dKEY(DIK_Z))
					{
						if (m_pSelectedOccluder)
						{
							RwV3d vPos = m_pSelectedOccluder->GetPosition();
							vPos.y -= DistY;
							m_pSelectedOccluder->SetPosition(&vPos);
							dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(m_pSelectedOccluder);
						}
					}
					else
					{
						RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
						RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
						RwV3d *pRight			= RwMatrixGetRight(pM);
						RwV3d *pUp				= RwMatrixGetUp(pM);
						RwV3d *pAt				= RwMatrixGetAt(pM);
						RwV3d vFront, vRight;
						RwReal TerrainHeight, TotalHeight, ObjHeight;

						CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
						CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
						RwV3dNormalize(&vFront, &vFront);
						RwV3dNormalize(&vRight, &vRight);

						RwV3d vPos = m_pSelectedOccluder->GetPosition();
						// delete from the previous field
						RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
						if(CurSectorIdx != -1)
						{
							pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(m_pSelectedOccluder), 6);
						}

						TotalHeight		= dGET_POSITIVE_VALUE(vPos.y);
						TerrainHeight	= dGET_POSITIVE_VALUE(pMgr->GetWorldSectorHeight(vPos));
						ObjHeight		= TotalHeight - TerrainHeight;

						// transform
						RwV3dIncrementScaled(&vPos, &vFront, -DistY);
						RwV3dIncrementScaled(&vPos, &vRight, -DistX);

						vPos.y = pMgr->GetWorldSectorHeight(vPos) + ObjHeight;
						m_pSelectedOccluder->SetPosition(&vPos);
						dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(m_pSelectedOccluder);

						// create into the new field
						CurSectorIdx = pMgr->GetSectorIdx(vPos);
						if(CurSectorIdx != -1)
						{
							pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(m_pSelectedOccluder), 6);
						}
					}
				}
				// Rot
				else if(dKEY(DIK_SPACE))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(1.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(1.0f);



					if(dKEY(DIK_V))
					{						
						// 						if (pOccluderQuad)
						// 						{							
						// 							pOccluderQuad->m_sOccluderSwap.vRotate.x -= DistX;
						// 							pOccluderQuad->CalcOccluderData();
						// 							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pOccluderQuad);
						// 						}
					}
					else if(dKEY(DIK_N))
					{
						// 						if (pOccluderQuad)
						// 						{							
						// 							pOccluderQuad->m_sOccluderSwap.vRotate.z -= DistX;
						// 							pOccluderQuad->CalcOccluderData();
						// 							dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pOccluderQuad);
						// 						}
					}
					else
					{
						if (pOccluderQuad)
						{							
							pOccluderQuad->m_sOccluderSwap.vRotate.y -= DistX;
							pOccluderQuad->CalcOccluderData();
							dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(pOccluderQuad);
						}						
					}
				}
				// Scale
				else if(dKEY(DIK_RSHIFT))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

					if (pOccluderQuad)
					{
						pOccluderQuad->m_sOccluderSwap.vScale.x -= DistY;
						pOccluderQuad->m_sOccluderSwap.vScale.y -= DistY;
						pOccluderQuad->m_sOccluderSwap.vScale.z -= DistY;
						pOccluderQuad->CalcOccluderData();
						dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(pOccluderQuad);
					}
				}
			}

			// Rotation using Keyboard
			if(dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					if(dOKEY(DIK_LEFTARROW))
					{
						if (pOccluderQuad)
						{
							pOccluderQuad->m_sOccluderSwap.vRotate.y += -m_RotationTabValue;
							pOccluderQuad->CalcOccluderData();
							dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(pOccluderQuad);
						}
					}
					else if(dOKEY(DIK_RIGHTARROW))
					{
						if (pOccluderQuad)
						{
							pOccluderQuad->m_sOccluderSwap.vRotate.y += m_RotationTabValue;
							pOccluderQuad->CalcOccluderData();
							dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(pOccluderQuad);
						}
					}
				}
				else
				{
					if(dKEY(DIK_LEFTARROW))
					{
						if (pOccluderQuad)
						{
							pOccluderQuad->m_sOccluderSwap.vRotate.y += -m_RotationTabValue;
							pOccluderQuad->CalcOccluderData();
							dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(pOccluderQuad);
						}
					}
					else if(dKEY(DIK_RIGHTARROW))
					{
						if (pOccluderQuad)
						{
							pOccluderQuad->m_sOccluderSwap.vRotate.y += m_RotationTabValue;
							pOccluderQuad->CalcOccluderData();
							dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(pOccluderQuad);
						}
					}
				}
			}

			// translation keyboard
			if(dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					if(dOKEY(DIK_UPARROW))
					{
						if (pOccluderQuad)
						{
							RwV3d vPos = pOccluderQuad->GetPosition();
							vPos.y += m_fObjMovementSensitivity;
							pOccluderQuad->SetPosition(&vPos);							
							dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(pOccluderQuad);
						}
					}
					else if(dOKEY(DIK_DOWNARROW))
					{
						if (pOccluderQuad)
						{
							RwV3d vPos = pOccluderQuad->GetPosition();
							vPos.y -= m_fObjMovementSensitivity;
							pOccluderQuad->SetPosition(&vPos);
							dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(pOccluderQuad);
						}
					}
				}
				else
				{
					if(dKEY(DIK_UPARROW))
					{
						if (pOccluderQuad)
						{
							RwV3d vPos = pOccluderQuad->GetPosition();
							vPos.y += m_fObjMovementSensitivity;
							pOccluderQuad->SetPosition(&vPos);
							dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(pOccluderQuad);
						}
					}
					else if(dKEY(DIK_DOWNARROW))
					{
						if (pOccluderQuad)
						{
							RwV3d vPos = pOccluderQuad->GetPosition();
							vPos.y -= m_fObjMovementSensitivity;
							pOccluderQuad->SetPosition(&vPos);
							dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(pOccluderQuad);
						}
					}
				}
			}

			if(!dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LEFTARROW) || dKEY(DIK_RIGHTARROW) || dKEY(DIK_UPARROW) || dKEY(DIK_DOWNARROW))
				{
					if (pOccluderQuad)
					{
						RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
						RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
						RwV3d *pRight			= RwMatrixGetRight(pM);
						RwV3d *pUp				= RwMatrixGetUp(pM);
						RwV3d *pAt				= RwMatrixGetAt(pM);
						RwV3d vFront, vRight;

						CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
						CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
						RwV3dNormalize(&vFront, &vFront);
						RwV3dNormalize(&vRight, &vRight);

						RwV3d vPos = pOccluderQuad->GetPosition();

						// delete from the previous field
						RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
						if(CurSectorIdx != -1)
						{
							pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pOccluderQuad), 6);
						}

						if(!dKEY(DIK_LSHIFT))
						{
							if(dKEY(DIK_LEFTARROW))
							{
								RwV3dIncrementScaled(&vPos, &vRight, m_fObjMovementSensitivity);
							}
							else if(dKEY(DIK_RIGHTARROW))
							{
								RwV3dIncrementScaled(&vPos, &vRight, -m_fObjMovementSensitivity);
							}
							else if(dKEY(DIK_UPARROW))
							{
								RwV3dIncrementScaled(&vPos, &vFront, m_fObjMovementSensitivity);
							}
							else if(dKEY(DIK_DOWNARROW))
							{
								RwV3dIncrementScaled(&vPos, &vFront, -m_fObjMovementSensitivity);
							}
						}
						else
						{
							if(dOKEY(DIK_LEFTARROW))
							{
								RwV3dIncrementScaled(&vPos, &vRight, m_fObjMovementSensitivity);
							}
							else if(dOKEY(DIK_RIGHTARROW))
							{
								RwV3dIncrementScaled(&vPos, &vRight, -m_fObjMovementSensitivity);
							}
							else if(dOKEY(DIK_UPARROW))
							{
								RwV3dIncrementScaled(&vPos, &vFront, m_fObjMovementSensitivity);
							}
							else if(dOKEY(DIK_DOWNARROW))
							{
								RwV3dIncrementScaled(&vPos, &vFront, -m_fObjMovementSensitivity);
							}
						}

						pOccluderQuad->SetPosition(&vPos);
						dGETFRM()->m_pPerformanceOcclusion->UpdatePropertyData(pOccluderQuad);

						// create into the new field
						CurSectorIdx = pMgr->GetSectorIdx(vPos);
						if(CurSectorIdx != -1)
						{
							pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pOccluderQuad), 6);
						}
					}
				}
			}
		}
	}
}

VOID CWorldEditorEventHander::UpdateHeatHaze()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	if(dGET_BRUSH_ENTITY())
	{
		m_pSelectedHeatHaze = NULL;

		// cancel entity
		if(dOKEY(DIK_SPACE))
		{
			if(dGET_BRUSH_ENTITY())
			{
				GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
				dGET_BRUSH_ENTITY() = NULL;
				dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(NULL);
				return;
			}
		}

		RwV3d vDist;
		vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
		vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
		vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;
		dGET_BRUSH_ENTITY()->SetPosition(&vDist);

		// place effects
		if(dGET_MOUSE()->GetLBOnce())
		{
			if(dGET_BRUSH_ENTITY())
			{
				RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vDist);
				if(CurSectorIdx != -1)
				{

					m_pSelectedHeatHaze			= static_cast<CNtlPLHeatHazeObject*>(dGET_BRUSH_ENTITY());
					dGET_BRUSH_ENTITY()			= NULL;

					dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);
					m_pSelectedHeatHaze->SetToolUI(TRUE);

					pMgr->m_pSectors[CurSectorIdx].InsertIn2World(m_pSelectedHeatHaze, 7);
					return;
				}
			}
		}
	}
	// capture mode; picking
	else if(!dGET_BRUSH_ENTITY())
	{
		RwReal					fDistResult		= RwRealMAXVAL;
		CNtlPLHeatHazeObject*	pEntityPicked	= NULL;

		if(dGET_MOUSE()->GetLBOnce() && !dKEY(DIK_LSHIFT) && !dKEY(DIK_RSHIFT) && !dKEY(DIK_SPACE))
		{
			for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
			{
				RwInt32 NumObj = static_cast<RwInt32>(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityHeatHaze.size());
				for(int k = 0; k < NumObj; ++k)
				{
					RwReal	fDist;
					CNtlPLHeatHazeObject* pEntityCurrent = (CNtlPLHeatHazeObject*)pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityHeatHaze[k];
					if(pEntityCurrent->Pick(&fDist))
					{
						if(fDist < fDistResult)
						{
							fDistResult		= fDist;
							pEntityPicked	= pEntityCurrent;
						}
					}
				}	
			}
			// if picked,
			if(pEntityPicked)
			{
				if(m_pSelectedHeatHaze)
				{
					m_pSelectedHeatHaze->SetToolUI(FALSE);
					m_pSelectedHeatHaze = NULL;
				}

				m_pSelectedHeatHaze = pEntityPicked;
				m_pSelectedHeatHaze->SetToolUI(TRUE);
				dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);
			}
			else
			{
				if(m_pSelectedHeatHaze)
				{
					m_pSelectedHeatHaze->SetToolUI(FALSE);
					m_pSelectedHeatHaze = NULL;
					dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(NULL);
				}
			}
		}
	}

	// editing mode
	if(m_pSelectedHeatHaze)
	{
		if(dOKEY(DIK_DELETE))
		{
			RwBool BreakFlag = FALSE;

			for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
			{
				RwInt32 NumObj = static_cast<RwInt32>(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityHeatHaze.size());
				for(int k = 0; k < NumObj; ++k)
				{
					if(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityHeatHaze[k] == m_pSelectedHeatHaze)
					{
						pMgr->m_pSectors[g_vecVisibleSectors[j]].DeleteEntity(m_pSelectedHeatHaze, 7);

						m_pSelectedHeatHaze->SetToolUI(FALSE);

						GetSceneManager()->DeleteEntity(m_pSelectedHeatHaze);

						m_pSelectedHeatHaze = NULL;

						dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(NULL);
						return;
					}
				}
			}
		}

		if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
		{
			if(dKEY(DIK_LSHIFT))
			{
				float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
				float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

				if(dKEY(DIK_Z))
				{
					RwV3d vPos = m_pSelectedHeatHaze->GetPosition();
					vPos.y -= DistY;
					m_pSelectedHeatHaze->SetPosition(&vPos);
					dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);
				}
				else
				{
					RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
					RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
					RwV3d *pRight			= RwMatrixGetRight(pM);
					RwV3d *pUp				= RwMatrixGetUp(pM);
					RwV3d *pAt				= RwMatrixGetAt(pM);
					RwV3d vFront, vRight;
					RwReal TerrainHeight, TotalHeight, ObjHeight;

					CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
					CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
					RwV3dNormalize(&vFront, &vFront);
					RwV3dNormalize(&vRight, &vRight);

					RwV3d vPos = m_pSelectedHeatHaze->GetPosition();
					// delete from the previous field
					RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
					if(CurSectorIdx != -1)
					{
						pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(m_pSelectedHeatHaze), 7);
					}

					TotalHeight		= dGET_POSITIVE_VALUE(vPos.y);
					TerrainHeight	= dGET_POSITIVE_VALUE(pMgr->GetWorldSectorHeight(vPos));
					ObjHeight		= TotalHeight - TerrainHeight;

					// transform
					RwV3dIncrementScaled(&vPos, &vFront, -DistY);
					RwV3dIncrementScaled(&vPos, &vRight, -DistX);

					vPos.y = pMgr->GetWorldSectorHeight(vPos) + ObjHeight;
					m_pSelectedHeatHaze->SetPosition(&vPos);
					dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);

					// create into the new field
					CurSectorIdx = pMgr->GetSectorIdx(vPos);
					if(CurSectorIdx != -1)
					{
						pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(m_pSelectedHeatHaze), 7);
					}
				}
			}
			// Rot
			else if(dKEY(DIK_SPACE))
			{
				float DistX = dGET_MOUSE()->GetXMovementWithRate(1.0f);
				float DistY = dGET_MOUSE()->GetYMovementWithRate(1.0f);



				if(dKEY(DIK_V))
				{						
				}
				else if(dKEY(DIK_N))
				{
				}
				else
				{
					m_pSelectedHeatHaze->m_HeatHazeObjectSwap.vRotate.y -= DistX;
					m_pSelectedHeatHaze->CalcHeatHazeData();
					dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);
				}
			}
			// Scale
			else if(dKEY(DIK_RSHIFT))
			{
				float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
				float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

				m_pSelectedHeatHaze->m_HeatHazeObjectSwap.vScale.x -= DistY;
				m_pSelectedHeatHaze->m_HeatHazeObjectSwap.vScale.y -= DistY;
				m_pSelectedHeatHaze->m_HeatHazeObjectSwap.vScale.z -= DistY;
				m_pSelectedHeatHaze->CalcHeatHazeData();
				dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);
			}
		}

		// Rotation using Keyboard
		if(dKEY(DIK_SPACE))
		{
			if(dKEY(DIK_LSHIFT))
			{
				if(dOKEY(DIK_LEFTARROW))
				{
					m_pSelectedHeatHaze->m_HeatHazeObjectSwap.vRotate.y += -m_RotationTabValue;
					m_pSelectedHeatHaze->CalcHeatHazeData();
					dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);
				}
				else if(dOKEY(DIK_RIGHTARROW))
				{
					m_pSelectedHeatHaze->m_HeatHazeObjectSwap.vRotate.y += m_RotationTabValue;
					m_pSelectedHeatHaze->CalcHeatHazeData();
					dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);
				}
			}
			else
			{
				if(dKEY(DIK_LEFTARROW))
				{
					m_pSelectedHeatHaze->m_HeatHazeObjectSwap.vRotate.y += -m_RotationTabValue;
					m_pSelectedHeatHaze->CalcHeatHazeData();
					dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);
				}
				else if(dKEY(DIK_RIGHTARROW))
				{
					m_pSelectedHeatHaze->m_HeatHazeObjectSwap.vRotate.y += m_RotationTabValue;
					m_pSelectedHeatHaze->CalcHeatHazeData();
					dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);
				}
			}
		}

		// translation keyboard
		if(dKEY(DIK_SPACE))
		{
			if(dKEY(DIK_LSHIFT))
			{
				if(dOKEY(DIK_UPARROW))
				{
					RwV3d vPos = m_pSelectedHeatHaze->GetPosition();
					vPos.y += m_fObjMovementSensitivity;
					m_pSelectedHeatHaze->SetPosition(&vPos);
					dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);
				}
				else if(dOKEY(DIK_DOWNARROW))
				{
					RwV3d vPos = m_pSelectedHeatHaze->GetPosition();
					vPos.y -= m_fObjMovementSensitivity;
					m_pSelectedHeatHaze->SetPosition(&vPos);
					dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);
				}
			}
			else
			{
				if(dKEY(DIK_UPARROW))
				{
					RwV3d vPos = m_pSelectedHeatHaze->GetPosition();
					vPos.y += m_fObjMovementSensitivity;
					m_pSelectedHeatHaze->SetPosition(&vPos);
					dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);
				}
				else if(dKEY(DIK_DOWNARROW))
				{
					RwV3d vPos = m_pSelectedHeatHaze->GetPosition();
					vPos.y -= m_fObjMovementSensitivity;
					m_pSelectedHeatHaze->SetPosition(&vPos);
					dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);
				}
			}
		}

		if(!dKEY(DIK_SPACE))
		{
			if(dKEY(DIK_LEFTARROW) || dKEY(DIK_RIGHTARROW) || dKEY(DIK_UPARROW) || dKEY(DIK_DOWNARROW))
			{
				RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
				RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
				RwV3d *pRight			= RwMatrixGetRight(pM);
				RwV3d *pUp				= RwMatrixGetUp(pM);
				RwV3d *pAt				= RwMatrixGetAt(pM);
				RwV3d vFront, vRight;

				CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
				CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
				RwV3dNormalize(&vFront, &vFront);
				RwV3dNormalize(&vRight, &vRight);

				RwV3d vPos = m_pSelectedHeatHaze->GetPosition();

				// delete from the previous field
				RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
				if(CurSectorIdx != -1)
				{
					pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(m_pSelectedHeatHaze), 7);
				}

				if(!dKEY(DIK_LSHIFT))
				{
					if(dKEY(DIK_LEFTARROW))
					{
						RwV3dIncrementScaled(&vPos, &vRight, m_fObjMovementSensitivity);
					}
					else if(dKEY(DIK_RIGHTARROW))
					{
						RwV3dIncrementScaled(&vPos, &vRight, -m_fObjMovementSensitivity);
					}
					else if(dKEY(DIK_UPARROW))
					{
						RwV3dIncrementScaled(&vPos, &vFront, m_fObjMovementSensitivity);
					}
					else if(dKEY(DIK_DOWNARROW))
					{
						RwV3dIncrementScaled(&vPos, &vFront, -m_fObjMovementSensitivity);
					}
				}
				else
				{
					if(dOKEY(DIK_LEFTARROW))
					{
						RwV3dIncrementScaled(&vPos, &vRight, m_fObjMovementSensitivity);
					}
					else if(dOKEY(DIK_RIGHTARROW))
					{
						RwV3dIncrementScaled(&vPos, &vRight, -m_fObjMovementSensitivity);
					}
					else if(dOKEY(DIK_UPARROW))
					{
						RwV3dIncrementScaled(&vPos, &vFront, m_fObjMovementSensitivity);
					}
					else if(dOKEY(DIK_DOWNARROW))
					{
						RwV3dIncrementScaled(&vPos, &vFront, -m_fObjMovementSensitivity);
					}
				}

				m_pSelectedHeatHaze->SetPosition(&vPos);
				dGETFRM()->m_pHeatHazePalette->UpdatePropertyData(m_pSelectedHeatHaze);

				// create into the new field
				CurSectorIdx = pMgr->GetSectorIdx(vPos);
				if(CurSectorIdx != -1)
				{
					pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(m_pSelectedHeatHaze), 7);
				}
			}
		}
	}
}

VOID CWorldEditorEventHander::UpdatePerformacePVS()	
{

}

VOID CWorldEditorEventHander::UpdatePerformaceImposter()
{

}

void CWorldEditorEventHander::UpdateBrushElement()
{
	static RwReal Deleay = 0.0f;
	if(dGET_CUR_PALETTE() == ePM_TERRAIN || dGET_CUR_PALETTE() == ePM_TEXTURE || dGET_CUR_PALETTE() == ePM_DIFFUSE)
	{
		if(dGET_MOUSE()->GetWheelMovement() || dKEY(DIK_LBRACKET) || dKEY(DIK_RBRACKET))
		{
			int Value = 0;

			if(dGET_MOUSE()->GetWheelMovement())
			{
				Value = static_cast<int>(dGET_MOUSE()->GetWheelMovement() / ::fabsf(dGET_MOUSE()->GetWheelMovement()));
			}
			else if(dKEY(DIK_LBRACKET))
			{
				Deleay += g_GetElapsedTime();

				if(Deleay >= 0.1f)
				{
					Value = -1;
					Deleay = 0.0f;
				}
			}
			else if(dKEY(DIK_RBRACKET))
			{
				Deleay += g_GetElapsedTime();

				if(Deleay >= 0.1f)
				{
					Value = 1;
					Deleay = 0.0f;
				}
			}

			if(dKEY(DIK_LCONTROL))
			{
				// sensitivity
				dGETFRM()->m_pTerrainPalette->OnUpdateBrushSensitivity(Value);
			}
			else
			{
				// size
				dGETFRM()->m_pTerrainPalette->OnUpdateBrushSize(Value);
			}
		}
	}	
}

void CWorldEditorEventHander::Update()
{
	// Compass camera update.
	UpdateCompass();

	// funtion key update.
	UpdateShortKey();

	// Update size, sensitivity of brush
	UpdateBrushElement();

	// Palette update.
	CNtlWorldFieldManager*			pMgr				= dGETMGR();
	int								MaxCurBrushTileNum	= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum * 4;
	sTARGET_VERT_SL_INFO			TargetVertSLInfo;
	vector<sTARGET_VERT_SL_INFO>	vecTargetVertSLInfo; vecTargetVertSLInfo.reserve(MaxCurBrushTileNum);

	// update palette
	switch(dGET_CUR_PALETTE())
	{
	case ePM_DOJO:						UpdateDojoPalette();			break;
	case ePM_DECAL_VOLUME:				UpdateDecalVolume();			break;
	case ePM_GAMEPROPERTY:				UpdateGameProperty();			break;
	case ePM_LIGHTMODE:					UpdateLightMode();				break;
	case ePM_LIGHTOBJECT:				UpdateLightObjectPalette();		break;
	case ePM_HEATHAZE:					UpdateHeatHaze();				break;
	case ePM_WORLDLIGHT:				UpdateWorldLight();				break;
	case ePM_PLANT:						UpdatePlant();					break;
	case ePM_DECAL:						m_DecalIOHandler.Update();		break;	
	case ePM_SHORE_LINE:				m_ShoreLineIOHandler.Update();	break;
	case ePM_PERFORMANCE_SECTORCULL:	UpdatePerformaceSectorCull();	break;
	case ePM_PERFORMANCE_OCCLUSION:		UpdatePerformaceOcclusion();	break;
	case ePM_PERFORMANCE_PVS:			UpdatePerformacePVS();			break;
	case ePM_PERFORMANCE_IMPOSTER:		UpdatePerformaceImposter();		break;
	// CZ-SS
	//case ePM_SOFTSPOT:					UpdateSoftSpot();				break;
	case ePM_SPAWN:						GetSpawnInstance()->Update();	break;
	case ePM_SHADOW:					UpdateShadow();					break;
	case ePM_FIELDUI:					UpdateFieldUI();				break;
	case ePM_SKYFLOATING:				UpdateSkyFloating();			break;
	case ePM_TERRAIN:					UpdateTerrainPalette();			break;
	case ePM_TEXTURE:
		{
			if(!dGET_MOUSE()->GetBehavior(MB_LBDN))
				return;

			if(dKEY(DIK_LCONTROL))
				return;

			if(!pMgr->GetTexAttr())
				return;

			RwInt32 IdxCurLyr	= pMgr->GetTexAttr()->IdxCurLT;
			RwChar	*pCurName	= NULL;

			if(!IdxCurLyr)
				return; 
			else
			{
				--IdxCurLyr;
				pCurName = pMgr->GetTexAttr()->DLT[IdxCurLyr].StrName;
			}

			for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
			{
				RwInt32			IdxResult	= -1;
				CNtlWorldSector *pCurSector = &pMgr->m_pSectors[g_vecVisibleSectors[j]];
				RpAtomic		*pCurAtomic = pMgr->m_pSectors[g_vecVisibleSectors[j]].m_pAtomic;
				RwV3d			*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
				RwRGBA			*pPrelights = RpGeometryGetPreLightColors(pCurAtomic->geometry);
				RwInt32			VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);
				RwV3d			Center		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
				RwReal			MaxRadius	= dGET_WORLD_PARAM()->WorldSectorTileSize * dGET_WORLD_PARAM()->NumBrushRadiusInTile;
				RwInt32			LenTex		= dGET_WORLD_PARAM()->WorldSectorTileNum - 1;
				RwReal			HemisphereHeightRate;
				RwReal			BrushSensitivity = dGET_WORLD_PARAM()->ValTextureSensitivity;
				RwReal			Angle;
				RwReal			Value;
				RwV2d			PosSector;
				RwV2d			TexPt;
				RwUInt8			RelativeValue;				

				// string check; different index but the same texture name
				for(int k = 0; k < dNTL_SPLAT_LAYER_NUM; ++k)
				{
					if(!pCurSector->m_pTextureSplat[k])
						continue;

					if(!::strcmp(pCurName, pCurSector->m_pTextureSplat[k]->name))
					{
						IdxResult = k;
						break;
					}
				}

				// when erase all fucn.
				if(!dKEY(DIK_LMENU) && !dKEY(DIK_Z))
				{
					if(IdxResult == -1)
					{
						continue;
					}
				}

				for(int i = 0; i < dGET_WORLD_PARAM()->WorldSectorVertNum; ++i)
				{
					for(int j = 0; j < dGET_WORLD_PARAM()->WorldSectorVertNum; ++j)
					{
						if(j == 0 && i == (dGET_WORLD_PARAM()->WorldSectorVertNum - 1 - 1))
							continue;

						if(j == 1 && j == (dGET_WORLD_PARAM()->WorldSectorVertNum - 1))
							continue;

						if(	j == (dGET_WORLD_PARAM()->WorldSectorVertNum - 1 - 1) &&
							i == (dGET_WORLD_PARAM()->WorldSectorVertNum - 1 - 1))
							continue;

						if(	j == 1 &&
							i == 0)
							continue;

						if(	j == 0 &&
							i == (dGET_WORLD_PARAM()->WorldSectorVertNum - 1))
							continue;

						if(	j == (dGET_WORLD_PARAM()->WorldSectorVertNum - 1) &&
							i == (dGET_WORLD_PARAM()->WorldSectorVertNum - 1))
							continue;

						if(j == 1 && i >= 1 && i <= (dGET_WORLD_PARAM()->WorldSectorVertNum - 1 - 1))
							continue;

						if(i == (dGET_WORLD_PARAM()->WorldSectorVertNum - 1 - 1) && j >= 1 && j <= (dGET_WORLD_PARAM()->WorldSectorVertNum - 1 - 1))
							continue;

						int		CurIdx		= i * dGET_WORLD_PARAM()->WorldSectorVertNum + j;
						float	Distance	= sqrtf((pVList[CurIdx].x - Center.x) * (pVList[CurIdx].x - Center.x) +
							(pVList[CurIdx].z - Center.z) * (pVList[CurIdx].z - Center.z));

						if(Distance <= MaxRadius)
						{
							HemisphereHeightRate = (MaxRadius - Distance) / MaxRadius;
							Angle = 90.0f * HemisphereHeightRate;
							Value = (BrushSensitivity * 10) * sinf(D3DXToRadian(Angle));

							// 10 times the Value for splatter
							RelativeValue = (RwUInt8)(Value * 4.0f);

							PosSector.x = pCurSector->DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
							PosSector.y = pCurSector->DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
							TexPt.x = pVList[CurIdx].x;
							TexPt.y = pVList[CurIdx].z;
							TexPt.x = LenTex - ((fabs(TexPt.x - PosSector.x) / (float)dGET_WORLD_PARAM()->WorldSectorSize) * LenTex);
							TexPt.y = LenTex - ((fabs(TexPt.y - PosSector.y) / (float)dGET_WORLD_PARAM()->WorldSectorSize) * LenTex);

							if(dKEY(DIK_LMENU))
							{
								if(dKEY(DIK_Z))
								{
									for(int z = 0; z < dNTL_SPLAT_LAYER_NUM; ++z)
									{
										if(pCurSector->m_pTextureAlpha[z])
											CNtlPLResourceManager::GetInstance()->SetAlphaValueDec(pCurSector->m_pTextureAlpha[z], RelativeValue, TexPt);
									}
								}
								else
									CNtlPLResourceManager::GetInstance()->SetAlphaValueDec(pCurSector->m_pTextureAlpha[IdxResult], RelativeValue, TexPt);
							}
							else
							{							
								if(!pCurSector->m_pTextureAlpha[IdxResult])
								{
									pCurSector->m_pTextureAlpha[IdxResult] = CNtlPLResourceManager::GetInstance()->CreateTexture("NTL", dGET_WORLD_PARAM()->WorldSectorTileNum, dGET_WORLD_PARAM()->WorldSectorTileNum, 32,	rwRASTERFORMAT8888);
									RwTextureSetAddressing(pCurSector->m_pTextureAlpha[IdxResult], rwTEXTUREADDRESSCLAMP);
									RwTextureSetFilterMode(pCurSector->m_pTextureAlpha[IdxResult], rwFILTERLINEAR);
								}

								CNtlPLResourceManager::GetInstance()->SetAlphaValueInc(pCurSector->m_pTextureAlpha[IdxResult], RelativeValue, TexPt);
							}
						}
					}
				}
			}
		}
		break;

	case ePM_WATER:
		{
			if(dGETFRM()->m_pWaterPalette->GetCurMode())
			{
				UpdateDepthMap();
			}
			else
			{
				UpdateWater();
			}
		}	
		break;

	case ePM_DIFFUSE:
		{
			static RwBool DoOnce = FALSE;

			if(dGET_MOUSE()->GetLBOnce())
			{
				DoOnce = TRUE;
				::CopyMemory(&m_vOldPos, &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, sizeof(RwV3d));
			}
			else
			{
				if(dGET_MOUSE()->GetLBUp())
				{
					::CopyMemory(&m_vOldPos, &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, sizeof(RwV3d));
				}

				if(dGET_MOUSE()->GetBehavior(MB_LBDN))
				{
					RwV3d  vDistWithoutSQRT;
					RwReal fDistWithoutSQRT;

					RwV3dSubMacro(&vDistWithoutSQRT, &m_vOldPos, &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
					fDistWithoutSQRT = CNtlMath::MathRwV3dSquaredLength(&vDistWithoutSQRT);

					if(fDistWithoutSQRT < dGET_WORLD_PARAM()->WorldSectorTileSize * dGET_WORLD_PARAM()->NumBrushRadiusInTile)
					{
						return;
					}
					else
					{
						DoOnce = TRUE;
						::CopyMemory(&m_vOldPos, &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, sizeof(RwV3d));
					}
				}
				else
				{
					return;
				}
			}

			if(!DoOnce)
				return;

			RwUInt8	ClrCurR = (RwUInt8)((dGET_WORLD_PARAM()->ClrDiffuserPicker & 0x00ff0000) >> 16);
			RwUInt8	ClrCurG = (RwUInt8)((dGET_WORLD_PARAM()->ClrDiffuserPicker & 0x0000ff00) >> 8);
			RwUInt8	ClrCurB = (RwUInt8)((dGET_WORLD_PARAM()->ClrDiffuserPicker & 0x000000ff) >> 0);

			for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
			{
				RpAtomic	*pCurAtomic = pMgr->m_pSectors[g_vecVisibleSectors[j]].m_pAtomic;
				RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
				RwRGBA		*pPrelights = RpGeometryGetPreLightColors(pCurAtomic->geometry);
				DWORD		*pDiffusePt = pMgr->m_pSectors[g_vecVisibleSectors[j]].m_pClrDiffusePalette;
				RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);
				RwV3d		Center		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
				RwReal		MaxRadius	= dGET_WORLD_PARAM()->WorldSectorTileSize * dGET_WORLD_PARAM()->NumBrushRadiusInTile;
				RwReal		HemisphereHeightRate;

				RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKPRELIGHT);
				{
					for(int i = 0; i < VertNum; ++i)
					{
						float Distance = ::sqrtf((pVList[i].x - Center.x) * (pVList[i].x - Center.x) +
							(pVList[i].z - Center.z) * (pVList[i].z - Center.z));

						if(Distance <= MaxRadius)
						{
							HemisphereHeightRate = (MaxRadius - Distance) / MaxRadius;
							RwReal fOpacity = dGET_WORLD_PARAM()->ValDiffuseOpacitySensitivity;

							DWORD ClrTmp = pDiffusePt[i];
							pDiffusePt[i] = 0xff000000;

							HemisphereHeightRate *= fOpacity;

							pDiffusePt[i] |= (DWORD)( ((float)ClrCurR * (HemisphereHeightRate)) + (float)( (float)((ClrTmp & 0x00ff0000) >> 16) * ((1.0f - HemisphereHeightRate)))) << 16;
							pDiffusePt[i] |= (DWORD)( ((float)ClrCurG * (HemisphereHeightRate)) + (float)( (float)((ClrTmp & 0x0000ff00) >> 8)  * ((1.0f - HemisphereHeightRate)))) << 8;
							pDiffusePt[i] |= (DWORD)( ((float)ClrCurB * (HemisphereHeightRate)) + (float)( (float)((ClrTmp & 0x000000ff) >> 0)  * ((1.0f - HemisphereHeightRate)))) << 0;							

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
				RpGeometryUnlock(pCurAtomic->geometry);
			}

			pMgr->OnSetSlopeLighting(vecTargetVertSLInfo);

			DoOnce = FALSE;
		}
		break;

	case ePM_ORNAMENT:
		{
			dGETFRM()->m_pOrnamentPalette->UpdatePreview();

			switch(dGET_MAPOBJ_MODE())
			{

				// selection mode; one or more files can be selected
			case eOEM_IDLE:
				{
					if(	dKEY(DIK_LEFTARROW) || dKEY(DIK_RIGHTARROW) || dKEY(DIK_UPARROW) || dKEY(DIK_DOWNARROW) ||
						dOKEY(DIK_INSERT) || dOKEY(DIK_HOME) || dOKEY(DIK_PGUP))
					{
						goto EDITLabel;
					}

					if(dKEY(DIK_LSHIFT) && dGET_MOUSE()->GetBehavior(MB_LBDN))
					{
						dGET_MAPOBJ_MODE() = eOEM_EDIT;
						return;
					}

					if(dKEY(DIK_SPACE) && dGET_MOUSE()->GetBehavior(MB_LBDN))
					{
						dGET_MAPOBJ_MODE() = eOEM_EDIT;
						return;
					}

					if(dKEY(DIK_RSHIFT) && dGET_MOUSE()->GetBehavior(MB_LBDN))
					{
						dGET_MAPOBJ_MODE() = eOEM_EDIT;
						return;
					}

					if(dKEY(DIK_LCONTROL) && dGET_MOUSE()->GetLBOnce() && m_vecSelectedObj.size() == 1)
					{
						dGET_MAPOBJ_MODE() = eOEM_PLACE;

						if(dGET_BRUSH_ENTITY())
						{
							GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
							dGET_BRUSH_ENTITY() = NULL;
						}

						RwV3d Pos = {-999.0f, -999.0f, -999.0f};
						SPLEntityCreateParam PLEntityCreateParam;
						PLEntityCreateParam.pPos = &Pos;

						dGET_BRUSH_ENTITY() = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, m_vecSelectedObj[0]->GetName(), &PLEntityCreateParam));
						DBO_ASSERT(dGET_BRUSH_ENTITY(), "Entity create failed.");

						(static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY()))->SetFadeEnable(FALSE);

						RwV3d vDist;
						vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
						vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
						vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;
						
						RwV3d		vScale		= *static_cast<CNtlPLObject*>(m_vecSelectedObj[0])->GetScale();
						RwV3d		vRot		= *static_cast<CNtlPLObject*>(m_vecSelectedObj[0])->GetRotate();
						RwUInt32	uiFlags		= (static_cast<CNtlPLObject*>(m_vecSelectedObj[0])->GetFlags() & (~NTL_PLEFLAG_TRIGGER));
						RwUInt32	uiMinimap	= static_cast<CNtlPLObject*>(m_vecSelectedObj[0])->GetMinimapLayer();

						static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetFlags(uiFlags);
						static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetMinimapLayer(uiMinimap);
						static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetScale(&vScale);
						static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetRotate(vRot.x, vRot.y, vRot.z);
						static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetPosition(&vDist);
						static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetVisibleCullingDistance(m_vecSelectedObj[0]->GetVisibleCullingDistance());

						if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
						{
							RwUInt32 uiLightMapObjectFlags	= static_cast<CNtlPLObject*>(m_vecSelectedObj[0])->GetLightMapObjectFlags();
							RwUInt32 uiLightMapMaterialFlags= static_cast<CNtlPLObject*>(m_vecSelectedObj[0])->GetLightMapMaterialFlags();
							static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetLightMapObjectFlags(uiLightMapObjectFlags);
							static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetLightMapMaterialFlags(uiLightMapMaterialFlags);
						}

						m_vecSelectedObj.clear();

						return;
					}					

					// popup object's property dialogs
					if(m_vecSelectedObj.size() == 1)
					{
						if(dGET_MOUSE()->GetLBDC())
						{
							CObjPropPopUp Dlg; 
							if(Dlg.DoModal() == IDOK)
							{
								sNTL_DOODADS_PROP_PARAM* pNtlDoodadsPropParam = &dGETDOC()->m_NtlDoodadsPropParam;
								if(pNtlDoodadsPropParam->VisibilityDist < 70 || pNtlDoodadsPropParam->VisibilityDist > 600)
								{
									AfxMessageBox("The value must be more than 70 and less than 600.");
									CLAMP(pNtlDoodadsPropParam->VisibilityDist, 70, 600);
								}

								dGETDOC()->SetSerialID(m_vecSelectedObj[0]);
								dGETDOC()->SetObjNameIdx(m_vecSelectedObj[0]);

								static_cast<CNtlPLObject*>(m_vecSelectedObj[0])->SetMilepostID(pNtlDoodadsPropParam->_MilePostID);

								m_vecSelectedObj[0]->SetVisibleCullingDistance(pNtlDoodadsPropParam->VisibilityDist);

								if(pNtlDoodadsPropParam->EnableTrigger)
								{
									m_vecSelectedObj[0]->SetFlags(m_vecSelectedObj[0]->GetFlags() | NTL_PLEFLAG_TRIGGER);
								}
								else
								{
									m_vecSelectedObj[0]->SetFlags(m_vecSelectedObj[0]->GetFlags() & ~NTL_PLEFLAG_TRIGGER);
								}

								if(pNtlDoodadsPropParam->EnableSelection)
								{
									m_vecSelectedObj[0]->SetFlags(m_vecSelectedObj[0]->GetFlags() | NTL_PLEFLAG_SELECTION);
								}
								else
								{
									m_vecSelectedObj[0]->SetFlags(m_vecSelectedObj[0]->GetFlags() & ~NTL_PLEFLAG_SELECTION);
								}

								if(pNtlDoodadsPropParam->EnableAlterable)
								{
									m_vecSelectedObj[0]->SetFlags(m_vecSelectedObj[0]->GetFlags() | NTL_PLEFLAG_ALTERABLE);
								}
								else
								{
									m_vecSelectedObj[0]->SetFlags(m_vecSelectedObj[0]->GetFlags() & ~NTL_PLEFLAG_ALTERABLE);
								}

								if(pNtlDoodadsPropParam->EnablePortal)
								{
									m_vecSelectedObj[0]->SetFlags(m_vecSelectedObj[0]->GetFlags() | NTL_PLEFLAG_PORTAL);
								}
								else
								{
									m_vecSelectedObj[0]->SetFlags(m_vecSelectedObj[0]->GetFlags() & ~NTL_PLEFLAG_PORTAL);
								}

								if(pNtlDoodadsPropParam->EnablePECollision)
								{
									m_vecSelectedObj[0]->SetFlags(m_vecSelectedObj[0]->GetFlags() | NTL_PLEFLAG_PECOLLISION);
								}
								else
								{
									m_vecSelectedObj[0]->SetFlags(m_vecSelectedObj[0]->GetFlags() & ~NTL_PLEFLAG_PECOLLISION);
								}

								if(pNtlDoodadsPropParam->EnableShadowProp)
								{
									m_vecSelectedObj[0]->SetFlags(m_vecSelectedObj[0]->GetFlags() | NTL_PLEFLAG_SHADOW_PROP);
								}
								else
								{
									m_vecSelectedObj[0]->SetFlags(m_vecSelectedObj[0]->GetFlags() & ~NTL_PLEFLAG_SHADOW_PROP);
								}
							}
						}
					}

					// add & remove only if some doodads were captured
					if(dGET_MOUSE()->GetLBOnce() && dKEY(DIK_LALT) && m_IsSomeCaptured)
					{
						RwV2d ScreenPos;
						ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
						ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

						RwLine cameraRay;
						RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

						RwV3d RwRayDir;
						RwV3dSubMacro(&RwRayDir, &cameraRay.end, &cameraRay.start);
						RwV3dNormalize(&RwRayDir, &RwRayDir);

						// pick map objects
						RpIntersection Intersection;
						Intersection.t.line	= cameraRay;
						Intersection.type	= rpINTERSECTLINE;

						sNtlMapObjCollisionParam CollisionParam;
						::memset(&CollisionParam, 0, sizeof(sNtlMapObjCollisionParam));

						RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &Intersection, WorldIntersectionAtomicCB, &CollisionParam);

						if(CollisionParam.IsCollided)
						{
							// skip specfic interface stuff
							if(!strcmp(static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(CollisionParam.pCurAtomic))->GetName(), "FLY_DUMMY"))
							{
								return;
							}
							else if (static_cast<CNtlPLObject*>(RpNtlAtomicGetData(CollisionParam.pCurAtomic))->GetDojo())
							{
								return;
							}
							else
							{
								// skip specfic interface stuff
								CNtlPLEntity* pCurEntity = static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(CollisionParam.pCurAtomic));
								if(::strcmp(pCurEntity->GetName(), "FLY_DUMMY"))
								{
									bool WasPlaced = FALSE;
									for(unsigned i = 0; i < m_vecSelectedObj.size(); ++i)
									{	
										if(m_vecSelectedObj[i] == pCurEntity)
										{
											WasPlaced = TRUE;
											break;
										}
									}

									if(WasPlaced)
									{
										vector<CNtlPLEntity*>::iterator where = std::find(m_vecSelectedObj.begin(), m_vecSelectedObj.end(), pCurEntity);
										m_vecSelectedObj.erase(where);
									}
									else
									{
										m_vecSelectedObj.push_back(pCurEntity);
									}
								}
							}
						}

						return;
					}

					if(dGET_MOUSE()->GetLBUp())
					{
						NTL_DELETE(m_pBrushSelector);
						NTL_DELETE(m_pBrushSelectorFrsutum);
						m_IsSomeCaptured = true;

						return;
					}

					// delete doodads
					if(dOKEY(DIK_DELETE))
					{
						for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
						{
							if (m_vecSelectedObj[i]->GetFlags() & NTL_PLEFLAG_TRIGGER)
							{
								if (AfxMessageBox("Do you want delete trigger?", MB_YESNO) == IDNO)
								{
									continue;
								}
							}
							for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
							{
								RwInt32 NumObj = static_cast<RwInt32>(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityOrnament.size());
								for(int k = 0; k < NumObj; ++k)
								{
									pMgr->m_pSectors[g_vecVisibleSectors[j]].DeleteEntity(m_vecSelectedObj[i]);
								}	
							}

							static_cast<CNtlPLObject*>(m_vecSelectedObj[i])->DeleteLightmapFile();
							GetSceneManager()->DeleteEntity(m_vecSelectedObj[i]);
						}

						m_vecSelectedObj.clear();

						if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_ANIMATIBLE_DOODAD_DUMMY))
						{
							SetAnimatibleDoodadsDummyVision(FALSE);
							SetAnimatibleDoodadsDummyVision(TRUE);
						}
					}

					// gather
					if(dOKEY(DIK_M))
					{
						RwInt32 NumSelectedObj = m_vecSelectedObj.size();
						if(NumSelectedObj > 1)
						{
							for(int i = 1; i < NumSelectedObj; ++i)
							{
								CNtlPLObject*	pNtlPLCurObj	= static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
								RwV3d			PosCur			= pNtlPLCurObj->GetPosition();
								RwInt32			CurSectorIdx	= pMgr->GetSectorIdx(PosCur);

								if(CurSectorIdx != -1)
								{
									pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pNtlPLCurObj));
								}
								else
								{
									DBO_TRACE(FALSE, "CWorldEditorEventHander::UpdateEvent, invalid sector index.");
								}

								PosCur = m_vecSelectedObj[0]->GetPosition();
								pNtlPLCurObj->SetPosition(&PosCur);

								// create into the new field
								CurSectorIdx = pMgr->GetSectorIdx(PosCur);
								if(CurSectorIdx != -1)
								{
									pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pNtlPLCurObj));
								}
							}							
						}
						else
						{
							AfxMessageBox("plz. select at least more than two doodads", MB_OK);
						}
					}

					if(dGET_MOUSE()->GetLBOnce())
					{	
						if(m_IsSomeCaptured)
						{
							m_vecSelectedObj.clear();
							m_MultipleSelect	= false;
							m_IsSomeCaptured	= false;
						}

						RwV2d ScreenPos;
						ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
						ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

						RwLine cameraRay;
						RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

						RwV3d RwRayDir;
						RwV3dSubMacro(&RwRayDir, &cameraRay.end, &cameraRay.start);
						RwV3dNormalize(&RwRayDir, &RwRayDir);

						// pick map objects
						RpIntersection Intersection;
						Intersection.t.line	= cameraRay;
						Intersection.type	= rpINTERSECTLINE;

						sNtlMapObjCollisionParam CollisionParam;
						::memset(&CollisionParam, 0, sizeof(sNtlMapObjCollisionParam));

						RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &Intersection, WorldIntersectionAtomicCB, &CollisionParam);

						if(CollisionParam.IsCollided)
						{
							RwReal	Len4T, Len4O;
							RwV3d	PosCam = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

							if(dGET_COLLISION_INFO()->IsCollidedAtSectors)
							{
								RwV3d Result4T, Result4O;
								RwV3dSubMacro(&Result4T, &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, &PosCam);
								Len4T = CNtlMath::MathRwV3dSquaredLength(&Result4T);

								RwV3dSubMacro(&Result4O, &CollisionParam.PtIntersection, &PosCam);
								Len4O = CNtlMath::MathRwV3dSquaredLength(&Result4O);

								if(Len4O > Len4T)
								{
									// the doodad must be invisible
									return;
								}
							}

							// skip specfic interface stuff
							if(!strcmp(static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(CollisionParam.pCurAtomic))->GetName(), "FLY_DUMMY"))
							{
								return;
							}
							else if (static_cast<CNtlPLObject*>(RpNtlAtomicGetData(CollisionParam.pCurAtomic))->GetDojo())
							{
								return;
							}

							m_MultipleSelect = false;
							m_vecSelectedObj.clear();

							CNtlPLEntity* pPLEntity = static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(CollisionParam.pCurAtomic));
							//if (!((pPLEntity->GetFlags() & NTL_PLEFLAG_COLLISION) && (RpNtlAtomicGetFlag(CollisionParam.pCurAtomic) & NTL_NOT_VISIBLE)) || CNtlPLGlobal::m_bCollObjVisible)
							{
								m_vecSelectedObj.push_back(pPLEntity);
							}
						}

						// refresh palette scale information.
						if(m_vecSelectedObj.size())
						{
							dGETFRM()->m_pOrnamentPalette->RefreshScaleInfo(m_vecSelectedObj[0]);
						}
					}
					else if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
					{
// 						if(!m_pBrushSelector)
// 						{
// 							if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 							{
// 								m_pBrushSelector = NTL_NEW CNtlWorldBrush;
// 
// 								CNtlWorldBrushController *pController = new CNtlWB_Rectangle<CNtlWorldFieldManager>(eNBT_RECTANGLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
// 								m_pBrushSelector->SetController(pController);
// 
// 								CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
// 								pCurBrush->SetPosS(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
// 							}
// 							else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
// 							{
// 								m_pBrushSelector = NTL_NEW CNtlWorldBrush;
// 
// 								CNtlWorldBrushController *pController = new CNTLWB_Frustum<CNtlWorldFieldManager>(eNBT_RECTANGLE, pMgr);
// 								m_pBrushSelector->SetController(pController);
// 
// 								RwV2d vPos;
// 								vPos.x = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().x);
// 								vPos.y = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().y);
// 								CNTLWB_Frustum<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNTLWB_Frustum<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
// 								pCurBrush->SetPosS(vPos);
// 							}
// 						}

						if(!m_pBrushSelector)
						{
							m_pBrushSelector = NTL_NEW CNtlWorldBrush;

							CNtlWorldBrushController *pController = new CNtlWB_Rectangle<CNtlWorldFieldManager>(eNBT_RECTANGLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
							m_pBrushSelector->SetController(pController);

							CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
							pCurBrush->SetPosS(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
						}
						if (!m_pBrushSelectorFrsutum)
						{
							m_pBrushSelectorFrsutum = NTL_NEW CNtlWorldBrush;

							CNtlWorldBrushController *pController = new CNTLWB_Frustum<CNtlWorldFieldManager>(eNBT_RECTANGLE, pMgr);
							m_pBrushSelectorFrsutum->SetController(pController);

							RwV2d vPos;
							vPos.x = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().x);
							vPos.y = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().y);
							CNTLWB_Frustum<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNTLWB_Frustum<CNtlWorldFieldManager>*>(m_pBrushSelectorFrsutum->GetController());
							pCurBrush->SetPosS(vPos);
						}

						m_MultipleSelect = true;

						UpdateObjCaptured();

						// refresh palette scale information.
						if(m_vecSelectedObj.size())
						{
							dGETFRM()->m_pOrnamentPalette->RefreshScaleInfo(m_vecSelectedObj[0]);
						}
					}
				}
				break;

				// placing mode; can place one or more objects at the same time
			case eOEM_PLACE:
				{
					// cancel
					if(dOKEY(DIK_SPACE))
					{
						if(dGET_BRUSH_ENTITY())
						{
							GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
							dGET_BRUSH_ENTITY() = NULL;
							dGET_MAPOBJ_MODE()	= eOEM_IDLE;
							return;
						}
					}

					// object moving with picking variation
					sNPE_COLLISION_PARAM sNPECollisionParam;
					sNPECollisionParam._pNtlPLEntity = dGET_BRUSH_ENTITY();

					RwV3d vDist;
					vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
					vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
					vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;

					dGET_BRUSH_ENTITY()->SetPosition(&vDist);

					// place objects
					if(dGET_MOUSE()->GetLBOnce())
					{
						if(dGET_BRUSH_ENTITY())
						{
							RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vDist);
							if(CurSectorIdx != -1)
							{
								SPLObjectCreateParam PLEntityCreateParam;
								PLEntityCreateParam.pPos		= &vDist;
								PLEntityCreateParam.bLoadMap	= TRUE;

								CNtlPLObject *pNtlPLObject = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, dGET_BRUSH_ENTITY()->GetName(), &PLEntityCreateParam));
								DBO_ASSERT(pNtlPLObject, "Entity create failed.");

								const RwV3d *pS	= static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->GetScale();
								const RwV3d *pR	= static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->GetRotate();
								RwV3d T	= static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->GetPosition();

								pNtlPLObject->SetFlags(dGET_BRUSH_ENTITY()->GetFlags());
								pNtlPLObject->SetScale(pS);
								pNtlPLObject->SetRotate(pR->x, pR->y, pR->z);
								pNtlPLObject->SetPosition(&T);
								pNtlPLObject->SetVisibleCullingDistance(dGET_BRUSH_ENTITY()->GetVisibleCullingDistance());

								if (GetNtlWorldSpace(&vDist, NULL, NULL))
								{
									pNtlPLObject->SetObjectType(EPL_OBJECT_TYPE_MINI_INDOOR_OBJECT);
								}

								if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
								{
									pNtlPLObject->SetLightMapObjectFlags(static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->GetLightMapObjectFlags());
									pNtlPLObject->SetLightMapMaterialFlags(static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->GetLightMapMaterialFlags());
								}							

								if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
								{
									pNtlPLObject->SetObjectSerialID(dGET_WORLD_PARAM()->WorldObjectSerialID);
									++dGET_WORLD_PARAM()->WorldObjectSerialID;
								}								

								pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pNtlPLObject));

								// when clicking, the object placed will get a circle
								m_MultipleSelect = false;
								m_vecSelectedObj.clear();
								UpdateObjCaptured(static_cast<CNtlPLEntity*>(pNtlPLObject));

								// refresh palette scale information.
								if(m_vecSelectedObj.size() == 1)
								{
									dGETFRM()->m_pOrnamentPalette->RefreshScaleInfo(m_vecSelectedObj[0]);
								}

								if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_ANIMATIBLE_DOODAD_DUMMY))
								{
									SetAnimatibleDoodadsDummyVision(FALSE);
									SetAnimatibleDoodadsDummyVision(TRUE);
								}

								// fade effect
								pNtlPLObject->SetFadeEnable(TRUE);
							}
						}
					}
				}
				break;

				// editing mode; for one or more objects
			case eOEM_EDIT:
				{
EDITLabel:
					if(!dKEY(DIK_LSHIFT) && !dKEY(DIK_SPACE) && !dKEY(DIK_RSHIFT))
					{
						dGET_MAPOBJ_MODE() = eOEM_IDLE;
					}					

					if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
					{
						if(dKEY(DIK_LSHIFT))
						{
							float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
							float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

							if(dKEY(DIK_Z))
							{
								for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
								{
									CNtlPLObject* pNtlPLCurObj	= static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
									RwV3d PosCur				= pNtlPLCurObj->GetPosition();

									PosCur.y -= DistY;
									pNtlPLCurObj->SetPosition(&PosCur);
								}
							}
							else
							{
								for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
								{
									RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
									RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
									RwV3d *pRight			= RwMatrixGetRight(pM);
									RwV3d *pUp				= RwMatrixGetUp(pM);
									RwV3d *pAt				= RwMatrixGetAt(pM);
									RwV3d vFront, vRight;
									RwReal TerrainHeight, TotalHeight, ObjHeight;

									CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
									CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
									RwV3dNormalize(&vFront, &vFront);
									RwV3dNormalize(&vRight, &vRight);

									CNtlPLObject* pNtlPLCurObj	= static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
									RwV3d PosCur				= pNtlPLCurObj->GetPosition();

									// delete from the previous field
									RwInt32 CurSectorIdx = pMgr->GetSectorIdx(PosCur);
									if(CurSectorIdx != -1)
									{
										pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pNtlPLCurObj));
									}

									TotalHeight		= dGET_POSITIVE_VALUE(PosCur.y);
									TerrainHeight	= dGET_POSITIVE_VALUE(pMgr->GetWorldSectorHeight(PosCur));
									ObjHeight		= TotalHeight - TerrainHeight;

									// transform
									RwV3dIncrementScaled(&PosCur, &vFront, -DistY);
									RwV3dIncrementScaled(&PosCur, &vRight, -DistX);

									PosCur.y = pMgr->GetWorldSectorHeight(PosCur) + ObjHeight;
									pNtlPLCurObj->SetPosition(&PosCur);

									// create into the new field
									CurSectorIdx = pMgr->GetSectorIdx(PosCur);
									if(CurSectorIdx != -1)
									{
										pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pNtlPLCurObj));
									}
								}
							}
						}
						else if(dKEY(DIK_SPACE))
						{
							float DistX = dGET_MOUSE()->GetXMovementWithRate(1.0f);
							float DistY = dGET_MOUSE()->GetYMovementWithRate(1.0f);

							if(dKEY(DIK_V))
							{
								for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
								{
									CNtlPLObject* pNtlPLCurObj = static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
									const RwV3d *pPosCur = pNtlPLCurObj->GetRotate();
									pNtlPLCurObj->SetRotate(pPosCur->x - DistX, pPosCur->y, pPosCur->z);
								}
							}
							else if(dKEY(DIK_N))
							{
								for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
								{
									CNtlPLObject* pNtlPLCurObj = static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
									const RwV3d *pPosCur = pNtlPLCurObj->GetRotate();
									pNtlPLCurObj->SetRotate(pPosCur->x, pPosCur->y, pPosCur->z - DistX);
								}
							}
							else
							{
								for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
								{
									CNtlPLObject* pNtlPLCurObj = static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
									const RwV3d *pPosCur = pNtlPLCurObj->GetRotate();
									pNtlPLCurObj->SetRotate(pPosCur->x, pPosCur->y - DistX, pPosCur->z);
								}
							}
						}
						else if(dKEY(DIK_RSHIFT))
						{
							float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
							float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

							for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
							{
								CNtlPLObject* pNtlPLCurObj = static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
								RwV3d *pPosCur = const_cast<RwV3d*>(pNtlPLCurObj->GetScale());
								pPosCur->x -= DistY;
								pPosCur->y -= DistY;
								pPosCur->z -= DistY;
								CLAMP(pPosCur->x, 0.0f, 10.0f);
								CLAMP(pPosCur->y, 0.0f, 10.0f);
								CLAMP(pPosCur->z, 0.0f, 10.0f);
								pNtlPLCurObj->SetScale(pPosCur);
							}

							// refresh palette scale information.
							if(m_vecSelectedObj.size() == 1)
							{
								dGETFRM()->m_pOrnamentPalette->RefreshScaleInfo(m_vecSelectedObj[0]);
							}
						}
					}

					// Rotation using Keyboard
					if(dKEY(DIK_SPACE))
					{
						if(dKEY(DIK_LSHIFT))
						{
							if(dOKEY(DIK_LEFTARROW))
							{
								for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
								{
									CNtlPLObject* pNtlPLCurObj = static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
									const RwV3d *pPosCur = pNtlPLCurObj->GetRotate();
									pNtlPLCurObj->SetRotate(pPosCur->x, pPosCur->y - m_RotationTabValue, pPosCur->z);
								}
							}
							else if(dOKEY(DIK_RIGHTARROW))
							{
								for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
								{
									CNtlPLObject* pNtlPLCurObj = static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
									const RwV3d *pPosCur = pNtlPLCurObj->GetRotate();
									pNtlPLCurObj->SetRotate(pPosCur->x, pPosCur->y + m_RotationTabValue, pPosCur->z);
								}
							}
						}
						else
						{
							if(dKEY(DIK_LEFTARROW))
							{
								for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
								{
									CNtlPLObject* pNtlPLCurObj = static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
									const RwV3d *pPosCur = pNtlPLCurObj->GetRotate();
									pNtlPLCurObj->SetRotate(pPosCur->x, pPosCur->y - m_RotationTabValue, pPosCur->z);
								}
							}
							else if(dKEY(DIK_RIGHTARROW))
							{
								for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
								{
									CNtlPLObject* pNtlPLCurObj = static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
									const RwV3d *pPosCur = pNtlPLCurObj->GetRotate();
									pNtlPLCurObj->SetRotate(pPosCur->x, pPosCur->y + m_RotationTabValue, pPosCur->z);
								}
							}
						}
					}

					if(dKEY(DIK_UPARROW) | dKEY(DIK_DOWNARROW))
					{
						if(dKEY(DIK_SPACE))
						{
							if(!dKEY(DIK_LSHIFT))
							{
								for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
								{
									CNtlPLObject* pNtlPLCurObj	= static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
									RwV3d PosCur				= pNtlPLCurObj->GetPosition();

									if(dKEY(DIK_UPARROW))
									{
										PosCur.y += m_fObjMovementSensitivity;
									}
									else if(dKEY(DIK_DOWNARROW))
									{
										PosCur.y -= m_fObjMovementSensitivity;
									}

									pNtlPLCurObj->SetPosition(&PosCur);
								}
							}
							else
							{
								for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
								{
									CNtlPLObject* pNtlPLCurObj	= static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
									RwV3d PosCur				= pNtlPLCurObj->GetPosition();

									if(dOKEY(DIK_UPARROW))
									{
										PosCur.y += m_fObjMovementSensitivity;
									}
									else if(dOKEY(DIK_DOWNARROW))
									{
										PosCur.y -= m_fObjMovementSensitivity;
									}

									pNtlPLCurObj->SetPosition(&PosCur);
								}
							}
						}
					}

					if(dKEY(DIK_LEFTARROW) || dKEY(DIK_RIGHTARROW) || dKEY(DIK_UPARROW) || dKEY(DIK_DOWNARROW))
					{			
						if(!dKEY(DIK_SPACE))
						{
							for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
							{
								RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
								RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
								RwV3d *pRight			= RwMatrixGetRight(pM);
								RwV3d *pUp				= RwMatrixGetUp(pM);
								RwV3d *pAt				= RwMatrixGetAt(pM);
								RwV3d vFront, vRight;

								CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
								CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
								RwV3dNormalize(&vFront, &vFront);
								RwV3dNormalize(&vRight, &vRight);

								CNtlPLObject* pNtlPLCurObj	= static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
								RwV3d PosCur				= pNtlPLCurObj->GetPosition();

								// delete from the previous field
								RwInt32 CurSectorIdx = pMgr->GetSectorIdx(PosCur);
								if(CurSectorIdx != -1)
								{
									pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pNtlPLCurObj));
								}

								if(!dKEY(DIK_LSHIFT))
								{
									if(dKEY(DIK_LEFTARROW))
									{
										RwV3dIncrementScaled(&PosCur, &vRight, m_fObjMovementSensitivity);
									}
									else if(dKEY(DIK_RIGHTARROW))
									{
										RwV3dIncrementScaled(&PosCur, &vRight, -m_fObjMovementSensitivity);
									}
									else if(dKEY(DIK_UPARROW))
									{
										RwV3dIncrementScaled(&PosCur, &vFront, m_fObjMovementSensitivity);
									}
									else if(dKEY(DIK_DOWNARROW))
									{
										RwV3dIncrementScaled(&PosCur, &vFront, -m_fObjMovementSensitivity);
									}
								}
								else
								{
									if(dOKEY(DIK_LEFTARROW))
									{
										RwV3dIncrementScaled(&PosCur, &vRight, m_fObjMovementSensitivity);
									}
									else if(dOKEY(DIK_RIGHTARROW))
									{
										RwV3dIncrementScaled(&PosCur, &vRight, -m_fObjMovementSensitivity);
									}
									else if(dOKEY(DIK_UPARROW))
									{
										RwV3dIncrementScaled(&PosCur, &vFront, m_fObjMovementSensitivity);
									}
									else if(dOKEY(DIK_DOWNARROW))
									{
										RwV3dIncrementScaled(&PosCur, &vFront, -m_fObjMovementSensitivity);
									}
								}

								//PosCur.y = pMgr->GetWorldSectorHeight(PosCur);

								pNtlPLCurObj->SetPosition(&PosCur);

								// create into the new field
								CurSectorIdx = pMgr->GetSectorIdx(PosCur);
								if(CurSectorIdx != -1)
								{
									pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pNtlPLCurObj));
								}
							}
						}
					}

					// backup transform
					if(dOKEY(DIK_INSERT) || dOKEY(DIK_HOME) || dOKEY(DIK_PGUP))
					{
						if(dOKEY(DIK_INSERT))
						{
							RwV3d vScaleUnit = { 1.0f, 1.0f, 1.0f };

							for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
							{
								static_cast<CNtlPLObject*>(m_vecSelectedObj[i])->SetScale(&vScaleUnit);
							}
						}

						if(dOKEY(DIK_HOME))
						{
							RwV3d vRotateUnit = { 0.0f, 0.0f, 0.0f };

							for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
							{
								static_cast<CNtlPLObject*>(m_vecSelectedObj[i])->SetRotate(vRotateUnit.x, vRotateUnit.y, vRotateUnit.z);
							}
						}

						if(dOKEY(DIK_PGUP))
						{
							RwV3d vTransUnit;

							for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
							{
								vTransUnit = static_cast<CNtlPLObject*>(m_vecSelectedObj[i])->GetPosition();
								vTransUnit.y = pMgr->GetWorldSectorHeight(vTransUnit);
								static_cast<CNtlPLObject*>(m_vecSelectedObj[i])->SetPosition(&vTransUnit);
							}
						}
					}

					if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_ANIMATIBLE_DOODAD_DUMMY))
					{
						SetAnimatibleDoodadsDummyVision(FALSE);
						SetAnimatibleDoodadsDummyVision(TRUE);
					}
				}
				break;
			}

			m_iAttachBoneIdx	= -1;
			m_pAttachBoneObject = NULL;
			if(m_vecSelectedObj.size() == 1)
			{						
				std::vector<std::string> vecBoneName;
				GetCurDoodad()->GetBoneName(vecBoneName);

				RwV3d		vCheckPos	= GetCurDoodad()->GetPosition();
				RwInt32		iSectorIdx	= pMgr->GetSectorIdx(vCheckPos);

				std::vector<RwInt32> vecSectors;
				pMgr->GetNeighborSectors(iSectorIdx, vecSectors);
				vecSectors.push_back(iSectorIdx);

				RwReal			fResultDistSq	= RwRealMAXVAL;
				RwInt32			fResultBoneIdx	= 0;
				CNtlPLObject*	pResultObject	= NULL;

				for (RwInt32 iSectorCnt = 0; iSectorCnt < (RwInt32)vecSectors.size(); ++iSectorCnt)
				{
					iSectorIdx = vecSectors.at(iSectorCnt);
					if (iSectorIdx == -1)
					{
						continue;
					}

					CNtlWorldSector* pSector = &pMgr->m_pSectors[iSectorIdx];
					for (RwInt32 iOrnamentCnt = 0; iOrnamentCnt < (RwInt32)pSector->m_vecNtlPLEntityOrnament.size(); ++ iOrnamentCnt)
					{
						CNtlPLObject* pPLObjectCheck = (CNtlPLObject*)pSector->m_vecNtlPLEntityOrnament.at(iOrnamentCnt);

						if (pPLObjectCheck == GetCurDoodad())
						{
							continue;
						}

						if (NtlTestSphereShpere(GetCurDoodad()->GetBoundingSphere(), pPLObjectCheck->GetBoundingSphere()))
						{
							for (RwInt32 iBoneCnt = 0; iBoneCnt < (RwInt32)vecBoneName.size(); ++iBoneCnt)
							{
								RwMatrix* pmatCheck = pPLObjectCheck->GetBoneMatrix(vecBoneName.at(iBoneCnt).c_str());
								if (pmatCheck)
								{
									RwMatrix* pmatAttach = GetCurDoodad()->GetBoneMatrix(vecBoneName.at(iBoneCnt).c_str());

									RwV3d vDelta;
									RwV3dSubMacro(&vDelta, RwMatrixGetPos(pmatAttach), RwMatrixGetPos(pmatCheck));

									RwReal fDeltaSq = RwV3dDotProduct(&vDelta, &vDelta);

									if (fResultDistSq > fDeltaSq + 0.01f)
									{
										fResultDistSq	= fDeltaSq;
										fResultBoneIdx	= iBoneCnt;
										pResultObject	= pPLObjectCheck;
									}
								}
							}
						}									
					}
				}

				if (pResultObject)
				{
					m_pAttachBoneObject = pResultObject;
					m_iAttachBoneIdx	= fResultBoneIdx;

					if (dOKEY(DIK_T))
					{
						RwMatrix* pMat1;
						RwMatrix* pMat2;							

						const RwV3d* pRotate = m_pAttachBoneObject->GetRotate();

						GetCurDoodad()->SetRotate(0.0f, pRotate->y, 0.0f);
						GetCurDoodad()->CalcBoundingSphere();

						pMat1 = GetCurDoodad()->GetBoneMatrix(vecBoneName.at(fResultBoneIdx).c_str());
						pMat2 = m_pAttachBoneObject->GetBoneMatrix(vecBoneName.at(m_iAttachBoneIdx).c_str());							

// 						RwMatrix matTemp;
// 						CNtlMath::MathGetRotationMatrix(&matTemp, RwMatrixGetAt(pMat1), RwMatrixGetAt(pMat2));
// 
// 						RwV3d vTemp;
// 						CNtlMath::MathGetMatrixEulerAngle(&matTemp, &vTemp);							
// 
// 						GetCurDoodad()->SetRotate(vTemp.x, vTemp.y, vTemp.z);
// 						GetCurDoodad()->CalcBoundingSphere();

						pMat1 = GetCurDoodad()->GetBoneMatrix(vecBoneName.at(fResultBoneIdx).c_str());
						pMat2 = m_pAttachBoneObject->GetBoneMatrix(vecBoneName.at(m_iAttachBoneIdx).c_str());							

						
						CNtlWorldFieldManager*	pMgr	= dGETMGR();
						RwV3d					vTemp	= GetCurDoodad()->GetPosition();
						RwInt32					iIdx	= pMgr->GetSectorIdx(vTemp);

						if(iIdx != -1)
						{
							pMgr->m_pSectors[iIdx].DeleteEntity(static_cast<CNtlPLEntity*>(GetCurDoodad()));
						}

						vTemp = GetCurDoodad()->GetPosition() + (*RwMatrixGetPos(pMat2) - *RwMatrixGetPos(pMat1));

						iIdx = pMgr->GetSectorIdx(vTemp);
						if(iIdx != -1)
						{
							GetCurDoodad()->SetPosition(&vTemp);
							pMgr->m_pSectors[iIdx].InsertIn2World(static_cast<CNtlPLEntity*>(GetCurDoodad()));							
						}
					}
				}					
			}
		}
		break;

	case ePM_EFFECT:
		{
			UpdateEffect();
		}	
		break;

	case ePM_NM_PROP:
		{
			UpdateNMProp();
		}
		break;

	case ePM_SE:
		{
			UpdateSE();
		}
		break;
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		((CNtlWEFrm*)AfxGetMainWnd())->GetWorldView()->UpdateMainView();
	}

	static ePALETTE_MODE ePaleteOld = ePM_DEFAULT;
	if (CNtlWorldPathEngineManager::GetInstance()->GetPathEngineMode())
	{		
		if (dGET_CUR_PALETTE() != ePM_NM_PROP)
		{
			UpdatePathEngineMode();
		}
	}
	ePaleteOld = dGET_CUR_PALETTE();
}

VOID CWorldEditorEventHander::SetScaleCurDoodad(RwReal _X, RwReal _Y, RwReal _Z)
{
	if(m_vecSelectedObj.size() == 1)
	{
		RwV3d vScale;
		vScale.x = _X;
		vScale.y = _Y;
		vScale.z = _Z;
		static_cast<CNtlPLObject*>(m_vecSelectedObj[0])->SetScale(&vScale);
	}
	else
	{
		::AfxMessageBox("can't scale, only one doodad can be scaled.");
	}
}

void CWorldEditorEventHander::VerifyObjCaptured()
{
	if(!m_MultipleSelect)
		return;

	if(!m_pBrushSelector)
		return;

	vector<CNtlPLEntity*>::iterator iter;
	iter = m_vecSelectedObj.begin(); 

	while(iter != m_vecSelectedObj.end())
	{
		CNtlPLEntity *pCurPLEntity	= *iter;
		RwV3d PosCur				= static_cast<CNtlPLObject*>(pCurPLEntity)->GetPosition();

		if(!VerifyObjCaptured(PosCur))
		{
			iter = m_vecSelectedObj.erase(iter);
			continue;
		}

		++iter;	
	}
}

bool CWorldEditorEventHander::VerifyObjCaptured(CNtlPLEntity* pNtlPLEntity)
{
	for(int i = 0; i < static_cast<int>(m_vecSelectedObj.size()); ++i)
	{
		CNtlPLEntity *pCurPLEntity = m_vecSelectedObj[i];
		if(pNtlPLEntity == pCurPLEntity)
			return false;
	}

	m_vecSelectedObj.push_back(pNtlPLEntity);

	return true;
}

bool CWorldEditorEventHander::VerifyObjCaptured(RwV3d& PosObjMid)
{
	if (m_pBrushSelectorFrsutum && (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD || CNtlPLWorldState::GetActiveMiniIndoor()))
	{
		CNTLWB_Frustum<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNTLWB_Frustum<CNtlWorldFieldManager>*>(m_pBrushSelectorFrsutum->GetController());
		return pCurBrush->FrustumTest(PosObjMid);
	}
	else if (m_pBrushSelector && GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush	= reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());

		RwV3d PosS = pCurBrush->GetPosS();
		RwV3d PosE = pCurBrush->GetPosE();

		bool AllTrue[2] = {0, 0};

		if(PosS.x > PosE.x)
		{
			if(PosObjMid.x > PosE.x && PosObjMid.x < PosS.x)
			{
				AllTrue[0] = true;
			}
		}
		else
		{
			if(PosObjMid.x < PosE.x && PosObjMid.x > PosS.x)
			{
				AllTrue[0] = true;
			}
		}

		if(PosS.z > PosE.z)
		{
			if(PosObjMid.z > PosE.z && PosObjMid.z < PosS.z)
			{
				AllTrue[1] = true;
			}
		}
		else
		{
			if(PosObjMid.z < PosE.z && PosObjMid.z > PosS.z)
			{
				AllTrue[1] = true;
			}
		}

		if(AllTrue[0] && AllTrue[1])
		{
			return true;
		}
		else
			return false;
	}
	return false;

// 	if(!m_pBrushSelector)
// 		return false;
// 
// 	CNtlWorldFieldManager *pMgr							= dGETMGR();
// 
// 	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 	{
// 		CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush	= reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
// 
// 		RwV3d PosS = pCurBrush->GetPosS();
// 		RwV3d PosE = pCurBrush->GetPosE();
// 
// 		bool AllTrue[2] = {0, 0};
// 
// 		if(PosS.x > PosE.x)
// 		{
// 			if(PosObjMid.x > PosE.x && PosObjMid.x < PosS.x)
// 			{
// 				AllTrue[0] = true;
// 			}
// 		}
// 		else
// 		{
// 			if(PosObjMid.x < PosE.x && PosObjMid.x > PosS.x)
// 			{
// 				AllTrue[0] = true;
// 			}
// 		}
// 
// 		if(PosS.z > PosE.z)
// 		{
// 			if(PosObjMid.z > PosE.z && PosObjMid.z < PosS.z)
// 			{
// 				AllTrue[1] = true;
// 			}
// 		}
// 		else
// 		{
// 			if(PosObjMid.z < PosE.z && PosObjMid.z > PosS.z)
// 			{
// 				AllTrue[1] = true;
// 			}
// 		}
// 
// 		if(AllTrue[0] && AllTrue[1])
// 		{
// 			return true;
// 		}
// 		else
// 			return false;
// 	}
// 	else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
// 	{
// 		CNTLWB_Frustum<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNTLWB_Frustum<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
// 		return pCurBrush->FrustumTest(PosObjMid);
// 	}
// 
// 	return false;
}

void CWorldEditorEventHander::UpdateObjCaptured(CNtlPLEntity* pNtlPLEntity)
{
	if(pNtlPLEntity)
	{
		VerifyObjCaptured(pNtlPLEntity);
		return;
	}

// 	CNtlWorldFieldManager *pMgr = dGETMGR();
// 	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 	{
// 		CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
// 		pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);		
// 	}
// 	else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
// 	{
// 		RwV2d vPos;
// 		vPos.x = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().x);
// 		vPos.y = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().y);
// 		CNTLWB_Frustum<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNTLWB_Frustum<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
// 		pCurBrush->SetPosE(vPos);
// 		pCurBrush->CalcFrustumData();
// 	}

	CNtlWorldFieldManager *pMgr = dGETMGR();
	{
		CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
		pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);		
	}
	{
		RwV2d vPos;
		vPos.x = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().x);
		vPos.y = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().y);
		CNTLWB_Frustum<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNTLWB_Frustum<CNtlWorldFieldManager>*>(m_pBrushSelectorFrsutum->GetController());
		pCurBrush->SetPosE(vPos);
		pCurBrush->CalcFrustumData();
	}


	for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
	{
		RwInt32 NumObj = static_cast<RwInt32>(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityOrnament.size());
		for(int k = 0; k < NumObj; ++k)
		{
			CNtlPLEntity* pCurEntity	= pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityOrnament[k];
			RwV3d PosCur				= static_cast<CNtlPLObject*>(pCurEntity)->GetPosition();

			if(VerifyObjCaptured(PosCur))
			{
				CNtlPLObject* pPLObject = static_cast<CNtlPLObject*>(pCurEntity);

				for (ENTITY_ATOMIC_VEC_ITER it = pPLObject->GetAtomicList()->begin(); it != pPLObject->GetAtomicList()->end(); ++it)
				{
					RpAtomic* pAtomic = (*it)->pData;
					if (((pPLObject->GetFlags() & NTL_PLEFLAG_COLLISION) && (RpNtlAtomicGetFlag(pAtomic) & NTL_NOT_VISIBLE)) && !CNtlPLGlobal::m_bCollObjVisible)
					{
						continue;
					}

					VerifyObjCaptured(pCurEntity);
					break;
				}

				// 				if (!CNtlPLGlobal::m_bCollObjVisible)
				// 				{
				// 					if (pPLObject->GetFlags() & NTL_PLEFLAG_COLLISION)
				// 					{
				// 						RwBool bBreak = TRUE;
				// 						for (ENTITY_ATOMIC_VEC_ITER it = pPLObject->GetAtomicList()->begin(); it != pPLObject->GetAtomicList()->end(); ++it)
				// 						{
				// 							if (!(RpNtlAtomicGetFlag((*it)->pData) & NTL_NOT_VISIBLE))
				// 							{
				// 								bBreak = FALSE;
				// 								break;
				// 							}
				// 						}
				// 
				// 						if (bBreak)
				// 						{
				// 							break;
				// 						}
				// 					}
				// 				}
				//				VerifyObjCaptured(pCurEntity);
			}
		}	
	}
}

RpAtomic* CWorldEditorEventHander::AtomicBrushRenderCB(RpAtomic *atomic, void *data)
{
	sBRUSHCB_PARAM*				pVoid	= static_cast<sBRUSHCB_PARAM*>(data);
	CWorldEditorEventHander*	pThis	= pVoid->pThis;
	CNtlWorldFieldManager*		pMgr	= dGETMGR();

	if(pThis->m_pBrushSelection == NULL)
	{
		pThis->m_pBrushSelection				= NTL_NEW CNtlWorldBrush;
		CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
		pThis->m_pBrushSelection->SetController(pController);
	}

	CNtlWB_Circle<CNtlWorldFieldManager>* pCurBrush	= reinterpret_cast<CNtlWB_Circle<CNtlWorldFieldManager>*>(pThis->m_pBrushSelection->GetController());

	RwV3d *pArgScale = const_cast<RwV3d*>(static_cast<CNtlPLObject*>(pThis->m_vecSelectedObj[pVoid->CurIdx])->GetScale());
	pCurBrush->Render(static_cast<CNtlPLObject*>(pThis->m_vecSelectedObj[pVoid->CurIdx])->GetPosition(), atomic->boundingSphere.radius, *pArgScale);

	return atomic;
}

RpAtomic* CWorldEditorEventHander::AtomicBrushRenderIndoorCB(RpAtomic *atomic, void *data)
{
	RwRGBA						rgba	= { 255, 255, 255, 255 };
	sBRUSHCB_PARAM*				pVoid	= static_cast<sBRUSHCB_PARAM*>(data);
	CWorldEditorEventHander*	pThis	= pVoid->pThis;
	CNtlWorldFieldManager*		pMgr	= dGETMGR();

	CNtlPLObject* pNtlPLObject = static_cast<CNtlPLObject*>(pThis->m_vecSelectedObj[pVoid->CurIdx]);
	RenderGeometryBillboardSphere(&pNtlPLObject->GetMatrix(), &rgba, atomic->boundingSphere.radius, CNtlPLGlobal::m_RwCamera, TRUE);

	return atomic;
}

void CWorldEditorEventHander::UpdatePathEngineMode()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	if(dOKEY(DIK_SPACE))
	{
		if(m_pBrushSelectorPE)
		{
			NTL_DELETE(m_pBrushSelectorPE);
		}			
	}

	if(!m_pBrushSelectorPE)
	{
		m_pBrushSelectorPE = NTL_NEW CNtlWorldBrush;

		CNtlWorldBrushController *pController = new CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>(eNBT_TILE_EX, CNtlWorldPathEngineManager::GetInstance(), &CNtlWorldPathEngineManager::GetHeight);
		m_pBrushSelectorPE->SetController(pController);

		UpdatePathEngineBrushLimit();
	}

	if(dGET_MOUSE()->GetLBOnce())
	{
		CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>*>(m_pBrushSelectorPE->GetController());
		pCurBrush->SetPosS(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
	}

	if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE) && m_pBrushSelectorPE)
	{
		CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>*>(m_pBrushSelectorPE->GetController());

		if(dKEY(DIK_LSHIFT))
		{
			pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, TRUE);
		}
		else
		{
			pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
		}
	}

	if (dOKEY(DIK_INSERT))
	{
		CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>*>(m_pBrushSelectorPE->GetController());
		vector<RwInt32> vecTileContainer = pCurBrush->GetTileContainer();

		RwInt32 CntX;
		RwInt32 CntZ;
		RwInt32 NumWorldTile	= dGET_WORLD_PARAM()->WorldSize / CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		RwInt32 LenTile			= CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		RwV3d	PosTile;

		for(RwUInt32 i = 0; i < vecTileContainer.size(); ++i)
		{
			CntX		= vecTileContainer[i] - ((vecTileContainer[i] / NumWorldTile) * NumWorldTile);
			CntZ		= vecTileContainer[i] / NumWorldTile;
			PosTile.x	= static_cast<RwReal>(CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
			PosTile.z	= static_cast<RwReal>(CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);

			CNtlWorldPathEngineManager::GetInstance()->SetPathEngineData(PosTile, CNtlWorldPathEngineManager::GetInstance()->GetHeight(PosTile));
		}

		if(m_pBrushSelectorPE)
		{
			NTL_DELETE(m_pBrushSelectorPE);
		}
	}

	if (dOKEY(DIK_DELETE))
	{
		CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>*>(m_pBrushSelectorPE->GetController());
		vector<RwInt32> vecTileContainer = pCurBrush->GetTileContainer();

		RwInt32 CntX;
		RwInt32 CntZ;
		RwInt32 NumWorldTile	= dGET_WORLD_PARAM()->WorldSize / CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		RwInt32 LenTile			= CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		RwV3d	PosTile;

		for(RwUInt32 i = 0; i < vecTileContainer.size(); ++i)
		{
			CntX		= vecTileContainer[i] - ((vecTileContainer[i] / NumWorldTile) * NumWorldTile);
			CntZ		= vecTileContainer[i] / NumWorldTile;
			PosTile.x	= static_cast<RwReal>(CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
			PosTile.z	= static_cast<RwReal>(CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);

			CNtlWorldPathEngineManager::GetInstance()->SetPathEngineData(PosTile, RwRealMAXVAL);
		}

		if(m_pBrushSelectorPE)
		{
			NTL_DELETE(m_pBrushSelectorPE);
		}
	}

	if (dOKEY(DIK_HOME))
	{
		CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>*>(m_pBrushSelectorPE->GetController());
		vector<RwInt32> vecTileContainer = pCurBrush->GetTileContainer();

		RwInt32 CntX;
		RwInt32 CntZ;
		RwInt32 NumWorldTile	= dGET_WORLD_PARAM()->WorldSize / CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		RwInt32 LenTile			= CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		RwV3d	PosTile;

		for(RwUInt32 i = 0; i < vecTileContainer.size(); ++i)
		{
			CntX		= vecTileContainer[i] - ((vecTileContainer[i] / NumWorldTile) * NumWorldTile);
			CntZ		= vecTileContainer[i] / NumWorldTile;
			PosTile.x	= static_cast<RwReal>(CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
			PosTile.z	= static_cast<RwReal>(CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
			PosTile.y   = CNtlWorldPathEngineManager::GetInstance()->GetPathEngineData(PosTile);

			if (PosTile.y != -999.0f && PosTile.y != RwRealMAXVAL)
			{
				CNtlWorldPathEngineManager::GetInstance()->SetPathEngineData(PosTile, dGETMGR()->GetWorldSectorHeight(PosTile));
			}
		}

		if(m_pBrushSelectorPE)
		{
			NTL_DELETE(m_pBrushSelectorPE);
		}
	}

	if (dOKEY(DIK_PGDN))
	{
		CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>*>(m_pBrushSelectorPE->GetController());
		vector<RwInt32> vecTileContainer = pCurBrush->GetTileContainer();

		RwInt32 CntX;
		RwInt32 CntZ;
		RwInt32 NumWorldTile	= dGET_WORLD_PARAM()->WorldSize / CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		RwInt32 LenTile			= CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		RwV3d	PosTile;

		RwReal	fHeight			= 9999.0f;//RwRealMAXVAL;
		for(RwUInt32 i = 0; i < vecTileContainer.size(); ++i)
		{
			CntX		= vecTileContainer[i] - ((vecTileContainer[i] / NumWorldTile) * NumWorldTile);
			CntZ		= vecTileContainer[i] / NumWorldTile;
			PosTile.x	= static_cast<RwReal>(CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
			PosTile.z	= static_cast<RwReal>(CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
			PosTile.y   = CNtlWorldPathEngineManager::GetInstance()->GetPathEngineData(PosTile);

			if (PosTile.y != -999.0f && PosTile.y != RwRealMAXVAL)
			{
				if (fHeight > PosTile.y)
				{
					fHeight = PosTile.y;
				}				
			}
		}

		for(RwUInt32 i = 0; i < vecTileContainer.size(); ++i)
		{
			CntX		= vecTileContainer[i] - ((vecTileContainer[i] / NumWorldTile) * NumWorldTile);
			CntZ		= vecTileContainer[i] / NumWorldTile;
			PosTile.x	= static_cast<RwReal>(CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
			PosTile.z	= static_cast<RwReal>(CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
			PosTile.y   = CNtlWorldPathEngineManager::GetInstance()->GetPathEngineData(PosTile);

			if (PosTile.y != -999.0f && PosTile.y != RwRealMAXVAL)
			{
				CNtlWorldPathEngineManager::GetInstance()->SetPathEngineData(PosTile, fHeight);
			}
		}	

		if(m_pBrushSelectorPE)
		{
			NTL_DELETE(m_pBrushSelectorPE);
		}
	}

	if (dOKEY(DIK_PGUP))
	{
		CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>*>(m_pBrushSelectorPE->GetController());
		vector<RwInt32> vecTileContainer = pCurBrush->GetTileContainer();

		RwInt32 CntX;
		RwInt32 CntZ;
		RwInt32 NumWorldTile	= dGET_WORLD_PARAM()->WorldSize / CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		RwInt32 LenTile			= CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		RwV3d	PosTile;

		RwReal	fHeight			= -9999.0f;//RwRealMINVAL;
		for(RwUInt32 i = 0; i < vecTileContainer.size(); ++i)
		{
			CntX		= vecTileContainer[i] - ((vecTileContainer[i] / NumWorldTile) * NumWorldTile);
			CntZ		= vecTileContainer[i] / NumWorldTile;
			PosTile.x	= static_cast<RwReal>(CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
			PosTile.z	= static_cast<RwReal>(CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
			PosTile.y   = CNtlWorldPathEngineManager::GetInstance()->GetPathEngineData(PosTile);

			if (PosTile.y != -999.0f && PosTile.y != RwRealMAXVAL)
			{
				if (fHeight < PosTile.y)
				{
					fHeight = PosTile.y;
				}				
			}
		}

		for(RwUInt32 i = 0; i < vecTileContainer.size(); ++i)
		{
			CntX		= vecTileContainer[i] - ((vecTileContainer[i] / NumWorldTile) * NumWorldTile);
			CntZ		= vecTileContainer[i] / NumWorldTile;
			PosTile.x	= static_cast<RwReal>(CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
			PosTile.z	= static_cast<RwReal>(CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
			PosTile.y   = CNtlWorldPathEngineManager::GetInstance()->GetPathEngineData(PosTile);

			if (PosTile.y != -999.0f && PosTile.y != RwRealMAXVAL)
			{
				CNtlWorldPathEngineManager::GetInstance()->SetPathEngineData(PosTile, fHeight);
			}
		}	

		if(m_pBrushSelectorPE)
		{
			NTL_DELETE(m_pBrushSelectorPE);
		}
	}

	if (dOKEY(DIK_END))
	{
		CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>*>(m_pBrushSelectorPE->GetController());
		vector<RwInt32> vecTileContainer = pCurBrush->GetTileContainer();

		RwInt32 CntX;
		RwInt32 CntZ;
		RwInt32 NumWorldTile	= dGET_WORLD_PARAM()->WorldSize / CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		RwInt32 LenTile			= CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		RwV3d	PosTile;

		CPeHeightPopup	PeHeightPopup;
		if (PeHeightPopup.DoModal() == IDOK)
		{
			for(RwUInt32 i = 0; i < vecTileContainer.size(); ++i)
			{
				CntX		= vecTileContainer[i] - ((vecTileContainer[i] / NumWorldTile) * NumWorldTile);
				CntZ		= vecTileContainer[i] / NumWorldTile;
				PosTile.x	= static_cast<RwReal>(CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
				PosTile.z	= static_cast<RwReal>(CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
				PosTile.y   = CNtlWorldPathEngineManager::GetInstance()->GetPathEngineData(PosTile);

				if (PosTile.y != -999.0f && PosTile.y != RwRealMAXVAL)
				{
					CNtlWorldPathEngineManager::GetInstance()->SetPathEngineData(PosTile, PeHeightPopup.m_fValue);
				}
			}
		}

		if(m_pBrushSelectorPE)
		{
			NTL_DELETE(m_pBrushSelectorPE);
		}
	}

	if (dGET_MOUSE()->GetWheelMovement())
	{
		int iValue = static_cast<int>(dGET_MOUSE()->GetWheelMovement() / ::fabsf(dGET_MOUSE()->GetWheelMovement()));

		CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTileEx<CNtlWorldPathEngineManager>*>(m_pBrushSelectorPE->GetController());
		vector<RwInt32> vecTileContainer = pCurBrush->GetTileContainer();

		RwInt32 CntX;
		RwInt32 CntZ;
		RwInt32 NumWorldTile	= dGET_WORLD_PARAM()->WorldSize / CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		RwInt32 LenTile			= CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		RwV3d	PosTile;
		DBO_WARNING_MESSAGE("NumWorldTile: " << NumWorldTile << "dGET_WORLD_PARAM()->WorldSize: " << dGET_WORLD_PARAM()->WorldSize << " Chunk: " << CNtlWorldPathEngineManager::GetInstance()->GetChunkSize());
		for(RwUInt32 i = 0; i < vecTileContainer.size(); ++i)
		{
			CntX		= vecTileContainer[i] - ((vecTileContainer[i] / NumWorldTile) * NumWorldTile);
			CntZ		= vecTileContainer[i] / NumWorldTile;
			PosTile.x	= static_cast<RwReal>(CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
			PosTile.z	= static_cast<RwReal>(CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
			PosTile.y   = CNtlWorldPathEngineManager::GetInstance()->GetPathEngineData(PosTile);

			if (PosTile.y != -999.0f && PosTile.y != RwRealMAXVAL)
			{
				if (dKEY(DIK_LSHIFT))
				{
					CNtlWorldPathEngineManager::GetInstance()->SetPathEngineData(PosTile, PosTile.y + (static_cast<RwReal>(iValue) * 0.1f));
				}
				else
				{
					CNtlWorldPathEngineManager::GetInstance()->SetPathEngineData(PosTile, PosTile.y + static_cast<RwReal>(iValue));
				}				
			}
		}
	}
}

void CWorldEditorEventHander::SetAnimatibleDoodadsDummyVision(BOOL _Flag)
{
	CNtlWorldFieldManager*	pMgr	= dGETMGR();
	CNtlWorldField*			pField	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	CNtlPLObject*			pNPObj;

	if(_Flag)
	{
		for(int j = 0; j < 36; ++j)
		{
			int		SectorIdx;
			RwV3d	SectorSPos;
			int		IdxField	= pMgr->m_Fields6x6[1][j];

			if(IdxField == -1)
			{
				DBO_TRACE(FALSE, "CWorldEditorEventHander::SetAnimatibleDoodadsDummyVision, invalid field index.");
				continue;
			}

			RwV3d SPos = pField[IdxField].GetSPos();

			for(int l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				for(int m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
				{
					SectorSPos.x = (RwReal)m;
					SectorSPos.z = (RwReal)l;
					SectorIdx = pMgr->GetSectorIdx(SectorSPos);

					int NumDoodads	= pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntityOrnament.size();
					for(int k = 0; k < NumDoodads; ++k)
					{
						pNPObj = static_cast<CNtlPLObject*>(pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntityOrnament[k]);
						if(pNPObj->GetPlayAnim())
						{
							RwV3d Pos;
							CNtlMath::MathRwV3dAssign(&Pos, pNPObj->GetPosition().x, pNPObj->GetPosition().y, pNPObj->GetPosition().z);

							SPLEntityCreateParam PLEntityCreateParam;
							PLEntityCreateParam.pPos = &Pos;

							CNtlPLObject *pNtlPLObject = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "FLY_DUMMY", &PLEntityCreateParam));
							DBO_ASSERT(pNtlPLObject, "Entity create failed.");

							pNtlPLObject->SetFadeEnable(FALSE);

							RwV3d Scale;
							Scale.x = 8.0f;
							Scale.y = 8.0f;
							Scale.z = 8.0f;
							pNtlPLObject->SetScale(&Scale);
							m_vecAnimatibleDoodadDummy.push_back(pNtlPLObject);
						}
					}
				}
			}
		}
	}
	else
	{
		for(int i = 0; i < static_cast<int>(m_vecAnimatibleDoodadDummy.size()); ++i)
		{
			GetSceneManager()->DeleteEntity(m_vecAnimatibleDoodadDummy[i]);
		}

		m_vecAnimatibleDoodadDummy.clear();
	}

}

VOID CWorldEditorEventHander::SetSEVision(RwBool _Flag)
{
	CNtlWorldFieldManager*	pMgr	= dGETMGR();
	CNtlWorldField*			pField	= const_cast<CNtlWorldField*>(pMgr->GetFields());

	for(int j = 0; j < 36; ++j)
	{
		int		SectorIdx;
		RwV3d	SectorSPos;
		int		IdxField	= pMgr->m_Fields6x6[1][j];

		if (IdxField != -1)
		{
			RwV3d	SPos		= pField[IdxField].GetSPos();
			for(int l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				for(int m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
				{
					SectorSPos.x = (RwReal)m;
					SectorSPos.z = (RwReal)l;
					SectorIdx = pMgr->GetSectorIdx(SectorSPos);

					if(SectorIdx == -1)
					{
						continue;
					}

					int NumSE = pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntitySound.size();
					for(int k = 0; k < NumSE; ++k)
					{
						static_cast<CNtlPLSound*>(pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntitySound[k])->SetVision(_Flag);
					}

					int NumBGM = pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntitySoundBGM.size();
					for(int k = 0; k < NumBGM; ++k)
					{
						static_cast<CNtlPLSoundBGM*>(pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntitySoundBGM[k])->SetVision(_Flag);
					}
				}
			}
		}
	}
}

VOID CWorldEditorEventHander::SetWLVision(RwBool _Flag)
{
	CNtlWorldFieldManager*	pMgr	= dGETMGR();
	CNtlWorldField*			pField	= const_cast<CNtlWorldField*>(pMgr->GetFields());

	for(int j = 0; j < 36; ++j)
	{
		int		SectorIdx;
		RwV3d	SectorSPos;
		int		IdxField	= pMgr->m_Fields6x6[1][j];

		if (IdxField != -1)
		{
			RwV3d	SPos		= pField[IdxField].GetSPos();
			for(int l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				for(int m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
				{
					SectorSPos.x = (RwReal)m;
					SectorSPos.z = (RwReal)l;
					SectorIdx = pMgr->GetSectorIdx(SectorSPos);

					if(SectorIdx == -1)
					{
						continue;
					}

					int Num;

					Num = pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntityWorldLight_Sphere.size();
					for(int k = 0; k < Num; ++k)
					{
						static_cast<CNtlPLWorldLight_Sphere*>(pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntityWorldLight_Sphere[k])->SetVision(_Flag);
					}
					Num = pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntityWorldLight_Box.size();
					for(int k = 0; k < Num; ++k)
					{
						static_cast<CNtlPLWorldLight_Box*>(pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntityWorldLight_Box[k])->SetVision(_Flag);
					}
				}
			}
		}
	}
}

VOID CWorldEditorEventHander::SetEffectsVision(RwBool Flag)
{
	CNtlWorldFieldManager*	pMgr	= dGETMGR();
	CNtlWorldField*			pField	= const_cast<CNtlWorldField*>(pMgr->GetFields());

	for(int j = 0; j < 36; ++j)
	{
		int		SectorIdx;
		RwV3d	SectorSPos;
		int		IdxField	= pMgr->m_Fields6x6[1][j];

		if (IdxField == -1)
		{
			continue;
		}

		RwV3d	SPos		= pField[IdxField].GetSPos();

		for(int l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(int m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x = (RwReal)m;
				SectorSPos.z = (RwReal)l;
				SectorIdx = pMgr->GetSectorIdx(SectorSPos);

				if (SectorIdx == -1)
				{
					continue;
				}

				int NumDoodads	= pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntityEffect.size();
				for(int k = 0; k < NumDoodads; ++k)
				{
					pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntityEffect[k]->SetVisible(static_cast<RwBool>(Flag));
				}
			}
		}
	}
}

VOID CWorldEditorEventHander::SetDoodadsVision(BOOL Flag)
{
	CNtlWorldFieldManager*	pMgr	= dGETMGR();
	CNtlWorldField*			pField	= const_cast<CNtlWorldField*>(pMgr->GetFields());

	for(int j = 0; j < 36; ++j)
	{
		int		SectorIdx;
		RwV3d	SectorSPos;
		int		IdxField	= pMgr->m_Fields6x6[1][j];
		RwV3d	SPos		= pField[IdxField].GetSPos();

		for(int l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(int m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x = (RwReal)m;
				SectorSPos.z = (RwReal)l;
				SectorIdx = pMgr->GetSectorIdx(SectorSPos);

				int NumDoodads	= pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntityOrnament.size();
				for(int k = 0; k < NumDoodads; ++k)
				{
					pMgr->m_pSectors[SectorIdx].m_vecNtlPLEntityOrnament[k]->SetVisible(static_cast<RwBool>(Flag));
				}
			}
		}
	}
}

CNtlPLObject* CWorldEditorEventHander::GetCurDoodad()
{
	if (m_vecSelectedObj.empty())
	{
		return NULL;
	}
	return static_cast<CNtlPLObject*>(m_vecSelectedObj[0]);
}

CNtlInstanceEffect* CWorldEditorEventHander::GetCurEffect()
{
	if (m_vecSelectedEffect.empty())
	{
		return NULL;
	}
	return static_cast<CNtlInstanceEffect*>(m_vecSelectedEffect[0]);
}

sNTL_DOODADS_PROP_PARAM	CWorldEditorEventHander::GetCurDoodadsPropParam()
{
	sNTL_DOODADS_PROP_PARAM	sNtlDoodadsPropParam;

	// serial
	sNtlDoodadsPropParam.SerialID			= m_vecSelectedObj[0]->GetSID();

	// milepost GUID
	sNtlDoodadsPropParam._MilePostEnable	= (static_cast<CNtlPLObject*>(m_vecSelectedObj[0])->GetProperty()->GetObjectType() == E_OBJECT_MILEPOST);
	sNtlDoodadsPropParam._MilePostID		= static_cast<CNtlPLObject*>(m_vecSelectedObj[0])->GetMilepostID();

	// AABB
	RwBBox AABB = (static_cast<CNtlPLObject*>(m_vecSelectedObj[0]))->GetTriggerAABBInfo();
	CNtlMath::MathRwV3dAssign(&sNtlDoodadsPropParam.AABB[0], AABB.inf.x, AABB.inf.y, AABB.inf.z);
	CNtlMath::MathRwV3dAssign(&sNtlDoodadsPropParam.AABB[1], AABB.sup.x, AABB.sup.y, AABB.sup.z);

	strcpy_s(sNtlDoodadsPropParam._Name, 64, m_vecSelectedObj[0]->GetName());
	sNtlDoodadsPropParam.VisibilityDist		= m_vecSelectedObj[0]->GetVisibleCullingDistance();
	sNtlDoodadsPropParam.EnableTrigger		= m_vecSelectedObj[0]->GetFlags() & NTL_PLEFLAG_TRIGGER;
	sNtlDoodadsPropParam.EnableSelection	= m_vecSelectedObj[0]->GetFlags() & NTL_PLEFLAG_SELECTION;
	sNtlDoodadsPropParam.EnableAlterable	= m_vecSelectedObj[0]->GetFlags() & NTL_PLEFLAG_ALTERABLE;
	sNtlDoodadsPropParam.EnablePortal		= m_vecSelectedObj[0]->GetFlags() & NTL_PLEFLAG_PORTAL;
	sNtlDoodadsPropParam.EnablePECollision	= m_vecSelectedObj[0]->GetFlags() & NTL_PLEFLAG_PECOLLISION;
	sNtlDoodadsPropParam.EnableShadowProp	= m_vecSelectedObj[0]->GetFlags() & NTL_PLEFLAG_SHADOW_PROP;
	sNtlDoodadsPropParam.NameIdx			= m_vecSelectedObj[0]->GetObjNameIdx();

	return sNtlDoodadsPropParam;
}

RpAtomic* CWorldEditorEventHander::SECircleUIDrawAllAtomicCB(RpAtomic* _pAtomic, void* _pData)
{
	CNtlPLEntity*				pPLEntity	=	static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(_pAtomic));
	CWorldEditorEventHander*	pWEEH		=	static_cast<CWorldEditorEventHander*>(_pData);

	if(	((pPLEntity->GetClassType() == PLENTITY_SOUND) || (pPLEntity->GetClassType() == PLENTITY_SOUND_BGM)) &&
		(pPLEntity != dGET_BRUSH_ENTITY()))
	{
		if(pWEEH->m_vecSelectedSE.size())
		{
			if(pWEEH->m_vecSelectedSE[0] != pPLEntity)
			{
				pWEEH->m_pNtlPLSE2RenderUI = pPLEntity;
			}
		}
		else
		{
			pWEEH->m_pNtlPLSE2RenderUI = pPLEntity;
		}

		return _pAtomic;
	}
	else
	{
		pWEEH->m_pNtlPLSE2RenderUI = NULL;

		return _pAtomic;
	}

	return _pAtomic;
}

RpClump* CWorldEditorEventHander::SECircleUIDrawAllClumpIndoorCB(RpClump* _pClump, void* _pData)
{
	CNtlWorldFieldManager*		pMgr	= dGETMGR();
	CWorldEditorEventHander*	pWEEH	= static_cast<CWorldEditorEventHander*>(_pData);

	pWEEH->m_pNtlPLSE2RenderUI = NULL;
	RpClumpForAllAtomics(_pClump, SECircleUIDrawAllAtomicCB, _pData);

	if (pWEEH->m_pNtlPLSE2RenderUI)
	{
		static RwRGBA	rgba	= { 255, 255, 255, 255 };
		RwMatrix		mat		= CNtlPLGlobal::m_matIden;
		*RwMatrixGetPos(&mat) = pWEEH->m_pNtlPLSE2RenderUI->GetPosition();

		RenderGeometryBillboardSphere(&mat, &rgba, (static_cast<CNtlPLSound*>(pWEEH->m_pNtlPLSE2RenderUI))->GetRadius(0), CNtlPLGlobal::m_RwCamera, TRUE);
		RenderGeometryBillboardSphere(&mat, &rgba, (static_cast<CNtlPLSound*>(pWEEH->m_pNtlPLSE2RenderUI))->GetRadius(1), CNtlPLGlobal::m_RwCamera, TRUE);
	}

	return _pClump;
}

RpClump* CWorldEditorEventHander::SECircleUIDrawAllClumpCB(RpClump* _pClump, void* _pData)
{
	CNtlWorldFieldManager*		pMgr	= dGETMGR();
	CWorldEditorEventHander*	pWEEH	= static_cast<CWorldEditorEventHander*>(_pData);

	pWEEH->m_pNtlPLSE2RenderUI = NULL;
	RpClumpForAllAtomics(_pClump, SECircleUIDrawAllAtomicCB, _pData);

	if(pWEEH->m_pNtlPLSE2RenderUI)
	{
		if(!pWEEH->m_pBrushSelection)
		{
			pWEEH->m_pBrushSelection				= NTL_NEW CNtlWorldBrush;
			CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
			pWEEH->m_pBrushSelection->SetController(pController);
		}

		if(!pWEEH->m_pBrushSelectionSub)
		{
			pWEEH->m_pBrushSelectionSub				= NTL_NEW CNtlWorldBrush;
			CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
			pWEEH->m_pBrushSelectionSub->SetController(pController);
		}

		CNtlWB_Circle<CNtlWorldFieldManager>* pCurBrush	= reinterpret_cast<CNtlWB_Circle<CNtlWorldFieldManager>*>(pWEEH->m_pBrushSelection->GetController());
		RwV3d Scale;
		Scale.x = 1.0f;
		Scale.y = 1.0f;
		Scale.z = 1.0f;

		pCurBrush->SetColor(0xffffffff);
		pCurBrush->Render(pWEEH->m_pNtlPLSE2RenderUI->GetPosition(), (static_cast<CNtlPLSound*>(pWEEH->m_pNtlPLSE2RenderUI))->GetRadius(0), Scale);

		pCurBrush = reinterpret_cast<CNtlWB_Circle<CNtlWorldFieldManager>*>(pWEEH->m_pBrushSelectionSub->GetController());
		pCurBrush->SetColor(0xffffffff);
		pCurBrush->Render(pWEEH->m_pNtlPLSE2RenderUI->GetPosition(), (static_cast<CNtlPLSound*>(pWEEH->m_pNtlPLSE2RenderUI))->GetRadius(1), Scale);
	}

	return _pClump;
}

VOID CWorldEditorEventHander::RefreshPerformanceTextInfo()
{
	CNtlWorldFieldManager*	pMgr		= dGETMGR();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwV3d					Pos			= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwInt32					CurFieldIdx	= pMgr->GetFieldIdx(Pos);

	if(CurFieldIdx == -1)
	{
		return;
	}

	// Reset performance texts
	m_CntDoodad[0]	=	0;
	m_CntDoodad[1]	=	0;
	m_CntEffect[0]	=	0;
	m_CntEffect[1]	=	0;
	m_CntWater[0]	=	0;
	m_CntWater[1]	=	0;

	// Count current field info.
	RwV3d	SPos = pFields[CurFieldIdx].GetSPos();
	RwV3d	PosSectorS;

	for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			PosSectorS.x = (RwReal)m;
			PosSectorS.z = (RwReal)l;

			RwInt32 CurSectorBlockIdx = pMgr->CNtlWorldSectorManager::GetSectorIdx(PosSectorS);

			if(CurSectorBlockIdx != -1)
			{
				m_CntDoodad[0]	+= pMgr->CNtlWorldSectorManager::m_pSectors[CurSectorBlockIdx].m_vecNtlPLEntityOrnament.size();
				m_CntEffect[0]	+= pMgr->CNtlWorldSectorManager::m_pSectors[CurSectorBlockIdx].m_vecNtlPLEntityEffect.size();

				if(pMgr->CNtlWorldSectorManager::m_pSectors[CurSectorBlockIdx].m_pWater)
				{
					++m_CntWater[0];
				}
			}
		}
	}

	// Count all fields info.
	for(RwUInt32 i = 0; i < 36; ++i)
	{
		RwInt32 CurFieldBlockIdx = pMgr->m_Fields6x6[1][i];
		if(CurFieldBlockIdx != -1)
		{
			RwV3d	SPos = pFields[CurFieldBlockIdx].GetSPos();
			RwV3d	PosSectorS;

			for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
				{
					PosSectorS.x = (RwReal)m;
					PosSectorS.z = (RwReal)l;

					RwInt32 CurSectorBlockIdx = pMgr->CNtlWorldSectorManager::GetSectorIdx(PosSectorS);

					if(CurSectorBlockIdx != -1)
					{
						m_CntDoodad[1]	+= pMgr->CNtlWorldSectorManager::m_pSectors[CurSectorBlockIdx].m_vecNtlPLEntityOrnament.size();
						m_CntEffect[1]	+= pMgr->CNtlWorldSectorManager::m_pSectors[CurSectorBlockIdx].m_vecNtlPLEntityEffect.size();

						if(pMgr->CNtlWorldSectorManager::m_pSectors[CurSectorBlockIdx].m_pWater)
						{
							++m_CntWater[1];
						}
					}
				}
			}
		}
	}
}

void CWorldEditorEventHander::RenderDisplayInfo(RtCharset* pRtCharSet, RwInt32 LenWindowWidth, RwInt32 LenWindowHeight)
{
	if(dGET_WORLD_PARAM()->FlgObjInfoShow)
	{
		RwChar	caption[256];
		RwV3d	v2dPoint;
		RwRGBA	ClrFont; ClrFont.alpha = 255;

		RenderDisplayInfoDojoPalette(pRtCharSet, LenWindowWidth, LenWindowHeight);

		if (m_pSelectedOccluder)
		{
			RwV3d		vPos = m_pSelectedOccluder->GetPosition();
			RwInt32		iNum = CNtlOccluderManager::GetInstance()->GetOccluderCulledNum(m_pSelectedOccluder);

			RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
			RwV3dTransformPoints(&v2dPoint, &vPos, 1, pMatView);
			v2dPoint.x = (v2dPoint.x / v2dPoint.z) * LenWindowWidth + 10.0f;
			v2dPoint.y = (v2dPoint.y / v2dPoint.z) * LenWindowHeight + 10.0f;

			ClrFont.red		= 255;
			ClrFont.green	= 0;
			ClrFont.blue	= 0;
			RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
			RsSprintf(caption, RWSTRING("P : %.2f, %.2f, %.2f"), vPos.x, vPos.y, vPos.z);
			RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);

			ClrFont.red		= 0;
			ClrFont.green	= 255;
			ClrFont.blue	= 0;
			RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
			RsSprintf(caption, RWSTRING("C : %d"), iNum);
			RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 15);

			RtCharsetSetColors(pRtCharSet, &dGETAPP().m_ForegroundColor, &dGETAPP().m_BackgroundColor);
		}

		if(m_vecSelectedWorldLight.size())
		{
			for(unsigned int i = 0; i < m_vecSelectedWorldLight.size(); ++i)
			{
				CNtlPLWorldLight_Base*	pWorldLight	= static_cast<CNtlPLWorldLight_Base*>(m_vecSelectedWorldLight[i]);
				RwV3d					vPos		= pWorldLight->GetPosition();
				RwReal					fRadius		= pWorldLight->GetRadius();

				RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
				RwV3dTransformPoints(&v2dPoint, &vPos, 1, pMatView);
				v2dPoint.x = (v2dPoint.x / v2dPoint.z) * LenWindowWidth + 10.0f;
				v2dPoint.y = (v2dPoint.y / v2dPoint.z) * LenWindowHeight + 10.0f;

				ClrFont.red		= 255;
				ClrFont.green	= 0;
				ClrFont.blue	= 0;
				RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
				RsSprintf(caption, RWSTRING("P : %.2f, %.2f, %.2f"), vPos.x, vPos.y, vPos.z);
				RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);

				ClrFont.red		= 0;
				ClrFont.green	= 255;
				ClrFont.blue	= 0;
				RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
				RsSprintf(caption, RWSTRING("R : %.2f"), fRadius);
				RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 15);

				RtCharsetSetColors(pRtCharSet, &dGETAPP().m_ForegroundColor, &dGETAPP().m_BackgroundColor);
			}
		}

		if(m_vecSelectedObj.size())
		{
			for(unsigned int i = 0; i < m_vecSelectedObj.size(); ++i)
			{
				CNtlPLObject*	pNtlPLObject	= static_cast<CNtlPLObject*>(m_vecSelectedObj[i]);
				RwV3d			Pos				= pNtlPLObject->GetPosition();
				const RwV3d*    pScale			= pNtlPLObject->GetScale();
				const RwV3d*	pRot			= pNtlPLObject->GetRotate();
				RwBool			IsRedClr4VD		= (static_cast<RwInt32>(pNtlPLObject->GetVisibleCullingDistance()) >= 512 ? TRUE : FALSE);

				RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
				RwV3dTransformPoints(&v2dPoint, &Pos, 1, pMatView);
				v2dPoint.x = (v2dPoint.x / v2dPoint.z) * LenWindowWidth + 10.0f;
				v2dPoint.y = (v2dPoint.y / v2dPoint.z) * LenWindowHeight + 10.0f;

				ClrFont.red		= 255;
				ClrFont.green	= 255;
				ClrFont.blue	= 255;
				RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
				RsSprintf(caption, RWSTRING("[ %s ]"), pNtlPLObject->GetName());
				RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);

				ClrFont.red		= 255;
				ClrFont.green	= 0;
				ClrFont.blue	= 0;
				RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
				RsSprintf(caption, RWSTRING("S : %.2f, %.2f, %.2f"), pScale->x, pScale->y,pScale->z);
				RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 15);

				ClrFont.red		= 0;
				ClrFont.green	= 255;
				ClrFont.blue	= 0;
				RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
				RsSprintf(caption, RWSTRING("R : %.2f, %.2f, %.2f"), pRot->x, pRot->y, pRot->z);
				RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 30);

				ClrFont.red		= 0;
				ClrFont.green	= 0;
				ClrFont.blue	= 255;
				RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
				RsSprintf(caption, RWSTRING("T : %.2f, %.2f, %.2f"), Pos.x, Pos.y, Pos.z);
				RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 45);

				if(IsRedClr4VD)
				{
					ClrFont.red		= 255;
					ClrFont.green	= 0;
					ClrFont.blue	= 0;
				}
				else
				{
					ClrFont.red		= 255;
					ClrFont.green	= 255;
					ClrFont.blue	= 255;
				}

				RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
				RsSprintf(caption, RWSTRING("VD(%d)"), static_cast<RwInt32>(pNtlPLObject->GetVisibleCullingDistance()));
				RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 60);

				RtCharsetSetColors(pRtCharSet, &dGETAPP().m_ForegroundColor, &dGETAPP().m_BackgroundColor);
			}
		}

		if(m_vecSelectedEffect.size())
		{
			if(m_vecSelectedEffect[0])
			{
				RwV3d			Pos		= m_vecSelectedEffect[0]->GetPosition();
				const RwReal    Scale	= m_vecSelectedEffect[0]->GetScale();
				const RwV3d*	pRot	= m_vecSelectedEffect[0]->GetRotate();
				const RwReal	VDist	= m_vecSelectedEffect[0]->GetVisibleCullingDistance();

				RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
				RwV3dTransformPoints(&v2dPoint, &Pos, 1, pMatView);
				v2dPoint.x = (v2dPoint.x / v2dPoint.z) * LenWindowWidth + 10.0f;
				v2dPoint.y = (v2dPoint.y / v2dPoint.z) * LenWindowHeight + 10.0f;

				ClrFont.red		= 255;
				ClrFont.green	= 255;
				ClrFont.blue	= 255;
				RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
				RsSprintf(caption, RWSTRING("[ %s ]"), m_vecSelectedEffect[0]->GetName());
				RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);

				ClrFont.red		= 255;
				ClrFont.green	= 0;
				ClrFont.blue	= 0;
				RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
				RsSprintf(caption, RWSTRING("S : %.2f, %.2f, %.2f"), Scale, Scale, Scale);
				RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 15);

				ClrFont.red		= 0;
				ClrFont.green	= 255;
				ClrFont.blue	= 0;
				RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
				RsSprintf(caption, RWSTRING("R : %.2f, %.2f, %.2f"), pRot->x, pRot->y, pRot->z);
				RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 30);

				ClrFont.red		= 0;
				ClrFont.green	= 0;
				ClrFont.blue	= 255;
				RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
				RsSprintf(caption, RWSTRING("T : %.2f, %.2f, %.2f"), Pos.x, Pos.y, Pos.z);
				RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 45);

				ClrFont.red		= 255;
				ClrFont.green	= 255;
				ClrFont.blue	= 255;
				RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
				RsSprintf(caption, RWSTRING("VD : %.2f"), VDist);
				RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 60);

				RtCharsetSetColors(pRtCharSet, &dGETAPP().m_ForegroundColor, &dGETAPP().m_BackgroundColor);
			}
		}

		// CZ-SS
		// 		if(m_SelectedSS)
		// 		{
		// 			RwV3d	Pos		= m_SelectedSS->GetPosition();
		// 			RwV3d*	pRot	= m_SelectedSS->GetRotate();
		// 
		// 			RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
		// 			RwV3dTransformPoints(&v2dPoint, &Pos, 1, pMatView);
		// 			v2dPoint.x = (v2dPoint.x / v2dPoint.z) * LenWindowWidth + 10.0f;
		// 			v2dPoint.y = (v2dPoint.y / v2dPoint.z) * LenWindowHeight + 10.0f;
		// 
		// 			ClrFont.red		= 255;
		// 			ClrFont.green	= 255;
		// 			ClrFont.blue	= 255;
		// 			RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
		// 			RsSprintf(caption, RWSTRING("POS : %.2f, %.2f, %.2f"), Pos.x, Pos.y, Pos.z);
		// 			RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);
		// 
		// 			ClrFont.red		= 255;
		// 			ClrFont.green	= 255;
		// 			ClrFont.blue	= 255;
		// 			RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
		// 			RsSprintf(caption, RWSTRING("ROT : %.2f, %.2f, %.2f"), pRot->x, pRot->y, pRot->z);
		// 			RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 15);
		// 
		// 			ClrFont.red		= 255;
		// 			ClrFont.green	= 0;
		// 			ClrFont.blue	= 0;
		// 			RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
		// 			RsSprintf(caption, RWSTRING("ANG : %.2f"), m_SelectedSS->GetAngle());
		// 			RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 30);
		// 
		// 			ClrFont.red		= 0;
		// 			ClrFont.green	= 255;
		// 			ClrFont.blue	= 0;
		// 			RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
		// 			RsSprintf(caption, RWSTRING("RAD : %.2f"), m_SelectedSS->GetRadius());
		// 			RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 45);
		// 
		// 			ClrFont.red		= 0;
		// 			ClrFont.green	= 0;
		// 			ClrFont.blue	= 255;
		// 			RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
		// 			RsSprintf(caption, RWSTRING("CLR : %d, %d, %d"),	(RwInt32)(m_SelectedSS->GetColor().red * 255.0f),
		// 				(RwInt32)(m_SelectedSS->GetColor().green * 255.0f),
		// 				(RwInt32)(m_SelectedSS->GetColor().blue * 255.0f));
		// 			RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 60);
		// 
		// 			RtCharsetSetColors(pRtCharSet, &dGETAPP().m_ForegroundColor, &dGETAPP().m_BackgroundColor);
		// 		}

		if(m_vecSelectedSE.size())
		{
			if(m_vecSelectedSE[0])
			{
				if (m_vecSelectedSE[0]->GetClassType() == PLENTITY_SOUND_BGM)
				{
					RwV3d Pos = m_vecSelectedSE[0]->GetPosition();

					RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
					RwV3dTransformPoints(&v2dPoint, &Pos, 1, pMatView);
					v2dPoint.x = (v2dPoint.x / v2dPoint.z) * LenWindowWidth + 10.0f;
					v2dPoint.y = (v2dPoint.y / v2dPoint.z) * LenWindowHeight + 10.0f;

					ClrFont.red		= 255;
					ClrFont.green	= 255;
					ClrFont.blue	= 255;
					RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
					RsSprintf(caption, RWSTRING("POS : %.2f, %.2f, %.2f"), Pos.x, Pos.y, Pos.z);
					RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);

					ClrFont.red		= 255;
					ClrFont.green	= 0;
					ClrFont.blue	= 0;
					RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
					RsSprintf(caption, RWSTRING("RADIUS : %.2f"), static_cast<CNtlPLSound*>(m_vecSelectedSE[0])->GetRadius(TRUE));
					RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 15);

					ClrFont.red		= 0;
					ClrFont.green	= 0;
					ClrFont.blue	= 0;
					RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
					RsSprintf(caption, RWSTRING("NAME : %s"), static_cast<CNtlPLSound*>(m_vecSelectedSE[0])->GetName());
					RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 30);

					RtCharsetSetColors(pRtCharSet, &dGETAPP().m_ForegroundColor, &dGETAPP().m_BackgroundColor);
				}
				else
				{
					RwV3d Pos = m_vecSelectedSE[0]->GetPosition();

					RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
					RwV3dTransformPoints(&v2dPoint, &Pos, 1, pMatView);
					v2dPoint.x = (v2dPoint.x / v2dPoint.z) * LenWindowWidth + 10.0f;
					v2dPoint.y = (v2dPoint.y / v2dPoint.z) * LenWindowHeight + 10.0f;

					ClrFont.red		= 255;
					ClrFont.green	= 255;
					ClrFont.blue	= 255;
					RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
					RsSprintf(caption, RWSTRING("POS : %.2f, %.2f, %.2f"), Pos.x, Pos.y, Pos.z);
					RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);

					ClrFont.red		= 255;
					ClrFont.green	= 0;
					ClrFont.blue	= 0;
					RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
					RsSprintf(caption, RWSTRING("RADIUS MIN : %.2f"), static_cast<CNtlPLSound*>(m_vecSelectedSE[0])->GetRadius(TRUE));
					RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 15);

					ClrFont.red		= 0;
					ClrFont.green	= 255;
					ClrFont.blue	= 0;
					RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
					RsSprintf(caption, RWSTRING("RADIUS MAX : %.2f"), static_cast<CNtlPLSound*>(m_vecSelectedSE[0])->GetRadius(FALSE));
					RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 30);

					ClrFont.red		= 0;
					ClrFont.green	= 0;
					ClrFont.blue	= 255;
					RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
					RsSprintf(caption, RWSTRING("VOL : %.2f"), static_cast<CNtlPLSound*>(m_vecSelectedSE[0])->GetVolume());
					RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 45);

					ClrFont.red		= 0;
					ClrFont.green	= 0;
					ClrFont.blue	= 0;
					RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);
					RsSprintf(caption, RWSTRING("NAME : %s"), static_cast<CNtlPLSound*>(m_vecSelectedSE[0])->GetName());
					RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 60);

					RtCharsetSetColors(pRtCharSet, &dGETAPP().m_ForegroundColor, &dGETAPP().m_BackgroundColor);
				}				
			}
		}

		m_DecalIOHandler.RenderText(pRtCharSet, LenWindowWidth, LenWindowHeight);
		GetSpawnInstance()->RenderText(pRtCharSet, LenWindowWidth, LenWindowHeight);
		GetSpawnInstance()->GetPathHandler()->RenderText(pRtCharSet, LenWindowWidth, LenWindowHeight);
		RtCharsetSetColors(pRtCharSet, &dGETAPP().m_ForegroundColor, &dGETAPP().m_BackgroundColor);
	}

	if(m_pFieldUIManager && m_pMergeUIManager)
	{
		m_pFieldUIManager->RenderText(pRtCharSet);
		m_pMergeUIManager->RenderText(pRtCharSet);
		m_PlanetIOHandler.RenderText(pRtCharSet, LenWindowWidth, LenWindowHeight);

		RtCharsetSetColors(pRtCharSet, &dGETAPP().m_ForegroundColor, &dGETAPP().m_BackgroundColor);
	}

	if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PVS_LOCKING))
	{
		CNtlWorldFieldManager* pFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

		RwV3d MPos;
		MPos.x = pFMgr->m_pSectors[CNtlWorldSectorPVS::m_LockedPVSSector].DatumPoint.x;
		MPos.z = pFMgr->m_pSectors[CNtlWorldSectorPVS::m_LockedPVSSector].DatumPoint.z;
		MPos.y = pFMgr->GetWorldSectorHeight(MPos);
		MPos.y += dGET_WORLD_PARAM()->WorldFarPlane / 2.0f;

		RwV3d v2dPoint;
		RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
		RwV3dTransformPoints(&v2dPoint, &MPos, 1, pMatView);
		v2dPoint.x = (v2dPoint.x / v2dPoint.z) * LenWindowWidth + 10.0f;
		v2dPoint.y = (v2dPoint.y / v2dPoint.z) * LenWindowHeight + 10.0f;

		RwRGBA ClrFont;
		RwChar caption[64];
		ClrFont.alpha	= 255;
		ClrFont.red		= 255;
		ClrFont.green	= 0;
		ClrFont.blue	= 0;
		RtCharsetSetColors(pRtCharSet, &ClrFont, &dGETAPP().m_BackgroundColor);

		RwInt32 FIdx = pFMgr->GetFieldIdx(MPos);
		RwInt32 SIdx = pFMgr->GetSectorIdx(MPos);
		RsSprintf(caption, RWSTRING("--> Current Locked PVS Sector(F[%d], S[%d])"), FIdx, SIdx);
		RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);

		RtCharsetSetColors(pRtCharSet, &dGETAPP().m_ForegroundColor, &dGETAPP().m_BackgroundColor);
	}
}

void CWorldEditorEventHander::RenderBrush()
{
	RenderTerrainPalette();	
	RenderTriggerUI();

	if(dGET_CUR_PALETTE() == ePM_WATER)
	{
		if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
		{
			if(m_pBrushSelector)
			{
				m_pBrushSelector->Render();
			}	
		}
	}
	else if(dGET_CUR_PALETTE() == ePM_SHADOW)
	{
		if(m_pBrushSelector)
		{
			m_pBrushSelector->Render();
		}	
	}
	else if(dGET_CUR_PALETTE() == ePM_PERFORMANCE_SECTORCULL)
	{
		if(m_pBrushSelector)
		{
			m_pBrushSelector->Render();
		}	
	}
	else if(dGET_CUR_PALETTE() == ePM_ORNAMENT)
	{
		if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD || CNtlPLWorldState::GetActiveMiniIndoor())
		{
			if(m_pBrushSelectorFrsutum)
			{
				m_pBrushSelectorFrsutum->Render();
			}
		}
		else if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
		{
			if(m_pBrushSelector)
			{
				m_pBrushSelector->Render();
			}
		}


		VerifyObjCaptured();

		int NumObj = static_cast<int>(m_vecSelectedObj.size());
		if(NumObj)
		{
			sBRUSHCB_PARAM Param;

			if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD || CNtlPLWorldState::GetActiveMiniIndoor())
			{
				for(int i = 0; i < NumObj; ++i)
				{
					Param.pThis  = this;
					Param.CurIdx = i;

					RpClumpForAllAtomics(static_cast<CNtlPLObject*>(m_vecSelectedObj[i])->GetClump(), AtomicBrushRenderIndoorCB, &Param);	
				}
			}
			else if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
			{
				for(int i = 0; i < NumObj; ++i)
				{
					Param.pThis  = this;
					Param.CurIdx = i;

					RpClumpForAllAtomics(static_cast<CNtlPLObject*>(m_vecSelectedObj[i])->GetClump(), AtomicBrushRenderCB, &Param);	
				}
			}			

// 			if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 			{
// 				for(int i = 0; i < NumObj; ++i)
// 				{
// 					Param.pThis  = this;
// 					Param.CurIdx = i;
// 
// 					RpClumpForAllAtomics(static_cast<CNtlPLObject*>(m_vecSelectedObj[i])->GetClump(), AtomicBrushRenderCB, &Param);	
// 				}
// 			}
// 			else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
// 			{
// 				for(int i = 0; i < NumObj; ++i)
// 				{
// 					Param.pThis  = this;
// 					Param.CurIdx = i;
// 
// 					RpClumpForAllAtomics(static_cast<CNtlPLObject*>(m_vecSelectedObj[i])->GetClump(), AtomicBrushRenderIndoorCB, &Param);	
// 				}
// 			}
		}

		if (m_vecSelectedObj.size() == 1 && m_pAttachBoneObject)
		{
			RwMatrix* pMat1;
			RwMatrix* pMat2;							

			std::vector<std::string> vecBoneName;
			GetCurDoodad()->GetBoneName(vecBoneName);

			pMat1 = m_pAttachBoneObject->GetBoneMatrix(vecBoneName.at(m_iAttachBoneIdx).c_str());							
			pMat2 = GetCurDoodad()->GetBoneMatrix(vecBoneName.at(m_iAttachBoneIdx).c_str());

			RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) FALSE);
			RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);

			RwRGBA rgba = { 255, 0 ,0 ,255 };
			RenderGeometryBillboardSphere(pMat1, &rgba, 1.0f, CNtlPLGlobal::m_RwCamera, TRUE);
			RenderGeometryBillboardSphere(pMat2, &rgba, 1.0f, CNtlPLGlobal::m_RwCamera, TRUE);

			dGETAPP().RenderText(*RwMatrixGetPos(pMat1), vecBoneName.at(m_iAttachBoneIdx).c_str(), rgba.red, rgba.green, rgba.blue);
			dGETAPP().RenderText(*RwMatrixGetPos(pMat2), vecBoneName.at(m_iAttachBoneIdx).c_str(), rgba.red, rgba.green, rgba.blue);

			RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
			RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) TRUE);
		}
	}
	else if(dGET_CUR_PALETTE() == ePM_EFFECT)
	{
		if(m_vecSelectedEffect.size())
		{
			CNtlWB_Circle<CNtlWorldFieldManager>* pCurBrush	= reinterpret_cast<CNtlWB_Circle<CNtlWorldFieldManager>*>(m_pBrushSelection->GetController());
			RwV3d Scale;
			Scale.x = 1.0f;
			Scale.y = 1.0f;
			Scale.z = 1.0f;
			pCurBrush->Render(m_vecSelectedEffect[0]->GetPosition(), 2.0f, Scale);
		}
	}
	else if(dGET_CUR_PALETTE() == ePM_SE)
	{		
		if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD || CNtlPLWorldState::GetActiveMiniIndoor())
		{
			RpWorldForAllClumps(CNtlPLGlobal::m_pRpWorld, SECircleUIDrawAllClumpIndoorCB, this);

			if(m_vecSelectedSE.size())
			{
				RwRGBA		rgba	= { 255, 0, 0, 255 };
				RwMatrix	mat		= CNtlPLGlobal::m_matIden;
				*RwMatrixGetPos(&mat) = m_vecSelectedSE[0]->GetPosition();

				RenderGeometryBillboardSphere(&mat, &rgba, (static_cast<CNtlPLSound*>(m_vecSelectedSE[0]))->GetRadius(0), CNtlPLGlobal::m_RwCamera, TRUE);
				RenderGeometryBillboardSphere(&mat, &rgba, (static_cast<CNtlPLSound*>(m_vecSelectedSE[0]))->GetRadius(1), CNtlPLGlobal::m_RwCamera, TRUE);
			}
		}
		else if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
		{
			// All SE Entity circle rendering
			RpWorldForAllClumps(CNtlPLGlobal::m_pRpWorld, SECircleUIDrawAllClumpCB, this);

			if(m_vecSelectedSE.size())
			{
				CNtlWB_Circle<CNtlWorldFieldManager>* pCurBrush	= reinterpret_cast<CNtlWB_Circle<CNtlWorldFieldManager>*>(m_pBrushSelection->GetController());
				RwV3d Scale;
				Scale.x = 1.0f;
				Scale.y = 1.0f;
				Scale.z = 1.0f;
				pCurBrush->SetColor(0xffff0000);
				pCurBrush->Render(m_vecSelectedSE[0]->GetPosition(), (static_cast<CNtlPLSound*>(m_vecSelectedSE[0]))->GetRadius(0), Scale);

				pCurBrush	= reinterpret_cast<CNtlWB_Circle<CNtlWorldFieldManager>*>(m_pBrushSelectionSub->GetController());
				pCurBrush->SetColor(0xffff0000);
				pCurBrush->Render(m_vecSelectedSE[0]->GetPosition(), (static_cast<CNtlPLSound*>(m_vecSelectedSE[0]))->GetRadius(1), Scale);
			}
		}

// 		if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 		{
// 			// All SE Entity circle rendering
// 			RpWorldForAllClumps(CNtlPLGlobal::m_pRpWorld, SECircleUIDrawAllClumpCB, this);
// 
// 			if(m_vecSelectedSE.size())
// 			{
// 				CNtlWB_Circle<CNtlWorldFieldManager>* pCurBrush	= reinterpret_cast<CNtlWB_Circle<CNtlWorldFieldManager>*>(m_pBrushSelection->GetController());
// 				RwV3d Scale;
// 				Scale.x = 1.0f;
// 				Scale.y = 1.0f;
// 				Scale.z = 1.0f;
// 				pCurBrush->SetColor(0xffff0000);
// 				pCurBrush->Render(m_vecSelectedSE[0]->GetPosition(), (static_cast<CNtlPLSound*>(m_vecSelectedSE[0]))->GetRadius(0), Scale);
// 
// 				pCurBrush	= reinterpret_cast<CNtlWB_Circle<CNtlWorldFieldManager>*>(m_pBrushSelectionSub->GetController());
// 				pCurBrush->SetColor(0xffff0000);
// 				pCurBrush->Render(m_vecSelectedSE[0]->GetPosition(), (static_cast<CNtlPLSound*>(m_vecSelectedSE[0]))->GetRadius(1), Scale);
// 			}
// 		}
// 		else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
// 		{
// 			RpWorldForAllClumps(CNtlPLGlobal::m_pRpWorld, SECircleUIDrawAllClumpIndoorCB, this);
// 
// 			if(m_vecSelectedSE.size())
// 			{
// 				RwRGBA		rgba	= { 255, 0, 0, 255 };
// 				RwMatrix	mat		= CNtlPLGlobal::m_matIden;
// 				*RwMatrixGetPos(&mat) = m_vecSelectedSE[0]->GetPosition();
// 
// 				RenderGeometryBillboardSphere(&mat, &rgba, (static_cast<CNtlPLSound*>(m_vecSelectedSE[0]))->GetRadius(0), CNtlPLGlobal::m_RwCamera, TRUE);
// 				RenderGeometryBillboardSphere(&mat, &rgba, (static_cast<CNtlPLSound*>(m_vecSelectedSE[0]))->GetRadius(1), CNtlPLGlobal::m_RwCamera, TRUE);
// 			}
// 		}
	}	
	else if (dGET_CUR_PALETTE() == ePM_PLANT)
	{
		CNtlWB_Circle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Circle<CNtlWorldFieldManager>*>(m_pBrushSelection->GetController());

		RwV3d vScale;	vScale.x = 1.0f;			vScale.y = 1.0f;			vScale.z = 1.0f;

		pCurBrush->SetColor(0xffffffff);
		pCurBrush->Render(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, dGET_WORLD_PARAM()->NumBrushRadiusInTile, vScale);
	}
	else if (dGET_CUR_PALETTE() == ePM_WORLDLIGHT)
	{
		if(m_vecSelectedWorldLight.size())
		{
			CNtlWB_Circle<CNtlWorldFieldManager>*	pCurBrush	= reinterpret_cast<CNtlWB_Circle<CNtlWorldFieldManager>*>(m_pBrushSelection->GetController());
			CNtlPLWorldLight_Base*					pWorldLight = (CNtlPLWorldLight_Base*)m_vecSelectedWorldLight[0];



			RwV3d vScale;	vScale.x = 1.0f;			vScale.y = 1.0f;			vScale.z = 1.0f;

			pCurBrush->SetColor(0xffffffff);
			pCurBrush->Render(pWorldLight->GetPosition(), pWorldLight->GetRadius(), vScale);
		}
	}
	CNtlWorldSectorPVS::RenderLockingGuideLine();
}

void CWorldEditorEventHander::Render()
{
	if(dGET_CUR_PALETTE() == ePM_WATER)
	{
		if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
		{
			if(m_pBrush2D)
			{
				CNtlWB_Circle2D<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Circle2D<CNtlWorldFieldManager>*>(m_pBrush2D->GetController());

				if(pCurBrush->GetEnable())
				{
					m_pBrush2D->Render();
				}
			}
		}
	}

	// CZ-SS
	// 	if(dGET_CUR_PALETTE() == ePM_SOFTSPOT)
	// 	{
	// 		CNtlWorldRpLightManager::GetInstance()->Render();
	// 	}

	RenderLightObjectPalette();
	// 	if (dGET_CUR_PALETTE() == ePM_LIGHTOBJECT)
	// 	{
	// 		CNtlWorldRpLightManager::GetInstance()->Render();
	// 	}

	if (dGET_CUR_PALETTE() == ePM_LIGHTMODE)
	{
		dGETFRM()->m_pLightModePalette->Render();
	}

	if(dGET_CUR_PALETTE() == ePM_SHORE_LINE)
	{
		m_ShoreLineIOHandler.Render();
	}

	if(dGET_CUR_PALETTE() == ePM_DECAL)
	{
		m_DecalIOHandler.Render();
	}

	if(dGET_CUR_PALETTE() == ePM_SHADOW)
	{
		CNtlWorldFieldManager *pMgr = dGETMGR();
		if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PSTEX) && m_vecSelectedShadowObj.size() == 1)
		{
			CNtlWorldShadowManager::GetInst()->RenderTexPanel(m_vecSelectedShadowObj, pMgr);
		}
	}

	if(dGET_CUR_PALETTE() == ePM_FIELDUI)
	{
		m_PlanetIOHandler.Render();

		UpdateAlphaState(TRUE);

		m_pFieldUIManager->Render();
		m_pMergeUIManager->Render();

		UpdateAlphaState(FALSE);
	}

	if(dGET_CUR_PALETTE() == ePM_ORNAMENT)
	{
		dGETFRM()->m_pOrnamentPalette->RenderPreview();
	}

	RenderDojoPalette();	

	if(dGET_CUR_PALETTE() == ePM_NM_PROP)
	{
		CNtlWorldFieldManager *pMgr = dGETMGR();

		if(m_pBrushSelector)
		{
			if(m_pTileAttrSelector)
			{
				UpdateAlphaState(TRUE);

				RwD3D9SetTexture(m_pTileAttrSelector, 0);
				m_pBrushSelector->Render();

				UpdateAlphaState(FALSE);
			}
		}

		if(m_vecTileAttr.size())
		{
			RwInt32 CntX;
			RwInt32 CntZ;
			RwInt32 NumWorldTile	= dGET_WORLD_PARAM()->WorldSize / dGET_WORLD_PARAM()->WorldSectorTileSize;
			RwReal	LenTile			= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
			RwV3d	PosRB, PosLT, PosLB, PosRT;

			RwD3D9SetTexture(m_pTileAttr, 0);

			for(RwUInt32 i = 0; i < m_vecTileAttr.size(); ++i)
			{
				CntX = m_vecTileAttr[i] - ((m_vecTileAttr[i] / NumWorldTile) * NumWorldTile);
				CntZ = m_vecTileAttr[i] / NumWorldTile;
				PosRB.x = CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf;
				PosRB.z = CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf;
				PosLT.x = PosRB.x + LenTile;
				PosLT.z = PosRB.z + LenTile;
				PosLB.x = PosRB.x + LenTile;
				PosLB.z = PosRB.z;
				PosRT.x = PosRB.x;
				PosRT.z = PosRB.z + LenTile;

				RwBool bIsMiniIndoor = FALSE;
				if (CNtlPLWorldState::GetActiveMiniIndoor())
				{
					RwV3d vTemp;
					vTemp.x = PosRB.x + (LenTile * 0.5f);
					vTemp.z = PosRB.z + (LenTile * 0.5f);				

					RwLine				rayIntersection;
					SWorldIntersect		sWorldIntersection;

					rayIntersection.start	= vTemp; rayIntersection.start.y = RpWorldGetBBox(CNtlPLGlobal::m_pRpWorld)->sup.y;
					rayIntersection.end		= vTemp; rayIntersection.end.y = RpWorldGetBBox(CNtlPLGlobal::m_pRpWorld)->inf.y;
					if (Collision_MiniIndoorIntersectionLineTopDown(rayIntersection, sWorldIntersection))
					{
						bIsMiniIndoor = TRUE;
						PosRB.y = PosLB.y = PosRT.y = PosLT.y = sWorldIntersection.vCollPos.y + 0.7f;
					}
				}

				if (!bIsMiniIndoor)
				{
					PosLT.y = pMgr->GetWorldSectorHeight(PosLT) + 0.7f;
					PosLB.y = pMgr->GetWorldSectorHeight(PosLB) + 0.7f;
					PosRT.y = pMgr->GetWorldSectorHeight(PosRT) + 0.7f;
					PosRB.y = pMgr->GetWorldSectorHeight(PosRB) + 0.7f;
				}

				// 0  2
				// 1  3
				RwIm3DVertexSetU(&m_VB[0], 0.0f);		
				RwIm3DVertexSetV(&m_VB[0], 0.0f);
				RwIm3DVertexSetU(&m_VB[1], 0.0f);		
				RwIm3DVertexSetV(&m_VB[1], 1.0f);
				RwIm3DVertexSetU(&m_VB[2], 1.0f);		
				RwIm3DVertexSetV(&m_VB[2], 0.0f);
				RwIm3DVertexSetU(&m_VB[3], 1.0f);		
				RwIm3DVertexSetV(&m_VB[3], 1.0f);
				RwIm3DVertexSetRGBA(&m_VB[0], 255, 255, 255, 255);
				RwIm3DVertexSetRGBA(&m_VB[1], 255, 255, 255, 255);
				RwIm3DVertexSetRGBA(&m_VB[2], 255, 255, 255, 255);
				RwIm3DVertexSetRGBA(&m_VB[3], 255, 255, 255, 255);
				RwIm3DVertexSetPos(&m_VB[0], PosLT.x, PosLT.y, PosLT.z);
				RwIm3DVertexSetPos(&m_VB[1], PosLB.x, PosLB.y, PosLB.z);
				RwIm3DVertexSetPos(&m_VB[2], PosRT.x, PosRT.y, PosRT.z);
				RwIm3DVertexSetPos(&m_VB[3], PosRB.x, PosRB.y, PosRB.z);

				if(RwIm3DTransform(m_VB, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);
					RwIm3DEnd();
				}
			}
		}
	}

	if(dGET_CUR_PALETTE() == ePM_DEFAULT && CNtlWorldPathEngineManager::GetInstance() && CNtlWorldPathEngineManager::GetInstance()->GetPathEngineMode())
	{
		CNtlWorldFieldManager *pMgr = dGETMGR();

		if(m_pBrushSelectorPE)
		{
			if(m_pTileAttrSelector)
			{
				BeginFogState(FALSE);

				UpdateAlphaState(TRUE);

				RwD3D9SetTexture(m_pTileAttrSelector, 0);
				m_pBrushSelectorPE->Render();

				UpdateAlphaState(FALSE);

				EndFogState();

			}
		}

		RwD3D9SetTexture(m_pTileAttr, 0);
		CNtlWorldPathEngineManager::GetInstance()->Render();
		RwD3D9SetTexture(NULL, 0);

		if(m_vecTileAttr.size())
		{
			RwInt32 CntX;
			RwInt32 CntZ;
			RwV3d	PosRB, PosLT, PosLB, PosRT;
			RwReal	LenTile	= static_cast<RwReal>(CNtlWorldPathEngineManager::GetInstance()->GetChunkSize());


			CntX	= static_cast<RwInt32>((dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x + dGET_WORLD_PARAM()->WorldSizeHalf) / LenTile);
			CntZ	= static_cast<RwInt32>((dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z + dGET_WORLD_PARAM()->WorldSizeHalf) / LenTile);
			PosRB.x = CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf;
			PosRB.z = CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf;

			RwReal fHeight = CNtlWorldPathEngineManager::GetInstance()->GetHeight(PosRB);

			if (fHeight != RwRealMAXVAL)
			{
				PosRB.y = fHeight + 1.1f;

				PosLT.x = PosRB.x + LenTile;
				PosLT.z = PosRB.z + LenTile;
				PosLT.y = PosRB.y;//CNtlWorldPathEngineManager::GetInstance()->GetHeight(PosLT) + 0.7f;
				PosLB.x = PosRB.x + LenTile;
				PosLB.z = PosRB.z;
				PosLB.y = PosRB.y; //CNtlWorldPathEngineManager::GetInstance()->GetHeight(PosLB) + 0.7f;
				PosRT.x = PosRB.x;
				PosRT.z = PosRB.z + LenTile;
				PosRT.y = PosRB.y; //CNtlWorldPathEngineManager::GetInstance()->GetHeight(PosRT) + 0.7f;

				// 0  2
				// 1  3
				RwIm3DVertexSetU(&m_VB[0], 0.0f);		
				RwIm3DVertexSetV(&m_VB[0], 0.0f);
				RwIm3DVertexSetU(&m_VB[1], 0.0f);		
				RwIm3DVertexSetV(&m_VB[1], 1.0f);
				RwIm3DVertexSetU(&m_VB[2], 1.0f);		
				RwIm3DVertexSetV(&m_VB[2], 0.0f);
				RwIm3DVertexSetU(&m_VB[3], 1.0f);		
				RwIm3DVertexSetV(&m_VB[3], 1.0f);
				RwIm3DVertexSetRGBA(&m_VB[0], 0, 255, 0, 255);
				RwIm3DVertexSetRGBA(&m_VB[1], 0, 255, 0, 255);
				RwIm3DVertexSetRGBA(&m_VB[2], 0, 255, 0, 255);
				RwIm3DVertexSetRGBA(&m_VB[3], 0, 255, 0, 255);
				RwIm3DVertexSetPos(&m_VB[0], PosLT.x, PosLT.y, PosLT.z);
				RwIm3DVertexSetPos(&m_VB[1], PosLB.x, PosLB.y, PosLB.z);
				RwIm3DVertexSetPos(&m_VB[2], PosRT.x, PosRT.y, PosRT.z);
				RwIm3DVertexSetPos(&m_VB[3], PosRB.x, PosRB.y, PosRB.z);

				if(RwIm3DTransform(m_VB, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);
					RwIm3DEnd();
				}
			}
		}
	}

	RenderThumbnail();

	RenderCompass();

	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		((CNtlWEFrm*)AfxGetMainWnd())->GetWorldView()->RenderMainView();
	}
}

void CWorldEditorEventHander::AddSelectedObject(CNtlPLEntity* pEntity)
{
	if (dGET_CUR_PALETTE() == ePM_ORNAMENT)
	{
		m_vecSelectedObj.push_back(pEntity);
	}
}

void CWorldEditorEventHander::AddSelectedEffect(CNtlPLEntity* pEntity)
{
	if (dGET_CUR_PALETTE() == ePM_EFFECT)
	{
		if(!m_pBrushSelection)
		{
			m_pBrushSelection						= NTL_NEW CNtlWorldBrush;
			CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, dGETMGR(), &CNtlWorldFieldManager::GetWorldSectorHeight);
			m_pBrushSelection->SetController(pController);
		}

		m_vecSelectedEffect.push_back(pEntity);
	}
}

void CWorldEditorEventHander::AddSelectedSE(CNtlPLEntity* pEntity)
{
	if (dGET_CUR_PALETTE() == ePM_SE)
	{
		if(!m_pBrushSelection)
		{
			m_pBrushSelection						= NTL_NEW CNtlWorldBrush;
			CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, dGETMGR(), &CNtlWorldFieldManager::GetWorldSectorHeight);
			m_pBrushSelection->SetController(pController);
		}

		if(!m_pBrushSelectionSub)
		{
			m_pBrushSelectionSub					= NTL_NEW CNtlWorldBrush;
			CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, dGETMGR(), &CNtlWorldFieldManager::GetWorldSectorHeight);
			m_pBrushSelectionSub->SetController(pController);
		}

		m_vecSelectedSE.push_back(pEntity);
	}
}

void CWorldEditorEventHander::AddSelectedWorldLight(CNtlPLEntity* pEntity)
{
	if (dGET_CUR_PALETTE() == ePM_WORLDLIGHT && pEntity)
	{
		if(!m_pBrushSelection)
		{
			m_pBrushSelection						= NTL_NEW CNtlWorldBrush;
			CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, dGETMGR(), &CNtlWorldFieldManager::GetWorldSectorHeight);
			m_pBrushSelection->SetController(pController);
		}

		m_vecSelectedWorldLight.clear();
		m_vecSelectedWorldLight.push_back(pEntity);
		dGETFRM()->m_pWorldLightPalette->UpdatePropertyData(pEntity);		
	}
}

void CWorldEditorEventHander::ApplyObjectProperty(sNTL_FIELD_PROP* pNtlFieldProp, eNTL_FIELD_PROPID eNtlFieldPropID)
{
	for (int i = 0; i < (int)m_vecSelectedObj.size(); ++i)
	{
		CNtlPLObject* pPLOjbect = (CNtlPLObject*)m_vecSelectedObj.at(i);
		if (pPLOjbect->GetObjectTypePtr() && pPLOjbect->GetObjectTypePtr()->GetFuncFlags() & EPL_OBJECT_FUNC_FLAG_PROPERTY)
		{
			CNtlPLObjectTypeHasProperty* pObjectType = (CNtlPLObjectTypeHasProperty*)pPLOjbect->GetObjectTypePtr();
			pObjectType->SetObjectProperty(*pNtlFieldProp, eNtlFieldPropID);
		}
	}
}

void CWorldEditorEventHander::RenderTriggerUI()
{
	if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_TRIGGER_INFO))
	{
		return;
	}

// 	RwV3d*					pvCamera			= RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
// 	CNtlWorldFieldManager*	pWorldFieldManager	= dGETMGR();
// 
// 	RwInt32					iSectorIdxCur = pWorldFieldManager->GetSectorIdx(*pvCamera);
// 	std::vector<RwInt32>	vecNeighborSectors;
// 
// 	if (iSectorIdxCur == -1)
// 	{
// 		return;
// 	}
// 
// 	pWorldFieldManager->GetNeighborSectors(iSectorIdxCur, vecNeighborSectors);
// 	vecNeighborSectors.push_back(iSectorIdxCur);
// 
// 	for (RwInt32 i = 0; i < (RwInt32)vecNeighborSectors.size(); ++i)
// 	{
// 		RwInt32 iSectorIdx = vecNeighborSectors.at(i); 
// 
// 		if (iSectorIdx == -1)
// 		{
// 			continue;
// 		}
// 
// 		CNtlWorldSector* pSector = &pWorldFieldManager->m_pSectors[iSectorIdx];
// 		for (RwInt32 i = 0; i < (RwInt32)pSector->m_vecNtlPLEntityOrnament.size(); ++i)
// 		{
// 			CNtlPLObject* pObject = static_cast<CNtlPLObject*>(pSector->m_vecNtlPLEntityOrnament.at(i));
// 			if (pObject->GetFlags() & NTL_PLEFLAG_TRIGGER)
// 			{
// 				RwSphere* pSphere = pObject->GetBoundingSphere();
// 				switch (GetSceneManager()->GetActiveWorldType())
// 				{
// 				case AW_HEGITHFIELD:
// 					{
// 						if(m_pBrushSelection == NULL)
// 						{
// 							m_pBrushSelection						= NTL_NEW CNtlWorldBrush;
// 							CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, pWorldFieldManager, &CNtlWorldFieldManager::GetWorldSectorHeight);
// 							m_pBrushSelection->SetController(pController);
// 						}
// 
// 						CNtlWB_Circle<CNtlWorldFieldManager>* pCurBrush	= reinterpret_cast<CNtlWB_Circle<CNtlWorldFieldManager>*>(m_pBrushSelection->GetController());
// 
// 						RwV3d			vScale	= { 1.0f, 1.0f, 1.0f };
// 						pCurBrush->Render(pSphere->center, pSphere->radius, vScale);
// 					}
// 					break;
// 				case AW_RWWORLD:
// 					{
// 						static RwMatrix	mat;
// 						static RwRGBA	rgba = { 255, 255, 255, 255 };								
// 
// 						RwMatrixTranslate(&mat, &pSphere->center, rwCOMBINEREPLACE);
// 						RenderGeometryBillboardSphere(&mat, &rgba, pSphere->radius, CNtlPLGlobal::m_RwCamera, TRUE);
// 					}
// 					break;
// 				}
// 
// 				RwV3d vObjectPos = pObject->GetPosition();
// 
// 				RwIm3DVertex im3DVertex[2];
// 				RwIm3DVertexSetPos(&im3DVertex[0], vObjectPos.x, vObjectPos.y, vObjectPos.z);
// 				RwIm3DVertexSetRGBA(&im3DVertex[0], 255, 0, 0, 255);
// 
// 				vObjectPos.y += 50.0f;
// 				RwIm3DVertexSetPos(&im3DVertex[1], vObjectPos.x, vObjectPos.y, vObjectPos.z);
// 				RwIm3DVertexSetRGBA(&im3DVertex[1], 255, 0, 0, 255);
// 
// 				RwD3D9SetTexture(NULL, 0);
// 				if (RwIm3DTransform(im3DVertex, 2, NULL, rwIM3D_VERTEXXYZ | rwIM3D_ALLOPAQUE))
// 				{
// 					RwIm3DRenderPrimitive(rwPRIMTYPEPOLYLINE);
// 					RwIm3DEnd();
// 				}
// 
// 				RwChar acTriggerInfo[256];
// 				sprintf_s(acTriggerInfo, 256, "Trigger:%s(%u)", pObject->GetName(), pObject->GetSID());
// 				dGETAPP().RenderText(vObjectPos, acTriggerInfo, 255, 0, 0);
// 			}
// 		}
// 	}


	RwV3d*					pvCamera			= RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	CNtlWorldFieldManager*	pWorldFieldManager	= dGETMGR();

	for (RwInt32 iFieldCnt = 0; iFieldCnt < 36; ++iFieldCnt)
	{
		RwInt32 iFieldIdx = pWorldFieldManager->m_Fields6x6[1][iFieldCnt];
		if (iFieldIdx == -1)
		{
			continue;
		}

		CNtlWorldField* pFields			= const_cast<CNtlWorldField*>(pWorldFieldManager->GetFields());
		RwV3d			vFieldStartPos	= pFields[iFieldIdx].GetSPos();
		for (RwInt32 z = (RwInt32)vFieldStartPos.z; z < (RwInt32)vFieldStartPos.z + dGET_WORLD_PARAM()->WorldFieldSize; z += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for (RwInt32 x = (RwInt32)vFieldStartPos.x; x < (RwInt32)vFieldStartPos.x + dGET_WORLD_PARAM()->WorldFieldSize; x += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				RwV3d vCurPos; vCurPos.x = (RwReal)x; vCurPos.z = (RwReal)z;

				RwInt32 iSectorIdx = pWorldFieldManager->GetSectorIdx(vCurPos);
				if (iSectorIdx == -1)
				{
					continue;
				}

				CNtlWorldSector* pSector = &pWorldFieldManager->m_pSectors[iSectorIdx];
				for (RwInt32 i = 0; i < (RwInt32)pSector->m_vecNtlPLEntityOrnament.size(); ++i)
				{
					CNtlPLObject* pObject = static_cast<CNtlPLObject*>(pSector->m_vecNtlPLEntityOrnament.at(i));
					if ((pObject->GetFlags() & NTL_PLEFLAG_TRIGGER))
					{
						RwSphere* pSphere = pObject->GetBoundingSphere();
						switch (GetSceneManager()->GetActiveWorldType())
						{
						case AW_HEGITHFIELD:
							{
								if(m_pBrushSelection == NULL)
								{
									m_pBrushSelection						= NTL_NEW CNtlWorldBrush;
									CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, pWorldFieldManager, &CNtlWorldFieldManager::GetWorldSectorHeight);
									m_pBrushSelection->SetController(pController);
								}

								CNtlWB_Circle<CNtlWorldFieldManager>* pCurBrush	= reinterpret_cast<CNtlWB_Circle<CNtlWorldFieldManager>*>(m_pBrushSelection->GetController());

								RwV3d			vScale	= { 1.0f, 1.0f, 1.0f };
								pCurBrush->Render(pSphere->center, pSphere->radius, vScale);
							}
							break;
						case AW_RWWORLD:
							{
								static RwMatrix	mat;
								static RwRGBA	rgba = { 255, 255, 255, 255 };								
								
								RwMatrixTranslate(&mat, &pSphere->center, rwCOMBINEREPLACE);
								RenderGeometryBillboardSphere(&mat, &rgba, pSphere->radius, CNtlPLGlobal::m_RwCamera, TRUE);
							}
							break;
						}

						RwV3d vObjectPos = pObject->GetPosition();
						
						RwIm3DVertex im3DVertex[2];
						RwIm3DVertexSetPos(&im3DVertex[0], vObjectPos.x, vObjectPos.y, vObjectPos.z);
						RwIm3DVertexSetRGBA(&im3DVertex[0], 255, 0, 0, 255);

						vObjectPos.y += 50.0f;
						RwIm3DVertexSetPos(&im3DVertex[1], vObjectPos.x, vObjectPos.y, vObjectPos.z);
						RwIm3DVertexSetRGBA(&im3DVertex[1], 255, 0, 0, 255);

						RwD3D9SetTexture(NULL, 0);
						if (RwIm3DTransform(im3DVertex, 2, NULL, rwIM3D_VERTEXXYZ | rwIM3D_ALLOPAQUE))
						{
							RwIm3DRenderPrimitive(rwPRIMTYPEPOLYLINE);
							RwIm3DEnd();
						}

						RwChar acTriggerInfo[256];
						sprintf_s(acTriggerInfo, 256, "%s(%u)", pObject->GetName(), pObject->GetSID());
						dGETAPP().RenderText(vObjectPos, acTriggerInfo, 255, 0, 0, TRUE);
					}
				}
			}
		}
	}
}