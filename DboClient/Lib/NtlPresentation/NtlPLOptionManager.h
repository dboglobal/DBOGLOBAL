#pragma once

//|-------------------------------------------------------------------------------------------------------------------
//|	Topic	| CNtlPLOptionManager
//|---------|---------------------------------------------------------------------------------------------------------
//|	Content | Visual Option Test를 위한 Class
//|---------|---------------------------------------------------------------------------------------------------------
//|	Author	| Cz
//|-------------------------------------------------------------------------------------------------------------------

class CNtlPLOptionManager
{
private:
	static CNtlPLOptionManager*		g_pOptionManager;

public:
	static CNtlPLOptionManager*		GetInstance() { return g_pOptionManager; }
	static void						CreateInstance();
	static void						DestroyInstance();

public:
	CNtlPLOptionManager();
	virtual ~CNtlPLOptionManager();

	RwBool		Create();
	void		Destroy();

	RwBool		Update(const RwCamera* pCameram, RwReal fElapsed);

public:
	// Object 가시거리
	void		SetObjectFar(RwReal fFar);
	RwReal		GetObjectFar();	
	
	// Terrain 가시거리
	void		SetTerrainFar(RwReal fFar);
	RwReal		GetTerrainFar();

	// Terrain Shadow Render 유무
	void		SetTerrainShadow(RwBool bRender);
	RwBool		GetTerrainShadow();

	void		SetWaterSpecular(RwBool bRender);
	RwBool		GetWaterSpecular();

	// 날씨 세부 묘사
	void		SetWeatherLevel(RwUInt32 uiLevel);
	RwUInt32	GetWeatherLevel();

	// 풀 가시거리
	void		SetPlantClipDist(RwReal fDist);
	RwReal		GetPlantClipDist();

	// 아지랑이 유무
	void		SetHeatHaze(RwBool bUse);
	RwBool		GetHeatHaze();

	// 텍스쳐 레벨 // 낮을수록 높음. 0 == 원본.
	void		SetTextureQuality(RwUInt32 uiQualityLevel);
	RwUInt32	GetTextureQuality();

#ifdef dNTL_WORLD_COMBINE
	// Terrain Texture Combine // I will not use it.
	void		SetTerrainTextrueCombine(RwBool bUse);
	RwBool		GetTerrainTextrueCombine();
#endif

private:
	RwReal			m_fFarObject;
	RwReal			m_fFarTerrain;

	RwBool			m_bWaterSpecular;

	RwUInt32		m_uiWeatherLevel;

#ifdef dNTL_WORLD_COMBINE
	RwBool			m_bTerrainCombine;
#endif
};

inline CNtlPLOptionManager* GetNtlPLOptionManager(void)
{
	return CNtlPLOptionManager::GetInstance(); 
}