#include "precomp_dboclient.h"
#include "TBMinorMatchResultGui.h"

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

#define BONUS_LINE_1_TEXT_X		69
#define BONUS_LINE_1_TEXT_Y		206
#define BONUS_LINE_1_NUMBER_X	328
#define BONUS_LINE_1_NUMBER_Y	206
#define BONUS_LINE_2_TEXT_X		69
#define BONUS_LINE_2_TEXT_Y		234
#define BONUS_LINE_2_NUMBER_X	328	
#define BONUS_LINE_2_NUMBER_Y	235
#define BATTLE_POINT_RATE		1.0f//0.696f	
#define TOTAL_POINT_RATE		1.0f//0.645f
#define BONUS_POINT_RATE		1.0f//0.842f

CTBMinorMatchResultGui::CTBMinorMatchResultGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CTBMinorMatchResultGui::~CTBMinorMatchResultGui(VOID)
{

}

VOID CTBMinorMatchResultGui::Init(VOID)
{
	m_fWaitTime = 0.0f;
	m_nMouseOnIndex = -1;	
	m_eResult = DRAW;

	for( RwInt32 i = 0 ; i < MAX_BUDOKAI_REWARD_ITEM ; ++i )
	{
		m_pstbStackCount[i] = NULL;
		m_pRewardItem[i] = NULL;
	}
}

