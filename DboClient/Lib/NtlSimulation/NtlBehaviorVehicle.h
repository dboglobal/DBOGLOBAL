/*****************************************************************************
 *
 * File			: NtlBehaviorVehicleMove.h
 * Author		: JeongHo, Rho
 * Copyright	: (аж)NTL
 * Date			: 2008. 11. 14	
 * Abstract		: vehicle behavior class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_BEHAVIOR_VEHICLE_H__
#define __NTL_BEHAVIOR_VEHICLE_H__


#include "NtlBehaviorBase.h"
#include "NtlBehaviorVehicleDirHelper.h"


class CNtlSobActor;


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBahaviorVehicleRideOn : public CNtlBehaviorBase, public CNtlBHVehicleEntityMng
{
	DECLEAR_MEMORY_POOL( CNtlBahaviorVehicleRideOn, 1 )
	DECLEAR_BEHAVIOR_FACTROY( CNtlBahaviorVehicleRideOn )

protected:
	CNtlSobActor*				m_pSobDriver;

public:
	CNtlBahaviorVehicleRideOn( void );
	virtual ~CNtlBahaviorVehicleRideOn( void );

	void						SetData( CNtlSobActor* pSobDriver );

	virtual void				Enter( void );
	virtual void				Exit( void );
	virtual void				Update( RwReal fElapsed );
	virtual RwUInt32			HandleEvents( RWS::CMsg& pMsg );
	virtual void				UpdateData( void );
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBahaviorVehicleRideOff : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL( CNtlBahaviorVehicleRideOff, 1 )
	DECLEAR_BEHAVIOR_FACTROY( CNtlBahaviorVehicleRideOff )

protected:
	CNtlSobActor*				m_pSobDriver;

public:
	CNtlBahaviorVehicleRideOff( void );
	virtual ~CNtlBahaviorVehicleRideOff( void );

	void						SetData( CNtlSobActor* pSobDriver );

	virtual void				Enter( void );
	virtual void				Exit( void );
	virtual void				Update( RwReal fElapsed );
	virtual RwUInt32			HandleEvents( RWS::CMsg& pMsg );
	virtual void				UpdateData( void );
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


#endif
