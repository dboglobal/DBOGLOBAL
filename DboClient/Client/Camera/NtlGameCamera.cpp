#include "precomp_dboclient.h"

#include "NtlGameCamera.h"
#include "NtlCameraHelper.h"
#include "NtlSobAvatar.h"
#include "NtlSLEventFunc.h"
#include "NtlPLSceneManager.h"

CNtlGameCamera::CNtlGameCamera(void) : 
	m_fDistance(10.f),
	m_fMinDistance(2.f),
	m_fMaxDistance(20.f),
	m_fPrefDistance(10.f),
	m_fSmoothDistance(0.025f),
	
	m_fXAngle(-10.0f),
    m_fMinXAngle(-89.0f),
	m_fMaxXAngle(89.0f),
    m_fPrefXAngle(-10.0f),
	m_fSmoothXAngle(0.05f),

	m_fYAngle(0.f),
	m_fMinYAngle(-180.f),
	m_fMaxYAngle(180.f),
	m_fPrefYAngle(0.f),
	m_fSmoothYAngle(0.05f),
	
	m_pTarget(NULL),
	m_fLookAtHeight(1.4f),
	m_bCameraRotateFlag(FALSE),
	m_bTargetMoveFlag(FALSE)
{
    m_pCachedMatrix = RwMatrixCreate();
	m_vLookAt.x = 0.f;
	m_vLookAt.y = m_fLookAtHeight;
	m_vLookAt.z = 0.f;

	m_iCameraType = ECameraType::CAMERA_LOOK_AT;
	m_handleMouseWheel = CInputHandler::GetInstance()->LinkMouseWheel(this, &CNtlGameCamera::MouseWheelHandler);
	m_handleMouseMove  = CInputHandler::GetInstance()->LinkMouseMove(this, &CNtlGameCamera::MouseMoveHandler);
	m_handleMouseDown  = CInputHandler::GetInstance()->LinkMouseDown(this, &CNtlGameCamera::MouseDownHandler);
	m_handleKeyDown    = CInputHandler::GetInstance()->LinkKeyDown(this, &CNtlGameCamera::KeyboardDownHandler);
	m_bMouseMoveValidFlag = FALSE;

	RegisterMsg (g_EventAvatarMoveStart, "g_EventAvatarMoveStartStr", "SNtlEventAvatarMove");
	RegisterMsg (g_EventAvatarMoveEnd,   "g_EventAvatarMoveEndStr",   "SNtlEventAvatarMove");

	RegisterMsg (g_EventCameraMoveStart, "g_EventCameraMoveStartStr", "SNtlCameraMove");
	RegisterMsg (g_EventCameraMoveEnd,   "g_EventCameraMoveEndStr",   "SNtlCameraMove");
	
	LinkMsg(g_EventAvatarMoveStart, 0);
	LinkMsg(g_EventAvatarMoveEnd, 0);
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
	
	UnLinkMsg(g_EventAvatarMoveStart);
	UnLinkMsg(g_EventAvatarMoveEnd);
	
	UnRegisterMsg (g_EventAvatarMoveEnd);
	UnRegisterMsg (g_EventAvatarMoveStart);

	UnRegisterMsg (g_EventCameraMoveStart);
	UnRegisterMsg (g_EventCameraMoveEnd);

}

void CNtlGameCamera::HandleMouse(RwReal fAngleX, RwReal fAngleY)
{
	m_fPrefYAngle = m_fYAngle + fAngleY;
	m_fPrefXAngle = m_fXAngle + fAngleX;
}


void CNtlGameCamera::HandleMouse(RwReal fDeltaZ)
{
	m_fPrefDistance = m_fDistance + fDeltaZ;
}

int	CNtlGameCamera::MouseWheelHandler(unsigned int uiMouseData)
{
	SMouseData *pData = (SMouseData *)uiMouseData;
	HandleMouse(-pData->zDelta * 0.05f);
	
	return TRUE;
}

int	CNtlGameCamera::MouseMoveHandler(unsigned int uiMouseData)
{
    SMouseData *pData = (SMouseData *)uiMouseData;
	if( !(pData->uiFlags & MK_LBUTTON) )
		return 1;
	
	if(m_bMouseMoveValidFlag)
	{
		RwV2d delta;
		delta.x = (m_OldMouseMove.x - pData->iPosX);
		delta.y = (m_OldMouseMove.y - pData->iPosY);
		HandleMouse(delta.y, delta.x);
		
		m_OldMouseMove.x = pData->iPosX;
		m_OldMouseMove.y = pData->iPosY;
	}
	else
	{
		m_bMouseMoveValidFlag = TRUE;
	}

	return TRUE;
}

