#include "stdafx.h"
//#include "NtlSharedCommon.h"
#include "NtlPLApi.h"
#include "ToolCamera.h"
#include "ModelToolApplication.h"

CNtlGameCamera::CNtlGameCamera(void) : 
	m_fDistance(3.f),
	m_fMinDistance(0.f),
	m_fMaxDistance(250.f),
	m_fPrefDistance(3.f),
	m_fSmoothDistance(0.1f),
    m_fRatioDistance(2.0f),
	
	m_fXAngle(-40.0f),
    m_fMinXAngle(-89.0f),
	m_fMaxXAngle(89.0f),
    m_fPrefXAngle(-40.0f),
	m_fSmoothXAngle(0.2f),

	m_fYAngle(0.f),
	m_fMinYAngle(-180.f),
	m_fMaxYAngle(180.f),
	m_fPrefYAngle(0.f),
	m_fSmoothYAngle(0.2f),
	
	m_pTarget(NULL),
	m_fLookAtHeight(1.f),
    m_bFreeCamera(FALSE)
{
    m_pCachedMatrix = RwMatrixCreate();
	m_vLookAt.x = 0.f;
	m_vLookAt.y = m_fLookAtHeight;
	m_vLookAt.z = 0.f;

    m_vLookAtDelta = ZeroAxis;        
    m_vCamOffset = m_vLookAtOffset = ZeroAxis;
	
	m_handleMouseWheel = CInputHandler::GetInstance()->LinkMouseWheel(this, &CNtlGameCamera::MouseWheelHandler);
	m_handleMouseMove  = CInputHandler::GetInstance()->LinkMouseMove(this, &CNtlGameCamera::MouseMoveHandler);
	m_handleMouseDown  = CInputHandler::GetInstance()->LinkMouseDown(this, &CNtlGameCamera::MouseDownHandler);
	m_handleMouseUp    = CInputHandler::GetInstance()->LinkMouseUp(this, &CNtlGameCamera::MouseUpHandler);
	m_handleKeyDown    = CInputHandler::GetInstance()->LinkKeyDown(this, &CNtlGameCamera::KeyboardDownHandler);
	m_bMouseMoveValidFlag = FALSE;
    m_bMouseMoveMButtonFlag = FALSE;
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
	CInputHandler::GetInstance()->UnLinkMouseUp(m_handleMouseUp);

	CInputHandler::GetInstance()->UnLinkKeyDown(m_handleKeyDown);
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
    if(pData->zDelta > 0)
    {
        HandleMouse(-m_fSmoothDistance * m_fRatioDistance);
    }
    else
    {
        HandleMouse(m_fSmoothDistance * m_fRatioDistance);
    }
	
	
	return TRUE;
}

int	CNtlGameCamera::MouseMoveHandler(unsigned int uiMouseData)
{
    SMouseData *pData = (SMouseData *)uiMouseData;
	//if( !(pData->uiFlags & MK_LBUTTON) )
	//	return 1;
	
	if(m_bMouseMoveValidFlag)
	{
		RwV2d delta;
		delta.x = (m_OldMouseMove.x - pData->iPosX);
		delta.y = (m_OldMouseMove.y - pData->iPosY);
		HandleMouse(delta.y, delta.x);
		
		m_OldMouseMove.x = (RwReal)pData->iPosX;
		m_OldMouseMove.y = (RwReal)pData->iPosY;
	}

    if(m_bMouseMoveMButtonFlag)
    {
        RwV2d delta;
        delta.x = (m_OldMouseMove.x - pData->iPosX);
        delta.y = (m_OldMouseMove.y - pData->iPosY);

        if(delta.x < 0)
        {
            OnPanning(MOVE_RIGHT);
        }
        else if(delta.x > 0)
        {
            OnPanning(MOVE_LEFT);
        }

        if(delta.y > 0)
        {
            OnPanning(MOVE_UP);
        }
        else if(delta.y < 0)
        {
            OnPanning(MOVE_DOWN);
        }

        m_OldMouseMove.x = (RwReal)pData->iPosX;
        m_OldMouseMove.y = (RwReal)pData->iPosY;
    }

	return TRUE;
}

int CNtlGameCamera::MouseDownHandler(unsigned int uiMouseData)
{
	SMouseData *pData = (SMouseData *)uiMouseData;
	if( pData->uiFlags & MK_LBUTTON)
	{
		m_OldMouseMove.x = (RwReal)pData->iPosX;
		m_OldMouseMove.y = (RwReal)pData->iPosY;
		m_bMouseMoveValidFlag = TRUE;
	}
    else if(pData->uiFlags & MK_MBUTTON)
    {
        m_OldMouseMove.x = (RwReal)pData->iPosX;
        m_OldMouseMove.y = (RwReal)pData->iPosY;
        m_bMouseMoveMButtonFlag = TRUE;
    }

	return TRUE;
}

int CNtlGameCamera::MouseUpHandler(unsigned int uiMouseData)
{
	m_bMouseMoveValidFlag = FALSE;
    m_bMouseMoveMButtonFlag = FALSE;
	
	return TRUE;
}

