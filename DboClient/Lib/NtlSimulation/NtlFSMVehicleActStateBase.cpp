#include "precomp_ntlsimulation.h"
#include "NtlFSMVehicleActStateBase.h"

#include "NtlDebug.h"
#include "NtlSLLogicDef.h"
#include "NtlSLLogic.h"
#include "NtlSLEvent.h"
#include "NtlFSMCharActEventProc.h"
#include "NtlBehaviorData.h"
#include "NtlSobActor.h"


CNtlFSMVehicleActStateBase::CNtlFSMVehicleActStateBase( void )
{
}

RwBool CNtlFSMVehicleActStateBase::Create( void )
{
	return CNtlFSMCharActStateBase::Create();
}

void CNtlFSMVehicleActStateBase::Destroy( void )
{
	CNtlFSMCharActStateBase::Destroy();
}

void CNtlFSMVehicleActStateBase::Enter( void )
{
	CNtlFSMCharActStateBase::Enter();
}

void CNtlFSMVehicleActStateBase::Exit( void )
{
	CNtlFSMCharActStateBase::Exit();
}

RwUInt32 CNtlFSMVehicleActStateBase::HandleEvents( RWS::CMsg& pMsg )
{
	return CNtlFSMCharActStateBase::HandleEvents( pMsg );
}

void CNtlFSMVehicleActStateBase::Clone( CNtlFSMStateBase* pState )
{
	CNtlFSMCharActStateBase::Clone( pState );
}

RwUInt32 CNtlFSMVehicleActStateBase::EventProcInputTerrainClick( RWS::CMsg& pMsg )
{
	NTL_FUNCTION( "CNtlFSMVehicleActStateBase::EventProcInputTerrainClick" );

	if ( GetFlags() & NTL_FSMSF_NOT_MOVE )
	{
		NTL_RETURN( NTL_FSM_EVENTRES_BLOCK );
	}

	if ( GetFlags() & NTL_FSMSF_NOT_INPUT )
	{
		NTL_RETURN( NTL_FSM_EVENTRES_BLOCK );
	}

	FSMEvent_CharActTerrainClick( m_pActor, reinterpret_cast< SNtlEventTerrainClick* > ( pMsg.pData ) ); 

	RwUInt32 uiStateId = GetStateId();

	if ( uiStateId == NTL_FSMSID_MOVE )
	{
		UpdateBehavior();
	}
	else
	{
		Finish();

		SetNextStateId( NTL_FSMSID_MOVE );

		NTL_RETURN( NTL_FSM_EVENTRES_CHANGE_STATE );
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMVehicleActStateBase::EventProcInputKeyboardMove( RWS::CMsg& pMsg )
{
	NTL_FUNCTION( "CNtlFSMVehicleActStateBase::EventProcInputKeyboardMove" );

	if ( GetFlags() & NTL_FSMSF_NOT_MOVE )
	{
		NTL_RETURN( NTL_FSM_EVENTRES_BLOCK );
	}

	if ( GetFlags() & NTL_FSMSF_NOT_INPUT )
	{
		NTL_RETURN( NTL_FSM_EVENTRES_BLOCK );
	}

	SNtlEventKeyboardMove* pKeyboardMove = reinterpret_cast< SNtlEventKeyboardMove* > ( pMsg.pData );

	if ( GetStateId() == NTL_FSMSID_MOVE )
	{
		CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* > ( m_pActor->GetBehaviorData() );
		SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();

		if ( pMoveStuff->byType == NTL_MOVETARGET_DIR &&
			 pKeyboardMove->byMoveFlags == pMoveStuff->byMoveFlags )
		{
			if ( pKeyboardMove->bAvatarAngleChange )
			{
				m_pActor->SetAngleY( pKeyboardMove->fAvatarAngleY );
			}

			NTL_RETURN( NTL_FSM_EVENTRES_BLOCK );
		}
	}

	FSMEvent_CharActKeyboardMove( m_pActor, pKeyboardMove ); 

	RwUInt32 uiStateId = GetStateId();

	if ( uiStateId == NTL_FSMSID_MOVE )
	{
		UpdateBehavior();
	}
	else
	{
		Finish();

		SetNextStateId( NTL_FSMSID_MOVE );

		NTL_RETURN( NTL_FSM_EVENTRES_CHANGE_STATE );
	}

	NTL_RETURN( NTL_FSM_EVENTRES_PASS );
}

RwUInt32 CNtlFSMVehicleActStateBase::EventProcInputKeyboardMoveStop( RWS::CMsg& pMsg )
{
	NTL_FUNCTION( "CNtlFSMVehicleActStateBase::EventProcInputKeyboardMoveStop" );

	if ( GetFlags() & NTL_FSMSF_NOT_MOVE )
	{
		NTL_RETURN( NTL_FSM_EVENTRES_BLOCK );
	}

	FSMEvent_CharActKeyboardMoveStop( m_pActor ); 

	Finish();
	SetNextStateId( NTL_FSMSID_IDLE );

	NTL_RETURN( NTL_FSM_EVENTRES_CHANGE_STATE );
}

RwUInt32 CNtlFSMVehicleActStateBase::EventProcSobMoveStop( RWS::CMsg& pMsg )
{
	NTL_FUNCTION( "CNtlFSMVehicleActStateBase::EventProcSobMoveStop" );

	RwUInt32 uiResult;

	SNtlEventSobMoveStop* pMoveStop = reinterpret_cast< SNtlEventSobMoveStop* > ( pMsg.pData );

	RwV3d vPos = m_pActor->GetPosition();

	RwV3dSubMacro( &vPos, &vPos, &pMoveStop->vLoc );

	RwReal fLen = RwV3dLength( &vPos );

	if ( fLen >= 0.1f )
	{
		CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* > ( m_pActor->GetBehaviorData() );
		SMoveStuff* pMoveStuff = pBeData->GetMoveStuff(); 

		if ( NTL_MOVETARGET_DIR == pMoveStuff->byType )
		{
			SetNextStateId( NTL_FSMSID_ADJUSTMOVE );

			uiResult = NTL_FSM_EVENTRES_CHANGE_STATE;
		}
		else
		{
			uiResult = NTL_FSM_EVENTRES_PASS;
		}
	}
	else
	{
		Behavior_AdjustPositionJumping( m_pActor, &pMoveStop->vLoc );
		m_pActor->SetDirection( &pMoveStop->vDir );

		uiResult = NTL_FSM_EVENTRES_CHANGE_STATE;
	}

	FSMEvent_CharActSobMoveStop( m_pActor, pMoveStop ); 

	NTL_RETURN( uiResult );
}