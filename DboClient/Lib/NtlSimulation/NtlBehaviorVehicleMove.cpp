#include "precomp_ntlsimulation.h"
#include "NtlBehaviorVehicleMove.h"

#include "NtlMath.h"
#include "GraphicDataTable.h"
#include "NtlPLEvent.h"
#include "NtlSLVisualDeclear.h"

#include "NtlDebug.h"
#include "NtlFSMDef.h"
#include "NtlBehaviorData.h"
#include "NtlSobActor.h"
#include "NtlSobProxy.h"
#include "NtlSobVehicle.h"
#include "NtlSobManager.h"
#include "NtlMovement.h"
#include "NtlSLApi.h"
#include "NtlSLPacketGenerator.h"
#include "DboMovingAlgorithm.h"
#include "NtlSobPlayer.h"


// jump
#define JUMP_GRAVITY_SPEED					11.0f

// collision
#define MOVE_POSITION_CHANGE_CHECK_TIME		(0.5f)
#define MOVE_POSITION_CHANGE_LIMIT_RANGE	(0.2f)
#define COLLISION_MOVE_IMPOSSIBLE_COUNT		(3)

// move sync send
#define MOVE_SYNC_SEND_TIME					(0.5f)

// falling
#define FALLING_CHECK_LEN					(2.f)
#define FALLING_WALK_LEN					(1.f)


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


CNtlBehaviorVehicleMoveBase::CNtlBehaviorVehicleMoveBase( void )
{
	m_fVelSumTime		= 0.f;

	m_fInitAccelerator	= 0.f;
	m_fDefAccelerator	= 0.f;

	m_fDefVelocity		= 0.f;
	m_fPreVelocity		= 0.f;
	m_fCurVelocity		= 0.f;

	m_fRotVelocity		= 4.f;
	m_fYDirInterpRatio	= 1.f;

	m_eMoveType			= eVEHICLE_MOVE_TYPE_INVALID;
	m_eTurnType			= eVEHICLE_TRUN_TYPE_INVALID;

	m_bStop				= FALSE;

	m_fPosChangeTime	= 0.f;
	m_byPosChangeCnt	= 0;

	m_byCollMoveImpossCnt	= 0;

	m_bServerSyncPos	= FALSE;
	m_fServerSyncDist	= 0.f;
	m_fCurrServerSyncDist = 0.f;
	m_fDeltaServerSyncDist = 0.f;

	m_bSyncSendCheck	= FALSE;
	m_fSyncSendTime		= 0.f;

	m_byGroundType		= eVEHICLE_GROUND_TYPE_INVALID;
	m_pPLRunSmoke		= NULL;
	m_pPLRunSpray		= NULL;
}

CNtlBehaviorVehicleMoveBase::~CNtlBehaviorVehicleMoveBase( void )
{
	m_byGroundType = eVEHICLE_GROUND_TYPE_INVALID;
	DestroyRunSmokeEffect();
	DestroyRunSprayEffect();
}

void CNtlBehaviorVehicleMoveBase::Enter( void )
{
	AttachPlayer( m_pActor->GetOwnerID(), m_pActor->GetSerialID() );

	CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();

	memcpy( &m_MoveStuff, pMoveStuff, sizeof(SMoveStuff) );

	m_fVelSumTime		= 0.f;

	m_fInitAccelerator	= Logic_GetFrontRunSpeed( (CNtlSobActor*)m_pPlayer );
	m_fDefAccelerator	= Logic_GetFrontRunSpeed( (CNtlSobActor*)m_pPlayer );

	m_fDefVelocity		= Logic_GetFrontRunSpeed( (CNtlSobActor*)m_pPlayer );
	m_fPreVelocity		= Logic_GetFrontRunSpeed( (CNtlSobActor*)m_pPlayer );
	m_fCurVelocity		= Logic_GetFrontRunSpeed( (CNtlSobActor*)m_pPlayer );

	m_bStop				= FALSE;

	SetServerSyncPosition( m_MoveStuff.vCurrLoc );
	SetSyncSendCheck( FALSE );
}

void CNtlBehaviorVehicleMoveBase::Exit( void )
{
	m_byGroundType = eVEHICLE_GROUND_TYPE_INVALID;

	DestroyRunSmokeEffect();
	DestroyRunSprayEffect();

	DetachPlayer();
}

void CNtlBehaviorVehicleMoveBase::Update( RwReal fElapsed )
{
	CNtlBehaviorBase::Update( fElapsed );
}

RwUInt32 CNtlBehaviorVehicleMoveBase::HandleEvents( RWS::CMsg& pMsg )
{
	UpdateEvent( pMsg );

	if ( pMsg.Id == g_EventAnimEnd )
	{
		SNtlEventAnimEnd* pData = (SNtlEventAnimEnd*) pMsg.pData;

		if ( pData->uiBaseAnimKey == VEHICLE_STOP )
		{
			Finish();
		}
	}

	return CNtlBehaviorBase::HandleEvents( pMsg );
}

void CNtlBehaviorVehicleMoveBase::UpdateData( void )
{
	CNtlBeCharData* pBeData = (CNtlBeCharData*)m_pActor->GetBehaviorData();
	SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();

	memcpy( &m_MoveStuff, pMoveStuff, sizeof(SMoveStuff) );

	if ( m_bStop )
	{
		m_fVelSumTime		= 0.f;

		m_fInitAccelerator	= Logic_GetFrontRunSpeed( (CNtlSobActor*)m_pPlayer );
		m_fDefAccelerator	= Logic_GetFrontRunSpeed( (CNtlSobActor*)m_pPlayer );

		m_fDefVelocity		= Logic_GetFrontRunSpeed( (CNtlSobActor*)m_pPlayer );
		m_fPreVelocity		= Logic_GetFrontRunSpeed( (CNtlSobActor*)m_pPlayer );
		m_fCurVelocity		= Logic_GetFrontRunSpeed( (CNtlSobActor*)m_pPlayer );

		m_bStop				= FALSE;

		SetServerSyncPosition( m_MoveStuff.vCurrLoc );
		SetSyncSendCheck( FALSE );
	}
}

void CNtlBehaviorVehicleMoveBase::SetStop( void )
{
	m_bStop = TRUE;

	m_fPreVelocity = m_fCurVelocity;
	m_fCurVelocity = 0.f;
}

void CNtlBehaviorVehicleMoveBase::UpdateVelocity( RwReal fElapsed )
{
	if ( !m_bStop )
	{
		m_fVelSumTime += fElapsed;
		m_fPreVelocity = m_fCurVelocity;
		m_fCurVelocity = ((m_fInitAccelerator + (m_fDefAccelerator * m_fVelSumTime * m_fVelSumTime) / 2.f) * m_fVelSumTime) * ((eVEHICLE_GROUND_TYPE_WATER == m_byGroundType) ? DBO_SWIMMING_SPEED_RATIO : 1.f);
		m_fCurVelocity = (m_fCurVelocity > m_fDefVelocity ? m_fDefVelocity : m_fCurVelocity );
	}
}

