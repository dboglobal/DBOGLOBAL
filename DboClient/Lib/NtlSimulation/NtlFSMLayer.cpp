#include "precomp_ntlsimulation.h"
#include "NtlFSMLayer.h"
#include "NtlDebug.h"
#include "NtlFSMStateBase.h"
#include "NtlFSMAgent.h"
#include "NtlFSMBase.h"
#include "NtlSobActor.h"
#include "NtlPLEvent.h"

CNtlFSMLayer::CNtlFSMLayer()
{
	m_pActor = NULL;
	m_pAgent = NULL;
	m_pFSM = NULL;
	m_pState = NULL;
}

CNtlFSMLayer::~CNtlFSMLayer()
{
	
}

void CNtlFSMLayer::Destroy(void)
{
	if(m_pState)
	{
		DELETE_FSMSTATE_FACTORY(m_pState);
		m_pState = NULL;
	}

	if(m_pAgent)
	{
		m_pAgent->Destroy();

		NTL_DELETE( m_pAgent );
	}
}

void CNtlFSMLayer::Update(RwReal fElapsed)
{
	//
	if(m_pState)
	{
		m_pState->Update(fElapsed);
		if(m_pState->IsFinish())
		{
			// state the next state
			RwUInt32 uiCurrStateId = m_pState->GetStateId();
			RwUInt32 uiNextStateId = m_pState->GetNextStateId();
			if(m_pState->GetNextStateId() == NTL_FSMSID_INVALID)
			{
				uiNextStateId = m_pAgent->AcquireNextState(uiCurrStateId); 
			}
		//	DBO_WARNING_MESSAGE("uiCurrStateId: " << uiCurrStateId << " uiNextStateId: " << uiNextStateId);
			m_pState = m_pFSM->Transition(m_pActor, m_pState, uiNextStateId); 
			NotifyUpdateChangeState(uiCurrStateId, uiNextStateId);
			m_pAgent->EventProc();
		}
	}

	m_pAgent->Update(fElapsed); 
	RwUInt32 uiNextStateId = m_pAgent->GetNextStateId();
	if(uiNextStateId != NTL_FSMSID_INVALID)
	{
		m_pAgent->SetNextStateClear();

		RwUInt32 uiCurrStateId = NTL_FSMSID_INVALID;
		if(m_pState)
			uiCurrStateId = m_pState->GetStateId();
		m_pState = m_pFSM->Transition(m_pActor, m_pState, uiNextStateId); 
		NotifyUpdateChangeState(uiCurrStateId, uiNextStateId);
		m_pAgent->EventProc();
	}
}

RwUInt32 CNtlFSMLayer::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMLayer::HandleEvents");

	RwUInt32 uiRes = NTL_FSM_EVENTRES_PASS;
	RwUInt32 uiNotifyParam1 = 0;
	RwUInt32 uiNotifyParam2 = 0;

	// agent의 event 처리.
	uiRes = m_pAgent->HandleEvents(pMsg);
	if(uiRes == NTL_FSM_EVENTRES_BLOCK)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}
	else if(uiRes == NTL_FSM_EVENTRES_CHANGE_STATE)
	{
		if(m_pState == NULL)
		{
			RwUInt32 uiNextStateId = m_pAgent->GetNextStateId(); 
			m_pAgent->SetNextStateClear();
			m_pState = m_pFSM->CreateState(m_pActor, uiNextStateId); 
			m_pState->HandleEvents(pMsg); 
			m_pState->Enter();
		}
		else 
		{
			// 다음 상태를 state 자신은 모르는 경우. agent한테 물어본다.
			RwUInt32 uiCurrStateId = m_pState->GetStateId();
			RwUInt32 uiNextStateId = m_pAgent->GetNextStateId();

			m_pState = m_pFSM->Transition(m_pActor, m_pState, uiNextStateId); 

			// event notify param setting
			uiNotifyParam1 = uiCurrStateId;
			uiNotifyParam2 = m_pState->GetStateId();

			// event notify
			NotifyEvents(pMsg, uiRes, uiNotifyParam1, uiNotifyParam2);

			m_pAgent->SetNextStateClear();
		}
	}
	else
	{	
		// state의 event 처리.
		if(m_pState)
		{
			uiRes = m_pState->HandleEvents(pMsg); 

			// 다음 상태를 state 자신은 모르는 경우. agent한테 물어본다.
			RwUInt32 uiCurrStateId = m_pState->GetStateId();
			RwUInt32 uiNextStateId = m_pState->GetNextStateId();

			if(uiRes == NTL_FSM_EVENTRES_CHANGE_STATE)
			{
				if(m_pState->GetNextStateId() == NTL_FSMSID_INVALID)
				{
					uiNextStateId = m_pAgent->AcquireNextState(uiCurrStateId); 
				}

				m_pState = m_pFSM->Transition(m_pActor, m_pState, uiNextStateId); 
			}

			// event notify param setting
			uiNotifyParam1 = uiCurrStateId;
			uiNotifyParam2 = uiNextStateId;

			// event notify
			NotifyEvents(pMsg, uiRes, uiNotifyParam1, uiNotifyParam2);
		}
	}

	
	NTL_RETURN(uiRes);
}

void CNtlFSMLayer::Dump(CNtlDumpTarget& dump)
{
	dump << "\n";
	dump.Dump();

	dump << "=== fsm dump(begin) ===" << "\n";
	dump.Dump();

	m_pAgent->Dump(dump);
	if(m_pState)
		m_pState->Dump(dump); 

	dump << "\n";
	dump.Dump();

	dump << "=== fsm dump(end) ===" << "\n";
	dump.Dump();
}

void CNtlFSMLayer::NotifyUpdateChangeState(RwUInt32 uiOldStateId, RwUInt32 uiNextStateId) 
{
	m_pAgent->NotifyUpdateChangeState(uiOldStateId, uiNextStateId);
}

void CNtlFSMLayer::NotifyEvents(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	m_pAgent->NotifyEvents(pMsg, uiEventResult, uiParam1, uiParam2);
}