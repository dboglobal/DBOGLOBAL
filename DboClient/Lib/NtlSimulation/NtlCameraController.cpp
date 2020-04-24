#include "precomp_ntlsimulation.h"
#include "NtlCameraController.h"

// shared
#include "NtlMovement.h"
#include "MobTable.h"
#include "GraphicDataTable.h"
#include "NtlXMLDoc.h"
#include "WorldTable.h"

// core
#include "NtlMath.h"

// framework
#include "NtlCamera.h"

// sound
#include "NtlSoundEventGenerator.h"

// presentation
#include "NtlPLEventGenerator.h"
#include "NtlPLVisualManager.h"
#include "NtlPLHelpers.h"
#include "NtlPLResourcePack.h"

// simulation
#include "NtlCameraManager.h"
#include "NtlSobActor.h"
#include "NtlSobManager.h"
#include "NtlSobProxy.h"
#include "NtlSLLogic.h"
#include "NtlDTNode.h"
#include "NtlSLApi.h"
#include "NtlBehaviorBase.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "DboTLDefine.h"
#include "NtlSobGroup.h"
#include "NtlSobMonsterAttr.h"
#include "NtlSlLuaGlue.h"
#include "TableContainer.h"
#include "NtlSobElapsedController.h"


RwMatrix CNtlCameraController::m_matTrans;		// camera trans matrix
RwReal   CNtlCameraController::m_fTargetHeightRatio = 0.7f;

CNtlCameraController::CNtlCameraController()
{
	m_bPause = FALSE;
	m_bFinish = FALSE;
	m_byFlags = 0;

	m_fHeightRatioBonus = 0.0f;
}

CNtlCameraController::~CNtlCameraController()
{
}

void CNtlCameraController::UpdateCameraTrans(RwV3d *pCamPos, const RwV3d *pLookAt,RwReal fDistOffset)
{
    RwReal fDist = GetNtlGameCameraManager()->GetDist();
    RwReal fPitch = GetNtlGameCameraManager()->GetPitch();
    RwReal fYaw = GetNtlGameCameraManager()->GetYaw();

    fDist += fDistOffset;

    RwMatrixSetIdentity(&m_matTrans);

    RwV3d vPos;
    CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, fDist); 
    RwMatrixTranslate (&m_matTrans, &vPos, rwCOMBINEREPLACE);									// Position
    RwMatrixRotate (&m_matTrans, &CNtlPLGlobal::m_vXAxisV3, fPitch, rwCOMBINEPOSTCONCAT);		// Pitch
    RwMatrixRotate (&m_matTrans, &CNtlPLGlobal::m_vYAxisV3, fYaw, rwCOMBINEPOSTCONCAT);			// Yaw
    RwMatrixUpdate(&m_matTrans);

    *pCamPos = *pLookAt - m_matTrans.pos;
}

RwReal CNtlCameraController::GetActorOffset(CNtlSobActor *pActor)
{
	if ( pActor )
	{
		// Height ratio does not reflect when swimming

		RwReal fOffset;
		RwV3d vPos = pActor->GetPosition();

		SWorldHeightStuff sHStuff;
		if ( Logic_IsSwimmingActor( pActor, &vPos, sHStuff ) )
		{
			fOffset = pActor->GetSobProxy()->GetPLEntityHeight();
		}
		else
		{
			fOffset = pActor->GetSobProxy()->GetPLEntityHeight() * m_fTargetHeightRatio;
		}

		if ( fOffset < 1.1f )
		{
			fOffset = 1.1f;
		}

		fOffset += m_fHeightRatioBonus;

		return fOffset;
	}
	else
	{
		return 0.f;
	}
}

void CNtlCameraController::SetVisibleAllObject( RwBool bVisible )
{
	GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_NPC_SHOP, SLCLASS_PLAYER, bVisible);
	GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_NPC_SHOP, SLCLASS_AVATAR, bVisible);
	GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_NPC_SHOP, SLCLASS_PET, bVisible);
	GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_NPC_SHOP, SLCLASS_VEHICLE, bVisible);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlCameraNormalController::CNtlCameraNormalController()
{
    m_byUpdateFlags			= CAMERA_NORAML_NONE;
    m_bySmoothingFlag		= CAMERA_NORAML_SMOOTHING_NONE;
    m_fSmoothTime			= 0.0f;
    m_bSmoothLookAtEnable	= FALSE;
    m_fDesiredDist			= 0.0f;
    m_pActor				= NULL;
    SetType(CAMERA_CONTROL_NORMAL);
}

CNtlCameraNormalController::~CNtlCameraNormalController()
{
}

void CNtlCameraNormalController::EnableRotate(RwBool bEnable)
{
    if(bEnable)
        m_byUpdateFlags |= CAMERA_NORAML_ROTATE;
    else
        m_byUpdateFlags &= ~CAMERA_NORAML_ROTATE;
}

void CNtlCameraNormalController::EnableZoomInOut(RwBool bEnable)
{
    if(bEnable)
        m_byUpdateFlags |= CAMERA_NORAML_ZOOM_INOUT;
    else
        m_byUpdateFlags &= ~CAMERA_NORAML_ZOOM_INOUT;
}

void CNtlCameraNormalController::EnableSmoothingLerp(RwBool bEnable)
{
    if(bEnable)
        m_bySmoothingFlag = CAMERA_NORAML_SMOOTHING_LERP;
    else
        m_bySmoothingFlag = CAMERA_NORAML_SMOOTHING_NONE;
}

RwV3d CNtlCameraNormalController::GetLookAt(CNtlSobActor *pActor)
{
    RwV3d vLookAt;
    if(m_pActor)
    {
        vLookAt = m_pActor->GetPosition();
        CNtlMath::MathRwV3dAssign(&vLookAt, vLookAt.x, vLookAt.y + GetActorOffset(pActor), vLookAt.z);
    }
    else
        CNtlMath::MathRwV3dAssign(&vLookAt, 0.0f, 0.0f, 0.0f);

    return vLookAt;
}

RwBool CNtlCameraNormalController::UpdateSmoothLookAt(RwV3d& vLookAt, RwReal fElapsed, RwReal fSpeed)
{
    RwV3d vPos = m_pActor->GetPosition();
    RwV3d vSub;
    RwV3dSubMacro(&vSub, &vPos, &m_vPrevActorPos);
    RwReal fDeltaDist = RwV3dLength(&vSub);
    RwReal fSpeedDist = Logic_GetFrontRunSpeed(m_pActor) * fElapsed;
    RwReal fMulDist = fDeltaDist/fSpeedDist;

    vLookAt = GetLookAt(m_pActor);

    RwV3d vDir;
    RwV3dSubMacro(&vDir, &vLookAt, &m_vSmoothLookAt);

    RwReal fLen = RwV3dLength(&vDir);

    RwReal fAdjustElapsed = fElapsed*fSpeed;

    if(fAdjustElapsed < fLen)
    {
        if(fSpeed > 1.0f && fMulDist)
            fAdjustElapsed = fElapsed*fSpeed*fMulDist;

        RwV3dNormalize(&vDir, &vDir);
        m_vSmoothLookAt.x += vDir.x*fAdjustElapsed;
        m_vSmoothLookAt.y += vDir.y*fAdjustElapsed;
        m_vSmoothLookAt.z += vDir.z*fAdjustElapsed;

        RwV3dAssignMacro(&vLookAt, &m_vSmoothLookAt);

        return FALSE;
    }

    return TRUE;
}

void CNtlCameraNormalController::UpdateZoomInOut(RwReal fElapsed)
{
    //위치 갱신
    RwReal fCurrDist = GetNtlGameCameraManager()->GetDist();
    float fDeltaDist = m_fDeltaDist * fElapsed * 4.0f;
    if( fabs(m_fDesiredDist - fCurrDist) <= fabs(fDeltaDist))
    {
        fCurrDist = m_fDesiredDist;
        m_fDeltaDist = 0.f;
        EnableZoomInOut(FALSE);
    }
    else
        fCurrDist += fDeltaDist;

    GetNtlGameCameraManager()->SetDist(fCurrDist);
}

void CNtlCameraNormalController::UpdateSmoothSmoothNone(RwV3d& vCamPos, RwV3d& vLookAt, RwReal& fDistOffset, RwReal fElapsed)
{
    vLookAt = GetLookAt(m_pActor);		
}

void CNtlCameraNormalController::UpdateSmoothLerp(RwV3d& vCamPos, RwV3d& vLookAt, RwReal& fDistOffset, RwReal fElapsed)
{
    RwV3d vPos;
    if(m_pActor)
        vPos = m_pActor->GetPosition();
    else
        CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);

    RwV3d vDir = vPos - m_vLerpPos;    
    RwReal fCurrLen = RwV3dLength(&vDir);
    RwV3dNormalize(&vDir, &vDir);        
    RwV3d vDelta = vDir * m_fLerpSpeed * fElapsed;

    if(fCurrLen > NTL_EPSILON)
    {
        // delta time 동안 이동 거리를 구한다.
        if(fCurrLen - RwV3dLength(&vDelta) <= NTL_EPSILON)           
        {
            EnableSmoothingLerp(FALSE);
        }
        else if(fCurrLen > GetNtlGameCameraManager()->GetMaxDist() * 1.2f)      /// 갑자기 범위가 멀리 벌어지는 경우를 대비한 방어코드 (텔레포트 할때 버그가 생기는 경우가 있다) by agebreak 2009.05.06)
        {
            m_vLerpPos = vPos + (vDir * -1.0f * GetNtlGameCameraManager()->GetMaxDist());
            EnableSmoothingLerp(FALSE);
        }
        else
        {
            m_vLerpPos += vDelta;
        }
    }
    else
    {
        EnableSmoothingLerp(FALSE);
    }

    CNtlMath::MathRwV3dAssign(&vLookAt, m_vLerpPos.x, m_vLerpPos.y + GetActorOffset(m_pActor), m_vLerpPos.z);
}

void CNtlCameraNormalController::UpdateSmoothFollowStart(RwV3d& vCamPos, RwV3d& vLookAt, RwReal& fDistOffset, RwReal fElapsed)
{
    vLookAt = GetLookAt(m_pActor);		

    RwReal fDist = GetNtlGameCameraManager()->GetDist();
    const RwV3d *pCamPos = GetNtlGameCameraManager()->GetCameraPos();

    RwV3d vSub;
    RwV3dSubMacro(&vSub, &m_vSmoothLookAt, pCamPos);
    RwReal fCurrLen = RwV3dLength(&vSub);

    if(fCurrLen >= fDist * 2.0f)
    {
        m_bySmoothingFlag		= CAMERA_NORAML_SMOOTHING_NONE;
        m_fSmoothTime			= 0.0f;
        m_bSmoothLookAtEnable	= FALSE;
    }
    else
    {
        m_fSmoothTime += fElapsed;
        m_fSmoothDist = m_fSmoothTime;
        if(m_fSmoothTime >= 0.2f)
        {
            m_fSmoothTime = 0.0f;		
            m_fSmoothDist = 0.1f;
            m_bySmoothingFlag = CAMERA_NORAML_SMOOTHING_FOLLOW_ADJUST;
        }

        RwReal fSpeed = Logic_GetFrontRunSpeed(m_pActor);
        fSpeed = fSpeed*0.4f;

        fDistOffset = m_fSmoothDist;

        UpdateSmoothLookAt(vLookAt, fElapsed, fSpeed);
    }
}

void CNtlCameraNormalController::UpdateSmoothFollowAdjust(RwV3d& vCamPos, RwV3d& vLookAt, RwReal& fDistOffset, RwReal fElapsed)
{
    RwReal fSpeed = Logic_GetFrontRunSpeed(m_pActor);
    fSpeed = fSpeed*1.2f;

    if(UpdateSmoothLookAt(vLookAt, fElapsed, fSpeed))
    {
        vLookAt = GetLookAt(m_pActor);
        m_bySmoothingFlag = CAMERA_NORAML_SMOOTHING_FOLLOW;
    }

    fDistOffset = m_fSmoothDist;
}

void CNtlCameraNormalController::UpdateSmoothSmoothFollow(RwV3d& vCamPos, RwV3d& vLookAt, RwReal& fDistOffset, RwReal fElapsed)
{
    m_bSmoothLookAtEnable = FALSE;
    fDistOffset = m_fSmoothDist;
    vLookAt = GetLookAt(m_pActor);
}

void CNtlCameraNormalController::UpdateSmoothSmoothStop(RwV3d& vCamPos, RwV3d& vLookAt, RwReal& fDistOffset, RwReal fElapsed)
{
    m_fSmoothTime += fElapsed;

    if(m_bSmoothLookAtEnable)
    {
        RwReal fSpeed = Logic_GetFrontRunSpeed(m_pActor);
        fSpeed = fSpeed*1.2f;

        if(UpdateSmoothLookAt(vLookAt, fElapsed, fSpeed))
        {
            vLookAt = GetLookAt(m_pActor);
            m_bSmoothLookAtEnable = FALSE;
        }
    }
    else
    {
        vLookAt = GetLookAt(m_pActor);
    }

    if(!m_bSmoothLookAtEnable && m_fSmoothTime >= 0.2f)
    {
        m_fSmoothDist = 0.0f;
        m_bySmoothingFlag = CAMERA_NORAML_SMOOTHING_NONE;
    }
    else
    {
        m_fSmoothDist = (0.1f - 0.1f*m_fSmoothTime/0.2f);
    }

    fDistOffset = m_fSmoothDist;
}

void CNtlCameraNormalController::Update(RwReal fElapsed)
{
    if(m_byUpdateFlags & CAMERA_NORAML_ZOOM_INOUT)
        UpdateZoomInOut(fElapsed);

    RwReal fDistOffset = 0.0f;
    RwV3d vLookAt, vCamPos;

    // 현재 camera position 과 actor position 사이 초당 10미터로 보간을 한다.
    if(m_bySmoothingFlag == CAMERA_NORAML_SMOOTHING_LERP)
        UpdateSmoothLerp(vCamPos, vLookAt, fDistOffset, fElapsed);
    else if(m_bySmoothingFlag == CAMERA_NORAML_SMOOTHING_FOLLOW_START)
        UpdateSmoothFollowStart(vCamPos, vLookAt, fDistOffset, fElapsed);
    else if(m_bySmoothingFlag == CAMERA_NORAML_SMOOTHING_FOLLOW_ADJUST)
        UpdateSmoothFollowAdjust(vCamPos, vLookAt, fDistOffset, fElapsed);
    else if(m_bySmoothingFlag == CAMERA_NORAML_SMOOTHING_FOLLOW)
        UpdateSmoothSmoothFollow(vCamPos, vLookAt, fDistOffset, fElapsed);
    else if(m_bySmoothingFlag == CAMERA_NORAML_SMOOTHING_STOP)
        UpdateSmoothSmoothStop(vCamPos, vLookAt, fDistOffset, fElapsed);

    if(m_bySmoothingFlag == CAMERA_NORAML_SMOOTHING_NONE)
        UpdateSmoothSmoothNone(vCamPos, vLookAt, fDistOffset, fElapsed);

    if(m_pActor)
        m_vPrevActorPos = m_pActor->GetPosition();

    UpdateCameraTrans(&vCamPos, &vLookAt, fDistOffset);
    GetNtlGameCameraManager()->SetLookAt(&vLookAt);
    GetNtlGameCameraManager()->SetCameraPos(&vCamPos);
}

void CNtlCameraNormalController::UpdateData(RwBool bUpdate /*= TRUE*/)
{
    if(!bUpdate)
    {
        EnableSmoothingLerp(FALSE);
        return;
    }

    RwV3d vPos = m_pActor->GetPosition();
    RwV3d vLookAt;
    CNtlMath::MathRwV3dAssign(&vLookAt, vPos.x, vPos.y + GetActorOffset(m_pActor), vPos.z);

    const RwV3d *pCamPos = GetNtlGameCameraManager()->GetCameraPos();

    RwV3d vUpdatePos, vOffset;
    UpdateCameraTrans(&vUpdatePos, &vLookAt);
    RwV3dSubMacro(&vOffset, pCamPos, &vUpdatePos);
    RwV3dAdd(&m_vLerpPos, &vOffset, &vPos);
    m_fLerpSpeed = RwV3dLength(&vOffset)*10.0f;
    RwReal fRunSpeed = Logic_GetFrontRunSpeed(m_pActor)*1.3f;
    if(m_fLerpSpeed < fRunSpeed)
        m_fLerpSpeed = fRunSpeed;

    EnableSmoothingLerp(TRUE);
}

void CNtlCameraNormalController::ZoomInOut(RwReal fDelta)
{
    RwReal fDist = GetNtlGameCameraManager()->GetDist();
    m_fDesiredDist = fDist + fDelta;
    m_fDeltaDist = fDelta;

    RwReal fMinDist = GetNtlGameCameraManager()->GetMinDist();
    RwReal fMaxDist = GetNtlGameCameraManager()->GetMaxDist();

    if (m_fDesiredDist < fMinDist) 
        m_fDesiredDist = fMinDist;
    if (m_fDesiredDist > fMaxDist) 
        m_fDesiredDist = fMaxDist;

    EnableZoomInOut(TRUE);
}

void CNtlCameraNormalController::MoveSmoothing(RwBool bEnable)
{
    if(bEnable)
    {
        m_vSmoothLookAt			= m_pActor->GetPosition();
        m_vSmoothLookAt.y		+= GetActorOffset(m_pActor);

        m_bySmoothingFlag		= CAMERA_NORAML_SMOOTHING_FOLLOW_START;
        m_fSmoothTime			= 0.0f;
        m_fSmoothDist			= 0.0f;
        m_bSmoothLookAtEnable	= TRUE;
    }
    else
    {
        if(m_bySmoothingFlag == CAMERA_NORAML_SMOOTHING_FOLLOW_START || m_bySmoothingFlag == CAMERA_NORAML_SMOOTHING_FOLLOW_ADJUST ||
            m_bySmoothingFlag == CAMERA_NORAML_SMOOTHING_FOLLOW)
        {
            m_bySmoothingFlag = CAMERA_NORAML_SMOOTHING_STOP;
            m_fSmoothTime  = 0.0f;
        }
        else
            m_bySmoothingFlag = CAMERA_NORAML_SMOOTHING_NONE;
    }
}

void CNtlCameraNormalController::ResetCamera(void)
{
	m_bySmoothingFlag = CAMERA_NORAML_SMOOTHING_NONE;

	RwReal fDistOffset = 0.0f;
	RwV3d vLookAt, vCamPos;

	UpdateSmoothSmoothNone( vCamPos, vLookAt, fDistOffset, 0.f );

	if ( m_pActor )
	{
		m_vPrevActorPos = m_pActor->GetPosition();
	}

	UpdateCameraTrans( &vCamPos, &vLookAt, fDistOffset );

	GetNtlGameCameraManager()->SetLookAt( &vLookAt );

	GetNtlGameCameraManager()->SetCameraPos( &vCamPos );
}

void CNtlCameraNormalController::SetActiveActor(const CNtlSobActor *pActor)
{
    m_pActor = const_cast<CNtlSobActor*>( pActor );
}

