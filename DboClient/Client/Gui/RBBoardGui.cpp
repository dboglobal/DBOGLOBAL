#include "precomp_dboclient.h"
#include "RBBoardGui.h"

// core
#include "NtlDebug.h"

// share
#include "NtlRankBattle.h"

// table
#include "TableContainer.h"
#include "RankBattleTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAvatar.h"
#include "NtlSLRankBattle.h"

// sound
#include "GUISoundDefine.h"

// client
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "RBGui.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"

////////////////////////////////////////////////////////////////////////////////
// Define
#define RBBOARDITEM_WIDTH			234
#define RBBOARDITEM_HEIGHT			24
#define RBBOARDITEM_X				8
#define RBBOARDITEM_Y(ItemID)		(129+ItemID*(RBBOARDITEM_HEIGHT))

#define RBBOARDITEM_FIELDNAME_X			8
#define RBBOARDITEM_FIELDNAME_Y			5
#define RBBOARDITEM_FIELDNAME_WIDTH		109
#define RBBOARDITEM_FIELDNAME_HEIGHT	15

#define RBBOARDITEM_GAUGE_X			122
#define RBBOARDITEM_GAUGE_Y			6
#define RBBOARDITEM_GAUGE_WIDTH		105
#define RBBOARDITEM_GAUGE_HEIGHT	13

#define RBBOARD_FOCUSBAR_X			RBBOARDITEM_X
#define RBBOARD_FOCUSBAR_Y(ItemID)	RBBOARDITEM_Y(ItemID)

#define RBBOARD_REFRESHTIME			20.0f
#define RBBOARD_MAX_CHALLENGER		10

////////////////////////////////////////////////////////////////////////////////
// CRBBoardItem

CRBBoardItem::CRBBoardItem(VOID)
: m_uiFieldID( INVALID_DWORD ), m_nItemID( -1 ), m_pBoardGui( NULL ),
  m_pTitle( NULL ), m_pGauge( NULL ), m_pGaugeBack( NULL ), m_pThis( NULL )
{

}

CRBBoardItem::~CRBBoardItem(VOID)
{
	
}

RwBool CRBBoardItem::Create( CRBBoardGui* pBoardGui, gui::CComponent* pParent, gui::CSurfaceManager* pSurfaceManager, RwInt32 nItemID )
{
	m_pBoardGui = pBoardGui;
	m_nItemID = nItemID;

	CRectangle rect;
	rect.SetRectWH( RBBOARDITEM_X, RBBOARDITEM_Y(nItemID), RBBOARDITEM_WIDTH, RBBOARDITEM_HEIGHT );
	m_pThis = NTL_NEW gui::CButton( rect, "", pParent, pSurfaceManager );
	m_pThis->SetFocusSound( NULL );
	
	rect.SetRectWH( RBBOARDITEM_FIELDNAME_X, RBBOARDITEM_FIELDNAME_Y, RBBOARDITEM_FIELDNAME_WIDTH, RBBOARDITEM_FIELDNAME_HEIGHT );
	m_pTitle = NTL_NEW gui::CStaticBox( rect, m_pThis, pSurfaceManager, COMP_TEXT_CENTER );
	m_pTitle->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pTitle->Enable( false );

	rect.SetRectWH( RBBOARDITEM_GAUGE_X, RBBOARDITEM_GAUGE_Y, RBBOARDITEM_GAUGE_WIDTH, RBBOARDITEM_GAUGE_HEIGHT );
	m_pGaugeBack = NTL_NEW gui::CPanel( rect, m_pThis, pSurfaceManager , pSurfaceManager->GetSurface( "RBBoard.srf", "srfRBGaugeBack" ) );
	m_pGaugeBack->Enable( false );

	m_pGauge = NTL_NEW gui::CProgressBar( rect, m_pThis, pSurfaceManager );
	m_pGauge->AddSurface( pSurfaceManager->GetSurface( "RBBoard.srf", "srfRBGauge" ) );
	m_pGauge->Enable( false );	
	m_pGauge->SetRange( 0, RBBOARD_MAX_CHALLENGER );

	m_pThis->Show( false );

	m_slotClickItem = m_pThis->SigClicked().Connect( this, &CRBBoardItem::OnClickItem );	

	return TRUE;
}

