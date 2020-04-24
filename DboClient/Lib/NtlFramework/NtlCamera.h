/*****************************************************************************
 *
 * File			: NTLCamera.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: NTl Camera
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef __NTL_CAMERA__
#define __NTL_CAMERA__

#include "NtlDebug.h"
#include "NtlMath.h"

//#define DEFAULT_ASPECTRATIO (4.0f/3.0f)
//#define DEFAULT_VIEWWINDOW (0.5f)

/**
 * \ingroup Toolkit
 * Renderware Camera Wrappler Class 이다.
 *
 *
 */
class CNtlCamera
{
protected:

	RwCamera*		m_pCamera;
	RwReal			m_fViewWindow;
	RwReal			m_fFov;
	
protected:

	void SetCalcFov(RwCamera* pCamera, RwReal fFov);

public:

	CNtlCamera(void);
	virtual ~CNtlCamera(void);

	virtual RwCamera* Create( RwInt32 width,
						      RwInt32 height, 
						      RwBool zBuffer, 
						      RwReal fNearClipPlane = 100.f, 
						      RwReal fFarClipPlane = 50000.f);
	
	virtual void SetCameraSize(RwRect* rect);												///< 화면 Size를 변했을 경우 호출되는 함수(OnResize)															
	
	void SetCameraResize(RwCamera* pCamera, RwRect* rect);									///< 화면 Size를 변했을 경우 실제 처리를 하는 함수
	
	virtual void Destroy();
	void SetCalcFov(RwReal fFov);			
																							///< Fov 적용을 하는 함수
	virtual RwCamera*	CameraClear(RwRGBA *rgba, RwInt32 uiClearMode);
	virtual void		CameraShowRaster(HWND hWnd, RwUInt32 uiFlags);

	virtual RwCamera* PostCameraBeginUpdate(RwReal fElapsed);
	virtual void PostCameraEndUpdate(void);

	virtual RwCamera* MainCameraBeginUpdate(RwReal fElapsed);
	virtual void MainCameraEndUpdate(void);

	virtual void UpdatePostEffectFilter(void) {}
	virtual void UpdateHeatHaze(RwReal fElapsedTime) {}										
    virtual void UpdateMotionBlur(RwReal fElapsedTime) {}									///< 모션 블러 텍스쳐를 갱신한다.
    virtual void SetEnableBlur(RwBool bEnable, RwReal fFadeTime = 0.0f) {}					///< 모션 블러 적용 유무를 설정한다.
    virtual RwBool GetEnableBlur() {return FALSE;}											///< 모션 블러 적용 유무를 반환한다.

	inline RwCamera* GetCamera()									{ return m_pCamera; }
	inline void SetCamera(RwCamera* pCamera)						{ m_pCamera = pCamera; }

	virtual RwCamera* GetMainCamera()								{ return m_pCamera; }

	
	inline RwReal	GetNearPlane()									{ NTL_ASSERTE(m_pCamera != NULL) return m_pCamera->nearPlane; }				///< Near Plane를 얻는다.
	inline RwReal	GetFarPlane()									{ NTL_ASSERTE(m_pCamera != NULL) return m_pCamera->farPlane; }				///< Far Plane를 얻는다.

	inline void		SetFov(RwReal fFov)								{ m_fFov = fFov; m_fViewWindow = RwTan(NTL_DEG2RAD(fFov * 0.5f)); }		///< Camera의 Fov를 설정한다.
	inline RwReal	GetFov()										{ return m_fFov; }				

    void     SetProjection(RwCameraProjection rwCameraProjection);  ///< 카메라의 프로젝션을 설정한다.
};

RwFrustumTestResult RpNtlCameraFrustumTestSphere(const RwCamera* pCamera, const RwSphere* pSphere, RwReal* pfUserFarDist);
RwFrustumTestResult RpNtlCameraFrustumTestSphere(const RwCamera* pCamera, const RwSphere* pSphere, RwPlane* pfUserFarPlane);
RwFrustumTestResult RpNtlFarPlaneFrustumTestSphere(const RwCamera* pCamera, const RwSphere* pSphere, RwPlane* pfUserFarPlane);

RwCamera*			RpNtlActiveCameraSetFarDistance(RwReal fFarDistance);
RwReal				RpNtlActiveCameraGetFarDistance();

RwBBox				RpNtlCameraFrustumGetStandardBBox(const RwCamera* pCamera, RwInt32 iStandardSize);

#endif