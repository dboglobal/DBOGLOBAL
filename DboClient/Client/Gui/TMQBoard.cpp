#include "precomp_dboclient.h"
#include "TMQBoard.h"

// core
#include "NtlDebug.h"
#include "NtlPLDef.h"

// shared
#include "Table.h"
#include "TimeQuestTable.h"
#include "NtlRankBattle.h"
#include "QuestTextDataTable.h"

// simulation
#include "DboEvent.h"
#include "NtlSoundManager.h"
#include "GUISoundDefine.h"

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

/**
* \brief Construction
*/
CTMQPartyListItem::CTMQPartyListItem()
: m_pTMQBoard(NULL)
, m_pDlgPartyListItem(NULL)
, m_pStbPartyName(NULL)
, m_pStbClearTime(NULL)
{
	
}

/**
* \brief Destruction
*/
CTMQPartyListItem::~CTMQPartyListItem()
{

}

/**
* \brief Create
* \param pBoard		(CTMQBoard*) 현재의 GUI가 부모로 가지는 CBoard*
* \param pParentGui	(gui::CComponent*) CTMQPartyListItem이 가지고 있는 Dialog의 부모 Component
* \param byIndex	(RwUInt8) 파티 리스트의 인덱스 ( 순번 또는 순위 )
* \param nTop		(RwInt32) 파티 리스트가 가지는 Dialog의 Y 좌표
*/
VOID CTMQPartyListItem::Create( CTMQBoard* pBoard, gui::CComponent* pParentGui, RwUInt8 byIndex, RwInt32 nTop )
{
	// TMQ Board의 포인터와 인덱스를 저장하고 있는다.
	m_pTMQBoard = pBoard;
	m_byIndex = byIndex;

	CRectangle rect;

	// Dialog
	rect.SetRectWH( 13, nTop, 316, dRANKBOARD_TMQ_PARTYTIEM_HEIGHT );
	m_pDlgPartyListItem = NTL_NEW gui::CDialog( &rect, pParentGui, GetNtlGuiManager()->GetSurfaceManager() );

	// 파티 이름
	rect.SetRectWH( 57, 6, 125, 20 );
	m_pStbPartyName = NTL_NEW gui::CStaticBox( &rect, m_pDlgPartyListItem, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbPartyName->CreateFontStd( DETAIL_FONT, dRANKBOARD_TMQ_PARTYITEM_FONT_HEIGHT, 0 );

	// 클리어 시간
	rect.SetRectWH( 184, 6, 107, 20 );
	m_pStbClearTime = NTL_NEW gui::CStaticBox( &rect, m_pDlgPartyListItem, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbClearTime->CreateFontStd( DETAIL_FONT, dRANKBOARD_TMQ_PARTYITEM_FONT_HEIGHT, 0 );

	// 밑줄
	rect.SetRectWH( 0, 33, 316, 1 );
	m_pPanUnderLine = NTL_NEW gui::CPanel( &rect, m_pDlgPartyListItem, GetNtlGuiManager()->GetSurfaceManager() );

	if( dRANKBOARD_TMQ_PARTYITEM_NUMS-1 != byIndex )
		m_pPanUnderLine->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "RankingGui.srf", "srfTMQUnderLine" ) );

	// 
	m_slotMouseUp = m_pDlgPartyListItem->SigMouseUp().Connect( this, &CTMQPartyListItem::OnMouseUp );

	Enable( TRUE );
}

/**
* \brief Destroy
*/
VOID CTMQPartyListItem::Destroy()
{
	NTL_DELETE( m_pPanUnderLine );
	NTL_DELETE( m_pStbPartyName );
	NTL_DELETE( m_pStbClearTime );
	NTL_DELETE( m_pDlgPartyListItem );
}

/**
* \brief 파티 리스트 항목을 세팅한다.
* \param nRank			(RwInt32) 순위
* \param pwcPartyName	(const WCHAR*) 파티 이름
* \param uiClearTime	(RwUInt32) 클리어 기록
*/
VOID CTMQPartyListItem::SetItem( RwInt32 nRank, const WCHAR* pwcPartyName, RwUInt32 uiClearTime )
{
	m_pStbPartyName->SetText( pwcPartyName );

	// ClearTime은 ms 단위
	RwReal fSec = (RwReal)uiClearTime / 1000.f;

	// 하루가 넘어가면 지정된 String을 출력한다.
	RwUInt32 uiDay = (RwUInt32)(fSec / 86400.f);
	if( uiDay > 0 )
		m_pStbClearTime->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_TMQ_OVER_TIME" ) );
	else
		m_pStbClearTime->SetText( Logic_GetTimeString( fSec ) );
}

