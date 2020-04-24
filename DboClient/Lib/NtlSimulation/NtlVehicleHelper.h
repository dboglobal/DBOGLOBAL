/*****************************************************************************
 *
 * File			: NtlVehicleHelper.h
 * Author		: JeongHo, Rho
 * Copyright	: (аж)NTL
 * Date			: 2008. 11. 6
 * Abstract		: Vehicle helper class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_VEHICLEHELPER_H__
#define __NTL_VEHICLEHELPER_H__


#include "NtlSLDef.h"
#include "ceventhandler.h"


class CNtlSob;
class CNtlSobPlayer;
class CNtlSobVehicle;


struct sVEHICLE_TBLDAT;


enum
{
	VEHICLE_SRP_X_ANIMATION_START	= 0,
	VEHICLE_SRP_X_IDLE				= VEHICLE_SRP_X_ANIMATION_START,
	VEHICLE_SRP_X_RUN,
	VEHICLE_SRP_X_START,
	VEHICLE_SRP_X_STOP,
	VEHICLE_SRP_X_JUMP,
	VEHICLE_SRP_X_TURN_LEFT,
	VEHICLE_SRP_X_TURN_RIGHT,
	VEHICLE_SRP_X_LANDING,
	VEHICLE_SRP_X_ANIMATION_END		= VEHICLE_SRP_X_LANDING,
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlVehicleHelper
{
public:
	typedef std::map< RwUInt8, RwUInt32 > mapdef_VEHICLE_ANIM_MAPPER;

protected:
	mapdef_VEHICLE_ANIM_MAPPER	m_defAminMapper;

	CNtlSobPlayer*				m_pPlayer;
	CNtlSobVehicle*				m_pVehicle;

	sVEHICLE_TBLDAT*			m_pVehicleTblData;

	RwBool						m_bStrunAniPlay;

public:
	CNtlVehicleHelper( void );
	virtual ~CNtlVehicleHelper( void );

	virtual void				AttachPlayer( SERIAL_HANDLE hDriver, SERIAL_HANDLE hVehicle );
	virtual void				DetachPlayer( void );

	RwBool						IsDriver( void );

	void						Vehicle_EnableVisible( RwBool bVisible );

	void						Vehicle_Anim_Idle( RwBool bBlend = TRUE );
	void						Vehicle_Anim_Run( void );
	void						Vehicle_Anim_Start( void );
	void						Vehicle_Anim_Stop( void );
	void						Vehicle_Anim_Jump( void );
	void						Vehicle_Anim_TurnLeft( void );
	void						Vehicle_Anim_TurnRight( void );
	void						Vehicle_Anim_Landing( void );

	void						UpdateEvent( RWS::CMsg& pMsg );

protected:
	RwUInt32					GetVehicleAnimIdxFromSRPType( RwUInt8 bySRPType, RwUInt8 byRVAnimIdx );
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


#endif
