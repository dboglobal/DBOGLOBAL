#include "precomp_ntlsimulation.h"
#include "NtlSobActor.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLEvent.h"

// simulation
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlFSMDef.h"
#include "NtlFSMLayer.h"
#include "NtlFSMAgent.h"
#include "NtlFSMCharActAgent.h"
#include "NtlFSMUtil.h"
#include "NtlFSMStateBase.h"
#include "NtlSobInfluence.h"
#include "NtlSobProxy.h"


CNtlSobActor::CNtlSobActor()
{
	m_uiActFlags				= 0;
	m_pFSMLayer					= NULL;
	m_pSobInfluence				= NULL;
	m_byServerSyncAvatarType	= 0;

	m_hFinialHurtSerialId		= INVALID_SERIAL_ID;
	m_fHurtOverTime				= 0.0f;
}

CNtlSobActor::~CNtlSobActor()
{
}

sCHARSTATE* CNtlSobActor::GetServerFullState(void) const
{
	if ( m_pFSMLayer )
	{
		CNtlFSMCharActAgent* pAgent = dynamic_cast< CNtlFSMCharActAgent* > ( m_pFSMLayer->GetFSMAgent() );

		if ( pAgent )
		{
			return (sCHARSTATE*)pAgent->GetServerFullState();
		}
	}

	return NULL;
}

void CNtlSobActor::ServerChangeOut(void)
{
	if(m_pSobInfluence)
	{
		m_pSobInfluence->SetSobParent(NULL);
		m_pSobInfluence->Destroy();

		NTL_DELETE( m_pSobInfluence );
	}

	CNtlSob::ServerChangeOut();
}

void CNtlSobActor::ServerChangeIn(void)
{
	m_pSobInfluence = CreateSobInfluence();
	if(m_pSobInfluence)
		m_pSobInfluence->SetSobParent(this);

	CNtlSob::ServerChangeIn();
}

RwBool CNtlSobActor::Create(void)
{
	NTL_FUNCTION("CNtlSobActor::Create");

	m_pFSMLayer = CreateFSM();
	m_pSobInfluence = CreateSobInfluence();
	if(m_pSobInfluence)
		m_pSobInfluence->SetSobParent(this);

	if(!CNtlSob::Create())
	{
		NTL_RETURN(FALSE);
	}

	// event handler function link
	m_mapEventFunc[g_EventSobTargetSelect.Get_pEventId()]			= &CNtlSobActor::SobTargetSelectEventHandler;
	m_mapEventFunc[g_EventSobAttackSelect.Get_pEventId()]			= &CNtlSobActor::SobAttackSelectEventHandler;
	m_mapEventFunc[g_EventSobTargetSelectRelease.Get_pEventId()]	= &CNtlSobActor::SobTargetSelectReleaseEventHandler;	
    
    // Proxy에서 처리하는 이벤트를 
    m_mapEventFunc[g_EventSobAttackMarkRelease.Get_pEventId()]		= &CNtlSobActor::SobProxyEventHandler;	
    m_mapEventFunc[g_EventSobGotFocus.Get_pEventId()]				= &CNtlSobActor::SobProxyEventHandler;
    m_mapEventFunc[g_EventSobLostFocus.Get_pEventId()]				= &CNtlSobActor::SobProxyEventHandler;    
    m_mapEventFunc[g_EventShareTargetSelect.Get_pEventId()]         = &CNtlSobActor::SobProxyEventHandler;
    m_mapEventFunc[g_EventShareTargetRelease.Get_pEventId()]        = &CNtlSobActor::SobProxyEventHandler;
    m_mapEventFunc[g_EventTenkaichiMarking.Get_pEventId()]          = &CNtlSobActor::SobProxyEventHandler;

	m_mapEventFunc[g_EventSobEventTitleEffect.Get_pEventId()] = &CNtlSobActor::SobProxyEventHandler;

    m_mapEventFunc[g_EventSobTargetMarkSelect.Get_pEventId()]       = &CNtlSobActor::SobProxyEventHandler;
    m_mapEventFunc[g_EventSobTargetMarkRelease.Get_pEventId()]      = &CNtlSobActor::SobProxyEventHandler;
    m_mapEventFunc[g_EventTransform.Get_pEventId()]                 = &CNtlSobActor::SobProxyEventHandler;    
    m_mapEventFunc[g_EventMobTransform.Get_pEventId()]              = &CNtlSobActor::SobProxyEventHandler;
    m_mapEventFunc[g_EventTransformCandy.Get_pEventId()]            = &CNtlSobActor::SobProxyEventHandler;
    m_mapEventFunc[g_EventSobSkillAction.Get_pEventId()]            = &CNtlSobActor::SobProxyEventHandler;
    m_mapEventFunc[g_EventSobSkillActioned.Get_pEventId()]            = &CNtlSobActor::SobProxyEventHandler;
    m_mapEventFunc[g_EventAnimHit.Get_pEventId()]                   = &CNtlSobActor::SobProxyEventHandler;
    
	m_mapEventFunc[g_EventCinematicScale.Get_pEventId()] = &CNtlSobActor::SobProxyEventHandler;

	NTL_RETURN(TRUE);
}

