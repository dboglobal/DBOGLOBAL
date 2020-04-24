#include "precomp_ntlsimulation.h"
#include "NtlCameraManager.h"

// presentation
#include "NtlPLGlobal.h"
#include "NtlPLApi.h"
#include "NtlPLSceneManager.h"
#include "NtlPLCharacter.h"
#include "NtlPLVisualManager.h"

// framework
#include "NtlCamera.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlCameraController.h"
#include "NtlSLLogic.h"
#include "NtlSobCharProxy.h"
#include "NtlSobActor.h"
#include "NtlSLLuaGlue.h"
#include "NtlSlGlobal.h"
#include "NtlMovement.h"
#include "NtlSLEventFunc.h"
#include "NtlSobGroup.h"
#include "NtlSobManager.h"

#define CAMERA_DEFAULT_PITCH		10.0f
#define CAMERA_DERAULT_DIST			10.0f
#define CAMERA_MAX_DIST				20.0f

CNtlGameCameraManager* CNtlGameCameraManager::m_pInstance = NULL;


//////////////////////////////////////////////////////////////////////////
//
//	CAMInterpolation
//
//////////////////////////////////////////////////////////////////////////


CNtlGameCameraManager::CCAMInterpolation::CCAMInterpolation( void )
: m_fRadius( 0.f )
, m_eCAMInterpType( eCAM_INTERP_TYPE_IDLE )
, m_fOriginOldCamDist( CAMERA_DERAULT_DIST )
, m_fOriginCurCamDist( CAMERA_DERAULT_DIST )
, m_fSourceCamDist( CAMERA_DERAULT_DIST )
, m_fTargetCamDist( CAMERA_DERAULT_DIST )
{
	m_parStateFunc[eCAM_INTERP_TYPE_RUN_FRONT][eCAM_INTERP_TYPE_RUN_FRONT]	= &CNtlGameCameraManager::CCAMInterpolation::OnMoveFrontToFront;
	m_parStateFunc[eCAM_INTERP_TYPE_RUN_FRONT][eCAM_INTERP_TYPE_RUN_BACK]	= &CNtlGameCameraManager::CCAMInterpolation::OnMoveBack;
	m_parStateFunc[eCAM_INTERP_TYPE_RUN_FRONT][eCAM_INTERP_TYPE_IDLE]		= &CNtlGameCameraManager::CCAMInterpolation::OnMoveIdle;

	m_parStateFunc[eCAM_INTERP_TYPE_RUN_BACK][eCAM_INTERP_TYPE_RUN_FRONT]	= &CNtlGameCameraManager::CCAMInterpolation::OnMoveIdleToFront;
	m_parStateFunc[eCAM_INTERP_TYPE_RUN_BACK][eCAM_INTERP_TYPE_RUN_BACK]	= &CNtlGameCameraManager::CCAMInterpolation::OnMoveBack;
	m_parStateFunc[eCAM_INTERP_TYPE_RUN_BACK][eCAM_INTERP_TYPE_IDLE]		= &CNtlGameCameraManager::CCAMInterpolation::OnMoveIdle;

	m_parStateFunc[eCAM_INTERP_TYPE_IDLE][eCAM_INTERP_TYPE_RUN_FRONT]		= &CNtlGameCameraManager::CCAMInterpolation::OnMoveIdleToFront;
	m_parStateFunc[eCAM_INTERP_TYPE_IDLE][eCAM_INTERP_TYPE_RUN_BACK]		= &CNtlGameCameraManager::CCAMInterpolation::OnMoveBack;
	m_parStateFunc[eCAM_INTERP_TYPE_IDLE][eCAM_INTERP_TYPE_IDLE]			= &CNtlGameCameraManager::CCAMInterpolation::OnMoveIdle;
}

void CNtlGameCameraManager::CCAMInterpolation::ChangeInterpState( CNtlGameCameraManager::CCAMInterpolation::eCAM_INTERP_TYPE eInterpType, RwReal fElapsed, RwV3d& vCamPos, RwV3d& vLookAt, RwReal fOriginCamDist )
{
	m_fOriginOldCamDist = m_fOriginCurCamDist;
	m_fOriginCurCamDist = fOriginCamDist;

	(this->*m_parStateFunc[m_eCAMInterpType][eInterpType])( fElapsed, vCamPos, vLookAt );
}

void CNtlGameCameraManager::CCAMInterpolation::Update( RwReal fElapsed, RwV3d& vCamPos, RwV3d& vLookAt )
{
	switch ( m_eCAMInterpType )
	{
	case eCAM_INTERP_TYPE_RUN_FRONT:
		{
			Interpolation( fElapsed, vCamPos, vLookAt, 0.3f );

			GetNtlGameCameraManager()->UpdateCameraForWorld( vCamPos, vLookAt, m_fRadius );
		}
		break;

	case eCAM_INTERP_TYPE_RUN_BACK:
		{
			Interpolation( fElapsed, vCamPos, vLookAt, 0.15f );

			RwV3d vNewPos;
			if ( GetSceneManager()->GetCameraCollision( &vCamPos, &vLookAt, m_fRadius, vNewPos ) )
			{
				vCamPos = vNewPos;
			}

			GetNtlGameCameraManager()->UpdateCameraForWorld( vCamPos, vLookAt, m_fRadius );
		}
		break;

	default:
		{
			GetNtlGameCameraManager()->UpdateCameraForWorld( vCamPos, vLookAt, m_fRadius );
		}
		break;
	}
}

void CNtlGameCameraManager::CCAMInterpolation::OnMoveIdleToFront( RwReal fElapsed, RwV3d& vCamPos, RwV3d& vLookAt )
{
	m_eCAMInterpType = eCAM_INTERP_TYPE_RUN_FRONT;

	RwReal fDist = CalCamDist( vCamPos, vLookAt );

	m_fSourceCamDist = fDist;
	m_fTargetCamDist = fDist;
}

void CNtlGameCameraManager::CCAMInterpolation::OnMoveFrontToFront( RwReal fElapsed, RwV3d& vCamPos, RwV3d& vLookAt )
{
	m_eCAMInterpType = eCAM_INTERP_TYPE_RUN_FRONT;

	RwReal fDist = CalCamDist( vCamPos, vLookAt );

	m_fSourceCamDist = fDist;
	m_fTargetCamDist = fDist;
}

void CNtlGameCameraManager::CCAMInterpolation::OnMoveBack( RwReal fElapsed, RwV3d& vCamPos, RwV3d& vLookAt )
{
	m_eCAMInterpType = eCAM_INTERP_TYPE_RUN_BACK;

	m_fSourceCamDist += (m_fOriginCurCamDist - m_fOriginOldCamDist);
	m_fTargetCamDist = m_fOriginCurCamDist;
}

void CNtlGameCameraManager::CCAMInterpolation::OnMoveIdle( RwReal fElapsed, RwV3d& vCamPos, RwV3d& vLookAt )
{
	m_eCAMInterpType = eCAM_INTERP_TYPE_IDLE;

	m_fSourceCamDist = m_fTargetCamDist;
	m_fSourceCamDist += (m_fOriginCurCamDist - m_fOriginOldCamDist);
}

RwReal CNtlGameCameraManager::CCAMInterpolation::CalCamDist( const RwV3d& vCamPos, const RwV3d& vLookAt )
{
	RwV3d vDist;
	RwV3dSubMacro( &vDist, &vCamPos, &vLookAt );

	RwReal fDist = RwV3dLength( &vDist );

	return fDist;
}

