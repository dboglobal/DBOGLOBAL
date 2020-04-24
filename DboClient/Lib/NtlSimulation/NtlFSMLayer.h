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

#ifndef __NTL_FSMLAYER_H__
#define __NTL_FSMLAYER_H__

#include "ceventhandler.h"
#include "NtlSLDef.h"
#include "NtlDumpTarget.h"
#include "NtlFSMDef.h"

class CNtlFSMBase;
class CNtlFSMAgent;
class CNtlFSMStateBase;
class CNtlSobActor;

class CNtlFSMLayer
{
protected:

	CNtlSobActor *m_pActor;
	CNtlFSMAgent *m_pAgent;			// fsm agent instance pointer
	CNtlFSMBase *m_pFSM;			// fsm instance pointer	
	CNtlFSMStateBase *m_pState;		// fsm state instance pointer

public:

	CNtlFSMLayer();
	virtual ~CNtlFSMLayer();
	
	virtual RwBool  Create(RwUInt32 uiController) { return TRUE; }
	virtual void    Destroy(void);
	virtual void    Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void    Dump(CNtlDumpTarget& dump);

	virtual void NotifyUpdateChangeState(RwUInt32 uiOldStateId, RwUInt32 uiNextStateId);    ///< update notify function(state change)
	virtual void NotifyEvents(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);   ///< event notify function(state change)

    CNtlFSMAgent*       GetFSMAgent(void) const;      ///< agent instance를 얻어오는 함수.
	CNtlFSMBase*        GetFSM(void) const;            ///< instance를 얻어오는 함수.
	CNtlFSMStateBase*   GetCurrentState(void) const;  ///< current state를 얻어오는 함수.
	
	void SetActor(const CNtlSobActor *pActor);          ///< actor 설정 함수.
	CNtlSobActor* GetActor(void) const;
};


inline CNtlFSMAgent* CNtlFSMLayer::GetFSMAgent(void) const
{
	return m_pAgent;
}

inline CNtlFSMBase* CNtlFSMLayer::GetFSM(void) const
{
	return m_pFSM;
}

inline CNtlFSMStateBase* CNtlFSMLayer::GetCurrentState(void) const
{
	return m_pState;
}

inline void CNtlFSMLayer::SetActor(const CNtlSobActor *pActor)
{
	m_pActor = const_cast<CNtlSobActor*>(pActor);
}

inline CNtlSobActor* CNtlFSMLayer::GetActor(void) const
{
	return m_pActor;
}

#endif