RwBool CTBMinorMatchResultGui::Create(VOID)
{
	NTL_FUNCTION( "CTBMinorMatchResultGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\BudokaiResult.srf", "gui\\BudokaiMinorResult.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain" ) );
	m_pflaResult = reinterpret_cast<gui::CFlash*>( GetComponent( "flaResult" ) );
	m_pflaBar = reinterpret_cast<gui::CFlash*>( GetComponent( "flaBar" ) );
	m_pstbWinner = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbWinner" ) );

	m_pThis->SetPriority( dDIALOGPRIORITY_RESULT );

	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 i = 0 ; i < MAX_BUDOKAI_REWARD_ITEM ; ++i )
	{
		m_rtRewardItem[i].SetRectWH( 706, 461, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
		m_surIcon[i].SetRect( m_rtRewardItem[i] );		
		m_surIcon[i].Show( FALSE );
		m_surIconBack[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "BudokaiResult.srf", "srfSlotBack" ) );
		m_surIconBack[i].SetPosition( m_rtRewardItem[i].left, m_rtRewardItem[i].top );
		m_surIconBack[i].Show( FALSE );
	}

	m_surFocusSlot.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_surFocusSlot.Show( FALSE );

	m_numBattlePoint.Create( ND_LEFT );
	m_numBattlePoint.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_0" ) );
	m_numBattlePoint.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_1" ) );
	m_numBattlePoint.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_2" ) );
	m_numBattlePoint.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_3" ) );
	m_numBattlePoint.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_4" ) );
	m_numBattlePoint.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_5" ) );
	m_numBattlePoint.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_6" ) );
	m_numBattlePoint.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_7" ) );
	m_numBattlePoint.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_8" ) );
	m_numBattlePoint.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScoreNum_9" ) );
	m_numBattlePoint.SetParentPos( 787, 353 );
	m_numBattlePoint.SetPosition( rtScreen.left, rtScreen.top );

	m_numTotalPoint.Create( ND_LEFT );
	m_numTotalPoint.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_0" ) );
	m_numTotalPoint.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_1" ) );
	m_numTotalPoint.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_2" ) );
	m_numTotalPoint.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_3" ) );
	m_numTotalPoint.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_4" ) );
	m_numTotalPoint.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_5" ) );
	m_numTotalPoint.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_6" ) );
	m_numTotalPoint.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_7" ) );
	m_numTotalPoint.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_8" ) );
	m_numTotalPoint.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_9" ) );
	m_numTotalPoint.SetParentPos( 789, 293 );
	m_numTotalPoint.SetPosition( rtScreen.left, rtScreen.top );

	m_numKOBonus.Create( ND_LEFT, NVD_TOP, 0, dNCFLAG_PLUS );
	m_numKOBonus.SetSurface( NC_PLUS, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusPlus" ) );
	m_numKOBonus.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_0" ) );
	m_numKOBonus.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_1" ) );
	m_numKOBonus.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_2" ) );
	m_numKOBonus.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_3" ) );
	m_numKOBonus.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_4" ) );
	m_numKOBonus.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_5" ) );
	m_numKOBonus.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_6" ) );
	m_numKOBonus.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_7" ) );
	m_numKOBonus.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_8" ) );
	m_numKOBonus.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_9" ) );
	m_numKOBonus.SetParentPos( 787, 382 );
	m_numKOBonus.SetPosition( rtScreen.left, rtScreen.top );

	m_numWinBonus.Create( ND_LEFT, NVD_TOP, 0, dNCFLAG_PLUS );
	m_numWinBonus.SetSurface( NC_PLUS, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusPlus" ) );
	m_numWinBonus.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_0" ) );
	m_numWinBonus.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_1" ) );
	m_numWinBonus.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_2" ) );
	m_numWinBonus.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_3" ) );
	m_numWinBonus.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_4" ) );
	m_numWinBonus.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_5" ) );
	m_numWinBonus.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_6" ) );
	m_numWinBonus.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_7" ) );
	m_numWinBonus.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_8" ) );
	m_numWinBonus.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_9" ) );
	m_numWinBonus.SetParentPos( 787, 410 );
	m_numWinBonus.SetPosition( rtScreen.left, rtScreen.top );

	m_slotFSCallBack = m_pflaResult->SigFSCallBack().Connect( this, &CTBMinorMatchResultGui::OnFSCallBack );
	m_slotPaint = m_pflaResult->SigPaint().Connect( this, &CTBMinorMatchResultGui::OnPaint );
	m_slotMove = m_pflaResult->SigMove().Connect( this, &CTBMinorMatchResultGui::OnMove );
	m_slotMouseMove = m_pflaResult->SigMouseMove().Connect( this, &CTBMinorMatchResultGui::OnMouseMove );
	m_slotMouseLeave = m_pflaResult->SigMouseLeave().Connect( this, &CTBMinorMatchResultGui::OnMouseLeave );

	CNtlPLGui::Show( false );
	Lower();

	GetNtlGuiManager()->AddUpdateFunc( this );

	LinkMsg( g_EventMinorMatchMatchFinish );
	LinkMsg( g_EventResize );	
	LinkMsg( g_EventFlashFinishNotify );

	NTL_RETURN( TRUE );
}

VOID CTBMinorMatchResultGui::Destroy(VOID)
{
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	OnMouseLeave( NULL ); // 인포윈도우 닫기.

	DeleteAllRewardItem(); // 아이템 아이콘 삭제.

	UnLinkMsg( g_EventMinorMatchMatchFinish );
	UnLinkMsg( g_EventResize );
	UnLinkMsg( g_EventFlashFinishNotify );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CTBMinorMatchResultGui::Update( RwReal fElapsed )
{
	if( m_fWaitTime > 0.0f )
	{
		m_fWaitTime -= fElapsed;

		if( m_fWaitTime <= TB_MATCH_RESULT_REMAIN_TIME && !CTBGui::GetInstance()->m_pCounter->IsShow() )
			CTBGui::GetInstance()->SetCounterTime( m_fWaitTime );
	}
	else if( IsShow() )
		Show( false );

	if( IsShow() )
	{
		m_pflaResult->Update( fElapsed );
		m_pflaBar->Update( fElapsed );

		m_numBattlePoint.Update( fElapsed );
		m_numKOBonus.Update( fElapsed );
		m_numWinBonus.Update( fElapsed );
		m_numTotalPoint.Update( fElapsed );		
	}	
}

VOID CTBMinorMatchResultGui::Show( bool bOpen )
{
	CNtlPLGui::Show( bOpen );

	if( bOpen )
	{
		CalcPosition( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );
		CTBGui::GetInstance()->m_pResultNarration->SetData( "TB_MatchFinish_npc.swf", GetDisplayStringManager()->GetString( "DST_BUDOKAI_RESULT_MESSAGE" ) );		
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

VOID CTBMinorMatchResultGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventMinorMatchMatchFinish )		
	{
		if( SetResultData( msg.pData ) )
			HideResultComopnent();						
	}
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		CalcPosition( pData->iWidht, pData->iHeight );
	}
	else if( msg.Id == g_EventFlashFinishNotify )
	{
		SDboEventFlashFinishNotify* pData = reinterpret_cast<SDboEventFlashFinishNotify*>( msg.pData );

		if( !strcmp( pData->pcFlashFile, "MinorMatchResult" ) )
		{
			Show( true );	
			m_pflaResult->PlayMovie( TRUE );
			m_pflaBar->PlayMovie( TRUE );
		}	
	}
}

RwBool CTBMinorMatchResultGui::SetResultData( VOID* pData )
{
	RwUInt32 uiBasePoint = 0;
	RwUInt32 uiKOBonus = 0;
	RwUInt32 uiWinBonus = 0;		

	CNtlTBudokai* pTBudokai = GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai();
	sBUDOKAI_TBLINFO* pBudokaiData = pTBudokai->GetBudokaiTblData();
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );	

	// 
	m_pflaResult->RestartMovie();
	m_pflaBar->RestartMovie();

	// 슬롯 삭제.
	DeleteAllRewardItem();

	SNtlEventMinorMatchMatchFinish* pFinishData = reinterpret_cast<SNtlEventMinorMatchMatchFinish*>( pData );		
	sMINORMATCH_REWARD* pReward = NULL;

	if( pTBWorldConcept->IsPartyBattle() )
	{
		if( GetBudokaiType( GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetBudokaiMainState() ) == BUDOKAI_TYPE_JUNIOR )
			pReward = &pBudokaiData->sJuniorTeamReward.sMinorMatch;
		else
			pReward = &pBudokaiData->sTeamReward.sMinorMatch;
	}
	else
	{
		if( GetBudokaiType( GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetBudokaiMainState() ) == BUDOKAI_TYPE_JUNIOR )
			pReward = &pBudokaiData->sJuniorIndividualReward.sMinorMatch;
		else
			pReward = &pBudokaiData->sIndividualReward.sMinorMatch;
	}

	uiBasePoint = pReward->dwBaseMudosaPoint;
	uiKOBonus = pReward->dwKillCountPoint * pFinishData->byKillCount;	

	if( pTBWorldConcept->GetMyTeam() )
	{// 참가자
		if( pFinishData->byMatchResult == MATCH_RESULT_WIN )
		{
			if( pTBWorldConcept->GetMyTeamType() == pFinishData->wMatchWinner )
			{
				uiWinBonus = pReward->dwWinnerMudosaPoint;
				SetRewardItem( 0, pReward->winnerItem, pReward->byWinerItemStackCount );	
				SetFlashResult( WIN );								
			}
			else 
			{
				SetRewardItem( 0, pReward->loserItem, pReward->byLoserItemStackCount );
				SetFlashResult( LOSE );				
			}
		}
		else if( pFinishData->byMatchResult == MATCH_RESULT_DRAW )
		{
			SetRewardItem( 0, pReward->loserItem, pReward->byLoserItemStackCount );
			SetFlashResult( DRAW );
		}	
		else if( pFinishData->byMatchResult == MATCH_RESULT_DOJO_RECOMMENDER )
		{
			uiWinBonus = pReward->dwWinnerMudosaPoint;
			SetRewardItem( 0, pReward->winnerItem, pReward->byWinerItemStackCount );	
			SetFlashResult( DOJO_RECOMMENDED );
		}
		else if( pFinishData->byMatchResult == MATCH_RESULT_UNEARNED_WIN )
		{
			uiWinBonus = pReward->dwWinnerMudosaPoint;
			SetRewardItem( 0, pReward->winnerItem, pReward->byWinerItemStackCount );	
			SetFlashResult( UNEARNED_WIN );
		}
	}
	else
	{// 관전자
		uiWinBonus = pReward->dwWinnerMudosaPoint;
		SetRewardItem( 0, pReward->winnerItem, pReward->byWinerItemStackCount );			
		SetFlashResult( NONE );
				
		stTBudokaiTeam* pWinnerTeam = pTBWorldConcept->FindTeam( pFinishData->wMatchWinner );
		if( pWinnerTeam )
			m_pstbWinner->SetText( pWinnerTeam->wstrTeamName.c_str() );
	}

	m_pflaResult->PlayMovie( FALSE );
	m_pflaBar->PlayMovie( FALSE );		

	m_numBattlePoint.SetNumber( uiBasePoint );
	m_numKOBonus.SetNumber( uiKOBonus );
	m_numWinBonus.SetNumber( uiWinBonus );
	m_numTotalPoint.SetNumber( uiBasePoint + uiKOBonus + uiWinBonus );

	// WaitTime
	m_fWaitTime = (RwReal)pTBWorldConcept->GetMatchTbl()->dwMatchFinishTime + 0.99f; 
	return TRUE;
}