void CNtlSobActor::Destroy(void)
{
	if(m_pSobInfluence)
	{
		m_pSobInfluence->SetSobParent(NULL);
		m_pSobInfluence->Destroy();

		NTL_DELETE( m_pSobInfluence );
	}

	if(m_pFSMLayer)
	{
		m_pFSMLayer->Destroy(); 

		NTL_DELETE( m_pFSMLayer );
	}

	CNtlSob::Destroy();
}

void CNtlSobActor::Update(RwReal fElapsed)
{
	if(m_pFSMLayer)
	{
		m_pFSMLayer->Update(fElapsed); 
	}

	if(m_pSobInfluence)
		m_pSobInfluence->Update(fElapsed);

	if(m_hFinialHurtSerialId != INVALID_SERIAL_ID)
	{
		m_fHurtOverTime += fElapsed;
		if(m_fHurtOverTime >= 5.0f)
			m_hFinialHurtSerialId = INVALID_SERIAL_ID;
	}

	CNtlSob::Update(fElapsed);
}

void CNtlSobActor::HandleEvents(RWS::CMsg &pMsg)
{
	MapFuncEventHandler::iterator it = m_mapEventFunc.find( pMsg.Id );
	if ( it != m_mapEventFunc.end() )
	{
		(this->*(*it).second)( pMsg );
	}

	CNtlSob* pVehicleObj = GetNtlSobManager()->GetSobObject( GetVehicleID() );

	if ( NULL == pVehicleObj || !GetEventHook() )
	{
		if ( pMsg.Id != g_EventShareTargetSelect &&
			 pMsg.Id != g_EventShareTargetRelease )
		{
			if ( m_pFSMLayer )
			{
				m_pFSMLayer->HandleEvents( pMsg );
			}
		}
	}
	else
	{
		pVehicleObj->HandleEvents( pMsg );
	}
}

void CNtlSobActor::Dump(CNtlDumpTarget& dump)
{
	CNtlSob::Dump(dump);
}

void CNtlSobActor::AddSob(CNtlSob *pSobObj)
{
	CNtlSob::AddSob(pSobObj);

	if(m_pSobInfluence)
		m_pSobInfluence->AddSob(pSobObj);
}

void CNtlSobActor::RemoveSob(CNtlSob *pSobObj)
{
	CNtlSob::RemoveSob(pSobObj);

	if(m_pSobInfluence)
		m_pSobInfluence->RemoveSob(pSobObj);
}

RwBool CNtlSobActor::CanMove(void)
{
	if(m_pFSMLayer)
	{
		CNtlFSMStateBase *pStateBase = m_pFSMLayer->GetCurrentState();
		return FSMUtil_CanMoveState(pStateBase);
	}
	else 
		return FALSE;
}

RwBool CNtlSobActor::CanAttack(void)
{
	if(m_pFSMLayer)
	{
		CNtlFSMStateBase *pStateBase = m_pFSMLayer->GetCurrentState();
		return FSMUtil_CanAttackState(pStateBase);
	}
	else 
		return FALSE;
}

RwBool CNtlSobActor::CanHurted(void)
{
	if(m_pFSMLayer)
	{
		if(m_pFSMLayer->GetFSMAgent()->IsAttackDisallow())
			return FALSE;

		CNtlFSMStateBase *pStateBase = m_pFSMLayer->GetCurrentState();
		return FSMUtil_CanHurtState(pStateBase);
	}

	return FALSE;
}

