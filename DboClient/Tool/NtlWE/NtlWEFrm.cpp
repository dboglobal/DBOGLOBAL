// NtlWEFrm.cpp : CNtlWEFrm 클래스의 구현
//

#include "stdafx.h"
#include "afxwin.h"
#include "NtlWE.h"
#include "NtlWEDoc.h"
#include "NtlWEFrm.h"
#include "FileNewDlg.h"
#include "io.h"
#include "CameraAttrDlg.h"
#include "PaletteTerrain.h"
#include "PaletteOrnament.h"
#include "PaletteUnit.h"
#include "PaletteRegion.h"
#include "PaletteTrigger.h"
#include "PaletteFieldUI.h"
#include "PaletteWater.h"
#include "PaletteEffect.h"
#include "PaletteShadow.h"
#include "PaletteNMProp.h"
#include "NtlPLWorldEntity.h"
#include "NtlText.h"
#include "NtlWorldFieldManager.h"
#include "CameraAttrDlg.h"
#include "SplashScreenfx.h"
#include "NtlWorldDI.h"
#include "ntlworldinterface.h"
#include "MergeManager.h"
#include "NtlSoundManager.h"
#include "PaletteSE.h"
#include "ProgressWnd.h"
#include "NtlWorldMergeManager.h"
#include "PaletteSpawn.h"
#include "PaletteSpawnPath.h"
#include "PaletteSoftSpot.h"
#include "PaletteWorldLight.h"
#include "PaletteLightAndShadow.h"
#include "PalettePerformance.h"
#include "PalettePerformanceSectorCull.h"
#include "PalettePerformanceOcclusion.h"
#include "PalettePerformancePVS.h"
#include "PalettePerformanceImposter.h"
#include "PaletteShoreLine.h"
#include "PaletteWaterTab.h"
#include "PaletteOrnamentTabDlg.h"
#include "paletteDecalDlg.h"
#include "palettePlantDlg.h"
#include "PaletteSpawnSearch.h"
#include "PaletteSkyFloatingEntityDlg.h"
#include "NtlWorldFieldManager.h"
#include "NtlWorldSectorPVS.h"
#include "FieldSearchDlg.h"
#include "NtlPLWeatherHandler.h"
#include "NtlPLOptionManager.h"
#include "PaletteHeatHazeDlg.h"
#include "PathEngineExportDlg.h"
#include "SaberTestDlg.h"
#include "WorldViewDlg.h"
#include "PathMeshFindDlg.h"
#include "PaletteLightObject.h"
#include "PaletteLightMode.h"
#include "PaletteGameProperty.h"
#include "BookmarkDlg.h"
#include "NtlWorldLTManager.h"
#include "PaletteDecalVolumeDlg.h"
#include "PaletteDojoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNtlWEFrm

IMPLEMENT_DYNCREATE(CNtlWEFrm, CFrameWnd)

BEGIN_MESSAGE_MAP(CNtlWEFrm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	// 전역 도움말 명령
	//ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	//ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	//ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	//ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_PALETTE_TERRAIN, OnPaletteTerrain)
	ON_COMMAND(ID_PALETTE_ORNAMENT, OnPaletteOrnament)
	ON_COMMAND(ID_PALETTE_UNIT, OnPaletteUnit)
	ON_COMMAND(ID_PALETTE_REGION, OnPaletteRegion)
	ON_COMMAND(ID_PALETTE_TRIGGER, OnPaletteTrigger)
	ON_COMMAND(ID_OPTION_FOG, &CNtlWEFrm::OnOptionFog)
	ON_UPDATE_COMMAND_UI(ID_OPTION_FOG, &CNtlWEFrm::OnUpdateOptionFog)
	ON_COMMAND(ID_UI_FIELDGUIDELINE, &CNtlWEFrm::OnUiFieldguideline)
	ON_UPDATE_COMMAND_UI(ID_UI_FIELDGUIDELINE, &CNtlWEFrm::OnUpdateUiFieldguideline)
	ON_COMMAND(ID_UI_SECTORGUIDELINE, &CNtlWEFrm::OnUiSectorguideline)
	ON_UPDATE_COMMAND_UI(ID_UI_SECTORGUIDELINE, &CNtlWEFrm::OnUpdateUiSectorguideline)
	ON_COMMAND(ID_UI_BLOCKGUIDELINE, &CNtlWEFrm::OnUiBlockguideline)
	ON_UPDATE_COMMAND_UI(ID_UI_BLOCKGUIDELINE, &CNtlWEFrm::OnUpdateUiBlockguideline)
	ON_UPDATE_COMMAND_UI(ID_PALETTE_TERRAIN, &CNtlWEFrm::OnUpdatePaletteTerrain)
	ON_UPDATE_COMMAND_UI(ID_PALETTE_ORNAMENT, &CNtlWEFrm::OnUpdatePaletteOrnament)
	ON_COMMAND(ID_CAMERA_PORTAL, &CNtlWEFrm::OnCameraPortal)
	ON_COMMAND(ID_FILE_SAVEALL, &CNtlWEFrm::OnFileSaveall)
	ON_WM_WINDOWPOSCHANGING()
	ON_COMMAND(ID_PALETTE_FIELDMERGE, &CNtlWEFrm::OnPaletteFieldmerge)
	ON_UPDATE_COMMAND_UI(ID_PALETTE_FIELDMERGE, &CNtlWEFrm::OnUpdatePaletteFieldmerge)
	ON_COMMAND(ID_PALETTE_WATER, &CNtlWEFrm::OnPaletteWater)
	ON_UPDATE_COMMAND_UI(ID_PALETTE_WATER, &CNtlWEFrm::OnUpdatePaletteWater)
	ON_COMMAND(ID_GENERATE_GENERATECYNISFIELD, &CNtlWEFrm::OnGenerateGeneratecynisfield)
	ON_COMMAND(ID_OPTION_MAPOBJ, &CNtlWEFrm::OnOptionMapobj)
	ON_UPDATE_COMMAND_UI(ID_OPTION_MAPOBJ, &CNtlWEFrm::OnUpdateOptionMapobj)
	ON_COMMAND(ID_OPTION_WATER, &CNtlWEFrm::OnOptionWater)
	ON_UPDATE_COMMAND_UI(ID_OPTION_WATER, &CNtlWEFrm::OnUpdateOptionWater)
	ON_COMMAND(ID_GENERATE_GENERATECYNISDOODADS, &CNtlWEFrm::OnGenerateGeneratecynisdoodads)
	ON_COMMAND(ID_PALETTE_SHADOW, &CNtlWEFrm::OnPaletteShadow)
	ON_UPDATE_COMMAND_UI(ID_PALETTE_SHADOW, &CNtlWEFrm::OnUpdatePaletteShadow)
	ON_COMMAND(ID_OBJECT_ANIMATIBLE_DOODAD_DUMMY, &CNtlWEFrm::OnObjectAnimatibleDoodadDummy)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_ANIMATIBLE_DOODAD_DUMMY, &CNtlWEFrm::OnUpdateObjectAnimatibleDoodadDummy)
	//ON_COMMAND(ID_OBJECT_WIRETERRAIN, &CNtlWEFrm::OnObjectWireterrain)
	//ON_UPDATE_COMMAND_UI(ID_OBJECT_WIRETERRAIN, &CNtlWEFrm::OnUpdateObjectWireterrain)
	ON_COMMAND(ID_OBJECT_DISPLAYWIREFRAME, &CNtlWEFrm::OnObjectDisplaywireframe)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_DISPLAYWIREFRAME, &CNtlWEFrm::OnUpdateObjectDisplaywireframe)
	ON_COMMAND(ID_PALETTE_EFFECT, &CNtlWEFrm::OnPaletteEffect)
	ON_UPDATE_COMMAND_UI(ID_PALETTE_EFFECT, &CNtlWEFrm::OnUpdatePaletteEffect)
	ON_COMMAND(ID_VISION_EFFECTS, &CNtlWEFrm::OnVisionEffects)
	ON_UPDATE_COMMAND_UI(ID_VISION_EFFECTS, &CNtlWEFrm::OnUpdateVisionEffects)
	ON_COMMAND(ID_VISION_DOODADS_ANIMATION, &CNtlWEFrm::OnVisionDoodadsAnimation)
	ON_UPDATE_COMMAND_UI(ID_VISION_DOODADS_ANIMATION, &CNtlWEFrm::OnUpdateVisionDoodadsAnimation)
	ON_COMMAND(ID_PALETTE_NORMALMAPPROPERTY, &CNtlWEFrm::OnPaletteNormalmapproperty)
	ON_UPDATE_COMMAND_UI(ID_PALETTE_NORMALMAPPROPERTY, &CNtlWEFrm::OnUpdatePaletteNormalmapproperty)
	//	ON_COMMAND(ID_PROP_CREATE, &CNtlWEFrm::OnPropCreate)
	//	ON_COMMAND(ID_PROP_DELETE, &CNtlWEFrm::OnPropDelete)
	//	ON_UPDATE_COMMAND_UI(ID_PROP_CREATE, &CNtlWEFrm::OnUpdatePropCreate)
	//	ON_UPDATE_COMMAND_UI(ID_PROP_DELETE, &CNtlWEFrm::OnUpdatePropDelete)
	//ON_COMMAND(ID_FIELD_BGM, &CNtlWEFrm::OnFieldBgm)
	//ON_UPDATE_COMMAND_UI(ID_FIELD_BGM, &CNtlWEFrm::OnUpdateFieldBgm)
	//ON_COMMAND(ID_FIELD_BGM, &CNtlWEFrm::OnFieldBgm)
	//ON_UPDATE_COMMAND_UI(ID_FIELD_BGM, &CNtlWEFrm::OnUpdateFieldBgm)
	ON_COMMAND(ID_VISION_SOUNDMUTE, &CNtlWEFrm::OnVisionSoundmute)
	ON_UPDATE_COMMAND_UI(ID_VISION_SOUNDMUTE, &CNtlWEFrm::OnUpdateVisionSoundmute)
	ON_COMMAND(ID_MENU_EXPORT_TRIGGER, &CNtlWEFrm::OnMenuExportTrigger)
	ON_COMMAND(ID_INFO_VERSION, &CNtlWEFrm::OnInfoVersion)
	ON_COMMAND(ID_EXPORT_EXPORTMAPPROP, &CNtlWEFrm::OnExportExportmapprop)
	ON_COMMAND(ID_EXPORT_PEC, &CNtlWEFrm::OnExportPec)
	ON_COMMAND(ID_VISION_VISIBILITYDISTANCE, &CNtlWEFrm::OnVisionVisibilitydistance)
	ON_UPDATE_COMMAND_UI(ID_VISION_VISIBILITYDISTANCE, &CNtlWEFrm::OnUpdateVisionVisibilitydistance)
	ON_COMMAND(ID_PALETTE_SPAWN, &CNtlWEFrm::OnPaletteSpawn)
	ON_UPDATE_COMMAND_UI(ID_PALETTE_SPAWN, &CNtlWEFrm::OnUpdatePaletteSpawn)
	ON_COMMAND(ID_VISION_COLLISIONOBJECT, &CNtlWEFrm::OnVisionCollisionobject)
	ON_UPDATE_COMMAND_UI(ID_VISION_COLLISIONOBJECT, &CNtlWEFrm::OnUpdateVisionCollisionobject)
	ON_COMMAND(ID_VISION_BLOOMEFFECT, &CNtlWEFrm::OnVisionBloomeffect)
	ON_UPDATE_COMMAND_UI(ID_VISION_BLOOMEFFECT, &CNtlWEFrm::OnUpdateVisionBloomeffect)
	ON_COMMAND(ID_PALETTE_SOUNDEFFECT, &CNtlWEFrm::OnPaletteSoundeffect)
	ON_UPDATE_COMMAND_UI(ID_PALETTE_SOUNDEFFECT, &CNtlWEFrm::OnUpdatePaletteSoundeffect)
	ON_COMMAND(ID_PALETTE_PERFORMANCE, &CNtlWEFrm::OnPalettePerformance)
	ON_UPDATE_COMMAND_UI(ID_PALETTE_PERFORMANCE, &CNtlWEFrm::OnUpdatePalettePerformance)
	ON_COMMAND(ID_VISION_PERFORMANCESECTORCULL, &CNtlWEFrm::OnVisionPerformancesectorcull)
	ON_UPDATE_COMMAND_UI(ID_VISION_PERFORMANCESECTORCULL, &CNtlWEFrm::OnUpdateVisionPerformancesectorcull)
	ON_COMMAND(ID_EXPORT_EXPORTMAPNAME, &CNtlWEFrm::OnExportExportmapname)
	ON_COMMAND(ID_VISION_PVSLOCKING, &CNtlWEFrm::OnVisionPvslocking)
	ON_UPDATE_COMMAND_UI(ID_VISION_PVSLOCKING, &CNtlWEFrm::OnUpdateVisionPvslocking)
	ON_COMMAND(ID_VISION_PVSACTIVATION, &CNtlWEFrm::OnVisionPvsactivation)
	ON_UPDATE_COMMAND_UI(ID_VISION_PVSACTIVATION, &CNtlWEFrm::OnUpdateVisionPvsactivation)
	ON_COMMAND(ID_VISION_PROJECTIONSHADOW, &CNtlWEFrm::OnVisionProjectionshadow)
	ON_UPDATE_COMMAND_UI(ID_VISION_PROJECTIONSHADOW, &CNtlWEFrm::OnUpdateVisionProjectionshadow)
	ON_COMMAND(ID_VISION_FOGACTIVATION, &CNtlWEFrm::OnVisionFogactivation)
	ON_UPDATE_COMMAND_UI(ID_VISION_FOGACTIVATION, &CNtlWEFrm::OnUpdateVisionFogactivation)
	ON_COMMAND(ID_VISION_PLANTSET, &CNtlWEFrm::OnVisionPlant)
	ON_UPDATE_COMMAND_UI(ID_VISION_PLANTSET, &CNtlWEFrm::OnUpdateVisionPlant)
	ON_COMMAND(ID_EXPORT_EXPORTHEIGHTFIELDY, &CNtlWEFrm::OnExportExportheightfieldy)
	ON_COMMAND(ID_VISION_OCCLUDERACTIVATION, &CNtlWEFrm::OnVisionOccluderactivation)
	ON_UPDATE_COMMAND_UI(ID_VISION_OCCLUDERACTIVATION, &CNtlWEFrm::OnUpdateVisionOccluderactivation)
	ON_COMMAND(ID_VISION_OBJECTWIREFRAME, &CNtlWEFrm::OnVisionObjectwireframe)
	ON_UPDATE_COMMAND_UI(ID_VISION_OBJECTWIREFRAME, &CNtlWEFrm::OnUpdateVisionObjectwireframe)
	ON_COMMAND(ID_UI_OBJECTWIREFRAME, &CNtlWEFrm::OnUiObjectwireframe)
	ON_COMMAND(ID_EXPORT_EXPORTHEIGHTFIELDPE, &CNtlWEFrm::OnExportExportheightfieldpe)
	ON_COMMAND(ID_EXPORT_EXPORTMAPPROPPE, &CNtlWEFrm::OnExportExportmappropPe)
	ON_COMMAND(ID_UI_OBJECTVISIBLE, &CNtlWEFrm::OnUiObjectvisible)
	ON_COMMAND(ID_VISION_SOUNDMUTE_BGM, &CNtlWEFrm::OnVisionSoundmuteBgm)
	ON_UPDATE_COMMAND_UI(ID_VISION_SOUNDMUTE_BGM, &CNtlWEFrm::OnUpdateVisionSoundmuteBgm)
	ON_COMMAND(ID_VISION_SOUNDMUTE_ENV, &CNtlWEFrm::OnVisionSoundmuteEnv)
	ON_UPDATE_COMMAND_UI(ID_VISION_SOUNDMUTE_ENV, &CNtlWEFrm::OnUpdateVisionSoundmuteEnv)
	ON_COMMAND(ID_VISION_WEATHER, &CNtlWEFrm::OnVisionWeather)
	ON_UPDATE_COMMAND_UI(ID_VISION_WEATHER, &CNtlWEFrm::OnUpdateVisionWeather)
	ON_COMMAND(ID_VISION_HEATHAZE, &CNtlWEFrm::OnVisionHeathaze)
	ON_UPDATE_COMMAND_UI(ID_VISION_HEATHAZE, &CNtlWEFrm::OnUpdateVisionHeathaze)
	ON_COMMAND(ID_WORLD_NEW, &CNtlWEFrm::OnWorldNew)
	ON_COMMAND(ID_WORLD_LOAD, &CNtlWEFrm::OnWorldLoad)
	ON_COMMAND(ID_EXPORT_EXPORTPATHENGINEDATA, &CNtlWEFrm::OnExportExportpathenginedata)
	ON_COMMAND(ID_UI_BSP_SECTOR, &CNtlWEFrm::OnUiBspSector)
	ON_UPDATE_COMMAND_UI(ID_UI_BSP_SECTOR, &CNtlWEFrm::OnUpdateUiBspSector)
	ON_COMMAND(ID_UI_BSP_WIREFRAME, &CNtlWEFrm::OnUiBspWireframe)
	ON_UPDATE_COMMAND_UI(ID_UI_BSP_WIREFRAME, &CNtlWEFrm::OnUpdateUiBspWireframe)
	ON_COMMAND(ID_UTIL_PATHENGINEUTIL, &CNtlWEFrm::OnUtilPathengineutil)
	ON_COMMAND(ID_UTIL_BOOKMARK, &CNtlWEFrm::OnUtilBookmark)
	ON_COMMAND(ID_PALETTE_GAMEPROPERTY, &CNtlWEFrm::OnPaletteGameproperty)
	ON_UPDATE_COMMAND_UI(ID_PALETTE_GAMEPROPERTY, &CNtlWEFrm::OnUpdatePaletteGameproperty)
	ON_COMMAND(ID_PALETTE_WORLDVIEW, &CNtlWEFrm::OnPaletteWorldview)
	ON_COMMAND(ID_FILE_SAVE_AUTO, &CNtlWEFrm::OnFileSaveAuto)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AUTO, &CNtlWEFrm::OnUpdateFileSaveAuto)
	ON_COMMAND(ID_VISION_LIGHTMAP, &CNtlWEFrm::OnVisionLightmap)
	ON_UPDATE_COMMAND_UI(ID_VISION_LIGHTMAP, &CNtlWEFrm::OnUpdateVisionLightmap)
	ON_COMMAND(ID_UI_BSP_WIREFRAME_NV, &CNtlWEFrm::OnUiBspWireframeNv)
	ON_UPDATE_COMMAND_UI(ID_UI_BSP_WIREFRAME_NV, &CNtlWEFrm::OnUpdateUiBspWireframeNv)
	ON_COMMAND(ID_VISION_TERRAIN, &CNtlWEFrm::OnVisionTerrain)
	ON_UPDATE_COMMAND_UI(ID_VISION_TERRAIN, &CNtlWEFrm::OnUpdateVisionTerrain)
	ON_COMMAND(ID_VISION_TRIGGERUI, &CNtlWEFrm::OnVisionTriggerui)
	ON_UPDATE_COMMAND_UI(ID_VISION_TRIGGERUI, &CNtlWEFrm::OnUpdateVisionTriggerui)
	ON_COMMAND(ID_EXPORT_EXPORTINVALIDSECTORLINK, &CNtlWEFrm::OnExportExportinvalidsectorlink)
	ON_COMMAND(ID_VISION_MINIINDOORCULL, &CNtlWEFrm::OnVisionMiniindoorcull)
	ON_UPDATE_COMMAND_UI(ID_VISION_MINIINDOORCULL, &CNtlWEFrm::OnUpdateVisionMiniindoorcull)
	ON_COMMAND(ID_FILE_RESETOBJECTSERIALID, &CNtlWEFrm::OnFileResetobjectserialid)
	END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CNtlWEFrm 생성/소멸

