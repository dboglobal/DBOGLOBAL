#include "precomp_dboclient.h"
#include "RankBattleBoard.h"

// core
#include "NtlDebug.h"
#include "NtlPLDef.h"

// shared
#include "NtlTimeQuest.h"
#include "NtlRankBattle.h"

// simulation
#include "DboEvent.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Dbo
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"
#include "InfoWndManager.h"
#include "RankBoardGui.h"

////////////////////////////////////////////////////////////////////////////////////
// CRankBattleSubject
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// CRankBattleRowItem
////////////////////////////////////////////////////////////////////////////////////

/**
* \brief 생성자
*/
CRankBattleRowItem::CRankBattleRowItem() 
: m_pStbRank(NULL)
, m_pStbName(NULL)
, m_pStbLevel(NULL)
, m_pPanClass(NULL)
, m_pStbGuildName(NULL)
, m_pStbPoint(NULL)
, m_pStbStraightWin(NULL)
, m_pStbCompare(NULL)
, m_pPanCompareMark(NULL)
, m_nRank(0)
, m_pDlgItem(NULL)
, m_byClass(INVALID_BYTE)
{

}

/**
* \brief 소멸자
*/
CRankBattleRowItem::~CRankBattleRowItem() 
{

}

/**
* \brief Create
* \param pParentGui		(gui::CComponent*) 부모 GUI의 포인터
* \param nTop			(RwInt32) 현재의 아이템이 시작될 Y 좌표
*/
VOID CRankBattleRowItem::Create( gui::CComponent* pParentGui , RwInt32 nTop ) 
{
	// 순위의 위치
	CRectangle rect;

	rect.SetRectWH( 0, nTop, 631, 25 );
	m_pDlgItem = NTL_NEW gui::CDialog( &rect, pParentGui, GetNtlGuiManager()->GetSurfaceManager() );
	
	// 순위(숫자)
	rect.SetRectWH( 6, 0, 43, 25 );
	m_pStbRank = NTL_NEW gui::CStaticBox( &rect, m_pDlgItem, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbRank->CreateFontStd( DETAIL_FONT , dRANKBOARD_RANKBATTLE_ROWITEM_FONT_HEIGHT, 0 );

	// 이름(문자열)
	rect.SetRectWH( 61, 0, 120, 25 );
	m_pStbName = NTL_NEW gui::CStaticBox( &rect, m_pDlgItem, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbName->CreateFontStd( DETAIL_FONT, dRANKBOARD_RANKBATTLE_ROWITEM_FONT_HEIGHT, 0 );
	
	// 레벨(숫자)
	rect.SetRectWH( 199, 0, 30, 25 );
	m_pStbLevel = NTL_NEW gui::CStaticBox( &rect, m_pDlgItem, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbLevel->CreateFontStd( DETAIL_FONT, dRANKBOARD_RANKBATTLE_ROWITEM_FONT_HEIGHT, 0 );

	// 직업(그림)
	rect.SetRectWH( 252, 2, 19, 29 );
	m_pPanClass = NTL_NEW gui::CPanel( &rect, m_pDlgItem, GetNtlGuiManager()->GetSurfaceManager() );
	
	// 길드이름(문자열)
	rect.SetRectWH( 293, 0, 136, 25 );
	m_pStbGuildName = NTL_NEW gui::CStaticBox( &rect, m_pDlgItem, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbGuildName->CreateFontStd( DETAIL_FONT, dRANKBOARD_RANKBATTLE_ROWITEM_FONT_HEIGHT, 0 );

	// 점수(숫자)
	rect.SetRectWH( 432, 0, 66, 25 );
	m_pStbPoint = NTL_NEW gui::CStaticBox( &rect, m_pDlgItem, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbPoint->CreateFontStd( DETAIL_FONT, dRANKBOARD_RANKBATTLE_ROWITEM_FONT_HEIGHT, 0 );
	
	// 연승(숫자)
	rect.SetRectWH( 514, 0, 24, 25 );
	m_pStbStraightWin = NTL_NEW gui::CStaticBox( &rect, m_pDlgItem, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbStraightWin->CreateFontStd( DETAIL_FONT, dRANKBOARD_RANKBATTLE_ROWITEM_FONT_HEIGHT, 0 );

	// 비교등수(숫자)
	rect.SetRectWH( 518, 0, 72, 25 );
	m_pStbCompare = NTL_NEW gui::CStaticBox( &rect, m_pDlgItem, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_RIGHT );
	m_pStbCompare->CreateFontStd( DETAIL_FONT, dRANKBOARD_RANKBATTLE_ROWITEM_FONT_HEIGHT, 0 );

	// 비교마크(그림)
	rect.SetRectWH( 593, 8, 8, 7 );
	m_pPanCompareMark = NTL_NEW gui::CPanel( &rect, m_pDlgItem, GetNtlGuiManager()->GetSurfaceManager() );

	// Signals
	m_slotMouseEnterFromClass = m_pPanClass->SigMouseEnter().Connect( this, &CRankBattleRowItem::OnMouseEnterFromClass );
	m_slotMouseLeaveFromClass = m_pPanClass->SigMouseLeave().Connect( this, &CRankBattleRowItem::OnMouseLeaveFromClass );
}

/**
* \brief Destory
*/
VOID CRankBattleRowItem::Destroy() 
{
	NTL_DELETE( m_pPanCompareMark );
	NTL_DELETE( m_pStbCompare );
	NTL_DELETE( m_pStbStraightWin );
	NTL_DELETE( m_pStbPoint );
	NTL_DELETE( m_pStbGuildName );
	NTL_DELETE( m_pPanClass );
	NTL_DELETE( m_pStbLevel );
	NTL_DELETE( m_pStbName );
	NTL_DELETE( m_pStbRank );
	NTL_DELETE( m_pDlgItem );
}

/**
* \brief 현재의 아이템을 보여질지 안 보여질지 셋팅한다.
* \param bShow		(RwBool) 보여지는 여부
* 
*/
VOID CRankBattleRowItem::Show( RwBool bShow /*= TRUE */ ) 
{
	m_pDlgItem->Show(B2b(bShow) );
}

/**
* \brief 항목의 데이터를 세팅
* \param nRank			순위	
* \param pwcName		이름
* \param nLevel			레벨
* \param byClass		직업(NtlCharacter.h)
* \param pwcGuildName	길드이름
* \param nPoint			점수
* \param nStraightWin	연승
* \param nCompare		비교등수
* \param bHighLight		강조( TRUE = 강조 FALSE = 강조하지 않음 )
*/
VOID CRankBattleRowItem::SetItem(  RwInt32 nRank, const WCHAR* pwcName, RwInt32 nLevel, 
									RwUInt8 byClass, const WCHAR* pwcGuildName, RwInt32 nPoint, 
									RwUInt16 nStraightWin, RwUInt32 nCompare , RwBool bHightLight/* FALSE  */) 
{	
	// 서페이스를 초기화
	m_pDlgItem->GetSurface()->clear();
	
	// 검색 된 대상을 강조하여 표시한다.
	if( bHightLight )
	{
		m_pDlgItem->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "RankingGui.srf", "srfHighLightLeft" ) );
		m_pDlgItem->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "RankingGui.srf", "srfHighLightCenter" ) );
		m_pDlgItem->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "RankingGui.srf", "srfHighLightRight" ) );
	}
		
	m_nRank = nRank;

	m_pStbRank->SetText( nRank );
	m_pStbName->SetText( pwcName );
	m_pStbLevel->SetText( nLevel );
	m_pPanClass->GetSurface()->clear();
	m_pPanClass->AddSurface( Logic_GetPCClassIconSurface( byClass, FALSE ) );
	m_byClass = byClass;							// 직업 정보 저장
	m_pStbGuildName->SetText( pwcGuildName );
	m_pStbPoint->SetText( nPoint );
	m_pStbStraightWin->SetText( nStraightWin );

	// nCompare가 DWORD_INVALID 일 경우 ( 서버에 그 날짜의 기록된 등수가 없다는 것 )
	if( nCompare == -1 )
	{
		m_pStbCompare->SetText( L"-" );
		m_pPanCompareMark->GetSurface()->clear();
		Show( TRUE );
		return;
	}

	// 비교 등수( 이전 순위에서 현재 순위를 빼준다. )
	RwInt32 nAbsComp = nCompare - nRank;

	m_pStbCompare->SetText( abs(nAbsComp) );

	m_pPanCompareMark->GetSurface()->clear();

	// 비교 마크
	if( nAbsComp > 0 )
		m_pPanCompareMark->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "RankingGui.srf", "srfCompareUp" ) );
	else if( nAbsComp < 0 )
		m_pPanCompareMark->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "RankingGui.srf", "srfCompareDown" ) );
	
	Show( TRUE );
}

