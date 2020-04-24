#include "precomp_ntlsimulation.h"
#include "NtlFSMVehicleActAgent.h"

#include "NtlDebug.h"
#include "NtlCharacter.h"
#include "NtlSobManager.h"
#include "NtlFSMLayer.h"
#include "NtlFSMAgent.h"
#include "NtlFSMStateBase.h"
#include "NtlSLEventFunc.h"


DEFINITION_MEMORY_POOL( CNtlFSMVehicleActAgent )


CNtlFSMVehicleActAgent::CNtlFSMVehicleActAgent()
{
}

CNtlFSMVehicleActAgent::~CNtlFSMVehicleActAgent()
{
}

void CNtlFSMVehicleActAgent::Update( RwReal fElapsed )
{
	CNtlFSMCharActAgent::Update( fElapsed ); 
}

RwUInt32 CNtlFSMVehicleActAgent::HandleEvents( RWS::CMsg& pMsg )
{
	return CNtlFSMCharActAgent::HandleEvents( pMsg );  
}

RwUInt32 CNtlFSMVehicleActAgent::AcquireNextState( RwUInt32 uiCurrStateId )
{
	return NTL_FSMSID_IDLE;
}

void CNtlFSMVehicleActAgent::NotifyUpdateChangeState( RwUInt32 uiOldState, RwUInt32 uiNextState )
{
	CNtlFSMCharActAgent::NotifyUpdateChangeState( uiOldState, uiNextState );
}

void CNtlFSMVehicleActAgent::NotifyEvents( RWS::CMsg& pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2 )
{
	CNtlFSMCharActAgent::NotifyEvents( pMsg, uiEventResult, uiParam1, uiParam2 );
}

void CNtlFSMVehicleActAgent::ServerStateCreateAnalysis( sCHARSTATE* pCharState )
{
	CopyCharState( &m_sCharState, pCharState );

	m_uActMode.bitSitDown = FALSE;
	SetAirMode(AIR_STATE_OFF);

	switch ( pCharState->sCharStateBase.byStateID )
	{
	case CHARSTATE_SPAWNING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_STANDING:
		ServerStateCreateStanding( pCharState );
		break;
	case CHARSTATE_CHARGING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_GUARD:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_SITTING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_MOVING:
		ServerStateCreateMoving( pCharState );
		break;
	case CHARSTATE_DESTMOVE:
		ServerStateCreateDestMove( pCharState );
		break;
	case CHARSTATE_DASH_PASSIVE:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_FOLLOWING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_FALLING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_TELEPORTING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_SLIDING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_KNOCKDOWN:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_STUNNED:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_SLEEPING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_PARALYZED:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_FOCUSING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_CASTING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_SKILL_AFFECTING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_HTB:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_SANDBAG:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_CASTING_ITEM:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_FAINTING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_PRIVATESHOP:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_DIRECT_PLAY:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_OPERATING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_DESPAWNING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_CAMPING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_LEAVING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_RIDEON:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	case CHARSTATE_TURNING:
		SetNextStateId( NTL_FSMSID_IDLE );
		break;
	default:
		{
			NTL_ASSERTFAIL( "CNtlFSMVehicleActAgent::ServerStateCreateAnalysis => not process server state" );
		}
		break;
	}

	m_byServerState = pCharState->sCharStateBase.byStateID;

	CNtlMath::MathRwV3dAssign( &m_vServerPos, pCharState->sCharStateBase.vCurLoc.x, pCharState->sCharStateBase.vCurLoc.y, pCharState->sCharStateBase.vCurLoc.z );
	CNtlMath::MathRwV3dAssign( &m_vServerDir, pCharState->sCharStateBase.vCurDir.x, pCharState->sCharStateBase.vCurDir.y, pCharState->sCharStateBase.vCurDir.z );

	ServerConditionUpdateAnalysis(m_sCharState.sCharStateBase.byStateID, pCharState->sCharStateBase.dwConditionFlag );
	ServerAspectUpdateAnalysis( pCharState->sCharStateBase.aspectState, TRUE );

	SetFightingMode( pCharState->sCharStateBase.bFightMode );
}

