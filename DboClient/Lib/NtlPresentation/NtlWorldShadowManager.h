////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include <vector>
using std::vector;


class CNtlWorldShadow;
class CNtlWorldFieldManager;
class CNtlWorldSector;
class CNtlPLEntity;
class CNtlPLObject;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define dTEXEL_OFFSET (0.5f)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(1)

struct sNTL_WORLD_SHADOW_PARAM
{
	RwBool	_AntiAlias;
	RwBool	_Fade;
	RwInt32 _LightDir;
	RwInt32	_BlurPass;
	RwInt32	_TexSize;	
	RwRGBA	_RGBA;


	sNTL_WORLD_SHADOW_PARAM()
	{
		_AntiAlias	= TRUE;
		_Fade		= FALSE;
		_LightDir	= 3;
		_BlurPass	= 0;
		_TexSize	= 128;	
		_RGBA.red	= 128;
		_RGBA.green	= 128;
		_RGBA.blue	= 128;
		_RGBA.alpha	= 100;
	}
};

#pragma pack()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlWorldShadowManager
{
public:
	CNtlWorldShadowManager(void);
	~CNtlWorldShadowManager(void);
	static CNtlWorldShadowManager* GetInst(void);

public:
	RwCamera*					m_pCam;
	RwCamera*					m_pIPCam;
	RwTexture*					m_pR2S;
	RwRaster*					m_pShadowCameraRaster;
	RwRaster*					m_pShadowCameraRasterAA;
	RwRaster*					m_pShadowCameraRasterBlur;
	
private:
	// projection shadow map, brush
	RwTexture*		m_pPSTexBrush;
	RwIm2DVertex	m_PSTexBrush[4];
	RwIm2DVertex	m_UITex[4];

	vector<CNtlPLObject*> m_vecShadowDoodads;

public:
	RwReal			m_BrushRadius;
	RwReal			m_Opacity;
	RwMatrix		m_matShadowProp;

private:
	void		CreateUITex();
	void		CreateCamViewWindow();
	RwCamera*	CreateShadowCamera(RwInt32 _TexSize);
	RwCamera*	CreateShadowCameraIP(RwInt32 _TexSize);
	RwBool		CreateAA(RwRaster* _pDstRaster, RwRaster* _pSrcRaster, RwCamera* _pCam);
	RwBool		CreateShadowRasterBlur(RwRaster* _pShadowRaster, RwRaster* _pTempRaster, RwCamera* _pCam, RwUInt32 _NumPass);
	void		DeleteShadowCamera(RwCamera* pShadowCamera);

	void		SetShadowCameraDir(RwMatrix& _MatLight);
	void		SetShadowCameraFrustum(RwReal _Radius);
	void		SetShadowCameraCenter(RwV3d& Center);
	void		GetNeighborSecMap(RwV3d& _Pos, CNtlWorldFieldManager* _pNWFManager, CNtlWorldSector* _pNeighborSecMap[9]);

	RwBool		RenderIm2DRenderQuad(RwReal x1, RwReal y1, RwReal x2, RwReal y2, RwReal z, RwReal recipCamZ, RwReal uvOffset);
	void		RenderAtR2S(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager, CNtlPLObject* _pDoodadPSObj = NULL);
	RwBool		RenderRasterFade(RwCamera *camera,	RwRaster *raster, /*RwReal shadowStrength*/ RwReal fadeDist, RwReal objectRadius, RwBool enableFading, RwRGBA& Color);

	RwBool		IsCollidedInTex();

	VOID CreateDoodadSPS(CNtlPLObject* _pNPObj, RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager);

public:
	RwBool	UpdateParam(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager);

	RwBool	CreateShadowProp(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager);
	RwBool	DeleteShadowProp(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager);
	RwBool	CreateDoodadShadow(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager);
	RwBool	DeleteDoodadShadow(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager);
	RwBool	Create(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager);
	RwBool	Delete(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager);
	RwBool	DeleteShadowObj();
	void	Destroy();

	void	RenderTexPanel(std::vector<CNtlPLEntity*>& vecSectors, CNtlWorldFieldManager* _pNWFManager);
	void	UpdateSectorPSTex(std::vector<CNtlPLEntity*>& vecSectors, CNtlWorldFieldManager* _pNWFManager);

	void	SetShadowProp(RwV3d& vTilePos, RwBool bIsShadow, CNtlWorldFieldManager* _pNWFManager);
	RwBool	CheckShadowFilter(RwTexture* pTexture, RwInt32 iIdx, RwInt32 iSize, RwRGBA rgbaShadow, RwReal fShadowPer = 1.0f); // iIdx(0) = Left Top
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////