void CNtlGameCameraManager::CCAMInterpolation::Interpolation( RwReal fElapsed, RwV3d& vCamPos, RwV3d& vLookAt, RwReal fSpeed )
{
	if ( m_fSourceCamDist == m_fTargetCamDist )
	{
		m_eCAMInterpType = eCAM_INTERP_TYPE_IDLE;
	}
	else
	{
		RwReal fDist = abs( m_fTargetCamDist - m_fSourceCamDist );
		RwReal fAccum = ( fDist * 0.8f > 1.f ? fDist * 0.8f : 1.f) * fSpeed;

		if ( m_fTargetCamDist - m_fSourceCamDist < 0.f )
		{
			m_fSourceCamDist -= fElapsed * fSpeed + fAccum / 2.f;

			if ( m_fSourceCamDist < m_fTargetCamDist )
			{
				m_eCAMInterpType = eCAM_INTERP_TYPE_IDLE;
				m_fSourceCamDist = m_fTargetCamDist;
			}
		}
		else if ( m_fTargetCamDist - m_fSourceCamDist > 0.f )
		{
			m_fSourceCamDist += fElapsed * fSpeed + fAccum / 2.f;

			if ( m_fSourceCamDist > m_fTargetCamDist )
			{
				m_eCAMInterpType = eCAM_INTERP_TYPE_IDLE;
				m_fSourceCamDist = m_fTargetCamDist;
			}
		}
		else
		{
			m_eCAMInterpType = eCAM_INTERP_TYPE_IDLE;
			m_fSourceCamDist = m_fTargetCamDist;
		}
	}

	RwV3d vDist;
	RwV3dSubMacro( &vDist, &vCamPos, &vLookAt );

	if ( RwV3dNormalize( &vDist, &vDist ) > 0.0001f )
	{
		vCamPos.x = vLookAt.x + vDist.x * m_fSourceCamDist;
		vCamPos.y = vLookAt.y + vDist.y * m_fSourceCamDist;
		vCamPos.z = vLookAt.z + vDist.z * m_fSourceCamDist;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//	GameCameraManager
//
//////////////////////////////////////////////////////////////////////////


CNtlGameCameraManager::CNtlGameCameraManager()
{
	NTL_ASSERT(m_pInstance == NULL, "CNtlGameCameraManager::CNtlGameCameraManager");

	m_pInstance = this;

	m_pCamera	= NULL;

	m_fMinDist	= 3.5f;    
	m_fMaxDist	= CAMERA_MAX_DIST;
	m_fDist		= CAMERA_DERAULT_DIST;
	
	m_fPitch	= CAMERA_DEFAULT_PITCH;	
	m_fYaw		= 0.0f;
	m_fRoll		= 0.0f;

	m_fMinPitch = -89.0f;
	m_fMaxPitch = 75.0f;
	
	m_vCamPos.x = 0.0f;
	m_vCamPos.y = 0.0f;
	m_vCamPos.z = 0.0f;

	m_vLookAt.x = 0.0f;
	m_vLookAt.y = 0.0f;
	m_vLookAt.z = 0.0f;

	m_vUp.x		= 0.0f;
	m_vUp.y		= -1.0f;
	m_vUp.z		= 0.0f;

	m_vOffset.x = 0.0f;
	m_vOffset.y = 0.0f;
	m_vOffset.z = 0.0f;

	m_vLookAtOffset.x	= 0.0f;
	m_vLookAtOffset.y	= 0.0f;
	m_vLookAtOffset.z	= 0.0f;

	m_bDTNodeAttach		= FALSE;	
	m_bDTAttrRestore	= TRUE;

	m_bCollisionCheck	= TRUE;
	m_bInterpCheck		= TRUE;

	m_bFinishLock		= FALSE;
	m_bUpdateData		= TRUE;
    m_bEnableCollisionObj = TRUE;

	m_bActorAlpha	= FALSE;
	m_pActor		= NULL;
    m_fCollisionObjCheckTime = 0.0f;

	m_fHeightRatioBonus = 0.0f;
}

CNtlGameCameraManager::~CNtlGameCameraManager()
{
	m_pInstance = NULL;
}

CNtlGameCameraManager* CNtlGameCameraManager::GetInstance(void)
{
	return m_pInstance;
}


void CNtlGameCameraManager::AddController(CNtlCameraController *pController)
{
	pController->Enter();

	m_listController.push_back(pController);
}

RwBool CNtlGameCameraManager::RemoveController(RwUInt8 byType)
{
	RwBool bSuccess = FALSE;

	CNtlCameraController *pController;
	ListCameraController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); ++it)
	{
		pController = (*it);
		if(pController->GetType() == byType)
		{
			bSuccess = TRUE;
			pController->Exit();
		}
	}

	return bSuccess;
}

CNtlCameraController* CNtlGameCameraManager::FindController(RwUInt8 byType)
{
	CNtlCameraController *pController;
	ListCameraController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); ++it)
	{
		pController = (*it);
		if(pController->GetType() == byType)
			return pController;
	}

	return NULL;
}

void CNtlGameCameraManager::CheckNormalControlPause(void)
{
	RwBool bPause = FALSE;

	CNtlCameraController *pController;
	ListCameraController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); ++it)
	{
		pController = (*it);
		if(pController->GetFlags() & NTL_CAMERA_CONTROL_NORMAL_PAUSE)
			bPause = TRUE;
	}

	if(bPause)
	{
		m_pBaseController		= NULL;
	}
	else
	{
		if(m_pBaseController == NULL)
		{
			if(m_eActiveCamera == ACTIVE_BASE_THIRD)
			{
				m_pBaseController = m_pThirdController;
			}
			else if(m_eActiveCamera == ACTIVE_BASE_FREE)
			{
				m_pBaseController = m_pFreeController;
			}
			
            if(m_pBaseController)
			    m_pBaseController->UpdateData(m_bUpdateData);

			if(!m_bUpdateData)
				m_bUpdateData = TRUE;
		}
	}
}

RwBool CNtlGameCameraManager::CheckRotatePossible(void)
{
	RwBool bRotatePossible = FALSE;

	if(m_pBaseController)
	{
		if( !(m_pBaseController->GetFlags() & NTL_CAMERA_CONTROL_NOT_ROTATE) )
			bRotatePossible = TRUE;
	}

	CNtlCameraController *pController;
	ListCameraController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); ++it)
	{
		pController = (*it);
		if(!pController->IsPause())
		{
			if( !(pController->GetFlags() & NTL_CAMERA_CONTROL_NOT_ROTATE) )
				bRotatePossible = TRUE;
		}
	}

	return bRotatePossible;
}

RwBool CNtlGameCameraManager::CheckZoomPossible(void)
{
	RwBool bZoomPossible = FALSE;

	if(m_pBaseController)
	{
		if( !(m_pBaseController->GetFlags() & NTL_CAMERA_CONTROL_NOT_ZOOM) )
			bZoomPossible = TRUE;
	}

	CNtlCameraController *pController;
	ListCameraController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); ++it)
	{
		pController = (*it);
		if(!pController->IsPause())
		{
			if( !(pController->GetFlags() & NTL_CAMERA_CONTROL_NOT_ZOOM) )
				bZoomPossible = TRUE;
		}
	}

	return bZoomPossible;
}

RwBool CNtlGameCameraManager::WorldHeight(RwV3d& vCamPos, RwV3d& vLookAt)
{
	if ( NULL == m_pCamera )
	{
		return FALSE;
	}

	RwCamera* pCamera = m_pCamera->GetCamera();

	if ( NULL == pCamera )
	{
		return FALSE;
	}

	RwReal fX = 0, fY = 0, fZ = 0;
	for ( int i = 0; i < 4; ++i )
	{
		fX += pCamera->frustumCorners[i].x;
		fY += pCamera->frustumCorners[i].y;
		fZ += pCamera->frustumCorners[i].z;
	}

	fX /= 4.f; fX -= pCamera->frustumCorners[0].x;
	fY /= 4.f; fY -= pCamera->frustumCorners[0].y;
	fZ /= 4.f; fZ -= pCamera->frustumCorners[0].z;

	RwReal fRadius = sqrtf( fX * fX + fY * fY + fZ * fZ );
	fRadius *= 2.f;

	if ( m_pActor == NULL )
	{
		return FALSE;
	}

	// camera 높이 구하기.
	RwV3d vTempPos;
	RwV3dAssignMacro(&vTempPos, &vCamPos);
	RwV3d vActorPos = m_pActor->GetPosition();
	vTempPos.y = vActorPos.y;

	SWorldHeightStuff sHStuff;
	if(!Logic_GetWorldHeight(&vTempPos, sHStuff))
		return FALSE;

	if(vCamPos.y - fRadius <= sHStuff.fFinialHeight)
	{
		vCamPos.y = sHStuff.fFinialHeight + fRadius;
		return TRUE;
	}

	return FALSE;
}

