#include "precomp_ntlsimulation.h"
#include "NtlFSMCharActLayer.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlFSMDef.h"
#include "NtlSobActor.h"
#include "NtlFSMCharAct.h"
#include "NtlFSMCharActAgent.h"
#include "NtlFSMCharController.h"
#include "NtlFSMLookAtManager.h"
#include "NtlBehaviorChar.h"
#include "NtlFSMCharActEventProc.h"

DEFINITION_MEMORY_POOL(CNtlFSMCharActLayer)

CNtlFSMCharActLayer::CNtlFSMCharActLayer()
{
	m_pBePush = NULL;
	m_pBeShock = NULL;
	m_pKnownDown = NULL;
}

CNtlFSMCharActLayer::~CNtlFSMCharActLayer()
{
	if (m_pAgent)
		delete m_pAgent;
}

RwBool CNtlFSMCharActLayer::Create(RwUInt32 uiController)
{
	NTL_FUNCTION("CNtlFSMCharActLayer::Create");

	m_pFSM = GetNtlFSMLookAtManager()->GetFSMCharAct();
	m_pAgent = NTL_NEW CNtlFSMCharActAgent;

	if(!m_pAgent->Create())
	{
		NTL_RETURN(FALSE);
	}
	m_pAgent->SetActor(m_pActor);

	CreateController(uiController);
	
	NTL_RETURN(TRUE);
}


void CNtlFSMCharActLayer::Destroy(void)
{
	NTL_DELETE( m_pBeShock );
	NTL_DELETE( m_pBePush );
	NTL_DELETE( m_pKnownDown );

	CNtlFSMLayer::Destroy();
}

void CNtlFSMCharActLayer::Update(RwReal fElapsed)
{
	CNtlFSMLayer::Update(fElapsed); 

	if(m_pBeShock)
	{
		m_pBeShock->Update(fElapsed);
		if(m_pBeShock->IsFinish())
		{
			m_pBeShock->Exit(); 

			NTL_DELETE( m_pBeShock );
		}
	}

	if(m_pBePush)
	{
		m_pBePush->Update(fElapsed);
		if(m_pBePush->IsFinish())
		{
			m_pBePush->Exit(); 

			NTL_DELETE( m_pBePush );
		}
	}

	if(m_pKnownDown)
	{
		m_pKnownDown->Update(fElapsed);
		if(m_pKnownDown->IsFinish())
		{
			m_pKnownDown->Exit(); 

			NTL_DELETE( m_pKnownDown );
		}
	}

}

RwUInt32 CNtlFSMCharActLayer::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobHit)
	{
		SNtlEventSobHit *pHit = reinterpret_cast<SNtlEventSobHit*>(pMsg.pData);

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SHitStuff *pHitStuff = pBeData->GetHitStuff(); 
		pHitStuff->Reset();
		
		memcpy(pHitStuff, &pHit->sHitStuff, sizeof(SHitStuff));

		RwUInt32 uiResult = pHit->sHitStuff.uiAttr.bitResult;
		RwUInt32 uiBehavior = pHit->sHitStuff.uiAttr.bitBehavior;

		if( (uiResult == NTL_ATTACK_RESULT_SUCCESS || uiResult == NTL_ATTACK_RESULT_KNOCKDOWN || uiResult == NTL_ATTACK_RESULT_SLIDING) 
			&& (uiBehavior == NTL_ATTACK_NORMAL || uiBehavior == NTL_ATTACK_PUSH) )
		{
			// hit shock를 적용하는 놈이면.?
			if( !(m_pActor->GetFlags() & SLFLAG_NOT_HITSHOCK) )
			{
				if(m_pBeShock)
					m_pBeShock->UpdateData(); 
				else
				{
					m_pBeShock = NTL_NEW CNtlBehaviorCharShock;
					m_pBeShock->SetActor(m_pActor); 
					m_pBeShock->Enter();
				}	
			}

			if(uiBehavior == NTL_ATTACK_PUSH)
			{
				if(m_pBePush)
					m_pBePush->UpdateData();
				else
				{
					m_pBePush = NTL_NEW CNtlBehaviorCharPush;
					m_pBePush->SetActor(m_pActor); 
					m_pBePush->Enter();
				}

				CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(GetFSMAgent());
				pCharActAgent->SetActiveAdjustPosition(FALSE);
			}
		}
	}

	return CNtlFSMLayer::HandleEvents(pMsg); 
}


void CNtlFSMCharActLayer::CreateController(RwUInt32 uiController)
{
	CNtlFSMController *pController;

	if(uiController == SLCONTROLLER_PLYAER)
	{
		pController = NTL_NEW CNtlFSMPlayerController;
		m_pAgent->SetController(pController); 
		pController->SetAgent(m_pAgent); 
		pController->SetActor(m_pActor);
	}

	else if(uiController == SLCONTROLLER_PLAYER_SYNC)
	{
		pController = NTL_NEW CNtlFSMPlayerSyncController;
		m_pAgent->SetController(pController); 
		pController->SetAgent(m_pAgent);
		pController->SetActor(m_pActor);
	}

	else if(uiController == SLCONTROLLER_SERVER)
	{
		pController = NTL_NEW CNtlFSMServerController;
		m_pAgent->SetController(pController); 
		pController->SetAgent(m_pAgent);
		pController->SetActor(m_pActor);
	}

	else if(uiController == SLCONTROLLER_SUMMON_PET)
	{
		pController = NTL_NEW CNtlFSMSummonPetController;
		m_pAgent->SetController(pController); 
		pController->SetAgent(m_pAgent);
		pController->SetActor(m_pActor);
	}
}
