#include "precomp_dboclient.h"
#include "RBGui.h"

// core
#include "NtlDebug.h"

// shared
#include "NtlRankBattle.h"

// presentation
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"
#include "NtlRankBattle.h"
#include "NtlSobManager.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlWorldConceptRB.h"

// Client
#include "DboGlobal.h"
#include "RBResultGui.h"
#include "RBBattleInfoGui.h"
#include "RBDirectionGui.h"
#include "DialogManager.h"
#include "SideDialogManager.h"
#include "ChatBalloonManager.h"
#include "AlarmManager.h"
#include "CounterGui.h"
#include "ResultNarrationGui.h"

CRBGui* CRBGui::m_pInstance = NULL;

CRBGui::CRBGui(VOID)
: m_pResultGui( NULL )
{
	m_pResultGui = NULL;
	m_pBattleInfoGui = NULL;
	m_pDirectionGui = NULL;
	m_pCounter = NULL;
	m_pResultNarration = NULL;

	m_bCreate = FALSE;
}

VOID CRBGui::CreateInstance(VOID)
{
	if( !m_pInstance )
	{
		m_pInstance = NTL_NEW CRBGui;
		NTL_ASSERT( m_pInstance, "CRBGui::CreateInstance : Instance allocate fale" );				

		m_pInstance->LinkMsg( g_EventRBBattlePlayerState, 0, 0x8100 );	
		m_pInstance->LinkMsg( g_EventRBBattleStateUpdate, 0, 0x8100 );
		m_pInstance->LinkMsg( g_EventChangeWorldConceptState );
	}
}

VOID CRBGui::DeleteInstance(VOID)
{
	if( m_pInstance )
	{
		m_pInstance->UnLinkMsg( g_EventRBBattlePlayerState );
		m_pInstance->UnLinkMsg( g_EventRBBattleStateUpdate );
		m_pInstance->UnLinkMsg( g_EventChangeWorldConceptState );

		m_pInstance->Destroy();
		NTL_DELETE( m_pInstance );
	}
}

VOID CRBGui::Create(VOID)
{
	RwBool bFail = FALSE;

	m_pResultGui = NTL_NEW CRBResultGui( "RBResultGui" );
	if( !m_pResultGui->Create() )
	{
		m_pResultGui->Destroy();
		NTL_DELETE( m_pResultGui );
		bFail = TRUE;
	}
	CNtlPLGuiManager::GetInstance()->AddGui( m_pResultGui );

	m_pBattleInfoGui = NTL_NEW CRBBattleInfoGui( "RBBattleInfoGui" );
	if( !m_pBattleInfoGui->Create() )
	{
		m_pBattleInfoGui->Destroy();
		NTL_DELETE( m_pBattleInfoGui );
		bFail = TRUE;
	}
	CNtlPLGuiManager::GetInstance()->AddGui( m_pBattleInfoGui );
	GetSideDialogManager()->RegistDialog( SDIALOG_RANKBATTLE_INFO, m_pBattleInfoGui, &CRBBattleInfoGui::SwitchDialog );

	m_pDirectionGui = NTL_NEW CRBDirectionGui( "RBDirectionGui" );
	if( !m_pDirectionGui->Create() )
	{
		m_pDirectionGui->Destroy();
		NTL_DELETE( m_pDirectionGui );
		bFail = TRUE;
	}
	CNtlPLGuiManager::GetInstance()->AddGui( m_pDirectionGui );

	m_pCounter = NTL_NEW CCounterGui( "RBCounter" );
	if( !m_pCounter->Create( COUNTER_DISPLAY_2, 0.0f ) )
	{
		m_pCounter->Destroy();
		NTL_DELETE( m_pCounter );
		bFail = TRUE;
	}
	CNtlPLGuiManager::GetInstance()->AddGui( m_pCounter );
	m_pCounter->Show( false );
	m_pCounter->Pause( TRUE );

	m_pResultNarration = NTL_NEW CResultNarrationGui( "RBResultNarration" );
	if( !m_pResultNarration->Create() )
	{
		m_pResultNarration->Destroy();
		NTL_DELETE( m_pResultNarration );
	}
	CNtlPLGuiManager::GetInstance()->AddGui( m_pResultNarration );

	if( bFail )
		NTL_ASSERTFAIL( "CRBGui::Create : Failed to create RBGui" );	

	//LinkMsg( g_EventSobInfoUpdate );

	m_bCreate = TRUE;
}