VOID CRBBoardItem::Destroy(VOID)
{
	if( m_pThis )
		NTL_DELETE( m_pThis );	
}

VOID CRBBoardItem::Clear(VOID)
{
	m_uiFieldID = INVALID_DWORD;
	m_pTitle->Clear();
	m_pGauge->SetPos( 0 );
}

VOID CRBBoardItem::SetItemData( RwUInt32 uiFieldID, const WCHAR* pName, RwUInt32 uiAppliedCount, const WCHAR* pInfo)
{
	m_uiFieldID = uiFieldID;
	
	m_pTitle->SetText( pName );
	m_pGauge->SetPos( uiAppliedCount );

	m_pThis->SetToolTip(pInfo);
}

VOID CRBBoardItem::OnClickItem( gui::CComponent *pComponent )
{
	m_pBoardGui->OnClickBoardItem( m_nItemID, m_uiFieldID );
}

////////////////////////////////////////////////////////////////////////////////
// CRBBoardGui

CRBBoardGui::CRBBoardGui(VOID)
: CNtlPLGui()
{
	Init();
}

CRBBoardGui::CRBBoardGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CRBBoardGui::~CRBBoardGui(VOID)
{

}

VOID CRBBoardGui::Init(VOID)
{
	m_hSerial = INVALID_SERIAL_ID;
	m_uiSelectedItemFieldID = INVALID_SERIAL_ID;
	m_nOffset = 0;
	m_fCurrentTime = 0.0f;
	m_fMatchRemainTime = 0.0f;
}

RwBool CRBBoardGui::Create(VOID)
{
	NTL_FUNCTION( "CRBBoardGui::Create" );

	if( !CNtlPLGui::Create( "gui\\RBBoard.rsr", "gui\\RBBoard.srf", "gui\\RBBoard.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain" ) );

	m_pstbTitle = reinterpret_cast<gui::CStaticBox*>(GetComponent("sttTitle"));
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_RANK_BATTLE"));

	m_pstbBattleApplyTitle = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbBattleApplyTitle" ) );
	m_pstbBattleFieldTitle = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbBattleFieldTitle" ) );
	m_pstbChallengerCount = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbAppliedChallenger" ) );
	m_pstbInfo = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbInfo" ) );

	m_pstbRankBattleCount = reinterpret_cast<gui::CStaticBox*>(GetComponent("stbRankBattleCount"));
	m_ppnlRankBattleType = reinterpret_cast<gui::CPanel*>(GetComponent("pnlRankBattleType"));

	m_pbtnIndividual = reinterpret_cast<gui::CButton*>( GetComponent( "btnIndividual" ) );
	m_pbtnParty = reinterpret_cast<gui::CButton*>( GetComponent( "btnParty" ) );
	m_pbtnRelay = reinterpret_cast<gui::CButton*>(GetComponent("btnRelay"));

	m_pbtnRequestPartyMatching = reinterpret_cast<gui::CButton*>(GetComponent("btnRequestPartyMatching"));
	m_pbtnDirectEnter = reinterpret_cast<gui::CButton*>(GetComponent("btnDirectEnter"));

	m_pbtnCancel = reinterpret_cast<gui::CButton*>( GetComponent( "btnCancel" ) );
	m_pbtnClose = reinterpret_cast<gui::CButton*>( GetComponent( "btnClose" ) );

	m_pbtnHelp = reinterpret_cast<gui::CButton*>(GetComponent("btnHelp"));

	m_ppnlFocusBar = reinterpret_cast<gui::CPanel*>( GetComponent( "pnlFocus" ) );
	m_pscbScroll = reinterpret_cast<gui::CScrollBar*>( GetComponent( "scbScroll" ) );

	for( RwInt32 i = 0 ; i < MAX_BOARD_ITEM ; i++ )
	{
		m_BoardItem[i].Create( this, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), i );
	}

	m_pbtnIndividual->SetTextDisableColor( DEFAULT_TEXTCOLOR );
	m_pbtnParty->SetTextDisableColor( DEFAULT_TEXTCOLOR );

	m_slotClickIndividual = m_pbtnIndividual->SigClicked().Connect( this, &CRBBoardGui::OnClickIndividual );
	m_slotClickParty = m_pbtnParty->SigClicked().Connect( this, &CRBBoardGui::OnClickParty );
	m_slotClickRelay = m_pbtnRelay->SigClicked().Connect(this, &CRBBoardGui::OnClickRelay);

	m_slotClickChallenge = m_pbtnDirectEnter->SigClicked().Connect( this, &CRBBoardGui::OnClickChallenge );
	m_slotClickCancel = m_pbtnCancel->SigClicked().Connect( this, &CRBBoardGui::OnClickCancel );
	m_slotClickClose = m_pbtnClose->SigClicked().Connect( this, &CRBBoardGui::OnClickClose );
	m_slotClickHelp = m_pbtnHelp->SigClicked().Connect(this, &CRBBoardGui::OnClickHelp);

	m_slotScrollChanged = m_pscbScroll->SigValueChanged().Connect( this, &CRBBoardGui::OnScrollChanged );
	m_slotScrollSliderMoved = m_pscbScroll->SigSliderMoved().Connect( this, &CRBBoardGui::OnScrollChanged );

	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CRBBoardGui::OnCaptureMouseDown );
	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CRBBoardGui::OnCaptureWheelMove );

	SetBasicUISetting();

	// Initial Setting
	m_pThis->Show( false );
	m_ppnlFocusBar->Show( false );
		
	// Event연결
	LinkMsg( g_EventRBBoardInfo );
	LinkMsg( g_EventRBJoin );
	LinkMsg( g_EventNPCDialogOpen );
	LinkMsg( g_EventDialog );	
	LinkMsg( g_EventPartyLeaderChange );
	LinkMsg( g_EventPartyLeave );
					
	NTL_RETURN( TRUE );
}

