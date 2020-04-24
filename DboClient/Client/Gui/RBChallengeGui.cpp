#include "precomp_dboclient.h"
#include "RBChallengeGui.h"
//
//// core
//#include "NtlDebug.h"
//
//// share
//#include "NtlRankBattle.h"
//
//// table
//#include "TableContainer.h"
//#include "RankBattleTable.h"
//
//// presentation
//#include "NtlPLGuiManager.h"
//
//// simulation
//#include "NtlSLGlobal.h"
//#include "NtlSLApi.h"
//#include "NtlSLEvent.h"
//#include "NtlSLEventFunc.h"
//#include "NtlSobAvatar.h"
//#include "NtlSLRankBattle.h"
//
//// sound
//#include "GUISoundDefine.h"
//
//// client
//#include "DboGlobal.h"
//#include "DboEvent.h"
//#include "DboEventGenerator.h"
//#include "DialogManager.h"
//#include "DisplayStringManager.h"
//
//////////////////////////////////////////////////////////////////////////////////
//// Define
//#define RBCHALLENGEITEM_WIDTH			228
//#define RBCHALLENGEITEM_HEIGHT			26
//#define RBCHALLENGEITEM_X				6
//#define RBCHALLENGEITEM_Y(ItemID)		(93+ItemID*(RBCHALLENGEITEM_HEIGHT+1))
//
//#define RBCHALLENGEITEM_NAME_X			6
//#define RBCHALLENGEITEM_NAME_Y			0
//#define RBCHALLENGEITEM_NAME_WIDTH		130
//#define RBCHALLENGEITEM_NAME_HEIGHT		26
//
//#define RBCHALLENGEITEM_CLASS_X			135
//#define RBCHALLENGEITEM_CLASS_Y			4
//#define RBCHALLENGEITEM_CLASS_WIDTH		19
//#define RBCHALLENGEITEM_CLASS_HEIGHT	19
//
//#define RBCHALLENGEITEM_RECORD_X		167
//#define RBCHALLENGEITEM_RECORD_Y		0
//#define RBCHALLENGEITEM_RECORD_WIDTH	53
//#define RBCHALLENGEITEM_RECORD_HEIGHT	26
//
//#define RBCHALLENGE_DEFENDER_NAME_COLOR	RGB(255,204,0)
//#define RBCHALLENGE_WINCOUNT_TEXT_COLOR	RGB(180,238,255)
//#define RBCHALLENGE_WINCOUNT_COLOR		RGB(254,254,254)
//
//////////////////////////////////////////////////////////////////////////////////
//
//CRBChallengeItem::CRBChallengeItem(VOID)
//: m_pName( NULL ), m_pLevel( NULL ), m_pClass( NULL )
//{
//
//}
//
//CRBChallengeItem::~CRBChallengeItem(VOID)
//{
//	
//}
//
//RwBool CRBChallengeItem::Create( gui::CComponent* pParent, gui::CSurfaceManager* pSurfaceManager, RwInt32 nItemID )
//{
//	CRectangle rect;
//	rect.SetRectWH( RBCHALLENGEITEM_X, RBCHALLENGEITEM_Y(nItemID), RBCHALLENGEITEM_WIDTH, RBCHALLENGEITEM_HEIGHT );
//	m_pThis = NTL_NEW gui::CDialog( rect, pParent, pSurfaceManager );
//
//	rect.SetRectWH( RBCHALLENGEITEM_NAME_X, RBCHALLENGEITEM_NAME_Y, RBCHALLENGEITEM_NAME_WIDTH, RBCHALLENGEITEM_NAME_HEIGHT );
//	m_pName = NTL_NEW gui::CStaticBox( rect, m_pThis, pSurfaceManager, COMP_TEXT_LEFT );
//	m_pName->Enable( false );
//	m_pName->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
//
//	rect.SetRectWH( RBCHALLENGEITEM_CLASS_X, RBCHALLENGEITEM_CLASS_Y, RBCHALLENGEITEM_CLASS_WIDTH, RBCHALLENGEITEM_CLASS_HEIGHT );
//	m_pClass = NTL_NEW gui::CPanel( rect, m_pThis, pSurfaceManager );
//		
//	rect.SetRectWH( RBCHALLENGEITEM_RECORD_X, RBCHALLENGEITEM_RECORD_Y, RBCHALLENGEITEM_RECORD_WIDTH, RBCHALLENGEITEM_RECORD_HEIGHT );
//	m_pLevel = NTL_NEW gui::CStaticBox( rect, m_pThis, pSurfaceManager, COMP_TEXT_RIGHT );
//	m_pLevel->Enable( false );
//	m_pLevel->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
//
//	m_pThis->Show( false );
//
//	return TRUE;
//}
//
//VOID CRBChallengeItem::Destroy(VOID)
//{
//	if( m_pThis )
//		NTL_DELETE( m_pThis );
//}
//
//VOID CRBChallengeItem::SetItemData( WCHAR* pName, RwInt32 nLevel, RwInt32 eClass )
//{
//	m_pName->SetText( pName );
//	m_pLevel->Format( GetDisplayStringManager()->GetString( DST_RB_LEVEL ), nLevel );
//	
//	m_pClass->GetSurface()->clear();
//	m_pClass->AddSurface( Logic_GetPCClassIconSurface( eClass, FALSE ) );
//	m_pClass->SetToolTip( std::wstring( Logic_GetClassName( eClass ) ) );
//}
//
//CRBChallengeGui::CRBChallengeGui(VOID)
//: CNtlPLGui()
//{
//	Init();
//}
//
//CRBChallengeGui::CRBChallengeGui( const RwChar* pName )
//: CNtlPLGui( pName )
//{
//	Init();
//}
//
//CRBChallengeGui::~CRBChallengeGui(VOID)
//{
//
//}
//
//VOID CRBChallengeGui::Init(VOID)
//{
//	m_pstbDefenderName = NULL;
//	m_pmdbDefenderBattleRecord = NULL;
//	m_pstbChallengeText = NULL;
//
//	m_pbtnClose = NULL;
//	m_pbtnChallenge = NULL;
//	m_pbtnCancel = NULL;
//
//	m_uiRoomID = INVALID_DWORD;
//}
//
//RwBool CRBChallengeGui::Create(VOID)
//{
//	NTL_FUNCTION( "CRBBoardGui::Create" );
//
//	if( !CNtlPLGui::Create( "", "gui\\RBBoard.srf", "gui\\RBChallengeBoard.frm" ) )
//		NTL_RETURN( FALSE );
//
//	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );
//
//	m_pThis = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain" ) );
//
//	m_pstbDefenderName = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbDefenderTitle" ) );
//	m_pmdbDefenderBattleRecord = reinterpret_cast<gui::CMDStaticBox*>( GetComponent( "mdbDefenderBattleRecord" ) );
//	m_pstbChallengeText = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbChallengeText" ) );
//		
//	m_pbtnCancel = reinterpret_cast<gui::CButton*>( GetComponent( "btnCancel" ) );
//	m_pbtnChallenge = reinterpret_cast<gui::CButton*>( GetComponent( "btnChallenge" ) );
//	m_pbtnClose = reinterpret_cast<gui::CButton*>( GetComponent( "btnClose" ) );
//
//	for( RwInt32 i = 0 ; i < MAX_CHALLENGE_ITEM ; ++i )
//	{
//		m_ChallengeItem[i].Create( m_pThis, GetNtlGuiManager()->GetSurfaceManager(), i );
//	}
//
//	m_slotClickCancel = m_pbtnCancel->SigClicked().Connect( this, &CRBChallengeGui::OnClickCancel );
//	m_slotClickChallenge = m_pbtnChallenge->SigClicked().Connect( this, &CRBChallengeGui::OnClickChallenge );
//	m_slotClickClose = m_pbtnClose->SigClicked().Connect( this, &CRBChallengeGui::OnClickClose );
//	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CRBChallengeGui::OnCaptureMouseDown );
//
//	SetBasicUISetting();
//
//	// Initial Setting
//	m_pThis->Show( false );
//	
//	// Event¿¬°á
//	LinkMsg( g_EventRBRoomInfo );	
//	LinkMsg( g_EventNPCDialogOpen );
//	LinkMsg( g_EventDialog );
//	LinkMsg( g_EventRBMatch );
//	LinkMsg( g_EventRBRoom );
//
//	NTL_RETURN( TRUE );
//}
//
//VOID CRBChallengeGui::Destroy(VOID)
//{
//	UnLinkMsg( g_EventRBRoomInfo );
//	UnLinkMsg( g_EventNPCDialogOpen );
//	UnLinkMsg( g_EventDialog );
//	UnLinkMsg( g_EventRBMatch );
//	UnLinkMsg( g_EventRBRoom );
//
//	CNtlPLGui::DestroyComponents();
//	CNtlPLGui::Destroy();
//}
//
//RwInt32 CRBChallengeGui::SwitchDialog( bool bOpen )
//{
//	if( bOpen )
//	{
//		Show( true );	
//	}
//	else
//	{
//		SetBoardObjectHandle( INVALID_SERIAL_ID );
//		Show( false );
//	}
//
//	return 1;
//}
//
//VOID CRBChallengeGui::HandleEvents( RWS::CMsg& msg )
//{
//	if( msg.Id == g_EventRBRoomInfo )
//	{
//		if( GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle()->IsEnableRBBoard() )
//		{
//			SetChallengeData( msg.pData );
//			GetDialogManager()->OpenDialog( DIALOG_RBCHALLENGE, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
//		}
//		else
//		{
//			GetAlarmManager()->AlarmMessage( DST_RANKBATTLE_MATCH_READY );
//		}
//	}
//	else if( msg.Id == g_EventNPCDialogOpen )
//	{
//		// intend to just store SerialID
//		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>( msg.pData );
//
//		if( pData->eDialog != DIALOG_RBCHALLENGE )
//			return;
//
//		SetBoardObjectHandle( pData->hSerialId );		
//	}
//	else if( msg.Id == g_EventDialog )
//	{
//		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>( msg.pData );
//		if( pData->iType == DIALOGEVENT_NPC_BYEBYE && pData->iDestDialog == DIALOG_RBCHALLENGE )
//		{
//			GetDialogManager()->CloseDialog( DIALOG_RBCHALLENGE );
//		}
//	}
//	else if( msg.Id == g_EventRBMatch )
//	{
//		SNtlEventRBMatch* pMatchData = reinterpret_cast<SNtlEventRBMatch*>( msg.pData );
//
//		if( pMatchData->type == SNtlEventRBMatch::MATCH_READY )
//			OnClickClose( NULL );		
//	}	
//	else if( msg.Id == g_EventRBRoom )
//	{
//		SNtlEventRBRoom* pRoomData = reinterpret_cast<SNtlEventRBRoom*>( msg.pData );
//
//		if( pRoomData->type == SNtlEventRBRoom::ROOM_LEAVE )
//		{
//			GetDialogManager()->CloseDialog( DIALOG_RBCHALLENGE );
//		}		
//	}
//}
//
//VOID CRBChallengeGui::SetBasicUISetting(VOID)
//{
//	m_pstbChallengeText->SetText( GetDisplayStringManager()->GetString( DST_RB_APPLY_CHALLANGE ) );
//
//	m_pbtnCancel->SetText( GetDisplayStringManager()->GetString( DST_RB_CANCEL ) );
//	m_pbtnChallenge->SetText( GetDisplayStringManager()->GetString( DST_RB_OK ) );
//}
//
//VOID CRBChallengeGui::SetChallengeData( VOID* pData )
//{
//	SNtlEventRBRoomInfo* pChallengeData = reinterpret_cast<SNtlEventRBRoomInfo*>( pData );
//	RwInt32 i;
//
//	for( i = 0 ; i < MAX_CHALLENGE_ITEM ; ++i )
//		m_ChallengeItem[i].Show( false );
//
//	if( pChallengeData->byRoomMode == RANKBATTLE_MODE_INDIVIDUAL )
//	{
//		sRANKBATTLE_ROOMINFO_INDIVIDUAL* pIndividualData = reinterpret_cast<sRANKBATTLE_ROOMINFO_INDIVIDUAL*>( pChallengeData->pRoomInfo );
//
//		m_pstbDefenderName->SetText( pIndividualData->sMember.wszName );
//		m_uiRoomID = pIndividualData->roomId;
//		SetWinCount( pIndividualData->wWinCount );
//		
//		m_ChallengeItem[0].SetItemData( pIndividualData->sMember.wszName, pIndividualData->sMember.byLevel, pIndividualData->sMember.byClass );								
//		m_ChallengeItem[0].Show( true );
//	}
//	else if( pChallengeData->byRoomMode == RANKBATTLE_MODE_PARTY )
//	{
//		sRANKBATTLE_ROOMINFO_PARTY* pPartyData = reinterpret_cast<sRANKBATTLE_ROOMINFO_PARTY*>( pChallengeData->pRoomInfo );
//
//		m_pstbDefenderName->SetText( pPartyData->wszPartyName );
//		m_uiRoomID = pPartyData->roomId;
//		SetWinCount( pPartyData->wWinCount );
//		
//		for( i = 0 ; i < pPartyData->byMemberCount ; ++i )
//		{
//			m_ChallengeItem[i].SetItemData( pPartyData->asMember[i].wszName, pPartyData->asMember[i].byLevel, pPartyData->asMember[i].byClass );
//			m_ChallengeItem[i].Show( true );
//		}
//	}
//	else
//	{
//		NTL_ASSERTFAIL( "CRBChallengeGui::SetChallengeData : Wrong Room Info Mode" );
//	}
//}
//
//VOID CRBChallengeGui::SetBoardObjectHandle( RwUInt32 hSerial )
//{
//	m_hSerial = hSerial;
//}
//
//VOID CRBChallengeGui::SetWinCount( RwUInt16 wWinCount )
//{
//	m_pmdbDefenderBattleRecord->SetItem( wWinCount, "2", 0, COMP_TEXT_LEFT, RBCHALLENGE_WINCOUNT_COLOR, 3 );
//	m_pmdbDefenderBattleRecord->SetItem( GetDisplayStringManager()->GetString( DST_RB_STRAIGHT_WIN ), "3", 0, COMP_TEXT_LEFT, RBCHALLENGE_WINCOUNT_TEXT_COLOR, 3, TRUE );
//	m_pmdbDefenderBattleRecord->DrawItem();
//}
//
//VOID CRBChallengeGui::OnClickCancel( gui::CComponent* pComponent )
//{
//	GetDialogManager()->CloseDialog( DIALOG_RBCHALLENGE );
//}
//
//VOID CRBChallengeGui::OnClickChallenge( gui::CComponent* pComponent )
//{
//	GetDboGlobal()->GetGamePacketGenerator()->SendRBChallengeReq( m_hSerial, m_uiRoomID );		
//	GetDialogManager()->CloseDialog( DIALOG_RBCHALLENGE, FALSE );	
//}
//
//VOID CRBChallengeGui::OnClickClose( gui::CComponent* pComponent )
//{
//	GetDialogManager()->CloseDialog( DIALOG_RBCHALLENGE );
//}
//
//VOID CRBChallengeGui::OnCaptureMouseDown( const CKey& key )
//{
//	CAPTURE_MOUSEDOWN_RAISE( DIALOG_RBCHALLENGE, key.m_fX, key.m_fY );
//}