const CNtlSobActor* CNtlCameraNormalController::GetActiveActor(void)
{
    return m_pActor;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwReal CNtlCameraShakeController::m_fShakeFactor = CAMERA_SHAKE_FACTOR;
RwReal CNtlCameraShakeController::m_fShakeMaxHeightFactor = CAMERA_SHAKE_MAX_HEIGHT_FACTOR;

CNtlCameraShakeController::CNtlCameraShakeController()
{
    m_fShakeElapsedTime = 0.0f;
    SetType(CAMERA_CONTROL_SHAKE);
}

CNtlCameraShakeController::~CNtlCameraShakeController()
{
    
}

void CNtlCameraShakeController::Update(RwReal fElapsed)
{
    RwReal fDeltaShake = cos(m_fShakeElapsedTime * 110.f)*(exp(-m_fShakeElapsedTime * 110.f/m_fShakeFactor))*m_fShakeMaxHeightFactor;
    m_fShakeElapsedTime += fElapsed;

    RwV3d vOffset;
    CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, fDeltaShake, 0.0f);

    GetNtlGameCameraManager()->AddCameraOffset(&vOffset);
    GetNtlGameCameraManager()->AddCameraLookAtOffset(&vOffset);

    if( fabs(fDeltaShake) < NTL_EPSILON)
        Finish();
}

void CNtlCameraShakeController::SetShakeFactor(RwReal fWeightValue, RwReal fHeightValue)
{
    m_fShakeFactor			= fWeightValue;
    m_fShakeMaxHeightFactor = fHeightValue;
}


void CNtlCameraShakeController::GetShakeFactor(RwReal& fWeightValue, RwReal& fHeightValue)
{
    fWeightValue			= m_fShakeFactor;
    fHeightValue			= m_fShakeMaxHeightFactor;
}

void CNtlCameraShakeController::ResetShakeFactor(void)
{
    m_fShakeFactor			= CAMERA_SHAKE_FACTOR;
    m_fShakeMaxHeightFactor = CAMERA_SHAKE_MAX_HEIGHT_FACTOR;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CAMERA_DASH_START_WEIGHT_TIME		0.2f
#define CAMERA_DASH_LOOP_WEIGHT_SPEED		1.2f
#define CAMERA_DASH_LOOP_DIST_LIMIT			0.5f
#define CAMERA_DASH_END_TIME				0.5f
#define CAMERA_DASH_END_WEIGHT_SPEED		3.0f

CNtlCameraDashController::CNtlCameraDashController()
{
    m_pActor = NULL;
    m_fDashSpeed = NTL_DASH_SPEED;
    SetType(CAMERA_CONTROL_DASH);
    SetFlags(NTL_CAMERA_CONTROL_NORMAL_PAUSE);
}

CNtlCameraDashController::~CNtlCameraDashController()
{
}

void CNtlCameraDashController::ResetData(void)
{
    m_byCameraDashState = CAMERA_DASH_STATE_START;
    m_fTime = 0.0f;
    m_bArrival = FALSE;

    RwV3d vDelta;
    RwV3dSubMacro(&vDelta, &m_vDestPos, &m_vStartPos);
    RwReal fLen = RwV3dLength(&vDelta);
    RwReal fTotTime = fLen/m_fDashSpeed;
    m_fStartTime = fTotTime*CAMERA_DASH_START_WEIGHT_TIME;
}

void CNtlCameraDashController::CalcCameraTrans(RwReal fSpeed, RwReal fLimitLen, RwReal fElapsed)
{
    RwV3d vCamPos, vDir, vDelta;

    RwV3d vPos = m_pActor->GetPosition();
    RwV3dSubMacro(&vDir, &vPos, &m_vStartPos); 
    RwV3dAssignMacro(&vDelta, &vDir);
    RwReal fCurrLen = RwV3dLength(&vDelta);
    RwV3dNormalize(&vDir, &vDir);

    CNtlMath::MathRwV3dAssign(&vDelta, fSpeed*vDir.x, fSpeed*vDir.y, fSpeed*vDir.z);

    if(fCurrLen > fLimitLen)
    {
        // delta time 동안 이동 거리를 구한다.
        if(fCurrLen - RwV3dLength(&vDelta) <= fLimitLen)
        {
            CNtlMath::MathRwV3dAssign(&m_vStartPos, vPos.x-fLimitLen*vDir.x, vPos.y-fLimitLen*vDir.y, vPos.z-fLimitLen*vDir.z);
            if(!m_bArrival)
                m_bArrival = TRUE;
        }
        else
            CNtlMath::MathRwV3dAddMul(&m_vStartPos, &vDir, fSpeed);
    }
    else
    {
        if(!m_bArrival)
            m_bArrival = TRUE;
    }

    RwV3d vLookAt;
    CNtlMath::MathRwV3dAssign(&vLookAt, m_vStartPos.x, m_vStartPos.y + GetActorOffset(m_pActor), m_vStartPos.z);
    UpdateCameraTrans(&vCamPos, &vLookAt);

    GetNtlGameCameraManager()->SetLookAt(&vLookAt);
    GetNtlGameCameraManager()->SetCameraPos(&vCamPos);
}

void CNtlCameraDashController::CalcCameraEndTrans(RwReal fElapsed)
{
    RwV3d vCamPos, vDir, vDelta;
    RwV3d vPos = m_pActor->GetPosition();

    RwV3dSubMacro(&vDir, &vPos, &m_vStartPos); 
    RwV3dAssignMacro(&vDelta, &vDir);
    RwReal fCurrLen = RwV3dLength(&vDelta);
    RwV3dNormalize(&vDir, &vDir);

    RwReal fSpeed = fElapsed/CAMERA_DASH_END_TIME * fCurrLen * CAMERA_DASH_END_WEIGHT_SPEED;

    CNtlMath::MathRwV3dAssign(&vDelta, fSpeed*vDir.x, fSpeed*vDir.y, fSpeed*vDir.z);

    if(fCurrLen > NTL_EPSILON)
    {
        // delta time 동안 이동 거리를 구한다.
        if(fCurrLen - RwV3dLength(&vDelta) <= NTL_EPSILON)
            m_vStartPos = vPos;
        else
            CNtlMath::MathRwV3dAddMul(&m_vStartPos, &vDir, fSpeed);
    }

    RwV3d vLookAt;
    CNtlMath::MathRwV3dAssign(&vLookAt, m_vStartPos.x, m_vStartPos.y + GetActorOffset(m_pActor), m_vStartPos.z);
    UpdateCameraTrans(&vCamPos, &vLookAt);

    GetNtlGameCameraManager()->SetLookAt(&vLookAt);
    GetNtlGameCameraManager()->SetCameraPos(&vCamPos);
}

void CNtlCameraDashController::UpdateStart(RwReal fElapsed)
{
    if(m_fTime >= m_fStartTime)
        m_byCameraDashState = CAMERA_DASH_STATE_LOOP;
}

void CNtlCameraDashController::UpdateLoop(RwReal fElapsed)
{
    if(m_bArrival)
    {
        CalcCameraTrans(fElapsed*m_fDashSpeed, CAMERA_DASH_LOOP_DIST_LIMIT, fElapsed);
    }
    else
    {
        CalcCameraTrans(fElapsed*m_fDashSpeed*CAMERA_DASH_LOOP_WEIGHT_SPEED, CAMERA_DASH_LOOP_DIST_LIMIT, fElapsed);
    }
}

void CNtlCameraDashController::UpdateEnd(RwReal fElapsed)
{
    if(m_fTime > CAMERA_DASH_END_TIME)
    {
        m_fTime = CAMERA_DASH_END_TIME;
        Finish();
    }

    CalcCameraEndTrans(fElapsed);
}

void CNtlCameraDashController::Enter(void)
{
    GetNtlSobElapsedControlManager()->Reset();

    m_vStartPos = m_pActor->GetPosition();

    ResetData();
}

void CNtlCameraDashController::Exit(void)
{
    if(m_byCameraDashState != CAMERA_DASH_STATE_END)
    {
        m_byCameraDashState = CAMERA_DASH_STATE_END;
        m_fTime = 0.0f;
    }
}

void CNtlCameraDashController::Update(RwReal fElapsed)
{
    m_fTime += fElapsed;

    if(m_byCameraDashState == CAMERA_DASH_STATE_START)
        UpdateStart(fElapsed);
    else if(m_byCameraDashState == CAMERA_DASH_STATE_LOOP)
        UpdateLoop(fElapsed);
    else
        UpdateEnd(fElapsed);
}

void CNtlCameraDashController::UpdateData(RwBool bUpdate)
{
    ResetData();
}

void CNtlCameraDashController::SetActiveActor(const CNtlSobActor *pActor)
{
    m_pActor = const_cast<CNtlSobActor*>( pActor );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CAMERA_NPC_CONTROLLER_SEQ_CNT		10
#define CAMERA_NPC_CONTROLLER_SEQ_TIME		0.5f

CNtlCameraNpcController::CNtlCameraNpcController()
{
    SetType(CAMERA_CONTROL_NPC);
    SetFlags(NTL_CAMERA_CONTROL_NORMAL_PAUSE);

    m_eState			= CAMERA_NPC_STATE_NONE;
    m_pSeq				= NULL;
    m_hLookAtSerialId	= INVALID_SERIAL_ID;
    m_fTime				= 0.0f;

    RwReal arrTime[CAMERA_NPC_CONTROLLER_SEQ_CNT]  = { 0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f, 0.35f, 0.40f, 0.45f, CAMERA_NPC_CONTROLLER_SEQ_TIME};
    RwReal arrDistZ[CAMERA_NPC_CONTROLLER_SEQ_CNT] = { 0.3f,  0.5f,  0.7f,  0.8f,  0.85f, 0.90f, 0.93f, 0.96f, 0.98f, 1.0f};
    RwReal arrDistY[CAMERA_NPC_CONTROLLER_SEQ_CNT] = { 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f};

    CNtl2DTimeSequence *pSeq = NULL;
    CNtl2DTimeSequence *pTempSeq = NULL;

    for(RwInt32 i = 0; i < CAMERA_NPC_CONTROLLER_SEQ_CNT; ++i)
    {
        pTempSeq = NTL_NEW CNtl2DTimeSequence;
        pTempSeq->SetTime(arrTime[i]);
        pTempSeq->SetDistance(arrDistZ[i], arrDistY[i]);

        if(i == 0)
        {
            m_pSeq = pTempSeq;
            pSeq = pTempSeq;
        }
        else
        {
            pSeq->Add(pTempSeq);
            pSeq = pTempSeq;
        }
    }
}

CNtlCameraNpcController::~CNtlCameraNpcController()
{
    CNtlTimeSequence *pTempSeq;
    CNtlTimeSequence *pSeq = m_pSeq;
    while(pSeq)
    {
        pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );

		pSeq = pTempSeq;
    }

    m_pSeq = NULL;


    // highlight enable
    CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_hLookAtSerialId);
    if(pSobObj)
	{
		RwUInt32 uiFlags = pSobObj->GetFlags();
		pSobObj->SetFlags(uiFlags & ~SLFLAG_NOT_ALPHA_CAMERA_COLLI);
        pSobObj->GetSobProxy()->EnableHighlight(TRUE);
	}
}

void CNtlCameraNpcController::Enter(void)
{
    SetVisibleAllObject(FALSE);	
}

void CNtlCameraNpcController::Exit(void)
{
    SetVisibleAllObject(TRUE);

    ChangeState(CAMERA_NPC_STATE_END);
}

void CNtlCameraNpcController::Update(RwReal fElapsed)
{
    if(m_eState == CAMERA_NPC_STATE_START)
        UpdateStart(fElapsed);
    else if(m_eState == CAMERA_NPC_STATE_LOOP)
        UpdateLoop(fElapsed);
    else if(m_eState == CAMERA_NPC_STATE_END)
        UpdateEnd(fElapsed);

    // It is called every time for objects created during camera application. (Do not worry about duplication) Do not worry.
    SetVisibleAllObject(FALSE);
}

void CNtlCameraNpcController::UpdateData(RwBool bUpdate)
{
}

void CNtlCameraNpcController::SetActiveActor(const CNtlSobActor *pActor)
{
    if(pActor == NULL)
    {
        Finish();
        return;
    }

	RwUInt32 uiFlags = pActor->GetFlags();
	((CNtlSobActor*)pActor)->SetFlags(uiFlags | SLFLAG_NOT_ALPHA_CAMERA_COLLI);

    // highlight disable
    pActor->GetSobProxy()->EnableHighlight(FALSE);

    m_hLookAtSerialId = pActor->GetSerialID();

    RwV3d vPos	= pActor->GetPosition();
    RwV3d vDir	= pActor->GetDirection();
    RwV3dScale(&m_vDir, &vDir, -1.0f);

    RwReal fActorHeight = pActor->GetSobProxy()->GetPLEntityHeight();
    RwReal fActorDepth	= pActor->GetSobProxy()->GetPLEntityDepth();

    RwReal fWeightFactor = 3.0f + (fActorDepth + fActorHeight)/2.0f;
    RwReal fStartDist	 = fActorDepth / 2.0f + fWeightFactor;	

    m_vStartPos.x = vPos.x + vDir.x * fStartDist;
    m_vStartPos.y = vPos.y + fActorHeight * 0.7f;
    m_vStartPos.z = vPos.z + vDir.z * fStartDist;

    RwV3d vLookAt;
    vLookAt.x = vPos.x;
    vLookAt.y = m_vStartPos.y;
    vLookAt.z = vPos.z;

    RwReal fRotY = CNtlMath::LineToAngleY(&m_vDir);

    RwMatrixSetIdentity(&m_matRot);
    RwMatrixRotate (&m_matRot, &CNtlPLGlobal::m_vYAxisV3, fRotY, rwCOMBINEPOSTCONCAT);		

    GetNtlGameCameraManager()->SetLookAt(&vLookAt);
    GetNtlGameCameraManager()->SetCameraPos(&m_vStartPos);

    ChangeState(CAMERA_NPC_STATE_START);
}

void CNtlCameraNpcController::UpdateStart(RwReal fElapsed)
{
    m_fTime += fElapsed;
    RwReal fTime = m_fTime;

    if(fTime >= CAMERA_NPC_CONTROLLER_SEQ_TIME)
    {
        fTime = CAMERA_NPC_CONTROLLER_SEQ_TIME;
        ChangeState(CAMERA_NPC_STATE_LOOP);
    }

    CNtl2DTimeSequence::S2DDist s2DDist = *(reinterpret_cast<CNtl2DTimeSequence::S2DDist*>(m_pSeq->GetData(fTime)));

    RwV3d vOffset, vAdd;
    CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, 0.0f, s2DDist.fX);

    RwV3dTransformPoint(&vAdd, &vOffset, &m_matRot);

    RwV3d vCamPos;
    RwV3dAdd(&vCamPos, &m_vStartPos, &vAdd);

    RwV3d vLookAt;
    CNtlMath::MathRwV3dAssign(&vLookAt, vCamPos.x + m_vDir.x*10.0f, vCamPos.y + m_vDir.y*10.0f, vCamPos.z + m_vDir.z*10.0f);

    GetNtlGameCameraManager()->SetLookAt(&vLookAt);
    GetNtlGameCameraManager()->SetCameraPos(&vCamPos);

    if(m_eState == CAMERA_NPC_STATE_LOOP)
    {
        CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_hLookAtSerialId);
        if(pSobObj == NULL)
        {
            Finish();
            return;
        }

        RwV3d vPos = pSobObj->GetPosition();
        RwV3dSubMacro(&m_vIdleOffsetPos, &vPos, &vCamPos);
    }
}

void CNtlCameraNpcController::UpdateLoop(RwReal fElapsed)
{
    CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_hLookAtSerialId);
    if(pSobObj == NULL)
    {
        Finish();
        return;
    }

    RwV3d vPos = pSobObj->GetPosition();
    RwV3d vCamPos, vLookAt;

    RwV3dSubMacro(&vCamPos, &vPos, &m_vIdleOffsetPos);

    CNtlMath::MathRwV3dAssign(&vLookAt, vCamPos.x + m_vDir.x*10.0f, vCamPos.y + m_vDir.y*10.0f, vCamPos.z + m_vDir.z*10.0f);

    GetNtlGameCameraManager()->SetLookAt(&vLookAt);
    GetNtlGameCameraManager()->SetCameraPos(&vCamPos);
}

void CNtlCameraNpcController::UpdateEnd(RwReal fElapsed)
{
    m_fTime += fElapsed;

    Finish();
}

void CNtlCameraNpcController::ChangeState(RwInt32 iState)
{
    if(iState == CAMERA_NPC_STATE_START)
    {
        m_fTime = 0.0f;
    }
    else if(iState == CAMERA_NPC_STATE_LOOP)
    {
        m_fTime = 0.0f;
    }
    else if(iState == CAMERA_NPC_STATE_END)
    {
        m_fTime = 0.0f;
    }

    m_eState = (ECameraNpcState)iState;
}

//////////////////////////////////////////////////////////////////////////
CNtlCameraFromArtData::CNtlCameraFromArtData() 
{
    SetFlags(NTL_CAMERA_CONTROL_NORMAL_PAUSE);
    m_fTime = 0.0f;
    m_pActor = NULL;
    RwMatrixSetIdentity(&m_matDir);
    m_ActPos = ZeroAxis;
}
CNtlCameraFromArtData::~CNtlCameraFromArtData() 
{

}

void CNtlCameraFromArtData::CreateSequenceData( CNtl3DTimeSequence** ppSeqPos, CNtl3DTimeSequence** ppSeqTarget, VecCamaraData* pVecCamaraData ) 
{
    CNtl3DTimeSequence *pSeqPos = NULL;
    CNtl3DTimeSequence *pTempSeqPos = NULL;
    CNtl3DTimeSequence *pSeqTarget = NULL;
    CNtl3DTimeSequence *pTempSeqTarget = NULL;

    for(UINT i = 0; i < pVecCamaraData->size(); ++i)
    {
        pTempSeqPos = NTL_NEW CNtl3DTimeSequence;        
        pTempSeqPos->SetTime(pVecCamaraData->at(i).fTime);        
        pTempSeqPos->SetData(pVecCamaraData->at(i).vPos);

        pTempSeqTarget = NTL_NEW CNtl3DTimeSequence;
        pTempSeqTarget->SetTime(pVecCamaraData->at(i).fTime);
        pTempSeqTarget->SetData(pVecCamaraData->at(i).vTargetPos);

        if(i == 0)
        {
            *ppSeqPos = pSeqPos = pTempSeqPos;		
            *ppSeqTarget = pSeqTarget = pTempSeqTarget;            
        }
        else
        {
            pSeqPos->Add(pTempSeqPos);
            pSeqPos = pTempSeqPos;

            pSeqTarget->Add(pTempSeqTarget);
            pSeqTarget = pTempSeqTarget;
        }
    }
}

void CNtlCameraFromArtData::UpdateCameraTrans( RwV3d *pCamPos, const RwV3d *pLookAt, RwReal fDistOffset /*= 0.0f*/ ) 
{
    RwV3d vPos, vTarget;    

    RwV3dTransformPoint(&vPos, pCamPos, &m_matDir);
    RwV3dTransformPoint(&vTarget, pLookAt, &m_matDir);

    vPos += m_ActPos;
    vTarget += m_ActPos;

    GetNtlGameCameraManager()->SetCameraPos(&vPos);	
    GetNtlGameCameraManager()->SetLookAt(&vTarget);
}