VOID CRBGui::Destroy(VOID)
{
	if( m_bCreate )
	{
		//UnLinkMsg( g_EventSobInfoUpdate );
	}

	if( m_pResultGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pResultGui );
		m_pResultGui->Destroy();
		NTL_DELETE( m_pResultGui );
	}	

	if( m_pBattleInfoGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pBattleInfoGui );
		GetSideDialogManager()->UnRegistDialog( SDIALOG_RANKBATTLE_INFO );
		m_pBattleInfoGui->Destroy();
		NTL_DELETE( m_pBattleInfoGui );
	}

	if( m_pDirectionGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pDirectionGui );
		m_pDirectionGui->Destroy();
		NTL_DELETE( m_pDirectionGui );
	}	

	if( m_pCounter )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pCounter );
		m_pCounter->Destroy();
		NTL_DELETE( m_pCounter );
	}

	if( m_pResultNarration )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pResultNarration );
		m_pResultNarration->Destroy();
		NTL_DELETE( m_pResultNarration );
	}

	m_bCreate = FALSE;	
}

CRBGui* CRBGui::GetInstance(VOID)
{
	return m_pInstance;
}

VOID CRBGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventRBBattleStateUpdate )
	{
		SNtlEventRBBattleStateUpdate* pData = reinterpret_cast<SNtlEventRBBattleStateUpdate*>( msg.pData );

		if (pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_PREPARE)
		{
			DirectionMode(FALSE);
		}
		else if( pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_READY || pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_RUN)
		{
			CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
			CNtlSob* pSobObj = NULL;

			CNtlRankBattle::MAP_RBMEMBER* pMap = pRankBattle->GetMyTeamMap();
			CNtlRankBattle::MAP_RBMEMBER_ITER it;

			for( it = pMap->begin() ; it != pMap->end() ; ++it )
			{
				if( (*it).second.byState != RANKBATTLE_MEMBER_STATE_NOAPPLY )
				{
					pSobObj = GetNtlSobManager()->GetSobObject( (*it).second.hSerial );
					if( pSobObj )
						GetBalloonManager()->AddBattleGauge( pSobObj, Logic_GetName( pSobObj ), Logic_GetMaxLp( pSobObj ), Logic_GetLp( pSobObj ), TRUE );
				}				
			}

			pMap = pRankBattle->GetEnemyTeamMap();
			
			for( it = pMap->begin() ; it != pMap->end() ; ++it )
			{
				if( (*it).second.byState != RANKBATTLE_MEMBER_STATE_NOAPPLY )
				{
					pSobObj = GetNtlSobManager()->GetSobObject( (*it).second.hSerial );
					if( pSobObj )
						GetBalloonManager()->AddBattleGauge( pSobObj, Logic_GetName( pSobObj ), Logic_GetMaxLp( pSobObj ), Logic_GetLp( pSobObj ), FALSE );
				}
			}			
		}
		else if( pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_FINISH)
		{
			CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
			CNtlSob* pSobObj = NULL;

			CNtlRankBattle::MAP_RBMEMBER* pMap = pRankBattle->GetMyTeamMap();
			CNtlRankBattle::MAP_RBMEMBER_ITER it;

			for( it = pMap->begin() ; it != pMap->end() ; ++it )
			{
				if( (*it).second.byState != RANKBATTLE_MEMBER_STATE_NOAPPLY )
				{
					pSobObj = GetNtlSobManager()->GetSobObject( (*it).second.hSerial );
					if( pSobObj )
						GetBalloonManager()->FadeOutBalloon( pSobObj, CBalloonGui::TYPE_BATTLEGAUGE );				
				}
			}

			pMap = pRankBattle->GetEnemyTeamMap();

			for( it = pMap->begin() ; it != pMap->end() ; ++it )
			{
				if( (*it).second.byState != RANKBATTLE_MEMBER_STATE_NOAPPLY )
				{
					pSobObj = GetNtlSobManager()->GetSobObject( (*it).second.hSerial );
					if( pSobObj )
						GetBalloonManager()->FadeOutBalloon( pSobObj, CBalloonGui::TYPE_BATTLEGAUGE );
				}
			}						
		}
		else if( pData->byBattleState == RANKBATTLE_BATTLESTATE_MATCH_FINISH)
		{
			DirectionMode( TRUE );
		}
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
	else if( msg.Id == g_EventRBBattlePlayerState )
	{
		SNtlEventRBBattlePlayerState* pPlayerState = reinterpret_cast<SNtlEventRBBattlePlayerState*>( msg.pData );
		CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
		CNtlRankBattle::stRankBattleMember* pMemberData = pRankBattle->FindMemberData( pPlayerState->hSerial );
		
		if( pMemberData )
		{
			pMemberData->byState = pPlayerState->byState;

			std::string nStringID;
			switch( pPlayerState->byState )
			{
			case RANKBATTLE_MEMBER_STATE_FAINT : nStringID = "DST_RANKBATTLE_MEMBER_FAINT"; break; 
			case RANKBATTLE_MEMBER_STATE_OUTOFAREA : nStringID = "DST_RANKBATTLE_MEMBER_OUTOFAREA"; break; 
			case RANKBATTLE_MEMBER_STATE_GIVEUP : nStringID = "DST_RANKBATTLE_MEMBER_GIVEUP"; break; 
			default: return;
			}

			GetAlarmManager()->FormattedAlarmMessage( nStringID, FALSE, NULL, pMemberData->wstrName.c_str() );		
		}		
		else
		{
			
		}
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pState = reinterpret_cast< SNtlEventWorldConceptState*>( msg.pData );

		if( pState->eWorldConcept == WORLD_PLAY_RANK_BATTLE )
		{
			if( pState->uiState == WORLD_STATE_ENTER )
			{
				// UI 감추기
				GetDialogManager()->CloseDialog( DIALOG_WORLDMAP );
				GetDialogManager()->CloseDialog( DIALOG_MINIMAP );

				// RB UI 생성
				Create();

				DirectionMode( TRUE );				
			}
			else if( pState->uiState == WORLD_STATE_EXIT )
			{
				DirectionMode( FALSE );
				Destroy();
			}
		}
	}
	//else if( msg.Id == g_EventSobCreate ) // Dummy코드, 캐릭터 부활시 없어지는 게이지를 살리기 찍기 위함.
	//{
	//	SNtlEventSobPlayerCreate* pData = reinterpret_cast<SNtlEventSobPlayerCreate*>( msg.pData );

	//	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
	//	CNtlSob* pSobObj = NULL;

	//	CNtlRankBattle::MAP_RBMEMBER* pMap = pRankBattle->GetMyTeamMap();
	//	CNtlRankBattle::MAP_RBMEMBER_ITER it;

	//	it = pMap->find( pData->hSerialId );
	//	if( it != pMap->end() )
	//	{
	//		pSobObj = reinterpret_cast<CNtlSob*>( pData->pSobObj );
	//		if( pSobObj )
	//			GetBalloonManager()->AddBattleGauge( pSobObj, Logic_GetName( pSobObj ), Logic_GetMaxLp( pSobObj ), Logic_GetLp( pSobObj ), TRUE );

	//		return;
	//	}

	//	pMap = pRankBattle->GetEnemyTeamMap();

	//	it = pMap->find( pData->hSerialId );
	//	if( it != pMap->end() )
	//	{
	//		pSobObj = reinterpret_cast<CNtlSob*>( pData->pSobObj );
	//		if( pSobObj )
	//			GetBalloonManager()->AddBattleGauge( pSobObj, Logic_GetName( pSobObj ), Logic_GetMaxLp( pSobObj ), Logic_GetLp( pSobObj ), TRUE );

	//		return;
	//	}
	//}
}

VOID CRBGui::SetCounterPosition( RwInt32 nScreenHeight )
{
	m_pCounter->SetPosition( 5, nScreenHeight - 125 );
}

VOID CRBGui::SetCounterTime( RwReal fTime )
{
	m_pCounter->SetTime( fTime );
	m_pCounter->Pause( FALSE );
	m_pCounter->Show( true );
	SetCounterPosition( GetDboGlobal()->GetScreenHeight() );
}

VOID CRBGui::HideCounterTime(VOID)
{
	m_pCounter->Pause( TRUE );
	m_pCounter->Show( false );
}

VOID CRBGui::DirectionMode( RwBool bDirection )
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