/**
* \brief 아이템을 초기화한다.
*/
VOID CTMQPartyListItem::ClearItem()
{
	m_pStbPartyName->SetText(L"---");
	m_pStbClearTime->SetText(L"---");
}

/**
* \brief bEnable 아이템을 비활/활성화 시킨다. 아이템이 비활성화가 되면 마우스 UP을 처리하지 않는다.
* \param bEnable	(RwBool) 가능
*/
VOID CTMQPartyListItem::Enable( RwBool bEnable )
{
	m_bEnable = bEnable;
	m_pDlgPartyListItem->Enable( B2b(bEnable) );
	m_pStbPartyName->Enable( false );
	m_pStbClearTime->Enable( false );
}

/**
* \brief 마우스의 UP을 받는다.
* \param key	(CKey&) 마우스 정보의 구조체
*/
VOID CTMQPartyListItem::OnMouseUp( const CKey& key )
{
	// 정보가 없다면 Mouse Up을 처리하지 않는다.
	if( !m_pStbPartyName->GetText().compare( L"---" ) )
		return;

	if( m_bEnable )
		m_pTMQBoard->SelectMemberList( m_byIndex );
}

/**
* \brief Construction
*/
CTMQPartyMemberItem::CTMQPartyMemberItem()
: m_pDlgPartyMemberItem(NULL)
, m_pStbPartyMemberName(NULL)
, m_pStbLevel(NULL)
, m_pPanClass(NULL)
, m_byClass(255)
{
	
}

/**
* \brief Destruction
*/
CTMQPartyMemberItem::~CTMQPartyMemberItem()
{

}

