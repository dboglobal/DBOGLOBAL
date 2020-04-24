#include "precomp_ntlsimulation.h"
#include "NtlBehaviorBase.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLEntity.h"
#include "NtlInstanceEffect.h"
#include "NtlPLSceneManager.h"

// simulation
#include "NtlSobActor.h"
#include "NtlSLLuaGlue.h"
#include "NtlFSMDef.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//

CNtlBehaviorBase::CNtlBehaviorBase()
{
	m_uiBehaviorId = 0;
	m_strName[0] = 0;
	m_bFinish = FALSE;
	m_pActor = NULL;
}

CNtlBehaviorBase::~CNtlBehaviorBase()
{
	RemoveAllPLEntity();
}

CNtlBehaviorBase& CNtlBehaviorBase::operator = (const CNtlBehaviorBase& Behavior)
{
	m_uiBehaviorId = Behavior.GetBehaviorId();
	m_bFinish = Behavior.IsFinish();
	m_pActor = Behavior.GetActor();

	return *this;
}

void CNtlBehaviorBase::RemoveAllPLEntity(void)
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

void CNtlBehaviorBase::Clone(CNtlBehaviorBase *pBehavior)
{
	*pBehavior = *this;
}

void CNtlBehaviorBase::Enter(void)
{
	RwUInt32 uiSerialId = m_pActor->GetSerialID();	

	LuaExec_CharBehaviorEnter(uiSerialId, this);
}

void CNtlBehaviorBase::Exit(void)
{
	RemoveAllPLEntity();

	RwUInt32 uiSerialId = m_pActor->GetSerialID();
	LuaExec_CharBehaviorExit(uiSerialId, this);
}

RwUInt32 CNtlBehaviorBase::HandleEvents(RWS::CMsg &pMsg)
{
	return NTL_FSM_EVENTRES_PASS;
}

void CNtlBehaviorBase::Dump(CNtlDumpTarget& dump)
{
	dump << "behavior name : " << m_strName << "\n";
	dump.Dump();
}

void CNtlBehaviorBase::AddPLEntity(CNtlPLEntity *pPLEntity)
{
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

///////////////////////////////////////////////////////////////////////////////////////////////////
//

DEFINITION_MEMORY_POOL(CNtlBehaviorFrame)

CNtlBehaviorFrame::CNtlBehaviorFrame()
{
}

CNtlBehaviorFrame::~CNtlBehaviorFrame()
{
	std::list<CNtlBehaviorBase*>::iterator it;
	for(it = m_listChild.begin(); it != m_listChild.end(); ++it)
	{
		NTL_DELETE( *it );
	}

	m_listChild.clear();
}

void CNtlBehaviorFrame::Enter(void)
{
	std::list<CNtlBehaviorBase*>::iterator it = m_listChild.begin();
	if(it != m_listChild.end())
	{
		CNtlBehaviorBase *pBehavior = (*it);
		pBehavior->Enter(); 
	}

	CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorFrame::Update(RwReal fElapsed) 
{
	if(m_listChild.size() <= 0)
	{
		m_bFinish = TRUE;
		return;
	}

	std::list<CNtlBehaviorBase*>::iterator it = m_listChild.begin();
	CNtlBehaviorBase *pBehavior = (*it);
	pBehavior->Update(fElapsed);
	if(pBehavior->IsFinish())
	{
		pBehavior->Exit(); 

		NTL_DELETE( pBehavior );

		m_listChild.erase(it);

		if(m_listChild.size() <= 0)
			m_bFinish = TRUE;
		else
		{
			it = m_listChild.begin();
			pBehavior = (*it);
			pBehavior->Enter(); 
		}
	}

	CNtlBehaviorBase::Update(fElapsed);
}

void CNtlBehaviorFrame::Exit(void)
{
	CNtlBehaviorBase::Exit();
}

RwUInt32 CNtlBehaviorFrame::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlBehaviorFrame::HandleEvents");

	if(m_listChild.size() <= 0)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_PASS);
	}
		
	CNtlBehaviorBase *pBehavior;
	std::list<CNtlBehaviorBase*>::iterator it = m_listChild.begin();
	pBehavior = (*it);

	NTL_RETURN(pBehavior->HandleEvents(pMsg));
}

void CNtlBehaviorFrame::Dump(CNtlDumpTarget& dump)
{
	if(m_listChild.size() <= 0)
		return;

	std::list<CNtlBehaviorBase*>::iterator it = m_listChild.begin();
	CNtlBehaviorBase *pBehavior = (*it);
	pBehavior->Dump(dump);
}

void CNtlBehaviorFrame::SetActor(const CNtlSobActor *pActor)
{
	CNtlBehaviorBase *pBehavior;
	std::list<CNtlBehaviorBase*>::iterator it;
	for(it = m_listChild.begin(); it != m_listChild.end(); ++it)
	{
		pBehavior = (*it);
		pBehavior->SetActor(pActor);
	}

	CNtlBehaviorBase::SetActor(pActor); 
}

void CNtlBehaviorFrame::AddChild(CNtlBehaviorBase *pBehavior)
{
	m_listChild.push_back(pBehavior);
}