//***********************************************************************************
//	File		:	NtlPostEffectCamera.h
//	Desc		:	
//	Begin		:	2006. 2.13
//	Copyright	:	¨Ï 2006 by Mahwang CO., Ltd
//	Author		:	Mahwang
//	Update		:	
//***********************************************************************************

#ifndef _NTL_POST_EFFECT_CAMERA_
#define _NTL_POST_EFFECT_CAMERA_


#include "NtlCamera.h"
#include "InputHandler.h"
#include "NtlDeviceRestoreHandler.h"
#include "d3dx9.h"

class CNtlPostEffectCamera : public CNtlCamera
{
private:
	static				CNtlPostEffectCamera*	m_pInstance;

public:
	RwBool				m_bUsePostEffect;

	RwInt32				m_nBlurTextureWidth;
	RwInt32				m_nBlurTextureHeight;

	RwTexture*			m_pSourceTexture;

	RwCamera*			m_pTempCamera;
	RwTexture*			m_pTempTexture;

	RwCamera*			m_pReductionCamera;
	RwTexture*			m_pReductionTexture;

	RwCamera*			m_pFakeHDRCamera;
	RwTexture*			m_pFakeHDRTexture;

	RwCamera*			m_pBlurCamera;

	RwIm3DVertex		m_3dVertex[4];
	RwIm2DVertex		m_RhwVertex[4];
	RwIm2DVertex		m_RhwReductionVertex[4];

	LPDIRECT3DDEVICE9	m_lpDevice;

	enum EFFECT_PASS
	{
		EFFECT_PASS_NONE = 0,
		EFFECT_PASS_LASTDRAW,
		EFFECT_PASS_MONO_POWER,
		EFFECT_PASS_16BOX,

		EFFECT_PASS_MAX_COUNT,
	};

	LPD3DXEFFECT		m_lpEffect;			// Effect File Handler
	D3DXHANDLE			m_hTechnique;

	D3DXHANDLE			m_hMapWidth;
	D3DXHANDLE			m_hMapHeight;

	D3DXHANDLE			m_hMonoPowerHeight;
	D3DXHANDLE			m_hRGB2Y;

	RwBool				m_bUseBlurEffect;

	RwReal				m_fMonoPowerHeight;
	D3DXVECTOR4			m_vRGB2Y;
	RwInt32				m_nTextureReductionSize;

	INPUT_HANDLE		m_handleKeyDown;

	DEVICE_RESTORE_HANDLE	m_handleRestore;
	// Help Draw Vertex
	RwIm2DVertex		m_SourceVertex[4];
	RwIm2DVertex		m_PowerVertex[4];
	RwIm2DVertex		m_FinalVertex[4];

	// comic effect
	enum COMIC_STATE
	{
		COMIC_STATE_IDLE = 0,
		COMIC_STATE_FADE_OUT,
		COMIC_STATE_FADE_IN,

		COMIC_STATE_MAX_COUNT,
	};

	RwTexture*			m_pCominTexture;
	RwInt32				m_nComicState;
	RwInt32				m_nComicAlpha;
	RwIm2DVertex		m_ComicVertex[18];

	RwImVertexIndex		m_ComicIndex[24];
	RwImVertexIndex		m_ComicIndex2[48];
	RwImVertexIndex		m_ComicIndex3[4];

public:
	CNtlPostEffectCamera(void);
	~CNtlPostEffectCamera(void);

	void		Destroy();
	void		DestroyBlurCamera(RwCamera*& pCamera, RwTexture*& pTexture);

	RwBool		CreateBlurCamera(RwCamera*& pCamera, RwTexture*& pTexture, RwInt32 nWidth, RwInt32 nHeight, RwReal fNearClipPlane, RwReal fFarClipPlane);
	RwBool		CreateEffect();

	RwCamera*	Create(RwInt32 width, RwInt32 height, RwBool zBuffer, RwReal fNearClipPlane, RwReal fFarClipPlane);

	void		BuildVertex(RwInt32 width, RwInt32 height);
	void		SetRhwVertex(RwIm2DVertex* pVertex, RwInt32 nStartX, RwInt32 nStartY, RwInt32 width, RwInt32 height);
	void		SetComicVertex(RwInt32 width, RwInt32 height);

	void		UpdateBlurFilter(RwRGBA& color);

	void		Update_Power_None(RwRGBA& color);
	void		Update_Power_MonoPower(RwRGBA& color);
	void		Update_FakeHDRFiltering(RwRGBA& color);

	RwBool		BeginCameraUpdate(RwRGBA& color);
	void		EndCameraUpdate(RwRGBA& color);

	void		CreateReductionTexture(RwInt32 nTextureSize);

	int			KeyboardDownHandler(unsigned int uiKeyData);

	void		DrawTexture();

	virtual		void SetCameraSize(RwRect* rect);

	int			RestoreHandler();

public:
	static CNtlPostEffectCamera*		GetInstance(void);
	inline RwCamera* GetBlurCamera()	{ return m_pBlurCamera; }
};

#endif