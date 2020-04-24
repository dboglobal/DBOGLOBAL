//***********************************************************************************
//	File		:	NtlBlurCamera.h
//	Desc		:	
//	Begin		:	2006. 2.13
//	Copyright	:	ⓒ 2006 by Mahwang CO., Ltd
//	Author		:	Mahwang
//	Update		:	
//***********************************************************************************

#ifndef _NTL_BLUR_CAMERA_
#define _NTL_BLUR_CAMERA_


#include "rtcharse.h"
#include "NtlGameCamera.h"
#include "d3dx9.h"

#define D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_TEX1)
struct D3D_VERTEX
{
    RwReal x, y, z;
    RwReal u, v;
};

#define D3DFVF_RHW_VERTEX (D3DFVF_XYZRHW|D3DFVF_TEX1)
struct D3D_RHW_VERTEX
{
    RwReal x, y, z, w;
    RwReal u, v;
};

class CNtlBlurCamera : public CNtlGameCamera
{
private:
	RwInt32				m_nBlurTextureWidth;
	RwInt32				m_nBlurTextureHeight;

	RwTexture*			m_pSourceTexture;

	RwCamera*			m_pTempCamera;
	RwTexture*			m_pTempTexture;

	RwCamera*			m_pGaussianCamera;
	RwTexture*			m_pGaussianTexture;

	RwCamera*			m_pPowerCamera;
	RwTexture*			m_pPowerTexture;

	RwCamera*			m_pReductionCamera;
	RwTexture*			m_pReductionTexture;

	RwCamera*			m_pFakeHDRCamera;
	RwTexture*			m_pFakeHDRTexture;

	RwTexture*			m_pTestTexture;

	RwCamera*			m_pBlurCamera;

	D3D_VERTEX			m_Vertex[4];

	D3D_RHW_VERTEX		m_RhwVertex[4];
	D3D_RHW_VERTEX		m_ReductionVertex[4];

	LPDIRECT3DDEVICE9	m_lpDevice;

	enum EFFECT_PASS
	{
		EFFECT_PASS_NONE = 0,
		EFFECT_PASS_LASTDRAW,
		EFFECT_PASS_BLOOM,
		EFFECT_PASS_POWER,
		EFFECT_PASS_MONO,
		EFFECT_PASS_MONO_POWER,
		EFFECT_PASS_GAUSSAIN_X,
		EFFECT_PASS_GAUSSAIN_Y,

		EFFECT_PASS_16BOX,
		EFFECT_PASS_64BOX,

		EFFECT_PASS_MAX_COUNT,
	};

	LPD3DXEFFECT		m_lpEffect;			// Effect File Handler
	D3DXHANDLE			m_hTechnique;

	D3DXHANDLE			m_hMapWidth;
	D3DXHANDLE			m_hMapHeight;

	D3DXHANDLE			m_hGaussainWeight;

	D3DXHANDLE			m_hPowerHeight;
	D3DXHANDLE			m_hBloomHeight;

	enum BLUR_FILLTER
	{
		BLUR_FILLTER_NONE = 0,
		BLUR_FILLTER_GAUSSIAN,
		BLUR_FILLTER_16BOX,
		BLUR_FILLTER_64BOX,
		BLUR_FILLTER_REDUCTION,
		BLUR_FILLTER_FAKE_HDR,

		BLUR_FILLTER_MAX_COUNT,
	};
	enum POWER_FILLTER
	{
		POWER_FILLTER_NONE = 0,
		POWER_FILLTER_BLOOM,
		POWER_FILLTER_POWER,
		POWER_FILLTER_MONO,
		POWER_FILLTER_MONO_POWER,

		POWER_FILLTER_MAX_COUNT,
	};
	enum LAST_RENDER
	{
		LAST_RENDER_NONE = 0,
		LAST_RENDER_POWER,
		LAST_RENDER_FILLTER,

		LAST_RENDER_MAX_COUNT,
	};

	RwBool				m_bDrawTexture;

	RwInt32				m_nPowerType;
	RwInt32				m_nBlurType;

	RwInt32				m_nRenderType;

	RwReal				m_fPowerHeight;

	RwReal				m_fBloomHeightLight;
	RwInt32				m_nTextureReductionSize;

	RwInt32				m_nFakeHDRBlurCount;


	// 가우시안 
	enum { GAUSSION_WEIGHT_COUNT = 7, };									// 가중치 계산할 개수
	RwReal				m_fGaussainDispersion_sq;						// 분산의 평방근
	RwReal				m_fGaussainWeightTable[GAUSSION_WEIGHT_COUNT];	// 가중치 배열


	INPUT_HANDLE		m_handleKeyDown;

	// Help Draw Vertex
	D3D_RHW_VERTEX		m_SourceVertex[4];
	D3D_RHW_VERTEX		m_PowerVertex[4];
	D3D_RHW_VERTEX		m_FinalVertex[4];


public:
	CNtlBlurCamera(void);
	~CNtlBlurCamera(void);

	void		Destroy();
	void		DestroyBlurCamera(RwCamera*& pCamera, RwTexture*& pTexture);

	int			KeyboardDownHandler(unsigned int uiKeyData);

	RwBool		CreateBlurCamera(RwCamera*& pCamera, RwTexture*& pTexture, RwInt32 nWidth, RwInt32 nHeight, RwReal fNearClipPlane, RwReal fFarClipPlane);
	RwBool		CreateEffect();

	RwCamera*	Create(RwInt32 width, RwInt32 height, RwBool zBuffer, RwReal fNearClipPlane, RwReal fFarClipPlane);

	void		Update(RwRGBA& color, HWND hWnd);

	void		Update_Power_None(RwRGBA& color, HWND hWnd);
	void		Update_Power_Bloom(RwRGBA& color, HWND hWnd);
	void		Update_Power_Power(RwRGBA& color, HWND hWnd);
	void		Update_Power_Mono(RwRGBA& color, HWND hWnd);
	void		Update_Power_MonoPower(RwRGBA& color, HWND hWnd);

	void		Update_GaussainFiltering(RwRGBA& color, HWND hWnd);
	void		Update_ReductionFiltering(RwRGBA& color, HWND hWnd);
	void		Update_FakeHDRFiltering(RwRGBA& color, HWND hWnd);
	void		Update_16BoxFiltering(RwRGBA& color, HWND hWnd);
	void		Update_64BoxFiltering(RwRGBA& color, HWND hWnd);

	void		Update_Last(RwRGBA& color, HWND hWnd);

	void		Update_Power(RwRGBA& color, HWND hWnd);

	void		DrawTexture();
	void		DrawInfo(RtCharset* pCharset, RwInt32 iIdx);

	void		UpdateGaussainWeightTable(FLOAT dispersion);

	void		CreateReductionTexture(RwInt32 nTextureSize);

public:
	inline RwCamera* GetBlurCamera()	{ return m_pBlurCamera; }
};

#endif