void CNtlCameraFromArtData::AddCameraData( RwReal fTime, RwReal posX, RwReal posY, RwReal posZ, RwReal targetX, RwReal targetY, RwReal targetZ, VecCamaraData* pVecCameraData ) 
{
    SCameraData data;
    data.fTime = fTime;
    data.vPos.x = posX;
    data.vPos.y = posY;
    data.vPos.z = posZ;
    data.vTargetPos.x = targetX;
    data.vTargetPos.y = targetY;
    data.vTargetPos.z = targetZ;    

    pVecCameraData->push_back(data);    
}

void CNtlCameraFromArtData::SetActiveActor( const CNtlSobActor *pActor ) 
{
    m_pActor = const_cast<CNtlSobActor*>(pActor);	

    RwV3d vRight, vUp;
    RwV3dCrossProduct(&vRight, &m_pActor->GetDirection(), &YAxis);
    RwV3dCrossProduct(&vUp, &vRight, &m_pActor->GetDirection());    
    RwV3dNormalize(&vRight, &vRight);
    RwV3dNormalize(&vUp, &vUp);

    *RwMatrixGetAt(&m_matDir) = m_pActor->GetDirection();
    *RwMatrixGetRight(&m_matDir) = vRight;
    *RwMatrixGetUp(&m_matDir) = YAxis;
    RwMatrixUpdate(&m_matDir);
    m_ActPos = m_pActor->GetPosition();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlCameraDragonBallController::CNtlCameraDragonBallController()
{
    SetType(CAMERA_CONTROL_DRAGONBALL);	

    m_eState			= CAMERA_DB_STATE_NONE;
    m_pSeqPos           = NULL;
    m_pSeqTarget        = NULL;	    
    m_fOrgFov           = 60.0f;    

    AddCameraData(0.0f, -7.402f, 8.172f, 24.871f, -2.176f, 6.381f, 3.98f, &m_vecData);
    AddCameraData(1.1f, -7.402f, 8.172f, 24.871f, -2.176f, 6.381f, 3.98f, &m_vecData);
    AddCameraData(2.13f,-7.402f, 10.812f,27.062f, -2.176f, 19.591f, 6.003f, &m_vecData);
    AddCameraData(3.33f, -7.402f, 18.332f, 31.755f, -2.176f, 27.136f, 15.634f, &m_vecData);
    AddCameraData(5.00f, -7.402f, 31.249f, 36.34f, -2.176f, 39.223f, 22.991f, &m_vecData);    
    AddCameraData(5.40f, -13.216f, 20.192f, 77.251f, -4.053f, 34.289f, 24.023f, &m_vecData);    
    AddCameraData(5.46f, -13.216f, 20.192f, 77.251f, -4.053f, 34.39f, 24.023f, &m_vecData);
    AddCameraData(5.53f, -13.216f, 20.192f, 77.251f, -4.053f, 34.19f, 24.023f, &m_vecData);
    AddCameraData(5.60f, -13.216f, 20.192f, 77.251f, -4.053f, 34.34f, 24.023f, &m_vecData);
    AddCameraData(5.67f, -13.216f, 20.192f, 77.251f, -4.053f, 34.29f, 24.023f, &m_vecData);
    CreateSequenceData(&m_pSeqPos, &m_pSeqTarget, &m_vecData);

    // Idle용 카메라        
    m_DataIdle.fTime = 7.0f;
    m_DataIdle.vPos.x = -13.216f;
    m_DataIdle.vPos.y = 20.192f;
    m_DataIdle.vPos.z = 77.251f;    
    m_DataIdle.vTargetPos.x = -4.053f;
    m_DataIdle.vTargetPos.y = 34.29f;
    m_DataIdle.vTargetPos.z = 24.023f;    

    CreateScatterData();
}

void CNtlCameraDragonBallController::CreateScatterData() 
{
    AddCameraData(0.0f, -13.585f, 6.0f, 25.872f, 0.0f, 8.0f, 0.0f, &m_vecScatterData);
    AddCameraData(0.3f, -13.585f, 6.0f, 25.872f, 0.0f, 28.0f, 0.0f, &m_vecScatterData);
    CreateSequenceData(&m_pScatterSeqPos, &m_pScatterSeqTarget, &m_vecScatterData);
}

CNtlCameraDragonBallController::~CNtlCameraDragonBallController()
{
    CNtlTimeSequence *pTempSeq;
    CNtlTimeSequence *pSeq = m_pSeqPos;
    while(pSeq)
    {
        pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );

		pSeq = pTempSeq;
    }

    pSeq = m_pSeqTarget;
    while(pSeq)
    {
        pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );
		
		pSeq = pTempSeq;
    }

    pSeq = m_pScatterSeqPos;
    while(pSeq)
    {
        pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );
		
		pSeq = pTempSeq;
    }

    pSeq = m_pScatterSeqTarget;
    while(pSeq)
    {
        pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );
		
		pSeq = pTempSeq;
    }


    m_pSeqPos = NULL;
    m_pSeqTarget = NULL;
    m_pScatterSeqPos = NULL;
    m_pScatterSeqTarget = NULL;
}



void CNtlCameraDragonBallController::Enter(void)
{
    GetSceneManager()->SetWorldPVSActivation(FALSE);
}

void CNtlCameraDragonBallController::Exit(void)
{
    GetNtlGameCameraManager()->SetFov(m_fOrgFov);
    GetSceneManager()->SetWorldPVSActivation(TRUE);

    ChangeState(CAMERA_DB_STATE_END);
}

void CNtlCameraDragonBallController::Update(RwReal fElapsed)
{
    m_fTime += fElapsed;

    if(m_eState == CAMERA_DB_STATE_START)
        UpdateStart(fElapsed);
    else if(m_eState == CAMERA_DB_STATE_LOOP)
        UpdateLoop(fElapsed);
    else if(m_eState == CAMERA_DB_STATE_SCATTER)
        UpdateScatter(fElapsed);
    else if(m_eState == CAMERA_DB_STATE_END)
        UpdateEnd(fElapsed);
}

void CNtlCameraDragonBallController::SetActiveActor(const CNtlSobActor *pActor)
{
    if(pActor == NULL)
    {
        Finish();
        return;
    }

    m_fOrgFov = GetNtlGameCameraManager()->GetFov();

    CNtlCameraFromArtData::SetActiveActor(pActor);

    // Idle Camera 위치 변환
    RwV3dTransformPoint(&m_DataIdle.vPos, &m_DataIdle.vPos, &m_matDir);
    RwV3dTransformPoint(&m_DataIdle.vTargetPos, &m_DataIdle.vTargetPos, &m_matDir);

    m_DataIdle.vPos += m_ActPos;
    m_DataIdle.vTargetPos += m_ActPos;

    ChangeState(CAMERA_DB_STATE_START);
}

void CNtlCameraDragonBallController::UpdateStart(RwReal fElapsed)
{
    if(m_fTime >= m_DataIdle.fTime)
    {
        ChangeState(CAMERA_DB_STATE_LOOP);
        return;
    }

    RwV3d vPos = *(RwV3d*)m_pSeqPos->GetData(m_fTime);
    RwV3d vTarget = *(RwV3d*)m_pSeqTarget->GetData(m_fTime);

    UpdateCameraTrans(&vPos, &vTarget);
}

void CNtlCameraDragonBallController::UpdateLoop(RwReal fElapsed)
{
    GetNtlGameCameraManager()->SetLookAt(&m_DataIdle.vTargetPos);
    GetNtlGameCameraManager()->SetCameraPos(&m_DataIdle.vPos);
}

void CNtlCameraDragonBallController::UpdateScatter( RwReal fElapsed ) 
{
    RwV3d vPos = *(RwV3d*)m_pScatterSeqPos->GetData(m_fTime);
    RwV3d vTarget = *(RwV3d*)m_pScatterSeqTarget->GetData(m_fTime);

    UpdateCameraTrans(&vPos, &vTarget);
}

void CNtlCameraDragonBallController::UpdateEnd(RwReal fElapsed)
{
    Finish();
}

void CNtlCameraDragonBallController::ChangeState(RwInt32 iState)
{
    if(iState == CAMERA_DB_STATE_START)
    {
        GetNtlGameCameraManager()->SetFov(45.0f);        
        m_fTime = 0.0f;
    }
    else if(iState == CAMERA_DB_STATE_LOOP)
    {
        m_fTime = 0.0f;        
    }
    else if(iState == CAMERA_DB_STATE_SCATTER) // 카메라 흩어지는 연출
    {
        GetNtlGameCameraManager()->SetFov(60.0f);
        m_fTime = 0.0f;
    }
    else if(iState == CAMERA_DB_STATE_END)
    {        
        m_fTime = 0.0f;
    }

    m_eState = (ECameraDBState)iState;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlSobCameraNode::CNtlSobCameraNode()
{
    m_hCinematic = INVALID_CINEMATIC_ID;
}


CNtlSobCameraNode::~CNtlSobCameraNode() 
{
    CNtlSobCameraNode *pNode;
    ListChildNode::iterator it;

    for(it = m_listChildNode.begin(); it != m_listChildNode.end(); it++)
    {
        pNode = (*it);
        pNode->Exit();

		NTL_DELETE( pNode );
    }

    m_listChildNode.clear();

    if(m_hCinematic != INVALID_CINEMATIC_ID)
    {
        CNtlPLEventGenerator::CinematicEcho(m_hCinematic);

        m_hCinematic = INVALID_CINEMATIC_ID;
    }
}

void CNtlSobCameraNode::Update(RwReal fElapsed)
{
    CNtlSobCameraNode *pNode;
    ListChildNode::iterator it;

    for(it = m_listChildNode.begin(); it != m_listChildNode.end(); )
    {
        pNode = (*it);
        pNode->Update(fElapsed);

        if(pNode->IsFinish())
        {
            pNode->Exit();

			NTL_DELETE( pNode );

            it = m_listChildNode.erase(it);
        }
        else
            it++;
    }
}

void CNtlSobCameraNode::Enter(void)
{
    ListChildNode::iterator it;

    for(it = m_listChildNode.begin(); it != m_listChildNode.end(); it++)
    {
        (*it)->Enter();
    }
}

RwBool CNtlSobCameraNode::IsFinish() 
{
    if(m_listChildNode.size() == 0)
        return TRUE;

    return FALSE;
}

void CNtlSobCameraNode::SetCinematicInstance(CINEMATIC_HANDLE hCinematic)
{
    m_hCinematic = hCinematic;
}

void CNtlSobCameraNode::AddChildNode(CNtlSobCameraNode *pChildNode)
{
    m_listChildNode.push_back(pChildNode);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlSobCameraActorDist::CNtlSobCameraActorDist()
{
    m_fCurrTime			= 0.0f;
    m_fLifeTime			= 0.0f;
    m_fHeightOffset		= 0.0f;

    m_bDynamicLookAt	= FALSE;
    m_hLookAtSerialId	= INVALID_SERIAL_ID;
}

CNtlSobCameraActorDist::~CNtlSobCameraActorDist()
{
}

void CNtlSobCameraActorDist::Update(RwReal fElapsed)
{
    CNtlSobCameraNode::Update(fElapsed);

    m_fCurrTime += fElapsed;

    if(m_bDynamicLookAt)
    {
        CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_hLookAtSerialId) );
        if(pActor == NULL)
            return;

        RwV3d vLookAt = pActor->GetPosition();

        RwReal fCharHeight = pActor->GetSobProxy()->GetPLEntityHeight();
        vLookAt.y += fCharHeight * m_fHeightOffset;

        GetNtlGameCameraManager()->SetLookAt(&vLookAt);
    }
}

void CNtlSobCameraActorDist::Enter(void) 
{
    CNtlSobCameraNode::Enter();

    CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_hLookAtSerialId) );
    if(pActor == NULL)
        return;

    RwV3d vPos = pActor->GetPosition();
    RwV3d vDir = pActor->GetDirection();

    RwReal fCurrAngleX = m_fAngleX;
    RwReal fCurrAngleY = CNtlMath::LineToAngleY(&vDir);
    fCurrAngleY += m_fAngleY;

    RwMatrix *pMat = RwMatrixCreate();

    RwMatrixSetIdentity(pMat);
    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vXAxisV3, fCurrAngleX, rwCOMBINEPOSTCONCAT);
    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vYAxisV3, fCurrAngleY, rwCOMBINEPOSTCONCAT);
    RwMatrixUpdate(pMat);

    vDir = pMat->at;

    RwMatrixDestroy(pMat);

    RwReal fCharHeight = pActor->GetSobProxy()->GetPLEntityHeight();
    vPos.y += fCharHeight * m_fHeightOffset;

    RwV3d vLookAt;
    if(m_bDynamicLookAt)
        vLookAt = vPos;
    else
        vLookAt = m_vLookAt;

    RwV3d vCamPos;
    CNtlMath::MathRwV3dAssign(&vCamPos, vPos.x+m_fDistance*vDir.x, vPos.y+m_fDistance*vDir.y, vPos.z+m_fDistance*vDir.z);

    GetNtlGameCameraManager()->SetLookAt(&vLookAt);
    GetNtlGameCameraManager()->SetCameraPos(&vCamPos);
}

RwBool CNtlSobCameraActorDist::IsFinish(void)
{
    if(!CNtlSobCameraNode::IsFinish())
        return FALSE;

    if(m_fCurrTime >= m_fLifeTime)
        return TRUE;

    return FALSE;
}

void CNtlSobCameraActorDist::SetData(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
                                     RwReal fDistance, RwReal fLifeTime, RwV3d vLookAt)
{
    m_fLifeTime			= fLifeTime;
    m_hLookAtSerialId	= hSerialId;
    m_fHeightOffset		= fHeightOffset;
    m_fAngleX			= fAngleX;
    m_fAngleY			= fAngleY;
    m_fDistance			= fDistance;
    m_vLookAt			= vLookAt;
}

void CNtlSobCameraActorDist::SetDynamicLookAtData(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
                                                  RwReal fDistance, RwReal fLifeTime)
{
    m_fLifeTime			= fLifeTime;
    m_hLookAtSerialId	= hSerialId;
    m_fHeightOffset		= fHeightOffset;
    m_fAngleX			= fAngleX;
    m_fAngleY			= fAngleY;
    m_fDistance			= fDistance;

    m_bDynamicLookAt	= TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlSobCameraRotate::CNtlSobCameraRotate()
{
    m_byRotType = CAMERA_DT_AXIS_X_ROTATE;

    m_fCurrTime		= 0.0f;
    m_fInterTime	= 0.0f;
    m_fLifeTime		= 0.0f;

    m_fCurrAngleX	= 0.0f;
    m_fCurrAngleY	= 0.0f;
    m_fCurrAngleZ	= 0.0f;

    m_fLastAngleX	= 0.0f;
    m_fLastAngleY	= 0.0f;
    m_fLastAngleZ	= 0.0f;
}

CNtlSobCameraRotate::~CNtlSobCameraRotate()
{
}

void CNtlSobCameraRotate::Update(RwReal fElapsed)
{
    CNtlSobCameraNode::Update(fElapsed);

    m_fCurrTime += fElapsed;

    if(m_fCurrTime < m_fInterTime)
    {
        if(m_byRotType & CAMERA_DT_AXIS_X_ROTATE)
            m_fCurrAngleX += fElapsed*m_fIncAngleX;
        if(m_byRotType & CAMERA_DT_AXIS_Y_ROTATE)
            m_fCurrAngleY += fElapsed*m_fIncAngleY;
        if(m_byRotType & CAMERA_DT_AXIS_Z_ROTATE)
        {
            m_fCurrAngleZ += fElapsed*m_fIncAngleZ;
            GetNtlGameCameraManager()->SetRoll(m_fCurrAngleZ);
        }
    }
    else
    {
        if(m_byRotType & CAMERA_DT_AXIS_X_ROTATE)
            m_fCurrAngleX = m_fLastAngleX;
        if(m_byRotType & CAMERA_DT_AXIS_Y_ROTATE)
            m_fCurrAngleY = m_fLastAngleY;
        if(m_byRotType & CAMERA_DT_AXIS_Z_ROTATE)
            GetNtlGameCameraManager()->SetRoll(m_fLastAngleZ);
    }

    RwMatrix *pMat = RwMatrixCreate();
    RwMatrixSetIdentity(pMat);
    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vXAxisV3, m_fCurrAngleX, rwCOMBINEPOSTCONCAT);
    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vYAxisV3, m_fCurrAngleY, rwCOMBINEPOSTCONCAT);
    RwMatrixUpdate(pMat);

    RwV3d vDir = pMat->at;

    RwMatrixDestroy(pMat);

    const RwV3d *pCamPos = GetNtlGameCameraManager()->GetCameraPos();

    RwV3d vLookAt;

    CNtlMath::MathRwV3dAssign(&vLookAt, pCamPos->x+vDir.x*10.0f, pCamPos->y+vDir.y*10.0f, pCamPos->z+vDir.z*10.0f); 

    GetNtlGameCameraManager()->SetLookAt(&vLookAt);
}

void CNtlSobCameraRotate::Enter(void)
{
    CNtlSobCameraNode::Enter();

    if(m_fLifeTime <= 0.0f)
        return;

    RwV3d vDir;
    RwV3dAssignMacro(&vDir, GetNtlGameCameraManager()->GetCameraDir());

    m_fCurrAngleX	= CNtlMath::LineToAngleX(&vDir);
    m_fCurrAngleY	= CNtlMath::LineToAngleY(&vDir);
    m_fCurrAngleZ	= GetNtlGameCameraManager()->GetRoll();

    if(m_byRotType & CAMERA_DT_AXIS_X_ROTATE)
        m_fIncAngleX	= (m_fLastAngleX - m_fCurrAngleX)/m_fInterTime;

    if(m_byRotType & CAMERA_DT_AXIS_Y_ROTATE)
        m_fIncAngleY	= (m_fLastAngleY - m_fCurrAngleY)/m_fInterTime;

    if(m_byRotType & CAMERA_DT_AXIS_Z_ROTATE)
        m_fIncAngleZ	= (m_fLastAngleZ - m_fCurrAngleZ)/m_fInterTime;
}

void CNtlSobCameraRotate::Exit(void)
{
    CNtlSobCameraNode::Exit();

    RwReal fLastAngleX = m_fCurrAngleX;
    RwReal fLastAngleY = m_fCurrAngleY;
    RwReal fLastAngleZ = m_fCurrAngleZ;

    if(m_byRotType & CAMERA_DT_AXIS_X_ROTATE)
        fLastAngleX = m_fLastAngleX;
    if(m_byRotType & CAMERA_DT_AXIS_Y_ROTATE)
        fLastAngleY = m_fLastAngleY;
    if(m_byRotType & CAMERA_DT_AXIS_Z_ROTATE)
        fLastAngleZ = m_fLastAngleZ;

    RwMatrix *pMat = RwMatrixCreate();
    RwMatrixSetIdentity(pMat);

    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vXAxisV3, fLastAngleX, rwCOMBINEPOSTCONCAT);
    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vYAxisV3, fLastAngleY, rwCOMBINEPOSTCONCAT);

    RwMatrixUpdate(pMat);

    RwV3d vDir = pMat->at;

    RwMatrixDestroy(pMat);

    const RwV3d *pCamPos = GetNtlGameCameraManager()->GetCameraPos();

    RwV3d vLookAt;

    CNtlMath::MathRwV3dAssign(&vLookAt, pCamPos->x+vDir.x*10.0f, pCamPos->y+vDir.y*10.0f, pCamPos->z+vDir.z*10.0f); 

    GetNtlGameCameraManager()->SetLookAt(&vLookAt);
}

