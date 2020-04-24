#include "precomp_dboclient.h"
#include "RBResultGui.h"

// core
#include "NtlDebug.h"

// share
#include "NtlSLRankBattle.h"

// table
#include "RankBattleTable.h"
#include "GraphicDataTable.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlSLRankBattle.h"

// client
#include "DboGlobal.h"
#include "DialogPriority.h"
#include "DboEvent.h"
#include "RBGui.h"
#include "ResultNarrationGui.h"
#include "CounterGui.h"

#define TEAMNAME_LINE_BLUE_TEXT_X	652
#define TEAMNAME_LINE_RED_TEXT_X	131
#define TEAMNAME_LINE_TEXT_Y		274
#define TEAMNAME_LINE_WIDTH			245
#define TEAMNAME_LINE_HEIGHT		22

#define MATCHRESULT_BLUE_NUMBER_X	652
#define MATCHRESULT_RED_NUMBER_X	376
#define MATCHRESULT_NUMBER_Y		321

#define MATCHSCORE_BLUE_NUMBER_X	652
#define MATCHSCORE_RED_NUMBER_X		376
#define MATCHSCORE_NUMBER_Y			366

#define MATCHBONUS_BLUE_NUMBER_X	652
#define MATCHBONUS_RED_NUMBER_X		376
#define MATCHBONUS_NUMBER_Y			410

#define MATCHINFO_LINE_BLUE_TEXT_X	652
#define MATCHINFO_LINE_RED_TEXT_X	131
#define MATCHINFO_LINE_WIDTH		245
#define MATCHINFO_LINE_HEIGHT		22

#define MATCHINFO_LINE1_TEXT_Y		453
#define MATCHINFO_LINE2_TEXT_Y		482

#define COLOR_REDTEAM_RED			255
#define COLOR_REDTEAM_BLUE			119
#define COLOR_REDTEAM_GREEN			119

#define COLOR_BLUETEAM_RED			141
#define COLOR_BLUETEAM_BLUE			238
#define COLOR_BLUETEAM_GREEN		255

#define RB_MATCH_RESULT_REMAIN_TIME 10.99f

CRBResultGui::CRBResultGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CRBResultGui::~CRBResultGui(VOID)
{

}

VOID CRBResultGui::Init(VOID)
{
	m_fWaitTime = 0.0f;
	m_eResult = DRAW;	
}

