#include "precomp_dboclient.h"
#include "TBMatchResultGui.h"

// core
#include "NtlDebug.h"

// share

// table
#include "ItemTable.h"
#include "GraphicDataTable.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlWorldConceptTB.h"

// client
#include "DboGlobal.h"
#include "DboEvent.h"
#include "InfoWndManager.h"
#include "DialogManager.h"
#include "TBGui.h"
#include "ResultNarrationGui.h"
#include "CounterGui.h"

#define TEAMNAME_LINE_BLUE_TEXT_X	652
#define TEAMNAME_LINE_RED_TEXT_X	131
#define TEAMNAME_LINE_TEXT_Y		261
#define TEAMNAME_LINE_WIDTH			245
#define TEAMNAME_LINE_HEIGHT		22

#define MATCHRESULT_BLUE_NUMBER_X	652
#define MATCHRESULT_RED_NUMBER_X	376
#define MATCHRESULT_NUMBER_Y		299

#define MATCHSCORE_BLUE_NUMBER_X	652
#define MATCHSCORE_RED_NUMBER_X		376
#define MATCHSCORE_NUMBER_Y			341

#define MATCHBONUS_BLUE_NUMBER_X	652
#define MATCHBONUS_RED_NUMBER_X		376
#define MATCHBONUS_NUMBER_Y			381

#define MATCHREWARD_BLUE_ICON_X		657
#define MATCHREWARD_RED_ICON_X		341
#define MATCHREWARD_ICON_Y			415

#define MATCHINFO_LINE_BLUE_TEXT_X	652
#define MATCHINFO_LINE_RED_TEXT_X	131
#define MATCHINFO_LINE_WIDTH		245
#define MATCHINFO_LINE_HEIGHT		22

#define MATCHINFO_LINE1_TEXT_Y		460
#define MATCHINFO_LINE2_TEXT_Y		489

#define COLOR_REDTEAM_RED			255
#define COLOR_REDTEAM_BLUE			119
#define COLOR_REDTEAM_GREEN			119

#define COLOR_BLUETEAM_RED			141
#define COLOR_BLUETEAM_BLUE			238
#define COLOR_BLUETEAM_GREEN		255

#define BUDOKAI_RESULT_TOURNAMENT_OPEN_REMAIN_TIME 5.99f

CTBMatchResultGui::CTBMatchResultGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CTBMatchResultGui::~CTBMatchResultGui(VOID)
{

}

VOID CTBMatchResultGui::Init(VOID)
{
	m_fWaitTime = 0.0f;
	m_eMatchType = INVALID_MATCHWORLD_TYPE;
	m_nMouseOnIndex = -1;
	m_eResult = DRAW;
	m_bTournamentTrigger = FALSE;

	for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
	{
		for( RwInt32 j = 0 ; j < MAX_BUDOKAI_REWARD_ITEM ; ++j )
		{
			m_pstbStackCount[i][j] = NULL;
			m_pRewardItem[i][j] = NULL;
		}			
	}
}