VOID CTBMinorMatchResultGui::SetRewardItem( RwInt32 nSlotIdx, RwUInt32 tblidx, RwUInt8 byStackCount )
{
	CItemTable* pItemTable = API_GetTableContainer()->GetItemTable();
	sITEM_TBLDAT* pTableData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData( tblidx ) );
	if( pTableData )
	{
		m_surIcon[nSlotIdx].SetTexture( Logic_CreateTexture( const_cast<RwChar*>( pTableData->szIcon_Name ) ) );
		if( pTableData->byMax_Stack > 1 )
			CreateStackNumber( nSlotIdx, byStackCount );			

		m_pRewardItem[nSlotIdx] = pTableData;
		//m_surIconBack[nSlotIdx].Show( TRUE );
	}
}

VOID CTBMinorMatchResultGui::DeleteRewardItem( RwInt32 nSlotIdx )
{
	m_pRewardItem[nSlotIdx] = NULL;
	m_surIconBack[nSlotIdx].Show( FALSE );
	Logic_DeleteTexture( m_surIcon[nSlotIdx].GetTexture() );
	m_surIcon[nSlotIdx].UnsetTexture();
	m_surIcon[nSlotIdx].Show( FALSE );
	DeleteStackNumber( nSlotIdx );	
}

VOID CTBMinorMatchResultGui::DeleteAllRewardItem(VOID)
{
	for( RwInt32 i = 0 ; i < MAX_BUDOKAI_REWARD_ITEM ; ++i )
	{
		DeleteRewardItem( i );
	}
}

