/*****************************************************************************
 *
 * File			: NTLCamera.cpp
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: NTl Camera
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#include "precomp_ntlframework.h"
#include "NtlCamera.h"


CNtlCamera::CNtlCamera(void) : m_pCamera(NULL)
{
	m_fFov = 45.f;
	RwReal m_fViewWindow = RwTan(NTL_DEG2RAD(m_fFov * 0.5f));
}


CNtlCamera::~CNtlCamera(void)
{
}

/**
 * Camera 생성
 * \param width FrameBuffer ZBuffer의 가로 크기
 * \param height FrameBuffer ZBuffer의 세로 크기
 * \param fNearClipPlane NearPlane 거리
 * \param fFarClipPlane FarPlane 거리
 * \return Camera 생성 성공시 RwCamera or Null
 */
RwCamera* CNtlCamera::Create(RwInt32 width, 
							 RwInt32 height, 
							 RwBool zBuffer, 
							 RwReal fNearClipPlane, 
							 RwReal fFarClipPlane)
{
	NTL_FUNCTION("CNtlCamera::Create" );
	//m_CamRenderTexture.Create(128, 128, GetFov());
	m_pCamera = RwCameraCreate();
    if( m_pCamera )
    {
        RwCameraSetFrame(m_pCamera, RwFrameCreate());

        RwCameraSetRaster(m_pCamera, RwRasterCreate(width, height, 0, rwRASTERTYPECAMERA));

        if( zBuffer )
        {
            RwCameraSetZRaster(m_pCamera, RwRasterCreate(width, height, 0, rwRASTERTYPEZBUFFER));
        }
		
		
        if( RwCameraGetFrame(m_pCamera) && RwCameraGetRaster(m_pCamera) && 
			(!zBuffer || RwCameraGetZRaster(m_pCamera)) )
        {
			SetCameraSize(NULL);
			
			RwCameraSetNearClipPlane (m_pCamera, fNearClipPlane);
			RwCameraSetFarClipPlane (m_pCamera, fFarClipPlane);
			
			NTL_RETURN(m_pCamera);
		}
	}
	Destroy();

	
	NTL_RETURN(NULL);
}

/**
 * Camera의 Buffer들의 크기 설정 및 Fov에 따른 설정
 * Window 창이 바뀔 때 불리어지는 함수이다.
 * \param rect Camera의 가로, 세로 Size
 */
void CNtlCamera::SetCameraSize(RwRect* rect)
{
	SetCameraResize(GetCamera(), rect);
}

/**
 * Camera의 Buffer들의 크기 설정 및 Fov에 따른 설정
 * \param pCamera Camera의 설정을 할 Camera
 * \param rect Camera의 가로, 세로 Size
 */
void CNtlCamera::SetCameraResize(RwCamera* pCamera, RwRect* rect)
{
	NTL_FUNCTION("CNtlCamera::SetCameraResize");
    if( pCamera )
    {
        RwVideoMode videoMode;
        RwEngineGetVideoModeInfo(&videoMode, RwEngineGetCurrentVideoMode());

        if( !rect )
        {
            static RwRect r;

            rect = &r;
			
			rect->w = RwRasterGetWidth(RwCameraGetRaster(pCamera));
            rect->h = RwRasterGetHeight(RwCameraGetRaster(pCamera));
            rect->x = rect->y = 0;
        }
		
		if( videoMode.flags & rwVIDEOMODEEXCLUSIVE )
        {
            rect->x = rect->y = 0;
            rect->w = videoMode.width;
            rect->h = videoMode.height;
        }

        if( (rect->w > 0) && 
			(rect->h > 0) )
        {	
            RwRaster *raster = NULL;
            RwRaster *zRaster = NULL;

            raster = RwCameraGetRaster(pCamera);
			zRaster = RwCameraGetZRaster(pCamera);

			raster->width = rect->w;
			raster->height= rect->h;

			zRaster->width = rect->w;
			zRaster->height= rect->h;
			
			SetCalcFov(pCamera, GetFov());
		}
	}

	NTL_RETURNVOID();
}

void CNtlCamera::SetCalcFov(RwCamera* pCamera, RwReal fFov)
{
	NTL_FUNCTION("CNtlCamera::SetFieldOfView");
	NTL_ASSERTE(pCamera);
	
	SetFov(fFov);

	RwVideoMode videoMode;
    RwEngineGetVideoModeInfo(&videoMode, RwEngineGetCurrentVideoMode());

    RwRect rect;
    RwV2d vw;

    rect.x = 0;
    rect.y = 0;
    rect.w = RwRasterGetWidth(RwCameraGetRaster(pCamera));
    rect.h = RwRasterGetHeight(RwCameraGetRaster(pCamera));

    if (videoMode.flags & rwVIDEOMODEFFINTERLACE) rect.h *= 2;
	
	vw.x = m_fViewWindow;
    vw.y = (rect.h * m_fViewWindow) / rect.w;

    RwCameraSetViewWindow(pCamera, &vw);

    NTL_RETURNVOID();
}