/**
* \brief 비교등수만 세팅
* \param nCompare 이전의 순위
*/
VOID CRankBattleRowItem::SetCompareInfo( RwInt32 nCompare ) 
{
	if( nCompare == -1 )
	{
		m_pStbCompare->SetText( L"-" );
		m_pPanCompareMark->GetSurface()->clear();
		Show( TRUE );
		return;
	}

	// 비교 등수( 이전 순위에서 현재 순위를 빼준다. )
	RwInt32 nAbsComp = nCompare - m_nRank;

	m_pStbCompare->SetText( abs(nAbsComp) );


	// 비교 마크
	m_pPanCompareMark->GetSurface()->clear();

	if( nAbsComp > 0 )
		m_pPanCompareMark->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "RankingGui.srf", "srfCompareUp" ) );
	else if( nAbsComp < 0 )
		m_pPanCompareMark->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "RankingGui.srf", "srfCompareDown" ) );
}

/**
* \brief 아이템을 초기화한다.
*/
VOID CRankBattleRowItem::ClearItem()
{
	m_pDlgItem->GetSurface()->clear();
	m_pStbRank->SetText( L"---" );
	m_pStbName->SetText( L"---" );
	m_pStbLevel->SetText( L"---" );
	m_pPanClass->GetSurface()->clear();
	m_pStbGuildName->SetText( L"---" );
	m_pStbPoint->SetText( L"---" );
	m_pStbStraightWin->SetText( L"---" );
	m_pStbCompare->SetText( L"---" );
	m_pPanCompareMark->GetSurface()->clear();
	m_nRank = INVALID_DWORD;
	m_byClass = INVALID_BYTE;
}

/**
* \brief 직업 마크에 툴팁을 단다.
* \param pComponent	(gui::CComponent*) 마우스가 들어온 컴포넌트
*/
VOID CRankBattleRowItem::OnMouseEnterFromClass( gui::CComponent* pComponent ) 
{
	// InfoWindow를 버튼에 달아준다.
	if( m_byClass == INVALID_BYTE )
		return;

	if( m_pPanClass )
	{
		CRectangle rtScreen = m_pPanClass->GetScreenRect();
		// ToolTip을 infoWindow로 생성한다.
		
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
				rtScreen.left, rtScreen.top, 
				(void*)Logic_GetClassName( m_byClass ), 
				DIALOG_RANKBOARD );
	}
}

