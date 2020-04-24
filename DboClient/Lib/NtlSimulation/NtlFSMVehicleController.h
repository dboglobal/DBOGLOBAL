/*****************************************************************************
 *
 * File			: NtlFSMVehicleController.h
 * Author		: JeongHo, Rho
 * Copyright	: (аж)NTL
 * Date			: 2008. 11. 12	
 * Abstract		: FSM vehicle controller class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_FSM_VEHICLECONTROLLER_H__
#define __NTL_FSM_VEHICLECONTROLLER_H__


#include "NtlSLEvent.h"
#include "NtlFSMCharController.h"


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlFSMVehicleController : public CNtlFSMPlayerController
{
	DECLEAR_MEMORY_POOL( CNtlFSMVehicleController, NTL_DEFAULT_MEMORY_POOL )

public:

	CNtlFSMVehicleController( void );
	~CNtlFSMVehicleController( void );

	virtual void		Update( RwReal fElapsed );	
	virtual RwUInt32	HandleEvents( RWS::CMsg &pMsg );
	virtual void		NotifyUpdateChangeState( RwUInt32 uiOldState, RwUInt32 uiNextState );
	virtual void		NotifyEvents( RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2 );
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlFSMVehicleSyncController : public CNtlFSMController
{
	DECLEAR_MEMORY_POOL( CNtlFSMVehicleSyncController, NTL_DEFAULT_MEMORY_POOL )

public:

	CNtlFSMVehicleSyncController( void );
	~CNtlFSMVehicleSyncController( void );

	virtual void		Update( RwReal fElapsed );	
	virtual RwUInt32	HandleEvents( RWS::CMsg &pMsg );
	virtual void		NotifyUpdateChangeState( RwUInt32 uiOldState, RwUInt32 uiNextState );
	virtual void		NotifyEvents( RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2 );
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


#endif