void CNtlBehaviorVehicleMoveBase::UpdateActorDir( RwReal fElapsed, RwV3d& vCurDir, RwV3d& vNextDir, RwV3d& vResultDir )
{
	RwV3d vSrcDir = vCurDir; vSrcDir.y = 0.f;

	if ( RwV3dNormalize( &vSrcDir, &vSrcDir ) <= 0.0001f )
	{
		vSrcDir.x = vSrcDir.y = 0.f;
		vSrcDir.z = 1.f;
	}

	RwV3d vDestDir = vNextDir; vDestDir.y = 0.f;

	if ( RwV3dNormalize( &vDestDir, &vDestDir ) <= 0.0001f )
	{
		vDestDir.x = vDestDir.y = 0.f;
		vDestDir.z = 1.f;
	}

	RwV3d vRotDir = vDestDir - vSrcDir;

	RwReal fRotDirLen = RwV3dNormalize( &vRotDir, &vRotDir );

	if ( fRotDirLen <= 0.0001f )
	{
		vResultDir = vDestDir;
	}
	else
	{
		if ( fRotDirLen - (m_fRotVelocity * fElapsed) < 0.0001f  )
		{
			vResultDir = vDestDir;
		}
		else
		{
			vResultDir = vSrcDir + vRotDir * (m_fRotVelocity * fElapsed );
			RwV3dNormalize( &vResultDir, &vResultDir );
		}
	}
}

void CNtlBehaviorVehicleMoveBase::UpdateActorSlop( RwReal fElapsed, RwV3d& vNextDir, RwV3d& vNextPos, RwReal fDirY, RwV3d& vResultDir, RwV3d& vResultPos )
{
	Logic_GetWorldSlopForVehicle( m_pVehicle, vNextPos, vNextDir, m_pActor->GetSobProxy()->GetPLEntityDepth() / 3.f, vResultPos, vResultDir, 10.f, 10.f );

	RwReal fCurYDir = fDirY;
	RwReal fLerpYDir = vResultDir.y - fCurYDir;

	fCurYDir += fLerpYDir * m_fYDirInterpRatio * fElapsed;

	if ( fLerpYDir > 0.0001f && fCurYDir > vResultDir.y )
	{
		fCurYDir = vResultDir.y;
	}
	else if ( fLerpYDir < 0.0001f && fCurYDir < vResultDir.y )
	{
		fCurYDir = vResultDir.y;
	}

	vResultDir.y = fCurYDir;

	RwV3dNormalize( &vResultDir, &vResultDir );
}

void CNtlBehaviorVehicleMoveBase::UpdatePosition( const RwV3d& vCurPos, const RwV3d& vDestPos, RwV3d& vResultPos )
{
	vResultPos = vDestPos;

	if ( vCurPos.y < vDestPos.y )
	{
		RwReal fHeightGap = vDestPos.y - vCurPos.y;
		RwReal fCurRideOnHeight = m_pVehicle->GetRideOnHeight() / 100.f;

		if ( fCurRideOnHeight - fHeightGap < 0.f )
		{
			vResultPos.y -= m_pVehicle->GetRideOnHeight() / 100.f;
			m_pVehicle->SetRideOnHeight( 0.f );
		}
		else if ( fCurRideOnHeight - fHeightGap < 0.35f )
		{
			vResultPos.y -= (fCurRideOnHeight - fHeightGap);
			m_pVehicle->SetRideOnHeight( m_pVehicle->GetRideOnHeight() - (fCurRideOnHeight - fHeightGap) );
		}
	}
	else if ( vCurPos.y > vDestPos.y )
	{
		RwReal fMaxRideOnHeight = m_pVehicle->GetMaxRideOnHeight() / 100.f;
		RwReal fCurRideOnHeight = m_pVehicle->GetRideOnHeight() / 100.f;

		if ( fMaxRideOnHeight - fCurRideOnHeight > 0.f )
		{
			vResultPos.y += (fMaxRideOnHeight - fCurRideOnHeight);
			m_pVehicle->SetRideOnHeight( fMaxRideOnHeight );
		}
	}
}

void CNtlBehaviorVehicleMoveBase::UpdateAnim( void )
{
	switch ( m_MoveStuff.byMoveFlags )
	{
		case NTL_MOVE_F:
		case NTL_MOVE_B:
		case NTL_MOVE_L:
		case NTL_MOVE_R:
		case NTL_MOVE_MOUSE_MOVEMENT:
		case NTL_MOVE_FOLLOW_MOVEMENT:
		{
			if ( m_fCurVelocity < m_fDefVelocity )
			{
				if ( m_fPreVelocity < m_fCurVelocity )
				{
					if ( eVEHICLE_MOVE_TYPE_SPEED_UP != m_eMoveType )
					{
						m_eMoveType = eVEHICLE_MOVE_TYPE_SPEED_UP;

						Vehicle_Anim_Start();
					}
				}
				else
				{
					if ( eVEHICLE_MOVE_TYPE_SPEED_DOWN != m_eMoveType )
					{
						m_eMoveType = eVEHICLE_MOVE_TYPE_SPEED_DOWN;

						Vehicle_Anim_Stop();
					}
				}
			}
			else
			{
				if ( eVEHICLE_MOVE_TYPE_UNIFORM_VELOCITY != m_eMoveType )
				{
					m_eMoveType = eVEHICLE_MOVE_TYPE_UNIFORM_VELOCITY;

					Vehicle_Anim_Run();
				}
			}

			m_eTurnType = eVEHICLE_TRUN_TYPE_INVALID;
		}
		break;

		case NTL_MOVE_TURN_L:
		case NTL_MOVE_F_TURN_L:
		case NTL_MOVE_B_TURN_L:
		case NTL_MOVE_L_TURN_L:
		case NTL_MOVE_R_TURN_L:
		case NTL_MOVE_F_TURN_L_JUMP:
		case NTL_MOVE_B_TURN_L_JUMP:
		case NTL_MOVE_L_TURN_L_JUMP:
		case NTL_MOVE_R_TURN_L_JUMP:
		{
			if ( eVEHICLE_TRUN_TYPE_LEFT != m_eTurnType )
			{
				m_eTurnType = eVEHICLE_TRUN_TYPE_LEFT;

				Vehicle_Anim_TurnLeft();
			}

			m_eMoveType = eVEHICLE_MOVE_TYPE_INVALID;
		}
		break;

		case NTL_MOVE_TURN_R:
		case NTL_MOVE_F_TURN_R:
		case NTL_MOVE_B_TURN_R:
		case NTL_MOVE_L_TURN_R:
		case NTL_MOVE_R_TURN_R:
		case NTL_MOVE_F_TURN_R_JUMP:
		case NTL_MOVE_B_TURN_R_JUMP:
		case NTL_MOVE_L_TURN_R_JUMP:
		case NTL_MOVE_R_TURN_R_JUMP:
		{
			if ( eVEHICLE_TRUN_TYPE_RIGHT != m_eTurnType )
			{
				m_eTurnType = eVEHICLE_TRUN_TYPE_RIGHT;

				Vehicle_Anim_TurnRight();
			}

			m_eMoveType = eVEHICLE_MOVE_TYPE_INVALID;
		}
		break;

		default:
		{
			m_eMoveType = eVEHICLE_MOVE_TYPE_INVALID;
			m_eTurnType = eVEHICLE_TRUN_TYPE_INVALID;

			Vehicle_Anim_Run();
		}
		break;
	}
}

