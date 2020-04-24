/*****************************************************************************
 *
 * File			: NtlFSMStateBase.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 9	
 * Abstract		: Finite state machine base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_FSM_STATEBASE_H__
#define __NTL_FSM_STATEBASE_H__

#include "ceventhandler.h"
#include "NtlSLDef.h"
#include "NtlFSMDef.h"
#include "NtlDumpTarget.h"
#include "NtlDebug.h"

class CNtlSobActor;
class CNtlFSMAgent;
class CNtlBehaviorBase;
class CNtlPLEntity;

// 아래의 new class_name 과 delete pState를 NTL_NEW 와 NTL_DELETE로 바꾸지 말아 주세요
// by Jang HyungSuk.

#define DECLEAR_FSMSTATE_FACTROY(class_name) \
public: \
	CNtlFSMStateBase* CreateStateFactory(void) \
	{ \
		return new class_name; \
	} \
	void DestroyStateFactory(CNtlFSMStateBase *pState) \
	{ \
		delete pState; \
	}

///////////////////////////////////////////////
	
#define CREATE_FSMSTATE_FACTORY(state_inst) \
	state_inst->CreateStateFactory(); 
	
#define DELETE_FSMSTATE_FACTORY(state_inst) \
	state_inst->DestroyStateFactory(state_inst);

///////////////////////////////////////////////

#define MAX_FSMS_NAME	32

///< FSM State 클래스들의 부모 추상 클래스
class CNtlFSMStateBase
{
protected:
	static RwBool		m_bTerminate;

	RwChar				m_strName[MAX_FSMS_NAME];
	RwUInt32			m_uiStateId;
	RwUInt32			m_uiNextStateId;
	RwUInt32			m_uiFlags;
	RwBool				m_bFinish;
	std::list<CNtlBehaviorBase*> m_listBehavior;
	std::list<CNtlPLEntity*> m_listPLEntity;
	
	CNtlSobActor		*m_pActor;
	CNtlFSMStateBase	*m_pStacked;

	CNtlFSMStateBase(); 

protected:
	void RemoveAllPLEntity(void);
		
public:

	CNtlFSMStateBase& operator = (const CNtlFSMStateBase& State);
	virtual ~CNtlFSMStateBase();

	/**
    *  state의 생성/소멸의 factory 함수.
    */
	virtual CNtlFSMStateBase* CreateStateFactory(void) = 0;
	virtual	void DestroyStateFactory(CNtlFSMStateBase *pState) = 0;

	/**
    *  state의 생성/소멸시 호출되는 함수.
    */
	virtual RwBool Create(void) { return TRUE; }
	virtual void Destroy(void);

	/**
    *  state의 update/event 처리 함수.
    */
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	
	virtual void Dump(CNtlDumpTarget& dump);        ///< Data Dump
	virtual void Clone(CNtlFSMStateBase *pState);   ///< State Clone

    //--- Property Methods-----------------------------//
	void            SetStateName(const RwChar *pName);         
	const RwChar*   GetStateName(void) const;         

	void            SetStateId(RwUInt32 uiStateId);
	RwUInt32        GetStateId(void) const;
	
	void            SetNextStateId(RwUInt32 uiNextStateId);
	RwUInt32        GetNextStateId(void) const;
	
	void            SetFlags(RwUInt32 uiFlags);
	RwUInt32        GetFlags(void) const;

	void            SetActor(const CNtlSobActor *pActor);
	CNtlSobActor*   GetActor(void) const;

	/**
    *  현재의 state가 끝나고, 예전 state로 복귀할 때 사용하는 함수.(move => jump가 들어오면 move 상태를 queuing 시킨다.)
    */
	void                SetQueued(const CNtlFSMStateBase *pQueuedState);
	CNtlFSMStateBase*   GetQueued(void) const;
	void                ReleaseQueued(void);

    /**
    *  현재의 state가 끝났는지를 검사하는 함수.
    */
    void    ResetFinish(void);
    void    Finish(void);
    RwBool  IsFinish(void) const;

	/**
    *  현재의 state에서 avtive된 behavior를 설정하는 함수.
    */
	void                AddBehavior(CNtlBehaviorBase *pBehavior);
	RwInt32             GetBehaviorCount(void) const;
	CNtlBehaviorBase*   GetBehavior(RwInt32 iIdx);
	void                RemoveAllBehavior(void);
	void                UpdateBehavior(void);

	void                AddPLEntity(CNtlPLEntity *pPLEntity);   ///< Visual 추가한다.	
	CNtlFSMAgent*       GetFSMAgent(void);                      ///< 현재 Agent를 얻어온다.
	virtual RwBool      IsActorActiveState(void);               ///< 객체가 Active 가능한 상태인가?

	static void Terminate(RwBool bTerminate);
};

inline void CNtlFSMStateBase::SetStateName(const RwChar *pName)
{
	strcpy_s(m_strName, pName);
}

inline const RwChar* CNtlFSMStateBase::GetStateName(void) const
{
	return m_strName;
}


inline void CNtlFSMStateBase::SetStateId(RwUInt32 uiStateId)
{
	m_uiStateId = uiStateId;
}

inline RwUInt32 CNtlFSMStateBase::GetStateId(void) const
{
	return m_uiStateId;
}

inline void CNtlFSMStateBase::SetNextStateId(RwUInt32 uiNextStateId)
{
	m_uiNextStateId = uiNextStateId;
}

inline RwUInt32 CNtlFSMStateBase::GetNextStateId(void) const
{
	return m_uiNextStateId;
}

inline void CNtlFSMStateBase::SetFlags(RwUInt32 uiFlags)
{
	m_uiFlags = uiFlags;
}

inline RwUInt32 CNtlFSMStateBase::GetFlags(void) const 
{
	return m_uiFlags;
}

inline void CNtlFSMStateBase::SetActor(const CNtlSobActor *pActor)
{
	m_pActor = const_cast<CNtlSobActor*>(pActor);
}

inline CNtlSobActor* CNtlFSMStateBase::GetActor(void) const
{
	return m_pActor;
}

inline void CNtlFSMStateBase::SetQueued(const CNtlFSMStateBase *pStackedState)
{
	m_pStacked = const_cast<CNtlFSMStateBase*>(pStackedState);
}

inline CNtlFSMStateBase* CNtlFSMStateBase::GetQueued(void) const
{
	return m_pStacked;
}

inline RwInt32 CNtlFSMStateBase::GetBehaviorCount(void) const
{
	return (RwInt32)m_listBehavior.size();
}

inline CNtlBehaviorBase* CNtlFSMStateBase::GetBehavior(RwInt32 iIdx)
{
	if(iIdx >= (RwInt32)m_listBehavior.size())
		return NULL;

	RwInt32 iNum = 0;
	std::list<CNtlBehaviorBase*>::iterator it;
	for(it = m_listBehavior.begin(); it != m_listBehavior.end(); ++it)
	{
		if(iIdx == iNum)
			return (*it);
		iNum++;
	}

	return NULL;
}

inline void CNtlFSMStateBase::ResetFinish(void)
{
	m_bFinish = FALSE;
}

inline void CNtlFSMStateBase::Finish(void)
{
	m_bFinish = TRUE;
}

inline RwBool CNtlFSMStateBase::IsFinish(void) const
{
	return m_bFinish;
}


#endif