RwBool CTBMatchResultGui::Create( eMATCHWORLD_TYPE eMatchType )
{
	NTL_FUNCTION( "CTBMatchResultGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\BudokaiResult.srf", "gui\\BudokaiResult.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain" ) );
	m_pflaResult = reinterpret_cast<gui::CFlash*>( GetComponent( "flaResult" ) );
	m_pflaBar = reinterpret_cast<gui::CFlash*>( GetComponent( "flaBar" ) );

	m_pstbName[BLUE] = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbBlueName" ) );
	m_pstbInfo1[BLUE] = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbBlueInfo1" ) );
	m_pstbInfo2[BLUE] = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbBlueInfo2" ) );
	m_pstbName[RED] = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbRedName" ) );
	m_pstbInfo1[RED] = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbRedInfo1" ) );
	m_pstbInfo2[RED] = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbRedInfo2" ) );

	m_pThis->SetPriority( dDIALOGPRIORITY_RESULT );

	CRectangle rtScreen = m_pThis->GetScreenRect();

	// 초기위치 설정
	m_rtRewardItem[BLUE][0].SetRectWH( MATCHREWARD_BLUE_ICON_X, MATCHREWARD_ICON_Y, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_rtRewardItem[RED][0].SetRectWH( MATCHREWARD_RED_ICON_X, MATCHREWARD_ICON_Y, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );

	m_surFocusSlot.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_surFocusSlot.Show( FALSE );

	for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
	{
		for( RwInt32 j = 0 ; j < MAX_BUDOKAI_REWARD_ITEM ; ++j )
		{
			m_surIcon[i][j].SetRect( m_rtRewardItem[i][j] );		
			m_surIcon[i][j].Show( FALSE );
			m_surIconBack[i][j].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "BudokaiResult.srf", "srfSlotBack" ) );
			m_surIconBack[i][j].SetPosition( m_rtRewardItem[i][j].left, m_rtRewardItem[i][j].top );
			m_surIconBack[i][j].Show( FALSE );
		}

		if( i == BLUE )
		{
			m_numResult[i].Create( ND_RIGHT );
			m_numResult[i].SetParentPos( MATCHRESULT_BLUE_NUMBER_X, MATCHRESULT_NUMBER_Y );		
			m_numScore[i].Create( ND_RIGHT, NVD_TOP, 0, dNCFLAG_PLUS );
			m_numScore[i].SetParentPos( MATCHSCORE_BLUE_NUMBER_X, MATCHSCORE_NUMBER_Y );			
			m_numBonus[i].Create( ND_RIGHT, NVD_TOP, 0, dNCFLAG_PLUS );
			m_numBonus[i].SetParentPos( MATCHBONUS_BLUE_NUMBER_X, MATCHBONUS_NUMBER_Y );				
		}
		else
		{
			m_numResult[i].Create( ND_LEFT );
			m_numResult[i].SetParentPos( MATCHRESULT_RED_NUMBER_X, MATCHRESULT_NUMBER_Y );		
			m_numScore[i].Create( ND_LEFT, NVD_TOP, 0, dNCFLAG_PLUS );
			m_numScore[i].SetParentPos( MATCHSCORE_RED_NUMBER_X, MATCHSCORE_NUMBER_Y );			
			m_numBonus[i].Create( ND_LEFT, NVD_TOP, 0, dNCFLAG_PLUS );
			m_numBonus[i].SetParentPos( MATCHBONUS_RED_NUMBER_X, MATCHBONUS_NUMBER_Y );						
		}
		
		m_numResult[i].SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN6_0" ) );
		m_numResult[i].SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN6_1" ) );
		m_numResult[i].SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN6_2" ) );
		m_numResult[i].SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN6_3" ) );
		m_numResult[i].SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN6_4" ) );
		m_numResult[i].SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN6_5" ) );
		m_numResult[i].SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN6_6" ) );
		m_numResult[i].SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN6_7" ) );
		m_numResult[i].SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN6_8" ) );
		m_numResult[i].SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN6_9" ) );
		m_numResult[i].SetPosition( rtScreen.left, rtScreen.top );

		m_numScore[i].SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_0" ) );
		m_numScore[i].SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_1" ) );
		m_numScore[i].SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_2" ) );
		m_numScore[i].SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_3" ) );
		m_numScore[i].SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_4" ) );
		m_numScore[i].SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_5" ) );
		m_numScore[i].SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_6" ) );
		m_numScore[i].SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_7" ) );
		m_numScore[i].SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_8" ) );
		m_numScore[i].SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_9" ) );
		m_numScore[i].SetPosition( rtScreen.left, rtScreen.top );

		m_numBonus[i].SetSurface( NC_PLUS, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_Plus" ) );
		m_numBonus[i].SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_0" ) );
		m_numBonus[i].SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_1" ) );
		m_numBonus[i].SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_2" ) );
		m_numBonus[i].SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_3" ) );
		m_numBonus[i].SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_4" ) );
		m_numBonus[i].SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_5" ) );
		m_numBonus[i].SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_6" ) );
		m_numBonus[i].SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_7" ) );
		m_numBonus[i].SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_8" ) );
		m_numBonus[i].SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber2.srf", "srfN5_9" ) );
		m_numBonus[i].SetPosition( rtScreen.left, rtScreen.top );

		if( i == BLUE )
		{
			m_numResult[i].SetColorOnly( COLOR_BLUETEAM_RED, COLOR_BLUETEAM_BLUE, COLOR_BLUETEAM_GREEN );
			m_numScore[i].SetColorOnly( COLOR_BLUETEAM_RED, COLOR_BLUETEAM_BLUE, COLOR_BLUETEAM_GREEN );
			m_numBonus[i].SetColorOnly( COLOR_BLUETEAM_RED, COLOR_BLUETEAM_BLUE, COLOR_BLUETEAM_GREEN );
		}
		else
		{
			m_numResult[i].SetColorOnly( COLOR_REDTEAM_RED, COLOR_REDTEAM_BLUE, COLOR_REDTEAM_GREEN );
			m_numScore[i].SetColorOnly( COLOR_REDTEAM_RED, COLOR_REDTEAM_BLUE, COLOR_REDTEAM_GREEN );
			m_numBonus[i].SetColorOnly( COLOR_REDTEAM_RED, COLOR_REDTEAM_BLUE, COLOR_REDTEAM_GREEN );
		}
	}	
	
	m_slotFSCallBack = m_pflaResult->SigFSCallBack().Connect( this, &CTBMatchResultGui::OnFSCallBack );
	m_slotPaint = m_pflaResult->SigPaint().Connect( this, &CTBMatchResultGui::OnPaint );
	m_slotMove = m_pflaResult->SigMove().Connect( this, &CTBMatchResultGui::OnMove );
	m_slotMouseMove = m_pflaResult->SigMouseMove().Connect( this, &CTBMatchResultGui::OnMouseMove );
	m_slotMouseLeave = m_pflaResult->SigMouseLeave().Connect( this, &CTBMatchResultGui::OnMouseLeave );
	
	CNtlPLGui::Show( false );
	Lower();

	GetNtlGuiManager()->AddUpdateFunc( this );

	m_eMatchType = (RwUInt8)eMatchType;
	switch( eMatchType )
	{		
	case MATCHWORLD_TYPE_MAJOR_MATCH: LinkMsg( g_EventMajorMatchMatchFinish ); break;
	case MATCHWORLD_TYPE_FINAL_MATCH: LinkMsg( g_EventFinalMatchMatchFinish ); break;
	}
	
	LinkMsg( g_EventResize );		

	NTL_RETURN( TRUE );
}