RwBool CNtlSobActor::CanClick(void)
{
	if(m_pFSMLayer)
	{
		if(m_pFSMLayer->GetFSMAgent()->IsClickDisable())
			return FALSE;
	}

    if(!Logic_CanTargetted(this))
		return FALSE;

	return TRUE;
}

RwBool CNtlSobActor::IsAirMode(void)
{
	if (m_pFSMLayer)
		return m_pFSMLayer->GetFSMAgent()->IsAirMode();

	return FALSE;
}

RwBool CNtlSobActor::IsFightingMode(void)
{
	if(m_pFSMLayer)
		return m_pFSMLayer->GetFSMAgent()->IsFightingMode();

	return FALSE;
}

RwBool CNtlSobActor::IsClickDisable(void)
{
	if(m_pFSMLayer)
		return m_pFSMLayer->GetFSMAgent()->IsClickDisable();

	return FALSE;
}

RwBool CNtlSobActor::IsClientUIDisable(void)
{
	if(m_pFSMLayer)
		return m_pFSMLayer->GetFSMAgent()->IsClientUIDisable();

	return FALSE;
}

RwBool CNtlSobActor::IsDie(void)
{
	if(m_pFSMLayer)
		return m_pFSMLayer->GetFSMAgent()->IsDie();

	return FALSE;
}


void CNtlSobActor::NotifyStateEnter(RwUInt32 uiStateId)
{
	if(GetSobProxy())
		GetSobProxy()->NotifyStateEnter(uiStateId);
}

void CNtlSobActor::NotifyStateExit(RwUInt32 uiOldState, RwUInt32 uiNewState)
{
	if(GetSobProxy())
		GetSobProxy()->NotifyStateExit(uiOldState, uiNewState);
}

CNtlFSMStateBase* CNtlSobActor::GetCurrentState(void)
{
	if(!m_pFSMLayer)
		return NULL;
	return m_pFSMLayer->GetCurrentState();
}


///////////////////////////////////////////////////////
// event handler
///////////////////////////////////////////////////////

void CNtlSobActor::SobTargetSelectEventHandler(RWS::CMsg &pMsg)
{
	CNtlSobProxy *pSobProxy = GetSobProxy();
	if(pSobProxy == NULL)
		return;

	pSobProxy->HandleEvents(pMsg);

	if(GetFlags() & SLFLAG_TARGET_SELECT_GUI_UPDATE)
	{
		SNtlEventSobTargetSelect *pSobTargetSelect = reinterpret_cast<SNtlEventSobTargetSelect*>( pMsg.pData );
		if(GetSerialID() == pSobTargetSelect->hSerialId)
			CNtlSLEventGenerator::SobTargetUpdate(GetSerialID(), TRUE); 
	}
}


void CNtlSobActor::SobAttackSelectEventHandler(RWS::CMsg &pMsg)
{
	CNtlSobProxy *pSobProxy = GetSobProxy();
	if(pSobProxy == NULL)
		return;

	pSobProxy->HandleEvents(pMsg);

	if(GetFlags() & SLFLAG_TARGET_SELECT_GUI_UPDATE)
	{
		SNtlEventSobAttackSelect *pSobAttackSelect = reinterpret_cast<SNtlEventSobAttackSelect*>( pMsg.pData );
		if(GetSerialID() == pSobAttackSelect->hSerialId)
			CNtlSLEventGenerator::SobTargetUpdate(GetSerialID(), TRUE); 
	}
}


void CNtlSobActor::SobTargetSelectReleaseEventHandler(RWS::CMsg &pMsg)
{
	CNtlSobProxy *pSobProxy = GetSobProxy();
	if(pSobProxy == NULL)
		return;

	pSobProxy->HandleEvents(pMsg);

	if(GetFlags() & SLFLAG_TARGET_SELECT_GUI_UPDATE)
	{
		CNtlSLEventGenerator::SobTargetUpdate(GetSerialID(), FALSE);
	}
}

void CNtlSobActor::SetFinialHurtSerialId(SERIAL_HANDLE hAttackSerialId)
{
	m_hFinialHurtSerialId	= hAttackSerialId;
	m_fHurtOverTime			= 0.0f;
}

void CNtlSobActor::SobProxyEventHandler( RWS::CMsg& pMsg ) 
{
    if(GetSobProxy())
    {
        GetSobProxy()->HandleEvents(pMsg);
    }
}

