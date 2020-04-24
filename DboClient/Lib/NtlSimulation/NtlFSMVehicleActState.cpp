#include "precomp_ntlsimulation.h"
#include "NtlFSMVehicleActState.h"

#include "NtlBehaviorData.h"
#include "NtlFSMCharActEventProc.h"
#include "NtlSobActor.h"
#include "NtlBehaviorVehicleMove.h"
#include "NtlSobManager.h"
#include "NtlSobVehicle.h"
#include "NtlSobPlayer.h"
#include "NtlBehaviorVehicle.h"
#include "NtlSLEventFunc.h"
#include "NtlFSMCharActAgent.h"
#include "NtlFSMLayer.h"
#include "NtlMovement.h"
#include "NtlFSMEventQueuingHelper.h"
#include "NtlSLApi.h"
#include "NtlSLPacketGenerator.h"
#include "NtlPacketLockManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlFSMVehicleRideOnOff )


CNtlFSMVehicleRideOnOff::CNtlFSMVehicleRideOnOff( void )
{
	SetStateId( NTL_FSMSID_RIDEONOFF );
	SetStateName( NTL_FSMSN_RIDEONOFF );
	SetFlags( NTL_FSMSF_NOT_ALL );

	m_hDriver = INVALID_SERIAL_ID;
}

void CNtlFSMVehicleRideOnOff::Enter( void )
{
	m_hDriver = m_pActor->GetOwnerID();

	CNtlSobActor* pDriver = (CNtlSobActor*)GetNtlSobManager()->GetSobObject( m_hDriver );
	if ( NULL == pDriver )
	{
		Finish();
		return;
	}

	CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* >( m_pActor->GetBehaviorData() );
	SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

	if ( pCtrlStuff->sVehicle.bRideOn )
	{
		CNtlBahaviorVehicleRideOn* pBehaivor = NTL_NEW CNtlBahaviorVehicleRideOn;
		pBehaivor->SetData( pDriver );
		AddBehavior( pBehaivor );
	}
	else
	{
		CNtlBahaviorVehicleRideOff* pBehaivor = NTL_NEW CNtlBahaviorVehicleRideOff;
		pBehaivor->SetData( pDriver );
		AddBehavior( pBehaivor );
	}
}

void CNtlFSMVehicleRideOnOff::Exit( void )
{
}

void CNtlFSMVehicleRideOnOff::Update( RwReal fElapsed )
{
	CNtlFSMVehicleActStateBase::Update( fElapsed );
}

RwUInt32 CNtlFSMVehicleRideOnOff::HandleEvents( RWS::CMsg &pMsg )
{
	return CNtlFSMVehicleActStateBase::HandleEvents( pMsg );
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlFSMVehicleActIdleState )


CNtlFSMVehicleActIdleState::CNtlFSMVehicleActIdleState( void )
{
	SetStateId( NTL_FSMSID_IDLE );
	SetStateName( NTL_FSMSN_IDLE );
	SetFlags(NTL_FSMSF_NOT_HURT_STATE);
}

void CNtlFSMVehicleActIdleState::Enter( void )
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	pMoveStuff->byType = NTL_MOVETARGET_NONE;
	pMoveStuff->byMoveFlags = NTL_MOVE_NONE;

	AttachPlayer( m_pActor->GetOwnerID(), m_pActor->GetSerialID() );

	Vehicle_Anim_Idle();
}

void CNtlFSMVehicleActIdleState::Exit( void )
{
	DetachPlayer();
}

void CNtlFSMVehicleActIdleState::Update( RwReal fElapsed )
{
	/*RwV3d vCurPos = m_pVehicle->GetPosition();

	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight( &vCurPos, sHStuff );
	vCurPos.y = sHStuff.fFinialHeight + m_pVehicle->GetRideOnHeight() / 100.f;

	m_pVehicle->SetPosition( &vCurPos );*/
}

RwUInt32 CNtlFSMVehicleActIdleState::HandleEvents( RWS::CMsg &pMsg )
{
	if ( pMsg.Id == g_EventKeyboardMoveStop )
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
		pMoveStuff->byType = NTL_MOVETARGET_NONE;
		pMoveStuff->byMoveFlags = NTL_MOVE_NONE;

		return NTL_FSM_EVENTRES_QUEUING_REFRESH;
	}

	return CNtlFSMVehicleActStateBase::HandleEvents( pMsg );
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlFSMVehicleMoveState )


CNtlFSMVehicleMoveState::CNtlFSMVehicleMoveState( void )
{
	SetStateId( NTL_FSMSID_MOVE );
	SetStateName( NTL_FSMSN_MOVE );
	SetFlags(NTL_FSMSF_NOT_HURT_STATE);
	m_byMoveType = NTL_MOVETARGET_NONE;
}