CNtlWEFrm::CNtlWEFrm()
{
	CRect rt;

	m_pSpawnPalette = new CPaletteSpawn;
	m_pSpawnPalette->Create(CPaletteSpawn::IDD, GetDesktopWindow());

	m_pTerrainPalette = new CPaletteTerrain;
	m_pTerrainPalette->Create(CPaletteTerrain::IDD, GetDesktopWindow());

	// Ornament, decal palette
	m_pOrnamentTabPalette = new CPaletteOrnamentTabDlg;
	m_pOrnamentTabPalette->Create(CPaletteOrnamentTabDlg::IDD, GetDesktopWindow());

	m_pOrnamentPalette = new CPaletteOrnament;
	m_pOrnamentPalette->Create(CPaletteOrnament::IDD, &m_pOrnamentTabPalette->m_TabCtrl);
	m_pOrnamentPalette->GetWindowRect(&rt);
	m_pOrnamentPalette->MoveWindow(5, 25, rt.Width(), rt.Height());

	m_pDojoPalette = new CPaletteDojoDlg;
	m_pDojoPalette->Create(CPaletteDojoDlg::IDD, &m_pOrnamentTabPalette->m_TabCtrl);
	m_pDojoPalette->GetWindowRect(&rt);
	m_pDojoPalette->MoveWindow(5, 25, rt.Width(), rt.Height());

	m_pDecalVolumePalette = new CPaletteDecalVolumeDlg;
	m_pDecalVolumePalette->Create(CPaletteDecalVolumeDlg::IDD, &m_pOrnamentTabPalette->m_TabCtrl);
	m_pDecalVolumePalette->GetWindowRect(&rt);
	m_pDecalVolumePalette->MoveWindow(5, 25, rt.Width(), rt.Height());

	m_pDecalPalette = new CPaletteDecalDlg;
	m_pDecalPalette->Create(CPaletteDecalDlg::IDD, &m_pOrnamentTabPalette->m_TabCtrl);
	m_pDecalPalette->GetWindowRect(&rt);
	m_pDecalPalette->MoveWindow(5, 25, rt.Width(), rt.Height());

	m_pPlantPalette = new CPalettePlantDlg;
	m_pPlantPalette->Create(CPalettePlantDlg::IDD, &m_pOrnamentTabPalette->m_TabCtrl);
	m_pPlantPalette->GetWindowRect(&rt);
	m_pPlantPalette->MoveWindow(5, 25, rt.Width(), rt.Height());

	m_pHeatHazePalette = new CPaletteHeatHazeDlg;
	m_pHeatHazePalette->Create(CPaletteHeatHazeDlg::IDD, &m_pOrnamentTabPalette->m_TabCtrl);
	m_pHeatHazePalette->GetWindowRect(&rt);
	m_pHeatHazePalette->MoveWindow(5, 25, rt.Width(), rt.Height());

	m_pFieldUI = new CPaletteFieldUI;
	m_pFieldUI->Create(CPaletteFieldUI::IDD, GetDesktopWindow());

	m_pEffectPalette = new CPaletteEffect;
	m_pEffectPalette->Create(CPaletteEffect::IDD, GetDesktopWindow());

	m_pUnitPalette = new CPaletteUnit;
	m_pUnitPalette->Create(CPaletteUnit::IDD, GetDesktopWindow());

	m_pRegionPalette = new CPaletteRegion;
	m_pRegionPalette->Create(CPaletteRegion::IDD, GetDesktopWindow());

	m_pTriggerPalette = new CPaletteTrigger;
	m_pTriggerPalette->Create(CPaletteTrigger::IDD, GetDesktopWindow());

	m_pNMPropPalette = new CPaletteNMProp;
	m_pNMPropPalette->Create(CPaletteNMProp::IDD, GetDesktopWindow());

	m_pSEPalette = new CPaletteSE;
	m_pSEPalette->Create(CPaletteSE::IDD, GetDesktopWindow());

	m_pCameraPortal = new CCameraAttrDlg;
	m_pCameraPortal->Create(CCameraAttrDlg::IDD, GetDesktopWindow());

	m_pWorldViewDlg	= new CWorldViewDlg;
	m_pWorldViewDlg->Create(CWorldViewDlg::IDD, GetDesktopWindow());

	m_pSaberTest = new CSaberTestDlg;
	m_pSaberTest->Create(CSaberTestDlg::IDD, GetDesktopWindow());

	// Water & ShoreLine Palette
	m_pWaterTabPalette = new CPaletteWaterTab;
	m_pWaterTabPalette->Create(CPaletteWaterTab::IDD, GetDesktopWindow());

	m_pGamePropertyPalette = new CPaletteGameProperty;
	m_pGamePropertyPalette->Create(CPaletteGameProperty::IDD, GetDesktopWindow());

	m_pWaterPalette = new CPaletteWater;
	m_pWaterPalette->Create(CPaletteWater::IDD, &m_pWaterTabPalette->m_TabCtrl);
	m_pWaterPalette->GetWindowRect(&rt);
	m_pWaterPalette->MoveWindow(5, 25, rt.Width(), rt.Height());

	m_pShoreLinePalette = new CPaletteShoreLine;
	m_pShoreLinePalette->Create(CPaletteShoreLine::IDD, &m_pWaterTabPalette->m_TabCtrl);
	m_pShoreLinePalette->GetWindowRect(&rt);
	m_pShoreLinePalette->MoveWindow(5, 25, rt.Width(), rt.Height());

	// Light & shadow palettes
	m_pLightAndShadowPalette = new CPaletteLightAndShadow;
	m_pLightAndShadowPalette->Create(CPaletteLightAndShadow::IDD, GetDesktopWindow());

	m_pShadowPalette = new CPaletteShadow;
	m_pShadowPalette->Create(CPaletteShadow::IDD, &m_pLightAndShadowPalette->m_TabCtrl);
	m_pShadowPalette->GetWindowRect(&rt);
	m_pShadowPalette->MoveWindow(5, 25, rt.Width(), rt.Height());

	// CZ-SS
// 	m_pSoftSpotPalette = new CPaletteSoftSpot;
// 	m_pSoftSpotPalette->Create(CPaletteSoftSpot::IDD, &m_pLightAndShadowPalette->m_TabCtrl);
// 	m_pSoftSpotPalette->GetWindowRect(&rt);
// 	m_pSoftSpotPalette->MoveWindow(5, 25, rt.Width(), rt.Height());

	m_pWorldLightPalette = new CPaletteWorldLight;
	m_pWorldLightPalette->Create(CPaletteWorldLight::IDD, &m_pLightAndShadowPalette->m_TabCtrl);
	m_pWorldLightPalette->GetWindowRect(&rt);
	m_pWorldLightPalette->MoveWindow(5, 25, rt.Width(), rt.Height());

	m_pLightObjectPalette = new CPaletteLightObject;
	m_pLightObjectPalette->Create(CPaletteLightObject::IDD, &m_pLightAndShadowPalette->m_TabCtrl);
	m_pLightObjectPalette->GetWindowRect(&rt);
	m_pLightObjectPalette->MoveWindow(5, 25, rt.Width(), rt.Height());

	m_pLightModePalette = new CPaletteLightMode;
	m_pLightModePalette->Create(CPaletteLightMode::IDD, &m_pLightAndShadowPalette->m_TabCtrl);
	m_pLightModePalette->GetWindowRect(&rt);
	m_pLightModePalette->MoveWindow(5, 25, rt.Width(), rt.Height());

	// Performance palettes
	m_pPerformancePalette = new CPalettePerformance;
	m_pPerformancePalette->Create(CPalettePerformance::IDD, GetDesktopWindow());

	m_pPerformanceSectorCull = new CPalettePerformanceSectorCull;
	m_pPerformanceSectorCull->Create(CPalettePerformanceSectorCull::IDD, &m_pPerformancePalette->m_TabCtrl);
	m_pPerformanceSectorCull->GetWindowRect(&rt);
	m_pPerformanceSectorCull->MoveWindow(5, 25, rt.Width(), rt.Height());

	m_pPerformanceOcclusion = new CPalettePerformanceOcclusion;
	m_pPerformanceOcclusion->Create(CPalettePerformanceOcclusion::IDD, &m_pPerformancePalette->m_TabCtrl);
	m_pPerformanceOcclusion->GetWindowRect(&rt);
	m_pPerformanceOcclusion->MoveWindow(5, 25, rt.Width(), rt.Height());

	m_pPerformancePVS = new CPalettePerformancePVS;
	m_pPerformancePVS->Create(CPalettePerformancePVS::IDD, &m_pPerformancePalette->m_TabCtrl);
	m_pPerformancePVS->GetWindowRect(&rt);
	m_pPerformancePVS->MoveWindow(5, 25, rt.Width(), rt.Height());

	m_pPerformanceImposter = new CPalettePerformanceImposter;
	m_pPerformanceImposter->Create(CPalettePerformanceImposter::IDD, &m_pPerformancePalette->m_TabCtrl);
	m_pPerformanceImposter->GetWindowRect(&rt);
	m_pPerformanceImposter->MoveWindow(5, 25, rt.Width(), rt.Height());

	m_pFieldSearchDlg = new CFieldSearchDlg;
	m_pFieldSearchDlg->Create(CFieldSearchDlg::IDD, GetDesktopWindow());

	m_pPathMeshFindDlg = new CPathMeshFindDlg;
	m_pPathMeshFindDlg->Create(CPathMeshFindDlg::IDD, GetDesktopWindow());

	m_pFieldHeatHazeDlg = new CFieldHeatHazeDlg;
	m_pFieldHeatHazeDlg->Create(CFieldHeatHazeDlg::IDD, GetDesktopWindow());

	m_pBookmarkDlg = new CBookmarkDlg;
	m_pBookmarkDlg->Create(CBookmarkDlg::IDD, GetDesktopWindow());

	m_pSkyFloatingDlg = new CPaletteSkyFloatingEntityDlg;
	m_pSkyFloatingDlg->Create(CPaletteSkyFloatingEntityDlg::IDD, GetDesktopWindow());
}

