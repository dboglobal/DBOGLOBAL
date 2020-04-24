/*****************************************************************************
 *
 * File			: NtlFSMBase.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 9	
 * Abstract		: Finite state machine class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_FSMBASE_H__
#define __NTL_FSMBASE_H__

#include "NtlSLDef.h"

class CNtlFSMStateBase;
class CNtlSobActor;

/**
 * \ingroup NtlSimulation
 * DBO에서 FSM을 구동시키는 class이다.
 * state의 container 기능과 transition 기능을 한다.
 * 
 */
class CNtlFSMBase
{
protected:

    typedef std::map<RwUInt32, CNtlFSMStateBase*> MapState;     ///< Key : State ID, Val : State Pointer
	MapState m_mapState;									    ///< State Container

	CNtlFSMStateBase* Find(RwUInt32 uiStateId);
public:

	virtual ~CNtlFSMBase() {}	
	virtual RwBool Create(void);    ///< 이 클래스가 생성된 후 바로 호출하는 함수	
	virtual void Destroy(void);     ///< 이 클래스가 소멸되기 전에 호출되는 함수

	RwBool            Register(CNtlFSMStateBase *pState);                                                   ///< State Container에 State를 Add한다.	
	CNtlFSMStateBase* CreateState(CNtlSobActor *pActor, RwUInt32 uiNewStateId);                             ///< 최초의 State를 생성하는 함수 	
	virtual CNtlFSMStateBase* Transition(CNtlSobActor *pActor, CNtlFSMStateBase *pOldState, RwUInt32 uiNewStateId); ///< State를 변환한다
};

#endif