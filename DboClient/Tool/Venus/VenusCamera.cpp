//***********************************************************************************
//	File		:	VenusFramework.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\venuscamera.h"

#include "NtlPLGlobal.h"
#include "rtpick.h"

#include <d3dx9.h>

#include "NtlEffectDefine.h"

CVenusCamera::CVenusCamera(void)
{
	m_vTargetPosition.x = 0.f;
	m_vTargetPosition.y = 0.f;
	m_vTargetPosition.z = 0.f;

	m_fViewDistance		= 10.f;

	m_bRButtonDown		= FALSE;
	m_RButtonDownPosition.x = 0;
	m_RButtonDownPosition.y = 0;

	m_bMButtonDown		= FALSE;
	m_MButtonDownPosition.x = 0;
	m_MButtonDownPosition.y = 0;

	m_fMoveSpeed		= 0.1f;
	m_fRotateSpeed		= 0.1f;
	m_fWheelSpeed		= 0.5f;    
    
    SetPostEffectFilters(POST_EFFECT_FILTER_NONE);

    // 게임이 60.0f이기때문에 게임과 똑같이 맞춘다.
    SetFov(60.0f);
}

CVenusCamera::~CVenusCamera(void)
{
}

CVenusCamera& CVenusCamera::GetInstance(void)
{
	static CVenusCamera camera;
	return camera;
}

//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusCamera::Destroy()
{
	RpWorldRemoveCamera (CNtlPLGlobal::m_pRpWorld, m_pCamera);
	CNtlPostEffectCamera::Destroy();
}

//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwCamera* CVenusCamera::Create(RwInt32 width, 
							 RwInt32 height, 
							 RwBool zBuffer, 
							 RwReal fNearClipPlane, 
							 RwReal fFarClipPlane)
{
	CNtlPostEffectCamera::Create(width, height, zBuffer, fNearClipPlane, fFarClipPlane);

	CNtlPLGlobal::SetActiveCamera(m_pCamera);
	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, m_pCamera);

	return m_pCamera;
}


//------------------------------------------------------------------
//	FuncName	: InitViewMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusCamera::InitViewMatrix()
{
	InitViewMatrix(m_pCamera);	
    InitViewMatrix(m_pFakeHDRCamera);
}

void CVenusCamera::InitViewMatrix(RwCamera*	pCamera)
{
    RwFrame* pFrame = RwCameraGetFrame(pCamera);

	RwV3d vViewPosition = { 0.f, 0.f, -m_fViewDistance};

	RwFrameTranslate(pFrame, &vViewPosition, rwCOMBINEREPLACE);
	RwFrameTranslate(pFrame, &m_vTargetPosition, rwCOMBINEPOSTCONCAT);

	RwMatrix* pMatrix = RwFrameGetMatrix(pFrame);
	RwV3d vRight = *RwMatrixGetRight(pMatrix);
	RwFrameRotate(pFrame, &vRight, 30, rwCOMBINEPOSTCONCAT);
}