VOID CTBMatchResultGui::Destroy(VOID)
{
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	OnMouseLeave( NULL ); // 인포윈도우 닫기.

	DeleteAllRewardItem(); // 아이템 아이콘 삭제.

	switch( m_eMatchType )
	{		
	case MATCHWORLD_TYPE_MAJOR_MATCH: UnLinkMsg( g_EventMajorMatchMatchFinish ); break;
	case MATCHWORLD_TYPE_FINAL_MATCH: UnLinkMsg( g_EventFinalMatchMatchFinish ); break;
	}
	m_eMatchType = INVALID_MATCHWORLD_TYPE;

	UnLinkMsg( g_EventResize );	

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CTBMatchResultGui::Update( RwReal fElapsed )
{
	if( m_fWaitTime > 0.0f )
	{
		m_fWaitTime -= fElapsed;

		if( m_bTournamentTrigger &&
			m_fWaitTime <= BUDOKAI_RESULT_TOURNAMENT_OPEN_REMAIN_TIME &&
			!GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_TOURNAMENT ) )
		{
			if( m_eMatchType == MATCHWORLD_TYPE_FINAL_MATCH ||
				m_eMatchType == MATCHWORLD_TYPE_MAJOR_MATCH )
				GetDialogManager()->OpenDialog( DIALOG_BUDOKAI_TOURNAMENT );

			m_bTournamentTrigger = FALSE;
		}		

		if( m_fWaitTime <= TB_MATCH_RESULT_REMAIN_TIME && !CTBGui::GetInstance()->m_pCounter->IsShow() )
			CTBGui::GetInstance()->SetCounterTime( m_fWaitTime );
	}
	else if( IsShow() )
		Show( false );
	
	if( IsShow() )
	{
		m_pflaResult->Update( fElapsed );
		m_pflaBar->Update( fElapsed );

		for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
		{
			m_numResult[i].Update( fElapsed );
			m_numBonus[i].Update( fElapsed );
			m_numScore[i].Update( fElapsed );		
		}		
	}	
}

VOID CTBMatchResultGui::Show( bool bOpen )
{
	CNtlPLGui::Show( bOpen );

	if( bOpen )
	{
		CalcPosition( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );
		
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBMatchResultGui::HandleEvents : must World concept is valid" );	

		RwUInt8 byMatchDepth = pTBWorldConcept->GetMatchDepth();
		const WCHAR* wszString = NULL;

		if( byMatchDepth == BUDOKAI_MATCH_DEPTH_4 )
		{
			if( pTBWorldConcept->GetMatchIndex() == 0 )
				wszString = GetDisplayStringManager()->GetString( "DST_BUDOKAI_RESULT_MESSAGE_SEMIFINAL_1" );
			else
				wszString = GetDisplayStringManager()->GetString( "DST_BUDOKAI_RESULT_MESSAGE_SEMIFINAL_2" );
		}
		else if( byMatchDepth == BUDOKAI_MATCH_DEPTH_2 )
		{
			wszString = GetDisplayStringManager()->GetString( "DST_BUDOKAI_RESULT_MESSAGE_FINAL" );
		}
		else
		{
			wszString = GetDisplayStringManager()->GetString( "DST_BUDOKAI_RESULT_MESSAGE" );
		}

		CTBGui::GetInstance()->m_pResultNarration->SetData( "TB_MatchFinish_npc.swf", wszString );				
	}
	else
	{
		m_fWaitTime = 0.0f;
		CTBGui::GetInstance()->m_pResultNarration->Show( false );
		CTBGui::GetInstance()->HideCounterTime();

		if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_TOURNAMENT ) )
			GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_TOURNAMENT );
	}
}