VOID CRBBoardGui::Destroy(VOID)
{
	UnLinkMsg( g_EventRBBoardInfo );
	UnLinkMsg( g_EventRBJoin );
	UnLinkMsg( g_EventNPCDialogOpen );
	UnLinkMsg( g_EventDialog );	
	UnLinkMsg( g_EventPartyLeaderChange );
	UnLinkMsg( g_EventPartyLeave );
		
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	for( RwInt32 i = 0 ; i < MAX_BOARD_ITEM ; ++i )
	{
		m_BoardItem[i].Destroy();
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CRBBoardGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		m_uiSelectedItemFieldID = INVALID_DWORD;
		m_ppnlFocusBar->Show( false );

		StateApplySetting();
		Show( true );
		GetNtlGuiManager()->AddUpdateFunc( this );
	}
	else
	{
		m_hSerial = INVALID_SERIAL_ID;
		Show( false );
		GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );
	}

	return 1;
}

VOID CRBBoardGui::Update( RwReal fElapsed )
{
	if( IsShow() )
	{
		m_fCurrentTime += fElapsed;
		
		if( m_fCurrentTime >= RBBOARD_REFRESHTIME )
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendRBInfoReq( m_byBattleType );
			m_fCurrentTime = 0.0f;
		}

		RwInt32 nMatchRemainTime = (RwInt32)m_fMatchRemainTime;
		m_fMatchRemainTime -= fElapsed;

		if( m_fMatchRemainTime < 0.0f )
			m_fMatchRemainTime = (RwReal)DBO_RANKBATTLE_MATCH_START_TERM;

		if( (RwInt32)m_fMatchRemainTime != nMatchRemainTime )
			SetInfoText();
	}	
}