int CNtlGameCamera::MouseDownHandler(unsigned int uiMouseData)
{
	SMouseData *pData = (SMouseData *)uiMouseData;
	if( pData->uiFlags & MK_LBUTTON)
	{
		m_OldMouseMove.x = pData->iPosX;
		m_OldMouseMove.y = pData->iPosY;
	}

	return TRUE;
}

int CNtlGameCamera::KeyboardDownHandler(unsigned int uiKeyData)
{
	return TRUE;
}

void CNtlGameCamera::Update(RwReal fElapsed)
{
	/*	
	if(m_iCameraType == ECameraType::CAMERA_LOOK_AT)
	{
		float deltaDistance = (m_fPrefDistance - m_fDistance) * m_fSmoothDistance;
		float deltaXAngle   = (m_fPrefXAngle - m_fXAngle) * m_fSmoothXAngle;
		float deltaYAngle   = (m_fPrefYAngle - m_fYAngle) * m_fSmoothYAngle;
		
		m_fXAngle += deltaXAngle;
		if (m_fXAngle > m_fMaxXAngle) m_fXAngle = m_fMaxXAngle;
		if (m_fXAngle < m_fMinXAngle) m_fXAngle = m_fMinXAngle;

		m_fYAngle += deltaYAngle;
	    
		m_fDistance += deltaDistance;
		if (m_fDistance > m_fMaxDistance) m_fDistance = m_fMaxDistance;
		if (m_fDistance < m_fMinDistance) m_fDistance = m_fMinDistance;
		
		RwV3d x_Axis = { 1.f, 0.f, 0.f};
		RwV3d y_Axis = { 0.f, 1.f, 0.f};
		
		RwMatrixSetIdentity(m_pCachedMatrix);
		
		RwV3d camPos;
		camPos.x = 0.f;
		camPos.y = 0.f;
		camPos.z = m_fDistance;
		RwMatrixTranslate (m_pCachedMatrix, &camPos, rwCOMBINEREPLACE);			    // Position
		RwMatrixRotate (m_pCachedMatrix, &x_Axis, m_fXAngle, rwCOMBINEPOSTCONCAT);	// Pitch
		RwMatrixRotate (m_pCachedMatrix, &y_Axis, m_fYAngle, rwCOMBINEPOSTCONCAT);	// Heading
		
		RwMatrixUpdate(m_pCachedMatrix);
		camPos.x = m_pCachedMatrix->pos.x + m_vLookAt.x;
		camPos.y = m_pCachedMatrix->pos.y + m_vLookAt.y;
		camPos.z = m_pCachedMatrix->pos.z + m_vLookAt.z;
		
		RwV3d eyeVector = { 0.f, -1.f, 0.f };
		CNtlCamera::LookAt(m_pRwCamera, &camPos, &m_vLookAt, &eyeVector);  
		
		if(m_pTarget)
		{
			m_vLookAt    = m_pTarget->GetPosition();
			m_vLookAt.y += m_fLookAtHeight;
			
			if(m_bTargetMoveFlag)
			{
				m_fPrefYAngle = m_pTarget->GetAngleY()-180.f;
			}
		}
	}
	else if(m_iCameraType == ECameraType::CAMERA_FREE)
	{
		float XAngleDelta = (m_fPrefXAngle - m_fXAngle) * m_fSmoothXAngle;
		float YAngleDelta = (m_fPrefYAngle - m_fYAngle) * m_fSmoothYAngle;
		float ZTransDelta = (m_fPrefDistance - m_fDistance) * m_fSmoothDistance;

		CNtlCameraHelper::CameraRotation(m_pRwCamera, XAngleDelta, YAngleDelta);
		CNtlCameraHelper::CameraTranslateAt(m_pRwCamera, ZTransDelta);
	}
	*/
}

void CNtlGameCamera::HandleEvents(RWS::CMsg &pMsg)
{
	
	if(pMsg.Id == g_EventAvatarMoveStart)
	{
		m_bTargetMoveFlag = TRUE;

		m_fOldSmoothYAngle = m_fSmoothYAngle;
		m_fSmoothYAngle = m_fOldSmoothYAngle / 5.f;
	}
	else if(pMsg.Id == g_EventAvatarMoveEnd)
	{	
		m_bTargetMoveFlag = FALSE;

		//현재 이동된 Angle에 고정을 한다.
		m_fPrefYAngle = m_fYAngle;
		m_fSmoothYAngle = m_fOldSmoothYAngle;
	}

	/*
	else if(pMsg.Id == g_EventAvatarRotateStart)
	{
		SNtlEventAvatarRotate* pData = (SNtlEventAvatarRotate *)pMsg.pData;

		m_bTargetCharacterRotate = pData->bRotate;
	}
	else if(pMsg.Id == g_EventAvatarRotateEnd)
	{
		SNtlEventAvatarRotate* pData = (SNtlEventAvatarRotate *)pMsg.pData;
		m_bTargetCharacterRotate = pData->bRotate;
	}
	*/
	
}