RwBool CRBResultGui::Create(VOID)
{
	NTL_FUNCTION( "CRBResultGui::Create" );

	if( !CNtlPLGui::Create( "", "", "gui\\RBResult.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain" ) );
	m_pflaResult = reinterpret_cast<gui::CFlash*>( GetComponent( "flaResult" ) );
	m_pflaBar = reinterpret_cast<gui::CFlash*>( GetComponent( "flaBar" ) );

	m_pstbName[BLUE] = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbBlueName" ) );
	m_pstbInfo1[BLUE] = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbBlueMileage" ) );
	m_pstbInfo2[BLUE] = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbBlueRankPoint" ) );
	m_pstbName[RED] = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbRedName" ) );
	m_pstbInfo1[RED] = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbRedMileage" ) );
	m_pstbInfo2[RED] = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbRedRankPoint" ) );

	m_pThis->SetPriority( dDIALOGPRIORITY_RESULT );

	CRectangle rtScreen = m_pThis->GetScreenRect();

	// 초기위치 설정
	for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
	{
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

	m_slotFSCallBack = m_pflaResult->SigFSCallBack().Connect( this, &CRBResultGui::OnFSCallBack );
	m_slotPaint = m_pflaResult->SigPaint().Connect( this, &CRBResultGui::OnPaint );
	m_slotMove = m_pflaResult->SigMove().Connect( this, &CRBResultGui::OnMove );

	CNtlPLGui::Show( false );
	Lower();

	GetNtlGuiManager()->AddUpdateFunc( this );

	LinkMsg( g_EventResize );		
	LinkMsg( g_EventRBBattleMatchFinish );

	NTL_RETURN( TRUE );
}

VOID CRBResultGui::Destroy(VOID)
{
	m_pflaResult->Unload();
	m_pflaBar->Unload();

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventResize );	
	UnLinkMsg( g_EventRBBattleMatchFinish );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CRBResultGui::Update( RwReal fElapsed )
{
	if( m_fWaitTime > 0.0f )
	{
		m_fWaitTime -= fElapsed;

		if( m_fWaitTime <= RB_MATCH_RESULT_REMAIN_TIME && !CRBGui::GetInstance()->m_pCounter->IsShow() )
			CRBGui::GetInstance()->SetCounterTime( m_fWaitTime );
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

VOID CRBResultGui::Show( bool bOpen )
{
	CNtlPLGui::Show( bOpen );

	if( bOpen )
	{
		CRBGui::GetInstance()->m_pResultNarration->SetData( "RankBattle_result_npc.swf", GetDisplayStringManager()->GetString( "DST_RB_RESULT_MESSAGE" ) );		

		CalcPosition(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
	}
	else
	{
		m_fWaitTime = 0.0f;
		CRBGui::GetInstance()->m_pResultNarration->Show( false );
		CRBGui::GetInstance()->HideCounterTime();
	}
}

VOID CRBResultGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventRBBattleMatchFinish )
	{		
		SetResultData( msg.pData );				
	}
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		CalcPosition( pData->iWidht, pData->iHeight );
	}
}

RwBool CRBResultGui::SetResultData( VOID* pData )
{
	//
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

	//RwUInt32 uiBlueScore = 0;
	//RwUInt32 uiRedScore = 0;
	//RwUInt32 uiBlueBonus = 0;
	//RwUInt32 uiRedBonus = 0;
	//RwUInt32 uiBlueResult = 0;
	//RwUInt32 uiRedResult = 0;
	//RwUInt16 wBlueLoseOffset = 0;
	//RwUInt16 wBlueWinOffset = 0;
	//RwUInt16 wRedLoseOffset = 0;
	//RwUInt16 wRedWinOffset = 0;

	SNtlEventRBBattleMatchFinish* pMatchFinishInfo = reinterpret_cast<SNtlEventRBBattleMatchFinish*>( pData );
	sRANKBATTLE_MATCH_FINISH_INFO* pFinishInfo = reinterpret_cast<sRANKBATTLE_MATCH_FINISH_INFO*>( pMatchFinishInfo->pMatchResult );
	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
	//sRANKBATTLE_TBLDAT* pTblData = pRankBattle->GetTblData();

	// Name
	m_pstbName[RED]->SetText( pRankBattle->GetRedTeamName().c_str() );
	m_pstbName[BLUE]->SetText( pRankBattle->GetBlueTeamName().c_str() );

	// Score
	m_numResult[RED].SetNumber( pFinishInfo->sScore.byChallenger);
	m_numScore[RED].SetNumber(pRankBattle->IsRedTeam() ? (RwInt32)pFinishInfo->dwMudosaPoint : 0);
	m_numBonus[RED].SetNumber(pRankBattle->IsRedTeam() ?  (RwInt32)pFinishInfo->fRankPoint : 0);

	m_numResult[BLUE].SetNumber(pFinishInfo->sScore.byOwner);
	m_numScore[BLUE].SetNumber(pRankBattle->IsBlueTeam() ? (RwInt32)pFinishInfo->dwMudosaPoint : 0);
	m_numBonus[BLUE].SetNumber(pRankBattle->IsBlueTeam() ? (RwInt32)pFinishInfo->fRankPoint : 0);

	// WaitTime
	m_fWaitTime = (RwReal)pRankBattle->GetTblData()->dwMatchFinishTime; 


	Show( true );
	m_pflaBar->PlayMovie(TRUE);

	// SET result win/lose/draw. Must be called after RankBattle_result.swf has been loaded
	CNtlRankBattle::RESULT eResult = pRankBattle->GetResult();
	FLASHRESULT eFlashResult = NONE;

	switch (eResult)
	{
		case CNtlRankBattle::WIN: eFlashResult = WIN; break;
		case CNtlRankBattle::LOSE: eFlashResult = LOSE; break;
		case CNtlRankBattle::DRAW: eFlashResult = DRAW; break;
	}

	SetFlashResult(eFlashResult);
	m_pflaResult->PlayMovie(TRUE);

	return TRUE;
}

VOID CRBResultGui::SetFlashResult( FLASHRESULT eResult )
{
	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();

	if( pRankBattle->GetMyTeamMap()->size() == 0 )
		eResult = NONE;

	m_pflaResult->Invoke( "Conclusion", "%d,%d", eResult, !pRankBattle->IsPartyBattle() );
	m_eResult = eResult;

	if( m_eResult == WIN )
		CNtlSLEventGenerator::SobAnimPlay( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), SOC_HAPPY, FALSE, 0.0f );
	else if( m_eResult == LOSE || m_eResult == DRAW )
		CNtlSLEventGenerator::SobAnimPlay( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), SOC_SAD, FALSE, 0.0f );
}