void CNtlWEFrm::InitializeDialogs()
{
	m_pSpawnPalette->InitializeWorldData();
	m_pTerrainPalette->InitializeWorldData();
	m_pOrnamentPalette->InitializeWorldData();
	m_pDojoPalette->InitializeWorldData();
	m_pDecalVolumePalette->InitializeWorldData();
	m_pDecalPalette->InitializeWorldData();
	m_pPlantPalette->InitializeWorldData();
	m_pHeatHazePalette->InitializeWorldData();
	m_pFieldUI->InitializeWorldData();
	m_pWaterPalette->InitializeWorldData();
	m_pShoreLinePalette->InitializeWorldData();
	m_pUnitPalette->InitializeWorldData();
	m_pRegionPalette->InitializeWorldData();
	m_pTriggerPalette->InitializeWorldData();
	m_pShadowPalette->InitializeWorldData();
	m_pEffectPalette->InitializeWorldData();
	m_pNMPropPalette->InitializeWorldData();
	m_pNMPropPalette->InitializeWorldData();
	m_pSEPalette->InitializeWorldData();
	// CZ-SS
//	m_pSoftSpotPalette->InitializeWorldData();
	m_pWorldLightPalette->InitializeWorldData();
	m_pLightObjectPalette->InitializeWorldData();
	m_pLightModePalette->InitializeWorldData();
	m_pCameraPortal->InitializeWorldData();
	m_pSaberTest->InitializeWorldData();
	m_pWorldViewDlg->InitializeWorldData();
	m_pPerformanceSectorCull->InitializeWorldData();
	m_pPerformanceOcclusion->InitializeWorldData();
	m_pPerformancePVS->InitializeWorldData();
	m_pPerformanceImposter->InitializeWorldData();
}

CNtlWEFrm::~CNtlWEFrm()
{
	m_pBookmarkDlg->Save();

	m_pSpawnPalette->DestroyWindow();			delete m_pSpawnPalette;					m_pSpawnPalette = NULL;
	m_pTerrainPalette->DestroyWindow();			delete m_pTerrainPalette;				m_pTerrainPalette = NULL;
	m_pOrnamentPalette->DestroyWindow();		delete m_pOrnamentPalette;				m_pOrnamentPalette = NULL;
	m_pDojoPalette->DestroyWindow();			delete m_pDojoPalette;					m_pDojoPalette= NULL;
	m_pDecalVolumePalette->DestroyWindow();		delete m_pDecalVolumePalette;			m_pDecalVolumePalette = NULL;
	m_pDecalPalette->DestroyWindow();			delete m_pDecalPalette;					m_pDecalPalette = NULL;
	m_pPlantPalette->DestroyWindow();			delete m_pPlantPalette;					m_pPlantPalette = NULL;
	m_pHeatHazePalette->DestroyWindow();		delete m_pHeatHazePalette;				m_pHeatHazePalette = NULL;
	m_pOrnamentTabPalette->DestroyWindow();		delete m_pOrnamentTabPalette;			m_pOrnamentTabPalette = NULL;
	m_pFieldUI->DestroyWindow();				delete m_pFieldUI;						m_pFieldUI = NULL;
	m_pWaterPalette->DestroyWindow();			delete m_pWaterPalette;					m_pWaterPalette = NULL;
	m_pShoreLinePalette->DestroyWindow();		delete m_pShoreLinePalette;				m_pShoreLinePalette = NULL;
	m_pWaterTabPalette->DestroyWindow();		delete m_pWaterTabPalette;				m_pWaterTabPalette = NULL;
	m_pGamePropertyPalette->DestroyWindow();	delete m_pGamePropertyPalette;			m_pGamePropertyPalette = NULL;
	m_pShadowPalette->DestroyWindow();			delete m_pShadowPalette;				m_pShadowPalette = NULL;
	m_pEffectPalette->DestroyWindow();			delete m_pEffectPalette;				m_pEffectPalette = NULL;
	m_pUnitPalette->DestroyWindow();			delete m_pUnitPalette;					m_pUnitPalette = NULL;
	m_pRegionPalette->DestroyWindow();			delete m_pRegionPalette;				m_pRegionPalette = NULL;
	m_pTriggerPalette->DestroyWindow();			delete m_pTriggerPalette;				m_pTriggerPalette = NULL;
	m_pNMPropPalette->DestroyWindow();			delete m_pNMPropPalette;				m_pNMPropPalette = NULL;
	m_pCameraPortal->DestroyWindow();			delete m_pCameraPortal;					m_pCameraPortal = NULL;
	m_pSaberTest->DestroyWindow();				delete m_pSaberTest;					m_pSaberTest = NULL;
	m_pWorldViewDlg->DestroyWindow();			delete m_pWorldViewDlg;					m_pWorldViewDlg = NULL;
	m_pSEPalette->DestroyWindow();				delete m_pSEPalette;					m_pSEPalette = NULL;
	// CZ-SS
//	m_pSoftSpotPalette->DestroyWindow();		delete m_pSoftSpotPalette;				m_pSoftSpotPalette = NULL;
	m_pWorldLightPalette->DestroyWindow();		delete m_pWorldLightPalette;			m_pWorldLightPalette = NULL;
	m_pLightObjectPalette->DestroyWindow();		delete m_pLightObjectPalette;			m_pLightObjectPalette = NULL;
	m_pLightModePalette->DestroyWindow();		delete m_pLightModePalette;				m_pLightModePalette = NULL;
	m_pLightAndShadowPalette->DestroyWindow();	delete m_pLightAndShadowPalette;		m_pLightAndShadowPalette = NULL;
	m_pPerformancePalette->DestroyWindow();		delete m_pPerformancePalette;			m_pPerformancePalette = NULL;
	m_pPerformanceSectorCull->DestroyWindow();	delete m_pPerformanceSectorCull;		m_pPerformanceSectorCull = NULL;
	m_pPerformanceOcclusion->DestroyWindow();	delete m_pPerformanceOcclusion;			m_pPerformanceOcclusion = NULL;
	m_pPerformancePVS->DestroyWindow();			delete m_pPerformancePVS;				m_pPerformancePVS = NULL;
	m_pPerformanceImposter->DestroyWindow();	delete m_pPerformanceImposter;			m_pPerformanceImposter = NULL;
	m_pFieldSearchDlg->DestroyWindow();			delete m_pFieldSearchDlg;				m_pFieldSearchDlg = NULL;
	m_pPathMeshFindDlg->DestroyWindow();		delete m_pPathMeshFindDlg;				m_pPathMeshFindDlg = NULL;
	m_pFieldHeatHazeDlg->DestroyWindow();		delete m_pFieldHeatHazeDlg;				m_pFieldHeatHazeDlg = NULL;
	m_pBookmarkDlg->DestroyWindow();			delete m_pBookmarkDlg;					m_pBookmarkDlg = NULL;
	m_pSkyFloatingDlg->DestroyWindow();			delete m_pSkyFloatingDlg;				m_pSkyFloatingDlg = NULL;
}

int CNtlWEFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{/*
 // SplashScreen
 CSplashScreenFx *pSplash = new CSplashScreenFx();
 pSplash->Create(AfxGetMainWnd(), "세이버 1.0 ", 0 ,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);

 ::srand( (unsigned )time(NULL));
 if(rand() % 2 == 0)
 {
 pSplash->SetBitmap(IDB_BMP_SPLASH00, 12, 41, 80);
 }
 else
 {
 pSplash->SetBitmap(IDB_BMP_SPLASH01, 12, 41, 80);
 }

 pSplash->SetTextFont("Arial",120,CSS_TEXT_NORMAL);
 pSplash->SetTextRect(CRect(160,220,390,260));
 pSplash->CreateProgressBar(CRect(200,288,450,295), CSize(0, 3), PBS_SMOOTH);
 pSplash->SetTextColor(RGB(180, 180, 180));
 pSplash->SetTextFormat(DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
 pSplash->Show();	
 pSplash->SetActiveWindow();

 // SplashScreen	
 pSplash->SetText("ARTRIA 召還...");
 {
 Sleep(200);
 }
 pSplash->ProgressStep();

 pSplash->SetText("ARTRIA 契約...");
 {
 Sleep(200);
 }
 pSplash->ProgressStep();

 pSplash->SetText("ARTRIA 起動...");
 {
 Sleep(200);
 }
 pSplash->ProgressStep();

 pSplash->Hide();
 */
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	/*
	if (!m_wndToolBar.CreateEx(this) ||
	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
	TRACE0("도구 모음을 만들지 못했습니다.\n");
	return -1;      // 만들지 못했습니다.
	}
	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
	CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
	TRACE0("대화 상자 모음을 만들지 못했습니다.\n");
	return -1;		// 만들지 못했습니다.
	}

	if (!m_wndReBar.Create(this) ||
	!m_wndReBar.AddBar(&m_wndToolBar) ||
	!m_wndReBar.AddBar(&m_wndDlgBar))
	{
	TRACE0("크기 조정 막대를 만들지 못했습니다.\n");
	return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this) ||
	!m_wndStatusBar.SetIndicators(indicators,
	sizeof(indicators)/sizeof(UINT)))
	{
	TRACE0("상태 표시줄을 만들지 못했습니다.\n");
	return -1;      // 만들지 못했습니다.
	}
	// TODO: 도구 설명을 표시하지 않으려면 이 줄을 제거하십시오.
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
	CBRS_TOOLTIPS | CBRS_FLYBY);*/

	return 0;
}

// void CNtlWEFrm::OnSize(UINT nType, int cx, int cy)
// {
// 	CFrameWnd::OnSize(nType, cx, cy);
// 
// 	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
// 	RECT rc;
// 	GetClientRect(&rc);
// 
// 	CNtlWEApplication::GetInstance()->OnSize(nType, rc.right-rc.left, rc.bottom - rc.top);
// }

BOOL CNtlWEFrm::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	if(cs.hMenu)
	{
		// attributes of window
		cs.style	&= ~FWS_ADDTOTITLE;
		cs.style	= WS_TILEDWINDOW;
		
		static RwChar cTemp[512];
		sprintf_s(cTemp, 512, "SABER(%s-%s) : Nothing is more despicable than respect based on fear-Albert Camus.", dNTL_WORLD_VERSION, dNTL_WORLD_TOOL_MODE);
		cs.lpszName = cTemp;

		// delete menu and specify menu is not used
		//::DestroyMenu(cs.hMenu);
		//cs.hMenu = NULL;
	}

	return TRUE;
}


// CNtlWEFrm 진단

#ifdef _DEBUG
void CNtlWEFrm::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CNtlWEFrm::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CNtlWEFrm 메시지 처리기


static RpClump *
ClumpLoad(RpClump *clump, const RwChar *name)
{
	RwStream *stream;
	RwChar *path;

	path = RsPathnameCreate(name);
	stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, path);
	RsPathnameDestroy(path);

	if( stream )
	{
		clump = RpClumpStreamRead(stream);

		RwStreamClose(stream, NULL);
	}

	return clump;
}

static RpClump *
ClumpSave(RpClump *clump, const RwChar *name)
{
	RwStream *stream;
	RwChar *path;

	path = RsPathnameCreate(name);
	stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMWRITE, path);
	RsPathnameDestroy(path);

	if( stream )
	{
		RpClumpStreamWrite(clump, stream);

		RwStreamClose(stream, NULL);
	}

	return clump;
}

RpAtomic* AtomicBuildDataCallBack(RpAtomic *pAtomic, void *data)
{
	if(RpCollisionGeometryBuildData(RpAtomicGetGeometry(pAtomic), NULL) == (RpGeometry *)NULL)
	{
		// something's wrong
	}

	return NULL;
}

//void CNtlWEFrm::OnCollisiontreeGeneration()
//{
//	char dirBackup[NTL_MAX_DIR_PATH];
//	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);
//
//	// set dir. info
//	char pszPathname[256];
//	ITEMIDLIST *pidlBrowse;
//
//	BROWSEINFO BrInfo;
//	BrInfo.hwndOwner = GetSafeHwnd();
//	BrInfo.pidlRoot = NULL;
//	memset(&BrInfo, 0, sizeof(BrInfo));
//	BrInfo.pszDisplayName = pszPathname;
//	BrInfo.lpszTitle = "find a project folder";
//	pidlBrowse = ::SHBrowseForFolder(&BrInfo);
//
//	CString strResult = "";
//	if(pidlBrowse != NULL)
//	{
//		SHGetPathFromIDList(pidlBrowse, pszPathname);
//		string FileName = pszPathname;
//
//		// generate clump collision tree data
//		CFileFind	ff;
//		CString		ResultPath;
//		CString		ImagePath;
//		BOOL		Continue	= true;
//		ResultPath				= pszPathname;
//		ImagePath				= pszPathname;
//		ImagePath				= ImagePath + _T("\\");
//		ResultPath				= ResultPath + _T("\\*.dff");
//		Continue				= ff.FindFile(ResultPath.GetBuffer(0));
//
//		while(Continue)
//		{
//			CString		strFullFilePath;
//			RwStream	*stream;
//			RwChar		*path;
//			RpClump		*pClump;
//			BOOL		Save = FALSE;
//
//			Continue		= ff.FindNextFile();
//			strFullFilePath = ff.GetFilePath();
//
//			// load
//			RwImageSetPath(ImagePath.GetBuffer());
//
//			path = RsPathnameCreate(strFullFilePath.GetBuffer(0));
//			stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, path);
//			RsPathnameDestroy(path);
//
//			if(stream)
//			{
//				RwStreamFindChunk(stream, rwID_CLUMP, NULL, NULL);
//				pClump = (RpClump*)RpClumpStreamRead(stream);
//				RpClumpForAllAtomics(pClump, AtomicBuildDataCallBack, NULL);
//				RwStreamClose(stream, NULL);
//				Save = TRUE;
//			}
//
//			if(Save)
//			{
//				path = RsPathnameCreate(strFullFilePath.GetBuffer(0));
//				stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMWRITE, path);
//				RsPathnameDestroy(path);
//
//				if(stream)
//				{
//					//RwStreamFindChunk(stream, rwID_CLUMP, NULL, NULL);
//					RpClumpStreamWrite(pClump, stream);
//					RwStreamClose(stream, NULL);
//					RpClumpDestroy(pClump);
//				}
//			}
//		}
//	}
//	else
//		return;
//
//	::SetCurrentDirectory(dirBackup);
//}


void CNtlWEFrm::OnFileNew()
{
	ITEMIDLIST *pidlBrowse;
	char pszPathname[NTL_MAX_DIR_PATH];

	// set dir. info
	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "create a project folder";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	CString strResult = "";
	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);

		// check this folder'got already ntl files
		char CurDir[NTL_MAX_DIR_PATH] = {0,};
		GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);
		SetCurrentDirectory(pszPathname);
		if((_access("#######.gwp", 0)) != -1)
		{
			::AfxMessageBox("this folder's got ntl world files, please use open menu", MB_OK);
			return;
		}
		SetCurrentDirectory(CurDir);
	}
	else
		return;

	// set dialogs
	bool OKBtnPushed = false;
	CFileNewDlg NewLevelDlg;
	if(NewLevelDlg.DoModal() == IDOK)
		OKBtnPushed = true;

	if(OKBtnPushed)
	{
		if (::strcmp(dGETDOC()->m_NtlWorldParam.WorldRAW, "Error"))
		{
			string FullPathFileName = pszPathname;
			if (((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.CreateWorld(FullPathFileName))
			{
				InitializeDialogs();
				switch (dGETMGR()->GetActiveWorldType())
				{					
				case AW_HEGITHFIELD:					
					((CNtlWEApp*)(AfxGetApp()))->SetOutdoorMode();
					break;
				case AW_RWWORLD:
					((CNtlWEApp*)(AfxGetApp()))->SetIndoorMode();
					break;
				}
			}			
		}
		else
		{
			::AfxMessageBox("RAW File was wrong. Plz try again", MB_OK);
		}
	}
}

void CNtlWEFrm::OnGenerateGeneratecynisfield()
{
	ITEMIDLIST*	pidlBrowse;
	char		pszPathname[NTL_MAX_DIR_PATH];
	char		pszPathnameSrc[NTL_MAX_DIR_PATH];
	char		pszPathnameDst[NTL_MAX_DIR_PATH];

	// set dir. info
	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select ntl world project folder";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);

		// check this folder'got already ntl files
		char CurDir[NTL_MAX_DIR_PATH] = {0,};
		GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);
		SetCurrentDirectory(pszPathname);
		if((_access("#######.gwp", 0)) == -1)
		{
			::AfxMessageBox("this folder has not ntl world files plz. try again.", MB_OK);
			return;
		}
		SetCurrentDirectory(CurDir);
	}
	else
		return;

	strcpy_s(pszPathnameSrc, NTL_MAX_DIR_PATH, pszPathname);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select folder to create heightfield files";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
	}
	else
		return;

	strcpy_s(pszPathnameDst, NTL_MAX_DIR_PATH, pszPathname);

	CNtlWorldMergeManager::GetInstance()->OnExportHeightfield(pszPathnameSrc, pszPathnameDst);
}

void CNtlWEFrm::OnFileOpen()
{
	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	// set dir. info
	char pszPathname[NTL_MAX_DIR_PATH];
	ITEMIDLIST *pidlBrowse;

	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "find a project folder";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	CString strResult = "";
	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);

		// check this folder'got already ntl files
		char CurDir[NTL_MAX_DIR_PATH] = {0,};
		GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);
		SetCurrentDirectory(pszPathname);

		if (FILE* pFile = fopen("#######.gwp", "rb"))
		{
			LoadFileWorldState(dGET_WORLD_PARAM(), pFile);
			fclose(pFile);

			if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
			{				
				string FileName = pszPathname;
				if(((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.CreateWorldFromFolder(FileName))
				{
					InitializeDialogs();
					switch (dGETMGR()->GetActiveWorldType())
					{
					case AW_HEGITHFIELD:

						((CNtlWEApp*)(AfxGetApp()))->SetOutdoorMode();
						break;
					case AW_RWWORLD:					
						((CNtlWEApp*)(AfxGetApp()))->SetIndoorMode();
						break;
					}
				}
			}				
			else
			{
				char acTemp[256];
				sprintf_s(acTemp, 256, "unsupported version : world(%s), tool(%s)", dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION);
				::AfxMessageBox(acTemp, MB_OK);
			}
		}
		else
		{
			::AfxMessageBox("this folder hasn't got ntl world files, please try another one", MB_OK);
		}
	}

	::SetCurrentDirectory(dirBackup);
}