RwBool CNtlGameCameraManager::UpdateCameraForWorld(RwV3d& vCamPos, RwV3d& vLookAt, RwReal fRadius)
{
	RwV3d vCenterDir = vCamPos - vLookAt;

	if ( RwV3dNormalize( &vCenterDir, &vCenterDir ) < 0.001f )
	{
		return FALSE;
	}

	SWorldPickInfo sPickInfo;
	if ( GetSceneManager()->PickTerrianAndWater( vLookAt, vCamPos, sPickInfo ) )
	{
		vCamPos = sPickInfo.vPickPos;
	}

	// Boundary ray 구성
	RwV3d vRight;
	RwV3dCrossProduct( &vRight, &CNtlPLGlobal::m_vYAxisV3, &vCenterDir );
	if ( RwV3dNormalize( &vRight, &vRight ) < 0.0001f )
	{
		return FALSE;
	}

	RwV3d vUp;
	RwV3dCrossProduct( &vUp, &vCenterDir, &vRight );
	if ( RwV3dNormalize( &vUp, &vUp ) < 0.0001f )
	{
		return FALSE;
	}

	RwV3d vDestPos[6];

	for ( int x = 0; x < 2; ++x )
	{
		for ( int y = -1; y < 2; ++y )
		{
			vDestPos[x + 2 * (y+1)] = vCamPos + (vRight * ((0 == x) ? (RwReal)-fRadius : (RwReal)fRadius )) + (vUp * (RwReal)y * fRadius);
		}
	}

	RwReal fLastDist = RwRealMAXVAL;
	RwV3d vLastPos;

	for ( int i = 0; i < 6; ++i )
	{
		if ( GetSceneManager()->PickTerrianAndWater( vLookAt, vDestPos[i], sPickInfo ) )
		{
			RwV3d vDist = sPickInfo.vPickPos - vLookAt;
			RwReal fDist = RwV3dDotProduct( &vCenterDir, &vDist ) - fRadius;

			if ( fLastDist > fDist )
			{
				if ( fDist < 0.0001f )
				{
					fDist = 0.01f;
				}

				fLastDist = fDist;
				vLastPos = vLookAt + vCenterDir * fDist;
			}
		}
	}

	if ( fLastDist == RwRealMAXVAL )
	{
		return FALSE;
	}

	vCamPos = vLastPos;

	//API_PL_CameraLookAt( m_pCamera->GetCamera(), &vCamPos, &vLookAt, &m_vUp, m_fRoll );
	//_rwFrameSyncDirty();

	return TRUE;
}

void CNtlGameCameraManager::CollisionActor(RwV3d& vCamPos, RwV3d& vLookAt)
{
	if(m_pActor == NULL)
		return;

    CNtlPLCharacter* pPLChar = (CNtlPLCharacter*)m_pActor->GetSobProxy()->GetPLMainEntity();    

    // 캐릭터와 카메라가 가까이 있을 때 캐릭터에 알파를 준다.	    
	if(CNtlMath::GetLength(vLookAt, vCamPos) < pPLChar->GetAlphaDistance())
	{
		if(!m_bActorAlpha)
		{
			m_pActor->EnableInput(FALSE);
            m_pActor->GetSobProxy()->SetAlpha(100);
            m_pActor->GetSobProxy()->SetInkThickness(0.0f);			
			m_bActorAlpha = TRUE;
		}
	}
	else
	{
		// 캐릭터가 알파가 들어 있을 경우.
		if(m_bActorAlpha)
		{
			m_pActor->EnableInput(TRUE);
            m_pActor->GetSobProxy()->SetAlpha(255);
            m_pActor->GetSobProxy()->SetInkThickness();			
			m_bActorAlpha = FALSE;
		}
	}
}

/**
 * 아바타이외에 카메라가 가까워지면 알파 처리하는 객체들을 처리한다.
 */
void CNtlGameCameraManager::CollisionObj() 
{
    std::vector<CNtlSobGroup*> vecSobGroups;
    vecSobGroups.push_back(GetNtlSobManager()->GetSobGroup(SLCLASS_MONSTER));
    vecSobGroups.push_back(GetNtlSobManager()->GetSobGroup(SLCLASS_PLAYER));
    vecSobGroups.push_back(GetNtlSobManager()->GetSobGroup(SLCLASS_NPC));
    vecSobGroups.push_back(GetNtlSobManager()->GetSobGroup(SLCLASS_PET));

    RwV3d vCameraPos = *Logic_GetCameraPos(m_pCamera->GetCamera());
    
    RwReal fLength = 0.0f;
    CNtlSob* pSobObj = NULL;    
    CNtlPLCharacter* pPLChar = NULL;    
    for each(CNtlSobGroup* pGroup in vecSobGroups)
    {
        if(!pGroup)
            continue;

        CNtlSobGroup::MapObject_Iter it;        
        for(it = pGroup->Begin(); it != pGroup->End(); ++it)
        {
            pSobObj = (*it).second;
        
            fLength = CNtlMath::GetLength(vCameraPos, pSobObj->GetPosition());
            pPLChar = (CNtlPLCharacter*)pSobObj->GetSobProxy()->GetPLMainEntity();            
            if(fLength < pPLChar->GetAlphaDistance())
            {
                pSobObj->GetSobProxy()->EnableCameraAlpha(TRUE);
            }
            else
            {
                pSobObj->GetSobProxy()->EnableCameraAlpha(FALSE);
            }
        }
    }
}


RwBool CNtlGameCameraManager::Create(void)
{
	m_eActiveCamera			= ACTIVE_BASE_THIRD;
	m_pThirdController		= NTL_NEW CNtlCameraNormalController;
	m_pFreeController		= NTL_NEW CNtlFreeCameraController;
	m_pBaseController		= m_pThirdController;

	LinkMsg(g_EventCameraSmoothingMove);
	LinkMsg(g_EventCameraShake);
	LinkMsg(g_EventCameraDash);
	LinkMsg(g_EventCameraNpc);
	LinkMsg(g_EventCameraDB);
	LinkMsg(g_EventCameraDTClear);
	LinkMsg(g_EventCameraDTSplineNode);
	LinkMsg(g_EventCameraFree);
    LinkMsg(g_EventDBCScatter);
    LinkMsg(g_EventCameraTimeMachine);
	LinkMsg(g_EventCameraExplosion);
	LinkMsg(g_EventCameraTimeMachineArrive);
	LinkMsg(g_EventCameraKnockDownMatrix);
    LinkMsg(g_EventCameraFPS);
	LinkMsg(g_EventCameraBus);
	LinkMsg(g_EventTLPlayCamera);
	LinkMsg(g_EventCameraControlDelete);
    LinkMsg(g_EventKeyboardMove);
    LinkMsg(g_EventKeyboardMoveStop);
    LinkMsg(g_EventCameraObserver);

	return TRUE;
}

void CNtlGameCameraManager::Destroy(void)
{
	UnLinkMsg(g_EventCameraSmoothingMove);
	UnLinkMsg(g_EventCameraShake);
	UnLinkMsg(g_EventCameraDash);
	UnLinkMsg(g_EventCameraNpc);
	UnLinkMsg(g_EventCameraDB);
	UnLinkMsg(g_EventCameraDTClear);
	UnLinkMsg(g_EventCameraDTSplineNode);
    UnLinkMsg(g_EventCameraFree);
    UnLinkMsg(g_EventDBCScatter);
    UnLinkMsg(g_EventCameraTimeMachine);
	UnLinkMsg(g_EventCameraExplosion);
	UnLinkMsg(g_EventCameraTimeMachineArrive);
	UnLinkMsg(g_EventCameraKnockDownMatrix);
    UnLinkMsg(g_EventCameraFPS);
	UnLinkMsg(g_EventCameraBus);
	UnLinkMsg(g_EventTLPlayCamera);
	UnLinkMsg(g_EventCameraControlDelete);
    UnLinkMsg(g_EventKeyboardMove);
    UnLinkMsg(g_EventKeyboardMoveStop);
    UnLinkMsg(g_EventCameraObserver);

	ListCameraController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); ++it)
	{
		NTL_DELETE( *it );
	}
	m_listController.clear();

	NTL_DELETE( m_pThirdController );
	NTL_DELETE( m_pFreeController );
}

