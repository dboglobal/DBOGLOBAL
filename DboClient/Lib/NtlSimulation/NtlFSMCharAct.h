/*****************************************************************************
 *
 * File			: NtlFSMCharAct.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 9	
 * Abstract		: character fsm class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_FSM_CHARACT_H__
#define __NTL_FSM_CHARACT_H__

#include "NtlFSMBase.h"
#include "NtlSLDef.h"

class CNtlFSMCharAct : public CNtlFSMBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharAct, NTL_DEFAULT_MEMORY_POOL)

public:

	CNtlFSMCharAct();
	virtual ~CNtlFSMCharAct();
	
	virtual RwBool Create(void);	
	virtual void Destroy(void);

    virtual CNtlFSMStateBase* Transition(CNtlSobActor *pActor, CNtlFSMStateBase *pOldState, RwUInt32 uiNewStateId); ///< State를 변환한다
};

#endif