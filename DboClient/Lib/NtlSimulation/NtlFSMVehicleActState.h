/*****************************************************************************
 *
 * File			: NtlFSMVehicleActState.h
 * Author		: JeongHo, Rho
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 13	
 * Abstract		: Finite state machine vehicle action base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_FSM_VEHICLEACTSTATE_H__
#define __NTL_FSM_VEHICLEACTSTATE_H__


#include "ceventhandler.h"

#include "NtlFSMVehicleActStateBase.h"
#include "NtlVehicleHelper.h"


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlFSMVehicleRideOnOff : public CNtlFSMVehicleActStateBase
{
	DECLEAR_MEMORY_POOL( CNtlFSMVehicleRideOnOff, NTL_DEFAULT_MEMORY_POOL )
	DECLEAR_FSMSTATE_FACTROY( CNtlFSMVehicleRideOnOff )

protected:

	SERIAL_HANDLE	m_hDriver;

public:

	CNtlFSMVehicleRideOnOff( void );

	/**
	*  state의 update/event 처리 함수.
	*/
	virtual void Enter( void );
	virtual void Exit( void );
	virtual void Update( RwReal fElapsed );
	virtual RwUInt32 HandleEvents( RWS::CMsg &pMsg );
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlFSMVehicleActIdleState : public CNtlFSMVehicleActStateBase, public CNtlVehicleHelper
{
	DECLEAR_MEMORY_POOL( CNtlFSMVehicleActIdleState, NTL_DEFAULT_MEMORY_POOL )
	DECLEAR_FSMSTATE_FACTROY( CNtlFSMVehicleActIdleState )

public:

	CNtlFSMVehicleActIdleState( void );

	/**
	*  state의 update/event 처리 함수.
	*/
	virtual void Enter( void );
	virtual void Exit( void );
	virtual void Update( RwReal fElapsed );
	virtual RwUInt32 HandleEvents( RWS::CMsg &pMsg );
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlFSMVehicleMoveState : public CNtlFSMVehicleActStateBase
{
	DECLEAR_MEMORY_POOL( CNtlFSMVehicleMoveState, NTL_DEFAULT_MEMORY_POOL )
	DECLEAR_FSMSTATE_FACTROY( CNtlFSMVehicleMoveState )

protected:
	RwUInt8 m_byMoveType;

public:

	CNtlFSMVehicleMoveState( void );

	/**
	*  state의 update/event 처리 함수.
	*/
	virtual void Enter( void );
	virtual void Exit( void );
	virtual void Update( RwReal fElapsed );
	virtual RwUInt32 HandleEvents( RWS::CMsg &pMsg );
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlFSMVehicleFollowState : public CNtlFSMVehicleActStateBase
{
	DECLEAR_MEMORY_POOL( CNtlFSMVehicleFollowState, NTL_DEFAULT_MEMORY_POOL )
	DECLEAR_FSMSTATE_FACTROY( CNtlFSMVehicleFollowState )

public:

	CNtlFSMVehicleFollowState( void );

	/**
	*  state의 update/event 처리 함수.
	*/
	virtual void Enter( void );
	virtual void Exit( void );
	virtual void Update( RwReal fElapsed );
	virtual RwUInt32 HandleEvents( RWS::CMsg &pMsg );
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlFSMVehicleAdjustMoveState : public CNtlFSMVehicleActStateBase
{
	DECLEAR_MEMORY_POOL( CNtlFSMVehicleAdjustMoveState, NTL_DEFAULT_MEMORY_POOL )
	DECLEAR_FSMSTATE_FACTROY( CNtlFSMVehicleAdjustMoveState )

public:

	CNtlFSMVehicleAdjustMoveState( void );

	/**
	*  state의 update/event 처리 함수.
	*/
	virtual void Enter( void );
	virtual void Exit( void );
	virtual void Update( RwReal fElapsed );
	virtual RwUInt32 HandleEvents( RWS::CMsg &pMsg );
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlFSMVehicleActFallingState : public CNtlFSMVehicleActStateBase
{
	DECLEAR_MEMORY_POOL( CNtlFSMVehicleActFallingState, NTL_DEFAULT_MEMORY_POOL )
	DECLEAR_FSMSTATE_FACTROY( CNtlFSMVehicleActFallingState )

public:

	CNtlFSMVehicleActFallingState( void );

	/**
	*  state의 update/event 처리 함수.
	*/
	virtual void Enter( void );
	virtual void Exit( void );
	virtual void Update( RwReal fElapsed );
	virtual RwUInt32 HandleEvents( RWS::CMsg &pMsg );
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


#endif