void CNtlGameCameraManager::Update(RwReal fElapsed)
{
	if(m_pCamera == NULL)
		return;

	RwBool bDelete = FALSE;

	// 기본 camera control
	if(m_pBaseController)
	{
		m_pBaseController->Update(fElapsed);
	}

    RwBool bCollisionCheck = m_pBaseController ? m_pBaseController->IsCollisionCheck() : TRUE;
	RwBool bInterpCheck = TRUE;

	// 확장 camera control 
	CNtlCameraController *pController;
	ListCameraController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); )
	{
		pController = (*it);
		if(!pController->IsPause())
		{
			if(!pController->IsCollisionCheck())
				bCollisionCheck = FALSE;

			if(!pController->IsInterpCheck())
				bInterpCheck = FALSE; 

			pController->Update(fElapsed);
		}

		if(pController->IsFinish())
		{
			pController->Exit();

			NTL_DELETE( pController );

			it = m_listController.erase(it);
			bDelete = TRUE;
		}
		else
			++it;
	}

	if(bDelete)
		CheckNormalControlPause();
	
	// ntl camera update
	RwV3d vCamPos = m_vCamPos + m_vOffset;
    RwV3d vLookAt = m_vLookAt + m_vLookAtOffset;

	if(m_bCollisionCheck && bCollisionCheck)
	{
		RwCamera* pCamera = m_pCamera->GetCamera();
		RwReal fX = 0, fY = 0, fZ = 0;
		for ( int i = 0; i < 4; ++i )
		{
			fX += pCamera->frustumCorners[i].x;
			fY += pCamera->frustumCorners[i].y;
			fZ += pCamera->frustumCorners[i].z;
		}
		fX /= 4.f; fX -= pCamera->frustumCorners[0].x;
		fY /= 4.f; fY -= pCamera->frustumCorners[0].y;
		fZ /= 4.f; fZ -= pCamera->frustumCorners[0].z;

		RwReal fRadius = sqrtf( fX * fX + fY * fY + fZ * fZ );
		fRadius *= 2.f;

		//////////////////////////////////////////////////////////////////////////
		//
		//	카메라 충돌
		//
		//////////////////////////////////////////////////////////////////////////

		UpdateCameraForWorld( vCamPos, vLookAt, fRadius );

		// Camera와 지형 지물과의 충돌 처리에 따른 위치 보정
		RwV3d vNewPos;
		RwBool bCollision = GetSceneManager()->GetCameraCollision( &vCamPos, &vLookAt, fRadius, vNewPos );

		//////////////////////////////////////////////////////////////////////////
		//
		//	카메라 보간
		//
		//////////////////////////////////////////////////////////////////////////

		//if ( bCollision )
		//{
		//	vCamPos = vNewPos;
		//}
		m_clCAMInterp.SetRadius( fRadius );

		if(m_bInterpCheck && bInterpCheck)
		{
			if ( bCollision )
			{
				m_clCAMInterp.ChangeInterpState( CCAMInterpolation::eCAM_INTERP_TYPE_RUN_FRONT, fElapsed, vNewPos, vLookAt, GetDist() );
			}
			else
			{
				m_clCAMInterp.ChangeInterpState( CCAMInterpolation::eCAM_INTERP_TYPE_RUN_BACK, fElapsed, vCamPos, vLookAt, GetDist() );
			}

			m_clCAMInterp.Update( fElapsed, vCamPos, vLookAt );
		}

		CollisionActor(vCamPos, vLookAt);
	}

	RwV3d vUpSub = vLookAt - vCamPos; 
	if ( RwV3dNormalize( &vUpSub, &vUpSub ) )
	{
		RwReal fUpDot = RwV3dDotProduct( &vUpSub, &m_vUp );
		if ( abs( fUpDot ) < 0.9999f )
		{
			API_PL_CameraLookAt(m_pCamera->GetCamera(), &vCamPos, &vLookAt, &m_vUp, m_fRoll);
		}
	}

    // SobObj들의 카메라 거리 처리    
    m_fCollisionObjCheckTime += fElapsed;
    if(m_fCollisionObjCheckTime > 0.2f)
    {

        if(m_bEnableCollisionObj)
            CollisionObj();

        m_fCollisionObjCheckTime = 0.0f;
    }
    
	CNtlMath::MathRwV3dAssign(&m_vOffset, 0.0f, 0.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(&m_vLookAtOffset, 0.0f, 0.0f, 0.0f);
}

