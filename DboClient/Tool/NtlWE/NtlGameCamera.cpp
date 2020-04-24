#include "stdafx.h"
#include "NtlGameCamera.h"
#include "NtlCameraHelper.h"
#include "NtlSLEventFunc.h"
#include "NtlTimer.h"
#include "ntlworldcommon.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldDI.h"
#include "NtlWeControlUi.h"

#include <d3d9.h>

#define dEPSILON 0.000001

CNtlGameCamera::CNtlGameCamera(void) : 
	m_fDistance(100.f),
	m_fMaxDistance(35000.f),
	m_fMinDistance(200.f),
	m_fPrefDistance(10000.f),
	m_fSmoothDistance(0.025f),
	m_fXAngle(0.0f),
    m_fMaxXAngle(90.0f),
    m_fMinXAngle(-90.0f),
    //m_fPrefXAngle(45.0f),
	m_fPrefXAngle(0.0f),
    m_fSmoothXAngle(0.025f),
	m_fYAngle(0.f),
	m_fMaxYAngle(180.f),
	m_fMinYAngle(-180.f),
	m_fPrefYAngle(0.f),
	m_fSmoothYAngle(0.025f),
	m_pTargetCharacter(NULL),
	m_fLookAtHeight(140.f),
	m_CameraFrontBackSpeed(0.0f),
	m_CameraStrafeSpeed(0.0f),
	m_CameraUpDownSpeed(0.0f),
	m_fSpeed(10.f)
{
	m_pCachedMatrix = RwMatrixCreate();
	m_vLookAt.x = 0.f;
	m_vLookAt.y = 0.0f;
	m_vLookAt.z = 0.f;

	m_OldMouseMove.x = 0;
	m_OldMouseMove.y = 0;

	m_iCameraType = CNtlGameCamera::CAMERA_LOOK_AT;
	m_handleMouseWheel = CInputHandler::GetInstance()->LinkMouseWheel(this, &CNtlGameCamera::MouseWheelHandler);
	m_handleMouseMove  = CInputHandler::GetInstance()->LinkMouseMove(this, &CNtlGameCamera::MouseMoveHandler);
	m_handleMouseDown  = CInputHandler::GetInstance()->LinkMouseDown(this, &CNtlGameCamera::MouseDownHandler);
	m_handleKeyDown    = CInputHandler::GetInstance()->LinkKeyDown(this, &CNtlGameCamera::KeyboardDownHandler);
	m_bMouseMoveValidFlag = false;

}	

CNtlGameCamera::~CNtlGameCamera(void)
{
	if(m_pCachedMatrix != NULL)
	{
		RwMatrixDestroy(m_pCachedMatrix);
		m_pCachedMatrix = NULL;
	}

	CInputHandler::GetInstance()->UnLinkMouseWheel(m_handleMouseWheel);
	CInputHandler::GetInstance()->UnLinkMouseMove(m_handleMouseMove);
	CInputHandler::GetInstance()->UnLinkMouseDown(m_handleMouseDown);
	CInputHandler::GetInstance()->UnLinkKeyDown(m_handleKeyDown);

}

void CNtlGameCamera::HandleMouse(RwReal fAngleX, RwReal fAngleY)
{
	m_fYAngle = fAngleY;
	m_fXAngle = fAngleX;
	
	if (m_fPrefXAngle > m_fMaxXAngle) m_fPrefXAngle = m_fMaxXAngle;
	if (m_fPrefXAngle < m_fMinXAngle) m_fPrefXAngle = m_fMinXAngle;
}


void CNtlGameCamera::HandleMouse(RwReal fDeltaZ)
{
	m_fPrefDistance = m_fPrefDistance + fDeltaZ;
	if (m_fPrefDistance > m_fMaxDistance) m_fPrefDistance = m_fMaxDistance;
	if (m_fPrefDistance < m_fMinDistance) m_fPrefDistance = m_fMinDistance;
}

int	CNtlGameCamera::MouseWheelHandler(unsigned int uiMouseData)
{
	SMouseData *pData = (SMouseData *)uiMouseData;
	HandleMouse(-pData->zDelta*5.0f);

	return TRUE;
}