RwBool CNtlSobCameraRotate::IsFinish(void)
{
    if(!CNtlSobCameraNode::IsFinish())
        return FALSE;

    if(m_fCurrTime >= m_fLifeTime)
        return TRUE;

    return FALSE;
}

void CNtlSobCameraRotate::SetAxisXData(RwReal fLastAngleX, RwReal fInterTime, RwReal fLifeTime)
{
    m_byRotType		= CAMERA_DT_AXIS_X_ROTATE;
    m_fLastAngleX	= fLastAngleX;
    m_fInterTime	= fInterTime;
    m_fLifeTime		= fLifeTime;

    NTL_ASSERT(m_fInterTime > 0.0f, "CNtlSobCameraRotate::SetAxisXData");
}

void CNtlSobCameraRotate::SetAxisYData(RwReal fLastAngleY, RwReal fInterTime, RwReal fLifeTime)
{
    m_byRotType		= CAMERA_DT_AXIS_Y_ROTATE;
    m_fLastAngleY	= fLastAngleY;
    m_fInterTime	= fInterTime;
    m_fLifeTime		= fLifeTime;

    NTL_ASSERT(m_fInterTime > 0.0f, "CNtlSobCameraRotate::SetAxisYData");
}

void CNtlSobCameraRotate::SetAxisZData(RwReal fLastAngleZ, RwReal fInterTime, RwReal fLifeTime)
{
    m_byRotType		= CAMERA_DT_AXIS_Z_ROTATE;
    m_fLastAngleZ	= fLastAngleZ;
    m_fInterTime	= fInterTime;
    m_fLifeTime		= fLifeTime;

    NTL_ASSERT(m_fInterTime > 0.0f, "CNtlSobCameraRotate::SetAxisZData");
}

void CNtlSobCameraRotate::SetAxisXYZData(RwReal fLastAngleX, RwReal fLastAngleY, RwReal fLastAngleZ, RwReal fInterTime, RwReal fLifeTime)
{
    m_byRotType		= CAMERA_DT_AXIS_X_ROTATE | CAMERA_DT_AXIS_Y_ROTATE | CAMERA_DT_AXIS_Z_ROTATE;
    m_fLastAngleX	= fLastAngleX;
    m_fLastAngleY	= fLastAngleY;
    m_fLastAngleZ	= fLastAngleZ;
    m_fInterTime	= fInterTime;
    m_fLifeTime		= fLifeTime;

    NTL_ASSERT(m_fInterTime > 0.0f, "CNtlSobCameraRotate::SetAxisXYZData");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlSobCameraFov::CNtlSobCameraFov()
{
    m_fCurrTime		= 0.0f;
    m_fInterTime	= 0.0f;	

    m_fCurrFov		= 0.0f;
    m_fIncFov		= 0.0f;
    m_fLastFov		= 0.0f;
}

CNtlSobCameraFov::~CNtlSobCameraFov()
{
}

void CNtlSobCameraFov::Update(RwReal fElapsed)
{
    CNtlSobCameraNode::Update(fElapsed);

    m_fCurrTime += fElapsed;

    m_fCurrFov += fElapsed*m_fIncFov;

    GetNtlGameCameraManager()->SetFov(m_fCurrFov);
}

void CNtlSobCameraFov::Enter(void)
{
    m_fCurrFov = GetNtlGameCameraManager()->GetFov();

    m_fIncFov = (m_fLastFov - m_fCurrFov)/m_fInterTime;

    CNtlSobCameraNode::Enter();
}

void CNtlSobCameraFov::Exit(void)
{
    CNtlSobCameraNode::Exit();
}

RwBool CNtlSobCameraFov::IsFinish(void)
{
    if(!CNtlSobCameraNode::IsFinish())
        return FALSE;

    if(m_fCurrTime >= m_fInterTime)
        return TRUE;

    return FALSE;
}

void CNtlSobCameraFov::SetFovData(RwReal fLastFov, RwReal fInterTime)
{
    m_fLastFov		= fLastFov;
    m_fInterTime	= fInterTime;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CNtlSobCameraPauseNode::CNtlSobCameraPauseNode()
{
    m_fCurrTime	= 0.0f;
    m_fLifeTime	= 0.0f;

    m_bLoop		= FALSE;
}

CNtlSobCameraPauseNode::~CNtlSobCameraPauseNode()
{

}

void CNtlSobCameraPauseNode::Update(RwReal fElapsed)
{
    CNtlSobCameraNode::Update(fElapsed);

    m_fCurrTime += fElapsed;
}

RwBool CNtlSobCameraPauseNode::IsFinish()
{
    if(!CNtlSobCameraNode::IsFinish())
        return FALSE;

    if(m_bLoop)
        return FALSE;

    if(m_fCurrTime >= m_fLifeTime)
        return TRUE;

    return FALSE;
}

void CNtlSobCameraPauseNode::SetData(RwReal fLifeTime, RwBool bLoop /*= FALSE*/) 
{
    m_fLifeTime	= fLifeTime;
    m_bLoop		= bLoop;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlSobCameraSplineNode::CNtlSobCameraSplineNode()
{
    m_pDTNode		= NULL;
    m_pDTProp		= NULL;
    m_fCurrTime		= 0.0f;
    m_fLifeTime		= 0.0f;

    m_eMode								= DT_SPLINE_POS_CAMERA_LOOKAT_ACTOR;
    m_sLookAtActor.hLookAtSerialId		= INVALID_SERIAL_ID;
    m_sLookAtActor.fLookAtHeightOffset	= 0.0f;
}

CNtlSobCameraSplineNode::~CNtlSobCameraSplineNode()
{
	NTL_DELETE( m_pDTNode );
}

void CNtlSobCameraSplineNode::UpdateLookAtActorMode(RwReal fElapsed)
{
    if(m_eMode == DT_SPLINE_POS_ACTOR_LOOKAT_TWO_ACTOR)
    {
        if(m_sLookAtActor.hLookAtSerialId != INVALID_SERIAL_ID)
        {
            CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_hPosSerialId) );
            if(pActor == NULL)
                return;

            CNtlSobActor *pTarActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_sLookAtActor.hLookAtSerialId) );
            if(pTarActor == NULL)
                return;

            RwV3d vLookAt = pActor->GetPosition();
            RwV3d vTarLookAt = pTarActor->GetPosition();

            vLookAt.x = ( vLookAt.x + vTarLookAt.x ) / 2.0f;
            vLookAt.y = ( vLookAt.y + vTarLookAt.y ) / 2.0f;
            vLookAt.z = ( vLookAt.z + vTarLookAt.z ) / 2.0f;

            RwReal fCharHeight = pActor->GetSobProxy()->GetPLEntityHeight();
            vLookAt.y += fCharHeight * m_sLookAtActor.fLookAtHeightOffset;

            GetNtlGameCameraManager()->SetLookAt(&vLookAt);
        }
    }
    else
    {
        if(m_sLookAtActor.hLookAtSerialId != INVALID_SERIAL_ID)
        {
            CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_sLookAtActor.hLookAtSerialId) );
            if(pActor == NULL)
                return;

            RwV3d vLookAt = pActor->GetPosition();

            RwReal fCharHeight = pActor->GetSobProxy()->GetPLEntityHeight();
            vLookAt.y += fCharHeight * m_sLookAtActor.fLookAtHeightOffset;

            GetNtlGameCameraManager()->SetLookAt(&vLookAt);
        }
    }
}

void CNtlSobCameraSplineNode::Update(RwReal fElapsed)
{
    m_fCurrTime += fElapsed;

    if(m_pDTNode == NULL)
        return;

    if(m_pDTNode)
    {
        m_pDTNode->Update(fElapsed);
    }

    if(m_eMode == DT_SPLINE_POS_CAMERA_LOOKAT_ACTOR || m_eMode == DT_SPLINE_POS_ACTOR_LOOKAT_ACTOR ||
        m_eMode == DT_SPLINE_POS_LOOKAT_ACTOR || m_eMode == DT_SPLINE_POS_ACTOR_LOOKAT_TWO_ACTOR)
    {
        UpdateLookAtActorMode(fElapsed);
    }
}

void CNtlSobCameraSplineNode::EnterPosCameraLookAtActorMode(void)
{
    CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_sLookAtActor.hLookAtSerialId) );
    if(pActor == NULL)
        return;

    RwV3d vStartPos;
    RwV3dAssignMacro(&vStartPos, GetNtlGameCameraManager()->GetCameraPos());

    RwV3d vActorPos = pActor->GetPosition();
    RwV3d vSub;
    RwV3dSubMacro(&vSub, &vActorPos, &vStartPos);
    RwV3dNormalize(&vSub, &vSub);

    CNtlDTSplineNode *pDTSplineNode = NTL_NEW CNtlDTSplineNode;
    pDTSplineNode->SetData(m_pDTProp, this, vActorPos, vSub);
    m_pDTNode = pDTSplineNode;
}

void CNtlSobCameraSplineNode::EnterPosActorLookAtActorMode(void)
{
    CNtlSobActor *pPosActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_hPosSerialId) );
    if(pPosActor == NULL)
        return;

    CNtlSobActor *pLookAtActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_sLookAtActor.hLookAtSerialId) );
    if(pLookAtActor == NULL)
        return;

    RwV3d vStartPos = pPosActor->GetPosition();
    RwV3d vDir	= pPosActor->GetDirection();
    m_fAngleY = CNtlMath::LineToAngleY(&vDir);

    RwMatrix *pMat = RwMatrixCreate();
    RwMatrixSetIdentity(pMat);

    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vXAxisV3, m_fAngleX, rwCOMBINEPOSTCONCAT);		// Pitch
    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vYAxisV3, m_fAngleY, rwCOMBINEPOSTCONCAT);		// Yaw
    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vZAxisV3, m_fAngleZ, rwCOMBINEPOSTCONCAT);		// Roll
    RwMatrixUpdate(pMat);

    RwV3d vLookAtPos;
    vLookAtPos.x = vStartPos.x + pMat->at.x*100.0f;
    vLookAtPos.y = vStartPos.y + pMat->at.y*100.0f;
    vLookAtPos.z = vStartPos.z + pMat->at.z*100.0f;

    CNtlDTSplineNode *pDTSplineNode = NTL_NEW CNtlDTSplineNode;
    pDTSplineNode->SetData(m_pDTProp, this, vStartPos, vStartPos, vLookAtPos, DIRECT_LOOKAT_SELF_YAXIS);

    RwMatrixDestroy(pMat);

    m_pDTNode = pDTSplineNode;
}

void CNtlSobCameraSplineNode::EnterPosActorLookAtTwoActorMode(void)
{
    CNtlSobActor *pPosActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_hPosSerialId) );
    if(pPosActor == NULL)
        return;

    CNtlSobActor *pLookAtActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_sLookAtActor.hLookAtSerialId) );
    if(pLookAtActor == NULL)
        return;

    RwV3d vStartPos = pPosActor->GetPosition();
    RwV3d vDir	= pPosActor->GetDirection();
    m_fAngleY = CNtlMath::LineToAngleY(&vDir);

    RwMatrix *pMat = RwMatrixCreate();
    RwMatrixSetIdentity(pMat);

    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vXAxisV3, m_fAngleX, rwCOMBINEPOSTCONCAT);		// Pitch
    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vYAxisV3, m_fAngleY, rwCOMBINEPOSTCONCAT);		// Yaw
    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vZAxisV3, m_fAngleZ, rwCOMBINEPOSTCONCAT);		// Roll
    RwMatrixUpdate(pMat);

    RwV3d vLookAtPos;
    vLookAtPos.x = vStartPos.x + pMat->at.x*100.0f;
    vLookAtPos.y = vStartPos.y + pMat->at.y*100.0f;
    vLookAtPos.z = vStartPos.z + pMat->at.z*100.0f;

    CNtlDTSplineNode *pDTSplineNode = NTL_NEW CNtlDTSplineNode;
    pDTSplineNode->SetData(m_pDTProp, this, vStartPos, vStartPos, vLookAtPos, DIRECT_LOOKAT_SELF_YAXIS);

    RwMatrixDestroy(pMat);

    m_pDTNode = pDTSplineNode;
}


void CNtlSobCameraSplineNode::EnterPosLookAtActorMode(void)
{
    CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_sLookAtActor.hLookAtSerialId) );
    if(pActor == NULL)
        return;

    RwV3d vStartPos;
    RwV3dAssignMacro(&vStartPos, &m_vStartPos);

    RwMatrix *pMat = RwMatrixCreate();
    RwMatrixSetIdentity(pMat);

    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vXAxisV3, m_fAngleX, rwCOMBINEPOSTCONCAT);		// Pitch
    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vYAxisV3, m_fAngleY, rwCOMBINEPOSTCONCAT);		// Yaw
    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vZAxisV3, m_fAngleZ, rwCOMBINEPOSTCONCAT);		// Roll
    RwMatrixUpdate(pMat);

    RwV3d vLookAtPos;
    vLookAtPos.x = m_vStartPos.x + pMat->at.x*100.0f;
    vLookAtPos.y = m_vStartPos.y + pMat->at.y*100.0f;
    vLookAtPos.z = m_vStartPos.z + pMat->at.z*100.0f;

    CNtlDTSplineNode *pDTSplineNode = NTL_NEW CNtlDTSplineNode;
    pDTSplineNode->SetData(m_pDTProp, this, vStartPos, vStartPos, vLookAtPos, DIRECT_LOOKAT_SELF_YAXIS);

    RwMatrixDestroy(pMat);

    m_pDTNode = pDTSplineNode;
}

void CNtlSobCameraSplineNode::EnterSelfMode(void)
{
    CNtlDTSplineNode *pDTSplineNode = NTL_NEW CNtlDTSplineNode;
    if(m_fAngleX == 0.0f && m_fAngleY == 0.0f && m_fAngleZ == 0.0f)
        pDTSplineNode->SetData(m_pDTProp, this, m_vStartPos, m_vStartPos, m_vStartPos, DIRECT_LOOKAT_SELF);
    else
    {
        RwMatrix *pMat = RwMatrixCreate();
        RwMatrixSetIdentity(pMat);

        RwMatrixRotate (pMat, &CNtlPLGlobal::m_vXAxisV3, m_fAngleX, rwCOMBINEPOSTCONCAT);		// Pitch
        RwMatrixRotate (pMat, &CNtlPLGlobal::m_vYAxisV3, m_fAngleY, rwCOMBINEPOSTCONCAT);			// Yaw
        RwMatrixRotate (pMat, &CNtlPLGlobal::m_vZAxisV3, m_fAngleZ, rwCOMBINEPOSTCONCAT);			// Yaw
        RwMatrixUpdate(pMat);

        RwV3d vLookAtPos;
        vLookAtPos.x = m_vStartPos.x + pMat->at.x*100.0f;
        vLookAtPos.y = m_vStartPos.y + pMat->at.y*100.0f;
        vLookAtPos.z = m_vStartPos.z + pMat->at.z*100.0f;

        if(m_fAngleX == 0.0f && m_fAngleZ == 0.0f)
        {
            pDTSplineNode->SetData(m_pDTProp, this, m_vStartPos, m_vStartPos, vLookAtPos, DIRECT_LOOKAT_SELF_YAXIS);
        }
        else
        {
            pDTSplineNode->SetData(m_pDTProp, this, m_vStartPos, m_vStartPos, vLookAtPos, DIRECT_LOOKAT_SELF_ALLAXIS);
        }

        RwMatrixDestroy(pMat);
    }

    m_pDTNode = pDTSplineNode;
}

void CNtlSobCameraSplineNode::EnterSelfModeActorRef(void)
{
    m_pDTNode = NULL;

    CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_sLookAtActor.hLookAtSerialId) );

    if(pActor)
	{
		m_vStartPos = pActor->GetPosition();
		RwV3d vDir	= pActor->GetDirection();
		m_fAngleY = CNtlMath::LineToAngleY(&vDir);
	}

    CNtlDTSplineNode *pDTSplineNode = NTL_NEW CNtlDTSplineNode;

    RwMatrix *pMat = RwMatrixCreate();
    RwMatrixSetIdentity(pMat);

    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vXAxisV3, m_fAngleX, rwCOMBINEPOSTCONCAT);		// Pitch
    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vYAxisV3, m_fAngleY, rwCOMBINEPOSTCONCAT);			// Yaw
    RwMatrixRotate (pMat, &CNtlPLGlobal::m_vZAxisV3, m_fAngleZ, rwCOMBINEPOSTCONCAT);			// Yaw
    RwMatrixUpdate(pMat);

    RwV3d vLookAtPos;
    vLookAtPos.x = m_vStartPos.x + pMat->at.x*100.0f;
    vLookAtPos.y = m_vStartPos.y + pMat->at.y*100.0f;
    vLookAtPos.z = m_vStartPos.z + pMat->at.z*100.0f;

    pDTSplineNode->SetData(m_pDTProp, this, m_vStartPos, m_vStartPos, vLookAtPos, DIRECT_LOOKAT_SELF_YAXIS);

    RwMatrixDestroy(pMat);

    m_pDTNode = pDTSplineNode;
}


void CNtlSobCameraSplineNode::Enter(void)
{
    m_pDTProp = API_GetDTContainter()->Find(m_strPropKey.c_str());
	if (m_pDTProp == NULL)
	{
		DBO_WARNING_MESSAGE("Could not find prop key: " << m_strPropKey.c_str());
		return;
	}

    if(m_eMode == DT_SPLINE_POS_CAMERA_LOOKAT_ACTOR)
        EnterPosCameraLookAtActorMode();
    else if(m_eMode == DT_SPLINE_POS_ACTOR_LOOKAT_ACTOR)
        EnterPosActorLookAtActorMode();
    else if(m_eMode == DT_SPLINE_POS_ACTOR_LOOKAT_TWO_ACTOR)
        EnterPosActorLookAtTwoActorMode();
    else if(m_eMode == DT_SPLINE_POS_LOOKAT_ACTOR)
        EnterPosLookAtActorMode();
    else if(m_eMode == DT_SPLINE_SELF)
        EnterSelfMode();
    else if(m_eMode == DT_SPLINE_ACTOR_REF)
        EnterSelfModeActorRef();
}

RwBool CNtlSobCameraSplineNode::IsFinish()
{
    if(!CNtlSobCameraNode::IsFinish())
        return FALSE;

    if(m_pDTNode == NULL)
        return TRUE;

    if(m_fCurrTime < m_fLifeTime)
        return FALSE;

    return m_pDTNode->IsFinish();
}

void CNtlSobCameraSplineNode::SetPosition(const RwV3d *pPos)
{
    CNtlSob::SetPosition(pPos);

    GetNtlGameCameraManager()->SetCameraPos(pPos);
}