void CNtlBehaviorVehicleMoveBase::CheckSwimming( void )
{
	RwUInt8 byGroundType;

	SWorldHeightStuff hStuff;
	if ( !Logic_IsSwimmingActor( m_pActor, &m_pActor->GetPosition(), hStuff ) )
	{
		byGroundType = eVEHICLE_GROUND_TYPE_GROUND;
	}
	else
	{
		byGroundType = eVEHICLE_GROUND_TYPE_WATER;
	}

	if ( m_byGroundType != byGroundType )
	{
		switch ( m_byGroundType )
		{
		case eVEHICLE_GROUND_TYPE_GROUND:
			{
				DestroyRunSmokeEffect();
			}
			break;
		case eVEHICLE_GROUND_TYPE_WATER:
			{
				DestroyRunSprayEffect();
			}
			break;
		}

		switch ( byGroundType )
		{
		case eVEHICLE_GROUND_TYPE_GROUND:
			{
				CreateRunSmokeEffect();
			}
			break;

		case eVEHICLE_GROUND_TYPE_WATER:
			{
				CreateRunSprayEffect();
			}
			break;
		}

		m_byGroundType = byGroundType;
	}
}

void CNtlBehaviorVehicleMoveBase::UpdateSwimEffect( void )
{
	RwV3d vPos = m_pActor->GetPosition();
	RwV3d vDir = m_pActor->GetDirection();

	vPos.y = m_sHStuff.fFinialHeight + 0.1f;

	RwMatrix mat;
	RwMatrixSetIdentity( &mat );
	CNtlMath::MathRwMatrixAssign( &mat, &vPos, &vDir );

	if ( m_pPLRunSmoke )
	{
		m_pPLRunSmoke->SetMatrix( mat );
	}

	if ( m_pPLRunSpray )
	{
		m_pPLRunSpray->SetMatrix( mat );
	}
}

void CNtlBehaviorVehicleMoveBase::SetServerSyncPosition( RwV3d vSyncPos )
{
	if ( m_pPlayer->GetFlags() & SLFLAG_SERVER_SENDER )
	{
		return;
	}

	RwV3d vSub;
	RwV3d vPos = m_pActor->GetPosition();
	RwV3dSubMacro( &vSub, &vSyncPos, &vPos );

	vSub.y = 0;

	m_bServerSyncPos		= TRUE;
	m_fCurrServerSyncDist	= 0.0f;
	m_fDeltaServerSyncDist	= 0.0f;
	m_fServerSyncDist		= RwV3dLength( &vSub );

	RwV3dNormalize( &m_vServerSyncDir, &vSub );
}

void CNtlBehaviorVehicleMoveBase::UpdateServerSyncPosition( RwReal fElapsed )
{
	if ( !m_bServerSyncPos )
	{
		return;
	}

	if ( m_fServerSyncDist == m_fCurrServerSyncDist )
	{
		m_bServerSyncPos = FALSE;
		return;
	}

	m_fDeltaServerSyncDist = m_fServerSyncDist * fElapsed;

	if ( m_fCurrServerSyncDist + m_fDeltaServerSyncDist > m_fServerSyncDist )
	{
		m_fDeltaServerSyncDist = m_fServerSyncDist - m_fCurrServerSyncDist;
		m_fCurrServerSyncDist = m_fServerSyncDist;
	}
	else
	{
		m_fCurrServerSyncDist += m_fDeltaServerSyncDist;
	}
}

void CNtlBehaviorVehicleMoveBase::SetSyncSendCheck( RwBool bSyncSendCheck )
{
	if ( m_bSyncSendCheck != bSyncSendCheck )
	{
		m_fSyncSendTime = 0.f;
	}

	m_bSyncSendCheck = bSyncSendCheck;
}

void CNtlBehaviorVehicleMoveBase::UpdateSendSyncCheck( RwReal fElapsed )
{
	if ( !m_bSyncSendCheck )
	{
		return;
	}

	if ( !(m_pPlayer->GetFlags() & SLFLAG_SERVER_SENDER) )
	{
		return;
	}

	m_fSyncSendTime += fElapsed;

	if ( m_fSyncSendTime >= MOVE_SYNC_SEND_TIME )
	{
		if ( m_MoveStuff.byType == NTL_MOVETARGET_DIR )
		{
			//RwV3d vPos				= m_pActor->GetPosition();
			//RwV3d vDir				= m_pActor->GetDirection();

			//API_GetSLPacketGenerator()->SendChangePosition( vPos, vDir );
		}
		else if ( m_MoveStuff.byType == NTL_MOVETARGET_LOC )
		{
			//RwV3d vPos				= m_pActor->GetPosition();
			//RwUInt8 byAvatarType	= m_pActor->GetServerSyncAvatarType();
			//API_GetSLPacketGenerator()->SendMouseMove( vPos, m_MoveStuff.vDest, byAvatarType );
		}

		m_fSyncSendTime -= MOVE_SYNC_SEND_TIME;
	}
}

RwBool CNtlBehaviorVehicleMoveBase::LimitPositionChangeCheck( RwReal fElapsed, RwV3d& vDestPos )
{
	m_fPosChangeTime += fElapsed;

	m_byPosChangeCnt++;

	if ( m_fPosChangeTime >= MOVE_POSITION_CHANGE_CHECK_TIME )
	{
		if ( m_byPosChangeCnt >= 3 )
		{
			RwV3d vPos = vDestPos;
			RwV3d vSub;

			RwV3dSubMacro( &vSub, &vPos, &m_vChangeOldPos );

			if ( RwV3dLength( &vSub ) <= MOVE_POSITION_CHANGE_LIMIT_RANGE )
			{
				return FALSE;
			}
			else
			{
				RwV3dAssignMacro( &m_vChangeOldPos, &vPos );
			}
		}

		m_fPosChangeTime -= MOVE_POSITION_CHANGE_CHECK_TIME;
		m_byPosChangeCnt = 0;
	}

	return TRUE;
}


void CNtlBehaviorVehicleMoveBase::CreateRunSmokeEffect( void )
{
	RwV3d vPos = m_pActor->GetPosition();
	m_pPLRunSmoke = m_pActor->GetSobProxy()->CreatePLEffect( NTL_VID_VEHICLE_RUN_SMOKE, &vPos );
}

void CNtlBehaviorVehicleMoveBase::DestroyRunSmokeEffect( void )
{
	if ( m_pPLRunSmoke )
	{
		m_pPLRunSmoke->Finish();
		m_pPLRunSmoke = NULL;
	}
}

void CNtlBehaviorVehicleMoveBase::CreateRunSprayEffect( void )
{
	RwV3d vPos = m_pActor->GetPosition();
	m_pPLRunSpray = m_pActor->GetSobProxy()->CreatePLEffect( NTL_VID_VEHICLE_RUN_SPRAY, &vPos );
}