VOID CRBBoardGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventRBBoardInfo )
	{
		if( IsShow() )
		{
			SNtlEventRBBoardInfo* pData = reinterpret_cast<SNtlEventRBBoardInfo*>( msg.pData );

			m_fCurrentTime = 0.0f;		
			m_fMatchRemainTime = (RwReal)pData->uiRemainTime;

			SetBoardData( pData->byBattleMode, pData->byArenaCount, pData->pArenaInfo );		
		}

		EnableButtons( true );
	}
	else if( msg.Id == g_EventNPCDialogOpen )
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>( msg.pData );
		
		if( pData->eDialog != DIALOG_RBBOARD )
			return;

		CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();

		if( pRankBattle->IsEnableRBBoard() )
		{
			m_hSerial = pData->hSerialId;	
			GetDboGlobal()->GetGamePacketGenerator()->SendRBInfoReq( RANKBATTLE_MODE_INDIVIDUAL );
			EnableButtons( false );

			GetDialogManager()->OpenDialog( DIALOG_RBBOARD, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
		}		
		else
		{
			GetAlarmManager()->AlarmMessage( "DST_RANKBATTLE_MATCH_READY" );
		}
	}
	else if( msg.Id == g_EventRBJoin )
	{
		SNtlEventRBJoin* pData = reinterpret_cast<SNtlEventRBJoin*>( msg.pData );

		if( pData->eType == SNtlEventRBJoin::STATE_JOIN )
		{
			if( IsShow() )
			{
				StateApplySetting();
				m_fMatchRemainTime = (RwReal)pData->uiRemainTime;

				if( pData->bRes )
				{
					CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();

					m_fCurrentTime = 0.0f;
					m_fMatchRemainTime = (RwReal)pData->uiRemainTime;
					SetBoardData( pRankBattle->GetTblData()->byBattleMode, pData->byArenaCount, pData->pArenaInfo );
					EnableButtons( true );
				}
			}			
		}
		else if( pData->eType == SNtlEventRBJoin::STATE_LEAVE )
		{
			if( IsShow() )
				GetDialogManager()->CloseDialog( DIALOG_RBBOARD );
		}
		else if( pData->eType == SNtlEventRBJoin::STATE_START )
		{
			if( IsShow() )
				GetDialogManager()->CloseDialog( DIALOG_RBBOARD );
		}
		else if( pData->eType == SNtlEventRBJoin::STATE_CANCELED )
		{
			if( IsShow() )
				GetDialogManager()->CloseDialog( DIALOG_RBBOARD );
		}
		else if( pData->eType == SNtlEventRBJoin::STATE_JOINFAIL )
		{
			if( IsShow() )
				EnableButtons( true );
		}
	}
	else if( msg.Id == g_EventPartyLeaderChange || msg.Id == g_EventPartyLeave )
	{
		if( IsShow() )
			StateApplySetting();
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>( msg.pData );
		if( pData->iType == DIALOGEVENT_NPC_BYEBYE && pData->iDestDialog == DIALOG_RBBOARD )
		{
			GetDialogManager()->CloseDialog( DIALOG_RBBOARD );
		}
	}
//	else if( msg.Id == g_EventRBRoom )
//	{
//		SNtlEventRBRoom* pRoomData = reinterpret_cast<SNtlEventRBRoom*>( msg.pData );
//
//		if( pRoomData->type == SNtlEventRBRoom::ROOM_LEAVE )
//		{
//			GetDialogManager()->CloseDialog(DIALOG_RBBOARD );
//		}		
//		else if( IsShow() )
//			StateSetting();				
//	}	
//	else if( msg.Id == g_EventRBMatch )
//	{
//		SNtlEventRBMatch* pMatchData = reinterpret_cast<SNtlEventRBMatch*>( msg.pData );
//
//		if( pMatchData->type == SNtlEventRBMatch::MATCH_READY )
//			OnClickClose( NULL );				
//	}			
}

