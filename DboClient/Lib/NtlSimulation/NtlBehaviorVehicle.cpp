#include "precomp_ntlsimulation.h"
#include "NtlBehaviorVehicle.h"

#include "NtlFSMDef.h"
#include "NtlSobActor.h"
#include "NtlSLEventFunc.h"
#include "NtlSobVehicle.h"
#include "NtlSobPlayer.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSobProxy.h"
#include "NtlSLLogic.h"


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlBahaviorVehicleRideOn )


CNtlBahaviorVehicleRideOn::CNtlBahaviorVehicleRideOn( void )
{
	m_pSobDriver = NULL;
}

CNtlBahaviorVehicleRideOn::~CNtlBahaviorVehicleRideOn( void )
{
}

void CNtlBahaviorVehicleRideOn::SetData( CNtlSobActor* pSobDriver )
{
	m_pSobDriver = pSobDriver;
}

void CNtlBahaviorVehicleRideOn::Enter( void )
{
	CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* >( m_pActor->GetBehaviorData() );
	SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

	if ( pCtrlStuff->sVehicle.bCreateUpdate )
	{
		CNtlBHVehicleEntityMng::AttachEntity( NTL_NEW CNtlBHVehicleAOIRideOn( m_pSobDriver, (CNtlSobVehicle*)m_pActor ) );
	}
	else
	{
		// Vehicle item을 투척한다
		CNtlBHVehicleEntityMng::AttachEntity( NTL_NEW CNtlBHVehicleCharThrowVehicleItem( m_pSobDriver, (CNtlSobVehicle*)m_pActor ) );

		// Vehicle 탑승
		CNtlBHVehicleEntityMng::AttachEntity( NTL_NEW CNtlBHVehicleNoDirRideOn( m_pSobDriver, (CNtlSobVehicle*)m_pActor ) );
	}
}

void CNtlBahaviorVehicleRideOn::Exit( void )
{
	// Avatar인 경우 UI를 출력 한다

	if ( m_pSobDriver->GetClassID() == SLCLASS_AVATAR )
	{
		m_pSobDriver->SetEventHook( FALSE );

		CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* >( m_pSobDriver->GetBehaviorData() );
		if (pBeData)
		{
			SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

			CNtlSLEventGenerator::SobVehicleEngine(m_pSobDriver->GetSerialID(),
				VEHICLE_READY,
				pCtrlStuff->sVehicle.hVehicleItem);

			m_pSobDriver->SetEventHook(TRUE);
		}
	}

	CNtlSLEventGenerator::SobServerUpdateStatePostMsg( m_pActor->GetSerialID(), m_pActor->GetServerFullState());
}

void CNtlBahaviorVehicleRideOn::Update( RwReal fElapsed )
{
	if ( !CNtlBHVehicleEntityMng::Update( fElapsed ) )
	{
		Finish();
	}
}

RwUInt32 CNtlBahaviorVehicleRideOn::HandleEvents( RWS::CMsg& pMsg )
{
	CNtlBHVehicleEntityMng::HandleEvents( pMsg );

	return 0;
}

void CNtlBahaviorVehicleRideOn::UpdateData( void )
{
}


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlBahaviorVehicleRideOff )


CNtlBahaviorVehicleRideOff::CNtlBahaviorVehicleRideOff( void )
{
	m_pSobDriver = NULL;
}

CNtlBahaviorVehicleRideOff::~CNtlBahaviorVehicleRideOff( void )
{
}

void CNtlBahaviorVehicleRideOff::SetData( CNtlSobActor* pSobDriver )
{
	m_pSobDriver = pSobDriver;
}

void CNtlBahaviorVehicleRideOff::Enter( void )
{
	// Effect when getting off
	m_pActor->GetSobProxy()->CreatePLEffect( NTL_VID_VEHICLE_RIDEOFF, &m_pActor->GetPosition() );

	m_pSobDriver->SetEventHook( FALSE );

	// Driver의 상태를 복구 시킨다

	sCHARSTATE* pState = m_pActor->GetServerFullState();
	CNtlSLEventGenerator::SobServerUpdateAspect( m_pSobDriver->GetSerialID(), pState->sCharStateBase.aspectState );
	CNtlSLEventGenerator::SobServerUpdateState( m_pSobDriver->GetSerialID(), pState );

	// Avatar인 경우 UI를 제거 한다

	if ( m_pSobDriver->GetClassID() == SLCLASS_AVATAR )
	{
		CNtlSLEventGenerator::SobVehicleEngine( m_pSobDriver->GetSerialID(), VEHICLE_ENGINE_END );
	}

	m_pSobDriver->SetEventHook( TRUE );

	Finish();
}

void CNtlBahaviorVehicleRideOff::Exit( void )
{
}

void CNtlBahaviorVehicleRideOff::Update( RwReal fElapsed )
{
}

RwUInt32 CNtlBahaviorVehicleRideOff::HandleEvents( RWS::CMsg& pMsg )
{
	return 0;
}

void CNtlBahaviorVehicleRideOff::UpdateData( void )
{
}


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
