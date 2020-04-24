#include "precomp_dboclient.h"
#include "TBGui.h"

// core
#include "NtlDebug.h"

// shared
//#include "NtlRankBattle.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"
#include "NtlSLGlobal.h"
//#include "NtlSobAvatar.h"
#include "NtlSLTBudokai.h"

// Client
#include "DboGlobal.h"
#include "TBMinorMatchInfoGui.h"
#include "TBMinorDirectionGui.h"
#include "TBMajorMatchInfoGui.h"
#include "TBMajorDirectionGui.h"
#include "TBFinalMatchInfoGui.h"
#include "TBFinalDirectionGui.h"
#include "TBTournamentMatchInfoGui.h"
//#include "BudokaiTournamentGui.h"
#include "TBTournamentGui.h"
#include "TBMinorMatchResultGui.h"
#include "TBMatchResultGui.h"
#include "TBAwardGui.h"
#include "TBNewsGui.h"
#include "TBRequestGui.h"
#include "DialogManager.h"
#include "SideDialogManager.h"
#include "ChatBalloonManager.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"
#include "DboEventGenerator.h"
#include "ResultNarrationGui.h"
#include "CounterGui.h"

// DBO(Observer)
#include "TBMatchIndiObRedGui.h"
#include "TBMatchIndiObBlueGui.h"
#include "TBMatchTeamObRedGui.h"
#include "TBMatchTeamObBlueGui.h"
#include "TBMatchObserverVersusGui.h"

CTBGui* CTBGui::m_pInstance = NULL;

CTBGui::CTBGui(VOID)
{
	m_pMinorMatchInfoGui = NULL;
	m_pMinorDirectionGui = NULL;
	m_pMajorMatchInfoGui = NULL;
	m_pMajorDirectionGui = NULL;
	m_pFinalMatchInfoGui = NULL;
	m_pFinalDirectionGui = NULL;
	m_pTournamentMatchInfoGui = NULL;
	m_pTournamentGui = NULL;
	m_pNewsGui = NULL;
	m_pRequestGui = NULL;
	m_pMinorMatchResultGui = NULL;
	m_pMatchResultGui = NULL;
	m_pAwardResultGui = NULL;
	m_pResultNarration = NULL;
	m_pCounter = NULL;
	m_pObserverVersusGui = NULL;
	m_pMatchIndiObRedGui = NULL;
	m_pMatchIndiObBlueGui = NULL;
	m_pMatchTeamObRedGui = NULL;
	m_pMatchTeamObBlueGui = NULL;
	m_bCreate = FALSE;
}

VOID CTBGui::CreateInstance(VOID)
{
	if( !m_pInstance )
	{
		m_pInstance = NTL_NEW CTBGui;
		DBO_ASSERT( m_pInstance, "CTBGui::CreateInstance : Instance allocate fail" );				

		m_pInstance->LinkMsg( g_EventBudokaiStateInfoNfy );
		m_pInstance->LinkMsg( g_EventBudokaiUpdateStateNfy );
		m_pInstance->LinkMsg( g_EventMinorMatchStateUpdate );
		m_pInstance->LinkMsg( g_EventMajorMatchStateUpdate );
		m_pInstance->LinkMsg( g_EventFinalMatchStateUpdate );
		m_pInstance->LinkMsg( g_EventMinorMatchPlayerState );
		m_pInstance->LinkMsg( g_EventMajorMatchPlayerState );
		m_pInstance->LinkMsg( g_EventFinalMatchPlayerState );
		m_pInstance->LinkMsg( g_EventMinorMatchUpdateScore );
		m_pInstance->LinkMsg( g_EventBudokaiProgressMessageNfy );
		m_pInstance->LinkMsg( g_EventChangeWorldConceptState );		
		m_pInstance->LinkMsg( g_EventSobCreate, NULL, 0x7000 );
	}
}

VOID CTBGui::DeleteInstance(VOID)
{
	if( m_pInstance )
	{
		m_pInstance->UnLinkMsg( g_EventBudokaiStateInfoNfy );
		m_pInstance->UnLinkMsg( g_EventBudokaiUpdateStateNfy );
		m_pInstance->UnLinkMsg( g_EventMinorMatchStateUpdate );
		m_pInstance->UnLinkMsg( g_EventMajorMatchStateUpdate );
		m_pInstance->UnLinkMsg( g_EventFinalMatchStateUpdate );
		m_pInstance->UnLinkMsg( g_EventMinorMatchPlayerState );
		m_pInstance->UnLinkMsg( g_EventMajorMatchPlayerState );
		m_pInstance->UnLinkMsg( g_EventFinalMatchPlayerState );
		m_pInstance->UnLinkMsg( g_EventMinorMatchUpdateScore );
		m_pInstance->UnLinkMsg( g_EventBudokaiProgressMessageNfy );
		m_pInstance->UnLinkMsg( g_EventChangeWorldConceptState );
		m_pInstance->UnLinkMsg( g_EventSobCreate );

		m_pInstance->Destroy();
		m_pInstance->DestroyBudokaiNews();
		NTL_DELETE( m_pInstance );
	}
}