int	CNtlGameCamera::MouseMoveHandler(unsigned int uiMouseData)
{
    SMouseData *pData = (SMouseData *)uiMouseData;
	if( !(pData->uiFlags & MK_RBUTTON) )
		return 1;


	if(m_bMouseMoveValidFlag)
	{
		RwV2d delta;
		delta.x = m_OldMouseMove.x - pData->iPosX;
		delta.y = m_OldMouseMove.y - pData->iPosY;
		HandleMouse(delta.y, delta.x);

		m_OldMouseMove.x = static_cast<RwReal>(pData->iPosX);
		m_OldMouseMove.y = static_cast<RwReal>(pData->iPosY);
	}
	else
	{
		m_bMouseMoveValidFlag = true;
	}

	return TRUE;
}

int CNtlGameCamera::MouseDownHandler(unsigned int uiMouseData)
{
	SMouseData *pData = (SMouseData *)uiMouseData;
	if( pData->uiFlags & MK_RBUTTON)
	{
		m_OldMouseMove.x = static_cast<RwReal>(pData->iPosX);
		m_OldMouseMove.y = static_cast<RwReal>(pData->iPosY);
	}

	return TRUE;
}

int CNtlGameCamera::KeyboardDownHandler(unsigned int uiKeyData)
{
	SKeyData *pData = (SKeyData *)uiKeyData;

	return TRUE;
}

void CNtlGameCamera::ClampTranslation(RwV3d& Pos, RwBool IsCamMoving)
{
	RwReal Max;
	RwReal Min;

	if(IsCamMoving)
	{
		//Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - RwCameraGetFarClipPlane(m_pCamera) - 1);
		//Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + RwCameraGetFarClipPlane(m_pCamera) + 1);
		Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - dNTL_WORLD_EXC_AOI_SIZE - 1);
		Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + dNTL_WORLD_EXC_AOI_SIZE + 1);
	}
	else
	{
		Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax);
		Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin);
	}

	CLAMP(Pos.x, Min, Max);
	//CLAMP(Pos.y, Min, Max);
	CLAMP(Pos.z, Min, Max);
}

void CNtlGameCamera::Translation(RwV3d& NewPos, CNtlPLWorldEntity *pWorldEntity)
{
	RwReal CurHeight = pWorldEntity->GetWorldSectorHeight(NewPos);

	if(NewPos.y < CurHeight + 10.0f)
		NewPos.y = CurHeight + 10.0f;

	RwFrameTranslate(RwCameraGetFrame(m_pCamera), &NewPos, rwCOMBINEREPLACE);
}

void CNtlGameCamera::HandleEvents(RWS::CMsg &pMsg)
{
	
}