void CNtlBehaviorVehicleMoveBase::DestroyRunSprayEffect( void )
{
	if ( m_pPLRunSpray )
	{
		m_pPLRunSpray->Finish();
		m_pPLRunSpray = NULL;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlBehaviorVehicleMouseMove )


CNtlBehaviorVehicleMouseMove::CNtlBehaviorVehicleMouseMove( void )
{
	SetBehaviorId( NTL_BEID_VEHICLE_MOVE );
	SetBehaviorName( NTL_BENAME_VEHICLE_MOVE );

	m_pHermiteMoveSpline = NTL_NEW CDboHermiteMoveSpline;

	m_fMoveDist = 0.f;
}

CNtlBehaviorVehicleMouseMove::~CNtlBehaviorVehicleMouseMove( void )
{
	NTL_DELETE( m_pHermiteMoveSpline );
}

void CNtlBehaviorVehicleMouseMove::Enter( void )
{
	CNtlBehaviorVehicleMoveBase::Enter();

	m_fMoveDist = 0.f;

	InitTraceOfMoving();
}

void CNtlBehaviorVehicleMouseMove::Exit( void )
{
	CNtlBehaviorVehicleMoveBase::Exit();
}

void CNtlBehaviorVehicleMouseMove::Update( RwReal fElapsed )
{
	if ( m_bStop )
	{
		UpdateAnim();
		return;
	}

	CNtlBehaviorVehicleMoveBase::Update( fElapsed );

	UpdateSendSyncCheck( fElapsed );

	UpdateServerSyncPosition( fElapsed );

	UpdateVelocity( fElapsed );

	RwV3d vCurDir = m_pActor->GetDirection();
	RwV3d vCurPos = m_pActor->GetPosition();

	RwReal fOldMoveDist = m_fMoveDist;
	m_fMoveDist += m_fCurVelocity * fElapsed;

	RwReal fTimeRatio = m_fMoveDist / m_pHermiteMoveSpline->GetMaxDistance();

	if ( fTimeRatio >= 1.f )
	{
		fTimeRatio = 1.f;

		SetStop();
	}

	RwV3d vDestDir, vDestPos;

	m_pHermiteMoveSpline->GetPosAndDir( fTimeRatio, vDestDir, vDestPos );

	if ( m_bServerSyncPos )
	{
		vDestPos.x += m_vServerSyncDir.x * m_fDeltaServerSyncDist;
		vDestPos.z += m_vServerSyncDir.z * m_fDeltaServerSyncDist;
	}

	vDestPos.y = vCurPos.y;
	Logic_GetWorldHeight( &vDestPos, m_sHStuff );
	vDestPos.y = m_sHStuff.fFinialHeight + m_pVehicle->GetRideOnHeight() / 100.f;

	RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;

	if ( m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION )
	{
		if ( !( NTL_MOVE_TURN_L == m_MoveStuff.byMoveFlags || NTL_MOVE_TURN_R == m_MoveStuff.byMoveFlags ) )
		{
			RwBool bCollMoveImPossible;

			byColliResult = Logic_VehicleCollisionEx( m_pVehicle, &vDestPos, m_fDefVelocity, vDestPos, bCollMoveImPossible, fElapsed );

			if ( NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT == byColliResult )
			{
				Finish();

				return;
			}
			else if ( byColliResult == NTL_CHARACTER_COLLI_OBJECT )
			{
				if ( bCollMoveImPossible )
				{
					m_byCollMoveImpossCnt++;
				}
				else
				{
					m_byCollMoveImpossCnt = 0;
				}

				SetSyncSendCheck( TRUE );

				m_fMoveDist = fOldMoveDist;
			}
			else
			{
				SetSyncSendCheck( FALSE );
			}

			if ( m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT )
			{
				Finish();

				return;
			}
		}
	}

	RwV3d vTempPos;
	UpdatePosition( vCurPos, vDestPos, vTempPos );

	RwV3d vTempDir = vDestDir;
	RwV3d vResultDir, vResultPos;
	UpdateActorSlop( fElapsed, vTempDir, vTempPos, vCurDir.y, vResultDir, vResultPos );
	vResultPos.y = vTempPos.y;

	// falling state check
	if ( m_pActor->GetFlags() & SLFLAG_FALLING_ENABLE && 
		vCurPos.y - vDestPos.y > FALLING_CHECK_LEN )
	{
		CNtlBeCharData* pBeData = (CNtlBeCharData*)m_pActor->GetBehaviorData();

		SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();
		pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_FALLING;

		SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

		RwV3d vSub = vDestPos - vCurPos; vSub.y = 0.f;
		RwV3dNormalize( &pCtrlStuff->uExtra.sFalling.vFallingDir, &vSub );

		pCtrlStuff->uExtra.sFalling.byMoveDirection = m_MoveStuff.byMoveFlags;

		if ( m_MoveStuff.byMoveFlags == NTL_MOVE_B )
		{
			pCtrlStuff->uExtra.sFalling.fSpeed = m_fDefVelocity * NTL_BACKWARD_MOVEMENT_SPEED_RATE;
		}
		else
		{
			pCtrlStuff->uExtra.sFalling.fSpeed = m_fDefVelocity;
		}

		pCtrlStuff->uExtra.sFalling.fSearchHeight = vDestPos.y;

		Finish();

		return;
	}

	if ( !( NTL_MOVE_TURN_L == m_MoveStuff.byMoveFlags || NTL_MOVE_TURN_R == m_MoveStuff.byMoveFlags ) )
	{
		if ( NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT != byColliResult )
		{
			if ( !LimitPositionChangeCheck( fElapsed, vDestPos ) )
			{
				Finish();

				return;
			}
		}
	}

	RwV3d vUpdateDir;
	UpdateActorDir( fElapsed, vCurDir, vDestDir, vUpdateDir );

	vResultDir = vUpdateDir;
	vResultPos = vDestPos;

	m_pActor->SetDirection( &vResultDir );
	m_pActor->SetPosition( &vResultPos );

	CheckSwimming();

	UpdateSwimEffect();

	UpdateAnimFlag( vCurDir, vDestDir );

	UpdateAnim();
}

RwUInt32 CNtlBehaviorVehicleMouseMove::HandleEvents( RWS::CMsg& pMsg )
{
	return CNtlBehaviorVehicleMoveBase::HandleEvents( pMsg );
}

void CNtlBehaviorVehicleMouseMove::UpdateData( void )
{
	CNtlBehaviorVehicleMoveBase::UpdateData();

	m_fMoveDist = 0.f;

	InitTraceOfMoving();

	RwV3d vCurDir = m_pActor->GetDirection();
	UpdateAnimFlag( vCurDir, vCurDir );
}

void CNtlBehaviorVehicleMouseMove::InitTraceOfMoving( void )
{
	m_pHermiteMoveSpline->Init();

	RwV3d vAddPos;

	vAddPos = m_pActor->GetPosition(); vAddPos.y = 0.f;
	m_pHermiteMoveSpline->AddNodePos( &vAddPos );

	RwV3d vVehicleDir = m_pActor->GetDirection(); vVehicleDir.y = 0.f;
	if ( RwV3dNormalize( &vVehicleDir, &vVehicleDir ) <= 0.f )
	{
		vVehicleDir.x = vVehicleDir.y = 0.f;
		vVehicleDir.z = 1.f;
	}

	RwV3d vVehiclePos = m_pActor->GetPosition(); vVehiclePos.y = 0.f;

	RwV3d vMoveDir = m_MoveStuff.vDest - vVehiclePos; vMoveDir.y = 0.f;
	if ( RwV3dNormalize( &vMoveDir, &vMoveDir ) > 0.f )
	{
		vAddPos = vVehiclePos + (vMoveDir * 2.f) + vVehicleDir;
		m_pHermiteMoveSpline->AddNodePos( &vAddPos );
	}

	vAddPos = m_MoveStuff.vDest; vAddPos.y = 0.f;
	m_pHermiteMoveSpline->AddNodePos( &vAddPos );

	m_pHermiteMoveSpline->BuildSpline();
}

void CNtlBehaviorVehicleMouseMove::UpdateAnimFlag( RwV3d& vCurDir, RwV3d& vNextDir )
{
	m_MoveStuff.byMoveFlags = NTL_MOVE_F;
}


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlBehaviorVehicleKeyboardMove )


CNtlBehaviorVehicleKeyboardMove::CNtlBehaviorVehicleKeyboardMove( void )
{
	SetBehaviorId( NTL_BEID_VEHICLE_MOVE );
	SetBehaviorName( NTL_BENAME_VEHICLE_MOVE );
}

CNtlBehaviorVehicleKeyboardMove::~CNtlBehaviorVehicleKeyboardMove( void )
{
}

void CNtlBehaviorVehicleKeyboardMove::Enter( void )
{
	CNtlBehaviorVehicleMoveBase::Enter();
}

void CNtlBehaviorVehicleKeyboardMove::Exit( void )
{
	CNtlBehaviorVehicleMoveBase::Exit();
}

void CNtlBehaviorVehicleKeyboardMove::Update( RwReal fElapsed )
{
	if ( m_bStop )
	{
		UpdateAnim();
		return;
	}

	CNtlBehaviorVehicleMoveBase::Update( fElapsed );

	UpdateSendSyncCheck( fElapsed );

	m_bServerSyncPos = FALSE; // BY DANEOS NOT SURE IF NEEDED
	UpdateServerSyncPosition( fElapsed );

	UpdateVelocity( fElapsed );

	RwV3d vCurDir = m_pActor->GetDirection();
	RwV3d vCurPos = m_pActor->GetPosition();

	CNtlVector vNextDir, vNextPos;

	NtlGetDestination_Keyboard( vCurDir.x,
								vCurDir.y,
								vCurDir.z,
								m_fCurVelocity,
								vCurPos.x,
								vCurPos.y,
								vCurPos.z,
								m_MoveStuff.byMoveFlags,
								(DWORD)(fElapsed*1000.f),
								1.f,
								&vNextDir,
								&vNextPos );

	RwV3d vDestDir, vDestPos;
	CNtlMath::MathRwV3dAssign( &vDestDir, vNextDir.x, 0.0f, vNextDir.z );
	CNtlMath::MathRwV3dAssign( &vDestPos, vNextPos.x, vNextPos.y, vNextPos.z );

	if ( m_bServerSyncPos )
	{
		vDestPos.x += m_vServerSyncDir.x * m_fDeltaServerSyncDist;
		vDestPos.z += m_vServerSyncDir.z * m_fDeltaServerSyncDist;
	}

	Logic_GetWorldHeight( &vDestPos, m_sHStuff );
	vDestPos.y = m_sHStuff.fFinialHeight + m_pVehicle->GetRideOnHeight() / 100.f;

	RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;

	if ( m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION )
	{
		if ( !( NTL_MOVE_TURN_L == m_MoveStuff.byMoveFlags || NTL_MOVE_TURN_R == m_MoveStuff.byMoveFlags ) )
		{
			RwBool bCollMoveImPossible;

			byColliResult = Logic_VehicleCollisionEx( m_pVehicle, &vDestPos, m_fDefVelocity, vDestPos, bCollMoveImPossible, fElapsed );

			if ( NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT == byColliResult )
			{
				Finish();

				return;
			}
			else if ( byColliResult == NTL_CHARACTER_COLLI_OBJECT )
			{
				if ( bCollMoveImPossible )
				{
					m_byCollMoveImpossCnt++;
				}
				else
				{
					m_byCollMoveImpossCnt = 0;
				}

				SetSyncSendCheck( TRUE );
			}
			else
			{
				SetSyncSendCheck( FALSE );
			}

			if ( m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT )
			{
				Finish();

				return;
			}
		}
	}

	RwV3d vTempPos;
	UpdatePosition( vCurPos, vDestPos, vTempPos );

	RwV3d vTempDir = vDestDir;
	RwV3d vResultDir, vResultPos;
	UpdateActorSlop( fElapsed, vTempDir, vTempPos, vCurDir.y, vResultDir, vResultPos );
	vResultPos.y = vTempPos.y;

	// falling state check
	if ( m_pActor->GetFlags() & SLFLAG_FALLING_ENABLE && 
		 vCurPos.y - vDestPos.y > FALLING_CHECK_LEN )
	{
		CNtlBeCharData* pBeData = (CNtlBeCharData*)m_pActor->GetBehaviorData();

		SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();
		pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_FALLING;

		SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

		RwV3d vSub = vDestPos - vCurPos; vSub.y = 0.f;
		RwV3dNormalize( &pCtrlStuff->uExtra.sFalling.vFallingDir, &vSub );

		pCtrlStuff->uExtra.sFalling.byMoveDirection = m_MoveStuff.byMoveFlags;

		if ( m_MoveStuff.byMoveFlags == NTL_MOVE_B )
		{
			pCtrlStuff->uExtra.sFalling.fSpeed = m_fDefVelocity * NTL_BACKWARD_MOVEMENT_SPEED_RATE;
		}
		else
		{
			pCtrlStuff->uExtra.sFalling.fSpeed = m_fDefVelocity;
		}

		pCtrlStuff->uExtra.sFalling.fSearchHeight = vDestPos.y;

		Finish();

		return;
	}

	if ( !( NTL_MOVE_TURN_L == m_MoveStuff.byMoveFlags || NTL_MOVE_TURN_R == m_MoveStuff.byMoveFlags ) )
	{
		if ( NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT != byColliResult )
		{
			if ( !LimitPositionChangeCheck( fElapsed, vDestPos ) )
			{
				Finish();

				return;
			}
		}
	}

	m_pActor->SetDirection( &vResultDir );
	m_pActor->SetPosition( &vResultPos );

	CheckSwimming();

	UpdateSwimEffect();

	UpdateAnim();
}

RwUInt32 CNtlBehaviorVehicleKeyboardMove::HandleEvents( RWS::CMsg& pMsg )
{
	if ( pMsg.Id == g_EventKeyboardMoveStop )
	{
		SetStop();
	}

	return CNtlBehaviorVehicleMoveBase::HandleEvents( pMsg );
}

void CNtlBehaviorVehicleKeyboardMove::UpdateData( void )
{
	CNtlBehaviorVehicleMoveBase::UpdateData();
}


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlBehaviorVehicleFollowMove )


CNtlBehaviorVehicleFollowMove::CNtlBehaviorVehicleFollowMove( void )
{
	SetBehaviorId( NTL_BEID_VEHICLE_MOVE );
	SetBehaviorName( NTL_BENAME_VEHICLE_MOVE );
}

CNtlBehaviorVehicleFollowMove::~CNtlBehaviorVehicleFollowMove( void )
{
}

void CNtlBehaviorVehicleFollowMove::Enter( void )
{
	CNtlBehaviorVehicleMouseMove::Enter();

	m_vTargetPos = m_pActor->GetPosition();

	RwBool bSlowDown;
	if ( eTARGET_UPDATE_TYPE_STOP == UpdateTargetPos( bSlowDown ) )
	{
		SetStop();
	}
}

void CNtlBehaviorVehicleFollowMove::Exit( void )
{
	CNtlBehaviorVehicleMouseMove::Exit();
}

void CNtlBehaviorVehicleFollowMove::Update( RwReal fElapsed )
{
	if ( m_bStop )
	{
		UpdateAnim();
		return;
	}

	RwBool bSlowDown;
	eTARGET_UPDATE_TYPE eUpdateType = UpdateTargetPos( bSlowDown );

	if ( eTARGET_UPDATE_TYPE_STOP == eUpdateType )
	{
		SetStop();

		return;
	}

	UpdateVelocity( fElapsed );

	RwV3d vCurDir = m_pActor->GetDirection();
	RwV3d vCurPos = m_pActor->GetPosition();

	RwReal fOldMoveDist = m_fMoveDist;
	m_fMoveDist += m_fCurVelocity * (bSlowDown ? fElapsed * 0.5f : fElapsed);

	RwReal fMaxDist = m_pHermiteMoveSpline->GetMaxDistance();

	RwReal fTimeRatio = (fMaxDist <= 0.0001f ? 1.f : m_fMoveDist / fMaxDist);

	if ( fTimeRatio >= 1.f )
	{
		fTimeRatio = 1.f;

		SetStop();
	}

	RwV3d vDestDir, vDestPos;

	m_pHermiteMoveSpline->GetPosAndDir( fTimeRatio, vDestDir, vDestPos );

	if ( m_bServerSyncPos )
	{
		vDestPos.x += m_vServerSyncDir.x * m_fDeltaServerSyncDist;
		vDestPos.z += m_vServerSyncDir.z * m_fDeltaServerSyncDist;
	}

	vDestPos.y = vCurPos.y;
	Logic_GetWorldHeight( &vDestPos, m_sHStuff );
	vDestPos.y = m_sHStuff.fFinialHeight + m_pVehicle->GetRideOnHeight() / 100.f;

	RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;

	if ( m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION )
	{
		if ( !( NTL_MOVE_TURN_L == m_MoveStuff.byMoveFlags || NTL_MOVE_TURN_R == m_MoveStuff.byMoveFlags ) )
		{
			RwBool bCollMoveImPossible;

			byColliResult = Logic_VehicleCollisionEx( m_pVehicle, &vDestPos, m_fDefVelocity, vDestPos, bCollMoveImPossible, fElapsed );

			if ( NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT == byColliResult )
			{
				Finish();

				return;
			}
			else if ( byColliResult == NTL_CHARACTER_COLLI_OBJECT )
			{
				if ( bCollMoveImPossible )
				{
					m_byCollMoveImpossCnt++;
				}
				else
				{
					m_byCollMoveImpossCnt = 0;
				}

				SetSyncSendCheck( TRUE );

				m_fMoveDist = fOldMoveDist;
			}
			else
			{
				SetSyncSendCheck( FALSE );
			}

			if ( m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT )
			{
				Finish();

				return;
			}
		}
	}

	RwV3d vTempPos;
	UpdatePosition( vCurPos, vDestPos, vTempPos );

	RwV3d vTempDir = vDestDir;
	RwV3d vResultDir, vResultPos;
	UpdateActorSlop( fElapsed, vTempDir, vTempPos, vCurDir.y, vResultDir, vResultPos );
	vResultPos.y = vTempPos.y;

	if ( NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT != byColliResult )
	{
		if ( !LimitPositionChangeCheck( fElapsed, vDestPos ) )
		{
			Finish();

			return;
		}
	}

	RwV3d vUpdateDir;
	UpdateActorDir( fElapsed, vCurDir, vDestDir, vUpdateDir );

	vResultDir = vUpdateDir;
	vResultPos = vDestPos;

	m_pActor->SetDirection( &vResultDir );
	m_pActor->SetPosition( &vResultPos );

	CheckSwimming();

	UpdateSwimEffect();

	UpdateAnimFlag( vCurDir, vDestDir );

	UpdateAnim();
}

RwUInt32 CNtlBehaviorVehicleFollowMove::HandleEvents( RWS::CMsg& pMsg )
{
	return CNtlBehaviorVehicleMouseMove::HandleEvents( pMsg );
}

void CNtlBehaviorVehicleFollowMove::UpdateData( void )
{
	CNtlBehaviorVehicleMouseMove::UpdateData();
}

CNtlBehaviorVehicleFollowMove::eTARGET_UPDATE_TYPE CNtlBehaviorVehicleFollowMove::UpdateTargetPos( RwBool& bSlowDown )
{
	CNtlSobActor* pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject( m_MoveStuff.hFollowSerialId );

	if ( NULL == pTargetActor )
	{
		return eTARGET_UPDATE_TYPE_STOP;
	}

	RwV3d vTargetPos = pTargetActor->GetPosition(); vTargetPos.y = 0.f;
	RwV3d vSub = vTargetPos - m_vTargetPos; vSub.y = 0.f;
	RwReal fSubDist = RwV3dLength( &vSub );

	if ( fSubDist >= 1.f )
	{
		m_vTargetPos = vTargetPos;

		m_fMoveDist = 0.f;

		InitTraceOfMoving();
	}

	RwV3d vMoveDir = vTargetPos - m_pActor->GetPosition(); vMoveDir.y = 0.f;
	RwReal fMoveDist = RwV3dNormalize( &vMoveDir, &vMoveDir );

	RwReal fVehicleRadius = m_pActor->GetSobProxy()->GetPLEntityWidth();
	fVehicleRadius = (fVehicleRadius < m_pActor->GetSobProxy()->GetPLEntityDepth()) ? m_pActor->GetSobProxy()->GetPLEntityDepth() : fVehicleRadius;

	RwReal fTargetRadius = pTargetActor->GetSobProxy()->GetPLEntityWidth();
	fTargetRadius = (fTargetRadius < pTargetActor->GetSobProxy()->GetPLEntityDepth()) ? pTargetActor->GetSobProxy()->GetPLEntityDepth() : fTargetRadius;

	if ( fVehicleRadius + fTargetRadius >= fMoveDist )
	{
		return eTARGET_UPDATE_TYPE_STOP;
	}

	bSlowDown = FALSE;

	if ( fMoveDist < SLOW_DOWN_DIST )
	{
		bSlowDown = TRUE;
	}

	return eTARGET_UPDATE_TYPE_MOVE;
}