void CNtlFSMVehicleActAgent::ServerStateUpdateAnalysis( SNtlEventSobServerUpdateState* pServerState )
{
	CopyCharState( &m_sCharState, pServerState->pState );

	sCHARSTATE* pCharState = pServerState->pState;

	m_uActMode.bitSitDown = FALSE;
	m_bActiveAdjustPosition = FALSE;

	switch ( pServerState->pState->sCharStateBase.byStateID )
	{
	case CHARSTATE_SPAWNING:
		break;
	case CHARSTATE_STANDING:
		ServerStateStanding( pServerState->pState );
		break;
	case CHARSTATE_CHARGING:
		break;
	case CHARSTATE_GUARD:
		break;
	case CHARSTATE_SITTING:
		break;
	case CHARSTATE_MOVING:
		ServerStateMoving( pServerState->pState );
		break;
	case CHARSTATE_DESTMOVE:
		ServerStateDestMove( pServerState->pState );
		break;
	case CHARSTATE_DASH_PASSIVE:
		break;
	case CHARSTATE_FOLLOWING:
		break;
	case CHARSTATE_FALLING:
		break;
	case CHARSTATE_TELEPORTING:
		break;
	case CHARSTATE_SLIDING:
		break;
	case CHARSTATE_KNOCKDOWN:
		break;
	case CHARSTATE_STUNNED:
		break;
	case CHARSTATE_SLEEPING:
		break;
	case CHARSTATE_PARALYZED:
		break;
	case CHARSTATE_FOCUSING:
		break;
	case CHARSTATE_CASTING:
		break;
	case CHARSTATE_SKILL_AFFECTING:
		break;
	case CHARSTATE_HTB:
		break;
	case CHARSTATE_SANDBAG:
		break;
	case CHARSTATE_CASTING_ITEM:
		break;
	case CHARSTATE_FAINTING:
		break;
	case CHARSTATE_PRIVATESHOP:
		break;
	case CHARSTATE_DIRECT_PLAY:
		break;
	case CHARSTATE_OPERATING:
		break;
	case CHARSTATE_DESPAWNING:
		break;
	case CHARSTATE_CAMPING:
		break;
	case CHARSTATE_LEAVING:
		break;
	case CHARSTATE_RIDEON:
		break;
	case CHARSTATE_TURNING:
		break;
	default:
		{
			NTL_ASSERTFAIL( "CNtlFSMVehicleActAgent::ServerStateUpdateAnalysis => not process server state" );
		}
		break;
	}

	m_byServerState = pServerState->pState->sCharStateBase.byStateID;

	CNtlMath::MathRwV3dAssign( &m_vServerPos, pCharState->sCharStateBase.vCurLoc.x, pCharState->sCharStateBase.vCurLoc.y, pCharState->sCharStateBase.vCurLoc.z );
	CNtlMath::MathRwV3dAssign( &m_vServerDir, pCharState->sCharStateBase.vCurDir.x, pCharState->sCharStateBase.vCurDir.y, pCharState->sCharStateBase.vCurDir.z );

	ServerConditionUpdateAnalysis(m_sCharState.sCharStateBase.byStateID, pCharState->sCharStateBase.dwConditionFlag );

	SetFightingMode( pCharState->sCharStateBase.bFightMode );
	SetAirMode(AIR_STATE_OFF);
}

void CNtlFSMVehicleActAgent::ServerConditionUpdateAnalysis( RwUInt8 byServerState, RwUInt64 qwCondition )
{
	CNtlFSMCharActAgent::ServerConditionUpdateAnalysis( byServerState, qwCondition );
}

void CNtlFSMVehicleActAgent::ServerAspectUpdateAnalysis( sASPECTSTATE& sAspectState, RwBool bCreateUpdate )
{
	CNtlFSMCharActAgent::ServerAspectUpdateAnalysis( sAspectState, bCreateUpdate );
}