VOID CTBGui::Create( RwUInt8 byMatchType )
{
	if( byMatchType == MATCHWORLD_TYPE_MINOR_MATCH )
	{
		m_pMinorMatchInfoGui = NTL_NEW CTBMinorMatchInfoGui( "TBMinorMatchInfoGui" );
		if( !m_pMinorMatchInfoGui->Create() )
		{
			m_pMinorMatchInfoGui->Destroy();
			NTL_DELETE( m_pMinorMatchInfoGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMinorMatchInfoGui );
		GetSideDialogManager()->RegistDialog( SDIALOG_BUDOKAI_MINORMATCH_INFO, m_pMinorMatchInfoGui, &CTBMinorMatchInfoGui::SwitchDialog );

		m_pMinorDirectionGui = NTL_NEW CTBMinorDirectionGui( "TBMinorDirectionGui" );
		if( !m_pMinorDirectionGui->Create() )
		{
			m_pMinorDirectionGui->Destroy();
			NTL_DELETE( m_pMinorDirectionGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMinorDirectionGui );		

		m_pMinorMatchResultGui = NTL_NEW CTBMinorMatchResultGui( "TBMatchResultGui" );
		if( !m_pMinorMatchResultGui->Create() )
		{
			m_pMinorMatchResultGui->Destroy();
			NTL_DELETE( m_pMinorMatchResultGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMinorMatchResultGui );		
	}	

	if( byMatchType == MATCHWORLD_TYPE_MAJOR_MATCH )
	{
		m_pMajorMatchInfoGui = NTL_NEW CTBMajorMatchInfoGui( "TBMajorMatchInfoGui" );
		if( !m_pMajorMatchInfoGui->Create() )
		{
			m_pMajorMatchInfoGui->Destroy();
			NTL_DELETE( m_pMajorMatchInfoGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMajorMatchInfoGui );
		GetSideDialogManager()->RegistDialog( SDIALOG_BUDOKAI_MAJORMATCH_INFO, m_pMajorMatchInfoGui, &CTBMajorMatchInfoGui::SwitchDialog );

		m_pMajorDirectionGui = NTL_NEW CTBMajorDirectionGui( "TBMajorDirectionGui" );
		if( !m_pMajorDirectionGui->Create() )
		{
			m_pMajorDirectionGui->Destroy();
			NTL_DELETE( m_pMajorDirectionGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMajorDirectionGui );		

		m_pMatchResultGui = NTL_NEW CTBMatchResultGui( "TBMatchResultGui" );
		if( !m_pMatchResultGui->Create( MATCHWORLD_TYPE_MAJOR_MATCH ) )
		{
			m_pMatchResultGui->Destroy();
			NTL_DELETE( m_pMatchResultGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMatchResultGui );

		m_pMatchIndiObRedGui = NTL_NEW CTBMatchIndiObRedGui( "TBMatchIndiObRedGui" );
		if( !m_pMatchIndiObRedGui->Create() )
		{
			m_pMatchIndiObRedGui->Destroy();
			NTL_DELETE( m_pMatchIndiObRedGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMatchIndiObRedGui );		

		m_pMatchIndiObBlueGui = NTL_NEW CTBMatchIndiObBlueGui( "TBMatchIndiObBlueGui" );
		if( !m_pMatchIndiObBlueGui->Create() )
		{
			m_pMatchIndiObBlueGui->Destroy();
			NTL_DELETE( m_pMatchIndiObBlueGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMatchIndiObBlueGui );

		m_pMatchTeamObRedGui = NTL_NEW CTBMatchTeamObRedGui( "TBMatchTeamObRedGui" );
		if( !m_pMatchTeamObRedGui->Create() )
		{
			m_pMatchTeamObRedGui->Destroy();
			NTL_DELETE( m_pMatchTeamObRedGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMatchTeamObRedGui );		

		m_pMatchTeamObBlueGui = NTL_NEW CTBMatchTeamObBlueGui( "TBMatchTeamObBlueGui" );
		if( !m_pMatchTeamObBlueGui->Create() )
		{
			m_pMatchTeamObBlueGui->Destroy();
			NTL_DELETE( m_pMatchTeamObBlueGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMatchTeamObBlueGui );

		m_pObserverVersusGui = NTL_NEW CTBMatchObserverVersusGui( "TBMatchObserverVersusGui" );
		if( !m_pObserverVersusGui->Create() )
		{
			m_pObserverVersusGui->Destroy();
			NTL_DELETE( m_pObserverVersusGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pObserverVersusGui );
	}

	if( byMatchType == MATCHWORLD_TYPE_FINAL_MATCH )
	{
		m_pFinalMatchInfoGui = NTL_NEW CTBFinalMatchInfoGui( "TBFinalMatchInfoGui" );
		if( !m_pFinalMatchInfoGui->Create() )
		{
			m_pFinalMatchInfoGui->Destroy();
			NTL_DELETE( m_pFinalMatchInfoGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pFinalMatchInfoGui );
		GetSideDialogManager()->RegistDialog( SDIALOG_BUDOKAI_FINALMATCH_INFO, m_pFinalMatchInfoGui, &CTBFinalMatchInfoGui::SwitchDialog );

		m_pFinalDirectionGui = NTL_NEW CTBFinalDirectionGui( "TBFinalDirectionGui" );
		if( !m_pFinalDirectionGui->Create() )
		{
			m_pFinalDirectionGui->Destroy();
			NTL_DELETE( m_pFinalDirectionGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pFinalDirectionGui );		

		m_pMatchResultGui = NTL_NEW CTBMatchResultGui( "TBMatchResultGui" );
		if( !m_pMatchResultGui->Create( MATCHWORLD_TYPE_FINAL_MATCH ) )
		{
			m_pMatchResultGui->Destroy();
			NTL_DELETE( m_pMatchResultGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMatchResultGui );		

		m_pAwardResultGui = NTL_NEW CTBAwardResultGui( "TBAwardResultGui" );
		if( !m_pAwardResultGui->Create() )
		{
			m_pMatchResultGui->Destroy();
			NTL_DELETE( m_pAwardResultGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pAwardResultGui );

		m_pMatchIndiObRedGui = NTL_NEW CTBMatchIndiObRedGui( "TBMatchIndiObRedGui" );
		if( !m_pMatchIndiObRedGui->Create() )
		{
			m_pMatchIndiObRedGui->Destroy();
			NTL_DELETE( m_pMatchIndiObRedGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMatchIndiObRedGui );		

		m_pMatchIndiObBlueGui = NTL_NEW CTBMatchIndiObBlueGui( "TBMatchIndiObBlueGui" );
		if( !m_pMatchIndiObBlueGui->Create() )
		{
			m_pMatchIndiObBlueGui->Destroy();
			NTL_DELETE( m_pMatchIndiObBlueGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMatchIndiObBlueGui );

		m_pMatchTeamObRedGui = NTL_NEW CTBMatchTeamObRedGui( "TBMatchTeamObRedGui" );
		if( !m_pMatchTeamObRedGui->Create() )
		{
			m_pMatchTeamObRedGui->Destroy();
			NTL_DELETE( m_pMatchTeamObRedGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMatchTeamObRedGui );		

		m_pMatchTeamObBlueGui = NTL_NEW CTBMatchTeamObBlueGui( "TBMatchTeamObBlueGui" );
		if( !m_pMatchTeamObBlueGui->Create() )
		{
			m_pMatchTeamObBlueGui->Destroy();
			NTL_DELETE( m_pMatchTeamObBlueGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pMatchTeamObBlueGui );

		m_pObserverVersusGui = NTL_NEW CTBMatchObserverVersusGui( "TBMatchObserverVersusGui" );
		if( !m_pObserverVersusGui->Create() )
		{
			m_pObserverVersusGui->Destroy();
			NTL_DELETE( m_pObserverVersusGui );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pObserverVersusGui );
	}

	m_pResultNarration = NTL_NEW CResultNarrationGui( "TBResultNarration" );
	if( !m_pResultNarration->Create() )
	{
		m_pResultNarration->Destroy();
		NTL_DELETE( m_pResultNarration );
	}
	CNtlPLGuiManager::GetInstance()->AddGui( m_pResultNarration );

	m_pCounter = NTL_NEW CCounterGui( "TBCounter" );
	if( !m_pCounter->Create( COUNTER_DISPLAY_2, 0.0f ) )
	{
		m_pCounter->Destroy();
		NTL_DELETE( m_pCounter );
	}
	CNtlPLGuiManager::GetInstance()->AddGui( m_pCounter );
	m_pCounter->Show( false );
	m_pCounter->Pause( TRUE );

	//LinkMsg( g_EventSobInfoUpdate );
	LinkMsg( g_EventResize );

	m_bCreate = TRUE;
}

VOID CTBGui::Destroy(VOID)
{
	if( m_bCreate )
	{
		//UnLinkMsg( g_EventSobInfoUpdate );
		UnLinkMsg( g_EventResize );
	}

	if( m_pMinorMatchInfoGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pMinorMatchInfoGui );
		GetSideDialogManager()->UnRegistDialog( SDIALOG_BUDOKAI_MINORMATCH_INFO );
		m_pMinorMatchInfoGui->Destroy();
		NTL_DELETE( m_pMinorMatchInfoGui );
	}

	if( m_pMinorDirectionGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pMinorDirectionGui );
		m_pMinorDirectionGui->Destroy();
		NTL_DELETE( m_pMinorDirectionGui );
	}

	if( m_pMajorMatchInfoGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pMajorMatchInfoGui );
		GetSideDialogManager()->UnRegistDialog( SDIALOG_BUDOKAI_MAJORMATCH_INFO );
		m_pMajorMatchInfoGui->Destroy();
		NTL_DELETE( m_pMajorMatchInfoGui );
	}

	if( m_pMajorDirectionGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pMajorDirectionGui );
		m_pMajorDirectionGui->Destroy();
		NTL_DELETE( m_pMajorDirectionGui );
	}

	if( m_pFinalMatchInfoGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pFinalMatchInfoGui );
		GetSideDialogManager()->UnRegistDialog( SDIALOG_BUDOKAI_FINALMATCH_INFO );
		m_pFinalMatchInfoGui->Destroy();
		NTL_DELETE( m_pFinalMatchInfoGui );
	}

	if( m_pFinalDirectionGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pFinalDirectionGui );
		m_pFinalDirectionGui->Destroy();
		NTL_DELETE( m_pFinalDirectionGui );
	}

	if( m_pMatchResultGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pMatchResultGui );
		m_pMatchResultGui->Destroy();
		NTL_DELETE( m_pMatchResultGui );
	}

	if( m_pMinorMatchResultGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pMinorMatchResultGui );
		m_pMinorMatchResultGui->Destroy();
		NTL_DELETE( m_pMinorMatchResultGui );
	}

	if( m_pAwardResultGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pAwardResultGui );
		m_pAwardResultGui->Destroy();
		NTL_DELETE( m_pAwardResultGui );
	}

	if( m_pResultNarration )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pResultNarration );
		m_pResultNarration->Destroy();
		NTL_DELETE( m_pResultNarration );
	}

	if( m_pCounter )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pCounter );
		m_pCounter->Destroy();
		NTL_DELETE( m_pCounter );
	}

	if( m_pObserverVersusGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pObserverVersusGui );
		m_pObserverVersusGui->Destroy();
		NTL_DELETE( m_pObserverVersusGui );
	}
	
	if( m_pMatchIndiObRedGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pMatchIndiObRedGui );
		m_pMatchIndiObRedGui->Destroy();
		NTL_DELETE( m_pMatchIndiObRedGui );
	}

	if( m_pMatchIndiObBlueGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pMatchIndiObBlueGui );
		m_pMatchIndiObBlueGui->Destroy();
		NTL_DELETE( m_pMatchIndiObBlueGui );
	}

	if( m_pMatchTeamObRedGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pMatchTeamObRedGui );
		m_pMatchTeamObRedGui->Destroy();
		NTL_DELETE( m_pMatchTeamObRedGui );
	}

	if( m_pMatchTeamObBlueGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pMatchTeamObBlueGui );
		m_pMatchTeamObBlueGui->Destroy();
		NTL_DELETE( m_pMatchTeamObBlueGui );
	}

	m_bCreate = FALSE;	
}

VOID CTBGui::CreateBudokaiNewsOnce()
{
	// 천하제일 무도회 정보가 CLOSE가 아니라면 Open한다.
	STenkaichiBudokaiInfo* pBudokaiState = GetNtlSLGlobal()->GetTBudokaiStateInfo();

	if( pBudokaiState->sStateInfo.byState != BUDOKAI_STATE_JUNIOR_CLOSE ||
		pBudokaiState->sStateInfo.byState != BUDOKAI_STATE_CLOSE )
		CreateBudokaiNews();
}

VOID CTBGui::CreateBudokaiNews()
{
	// 천하제일 무도회 소식지
	if( m_pNewsGui == NULL )
	{
		m_pNewsGui = NTL_NEW CTBNewsGui( "TBNewsGui" );
		if( !m_pNewsGui->Create() )
		{
			m_pNewsGui->Destroy();
			NTL_DELETE( m_pNewsGui );
		}
		else
		{
			CNtlPLGuiManager::GetInstance()->AddGui( m_pNewsGui );
			GetDialogManager()->RegistDialog( DIALOG_BUDOKAI_NEWS, m_pNewsGui, &CTBNewsGui::SwitchDialog );
		}
	}

	// 천하제일 무도회 신청서
	if( m_pRequestGui == NULL )
	{
		m_pRequestGui = NTL_NEW CTBRequestGui( "TBRequestGui" );
		if( !m_pRequestGui->Create() )
		{
			m_pRequestGui->Destroy();
			NTL_DELETE( m_pRequestGui );
		}
		else
		{
			 CNtlPLGuiManager::GetInstance()->AddGui( m_pRequestGui );
			 GetDialogManager()->RegistDialog( DIALOG_BUDOKAI_REQUEST, m_pRequestGui, &CTBRequestGui::SwitchDialog );
		}
	}

	if( m_pTournamentGui == NULL )
	{
		m_pTournamentGui = NTL_NEW CTBTournamentGui( "BudokaiTournamentGui" );
		if( !m_pTournamentGui->Create() )
		{
			m_pTournamentGui->Destroy();
			NTL_DELETE( m_pTournamentGui );
		}
		else
		{
			CNtlPLGuiManager::GetInstance()->AddGui( m_pTournamentGui );
			GetDialogManager()->RegistDialog( DIALOG_BUDOKAI_TOURNAMENT, m_pTournamentGui, &CTBTournamentGui::SwitchDialog );
		}
	}

	if( m_pTournamentMatchInfoGui == NULL )
	{
		m_pTournamentMatchInfoGui = NTL_NEW CTBTournamentMatchInfoGui( "BudokaiTournamentMatchInfoGui" );
		if( !m_pTournamentMatchInfoGui->Create() )
		{
			m_pTournamentMatchInfoGui->Destroy();
			NTL_DELETE( m_pTournamentMatchInfoGui )
		}
		else
		{
			CNtlPLGuiManager::GetInstance()->AddGui( m_pTournamentMatchInfoGui );
			GetDialogManager()->RegistDialog( DIALOG_BUDOKAI_TOURNAMENT_MATCHINFO, m_pTournamentMatchInfoGui, &CTBTournamentMatchInfoGui::SwitchDialog );
		}	
	}
}

VOID CTBGui::DestroyBudokaiNews()
{
	if( m_pNewsGui )
	{
		GetDialogManager()->UnRegistDialog( DIALOG_BUDOKAI_NEWS );
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pNewsGui );
		m_pNewsGui->Destroy();
		NTL_DELETE( m_pNewsGui );
	}

	if( m_pRequestGui )
	{
		GetDialogManager()->UnRegistDialog( DIALOG_BUDOKAI_REQUEST );
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pRequestGui );
		m_pRequestGui->Destroy();
		NTL_DELETE( m_pRequestGui );
	}

	if( m_pTournamentGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pTournamentGui );
		GetDialogManager()->UnRegistDialog( DIALOG_BUDOKAI_TOURNAMENT );
		m_pTournamentGui->Destroy();
		NTL_DELETE( m_pTournamentGui );
	}

	if( m_pTournamentMatchInfoGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pTournamentMatchInfoGui );
		GetDialogManager()->UnRegistDialog( DIALOG_BUDOKAI_TOURNAMENT_MATCHINFO );
		m_pTournamentMatchInfoGui->Destroy();
		NTL_DELETE( m_pTournamentMatchInfoGui );
	}
}