RwBool CTBMinorMatchResultGui::CreateStackNumber( RwInt32 nSlotIdx, RwUInt8 uiStackCount )
{
	NTL_FUNCTION( "CTBMinorMatchResultGui::CreateStackNubmer" );

	CRectangle rect;
	rect.SetRect( m_rtRewardItem[nSlotIdx].left, m_rtRewardItem[nSlotIdx].bottom - DBOGUI_STACKNUM_HEIGHT,
		m_rtRewardItem[nSlotIdx].left + DBOGUI_STACKNUM_WIDTH, m_rtRewardItem[nSlotIdx].bottom );
	m_pstbStackCount[nSlotIdx] = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN );
	if( !m_pstbStackCount[nSlotIdx] )
		NTL_RETURN( FALSE );

	m_pstbStackCount[nSlotIdx]->CreateFontStd( DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR );
	m_pstbStackCount[nSlotIdx]->SetEffectMode( DBOGUI_STACKNUM_FONTEFFECTMODE );
	m_pstbStackCount[nSlotIdx]->SetText( uiStackCount );
	m_pstbStackCount[nSlotIdx]->Enable( false );
	m_pstbStackCount[nSlotIdx]->Show( false );

	NTL_RETURN( TRUE );
}

VOID CTBMinorMatchResultGui::DeleteStackNumber( RwInt32 nSlotIdx )
{
	if( m_pstbStackCount[nSlotIdx] )
		NTL_DELETE( m_pstbStackCount[nSlotIdx] );
}

VOID CTBMinorMatchResultGui::HideResultComopnent(VOID)
{
	m_numBattlePoint.Show( FALSE );
	m_numKOBonus.Show( FALSE );
	m_numWinBonus.Show( FALSE );
	m_numTotalPoint.Show( FALSE );
	m_surFocusSlot.Show( FALSE );	
}

VOID CTBMinorMatchResultGui::SetMouseOnIndex( RwInt32 nIndex )
{
	NTL_ASSERT( nIndex < MAX_BUDOKAI_REWARD_ITEM, "CTBMinorMatchResultGui::SetMouseOnIndex : Invalid Array Index" );

	m_nMouseOnIndex = nIndex;

	if( nIndex >= 0 )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		m_surFocusSlot.SetPosition( rtScreen.left + m_rtRewardItem[nIndex].left - 2, rtScreen.top + m_rtRewardItem[nIndex].top - 2 );
		m_surFocusSlot.Show( TRUE );
	}
	else
		m_surFocusSlot.Show( FALSE );

}

RwInt32 CTBMinorMatchResultGui::GetRewardSlotIndex( RwInt32 nX, RwInt32 nY )
{
	for( RwInt32 i = 0 ; i < MAX_BUDOKAI_REWARD_ITEM ; ++i )
	{
		if( m_pRewardItem[i] &&
			m_rtRewardItem[i].PtInRect( nX, nY ) )
			return i;
	}

	return -1;
}

VOID CTBMinorMatchResultGui::SetFlashResult( FLASHRESULT eResult )
{
	m_pflaResult->Invoke( "Conclusion", "%d", eResult );
	m_eResult = eResult;

	if( m_eResult == WIN || m_eResult == UNEARNED_WIN || m_eResult == DOJO_RECOMMENDED )
		CNtlSLEventGenerator::SobAnimPlay( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), SOC_HAPPY, FALSE, 0.0f );
	else if( m_eResult == LOSE || m_eResult == DRAW )
		CNtlSLEventGenerator::SobAnimPlay( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), SOC_SAD, FALSE, 0.0f );
}

