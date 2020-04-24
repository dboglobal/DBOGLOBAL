#include "precomp_ntlsimulation.h"
#include "NtlBehaviorVehicleDirHelper.h"

#include "NtlPLEvent.h"
#include "NtlSobVehicle.h"
#include "NtlSobProxy.h"
#include "NtlSLLogic.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSLApi.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSobVehicleAttr.h"
#include "NtlVehicleHelper.h"


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


CNtlBHVehicleEntityMng::CNtlBHVehicleEntityMng( void )
{
	m_defEntityListIT = m_defEntityList.end();
}

CNtlBHVehicleEntityMng::~CNtlBHVehicleEntityMng( void )
{
	ClearEntity();
}

void CNtlBHVehicleEntityMng::AttachEntity( CNtlBHVehicleEntity* pEntity )
{
	m_defEntityList.push_back( pEntity );
	m_defEntityListIT = m_defEntityList.end();
}

void CNtlBHVehicleEntityMng::ClearEntity( void )
{
	vecdef_EntityList::iterator it = m_defEntityList.begin();
	for ( ; it != m_defEntityList.end(); ++it )
	{
		NTL_DELETE( *it );
	}
	m_defEntityList.clear();

	m_defEntityListIT = m_defEntityList.end();
}

RwBool CNtlBHVehicleEntityMng::Update( RwReal fElapseTime )
{
	if ( m_defEntityList.empty() )
	{
		return FALSE;
	}

	// Begin
	if ( m_defEntityListIT == m_defEntityList.end() )
	{
		m_defEntityListIT = m_defEntityList.begin();

		if ( !(*m_defEntityListIT)->Begin() )
		{
			// End
			(*m_defEntityListIT)->End();

			NTL_DELETE( *m_defEntityListIT );

			m_defEntityList.erase( m_defEntityListIT );

			m_defEntityListIT = m_defEntityList.end();
		}

		return TRUE;
	}

	// Update
	if ( !(*m_defEntityListIT)->Update( fElapseTime ) )
	{
		// End
		(*m_defEntityListIT)->End();

		NTL_DELETE( *m_defEntityListIT );

		m_defEntityList.erase( m_defEntityListIT );

		m_defEntityListIT = m_defEntityList.end();

		return TRUE;
	}

    return TRUE;
}