CTBGui* CTBGui::GetInstance(VOID)
{
	return m_pInstance;
}

VOID CTBGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventBudokaiUpdateStateNfy )
	{
		sNtlEventBudokaiUpdateStateNfy* pNotify = reinterpret_cast<sNtlEventBudokaiUpdateStateNfy*>( msg.pData );

		if( pNotify->sStateInfo.byState == BUDOKAI_STATE_OPEN_NOTICE ||
			pNotify->sStateInfo.byState == BUDOKAI_STATE_JUNIOR_OPEN_NOTICE )
		{
			CreateBudokaiNews();
			ProgressMessageNotify( BUDOKAI_PROGRESS_MESSAGE_START );
			
		}
		else if( pNotify->sStateInfo.byState == BUDOKAI_STATE_CLOSE ||
			pNotify->sStateInfo.byState == BUDOKAI_STATE_JUNIOR_CLOSE )
		{
			DestroyBudokaiNews();
			ProgressMessageNotify( BUDOkAI_PROGRESS_MESSAGE_END );
		}
	}
	else if( msg.Id == g_EventBudokaiStateInfoNfy )
	{
		sNtlEventBudokaiStateInfoNfy* pNotify = reinterpret_cast< sNtlEventBudokaiStateInfoNfy*>( msg.pData );

		if( pNotify->sStateInfo.byState != BUDOKAI_STATE_CLOSE ||
			pNotify->sStateInfo.byState != BUDOKAI_STATE_JUNIOR_CLOSE )
			CreateBudokaiNews();
		else
			DestroyBudokaiNews();
	}
	else if( msg.Id == g_EventMinorMatchStateUpdate )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		DBO_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );	

		// 참가자와 관전자를 구분하여 처리한다.
		stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
		if( pMember )
			HandleEventMinorMatchStateUpdateEntrance( msg );
		else
			HandleEventMinorMatchStateUpdateObserver( msg );
	}
	else if( msg.Id == g_EventMajorMatchStateUpdate )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		DBO_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );

		stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
		if( pMember )
			HandleEventMajorMatchStateUpdateEntrance( msg );
		else
			HandleEventMajorMatchStateUpdateObserver( msg );
	}
	else if( msg.Id == g_EventFinalMatchStateUpdate )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		DBO_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );

		stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
		if( pMember )
			HandleEventMajorMatchStateUpdateEntrance( msg );
		else
			HandleEventMajorMatchStateUpdateObserver( msg );
	}
	//else if( msg.Id == g_EventSobInfoUpdate )
	//{
	//	SNtlEventSobInfoUpdate *pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData ); 

	//	if( pUpdate->uiUpdateType & EVENT_AIUT_ATTR )
	//	{
	//		CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( pUpdate->hSerialId );
	//		GetBalloonManager()->SetBattleGaugeData( pSobObj, Logic_GetMaxLp( pSobObj ), Logic_GetLp( pSobObj ) );
	//	}
	//}
	else if( msg.Id == g_EventMinorMatchPlayerState )
	{
		SNtlEventMinorMatchPlayerState* pPlayerState = reinterpret_cast<SNtlEventMinorMatchPlayerState*>( msg.pData );
		SetPlayerState( pPlayerState->hPlayer );
	}
	else if( msg.Id == g_EventMajorMatchPlayerState )
	{
		SNtlEventMajorMatchPlayerState* pPlayerState = reinterpret_cast<SNtlEventMajorMatchPlayerState*>( msg.pData );
		SetPlayerState( pPlayerState->hPlayer );
	}
	else if( msg.Id == g_EventFinalMatchPlayerState )
	{
		SNtlEventFinalMatchPlayerState* pPlayerState = reinterpret_cast<SNtlEventFinalMatchPlayerState*>( msg.pData );
		SetPlayerState( pPlayerState->hPlayer );
	}
	else if( msg.Id == g_EventMinorMatchUpdateScore )
	{
		SNtlEventMinorMatchUpdateScore* pUpdateScore = reinterpret_cast<SNtlEventMinorMatchUpdateScore*>( msg.pData );

		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		DBO_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		stTBudokaiMember* pSlayer = pTBWorldConcept->FindMember( pUpdateScore->hSlayer );
		stTBudokaiMember* pFainter = pTBWorldConcept->FindMember( pUpdateScore->hFainter );

		GetAlarmManager()->FormattedAlarmMessage( "DST_BUDOKAI_MINOR_KILL_ANNOUNCE", FALSE, NULL, pSlayer->wstrName.c_str(), pFainter->wstrName.c_str() );
	}
	else if( msg.Id == g_EventBudokaiProgressMessageNfy )
	{
		SNtlEventBudokaiProgressMessageNfy* pNotify = reinterpret_cast< SNtlEventBudokaiProgressMessageNfy* >( msg.pData );

		ProgressMessageNotify( pNotify->byMsgId );
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pState = reinterpret_cast< SNtlEventWorldConceptState*>(msg.pData);

		if( pState->eWorldConcept == WORLD_PLAY_T_BUDOKAI )
		{
			if( pState->uiState == WORLD_STATE_IDLE )
			{
				// peessi: WORLD_STATE_ENTER 상태에서는 MINOR,MAJOR,FINAL 판별이 불가능. IDLE이 한번만 들어오기 때문에 이용. 
				CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
				DBO_ASSERT( pTBWorldConcept, "CTBGui::HandleEvents : must World concept is valid" );		
	
				if( pTBWorldConcept->GetMatchType() == MATCHWORLD_TYPE_MINOR_MATCH )			
				{
					if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_NEWS ) )
						GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_NEWS );

					if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_REQUEST ) )
						GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_REQUEST );

					if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_TOURNAMENT ) )
						GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_TOURNAMENT );					
				}
				else
				{
					DirectionMode( TRUE );
				}

				Create( pTBWorldConcept->GetMatchType() );			
			}
			else if( pState->uiState == WORLD_STATE_EXIT )
			{
				if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_TOURNAMENT ) )
					GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_TOURNAMENT );

				DirectionMode( FALSE );
				Destroy();
			}
		}
	}
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		SetCounterPosition( pData->iHeight );		
	}
	else if( msg.Id == g_EventSobCreate )
	{
		HandleEventSobCreate( msg );
	}
}

