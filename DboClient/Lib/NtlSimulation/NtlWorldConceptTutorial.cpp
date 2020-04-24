#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptTutorial.h"

// core
#include "NtlDebug.h"

// shared
#include "TimeQuestTable.h"
#include "NtlLuaState.h"

// simulation
#include "NtlSLEventFunc.h"
#include "NtlSLLuaGlue.h"
#include "NtlSLLogic.h"


CNtlWorldConceptTutorial::CNtlWorldConceptTutorial(void)
:m_bDisableMinimap(FALSE)
,m_pTLSystem(NULL)
,m_pConditionCheck(NULL)
{
	CNtlWorldConcept::GetInstance()->SetHaveTutorialWorldConcept( TRUE );

	m_pConditionCheck = NTL_NEW CDboTLConditionCheck;
	if ( !m_pConditionCheck->Create() )
	{
		NtlLogFilePrint( "Creating TL conditon check failed" );
		NTL_DELETE( m_pConditionCheck );
	}
}

CNtlWorldConceptTutorial::~CNtlWorldConceptTutorial(void)
{
	CNtlWorldConcept::GetInstance()->SetHaveTutorialWorldConcept( FALSE );

	NTL_DELETE( m_pConditionCheck );
	NTL_DELETE( m_pTLSystem );
}

void CNtlWorldConceptTutorial::Update(RwReal fElapsedTime)
{
	CNtlWorldConceptController::Update( fElapsedTime );

	if ( m_pTLSystem )
	{
		m_pTLSystem->Update( fElapsedTime );
	}

	if ( m_pConditionCheck )
	{
		m_pConditionCheck->Update( fElapsedTime );
	}
}

RwBool CNtlWorldConceptTutorial::IsEnableAction(EAvatarAction eAction) 
{
	if ( eAction == E_ACTION_ATTACK )
	{
		return Logic_CanAttack_in_Tutorial( ETL_ATTACK_TYPE_NORMAL_ATTACK );
	}
	if ( eAction == E_ACTION_PICK_WORLD ||
		 eAction == E_ACTION_PICK_OBJECT )
	{
		return CanMouseInput( ETL_MOUSE_INPUT_TYPE_MOVE );
	}

	return TRUE;
}

void CNtlWorldConceptTutorial::HandleEvents(RWS::CMsg &pMsg)
{
	if ( m_pTLSystem )
	{
		m_pTLSystem->HandleEvents( pMsg );
	}

	if ( m_pConditionCheck )
	{
		m_pConditionCheck->HandleEvents( pMsg );
	}

	if( pMsg.Id == g_EventTLDisableMinimap )
	{
		m_bDisableMinimap = TRUE;
	}
}

void CNtlWorldConceptTutorial::BeforeLoadingDirect(void)
{
}

void CNtlWorldConceptTutorial::AfterLoadingDirect(void)
{
}

RwBool CNtlWorldConceptTutorial::IsBeforeLoadingDirect(void)
{
	return FALSE;
}

RwBool CNtlWorldConceptTutorial::IsAfterLoadingDirect(void)
{
	return FALSE;
}

RwBool CNtlWorldConceptTutorial::CanMouseInput(ETL_MOUSE_INPUT_TYPE byType)
{
	return m_pConditionCheck->CanMouseInput(byType);
}

RwBool CNtlWorldConceptTutorial::CanKeybaordInput(ETL_KEYBOARD_INPUT_TYPE byType)
{
	return m_pConditionCheck->CanKeybaordInput(byType);
}

RwBool CNtlWorldConceptTutorial::CanAttack(ETL_ATTACK_TYPE byType)
{
	return m_pConditionCheck->CanAttack(byType);
}

RwBool CNtlWorldConceptTutorial::IsDisableMinimap()
{
	return m_bDisableMinimap;
}

RwBool CNtlWorldConceptTutorial::IsStepSkipping()
{
	if( m_pTLSystem )
		return m_pTLSystem->IsSkipping();

	return FALSE;
}

void CNtlWorldConceptTutorial::SetTMQState(eTIMEQUEST_GAME_STATE eState)
{
	// 서버 상태를 클라이언트 상태로 컨버트
	switch(eState)
	{
	case TIMEQUEST_GAME_STATE_WAIT:				m_byTMQState = WORLD_STATE_TMQ_WAIT;				break;
	case TIMEQUEST_GAME_STATE_PREPARE:			m_byTMQState = WORLD_STATE_TMQ_PREPARE;				break;
	case TIMEQUEST_GAME_STATE_ARRIVE:			m_byTMQState = WORLD_STATE_TMQ_ARRIVE;				break;
	case TIMEQUEST_GAME_STATE_BEGIN:			m_byTMQState = WORLD_STATE_TMQ_BEGIN;				break;
	case TIMEQUEST_GAME_STATE_STAGE_READY:		m_byTMQState = WORLD_STATE_TMQ_STAGE_READY;			break;
	case TIMEQUEST_GAME_STATE_STAGE_START:		m_byTMQState = WORLD_STATE_TMQ_STAGE_START;			break;
	case TIMEQUEST_GAME_STATE_STAGE_FINISH:		m_byTMQState = WORLD_STATE_TMQ_STAGE_FINISH;		break;
	case TIMEQUEST_GAME_STATE_END:				m_byTMQState = WORLD_STATE_TMQ_END;					break;
	case TIMEQUEST_GAME_STATE_LEAVE:			m_byTMQState = WORLD_STATE_TMQ_LEAVE;				break;
	case TIMEQUEST_GAME_STATE_FAIL:				m_byTMQState = WORLD_STATE_TMQ_FAIL;				break;
	case TIMEQUEST_GAME_STATE_CLOSE:			m_byTMQState = WORLD_STATE_TMQ_CLOSE;				break;
	default:	DBO_FAIL("Can not treat server tmq state : " << eState);
	}

	NotifyTMQStateUpdate();
}

void CNtlWorldConceptTutorial::SetTMQState_from_Cleint(RwUInt8 byState)
{
	m_byTMQState = byState;

	NotifyTMQStateUpdate();
}

void CNtlWorldConceptTutorial::NotifyTMQStateUpdate()
{
	CNtlSLEventGenerator::TMQCleintState(WORLD_PLAY_TUTORIAL, m_byTMQState);

	switch ( m_byTMQState )
	{
	case WORLD_STATE_TMQ_TERRAIN_READY:
		{
			LoadScript();

			if ( m_pTLSystem )
			{
				m_pTLSystem->Update( 0.f );
			}
		}
		break;
	}
}

VOID CNtlWorldConceptTutorial::NextStep(void)
{
	if( m_pTLSystem )
		m_pTLSystem->Skip();
}

VOID CNtlWorldConceptTutorial::LoadScript(void)
{
	NTL_DELETE( m_pTLSystem );

	m_pTLSystem = NTL_NEW CDboTLSystem;
	if ( !m_pTLSystem->Create() )
	{
		NtlLogFilePrint( "Creating TL System failed" );
		NTL_DELETE( m_pTLSystem );
		return;
	}

	m_pTLSystem->SetActive( TRUE );
}