VOID CRBBoardGui::SetBasicUISetting(VOID)
{
	m_pstbBattleApplyTitle->SetText( GetDisplayStringManager()->GetString( "DST_RB_TITLE" ) );
	m_pstbBattleFieldTitle->SetText( GetDisplayStringManager()->GetString( "DST_RB_FIELD" ) );
	m_pstbChallengerCount->SetText( GetDisplayStringManager()->GetString( "DST_RB_CHALLENGER_COUNT" ) );
	
	m_pbtnIndividual->SetText( GetDisplayStringManager()->GetString( "DST_RB_BTN_INDIVIDUAL" ) );
	m_pbtnParty->SetText( GetDisplayStringManager()->GetString( "DST_RB_BTN_PARTY" ) );
	m_pbtnRelay->SetText(GetDisplayStringManager()->GetString("DST_RB_BTN_RELAY"));
	m_pbtnRequestPartyMatching->SetText(GetDisplayStringManager()->GetString("DST_PARTY_MATCHING_REGIST"));
	m_pbtnDirectEnter->SetText( GetDisplayStringManager()->GetString( "DST_RANKBATTLE_DIRECT_REGISTER" ) );
	m_pbtnCancel->SetText( GetDisplayStringManager()->GetString( "DST_RB_BTN_CANCEL" ) );
}

VOID CRBBoardGui::SetBoardData( RwUInt8 byBattleType, RwUInt8 byArenaCount, sRANKBATTLE_ARENA_INFO* pArenaInfo )
{
	RwUInt32 uiAppliedFieldID = GetAppliedID( byBattleType );
	m_vecBoardData.clear();
	m_nOffset = 0;

	if( uiAppliedFieldID != INVALID_DWORD )
		m_uiSelectedItemFieldID = uiAppliedFieldID;

	StateBattleTypeSetting( byBattleType );

	m_pscbScroll->SetRange( 0, byArenaCount - MAX_BOARD_ITEM );	

	CRankBattleTable* pRankBattleTable = API_GetTableContainer()->GetRankBattleTable();
	
	for( RwInt32 i = 0 ; i < byArenaCount ; ++i )
	{
		sRANKBATTLE_TBLDAT* pRBTableData = reinterpret_cast<sRANKBATTLE_TBLDAT*>( pRankBattleTable->FindData( pArenaInfo[i].rankBattleTblidx ) );
		NTL_WARNING( pRBTableData, "CRBBoardGui::SetBoardData : TableData is Invalid" );	
		if( pRBTableData )
		{
			sRBBoardData sData;
			sData._uiAppliedCount = pArenaInfo[i].wRegisterCount;
			sData._uiFieldID = pArenaInfo[i].rankBattleTblidx;
			sData._wstrFieldName = pRBTableData->wszName;
			sData._infoIndex = pRBTableData->dwInfoIndex;
			
			m_vecBoardData.push_back( sData );

			if( pArenaInfo[i].rankBattleTblidx == m_uiSelectedItemFieldID )
			{
				if( byArenaCount - i >= MAX_BOARD_ITEM )
					m_nOffset = i;
				else
					m_nOffset = byArenaCount - MAX_BOARD_ITEM;
			}
		}					
	}			

	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
	
	if( pRankBattle->GetRBRequestState() == CNtlRankBattle::RB_REQUEST_STATE_NONE &&
		m_vecBoardData.size() > 0 && m_uiSelectedItemFieldID == INVALID_DWORD )
		m_uiSelectedItemFieldID = m_vecBoardData[0]._uiFieldID;		

	m_pscbScroll->SetValue( m_nOffset, FALSE );
	SetBoardItem( m_nOffset );
	//SNtlEventRBBoardInfo* pBoardData = reinterpret_cast<SNtlEventRBBoardInfo*>( pData );

	//CRankBattleTable* pRankBattleTable = API_GetTableContainer()->GetRankBattleTable();
	////sRANKBATTLE_TBLDAT* pRankBattleData = reinterpret_cast<sRANKBATTLE_TBLDAT*>( pRankBattleTable->FindData( pBoardData->tblidx ) );

	//m_pRankBattleData = pRankBattleData;

	//if( pBoardData->byRoomCount == 0 )
	//	m_ppnlFocusBar->Show( false );	
	//else
	//{// 기본선택
	//	RwInt32 nFocusItemID = GetFocusItemID();

	//	if( pBoardData->byRoomCount <= nFocusItemID )
	//		OnClickBoardItem( pBoardData->byRoomCount - 1 );
	//	else if( nFocusItemID < 0 )
	//		OnClickBoardItem( 0 );
	//}	

	//// 레벨제한 표시
	////m_pstbCanEnterLevel->Format( GetDisplayStringManager()->GetString( DST_RB_LEVEL_LIMIT ), pRankBattleData->byMinLevel, pRankBattleData->byMaxLevel );
	//
	//// 페이지 표시
	////SetPage( pBoardData->wCurPage, pBoardData->wTotalPage );	
	////m_pstbPage->Format( GetDisplayStringManager()->GetString( DST_RB_PAGE ), m_nCurrentPage + 1, ( m_nTotalPage == 0 ) ? m_nTotalPage + 1 : m_nTotalPage );
	//
	//// 아이템 표시
	//for( RwInt32 i = 0 ; i < MAX_BOARD_ITEM ; ++i )
	//{
	//	if( i < pBoardData->byRoomCount )
	//	{
	//		// 아이템이 있는 경우
	//		m_BoardItem[i].Show( true );

	//		if( pBoardData->byRoomType == RANKBATTLE_MODE_INDIVIDUAL )
	//		{
	//			sRANKBATTLE_ROOMLIST_INDIVIDUAL* pIndividualRoomList = reinterpret_cast<uRANKBATTLE_ROOMINFO_LIST*>( pBoardData->pRoomList )->asIndividualRoom;
	//			m_BoardItem[i].SetItemData( pIndividualRoomList[i].roomId, pIndividualRoomList[i].wszOwnerPc, pIndividualRoomList[i].wWinCounter );
	//		}
	//		else if( pBoardData->byRoomType == RANKBATTLE_MODE_PARTY )
	//		{
	//			sRANKBATTLE_ROOMLIST_PARTY* pPartyRoomList = reinterpret_cast<uRANKBATTLE_ROOMINFO_LIST*>( pBoardData->pRoomList )->asPartyRoom;
	//			m_BoardItem[i].SetItemData( pPartyRoomList[i].roomId, pPartyRoomList[i].wszOwnerParty, pPartyRoomList[i].wWinCounter );
	//		}
	//		else
	//		{
	//			NTL_ASSERTFAIL( "CRBBoardGui::SetBoardData : invalid rank battle mode" );
	//		}

	//	}
	//	else
	//	{
	//		// 아이템이 없는 경우
	//		m_BoardItem[i].Show( false );
	//	}
	//}
}