int CNtlGameCamera::KeyboardDownHandler(unsigned int uiKeyData)
{
    SKeyData* pData = (SKeyData*)uiKeyData;

    switch(pData->uiChar)
    {
    case 'W':
        if(m_bFreeCamera)
        {
            OnPanning(MOVE_FORWARD);
        }
        else
        {
            HandleMouse(-m_fSmoothDistance * m_fRatioDistance);
        }        
        break;
    case 'S':
        if(m_bFreeCamera)
        {
            OnPanning(MOVE_BACKWARD);
        }
        else
        {
            HandleMouse(m_fSmoothDistance * m_fRatioDistance);
        }        
        break;
    case 'A':        
        OnPanning(MOVE_LEFT);
        break;
    case 'D':
        OnPanning(MOVE_RIGHT);
        break;
    case 'Q':
        OnPanning(MOVE_UP);
        break;
    case 'E':
        OnPanning(MOVE_DOWN);
        break;
    }
	return TRUE;
}

void CNtlGameCamera::Update(RwReal fElapsed)
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
	camPos.y = -0.0f;
	camPos.z = m_fDistance;
	RwMatrixTranslate (m_pCachedMatrix, &camPos, rwCOMBINEREPLACE);			    // Position
	RwMatrixRotate (m_pCachedMatrix, &x_Axis, m_fXAngle, rwCOMBINEPOSTCONCAT);	// Pitch
	RwMatrixRotate (m_pCachedMatrix, &y_Axis, m_fYAngle, rwCOMBINEPOSTCONCAT);	// Heading
	
	RwMatrixUpdate(m_pCachedMatrix);
    m_vLookAt = m_vLookAt + m_vLookAtDelta;    	
    camPos = m_pCachedMatrix->pos + m_vLookAt;
    m_vLookAtDelta.x = 0.0f;
    m_vLookAtDelta.y = 0.0f;
    m_vLookAtDelta.z = 0.0f;
	
    // 카메라 셰이크용 오프셋
    camPos += m_vCamOffset;
    RwV3d vLookAt = m_vLookAt + m_vLookAtOffset;
	RwV3d eyeVector = { 0.f, -1.f, 0.f };    
    
	API_PL_CameraLookAt(m_pCamera, &camPos, &vLookAt, &eyeVector);
	
	if(m_pTarget)
	{
		m_vLookAt    = m_pTarget->GetPosition();        
		m_vLookAt.y += m_fLookAtHeight;        
	}
}

void CNtlGameCamera::SetLeftView()
{
    m_fPrefXAngle = 0.0f;
    m_fPrefYAngle = 90.0f;
}

void CNtlGameCamera::SetRightView()
{
    m_fPrefXAngle = 0.0f;
    m_fPrefYAngle = -90.0f;    
}
    
void CNtlGameCamera::SetFrontView()
{
    m_fPrefXAngle = 0.0f;
    m_fPrefYAngle = 0.0f;
}

void CNtlGameCamera::SetRearView()
{
    m_fPrefXAngle = 0.0f;
    m_fPrefYAngle = 180.0f;
}

void CNtlGameCamera::SetTopView()
{
    m_fPrefXAngle = -90.0f;
    m_fPrefYAngle = 0.0f;
}

void CNtlGameCamera::SetBottomView()
{
    m_fPrefXAngle = 90.0f;
    m_fPrefYAngle = 0.0f;
}

void CNtlGameCamera::OnPanning(ECameraMove eMove)
{
    RwFrame* pCameraFrame = RwCameraGetFrame(m_pCamera);
    RwMatrix* pCameraMatrix = RwFrameGetMatrix(pCameraFrame);
    RwV3d* rightVec = RwMatrixGetRight(pCameraMatrix);
    RwV3d* upVec = RwMatrixGetUp(pCameraMatrix);
    RwV3d* lookVec = RwMatrixGetAt(pCameraMatrix);

    RwReal fSmooth = m_fSmoothDistance * 0.5f;

    switch(eMove)
    {
    case MOVE_LEFT:
        m_vLookAtDelta += (*rightVec * fSmooth * m_fRatioDistance);
        break;
    case MOVE_RIGHT:
        m_vLookAtDelta -= (*rightVec * fSmooth * m_fRatioDistance);
        break;
    case MOVE_UP:
        m_vLookAtDelta += (*upVec * fSmooth * m_fRatioDistance);
        break;
    case MOVE_DOWN:
        m_vLookAtDelta -= (*upVec * fSmooth * m_fRatioDistance);
        break;
    case MOVE_FORWARD:
        m_vLookAtDelta += (*lookVec * fSmooth * m_fRatioDistance);
        break;
    case MOVE_BACKWARD:
        m_vLookAtDelta -= (*lookVec * fSmooth * m_fRatioDistance);
        break;
    }    
}

void CNtlGameCamera::SetDistance( RwReal fDistance ) 
{
    m_fPrefDistance = fDistance;
    m_vLookAtDelta = ZeroAxis;    
    m_vLookAt.x = 0.0f;
    m_vLookAt.y = 1.0f;
    m_vLookAt.z = 0.0f;
}

void CNtlGameCamera::SetEnableFreeCamera( RwBool bEnable ) 
{
    m_bFreeCamera = bEnable;

    if(!bEnable)
    {
        m_vLookAt = ZeroAxis;
        m_vLookAtDelta = ZeroAxis;
    }
}
void CNtlGameCamera::SetCameraOffset( const RwV3d& vOffset ) 
{
    if(CModelToolApplication::GetInstance()->GetCameraShakeDisable())
        return;

    m_vCamOffset = vOffset;
}

void CNtlGameCamera::SetCameraLookAtOffset( const RwV3d& vOffset ) 
{
    if(CModelToolApplication::GetInstance()->GetCameraShakeDisable())
        return;

    m_vLookAtOffset = vOffset;
}