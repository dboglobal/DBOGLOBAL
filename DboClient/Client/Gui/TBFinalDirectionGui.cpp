#include "precomp_dboclient.h"
#include "TBFinalDirectionGui.h"

// core
#include "NtlDebug.h"
#include "NtlLuaState.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlWorldConceptTB.h"
#include "NtlCameraManager.h"
#include "NtlSLLuaGlue.h"

// client
#include "DboGlobal.h"

CTBFinalDirectionGui::CTBFinalDirectionGui( const RwChar* pName )
: CTBMainCommonDirectionGui( pName ), m_fTempAwardBeginTime( 0.0f )
{
	
}

CTBFinalDirectionGui::~CTBFinalDirectionGui(VOID)
{

}

RwBool CTBFinalDirectionGui::Create(VOID)
{
	NTL_FUNCTION( "CTBFinalDirectionGui::Create" );

	if( !CNtlPLGui::Create( "", "", "gui\\BudokaiFinalDirection.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain" ) );

	m_stOneLine.Create( reinterpret_cast<gui::CFlash*>( GetComponent( "flaOne" ) ) );
	m_stTwoLine.Create( reinterpret_cast<gui::CFlash*>( GetComponent( "flaFirst" ) ), reinterpret_cast<gui::CFlash*>( GetComponent( "flaSecond" ) ) ); 
	//m_stScroll.Create( reinterpret_cast<gui::CFlash*>( GetComponent( "flaScroll" ) ), reinterpret_cast<gui::CHtmlBox*>( GetComponent( "hbxText" ) ) );
	m_stBlueMember.Create( reinterpret_cast<gui::CFlash*>( GetComponent( "flaBlueTeam" ) ) );
	m_stRedMember.Create( reinterpret_cast<gui::CFlash*>( GetComponent( "flaRedTeam" ) ) );
	m_stScore.Create( reinterpret_cast<gui::CFlash*>( GetComponent( "flaScore" ) ) );

	GetNtlGuiManager()->AddUpdateFunc( this );

	LinkMsg( g_EventFinalMatchStageFinish );
	//LinkMsg( g_EventFinalMatchMatchFinish );
	LinkMsg( g_EventFinalMatchStateUpdate );
	LinkMsg( g_EventFinalMatchTeamInfo );
	LinkMsg( g_EventCinematicGuiCommand );
	LinkMsg( g_EventResize );

	ReCalcPosition( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );
	Show( true );

	// InitSetting
	m_stScore.LoadFlash( GetTBFlashName()->GetNormalCounter() );

	NTL_RETURN( TRUE );
}

VOID CTBFinalDirectionGui::Destroy(VOID)
{
	m_stOneLine.Destroy();
	m_stTwoLine.Destroy();
	m_stScore.Destroy();
	m_stBlueMember.Destroy();
	m_stRedMember.Destroy();
	m_stScore.Destroy();

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventFinalMatchStageFinish );
	//UnLinkMsg( g_EventFinalMatchMatchFinish );
	UnLinkMsg( g_EventFinalMatchStateUpdate );
	UnLinkMsg( g_EventFinalMatchTeamInfo );
	UnLinkMsg( g_EventCinematicGuiCommand );
	UnLinkMsg( g_EventResize );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CTBFinalDirectionGui::Update( RwReal fElapsed )
{
	CTBMainCommonDirectionGui::Update( fElapsed );

	//
	if( m_fTempAwardBeginTime > 0.0f )
	{
		m_fTempAwardBeginTime -= fElapsed;

		if( m_fTempAwardBeginTime <= 0.0f )
		{
			CNtlLuaState* pLuaState = GetLuaState();
			pLuaState->RunString( GetTBFlashName()->GetLuaAward() );
		}
	}
	//
}