void CNtlFSMVehicleMoveState::Enter( void )
{
	CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* >( m_pActor->GetBehaviorData() );

	SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();

	if ( NTL_MOVETARGET_DIR == pMoveStuff->byType )
	{
		m_byMoveType = NTL_MOVETARGET_DIR;

		CNtlBehaviorVehicleKeyboardMove* pBehavior = NTL_NEW CNtlBehaviorVehicleKeyboardMove;
		AddBehavior( pBehavior );
	}
	else if ( NTL_MOVETARGET_LOC == pMoveStuff->byType )
	{
		m_byMoveType = NTL_MOVETARGET_LOC;

		CNtlBehaviorVehicleMouseMove* pBehavior = NTL_NEW CNtlBehaviorVehicleMouseMove;
		AddBehavior( pBehavior );
	}
	else
	{
		Finish();
	}
}

void CNtlFSMVehicleMoveState::Exit( void )
{
}

void CNtlFSMVehicleMoveState::Update( RwReal fElapsed )
{
	CNtlFSMVehicleActStateBase::Update( fElapsed );

	CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* > ( m_pActor->GetBehaviorData() );

	SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();
	if ( pMoveStuff->byMoveResult & NTL_MOVE_RESULT_FALLING )
	{
		Finish();

		SetNextStateId( NTL_FSMSID_FALLING );

		return;
	}
}

RwUInt32 CNtlFSMVehicleMoveState::HandleEvents( RWS::CMsg &pMsg )
{
	if ( pMsg.Id == g_EventTerrainClick )
	{
		if ( m_byMoveType != NTL_MOVETARGET_LOC )
		{
			m_byMoveType = NTL_MOVETARGET_LOC;

			RemoveAllBehavior();

			CNtlBehaviorVehicleMouseMove* pBehavior = NTL_NEW CNtlBehaviorVehicleMouseMove;

			AddBehavior( pBehavior );
		}
	}
	else if ( pMsg.Id == g_EventKeyboardMove )
	{
		if ( m_byMoveType != NTL_MOVETARGET_DIR )
		{
			m_byMoveType = NTL_MOVETARGET_DIR;

			RemoveAllBehavior();

			CNtlBehaviorVehicleKeyboardMove* pBehavior = NTL_NEW CNtlBehaviorVehicleKeyboardMove;

			AddBehavior( pBehavior );
		}
	}
	else if ( pMsg.Id == g_EventSobMove )
	{
		FSMEvent_CharActSobMove( m_pActor, (SNtlEventSobMove*) pMsg.pData ); 

		UpdateBehavior();

		return NTL_FSM_EVENTRES_PASS;
	}

	return CNtlFSMVehicleActStateBase::HandleEvents( pMsg );
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlFSMVehicleFollowState )


CNtlFSMVehicleFollowState::CNtlFSMVehicleFollowState( void )
{
	SetStateId( NTL_FSMSID_FOLLOW );
	SetFlags(NTL_FSMSF_NOT_HURT_STATE);
	SetStateName( NTL_FSMSN_FOLLOW );
}

void CNtlFSMVehicleFollowState::Enter( void )
{
	CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* >( m_pActor->GetBehaviorData() );
	SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

	CNtlBehaviorVehicleFollowMove* pBehavior = NTL_NEW CNtlBehaviorVehicleFollowMove;
	AddBehavior( pBehavior );
}

void CNtlFSMVehicleFollowState::Exit( void )
{
	CNtlFSMVehicleActStateBase::Exit();
}

void CNtlFSMVehicleFollowState::Update( RwReal fElapsed )
{
	CNtlFSMVehicleActStateBase::Update( fElapsed );

	CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* > ( m_pActor->GetBehaviorData() );
}

RwUInt32 CNtlFSMVehicleFollowState::HandleEvents( RWS::CMsg &pMsg )
{
	return CNtlFSMVehicleActStateBase::HandleEvents( pMsg );
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlFSMVehicleAdjustMoveState )


CNtlFSMVehicleAdjustMoveState::CNtlFSMVehicleAdjustMoveState( void )
{
	SetStateId( NTL_FSMSID_ADJUSTMOVE );
	SetStateName( NTL_FSMSN_ADJUSTMOVE );
	SetFlags(NTL_FSMSF_NOT_HURT_STATE);
}

void CNtlFSMVehicleAdjustMoveState::Enter( void )
{
	CNtlBehaviorVehicleAdjustMove* pBehavior = NTL_NEW CNtlBehaviorVehicleAdjustMove;
	AddBehavior( pBehavior );
}