/**
* \brief 직업 마크의 툴팁을 삭제한다.
* \param pComponent (gui::CComponent*) 마우스 포인터가 나간 컴포넌트
*/
VOID CRankBattleRowItem::OnMouseLeaveFromClass( gui::CComponent* pComponent ) 
{
	/*if( m_byClass == INVALID_BYTE )
		return;*/
	if( m_pPanClass )
	{
		if( DIALOG_RANKBOARD == GetInfoWndManager()->GetRequestGui() )
			GetInfoWndManager()->ShowInfoWindow( FALSE );
	}

}


////////////////////////////////////////////////////////////////////////////////////
// CRankBattleBoard
////////////////////////////////////////////////////////////////////////////////////

/**
* \brief 생성자
*/
CRankBattleBoard::CRankBattleBoard()
: m_pRankBattleBoardDlg(NULL)
, m_pParentGui(NULL)
, m_nCurrentCompareDay(1)
, m_nCurrentPage(0)
, m_nRankBattleItemCount(0)
{
	// Clear
	ZeroMemory( m_sRankBattleItem, sizeof(sRANKBATTLE_ITEM) * dRANKBOARD_RANKBATTLE_ROWITEM_NUMS );
	ZeroMemory( m_awcSearchName, sizeof(WCHAR) * (NTL_MAX_SIZE_CHAR_NAME+1) );
	m_byDataResult = eDATA_NONE;
	m_nSearchMode = eSEARCH_RANK;
	m_bSearchRank = FALSE;
}

/**
* \brief 소멸자
*/
CRankBattleBoard::~CRankBattleBoard() 
{
}