VOID CRBResultGui::OnFSCallBack( const RwChar* szCommand, const RwChar* szArgs )
{
	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();

	if( !strcmp( szCommand, "TeamTextStart" ) )	// Team Name
	{
		for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
		{
			m_pstbName[i]->Show( true );	
		}		
	}
	else if( !strcmp( szCommand, "ResultTextStart" ) )	// Result
	{
		for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
		{
			m_numResult[i].EffectRate( 8.0f, 1.0f, 0.5f );
			m_numResult[i].Show( TRUE );	
		}			
	}
	else if( !strcmp( szCommand, "ScoreTextStart" ) )	// Score
	{
		for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
		{

			m_numScore[i].EffectRate( 8.0f, 1.0f, 0.5f );
			m_numScore[i].Show( TRUE );

		}		
	}
	else if( !strcmp( szCommand, "BonusTextStart" ) )	// Bonus
	{
		for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
		{

			m_numBonus[i].EffectRate( 8.0f, 1.0f, 0.5f );
			m_numBonus[i].Show( TRUE );

		}		
	}	
	else if( !strcmp( szCommand, "ProfileTextStart" ) )	// Info
	{
		for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
		{
			m_pstbInfo1[i]->Show( true );
			m_pstbInfo2[i]->Show( true );
		}
	}	
}

VOID CRBResultGui::OnPaint(VOID)
{
	for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
	{
		m_numBonus[i].Render();
		m_numScore[i].Render();
		m_numResult[i].Render();
	}
}

VOID CRBResultGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 i = 0 ; i < TEAM_COUNT ; ++i )
	{
		m_numResult[i].SetPosition( rtScreen.left, rtScreen.top );
		m_numBonus[i].SetPosition( rtScreen.left, rtScreen.top );
		m_numScore[i].SetPosition( rtScreen.left, rtScreen.top );
	}	
}

VOID CRBResultGui::CalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	CenterAlign( nScreenWidth, nScreenHeight );

	RwInt32 nBarLeft = -m_pThis->GetPosition().left;
	CRectangle rtBar = m_pflaBar->GetPosition();

	m_pflaBar->SetPosition( CRectangle( nBarLeft, rtBar.top, nBarLeft + nScreenWidth, rtBar.bottom ) );	
}