void CNtlFSMVehicleAdjustMoveState::Exit( void )
{
}

void CNtlFSMVehicleAdjustMoveState::Update( RwReal fElapsed )
{
	CNtlFSMVehicleActStateBase::Update( fElapsed );
}

RwUInt32 CNtlFSMVehicleAdjustMoveState::HandleEvents( RWS::CMsg &pMsg )
{
	return CNtlFSMVehicleActStateBase::HandleEvents( pMsg );
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlFSMVehicleActFallingState )


CNtlFSMVehicleActFallingState::CNtlFSMVehicleActFallingState( void )
{
	SetStateId( NTL_FSMSID_FALLING );
	SetStateName( NTL_FSMSN_FALLING );

	SetFlags( NTL_FSMSF_NOT_JUMP|
			  NTL_FSMSF_NOT_SIT|
			  NTL_FSMSF_NOT_SKILLUSE|
			  NTL_FSMSF_NOT_SOCIALUSE|
			  NTL_FSMSF_NOT_MOVE|
			  NTL_FSMSF_NOT_CAHNGE_HEADING|
			  NTL_FSMSF_NOT_ATTACK_STATE|
			  NTL_FSMSF_NOT_HURT_STATE|
			  NTL_FSMSF_NOT_ATTACK_CAN );
}

void CNtlFSMVehicleActFallingState::Enter( void )
{
	if ( m_pActor->GetFlags() & SLFLAG_SERVER_SENDER )
	{
		CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* >( m_pActor->GetBehaviorData() );
		SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();
		if ( pMoveStuff->byType == NTL_MOVETARGET_LOC )
		{
			AgentQueuingTerrainClick( GetFSMAgent(), pMoveStuff->vDest );
		}
		else if ( pMoveStuff->byType == NTL_MOVETARGET_DIR )
		{
			AgentQueuingKeyboardMove( GetFSMAgent(), pMoveStuff->byMoveFlags );
		}
	}

	CNtlBehaviorBase* pBehavior = NTL_NEW CNtlBehaviorVehicleFalling;
	AddBehavior( pBehavior );
}

void CNtlFSMVehicleActFallingState::Exit( void )
{
}

void CNtlFSMVehicleActFallingState::Update( RwReal fElapsed )
{
	CNtlFSMVehicleActStateBase::Update( fElapsed );
}

RwUInt32 CNtlFSMVehicleActFallingState::HandleEvents( RWS::CMsg &pMsg )
{
	if ( pMsg.Id == g_EventKeyboardMove )
	{
		SNtlEventKeyboardMove* pKeyboardMove = reinterpret_cast< SNtlEventKeyboardMove* >( pMsg.pData );

		CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* >( m_pActor->GetBehaviorData() ); 
		SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();

		pMoveStuff->byMoveFlags = JumpKeyMapping( pMoveStuff->byMoveFlags, pKeyboardMove->byMoveFlags );

		return NTL_FSM_EVENTRES_QUEUING;
	}
	else if ( pMsg.Id == g_EventKeyboardMoveStop )
	{
		CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* >( m_pActor->GetBehaviorData() );
		SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();
		pMoveStuff->byType = NTL_MOVETARGET_NONE;
		pMoveStuff->byMoveFlags = NTL_MOVE_NONE;

		return NTL_FSM_EVENTRES_QUEUING;
	}
	else if ( pMsg.Id == g_EventSobMove )
	{
		return NTL_FSM_EVENTRES_QUEUING;
	}
	else if ( pMsg.Id == g_EventSobMoveStop )
	{
		return NTL_FSM_EVENTRES_QUEUING;
	}
	else if ( pMsg.Id == g_EventInputChangeHeading )
	{
		SNtlEventInputChangeHading* pChangeHeading = reinterpret_cast< SNtlEventInputChangeHading* >( pMsg.pData );

		if ( !pChangeHeading->bSoothing )
		{
			CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* >( m_pActor->GetBehaviorData() );

			m_pActor->SetDeltaAngleY( pChangeHeading->fDeltaAngleY );

			return NTL_FSM_EVENTRES_BLOCK;
		}

		return NTL_FSM_EVENTRES_BLOCK;
	}
	else if ( pMsg.Id == g_EventSobDirectionFloat )
	{
		SNtlEventSobDirectionFloat* pFloat = reinterpret_cast< SNtlEventSobDirectionFloat* >( pMsg.pData );

		FSMEvent_CharActSobDirectionFloat( m_pActor, pFloat );

		return NTL_FSM_EVENTRES_BLOCK;
	}

	return CNtlFSMCharActStateBase::HandleEvents( pMsg );
}