/**
* \brief Create
* \param eType		보드의 타입(CBoard*)
* \param pParentGui 부모 GUI의 포인터
*/
void CRankBattleBoard::Create( eBoardType eType , CRankBoardGui* pParent ) 
{
	m_pParentGui = pParent;

	m_eBoardType = eType;

	// UI
	m_pRankBattleBoardDlg = (gui::CDialog*)pParent->GetComponent( "dlgRankBattle" );
	m_pBtnPrev = (gui::CButton*)pParent->GetComponent( "btnRankBattlePrev" );
	m_pBtnNext = (gui::CButton*)pParent->GetComponent( "btnRankBattleNext" );
	m_pCbbSearchTitle = (gui::CComboBox*)pParent->GetComponent( "cbbSearchTitle" );
	m_pCbbSearchTitle->AddItem( GetDisplayStringManager()->GetString( "DST_RANKBOARD_RANKBATTLE_SUBJECT_RANK" ) );
	m_pCbbSearchTitle->AddItem( GetDisplayStringManager()->GetString( "DST_RANKBOARD_RANKBATTLE_SUBJECT_NAME" ) );
	m_pCbbSearchTitle->SelectItem( m_nSearchMode );
	m_pIpbSearch = (gui::CInputBox*)pParent->GetComponent( "ipbRankBattleSearch" );
	m_pBtnSearch = (gui::CButton*)pParent->GetComponent( "btnRankBattleSearch" );

	m_slotClickedBtnPrev = m_pBtnPrev->SigClicked().Connect( this, &CRankBattleBoard::OnClickedBtnPrev );
	m_slotClickedBtnNext = m_pBtnNext->SigClicked().Connect( this, &CRankBattleBoard::OnClickedBtnNext );
	m_slotClickedBtnSearch = m_pBtnSearch->SigClicked().Connect( this, &CRankBattleBoard::OnClickedBtnSearch );
	m_slotReturnIptSearch = m_pIpbSearch->SigReturnPressed().Connect( this, &CRankBattleBoard::OnInputReturnSearch );

	m_slotIpbSearchGotFocus = m_pIpbSearch->SigGotFocus().Connect( this, &CRankBattleBoard::OnIpbSearchGotFocus );
	m_slotIpbSearchLostFocus = m_pIpbSearch->SigLostFocus().Connect( this, &CRankBattleBoard::OnIpbSearchLostFocus );

	m_slotSearchItemSelect = m_pCbbSearchTitle->SigSelected().Connect( this, &CRankBattleBoard::OnSearchItemSelect );
	m_slotSearchListToggled = m_pCbbSearchTitle->SigListToggled().Connect( this, &CRankBattleBoard::OnSearchListToggled );

	// String
	m_pBtnPrev->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_BOARD_PREV" ) );
	m_pBtnNext->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_BOARD_NEXT" ) );

	// Make Subject
	m_pStbRank = (gui::CStaticBox*)pParent->GetComponent( "stbRankBattleRank" );
	m_pStbRank->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_RANKBATTLE_SUBJECT_RANK" ) );
	m_pStbName = (gui::CStaticBox*)pParent->GetComponent( "stbRankBattleName" );
	m_pStbName->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_RANKBATTLE_SUBJECT_NAME" ) );
	m_pStbLevel = (gui::CStaticBox*)pParent->GetComponent( "stbRankBattleLevel" );
	m_pStbLevel->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_RANKBATTLE_SUBJECT_LEVEL" ) );
	m_pStbClass = (gui::CStaticBox*)pParent->GetComponent( "stbRankBattleClass" );
	m_pStbClass->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_RANKBATTLE_SUBJECT_CLASS" ) );
	m_pStbGuildName = (gui::CStaticBox*)pParent->GetComponent( "stbRankBattleGuildName" );
	m_pStbGuildName->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_RANKBATTLE_SUBJECT_GUILD" ) );
	m_pStbPoint = (gui::CStaticBox*)pParent->GetComponent( "stbRankBattlePoint" );
	m_pStbPoint->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_RANKBATTLE_SUBJECT_POINT" ) );
	m_pStbStraightWin = (gui::CStaticBox*)pParent->GetComponent( "stbRankBattleStraightWin" );
	m_pStbStraightWin->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_RANKBATTLE_SUBJECT_STRAIGHTWIN" ) );

	m_pStbRankBattleCompare = (gui::CStaticBox*)pParent->GetComponent( "stbRankBattleCompare" );
	m_pStbRankBattleCompare->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_RANKBATTLE_SUBJECT_COMPARE" ) );

	/*m_pCbbCompare = (gui::CComboBox*)pParent->GetComponent( "cbbRankBattleCompare" );*/

	// Data
	/*m_pCbbCompare->ClearAllItems();
	m_pCbbCompare->AddItem( GetDisplayStringManager()->GetString( DST_RANKBOARD_RANKBATTLE_SUBJECT_COMPARE_1 ) );
	m_pCbbCompare->AddItem( GetDisplayStringManager()->GetString( DST_RANKBOARD_RANKBATTLE_SUBJECT_COMPARE_2 ) );
	m_pCbbCompare->AddItem( GetDisplayStringManager()->GetString( DST_RANKBOARD_RANKBATTLE_SUBJECT_COMPARE_3 ) );
	m_pCbbCompare->AddItem( GetDisplayStringManager()->GetString( DST_RANKBOARD_RANKBATTLE_SUBJECT_COMPARE_4 ) );
	m_pCbbCompare->AddItem( GetDisplayStringManager()->GetString( DST_RANKBOARD_RANKBATTLE_SUBJECT_COMPARE_5 ) );
	m_pCbbCompare->AddItem( GetDisplayStringManager()->GetString( DST_RANKBOARD_RANKBATTLE_SUBJECT_COMPARE_6 ) );
	m_pCbbCompare->SelectItem( 0 );*/
	m_nCurrentCompareDay = 1;

	// Make Underline
	CRectangle rect;
	for(int i=0; i< dRANKBOARD_RANKBATTLE_UNDERLINE_NUMS; ++i)
	{
		rect.SetRectWH( 8, 13 + (24 * i), dRANKBOARD_RANKBATTLE_UNDERLINE_WIDTH, 1 );
		m_pPanUnderLine[i] = NTL_NEW gui::CPanel( &rect, m_pRankBattleBoardDlg, GetNtlGuiManager()->GetSurfaceManager() );
		m_pPanUnderLine[i]->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "RankingGui.srf", "srfUnderLine" ) );
	}

	// Make Items
	for(int i=0; i< dRANKBOARD_RANKBATTLE_ROWITEM_NUMS; ++i )
		m_RankBattleRowItem[i].Create( m_pRankBattleBoardDlg, 25 + ( 24 * i) );

	/*m_slotItemSelect = m_pCbbCompare->SigSelected().Connect( this, &CRankBattleBoard::OnItemSelect );
	m_slotListToggled = m_pCbbCompare->SigListToggled().Connect( this, &CRankBattleBoard::OnListToggled );
	m_slotClickedCompare = m_pCbbCompare->GetButton()->SigClicked().Connect( this, &CRankBattleBoard::OnClickedCompare );*/

	// Init
	OnSearchItemSelect( m_nSearchMode );
}

/**
* \brief Destroy
*/
void CRankBattleBoard::Destroy() 
{
	for(int i=0; i< dRANKBOARD_RANKBATTLE_ROWITEM_NUMS; ++i )
		m_RankBattleRowItem[i].Destroy();
	
	//m_RankBattleSubject.Destroy();

	for(int i=0; i< dRANKBOARD_RANKBATTLE_UNDERLINE_NUMS; ++i )
		NTL_DELETE( m_pPanUnderLine[i] );
}

/**
* \brief Show
*/
void CRankBattleBoard::Show( RwBool bShow /*= TRUE */ ) 
{
	if( !bShow )
		::ZeroMemory( m_awcSearchName, sizeof(WCHAR) * NTL_MAX_SIZE_CHAR_NAME+1 );

	if( bShow )
		RowItemClear();

	m_pRankBattleBoardDlg->Show( B2b(bShow) );
}

/**
* \brief Enable
*/
void CRankBattleBoard::Enable( RwBool bEnable /*= TRUE */ ) 
{
	//m_pRankBattleBoardDlg->Enable( bEnable );
	//m_RankBattleSubject.EnableComboBox( bEnable );
	/*m_pCbbCompare->GetButton()->ClickEnable( bEnable );*/
	
	m_pCbbSearchTitle->GetButton()->ClickEnable( bEnable );
	
	m_pBtnPrev->ClickEnable( bEnable );
	
	m_pBtnNext->ClickEnable( bEnable );
	m_pBtnSearch->ClickEnable( bEnable );
	m_pIpbSearch->Enable( B2b(bEnable) );

	// 현재 페이지가 0 이라면 항상 ClickEnable( FALSE ) 상태가 되게 한다.
	if( m_nCurrentPage == 0 )
		m_pBtnPrev->ClickEnable( FALSE );

	if( bEnable )
		ApplyData();
}