void CNtlBehaviorVehicleFollowMove::InitTraceOfMoving( void )
{
	m_pHermiteMoveSpline->Init();

	RwV3d vAddPos;

	vAddPos = m_pActor->GetPosition(); vAddPos.y = 0.f;
	m_pHermiteMoveSpline->AddNodePos( &vAddPos );

	RwV3d vVehicleDir = m_pActor->GetDirection(); vVehicleDir.y = 0.f;
	if ( RwV3dNormalize( &vVehicleDir, &vVehicleDir ) <= 0.0001f )
	{
		vVehicleDir.x = vVehicleDir.y = 0.f;
		vVehicleDir.z = 1.f;
	}

	RwV3d vVehiclePos = m_pActor->GetPosition(); vVehiclePos.y = 0.f;

	RwV3d vMoveDir = m_vTargetPos - vVehiclePos; vMoveDir.y = 0.f;
	if ( RwV3dNormalize( &vMoveDir, &vMoveDir ) > 0.f )
	{
		vAddPos = vVehiclePos + (vMoveDir * 2.f) + vVehicleDir;
		m_pHermiteMoveSpline->AddNodePos( &vAddPos );
	}

	vAddPos = m_vTargetPos; vAddPos.y = 0.f;
	m_pHermiteMoveSpline->AddNodePos( &vAddPos );

	m_pHermiteMoveSpline->BuildSpline();
}


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlBehaviorVehicleAdjustMove )


CNtlBehaviorVehicleAdjustMove::CNtlBehaviorVehicleAdjustMove( void )
{
	m_fMoveSpeed		= 0.f;
	m_fMoveFinishTime	= 0.f;
	m_fMoveCurTime		= 0.f;

	m_bStop				= FALSE;
}

CNtlBehaviorVehicleAdjustMove::~CNtlBehaviorVehicleAdjustMove( void )
{
}

void CNtlBehaviorVehicleAdjustMove::Enter( void )
{
	AttachPlayer( m_pActor->GetOwnerID(), m_pActor->GetSerialID() );

	UpdateData();
}

void CNtlBehaviorVehicleAdjustMove::Exit( void )
{
	DetachPlayer();
}

void CNtlBehaviorVehicleAdjustMove::Update( RwReal fElapsed )
{
	m_fMoveCurTime += fElapsed;

	if ( m_fMoveCurTime >= m_fMoveFinishTime )
	{
		m_fMoveCurTime = m_fMoveFinishTime;

		if ( !m_bStop )
		{
			m_bStop = TRUE;

			Vehicle_Anim_Stop();
		}
	}

	RwV3d vNextDir = m_MoveStuff.vDir;
	RwV3d vNextPos = m_vSourcePos + m_vMoveDir * m_fMoveSpeed * m_fMoveCurTime;

	RwV3d vResultDir, vResultPos;
	UpdateActorSlop( fElapsed, vNextDir, vNextPos, 0.f, vResultDir, vResultPos );

	Logic_GetWorldHeight(&vResultPos, m_sHStuff);
	vResultPos.y = m_sHStuff.fFinialHeight + m_pVehicle->GetRideOnHeight() / 100.f;

	m_pActor->SetDirection( &vResultDir );
	m_pActor->SetPosition( &vResultPos );
}

RwUInt32 CNtlBehaviorVehicleAdjustMove::HandleEvents( RWS::CMsg& pMsg )
{
	if ( pMsg.Id == g_EventAnimEnd )
	{
		SNtlEventAnimEnd* pData = (SNtlEventAnimEnd*) pMsg.pData;

		if ( pData->uiBaseAnimKey == VEHICLE_STOP )
		{
			Finish();
		}
	}

	return CNtlBehaviorBase::HandleEvents( pMsg );
}

void CNtlBehaviorVehicleAdjustMove::UpdateData( void )
{
	CNtlBeCharData* pBeData = (CNtlBeCharData*)m_pActor->GetBehaviorData();
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	memcpy( &m_MoveStuff, pMoveStuff, sizeof(SMoveStuff) );

	m_vSourcePos = m_pActor->GetPosition();

	m_vMoveDir = m_MoveStuff.vDest - m_vSourcePos; m_vMoveDir.y = 0.f;
	RwV3dNormalize( &m_vMoveDir, &m_vMoveDir );

	m_fMoveSpeed = Logic_GetFrontRunSpeed( (CNtlSobActor*)m_pPlayer );

	m_fMoveFinishTime = RwV3dNormalize( &m_vMoveDir, &m_vMoveDir ) / m_fMoveSpeed;

	m_fMoveCurTime = 0.f;

	m_pActor->SetDirection( &m_MoveStuff.vDir );

	m_bStop = FALSE;
}


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlBehaviorVehicleFalling )

CNtlBehaviorVehicleFalling::CNtlBehaviorVehicleFalling( void )
{
	m_byFallingState	= FALIINGSTATE_WALK;
	m_fFallingSpeed		= 0.0f;
}

CNtlBehaviorVehicleFalling::~CNtlBehaviorVehicleFalling( void )
{
}