void CNtlWEFrm::OnPaletteShow(RwInt32 ePalette)
{
	if(!((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.IsWorking())
	{
		::AfxMessageBox("plz, import ntl terrains first.", MB_OK);
		return;
	}

	if (dGET_CUR_PALETTE() == ePalette)
	{
		return;
	}

	m_pSpawnPalette->ShowWindow(SW_HIDE);
	m_pSpawnPalette->m_pPaletteSpawnPath->ShowWindow(SW_HIDE);
	m_pSpawnPalette->m_pPaletteSpawnNavigator->ShowWindow(SW_HIDE);
	m_pTerrainPalette->ShowWindow(SW_HIDE);
	m_pOrnamentTabPalette->ShowWindow(SW_HIDE);
	m_pDecalVolumePalette->ShowWindow(SW_HIDE);
	m_pDecalPalette->ShowWindow(SW_HIDE);
	m_pPlantPalette->ShowWindow(SW_HIDE);
	m_pHeatHazePalette->ShowWindow(SW_HIDE);
	m_pOrnamentPalette->ShowWindow(SW_HIDE);
	m_pDojoPalette->ShowWindow(SW_HIDE);
	m_pFieldUI->ShowWindow(SW_HIDE);
	//m_pFieldUI->m_pPaletteSkyFloatingEntitydlg->ShowWindow(SW_HIDE);
	m_pWaterTabPalette->ShowWindow(SW_HIDE);
	m_pGamePropertyPalette->ShowWindow(SW_HIDE);
	m_pShoreLinePalette->ShowWindow(SW_HIDE);
	m_pWaterPalette->ShowWindow(SW_HIDE);
	m_pShadowPalette->ShowWindow(SW_HIDE);
	m_pEffectPalette->ShowWindow(SW_HIDE);
	m_pNMPropPalette->ShowWindow(SW_HIDE);
	m_pSEPalette->ShowWindow(SW_HIDE);
	// CZ-SS
//	m_pSoftSpotPalette->ShowWindow(SW_HIDE);
	m_pWorldLightPalette->ShowWindow(SW_HIDE);
	m_pLightObjectPalette->ShowWindow(SW_HIDE);
	m_pLightModePalette->ShowWindow(SW_HIDE);
	m_pLightAndShadowPalette->ShowWindow(SW_HIDE);
	m_pPerformancePalette->ShowWindow(SW_HIDE);
	m_pPerformanceSectorCull->ShowWindow(SW_HIDE);
	m_pPerformanceOcclusion->ShowWindow(SW_HIDE);
	m_pPerformancePVS->ShowWindow(SW_HIDE);
	m_pPerformanceImposter->ShowWindow(SW_HIDE);
	m_pSkyFloatingDlg->ShowWindow(SW_HIDE);

	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.m_pEventHandler->SetEffectsDummy(FALSE);
	((CNtlWEApp*)(AfxGetApp()))->m_pFrm->m_pShoreLinePalette->Reset();
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.m_pEventHandler->DeleteAllPaletteDummies();

	dGET_CUR_PALETTE() = (ePALETTE_MODE)ePalette;

	switch (dGET_CUR_PALETTE())
	{
	case ePM_DEFAULT :
		{
		}
		break;
	case ePM_TERRAIN :
		{
			m_pTerrainPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pTerrainPalette->ShowWindow(SW_SHOW);
		}
		break;
	case ePM_TEXTURE :
		{
		}
		break;
	case ePM_DIFFUSE :
		{
		}
		break;
	case ePM_ORNAMENT :
		{		
			m_pOrnamentPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,	SWP_NOMOVE | SWP_NOSIZE);
			m_pOrnamentPalette->ShowWindow(SW_SHOW);
			m_pOrnamentTabPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pOrnamentTabPalette->ShowWindow(SW_SHOW);

			m_pOrnamentTabPalette->InitializeWorldData();
		}
		break;
	case ePM_FIELDUI :
		{
			m_pFieldUI->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,	SWP_NOMOVE | SWP_NOSIZE);
			m_pFieldUI->ShowWindow(SW_SHOW);
		}
		break;
	case ePM_WATER :
		{
			m_pWaterPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pWaterPalette->ShowWindow(SW_SHOW);
			m_pWaterTabPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pWaterTabPalette->ShowWindow(SW_SHOW);

			m_pWaterTabPalette->InitializeWorldData();
		}
		break;
	case ePM_SHADOW :
		{
			m_pShadowPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pShadowPalette->ShowWindow(SW_SHOW);
			m_pLightAndShadowPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pLightAndShadowPalette->ShowWindow(SW_SHOW);

			m_pLightAndShadowPalette->InitializeWorldData();
		}
		break;
	case ePM_EFFECT :
		{
			m_pEffectPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pEffectPalette->ShowWindow(SW_SHOW);

			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.m_pEventHandler->SetEffectsDummy(TRUE);
		}
		break;
	case ePM_NM_PROP:
		{
			m_pNMPropPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pNMPropPalette->ShowWindow(SW_SHOW);
		}
		break;
	case ePM_SE :
		{
			m_pSEPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pSEPalette->ShowWindow(SW_SHOW);

			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.m_pEventHandler->SetSEVision(TRUE);
		}
		break;
	case ePM_UNIT :
		{
		}
	case ePM_REGION :
		{
		}
		break;
	case ePM_TRIGGER :
		{
		}
		break;
	case ePM_SPAWN :
		{
			m_pSpawnPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pSpawnPalette->ShowWindow(SW_SHOW);

			GetSpawnInstance()->CreateEntitiesAtCurFields();
		}
		break;
		// CZ-SS
// 	case ePM_SOFTSPOT :
// 		{
// 		}
// 		break;
	case ePM_PERFORMANCE_SECTORCULL :
		{
			m_pPerformanceSectorCull->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pPerformanceSectorCull->ShowWindow(SW_SHOW);			
			m_pPerformancePalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pPerformancePalette->ShowWindow(SW_SHOW);

			m_pPerformancePalette->InitializeWorldData();			
			m_pPerformanceSectorCull->InitializeWorldData();

		}
		break;
	case ePM_PERFORMANCE_OCCLUSION :
		{
			m_pPerformanceOcclusion->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pPerformanceOcclusion->ShowWindow(SW_SHOW);
			m_pPerformancePalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pPerformancePalette->ShowWindow(SW_SHOW);

			m_pPerformancePalette->InitializeWorldData();
			m_pPerformanceOcclusion->InitializeWorldData();			
		}
		break;
	case ePM_PERFORMANCE_PVS :
		{
			m_pPerformancePVS->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pPerformancePVS->ShowWindow(SW_SHOW);
			m_pPerformancePalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pPerformancePalette->ShowWindow(SW_SHOW);

			m_pPerformancePalette->InitializeWorldData();
			m_pPerformancePVS->InitializeWorldData();			
		}
		break;
	case ePM_PERFORMANCE_IMPOSTER :
		{
			m_pPerformancePalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pPerformancePalette->ShowWindow(SW_SHOW);			
			m_pPerformanceImposter->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pPerformanceImposter->ShowWindow(SW_SHOW);

			m_pPerformancePalette->InitializeWorldData();
			m_pPerformanceImposter->InitializeWorldData();
		}
		break;
	case ePM_SHORE_LINE :
		{
		}
		break;
	case ePM_DECAL :
		{
		}
		break;
	case ePM_PLANT :
		{
		}
		break;
	case ePM_WORLDLIGHT :
		{
			m_pWorldLightPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pWorldLightPalette->ShowWindow(SW_SHOW);
			m_pLightAndShadowPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pLightAndShadowPalette->ShowWindow(SW_SHOW);

			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.m_pEventHandler->SetWLVision(TRUE);

			m_pLightAndShadowPalette->InitializeWorldData();
		}
		break;
	case ePM_HEATHAZE :
		{
		}
		break;
	case ePM_GAMEPROPERTY:
		{
			m_pGamePropertyPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pGamePropertyPalette->ShowWindow(SW_SHOW);

			m_pGamePropertyPalette->InitializeWorldData();
		}
		break;
	case ePM_SKYFLOATING:
		{
			m_pSkyFloatingDlg->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			m_pSkyFloatingDlg->ShowWindow(SW_SHOW);
		}
		break;
	}
}

void CNtlWEFrm::OnPaletteSpawn()
{
	OnPaletteShow(ePM_SPAWN);
}

void CNtlWEFrm::OnPaletteTerrain()
{
	OnPaletteShow(ePM_TERRAIN);
}

void CNtlWEFrm::OnPaletteOrnament()
{
	OnPaletteShow(ePM_ORNAMENT);
}

void CNtlWEFrm::OnPaletteFieldmerge()
{
	OnPaletteShow(ePM_FIELDUI);
}

void CNtlWEFrm::OnPaletteWater()
{
	OnPaletteShow(ePM_WATER);
}

void CNtlWEFrm::OnPaletteShadow()
{
	// 인도어
	switch (GetSceneManager()->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		{
			OnPaletteShow(ePM_SHADOW);
		}
		break;
	case AW_RWWORLD:
		{
			OnPaletteShow(ePM_WORLDLIGHT);
		}
	}
}

void CNtlWEFrm::OnPaletteEffect()
{
	OnPaletteShow(ePM_EFFECT);
}

void CNtlWEFrm::OnPaletteNormalmapproperty()
{
	OnPaletteShow(ePM_NM_PROP);
}

void CNtlWEFrm::OnPaletteSE()
{
	OnPaletteShow(ePM_SE);
}

void CNtlWEFrm::OnPalettePerformanceSectorCull()
{
	OnPaletteShow(ePM_PERFORMANCE_SECTORCULL);
}

void CNtlWEFrm::OnPalettePerformanceOcclusion()
{
	OnPaletteShow(ePM_PERFORMANCE_OCCLUSION);
}

void CNtlWEFrm::OnPalettePerformancePVS()
{
	OnPaletteShow(ePM_PERFORMANCE_PVS);
}

void CNtlWEFrm::OnPalettePerformanceImposter()
{
	OnPaletteShow(ePM_PERFORMANCE_IMPOSTER);
}

void CNtlWEFrm::OnPaletteSoundeffect()
{
	if(!((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.IsWorking())
	{
		::AfxMessageBox("plz, import ntl terrains first.", MB_OK);
		return;
	}

	OnPaletteSE();
}

void CNtlWEFrm::OnPalettePerformance()
{
	if(!((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.IsWorking())
	{
		::AfxMessageBox("plz, import ntl terrains first.", MB_OK);
		return;
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		OnPalettePerformanceSectorCull();
	}
	else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		OnPalettePerformanceOcclusion();
	}
}

void CNtlWEFrm::OnPaletteUnit()
{
	if(!((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.IsWorking())
	{
		::AfxMessageBox("plz, import ntl terrains first.", MB_OK);
		return;
	}

	m_pUnitPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,	SWP_NOMOVE | SWP_NOSIZE);
	m_pUnitPalette->ShowWindow(SW_SHOW);
	dGET_CUR_PALETTE() = ePM_ORNAMENT;
}

void CNtlWEFrm::OnPaletteRegion()
{
	if(!((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.IsWorking())
	{
		::AfxMessageBox("plz, import ntl terrains first.", MB_OK);
		return;
	}

	m_pRegionPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,	SWP_NOMOVE | SWP_NOSIZE);
	m_pRegionPalette->ShowWindow(SW_SHOW);
}

void CNtlWEFrm::OnPaletteTrigger()
{
	if(!((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.IsWorking())
	{
		::AfxMessageBox("plz, import ntl terrains first.", MB_OK);
		return;
	}

	m_pTriggerPalette->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,	SWP_NOMOVE | SWP_NOSIZE);
	m_pTriggerPalette->ShowWindow(SW_SHOW);
}

void CNtlWEFrm::OnOptionFog()
{
	dGETAPP().Switch(eAO_FOG);
}

void CNtlWEFrm::OnUiFieldguideline()
{
	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		dGETAPP().Switch(eAO_FIELD_GUIDE_LINE);
	}
	else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		m_pWorldViewDlg->SetGuideField(!m_pWorldViewDlg->GetGuideField());
	}
}

void CNtlWEFrm::OnUiSectorguideline()
{	
	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		dGETAPP().Switch(eAO_SECTOR_GUIDE_LINE);
	}
	else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		m_pWorldViewDlg->SetGuideSector(!m_pWorldViewDlg->GetGuideSector());
	}
}

void CNtlWEFrm::OnUiBlockguideline()
{
	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		dGETAPP().Switch(eAO_BLOCK_GUIDE_LINE);
	}
	else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		m_pWorldViewDlg->SetGuideBlock(!m_pWorldViewDlg->GetGuideBlock());
	}
}

void CNtlWEFrm::OnSaberTest()
{
	m_pSaberTest->ShowWindow(SW_SHOW);
}

void CNtlWEFrm::OnWorldView()
{
	m_pWorldViewDlg->ShowWindow(SW_SHOW);
}

void CNtlWEFrm::OnUpdateOptionFog(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(dGETAPP().IsWorking(eAO_FOG));
}

void CNtlWEFrm::OnFileSaveall()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();
	dGETHDL()->SaveAll();
}

void CNtlWEFrm::OnOptionMapobj()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_DOODADS);
	CNtlPLGlobal::m_bObjectRender = CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_DOODADS);
	GetSceneManager()->SetDistanceFilter(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_VISIBILTITY_DIST));

	//dGETHDL()->SetDoodadsVision(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_DOODADS));
}

void CNtlWEFrm::OnOptionWater()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_WATER);
}

void CNtlWEFrm::OnObjectAnimatibleDoodadDummy()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_ANIMATIBLE_DOODAD_DUMMY);
	dGETHDL()->SetAnimatibleDoodadsDummyVision(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_ANIMATIBLE_DOODAD_DUMMY));
}

void CNtlWEFrm::OnUpdateOptionMapobj(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_DOODADS));
}