VOID CRBBoardGui::SetBoardItem( RwInt32 nOffset )
{
	CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetETCTbl();

	RwInt32 i = 0;
	m_nOffset = nOffset;
	
	for( i = 0 ; i < MAX_BOARD_ITEM ; ++i )
	{
		m_BoardItem[i].Show( false );
	}
	m_ppnlFocusBar->Show( false );

	VEC_RBBOARDDATA_ITER iter;
	for( iter = m_vecBoardData.begin(), i = 0 ; i < m_nOffset && iter != m_vecBoardData.end() ; ++iter, ++i );

	for( i = 0 ; i < MAX_BOARD_ITEM && iter != m_vecBoardData.end() ; ++iter, ++i )
	{
		sRBBoardData* data = &(*iter);

		m_BoardItem[i].SetItemData( data->_uiFieldID, data->_wstrFieldName.c_str(), data->_uiAppliedCount, pTextTable->GetText(data->_infoIndex).c_str());
		m_BoardItem[i].Show( true );

		if( data->_uiFieldID == m_uiSelectedItemFieldID )
		{
			m_ppnlFocusBar->SetPosition( RBBOARD_FOCUSBAR_X, RBBOARD_FOCUSBAR_Y( i ) );
			m_ppnlFocusBar->Show( true );
		}		
	}
}

VOID CRBBoardGui::SetInfoText(VOID)
{
	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
	RwUInt32 uiState = pRankBattle->GetRBRequestState();

	if (uiState == CNtlRankBattle::RB_REQUEST_STATE_NONE)
	{
		m_pstbInfo->Format(GetDisplayStringManager()->GetString("DST_RB_INFO_1"), Logic_GetTimeString(m_fMatchRemainTime + 0.99f));
	}
	else
	{
		const WCHAR* wszBattleType = NULL;

		if( pRankBattle->IsPartyBattle() )
			wszBattleType = GetDisplayStringManager()->GetString( "DST_RB_BTN_PARTY" );
		else
			wszBattleType = GetDisplayStringManager()->GetString( "DST_RB_BTN_INDIVIDUAL" );
		
		m_pstbInfo->Format( GetDisplayStringManager()->GetString( "DST_RB_INFO_2" ), wszBattleType, pRankBattle->GetTblData()->wszName, Logic_GetTimeString( m_fMatchRemainTime + 0.99f ) );
	}
}

