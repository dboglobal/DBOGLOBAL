#pragma once


#include "ceventhandler.h"
#include "gui_Staticbox.h"
#include "ShoreLineIOHandler.h"
#include "DecalIOHandler.h"
#include "PlanetIOHandler.h"

#include "NtlHistoryController.h"

#include <vector>
using std::vector;

#include "UpdateEventCompass.h"
#include "UpdateEventThumbnail.h"
#include "UpdateEventShortKey.h"
#include "UpdateEventTerrainPalette.h"

#include "UpdateEventDojoPalette.h"
#include "UpdateEventLightObjectPalette.h"

class CStaticBox;
class CNtlWorldBrush;
class CFieldUIManager;
class CMergeUIManager;
class CNtlWorldEntityPreview;
// CZ-SS
//class CNtlPLSpot;
class CNtlPLOccluder_Base;
class CNtlPLHeatHazeObject;
struct sNTL_WORLD_SHADOW_PARAM;
struct sNTL_DOODADS_PROP_PARAM;


#define dWATER_MOVE_HEIGHT_MPS (0.5f)
#define dWATER_MOVE_HEIGHT_TAB (0.1f)

class CWorldEditorEventHander : public CUpdateEventCompass,
								public CUpdateEventThumbnail,
								public CUpdateEventShortKey,
								public CUpdateEventTerrainPalette,
								public CUpdateEventLightObjectPalette,
								public CUpdateEventDojoPalette,
								public RWS::CEventHandler
{
private:
	struct sBRUSHCB_PARAM
	{
		CWorldEditorEventHander* pThis;
		int						 CurIdx;
	};

public:
	CWorldEditorEventHander(RwInt32 WindowWith, RwInt32 WindowHeight);
	~CWorldEditorEventHander();
	void Create();

public:
	CNtlWorldBrush*			m_pBrushSelector;
	CNtlWorldBrush*			m_pBrushSelectorFrsutum;
	CNtlWorldBrush*			m_pBrushSelectorPE;

private:	
	bool							m_MultipleSelect;
	bool							m_IsSomeCaptured;

	vector<CNtlPLEntity*>			m_vecSelectedObj;
	vector<CNtlPLEntity*>			m_vecAnimatibleDoodadDummy;
	CNtlWorldBrush*					m_pBrushSelection;
	CNtlWorldBrush*					m_pBrushSelectionSub;
	CNtlWorldBrush*					m_pBrush2D;

	// diffuse brush timing & Activation
	RwV3d m_vOldPos;

	// Effects
	vector<CNtlPLEntity*>	m_vecSelectedEffect;

	// World Light
	vector<CNtlPLEntity*>	m_vecSelectedWorldLight;

	// Light Indoor
	vector<CNtlPLEntity*>	m_vecSelectedLightObject;

	vector<CNtlPLEntity*>	m_vecSelectedDecalVolume;

	// Water Atomic
	vector<CNtlPLEntity*>	m_vecSelectedWaterObj;
	RwInt32					m_IdxCopyWater;
	RwBBox					m_bboxSelectWater;

	// Shadow Atomic
	vector<CNtlPLEntity*>	m_vecSelectedShadowObj;
	RwInt32					m_IdxCopyShadow;

	// normal map property
	RwIm3DVertex			m_VB[4];
	RwTexture*				m_pTileAttr;
	RwTexture*				m_pTileAttrSelector;
	RwIm3DVertex*			m_pFieldAttrGeom;

	// Effects
	vector<CNtlPLEntity*>	m_vecSelectedSE;
	CNtlPLEntity*			m_pNtlPLSE2RenderUI;

public:
	CShoreLineIOHandler m_ShoreLineIOHandler;
	CDecalIOHandler		m_DecalIOHandler;
	CPlanetIOHandler	m_PlanetIOHandler;

	// Naming
	gui::CStaticBox* m_pFieldName;

	// CZ-SS
	// Soft edged spot light
	//CNtlPLSpot* m_SelectedSS;

	// Occluder
	CNtlPLOccluder_Base* m_pSelectedOccluder;

	// HeatHaze
	CNtlPLHeatHazeObject* m_pSelectedHeatHaze;

	// normal map property
	vector<RwInt32>	m_vecTileAttr;

	// preview
	CNtlWorldEntityPreview* m_pPreview;

	// Display doodad, effect, water entities
	RwInt32 m_CntDoodad[2];
	RwInt32 m_CntEffect[2];
	RwInt32 m_CntWater[2];

	// Sector Cull
	vector<CNtlPLEntity*> m_vecSelectedSectorCullObj;

	CFieldUIManager* m_pFieldUIManager;
	CMergeUIManager* m_pMergeUIManager;

	// object editing var.
	float m_fObjMovementSensitivity;
	RwReal m_ScalingValue;
	RwReal m_RotationTabValue;

	// effect editing var; scaling, rotation, translation
	RwReal m_EffectTransform[3];

	// object bone attach
	CNtlPLObject*	m_pAttachBoneObject;
	RwInt32			m_iAttachBoneIdx;

public:
	virtual void HandleEvents(RWS::CMsg &pMsg);

private:
	// SE Circle brushes
	static RpClump*		SECircleUIDrawAllClumpCB(RpClump* _pClump, void* _pData);
	static RpClump*		SECircleUIDrawAllClumpIndoorCB(RpClump* _pClump, void* _pData);
	static RpAtomic*	SECircleUIDrawAllAtomicCB(RpAtomic* _pAtomic, void* _pData);

	// doodads capture methods
	void VerifyObjCaptured();
	bool VerifyObjCaptured(RwV3d& PosObjMid);
	bool VerifyObjCaptured(CNtlPLEntity* pNtlPLEntity);
	void UpdateObjCaptured(CNtlPLEntity* pNtlPLEntity = NULL);

	VOID RefreshPerformanceTextInfo();

	RwBool AreThereObjPSMapSet(CNtlWorldSector* _pSector);

public:
	static RpCollisionTriangle* IntersectionWaterTriCB(RpIntersection* _pIntersection, RpCollisionTriangle* _pCollTriangle, RwReal _Distance, void* _pData);
	static RpAtomic* AtomicBrushRenderCB(RpAtomic *atomic, void *data);
	static RpAtomic* AtomicBrushRenderIndoorCB(RpAtomic *atomic, void *data);

	void DeleteAllPaletteDummies();

	// 0 : Create water, 1 : Modify water, 2 : Delete water, 3 : Build Depth map, 4 : delete Depth map, 5 : modify height
	void OnWaterProc(RwInt32 Menu);

	// 0 : Create shadow, 1 : Delete shadow
	void OnShadowProc(RwInt32 _MenuIdx, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam);

	// 0 : Cull sectors, 1 : not cull sectors
	void OnSectorCullProc(RwInt32 _Menu);

	CDecalIOHandler&		GetDecalIOHandler() { return m_DecalIOHandler; }
	HWND					GetTextureViewHWnd();
	RwInt32					GetCapturedDoodadsNum() { return static_cast<RwInt32>(m_vecSelectedObj.size()); }
	RwInt32					GetCapturedShadowSectorsNum() { return static_cast<RwInt32>(m_vecSelectedShadowObj.size()); }
	RwInt32					GetCapturedWaterSectorsNum() { return static_cast<RwInt32>(m_vecSelectedWaterObj.size()); }
	
	CNtlPLObject*			GetCurDoodad();	
	sNTL_DOODADS_PROP_PARAM	GetCurDoodadsPropParam();

	void					ApplyObjectProperty(sNTL_FIELD_PROP* pNtlFieldProp, eNTL_FIELD_PROPID eNtlFieldPropID);

	// CZ-SS
	//RwBool					IsValidSoftSpot(RwV3d& _Pos, RwBool _MovingFlag = FALSE);

	CNtlInstanceEffect*		GetCurEffect();

	VOID	SetDoodadsVision(BOOL Flag);
	VOID	SetEffectsVision(RwBool Flag);
	VOID	SetSEVision(RwBool _Flag);
	VOID	SetWLVision(RwBool _Flag);
	VOID    SetAnimatibleDoodadsDummyVision(BOOL _Flag);
	VOID	SetScaling(RwInt32 MenuIdx = 0);
	VOID	SetEffectsDummy(RwBool _Flag);
	VOID	SetScaleCurDoodad(RwReal _X, RwReal _Y, RwReal _Z);
	VOID	SetTransparentTiles(RwBool _IsTileTransparency);
	VOID	SetWaterBrushRadius(RwReal _Radius);

	RwBool	SaveAll();
	RwBool	SaveConfig();
	RwBool	LoadConfig();
	void	Restore();

	void UpdatePathEngineMode();
	void UpdatePathEngineBrushLimit();
	
	void Update();
	void UpdateBrushElement();

	void UpdateSE();
	void UpdateWater();
	void UpdateDepthMap();
	VOID UpdateDepthMapAlpha();
	void UpdateShadow();
	void UpdateFieldUI();
	void UpdateEffect();
	void UpdateNMProp();
	void UpdateNMPropBrushLimit();
	void UpdateNMPropTile();
	void UpdatePlant();
	void UpdateWorldLight();
	//void UpdateLightObject();
	void UpdateLightMode();
	void UpdateHeatHaze();
	// CZ-SS
	//void UpdateSoftSpot();
	void UpdatePerformaceSectorCull();
	void UpdatePerformaceOcclusion();
	void UpdatePerformacePVS();		
	void UpdatePerformaceImposter();
	void UpdateGameProperty();
	void UpdateSkyFloating();
	void UpdateDecalVolume();

	void Render();
	void RenderBrush();
	void RenderDisplayInfo(RtCharset* pRtCharSet, RwInt32 LenWindowWidth, RwInt32 LenWindowHeight);

	void RenderTriggerUI();

	void AddSelectedObject(CNtlPLEntity* pEntity);
	void AddSelectedEffect(CNtlPLEntity* pEntity);
	void AddSelectedSE(CNtlPLEntity* pEntity);
	void AddSelectedWorldLight(CNtlPLEntity* pEntity);

	CNtlHistoryController		m_NtlHistoryController;
	CNtlHistoryAction*			m_pNtlHistoryAction;
};