void CNtlGameCameraManager::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventCameraSmoothingMove)
	{
		SNtlEventCameraSmoothingMove *pSmoothingMove = reinterpret_cast<SNtlEventCameraSmoothingMove*>( pMsg.pData );
		MoveSmoothing(pSmoothingMove->bEnable);
	}
	else if(pMsg.Id == g_EventCameraShake)
	{
		CNtlCameraShakeController *pShakeController = NTL_NEW CNtlCameraShakeController;
		AddController(pShakeController);
		CheckNormalControlPause();
	}
	else if(pMsg.Id == g_EventCameraDash)
	{
        // 넉다운 카메라가 있으면 해제한다.
        if(FindController(CAMERA_CONTROL_KNOCKDOWN_MATRIX))
        {
            RemoveController(CAMERA_CONTROL_KNOCKDOWN_MATRIX);
        }

        if(FindController(CAMERA_CONTROL_FPS))
            return;

		SNtlEventCameraDash *pDashCamera = reinterpret_cast<SNtlEventCameraDash*>( pMsg.pData );

		CNtlCameraDashController *pDashController = reinterpret_cast<CNtlCameraDashController*>( FindController(CAMERA_CONTROL_DASH) );
		if(pDashController)
		{
			pDashController->SetActiveActor(reinterpret_cast<CNtlSobActor*>( pDashCamera->pActor ));
			pDashController->SetDashSpeed(pDashCamera->fDashSpeed);
			pDashController->SetDashDest(&pDashCamera->vDest);
			pDashController->UpdateData();
		}
		else
		{
			pDashController = NTL_NEW CNtlCameraDashController;
			pDashController->SetActiveActor(reinterpret_cast<CNtlSobActor*>( pDashCamera->pActor ));
			pDashController->SetDashSpeed(pDashCamera->fDashSpeed);
			pDashController->SetDashDest(&pDashCamera->vDest);
			AddController(pDashController);
		}

		CheckNormalControlPause();
	}
	else if(pMsg.Id == g_EventCameraNpc)
	{
		SNtlEventCameraNpc *pNpcCamera = reinterpret_cast<SNtlEventCameraNpc*>( pMsg.pData );

		CNtlCameraNpcController *pNpcController = reinterpret_cast<CNtlCameraNpcController*>( FindController(CAMERA_CONTROL_NPC) );
		NTL_ASSERT(pNpcController == NULL, "CNtlGameCameraManager::HandleEvents => pMsg.Id == g_EventCameraNpc");

		pNpcController = NTL_NEW CNtlCameraNpcController;
		pNpcController->SetActiveActor(reinterpret_cast<CNtlSobActor*>( pNpcCamera->pActor ));
		AddController(pNpcController);
		
		CheckNormalControlPause();
	}
	else if(pMsg.Id == g_EventCameraDB)
	{
		SNtlEventCameraDB *pDBCamera = reinterpret_cast<SNtlEventCameraDB*>( pMsg.pData );

		CNtlCameraDragonBallController *pDBController = reinterpret_cast<CNtlCameraDragonBallController*>( FindController(CAMERA_CONTROL_DRAGONBALL) );
		NTL_ASSERT(pDBController == NULL, "CNtlGameCameraManager::HandleEvents => pMsg.Id == g_EventCameraDB");

		pDBController = NTL_NEW CNtlCameraDragonBallController;
		pDBController->SetActiveActor(reinterpret_cast<CNtlSobActor*>( pDBCamera->pActor ));
		AddController(pDBController);
		
		CheckNormalControlPause();
	}
    else if(pMsg.Id == g_EventCameraTimeMachine)
    {
        SNtlEventCameraTimeMachine* pEvent = reinterpret_cast<SNtlEventCameraTimeMachine*>(pMsg.pData);

        if(FindController(CAMERA_CONTROL_TIMEMACHINE))
            return;

        CNtlCameraTimeMachineController* pController = NTL_NEW CNtlCameraTimeMachineController();
        pController->SetActiveActor(reinterpret_cast<CNtlSobActor*>(pEvent->pObject));
        AddController(pController);

        CheckNormalControlPause();
    }
	else if(pMsg.Id == g_EventCameraExplosion)
	{
		SNtlEventCameraExplosion* pExplosionCamera = reinterpret_cast<SNtlEventCameraExplosion*>( pMsg.pData );

		// 기존에 Explosion이 동작 중에 있으면 해당 이벤트는 동작시키지 않는다
		if(FindController(CAMERA_CONTROL_EXPLOSION))
			return;

		// Explosion 객체 생성 및 등록
		CNtlSobCameraExplosion* pExplosionController = NTL_NEW CNtlSobCameraExplosion;
		pExplosionController->SetActiveActor(reinterpret_cast<CNtlSobActor*>(pExplosionCamera->pObject));
		AddController(pExplosionController);

		// Explosion lua script를 실행한다
		LuaExec_ExplosionCamera( pExplosionCamera->uiExplosionId );

		CheckNormalControlPause();
	}
	else if(pMsg.Id == g_EventCameraTimeMachineArrive)
	{
		SNtlEventCameraTimeMachineArrive* pEvent = reinterpret_cast<SNtlEventCameraTimeMachineArrive*>(pMsg.pData);

		if ( pEvent->bStart )
		{
			if(FindController(CAMERA_CONTROL_TIMEMACHINE_ARRIVE))
			{
				return;
			}

			CNtlCameraTimeMachineArriveController* pController = NTL_NEW CNtlCameraTimeMachineArriveController();
			pController->SetActiveActor(reinterpret_cast<CNtlSobActor*>(pEvent->pObject));
			pController->Start();

			AddController(pController);

			CheckNormalControlPause();
		}
		else
		{
			CNtlCameraTimeMachineArriveController* pTMArriveCtr = dynamic_cast< CNtlCameraTimeMachineArriveController* > (FindController(CAMERA_CONTROL_TIMEMACHINE_ARRIVE));

			if ( pTMArriveCtr )
			{
				pTMArriveCtr->End();
			}
		}
	}
    else if(pMsg.Id == g_EventCameraFree)
    {
		m_eActiveCamera			= ACTIVE_BASE_FREE;
		m_pBaseController		= m_pFreeController;
        m_pFreeController->Enter();
    }

	else if(pMsg.Id == g_EventCameraDTSplineNode)
	{
		/*
		SNtlEventCameraDTSplineNode *pDTSplineNode = reinterpret_cast<SNtlEventCameraDTSplineNode*>( pMsg.pData );

		CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
		if(pDTController == NULL)
		{
			pDTController = NTL_NEW CNtlCameraDTController;
			AddController(pDTController);
		}

		pDTController->AddSplineNode(pDTSplineNode->fDelayTime, pDTSplineNode->vStartPos, pDTSplineNode->vEndPos, pDTSplineNode->vLookAtPos, pDTSplineNode->byDirectLookAtType, pDTSplineNode->chKey, m_bDTNodeAttach);
		
		CheckNormalControlPause();
		*/
	}
	else if(pMsg.Id == g_EventCameraControlDelete)
	{
		SNtlEventCameraControlDelete *pControllerType = reinterpret_cast<SNtlEventCameraControlDelete*>( pMsg.pData );
		if(pControllerType->byControllerType == CAMERA_CONTROL_FREE)
		{
			m_eActiveCamera			= ACTIVE_BASE_THIRD;
			m_pBaseController		= m_pThirdController;
		}
		else
		{
			RemoveController(pControllerType->byControllerType);
			CheckNormalControlPause();
		}
	}
    else if(pMsg.Id == g_EventDBCScatter)       // 드래곤볼 흩어지는 카메라 연출을 위한 이벤트
    {
        CNtlCameraDragonBallController *pDBController = reinterpret_cast<CNtlCameraDragonBallController*>( FindController(CAMERA_CONTROL_DRAGONBALL) );
        if(!pDBController)
            return;

        pDBController->ChangeState(CNtlCameraDragonBallController::CAMERA_DB_STATE_SCATTER);
    }
	else if(pMsg.Id == g_EventCameraKnockDownMatrix)
	{
		SNtlEventCameraKnockDownMatrix *pKDMatrix = reinterpret_cast<SNtlEventCameraKnockDownMatrix*>( pMsg.pData );
		CNtlCameraKnockDownMatrixController *pKDMatrixController = reinterpret_cast<CNtlCameraKnockDownMatrixController*>( FindController(CAMERA_CONTROL_KNOCKDOWN_MATRIX) );

		if(pKDMatrixController == NULL)
		{
			pKDMatrixController = NTL_NEW CNtlCameraKnockDownMatrixController;
			pKDMatrixController->SetActiveActor(reinterpret_cast<CNtlSobActor*>( pKDMatrix->pActor ));
			pKDMatrixController->SetData(pKDMatrix->fLifeTime);
			AddController(pKDMatrixController);
			
			CheckNormalControlPause();
		}
	}
    else if(pMsg.Id == g_EventCameraFPS)            // 1인칭 카메라
    {
        if(FindController(CAMERA_CONTROL_FPS))
            return;

        SNtlEventCameraFPS* pData = reinterpret_cast<SNtlEventCameraFPS*>(pMsg.pData);
        CNtlCameraFPSController* pCameraFPS = NTL_NEW CNtlCameraFPSController();        
        pCameraFPS->SetActiveActor((const CNtlSobActor*)GetNtlSLGlobal()->GetSobAvatar());
        pCameraFPS->SetLookAtDir(pData->vCurrLookAt, pData->vTargetLookAt);
        AddController(pCameraFPS);

        CheckNormalControlPause();
    }
	else if(pMsg.Id == g_EventCameraDTClear)
	{
		RwBool bSuccess = RemoveController(CAMERA_CONTROL_DT);

		if(bSuccess)
			CheckNormalControlPause();
	}
	else if(pMsg.Id == g_EventCameraBus)
	{
		SNtlEventCameraControlBus* pEvent = reinterpret_cast<SNtlEventCameraControlBus*>( pMsg.pData );
		
		CNtlCameraBusController* pCameraBus = NTL_NEW CNtlCameraBusController();
		pCameraBus->SetActiveActor(pEvent->pActor);
		AddController(pCameraBus);

		CheckNormalControlPause();
	}
	else if(pMsg.Id == g_EventTLPlayCamera)
	{
		SNtlEventTLPlayCamera* pEvent = reinterpret_cast<SNtlEventTLPlayCamera*>( pMsg.pData );

		if(FindController(CAMERA_CONTROL_TURORIAL))
			return;

		CNtlCameraTutorialController* pTutorialCtrl = NTL_NEW CNtlCameraTutorialController;
		pTutorialCtrl->SetActiveActor((const CNtlSobActor*)GetNtlSLGlobal()->GetSobAvatar());
		pTutorialCtrl->SetData( pEvent->uiCameraType, pEvent->uiTblIdx, pEvent->vTargetPos );
		AddController(pTutorialCtrl);

		CheckNormalControlPause();
	}
    else if(pMsg.Id == g_EventKeyboardMove)
    {
        // Change the Yaw from the keyboard in the first person camera (scout) (by agebreak)
        CNtlCameraFPSController* pFPSController = (CNtlCameraFPSController*)FindController(CAMERA_CONTROL_FPS);
        if(!pFPSController)
            return;

        pFPSController->HandleEvents(pMsg);
    
    }
    else if(pMsg.Id == g_EventKeyboardMoveStop)
    {
        CNtlCameraFPSController* pFPSController = (CNtlCameraFPSController*)FindController(CAMERA_CONTROL_FPS);
        if(!pFPSController)
            return;

        pFPSController->HandleEvents(pMsg);
    }
    else if(pMsg.Id == g_EventCameraObserver)
    {
        SNtlEventCameraObserver* pEvent = (SNtlEventCameraObserver*)pMsg.pData;        
        CNtlCameraObserver* pCameraController = (CNtlCameraObserver*)FindController(CAMERA_CONTROL_OBSERVER);
        if(!pCameraController)                
        {
            pCameraController = NTL_NEW CNtlCameraObserver();            
            AddController(pCameraController);
        }
        
        CheckNormalControlPause();

        pCameraController->SetIndex(pEvent->eType, pEvent->nIndex);        
    }
}

