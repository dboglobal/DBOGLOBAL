#include "stdafx.h"
#include "NtlCameraTop.h"
#include "NtlCameraHelper.h"
#include "NtlSLEventFunc.h"
#include "NtlTimer.h"
#include "ntlworldcommon.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldDI.h"
#include "NtlWeControlUi.h"
#include "NtlWe.h"
#include "NtlWorldFieldManager.h"

#include <d3d9.h>


CNtlCameraTop::CNtlCameraTop(void) : 
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
	m_fSpeed(10.f)
{
	m_vLookAt.x = 0.f;
	m_vLookAt.y = 0.0f;
	m_vLookAt.z = 0.f;

	m_OldMouseMove.x = 0;
	m_OldMouseMove.y = 0;

	m_iCameraType = CNtlCameraTop::CAMERA_LOOK_AT;
	m_bMouseMoveValidFlag = false;

}	

CNtlCameraTop::~CNtlCameraTop(void)
{
}

VOID CNtlCameraTop::Init()
{
	RwFrame *pCameraFrame	= RwCameraGetFrame(m_pCamera);
	RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
	RwV3d *pRight			= RwMatrixGetRight(pM);
	RwV3d *pUp				= RwMatrixGetUp(pM);
	RwV3d pos, invPos;

	// Translate the camera back to the rotation origin. 
	pos = *RwMatrixGetPos(pM);
	RwV3dNegate(&invPos, &pos);
	RwFrameTranslate(pCameraFrame, &invPos, rwCOMBINEPOSTCONCAT);

	RwFrameSetIdentity(pCameraFrame);
	pRight = RwMatrixGetRight(RwFrameGetMatrix(pCameraFrame));
	RwFrameRotate(pCameraFrame, pRight, 90.0f, rwCOMBINEREPLACE);

	RwV3d vZUnit = { 0.0f, 0.0f, 1.0f };

	// Move along AT and translate back to orginal origin 
	RwV3dIncrementScaled(&pos, &vZUnit, 0.0f);
	RwV3dIncrementScaled(&pos, RwMatrixGetRight(pM), 0.0f);

	pos.y = dGETMGR()->GetWorldSectorHeight(pos);

	RwV3dIncrementScaled(&pos, RwMatrixGetAt(pM), -300.0f);

	ClampTranslation(pos, FALSE);
	RwFrameTranslate(pCameraFrame, &pos, rwCOMBINEPOSTCONCAT);

	GetSceneManager()->SetSoundListener(NULL);
}

void CNtlCameraTop::HandleMouse(RwReal fAngleX, RwReal fAngleY)
{
	m_fYAngle = fAngleY;
	m_fXAngle = fAngleX;
	
	if (m_fPrefXAngle > m_fMaxXAngle) m_fPrefXAngle = m_fMaxXAngle;
	if (m_fPrefXAngle < m_fMinXAngle) m_fPrefXAngle = m_fMinXAngle;
}


void CNtlCameraTop::HandleMouse(RwReal fDeltaZ)
{
	m_fPrefDistance = m_fPrefDistance + fDeltaZ;
	if (m_fPrefDistance > m_fMaxDistance) m_fPrefDistance = m_fMaxDistance;
	if (m_fPrefDistance < m_fMinDistance) m_fPrefDistance = m_fMinDistance;
}

void CNtlCameraTop::ClampTranslation(RwV3d& Pos, RwBool IsCamMoving)
{
	RwReal Max;
	RwReal Min;

	if(IsCamMoving)
	{
// 		Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - (dGET_WORLD_PARAM()->WorldFieldSize * 2) - 1);
// 		Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + (dGET_WORLD_PARAM()->WorldFieldSize * 2) + 1);
		Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - dNTL_WORLD_EXC_AOI_SIZE - 1);
		Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + dNTL_WORLD_EXC_AOI_SIZE + 1);
	}
	
	else
	{
		Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax);
		Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin);
	}

	CLAMP(Pos.x, Min, Max);
	CLAMP(Pos.y, (CNtlPLGlobal::m_RwCamera->farPlane * 0.5f), -(CNtlPLGlobal::m_RwCamera->farPlane * 0.5f));
	CLAMP(Pos.z, Min, Max);
}

void CNtlCameraTop::Translation(RwV3d& NewPos, CNtlPLWorldEntity *pWorldEntity)
{
	RwReal CurHeight = pWorldEntity->GetWorldSectorHeight(NewPos);

	if(NewPos.y < CurHeight + 10.0f)
		NewPos.y = CurHeight + 10.0f;

	RwFrameTranslate(RwCameraGetFrame(m_pCamera), &NewPos, rwCOMBINEREPLACE);
}

