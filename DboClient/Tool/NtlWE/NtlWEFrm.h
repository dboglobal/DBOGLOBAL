// NtlWEFrm.h : CNtlWEFrm 클래스의 인터페이스
//


#pragma once


class CCameraAttrDlg;
class CPaletteTerrain;
class CPaletteOrnament;	
class CPaletteUnit;		
class CPaletteRegion;		
class CPaletteTrigger;		
class CPaletteFieldUI;
class CPaletteWater;
class CPaletteShadow;
class CPaletteEffect;
class CPaletteNMProp;
class CPaletteSE;
class CPaletteSpawn;
class CPaletteSoftSpot;
class CPaletteWorldLight;
class CPaletteLightAndShadow;
class CPalettePerformance;
class CPalettePerformanceSectorCull;
class CPalettePerformanceOcclusion;
class CPalettePerformancePVS;
class CPalettePerformanceImposter;
class CPaletteWaterTab;
class CPaletteShoreLine;
class CPaletteOrnamentTabDlg;
class CPaletteDecalDlg;
class CPalettePlantDlg;
class CFieldSearchDlg;
class CFieldHeatHazeDlg;
class CPaletteHeatHazeDlg;
class CWorldViewDlg;
class CPathMeshFindDlg;
class CSaberTestDlg;
class CPaletteLightObject;
class CBookmarkDlg;
class CPaletteLightMode;
class CPaletteGameProperty;
class CPaletteSkyFloatingEntityDlg;
class CPaletteDecalVolumeDlg;
class CPaletteDojoDlg;

class CNtlWEFrm : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CNtlWEFrm();
	DECLARE_DYNCREATE(CNtlWEFrm)

// 특성
public:

// 작업
public:

// 재정의
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현
public:
	virtual ~CNtlWEFrm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CDialogBar  m_wndDlgBar;

public:
	CPaletteSpawn*					m_pSpawnPalette;
	CPaletteTerrain*				m_pTerrainPalette;
	CPaletteOrnament*				m_pOrnamentPalette;
	CPaletteOrnamentTabDlg*			m_pOrnamentTabPalette;
	CPaletteDecalDlg*				m_pDecalPalette;
	CPaletteDecalVolumeDlg*			m_pDecalVolumePalette;
	CPalettePlantDlg*				m_pPlantPalette;
	CPaletteHeatHazeDlg*			m_pHeatHazePalette;
	CPaletteFieldUI*				m_pFieldUI;
	CPaletteWater*					m_pWaterPalette;
	CPaletteShadow*					m_pShadowPalette;
	CPaletteEffect*					m_pEffectPalette;
	CPaletteUnit*					m_pUnitPalette;
	CPaletteRegion*					m_pRegionPalette;
	CPaletteTrigger*				m_pTriggerPalette;
	CPaletteNMProp*					m_pNMPropPalette;
	CPaletteSE*						m_pSEPalette;
	// CZ-SS
	//CPaletteSoftSpot*				m_pSoftSpotPalette;
	CPaletteWorldLight*				m_pWorldLightPalette;
	CPaletteLightObject*			m_pLightObjectPalette;
	CPaletteLightMode*				m_pLightModePalette;
	CPaletteLightAndShadow*			m_pLightAndShadowPalette;
	CPalettePerformance*			m_pPerformancePalette;
	CPalettePerformanceSectorCull*	m_pPerformanceSectorCull;
	CPalettePerformanceOcclusion*	m_pPerformanceOcclusion;
	CPalettePerformancePVS*			m_pPerformancePVS;
	CPalettePerformanceImposter*	m_pPerformanceImposter;
	CPaletteShoreLine*				m_pShoreLinePalette;
	CPaletteWaterTab*				m_pWaterTabPalette;
	CPaletteGameProperty*			m_pGamePropertyPalette;
	CPaletteDojoDlg*				m_pDojoPalette;

	CCameraAttrDlg*					m_pCameraPortal;
	CFieldSearchDlg*				m_pFieldSearchDlg;
	CPathMeshFindDlg*				m_pPathMeshFindDlg;
	CFieldHeatHazeDlg*				m_pFieldHeatHazeDlg;
	CWorldViewDlg*					m_pWorldViewDlg;
	CSaberTestDlg*					m_pSaberTest;
	CBookmarkDlg*					m_pBookmarkDlg;
	CPaletteSkyFloatingEntityDlg*	m_pSkyFloatingDlg;