void CNtlBehaviorVehicleFalling::Enter( void )
{
	AttachPlayer( m_pActor->GetOwnerID(), m_pActor->GetSerialID() );

	RwV3d vDir = m_pActor->GetDirection(); vDir.y = 0.f; RwV3dNormalize( &vDir, &vDir );
	m_pActor->SetDirection( &vDir );

	CNtlBeCharData* pBeData = (CNtlBeCharData*)m_pActor->GetBehaviorData();
	SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

	m_vFallingPos		= m_pActor->GetPosition();	
	m_vFallingDir		= pCtrlStuff->uExtra.sFalling.vFallingDir;
	m_fFallingHeight	= pCtrlStuff->uExtra.sFalling.fSearchHeight;
	m_fFallingWalkSpeed	= pCtrlStuff->uExtra.sFalling.fSpeed * 0.5f;

	CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorVehicleFalling::Exit(void)
{
	DetachPlayer();

	CNtlBehaviorBase::Exit(); 
}

void CNtlBehaviorVehicleFalling::Update(RwReal fElapsed)
{
	if ( m_byFallingState == FALIINGSTATE_END )
	{
		return;
	}

	m_fFallingSpeed = m_fFallingSpeed - (JUMP_GRAVITY_SPEED * fElapsed);

	RwV3d vDir = m_pActor->GetDirection();
	RwV3d vPos = m_pActor->GetPosition();

	UpdatePositionMove( vPos, fElapsed );

	if ( m_byFallingState == FALIINGSTATE_WALK )
	{
		UpdateWalking( vPos, fElapsed );
	}
	else if ( m_byFallingState == FALIINGSTATE_FALLING )
	{
		UpdateFalling( vPos, fElapsed );
	}
	else if ( m_byFallingState == FALIINGSTATE_MOVE_LADNDING )
	{
		UpdateMoveLanding( vPos, fElapsed );
	}
	else if ( m_byFallingState == FALIINGSTATE_WATER_LADNDING )
	{
		UPdateWaterLanding( vPos, fElapsed );
	}

	RwBool bLandSuccess = FALSE;

	if ( m_fFallingHeight + FALLING_CHECK_LEN / 2.0f < m_sHStuff.fFinialHeight + GetHeightOffset() )
	{
		if ( vPos.y <= m_sHStuff.fFinialHeight + GetHeightOffset() )
		{
			bLandSuccess = TRUE;
		}
	}
	else
	{
		if ( vPos.y < m_fFallingHeight + FALLING_CHECK_LEN / 2.0f && vPos.y < m_sHStuff.fFinialHeight + GetHeightOffset() )
		{
			bLandSuccess = TRUE;
		}
	}

	if ( bLandSuccess )
	{
		vPos.y = m_sHStuff.fFinialHeight + GetHeightOffset();

		ChangeFallingState( FALIINGSTATE_END );
	}

	CNtlBeCharData* pBeData = (CNtlBeCharData*)m_pActor->GetBehaviorData();
	SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();

	CNtlVector vHeading, vDest;
	NtlGetDestination_Jump( vDir.x, vDir.y, vDir.z, m_fFallingSpeed, vPos.x, vPos.y, vPos.z, m_vFallingDir.x, m_vFallingDir.z, pMoveStuff->byMoveFlags, (DWORD)(fElapsed*1000.f), 1.0f, &vHeading, &vDest );

	RwV3d vNewDir;
	CNtlMath::MathRwV3dAssign( &vNewDir, vHeading.x, 0.0f, vHeading.z );

	m_pActor->SetPosition( &vPos );
	m_pActor->SetDirection( &vNewDir );
}

void CNtlBehaviorVehicleFalling::UpdateData( void )
{
}

RwUInt32 CNtlBehaviorVehicleFalling::HandleEvents( RWS::CMsg& pMsg )
{
	NTL_FUNCTION( "CNtlBehaviorVehicleFalling::HandleEvents" );

	if ( pMsg.Id == g_EventAnimEnd )
	{
		SNtlEventAnimEnd* pAnimEnd = reinterpret_cast< SNtlEventAnimEnd* >( pMsg.pData );
		RwUInt32 uiAnimKey = pAnimEnd->uiBaseAnimKey;

		if ( uiAnimKey == VEHICLE_LANDING )
		{
			Finish();
		}
	}

	NTL_RETURN( NTL_FSM_EVENTRES_PASS );
}

void CNtlBehaviorVehicleFalling::SetAnimFalling( void )
{
	Vehicle_Anim_Run();
}

void CNtlBehaviorVehicleFalling::SetAnimMoveLanding( void )
{
	Vehicle_Anim_Landing();

	RwV3d vPos = m_pActor->GetPosition();
	vPos.y = m_sHStuff.fFinialHeight + GetHeightOffset();
	m_pActor->GetSobProxy()->CreatePLEffect( NTL_VID_VEHICLE_LANDING_NORMAL, &vPos );
}

void CNtlBehaviorVehicleFalling::SetAnimWaterLanding( void )
{
	Vehicle_Anim_Landing();

	RwV3d vPos = m_pActor->GetPosition();
	vPos.y = m_sHStuff.fFinialHeight + GetHeightOffset();
	m_pActor->GetSobProxy()->CreatePLEffect( NTL_VID_VEHICLE_LANDING_WATER, &vPos );
}

RwBool CNtlBehaviorVehicleFalling::LandingCheck( RwV3d& vPos )
{
	RwBool bLandSuccess = FALSE;

	if ( m_fFallingHeight + FALLING_CHECK_LEN / 2.f < m_sHStuff.fFinialHeight + GetHeightOffset() )
	{
		if ( vPos.y <= m_sHStuff.fFinialHeight + GetHeightOffset() + 0.25f )
		{
			bLandSuccess = TRUE;
		}
	}
	else
	{
		if ( vPos.y < m_fFallingHeight + FALLING_CHECK_LEN / 2.0f && vPos.y <= m_sHStuff.fFinialHeight + GetHeightOffset() + 0.25f )
		{
			bLandSuccess = TRUE;
		}
	}

	if ( bLandSuccess )
	{
		if ( Logic_IsSwimming( m_pActor, &vPos, m_sHStuff.fWorldHeight, m_sHStuff.fWaterHeight ) )
		{
			ChangeFallingState( FALIINGSTATE_WATER_LADNDING );
		}
		else
		{
			ChangeFallingState( FALIINGSTATE_MOVE_LADNDING );
		}

		return TRUE;
	}

	return FALSE;
}

void CNtlBehaviorVehicleFalling::UpdatePositionMove( RwV3d& vPos, RwReal fElapsed )
{
	RwV3d vOldPos;
	RwV3dAssignMacro( &vOldPos, &vPos );

	RwReal fSpeed = m_fFallingWalkSpeed * fElapsed;

	vPos.x += m_vFallingDir.x * fSpeed;
	vPos.y += m_fFallingSpeed * fElapsed;
	vPos.z += m_vFallingDir.z * fSpeed;

	Logic_GetWorldHeight( m_pActor, &vPos, m_sHStuff );

	RwV3d vNextPos;
	RwV3dAssignMacro( &vNextPos, &vPos );

	RwBool bCollMoveImPossible;
	RwUInt8 byColliResult = Logic_VehicleCollisionEx( m_pVehicle, &vPos, fSpeed, vNextPos, bCollMoveImPossible, fElapsed );

	if ( byColliResult == NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT )
	{
		vPos.x = vOldPos.x;
		vPos.z = vOldPos.z;
	}
	else
	{
		vPos.x = vNextPos.x;
		vPos.z = vNextPos.z;
	}
}

void CNtlBehaviorVehicleFalling::UpdateWalking( RwV3d& vPos, RwReal fElapsed )
{
	RwV3d vSub;
	RwV3dSubMacro( &vSub, &vPos, &m_vFallingPos );
	RwReal fWalkLen = RwV3dLength( &vSub );

	if ( fWalkLen > FALLING_WALK_LEN )
	{
		ChangeFallingState( FALIINGSTATE_FALLING );
	}

	LandingCheck( vPos );
}

void CNtlBehaviorVehicleFalling::UpdateFalling( RwV3d& vPos, RwReal fElapsed )
{
	LandingCheck( vPos );
}

void CNtlBehaviorVehicleFalling::UpdateMoveLanding( RwV3d& vPos, RwReal fElapsed )
{

}

void CNtlBehaviorVehicleFalling::UPdateWaterLanding( RwV3d& vPos, RwReal fElapsed )
{
}

void CNtlBehaviorVehicleFalling::ChangeFallingState( RwUInt8 byFallingState )
{
	switch ( byFallingState )
	{
	case FALIINGSTATE_WALK:
		break;
	case FALIINGSTATE_FALLING:
		SetAnimFalling();
		break;
	case FALIINGSTATE_MOVE_LADNDING:
		SetAnimMoveLanding();
		break;
	case FALIINGSTATE_WATER_LADNDING:
		SetAnimWaterLanding();
		break;
	case FALIINGSTATE_END:
		if ( m_byFallingState == FALIINGSTATE_WATER_LADNDING )
		{
			Finish();
		}
		break;
	}

	m_byFallingState = byFallingState;
}

RwReal CNtlBehaviorVehicleFalling::GetHeightOffset( void )
{
	return m_pVehicle->GetRideOnHeight() / 100.f;
}
