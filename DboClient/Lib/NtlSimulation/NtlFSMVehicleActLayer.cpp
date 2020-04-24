#include "precomp_ntlsimulation.h"
#include "NtlFSMVehicleActLayer.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlFSMLookAtManager.h"
#include "NtlFSMVehicleActAgent.h"
#include "NtlFSMVehicleAct.h"
#include "NtlFSMVehicleController.h"


DEFINITION_MEMORY_POOL( CNtlFSMVehicleActLayer )


CNtlFSMVehicleActLayer::CNtlFSMVehicleActLayer( void )
{
}

RwBool CNtlFSMVehicleActLayer::Create( RwUInt32 uiController )
{
	m_pFSM = GetNtlFSMLookAtManager()->GetFSMVehicleAct();

	m_pAgent = NTL_NEW CNtlFSMVehicleActAgent;

	if ( !m_pAgent->Create() )
	{
		return FALSE;
	}

	m_pAgent->SetActor( m_pActor );

	CreateController( uiController );

	return TRUE;
}

void CNtlFSMVehicleActLayer::Destroy( void )
{
	CNtlFSMLayer::Destroy();
}

void CNtlFSMVehicleActLayer::Update( RwReal fElapsed )
{
	CNtlFSMLayer::Update( fElapsed ); 
}

RwUInt32 CNtlFSMVehicleActLayer::HandleEvents( RWS::CMsg &pMsg )
{
	return CNtlFSMLayer::HandleEvents( pMsg ); 
}

void CNtlFSMVehicleActLayer::CreateController( RwUInt32 uiController )
{
	if ( uiController == SLCONTROLLER_VEHICLE )
	{
		CNtlFSMController* pController = NTL_NEW CNtlFSMVehicleController;
		m_pAgent->SetController( pController ); 
		pController->SetAgent( m_pAgent ); 
		pController->SetActor( m_pActor );
	}
	else if ( uiController == SLCONTROLLER_VEHICLE_SYNC )
	{
		CNtlFSMController* pController = NTL_NEW CNtlFSMVehicleSyncController;
		m_pAgent->SetController( pController ); 
		pController->SetAgent( m_pAgent ); 
		pController->SetActor( m_pActor );
	}
}