void CNtlGameCameraManager::SetActiveCamera(const CNtlCamera *pCamera)
{
	m_pCamera = const_cast<CNtlCamera*>( pCamera );
}

void CNtlGameCameraManager::SetActiveActor(const CNtlSobActor *pSobActor)
{
	m_pActor = const_cast<CNtlSobActor*>( pSobActor );

	m_fPitch	= CAMERA_DEFAULT_PITCH;	
	m_fYaw		= 0.0f;
	m_fRoll		= 0.0f;

	if(m_pActor)
	{
		RwV3d vDir = m_pActor->GetDirection();
		m_fYaw		= CNtlMath::LineToAngleY(&vDir);
	}
	
	if(m_pThirdController)
		m_pThirdController->SetActiveActor(pSobActor);
	if(m_pFreeController)
		m_pFreeController->SetActiveActor(pSobActor);

	CNtlCameraController *pController;

	ListCameraController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); ++it)
	{
		pController = (*it);
		pController->SetActiveActor(pSobActor);
	}
}

void CNtlGameCameraManager::ZoomInOut(RwReal fDelta)
{
	if(m_pBaseController)
		m_pBaseController->ZoomInOut(fDelta);

	CNtlCameraController *pController;

	ListCameraController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); ++it)
	{
		pController = (*it);
		if(!pController->IsPause())
			pController->ZoomInOut(fDelta);
	}
}


void CNtlGameCameraManager::MoveSmoothing(RwBool bEnable)
{
	if(m_pBaseController)
		m_pBaseController->MoveSmoothing(bEnable);

	CNtlCameraController *pController;

	ListCameraController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); ++it)
	{
		pController = (*it);
		pController->MoveSmoothing(bEnable);
	}
}

void CNtlGameCameraManager::EnableUpdateData(RwBool bUpdateData)
{
	m_bUpdateData = bUpdateData;

	if(!m_bUpdateData)
	{
		MoveSmoothing(FALSE);
	}
}

void CNtlGameCameraManager::UserCameraControlEnable(RwBool bEnable)
{
	if(m_pBaseController)
		m_pBaseController->UserCameraControlEnable(bEnable);

	CNtlCameraController *pController;

	ListCameraController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); ++it)
	{
		pController = (*it);
		pController->UserCameraControlEnable(bEnable);
	}
}

/**
 * Folder가 없을시 Folder 생성을 위해서 항상 CreateDirectoty()를 호출을 한다.
 * png로 저장 할 경우 Edge가 흰색으로 저장되고 저장시 시간이 조금 걸린다.
 * (by HoDong 2006. 8. 5)
 */
void CNtlGameCameraManager::SetCaptureScreenShot()
{
	if(m_pCamera)
	{
		CreateDirectory("./ScreenShot",NULL);
		SYSTEMTIME systemTime;		
        GetLocalTime(&systemTime);
		
		RwChar szBuffer[256];
		sprintf_s(szBuffer, "./ScreenShot/%d-%02d-%02d-%02d-%02d-%02d.bmp", 
            systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

		RwCamera *pCamera = m_pCamera->GetMainCamera();
		if(pCamera)
		{
			RwRaster *pRaster = RwCameraGetRaster(pCamera);		
			API_PL_RasterSaveImage(pRaster, szBuffer);
            CNtlSLEventGenerator::ActionScreenShot(szBuffer);
		}
	}
}

void CNtlGameCameraManager::SetDist(RwReal fDist)
{
	if(!CheckZoomPossible())
		return;

	m_fDist = fDist;
}

void CNtlGameCameraManager::SetPitch(RwReal fPitch)
{
	if(!CheckRotatePossible())
    {        
		return;
    }
		
	m_fPitch = fPitch;

	if(m_fPitch < m_fMinPitch) 
		m_fPitch = m_fMinPitch;

	if(m_fPitch > m_fMaxPitch) 
		m_fPitch = m_fMaxPitch;
}

void CNtlGameCameraManager::SetYaw(RwReal fYaw)
{
	if(!CheckRotatePossible())
		return;

	m_fYaw = fYaw;

	// 카메라 각도는 0 ~ 360 을 사용한다. 
	if(m_fYaw >= 360.0f)
		m_fYaw -= 360.0f;

	if(m_fYaw < 0.0f) 
		m_fYaw += 360.0f;
}

void CNtlGameCameraManager::SetRoll(RwReal fRoll)
{
	m_fRoll = fRoll;
}

void CNtlGameCameraManager::AddPitch(RwReal fDeltaPitch)
{
	if(!CheckRotatePossible())
    {
        CNtlCameraFPSController* pFPSController = (CNtlCameraFPSController*)FindController(CAMERA_CONTROL_FPS);
        if(pFPSController)
        {
            pFPSController->AddPitch(fDeltaPitch);
        }

		return;
    }

	m_fPitch += fDeltaPitch;

	if(m_fPitch < m_fMinPitch) 
		m_fPitch = m_fMinPitch;

	if(m_fPitch > m_fMaxPitch) 
		m_fPitch = m_fMaxPitch;
}

void CNtlGameCameraManager::AddYaw(RwReal fDeltaYaw)
{
	if(!CheckRotatePossible())
    {
        CNtlCameraFPSController* pFPSController = (CNtlCameraFPSController*)FindController(CAMERA_CONTROL_FPS);
        if(pFPSController)
        {
            pFPSController->AddYaw(fDeltaYaw);
        }

        return;
    }

	m_fYaw += fDeltaYaw;
		
	// 카메라 각도는 0 ~ 360 을 사용한다. 
	if(m_fYaw >= 360.0f)
		m_fYaw -= 360.0f;

	if(m_fYaw < 0.0f) 
		m_fYaw += 360.0f;
}

void CNtlGameCameraManager::Reset(void)
{
	CNtlCameraController *pController;
	ListCameraController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); ++it)
	{
		pController = (*it);
		pController->Exit();

		NTL_DELETE( pController );
	}

	m_listController.clear();

	m_pBaseController = m_pThirdController;
	SetActiveActor(NULL);
	ResetAttribute();
}

void CNtlGameCameraManager::ResetAttribute(void)
{
	m_fDist = CAMERA_DERAULT_DIST;

	ResetRotate();
	
	m_vCamPos.x = 0.0f;
	m_vCamPos.y = 0.0f;
	m_vCamPos.z = 0.0f;

	m_vLookAt.x = 0.0f;
	m_vLookAt.y = 0.0f;
	m_vLookAt.z = 0.0f;

	m_vUp.x = 0.0f;
	m_vUp.y = -1.0f;
	m_vUp.z = 0.0f;

	m_vOffset.x = 0.0f;
	m_vOffset.y = 0.0f;
	m_vOffset.z = 0.0f;

	m_vLookAtOffset.x = 0.0f;
	m_vLookAtOffset.y = 0.0f;
	m_vLookAtOffset.z = 0.0f;

	m_bCollisionCheck = TRUE;	
}

void CNtlGameCameraManager::ResetRotate(void)
{
	m_fPitch	= CAMERA_DEFAULT_PITCH;	
	m_fYaw		= 0.0f;
	m_fRoll		= 0.0f;

	if(m_pActor)
	{
		RwV3d vDir = m_pActor->GetDirection();
		m_fYaw		= CNtlMath::LineToAngleY(&vDir);
	}
}