void CNtlSobCameraSplineNode::SetDirection(const RwV3d *pDir)
{
    CNtlSob::SetDirection(pDir);

    RwV3d vPos = CNtlSob::GetPosition();

    CNtlMath::MathRwV3dAddMul(&vPos, pDir, 1000.0f);

    GetNtlGameCameraManager()->SetLookAt(&vPos);
}


void CNtlSobCameraSplineNode::SetDataPosCameraLookAtActor(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime)
{
    m_eMode								= DT_SPLINE_POS_CAMERA_LOOKAT_ACTOR;
    m_strPropKey						= pPropKey;
    m_sLookAtActor.hLookAtSerialId		= hSerialId;
    m_sLookAtActor.fLookAtHeightOffset	= fHeightOffset;
    m_fLifeTime							= fLifeTime;
}

void CNtlSobCameraSplineNode::SetDataPosActorLookAtActor(SERIAL_HANDLE hPosSerialId, SERIAL_HANDLE hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime)
{
    m_eMode								= DT_SPLINE_POS_ACTOR_LOOKAT_ACTOR;
    m_strPropKey						= pPropKey;
    m_hPosSerialId						= hPosSerialId;
    m_sLookAtActor.hLookAtSerialId		= hLookAtSerialId;
    m_sLookAtActor.fLookAtHeightOffset	= fHeightOffset;
    m_fLifeTime							= fLifeTime;

    m_fAngleX							= 0.0f;
    m_fAngleY							= 0.0f;
    m_fAngleZ							= 0.0f;
}

void CNtlSobCameraSplineNode::SetDataPosActorLookAtTwoActor(SERIAL_HANDLE hPosSerialId, SERIAL_HANDLE hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime)
{
    m_eMode								= DT_SPLINE_POS_ACTOR_LOOKAT_TWO_ACTOR;
    m_strPropKey						= pPropKey;
    m_hPosSerialId						= hPosSerialId;
    m_sLookAtActor.hLookAtSerialId		= hLookAtSerialId;
    m_sLookAtActor.fLookAtHeightOffset	= fHeightOffset;
    m_fLifeTime							= fLifeTime;

    m_fAngleX							= 0.0f;
    m_fAngleY							= 0.0f;
    m_fAngleZ							= 0.0f;
}


void CNtlSobCameraSplineNode::SetDataPosLookAtActor(RwV3d vPos, RwReal fAngleY, SERIAL_HANDLE hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime)
{
    m_eMode								= DT_SPLINE_POS_LOOKAT_ACTOR;
    m_strPropKey						= pPropKey;
    m_vStartPos							= vPos;
    m_sLookAtActor.hLookAtSerialId		= hLookAtSerialId;
    m_sLookAtActor.fLookAtHeightOffset	= fHeightOffset;
    m_fLifeTime							= fLifeTime;

    m_fAngleX							= 0.0f;
    m_fAngleY							= fAngleY;
    m_fAngleZ							= 0.0f;
}

void CNtlSobCameraSplineNode::SetDataSelf(RwV3d vPos, RwReal fAngleX, RwReal fAngleY, RwReal fAngleZ, const RwChar *pPropKey, RwReal fLifeTime)
{
    m_eMode								= DT_SPLINE_SELF;
    m_strPropKey						= pPropKey;
    m_vStartPos							= vPos;
    m_fAngleX							= fAngleX;
    m_fAngleY							= fAngleY;
    m_fAngleZ							= fAngleZ;
    m_fLifeTime							= fLifeTime;
}

void CNtlSobCameraSplineNode::SetDataSelfBeginActorRef(SERIAL_HANDLE hSerialId, const RwChar *pPropKey, RwReal fLifeTime)
{
    m_eMode								= DT_SPLINE_SELF;
    m_strPropKey						= pPropKey;
    m_sLookAtActor.hLookAtSerialId		= hSerialId;
    m_fAngleX							= 0.0f;
    m_fAngleY							= 0.0f;
    m_fAngleZ							= 0.0f;
    m_fLifeTime							= fLifeTime;

    CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(hSerialId) );
    if(pActor)
    {
        m_vStartPos = pActor->GetPosition();
        RwV3d vDir	= pActor->GetDirection();
        m_fAngleY = CNtlMath::LineToAngleY(&vDir);
    }
}

void CNtlSobCameraSplineNode::SetDataSelfActorRef(SERIAL_HANDLE hSerialId, const RwChar *pPropKey, RwReal fLifeTime)
{
	m_eMode								= DT_SPLINE_ACTOR_REF;
    m_strPropKey						= pPropKey;
    m_sLookAtActor.hLookAtSerialId		= hSerialId;
    m_fAngleX							= 0.0f;
    m_fAngleY							= 0.0f;
    m_fAngleZ							= 0.0f;
    m_fLifeTime							= fLifeTime;

	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(hSerialId) );
    if(pActor)
    {
        m_vStartPos = pActor->GetPosition();
        RwV3d vDir	= pActor->GetDirection();
        m_fAngleY = CNtlMath::LineToAngleY(&vDir);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlCameraDTController::CNtlCameraDTController()
{
    SetType(CAMERA_CONTROL_DT);
    SetFlags(NTL_CAMERA_CONTROL_NORMAL_PAUSE | NTL_CAMERA_CONTROL_NOT_ROTATE);

    m_fCurrFov			= GetNtlGameCameraManager()->GetFov();
    m_bAttributeRestore = TRUE;
}

CNtlCameraDTController::~CNtlCameraDTController()
{
    CNtlSobCameraNode *pNode;

    ListNode::iterator it;
    for(it = m_listNode.begin(); it != m_listNode.end(); ++it)
    {
        pNode = (*it);

		NTL_DELETE( pNode );
    }
	m_listNode.clear();

    if(m_bAttributeRestore)
    {
        GetNtlGameCameraManager()->SetRoll(0.0f);
        GetNtlGameCameraManager()->SetFov(m_fCurrFov);
    }
}


void CNtlCameraDTController::Enter(void)
{
}

void CNtlCameraDTController::Exit(void)
{
    Finish();
}

void CNtlCameraDTController::Update(RwReal fElapsed)
{
    if(m_listNode.size() == 0)
    {
		if (!GetNtlGameCameraManager()->IsFinishLock())
		{
			Finish();
		}

        return;
    }

    ListNode::iterator it = m_listNode.begin();
    CNtlSobCameraNode *pNode = (*it);

    pNode->Update(fElapsed);

    if(pNode->IsFinish())
    {
        pNode->Exit();

		NTL_DELETE( pNode );

        m_listNode.erase(it);

        if(m_listNode.size() == 0)
        {
            if(!GetNtlGameCameraManager()->IsFinishLock())
                Finish();
        }
        else
        {
            it = m_listNode.begin();
            pNode = (*it);
            pNode->Enter();
        }
    }
}

void CNtlCameraDTController::UpdateData(RwBool bUpdate /*= TRUE*/)
{
}

void CNtlCameraDTController::SetActiveActor(const CNtlSobActor *pActor)
{
}

void CNtlCameraDTController::AddListNode(CNtlSobCameraNode *pNode, RwBool AttachNode)
{
    if(m_listNode.size() == 0)
    {
        pNode->Enter();
        m_listNode.push_back(pNode);
    }
    else
    {
        if(AttachNode)
        {
            ListNode::reverse_iterator it = m_listNode.rbegin();
            CNtlSobCameraNode *pParentNode = (*it);
            if(m_listNode.size() == 1)
                pNode->Enter();

            pParentNode->AddChildNode(pNode);
        }
        else
            m_listNode.push_back(pNode);
    }
}

void CNtlCameraDTController::SetAttributeRestore(RwBool bRestore)
{
    m_bAttributeRestore = bRestore;
}

void CNtlCameraDTController::AddDTSoftActorDistNode(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY,
                                                    RwReal fDistance, RwReal fLifeTime, RwV3d vLookAt, RwBool bAttachNode, CINEMATIC_HANDLE hCinematic)
{
    CNtlSobCameraActorDist *pSobActorDistNode = NTL_NEW CNtlSobCameraActorDist;
    pSobActorDistNode->SetCinematicInstance(hCinematic);
    pSobActorDistNode->SetData(hSerialId, fHeightOffset, fAngleX, fAngleY, fDistance, fLifeTime, vLookAt);

    AddListNode(pSobActorDistNode, bAttachNode);
}

void CNtlCameraDTController::AddDTSoftActorDynamicLookAtDistNode(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
                                                                 RwReal fDistance, RwReal fLifeTime, RwBool bAttachNode, CINEMATIC_HANDLE hCinematic)
{
    CNtlSobCameraActorDist *pSobActorDistNode = NTL_NEW CNtlSobCameraActorDist;
    pSobActorDistNode->SetCinematicInstance(hCinematic);
    pSobActorDistNode->SetDynamicLookAtData(hSerialId, fHeightOffset, fAngleX, fAngleY, fDistance, fLifeTime);

    AddListNode(pSobActorDistNode, bAttachNode);
}

void CNtlCameraDTController::AddDTSoftCurrentRotateX(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwBool bAttachNode, CINEMATIC_HANDLE hCinematic)
{
    CNtlSobCameraRotate *pSobRotateNode = NTL_NEW CNtlSobCameraRotate;
    pSobRotateNode->SetCinematicInstance(hCinematic);
    pSobRotateNode->SetAxisXData(fLastAngle, fInterTime, fLifeTime);

    AddListNode(pSobRotateNode, bAttachNode);
}

void CNtlCameraDTController::AddDTSoftCurrentRotateY(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwBool bAttachNode, CINEMATIC_HANDLE hCinematic)
{
    CNtlSobCameraRotate *pSobRotateNode = NTL_NEW CNtlSobCameraRotate;
    pSobRotateNode->SetCinematicInstance(hCinematic);
    pSobRotateNode->SetAxisYData(fLastAngle, fInterTime, fLifeTime);

    AddListNode(pSobRotateNode, bAttachNode);
}

void CNtlCameraDTController::AddDTSoftCurrentRotateZ(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwBool bAttachNode, CINEMATIC_HANDLE hCinematic)
{
    CNtlSobCameraRotate *pSobRotateNode = NTL_NEW CNtlSobCameraRotate;
    pSobRotateNode->SetCinematicInstance(hCinematic);
    pSobRotateNode->SetAxisZData(fLastAngle, fInterTime, fLifeTime);

    AddListNode(pSobRotateNode, bAttachNode);
}

void CNtlCameraDTController::AddDTSoftCurrentRotateXYZ(RwReal fLastAngleX, RwReal fLastAngleY, RwReal fLastAngleZ, RwReal fInterTime, RwReal fLifeTime, RwBool bAttachNode, CINEMATIC_HANDLE hCinematic)
{
    CNtlSobCameraRotate *pSobRotateNode = NTL_NEW CNtlSobCameraRotate;
    pSobRotateNode->SetCinematicInstance(hCinematic);
    pSobRotateNode->SetAxisXYZData(fLastAngleX, fLastAngleY, fLastAngleZ, fInterTime, fLifeTime);

    AddListNode(pSobRotateNode, bAttachNode);
}

void CNtlCameraDTController::AddDTSoftCurrentFov(RwReal fLastFov, RwReal fInterTime, RwBool bAttachNode, CINEMATIC_HANDLE hCinematic)
{
    CNtlSobCameraFov *pSobFovNode = NTL_NEW CNtlSobCameraFov;
    pSobFovNode->SetCinematicInstance(hCinematic);
    pSobFovNode->SetFovData(fLastFov, fInterTime);

    AddListNode(pSobFovNode, bAttachNode);
}


void CNtlCameraDTController::AddDTSoftPauseNode(RwReal fLifeTime, RwBool bLoop, RwBool AttachNode, CINEMATIC_HANDLE hCinematic)
{
    CNtlSobCameraPauseNode *pSobPauseNode = NTL_NEW CNtlSobCameraPauseNode;
    pSobPauseNode->SetCinematicInstance(hCinematic);
    pSobPauseNode->SetData(fLifeTime, bLoop);

    AddListNode(pSobPauseNode, AttachNode);

    if(bLoop)
    {
        m_listLoopPause.push_back(pSobPauseNode);
    }
}


void CNtlCameraDTController::AddDTSplineCurrentDynamicLookAtNode(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, const RwChar *pPropKey, 
                                                                 RwReal fLifeTime, RwBool bAttach, CINEMATIC_HANDLE hCinematic)
{
    CNtlSobCameraSplineNode *pSobSplineNode = NTL_NEW CNtlSobCameraSplineNode;
    pSobSplineNode->SetCinematicInstance(hCinematic);
    pSobSplineNode->SetDataPosCameraLookAtActor(hSerialId, fHeightOffset, pPropKey, fLifeTime);

    AddListNode(pSobSplineNode, bAttach);
}

void CNtlCameraDTController::AddDTSplineActorPositionDynamicLookAtNode(SERIAL_HANDLE hPosSerialId, SERIAL_HANDLE hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, RwBool bAttach, CINEMATIC_HANDLE hCinematic)
{
    CNtlSobCameraSplineNode *pSobSplineNode = NTL_NEW CNtlSobCameraSplineNode;
    pSobSplineNode->SetCinematicInstance(hCinematic);
    pSobSplineNode->SetDataPosActorLookAtActor(hPosSerialId, hLookAtSerialId, fHeightOffset, pPropKey, fLifeTime);

    AddListNode(pSobSplineNode, bAttach);
}

void CNtlCameraDTController::AddDTSplineActorPositionDynamicLookAtCenterNode(SERIAL_HANDLE hPosSerialId, SERIAL_HANDLE hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, RwBool bAttach, CINEMATIC_HANDLE hCinematic)
{
    CNtlSobCameraSplineNode *pSobSplineNode = NTL_NEW CNtlSobCameraSplineNode;
    pSobSplineNode->SetCinematicInstance(hCinematic);
    pSobSplineNode->SetDataPosActorLookAtTwoActor(hPosSerialId, hLookAtSerialId, fHeightOffset, pPropKey, fLifeTime);

    AddListNode(pSobSplineNode, bAttach);
}

void CNtlCameraDTController::AddDTSplinePositionDynamicLookAtNode(RwV3d vPos, RwReal fAngleY, SERIAL_HANDLE hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, RwBool bAttach, CINEMATIC_HANDLE hCinematic)
{
    CNtlSobCameraSplineNode *pSobSplineNode = NTL_NEW CNtlSobCameraSplineNode;
    pSobSplineNode->SetCinematicInstance(hCinematic);
    pSobSplineNode->SetDataPosLookAtActor(vPos, fAngleY, hLookAtSerialId, fHeightOffset, pPropKey, fLifeTime);

    AddListNode(pSobSplineNode, bAttach);
}

void CNtlCameraDTController::AddSplineSelfNode(RwV3d vPos, RwReal fAngleX, RwReal fAngleY, RwReal fAngleZ, const RwChar *pPropKey, RwBool bAttach, CINEMATIC_HANDLE hCinematic)
{
    CNtlSobCameraSplineNode *pSobSplineNode = NTL_NEW CNtlSobCameraSplineNode;
    pSobSplineNode->SetCinematicInstance(hCinematic);
    pSobSplineNode->SetDataSelf(vPos, fAngleX, fAngleY, fAngleZ, pPropKey, 0.0f);

    AddListNode(pSobSplineNode, bAttach);
}


void CNtlCameraDTController::AddSplineSelfNodeBeginActorRef(SERIAL_HANDLE hSerialId, const RwChar *pPropKey, RwBool bAttach, CINEMATIC_HANDLE hCinematic)
{
    CNtlSobCameraSplineNode *pSobSplineNode = NTL_NEW CNtlSobCameraSplineNode;
    pSobSplineNode->SetCinematicInstance(hCinematic);
    pSobSplineNode->SetDataSelfBeginActorRef(hSerialId, pPropKey, 0.0f);

    AddListNode(pSobSplineNode, bAttach);
}

void CNtlCameraDTController::AddSplineSelfNodeActorRef(SERIAL_HANDLE hSerialId, const RwChar *pPropKey, RwBool bAttach, CINEMATIC_HANDLE hCinematic)
{
	CNtlSobCameraSplineNode *pSobSplineNode = NTL_NEW CNtlSobCameraSplineNode;
    pSobSplineNode->SetCinematicInstance(hCinematic);
    pSobSplineNode->SetDataSelfActorRef(hSerialId, pPropKey, 0.0f);

    AddListNode(pSobSplineNode, bAttach);
}


void CNtlCameraDTController::ClearLoopPauseNode(void)
{
    ListPauseNode::iterator it;
    for(it = m_listLoopPause.begin(); it != m_listLoopPause.end(); it++)
    {
        (*it)->SetData(0.0f, FALSE);
    }

    m_listLoopPause.clear();
}

//////////////////////////////////////////////////////////////////////////
RwReal CNtlFreeCameraController::m_fMoveSpeed = 0.2f;

CNtlFreeCameraController::CNtlFreeCameraController() 
{
    SetType(CAMERA_CONTROL_FREE);
    SetFlags(NTL_CAMERA_CONTROL_NORMAL_PAUSE);

    m_vPrevMoveFlag = NTL_MOVE_NONE;
    m_fPrevSpeed = 0.0f;
}

CNtlFreeCameraController::~CNtlFreeCameraController() 
{

}

void CNtlFreeCameraController::Enter( void ) 
{
    sWORLD_TBLDAT *pWorldTblData = (sWORLD_TBLDAT*)API_GetTableContainer()->GetWorldTable()->FindData(Logic_GetActiveWorldTableId());
    m_box.sup.x = pWorldTblData->vStart.x;
    m_box.sup.z = pWorldTblData->vStart.z;
    m_box.sup.y = pWorldTblData->fFreeCamera_Height;
    m_box.inf.x = pWorldTblData->vEnd.x;
    m_box.inf.z = pWorldTblData->vEnd.z;
    m_box.inf.y = -RwRealMAXVAL;    
}

void CNtlFreeCameraController::Exit( void ) 
{
    Finish();
}

void CNtlFreeCameraController::Update( RwReal fElapsed ) 
{
    RwV3d vCamPos = *GetNtlGameCameraManager()->GetCameraPos();    
    UpdateRotate(vCamPos);

    if(m_vPrevMoveFlag != NTL_MOVE_NONE)
    {
        Move(m_vPrevMoveFlag, m_fPrevSpeed);
    }    
}

void CNtlFreeCameraController::Move(RwUInt32 moveType, RwReal fSpeed)
{
    RwV3d vCamPos = *GetNtlGameCameraManager()->GetCameraPos();
    RwV3d vCamLookAt = GetNtlGameCameraManager()->GetLookAt();    
    RwV3d vCamLookVec = vCamLookAt - vCamPos;
    RwV3dNormalize(&vCamLookVec, &vCamLookVec);

	// do we have to do something here? by daneos
    switch(moveType)
    {
    case NTL_MOVE_F:
        vCamPos += vCamLookVec * fSpeed;
        vCamLookAt += vCamLookVec * fSpeed;
        break;
    case NTL_MOVE_B:
        vCamPos -= vCamLookVec * fSpeed;
        vCamLookAt -= vCamLookVec * fSpeed;
        break;
    case NTL_MOVE_TURN_L:
        {
            RwV3d vLeft;
            RwV3dCrossProduct(&vLeft, &YAxis, & vCamLookVec);
            RwV3dNormalize(&vLeft, &vLeft);
            vCamPos += vLeft * fSpeed;
            vCamLookAt += vLeft * fSpeed;
        }
        break;
    case NTL_MOVE_TURN_R:
        {
            RwV3d vLeft;
            RwV3dCrossProduct(&vLeft, &YAxis, & vCamLookVec);
            RwV3dNormalize(&vLeft, &vLeft);
            vCamPos -= vLeft * fSpeed;
            vCamLookAt -= vLeft * fSpeed;            
        }
        break;
    }

    if(!RwBBoxContainsPoint(&m_box, &vCamPos))
    {
        vCamPos = *GetNtlGameCameraManager()->GetCameraPos();
    }

    m_vPrevMoveFlag = moveType;
    m_fPrevSpeed = fSpeed;

    GetNtlGameCameraManager()->SetCameraPos(&vCamPos);    
    GetNtlGameCameraManager()->SetLookAt(&vCamLookAt);    
}

void CNtlFreeCameraController::Stop() 
{
    m_fPrevSpeed = 0.0f;
    m_vPrevMoveFlag = NTL_MOVE_NONE;
}

void CNtlFreeCameraController::UpdateRotate(const RwV3d& vCamPos)
{
    RwReal fPitch = GetNtlGameCameraManager()->GetPitch();
    RwReal fYaw = GetNtlGameCameraManager()->GetYaw();
    RwV3d vLookAtVec = ZAxis;

    RwMatrixSetIdentity(&m_matTrans);        
    RwMatrixRotate (&m_matTrans, &CNtlPLGlobal::m_vXAxisV3, fPitch, rwCOMBINEPOSTCONCAT);		// Pitch
    RwMatrixRotate (&m_matTrans, &CNtlPLGlobal::m_vYAxisV3, fYaw, rwCOMBINEPOSTCONCAT);			// Yaw
    RwMatrixUpdate(&m_matTrans);

    RwV3dTransformVector(&vLookAtVec, &vLookAtVec, &m_matTrans);

    RwV3d vLookAt = vCamPos + vLookAtVec;

    GetNtlGameCameraManager()->SetLookAt(&vLookAt);
}

//////////////////////////////////////////////////////////////////////////
CNtlCameraTimeMachineController::CNtlCameraTimeMachineController() 
{
    SetType(CAMERA_CONTROL_TIMEMACHINE);

    m_eState        = CAMERA_TM_STATE_NONE;
    m_pSeqPos       = NULL;
    m_pSeqTarget    = NULL;    

    AddCameraData(0.0f, -7.848f, 5.08f, 10.895f, 0.0f, 1.982f, 0.0f, &m_vecCameraData);
    AddCameraData(6.2f, -7.335f, 9.102f, 29.443f, 0.0f, 1.982f, 0.0f, &m_vecCameraData);
    AddCameraData(6.21f, 8.5f, 2.874f, 13.144f, 0.0f, 6.741f, 0.0f, &m_vecCameraData);
    AddCameraData(7.83f, 8.5f, 2.874f, 13.144f, 0.0f, 10.465f, 0.0f, &m_vecCameraData);
    AddCameraData(7.84f, -22.86f, 2.925f, 16.757f, 0.0f, 11.239f, -1.236f, &m_vecCameraData);
    AddCameraData(12.23f, -17.172f, 4.017f, 16.413f, 0.0f, 11.239f, -3.782f, &m_vecCameraData);
    CreateSequenceData(&m_pSeqPos, &m_pSeqTarget, &m_vecCameraData);
}

CNtlCameraTimeMachineController::~CNtlCameraTimeMachineController() 
{
    CNtlTimeSequence *pTempSeq;
    CNtlTimeSequence *pSeq = m_pSeqPos;
    while(pSeq)
    {
        pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );

        pSeq = pTempSeq;
    }

    pSeq = m_pSeqTarget;
    while(pSeq)
    {
        pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );
		
		pSeq = pTempSeq;
    }

    m_pSeqPos = NULL;
    m_pSeqTarget = NULL;
}

