#include "precomp_ntlsimulation.h"
#include "NtlFSMBase.h"
#include "NtlDebug.h"
#include "NtlFSMStateBase.h"
#include "NtlSobActor.h"

RwBool CNtlFSMBase::Create(void)
{
	return TRUE;
}

void CNtlFSMBase::Destroy(void)
{
	NTL_FUNCTION("CNtlFSM::Destroy" );

	CNtlFSMStateBase *pState;
	MapState::iterator it;

	for(it = m_mapState.begin(); it != m_mapState.end(); ++it)
	{
		pState = (*it).second;
		pState->Destroy();

		NTL_DELETE( pState );
	}

	m_mapState.clear();

	NTL_RETURNVOID();
}

CNtlFSMStateBase* CNtlFSMBase::Find(RwUInt32 uiNewStateId)
{
	MapState::iterator it;

	it = m_mapState.find(uiNewStateId); 
	if(it == m_mapState.end())
		return NULL;
	return (*it).second; 
}


RwBool CNtlFSMBase::Register(CNtlFSMStateBase *pState)
{
	if(Find(pState->GetStateId()))
	{
		NTL_ASSERTFAIL("the state name exists always to FSM");
		return FALSE;
	}

	m_mapState[pState->GetStateId()] = pState;

	return TRUE;
}

CNtlFSMStateBase* CNtlFSMBase::CreateState(CNtlSobActor *pActor, RwUInt32 uiNewStateId)
{
	NTL_FUNCTION("CNtlFSMBase::CreateState" );

	CNtlFSMStateBase *pNewState = Find(uiNewStateId);
	NTL_ASSERTE(pNewState);

	pNewState = CREATE_FSMSTATE_FACTORY(pNewState);
	pNewState->SetActor(pActor);

	pActor->NotifyStateEnter(pNewState->GetStateId());
	
	NTL_RETURN(pNewState);
}

CNtlFSMStateBase* CNtlFSMBase::Transition(CNtlSobActor *pActor, CNtlFSMStateBase *pOldState, RwUInt32 uiNewStateId)
{
	NTL_FUNCTION("CNtlFSMBase::Transition" );

	NTL_PRE(pOldState);
	pOldState->SetNextStateId(uiNewStateId);
	pOldState->Exit();
	pActor->NotifyStateExit(pOldState->GetStateId(),uiNewStateId);
	DELETE_FSMSTATE_FACTORY(pOldState);

	CNtlFSMStateBase *pNewState = Find(uiNewStateId);
  	//NTL_ASSERTE(pNewState);
    
    if(!pNewState || uiNewStateId == NTL_FSMSID_INVALID)    // Go to IDLE if you do not have state
    {
        pNewState = Find(NTL_FSMSID_IDLE);
    }

	pNewState = CREATE_FSMSTATE_FACTORY(pNewState);
	pNewState->SetActor(pActor);
	pNewState->Enter();
	pActor->NotifyStateEnter(pNewState->GetStateId());
	
	NTL_RETURN(pNewState);
}