//------------------------------------------------------------------
//	FuncName	: MouseMessages
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusCamera::MouseMessages(UINT uMsg, POINT point, short zDelta, UINT nFlags)
{
    switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
			RwReal fWheelSpeed = m_fWheelSpeed;
			if (nFlags & MK_CONTROL) fWheelSpeed *= 0.1f;
			if (nFlags & MK_SHIFT) fWheelSpeed *= 10.1f;
			m_fViewDistance -= (zDelta / 120) * fWheelSpeed;
			UpdateViewMatrix();
		}
		break;
	case WM_RBUTTONDOWN:
		{
			m_bRButtonDown = TRUE;
			m_RButtonDownPosition = point;
		}
		break;
	case WM_RBUTTONUP:
		{
			m_bRButtonDown = FALSE;
		}
		break;
	case WM_MBUTTONDOWN:
		{
			m_bMButtonDown = TRUE;
			m_MButtonDownPosition = point;
		}
		break;
	case WM_MBUTTONUP:
		{
			m_bMButtonDown = FALSE;
		}
		break;
	case WM_MOUSEMOVE:
		{
			if (m_bRButtonDown && m_bMButtonDown)
			{
				POINT gap;
				gap.x = point.x - m_RButtonDownPosition.x;
				gap.y = point.y - m_RButtonDownPosition.y;

				MoveViewMatrix(gap, m_pCamera);                
				MoveViewMatrix(gap, m_pFakeHDRCamera);

				UpdateViewMatrix();
				m_RButtonDownPosition = point;
			}
			else if (m_bRButtonDown)
			{
				POINT gap;
				gap.x = point.x - m_RButtonDownPosition.x;
				gap.y = point.y - m_RButtonDownPosition.y;

				RotateViewMatrix(gap, m_pCamera);
				RotateViewMatrix(gap, m_pFakeHDRCamera);

				UpdateViewMatrix();

				m_RButtonDownPosition = point;
			}
/*			else if (m_bMButtonDown)
			{
				POINT gap;
				gap.x = point.x - m_MButtonDownPosition.x;
				gap.y = point.y - m_MButtonDownPosition.y;

				RwFrame* pFrame = RwCameraGetFrame(m_pRwCamera);
				RwMatrix* pMatrix = RwFrameGetMatrix(pFrame);

				RwV3d vFront = *RwMatrixGetAt(pMatrix);
				vFront.y = 0.f;
				RwV3dNormalize (&vFront, &vFront);

				RwV3d vRight = *RwMatrixGetRight(pMatrix);
				RwV3dNormalize (&vRight, &vRight);

				if (gap.x != 0)
				{
					m_vTargetPosition.x += vRight.x * gap.x * 0.1f;
					m_vTargetPosition.y += vRight.y * gap.x * 0.1f;
					m_vTargetPosition.z += vRight.z * gap.x * 0.1f;
				}
				if (gap.y != 0)
				{
					m_vTargetPosition.x += vFront.x * gap.y * 0.1f;
					m_vTargetPosition.y += vFront.y * gap.y * 0.1f;
					m_vTargetPosition.z += vFront.z * gap.y * 0.1f;
				}

				UpdateViewMatrix();

				m_MButtonDownPosition = point;
			}
*/
		}
		break;
	}
}

//-------------------------------------------------	-----------------
//	FuncName	: RotateViewMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusCamera::RotateViewMatrix(POINT gap, RwCamera* pCamera)
{
	RwFrame* pFrame = RwCameraGetFrame(pCamera);
	RwMatrix* pMatrix = RwFrameGetMatrix(pFrame);
	RwV3d vPosition = *RwMatrixGetPos(pMatrix);
	vPosition.x = -vPosition.x;
	vPosition.y = -vPosition.y;
	vPosition.z = -vPosition.z;

	RwFrameTranslate(pFrame, &vPosition, rwCOMBINEPOSTCONCAT);

	if (gap.x != 0)
	{
		RwFrameRotate(pFrame, &VEC_Y, gap.x * -m_fRotateSpeed, rwCOMBINEPOSTCONCAT);
	}
	if (gap.y != 0)
	{
		RwV3d vRight = *RwMatrixGetRight(pMatrix);
		RwFrameRotate(pFrame, &vRight, gap.y * m_fRotateSpeed, rwCOMBINEPOSTCONCAT);
	}
}
//-------------------------------------------------	-----------------
//	FuncName	: RotateViewMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusCamera::MoveViewMatrix(POINT gap, RwCamera* pCamera)
{
	RwFrame* pFrame = RwCameraGetFrame(pCamera);
	RwMatrix matView = *RwFrameGetMatrix(pFrame);

	RwV3d vHeight = { 0, 0, 0 };
	if (gap.y != 0)
	{
		vHeight.y += gap.y * m_fMoveSpeed;
	}

	RwMatrixTranslate(&matView, &vHeight, rwCOMBINEPOSTCONCAT);

	RwV3d vLength;
	RwV3dSubMacro(&vLength, &m_vTargetPosition, RwMatrixGetPos(&matView));
	m_fViewDistance = RwV3dLength(&vLength);

    if(RwV3dLength(&vLength) != 0)    
	    RwV3dNormalize(&vLength, &vLength);

	*RwMatrixGetAt(&matView) = vLength;
	RwV3dCrossProduct(RwMatrixGetUp(&matView), RwMatrixGetAt(&matView), RwMatrixGetRight(&matView));
	RwFrameTransform(pFrame, &matView, rwCOMBINEREPLACE);
}