void CNtlCameraTimeMachineController::Enter( void ) 
{
    CNtlPLEventGenerator::CinematicViewShow(TRUE);
}

void CNtlCameraTimeMachineController::Exit( void ) 
{
    CNtlPLEventGenerator::CinematicViewShow(FALSE);
    ChangeState(CAMERA_TM_STATE_END);
}

void CNtlCameraTimeMachineController::Update( RwReal fElapsed ) 
{
    m_fTime += fElapsed;

    if(m_eState == CAMERA_TM_STATE_START)
        UpdateStart(fElapsed);
    else if(m_eState == CAMERA_TM_STATE_END)
        UpdateEnd(fElapsed);
}

void CNtlCameraTimeMachineController::SetActiveActor( const CNtlSobActor *pActor ) 
{
    if(pActor == NULL)
    {
        Finish();
        return;
    }

    CNtlCameraFromArtData::SetActiveActor(pActor);

    ChangeState(CAMERA_TM_STATE_START);
}

void CNtlCameraTimeMachineController::UpdateStart( RwReal fElapsed ) 
{
    RwV3d vPos = *(RwV3d*)m_pSeqPos->GetData(m_fTime);
    RwV3d vTarget = *(RwV3d*)m_pSeqTarget->GetData(m_fTime);

    UpdateCameraTrans(&vPos, &vTarget);
}

void CNtlCameraTimeMachineController::UpdateEnd( RwReal fElspase ) 
{
    Finish();
}

void CNtlCameraTimeMachineController::ChangeState( RwInt32 iState ) 
{
    m_eState = (ECameraTMState)iState;
    m_fTime = 0.0f;
}

//////////////////////////////////////////////////////////////////////////
CNtlSobCameraExplosion::CNtlSobCameraExplosion()
{
    SetType(CAMERA_CONTROL_EXPLOSION);

    m_pActor = NULL;
}

CNtlSobCameraExplosion::~CNtlSobCameraExplosion()
{
}

void CNtlSobCameraExplosion::SetActiveActor(const CNtlSobActor *pActor)
{
    m_pActor = (CNtlSobActor*)pActor;
}

void CNtlSobCameraExplosion::Enter(void)
{
}

void CNtlSobCameraExplosion::Exit(void)
{
    m_defAlgorithmLayerList.clear();

    Finish();
}

void CNtlSobCameraExplosion::Update(RwReal fElapsed)
{
    if ( m_defAlgorithmLayerList.empty() )
    {
        Exit();
    }
    else
    {
        mapdef_AlgorithmLayerList::iterator itBegin = m_defAlgorithmLayerList.begin();
        CExplosionAlgorithm_Layer& clLayer = itBegin->second;

        bool bActive = clLayer.Update( fElapsed );

        RwMatrix matResult;
        RwMatrixSetIdentity( &matResult );

        clLayer.GetResult( matResult );

        RwMatrixUpdate( &matResult );

        UpdateCamera( matResult );

        if ( !bActive )
        {
            m_defAlgorithmLayerList.erase( itBegin );
        }
    }
}


void CNtlSobCameraExplosion::AddSinExplosionAlgorithm_X( RwUInt32 uiExcuteID, RwReal fLifeTime, RwReal fFrequancy, RwReal fPhase, RwReal fAmp, RwReal fDescFactor )
{
    if ( m_defAlgorithmLayerList.find( uiExcuteID ) == m_defAlgorithmLayerList.end() )
    {
        m_defAlgorithmLayerList.insert( mapdef_AlgorithmLayerList::value_type( uiExcuteID, CExplosionAlgorithm_Layer() ) );
    }

    CExplosionAlgorithm_Layer& clLayer = m_defAlgorithmLayerList[uiExcuteID];

    CExplosionAlgorithm_Sin* pAlgorithm = NTL_NEW CExplosionAlgorithm_Sin( EXPLOSION_AXIS_TYPE_X );
    pAlgorithm->fLifeTime	= fLifeTime;
    pAlgorithm->fFrequancy	= fFrequancy;
    pAlgorithm->fPhase		= fPhase;
    pAlgorithm->fAmp		= fAmp;
    pAlgorithm->fDescFactor	= fDescFactor;

    clLayer.AddAlgorithm( pAlgorithm );
}


void CNtlSobCameraExplosion::AddSinExplosionAlgorithm_Y( RwUInt32 uiExcuteID, RwReal fLifeTime, RwReal fFrequancy, RwReal fPhase, RwReal fAmp, RwReal fDescFactor )
{
    if ( m_defAlgorithmLayerList.find( uiExcuteID ) == m_defAlgorithmLayerList.end() )
    {
        m_defAlgorithmLayerList.insert( mapdef_AlgorithmLayerList::value_type( uiExcuteID, CExplosionAlgorithm_Layer() ) );
    }

    CExplosionAlgorithm_Layer& clLayer = m_defAlgorithmLayerList[uiExcuteID];

    CExplosionAlgorithm_Sin* pAlgorithm = NTL_NEW CExplosionAlgorithm_Sin( EXPLOSION_AXIS_TYPE_Y );
    pAlgorithm->fLifeTime	= fLifeTime;
    pAlgorithm->fFrequancy	= fFrequancy;
    pAlgorithm->fPhase		= fPhase;
    pAlgorithm->fAmp		= fAmp;
    pAlgorithm->fDescFactor	= fDescFactor;

    clLayer.AddAlgorithm( pAlgorithm );
}


void CNtlSobCameraExplosion::AddSinExplosionAlgorithm_Z( RwUInt32 uiExcuteID, RwReal fLifeTime, RwReal fFrequancy, RwReal fPhase, RwReal fAmp, RwReal fDescFactor )
{
    if ( m_defAlgorithmLayerList.find( uiExcuteID ) == m_defAlgorithmLayerList.end() )
    {
        m_defAlgorithmLayerList.insert( mapdef_AlgorithmLayerList::value_type( uiExcuteID, CExplosionAlgorithm_Layer() ) );
    }

    CExplosionAlgorithm_Layer& clLayer = m_defAlgorithmLayerList[uiExcuteID];

    CExplosionAlgorithm_Sin* pAlgorithm = NTL_NEW CExplosionAlgorithm_Sin( EXPLOSION_AXIS_TYPE_Z );
    pAlgorithm->fLifeTime	= fLifeTime;
    pAlgorithm->fFrequancy	= fFrequancy;
    pAlgorithm->fPhase		= fPhase;
    pAlgorithm->fAmp		= fAmp;
    pAlgorithm->fDescFactor	= fDescFactor;

    clLayer.AddAlgorithm( pAlgorithm );
}


void CNtlSobCameraExplosion::UpdateCamera( RwMatrix& matResult )
{
    if ( NULL == m_pActor )
    {
        return;
    }

    RwFrame* pCameraFrame = RwCameraGetFrame( GetNtlGameCameraManager()->GetCamera()->GetCamera() );
    if ( NULL == pCameraFrame )
    {
        return;
    }

    RwMatrix* pCameraMatrix = RwFrameGetMatrix( pCameraFrame );
    if ( NULL == pCameraMatrix )
    {
        return;
    }

    RwV3d v3AvatarPos;
    CNtlSobAvatar* pAvatar = CNtlSLGlobal::GetInstance()->GetSobAvatar();
    if ( NULL != pAvatar )
    {
        v3AvatarPos = pAvatar->GetPosition();
    }
    else
    {
        v3AvatarPos = pCameraMatrix->pos;
    }

    RwV3d v3ActorPos = m_pActor->GetPosition();
    RwV3d v3CamPos = pCameraMatrix->pos;

    // 폭발 영향 반경 밖에 존재하는 경우 폭발 시키지 않는다
    RwReal fDist = sqrtf( (v3AvatarPos.x - v3ActorPos.x) * (v3AvatarPos.x - v3ActorPos.x) +
        (v3AvatarPos.z - v3ActorPos.z) * (v3AvatarPos.z - v3ActorPos.z) );

    RwReal fDistFactor = 1.f;

    if ( !GetNtlGameCameraManager()->FindController( CAMERA_CONTROL_DT ) )
    {
        if ( fDist <= EXPLOSION_MIN_DISTANCE )
        {
            // 최소 영역 안에 존재하는 경우는 최대로 폭발시킨다
            fDistFactor = 1.f;
        }
        else if ( fDist >= EXPLOSION_MAX_DISTANCE )
        {
            // 최대 영역 밖에 존재하는 경우는 폭발시키지 않는다
            return;
        }
        else
        {
            fDistFactor = 1.f - (fDist - EXPLOSION_MIN_DISTANCE) / (EXPLOSION_MAX_DISTANCE - EXPLOSION_MIN_DISTANCE );
        }
    }

    // 회전 매트릭스 계산
    RwMatrix matRotate;
    RwMatrixSetIdentity( &matRotate );

    RwV3d v3Dir;
    v3Dir.x = v3CamPos.x - v3ActorPos.x;
    v3Dir.y = 0.f;
    v3Dir.z = v3CamPos.z - v3ActorPos.z;
    RwV3dNormalize( &matRotate.at, &v3Dir );

    RwV3dCrossProduct( &matRotate.right, &matRotate.up, &matRotate.at );

    // Explosion offset 값 회전
    RwV3d v3Offset;
    RwV3dTransformPoint( &v3Offset, &matResult.pos, &matRotate );

    // 폭발 거리에 따른 가감
    RwV3dScale( &v3Offset, &v3Offset, fDistFactor );

    GetNtlGameCameraManager()->AddCameraOffset( &v3Offset );
    GetNtlGameCameraManager()->AddCameraLookAtOffset( &v3Offset );
}


//////////////////////////////////////////////////////////////////////////
CNtlCameraTimeMachineArriveController::CNtlCameraTimeMachineArriveController( void )
{
    SetType( CAMERA_CONTROL_TIMEMACHINE_ARRIVE );
    SetFlags( NTL_CAMERA_CONTROL_NORMAL_PAUSE );

    m_eState = eTMARRIVE_STATE_INVALID;
    m_pTimemachineActor = NULL;

    m_fOldFOV = 60.f;

    m_v3Pos.x = m_v3Pos.y = m_v3Pos.z = 0.f;
    m_v3Target.x = m_v3Target.y = m_v3Target.z = 0.f;
    m_fFOV = 60.f;
}


CNtlCameraTimeMachineArriveController::~CNtlCameraTimeMachineArriveController( void )
{
}


void CNtlCameraTimeMachineArriveController::Enter( void )
{
    SetVisibleAllObject(FALSE);
}


void CNtlCameraTimeMachineArriveController::Exit( void )
{
    SetVisibleAllObject(TRUE);
    Finish();
}


void CNtlCameraTimeMachineArriveController::Update( RwReal fElapsed )
{
    SetVisibleAllObject(FALSE);

    switch ( m_eState )
    {
    case eTMARRIVE_STATE_START:
        {
            CNtlSobAvatar* pAvatar = CNtlSLGlobal::GetInstance()->GetSobAvatar();
            if ( NULL == pAvatar )
            {
                return;
            }

            if ( NULL == m_pTimemachineActor )
            {
                return;
            }

            // Backup //////////////////////////////////////////////////////////////////////////
            m_fOldFOV = GetNtlGameCameraManager()->GetFov();

            // Setting /////////////////////////////////////////////////////////////////////////
            m_v3Pos.x = 26.471f;
            m_v3Pos.y = 16.915f;
            m_v3Pos.z = 14.553f;

            m_v3Target.x = 0.f;
            m_v3Target.y = 6.421f;
            m_v3Target.z = 4.3f;

            m_fFOV = 50.f;

            // Calculate ///////////////////////////////////////////////////////////////////////

            RwV3d v3TMDir = m_pTimemachineActor->GetDirection();
            RwV3d v3TMPos = m_pTimemachineActor->GetPosition();

            // Time machine의 메트릭스 계산
            RwMatrix matTMRotate;
            CNtlMath::MathRwMatrixAssign( &matTMRotate, &v3TMPos, &v3TMDir );

            // 로컬 카메라 위치 및 방향 정보 회전
            RwV3d v3Temp;

            v3Temp = m_v3Pos;
            RwV3dTransformPoint( &v3Temp, &m_v3Pos, &matTMRotate );
            m_v3Pos = v3Temp;

            v3Temp = m_v3Target;
            RwV3dTransformPoint( &v3Temp, &m_v3Target, &matTMRotate );
            m_v3Target = v3Temp;

            Run();
        }
        break;

    case eTMARRIVE_STATE_RUN:
        {
            GetNtlGameCameraManager()->SetCameraPos( &m_v3Pos );
            GetNtlGameCameraManager()->SetLookAt( &m_v3Target );
            GetNtlGameCameraManager()->SetFov( m_fFOV );
        }
        break;

    case eTMARRIVE_STATE_END:
        {
            GetNtlGameCameraManager()->SetFov( m_fOldFOV );

            Exit();
        }
        break;
    }
}


void CNtlCameraTimeMachineArriveController::SetActiveActor( const CNtlSobActor *pActor )
{
    m_pTimemachineActor = const_cast<CNtlSobActor*>( pActor );
}


void CNtlCameraTimeMachineArriveController::Start()
{
    m_eState = eTMARRIVE_STATE_START;
}


void CNtlCameraTimeMachineArriveController::End()
{
    m_eState = eTMARRIVE_STATE_END;
}


void CNtlCameraTimeMachineArriveController::Run( void )
{
    m_eState = eTMARRIVE_STATE_RUN;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CNtlCameraKnockDownMatrixController::CNtlCameraKnockDownMatrixController()
{
    m_hTargetSerialId	= INVALID_SERIAL_ID;
    m_fTargetHeight		= 1.0f;
    m_fTargetRound		= 1.0f;
    m_fZoomSpeed		= 50.0f;
    m_fCurrTime			= 0.0f;
    m_fLifeTime			= 1.0f;

    m_byBlurState		= ECKDM_BLUR_NONE;
    m_fBlurTime			= 0.0f;

    SetType(CAMERA_CONTROL_KNOCKDOWN_MATRIX);
    SetFlags(NTL_CAMERA_CONTROL_NORMAL_PAUSE);
}

CNtlCameraKnockDownMatrixController::~CNtlCameraKnockDownMatrixController()
{
    GetNtlGameCameraManager()->GetCamera()->SetEnableBlur(FALSE);
    GetNtlGameCameraManager()->SetCollisionCheck(TRUE);
}

void CNtlCameraKnockDownMatrixController::UpdateZoom(RwReal fElapsed)
{
    CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_hTargetSerialId);
    if(pSobObj == NULL)
    {
        Finish();
        return;
    }

    RwV3d vCamPos;
    RwV3d vLookAt;

    const RwV3d *pCamPos = GetNtlGameCameraManager()->GetCameraPos();
    RwV3d vTarPos = pSobObj->GetPosition();
    vTarPos.y += m_fTargetHeight;

    RwV3d vSub;
    RwV3dSubMacro(&vSub, &vTarPos, pCamPos);
    RwReal fCurrLen = RwV3dLength(&vSub);

    RwV3d vDir;
    RwV3dNormalize(&vDir, &vSub);

    RwV3d vDelta;
    CNtlMath::MathRwV3dAssign(&vDelta, vDir.x * m_fZoomSpeed, vDir.y * m_fZoomSpeed, vDir.z * m_fZoomSpeed);

    RwReal fNextLen = RwV3dLength(&vDelta);

    if(fCurrLen - m_fTargetRound <= fNextLen)
    {
        CNtlMath::MathRwV3dAssign(&vCamPos, 
            vTarPos.x - vDir.x * m_fTargetRound, 
            vTarPos.y - vDir.y * m_fTargetRound, 
            vTarPos.z - vDir.z * m_fTargetRound);

        m_eState = ECKDM_STATE_LOOK;
    }
    else
    {
        CNtlMath::MathRwV3dAssign(&vCamPos, pCamPos->x + vDelta.x, pCamPos->y + vDelta.y, pCamPos->z + vDelta.z);
    }

    RwV3dSubMacro(&vSub, &vTarPos, &vCamPos);
    RwV3dNormalize(&vDir, &vSub);

    CNtlMath::MathRwV3dAssign(&vLookAt, vCamPos.x * vDir.x * 100.0f , vCamPos.y * vDir.y * 100.0f, vCamPos.z * vDir.z * 100.0f);

    GetNtlGameCameraManager()->SetLookAt(&vLookAt);
    GetNtlGameCameraManager()->SetCameraPos(&vCamPos);
}