VOID CTBGui::HandleEventMinorMatchStateUpdateEntrance( RWS::CMsg& msg )
{
	SNtlEventMinorMatchStateUpdate* pStateUpdate = reinterpret_cast<SNtlEventMinorMatchStateUpdate*>( msg.pData );

	if( pStateUpdate->bEnterState )
	{
		if( pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_WAIT )
		{
			DirectionMode( TRUE );
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_READY )
		{
			DirectionMode( FALSE );
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_RUN )
		{
			SetBattleGauge();
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_FINISH )
		{
			UnsetBattleGauge();
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_MATCH_FINISH )
		{
			DirectionMode( TRUE );
		}			
	}	
}

VOID CTBGui::HandleEventMinorMatchStateUpdateObserver( RWS::CMsg& msg )
{
	SNtlEventMinorMatchStateUpdate* pStateUpdate = reinterpret_cast<SNtlEventMinorMatchStateUpdate*>( msg.pData );
	
	if( pStateUpdate->bEnterState )
	{
		if( pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_READY ||
			pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_RUN	||
			pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_FINISH )
		{
			DirectionMode( FALSE );

			if( pStateUpdate->byMatchState != BUDOKAI_MINORMATCH_STATE_STAGE_FINISH )
				SetBattleGauge();
			else
				UnsetBattleGauge();
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_MATCH_FINISH )
		{
			DirectionMode( TRUE );
		}	
	}	
}