void CNtlWEFrm::OnUpdateOptionWater(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_WATER));
}

void CNtlWEFrm::OnUpdateObjectAnimatibleDoodadDummy(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_ANIMATIBLE_DOODAD_DUMMY));
}

void CNtlWEFrm::OnUpdateUiFieldguideline(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		pCmdUI->SetCheck(dGETAPP().IsWorking(eAO_FIELD_GUIDE_LINE));
	}
	else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		pCmdUI->SetCheck(m_pWorldViewDlg->GetGuideField());
	}
}

void CNtlWEFrm::OnUpdateUiSectorguideline(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		pCmdUI->SetCheck(dGETAPP().IsWorking(eAO_SECTOR_GUIDE_LINE));
	}
	else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		pCmdUI->SetCheck(m_pWorldViewDlg->GetGuideSector());
	}
}

void CNtlWEFrm::OnUpdateUiBlockguideline(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		pCmdUI->SetCheck(dGETAPP().IsWorking(eAO_BLOCK_GUIDE_LINE));
	}
	else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		pCmdUI->SetCheck(m_pWorldViewDlg->GetGuideBlock());
	}
}

// Markable palette menu
void CNtlWEFrm::OnUpdatePalettePerformance(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(dGET_CUR_PALETTE() == ePM_PERFORMANCE_SECTORCULL);
}

void CNtlWEFrm::OnUpdatePaletteTerrain(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(dGET_CUR_PALETTE() == ePM_TERRAIN || dGET_CUR_PALETTE() == ePM_TEXTURE || dGET_CUR_PALETTE() == ePM_DIFFUSE);
}

void CNtlWEFrm::OnUpdatePaletteOrnament(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(dGET_CUR_PALETTE() == ePM_ORNAMENT);
}

void CNtlWEFrm::OnUpdatePaletteNormalmapproperty(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(dGET_CUR_PALETTE() == ePM_NM_PROP);
}

void CNtlWEFrm::OnUpdatePaletteFieldmerge(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(dGET_CUR_PALETTE() == ePM_FIELDUI);
}

void CNtlWEFrm::OnUpdatePaletteWater(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(dGET_CUR_PALETTE() == ePM_WATER);
}

void CNtlWEFrm::OnUpdatePaletteShadow(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(dGET_CUR_PALETTE() == ePM_SHADOW);
}

void CNtlWEFrm::OnUpdatePaletteEffect(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(dGET_CUR_PALETTE() == ePM_EFFECT);
}

void CNtlWEFrm::OnUpdatePaletteSpawn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(dGET_CUR_PALETTE() == ePM_SPAWN);
}

void CNtlWEFrm::OnUpdatePaletteSoundeffect(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(dGET_CUR_PALETTE() == ePM_SE);
}
/*
void CNtlWEFrm::OnUpdateObjectWireterrain(CCmdUI *pCmdUI)
{
pCmdUI->Enable();
pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_WIRE));
}
*/
void CNtlWEFrm::OnObjectDisplaywireframe()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_WIRE_SOLID);	
}

void CNtlWEFrm::OnUpdateObjectDisplaywireframe(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_WIRE_SOLID));	
}

void CNtlWEFrm::OnVisionEffects()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_EFFECTS);
	dGETHDL()->SetEffectsVision(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_EFFECTS));
}

void CNtlWEFrm::OnUpdateVisionEffects(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_EFFECTS));	
}

void CNtlWEFrm::OnVisionDoodadsAnimation()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_OBJECT_ANIMATION);
	dGETHDL()->SetEffectsVision(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_OBJECT_ANIMATION));
}

void CNtlWEFrm::OnUpdateVisionDoodadsAnimation(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_OBJECT_ANIMATION));	
}

void CNtlWEFrm::OnVisionSoundmute()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_SOUND);
	if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_SOUND))
	{
		GetSoundManager()->SetMasterVolume(0.0f);
	}
	else
	{ 
		GetSoundManager()->SetMasterVolume(1.0f);
	}
}

void CNtlWEFrm::OnUpdateVisionSoundmute(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_SOUND));	
}

void CNtlWEFrm::OnVisionSoundmuteBgm()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_BGM);
	if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_BGM))
	{
		GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_BGM, 0.0f);
	}
	else
	{ 
		GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_BGM, 1.0f);
	}
}

void CNtlWEFrm::OnUpdateVisionSoundmuteBgm(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_BGM));	
}

void CNtlWEFrm::OnVisionSoundmuteEnv()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_ENV);
	if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_ENV))
	{
		GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_AMBIENT_MUSIC, 0.0f);
	}
	else
	{ 
		GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_AMBIENT_MUSIC, 1.0f);
	}
}

void CNtlWEFrm::OnUpdateVisionSoundmuteEnv(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_ENV));	
}


void CNtlWEFrm::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CRect wndRect, trayRect; 

	int leftTaskbar = 0, rightTaskbar = 0, topTaskbar = 0, bottomTaskbar = 0; 

	GetWindowRect(&wndRect); 

	int m_nXOffset = 33;
	int m_nYOffset = 33;

	// Screen resolution 

	int screenWidth = GetSystemMetrics(SM_CXSCREEN); 

	int screenHeight = GetSystemMetrics(SM_CYSCREEN); 



	// Find the taskbar 

	CWnd* pWnd = FindWindow(_T("Shell_TrayWnd"), _T("")); 

	pWnd->GetWindowRect(&trayRect); 



	int wndWidth = wndRect.right - wndRect.left; 

	int wndHeight = wndRect.bottom - wndRect.top; 



	if(trayRect.top <= 0 && trayRect.left <= 0 && trayRect.right >= screenWidth) { 

		// top taskbar 

		topTaskbar = trayRect.bottom - trayRect.top; 

	} 

	else if(trayRect.top > 0 && trayRect.left <= 0) { 

		// bottom taskbar 

		bottomTaskbar = trayRect.bottom - trayRect.top; 

	} 

	else if(trayRect.top <= 0 && trayRect.left > 0) { 

		// right taskbar 

		rightTaskbar = trayRect.right - trayRect.left; 

	} 

	else { 

		// left taskbar 

		leftTaskbar = trayRect.right - trayRect.left; 

	} 



	// Snap to screen border 

	// Left border 

	if(lpwndpos->x >= -m_nXOffset + leftTaskbar && lpwndpos->x <= leftTaskbar + m_nXOffset) { 

		lpwndpos->x = leftTaskbar; 

	} 



	// Top border 

	if(lpwndpos->y >= -m_nYOffset && lpwndpos->y <= topTaskbar + m_nYOffset) { 

		lpwndpos->y = topTaskbar; 

	} 



	// Right border 

	if(lpwndpos->x + wndWidth <= screenWidth - rightTaskbar + m_nXOffset && 

		lpwndpos->x + wndWidth >= screenWidth - rightTaskbar - m_nXOffset) { 

			lpwndpos->x = screenWidth - rightTaskbar - wndWidth; 

	} 



	// Bottom border 

	if( lpwndpos->y + wndHeight <= screenHeight - bottomTaskbar + m_nYOffset 

		&& lpwndpos->y + wndHeight >= screenHeight - bottomTaskbar - m_nYOffset) { 

			lpwndpos->y = screenHeight - bottomTaskbar - wndHeight; 

	} 

	CFrameWnd::OnWindowPosChanging(lpwndpos);
}

void CNtlWEFrm::OnExportExportmapname()
{
	ITEMIDLIST*	pidlBrowse;
	char		pszPathname[NTL_MAX_DIR_PATH];
	char		pszPathnameSrc[NTL_MAX_DIR_PATH];
	char		pszPathnameDst[NTL_MAX_DIR_PATH];

	// set dir. info
	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select ntl world project folder";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);

		// check this folder'got already ntl files
		char CurDir[NTL_MAX_DIR_PATH] = {0,};
		GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);
		SetCurrentDirectory(pszPathname);
		if((_access("#######.gwp", 0)) == -1)
		{
			::AfxMessageBox("this folder has not ntl world files plz. try again.", MB_OK);
			return;
		}
		SetCurrentDirectory(CurDir);
	}
	else
		return;

	strcpy_s(pszPathnameSrc, NTL_MAX_DIR_PATH, pszPathname);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select folder to create map name files";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
	}
	else
		return;

	strcpy_s(pszPathnameDst, NTL_MAX_DIR_PATH, pszPathname);

	CNtlWorldMergeManager::GetInstance()->OnExportMapName(pszPathnameSrc, pszPathnameDst);	
}

void CNtlWEFrm::OnExportExportmapprop()
{
	ITEMIDLIST*	pidlBrowse;
	char		pszPathname[NTL_MAX_DIR_PATH];
	char		pszPathnameSrc[NTL_MAX_DIR_PATH];
	char		pszPathnameDst[NTL_MAX_DIR_PATH];

	// set dir. info
	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select ntl world project folder";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);

		// check this folder'got already ntl files
		char CurDir[NTL_MAX_DIR_PATH] = {0,};
		GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);
		SetCurrentDirectory(pszPathname);
		if((_access("#######.gwp", 0)) == -1)
		{
			::AfxMessageBox("this folder has not ntl world files plz. try again.", MB_OK);
			return;
		}
		SetCurrentDirectory(CurDir);
	}
	else
		return;

	strcpy_s(pszPathnameSrc, NTL_MAX_DIR_PATH, pszPathname);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select folder to create map prop. files";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
	}
	else
		return;

	strcpy_s(pszPathnameDst, NTL_MAX_DIR_PATH, pszPathname);

	CNtlWorldMergeManager::GetInstance()->OnExportMapProp(pszPathnameSrc, pszPathnameDst);
}

void CNtlWEFrm::OnGenerateGeneratecynisdoodads()
{
	ITEMIDLIST*	pidlBrowse;
	char		pszPathname[NTL_MAX_DIR_PATH];
	char		pszPathnameSrc[NTL_MAX_DIR_PATH];
	char		pszPathnameDst[NTL_MAX_DIR_PATH];

	// set dir. info
	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select ntl world project folder";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);

		// check this folder'got already ntl files
		char CurDir[NTL_MAX_DIR_PATH] = {0,};
		GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);
		SetCurrentDirectory(pszPathname);
		if((_access("#######.gwp", 0)) == -1)
		{
			::AfxMessageBox("this folder has not ntl world files plz. try again.", MB_OK);
			return;
		}
		SetCurrentDirectory(CurDir);
	}
	else
		return;

	strcpy_s(pszPathnameSrc, NTL_MAX_DIR_PATH, pszPathname);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select folder to create doodads files";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
	}
	else
		return;

	strcpy_s(pszPathnameDst, NTL_MAX_DIR_PATH, pszPathname);

	CNtlWorldMergeManager::GetInstance()->OnExportMapObj(pszPathnameSrc, pszPathnameDst);
}