VOID CTBMatchResultGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventMajorMatchMatchFinish || 
		msg.Id == g_EventFinalMatchMatchFinish )
	{
		HideResultComopnent();						
		SetResultData( msg.pData );			
	}
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		CalcPosition( pData->iWidht, pData->iHeight );
	}
}

RwBool CTBMatchResultGui::SetResultData( VOID* pData )
{
	RwUInt32 uiBlueScore = 0;
	RwUInt32 uiRedScore = 0;
	RwUInt32 uiBlueBonus = 0;
	RwUInt32 uiRedBonus = 0;
	RwUInt32 uiBlueResult = 0;
	RwUInt32 uiRedResult = 0;

	m_bPresent[RED] = TRUE;
	m_bPresent[BLUE] = TRUE;
	
	CNtlTBudokai* pTBudokai = GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai();
	sBUDOKAI_TBLINFO* pBudokaiData = pTBudokai->GetBudokaiTblData();
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );	

	if( m_eMatchType == MATCHWORLD_TYPE_MAJOR_MATCH )
	{
		SNtlEventMajorMatchMatchFinish* pFinishData = reinterpret_cast<SNtlEventMajorMatchMatchFinish*>( pData );
		sMATCH_REWARD* pReward = NULL;

		if( pTBWorldConcept->IsPartyBattle() )
		{
			if( GetBudokaiType( GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetBudokaiMainState() ) == BUDOKAI_TYPE_JUNIOR )
				pReward = &pBudokaiData->sJuniorTeamReward.aMajorMatch[pTBWorldConcept->GetMatchDepth()];
			else
				pReward = &pBudokaiData->sTeamReward.aMajorMatch[pTBWorldConcept->GetMatchDepth()];
		}
		else
		{
			if( GetBudokaiType( GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetBudokaiMainState() ) == BUDOKAI_TYPE_JUNIOR )
				pReward = &pBudokaiData->sJuniorIndividualReward.aMajorMatch[pTBWorldConcept->GetMatchDepth()];		
			else
				pReward = &pBudokaiData->sIndividualReward.aMajorMatch[pTBWorldConcept->GetMatchDepth()];		
		}
		
		uiBlueResult = pFinishData->pStageScore->byTeam1;
		uiRedResult	= pFinishData->pStageScore->byTeam2;

		if( pFinishData->byMatchResult == MATCH_RESULT_WIN )
		{
			if( pTBWorldConcept->IsBlueTeam( pFinishData->wMatchWinner ) )
			{
				uiBlueBonus = pReward->dwWinnerMudosaPoint;
				uiBlueScore = pReward->dwBaseMudosaPoint;
				uiRedScore = pReward->dwBaseMudosaPoint;					
				
				SetRewardItem( BLUE, 0, pReward->winnerItem, pReward->byWinerItemStackCount );
				SetRewardItem( RED, 0, pReward->loserItem, pReward->byLoserItemStackCount );
			}
			else if( pTBWorldConcept->IsRedTeam( pFinishData->wMatchWinner ) )
			{
				uiRedBonus = pReward->dwWinnerMudosaPoint;
				uiBlueScore = pReward->dwBaseMudosaPoint;
				uiRedScore = pReward->dwBaseMudosaPoint;				

				SetRewardItem( RED, 0, pReward->winnerItem, pReward->byWinerItemStackCount );
				SetRewardItem( BLUE, 0, pReward->loserItem, pReward->byLoserItemStackCount );
			}

			if( pFinishData->wMatchWinner == pTBWorldConcept->GetMyTeamType() )
				SetFlashResult( WIN );
			else
				SetFlashResult( LOSE );			
		}
		else if( pFinishData->byMatchResult == MATCH_RESULT_DRAW )
		{
			SetRewardItem( RED, 0, pReward->loserItem, pReward->byLoserItemStackCount );
			SetRewardItem( BLUE, 0, pReward->loserItem, pReward->byLoserItemStackCount );
			SetFlashResult( DRAW );
		}		
		else if( pFinishData->byMatchResult == MATCH_RESULT_UNEARNED_WIN )
		{
			if( pTBWorldConcept->IsBlueTeam( pFinishData->wMatchWinner ) )
			{
				uiBlueBonus = pReward->dwWinnerMudosaPoint;
				uiBlueScore = pReward->dwBaseMudosaPoint;
				
				SetRewardItem( BLUE, 0, pReward->winnerItem, pReward->byWinerItemStackCount );		
				m_bPresent[RED] = FALSE;
			}
			else if( pTBWorldConcept->IsRedTeam( pFinishData->wMatchWinner ) )
			{
				uiRedBonus = pReward->dwWinnerMudosaPoint;
				uiRedScore = pReward->dwBaseMudosaPoint;				

				SetRewardItem( RED, 0, pReward->winnerItem, pReward->byWinerItemStackCount );		
				m_bPresent[BLUE] = FALSE;
			}

			SetFlashResult( UNEARNED_WIN );	
		}

		m_pflaResult->PlayMovie( TRUE );
		m_pflaBar->PlayMovie( TRUE );
		Show( true );
	}
	else if( m_eMatchType == MATCHWORLD_TYPE_FINAL_MATCH )
	{
		SNtlEventFinalMatchMatchFinish* pFinishData = reinterpret_cast<SNtlEventFinalMatchMatchFinish*>( pData );
		sMATCH_REWARD* pReward = NULL;

		if( pTBWorldConcept->IsPartyBattle() )
		{
			if( GetBudokaiType( GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetBudokaiMainState() ) == BUDOKAI_TYPE_JUNIOR )
				pReward = &pBudokaiData->sJuniorTeamReward.aMajorMatch[pTBWorldConcept->GetMatchDepth()];
			else
				pReward = &pBudokaiData->sTeamReward.aMajorMatch[pTBWorldConcept->GetMatchDepth()];
		}
		else
		{
			if( GetBudokaiType( GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetBudokaiMainState() ) == BUDOKAI_TYPE_JUNIOR )
				pReward = &pBudokaiData->sJuniorIndividualReward.aMajorMatch[pTBWorldConcept->GetMatchDepth()];
			else
				pReward = &pBudokaiData->sIndividualReward.aMajorMatch[pTBWorldConcept->GetMatchDepth()];
		}

		uiBlueResult = pFinishData->pStageScore->byTeam1;
		uiRedResult	= pFinishData->pStageScore->byTeam2;

		if( pFinishData->byMatchResult == MATCH_RESULT_WIN )
		{
			if( pTBWorldConcept->IsBlueTeam( pFinishData->wMatchWinner ) )
			{
				uiBlueBonus = pReward->dwWinnerMudosaPoint;
				uiBlueScore = pReward->dwBaseMudosaPoint;
				uiRedScore = pReward->dwBaseMudosaPoint;				

				SetRewardItem( BLUE, 0, pReward->winnerItem, pReward->byWinerItemStackCount );
				SetRewardItem( RED, 0, pReward->loserItem, pReward->byLoserItemStackCount );
			}
			else if( pTBWorldConcept->IsRedTeam( pFinishData->wMatchWinner ) )
			{
				uiRedBonus = pReward->dwWinnerMudosaPoint;
				uiBlueScore = pReward->dwBaseMudosaPoint;
				uiRedScore = pReward->dwBaseMudosaPoint;

				SetRewardItem( RED, 0, pReward->winnerItem, pReward->byWinerItemStackCount );
				SetRewardItem( BLUE, 0, pReward->loserItem, pReward->byLoserItemStackCount );
			}

			if( pFinishData->wMatchWinner == pTBWorldConcept->GetMyTeamType() )
				SetFlashResult( WIN );
			else
				SetFlashResult( LOSE );			
		}
		else if( pFinishData->byMatchResult == MATCH_RESULT_DRAW )
		{
			SetRewardItem( BLUE, 0, pReward->loserItem, pReward->byLoserItemStackCount );
			SetRewardItem( RED, 0, pReward->loserItem, pReward->byLoserItemStackCount );
			SetFlashResult( DRAW );
		}	
		else if( pFinishData->byMatchResult == MATCH_RESULT_UNEARNED_WIN )
		{
			if( pTBWorldConcept->IsBlueTeam( pFinishData->wMatchWinner ) )
			{
				uiBlueBonus = pReward->dwWinnerMudosaPoint;
				uiBlueScore = pReward->dwBaseMudosaPoint;

				SetRewardItem( BLUE, 0, pReward->winnerItem, pReward->byWinerItemStackCount );	
				m_bPresent[RED] = FALSE;
			}
			else if( pTBWorldConcept->IsRedTeam( pFinishData->wMatchWinner ) )
			{
				uiRedBonus = pReward->dwWinnerMudosaPoint;
				uiRedScore = pReward->dwBaseMudosaPoint;				

				SetRewardItem( RED, 0, pReward->winnerItem, pReward->byWinerItemStackCount );	
				m_bPresent[BLUE] = FALSE;
			}

			SetFlashResult( UNEARNED_WIN );	
		}

		m_pflaResult->PlayMovie( TRUE );
		m_pflaBar->PlayMovie( TRUE );
		Show( true );
	}
	else
		return FALSE;

	m_numResult[BLUE].SetNumber( uiBlueResult );
	m_numResult[RED].SetNumber( uiRedResult );
	m_numScore[BLUE].SetNumber( uiBlueScore );
	m_numScore[RED].SetNumber( uiRedScore );
	m_numBonus[BLUE].SetNumber( uiBlueBonus );
	m_numBonus[RED].SetNumber( uiRedBonus );

	if( pTBWorldConcept->GetBlueTeam() )
		m_pstbName[BLUE]->SetText( pTBWorldConcept->GetBlueTeam()->wstrTeamName.c_str() );
	if( pTBWorldConcept->GetRedTeam() )
		m_pstbName[RED]->SetText( pTBWorldConcept->GetRedTeam()->wstrTeamName.c_str() );

	if( !pTBWorldConcept->IsPartyBattle() )
	{
		stTBudokaiTeam* pBlueTeam = pTBWorldConcept->GetBlueTeam();
		stTBudokaiTeam* pRedTeam = pTBWorldConcept->GetRedTeam();

		if( pBlueTeam )
		{
			if( pBlueTeam->vecpMember.size() >= 0 )
			{
				stTBudokaiMember* pMember = pBlueTeam->vecpMember.front();
				m_pstbInfo1[BLUE]->Format( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_LEVEL_VALUE" ), pMember->byLevel );
				m_pstbInfo2[BLUE]->Format( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_RECORD_DATA" ), pMember->wWinCount + pMember->wLoseCount + pMember->wDrawCount,
										   pMember->wWinCount, pMember->wLoseCount );	
			}
			else
			{
				m_pstbInfo1[BLUE]->Clear();
				m_pstbInfo2[BLUE]->Clear();
			}
		}			

		if( pRedTeam )
		{
			if( pRedTeam->vecpMember.size() >= 0 )
			{
				stTBudokaiMember* pMember = pRedTeam->vecpMember.front();
				m_pstbInfo1[RED]->Format( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_LEVEL_VALUE" ), pMember->byLevel );
				m_pstbInfo2[RED]->Format( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_RECORD_DATA" ), pMember->wWinCount + pMember->wLoseCount + pMember->wDrawCount,
					pMember->wWinCount, pMember->wLoseCount );	
			}
			else
			{
				m_pstbInfo1[RED]->Clear();
				m_pstbInfo2[RED]->Clear();
			}
		}
	}

	// WaitTime
	m_fWaitTime = (RwReal)pTBWorldConcept->GetMatchTbl()->dwMatchFinishTime + 0.99f; 
	
	m_bTournamentTrigger = TRUE;

	return TRUE;
}

VOID CTBMatchResultGui::SetRewardItem( RwInt32 eTeam, RwInt32 nSlotIdx, RwUInt32 tblidx, RwUInt8 byStackCount )
{
	CItemTable* pItemTable = API_GetTableContainer()->GetItemTable();
	sITEM_TBLDAT* pTableData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData( tblidx ) );
	if( pTableData )
	{
		m_surIcon[eTeam][nSlotIdx].SetTexture( Logic_CreateTexture( const_cast<RwChar*>( pTableData->szIcon_Name ) ) );
		if( pTableData->byMax_Stack > 1 )
			CreateStackNumber( eTeam, nSlotIdx, byStackCount );			

		m_pRewardItem[eTeam][nSlotIdx] = pTableData;		
	}
}

