#include "precomp_ntlsimulation.h"

// core
#include "NtlEventQueue.h"

// simulation
#include "NtlFSMAgent.h"
#include "NtlDebug.h"
#include "NtlFSMController.h"
#include "NtlFSMDef.h"
#include "NtlFSMEventQueue.h"
#include "NtlSLHelpers.h"

CNtlFSMAgent::CNtlFSMAgent()
{
	m_pActor = NULL;
	m_pController = NULL;
	m_pEventQueued =  NTL_NEW CNtlFSMEventQueue;	
	m_uiNextStateId = NTL_FSMSID_INVALID;

	SetNextStateClear();
}

CNtlFSMAgent::~CNtlFSMAgent()
{
	NTL_DELETE( m_pEventQueued );
	NTL_DELETE( m_pController );
}

void CNtlFSMAgent::Destroy(void)
{
	NTL_DELETE( m_pController );
}

void CNtlFSMAgent::Update(RwReal fElapsed)
{
	if(m_pController)
		m_pController->Update(fElapsed);
}

RwUInt32 CNtlFSMAgent::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMAgent::HandleEvents");

	RwUInt32 uiRes = m_pController->HandleEvents(pMsg);
	
	if(uiRes == NTL_FSM_EVENTRES_QUEUING_REFRESH)
	{
		RemoveAllEvents();
	}

	NTL_RETURN(uiRes);
}

void CNtlFSMAgent::Dump(CNtlDumpTarget& dump)
{
	if(m_pController)
		m_pController->Dump(dump);
}

void CNtlFSMAgent::NotifyUpdateChangeState(RwUInt32 uiOldState, RwUInt32 uiNextState)
{
	if (m_pController)
		m_pController->NotifyUpdateChangeState(uiOldState, uiNextState);

	SetCurrStateId(uiNextState);
}

void CNtlFSMAgent::NotifyEvents(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if(uiEventResult == NTL_FSM_EVENTRES_QUEUING)
	{
		AddEvent(pMsg);
	}
	else if(uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE)
	{
		SetCurrStateId(uiParam2);
	}
	else if(uiEventResult == NTL_FSM_EVENTRES_QUEUING_REFRESH)
	{
		RemoveAllEvents();
	}

	if(m_pController)
		m_pController->NotifyEvents(pMsg, uiEventResult, uiParam1, uiParam2);
}

void CNtlFSMAgent::SetController(const CNtlFSMController *pController)
{
	m_pController = const_cast<CNtlFSMController*>(pController);
}

void CNtlFSMAgent::AddEvent(RWS::CMsg &pMsg)
{
	m_pEventQueued->RemoveAll();
	RwInt32 iLen = Helper_GetSLEventDataSize(pMsg); 
	if(iLen < 0)
	{
		NTL_ASSERTFAIL("CNtlFSMAgent::AddEvent : event data length is -1( " << pMsg.Id->p_msgname << " )");
	}

	m_pEventQueued->Push(pMsg, pMsg.pData, iLen); 
}

void CNtlFSMAgent::RemoveAllEvents(void)
{
	m_pEventQueued->RemoveAll();
}

void CNtlFSMAgent::EventProc(void)
{
	CNtlFSMEventData *pEventData = m_pEventQueued->Pop();
	while(pEventData)
	{
		PostMsg(pEventData->msg, pEventData->chData, pEventData->iDataLen);

		NTL_DELETE( pEventData );

		pEventData = m_pEventQueued->Pop();
	};
}

RwBool CNtlFSMAgent::IsQueuingEventExist(void)
{
	return m_pEventQueued->IsEventExist();
}