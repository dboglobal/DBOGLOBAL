/*****************************************************************************
 *
 * File			: NtlFSMVehicleAct.h
 * Author		: JeongHo, Rho
 * Copyright	: (аж)NTL
 * Date			: 2008. 11. 12	
 * Abstract		: vehicle fsm class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_FSM_VEHICLE_H__
#define __NTL_FSM_VEHICLE_H__


#include "NtlFSMBase.h"
#include "NtlSLDef.h"


class CNtlFSMVehicleAct : public CNtlFSMBase
{
	DECLEAR_MEMORY_POOL( CNtlFSMVehicleAct, NTL_DEFAULT_MEMORY_POOL )

public:

	CNtlFSMVehicleAct( void );
	virtual ~CNtlFSMVehicleAct( void );
	
	virtual RwBool Create( void );
	virtual void Destroy( void );

    virtual CNtlFSMStateBase* Transition( CNtlSobActor *pActor, CNtlFSMStateBase *pOldState, RwUInt32 uiNewStateId );
};

#endif