VOID CTBFinalDirectionGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventFinalMatchStateUpdate )
	{
		SNtlEventFinalMatchStateUpdate* pData = reinterpret_cast<SNtlEventFinalMatchStateUpdate*>( msg.pData );

		if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_WAIT )
		{
			if( pData->bEnterState )
			{
				m_stOneLine.LoadFlash( GetTBFlashName()->GetWait(), WAIT );
				m_stOneLine.StartMovie();
			}
			else
			{
				m_stOneLine.EndMovie();
			}
		}
		else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_MATCH_READY )
		{
			CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
			NTL_ASSERT( pTBWorldConcept, "CTBFinalDirectionGui::HandleEvents : must World concept is valid" );		

			if( pData->bEnterState )
			{
				if( pTBWorldConcept->GetMatchDepth() == BUDOKAI_MATCH_DEPTH_2 )
				{
					m_stOneLine.LoadFlash( GetTBFlashName()->GetDepth( pTBWorldConcept->GetMatchDepth() ), FINALTITLE );
					m_stOneLine.StartMovie();		
				}
				else
				{
					m_stOneLine.LoadFlash( GetTBFlashName()->GetDepth( pTBWorldConcept->GetMatchDepth() ), TITLE );
					m_stOneLine.StartMovie();
				}			
			}			
			else
			{
				m_stOneLine.EndMovie();
			}
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_DIRECTION )
		{
			CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
			NTL_ASSERT( pTBWorldConcept, "CTBFinalDirectionGui::HandleEvents : must World concept is valid" );		

			if( pData->bEnterState )
			{
				m_stBlueMember.pMemberInfo->SetBluePos();
				m_stRedMember.pMemberInfo->SetRedPos();

				m_stBlueMember.LoadFlash( GetTBFlashName()->GetNormalBlueTitle() );
				m_stRedMember.LoadFlash( GetTBFlashName()->GetNormalRedTitle() );

				CNtlLuaState* pLuaState = GetLuaState();
				pLuaState->RunString( GetTBFlashName()->GetLuaPreFinal() );			
			}			
		}				
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_READY )
		{
			CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
			NTL_ASSERT( pTBWorldConcept, "CTBFinalDirectionGui::HandleEvents : must World concept is valid" );		

			if( pData->bEnterState )
			{
				if( pTBWorldConcept->GetMatchDepth() == 0 )
					m_stTwoLine.LoadFlash( GetTBFlashName()->GetSpecialRound( pData->byStage + 1 ), GetTBFlashName()->GetSpecialRoundStart(), ROUNDSTART, FALSE );
				else
					m_stTwoLine.LoadFlash( GetTBFlashName()->GetNormalRound( pData->byStage + 1 ), GetTBFlashName()->GetNormalRoundStart(), ROUNDSTART, FALSE );

				m_stTwoLine.StartFirstMovie();				
			}			
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_RUN )
		{
			if( pData->bEnterState )
				m_stTwoLine.StartSecondMovie();
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_FINISH )
		{
			if( !pData->bEnterState )
				m_stScore.EndMovie();
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_MATCH_FINISH )
		{
			
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_FINALDIRECTION )
		{
			CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
			NTL_ASSERT( pTBWorldConcept, "CTBFinalDirectionGui::HandleEvents : must World concept is valid" );	

			if( pData->bEnterState )
			{
				m_stBlueMember.pMemberInfo->SetBluePos();
				m_stRedMember.pMemberInfo->SetRedPos();

				m_stBlueMember.LoadFlash( GetTBFlashName()->GetSpecialBlueTitle() );
				m_stRedMember.LoadFlash( GetTBFlashName()->GetSpecialRedTitle() );

				CNtlLuaState* pLuaState = GetLuaState();
				pLuaState->RunString( GetTBFlashName()->GetLuaFinal() );				

				// 미리 로딩
				m_stScore.LoadFlash( GetTBFlashName()->GetSpecialCounter() );
			}			
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_AWARDING )
		{
			//m_stOneLine.pflaFirst->SigMovieEnd().Disconnect( m_slotWinLoseMovieEnd );	
			// 텔레포트 타이밍과 안맞아서 시간 지연후(2초) 출력.
			//CNtlLuaState* pLuaState = GetLuaState();
			//pLuaState->RunString( GetTBFlashName()->GetLuaAward() );
			if( pData->bEnterState )
				m_fTempAwardBeginTime = 2.0f;
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_END )
		{
			if( pData->bEnterState )
				Show( false );
		}
	}
	else if( msg.Id == g_EventFinalMatchStageFinish )
	{
		SNtlEventFinalMatchStageFinish* pData = reinterpret_cast<SNtlEventFinalMatchStageFinish*>( msg.pData );
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CTBFinalDirectionGui::HandleEvents : must World concept is valid" );		

		if( !pData->bIsObserver )
		{
			m_stScore.StartMovie();
			m_stScore.SetScore( pData->pStageScore->byTeam1, pData->pStageScore->byTeam2 );		
		}
	}
	//else if( msg.Id == g_EventFinalMatchMatchFinish )
	//{
		//SNtlEventFinalMatchMatchFinish* pData = reinterpret_cast<SNtlEventFinalMatchMatchFinish*>( msg.pData );
		//CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		//NTL_ASSERT( pTBWorldConcept, "CTBFinalDirectionGui::HandleEvents : must World concept is valid" );		

		//stTBudokaiTeam* pMyTeam = pTBWorldConcept->GetMyTeam();
		//if( !pMyTeam )
		//	return;

		//if( pData->byMatchResult == MATCH_RESULT_WIN )
		//{
		//	if( pData->wMatchWinner == pMyTeam->wTeamType )
		//		m_stOneLine.LoadFlash( GetTBFlashName()->GetWin(), WINLOSE );			
		//	else
		//		m_stOneLine.LoadFlash( GetTBFlashName()->GetLose(), WINLOSE );						
		//}
		//else
		//	m_stOneLine.LoadFlash( GetTBFlashName()->GetDraw(), WINLOSE );			

		//m_stOneLine.StartMovie();
		//m_slotWinLoseMovieEnd = m_stOneLine.pflaFirst->SigMovieEnd().Connect( this, &CTBFinalDirectionGui::OnWinLoseMovieEnd );
	//}
	else if( msg.Id == g_EventCinematicGuiCommand )
	{
		SNtlEventCinematicGuiCommand* pData = reinterpret_cast<SNtlEventCinematicGuiCommand*>( msg.pData );

		if( !strcmp( pData->szCommand, GetTBFlashName()->GetLuaBlueTeamInfoCallBack() ) )
		{
			if( pData->bShow )
				m_stBlueMember.StartMovie();
			else
				m_stBlueMember.pflaTeam->Invoke( "CloseScroll", "" );
		}
		else if( !strcmp( pData->szCommand, GetTBFlashName()->GetLuaRedTeamInfoCallBack() ) )
		{
			if( pData->bShow )
				m_stRedMember.StartMovie();
			else
				m_stRedMember.pflaTeam->Invoke( "CloseScroll", "" );
		}

	}
	else if( msg.Id == g_EventFinalMatchTeamInfo )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CTBFinalDirectionGui::HandleEvents : must World concept is valid" );	

		if( pTBWorldConcept->IsPartyBattle() )
		{
			stPARTYINFO* pBlueInfo = NTL_NEW stPARTYINFO;
			stPARTYINFO* pRedInfo = NTL_NEW stPARTYINFO;

			pBlueInfo->Create( m_stBlueMember.pflaTeam );
			pRedInfo->Create( m_stRedMember.pflaTeam );
			m_stBlueMember.SetMemberInfo( pBlueInfo );
			m_stRedMember.SetMemberInfo( pRedInfo );
		}
		else
		{
			stINDIVIDUALINFO* pBlueInfo = NTL_NEW stINDIVIDUALINFO;
			stINDIVIDUALINFO* pRedInfo = NTL_NEW stINDIVIDUALINFO;

			pBlueInfo->Create( m_stBlueMember.pflaTeam );
			pRedInfo->Create( m_stRedMember.pflaTeam );
			m_stBlueMember.SetMemberInfo( pBlueInfo );
			m_stRedMember.SetMemberInfo( pRedInfo );
		}
	}
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		ReCalcPosition( pData->iWidht, pData->iHeight );
	}
}

VOID CTBFinalDirectionGui::ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	CTBMainCommonDirectionGui::ReCalcPosition( nScreenWidth, nScreenHeight );
}

//VOID CTBFinalDirectionGui::OnTitleMovieEnd( gui::CComponent* pComponent )
//{
//	//m_stScroll.SetHtmlText( "" );
//	m_stScroll.StartMovie();
//}
//
//VOID CTBFinalDirectionGui::OnFinalTitleMovieEnd( gui::CComponent* pComponent )
//{
//	CNtlLuaState* pLuaState = GetLuaState();
//	pLuaState->RunString( GetTBFlashName()->GetLuaFinal() );	
//}
//
//VOID CTBFinalDirectionGui::OnWinLoseMovieEnd( gui::CComponent* pComponent )
//{
//	CDboEventGenerator::FlashFinishNotify( 0, 0, 0, "FinalMatchResult", NULL );
//}