public:
	

// 메시지 맵 함수를 생성했습니다.
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

private:
	void InitializeDialogs();

public:
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();

	afx_msg void OnPaletteShow(RwInt32 ePalette);
	afx_msg void OnPaletteTerrain();
	afx_msg void OnPaletteOrnament();
	afx_msg void OnPaletteUnit();
	afx_msg void OnPaletteRegion();
	afx_msg void OnPaletteTrigger();
	afx_msg void OnPaletteSE();
	afx_msg void OnPaletteSpawn();
	afx_msg void OnPaletteWater();
	afx_msg void OnPaletteShadow();
	afx_msg void OnPaletteSoftSpot();
	afx_msg void OnPaletteEffect();
	afx_msg void OnPalettePerformanceSectorCull();
	afx_msg void OnPalettePerformanceOcclusion();
	afx_msg void OnPalettePerformancePVS();
	afx_msg void OnPalettePerformanceImposter();

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnOptionFog();
	afx_msg void OnUpdateOptionFog(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaletteTerrain(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaletteOrnament(CCmdUI *pCmdUI);

	afx_msg void OnUiFieldguideline();
	afx_msg void OnUpdateUiFieldguideline(CCmdUI *pCmdUI);
	afx_msg void OnUiSectorguideline();
	afx_msg void OnUpdateUiSectorguideline(CCmdUI *pCmdUI);
	afx_msg void OnUiBlockguideline();
	afx_msg void OnUpdateUiBlockguideline(CCmdUI *pCmdUI);

	afx_msg void OnCameraPortal();
	afx_msg void OnSaberTest();
	afx_msg void OnWorldView();
	
	afx_msg void OnFileSaveall();
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnPaletteFieldmerge();
	afx_msg void OnUpdatePaletteFieldmerge(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaletteWater(CCmdUI *pCmdUI);
	afx_msg void OnGenerateGeneratecynisfield();
	afx_msg void OnOptionMapobj();
	afx_msg void OnUpdateOptionMapobj(CCmdUI *pCmdUI);
	afx_msg void OnOptionWater();
	afx_msg void OnUpdateOptionWater(CCmdUI *pCmdUI);
	afx_msg void OnGenerateGeneratecynisdoodads();
	afx_msg void OnUpdatePaletteShadow(CCmdUI *pCmdUI);
	afx_msg void OnObjectAnimatibleDoodadDummy();
	afx_msg void OnUpdateObjectAnimatibleDoodadDummy(CCmdUI *pCmdUI);
	//afx_msg void OnObjectWireterrain();
	afx_msg void OnUpdateObjectWireterrain(CCmdUI *pCmdUI);
	afx_msg void OnObjectDisplaywireframe();
	afx_msg void OnUpdateObjectDisplaywireframe(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaletteEffect(CCmdUI *pCmdUI);
	afx_msg void OnVisionEffects();
	afx_msg void OnUpdateVisionEffects(CCmdUI *pCmdUI);
	afx_msg void OnVisionDoodadsAnimation();
	afx_msg void OnUpdateVisionDoodadsAnimation(CCmdUI *pCmdUI);
	afx_msg void OnPaletteNormalmapproperty();
	afx_msg void OnUpdatePaletteNormalmapproperty(CCmdUI *pCmdUI);
	afx_msg void OnVisionSoundmute();
	afx_msg void OnUpdateVisionSoundmute(CCmdUI *pCmdUI);
	afx_msg void OnInfoVersion();
	afx_msg void OnVisionVisibilitydistance();
	afx_msg void OnUpdateVisionVisibilitydistance(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaletteSpawn(CCmdUI *pCmdUI);
	afx_msg void OnVisionCollisionobject();
	afx_msg void OnUpdateVisionCollisionobject(CCmdUI *pCmdUI);

public:
	//VOID ExportTriggerTable(FILE* pFile);

	afx_msg void OnExportExportmapprop();
	afx_msg void OnExportPec();
	afx_msg void OnMenuExportTrigger();
public:
	afx_msg void OnVisionBloomeffect();
public:
	afx_msg void OnUpdateVisionBloomeffect(CCmdUI *pCmdUI);
public:
	afx_msg void OnPaletteSoundeffect();
public:
	afx_msg void OnUpdatePaletteSoundeffect(CCmdUI *pCmdUI);
public:
	afx_msg void OnPalettePerformance();
public:
	afx_msg void OnUpdatePalettePerformance(CCmdUI *pCmdUI);
public:
	afx_msg void OnVisionPerformancesectorcull();
public:
	afx_msg void OnUpdateVisionPerformancesectorcull(CCmdUI *pCmdUI);
public:
	afx_msg void OnExportExportmapname();
public:
	afx_msg void OnVisionPvslocking();
public:
	afx_msg void OnUpdateVisionPvslocking(CCmdUI *pCmdUI);
public:
	afx_msg void OnVisionPvsactivation();
public:
	afx_msg void OnUpdateVisionPvsactivation(CCmdUI *pCmdUI);
public:
	afx_msg void OnVisionProjectionshadow();
public:
	afx_msg void OnUpdateVisionProjectionshadow(CCmdUI *pCmdUI);
public:
	afx_msg void OnVisionFogactivation();
public:
	afx_msg void OnUpdateVisionFogactivation(CCmdUI *pCmdUI);
public:
	afx_msg void OnVisionPlant();
public:
	afx_msg void OnUpdateVisionPlant(CCmdUI *pCmdUI);
public:
	afx_msg void OnExportExportheightfieldy();
public:
	afx_msg void OnVisionOccluderactivation();
public:
	afx_msg void OnUpdateVisionOccluderactivation(CCmdUI *pCmdUI);
	afx_msg void OnVisionObjectwireframe();
	afx_msg void OnUpdateVisionObjectwireframe(CCmdUI *pCmdUI);
	afx_msg void OnUiObjectwireframe();
	afx_msg void OnExportExportheightfieldpe();
	afx_msg void OnExportExportmappropPe();
	afx_msg void OnUiObjectvisible();
	afx_msg void OnVisionSoundmuteBgm();
	afx_msg void OnUpdateVisionSoundmuteBgm(CCmdUI *pCmdUI);
	afx_msg void OnVisionSoundmuteEnv();
	afx_msg void OnUpdateVisionSoundmuteEnv(CCmdUI *pCmdUI);
	afx_msg void OnVisionWeather();
	afx_msg void OnUpdateVisionWeather(CCmdUI *pCmdUI);
	afx_msg void OnVisionHeathaze();
	afx_msg void OnUpdateVisionHeathaze(CCmdUI *pCmdUI);

	// 인도어
	afx_msg void OnWorldNew();
	afx_msg void OnWorldLoad();
	afx_msg void OnExportExportpathenginedata();
	afx_msg void OnUiBspSector();
	afx_msg void OnUpdateUiBspSector(CCmdUI *pCmdUI);
	afx_msg void OnUiBspWireframe();
	afx_msg void OnUpdateUiBspWireframe(CCmdUI *pCmdUI);
	afx_msg void OnUtilPathengineutil();
	afx_msg void OnSkyFloatDlg();
	
	afx_msg void OnUtilBookmark();

	// 인도어
	afx_msg void OnPaletteGameproperty();
	afx_msg void OnUpdatePaletteGameproperty(CCmdUI *pCmdUI);
	afx_msg void OnPaletteWorldview();
public:
	afx_msg void OnFileSaveAuto();
public:
	afx_msg void OnUpdateFileSaveAuto(CCmdUI *pCmdUI);
public:
	afx_msg void OnVisionLightmap();
public:
	afx_msg void OnUpdateVisionLightmap(CCmdUI *pCmdUI);
	afx_msg void OnUiBspWireframeNv();
	afx_msg void OnUpdateUiBspWireframeNv(CCmdUI *pCmdUI);

	CWorldViewDlg* GetWorldView() { return m_pWorldViewDlg; }
	afx_msg void OnVisionTerrain();
	afx_msg void OnUpdateVisionTerrain(CCmdUI *pCmdUI);
	afx_msg void OnVisionTriggerui();
	afx_msg void OnUpdateVisionTriggerui(CCmdUI *pCmdUI);
	afx_msg void OnExportExportinvalidsectorlink();
	afx_msg void OnVisionMiniindoorcull();
	afx_msg void OnUpdateVisionMiniindoorcull(CCmdUI *pCmdUI);
	afx_msg void OnFileResetobjectserialid();
};