/**
* \brief 파티 멤버 리스트의 컴포넌트들을 동적으로 생성한다.
* \param pParentGui	(gui::CComponent*) 부모의 GUI
* \param nTop		(RwInt32) 현재 아이템의 Y 시작 위치
*/
VOID CTMQPartyMemberItem::Create( gui::CComponent* pParentGui, RwInt32 nTop )
{
	CRectangle rect;

	rect.SetRectWH( 362, nTop, 240, 17 );
	m_pDlgPartyMemberItem = NTL_NEW gui::CDialog( &rect, pParentGui, GetNtlGuiManager()->GetSurfaceManager() );

	rect.SetRectWH( 12, 0, 126, 17 );
	m_pStbPartyMemberName = NTL_NEW gui::CStaticBox( &rect, m_pDlgPartyMemberItem, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbPartyMemberName->CreateFontStd( DETAIL_FONT, dRANKBOARD_TMQ_PARTYMEMBERITEM_FONT_HEIGHT, 0 );

	rect.SetRectWH( 142, 0, 28, 17 );
	m_pStbLevel = NTL_NEW gui::CStaticBox( &rect, m_pDlgPartyMemberItem, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbLevel->CreateFontStd( DETAIL_FONT, dRANKBOARD_TMQ_PARTYMEMBERITEM_FONT_HEIGHT, 0 );

	rect.SetRectWH( 203, 0, 19, 17 );
	m_pPanClass = NTL_NEW gui::CPanel( &rect, m_pDlgPartyMemberItem, GetNtlGuiManager()->GetSurfaceManager() );
}

/**
* \brief Destroy
*/
VOID CTMQPartyMemberItem::Destroy()
{
	NTL_DELETE( m_pStbPartyMemberName );
	NTL_DELETE( m_pStbLevel );
	NTL_DELETE( m_pPanClass );
	NTL_DELETE( m_pDlgPartyMemberItem );
}

/**
* \brief 멤버 아이템을 데이타로 세팅한다.
* \param pwcMemberName	(const WCHAR*) 파티 멤버의 이름
* \param nLevel			(RwInt32) 레벨
* \param byClass		(RwUInt8) 클래스
*/
VOID CTMQPartyMemberItem::SetItem( const WCHAR* pwcMemberName, RwInt32 nLevel, RwUInt8 byClass )
{
	m_pStbPartyMemberName->SetText( pwcMemberName );
	m_pStbLevel->SetText( nLevel );
	
	// ToolTip 및 Class의 Icon을 달아준다.
	m_pPanClass->DeleteToolTip();
	m_pPanClass->GetSurface()->clear();
	m_pPanClass->AddSurface( Logic_GetPCClassIconSurface( byClass, FALSE ) );
	m_pPanClass->SetToolTip( Logic_GetClassName(byClass) );

	m_byClass = byClass;
}

/**
* \brief 아이템을 초기화한다.
*/
VOID CTMQPartyMemberItem::ClearItem()
{
	m_pStbPartyMemberName->SetText( L"---" );
	m_pStbLevel->SetText( L"---" );
	m_pPanClass->GetSurface()->clear();
}

/**
* \brief Construction
*/
CTMQBoard::CTMQBoard()
: m_pTMQBoardDlg(NULL)
, m_pParentGui(NULL)
{
	m_nCurrentDifficulty = TIMEQUEST_DIFFICULTY_EASY;
	m_nCurrentScenario = 0;

	m_byDataResult = eDATA_NONE;
	m_byTeamCount = 0;
	m_byMemberCount = 0;

	ZeroMemory( m_sTeamRankData, sizeof(sTIMEQUEST_TEAM_RANK_DATA) * dRANKBOARD_TMQ_PARTYITEM_NUMS );
	ZeroMemory( m_sMemberData, sizeof(sTIMEQUEST_PARTY_MEMBER_DATA) * dRANKBOARD_TMQ_PARTYMEMBERITEM_NUMS );
}

/**
* \brief Destruction
*/
CTMQBoard::~CTMQBoard()
{

}

/**
* \brief TMQ Board를 생성한다.
* \param eType		(eBoardType) 보드의 타입
* \param pParentGui	(CRankBoardGui*) 랭킹 보드의 포인터
*/
void CTMQBoard::Create( eBoardType eType, CRankBoardGui* pParentGui )
{
	m_pParentGui = pParentGui;

	// UI
	m_pTMQBoardDlg = (gui::CDialog*)pParentGui->GetComponent( "dlgTMQ" );
	m_pBtnPrev = (gui::CButton*)pParentGui->GetComponent( "btnTMQPrev" );
	m_pBtnNext = (gui::CButton*)pParentGui->GetComponent( "btnTMQNext" );
	/*m_pStbTMQTitle = (gui::CStaticBox*)pParentGui->GetComponent( "stbTMQTitle" );*/
	m_pCbbTitle = (gui::CComboBox*)pParentGui->GetComponent( "cbbTMQScenarioList" );
	m_pStbTMQRank = (gui::CStaticBox*)pParentGui->GetComponent( "stbTMQRank" );
	m_pStbTMQPartyName = (gui::CStaticBox*)pParentGui->GetComponent( "stbTMQPartyName" );
	m_pStbTMQClearTime = (gui::CStaticBox*)pParentGui->GetComponent( "stbTMQClearTime" );
	m_pStbPartyMemberInfo = (gui::CStaticBox*)pParentGui->GetComponent( "stbPartyMemberInfo" );
	m_pStbPartyMember = (gui::CStaticBox*)pParentGui->GetComponent( "stbPartyMember" );
	m_pStbPartyLevel = (gui::CStaticBox*)pParentGui->GetComponent( "stbPartyLevel" );
	m_pStbPartyClass = (gui::CStaticBox*)pParentGui->GetComponent( "stbPartyClass" );
	/*m_pCbbTMQScenarioList = (gui::CComboBox*)pParentGui->GetComponent( "cbbTMQScenarioList" );*/
	m_pPanTMQHighLight = (gui::CPanel*)pParentGui->GetComponent( "panTMQHighLight" );
	m_pBtnNormal = (gui::CButton*)pParentGui->GetComponent("btnTMQNormal");
	m_pBtnHard = (gui::CButton*)pParentGui->GetComponent("btnTMQHard");
	
	m_pBtnPrev->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_BOARD_PREV" ) );
	m_pBtnNext->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_BOARD_NEXT" ) );
	m_pStbTMQRank->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_TMQ_SUBJECT_RANK" ) );
	m_pStbTMQPartyName->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_TMQ_SUBJECT_PARTYNAME" ) );
	m_pStbTMQClearTime->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_TMQ_SUBJECT_CLEARTIME" ) );
	m_pStbPartyMemberInfo->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_TMQ_SUBJECT_PARTYMEMBERINFO" ) );
	m_pStbPartyMember->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_TMQ_SUBJECT_PARTYMEMBER" ) );
	m_pStbPartyLevel->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_TMQ_SUBJECT_LEVEL" ) );
	m_pStbPartyClass->SetText( GetDisplayStringManager()->GetString( "DST_RANKBOARD_TMQ_SUBJECT_CLASS" ) );
	
	// Data
	m_pCbbTitle->ClearAllItems();
	m_slotClickedBtnPrev = m_pBtnPrev->SigClicked().Connect( this, &CTMQBoard::OnClickedBtnPrev );
	m_slotClickedBtnNext = m_pBtnNext->SigClicked().Connect( this, &CTMQBoard::OnClickedBtnNext );
	m_slotClickedBtnNormal = m_pBtnNormal->SigClicked().Connect( this, &CTMQBoard::OnClickedBtnNormal);
	m_slotClickedBtnHard = m_pBtnHard->SigClicked().Connect( this, &CTMQBoard::OnClickedBtnHard );
	m_slotOnItemSelect = m_pCbbTitle->SigSelected().Connect( this, &CTMQBoard::OnItemSelect );
	m_slotListToggled = m_pCbbTitle->SigListToggled().Connect( this, &CTMQBoard::OnListToggled );

	// 파티 리스트를 생성한다.
	for( RwUInt8 i = 0; i< dRANKBOARD_TMQ_PARTYITEM_NUMS; ++i )
		m_itemPartyList[i].Create( this, m_pTMQBoardDlg, i, (63) + (dRANKBOARD_TMQ_PARTYTIEM_HEIGHT*i));

	// 멤버 리스트를 생성한다.
	for( RwUInt8 i = 0; i< dRANKBOARD_TMQ_PARTYMEMBERITEM_NUMS; ++i )
		m_itemPartyMember[i].Create( m_pTMQBoardDlg, (106) + (30*i) );

	CTimeQuestTable* pTMQTable = API_GetTableContainer()->GetTimeQuestTable();	

	CTable::TABLEIT itTable = pTMQTable->Begin();
	RwUInt8 byCount = 1;
	
	while( itTable != pTMQTable->End() )
	{
		sTIMEQUEST_TBLDAT* pTIMEQUEST_TABLE = (sTIMEQUEST_TBLDAT*)((*itTable).second);	
		if( pTIMEQUEST_TABLE->byTimeQuestType == TIMEQUEST_TYPE_PARTY ||
			pTIMEQUEST_TABLE->byTimeQuestType == TIMEQUEST_TYPE_GROUP )
		{
			CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
			sTIMEQUEST_DATASET* pTIMEQUEST_DATASET = &pTIMEQUEST_TABLE->sTimeQuestDataset[TIMEQUEST_DIFFICULTY_EASY];
			sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pTIMEQUEST_DATASET->nameTblidx ) );
			if( !pQUEST_TEXT_DATA_TBLDAT )
			{
				DBO_FAIL( "QuestTextDataTbl Data is null" );
				return;
			}

			std::wstring& wstrText = gui::GetHtmlFromMemoryString(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size());
			m_pCbbTitle->AddItem( wstrText.c_str() );
			m_vecTblIdx.push_back( (*itTable).first );
			++byCount;
		}
			
		++itTable;
	}

	m_pCbbTitle->SelectItem( 0 );
	SetDifficultButton( TIMEQUEST_DIFFICULTY_EASY );
}

