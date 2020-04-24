#include "precomp_dboclient.h"
#include "TBMajorDirectionGui.h"

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

CTBMajorDirectionGui::CTBMajorDirectionGui( const RwChar* pName )
: CTBMainCommonDirectionGui( pName )
{
	
}

CTBMajorDirectionGui::~CTBMajorDirectionGui(VOID)
{

}

RwBool CTBMajorDirectionGui::Create(VOID)
{
	NTL_FUNCTION( "CTBMajorDirectionGui::Create" );

	if( !CNtlPLGui::Create( "", "", "gui\\BudokaiMajorDirection.frm" ) )
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

	LinkMsg( g_EventMajorMatchStageFinish );
	LinkMsg( g_EventMajorMatchMatchFinish );
	LinkMsg( g_EventMajorMatchStateUpdate );
	LinkMsg( g_EventMajorMatchTeamInfo );
	LinkMsg( g_EventCinematicGuiCommand );
	LinkMsg( g_EventResize );

	ReCalcPosition( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );
	Show( true );

	// InitSetting
	m_stScore.LoadFlash( GetTBFlashName()->GetNormalCounter() );

	NTL_RETURN( TRUE );
}

VOID CTBMajorDirectionGui::Destroy(VOID)
{
	m_stOneLine.Destroy();
	m_stTwoLine.Destroy();
	m_stScore.Destroy();
	m_stBlueMember.Destroy();
	m_stRedMember.Destroy();	

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventMajorMatchMatchFinish );
	UnLinkMsg( g_EventMajorMatchStageFinish );
	UnLinkMsg( g_EventMajorMatchStateUpdate );
	UnLinkMsg( g_EventMajorMatchTeamInfo );
	UnLinkMsg( g_EventCinematicGuiCommand );
	UnLinkMsg( g_EventResize );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CTBMajorDirectionGui::Update( RwReal fElapsed )
{
	CTBMainCommonDirectionGui::Update( fElapsed );
}

VOID CTBMajorDirectionGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventMajorMatchStateUpdate )
	{
		SNtlEventMajorMatchStateUpdate* pData = reinterpret_cast<SNtlEventMajorMatchStateUpdate*>( msg.pData );

		if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_WAIT )
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
			NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

			if( pData->bEnterState )
			{
				m_stOneLine.LoadFlash( GetTBFlashName()->GetDepth( pTBWorldConcept->GetMatchDepth() ), TITLE );
				m_stOneLine.StartMovie();
			}			
			else
			{
				m_stOneLine.EndMovie();
			}

			//m_stOneLine.EndMovie();
			//m_stTwoLine.LoadFlash( GetTBFlashName()->GetTitle(), GetTBFlashName()->GetDepth( pTBWorldConcept->GetMatchDepth() ), TITLE, TRUE );
			//m_stTwoLine.StartFirstMovie();

			//m_slotTitleMovieEnd = m_stTwoLine.pflaSecond->SigMovieEnd().Connect( this, &CTBMajorDirectionGui::OnTitleMovieEnd );			
		}
		else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_DIRECTION )
		{
			if( pData->bEnterState )
			{
				CNtlLuaState* pLuaState = GetLuaState();
				pLuaState->RunString( GetTBFlashName()->GetLuaPreFinal() );

				// 이름 입력
				CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
				NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

				m_stBlueMember.pMemberInfo->SetBluePos();
				m_stRedMember.pMemberInfo->SetRedPos();				

				m_stBlueMember.LoadFlash( GetTBFlashName()->GetNormalBlueTitle() );
				m_stRedMember.LoadFlash( GetTBFlashName()->GetNormalRedTitle() );
			}
		}		
		//else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_MATCH_READY )
		//{
		//	Enable( false );
		//}
		else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_READY )
		{
			if( pData->bEnterState )
			{
				m_stTwoLine.LoadFlash( GetTBFlashName()->GetNormalRound( pData->byStage +1 ), GetTBFlashName()->GetNormalRoundStart(), ROUNDSTART, FALSE );
				m_stTwoLine.StartFirstMovie();
			}			
		}
		else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_RUN )
		{
			if( pData->bEnterState )
				m_stTwoLine.StartSecondMovie();
		}
		else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH )
		{
			if( !pData->bEnterState )
				m_stScore.EndMovie();			
		}
		else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_MATCH_FINISH )
		{
						
		}
		else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_END )
		{
			//m_stOneLine.pflaFirst->SigMovieEnd().Disconnect( m_slotWinLoseMovieEnd );
		}
	}
	else if( msg.Id == g_EventMajorMatchStageFinish )
	{
		SNtlEventMajorMatchStageFinish* pData = reinterpret_cast<SNtlEventMajorMatchStageFinish*>( msg.pData );
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CTBMajorDirectionGui::HandleEvents : must World concept is valid" );		

		if( !pData->bIsObserver )
		{
			m_stScore.StartMovie();
			m_stScore.SetScore( pData->pStageScore->byTeam1, pData->pStageScore->byTeam2 );		
		}
	}
	//else if( msg.Id == g_EventMajorMatchMatchFinish )
	//{
	//	SNtlEventMajorMatchMatchFinish* pData = reinterpret_cast<SNtlEventMajorMatchMatchFinish*>( msg.pData );
	//	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	//	NTL_ASSERT( pTBWorldConcept, "CTBMajorDirectionGui::HandleEvents : must World concept is valid" );		

	//	if( pData->byMatchResult == MATCH_RESULT_WIN )
	//	{
	//		stTBudokaiTeam* pMyTeam = pTBWorldConcept->GetMyTeam();

	//		if( !pMyTeam )
	//			return;

	//		if( pData->wMatchWinner == pMyTeam->wTeamType )
	//			m_stOneLine.LoadFlash( GetTBFlashName()->GetWin(), WINLOSE );			
	//		else
	//			m_stOneLine.LoadFlash( GetTBFlashName()->GetLose(), WINLOSE );			
	//	}
	//	else
	//		m_stOneLine.LoadFlash( GetTBFlashName()->GetDraw(), WINLOSE );			

	//	m_stOneLine.StartMovie();
	//	m_slotWinLoseMovieEnd = m_stOneLine.pflaFirst->SigMovieEnd().Connect( this, &CTBMajorDirectionGui::OnWinLoseMovieEnd );
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
	else if( msg.Id == g_EventMajorMatchTeamInfo )
	{
		// 팀 정보를 구축
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );				

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

VOID CTBMajorDirectionGui::ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	CTBMainCommonDirectionGui::ReCalcPosition( nScreenWidth, nScreenHeight );
}

//VOID CTBMajorDirectionGui::OnTitleMovieEnd( gui::CComponent* pComponent )
//{
//	//m_stScroll.SetHtmlText( "" );
//	//m_stScroll.StartMovie();
//}

//VOID CTBMajorDirectionGui::OnWinLoseMovieEnd( gui::CComponent* pComponent )
//{
//	Show( false );
//
//	CDboEventGenerator::FlashFinishNotify( 0, 0, 0, "MajorMatchResult", NULL );
//}