/**
* \brief 현재 페이지 요청
*/
void CRankBattleBoard::CurrentPage() 
{
	GetDboGlobal()->GetChatPacketGenerator()->SendRankBattle_Rank_List_Req( 
		m_nCurrentPage, 
		(RwInt8)m_nCurrentCompareDay );
}

/**
* \brief 랭크배틀의 순위게시판이 받을 Msg를 받아서 처리한다.
* CRankBoardGui에서 현재 이 Board에 필요한 이벤트만 골라서 보내준다.
* \param msg	Event의 메시지
*/
void CRankBattleBoard::MessageProc( RWS::CMsg& msg ) 
{
	// 페이지 리스트
	if( msg.Id == g_EventRankBattleRankListRes )
	{
		SDboEventRankBattleRankListRes* pRes = 
			reinterpret_cast<SDboEventRankBattleRankListRes*>(msg.pData);

		// 랭크정보의 카운트가 0 이라면 없는 페이지를 요청한 것.
		if( pRes->byRankInfoCount == 0 )
		{
			// 만약 0페이지의 카운트가 하나도 없다면 서버에 랭크배틀 데이터가 없는 것.
			if( pRes->dwPage == 0 )
			{
				/*GetAlarmManager()->AlarmMessage( DST_RANKBOARD_BOARD_MSGBOX_NODATA );*/
				SetDataResult( eDATA_NONE );
				return;
			}

			/*GetAlarmManager()->AlarmMessage( DST_RANKBOARD_BOARD_MSGBOX_NOTPAGE );*/
			SetDataResult( eDATA_NOTPAGE );
			return;
		}
	
		// get sVARIABLE_DATA
		sVARIABLE_DATA* pData = reinterpret_cast<sVARIABLE_DATA*>(pRes->sData);
		
		// get sRANKBATTLE_RANK_INFO
		sRANKBATTLE_RANK_INFO* apRank = 
			reinterpret_cast<sRANKBATTLE_RANK_INFO*>( pData->Read( pRes->wRankInfo ) );

		// 페이지 정보 저장
		m_nCurrentPage = pRes->dwPage;
		m_nCurrentCompareDay = pRes->byCompareDay;

		// 아이템 구성
		m_nRankBattleItemCount = pRes->byRankInfoCount;
		for( int i = 0; i< pRes->byRankInfoCount; ++i )
		{
			m_sRankBattleItem[i].nRank = (pRes->dwPage*10) + i + 1;
			
			// sVARIABLE_DATA에서 캐릭터 이름
			int nSize = pData->GetSize( apRank[i].wCharName );
			::ZeroMemory( m_sRankBattleItem[i].awcName, sizeof(WCHAR) * NTL_MAX_SIZE_CHAR_NAME+1 );
			::CopyMemory( m_sRankBattleItem[i].awcName, pData->Read( apRank[i].wCharName ), nSize);
			m_sRankBattleItem[i].awcName[ nSize / sizeof(WCHAR) ] = L'\0';

			m_sRankBattleItem[i].nLevel = apRank[i].byLevel;
			m_sRankBattleItem[i].byClass = apRank[i].byClass;
		
			// sVARIABLE_DATA에서의 길드 이름
			nSize = pData->GetSize( apRank[i].wGuildName );
			::ZeroMemory( m_sRankBattleItem[i].awcGuildName, sizeof(WCHAR) * NTL_MAX_SIZE_GUILD_NAME+1 );
			::CopyMemory( m_sRankBattleItem[i].awcGuildName, pData->Read( apRank[i].wGuildName ), nSize );
			m_sRankBattleItem[i].awcGuildName[ nSize / sizeof(WCHAR) ] = L'\0';

			m_sRankBattleItem[i].nPoint = apRank[i].nPoint;
			m_sRankBattleItem[i].wStraightWin = apRank[i].wStraightWinCount;
			m_sRankBattleItem[i].nCompare = apRank[i].dwCompareRank;
		}

		SetDataResult( eDATA_OK );
	}
	// 검색 결과 리스트
	else if( msg.Id == g_EventRankBattleRankFindCharacterRes )
	{
		SDboEventRankBattleRankFindCharacterRes* pRes = reinterpret_cast<SDboEventRankBattleRankFindCharacterRes*>(msg.pData);
		
		m_nCurrentPage = pRes->dwPage;
		m_nCurrentCompareDay = pRes->byCompareDay;
				
		sVARIABLE_DATA* pData = reinterpret_cast<sVARIABLE_DATA*>(pRes->sData);
		sRANKBATTLE_RANK_INFO*  apRank = reinterpret_cast<sRANKBATTLE_RANK_INFO*>(pData->Read( pRes->wRankInfo ));

		if( apRank == NULL ||
			pRes->byRankInfoCount == 0 )
		{
			SetDataResult( eDATA_NOTFIND );
			return;
		}

		// Search Name
		::ZeroMemory( m_awcSearchName, sizeof(WCHAR) * NTL_MAX_SIZE_CHAR_NAME+1 );
		::CopyMemory( m_awcSearchName, pRes->pwsCharName, sizeof(WCHAR) * wcslen( pRes->pwsCharName ) );

		// Data Loading
		m_nRankBattleItemCount = pRes->byRankInfoCount;
		for( int i = 0; i < pRes->byRankInfoCount; ++i)
		{
			m_sRankBattleItem[i].nRank = (pRes->dwPage*10) + i + 1;

			// sVARIABLE_DATA에서 캐릭터 이름
			int nSize = pData->GetSize( apRank[i].wCharName );
			::ZeroMemory( m_sRankBattleItem[i].awcName, sizeof(WCHAR) * NTL_MAX_SIZE_CHAR_NAME+1 );
			::CopyMemory( m_sRankBattleItem[i].awcName, pData->Read( apRank[i].wCharName ), nSize);
			m_sRankBattleItem[i].awcName[ nSize / sizeof(WCHAR) ] = L'\0';

			m_sRankBattleItem[i].nLevel = apRank[i].byLevel;
			m_sRankBattleItem[i].byClass = apRank[i].byClass;

			// sVARIABLE_DATA에서의 길드 이름
			nSize = pData->GetSize( apRank[i].wGuildName );
			::ZeroMemory( m_sRankBattleItem[i].awcGuildName, sizeof(WCHAR) * NTL_MAX_SIZE_GUILD_NAME+1 );
			::CopyMemory( m_sRankBattleItem[i].awcGuildName, pData->Read( apRank[i].wGuildName ), nSize );
			m_sRankBattleItem[i].awcGuildName[ nSize / sizeof(WCHAR) ] = L'\0';

			m_sRankBattleItem[i].nPoint = apRank[i].nPoint;
			m_sRankBattleItem[i].wStraightWin = apRank[i].wStraightWinCount;
			m_sRankBattleItem[i].nCompare = apRank[i].dwCompareRank;
		}

		SetDataResult( eDATA_OK );
	}
	// 비교날짜 변경 리스트
	else if( msg.Id == g_EventRankBattleRankCompareDayRes )
	{
		SDboEventRankBattleRankCompareDayRes* pRes = 
		reinterpret_cast<SDboEventRankBattleRankCompareDayRes*>(msg.pData);
		RwBool		bIsFullList = pRes->bIsFullList;		
		sVARIABLE_DATA* pData = reinterpret_cast<sVARIABLE_DATA*>(pRes->sData);

		m_nCurrentPage = pRes->dwPage;
		m_nCurrentCompareDay = pRes->byCompareDay;
	
		// bIsFullList == TRUE : 서버의 캐쉬에 저장된 데이타가 없어서 그 페이지의 전체 리스트가 내려온다
		// bIsFullList == FALSE : 서버의 캐쉬에 저장된 데이타가 있어서 비교된 날짜만 내려온다.
		if( bIsFullList )
		{
			sRANKBATTLE_RANK_INFO* apRank = reinterpret_cast<sRANKBATTLE_RANK_INFO*>( pData->Read(pRes->wRankInfo) );

			if( apRank == NULL )
			{
				SetDataResult( eDATA_NOTPAGE );
				return;
			}
	

			m_nRankBattleItemCount = pRes->byInfoCount;
			for( int i = 0; i < pRes->byInfoCount; ++i )
			{
				m_sRankBattleItem[i].nRank = (pRes->dwPage*10) + i + 1;

				// sVARIABLE_DATA에서 캐릭터 이름
				int nSize = pData->GetSize( apRank[i].wCharName );
				::ZeroMemory( m_sRankBattleItem[i].awcName, sizeof(WCHAR) * NTL_MAX_SIZE_CHAR_NAME+1 );
				::CopyMemory( m_sRankBattleItem[i].awcName, pData->Read( apRank[i].wCharName ), nSize);
				m_sRankBattleItem[i].awcName[ nSize / sizeof(WCHAR) ] = L'\0';

				m_sRankBattleItem[i].nLevel = apRank[i].byLevel;
				m_sRankBattleItem[i].byClass = apRank[i].byClass;

				// sVARIABLE_DATA에서의 길드 이름
				nSize = pData->GetSize( apRank[i].wGuildName );
				::ZeroMemory( m_sRankBattleItem[i].awcGuildName, sizeof(WCHAR) * NTL_MAX_SIZE_GUILD_NAME+1 );
				::CopyMemory( m_sRankBattleItem[i].awcGuildName, pData->Read( apRank[i].wGuildName ), nSize );
				m_sRankBattleItem[i].awcGuildName[ nSize / sizeof(WCHAR) ] = L'\0';

				m_sRankBattleItem[i].nPoint = apRank[i].nPoint;
				m_sRankBattleItem[i].wStraightWin = apRank[i].wStraightWinCount;
				m_sRankBattleItem[i].nCompare = apRank[i].dwCompareRank;
			}

			SetDataResult( eDATA_OK );
		}
		else
		{
			DWORD* adwCompareRank = reinterpret_cast<DWORD*>( pData->Read(pRes->wCompareInfo) );

			if( adwCompareRank == NULL )
			{
				SetDataResult( eDATA_NOTPAGE );
				return;
			}
	
			for(int i=0; i<pRes->byInfoCount; ++i )
			{
				m_sRankBattleItem[i].nCompare = adwCompareRank[i];
			}

			SetDataResult( eDATA_ONLYCOMPARE );
		}
	}
}