void CNtlWEFrm::OnExportPec()
{
	ITEMIDLIST*	pidlBrowse;
	char		pszPathname[NTL_MAX_DIR_PATH];
	char		pszPathnameSrc[NTL_MAX_DIR_PATH];
	char		pszPathnameDst[NTL_MAX_DIR_PATH];

	// set dir. info
	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select ntl world project folder";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);

		// check this folder'got already ntl files
		char CurDir[NTL_MAX_DIR_PATH] = {0,};
		GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);
		SetCurrentDirectory(pszPathname);
		if((_access("#######.gwp", 0)) == -1)
		{
			::AfxMessageBox("this folder has not ntl world files plz. try again.", MB_OK);
			return;
		}
		SetCurrentDirectory(CurDir);
	}
	else
		return;

	strcpy_s(pszPathnameSrc, NTL_MAX_DIR_PATH, pszPathname);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select folder to create path-engine collision info. files";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
	}
	else
		return;

	strcpy_s(pszPathnameDst, NTL_MAX_DIR_PATH, pszPathname);

	CNtlWorldMergeManager::GetInstance()->OnExportPECollision(pszPathnameSrc, pszPathnameDst);
}
/*
void CNtlWEFrm::OnObjectWireterrain()
{
CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_WIRE);

if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_WIRE))
{
RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}
else
{
RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
}
*/
void CNtlWEFrm::OnMenuExportTrigger()
{
	char acOldPath[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acOldPath);

	char acTemp[NTL_MAX_DIR_PATH];
	char acWindowTitle[NTL_MAX_DIR_PATH];
	sprintf_s(acWindowTitle, NTL_MAX_DIR_PATH, "select ntl world project folder");

	ITEMIDLIST*	pidlBrowse;

	// set dir. info
	BROWSEINFO BrInfo;
	memset(&BrInfo, 0, sizeof(BrInfo));

	BrInfo.hwndOwner		= NULL;
	BrInfo.pidlRoot			= NULL;
	BrInfo.pszDisplayName	= acTemp;
	BrInfo.lpszTitle		= acWindowTitle;
	BrInfo.ulFlags			= BIF_NEWDIALOGSTYLE;
	pidlBrowse				= ::SHBrowseForFolder(&BrInfo);

	if (pidlBrowse)
	{
		SHGetPathFromIDList(pidlBrowse, acTemp);
		::SetCurrentDirectory(acTemp);

		FILE* pFile = NULL;
		if(fopen_s(&pFile, "#######.gwp", "rb"))
		{
			::SetCurrentDirectory(acOldPath);
			AfxMessageBox("wroong worldpath!");
			return;
		}
		else
		{
			sNtlWorldParam sWorldParam;
			LoadFileWorldState(&sWorldParam, pFile);
			fclose(pFile);
			::SetCurrentDirectory(acOldPath);

			CFileDialog	FileDlg(FALSE,
				_T( "tt" ),
				NULL,
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				_T("trigger table files (*.tt)|*.tt|All Files (*.*)|*.*||" ),
				NULL);

			FileDlg.m_ofn.lpstrInitialDir = "c:";

			if (FileDlg.DoModal() == IDCANCEL)
			{
				::SetCurrentDirectory(acOldPath);
				return;
			}

			::SetCurrentDirectory(acOldPath);

			if (sWorldParam.WorldType == 0)
			{
				CNtlWorldMergeManager::GetInstance()->OnExportTriggerInfoOutdoor(acTemp, FileDlg.GetPathName().GetBuffer(0));
			}
			else
			{
				CNtlWorldMergeManager::GetInstance()->OnExportTriggerInfoIndoor(acTemp, FileDlg.GetPathName().GetBuffer(0));
			}
		}

	}
	::SetCurrentDirectory(acOldPath);

	// old source
// 	static RwBool OneAtATime = FALSE;
// 
// 	if(OneAtATime)
// 		return;
// 
// 	OneAtATime = TRUE;
// 
// 	RwChar CurDir[NTL_MAX_DIR_PATH] = {0,};
// 	::GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);
// 
// 	CFileDialog	Dlg(FALSE,
// 		_T( "tt" ),
// 		NULL,
// 		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
// 		_T("trigger table files (*.tt)|*.tt|All Files (*.*)|*.*||" ),
// 		NULL);
// 
// 	Dlg.m_ofn.lpstrInitialDir = "c:";
// 
// 	if(Dlg.DoModal() == IDCANCEL)
// 	{
// 		OneAtATime = FALSE;
// 		return;
// 	}
// 
// 	::SetCurrentDirectory(CurDir);
// 
// 	// export trigger table files
// 	FILE *pFile = NULL;
// 	if(::fopen_s(&pFile, Dlg.GetPathName().GetBuffer(0), "wt") != 0)
// 	{
// 		DBO_TRACE(FALSE, "file open failed. (" << Dlg.GetPathName().GetBuffer(0) << ")");
// 		OneAtATime = FALSE;
// 		return;
// 	}
// 	else
// 	{
// 		ExportTriggerTable(pFile);
// 		fclose(pFile);
// 	}
// 
// 	OneAtATime = FALSE;
// 
// 	::SetCurrentDirectory(CurDir);
}

void CNtlWEFrm::OnInfoVersion()
{
	if(!((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.IsWorking())
	{
		::AfxMessageBox("plz, import ntl terrains first.", MB_OK);
		return;
	}

	CAboutDlg VerDlg;
	VerDlg.DoModal();
}

// VOID CNtlWEFrm::ExportTriggerTable(FILE* _pFile)
// {
// 	RwChar CurDir[NTL_MAX_DIR_PATH] = {0,};
// 	GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);
// 
// 	CNtlWorldFieldManager*	pFieldMgr		= dGETMGR();
// 	RwInt32					WorldSectorNum	= dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldFieldSectorNum;
// 	FILE*					pFile;
// 	RwV3d					SRT[3];
// 	RwChar					Name[128];
// 	RwBool					EnableTrigger, EnableSelection, EnableAlterable, EnablePortal, EnablePECollision, EnableShadowProp;
// 	RwUInt32				SID;
// #ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
// 	RwUInt32				ObjectID;
// #endif
// 	RwReal					VisibilityDist;
// 	RwChar					Text[128];
// 	DWORD					ObjNameIdx;
// 
// 	CProgressWnd WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Export Trigger Table.(TT)");
// 	WndProgress.GoModal();
// 	WndProgress.SetRange(0, WorldSectorNum);
// 
// 	for(int i = 0; i < WorldSectorNum; ++i)
// 	{
// 		::sprintf_s(Text, 128, "%d / %d sector is now exporting...", i, WorldSectorNum);
// 		WndProgress.SetText(Text);
// 
// 		RwV3d Pos;
// 		Pos.x = pFieldMgr->m_pSectors[i].DatumPoint.x;
// 		Pos.y = pFieldMgr->m_pSectors[i].DatumPoint.y;
// 		Pos.z = pFieldMgr->m_pSectors[i].DatumPoint.z;
// 		pFile = CMergeManager::GetInstance()->SetFilePosAtSector(Pos, eFPAS_DOODADS);
// 
// 		// load doodads info.
// 		RwInt32 TmpObjNum;
// 		fread(&TmpObjNum, sizeof(RwInt32), 1, pFile);
// 
// 		for(int i = 0; i < TmpObjNum; ++i)
// 		{
// 			// name
// 			fread(Name, sizeof(RwChar) * 128, 1, pFile);
// 
// 			// scaling, rotation, translation
// 			fread(&SRT[0], sizeof(RwV3d), 1, pFile);
// 			fread(&SRT[1], sizeof(RwV3d), 1, pFile);
// 			fread(&SRT[2], sizeof(RwV3d), 1, pFile);
// 
// 			// serial id
// 			fread(&SID, sizeof(RwUInt32), 1, pFile);
// 
// 			// object id
// #ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
// 			fread(&ObjectID, sizeof(RwUInt32), 1, pFile);
// #endif
// 
// 			// object name index
// 			fread(&ObjNameIdx, sizeof(DWORD), 1, pFile);
// 
// 			// visibility distance
// 			fread(&VisibilityDist, sizeof(RwReal), 1, pFile);
// 
// 			// enable trigger
// 			fread(&EnableTrigger, sizeof(RwBool), 1, pFile);
// 
// 			// enable selection
// 			fread(&EnableSelection, sizeof(RwBool), 1, pFile);
// 
// 			// enable alterable
// 			fread(&EnableAlterable, sizeof(RwBool), 1, pFile);
// 
// 			// enable portal
// 			fread(&EnablePortal, sizeof(RwBool), 1, pFile);
// 
// 			// enable p.e. collision
// 			fread(&EnablePECollision, sizeof(RwBool), 1, pFile);
// 
// 			// enable shadowprop
// 			fread(&EnableShadowProp, sizeof(RwBool), 1, pFile);
// 
// 			// AABB
// 			SPLEntityCreateParam PLEntityCreateParam;
// 			PLEntityCreateParam.pPos = &SRT[2];
// 			
// 			CNtlPLEntity* pNtlPLObj = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, Name, &PLEntityCreateParam));
// 			DBO_ASSERT(pNtlPLObj, "Entity create failed.");
// 
// 			RwBBox AABB = pNtlPLObj->GetTriggerAABBInfo(SRT[2], SRT[1], SRT[0]);
// 
// 			// direction
// 			//서버에서 필요한 방향벡터가 무엇인지를 알아봐야한다. 일단 0,0,1로 한다.
// 
// 			if(SID != 0xffffffff && EnableTrigger)
// 			{
// 				fprintf_s(_pFile, "%d\t%u\t%.2f\t%.2f\t%.2f\t0.00\t0.00\t1.00\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%d\t%d\t%d\t%s\t\n",	SID,
// 					ObjNameIdx,
// 					SRT[2].x, SRT[2].y, SRT[2].z,
// 					// Dir(RwV3d),
// 					AABB.inf.x, AABB.inf.y, AABB.inf.z,
// 					AABB.sup.x, AABB.sup.y, AABB.sup.z,
// 					CLAMP(EnableAlterable, 0, 1),
// 					CLAMP(EnableSelection, 0, 1),
// 					CLAMP(EnablePortal, 0, 1),
// 					Name);
// 
// 			}
// 
// 			GetSceneManager()->DeleteEntity(static_cast<CNtlPLEntity*>(pNtlPLObj));
// 
// 			// PS Map Size
// 			RwInt32 PSAtomicCnt = 0;
// 			fread(&PSAtomicCnt, sizeof(RwInt32), 1, pFile);
// 
// 			for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
// 			{
// 				// Map name
// 				fseek(pFile, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, SEEK_CUR);
// 
// 				// Frustum view window extent
// 				fseek(pFile, sizeof(RwReal), SEEK_CUR);
// 
// 				// Light matrices
// 				fseek(pFile, sizeof(RwMatrix), SEEK_CUR);
// 
// 				// Map size
// 				RwInt32 Size = 0;
// 				fread(&Size, sizeof(RwInt32), 1, pFile);
// 				fseek(pFile, sizeof(DWORD) * Size * Size, SEEK_CUR);
// 			}
// 
// 			// milepost
// 			RwBool EnableFlag;
// 			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
// 			if(EnableFlag)
// 			{
// 				fseek(pFile, sizeof(RwUInt32), SEEK_CUR);
// 			}
// 
// 			// attach light object skip
// 			RwInt32 iLightObjectNum;
// 			fread(&iLightObjectNum, sizeof(RwInt32), 1, pFile);
// 
// 			for (int j = 0; j < iLightObjectNum; ++j)
// 			{
// 				fseek(pFile, sizeof(RwChar) * MAX_ATTR_BONE_NAME, SEEK_CUR);
// 
// 				fseek(pFile, sizeof(RpLightType), SEEK_CUR);
// 				fseek(pFile, sizeof(RwBool), SEEK_CUR);
// 
// 				fseek(pFile, sizeof(RwV3d), SEEK_CUR);
// 
// 				fseek(pFile, sizeof(RwV3d), SEEK_CUR);
// 				fseek(pFile, sizeof(RwRGBAReal), SEEK_CUR);
// 				fseek(pFile, sizeof(RwReal), SEEK_CUR);
// 				fseek(pFile, sizeof(RwReal), SEEK_CUR);
// 
// 				fseek(pFile, sizeof(RwBool), SEEK_CUR);
// 				fseek(pFile, sizeof(RwBool), SEEK_CUR);
// 			}
// 		}
// 
// 		fclose(pFile);
// 
// 		WndProgress.StepIt();
// 		WndProgress.PeekAndPump();
// 	}
// 
// 	WndProgress.SetText("Done.");
// 	Sleep(1000);
// 
// 	::SetCurrentDirectory(CurDir);
// }

void CNtlWEFrm::OnVisionVisibilitydistance()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_VISIBILTITY_DIST);
	GetSceneManager()->SetDistanceFilter(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_VISIBILTITY_DIST));
}

void CNtlWEFrm::OnUpdateVisionVisibilitydistance(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_VISIBILTITY_DIST));
}

void CNtlWEFrm::OnVisionCollisionobject()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_COLLISION_OBJ);
	CNtlPLGlobal::m_bCollObjVisible = static_cast<RwBool>(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_COLLISION_OBJ));
}

void CNtlWEFrm::OnUpdateVisionCollisionobject(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_COLLISION_OBJ));
}

void CNtlWEFrm::OnVisionBloomeffect()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_BLOOM_EFFECT);

	RwBool Vision = static_cast<RwBool>(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_BLOOM_EFFECT));
	if(Vision)
	{
		CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_HDR);
	}
	else
	{
		CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_NONE);
	}
}

void CNtlWEFrm::OnUpdateVisionBloomeffect(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_BLOOM_EFFECT));
}

void CNtlWEFrm::OnVisionPerformancesectorcull()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_PERFORMANCE_SECTORCULL);
}

void CNtlWEFrm::OnUpdateVisionPerformancesectorcull(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PERFORMANCE_SECTORCULL));
}

void CNtlWEFrm::OnVisionPvslocking()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_PVS_LOCKING);

	RwBool Vision = static_cast<RwBool>(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PVS_LOCKING));
	if(!Vision)
	{
		// meaning it should be refresh PVS for current area
		CNtlWorldFieldManager* pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
		pFieldMgr->RefreshPVS();
	}
	else
	{
		CNtlWorldFieldManager* pFieldMgr		= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
		CNtlWorldSectorPVS::m_LockedPVSSector	= pFieldMgr->GetSectorIdx(*RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera))));
	}
}