void CNtlCameraTop::Update()
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

	// Translate the camera back to the rotation origin. 
	pos = *RwMatrixGetPos(pM);
	RwV3dNegate(&invPos, &pos);
	RwFrameTranslate(pCameraFrame, &invPos, rwCOMBINEPOSTCONCAT);

	RwFrameSetIdentity(pCameraFrame);
	pRight = RwMatrixGetRight(RwFrameGetMatrix(pCameraFrame));
	RwFrameRotate(pCameraFrame, pRight, 90.0f, rwCOMBINEREPLACE);

	RwV3d vZUnit = { 0.0f, 0.0f, 1.0f };

	// Move along AT and translate back to orginal origin 
	RwV3dIncrementScaled(&pos, &vZUnit, m_CameraFrontBackSpeed);
	RwV3dIncrementScaled(&pos, RwMatrixGetRight(pM), m_CameraStrafeSpeed);
	RwV3dIncrementScaled(&pos, RwMatrixGetAt(pM), (float(dGET_MOUSE()->GetWheelMovement()) / 100.0f));

	if(CNtlPLGlobal::m_IsMinimapRendering)
	{
		ClampTranslation(pos, FALSE);
	}
	else
	{
		ClampTranslation(pos, TRUE);
	}

	pos.y = 512.0f;
	RwFrameTranslate(pCameraFrame, &pos, rwCOMBINEPOSTCONCAT);


	m_fXAngle = 0.0f;
	m_fYAngle = 0.0f;
	m_CameraFrontBackSpeed = 0.0f;
	m_CameraStrafeSpeed = 0.0f;
}

void CNtlCameraTop::UpdateControl(float fElapsed)
{
	// reset poly counts;
	// dGET_WORLD_PARAM()->WorldPolyCnt = 0;

	RwReal CurSpeed = m_fSpeed * fElapsed;

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

	if(m_iCameraType == CNtlCameraTop::CAMERA_LOOK_AT)
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

		if(dGET_MOUSE()->GetWheelMovement() || m_CameraFrontBackSpeed != 0.0f || m_CameraStrafeSpeed != 0.0f || m_fXAngle != 0.0f || m_fYAngle != 0.0f || CNtlPLGlobal::m_IsMinimapRendering)
		{
			// Translate the camera back to the rotation origin. 
			pos = *RwMatrixGetPos(pM);
			RwV3dNegate(&invPos, &pos);
			RwFrameTranslate(pCameraFrame, &invPos, rwCOMBINEPOSTCONCAT);

			RwFrameSetIdentity(pCameraFrame);
			pRight = RwMatrixGetRight(RwFrameGetMatrix(pCameraFrame));
			RwFrameRotate(pCameraFrame, pRight, 90.0f, rwCOMBINEREPLACE);

			RwV3d vZUnit = { 0.0f, 0.0f, 1.0f };

			// Move along AT and translate back to orginal origin 
			RwV3dIncrementScaled(&pos, &vZUnit, m_CameraFrontBackSpeed);
			RwV3dIncrementScaled(&pos, RwMatrixGetRight(pM), m_CameraStrafeSpeed);
			RwV3dIncrementScaled(&pos, RwMatrixGetAt(pM), (float(dGET_MOUSE()->GetWheelMovement()) / 100.0f));

			if(CNtlPLGlobal::m_IsMinimapRendering)
			{
				ClampTranslation(pos, FALSE);
			}
			else
			{
				ClampTranslation(pos);
			}

			pos.y = 512.0f;
			RwFrameTranslate(pCameraFrame, &pos, rwCOMBINEPOSTCONCAT);
			

			m_fXAngle = 0.0f;
			m_fYAngle = 0.0f;
			m_CameraFrontBackSpeed = 0.0f;
			m_CameraStrafeSpeed = 0.0f;
		}
	}
	else if(m_iCameraType == CNtlCameraTop::CAMERA_FREE)
	{
		float XAngleDelta = (m_fPrefXAngle - m_fXAngle) * m_fSmoothXAngle;
		float YAngleDelta = (m_fPrefYAngle - m_fYAngle) * m_fSmoothYAngle;
		float ZTransDelta = (m_fPrefDistance - m_fDistance) * m_fSmoothDistance;

		CNtlCameraHelper::CameraRotation(m_pCamera, XAngleDelta, YAngleDelta);
		CNtlCameraHelper::CameraTranslateAt(m_pCamera, ZTransDelta);
	}

	GetSceneManager()->GetWorld()->SetPlayerPosition(*RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pCamera))));
}

void CNtlCameraTop::SetFov(float fFov)
{
	SetCalcFov(fFov);
}

void CNtlCameraTop::SetSpeed(float fSpeed)
{
	m_fSpeed								= fSpeed;
	dGET_WORLD_PARAM()->WorldCamSpeedPerSec	= fSpeed;
}

void CNtlCameraTop::SetZoomDist(float fZoom)
{
	dGET_MOUSE()->SetWheelMovement(static_cast<short>(fZoom));
}