#pragma once


#include "ntlworldcommon.h"
#include "rtltmap.h"


struct sSWAP_PLOBJ_INFO
{
	RwChar		_Name[64];
	RwV3d		_SRT[3];
};


#define dAUTO_SAVE_TIME (60.0f * 30.0f)


class CNtlWorldLTManager
{
protected:
	static CNtlWorldLTManager* g_pNtlWorldLTManager;

protected:
	CNtlWorldLTManager();
	virtual ~CNtlWorldLTManager();

public:
	static RwReal					m_LocalLightMapDensity;
	static vector<CNtlPLEntity*>	m_vecEntities4UVReset;
	static vector<RwTexture*>		m_vecBSPTextures;

protected:
	RtLtMapLightingSession	m_LightingSession;

	// creation modifiers
	RwUInt32		m_LightMapSize;			// should be square, a power of 2 in length, PNG and linear sampling
	RwChar			m_LightMapPrefix[32];	// prefix for texture name generation
	RwReal			m_LightMapDensity;		// Number of lightmap texels
	RwInt32			m_LightMapScale;		// lightmap material sample density
	RpLtMapStyle	m_RenderStyle;

	// state flags
	RwBool	m_bCreatingLightMaps;
	RwBool	m_bClearingLightMaps;
	RwBool  m_bLightingWorld;
	RwReal  m_bLightProgress; 
	RwBool  m_bResetLighting;
	RwReal  m_bLightingProgress;
	RwBool	m_bFullSceneLighting;

	// auto save
	RwBool	m_bAutoSaving;
	RwReal	m_AutoSaveTime;
	RwBool	m_bAutoSaveTextUI;

protected:
	// illumination callbacks
	static RpWorldSector*	SectorSetupCB(RpWorldSector* _pSector, void* _pData);
	static RpMaterial*		SetupMaterialsCB(RpMaterial* _pMat, void* _pData);
	static RpMaterial*		ScaleMaterialsDensityCB(RpMaterial* _pMat, void* _pData);
	static RpWorldSector*	SectorPreLightClearCB(RpWorldSector* _pSector, void* _pData);
	static RpClump*			ClumpPreLightClearAll128CB(RpClump* _pClump, void* _pData);
	static RpClump*			ClumpPreLightClearLMappedBlackCB(RpClump* _pClump, void* _pData);
	static RpAtomic*		AtomicPreLightClearAll128CB(RpAtomic* _pAtomic, void* _data);
	static RpAtomic*		AtomicPreLightClearLMappedBlackCB(RpAtomic* _pAtomic, void* _pData);
	static RpLight*			LightToggleCB(RpLight* _pLight, void* _pData);
	static RpAtomic*		DisableLightmapsAtomicCB(RpAtomic* _pAtomic, void* _pData);
	static RpWorldSector*	GetBSPTex4Sector(RpWorldSector* _pSector, void* _pData);
	static RpAtomic*		GetBSPTex4Mesh(RpAtomic* _pAtomic, void* _pData);
	static RwTexDictionary*	TexDictDestroyCB(RwTexDictionary *texDict, void *data __RWUNUSED__);
	static RpAtomic*		FindEntities4UVReset(RpAtomic* _pAtomic, void* _pData);

	// illumination methods
	void	CreateLightMaps();
	void	ClearLightMaps();
	void	DestroyLightmaps(RwBool _DestroyAll = FALSE);
	void	LightScene();
	void	ResetPLObjUV();

	// auto save
	void	UpdateAutoSave(RwReal _ElapsedTime);

public:
	static CNtlWorldLTManager*	GetInstance() { return g_pNtlWorldLTManager; }
	static void					CreateInstance();
	static void					DestroyInstance();
	

	void	Create();
	void	Update(RwReal _ElapsedTime);
	void	Destroy();
	void	Restore();
	void	SetAutoSave(RwBool _Flag);
	RwBool	GetAutoSave() { return m_bAutoSaving; }
	RwBool  SaveLightmap(RwTexture* _pLightmap, RwChar* _pszPLObjUID);
	RwBool	SaveBSP();
	RwBool	SaveBSPLightmap();
	RwBool	DeleteLightmapTextureFile(RwChar* _PathName);
	RwBool	RenderUIText(RtCharset* pRtCharSet, RwInt32 LenWindowWidth, RwInt32 LenWindowHeight);

	void	SetLightMapScale(RwInt32 iLightMapScale) { m_LightMapScale = iLightMapScale; }
	RwInt32	GetLightMapScale() { return m_LightMapScale; }
	void	ToggleLightMapVisibility(RwBool _VisibilityFlag);
};