void CNtlWEFrm::OnUpdateVisionPvslocking(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PVS_LOCKING));
}

void CNtlWEFrm::OnVisionPvsactivation()
{
	//CNtlWorldFieldManager*	pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	//CNtlPLGlobal::m_UseTerrainPVSMode = !CNtlPLGlobal::m_UseTerrainPVSMode;

	//pFieldMgr->SetPVSActivation(CNtlPLGlobal::m_UseTerrainPVSMode);

	GetSceneManager()->SetWorldPVSActivation(!CNtlPLGlobal::m_UseTerrainPVSMode);
}

void CNtlWEFrm::OnUpdateVisionPvsactivation(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlPLGlobal::m_UseTerrainPVSMode);
}

void CNtlWEFrm::OnVisionProjectionshadow()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_PROJECTION_SHADOW);
}

void CNtlWEFrm::OnUpdateVisionProjectionshadow(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PROJECTION_SHADOW));
}

void CNtlWEFrm::OnVisionFogactivation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_FOG_ACTIVATION);
}

void CNtlWEFrm::OnUpdateVisionFogactivation(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_FOG_ACTIVATION));
}

void CNtlWEFrm::OnVisionPlant()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_PLNAT);
}

void CNtlWEFrm::OnUpdateVisionPlant(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PLNAT));
}

void CNtlWEFrm::OnExportExportheightfieldy()
{
	ITEMIDLIST*	pidlBrowse;
	char		pszPathname[NTL_MAX_DIR_PATH];
	char		pszPathnameSrc[NTL_MAX_DIR_PATH];
	char		pszPathnameDst[NTL_MAX_DIR_PATH];

	// set dir. info
	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select ntl world project folder";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);

		// check this folder'got already ntl files
		char CurDir[NTL_MAX_DIR_PATH] = {0,};
		GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);
		SetCurrentDirectory(pszPathname);
		if((_access("#######.gwp", 0)) == -1)
		{
			::AfxMessageBox("this folder has not ntl world files plz. try again.", MB_OK);
			return;
		}
		SetCurrentDirectory(CurDir);
	}
	else
		return;

	strcpy_s(pszPathnameSrc, NTL_MAX_DIR_PATH, pszPathname);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select folder to create heightfield files";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
	}
	else
		return;

	strcpy_s(pszPathnameDst, NTL_MAX_DIR_PATH, pszPathname);

	CNtlWorldMergeManager::GetInstance()->OnExportHeightfieldY(pszPathnameSrc, pszPathnameDst);
}

void CNtlWEFrm::OnVisionOccluderactivation()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_OCCLUDER_ACTIVATION);
}

void CNtlWEFrm::OnUpdateVisionOccluderactivation(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_OCCLUDER_ACTIVATION));
}

void CNtlWEFrm::OnVisionObjectwireframe()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_WIRE_OBJECT);
}

void CNtlWEFrm::OnUpdateVisionObjectwireframe(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_WIRE_OBJECT));
}

void CNtlWEFrm::OnUiObjectwireframe()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_WIRE_OBJECT);
}

void CNtlWEFrm::OnExportExportheightfieldpe()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	ITEMIDLIST*	pidlBrowse;
	char		pszPathname[NTL_MAX_DIR_PATH];
	char		pszPathnameSrc[NTL_MAX_DIR_PATH];
	char		pszPathnameDst[NTL_MAX_DIR_PATH];

	// set dir. info
	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select ntl world project folder";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);

		// check this folder'got already ntl files
		char CurDir[NTL_MAX_DIR_PATH] = {0,};
		GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);
		SetCurrentDirectory(pszPathname);
		if((_access("#######.gwp", 0)) == -1)
		{
			::AfxMessageBox("this folder has not ntl world files plz. try again.", MB_OK);
			return;
		}
		SetCurrentDirectory(CurDir);
	}
	else
		return;

	strcpy_s(pszPathnameSrc, NTL_MAX_DIR_PATH, pszPathname);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select folder to create heightfield files";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
	}
	else
		return;

	strcpy_s(pszPathnameDst, NTL_MAX_DIR_PATH, pszPathname);

	CNtlWorldMergeManager::GetInstance()->OnExportHeightfieldPE(pszPathnameSrc, pszPathnameDst);
}

void CNtlWEFrm::OnExportExportmappropPe()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	ITEMIDLIST*	pidlBrowse;
	char		pszPathname[NTL_MAX_DIR_PATH];
	char		pszPathnameSrc[NTL_MAX_DIR_PATH];
	char		pszPathnameDst[NTL_MAX_DIR_PATH];

	// set dir. info
	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select ntl world project folder";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);

		// check this folder'got already ntl files
		char CurDir[NTL_MAX_DIR_PATH] = {0,};
		GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);
		SetCurrentDirectory(pszPathname);
		if((_access("#######.gwp", 0)) == -1)
		{
			::AfxMessageBox("this folder has not ntl world files plz. try again.", MB_OK);
			return;
		}
		SetCurrentDirectory(CurDir);
	}
	else
		return;

	strcpy_s(pszPathnameSrc, NTL_MAX_DIR_PATH, pszPathname);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "select folder to create MapPropPE files";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
	}
	else
		return;

	strcpy_s(pszPathnameDst, NTL_MAX_DIR_PATH, pszPathname);

	CNtlWorldMergeManager::GetInstance()->OnExportMapPropPE(pszPathnameSrc, pszPathnameDst);
}

void CNtlWEFrm::OnUiObjectvisible()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_DOODADS);
	CNtlPLGlobal::m_bObjectRender = CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_DOODADS);
}

void CNtlWEFrm::OnVisionWeather()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_WEATHER);
	dGETMGR()->GetWeatherHandler()->SetVisible(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_WEATHER));
}

void CNtlWEFrm::OnUpdateVisionWeather(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_WEATHER));	
}

void CNtlWEFrm::OnVisionHeathaze()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_HEATHAZE);
	GetNtlPLOptionManager()->SetHeatHaze(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_HEATHAZE));	
}

void CNtlWEFrm::OnUpdateVisionHeathaze(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_HEATHAZE));	
}

void CNtlWEFrm::OnWorldNew()
{
	OnFileNew();
}

void CNtlWEFrm::OnWorldLoad()
{
	OnFileOpen();
}

void CNtlWEFrm::OnExportExportpathenginedata()
{
	CPathEngineExportDlg PathEngineExportDlg;

	if (PathEngineExportDlg.DoModal())
	{
	}
}
void CNtlWEFrm::OnUiBspSector()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_BSP_SECTOR);
}

void CNtlWEFrm::OnUpdateUiBspSector(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_BSP_SECTOR));	
}

void CNtlWEFrm::OnUiBspWireframe()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_BSP_WIREFRAME);
}

void CNtlWEFrm::OnUpdateUiBspWireframe(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_BSP_WIREFRAME));	
}

void CNtlWEFrm::OnUtilPathengineutil()
{
	m_pPathMeshFindDlg->ShowWindow(SW_SHOW);
}

void CNtlWEFrm::OnUtilBookmark()
{
	if(!((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.IsWorking())
	{
		::AfxMessageBox("plz, import ntl terrains first.", MB_OK);

		return;
	}

	m_pBookmarkDlg->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	m_pBookmarkDlg->ShowWindow(SW_SHOW);
	m_pBookmarkDlg->RefreshWindow();
}

void CNtlWEFrm::OnSkyFloatDlg()
{
	OnPaletteShow(ePM_SKYFLOATING);
}

void CNtlWEFrm::OnCameraPortal()
{
	// 인도어
	if (dGETMGR()->GetActiveWorldType()/* == AW_HEGITHFIELD*/)
	{
		if(!((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.IsWorking())
		{
			::AfxMessageBox("plz, import ntl terrains first.", MB_OK);
			return;
		}

		m_pCameraPortal->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		m_pCameraPortal->ShowWindow(SW_SHOW);
		m_pCameraPortal->SetInfo();
	}
}

void CNtlWEFrm::OnPaletteGameproperty()
{
	OnPaletteShow(ePM_GAMEPROPERTY);
}

void CNtlWEFrm::OnUpdatePaletteGameproperty(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(dGET_CUR_PALETTE() == ePM_GAMEPROPERTY);
}

void CNtlWEFrm::OnPaletteWorldview()
{
	OnWorldView();
}

void CNtlWEFrm::OnFileSaveAuto()
{
	CNtlWorldLTManager::GetInstance()->SetAutoSave(!CNtlWorldLTManager::GetInstance()->GetAutoSave());
}

void CNtlWEFrm::OnUpdateFileSaveAuto(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldLTManager::GetInstance()->GetAutoSave() == TRUE);
}

void CNtlWEFrm::OnVisionLightmap()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_LIGHTMAP);
	CNtlWorldLTManager::GetInstance()->ToggleLightMapVisibility(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_LIGHTMAP));
}

void CNtlWEFrm::OnUpdateVisionLightmap(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_LIGHTMAP));	
}
void CNtlWEFrm::OnUiBspWireframeNv()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_BSP_WIREFRAME_NV);
}

void CNtlWEFrm::OnUpdateUiBspWireframeNv(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_BSP_WIREFRAME_NV));
}
void CNtlWEFrm::OnVisionTerrain()
{
	CNtlPLGlobal::m_bWorldTerrainVisible = !CNtlPLGlobal::m_bWorldTerrainVisible;
}

void CNtlWEFrm::OnUpdateVisionTerrain(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlPLGlobal::m_bWorldTerrainVisible);
}

void CNtlWEFrm::OnVisionTriggerui()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_TRIGGER_INFO);
}

void CNtlWEFrm::OnUpdateVisionTriggerui(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_TRIGGER_INFO));
}

void CNtlWEFrm::OnExportExportinvalidsectorlink()
{
	char acOldPath[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acOldPath);

	char acTemp[NTL_MAX_DIR_PATH];
	char acWindowTitle[NTL_MAX_DIR_PATH];
	sprintf_s(acWindowTitle, NTL_MAX_DIR_PATH, "select ntl world project folder");

	ITEMIDLIST*	pidlBrowse;

	// set dir. info
	BROWSEINFO BrInfo;
	memset(&BrInfo, 0, sizeof(BrInfo));

	BrInfo.hwndOwner		= NULL;
	BrInfo.pidlRoot			= NULL;
	BrInfo.pszDisplayName	= acTemp;
	BrInfo.lpszTitle		= acWindowTitle;
	BrInfo.ulFlags			= BIF_NEWDIALOGSTYLE;
	pidlBrowse				= ::SHBrowseForFolder(&BrInfo);

	if (pidlBrowse)
	{
		SHGetPathFromIDList(pidlBrowse, acTemp);
		::SetCurrentDirectory(acTemp);

		FILE* pFile = NULL;
		if(fopen_s(&pFile, "#######.gwp", "rb"))
		{
			::SetCurrentDirectory(acOldPath);
			AfxMessageBox("wroong worldpath!");
			return;
		}
		else
		{
			sNtlWorldParam sWorldParam;
			LoadFileWorldState(&sWorldParam, pFile);
			fclose(pFile);
			::SetCurrentDirectory(acOldPath);

			CFileDialog	FileDlg(FALSE,
				_T( "txt" ),
				NULL,
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				_T("trigger table files (*.txt)|*.txt|All Files (*.*)|*.*||" ),
				NULL);

			FileDlg.m_ofn.lpstrInitialDir = "c:";

			if (FileDlg.DoModal() == IDCANCEL)
			{
				::SetCurrentDirectory(acOldPath);
				return;
			}

			::SetCurrentDirectory(acOldPath);

			if (sWorldParam.WorldType == 0)
			{
				CNtlWorldMergeManager::GetInstance()->OnExportInvalidSectorLink(acTemp, FileDlg.GetPathName().GetBuffer(0));
			}
		}

	}
	::SetCurrentDirectory(acOldPath);
}

void CNtlWEFrm::OnVisionMiniindoorcull()
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->Toggle(eNWOVA_MINI_INDOOR_CULL);
}

void CNtlWEFrm::OnUpdateVisionMiniindoorcull(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_MINI_INDOOR_CULL));
}
void CNtlWEFrm::OnFileResetobjectserialid()
{
	CNtlWorldMergeManager::GetInstance()->OnReindexingObjectSerialID();
}