void CNtlCameraKnockDownMatrixController::UpdateLook(RwReal fElasped)
{
    CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_hTargetSerialId);
    if(pSobObj == NULL)
    {
        Finish();
        return;
    }

    RwV3d vDir, vLookAt;

    const RwV3d *pCamPos = GetNtlGameCameraManager()->GetCameraPos();
    RwV3d vTarPos = pSobObj->GetPosition();
    vTarPos.y += m_fTargetHeight;

    RwV3dSubMacro(&vDir, &vTarPos, pCamPos);
    RwV3dNormalize(&vDir, &vDir);

    CNtlMath::MathRwV3dAssign(&vLookAt, pCamPos->x + vDir.x * 100.0f , pCamPos->y + vDir.y * 100.0f, pCamPos->z + vDir.z * 100.0f);

    GetNtlGameCameraManager()->SetLookAt(&vLookAt);
}

void CNtlCameraKnockDownMatrixController::Update( RwReal fElapsed )
{
    m_fCurrTime += fElapsed;
    if(m_fCurrTime >= m_fLifeTime)
    {
        Finish();
        return;
    }

    if(m_byBlurState == ECKDM_BLUR_ENTER)
    {
        GetNtlGameCameraManager()->GetCamera()->SetEnableBlur(TRUE, 0.3f);
        m_byBlurState = ECKDM_BLUR_LOOP;
    }
    else if(m_byBlurState == ECKDM_BLUR_LOOP)
    {
        m_fBlurTime += fElapsed;
        if(m_fBlurTime > 0.5f)
        {
            GetNtlGameCameraManager()->GetCamera()->SetEnableBlur(FALSE);
            m_byBlurState = ECKDM_BLUR_END;
        }
    }

    if(m_eState == ECKDM_STATE_ZOOM)
    {
        UpdateZoom(fElapsed);
    }
    else
    {
        UpdateLook(fElapsed);
    }
}

void CNtlCameraKnockDownMatrixController::Enter( void )
{
    m_eState = ECKDM_STATE_ZOOM;

    GetNtlGameCameraManager()->SetCollisionCheck(FALSE);
	CNtlSoundEventGenerator::SendKnockdown(TRUE);
}

void CNtlCameraKnockDownMatrixController::Exit( void )
{
	CNtlSoundEventGenerator::SendKnockdown(FALSE);
}

void CNtlCameraKnockDownMatrixController::SetActiveActor(const CNtlSobActor *pActor)
{
    if(pActor)
    {
        CNtlSobProxy *pSobProxy = pActor->GetSobProxy();
        m_hTargetSerialId	= pActor->GetSerialID();
        m_fTargetHeight		= pSobProxy->GetPLEntityHeight() * 0.45f;
        m_fTargetRound		= (pSobProxy->GetPLEntityWidth() + pSobProxy->GetPLEntityDepth()) / 2.0f + 3.0f;

        CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_hTargetSerialId);
        if(pSobObj)
        {
            const RwV3d *pCamPos = GetNtlGameCameraManager()->GetCameraPos();
            RwV3d vTarPos = pSobObj->GetPosition();
            vTarPos.y += m_fTargetHeight;

            RwV3d vSub;
            RwV3dSubMacro(&vSub, &vTarPos, pCamPos);
            RwReal fCurrLen = RwV3dLength(&vSub);

            if(fCurrLen < m_fTargetRound)
            {
                m_eState = ECKDM_STATE_LOOK;
            }

            m_byBlurState = ECKDM_BLUR_ENTER;
            m_fZoomSpeed += fCurrLen;
        }
    }
}

void CNtlCameraKnockDownMatrixController::SetData(RwReal fLifeTime)
{
    m_fLifeTime = fLifeTime;
}

//////////////////////////////////////////////////////////////////////////
CNtlCameraFPSController::CNtlCameraFPSController() 
{
    SetType(CAMERA_CONTROL_FPS);
    SetFlags(NTL_CAMERA_CONTROL_NORMAL_PAUSE | NTL_CAMERA_CONTROL_NOT_ROTATE);

    m_pActor = NULL;
    m_bEnableZoom = FALSE;

    m_vStartPos = m_vTargetPos = ZeroAxis;
    m_vCurrLookAt = m_vTargetLookAt = ZeroAxis;
    m_bInter = TRUE;
    m_fInterTime = 0.0f;
    m_fOrgPitch = m_fOrgYaw = 0.0f;
    m_uiMoveFlag = NTL_MOVE_NONE;
}

CNtlCameraFPSController::~CNtlCameraFPSController() 
{

}

void CNtlCameraFPSController::Enter() 
{
    m_vStartPos = *GetNtlGameCameraManager()->GetCameraPos();
    m_fOldDist = GetNtlGameCameraManager()->GetDist();
    //m_fOrgPitch = GetNtlGameCameraManager()->GetPitch();
    //m_fOrgYaw = GetNtlGameCameraManager()->GetYaw();
    m_bInter = TRUE;
    m_uiMoveFlag = NTL_MOVE_NONE;

    // 카메라 위치
    m_vTargetPos = m_pActor->GetPosition();
    if(m_pActor->GetSobProxy()->GetBaseAnimationKey() == SWIM_IDLE)
    {
        // NOTE: 물속에서는 카메라가 낮으면 물이 렌더링안된다. 그래서 높이를 좀더 높였다(by agebreak)
        m_vTargetPos.y += m_pActor->GetSobProxy()->GetPLEntityHeight() * 1.3f;      
    }
    else
    {
        m_vTargetPos.y += m_pActor->GetSobProxy()->GetPLEntityHeight() * 0.8f;
    }    
}

void CNtlCameraFPSController::Exit() 
{
    Finish();

    GetNtlGameCameraManager()->SetDist( m_fOldDist );
}

void CNtlCameraFPSController::Update( RwReal fElapsed ) 
{
    if ( IsFinish() )
        return;

    if(!m_pActor)
        return;

    switch(m_uiMoveFlag)
    {
    case NTL_MOVE_TURN_L:
        AddYaw(2.0f);
        break;
    case NTL_MOVE_TURN_R:
        AddYaw(-2.0f);
        break;
    }

    RwV3d vCamPos, vLookAt;    

    if(m_bInter)
    {
        m_fInterTime += fElapsed;

        if(m_fInterTime >= FPS_INTERPOLATION_TIME)
        {
            m_fInterTime = 0.0f;
            m_bInter = FALSE;

            vCamPos = m_vTargetPos;
            vLookAt = m_vTargetLookAt;
        }
        else
        {
            RwReal fDeltaTime = min(m_fInterTime / FPS_INTERPOLATION_TIME, 1.0f);

            // 위치 보간
            vCamPos = CNtlMath::Interpolation(m_vStartPos, m_vTargetPos, fDeltaTime);

            // LookAt 보간
            vLookAt = CNtlMath::Interpolation(m_vCurrLookAt, m_vTargetLookAt, fDeltaTime);            
        }        
    }
    else
    {
        vCamPos = m_vTargetPos;
        vLookAt = m_vTargetLookAt - vCamPos;   
        RwV3dNormalize(&vLookAt, &vLookAt);
        vLookAt.y = -tan(NTL_DEG2RAD(m_fOrgPitch));


        // Look At               
        RwMatrixSetIdentity(&m_matTrans);                
        RwMatrixRotate (&m_matTrans, &CNtlPLGlobal::m_vYAxisV3, m_fOrgYaw, rwCOMBINEPOSTCONCAT);			// Yaw
        RwMatrixUpdate(&m_matTrans);
        RwV3dTransformVector(&vLookAt, &vLookAt, &m_matTrans);
        vLookAt += vCamPos;
    }

    // Distance
    RwV3d vDist = vCamPos - m_vTargetPos;
    RwReal fCurDist = RwV3dLength( &vDist );

    GetNtlGameCameraManager()->SetDist( fCurDist );
    GetNtlGameCameraManager()->SetLookAt(&vLookAt);
    GetNtlGameCameraManager()->SetCameraPos(&vCamPos);
}

void CNtlCameraFPSController::SetActiveActor( const CNtlSobActor *pActor ) 
{
    if(!pActor)
        return;

    m_pActor = (CNtlSobActor*)pActor;    
}

void CNtlCameraFPSController::ZoomInOut( RwReal fDelta ) 
{
    if(!m_bEnableZoom)
        return;

    if(fDelta > 0)
    {
        CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_FPS);       // 줌을 시키면 FPS카메라에서 빠진다.
        GetNtlGameCameraManager()->SetDist(GetNtlGameCameraManager()->GetMinDist() + 0.1f);
    }    
}

void CNtlCameraFPSController::EnableZoomInOut( RwBool bEnable ) 
{
    m_bEnableZoom = bEnable;
    m_bInter = !bEnable;
}

void CNtlCameraFPSController::SetLookAtDir( RwV3d& vCurrLookAt, RwV3d& vTargetLookAt ) 
{
    m_vCurrLookAt = vCurrLookAt;
    m_vTargetLookAt = vTargetLookAt;
}

void CNtlCameraFPSController::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventKeyboardMove)
    {

        SNtlEventKeyboardMove* pData = (SNtlEventKeyboardMove*)pMsg.pData;
        switch(pData->byMoveFlags)
        {
        case NTL_MOVE_TURN_L:            
        case NTL_MOVE_TURN_R:
            m_uiMoveFlag = pData->byMoveFlags;
            break;
        default:
            m_uiMoveFlag = NTL_MOVE_NONE;
            break;
        }    
    }
    else if(pMsg.Id == g_EventKeyboardMoveStop)
    {
        m_uiMoveFlag = NTL_MOVE_NONE;
    }
}

void CNtlCameraFPSController::AddPitch( RwReal fDeltaPitch ) 
{
    m_fOrgPitch += fDeltaPitch;

    m_fOrgPitch = max(-89.0f, m_fOrgPitch);
    m_fOrgPitch = min(75.0f, m_fOrgPitch);
}

