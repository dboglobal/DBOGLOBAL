#include "precomp_ntlsimulation.h"
#include "NtlFSMStateBase.h"
#include "NtlDebug.h"

// presentation
#include "NtlPLEntity.h"
#include "NtlInstanceEffect.h"
#include "NtlPLSceneManager.h"

// simulation
#include "NtlSobActor.h"
#include "NtlFSMLayer.h"
#include "NtlBehaviorBase.h"
#include "NtlSLLuaGlue.h"

RwBool CNtlFSMStateBase::m_bTerminate = FALSE;

CNtlFSMStateBase::CNtlFSMStateBase() 
{
	memset(m_strName, 0, MAX_FSMS_NAME);
	m_uiStateId = NTL_FSMSID_INVALID; 
	m_uiNextStateId = NTL_FSMSID_INVALID; 
	m_uiFlags = 0;
	m_bFinish = FALSE; 
	m_pActor = NULL;
	m_pStacked = NULL;
}

CNtlFSMStateBase& CNtlFSMStateBase::operator = (const CNtlFSMStateBase& State)
{
	Destroy();

	SetStateName(State.GetStateName());
	SetStateId(State.GetStateId());
	SetNextStateId(State.GetNextStateId());
	SetFlags(State.GetNextStateId());
	m_bFinish = State.IsFinish();

	RwInt32 iBehaviorNum = State.GetBehaviorCount();
	for(RwInt32 i = 0; i < iBehaviorNum; ++i)
	{
		CNtlBehaviorBase *pBehavior = GetBehavior(i);
		CNtlBehaviorBase *pNewBehavior = CREATE_BEHAVIOR_FACTORY(pBehavior);
		AddBehavior(pNewBehavior);
	}
	
	SetActor(State.GetActor());

	CNtlFSMStateBase *pStackedState = State.GetQueued();
	if(pStackedState)
	{
		m_pStacked = CREATE_FSMSTATE_FACTORY(pStackedState);
		m_pStacked->Clone(pStackedState);
	}
	else
	{
		m_pStacked = NULL;
	}

	return *this;
}

CNtlFSMStateBase::~CNtlFSMStateBase() 
{
	Destroy();
}

void CNtlFSMStateBase::Destroy(void)
{
	RemoveAllPLEntity();
	RemoveAllBehavior();
	ReleaseQueued();
}

void CNtlFSMStateBase::Enter(void)
{
	LuaExec_CharStateEnter(m_pActor->GetSerialID(), this);
}

void CNtlFSMStateBase::Exit(void)
{
	RemoveAllPLEntity();

	LuaExec_CharStateExit(m_pActor->GetSerialID(), this);
}

void CNtlFSMStateBase::Update(RwReal fElapsed)
{
	if(m_listBehavior.size() <= 0)
	{
		m_bFinish = TRUE;
		return;
	}

	CNtlBehaviorBase *pBehavior;
	std::list<CNtlBehaviorBase*>::iterator it = m_listBehavior.begin();
	for(it = m_listBehavior.begin(); it != m_listBehavior.end(); ) 
	{
		pBehavior = (*it);
		pBehavior->Update(fElapsed);
		if(pBehavior->IsFinish())
		{
			pBehavior->Exit(); 

			NTL_DELETE( pBehavior );

			it = m_listBehavior.erase(it);

			if(m_listBehavior.size() <= 0)
				m_bFinish = TRUE;
		}
		else
		{
			++it;
		}
	}
}

RwUInt32 CNtlFSMStateBase::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMStateBase::HandleEvents");

	CNtlBehaviorBase *pBehavior;
	std::list<CNtlBehaviorBase*>::iterator it;
	for(it = m_listBehavior.begin(); it != m_listBehavior.end(); ++it) 
	{
		pBehavior = (*it);
		pBehavior->HandleEvents(pMsg);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlFSMStateBase::Dump(CNtlDumpTarget& dump)
{
	dump << "state name : " << m_strName << "\n";
	dump.Dump();

	std::list<CNtlBehaviorBase*>::iterator it;
	for(it = m_listBehavior.begin(); it != m_listBehavior.end(); ++it)
	{
		(*it)->Dump(dump);
	}
}

void CNtlFSMStateBase::Clone(CNtlFSMStateBase *pState)
{
	*pState = *this;
}

void CNtlFSMStateBase::AddBehavior(CNtlBehaviorBase *pBehavior)
{
	pBehavior->SetActor(m_pActor); 
	pBehavior->Enter(); 
	m_listBehavior.push_back(const_cast<CNtlBehaviorBase*>(pBehavior));
}

void CNtlFSMStateBase::RemoveAllBehavior(void)
{
	std::list<CNtlBehaviorBase*>::iterator it;
	for(it = m_listBehavior.begin(); it != m_listBehavior.end(); ++it)
	{
		CNtlBehaviorBase *pBehavior = (*it);
		if(!m_bTerminate)
			pBehavior->Exit();

		NTL_DELETE( pBehavior );
	}

	m_listBehavior.clear();
}

void CNtlFSMStateBase::UpdateBehavior(void)
{
	NTL_FUNCTION("CNtlFSMStateBase::UpdateBehavior");

	std::list<CNtlBehaviorBase*>::iterator it;
	for(it = m_listBehavior.begin(); it != m_listBehavior.end(); ++it)
	{
		CNtlBehaviorBase *pBehavior = (*it);
		pBehavior->UpdateData();
	}

	NTL_RETURNVOID();
}

void CNtlFSMStateBase::ReleaseQueued(void)
{
	if(m_pStacked)
	{
		m_pStacked->Destroy();
		DELETE_FSMSTATE_FACTORY(m_pStacked);
		m_pStacked = NULL;
	}
}

void CNtlFSMStateBase::AddPLEntity(CNtlPLEntity *pPLEntity)
{
	if(pPLEntity == NULL)
		return;

	if(pPLEntity->GetClassType() == PLENTITY_EFFECT)
	{
		CNtlInstanceEffect *pPLEffect = reinterpret_cast<CNtlInstanceEffect*>(pPLEntity);
		if(!pPLEffect->IsAutoDelete())
			m_listPLEntity.push_back(pPLEntity);
	}
	else
	{
		m_listPLEntity.push_back(pPLEntity);
	}
}

void CNtlFSMStateBase::RemoveAllPLEntity(void)
{
	CNtlPLEntity *pPLEntity;
	std::list<CNtlPLEntity*>::iterator it;
	for(it = m_listPLEntity.begin(); it != m_listPLEntity.end(); it++)
	{
		pPLEntity = (*it);
		GetSceneManager()->DeleteEntity(pPLEntity);
	}
	m_listPLEntity.clear(); 
}


CNtlFSMAgent* CNtlFSMStateBase::GetFSMAgent(void)
{
	CNtlFSMLayer *pFSMLayer = m_pActor->GetFSMLayer();
	return pFSMLayer->GetFSMAgent();
}

RwBool CNtlFSMStateBase::IsActorActiveState(void)
{
	return TRUE;
}


void CNtlFSMStateBase::Terminate(RwBool bTerminate)
{
	m_bTerminate = bTerminate;
}