/**
* \brief TMQ Board에서 생성한 아이템들을 삭제한다.
*/
void CTMQBoard::Destroy()
{
	for( RwInt32 i = 0; i< dRANKBOARD_TMQ_PARTYITEM_NUMS; ++i )
		m_itemPartyList[i].Destroy();

	for( RwInt32 i = 0; i< dRANKBOARD_TMQ_PARTYMEMBERITEM_NUMS; ++i )
		m_itemPartyMember[i].Destroy();
}

/**
* \brief 현재 보드가 가지고 있는 Dialog의 Show 여부 세팅
* \param bShow	(RwBool) TRUE : 보임, FALSE : 보이지 않음
*/
void CTMQBoard::Show( RwBool bShow /*= TRUE */ )
{
	m_pTMQBoardDlg->Show( B2b(bShow) );
	if( bShow )
		m_pPanTMQHighLight->Show( false );
}

/**
* \brief 현재 보드의 활성화 여부 셋팅
* \param bEnable	(RwBool) TRUE : 활성화   FALSE : 비활성화
*/
void CTMQBoard::Enable( RwBool bEnable /*= TRUE */ )
{
	m_pCbbTitle->Enable(B2b(bEnable) );
	m_pCbbTitle->GetButton()->ClickEnable( bEnable );

	
	m_pBtnPrev->ClickEnable( bEnable );
	m_pBtnNext->ClickEnable( bEnable );

	for(RwInt32 i = 0; i< dRANKBOARD_TMQ_PARTYITEM_NUMS; ++i )
		m_itemPartyList[i].Enable( bEnable );

	if( m_nCurrentScenario == 0 )
		m_pBtnPrev->ClickEnable( FALSE );
	else if( m_nCurrentScenario+1 == (RwInt32)m_vecTblIdx.size() )
		m_pBtnNext->ClickEnable( FALSE );
	
	if( bEnable )
		ApplyData();
}