void CNtlCamera::SetCalcFov(RwReal fFov)
{
	SetCalcFov(m_pCamera, fFov);
}

/**
 * Camera의 소멸
 */
void CNtlCamera::Destroy()
{
	if(m_pCamera)
	{
		RwRaster *raster;
		RwFrame *frame;

		frame = RwCameraGetFrame(m_pCamera);
		if(frame)
		{
			RwCameraSetFrame(m_pCamera, NULL);
			RwFrameDestroy(frame);
		}

		raster = RwCameraGetRaster(m_pCamera);
		if(raster)
		{
			RwRasterDestroy(raster);
			RwCameraSetRaster(m_pCamera, NULL);
		}

		raster = RwCameraGetZRaster(m_pCamera);
		if(raster)
		{
			RwRasterDestroy(raster);
			RwCameraSetZRaster(m_pCamera, NULL);
		}

		RwCameraDestroy(m_pCamera);

		m_pCamera = NULL;
	}

	//m_CamRenderTexture.Destroy();
}

RwCamera* CNtlCamera::CameraClear(RwRGBA *rgba, RwInt32 uiClearMode)
{
	return RwCameraClear(m_pCamera, rgba, uiClearMode);
}

void CNtlCamera::CameraShowRaster(HWND hWnd, RwUInt32 uiFlags)
{
	RwCameraShowRaster(m_pCamera, hWnd, uiFlags);
}

RwCamera* CNtlCamera::PostCameraBeginUpdate(RwReal fElapsed)
{
	return RwCameraBeginUpdate(m_pCamera);
}

void CNtlCamera::PostCameraEndUpdate(void)
{
	RwCameraEndUpdate(m_pCamera);
}

RwCamera* CNtlCamera::MainCameraBeginUpdate(RwReal fElapsed)
{
	return RwCameraBeginUpdate(m_pCamera);
}

void CNtlCamera::MainCameraEndUpdate(void)
{
	RwCameraEndUpdate(m_pCamera);
}

void CNtlCamera::SetProjection( RwCameraProjection rwCameraProjection ) 
{
    if(!m_pCamera)
        return;

    RwCameraSetProjection(m_pCamera, rwCameraProjection);
}

RwFrustumTestResult RpNtlCameraFrustumTestSphere(const RwCamera* pCamera, const RwSphere* pSphere, RwReal* pfUserFarDist)
{
	RwMatrix*	pmatCamera	= RwFrameGetLTM(RwCameraGetFrame(pCamera));
	RwPlane		planeFar;

	planeFar.normal		= pmatCamera->at;
	planeFar.distance	= RwV3dDotProduct(&(pmatCamera->pos + pmatCamera->at * (*pfUserFarDist)), &pmatCamera->at);

	return RpNtlCameraFrustumTestSphere(pCamera, pSphere, &planeFar);
}

RwFrustumTestResult RpNtlCameraFrustumTestSphere(const RwCamera* pCamera, const RwSphere* pSphere, RwPlane* pfUserFarPlane)
{
	const RwFrustumPlane*	frustumPlanes	= pCamera->frustumPlanes;
	RwReal					fDot;
	RwFrustumTestResult     result			= rwSPHEREINSIDE;

	fDot = RwV3dDotProduct(&pSphere->center, &pfUserFarPlane->normal) -	pfUserFarPlane->distance;
	if (fDot > pSphere->radius)
	{
		return rwSPHEREOUTSIDE;
	}
	else if (fDot > (-pSphere->radius))
	{
		result = rwSPHEREBOUNDARY;
	}

	fDot = RwV3dDotProduct(&pSphere->center, &frustumPlanes[0].plane.normal) - frustumPlanes[0].plane.distance;
	if (fDot > pSphere->radius)
	{
		return rwSPHEREOUTSIDE;
	}
	else if (fDot > (-pSphere->radius))
	{
		result = rwSPHEREBOUNDARY;
	}

	fDot = RwV3dDotProduct(&pSphere->center, &frustumPlanes[1].plane.normal) - frustumPlanes[1].plane.distance;
	if (fDot > pSphere->radius)
	{
		return rwSPHEREOUTSIDE;
	}
	else if (fDot > (-pSphere->radius))
	{
		result = rwSPHEREBOUNDARY;
	}

	fDot = RwV3dDotProduct(&pSphere->center, &frustumPlanes[2].plane.normal) - frustumPlanes[2].plane.distance;
	if (fDot > pSphere->radius)
	{
		return rwSPHEREOUTSIDE;
	}
	else if (fDot > (-pSphere->radius))
	{
		result = rwSPHEREBOUNDARY;
	}

	fDot = RwV3dDotProduct(&pSphere->center, &frustumPlanes[3].plane.normal) - frustumPlanes[3].plane.distance;
	if (fDot > pSphere->radius)
	{
		return rwSPHEREOUTSIDE;
	}
	else if (fDot > (-pSphere->radius))
	{
		result = rwSPHEREBOUNDARY;
	}

	fDot = RwV3dDotProduct(&pSphere->center, &frustumPlanes[4].plane.normal) - frustumPlanes[4].plane.distance;
	if (fDot > pSphere->radius)
	{
		return rwSPHEREOUTSIDE;
	}
	else if (fDot > (-pSphere->radius))
	{
		result = rwSPHEREBOUNDARY;
	}

	fDot = RwV3dDotProduct(&pSphere->center, &frustumPlanes[5].plane.normal) - frustumPlanes[5].plane.distance;
	if (fDot > pSphere->radius)
	{
		return rwSPHEREOUTSIDE;
	}
	else if (fDot > (-pSphere->radius))
	{
		result = rwSPHEREBOUNDARY;
	}

	return result;
}