void CNtlGameCameraManager::ResetCamera(void)
{
	if ( m_pBaseController )
	{
		m_pBaseController->ResetCamera();
	}

	// 확장 camera control 
	CNtlCameraController *pController;
	ListCameraController::iterator it;
	for ( it = m_listController.begin(); it != m_listController.end(); ++it )
	{
		pController = (*it);

		if ( pController )
		{
			pController->ResetCamera();
		}
	}

	RwV3d vCamPos = m_vCamPos + m_vOffset;
	RwV3d vLookAt = m_vLookAt + m_vLookAtOffset;

	API_PL_CameraLookAt( m_pCamera->GetCamera(), &vCamPos, &vLookAt, &m_vUp, m_fRoll );

	CNtlMath::MathRwV3dAssign( &m_vOffset, 0.0f, 0.0f, 0.0f );
	CNtlMath::MathRwV3dAssign( &m_vLookAtOffset, 0.0f, 0.0f, 0.0f );
}

void CNtlGameCameraManager::SetFov(RwReal fFov)
{
	NTL_ASSERT(m_pCamera, "CNtlGameCameraManager::SetFov => camera instance is invalid");
	m_pCamera->SetCalcFov(fFov);
}


RwReal CNtlGameCameraManager::GetFov(void) const
{
	return m_pCamera->GetFov();
}

void CNtlGameCameraManager::SetHeightRatioBonus(float fBonus)
{
	if (m_pThirdController)
		m_pThirdController->SetTargetHeightRatioBonus(fBonus * 0.06f);
}

float CNtlGameCameraManager::GetHeightRatioBonus()
{
	return m_fHeightRatioBonus;
}

void CNtlGameCameraManager::SetCameraPos(const RwV3d *pPos)
{
	RwV3dAssignMacro(&m_vCamPos, pPos);
}

const RwV3d* CNtlGameCameraManager::GetCameraPos(void)
{
	return &m_vCamPos;
}

const RwV3d* CNtlGameCameraManager::GetCameraDir(void)
{
	RwCamera *pRwCamera = m_pCamera->GetCamera();
	
	RwFrame *pFrame = RwCameraGetFrame( pRwCamera );
	RwMatrix *pMatrix = RwFrameGetMatrix( pFrame );

	return &pMatrix->at;
}

void CNtlGameCameraManager::SetCollisionCheck(RwBool bCollisionCheck)
{
	m_bCollisionCheck = bCollisionCheck;
}

void CNtlGameCameraManager::SetInterpCheck(RwBool bInterpCheck)
{
	m_bInterpCheck = bInterpCheck;
}

void CNtlGameCameraManager::SetDTNodeAttach(RwBool bAttach)
{
	m_bDTNodeAttach = bAttach;
}

void CNtlGameCameraManager::SetDTAttributeRestore(RwBool bRestore)
{
	m_bDTAttrRestore = bRestore;
}

void CNtlGameCameraManager::AddDTSoftActorDistNode(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
												   RwReal fDistance, RwReal fLifeTime, RwV3d vLookAt, CINEMATIC_HANDLE hCinematic /*= INVALID_CINEMATIC_ID*/)
{
	CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
	if(pDTController == NULL)
	{
		pDTController = NTL_NEW CNtlCameraDTController;
		pDTController->SetAttributeRestore(m_bDTAttrRestore);
		AddController(pDTController);
	}

	pDTController->AddDTSoftActorDistNode(hSerialId, fHeightOffset, fAngleX, fAngleY, fDistance, fLifeTime, vLookAt, m_bDTNodeAttach, hCinematic);
	
	CheckNormalControlPause();
}


void CNtlGameCameraManager::AddDTSoftActorDynamicLookAtDistNode(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
																RwReal fDistance, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic /*= INVALID_CINEMATIC_ID*/)
{
	CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
	if(pDTController == NULL)
	{
		pDTController = NTL_NEW CNtlCameraDTController;
		pDTController->SetAttributeRestore(m_bDTAttrRestore);
		AddController(pDTController);
	}

	pDTController->AddDTSoftActorDynamicLookAtDistNode(hSerialId, fHeightOffset, fAngleX, fAngleY, fDistance, fLifeTime, m_bDTNodeAttach, hCinematic);
	
	CheckNormalControlPause();
}

void CNtlGameCameraManager::AddDTSoftCurrentRotateX(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic /*= INVALID_CINEMATIC_ID*/)
{
	CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
	if(pDTController == NULL)
	{
		pDTController = NTL_NEW CNtlCameraDTController;
		pDTController->SetAttributeRestore(m_bDTAttrRestore);
		AddController(pDTController);
	}

	pDTController->AddDTSoftCurrentRotateX(fLastAngle, fInterTime, fLifeTime, m_bDTNodeAttach, hCinematic);
	
	CheckNormalControlPause();
}

void CNtlGameCameraManager::AddDTSoftCurrentRotateY(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic /*= INVALID_CINEMATIC_ID*/)
{
	CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
	if(pDTController == NULL)
	{
		pDTController = NTL_NEW CNtlCameraDTController;
		pDTController->SetAttributeRestore(m_bDTAttrRestore);
		AddController(pDTController);
	}

	pDTController->AddDTSoftCurrentRotateY(fLastAngle, fInterTime, fLifeTime, m_bDTNodeAttach, hCinematic);
	
	CheckNormalControlPause();
}


void CNtlGameCameraManager::AddDTSoftCurrentRotateZ(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic /*= INVALID_CINEMATIC_ID*/)
{
	CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
	if(pDTController == NULL)
	{
		pDTController = NTL_NEW CNtlCameraDTController;
		pDTController->SetAttributeRestore(m_bDTAttrRestore);
		AddController(pDTController);
	}

	pDTController->AddDTSoftCurrentRotateZ(fLastAngle, fInterTime, fLifeTime, m_bDTNodeAttach, hCinematic);
	
	CheckNormalControlPause();
}

void CNtlGameCameraManager::AddDTSoftCurrentFov(RwReal fLastFov, RwReal fInterTime, CINEMATIC_HANDLE hCinematic /*= INVALID_CINEMATIC_ID*/)
{
	CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
	if(pDTController == NULL)
	{
		pDTController = NTL_NEW CNtlCameraDTController;
		pDTController->SetAttributeRestore(m_bDTAttrRestore);
		AddController(pDTController);
	}

	pDTController->AddDTSoftCurrentFov(fLastFov, fInterTime, m_bDTNodeAttach, hCinematic);
	
	CheckNormalControlPause();
}

void CNtlGameCameraManager::AddDTSoftPauseNode(RwReal fLifeTime, RwBool bLoop, CINEMATIC_HANDLE hCinematic /*= INVALID_CINEMATIC_ID*/)
{
	CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
	if(pDTController == NULL)
	{
		pDTController = NTL_NEW CNtlCameraDTController;
		pDTController->SetAttributeRestore(m_bDTAttrRestore);
		AddController(pDTController);
	}

	pDTController->AddDTSoftPauseNode(fLifeTime, bLoop, m_bDTNodeAttach, hCinematic);
	
	CheckNormalControlPause();
}

void CNtlGameCameraManager::AddDTSplineSelfNode(RwV3d vPos, RwReal fAngleX, RwReal fAngleY, RwReal fAngleZ, const RwChar *pPropKey, CINEMATIC_HANDLE hCinematic /*= INVALID_CINEMATIC_ID*/)
{
    CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_OBSERVER) );
    if(!pDTController)
    {
        pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
        if(pDTController == NULL)
        {
            pDTController = NTL_NEW CNtlCameraDTController;
            pDTController->SetAttributeRestore(m_bDTAttrRestore);
            AddController(pDTController);
        }
    }	

	pDTController->AddSplineSelfNode(vPos, fAngleX, fAngleY, fAngleZ, pPropKey, m_bDTNodeAttach, hCinematic);
	
	CheckNormalControlPause();
}