/**
* \brief 이벤티 처리 함수
* \param msg	(RWS::CMsg*) 이벤트 구조체
*/
void CTMQBoard::MessageProc( RWS::CMsg& msg )
{
	// TMQ 기록의 리스트를 받는다.
	if( msg.Id == g_EventTMQRecordListRes )
	{
		SDboEventTMQRecordListRes* pResult = reinterpret_cast<SDboEventTMQRecordListRes*>(msg.pData);

		// 난이도
		SetDifficultButton( pResult->byDifficult );

		sTIMEQUEST_TEAM_RANK_DATA* pTeamData = reinterpret_cast<sTIMEQUEST_TEAM_RANK_DATA*>( pResult->paTeam );

		// 첫번째(Best) ClearTime이 0이라면 데이타가 없음
		if( pTeamData[0].dwClearTime == 0 )
		{
			SetDataResult( eDATA_NONE );
			
			// Select 는 0번을 해준다.
			CRectangle rect = m_itemPartyList[0].GetScreenRect();
			CRectangle DialogRect = m_pTMQBoardDlg->GetScreenRect();

			m_pPanTMQHighLight->SetPosition( rect-DialogRect );
			m_pPanTMQHighLight->Show( false );
			m_pParentGui->DisableAllButton();
			return;
		}

		// 데이터를 GUI에서 보관한다.
		m_byTeamCount = pResult->byTeamCount;
		for( RwInt32 i = 0; i< pResult->byTeamCount; ++i )
		{
			m_sTeamRankData[i].dwClearTime = pTeamData[i].dwClearTime;
			m_sTeamRankData[i].byMode = pTeamData[i].byMode;
			memcpy( m_sTeamRankData[i].wszPartyName,
				pTeamData[i].wszPartyName,
				sizeof(WCHAR) * NTL_MAX_SIZE_PARTY_NAME );
		}

		// 멤버를 성공적으로 받았으면 0 번째 멤버 리스트를 요청한다.
		SelectMemberList( 0 );

		//// 성공적으로 데이터를 받았다.
		//SetDataResult( eDATA_OK );
	}
	// MEMBER LIST를 받는다.
	else if( msg.Id == g_EventTMQMemberListRes )
	{
		SDboEventTMQMemberListRes* pResult = reinterpret_cast<SDboEventTMQMemberListRes*>(msg.pData);

		sTIMEQUEST_TEAM_MEMBER_DATA* pTeamData = reinterpret_cast<sTIMEQUEST_TEAM_MEMBER_DATA*>( pResult->pTeam );
		sTIMEQUEST_PARTY_MEMBER_DATA* pMemberData = pTeamData->asMember;
		
		if( pTeamData->byMemberCount <= 0 )
		{
			SetDataResult( eDATA_NOMEMBER );
			return;
		}

		m_byMemberCount = pTeamData->byMemberCount;
		for( RwInt32 i = 0; i< pTeamData->byMemberCount; ++i )
		{
			m_sMemberData[i].byLevel = pMemberData[i].byLevel;
			m_sMemberData[i].byClass = pMemberData[i].byClass;
			memcpy( m_sMemberData[i].wszName,
				pMemberData[i].wszName,
				sizeof(WCHAR) * NTL_MAX_SIZE_CHAR_NAME );
		}

		SetDataResult( eDATA_OK );
	}
}

