#include "precomp_ntlsimulation.h"
#include "NtlVehicleHelper.h"

// shared
#include "GraphicDataTable.h"
#include "VehicleTable.h"

// Presentation
#include "NtlPLEvent.h"

// simmulation
#include "NtlSLLogic.h"
#include "NtlSob.h"
#include "NtlSobManager.h"
#include "NtlSobPlayer.h"
#include "NtlSobVehicle.h"
#include "NtlSobProxy.h"
#include "NtlSobVehicleAttr.h"


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


#if ((VEHICLE_SRP1_ANIMATION_END - VEHICLE_SRP1_ANIMATION_START) != (VEHICLE_SRP_X_ANIMATION_END - VEHICLE_SRP_X_ANIMATION_START))
	#error Please checking the vehicle animation list
#endif


CNtlVehicleHelper::CNtlVehicleHelper( void )
{
	// Vehicle animation mapper
	m_defAminMapper[1] = VEHICLE_SRP1_ANIMATION_START;
	m_defAminMapper[2] = VEHICLE_SRP2_ANIMATION_START;

	m_pPlayer = NULL;
	m_pVehicle = NULL;

	m_pVehicleTblData = NULL;

	m_bStrunAniPlay = FALSE;
}

CNtlVehicleHelper::~CNtlVehicleHelper( void )
{
	DetachPlayer();
}

void CNtlVehicleHelper::AttachPlayer( SERIAL_HANDLE hDriver, SERIAL_HANDLE hVehicle )
{
	CNtlSobPlayer* pPlayer = (CNtlSobPlayer*) GetNtlSobManager()->GetSobObject( hDriver );
	CNtlSobVehicle* pVehicle = (CNtlSobVehicle*) GetNtlSobManager()->GetSobObject( hVehicle );

	if ( pPlayer )
	{
		RwUInt32 uiClassID = pPlayer->GetClassID();

		if ( uiClassID == SLCLASS_PLAYER ||
			 uiClassID == SLCLASS_AVATAR )
		{
			m_pPlayer = (CNtlSobPlayer*) pPlayer;
			m_pVehicle = (CNtlSobVehicle*) pVehicle;
			m_pVehicleTblData = ((CNtlSobVehicleAttr*)m_pVehicle->GetSobAttr())->GetVehicleTable();
		}
		else
		{
			m_pPlayer = NULL;
			m_pVehicle = NULL;
			m_pVehicleTblData = NULL;
		}
	}
	else
	{
		m_pPlayer = NULL;
		m_pVehicle = NULL;
		m_pVehicleTblData = NULL;
	}
}

void CNtlVehicleHelper::DetachPlayer( void )
{
	m_pPlayer = NULL;
	m_pVehicle = NULL;
	m_pVehicleTblData = NULL;
}

RwBool CNtlVehicleHelper::IsDriver( void )
{
	return Logic_IsVehicleDriver( m_pPlayer );
}

void CNtlVehicleHelper::Vehicle_EnableVisible( RwBool bVisible )
{
	if ( m_pPlayer )
	{
		m_pPlayer->EnableVisible( bVisible );
	}

	if ( m_pVehicle )
	{
		m_pVehicle->EnableVisible( bVisible );
	}
}

void CNtlVehicleHelper::Vehicle_Anim_Idle( RwBool bBlend /*= TRUE*/ )
{
	if ( !IsDriver() ) return;

	if ( m_bStrunAniPlay ) return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		if ( !bBlend )
		{
			m_pPlayer->GetSobProxy()->SetAnimBlendNoneOneTime();
		}

		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_IDLE );

		if (uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation(uiVehicleAnimIdx);
	}

	if ( m_pVehicle )
	{
		if ( !bBlend )
		{
			m_pVehicle->GetSobProxy()->SetAnimBlendNoneOneTime();
		}

		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_IDLE );
	}
}

void CNtlVehicleHelper::Vehicle_Anim_Run( void )
{
	if ( !IsDriver() ) return;

	if ( m_bStrunAniPlay ) return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_RUN );

		if(uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation( uiVehicleAnimIdx );
	}

	if ( m_pVehicle )
	{
		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_RUN );
	}
}

void CNtlVehicleHelper::Vehicle_Anim_Start( void )
{
	if ( !IsDriver() ) return;

	if ( m_bStrunAniPlay ) return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_START );
		
		if (uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation(uiVehicleAnimIdx);
	}

	if ( m_pVehicle )
	{
		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_START, FALSE );
	}
}

void CNtlVehicleHelper::Vehicle_Anim_Stop( void )
{
	if ( !IsDriver() ) return;

	if ( m_bStrunAniPlay ) return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_STOP );
		
		if (uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation(uiVehicleAnimIdx);
	}

	if ( m_pVehicle )
	{
		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_STOP, FALSE );
	}
}

void CNtlVehicleHelper::Vehicle_Anim_Jump( void )
{
	if(!IsDriver())
		return;

	if (m_bStrunAniPlay)
		return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_JUMP );
		
		if (uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation(uiVehicleAnimIdx);
	}

	if ( m_pVehicle )
	{
		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_JUMP, FALSE );
	}
}

void CNtlVehicleHelper::Vehicle_Anim_TurnLeft( void )
{
	if ( !IsDriver() ) return;

	if ( m_bStrunAniPlay ) return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_TURN_LEFT );
		
		if (uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation(uiVehicleAnimIdx);
	}

	if ( m_pVehicle )
	{
		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_TURN_LEFT, FALSE );
	}
}

void CNtlVehicleHelper::Vehicle_Anim_TurnRight( void )
{
	if ( !IsDriver() ) return;

	if ( m_bStrunAniPlay ) return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_TURN_RIGHT );
		
		if (uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation(uiVehicleAnimIdx);
	}

	if ( m_pVehicle )
	{
		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_TURN_RIGHT, FALSE );
	}
}

void CNtlVehicleHelper::Vehicle_Anim_Landing( void )
{
	if ( !IsDriver() ) return;

	if ( m_bStrunAniPlay ) return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_LANDING );
	
		if (uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation(uiVehicleAnimIdx);
	}

	if ( m_pVehicle )
	{
		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_LANDING, FALSE );
	}
}

void CNtlVehicleHelper::UpdateEvent( RWS::CMsg& pMsg )
{
	if ( pMsg.Id == g_EventSobVehicleStunt )
	{
		Vehicle_Anim_Jump();

		m_bStrunAniPlay = TRUE;
	}
	else if ( pMsg.Id == g_EventAnimEnd )
	{
		SNtlEventAnimEnd* pData = (SNtlEventAnimEnd*) pMsg.pData;

		if ( pData->uiBaseAnimKey == VEHICLE_JUMP )
		{
			m_bStrunAniPlay = FALSE;
		}
	}
}

RwUInt32 CNtlVehicleHelper::GetVehicleAnimIdxFromSRPType( RwUInt8 bySRPType, RwUInt8 byRVAnimIdx )
{
	mapdef_VEHICLE_ANIM_MAPPER::iterator it = m_defAminMapper.find( bySRPType );
	if ( it == m_defAminMapper.end() ) return 0xffffffff;

	return (RwUInt32)it->second + (RwUInt32)byRVAnimIdx;
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
