#include "precomp_ntlsimulation.h"
#include "NtlFSMVehicleAct.h"

#include "NtlDebug.h"

#include "NtlFSMStateBase.h"
#include "NtlFSMVehicleActState.h"
#include "NtlSobActor.h"


DEFINITION_MEMORY_POOL( CNtlFSMVehicleAct )


CNtlFSMVehicleAct::CNtlFSMVehicleAct( void )
{
}

CNtlFSMVehicleAct::~CNtlFSMVehicleAct( void )
{
}

RwBool CNtlFSMVehicleAct::Create(void)
{
	CNtlFSMBase::Create();

	CNtlFSMStateBase *pState;

	// ride on / off
	pState = NTL_NEW CNtlFSMVehicleRideOnOff;
	Register( pState );

	// idle
	pState = NTL_NEW CNtlFSMVehicleActIdleState;
	Register( pState );

	// move
	pState = NTL_NEW CNtlFSMVehicleMoveState;
	Register( pState );

	// follow
	pState = NTL_NEW CNtlFSMVehicleFollowState;
	Register( pState );

	// adjust move
	pState = NTL_NEW CNtlFSMVehicleAdjustMoveState;
	Register( pState );

	// falling
	pState = NTL_NEW CNtlFSMVehicleActFallingState;
	Register( pState );

	return TRUE;
}

void CNtlFSMVehicleAct::Destroy( void )
{
	CNtlFSMBase::Destroy();
}

CNtlFSMStateBase* CNtlFSMVehicleAct::Transition( CNtlSobActor* pActor, CNtlFSMStateBase* pOldState, RwUInt32 uiNewStateId ) 
{
	pOldState->Exit();
	pActor->NotifyStateExit( pOldState->GetStateId(), uiNewStateId );
	DELETE_FSMSTATE_FACTORY( pOldState );

	CNtlFSMStateBase* pNewState = Find( uiNewStateId );

	if ( NULL == pNewState )
	{
		pNewState = Find( NTL_FSMSID_IDLE );
	}

	pNewState = CREATE_FSMSTATE_FACTORY( pNewState );
	pNewState->SetActor( pActor );
	pNewState->Enter();
	pActor->NotifyStateEnter( pNewState->GetStateId() );

	return pNewState;
}
