/*****************************************************************************
 *
 * File			: NtlBehaviorVehicleDirHelper.h
 * Author		: JeongHo, Rho
 * Copyright	: (аж)NTL
 * Date			: 2008. 11. 14	
 * Abstract		: vehicle helper class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_BEHAVIOR_VEHICLE_DIR_HELPER_H_
#define __NTL_BEHAVIOR_VEHICLE_DIR_HELPER_H_


#include "ceventhandler.h"


class CNtlSobActor;
class CNtlSobVehicle;
class CNtlBHVehicleEntity;


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBHVehicleEntityMng
{
public:
	typedef std::vector< CNtlBHVehicleEntity* > vecdef_EntityList;
	typedef vecdef_EntityList::iterator vecdef_EntityListIT;

protected:
	vecdef_EntityList			m_defEntityList;
	vecdef_EntityListIT			m_defEntityListIT;

public:
	CNtlBHVehicleEntityMng( void );
	~CNtlBHVehicleEntityMng( void );

public:
	void						AttachEntity( CNtlBHVehicleEntity* pEntity );
	void						ClearEntity( void );
	RwBool						Update( RwReal fElapseTime );
	void						HandleEvents( RWS::CMsg& pMsg );
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBHVehicleEntity
{
protected:
	CNtlSobActor*				m_pDriver;
	CNtlSobVehicle*				m_pVehicle;

public:
	CNtlBHVehicleEntity( CNtlSobActor* pDriver, CNtlSobVehicle* pVehicle )
	{
		m_pDriver = pDriver;
		m_pVehicle = pVehicle;
	}

	virtual ~CNtlBHVehicleEntity( void ) { return; }

public:
	virtual RwBool				Begin( void )					= 0;
	virtual RwBool				Update( RwReal fElapseTime )	= 0;
	virtual void				End( void )						= 0;
	virtual void				HandleEvents( RWS::CMsg& pMsg )	= 0;
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBHVehicleSyncGroup : public CNtlBHVehicleEntity
{
public:
	struct sSYNC_ENTITY
	{
		RwBool					bFinish;
		CNtlBHVehicleEntity*	pEntity;

		sSYNC_ENTITY( void )
		{
			bFinish				= FALSE;
			pEntity				= NULL;
		}
	};

	typedef std::vector< sSYNC_ENTITY > vecdef_SyncGroupList;

protected:
	vecdef_SyncGroupList		m_defSyncGroupList;

public:
	CNtlBHVehicleSyncGroup( CNtlSobActor* pDriver, CNtlSobVehicle* pVehicle )
		: CNtlBHVehicleEntity( pDriver, pVehicle ) { return; }

	virtual ~CNtlBHVehicleSyncGroup( void );

public:
	void						AttachEntity( CNtlBHVehicleEntity* pEntity );
	void						ClearEntity( void );

	virtual RwBool				Begin( void );
	virtual RwBool				Update( RwReal fElapseTime );
	virtual void				End( void );
	virtual void				HandleEvents( RWS::CMsg& pMsg );
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBHVehicleAOIRideOn : public CNtlBHVehicleEntity
{
protected:
	RwBool						m_bFinish;

public:
	CNtlBHVehicleAOIRideOn( CNtlSobActor* pDriver, CNtlSobVehicle* pVehicle )
		: CNtlBHVehicleEntity( pDriver, pVehicle ) { return; }

	virtual ~CNtlBHVehicleAOIRideOn( void ) { return; }

public:
	virtual RwBool				Begin( void );
	virtual RwBool				Update( RwReal fElapseTime );
	virtual void				End( void );
	virtual void				HandleEvents( RWS::CMsg& pMsg );
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBHVehicleNoDirRideOn : public CNtlBHVehicleEntity
{
protected:
	RwReal						m_fSpawnTime;

	RwV3d						m_vSpawnPos;
	RwV3d						m_vVehiclePos;

	RwBool						m_bFinish;

public:
	CNtlBHVehicleNoDirRideOn( CNtlSobActor* pDriver, CNtlSobVehicle* pVehicle );

	virtual ~CNtlBHVehicleNoDirRideOn( void ) { return; }

public:
	virtual RwBool				Begin( void );
	virtual RwBool				Update( RwReal fElapseTime );
	virtual void				End( void );
	virtual void				HandleEvents( RWS::CMsg& pMsg );
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBHVehicleCharThrowVehicleItem : public CNtlBHVehicleEntity
{
protected:
	RwBool						m_bFinish;

public:
	CNtlBHVehicleCharThrowVehicleItem( CNtlSobActor* pDriver, CNtlSobVehicle* pVehicle )
		: CNtlBHVehicleEntity( pDriver, pVehicle ) { return; }

	virtual ~CNtlBHVehicleCharThrowVehicleItem( void ) { return; }

public:
	virtual RwBool				Begin( void );
	virtual RwBool				Update( RwReal fElapseTime );
	virtual void				End( void );
	virtual void				HandleEvents( RWS::CMsg& pMsg );
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////



#endif