VOID CTBMatchResultGui::DeleteRewardItem( RwInt32 eTeam, RwInt32 nSlotIdx )
{
	m_pRewardItem[eTeam][nSlotIdx] = NULL;
	m_surIconBack[eTeam][nSlotIdx].Show( FALSE );
	Logic_DeleteTexture( m_surIcon[eTeam][nSlotIdx].GetTexture() );
	m_surIcon[eTeam][nSlotIdx].UnsetTexture();
	m_surIcon[eTeam][nSlotIdx].Show( FALSE );
	DeleteStackNumber( eTeam, nSlotIdx );	
}

VOID CTBMatchResultGui::DeleteAllRewardItem(VOID)
{
	for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
	{
		for( RwInt32 j = 0 ; j < MAX_BUDOKAI_REWARD_ITEM ; ++j )
		{
			DeleteRewardItem( i, j );
		}
	}	
}

RwBool CTBMatchResultGui::CreateStackNumber( RwInt32 eTeam, RwInt32 nSlotIdx, RwUInt8 uiStackCount )
{
	NTL_FUNCTION( "CTBMatchResultGui::CreateStackNubmer" );

	CRectangle rect;
	rect.SetRect( m_rtRewardItem[eTeam][nSlotIdx].left, m_rtRewardItem[eTeam][nSlotIdx].bottom - DBOGUI_STACKNUM_HEIGHT,
				  m_rtRewardItem[eTeam][nSlotIdx].left + DBOGUI_STACKNUM_WIDTH, m_rtRewardItem[eTeam][nSlotIdx].bottom );
	m_pstbStackCount[eTeam][nSlotIdx] = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN );
	if( !m_pstbStackCount[eTeam][nSlotIdx] )
		NTL_RETURN( FALSE );

	m_pstbStackCount[eTeam][nSlotIdx]->CreateFontStd( DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR );
	m_pstbStackCount[eTeam][nSlotIdx]->SetEffectMode( DBOGUI_STACKNUM_FONTEFFECTMODE );
	m_pstbStackCount[eTeam][nSlotIdx]->SetText( uiStackCount );
	m_pstbStackCount[eTeam][nSlotIdx]->Enable( false );
	m_pstbStackCount[eTeam][nSlotIdx]->Show( false );

	NTL_RETURN( TRUE );
}