void CNtlCameraFPSController::AddYaw( RwReal fDeltaYaw ) 
{
    m_fOrgYaw += fDeltaYaw;

    if(m_fOrgYaw >= 360.0f)
        m_fOrgYaw -= 360.0f;

    if(m_fOrgYaw < 0.0f)
        m_fOrgYaw += 360.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CAMERA_BUS_GET_ON_TIME		1.5f
#define CAMERA_BUS_GET_ON_ANGLE_X	35.f

#define CAMERA_BUS_TRACE_ANGLE_X			18.f
#define CAMERA_BUS_TRACE_DELAY_TIME			.8f
#define CAMERA_BUS_TRACE_ROTATE_Y_SPEED		20.f

CNtlCameraBusController::CNtlCameraBusController()
:m_eState(EBS_STATE_NONE)
{
    SetType(CAMERA_CONTROL_BUS);
    SetFlags(NTL_CAMERA_CONTROL_NORMAL_PAUSE);
}

CNtlCameraBusController::~CNtlCameraBusController()
{

}

void CNtlCameraBusController::Enter( void ) 
{
    ChangeState(EBS_STATE_START);	

    m_BusStartCamera.fInterpolationElaped	= 0.f;

    m_BusStartCamera.fOriginalMinDist	= GetNtlGameCameraManager()->GetMinDist();
    m_BusStartCamera.fOriginalMaxDist	= GetNtlGameCameraManager()->GetMaxDist();
    m_BusStartCamera.fOriginalCurDist	= GetNtlGameCameraManager()->GetDist();
    m_BusStartCamera.fOriginalAngleX	= GetNtlGameCameraManager()->GetPitch();
    m_BusStartCamera.fOriginalAngleY	= GetNtlGameCameraManager()->GetYaw();
    m_BusStartCamera.fOriginalFov		= GetNtlGameCameraManager()->GetFov();

    m_BusStartCamera.fDestAngleY = GetDeltaYaw(m_BusStartCamera.fOriginalAngleY);

    GetNtlGameCameraManager()->SetMinDist( 20.f );
    GetNtlGameCameraManager()->SetMaxDist( 30.f );
    GetNtlGameCameraManager()->SetFov( 75.f );
    GetSceneManager()->SetWorldPVSActivation(FALSE);
}

void CNtlCameraBusController::Exit( void ) 
{
    ChangeState(EBS_STATE_NONE);

    GetNtlGameCameraManager()->SetMinDist( m_BusStartCamera.fOriginalMinDist );
    GetNtlGameCameraManager()->SetMaxDist( m_BusStartCamera.fOriginalMaxDist );
    GetNtlGameCameraManager()->SetDist( m_BusStartCamera.fOriginalCurDist );
    GetNtlGameCameraManager()->SetFov( m_BusStartCamera.fOriginalFov );
    GetSceneManager()->SetWorldPVSActivation(TRUE);

    Finish();
}

void CNtlCameraBusController::Update( RwReal fElapsed ) 
{
    if( m_eState == EBS_STATE_START )
    {
        Update_Start(fElapsed);
    }
    else if( m_eState == EBS_STATE_USER_CONTROLL )
    {
        Update_UserCamera(fElapsed);
    }
    else if( m_eState == EBS_STATE_TRACE_BUSBACK )
    {
        Update_TraceBusBack(fElapsed);
    }
	else
	{
		return;
	}

    CNtlCameraNormalController::Update(fElapsed);
}

void CNtlCameraBusController::UserCameraControlEnable(RwBool bEnable)
{
    if( m_eState == EBS_STATE_START )
        return;

    if( bEnable )
    {
        ChangeState(EBS_STATE_USER_CONTROLL);
    }
    else if( m_eState == EBS_STATE_USER_CONTROLL )
    {
        m_UserCamera.bChange = TRUE;
    }
}

void CNtlCameraBusController::InitTraceCamera()
{
    RwBool bLeftTurn;

    m_BusTraceInfo.fOriginalAngleX		= GetNtlGameCameraManager()->GetPitch();
    m_BusTraceInfo.fOriginalAngleY		= GetNtlGameCameraManager()->GetYaw();
    m_BusTraceInfo.fDestAngleY			= GetDeltaYaw(m_BusTraceInfo.fOriginalAngleY, &bLeftTurn);
    m_BusTraceInfo.fInterpolationElaped	= 0.f;
    m_BusTraceInfo.v3BusDir				= GetActiveActor()->GetDirection();


    if( bLeftTurn )
    {
        RwReal fAngleAdjust = m_BusTraceInfo.fOriginalAngleY - m_BusTraceInfo.fDestAngleY;

        if( fAngleAdjust < 10.f )
            m_BusTraceInfo.fInterpolationTime = .5f;
        else
            m_BusTraceInfo.fInterpolationTime = fAngleAdjust/CAMERA_BUS_TRACE_ROTATE_Y_SPEED;
    }
    else
    {
        RwReal fAngleAdjust = m_BusTraceInfo.fDestAngleY - m_BusTraceInfo.fOriginalAngleY;

        if( fAngleAdjust < 10.f )
            m_BusTraceInfo.fInterpolationTime = .5f;
        else
            m_BusTraceInfo.fInterpolationTime = fAngleAdjust/CAMERA_BUS_TRACE_ROTATE_Y_SPEED;
    }
}

void CNtlCameraBusController::ChangeState( ECameraBusState eState)
{
    m_eState = eState;

    switch(m_eState)
    {
    case EBS_STATE_START:
        {
            break;
        }
    case EBS_STATE_USER_CONTROLL:
        {
            m_UserCamera.bChange			= FALSE;
            m_UserCamera.fChangeDelayTime	= 0.f;
            break;
        }
    case EBS_STATE_TRACE_BUSBACK:
        {
            InitTraceCamera();
            m_BusTraceInfo.bTracing			= FALSE;
            break;
        }
    }
}

void CNtlCameraBusController::Update_Start( RwReal fElapsed )
{
    m_BusStartCamera.fInterpolationElaped += fElapsed;

    if( m_BusStartCamera.fInterpolationElaped > CAMERA_BUS_GET_ON_TIME )
        m_BusStartCamera.fInterpolationElaped = CAMERA_BUS_GET_ON_TIME;

    RwReal fDelta		= m_BusStartCamera.fInterpolationElaped/CAMERA_BUS_GET_ON_TIME;
    RwReal fCurDist		= CNtlMath::Interpolation(m_BusStartCamera.fOriginalCurDist, 25.f, fDelta);
    RwReal fCurAngleX	= CNtlMath::Interpolation(m_BusStartCamera.fOriginalAngleX, CAMERA_BUS_GET_ON_ANGLE_X, fDelta);
    RwReal fCurAngleY	= CNtlMath::Interpolation(m_BusStartCamera.fOriginalAngleY, m_BusStartCamera.fDestAngleY, fDelta);

    GetNtlGameCameraManager()->SetDist(fCurDist);
    GetNtlGameCameraManager()->SetPitch(fCurAngleX);
    GetNtlGameCameraManager()->SetYaw(fCurAngleY);

    if( m_BusStartCamera.fInterpolationElaped == CAMERA_BUS_GET_ON_TIME )
        ChangeState(EBS_STATE_TRACE_BUSBACK);
}

void CNtlCameraBusController::Update_UserCamera( RwReal fElapsed )
{
    if( !m_UserCamera.bChange )
        return;

    m_UserCamera.fChangeDelayTime += fElapsed;

    if( m_UserCamera.fChangeDelayTime >= CAMERA_BUS_TRACE_DELAY_TIME )
        ChangeState(EBS_STATE_TRACE_BUSBACK);
}

void CNtlCameraBusController::Update_TraceBusBack( RwReal fElapsed )
{
    RwV3d v3BusDir = GetActiveActor()->GetDirection();

    if( !m_BusTraceInfo.bTracing )
    {
        // 버스의 방향이 바뀌었는지 체크
        if( v3BusDir.x != m_BusTraceInfo.v3BusDir.x ||
            v3BusDir.y != m_BusTraceInfo.v3BusDir.y ||
            v3BusDir.z != m_BusTraceInfo.v3BusDir.z )
        {
            ChangeState(EBS_STATE_TRACE_BUSBACK);
            m_BusTraceInfo.bTracing = TRUE;
            return;
        }		
    }

    m_BusTraceInfo.bTracing = TRUE;

    m_BusTraceInfo.fInterpolationElaped += fElapsed;	

    if( m_BusTraceInfo.fInterpolationTime == 0.f )
    {
        ChangeState(EBS_STATE_TRACE_BUSBACK);
        return;
    }

    if( m_BusTraceInfo.fInterpolationElaped > m_BusTraceInfo.fInterpolationTime )
        m_BusTraceInfo.fInterpolationElaped = m_BusTraceInfo.fInterpolationTime;

    RwReal fDelta		= m_BusTraceInfo.fInterpolationElaped/m_BusTraceInfo.fInterpolationTime;
    RwReal fCurAngleX	= CNtlMath::Interpolation(m_BusTraceInfo.fOriginalAngleX, CAMERA_BUS_TRACE_ANGLE_X, fDelta);
    RwReal fCurAngleY	= CNtlMath::Interpolation(m_BusTraceInfo.fOriginalAngleY, m_BusTraceInfo.fDestAngleY, fDelta);	

    GetNtlGameCameraManager()->SetPitch(fCurAngleX);
    GetNtlGameCameraManager()->SetYaw(fCurAngleY);

    if( m_BusTraceInfo.fInterpolationElaped == m_BusTraceInfo.fInterpolationTime )
        ChangeState(EBS_STATE_TRACE_BUSBACK);
}

RwReal CNtlCameraBusController::GetDeltaYaw(RwReal fOriginalAngleY, RwBool* pbLeftTurn /* = NULL */)
{
#define dCOSINE_LIMIT_EPSION		(1.f - NTL_EPSILON)

    RwReal fAngle, fDestAngle, fTemp;
    RwBool bLeftTurn;
    RwV3d v3BusOppositeDir = GetActiveActor()->GetDirection() * -1.f;
    RwV3d v3CameraDir = *GetNtlGameCameraManager()->GetCameraDir();


    v3BusOppositeDir.y		= 0.f;			RwV3dNormalize(&v3BusOppositeDir, &v3BusOppositeDir);
    v3CameraDir.y			= 0.f;			RwV3dNormalize(&v3CameraDir, &v3CameraDir);	


    fTemp = RwV3dDotProduct(&v3BusOppositeDir, &ZAxis);

    if( fTemp < -dCOSINE_LIMIT_EPSION )
        fTemp = -1.f;
    else if( fTemp > dCOSINE_LIMIT_EPSION )
        fTemp = 1.f;	


    RwReal fBusOppositeAngle = NTL_RAD2DEG( acos(fTemp) );
    RwReal fOppositeDirCameraAngle = fOriginalAngleY + 180.f;

    if( v3BusOppositeDir.x < 0.f )
        fBusOppositeAngle = 360.f - fBusOppositeAngle;

    if( fOppositeDirCameraAngle >= 360.f )
        fOppositeDirCameraAngle -= 360.f;

    if( fOriginalAngleY >= 180 )
    {
        if( fOppositeDirCameraAngle <= fBusOppositeAngle && fBusOppositeAngle <= fOriginalAngleY )
            bLeftTurn = FALSE;
        else
            bLeftTurn = TRUE;		
    }
    else
    {
        if( fOriginalAngleY <= fBusOppositeAngle && fBusOppositeAngle <= fOppositeDirCameraAngle )
            bLeftTurn = TRUE;
        else
            bLeftTurn = FALSE;
    }


    v3CameraDir *= -1.f;

    fTemp = RwV3dDotProduct(&v3BusOppositeDir, &v3CameraDir);

    if( fTemp < -dCOSINE_LIMIT_EPSION )
        fTemp = -1.f;
    else if( fTemp > dCOSINE_LIMIT_EPSION )
        fTemp = 1.f;		

    fAngle = NTL_RAD2DEG( acos(fTemp) );


    if( bLeftTurn )
        fDestAngle = fOriginalAngleY - fAngle;
    else
        fDestAngle = fOriginalAngleY + fAngle;

    if( pbLeftTurn )
        *pbLeftTurn = bLeftTurn;

    return fDestAngle;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CNtlCameraTutorialController::CNtlCameraTutorialController( void )
{
    SetType(CAMERA_CONTROL_TURORIAL);
    SetFlags(NTL_CAMERA_CONTROL_NORMAL_PAUSE);

    m_pActor = NULL;

    m_uiType = 0xffffffff;
    m_uiTblIdx = 0xffffffff;

    m_vTargetPos.x = m_vTargetPos.y = m_vTargetPos.z = 0.f;

    m_byFinishFlag = 0;
}


CNtlCameraTutorialController::~CNtlCameraTutorialController( void )
{
    Exit();
}


void CNtlCameraTutorialController::Enter( void )
{
    UpdateTargetPos();

    RwV3d vCamDir = *GetNtlGameCameraManager()->GetCameraDir();

    m_fOldYaw = CNtlMath::LineToAngleY( &vCamDir );
    m_fCurYaw = m_fOldYaw;
    m_fOffsetYaw = 0.f;

    m_fOldPitch = CNtlMath::LineToAngleX( &vCamDir );
    m_fCurPitch = m_fOldPitch;
    m_fOffsetPitch = 0.f;
}


void CNtlCameraTutorialController::Exit( void )
{
}


void CNtlCameraTutorialController::Update( RwReal fElapsed )
{
    if ( IsFinish() )
    {
        return;
    }

    UpdateTargetPos();

    RwV3d vDistDir = m_pActor->GetPosition() - m_vTargetPos; vDistDir.y = 0.f;
    if ( RwV3dLength( &vDistDir ) < 0.5f )
    {
        Finish();
        return;
    }

    if ( (m_byFinishFlag & EYAW_FINISH) &&
        (m_byFinishFlag & EPITCH_FINISH) )
    {
        Finish();
        return;
    }

    m_fOffsetYaw = 0.f;
    m_fOffsetPitch = 0.f;

    if ( (m_byFinishFlag & EYAW_FINISH) != EYAW_FINISH ) UpdateYaw( fElapsed );
    if ( (m_byFinishFlag & EPITCH_FINISH) != EPITCH_FINISH ) UpdatePitch( fElapsed );

    CNtlCameraFPSController* pFPSCtrl = (CNtlCameraFPSController*)GetNtlGameCameraManager()->FindController( CAMERA_CONTROL_FPS );

    if ( pFPSCtrl )
    {
        pFPSCtrl->AddPitch( m_fOffsetPitch );
        pFPSCtrl->AddYaw( m_fOffsetYaw );
    }
    else
    {
        GetNtlGameCameraManager()->SetPitch( m_fCurPitch );
        GetNtlGameCameraManager()->SetYaw( m_fCurYaw );

        RwMatrix matTrans;
        RwMatrixSetIdentity( &matTrans );

        RwV3d vPos;
        CNtlMath::MathRwV3dAssign( &vPos, 0.0f, 0.0f, GetNtlGameCameraManager()->GetDist() ); 
        RwMatrixTranslate( &matTrans, &vPos, rwCOMBINEREPLACE );
        RwMatrixRotate( &matTrans, &CNtlPLGlobal::m_vXAxisV3, GetNtlGameCameraManager()->GetPitch(), rwCOMBINEPOSTCONCAT );
        RwMatrixRotate( &matTrans, &CNtlPLGlobal::m_vYAxisV3, GetNtlGameCameraManager()->GetYaw(), rwCOMBINEPOSTCONCAT );
        RwMatrixUpdate( &matTrans );

        RwV3d vLookAt = m_pActor->GetPosition();
        vLookAt.y += GetActorOffset();

        RwV3d vCamPos = vLookAt - matTrans.pos;

        GetNtlGameCameraManager()->SetLookAt( &vLookAt );
        GetNtlGameCameraManager()->SetCameraPos( &vCamPos );
    }
}


void CNtlCameraTutorialController::SetActiveActor( const CNtlSobActor *pActor )
{
    m_pActor = (CNtlSobActor*)pActor;
}


void CNtlCameraTutorialController::SetData( RwUInt32 uiType, RwUInt32 uiTblIdx, RwV3d& vTargetPos )
{
    m_uiType = uiType;
    m_uiTblIdx = uiTblIdx;

    if ( m_uiType == ETL_CAMERA_TYPE_POSITION )
    {
        m_vTargetPos = vTargetPos;

        RwReal fHeight;
        GetSceneManager()->GetWorldHeight( &m_vTargetPos, fHeight, NULL);
        m_vTargetPos.y = fHeight;
    }
    else
    {
        m_vTargetPos.x = RwRealMAXVAL;
        m_vTargetPos.y = RwRealMAXVAL;
        m_vTargetPos.z = RwRealMAXVAL;
    }
}


void CNtlCameraTutorialController::UpdateTargetPos( void )
{
    switch ( m_uiType )
    {
    case ETL_CAMERA_TYPE_MOB:
        {
            CNtlSobGroup* pGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_MONSTER );
            if ( NULL == pGroup )
            {
                Finish();
                return;
            }

            CNtlSob* pSobObj = NULL;
            CNtlSobMonsterAttr* pAttr = NULL;

            CNtlSobGroup::MapObject_Iter it = pGroup->Begin();
            CNtlSobGroup::MapObject_Iter itEnd = pGroup->End();

            for ( ; it != itEnd; ++it )
            {
                pAttr = (CNtlSobMonsterAttr*)it->second->GetSobAttr();

                if ( pAttr->GetMobTbl()->tblidx == m_uiTblIdx )
                {
                    pSobObj = it->second;
                    m_vTargetPos = pSobObj->GetPosition();
                    break;
                }
            }

            if ( NULL == pSobObj )
            {
                Finish();
                return;
            }
        }
        break;

    case ETL_CAMERA_TYPE_NPC:
        {
            SERIAL_HANDLE hHandle = GetNtlSobManager()->GetNpcTriggerSerialFromId( m_uiTblIdx );

            CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( hHandle );

            if ( NULL != pSobObj )
            {
                m_vTargetPos = pSobObj->GetPosition();
            }
            else
            {
                Finish();
                return;
            }
        }
        break;

    case ETL_CAMERA_TYPE_POSITION:
        {
        }
        break;

    default:
        {
            Finish();
        }
        break;
    }
}


void CNtlCameraTutorialController::UpdateYaw( RwReal fElapsed )
{
#define ROT_SPEED	(0.30f)

    RwV3d vResultDir;

    RwV3d vSrcDir = *GetNtlGameCameraManager()->GetCameraDir(); vSrcDir.y = 0.f;
    RwV3dNormalize( &vSrcDir, &vSrcDir );

    RwV3d vDestDir = m_vTargetPos - m_pActor->GetPosition(); vDestDir.y = 0.f;
    RwV3dNormalize( &vDestDir, &vDestDir );

    RwV3d vDistDir = vDestDir - vSrcDir;
    RwReal fDistDirLen = RwV3dNormalize( &vDistDir, &vDistDir );
    if ( fDistDirLen < 0.01f )
    {
        vResultDir = vDestDir;

        m_byFinishFlag |= EYAW_FINISH;
    }
    else
    {
        RwReal fAccum = fDistDirLen * ROT_SPEED;
        RwReal fRotSpeed = ROT_SPEED * fElapsed + fAccum;

        if ( fDistDirLen < fRotSpeed )
        {
            vResultDir = vDestDir;

            m_byFinishFlag |= EYAW_FINISH;
        }
        else
        {
            vDistDir = vDistDir * fRotSpeed;
            vResultDir = vSrcDir + vDistDir;
            RwV3dNormalize( &vResultDir, &vResultDir );
        }
    }

    RwReal fCurYaw = CNtlMath::LineToAngleY( &vResultDir );
    m_fOffsetYaw = fCurYaw - m_fCurYaw;
    m_fCurYaw = fCurYaw;
}


void CNtlCameraTutorialController::UpdatePitch( RwReal fElapsed )
{
#define HEIGHT_SPEED	(0.28f)

    RwV3d vResultDir;

    RwReal fSrcY = GetNtlGameCameraManager()->GetCameraDir()->y;

    RwV3d vActorPos = m_pActor->GetPosition(); vActorPos.y += GetActorOffset();
    RwV3d vDestDir = m_vTargetPos - vActorPos;
    if ( RwV3dNormalize( &vDestDir, &vDestDir ) < 0.0001f )
    {
        m_byFinishFlag |= EPITCH_FINISH;
        return;
    }

    RwReal fDestY = vDestDir.y;

    if ( abs( fSrcY - fDestY ) < 0.01f )
    {
        fSrcY = fDestY;
        m_byFinishFlag |= EPITCH_FINISH;
    }
    else
    {
        RwReal fAccum = (abs( fSrcY - fDestY ) > 1.0001f ? abs( fSrcY - fDestY ) : 1.f) * HEIGHT_SPEED;
        RwReal fVec = HEIGHT_SPEED * fElapsed + fAccum / 4.f;

        if ( fSrcY < fDestY )
        {
            if ( fSrcY + fVec > fDestY )
            {
                fSrcY = fDestY;
                m_byFinishFlag |= EPITCH_FINISH;
            }
            else
            {
                fSrcY += fVec;
            }
        }
        else if ( fSrcY > fDestY )
        {
            if ( fSrcY - fVec < fDestY )
            {
                fSrcY = fDestY;
                m_byFinishFlag |= EPITCH_FINISH;
            }
            else
            {
                fSrcY -= fVec;
            }
        }
        else
        {
            fSrcY = fDestY;
            m_byFinishFlag |= EPITCH_FINISH;
        }
    }

    vResultDir = *GetNtlGameCameraManager()->GetCameraDir();
    vResultDir.y = fSrcY;

    RwReal fCurPitch = CNtlMath::LineToAngleX( &vResultDir );
    m_fOffsetPitch = fCurPitch - m_fCurPitch;
    m_fCurPitch = fCurPitch;
}


RwReal CNtlCameraTutorialController::GetActorOffset( void )
{
	if ( m_pActor )
	{
		// 수영 중인 경우는 Height ratio 를 반영하지 않는다

		RwReal fOffset;
		RwV3d vPos = m_pActor->GetPosition();

		SWorldHeightStuff sHStuff;
		if ( Logic_IsSwimmingActor( m_pActor, &vPos, sHStuff ) )
		{
			fOffset = m_pActor->GetSobProxy()->GetPLEntityHeight();
		}
		else
		{
			fOffset = m_pActor->GetSobProxy()->GetPLEntityHeight() * m_fTargetHeightRatio;
		}

		if ( fOffset < 1.1f )
		{
			fOffset = 1.1f;
		}

		fOffset += m_fHeightRatioBonus;

		return fOffset;
	}
	else
	{
		return 0.f;
	}
}

//////////////////////////////////////////////////////////////////////////
#define OBSERVER_XML            ".\\script\\ObserverCamera.xml"
#define OBSERVER_NODE_CAMERA    "OBSERVER/CAMERA"

CNtlCameraObserver::CNtlCameraObserver() 
{
    SetType(CAMERA_CONTROL_OBSERVER);
    SetFlags(NTL_CAMERA_CONTROL_NORMAL_PAUSE);

    m_nIndex = -1;
}

CNtlCameraObserver::~CNtlCameraObserver() 
{

}

void CNtlCameraObserver::Enter( void ) 
{
    __super::Enter();

    if(!LoadObserverCameraData())
    {
        Finish();
    }
}

void CNtlCameraObserver::Exit( void ) 
{
    m_mapPos.clear();

    __super::Exit();
}

void CNtlCameraObserver::Update( RwReal fElapsed ) 
{
    __super::Update(fElapsed);

    if(IsFinish())
    {       
        RwV3d vPos = *GetNtlGameCameraManager()->GetCameraPos();
        RwV3d vTarget = GetNtlGameCameraManager()->GetLookAt();

        // 프리 카메라를 위해서 Pitch와 Yaw를 저장한다.
        RwV3d vLookAt = vTarget - vPos;
        RwV3dNormalize(&vLookAt, &vLookAt);        
        GetNtlGameCameraManager()->SetPitch(CNtlMath::LineToAngleX(&vLookAt));
        GetNtlGameCameraManager()->SetYaw(CNtlMath::LineToAngleY(&vLookAt));

        return;
    }
        

    if(m_nIndex == -1)
        return;    
}

void CNtlCameraObserver::SetIndex(RwInt32 nType, RwInt32 nIndex)
{
    if(nType == E_OBSERVER_TYPE_STATIC)
    {
        if(m_mapPos.find(nIndex) == m_mapPos.end())
            return;

        m_nIndex = nIndex;

        GetNtlGameCameraManager()->SetCameraPos(&m_mapPos[m_nIndex].vPos);
        GetNtlGameCameraManager()->SetLookAt(&m_mapPos[m_nIndex].vTarget);

        //// 프리 카메라를 위해서 Pitch와 Yaw를 저장한다.
        //RwV3d vLookAt = m_mapPos[m_nIndex].vTarget - m_mapPos[m_nIndex].vPos;
        //RwV3dNormalize(&vLookAt, &vLookAt);        
        //GetNtlGameCameraManager()->SetPitch(CNtlMath::LineToAngleX(&vLookAt));
        //GetNtlGameCameraManager()->SetYaw(CNtlMath::LineToAngleY(&vLookAt));
    }
    else if(nType == E_OBSERVER_TYPE_NODE)
    {
        LuaExec_ObserverCamera(nIndex);
    }
}

RwBool CNtlCameraObserver::LoadObserverCameraData() 
{
    CNtlXMLDoc doc;
    if(!doc.Create())
        return FALSE;

    if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT)
    {
        char* pData = NULL;
        int nSize = 0;
        
        GetNtlResourcePackManager()->LoadScript(OBSERVER_XML, (void**)&pData, &nSize);
        if(pData == NULL)
            return FALSE;

        // 버퍼 + 1 생성
        char* pBuffer = NTL_NEW char[nSize + 1];
        memcpy(pBuffer, pData, sizeof(char) * nSize);
        pBuffer[nSize] = '\0';
        if(!doc.LoadXML(pBuffer))
            return FALSE;
    }
    else
    {
        if(!doc.Load(OBSERVER_XML))
            return FALSE;
    }
    
    m_mapPos.clear();
    IXMLDOMNodeList* pNodeList = doc.SelectNodeList(OBSERVER_NODE_CAMERA);
    if(pNodeList)
    {
        long listLen = 0;
        pNodeList->get_length(&listLen);
        IXMLDOMNode* pNode = NULL;
        for(long i = 0; i < listLen; ++i)
        {
            pNodeList->get_item(i, &pNode);
            if(pNode)
            {
                char szIndex[8]     = {0,};
                char szPos[64]      = {0,};
                char szTarget[64]   = {0,};

                doc.GetTextWithAttributeName(pNode, "INDEX", szIndex, sizeof(szIndex));
                doc.GetTextWithAttributeName(pNode, "POS", szPos, sizeof(szPos));
                doc.GetTextWithAttributeName(pNode, "TARGET", szTarget, sizeof(szTarget));

                SObserverCameraData data;
                data.nIndex     = (RwInt32)atoi(szIndex);
                data.vPos       = Helper_String2V3D(szPos);
                data.vTarget    = Helper_String2V3D(szTarget);

                m_mapPos[data.nIndex] = data;

                pNode->Release();
            }
        }

        pNodeList->Release();
    }
    
    doc.Destroy();

    return TRUE;
}