VOID CTBGui::HandleEventMajorMatchStateUpdateEntrance( RWS::CMsg& msg )
{
	SNtlEventMajorMatchStateUpdate* pStateUpdate = reinterpret_cast<SNtlEventMajorMatchStateUpdate*>( msg.pData );

	if( pStateUpdate->bEnterState )
	{
		if( pStateUpdate->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_READY )
		{
			DirectionMode( FALSE );
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_RUN )
		{
			SetBattleGauge();
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH )
		{
			UnsetBattleGauge();
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MAJORMATCH_STATE_MATCH_FINISH )
		{
			DirectionMode( TRUE );
		}	
	}	
}

VOID CTBGui::HandleEventMajorMatchStateUpdateObserver( RWS::CMsg& msg )
{
	SNtlEventMajorMatchStateUpdate* pStateUpdate = reinterpret_cast<SNtlEventMajorMatchStateUpdate*>( msg.pData );

	if( pStateUpdate->bEnterState )
	{
		if( pStateUpdate->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_READY	||
			pStateUpdate->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_RUN	||
			pStateUpdate->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH)
		{
			DirectionMode( FALSE );

			// 불필요한 UI들을 닫는다.
			GetDialogManager()->CloseDialog( DIALOG_HP );
			GetDialogManager()->CloseDialog( DIALOG_TARGER_UI );
			GetDialogManager()->CloseDialog( DIALOG_TIME_NOTIFY );
			GetDialogManager()->CloseDialog( DIALOG_SIDEDIALOG_MANAGER );
			GetDialogManager()->CloseDialog( DIALOG_PARTYMENU );

			if( pStateUpdate->byMatchState != BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH )
				SetBattleGauge();
			else
				UnsetBattleGauge();
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MAJORMATCH_STATE_MATCH_FINISH )
		{
			DirectionMode( TRUE );
		}	
	}	
}