void CNtlGameCameraManager::AddDTSplineSelfNodeBeginActor(SERIAL_HANDLE hSerialId, const RwChar *pPropKey, CINEMATIC_HANDLE hCinematic /*= INVALID_CINEMATIC_ID*/)
{
	CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
	if(pDTController == NULL)
	{
		pDTController = NTL_NEW CNtlCameraDTController;
		pDTController->SetAttributeRestore(m_bDTAttrRestore);
		AddController(pDTController);
	}

	pDTController->AddSplineSelfNodeBeginActorRef(hSerialId, pPropKey, m_bDTNodeAttach, hCinematic);
	
	CheckNormalControlPause();
}


void CNtlGameCameraManager::AddDTSplineSelfNodeActor(SERIAL_HANDLE hSerialId, const RwChar *pPropKey, CINEMATIC_HANDLE hCinematic /*= INVALID_CINEMATIC_ID*/)
{
	CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
	if(pDTController == NULL)
	{
		pDTController = NTL_NEW CNtlCameraDTController;
		pDTController->SetAttributeRestore(m_bDTAttrRestore);
		AddController(pDTController);
	}

	pDTController->AddSplineSelfNodeActorRef(hSerialId, pPropKey, m_bDTNodeAttach, hCinematic);
	
	CheckNormalControlPause();
}


void CNtlGameCameraManager::AddDTSplineCurrentDynamicLookAtNode(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic /*= INVALID_CINEMATIC_ID*/)
{
	CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
	if(pDTController == NULL)
	{
		pDTController = NTL_NEW CNtlCameraDTController;
		pDTController->SetAttributeRestore(m_bDTAttrRestore);
		AddController(pDTController);
	}

	pDTController->AddDTSplineCurrentDynamicLookAtNode(hSerialId, fHeightOffset, pPropKey, fLifeTime, m_bDTNodeAttach, hCinematic);
	
	CheckNormalControlPause();
}


void CNtlGameCameraManager::AddDTSplineActorPositionDynamicLookAtNode(SERIAL_HANDLE hPosSerialId, SERIAL_HANDLE hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic /*= INVALID_CINEMATIC_ID*/)
{
    CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_OBSERVER) );
    if(!pDTController)
    {
        pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
        if(pDTController == NULL)
        {
            pDTController = NTL_NEW CNtlCameraDTController;
            pDTController->SetAttributeRestore(m_bDTAttrRestore);
            AddController(pDTController);
        }
    }

	pDTController->AddDTSplineActorPositionDynamicLookAtNode(hPosSerialId, hLookAtSerialId, fHeightOffset, pPropKey, fLifeTime, m_bDTNodeAttach, hCinematic);
	
	CheckNormalControlPause();
}

void CNtlGameCameraManager::AddDTSplineActorPositionDyncmicLookAtCenterNode(SERIAL_HANDLE hPosSerialId, SERIAL_HANDLE hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic /*= INVALID_CINEMATIC_ID*/)
{
	CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_OBSERVER) );
    if(!pDTController)
    {
        pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
        if(pDTController == NULL)
        {
            pDTController = NTL_NEW CNtlCameraDTController;
            pDTController->SetAttributeRestore(m_bDTAttrRestore);
            AddController(pDTController);
        }
    }


	pDTController->AddDTSplineActorPositionDynamicLookAtCenterNode(hPosSerialId, hLookAtSerialId, fHeightOffset, pPropKey, fLifeTime, m_bDTNodeAttach, hCinematic);
	
	CheckNormalControlPause();
}


void CNtlGameCameraManager::AddDTSplinePositionDynamicLookAtNode(RwV3d vStartPos, RwReal fAngleY, SERIAL_HANDLE  hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic /*= INVALID_CINEMATIC_ID*/)
{
    CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_OBSERVER) );
    if(!pDTController)
    {
        pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
        if(pDTController == NULL)
        {
            pDTController = NTL_NEW CNtlCameraDTController;
            pDTController->SetAttributeRestore(m_bDTAttrRestore);
            AddController(pDTController);
        }
    }

	pDTController->AddDTSplinePositionDynamicLookAtNode(vStartPos, fAngleY, hLookAtSerialId, fHeightOffset, pPropKey, fLifeTime, m_bDTNodeAttach, hCinematic);
	
	CheckNormalControlPause();
}


void CNtlGameCameraManager::ClearLoopPauseNode(void)
{
	CNtlCameraDTController *pDTController = reinterpret_cast<CNtlCameraDTController*>( FindController(CAMERA_CONTROL_DT) );
	if(pDTController == NULL)
		return;

	pDTController->ClearLoopPauseNode();
}

void CNtlGameCameraManager::FinishLock(void)
{
	m_bFinishLock = TRUE;
}

void CNtlGameCameraManager::FinishUnLock(void)
{
	m_bFinishLock = FALSE;
}

RwBool CNtlGameCameraManager::IsFinishLock(void)
{
	return m_bFinishLock;
}

void CNtlGameCameraManager::FreeCameraMove(RwUInt32 moveType, RwReal fSpeed)
{
	if(m_pBaseController != m_pFreeController)
		return;

    if(fSpeed == 0.0f)
        fSpeed = CNtlFreeCameraController::GetMoveSpeed();

    CNtlFreeCameraController* pFreeCamera = dynamic_cast<CNtlFreeCameraController*>(m_pBaseController);
    if ( pFreeCamera ) pFreeCamera->Move(moveType, fSpeed);
}

RwBool CNtlGameCameraManager::IsActiveFreeCamera() 
{
	if(m_pBaseController == m_pFreeController)
		return TRUE;
	
	return FALSE;
}

void CNtlGameCameraManager::FreeCameraMoveStop() 
{
	if(m_pBaseController != m_pFreeController)
		return;

    CNtlFreeCameraController* pFreeCamera = dynamic_cast<CNtlFreeCameraController*>(m_pBaseController);
    if ( pFreeCamera ) pFreeCamera->Stop();
}


void CNtlGameCameraManager::AddSinExplosionAlgorithm_XAxis( RwUInt32 uiExcuteID, RwReal fLifeTime, RwReal fFrequancy, RwReal fPhase, RwReal fAmp, RwReal fDescFactor )
{
	CNtlSobCameraExplosion* pCameraExplosion = dynamic_cast< CNtlSobCameraExplosion* > ( FindController( CAMERA_CONTROL_EXPLOSION ) );

	if ( pCameraExplosion ) pCameraExplosion->AddSinExplosionAlgorithm_X( uiExcuteID, fLifeTime, fFrequancy, fPhase, fAmp, fDescFactor );
}


void CNtlGameCameraManager::AddSinExplosionAlgorithm_YAxis( RwUInt32 uiExcuteID, RwReal fLifeTime, RwReal fFrequancy, RwReal fPhase, RwReal fAmp, RwReal fDescFactor )
{
	CNtlSobCameraExplosion* pCameraExplosion = dynamic_cast< CNtlSobCameraExplosion* > ( FindController( CAMERA_CONTROL_EXPLOSION ) );

	if ( pCameraExplosion ) pCameraExplosion->AddSinExplosionAlgorithm_Y( uiExcuteID, fLifeTime, fFrequancy, fPhase, fAmp, fDescFactor );
}


void CNtlGameCameraManager::AddSinExplosionAlgorithm_ZAxis( RwUInt32 uiExcuteID, RwReal fLifeTime, RwReal fFrequancy, RwReal fPhase, RwReal fAmp, RwReal fDescFactor )
{
	CNtlSobCameraExplosion* pCameraExplosion = dynamic_cast< CNtlSobCameraExplosion* > ( FindController( CAMERA_CONTROL_EXPLOSION ) );

	if ( pCameraExplosion ) pCameraExplosion->AddSinExplosionAlgorithm_Z( uiExcuteID, fLifeTime, fFrequancy, fPhase, fAmp, fDescFactor );
}

RwReal CNtlGameCameraManager::FreeCameraGetMoveSpeed() 
{
    return CNtlFreeCameraController::GetMoveSpeed();
}

void CNtlGameCameraManager::FreeCameraSetMoveSpeed( RwReal fSpeed ) 
{
    CNtlFreeCameraController::SetMoveSpeed(fSpeed);
}