/**
* \brief UI에서 가지고 있는 정보를 적용하고 데이타의 결과값에 따른 안내 메시지를 출력한다.
*/
VOID CRankBattleBoard::ApplyData()
{
	switch( m_byDataResult )
	{
		// 데이타가 정상적으로 처리되었다.
	case eDATA_OK:
		{
			// 순위 검색 모드였을 경우
			if( m_bSearchRank )
			{
				m_bSearchRank = FALSE;

				// UI가 가지고 있는 정보를 정상적으로 아이템에 입력한다.
				for( RwInt32 i = 0; i< m_nRankBattleItemCount; ++i )
				{
					RwBool bHighLight = FALSE;
					if( m_nSearchRankIndex == i )
						bHighLight = TRUE;

					m_RankBattleRowItem[i].SetItem( m_sRankBattleItem[i].nRank,
						m_sRankBattleItem[i].awcName,
						m_sRankBattleItem[i].nLevel,
						m_sRankBattleItem[i].byClass,
						m_sRankBattleItem[i].awcGuildName,
						m_sRankBattleItem[i].nPoint,
						m_sRankBattleItem[i].wStraightWin,
						m_sRankBattleItem[i].nCompare,
						bHighLight);

				}

				// 받지 못한 정보의 아이템은 보여주지 않는다.
				if( m_nRankBattleItemCount < dRANKBOARD_RANKBATTLE_ROWITEM_NUMS )
				{
					for(int i=m_nRankBattleItemCount; i< dRANKBOARD_RANKBATTLE_ROWITEM_NUMS; ++i )
					{
						m_RankBattleRowItem[i].Show( FALSE );
					}
				}
			}
			else
			{
				// UI가 가지고 있는 정보를 정상적으로 아이템에 입력한다.
				for( RwInt32 i = 0; i< m_nRankBattleItemCount; ++i )
				{
					RwBool bHighLight = FALSE;
					if( !wcscmp( m_awcSearchName, m_sRankBattleItem[i].awcName ) )
						bHighLight = TRUE;

					m_RankBattleRowItem[i].SetItem( m_sRankBattleItem[i].nRank,
						m_sRankBattleItem[i].awcName,
						m_sRankBattleItem[i].nLevel,
						m_sRankBattleItem[i].byClass,
						m_sRankBattleItem[i].awcGuildName,
						m_sRankBattleItem[i].nPoint,
						m_sRankBattleItem[i].wStraightWin,
						m_sRankBattleItem[i].nCompare,
						bHighLight);

				}

				// 받지 못한 정보의 아이템은 보여주지 않는다.
				if( m_nRankBattleItemCount < dRANKBOARD_RANKBATTLE_ROWITEM_NUMS )
				{
					for(int i=m_nRankBattleItemCount; i< dRANKBOARD_RANKBATTLE_ROWITEM_NUMS; ++i )
					{
						m_RankBattleRowItem[i].Show( FALSE );
					}
				}
			}
		}
		break;
		// 비교날짜만 내려왔을 경우
	case eDATA_ONLYCOMPARE:
		{
			for( RwInt32 i = 0; i< m_nRankBattleItemCount; ++i )
			{
				m_RankBattleRowItem[i].SetCompareInfo( m_sRankBattleItem[i].nCompare );
			}
		}
		break;
		// 서버에 아무런 정보가 없다.
	case eDATA_NONE:
		{
			if( m_pParentGui->IsShow() )
				GetAlarmManager()->AlarmMessage( "DST_RANKBOARD_BOARD_MSGBOX_NODATA" );
		}
		break;
		// 없는 페이지입니다.
	case eDATA_NOTPAGE:
		{
			if( m_bSearchRank )
			{
				if( m_pParentGui->IsShow() )
					GetAlarmManager()->AlarmMessage( "DST_RANKBOARD_BOARD_MSGBOX_NODATA" );
				
				m_bSearchRank = FALSE;
			}
			else
			{
				if( m_pParentGui->IsShow() )
					GetAlarmManager()->AlarmMessage( "DST_RANKBOARD_BOARD_MSGBOX_NOTPAGE" );
			}

		}
		break;
		// 검색 결과가 없습니다.
	case eDATA_NOTFIND:
		{
			if( m_pParentGui->IsShow() )
				GetAlarmManager()->AlarmMessage( "DST_RANKBOARD_BOARD_MSGBOX_NODATA" );
		}
		break;
	default:
		break;
	}

	
}