//-------------------------------------------------	-----------------
//	FuncName	: UpdateViewMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusCamera::UpdateViewMatrix(RwV3d* pMove)
{
	if (m_bRButtonDown) return;

	m_vTargetPosition.x += pMove->x;
	m_vTargetPosition.y += pMove->y;
	m_vTargetPosition.z += pMove->z;
	UpdateViewMatrix();
}

//------------------------------------------------------------------
//	FuncName	: UpdateViewMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusCamera::UpdateViewMatrix()
{
	UpdateViewMatrix(m_pCamera);
	UpdateViewMatrix(m_pFakeHDRCamera);
}
//------------------------------------------------------------------
//	FuncName	: UpdateViewMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusCamera::UpdateViewMatrix(RwCamera* pCamera)
{
	RwFrame* pFrame = RwCameraGetFrame(pCamera);
	RwMatrix* pMatrix = RwFrameGetMatrix(pFrame);

	RwV3d vPosition = *RwMatrixGetPos(pMatrix);
	vPosition.x = -vPosition.x;
	vPosition.y = -vPosition.y;
	vPosition.z = -vPosition.z;

	RwFrameTranslate(pFrame, &vPosition, rwCOMBINEPOSTCONCAT);

	vPosition.x = RwMatrixGetAt(pMatrix)->x * -m_fViewDistance;
	vPosition.y = RwMatrixGetAt(pMatrix)->y * -m_fViewDistance;
	vPosition.z = RwMatrixGetAt(pMatrix)->z * -m_fViewDistance;

	RwFrameTranslate(pFrame, &vPosition, rwCOMBINEPOSTCONCAT);
	RwFrameTranslate(pFrame, &m_vTargetPosition, rwCOMBINEPOSTCONCAT);
}

//------------------------------------------------------------------
//	FuncName	: GetMouseRay
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusCamera::GetMouseRay(const POINT& point, RwV3d& vRayPos, RwV3d& vRayDir)
{
    RwLine lineCameraRay;
    RwV2d  v2dScreenPos;
    v2dScreenPos.x = (RwReal)point.x;
    v2dScreenPos.y = (RwReal)point.y;

    RwCameraCalcPixelRay(m_pCamera, &lineCameraRay, &v2dScreenPos);
    vRayDir = lineCameraRay.end - lineCameraRay.start;
    RwV3dNormalize(&vRayDir, &vRayDir);

    RwMatrix matViewInvert;
    RwMatrixInvert(&matViewInvert, RwCameraGetViewMatrix(m_pCamera));
    vRayPos = *RwMatrixGetPos(&matViewInvert);
}

