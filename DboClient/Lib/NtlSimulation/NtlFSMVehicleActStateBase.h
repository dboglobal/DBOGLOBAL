/*****************************************************************************
 *
 * File			: NtlFSMVehicleActStateBase.h
 * Author		: JeongHo, Rho
 * Copyright	: (аж)NTL
 * Date			: 2006. 2. 13	
 * Abstract		: Finite state machine vehicle action base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_FSM_VEHICLEACTSTATEBASE_H__
#define __NTL_FSM_VEHICLEACTSTATEBASE_H__


#include "NtlFSMCharActStateBase.h"


class CNtlFSMVehicleActStateBase : public CNtlFSMCharActStateBase
{
protected:
	CNtlFSMVehicleActStateBase( void );


public:
	virtual ~CNtlFSMVehicleActStateBase( void ) { return; }

	virtual RwBool		Create( void );
	virtual void		Destroy( void );

	virtual void		Enter( void );
	virtual void		Exit( void );
	virtual RwUInt32	HandleEvents( RWS::CMsg& pMsg );

	virtual void		Clone( CNtlFSMStateBase* pState );

protected:
	virtual RwUInt32	EventProcInputTerrainClick(RWS::CMsg &pMsg);
	virtual RwUInt32	EventProcInputKeyboardMove(RWS::CMsg &pMsg);
	virtual RwUInt32	EventProcInputKeyboardMoveStop( RWS::CMsg& pMsg );
	virtual RwUInt32	EventProcSobMoveStop( RWS::CMsg& pMsg );
};



#endif