VOID CTBGui::HandleEventFinalMatchStateUpdateEntrance( RWS::CMsg& msg )
{
	SNtlEventFinalMatchStateUpdate* pStateUpdate = reinterpret_cast<SNtlEventFinalMatchStateUpdate*>( msg.pData );

	if( pStateUpdate->bEnterState )
	{
		if( pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_READY )
		{
			DirectionMode( FALSE );
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_RUN )
		{
			SetBattleGauge();
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_FINISH )
		{
			UnsetBattleGauge();
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_MATCH_FINISH )
		{
			DirectionMode( TRUE );
		}	
	}	
}

VOID CTBGui::HandleEventFinalMatchStateUpdateObserver( RWS::CMsg& msg )
{
	SNtlEventFinalMatchStateUpdate* pStateUpdate = reinterpret_cast<SNtlEventFinalMatchStateUpdate*>( msg.pData );

	if( pStateUpdate->bEnterState )
	{
		if( pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_READY	||
			pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_RUN	||
			pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_FINISH	)
		{
			DirectionMode( FALSE );

			// 불필요한 UI들을 닫는다.
			GetDialogManager()->CloseDialog( DIALOG_HP );
			GetDialogManager()->CloseDialog( DIALOG_TARGER_UI );
			GetDialogManager()->CloseDialog( DIALOG_TIME_NOTIFY );
			GetDialogManager()->CloseDialog( DIALOG_SIDEDIALOG_MANAGER );
			GetDialogManager()->CloseDialog( DIALOG_PARTYMENU );

			if( pStateUpdate->byMatchState != BUDOKAI_FINALMATCH_STATE_STAGE_FINISH )
				SetBattleGauge();
			else
				UnsetBattleGauge();
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_MATCH_FINISH )
		{
			DirectionMode( TRUE );
		}	

	}
}