VOID CRBBoardGui::StateApplySetting(VOID)
{
	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
	RwUInt32 uiState = pRankBattle->GetRBRequestState();
		
	if( uiState == CNtlRankBattle::RB_REQUEST_STATE_NONE )
	{
		m_pbtnDirectEnter->ClickEnable( TRUE );
		m_pbtnCancel->Show( false );				
	}
	else
	{
		m_pbtnDirectEnter->ClickEnable( FALSE );
		m_pbtnCancel->Show( true );		

		if( pRankBattle->IsPartyBattle() )
		{
			if( pRankBattle->IsPartyBattle() && Logic_I_am_PartyLeader() )
				m_pbtnCancel->ClickEnable( TRUE );
			else
				m_pbtnCancel->ClickEnable( FALSE );			
		}
		else
		{
			m_pbtnCancel->ClickEnable( TRUE );					
		}		
	}

	m_pstbRankBattleCount->Format(GetDisplayStringManager()->GetString("DST_RB_COUNT_INFO"), 0); // to do

	SetInfoText();
}

VOID CRBBoardGui::StateBattleTypeSetting( RwUInt8 byBattleType )
{
	if( byBattleType == RANKBATTLE_MODE_INDIVIDUAL )
	{
		m_pbtnIndividual->ClickEnable( FALSE );
		m_pbtnParty->ClickEnable( TRUE );
	}
	else
	{
		m_pbtnIndividual->ClickEnable( TRUE );
		m_pbtnParty->ClickEnable( FALSE );
	}

	m_byBattleType = byBattleType;
}

RwInt32 CRBBoardGui::GetFocusItemID(VOID)
{
	INT nID = -1;

	for( RwInt32 i = 0 ; i < MAX_BOARD_ITEM ; ++i )
	{
		if( m_BoardItem[i].GetFieldID() == m_uiSelectedItemFieldID )
		{
			nID = i;
			break;
		}
	}

	return nID;
}

RwUInt32 CRBBoardGui::GetAppliedID( RwUInt8 byBattleType )
{
	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
	RwUInt32 bAppliedID = INVALID_DWORD;

	if( pRankBattle->GetRBRequestState() != CNtlRankBattle::RB_REQUEST_STATE_NONE )
	{
		sRANKBATTLE_TBLDAT* pData = pRankBattle->GetTblData();
		NTL_WARNING( pData, "CRBBoardGui::GetAppliedID : Must have TableData at this RBState" );

		if( pData )
		{
			if( pData->byBattleMode == byBattleType )
				bAppliedID = pRankBattle->GetTblData()->tblidx;		
		}
	}
	
	return bAppliedID;
}

VOID CRBBoardGui::EnableButtons( bool bEnable )
{
	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
	RwUInt32 uiState = pRankBattle->GetRBRequestState();

	m_pbtnIndividual->Enable( bEnable );
	m_pbtnParty->Enable( bEnable );

	if (uiState == CNtlRankBattle::RB_REQUEST_STATE_REQUEST)
		m_pbtnDirectEnter->Enable( bEnable );

	m_pbtnCancel->Enable( bEnable );
	m_pbtnClose->Enable( bEnable );
	m_pscbScroll->Enable( bEnable );

	for( RwInt32 i = 0 ; i < MAX_BOARD_ITEM ; ++i )
		m_BoardItem[i].Enable( bEnable );
}

VOID CRBBoardGui::ResetItem(VOID)
{
	for( RwInt32 i = 0 ; i < MAX_BOARD_ITEM ; ++i )
		m_BoardItem[i].Clear();
}