RwFrustumTestResult RpNtlFarPlaneFrustumTestSphere(const RwCamera* pCamera, const RwSphere* pSphere, RwPlane* pfUserFarPlane)
{
	RwReal fDot = RwV3dDotProduct(&pSphere->center, &pfUserFarPlane->normal) -	pfUserFarPlane->distance;
	if (fDot > pSphere->radius)
	{
		return rwSPHEREOUTSIDE;
	}
	else if (fDot > (-pSphere->radius))
	{
		return rwSPHEREBOUNDARY;
	}
	return rwSPHEREINSIDE;
}

RwCamera* RpNtlActiveCameraSetFarDistance(RwReal fFarDistance)
{
	RwCamera* pCamera = RwCameraGetCurrentCamera();

	if (pCamera)
	{
		RwCameraEndUpdate(pCamera);

		RwUInt32 uiFogStart, uiFogEnd;

		RwD3D9GetRenderState(D3DRS_FOGSTART, (void*)&uiFogStart);
		RwD3D9GetRenderState(D3DRS_FOGEND, (void*)&uiFogEnd);

		RwCameraSetFarClipPlane(pCamera, fFarDistance);	

		pCamera = RwCameraBeginUpdate(pCamera); // BeginUpdate 시 Fog 거리가 변경된다.

		RwD3D9SetRenderState(D3DRS_FOGSTART, uiFogStart);
		RwD3D9SetRenderState(D3DRS_FOGEND, uiFogEnd);
	}	

	return pCamera;

// 	RwCamera* pCamera = RwCameraGetCurrentCamera();
// 	if (pCamera)
// 	{
// 		if (pCamera = RwCameraEndUpdate(pCamera))
// 		{
// 			if (pCamera = RwCameraSetFarClipPlane(pCamera, fFarDistance))
// 			{
// 				RwUInt32 uiFogStart, uiFogEnd;
// 
// 				RwD3D9GetRenderState(D3DRS_FOGSTART, (void*)&uiFogStart);
// 				RwD3D9GetRenderState(D3DRS_FOGEND, (void*)&uiFogEnd);
// 
// 				pCamera = RwCameraBeginUpdate(pCamera); // BeginUpdate 시 Fog 거리가 변경된다.
// 
// 				RwD3D9SetRenderState(D3DRS_FOGSTART, uiFogStart);
// 				RwD3D9SetRenderState(D3DRS_FOGEND, uiFogEnd);
// 			}
// 		}
// 		DBO_ASSERT(pCamera, "Camera Lost.");
// 	}
// 	return pCamera;
}

RwReal RpNtlActiveCameraGetFarDistance()
{
	return RwCameraGetFarClipPlane(RwCameraGetCurrentCamera());
}

RwBBox RpNtlCameraFrustumGetStandardBBox(const RwCamera* pCamera, RwInt32 iStandardSize)
{
	RwBBox resultBox = pCamera->frustumBoundBox;

	resultBox.inf.x = static_cast<RwReal>((static_cast<RwInt32>(resultBox.inf.x) / iStandardSize - 1) * iStandardSize);
	resultBox.inf.z = static_cast<RwReal>((static_cast<RwInt32>(resultBox.inf.z) / iStandardSize - 1) * iStandardSize);
	resultBox.sup.x = static_cast<RwReal>((static_cast<RwInt32>(resultBox.sup.x) / iStandardSize + 1) * iStandardSize);
	resultBox.sup.z = static_cast<RwReal>((static_cast<RwInt32>(resultBox.sup.z) / iStandardSize + 1) * iStandardSize);

	return resultBox;
}