/**
* \brief 객체가 소멸하고 다시 생성되었을 때 매치 중의 Battle Gauge를 다시 세팅해주기 위해 사용된다.
*/
VOID CTBGui::HandleEventSobCreate( RWS::CMsg& msg )
{
	SNtlEventSobBaseCreate* pEvent = reinterpret_cast<SNtlEventSobBaseCreate*>( msg.pData );

	if( GetNtlWorldConcept()->IsActivePlayConcept( WORLD_PLAY_T_BUDOKAI ) )
	{
		if( pEvent->uiClassId != SLCLASS_PLAYER )
			return;

		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		DBO_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );

		if( pTBWorldConcept->GetMatchState() == BUDOKAI_MINORMATCH_STATE_STAGE_RUN ||
			pTBWorldConcept->GetMatchState() == BUDOKAI_MAJORMATCH_STATE_STAGE_RUN ||
			pTBWorldConcept->GetMatchState() == BUDOKAI_FINALMATCH_STATE_STAGE_RUN )
		{
			stTBudokaiMember* pMember = pTBWorldConcept->FindMember( pEvent->hSerialId );

			if( pMember )
			{
				if( pMember->byState != MATCH_MEMBER_STATE_NOAPPLY )
				{
 					RwBool bMyTeam = FALSE;

					if( pMember->pTeam->wTeamType == pTBWorldConcept->GetMyTeamType() )
						bMyTeam = TRUE;

					CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( pMember->hSerial );

					if( pSobObj )
						GetBalloonManager()->AddBattleGauge( pSobObj, Logic_GetName( pSobObj ), Logic_GetMaxLp( pSobObj ), Logic_GetLp( pSobObj ), bMyTeam );
				}				
			}
		}	
	}
}

VOID CTBGui::SetPlayerState( RwUInt32 hSerial )
{
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	DBO_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

	stTBudokaiMember* pMemberData = pTBWorldConcept->FindMember( hSerial );

	if( pMemberData )
	{
		std::string nStringID;

		if( pTBWorldConcept->GetMatchType() == MATCHWORLD_TYPE_MINOR_MATCH &&
			GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() == hSerial )
		{
			switch( pMemberData->byState )
			{
			case MATCH_MEMBER_STATE_FAINT : nStringID = "DST_BUDOKAI_MINOR_WAIT_FOR_KO"; break; 
			case MATCH_MEMBER_STATE_OUTOFAREA : nStringID = "DST_BUDOKAI_MINOR_WAIT_FOR_RINGOUT"; break; 
			case MATCH_MEMBER_STATE_NORMAL : HideCounterTime(); return;
			default: return;
			}

			GetAlarmManager()->FormattedAlarmMessage( nStringID, FALSE, NULL, BUDOKAI_MINORMATCH_RESCUE_TIME );
			SetCounterTime( BUDOKAI_MINORMATCH_RESCUE_TIME );
		}
		else
		{
			switch( pMemberData->byState )
			{
			case MATCH_MEMBER_STATE_FAINT : nStringID = "DST_RANKBATTLE_MEMBER_FAINT"; break; 
			case MATCH_MEMBER_STATE_OUTOFAREA : nStringID = "DST_RANKBATTLE_MEMBER_OUTOFAREA"; break; 
			case MATCH_MEMBER_STATE_GIVEUP : nStringID = "DST_RANKBATTLE_MEMBER_GIVEUP"; break; 
			default: return;
			}

			GetAlarmManager()->FormattedAlarmMessage( nStringID, FALSE, NULL, pMemberData->wstrName.c_str() );
		}		
	}	
}

VOID CTBGui::SetBattleGauge(VOID)
{
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	DBO_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

	CNtlSob* pSobObj = NULL;

	MAP_TBMEMBER* pmapMembers = pTBWorldConcept->GetMembers(); 
	MAP_TBMEMBER_ITER it; 
 
	for( it = pmapMembers->begin() ; it != pmapMembers->end() ; ++it )
	{
		stTBudokaiMember* pMember = (*it).second;

		if( pMember->byState != MATCH_MEMBER_STATE_NOAPPLY )
		{
			RwBool bMyTeam = FALSE;

			if( pMember->pTeam->wTeamType == pTBWorldConcept->GetMyTeamType() )
				bMyTeam = TRUE;

			pSobObj = GetNtlSobManager()->GetSobObject( pMember->hSerial );

			if( pSobObj )
				GetBalloonManager()->AddBattleGauge( pSobObj, Logic_GetName( pSobObj ), Logic_GetMaxLp( pSobObj ), Logic_GetLp( pSobObj ), bMyTeam );
		}				
	}
}

VOID CTBGui::UnsetBattleGauge(VOID)
{
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	DBO_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

	CNtlSob* pSobObj = NULL;

	MAP_TBMEMBER* pmapMembers = pTBWorldConcept->GetMembers();
	MAP_TBMEMBER_ITER it;

	for( it = pmapMembers->begin() ; it != pmapMembers->end() ; ++it )
	{
		stTBudokaiMember* pMember = (*it).second;

		if( pMember->byState != MATCH_MEMBER_STATE_NOAPPLY )
		{
			pSobObj = GetNtlSobManager()->GetSobObject( pMember->hSerial );

			if( pSobObj )
				GetBalloonManager()->FadeOutBalloon( pSobObj, CBalloonGui::TYPE_BATTLEGAUGE );
		}				
	}
}

