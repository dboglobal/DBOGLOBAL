#pragma once

#include "NtlWorldCommon.h"
#include "NtlPLEvent.h"

class CNtlSpecularManager : public RWS::CEventHandler
{
private:
	static CNtlSpecularManager* g_pSpecularManager;

public:
	CNtlSpecularManager();
	virtual ~CNtlSpecularManager();

	static CNtlSpecularManager*		GetInstance() { return g_pSpecularManager; }
	static void						CreateInstance();
	static void						DestroyInstance();
	virtual void					HandleEvents(RWS::CMsg &pMsg);

	int			OnDeviceRestore();		

	RwBool		Create();
	void		Destroy();
	void		Update(RwReal fElapsed);

	RwTexture*	GetTexture();
	RwMatrix	GetMatrixTexture();

	RwBool		IsInSpecualr(const RwSphere* pSphere);	
	void		SetSpecular(const sNTL_SPECULAR& sNtlSpecular, RwBool bCompareCheck = TRUE, RwBool bActive = FALSE);

private:
	RwBool		CreateCamera();
	void		DestoryCamera();

	RwBool		CreateCameraFilter();
	void		DestroyCameraFilter();

	void		UpdateCamera(RwReal fElapsed);
	void		CalcCameraFrustum();
	void		CalcMatrixTexture();

	void		RenderToTexture();

	void		SetIm2DVertexRect(RwReal fLeft, RwReal fTop, RwReal fRight, RwReal fBottom, RwCamera* pCamera);
	void		SetIm2DVertexRGBA(RwUInt8 uiRed, RwUInt8 uiGreen, RwUInt8 uiBlue, RwUInt8 uiAlpha);

	void		RunableFade(RwUInt8 UiAlpha, RwReal fTime = 1.0f);
	void		UpdateFade(RwReal fElapsed);

private:
	DEVICE_RESTORE_HANDLE	m_hDeviceRestore;

	RwCamera*				m_pCamera;
	RwFrame*				m_pFrame;

	RwCamera*				m_pCameraFilter;
	RwFrame*				m_pFrameFilter;	
	RwRaster*				m_pRasterFilter;
	RwTexture*				m_pTextureFilter;
	RwTexture*				m_pTextureReflection;
	RwIm2DVertex			m_aIm2DBuffer[4];

	RwMatrix				m_matTexture;

	RwUInt8					m_uiAlphaOrigin;
	RwUInt8					m_uiAlphaPresent;
	RwUInt8					m_uiAlphaFade;
	RwReal					m_fFadeTime;
	RwReal					m_fFadeElapsed;
	RwBool					m_bFading;

	RwReal					m_fOffsetY;
	RwReal					m_fOffsetNear;
	RwV3d					m_vDirection;

	sNTL_SPECULAR			m_sNtlSpecular;		// 현재 스페큘라
	sNTL_SPECULAR			m_sNtlSpecularNew;	// 이제 적용될 스페큘라
};

/*
class CNtlPLSun;
class CNtlSpecularManager
{
public:
	CNtlSpecularManager();
	virtual ~CNtlSpecularManager();

	static CNtlSpecularManager* GetInstance() { static CNtlSpecularManager SpecularMananger; return &SpecularMananger; }

	RwBool		Create();
	void		Destroy();
	void		Update(RwReal fElapsed);

	RwTexture*	GetTexture();
	RwMatrix	GetMatrixTexture();

	RwBool		CanRenderSpecualr(const RwSphere* pSphere);	
	RwBool		RenderSpecualr();

private:
	RwBool		CreateCamera();
	void		DestoryCamera();

	RwBool		CreateCameraFilter();
	void		DestroyCameraFilter();

	void		UpdateCamera(RwReal fElapsed);
	void		CalcCameraFrustum();
	void		CalcMatrixTexture();
	
	void		RenderToTexture();

	void		SetIm2DVertexRect(RwReal fLeft, RwReal fTop, RwReal fRight, RwReal fBottom, RwCamera* pCamera);
	void		SetIm2DVertexRGBA(RwUInt8 uiRed, RwUInt8 uiGreen, RwUInt8 uiBlue, RwUInt8 uiAlpha);

	void		RunableFade(RwReal fDelta, RwReal fTime = 1.0f);
	void		UpdateFade(RwReal fElapsed);

	void		SetActiveSun(CNtlPLSun* pSun);

private:
	RwCamera*			m_pCamera;
	RwFrame*			m_pFrame;

	RwCamera*			m_pCameraFilter;
	RwFrame*			m_pFrameFilter;	
	RwRaster*			m_pRasterFilter;
	RwTexture*			m_pTextureFilter;
	RwTexture*			m_pTextureReflection;
	RwIm2DVertex		m_aIm2DBuffer[4];

	RwMatrix			m_matTexture;

	CNtlPLSun*			m_pActiveSun;

	RwReal				m_fWidth;
	RwReal				m_fHeight;
	RwV3d				m_vPosSun;

	RwUInt8				m_uiAlphaOrigin;
	RwUInt8				m_uiAlphaPresent;
	RwUInt8				m_uiAlphaFade;
	RwReal				m_fFadeTime;
	RwReal				m_fFadeElapsed;
	RwBool				m_bFading;
};
*/