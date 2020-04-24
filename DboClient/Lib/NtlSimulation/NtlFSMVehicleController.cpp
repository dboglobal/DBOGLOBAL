#include "precomp_ntlsimulation.h"
#include "NtlFSMVehicleController.h"

// simulation
#include "NtlFSMDef.h"
#include "NtlSobActor.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSLApi.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlFSMVehicleController )


CNtlFSMVehicleController::CNtlFSMVehicleController( void )
{
	SetControllerId( SLCONTROLLER_VEHICLE );
}

CNtlFSMVehicleController::~CNtlFSMVehicleController( void )
{
}

void CNtlFSMVehicleController::Update( RwReal fElapsed )
{
	CNtlFSMPlayerController::Update( fElapsed );
}

RwUInt32 CNtlFSMVehicleController::HandleEvents( RWS::CMsg &pMsg )
{
	return CNtlFSMPlayerController::HandleEvents( pMsg );
}

void CNtlFSMVehicleController::NotifyUpdateChangeState( RwUInt32 uiOldState, RwUInt32 uiNextState )
{
	if (uiOldState == NTL_FSMSID_FOLLOW)
	{
		if (uiNextState != NTL_FSMSID_SWIMMING && uiNextState != NTL_FSMSID_FALLING)
		{
			RwV3d vPos = m_pActor->GetPosition();
			RwV3d vDir = m_pActor->GetDirection();

			API_GetSLPacketGenerator()->SendKeyboardMove(vPos, vDir, NTL_MOVE_NONE, m_pActor->GetServerSyncAvatarType());
		}

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
		CNtlSobActor *pTarActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(pCtrlStuff->hTargetSerialId));
		if (pTarActor)
		{
			if (Logic_IsCommunityActor(pTarActor) && Logic_CanActorCommunity(m_pActor, pTarActor, pCtrlStuff->fFollowRange))
			{
				Logic_BeginActorCommunity(m_pActor, pTarActor);
			}
		}
	}
	else
	{
		CNtlFSMPlayerController::NotifyUpdateChangeState(uiOldState, uiNextState);
	}
}

void CNtlFSMVehicleController::NotifyEvents( RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2 )
{
	return CNtlFSMPlayerController::NotifyEvents( pMsg, uiEventResult, uiParam1, uiParam2 );
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlFSMVehicleSyncController )


CNtlFSMVehicleSyncController::CNtlFSMVehicleSyncController( void )
{
	SetControllerId( SLCONTROLLER_VEHICLE_SYNC );
}

CNtlFSMVehicleSyncController::~CNtlFSMVehicleSyncController( void )
{
}

void CNtlFSMVehicleSyncController::Update( RwReal fElapsed )
{
}

RwUInt32 CNtlFSMVehicleSyncController::HandleEvents( RWS::CMsg &pMsg )
{
	return NTL_FSM_EVENTRES_PASS;
}

void CNtlFSMVehicleSyncController::NotifyUpdateChangeState( RwUInt32 uiOldState, RwUInt32 uiNextState )
{
}

void CNtlFSMVehicleSyncController::NotifyEvents( RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2 )
{
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