/**
* \brief 현재 페이지를 요청한다.
*/
void CTMQBoard::CurrentPage()
{
	if( m_nCurrentScenario >= (RwInt32)m_vecTblIdx.size() || m_nCurrentScenario < 0 )
		return;
	
	// 현재 페이지 요청
	GetDboGlobal()->GetChatPacketGenerator()->SendTMQ_Record_List_Req( m_vecTblIdx[m_nCurrentScenario], m_nCurrentDifficulty );
	ClearPartyList();
	m_pCbbTitle->SelectItem( m_nCurrentScenario );
	
	// Best의 멤버 리스트를 요청
	/*SelectMemberList( 0 );*/
}

/**
* \brief 파티 리스트를 초기화한다.
*/
VOID CTMQBoard::ClearPartyList()
{
	for(RwInt32 i = 0; i< dRANKBOARD_TMQ_PARTYITEM_NUMS; ++i)
		m_itemPartyList[i].ClearItem();
}

/**
* \brief 멤버 리스트를 초기화한다.
*/
VOID CTMQBoard::ClearMemberList()
{
	for(RwInt32 i = 0; i< dRANKBOARD_TMQ_PARTYMEMBERITEM_NUMS; ++i)
		m_itemPartyMember[i].ClearItem();
}

/**
* \brief 멤버 리스트를 요청한다.
* \param byPartyIndex	(RwUInt8) 선택 파티의 인덱스 번호 ( ex : 0, 1, 2, ... , 6)
*/
VOID CTMQBoard::SelectMemberList( RwUInt8 byPartyIndex )
{
	// 어떤 아이템을 셀렉했는지
	CRectangle rect = m_itemPartyList[byPartyIndex].GetScreenRect();
	CRectangle DialogRect = m_pTMQBoardDlg->GetScreenRect();

	m_pPanTMQHighLight->SetPosition( rect-DialogRect );
	m_pPanTMQHighLight->Show( true );

	GetDboGlobal()->GetChatPacketGenerator()->SendTMQ_Member_List_Req( m_vecTblIdx[m_nCurrentScenario], m_nCurrentDifficulty, byPartyIndex );
	
	// Disable
	ClearMemberList();
	m_pParentGui->DisableAllButton();
}

/**
* \brief 현재 TMQ 시나리오의 난이도를 설정한다.
* \param byDifficult	(RwUInt8) 난이도 상수
*/
VOID CTMQBoard::SelectDifficult( RwUInt8 byDifficult )
{
	m_nCurrentDifficulty = byDifficult;
	CurrentPage();
}

/**
* \brief 선택한 난이도의 버튼을 제외한 난이도 버튼을 활성화 시킨다.
* \param byDifficult	(RwUInt8) 난이도 상수
*/
VOID CTMQBoard::SetDifficultButton( RwUInt8 byDifficult )
{
	switch( byDifficult )
	{
	case TIMEQUEST_DIFFICULTY_EASY:
		m_pBtnNormal->ClickEnable( FALSE );
		m_pBtnHard->ClickEnable( TRUE );
		break;
	case TIMEQUEST_DIFFICULTY_NORMAL:
		m_pBtnNormal->ClickEnable( FALSE );
		m_pBtnHard->ClickEnable( TRUE );
		break;
	case TIMEQUEST_DIFFICULTY_HARD:
		m_pBtnNormal->ClickEnable( TRUE );
		m_pBtnHard->ClickEnable( FALSE );
		break;
	default:
		break;
	}
}

/**
* \brief 서버에서 내려온 데이터의 유효성을 기록한다.
* \param byDataResult	(RwUInt8) eDataResult
*/
VOID CTMQBoard::SetDataResult( RwUInt8 byDataResult )
{
	m_byDataResult = byDataResult;
}