////CRBResultGui::CRBResultGui(VOID)
////: CNtlPLGui()
////{
////	Init();
////}
////
////CRBResultGui::CRBResultGui( const RwChar* pName )
////: CNtlPLGui( pName )
////{
////	Init();
////}
////
////CRBResultGui::~CRBResultGui(VOID)
////{
////			
////}
////
////VOID CRBResultGui::Init(VOID)
////{
////	m_fWaitTime = 0.0f;
////}
////
////RwBool CRBResultGui::Create(VOID)
////{
////	NTL_FUNCTION( "CRBResultGui::Create" );
////
////	if( !CNtlPLGui::Create( "gui\\RankBattle.rsr", "gui\\RankBattle.srf", "gui\\RBResult.frm" ) )
////		NTL_RETURN( FALSE );
////
////	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );
////
////	m_pThis = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain" ) );
////
////	m_pstbKameName = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbKameName" ) );
////	m_pstbTsuruName = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbTsuruName" ) );
////	m_pstbKameRecord = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbKameRecord" ) );
////	m_pstbTsuruRecord = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbTsuruRecord" ) );
////	m_pflaGrade = reinterpret_cast<gui::CFlash*>( GetComponent( "flaGrade" ) );
////	m_pstbWaitTime = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbWaitTime" ) );
////
////	m_pstbResultText = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbResultText" ) );
////	m_pstbWaitText = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbWaitText" ) );	
////	
////	m_pbtnOK = reinterpret_cast<gui::CButton*>( GetComponent( "btnOK" ) );
////
////	m_pstbResultText->SetText( GetDisplayStringManager()->GetString( DST_RB_RESULT_POINT_RESULT ) );
////	m_pstbWaitTime->SetTextColor( RANKBATTLE_COLOR_RED );
////	m_pstbWaitText->SetText( GetDisplayStringManager()->GetString( DST_RB_RESULT_WAIT_TEXT ) );
////	m_pbtnOK->SetText( GetDisplayStringManager()->GetString( DST_RB_OK ) );
////
////	m_pflaGrade->SetFrameCallback( TRUE );
////	
////	CRectangle rtScreen = m_pThis->GetScreenRect();
////
////	m_numKameStageScore.Create( ND_LEFT );
////	m_numKameStageScore.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfStageNum_0" ) );
////	m_numKameStageScore.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfStageNum_1" ) );
////	m_numKameStageScore.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfStageNum_2" ) );
////	m_numKameStageScore.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfStageNum_3" ) );
////	m_numKameStageScore.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfStageNum_4" ) );
////	m_numKameStageScore.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfStageNum_5" ) );
////	m_numKameStageScore.SetParentPos( 155, 114 );
////	m_numKameStageScore.SetPosition( rtScreen.left, rtScreen.top );
////	m_numKameStageScore.SetNumGap(-5);
////
////	m_numTsuruStageScore.Create( ND_RIGHT );
////	m_numTsuruStageScore.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfStageNum_0" ) );
////	m_numTsuruStageScore.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfStageNum_1" ) );
////	m_numTsuruStageScore.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfStageNum_2" ) );
////	m_numTsuruStageScore.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfStageNum_3" ) );
////	m_numTsuruStageScore.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfStageNum_4" ) );
////	m_numTsuruStageScore.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfStageNum_5" ) );
////	m_numTsuruStageScore.SetParentPos( 219, 114 );
////	m_numTsuruStageScore.SetPosition( rtScreen.left, rtScreen.top );
////	m_numTsuruStageScore.SetNumGap(-5);
////
////	m_numColon.Create( ND_CENTER );
////	m_numColon.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfStageNum_Colon" ) );
////	m_numColon.SetParentPos( 187, 114 );
////	m_numColon.SetPosition( rtScreen.left, rtScreen.top );
////	m_numColon.SetNumber( 0 );
////
////	m_numKameScore.Create( ND_CENTER, NVD_TOP, 0, dNCFLAG_MINUS );
////	m_numKameScore.SetSurface( NC_MINUS, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreMinus" ) );
////	m_numKameScore.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_0" ) );
////	m_numKameScore.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_1" ) );
////	m_numKameScore.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_2" ) );
////	m_numKameScore.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_3" ) );
////	m_numKameScore.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_4" ) );
////	m_numKameScore.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_5" ) );
////	m_numKameScore.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_6" ) );
////	m_numKameScore.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_7" ) );
////	m_numKameScore.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_8" ) );
////	m_numKameScore.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_9" ) );
////	m_numKameScore.SetParentPos( 93, 250 );
////	m_numKameScore.SetPosition( rtScreen.left, rtScreen.top );
////	m_numKameScore.SetNumGap(-5);
////	
////	m_numKameBonusScore.Create( ND_CENTER, NVD_TOP, 0, dNCFLAG_PLUS );
////	m_numKameBonusScore.SetSurface( NC_PLUS, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusPlus" ) );
////	m_numKameBonusScore.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_0" ) );
////	m_numKameBonusScore.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_1" ) );
////	m_numKameBonusScore.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_2" ) );
////	m_numKameBonusScore.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_3" ) );
////	m_numKameBonusScore.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_4" ) );
////	m_numKameBonusScore.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_5" ) );
////	m_numKameBonusScore.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_6" ) );
////	m_numKameBonusScore.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_7" ) );
////	m_numKameBonusScore.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_8" ) );
////	m_numKameBonusScore.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_9" ) );
////	m_numKameBonusScore.SetParentPos( 113, 270 );
////	m_numKameBonusScore.SetPosition( rtScreen.left, rtScreen.top );
////	m_numKameBonusScore.SetNumGap(-5);
////	
////	m_numTsuruScore.Create( ND_CENTER, NVD_TOP, 0, dNCFLAG_MINUS );
////	m_numTsuruScore.SetSurface( NC_MINUS, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreMinus" ) );
////	m_numTsuruScore.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_0" ) );
////	m_numTsuruScore.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_1" ) );
////	m_numTsuruScore.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_2" ) );
////	m_numTsuruScore.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_3" ) );
////	m_numTsuruScore.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_4" ) );
////	m_numTsuruScore.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_5" ) );
////	m_numTsuruScore.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_6" ) );
////	m_numTsuruScore.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_7" ) );
////	m_numTsuruScore.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_8" ) );
////	m_numTsuruScore.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_9" ) );
////	m_numTsuruScore.SetParentPos( 281, 250 );
////	m_numTsuruScore.SetPosition( rtScreen.left, rtScreen.top );
////	m_numTsuruScore.SetNumGap(-5);
////	
////	m_numTsuruBonusScore.Create( ND_CENTER, NVD_TOP, 0, dNCFLAG_PLUS );
////	m_numTsuruBonusScore.SetSurface( NC_PLUS, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusPlus" ) );
////	m_numTsuruBonusScore.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_0" ) );
////	m_numTsuruBonusScore.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_1" ) );
////	m_numTsuruBonusScore.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_2" ) );
////	m_numTsuruBonusScore.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_3" ) );
////	m_numTsuruBonusScore.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_4" ) );
////	m_numTsuruBonusScore.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_5" ) );
////	m_numTsuruBonusScore.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_6" ) );
////	m_numTsuruBonusScore.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_7" ) );
////	m_numTsuruBonusScore.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_8" ) );
////	m_numTsuruBonusScore.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_9" ) );
////	m_numTsuruBonusScore.SetParentPos( 301, 270 );
////	m_numTsuruBonusScore.SetPosition( rtScreen.left, rtScreen.top );
////	m_numTsuruBonusScore.SetNumGap(-5);
////	
////	m_slotFlashMovieEnd = m_pflaGrade->SigMovieEnd().Connect( this, &CRBResultGui::OnFlashMovieEnd );
////	m_slotPaint = m_pThis->SigPaint().Connect( this, &CRBResultGui::OnPaint );
////	m_slotMove = m_pThis->SigMove().Connect( this, &CRBResultGui::OnMove );
////
////	m_numKameStageScore.LinkRateEffect( this, &CRBResultGui::OnKameScoreEffectEnd );
////	m_numColon.LinkRateEffect( this, &CRBResultGui::OnColonEffectEnd );
////	m_numTsuruStageScore.LinkRateEffect( this, &CRBResultGui::OnTsuruScoreEffectEnd );
////	m_numKameScore.LinkRateEffect( this, &CRBResultGui::OnKamePointEffectEnd );
////	m_numKameBonusScore.LinkRateEffect( this, &CRBResultGui::OnKameBonusPointEffectEnd );
////	m_numTsuruScore.LinkRateEffect( this, &CRBResultGui::OnTsuruPointEffectEnd );
////	m_numTsuruBonusScore.LinkRateEffect( this, &CRBResultGui::OnTsuruBonusPointEffectEnd );
////
////	Show( false );
////	Lower();
////
////	GetNtlGuiManager()->AddUpdateFunc( this );
////
////	LinkMsg( g_EventRBBattleMatchFinish );
////	LinkMsg( g_EventResize );	
////	LinkMsg( g_EventFlashFinishNotify );
////
////	NTL_RETURN( TRUE );
////}
////
////VOID CRBResultGui::Destroy(VOID)
////{
////	GetNtlGuiManager()->RemoveUpdateFunc( this );
////
////	UnLinkMsg( g_EventRBBattleMatchFinish );
////	UnLinkMsg( g_EventResize );
////	UnLinkMsg( g_EventFlashFinishNotify );
////
////	CNtlPLGui::DestroyComponents();
////	CNtlPLGui::Destroy();
////}
////
////VOID CRBResultGui::Update( RwReal fElapsed )
////{
////	if( m_pThis->IsVisible() )
////	{
////		if( m_fWaitTime > 0.0f )
////		{
////			m_fWaitTime -= fElapsed;
////
////			if( m_pstbWaitTime->IsVisible() )
////				m_pstbWaitTime->Format( GetDisplayStringManager()->GetString( DST_RB_RESULT_WAIT_TIME ), (RwUInt32)( m_fWaitTime + 0.99f ) );
////		}
////
////		m_pflaGrade->Update( fElapsed );
////
////		m_numKameStageScore.Update( fElapsed );
////		m_numTsuruStageScore.Update( fElapsed );
////		m_numKameScore.Update( fElapsed );
////		m_numKameBonusScore.Update( fElapsed );
////		m_numTsuruScore.Update( fElapsed );
////		m_numTsuruBonusScore.Update( fElapsed );
////		m_numColon.Update( fElapsed );
////	}	
////}
////
////VOID CRBResultGui::Show( bool bOpen )
////{
////	CNtlPLGui::Show( bOpen );
////
////	if( bOpen )
////		CenterAlign( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );
////}
////
////VOID CRBResultGui::HandleEvents( RWS::CMsg& msg )
////{
////	if( msg.Id == g_EventRBBattleMatchFinish )
////	{
////		SetResultData( msg.pData );
////
////		Show( true );
////
////		m_pstbKameName->Show( false );
////		m_pstbTsuruName->Show( false );
////		m_numKameStageScore.Show( FALSE );
////		m_numTsuruStageScore.Show( FALSE );
////		m_numKameScore.Show( FALSE );
////		m_numKameBonusScore.Show( FALSE );
////		m_numTsuruScore.Show( FALSE );
////		m_numTsuruBonusScore.Show( FALSE );
////		m_numColon.Show( FALSE );
////		m_pstbKameRecord->Show( false );
////		m_pstbTsuruRecord->Show( false );				
////		m_pstbResultText->Show( false );
////
////		m_pbtnOK->Enable( false );
////	}
////	else if( msg.Id == g_EventResize )
////	{
////		SNtlPLEventResize* pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
////
////		CenterAlign( pData->iWidht, pData->iHeight );
////	}
////	else if( msg.Id == g_EventFlashFinishNotify )
////	{
////		SDboEventFlashFinishNotify* pData = reinterpret_cast<SDboEventFlashFinishNotify*>( msg.pData );
////		if( !strcmp( pData->pcFlashFile, "RankBattleResult" ) )
////		{
////			m_pflaGrade->PlayMovie( TRUE );
////		}
////	}
////}
////
////VOID CRBResultGui::SetResultData( VOID* pData )
////{
////	SNtlEventRBBattleMatchFinish* pMatchFinishInfo = reinterpret_cast<SNtlEventRBBattleMatchFinish*>( pData );
////	sRANKBATTLE_MATCH_FINISH_INFO* pFinishInfo = reinterpret_cast<sRANKBATTLE_MATCH_FINISH_INFO*>( pMatchFinishInfo->pMatchResult );
////	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
////	sRANKBATTLE_TBLDAT* pTblData = pRankBattle->GetTblData();
////
////	// Grade 
////	if( pRankBattle->GetResult() == CNtlRankBattle::WIN )
////	{
////		RwUInt16 wScore;
////
////		if( pRankBattle->IsRoomOwner() )
////			wScore = (RwUInt16)( pFinishInfo->sOwnerInfo.fPoint + pFinishInfo->sOwnerInfo.fBonusPoint );
////		else
////			wScore = (RwUInt16)( pFinishInfo->sChallengerInfo.fPoint + pFinishInfo->sChallengerInfo.fBonusPoint );
////
////		if( wScore >= pTblData->chResultExcellent )
////			m_pflaGrade->Load( "TMQ_result_Excellent.swf" );			
////		else if( wScore >= pTblData->chResultGreate )
////			m_pflaGrade->Load( "TMQ_result_Great.swf" );
////		else
////			m_pflaGrade->Load( "TMQ_result_Good.swf" );			
////	}
////	else 
////	{
////		m_pflaGrade->Load( "TMQ_result_Bad.swf" );			
////	}
////
////	// MatchScore, Point
////	RwUInt16 wOwnerStraightWinCount = 0;
////	RwUInt16 wOwnerPerfectWinCount = 0;
////	RwUInt16 wChallengerStraightWinCount = 0;
////	RwUInt16 wChallengerPerfectWinCount = 0;
////
////	if( pRankBattle->GetResult() == CNtlRankBattle::WIN )
////	{
////		if( pRankBattle->IsRoomOwner() )
////		{
////			wOwnerStraightWinCount = pFinishInfo->sWinerRecord.wStraightWinCount;
////			wOwnerPerfectWinCount = pFinishInfo->sWinerRecord.wStraightKOWinCount;	
////		}
////		else
////		{
////			wChallengerStraightWinCount = pFinishInfo->sWinerRecord.wStraightWinCount;
////			wChallengerPerfectWinCount = pFinishInfo->sWinerRecord.wStraightKOWinCount;
////		}
////	}
////	else
////	{
////		if( pRankBattle->IsRoomOwner() )
////		{
////			wChallengerStraightWinCount = pFinishInfo->sWinerRecord.wStraightWinCount;
////			wChallengerPerfectWinCount = pFinishInfo->sWinerRecord.wStraightKOWinCount;
////		}
////		else
////		{
////			wOwnerStraightWinCount = pFinishInfo->sWinerRecord.wStraightWinCount;
////			wOwnerPerfectWinCount = pFinishInfo->sWinerRecord.wStraightKOWinCount;
////		}
////	}
////
////	if( pRankBattle->IsRoomOwner() )
////	{
////		m_pstbKameName->SetTextColor( RANKBATTLE_COLOR_YELLOW );
////		m_pstbKameRecord->SetTextColor( RANKBATTLE_COLOR_YELLOW );
////		m_pstbTsuruName->SetTextColor( RANKBATTLE_COLOR_WHITE );
////		m_pstbTsuruRecord->SetTextColor( RANKBATTLE_COLOR_WHITE );
////
////		m_pstbKameName->SetText( pRankBattle->GetMyTeamName().c_str() );		
////		m_pstbTsuruName->SetText( pRankBattle->GetEnemyTeamName().c_str() );		
////	}
////	else
////	{
////		m_pstbKameName->SetTextColor( RANKBATTLE_COLOR_WHITE );
////		m_pstbKameRecord->SetTextColor( RANKBATTLE_COLOR_WHITE );
////		m_pstbTsuruName->SetTextColor( RANKBATTLE_COLOR_YELLOW );
////		m_pstbTsuruRecord->SetTextColor( RANKBATTLE_COLOR_YELLOW );
////
////		m_pstbKameName->SetText( pRankBattle->GetEnemyTeamName().c_str() );
////		m_pstbTsuruName->SetText( pRankBattle->GetMyTeamName().c_str() );	
////	}
////
////	m_pstbKameRecord->Format( GetDisplayStringManager()->GetString( DST_RB_RESULT_RECORD_INFO ), 
////							  pFinishInfo->sOwnerInfo.wWinCount + pFinishInfo->sOwnerInfo.wLoseCount + pFinishInfo->sOwnerInfo.wDrawCount,
////							  pFinishInfo->sOwnerInfo.wWinCount, pFinishInfo->sOwnerInfo.wLoseCount, wOwnerStraightWinCount, wOwnerPerfectWinCount );
////	m_pstbTsuruRecord->Format( GetDisplayStringManager()->GetString( DST_RB_RESULT_RECORD_INFO ), 
////							   pFinishInfo->sChallengerInfo.wWinCount + pFinishInfo->sChallengerInfo.wLoseCount + pFinishInfo->sChallengerInfo.wDrawCount,
////							   pFinishInfo->sChallengerInfo.wWinCount, pFinishInfo->sChallengerInfo.wLoseCount, wChallengerStraightWinCount, wChallengerPerfectWinCount );
////
////	m_numKameStageScore.SetNumber( pFinishInfo->sOwnerInfo.byScore );
////	m_numKameScore.SetNumber( (RwInt32)pFinishInfo->sOwnerInfo.fPoint );
////	m_numKameBonusScore.SetNumber( (RwInt32)pFinishInfo->sOwnerInfo.fBonusPoint );
////	m_numTsuruStageScore.SetNumber( pFinishInfo->sChallengerInfo.byScore );
////	m_numTsuruScore.SetNumber( (RwInt32)pFinishInfo->sChallengerInfo.fPoint );
////	m_numTsuruBonusScore.SetNumber( (RwInt32)pFinishInfo->sChallengerInfo.fBonusPoint );
////
////	// WaitTime
////	m_fWaitTime = (RwReal)pRankBattle->GetTblData()->dwMatchFinishTime; 
////}
////
////VOID CRBResultGui::OnFlashMovieEnd( gui::CComponent* pComponent )
////{
////	m_numKameStageScore.EffectRate( 2.0f, 1.0f, 0.5f );
////	m_numKameStageScore.Show( TRUE );
////}
////
////VOID CRBResultGui::OnPaint(VOID)
////{
////	m_numKameStageScore.Render();
////	m_numKameScore.Render();
////	m_numKameBonusScore.Render();
////	m_numTsuruStageScore.Render();
////	m_numTsuruScore.Render();
////	m_numTsuruBonusScore.Render();
////	m_numColon.Render();
////}
////
////VOID CRBResultGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
////{
////	CRectangle rtScreen = m_pThis->GetScreenRect();
////
////	m_numKameStageScore.SetPosition( rtScreen.left, rtScreen.top );
////	m_numKameScore.SetPosition( rtScreen.left, rtScreen.top );
////	m_numKameBonusScore.SetPosition( rtScreen.left, rtScreen.top );
////	m_numTsuruStageScore.SetPosition( rtScreen.left, rtScreen.top );
////	m_numTsuruScore.SetPosition( rtScreen.left, rtScreen.top );
////	m_numTsuruBonusScore.SetPosition( rtScreen.left, rtScreen.top );
////	m_numColon.SetPosition( rtScreen.left, rtScreen.top );
////}
////
////RwInt32 CRBResultGui::OnKameScoreEffectEnd(VOID)
////{
////	m_numColon.EffectRate( 2.0f, 1.0f, 0.5f );
////	m_numColon.Show( TRUE );		
////	return 1;
////}
////
////RwInt32 CRBResultGui::OnColonEffectEnd(VOID)
////{
////	m_numTsuruStageScore.EffectRate( 2.0f, 1.0f, 0.5f );
////	m_numTsuruStageScore.Show( TRUE );	
////	return 1;
////}
////
////RwInt32 CRBResultGui::OnTsuruScoreEffectEnd(VOID)
////{
////	m_numKameScore.EffectRate( 2.0f, 1.0f, 0.5f );
////	m_numKameScore.Show( TRUE );
////	m_pstbResultText->Show( true );
////
////	return 1;
////}
////
////RwInt32 CRBResultGui::OnKamePointEffectEnd(VOID)
////{
////	if( m_numKameBonusScore.GetNumber() )
////	{
////		m_numKameBonusScore.EffectRate( 2.0f, 1.0f, 0.5f );
////		m_numKameBonusScore.Show( TRUE );	
////	}
////	else
////	{
////		OnKameBonusPointEffectEnd();
////	}
////
////	return 1;
////}
////
////RwInt32 CRBResultGui::OnKameBonusPointEffectEnd(VOID)
////{
////	m_numTsuruScore.EffectRate( 2.0f, 1.0f, 0.5f );
////	m_numTsuruScore.Show( TRUE );
////
////	return 1;
////}
////
////RwInt32 CRBResultGui::OnTsuruPointEffectEnd(VOID)
////{
////	if( m_numTsuruBonusScore.GetNumber() )
////	{
////		m_numTsuruBonusScore.EffectRate( 2.0f, 1.0f, 0.5f );
////		m_numTsuruBonusScore.Show( TRUE );
////	}
////	else
////	{
////		OnTsuruBonusPointEffectEnd();
////	}
////
////	return 1;
////}
////
////RwInt32 CRBResultGui::OnTsuruBonusPointEffectEnd(VOID)
////{
////	m_pstbKameName->Show( true );
////	m_pstbTsuruName->Show( true );
////	m_pstbKameRecord->Show( true );
////	m_pstbTsuruRecord->Show( true );	
////	m_pbtnOK->Enable( true );
////
////	return 1;
////}
////