/**
* \brief 아이템 초기화
*/
VOID CRankBattleBoard::RowItemClear()
{
	for(RwInt32 i = 0; i < dRANKBOARD_RANKBATTLE_ROWITEM_NUMS; ++i )
	{
		m_RankBattleRowItem[i].ClearItem();
	}
}

/**
* \brief 서버에서 받은 데이타의 유효성 결과를 세팅한다.
* \param byDataResult	(RwUInt8) eDataResult
*/
VOID CRankBattleBoard::SetDataResult( RwUInt8 byDataResult )
{
	m_byDataResult = byDataResult;
}


/**
* \brief '이전' 버튼을 클릭
*/
VOID CRankBattleBoard::OnClickedBtnPrev( gui::CComponent* pComponent ) 
{
	if( m_nCurrentPage - 1 < 0 )
	{
		GetAlarmManager()->AlarmMessage( "DST_RANKBOARD_BOARD_MSGBOX_TOPPAGE" );
		return;
	}

	GetDboGlobal()->GetChatPacketGenerator()->SendRankBattle_Rank_List_Req(
		m_nCurrentPage - 1,
		(RwInt8)m_nCurrentCompareDay );
	
	// 비활성화
	m_pParentGui->DisableAllButton();
}

/**
* \brief '다음' 버튼을 클릭
*/
VOID CRankBattleBoard::OnClickedBtnNext( gui::CComponent* pComponent ) 
{
	GetDboGlobal()->GetChatPacketGenerator()->SendRankBattle_Rank_List_Req( 
		m_nCurrentPage + 1, 
		(RwInt8)m_nCurrentCompareDay );

	// 비활성화
	m_pParentGui->DisableAllButton();
}