//---------------------------------------------------------------------------
//Name		: GetArrowPos
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
RwBool CVenusCamera::GetArrowMatrix(RwMatrix& matArrow)
{
	RwV3d vPosition;
	RwReal fNearPlane = RwCameraGetNearClipPlane(m_pCamera) + 1.f;

	RwV2d vOffset = *RwCameraGetViewOffset(m_pCamera);
	RwV2d vViewWindow = *RwCameraGetViewWindow(m_pCamera);

	vPosition.x = -vOffset.x + fNearPlane * ( vViewWindow.x + vOffset.x);
	vPosition.y =  vOffset.y + fNearPlane * (-vViewWindow.y - vOffset.y);
	vPosition.z = fNearPlane;

	vPosition.x -= 0.1f;
	vPosition.y += 0.1f;

    RwMatrix LTM = *RwFrameGetLTM(RwCameraGetFrame(m_pCamera));

	RwV3dTransformPoint(&vPosition, &vPosition, &LTM);

	RwMatrixGetPos(&LTM)->x = RwMatrixGetPos(&LTM)->y = RwMatrixGetPos(&LTM)->z = 0.f;
	RwMatrixTransform(&matArrow, &LTM, rwCOMBINEREPLACE);

	RwMatrixTranslate(&matArrow, &vPosition, rwCOMBINEPOSTCONCAT);

	return TRUE;
}
//---------------------------------------------------------------------------
//Name		: GetClumpMatrix
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
RwBool CVenusCamera::GetClumpMatrix(RwMatrix& matClump)
{
	RwV3d vPosition;
	RwReal fNearPlane = RwCameraGetNearClipPlane(m_pCamera) + 1.f;

	RwV2d vOffset = *RwCameraGetViewOffset(m_pCamera);
	RwV2d vViewWindow = *RwCameraGetViewWindow(m_pCamera);

	vPosition.x = -vOffset.x + fNearPlane * (-vViewWindow.x + vOffset.x);
	vPosition.y =  vOffset.y + fNearPlane * (-vViewWindow.y - vOffset.y);
	vPosition.z = fNearPlane;

	vPosition.x += 0.1f;
	vPosition.y += 0.1f;

    RwMatrix LTM = *RwFrameGetLTM(RwCameraGetFrame(m_pCamera));

	RwV3dTransformPoint(&vPosition, &vPosition, &LTM);

	RwMatrixGetPos(&LTM)->x = RwMatrixGetPos(&LTM)->y = RwMatrixGetPos(&LTM)->z = 0.f;
	RwMatrixTransform(&matClump, &LTM, rwCOMBINEREPLACE);

	RwMatrixTranslate(&matClump, &vPosition, rwCOMBINEPOSTCONCAT);

	return TRUE;
}

RwMatrix* CVenusCamera::GetViewMatrix()
{
    RwFrame* pFrame = RwCameraGetFrame(m_pCamera);
    return RwFrameGetMatrix(pFrame);
}

RwV3d* CVenusCamera::GetViewPosition()
{
    return RwMatrixGetPos(GetViewMatrix());
}

void CVenusCamera::SetStaticView( EVenusCameraStaticView eStaticView ) 
{
    RwV3d vPos, vUp, vRight, vAt;
    vUp = YAxis;

    switch(eStaticView)
    {
    case E_STATIC_VIEW_FRONT:
        vPos = ZAxis * -m_fViewDistance;
        vRight = XAxis;
        vAt = ZAxis;        
        break;
    case E_STATIC_VIEW_BACK:
        vPos = ZAxis * m_fViewDistance;
        vRight = XAxis * -1.0f;
        vAt = ZAxis * -1.0f;
        break;
    case E_STATIC_VIEW_LEFT:
        vPos = XAxis * m_fViewDistance;
        vRight = ZAxis;
        vAt = XAxis * -1.0f;
        break;
    case E_STATIC_VIEW_RIGHT:
        vPos = XAxis * -m_fViewDistance;
        vRight = ZAxis * -1.0f;
        vAt = XAxis;
        break;
    case E_STATIC_VIEW_UP:
        vPos = YAxis * m_fViewDistance;
        vUp = ZAxis * -1.0f;
        vRight = XAxis * -1.0f;
        vAt = YAxis * -1.0f;
        break;
    case E_STATIC_VIEW_DOWN:
        vPos = YAxis * -m_fViewDistance;
        vUp = ZAxis;
        vRight = XAxis * -1.0f;
        vAt = YAxis;
        break;
    }

    RwMatrix* pmatCamera = GetViewMatrix();
    *RwMatrixGetRight(pmatCamera) = vRight;
    *RwMatrixGetUp(pmatCamera)    = vUp;
    *RwMatrixGetAt(pmatCamera)    = vAt;
    *RwMatrixGetPos(pmatCamera)   = vPos;
    RwMatrixUpdate(pmatCamera);

    // FakeHDR 카메라도 업데이트 해줘야 한다.
    RwFrame* pFrame = RwCameraGetFrame(m_pFakeHDRCamera);
    RwMatrix* pmatHDRCamera = RwFrameGetMatrix(pFrame);
    *pmatHDRCamera = *pmatCamera;

    UpdateViewMatrix();
}