void CNtlGameCamera::UpdateControl(float fElapsed)
{
	// reset poly counts;
	// dGET_WORLD_PARAM()->WorldPolyCnt = 0;

	RwReal	CurSpeed	= m_fSpeed * fElapsed;

	// Critical exception could occur on Fields Block System
	CLAMP(CurSpeed, 0, dGET_WORLD_PARAM()->WorldSectorSize);

	if(dKEY(DIK_W))
		m_CameraFrontBackSpeed = CurSpeed;
	if(dKEY(DIK_S))
		m_CameraFrontBackSpeed = -CurSpeed;
	if(dKEY(DIK_A))
		m_CameraStrafeSpeed = CurSpeed;
	if(dKEY(DIK_D))
		m_CameraStrafeSpeed = -CurSpeed;
	if(dKEY(DIK_Q))
		m_CameraUpDownSpeed = CurSpeed;				
	if(dKEY(DIK_E))
		m_CameraUpDownSpeed = -CurSpeed;				

	if(m_iCameraType == CNtlGameCamera::CAMERA_LOOK_AT)
	{
		RwFrame *pCameraFrame	= RwCameraGetFrame(m_pCamera);
		RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
		RwV3d *pRight			= RwMatrixGetRight(pM);
		RwV3d *pUp				= RwMatrixGetUp(pM);
		RwV3d pos, invPos;

		// Translate the camera back to the rotation origin. 
		pos = *RwMatrixGetPos(pM);

		float deltaXAngle = (m_fPrefXAngle - m_fXAngle);
		float deltaYAngle = (m_fPrefYAngle - m_fYAngle);

		if(m_fXAngle > m_fMaxXAngle) m_fXAngle = m_fMaxXAngle;
		if(m_fXAngle < m_fMinXAngle) m_fXAngle = m_fMinXAngle;

		//RwReal CurHeight = pWorldEntity->GetWorldSectorHeight(pos);

		if(dOKEY(DIK_DIVIDE))
		{
			//CNtlMath::MathRwV3dAssign(RwMatrixGetAt(pM), RwMatrixGetAt(pM)->x * 0.001f, -1.0f, RwMatrixGetAt(pM)->z * 0.001f);
			//RwV3dNormalize(RwMatrixGetAt(pM), RwMatrixGetAt(pM));

			CNtlMath::MathRwV3dAssign(RwMatrixGetAt(pM), 0.0f, -1.0f, 0.0f);
			
			RwV3dCrossProduct(RwMatrixGetUp(pM), RwMatrixGetAt(pM), RwMatrixGetRight(pM));
			RwV3dNormalize(RwMatrixGetUp(pM), RwMatrixGetUp(pM));			

			RwFrameUpdateObjects(pCameraFrame);
		}		
		else if(dGET_MOUSE()->GetWheelMovement() || m_CameraFrontBackSpeed != 0.0f || m_CameraStrafeSpeed != 0.0f || m_fXAngle != 0.0f || m_fYAngle != 0.0f || m_CameraUpDownSpeed != 0.0f)
		{
			// Translate the camera back to the rotation origin. 
			pos = *RwMatrixGetPos(pM);
			RwV3dNegate(&invPos, &pos);
			RwFrameTranslate(pCameraFrame, &invPos, rwCOMBINEPOSTCONCAT);

			// Rotate camera 
			RwV3d yAxis = {0.0f, 1.0f, 0.0f};
			RwFrameRotate(pCameraFrame, pRight, -m_fXAngle / 10.0f, rwCOMBINEPOSTCONCAT);
			RwFrameRotate(pCameraFrame, &yAxis, m_fYAngle / 10.0f, rwCOMBINEPOSTCONCAT);

			// Move along AT and translate back to orginal origin 
			RwV3dIncrementScaled(&pos, RwMatrixGetAt(pM), m_CameraFrontBackSpeed);
			RwV3dIncrementScaled(&pos, RwMatrixGetRight(pM), m_CameraStrafeSpeed);
			RwV3dIncrementScaled(&pos, &CNtlPLGlobal::m_vYAxisV3, m_CameraUpDownSpeed);

			//if(pos.y < CurHeight + 10.0f)
			//	pos.y = CurHeight + 10.0f;
			ClampTranslation(pos);
			RwFrameTranslate(pCameraFrame, &pos, rwCOMBINEPOSTCONCAT);

			m_fXAngle				= 0.0f;	
			m_fYAngle				= 0.0f;
			m_CameraFrontBackSpeed	= 0.0f;
			m_CameraStrafeSpeed		= 0.0f;
			m_CameraUpDownSpeed		= 0.0f;
		}
	}
	else if(m_iCameraType == CNtlGameCamera::CAMERA_FREE)
	{
		float XAngleDelta = (m_fPrefXAngle - m_fXAngle) * m_fSmoothXAngle;
		float YAngleDelta = (m_fPrefYAngle - m_fYAngle) * m_fSmoothYAngle;
		float ZTransDelta = (m_fPrefDistance - m_fDistance) * m_fSmoothDistance;

		CNtlCameraHelper::CameraRotation(m_pCamera, XAngleDelta, YAngleDelta);
		CNtlCameraHelper::CameraTranslateAt(m_pCamera, ZTransDelta);
	}

	GetSceneManager()->GetWorld()->SetPlayerPosition(*RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pCamera))));
}

void CNtlGameCamera::SetFov(float fFov)
{
	SetCalcFov(fFov);
}

void CNtlGameCamera::SetSpeed(float fSpeed)
{
	m_fSpeed								= fSpeed;
	dGET_WORLD_PARAM()->WorldCamSpeedPerSec	= fSpeed;
}

VOID CNtlGameCamera::Init()
{
	GetSceneManager()->SetSoundListener(NULL);
}