VOID CTBMatchResultGui::DeleteStackNumber( RwInt32 eTeam, RwInt32 nSlotIdx )
{
	if( m_pstbStackCount[eTeam][nSlotIdx] )
		NTL_DELETE( m_pstbStackCount[eTeam][nSlotIdx] );
}

VOID CTBMatchResultGui::HideResultComopnent(VOID)
{
	m_pflaResult->RestartMovie();
	m_pflaBar->RestartMovie();

	m_surFocusSlot.Show( FALSE );	

	DeleteAllRewardItem();

	for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
	{
		m_numResult[i].Show( FALSE );
		m_numBonus[i].Show( FALSE );
		m_numScore[i].Show( FALSE );

		m_pstbName[i]->Clear();
		m_pstbInfo1[i]->Clear();
		m_pstbInfo2[i]->Clear();
		m_pstbName[i]->Show( false );
		m_pstbInfo1[i]->Show( false );
		m_pstbInfo2[i]->Show( false );
	}
}

VOID CTBMatchResultGui::SetMouseOnIndex( RwInt32 nIndex )
{
	NTL_ASSERT( nIndex < MAX_BUDOKAI_REWARD_ITEM * TEAM_COUNT, "CTBMatchResultGui::SetMouseOnIndex : Invalid Array Index" );

	m_nMouseOnIndex = nIndex;

	RwInt32 eTeam = nIndex / MAX_BUDOKAI_REWARD_ITEM;
	RwInt32 nSlotIdx = nIndex % MAX_BUDOKAI_REWARD_ITEM;
	
	if( nIndex >= 0 )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		m_surFocusSlot.SetPosition( rtScreen.left + m_rtRewardItem[eTeam][nSlotIdx].left - 2, rtScreen.top + m_rtRewardItem[eTeam][nSlotIdx].top - 2 );
		m_surFocusSlot.Show( TRUE );
	}
	else
		m_surFocusSlot.Show( FALSE );
	
}