void CNtlBHVehicleEntityMng::HandleEvents( RWS::CMsg& pMsg )
{
	if ( m_defEntityListIT != m_defEntityList.end() )
	{
		(*m_defEntityListIT)->HandleEvents( pMsg );
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


CNtlBHVehicleSyncGroup::~CNtlBHVehicleSyncGroup( void )
{
	ClearEntity();
}

void CNtlBHVehicleSyncGroup::AttachEntity( CNtlBHVehicleEntity* pEntity )
{
	sSYNC_ENTITY sSyncEntity;

	sSyncEntity.pEntity = pEntity;

	m_defSyncGroupList.push_back( sSyncEntity );
}

void CNtlBHVehicleSyncGroup::ClearEntity( void )
{
	vecdef_SyncGroupList::iterator it = m_defSyncGroupList.begin();
	for ( ; it != m_defSyncGroupList.end(); ++it )
	{
		sSYNC_ENTITY& sSyncEntity = *it;

		NTL_DELETE( sSyncEntity.pEntity );
	}
	m_defSyncGroupList.clear();
}

RwBool CNtlBHVehicleSyncGroup::Begin( void )
{
	vecdef_SyncGroupList::iterator it = m_defSyncGroupList.begin();
	for ( ; it != m_defSyncGroupList.end(); ++it )
	{
		sSYNC_ENTITY& sSyncEntity = *it;

		if ( !sSyncEntity.pEntity->Begin() )
		{
			sSyncEntity.bFinish = TRUE;
		}
	}

	return TRUE;
}

RwBool CNtlBHVehicleSyncGroup::Update( RwReal fElapseTime )
{
	RwInt32 nFinishCnt = 0;

	vecdef_SyncGroupList::iterator it = m_defSyncGroupList.begin();
	for ( ; it != m_defSyncGroupList.end(); ++it )
	{
		sSYNC_ENTITY& sSyncEntity = *it;

		if ( !sSyncEntity.bFinish )
		{
			if ( !sSyncEntity.pEntity->Update( fElapseTime ) )
			{
				sSyncEntity.bFinish = TRUE;

				nFinishCnt++;
			}
		}
		else
		{
			nFinishCnt++;
		}
	}

	return nFinishCnt == m_defSyncGroupList.size() ? FALSE : TRUE;
}

void CNtlBHVehicleSyncGroup::End( void )
{
	vecdef_SyncGroupList::iterator it = m_defSyncGroupList.begin();
	for ( ; it != m_defSyncGroupList.end(); ++it )
	{
		sSYNC_ENTITY& sSyncEntity = *it;

		sSyncEntity.pEntity->End();

		NTL_DELETE( sSyncEntity.pEntity );
	}

	m_defSyncGroupList.clear();
}

void CNtlBHVehicleSyncGroup::HandleEvents( RWS::CMsg& pMsg )
{
	vecdef_SyncGroupList::iterator it = m_defSyncGroupList.begin();
	for ( ; it != m_defSyncGroupList.end(); ++it )
	{
		sSYNC_ENTITY& sSyncEntity = *it;

		sSyncEntity.pEntity->HandleEvents( pMsg );
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


RwBool CNtlBHVehicleAOIRideOn::Begin( void )
{
	m_pVehicle->EnableVisible( TRUE );

	m_pVehicle->AttachDriver( TRUE );

	CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pVehicle->GetBehaviorData()); 
	SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

	// engine disabled by daneos
	//RwReal fRideOnHeight = 0.f;
	/*if ( pCtrlStuff->sVehicle.bEngineOn )*/
	//{
	//	CNtlSobVehicleAttr* pAttr = (CNtlSobVehicleAttr*)m_pVehicle->GetSobAttr();
	//	fRideOnHeight = pAttr->GetVehicleTable()->wRunHeight;
	//}

	RwV3d vCurPos = m_pVehicle->GetPosition();
	vCurPos.y += 1.f;

	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight( &vCurPos, sHStuff );
	vCurPos.y = sHStuff.fFinialHeight;
	vCurPos.y += m_pVehicle->GetRideOnHeight() / 100.f;	// Centimeter를 Meter로 환산한다

	m_pVehicle->SetPosition( &vCurPos );

	m_bFinish = TRUE;

	return FALSE;
}

RwBool CNtlBHVehicleAOIRideOn::Update( RwReal fElapseTime )
{
	return !m_bFinish;
}

void CNtlBHVehicleAOIRideOn::End( void )
{
}

void CNtlBHVehicleAOIRideOn::HandleEvents( RWS::CMsg& pMsg )
{
}

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


CNtlBHVehicleNoDirRideOn::CNtlBHVehicleNoDirRideOn( CNtlSobActor* pDriver, CNtlSobVehicle* pVehicle )
: CNtlBHVehicleEntity( pDriver, pVehicle )
{
	m_fSpawnTime = 0.5f;
	m_bFinish = FALSE;
}


RwBool CNtlBHVehicleNoDirRideOn::Begin( void )
{
	m_vSpawnPos = m_pVehicle->GetPosition();
	m_vSpawnPos.y += 1.f;

	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight( &m_vSpawnPos, sHStuff );
	m_vSpawnPos.y = sHStuff.fFinialHeight;

	m_vVehiclePos = m_vSpawnPos;
	m_vVehiclePos.y += m_pVehicle->GetRideOnHeight() / 100.f;	// Centimeter를 Meter로 환산한다

	m_pDriver->GetSobProxy()->CreatePLEffect( NTL_VID_VEHICLE_SPAWN, &m_vSpawnPos );

	return TRUE;
}

RwBool CNtlBHVehicleNoDirRideOn::Update( RwReal fElapseTime )
{
	m_fSpawnTime -= fElapseTime;

	if ( m_fSpawnTime < 0.f )
	{
		m_pVehicle->SetPosition( &m_vVehiclePos );

		m_pVehicle->EnableVisible( TRUE );

		m_pDriver->SetPosition( &m_vVehiclePos );

		m_pVehicle->AttachDriver( TRUE );

		CNtlVehicleHelper clVehicleHelper;
		clVehicleHelper.AttachPlayer( m_pDriver->GetSerialID(), m_pVehicle->GetSerialID() );
		clVehicleHelper.Vehicle_Anim_Idle( FALSE );
		clVehicleHelper.DetachPlayer();

		m_pVehicle->UpdateAttachDriverAndVehicle( 0.f );

		if (m_pDriver->GetClassID() == SLCLASS_AVATAR)
		{
			API_GetSLPacketGenerator()->SendVehicleDirectPlayCancelNfy();
		}

		m_bFinish = TRUE;
	}

	return !m_bFinish;
}

void CNtlBHVehicleNoDirRideOn::End( void )
{
}

void CNtlBHVehicleNoDirRideOn::HandleEvents( RWS::CMsg& pMsg )
{
}

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


RwBool CNtlBHVehicleCharThrowVehicleItem::Begin( void )
{
	m_bFinish = FALSE;

	RwV3d vDir = m_pVehicle->GetPosition() - m_pDriver->GetPosition(); vDir.y = 0.f;

	if ( RwV3dNormalize( &vDir, &vDir ) > 0.0001f )
	{
		m_pDriver->SetDirection( &vDir );
	}

	m_pDriver->GetSobProxy()->SetBaseAnimation( NML_USE_HOIPOI_CAPSULE, FALSE );

	return TRUE;
}

RwBool CNtlBHVehicleCharThrowVehicleItem::Update( RwReal fElapseTime )
{
	return !m_bFinish;
}

void CNtlBHVehicleCharThrowVehicleItem::End( void )
{
}

void CNtlBHVehicleCharThrowVehicleItem::HandleEvents( RWS::CMsg& pMsg )
{
	if ( pMsg.Id == g_EventAnimEnd )
	{
		SNtlEventAnimEnd* pData = (SNtlEventAnimEnd*) pMsg.pData;

		if ( pData->uiBaseAnimKey == NML_USE_HOIPOI_CAPSULE )
		{
			m_bFinish = TRUE;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
