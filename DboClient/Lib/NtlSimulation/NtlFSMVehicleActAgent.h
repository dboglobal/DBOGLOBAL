/*****************************************************************************
 *
 * File			: NtlFSMVehicleActAgent.h
 * Author		: JeongHo, Rho
 * Copyright	: (аж)NTL
 * Date			: 2008. 11. 12
 * Abstract		: vehicle act agent class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_FSM_VEHICLE_AGENT_H__
#define __NTL_FSM_VEHICLE_AGENT_H__


#include "NtlSLEvent.h"
#include "NtlFSMCharActAgent.h"


class CNtlFSMVehicleActAgent : public CNtlFSMCharActAgent
{
	DECLEAR_MEMORY_POOL( CNtlFSMVehicleActAgent, NTL_DEFAULT_MEMORY_POOL )


public:

	CNtlFSMVehicleActAgent( void );
	~CNtlFSMVehicleActAgent( void );

	virtual void			Update( RwReal fElapsed );
	virtual RwUInt32		HandleEvents( RWS::CMsg& pMsg );	
	virtual RwUInt32		AcquireNextState( RwUInt32 uiCurrStateId );

	virtual void			NotifyUpdateChangeState( RwUInt32 uiOldState, RwUInt32 uiNextState );
	virtual void			NotifyEvents( RWS::CMsg& pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2 );


protected:

	virtual void			ServerStateCreateAnalysis( sCHARSTATE* pCharState );
	virtual void			ServerStateUpdateAnalysis( SNtlEventSobServerUpdateState* pServerState );
	virtual void			ServerConditionUpdateAnalysis( RwUInt8 byServerState, RwUInt64 qwCondition );
	virtual void			ServerAspectUpdateAnalysis( sASPECTSTATE& sAspectState, RwBool bCreateUpdate );
};


#endif