RwInt32 CTBMatchResultGui::GetRewardSlotIndex( RwInt32 nX, RwInt32 nY )
{
	for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
	{
		for( RwInt32 j = 0 ; j < MAX_BUDOKAI_REWARD_ITEM ; ++j )
		{
			if( m_pRewardItem[i][j] &&
				m_rtRewardItem[i][j].PtInRect( nX, nY ) )
				return i * MAX_BUDOKAI_REWARD_ITEM + j;
		}
	}	

	return -1;
}

VOID CTBMatchResultGui::SetFlashResult( FLASHRESULT eResult )
{
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );	

	// 참가자가 아니라면
	if( !pTBWorldConcept->GetMyTeam() )
		eResult = NONE;

	m_pflaResult->Invoke( "Conclusion", "%d,%d", eResult, !pTBWorldConcept->IsPartyBattle() );
	m_eResult = eResult;

	if( m_eResult == WIN || m_eResult == UNEARNED_WIN )
		CNtlSLEventGenerator::SobAnimPlay( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), SOC_HAPPY, FALSE, 0.0f );
	else if( m_eResult == LOSE || m_eResult == DRAW )
		CNtlSLEventGenerator::SobAnimPlay( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), SOC_SAD, FALSE, 0.0f );
}

VOID CTBMatchResultGui::OnFSCallBack( const RwChar* szCommand, const RwChar* szArgs )
{
	if( !strcmp( szCommand, "TeamTextStart" ) )	// Team Name
	{
		for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
		{
			if( m_bPresent[i] )
				m_pstbName[i]->Show( true );	
		}		
	}
	else if( !strcmp( szCommand, "ResultTextStart" ) )	// Result
	{
		for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
		{
			if( m_bPresent[i] )
			{
				m_numResult[i].EffectRate( 8.0f, 1.0f, 0.5f );
				m_numResult[i].Show( TRUE );	
			}			
		}			
	}
	else if( !strcmp( szCommand, "ScoreTextStart" ) )	// Score
	{
		for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
		{
			if( m_bPresent[i] )
			{
				m_numScore[i].EffectRate( 8.0f, 1.0f, 0.5f );
				m_numScore[i].Show( TRUE );
			}			
		}		
	}
	else if( !strcmp( szCommand, "BonusTextStart" ) )	// Bonus
	{
		for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
		{
			if( m_bPresent[i] )
			{
				m_numBonus[i].EffectRate( 8.0f, 1.0f, 0.5f );
				m_numBonus[i].Show( TRUE );
			}			
		}		
	}	
	else if( !strcmp( szCommand, "RewardTextStart" ) )	// Reward
	{
		for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
		{
			if( m_bPresent[i] )
			{
				for( RwInt32 j = 0 ; j < MAX_BUDOKAI_REWARD_ITEM ; ++j )
				{
					if( m_pRewardItem[i][j] )
					{
						m_surIcon[i][j].Show( TRUE );
						m_surIconBack[i][j].Show( TRUE );
						if( m_pstbStackCount[i][j] )
							m_pstbStackCount[i][j]->Show( true );
					}
				}
			}			
		}		
	}
	else if( !strcmp( szCommand, "InfoTextStart" ) )	// Info
	{
		for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
		{
			if( m_bPresent[i] )
			{
				m_pstbInfo1[i]->Show( true );
				m_pstbInfo2[i]->Show( true );
			}			
		}
	}	
}