VOID CTBMinorMatchResultGui::OnFSCallBack( const RwChar* szCommand, const RwChar* szArgs )
{
	if( !strcmp( szCommand, "TextEnd1" ) )	// Item
	{
		for( RwInt32 i = 0 ; i < MAX_BUDOKAI_REWARD_ITEM ; ++i )
		{
			if( m_pRewardItem[i] )
			{
				m_surIcon[i].Show( TRUE );
				m_surIconBack[i].Show( TRUE );
				if( m_pstbStackCount[i] )
					m_pstbStackCount[i]->Show( true );
			}
		}
	}
	else if( !strcmp( szCommand, "TextEnd2" ) )	// WinBonus
	{
		m_numWinBonus.EffectRate( BONUS_POINT_RATE * 8.0f, BONUS_POINT_RATE, 0.5f );
		m_numWinBonus.Show( TRUE );			
	}
	else if( !strcmp( szCommand, "TextEnd3" ) )	// KOBonus
	{
		m_numKOBonus.EffectRate( BONUS_POINT_RATE * 8.0f, BONUS_POINT_RATE, 0.5f );
		m_numKOBonus.Show( TRUE );		
	}
	else if( !strcmp( szCommand, "TextEnd4" ) )	// BattleScore
	{
		m_numBattlePoint.EffectRate( BATTLE_POINT_RATE * 8.0f, BATTLE_POINT_RATE, 0.5f );
		m_numBattlePoint.Show( TRUE );
	}
	else if( !strcmp( szCommand, "TextEnd5" ) )	// TotalScore
	{
		m_numTotalPoint.EffectRate( TOTAL_POINT_RATE * 8.0f, TOTAL_POINT_RATE, 0.5f );
		m_numTotalPoint.Show( TRUE );
	}	
	else if( !strcmp( szCommand, "TextWinnerStart" ) ) // The Winner
	{
		m_pstbWinner->Show( true );
	}
}

VOID CTBMinorMatchResultGui::OnPaint(VOID)
{
	for( RwInt32 i = 0 ; i < MAX_BUDOKAI_REWARD_ITEM ; ++i )
	{
		m_surIconBack[i].Render();
		m_surIcon[i].Render();		
	}

	m_surFocusSlot.Render();

	m_numWinBonus.Render();
	m_numKOBonus.Render();	
	m_numBattlePoint.Render();
	m_numTotalPoint.Render();
}

VOID CTBMinorMatchResultGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_numWinBonus.SetPosition( rtScreen.left, rtScreen.top );
	m_numKOBonus.SetPosition( rtScreen.left, rtScreen.top );
	m_numBattlePoint.SetPosition( rtScreen.left, rtScreen.top );
	m_numTotalPoint.SetPosition( rtScreen.left, rtScreen.top );

	for( RwInt32 i = 0 ; i < MAX_BUDOKAI_REWARD_ITEM ; ++i )
	{
		m_surIconBack[i].SetPosition( rtScreen.left + m_rtRewardItem[i].left, rtScreen.top +m_rtRewardItem[i].top );
		m_surIcon[i].SetPosition( rtScreen.left + m_rtRewardItem[i].left, rtScreen.top +m_rtRewardItem[i].top );
	}

	m_surFocusSlot.SetPosition( rtScreen.left, rtScreen.top );
}

VOID CTBMinorMatchResultGui::OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY )
{
	RwInt32 nMouseOnIdx = GetRewardSlotIndex( nX, nY );

	if( nMouseOnIdx >= 0 )
	{
		if( m_nMouseOnIndex != nMouseOnIdx )
		{
			CRectangle rtScreen = m_pThis->GetScreenRect();
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_TABLE_ITEM, m_rtRewardItem[nMouseOnIdx].left + rtScreen.left, m_rtRewardItem[nMouseOnIdx].top + rtScreen.top, m_pRewardItem[nMouseOnIdx], DIALOG_UNKNOWN );
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

VOID CTBMinorMatchResultGui::OnMouseLeave( gui::CComponent* pComponent )
{
	if( m_nMouseOnIndex >= 0 )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		SetMouseOnIndex( -1 );
	}
}

VOID CTBMinorMatchResultGui::CalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	CenterAlign( nScreenWidth, nScreenHeight );

	RwInt32 nBarLeft = -m_pThis->GetPosition().left;
	CRectangle rtBar = m_pflaBar->GetPosition();

	m_pflaBar->SetPosition( CRectangle( nBarLeft, rtBar.top, nBarLeft + nScreenWidth, rtBar.bottom ) );	
}