/**
* \brief 상황에 맞춘 Notify를 출력한다.
* \param byMsgId	(RwUInt8) NtlBudokai.h 에 정의되어 있는 상황들
*/
VOID CTBGui::ProgressMessageNotify( RwUInt8 byMsgId )
{
	switch( byMsgId )
	{
	case BUDOKAI_PROGRESS_MESSAGE_START:
		{
			CDboEventGenerator::TimeFixedNotifyMessage(TRUE, "DST_BUDOKAI_PROGRESS_MESSAGE_START",
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_START" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_RECEIVE_NEWS" ) );	
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_ENTERING:
		{
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_ENTERING" ) );	
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_ENTERING2" ) );	
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_BEFORE_1MIN:
		{
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_BEFORE_1MIN" ) );	
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_BEFORE_1MIN2" ) );	
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_WINNER:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_END" ), 
				Logic_GetAvatarName(), GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MINOR" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );

			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_WINNER" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_32" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );	
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_LOSER:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_END" ), 
				Logic_GetAvatarName(), GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MINOR" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );

			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_LOSER" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_32" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );	
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_ALL_FINISHED:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_ALL_FINISHED" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_REQUEST_MINOR" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );

			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_DECISION_NEXT" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_REQUEST_MAJOR" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_DEPTH_32_BEFORE_10SEC:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_BEFORE_10SEC" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_32" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_DEPTH_32_WINNER:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_END" ), 
				Logic_GetAvatarName(), GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_32" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );

			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_WINNER" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_16" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_DEPTH_32_LOSER:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_END" ), 
				Logic_GetAvatarName(), GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_32" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );

			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_LOSER" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_16" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_DEPTH_32_ALL_FINISHED:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_ALL_FINISHED" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_32" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );

			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_DECISION_NEXT" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_16" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_DEPTH_16_BEFORE_10SEC:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_BEFORE_10SEC" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_16" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_DEPTH_16_WINNER:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_END" ), 
				Logic_GetAvatarName(), GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_16" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );

			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_WINNER" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_8" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_DEPTH_16_LOSER:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_END" ), 
				Logic_GetAvatarName(), GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_16" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );

			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_LOSER" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_8" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_DEPTH_16_ALL_FINISHED:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_ALL_FINISHED" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_16" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );

			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_DECISION_NEXT" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_8" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_DEPTH_08_BEFORE_10SEC:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_BEFORE_10SEC" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_8" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_DEPTH_08_WINNER:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_END" ), 
				Logic_GetAvatarName(), GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_8" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );

			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_WINNER" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_SF" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );

		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_DEPTH_08_LOSER:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_END" ), 
				Logic_GetAvatarName(), GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_8" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );

			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_LOSER" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_SF" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_DEPTH_08_ALL_FINISHED:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_ALL_FINISHED" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_8" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );

			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_DECISION_NEXT" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_SF" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );
		}
		break;
	case BUDOKAI_PROGRESS_MESSAGE_SEMI_FINAL_BEFORE_10SEC:
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_BEFORE_10SEC" ), 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_SF" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, awcBuffer );
		}
		break;
	case BUDOkAI_PROGRESS_MESSAGE_SEMI_FINAL_RUNNING:
		{
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_SEMI_FINAL_RUNNING" ) );
		}
		break;
	case BUDOkAI_PROGRESS_MESSAGE_FINAL_WAIT_LOSER:
		{
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_FINAL_WAIT_LOSER" ) );
			CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_FINAL_WAIT_LOSER1" ) );
		}
		break;
	case BUDOkAI_PROGRESS_MESSAGE_END:
		{
			CDboEventGenerator::TimeFixedNotifyMessage(TRUE, "DST_BUDOKAI_PROGRESS_MESSAGE_END",
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_END" ) );
		}
		break;
	default:
		break;
	}
}

VOID CTBGui::DirectionMode( RwBool bDirection )
{
	if( bDirection )
	{
		if( !GetDialogManager()->IsMode( DIALOGMODE_NARRATION ) )
		{
			GetDialogManager()->OnMode( DIALOGMODE_NARRATION );
			GetNtlWorldConcept()->AddWorldPlayConcept( WORLD_PLAY_DIRECT );
		}		
	}
	else
	{
		if( GetDialogManager()->IsMode( DIALOGMODE_NARRATION ) )
		{
			GetDialogManager()->OffMode();
			GetDialogManager()->OpenGMDialog();
			GetDialogManager()->OpenDefaultDialog();
			GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_DIRECT );
		}
	}
}

VOID CTBGui::SetCounterPosition( RwInt32 nScreenHeight )
{
	m_pCounter->SetPosition( 5, nScreenHeight - 125 );
}

VOID CTBGui::SetCounterTime( RwReal fTime )
{
	m_pCounter->SetTime( fTime );
	m_pCounter->Pause( FALSE );
	m_pCounter->Show( true );
	SetCounterPosition( GetDboGlobal()->GetScreenHeight() );
}

VOID CTBGui::HideCounterTime(VOID)
{
	m_pCounter->Pause( TRUE );
	m_pCounter->Show( false );
}