VOID CTBMatchResultGui::OnPaint(VOID)
{
	for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
	{
		for( RwInt32 j = 0 ; j < MAX_BUDOKAI_REWARD_ITEM ; ++j )
		{
			m_surIconBack[i][j].Render();
			m_surIcon[i][j].Render();		
		}

		m_numBonus[i].Render();
		m_numScore[i].Render();
		m_numResult[i].Render();
	}
	
	m_surFocusSlot.Render();
}

VOID CTBMatchResultGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
	{
		for( RwInt32 j = 0 ; j < MAX_BUDOKAI_REWARD_ITEM ; ++j )
		{
			m_surIconBack[i][j].SetPosition( rtScreen.left + m_rtRewardItem[i][j].left, rtScreen.top +m_rtRewardItem[i][j].top );
			m_surIcon[i][j].SetPosition( rtScreen.left + m_rtRewardItem[i][j].left, rtScreen.top +m_rtRewardItem[i][j].top );
		}

		m_numResult[i].SetPosition( rtScreen.left, rtScreen.top );
		m_numBonus[i].SetPosition( rtScreen.left, rtScreen.top );
		m_numScore[i].SetPosition( rtScreen.left, rtScreen.top );
	}	

	m_surFocusSlot.SetPosition( rtScreen.left, rtScreen.top );
}

VOID CTBMatchResultGui::OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY )
{
	RwInt32 nMouseOnIdx = GetRewardSlotIndex( nX, nY );
	RwInt32 eTeam = nMouseOnIdx / MAX_BUDOKAI_REWARD_ITEM;
	RwInt32 nSlotIdx = nMouseOnIdx % MAX_BUDOKAI_REWARD_ITEM;

	if( nMouseOnIdx >= 0 )
	{
		if( m_nMouseOnIndex != nMouseOnIdx )
		{
			CRectangle rtScreen = m_pThis->GetScreenRect();
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_TABLE_ITEM, m_rtRewardItem[eTeam][nSlotIdx].left + rtScreen.left, m_rtRewardItem[eTeam][nSlotIdx].top + rtScreen.top, m_pRewardItem[eTeam][nSlotIdx], DIALOG_UNKNOWN );
			SetMouseOnIndex( nMouseOnIdx );
		}
	}
	else
	{
		if( m_nMouseOnIndex >= 0 )
		{
			GetInfoWndManager()->ShowInfoWindow( FALSE );
			SetMouseOnIndex( -1 );
		}
	}
}

VOID CTBMatchResultGui::OnMouseLeave( gui::CComponent* pComponent )
{
	if( m_nMouseOnIndex >= 0 )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		SetMouseOnIndex( -1 );
	}
}

VOID CTBMatchResultGui::CalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	CenterAlign( nScreenWidth, nScreenHeight );
	
	RwInt32 nBarLeft = -m_pThis->GetPosition().left;
	CRectangle rtBar = m_pflaBar->GetPosition();
	
	m_pflaBar->SetPosition( CRectangle( nBarLeft, rtBar.top, nBarLeft + nScreenWidth, rtBar.bottom ) );	
}