/**
* \brief 가지고 있는 데이터를 적용한다. 만약 유효하지 않은 데이타라고 판단되면 알맞은 안내 메시지를 출력한다.
*/
VOID CTMQBoard::ApplyData()
{
	ClearPartyList();
	ClearMemberList();

	// 활성화 되었을 경우 처리
	switch( m_byDataResult )
	{
	case eDATA_OK:
		{
			for(int i=0; i< m_byTeamCount; ++i )
				m_itemPartyList[i].SetItem( i, m_sTeamRankData[i].wszPartyName, m_sTeamRankData[i].dwClearTime );

			for(int i=0; i< m_byMemberCount; ++i )
				m_itemPartyMember[i].SetItem( m_sMemberData[i].wszName, m_sMemberData[i].byLevel, m_sMemberData[i].byClass );
		}
		break;
	case eDATA_NONE:
		{
			if( m_pParentGui->IsShow() )
				GetAlarmManager()->AlarmMessage( "DST_RANKBOARD_BOARD_MSGBOX_NODATA" );
		}
		break;
	case eDATA_NOMEMBER:
		{
			if( m_pParentGui->IsShow() )
				GetAlarmManager()->AlarmMessage( "DST_RANKBOARD_BOARD_MSGBOX_NOTMEMBER" );
		}
		break;
	default:
		break;
	}
}

/**
* \brief 이전 버튼을 클릭하면 한 단계 이전의 시나리오를 선택한다.
* \param pComponent		(gui::CComponent*) 클릭된 컴포넌트
*/
VOID CTMQBoard::OnClickedBtnPrev( gui::CComponent* pComponent )
{
	// Prev
	if( m_nCurrentScenario <= 0 )
	{
		GetAlarmManager()->AlarmMessage( "DST_RANKBOARD_BOARD_MSGBOX_NONESCENARIO" );
		return;
	}

	--m_nCurrentScenario;
	CurrentPage();
}

/**
* \brief 다음 버튼을 클릭하면 한 단계 다음의 시나리오를 선택한다.
* \param pComponent		(gui::CCOmponent*) 클릭된 컴포넌트
*/
VOID CTMQBoard::OnClickedBtnNext( gui::CComponent* pComponent )
{
	// Next
	if( m_nCurrentScenario >= (RwInt32)m_vecTblIdx.size() )
	{
		// 없는 시나리오입니다.
		GetAlarmManager()->AlarmMessage( "DST_RANKBOARD_BOARD_MSGBOX_NONESCENARIO" );
		return;
	}
	
	++m_nCurrentScenario;
	CurrentPage();
}

/**
* \brief 리스트 박스를 토글하였을 때
* \param bToggled	(RwBool) 토글의 여부
* \param pComponent	(gui::CComponent*) 토글된 리스트박스의 컴포넌트
*/
VOID CTMQBoard::OnListToggled( RwBool bToggled, gui::CComponent* pComponent )
{
	// 보여지고 있다면 다른 gui에 겹쳐지지 않게 Raise() 시켜준다.
	if( bToggled )
	{
		pComponent->Raise();
	}
}

/**
* \brief Easy 버튼을 클릭하였을 때
*/
VOID CTMQBoard::OnClickedBtnEasy( gui::CComponent* pComponent )
{
	if( m_pParentGui->IsEnableButton() )
		SelectDifficult( TIMEQUEST_DIFFICULTY_EASY );
}

/**
* \brief Normal 버튼을 클릭하였을 때
*/
VOID CTMQBoard::OnClickedBtnNormal( gui::CComponent* pComponent )
{
	if( m_pParentGui->IsEnableButton() )
		SelectDifficult( TIMEQUEST_DIFFICULTY_EASY );
}

/**
* \brief Hard 버튼을 클릭하였을 경우
*/
VOID CTMQBoard::OnClickedBtnHard( gui::CComponent* pComponent )
{
	if( m_pParentGui->IsEnableButton() )
		SelectDifficult( TIMEQUEST_DIFFICULTY_HARD );
}

/**
* \brief 리스트에서 시나리오를 선택했을 경우
*/
VOID CTMQBoard::OnItemSelect( RwInt32 nIndex )
{
	// Next
	if( nIndex >= (RwInt32)m_vecTblIdx.size() )
	{
		// 없는 시나리오입니다.
		GetAlarmManager()->AlarmMessage( "DST_RANKBOARD_BOARD_MSGBOX_NONESCENARIO" );
		return;
	}

	m_nCurrentScenario = nIndex;
	CurrentPage();
}