VOID CRBBoardGui::OnClickBoardItem( RwInt32 nItemIndex, RwUInt32 uiFieldID )
{
	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();

	if( pRankBattle->GetRBRequestState() == CNtlRankBattle::RB_REQUEST_STATE_NONE )
	{
		m_uiSelectedItemFieldID = uiFieldID;

		m_ppnlFocusBar->SetPosition( RBBOARD_FOCUSBAR_X, RBBOARD_FOCUSBAR_Y( nItemIndex ) );
		m_ppnlFocusBar->Show( true );
	}	
	else
	{
		// peessitemp : 신청중에는 다른 것을 선택할 수 없다는 메시지 필요.

	}
}

VOID CRBBoardGui::OnClickIndividual( gui::CComponent* pComponent )
{
	GetDboGlobal()->GetGamePacketGenerator()->SendRBInfoReq( RANKBATTLE_MODE_INDIVIDUAL );

	EnableButtons( false );		

	m_uiSelectedItemFieldID = INVALID_DWORD;
	m_ppnlFocusBar->Show( false );
}

VOID CRBBoardGui::OnClickParty( gui::CComponent* pComponent )
{
	GetDboGlobal()->GetGamePacketGenerator()->SendRBInfoReq( RANKBATTLE_MODE_PARTY );

	EnableButtons( false );		

	m_uiSelectedItemFieldID = INVALID_DWORD;
	m_ppnlFocusBar->Show( false );
}

VOID CRBBoardGui::OnClickRelay(gui::CComponent * pComponent)
{
	// to do

}

VOID CRBBoardGui::OnClickChallenge( gui::CComponent* pComponent )
{
	if (m_hSerial == INVALID_SERIAL_ID)
	{
		DBO_WARNING_MESSAGE("CRBBoardGui::OnClickChallenge : Must Have Object ID. It is applystate Error");
		return;
	}

	if( m_uiSelectedItemFieldID != INVALID_DWORD )
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendRBJoinReq( m_hSerial, m_uiSelectedItemFieldID );
		EnableButtons( false );
	}
	else
		GetAlarmManager()->AlarmMessage( "DST_RANKBATTLE_MUST_SELECT_ROOMID" );
}

VOID CRBBoardGui::OnClickCancel( gui::CComponent* pComponent )
{
	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
	RwUInt32 uiState = pRankBattle->GetRBRequestState();

	if(uiState == CNtlRankBattle::RB_REQUEST_STATE_REQUEST)
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendRBLeaveReq( pRankBattle->GetTblData()->tblidx );
		StateApplySetting();		
	}	
}

VOID CRBBoardGui::OnClickClose( gui::CComponent* pComponent )
{
	GetDialogManager()->CloseDialog( DIALOG_RBBOARD );
}

VOID CRBBoardGui::OnClickHelp(gui::CComponent* pComponent)
{
	CDboEventGenerator::OpenHelpContent(DIALOG_RBBOARD);
}

VOID CRBBoardGui::OnScrollChanged( RwInt32 nNewOffset )
{
	SetBoardItem( nNewOffset );
}

VOID CRBBoardGui::OnCaptureMouseDown( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_RBBOARD, key.m_fX, key.m_fY );
}

VOID CRBBoardGui::OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos )
{
	CAPTURE_MOUSEWHEELMOVE_SCROLLBAR( IsShow(), m_pThis, m_pThis, m_pscbScroll, sDelta, pos );
}

//VOID CRBBoardGui::OnClickPrevPage( gui::CComponent* pComponent )
//{
//	if( m_nCurrentPage <= 0 )
//	{
//		m_nCurrentPage = 0;
//		return;
//	}
//	
//	GetDboGlobal()->GetGamePacketGenerator()->SendRBRoomListReq( m_hSerial, --m_nCurrentPage );	
//}

//VOID CRBBoardGui::OnClickNextPage( gui::CComponent* pComponent )
//{
//	if( m_nCurrentPage >= m_nTotalPage - 1 )
//	{
//		m_nCurrentPage = m_nTotalPage - 1;
//		return;
//	}
//
//	GetDboGlobal()->GetGamePacketGenerator()->SendRBRoomListReq( m_hSerial, ++m_nCurrentPage );	
//}