/*****************************************************************************
 *
 * File			: NtlFSMController.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 9	
 * Abstract		: FSM controller base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_FSM_CONTROLLER_H__
#define __NTL_FSM_CONTROLLER_H__

#include "ceventhandler.h"
#include "NtlSLDef.h"
#include "NtlDumpTarget.h"

class CNtlSobActor;
class CNtlFSMAgent;

class CNtlFSMController
{
protected:

	RwUInt32 m_uiControllerId;
	CNtlSobActor *m_pActor;
	CNtlFSMAgent *m_pAgent;

public:
	CNtlFSMController() 
	{
		m_uiControllerId = 0;
		m_pActor = NULL;
		m_pAgent = NULL; 
	}
	virtual ~CNtlFSMController() {}

	virtual void     Update(RwReal fElapsed) = 0;
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg) = 0;
    virtual void     NotifyUpdateChangeState(RwUInt32 uiOldState, RwUInt32 uiNextState) = 0;    ///< update notify function(state change)
    virtual void     NotifyEvents(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2) = 0; ///< event notify function(state change)
	
	virtual void Dump(CNtlDumpTarget& dump)
	{
		dump << "controller id : " << m_uiControllerId << "\n";
		dump.Dump();
	}

    //--- Property 함수들 ---//
	
	void     SetControllerId(RwUInt32 uiStateId);
	RwUInt32 GetControllerId(void) const;
	
    void          SetAgent(const CNtlFSMAgent *pAgent);
	CNtlFSMAgent* GetAgent(void) const;
	
	void          SetActor(const CNtlSobActor *pActor);
	CNtlSobActor* GetActor(void) const;
};

inline void CNtlFSMController::SetControllerId(RwUInt32 uiStateId)
{
	m_uiControllerId = uiStateId;
}

inline RwUInt32 CNtlFSMController::GetControllerId(void) const
{
	return m_uiControllerId;
}


inline void CNtlFSMController::SetAgent(const CNtlFSMAgent *pAgent)
{
	m_pAgent = const_cast<CNtlFSMAgent*>(pAgent);
}

inline CNtlFSMAgent* CNtlFSMController::GetAgent(void) const
{
	return m_pAgent;
}

inline void CNtlFSMController::SetActor(const CNtlSobActor *pActor)
{
	m_pActor = const_cast<CNtlSobActor*>(pActor);
}

inline CNtlSobActor* CNtlFSMController::GetActor(void) const
{
	return m_pActor;
}

#endif