/**
* \brief 검색 창에서 엔터를 쳤을 경우
*/
VOID CRankBattleBoard::OnInputReturnSearch() 
{
	if( !m_pParentGui->IsEnableButton() )
		return;

	const WCHAR* pwcText = m_pIpbSearch->GetText();

	// 입력창에 한 글자 이상 입력되었을 경우 처리한다.
	if( wcslen(pwcText) > 0 )
	{
		if( (eSearchMode)m_nSearchMode == eSEARCH_NAME )
		{
			GetDboGlobal()->GetChatPacketGenerator()->SendRankBattle_Rank_Find_Character_Req( 
				m_nCurrentPage, 
				(RwInt8)m_nCurrentCompareDay, 
				pwcText );

			m_pParentGui->DisableAllButton();
		}
		else
		{
			RwInt32 nSearchRank = _wtoi( pwcText );
			if( nSearchRank <= 0 )
				return;

			--nSearchRank;

			// 페이지와 몇번째인지를 찾는다.
			m_nSearchRankPage = 0;
			m_nSearchRankIndex = nSearchRank % 10;
			if( nSearchRank > 10 )
				m_nSearchRankPage = (RwInt32)((RwReal)nSearchRank * 0.1f);

			// 페이지를 요청
			m_bSearchRank = TRUE;
			GetDboGlobal()->GetChatPacketGenerator()->SendRankBattle_Rank_List_Req( m_nSearchRankPage, (RwInt8)m_nCurrentCompareDay );

			m_pParentGui->DisableAllButton();
		}
	}
	else
	{
		GetAlarmManager()->AlarmMessage( "DST_RANKBOARD_BOARD_MSGBOX_INPUTSEARCH" );
	}
}

/**
* \brief 검색 버튼을 클릭하였을 경우
*/
VOID CRankBattleBoard::OnClickedBtnSearch( gui::CComponent* pComponent ) 
{
	if( !m_pParentGui->IsEnableButton() )
		return;

	const WCHAR* pwcText = m_pIpbSearch->GetText();

	if( wcslen(pwcText) > 0 )
	{
		if( (eSearchMode)m_nSearchMode == eSEARCH_NAME )
		{
			GetDboGlobal()->GetChatPacketGenerator()->SendRankBattle_Rank_Find_Character_Req( 
				m_nCurrentPage, 
				(RwInt8)m_nCurrentCompareDay, 
				pwcText );

			m_pParentGui->DisableAllButton();
		}
		else
		{
			RwInt32 nSearchRank = _wtoi( pwcText );
			if( nSearchRank <= 0 )
				return;

			--nSearchRank;
			
			// 페이지와 몇번째인지를 찾는다.
			m_nSearchRankPage = 0;
			m_nSearchRankIndex = nSearchRank % 10;
			if( nSearchRank > 10 )
				m_nSearchRankPage = (RwInt32)((RwReal)nSearchRank * 0.1f);

			// 페이지를 요청
			m_bSearchRank = TRUE;
			GetDboGlobal()->GetChatPacketGenerator()->SendRankBattle_Rank_List_Req( m_nSearchRankPage, (RwInt8)m_nCurrentCompareDay );
			
			m_pParentGui->DisableAllButton();
		}
	}
	else
	{
		GetAlarmManager()->AlarmMessage( "DST_RANKBOARD_BOARD_MSGBOX_INPUTSEARCH" );
	}
}


/**
* \brief 콤보박스의 아이템을 선택하였을 경우
* \param nIndex		선택한 Index
*/
VOID CRankBattleBoard::OnItemSelect( INT nIndex ) 
{
	if( m_nCurrentCompareDay == nIndex+1 )
		return;

	GetDboGlobal()->GetChatPacketGenerator()->SendRankBattle_Rank_Compare_Day_Req( m_nCurrentPage, (RwInt8)(nIndex+1) );

	// 랭크보드를 Disable 상태로 전환
	m_pParentGui->DisableAllButton();
}

/**
* \brief 콤보박스의 리스트박스를 열었을 경우
* \param bToggled	오픈여부
* \param pComponent	오픈된 콤포넌트
*/
VOID CRankBattleBoard::OnListToggled( RwBool bToggled, gui::CComponent* pComponent ) 
{
	if( bToggled )
	{
		pComponent->Raise();
	}
}

VOID CRankBattleBoard::OnClickedCompare( gui::CComponent* pComponent ) 
{
}

VOID CRankBattleBoard::OnSearchItemSelect( INT nIndex )
{
	m_nSearchMode = nIndex;
	m_pIpbSearch->Clear();

	if( m_nSearchMode == eSEARCH_RANK )
	{
		
		m_pIpbSearch->SetNumberOnly( TRUE );
	}
	else
	{
		m_pIpbSearch->SetNumberOnly( FALSE );
	}
}

VOID CRankBattleBoard::OnSearchListToggled( RwBool bToggled, gui::CComponent* pComponent )
{
	if( bToggled )
	{
		pComponent->Raise();
	}
}

VOID CRankBattleBoard::OnIpbSearchGotFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = TRUE;
}

VOID CRankBattleBoard::OnIpbSearchLostFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = FALSE;
}