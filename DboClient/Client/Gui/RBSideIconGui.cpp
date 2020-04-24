#include "precomp_dboclient.h"
#include "RBSideIconGui.h"

// core
#include "NtlDebug.h"

// server
#include "DboPacketHandler.h"

// presentation
#include "NtlPLGuiManager.h"

// Sound
#include "GUISoundDefine.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlParty.h"

#include "NtlWorldConceptRB.h"
#include "NtlSLRankBattle.h"

// client
#include "DboGlobal.h"
#include "DboPacketGenerator.h"
#include "DisplayStringManager.h"

#define RANKBATTLE_SIDE_FONTCOLOR_NORMAL	INFOCOLOR_0
#define RANKBATTLE_SIDE_FONTCOLOR_ERROR		INFOCOLOR_5

#define RANKBATTLE_SIDE_VIEW_BOTTOM_MARGIN	15
#define RANKBATTLE_SIDE_VIEW_BUTTON_YDELTA	( RANKBATTLE_SIDE_VIEW_BOTTOM_MARGIN + 23 )
#define RANKBATTLE_SIDE_VIEW_BUTTON_XDELTA	15
#define RANKBATTLE_SIDE_VIEW_CLOSE_BUTTON_XYDELTA 15
#define RANKBATTLE_SIDE_VIEW_RESULT_MARGIN	15
#define RANKBATTLE_SIDE_VIEW_ENTER_TIME	10.0f

#define RANKBATTLE_SIDE_VIEW_SHOW_TIME	5.0f

CRBSideIconGui::CRBSideIconGui( const RwChar* pName )
: CSideIconBase( pName ), m_pbtnIcon( NULL )
{

}

CRBSideIconGui::~CRBSideIconGui(VOID)
{

}

RwBool CRBSideIconGui::Create(VOID)
{
	NTL_FUNCTION( "CRBSideIconGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\RankBattleSide.srf", "gui\\RankBattleSideIcon.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pbtnIcon = (gui::CButton*)GetComponent( "btnIcon" );

	m_slotIconButtonClicked = m_pbtnIcon->SigClicked().Connect( this, &CRBSideIconGui::OnIconButtonClicked );

	LinkMsg( g_EventRBJoin );
	//LinkMsg( g_EventPartyUpdateValue );
	//LinkMsg( g_EventRBBattleEnd );
	
	Show( false );

	NTL_RETURN( TRUE );
}

VOID CRBSideIconGui::Destroy(VOID)
{
	UnLinkMsg( g_EventRBJoin );
	//UnLinkMsg( g_EventPartyUpdateValue );
	//UnLinkMsg( g_EventRBBattleEnd );
	
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CRBSideIconGui::OnSideViewClosed(VOID)
{
	
}

VOID CRBSideIconGui::HandleEvents( RWS::CMsg &msg )
{
/*	if( msg.Id == g_EventRBRoom )
	{
		SNtlEventRBRoom* pRoomData = reinterpret_cast<SNtlEventRBRoom*>( msg.pData );

		if( pRoomData->type == SNtlEventRBRoom::ROOM_CREATE ||
			pRoomData->type == SNtlEventRBRoom::ROOM_CHALLENGE ||
			pRoomData->type == SNtlEventRBRoom::ROOM_JOIN_CHALLENGE ||
			pRoomData->type == SNtlEventRBRoom::ROOM_JOIN_OWNER )
		{
			Show( true );			
			CSideIconGui::GetInstance()->OpenSideView( this, SIDEVIEW_RB, NULL );	
			
			Logic_PlayGUISound(GSD_SYSTEM_WINDOW_ALREAT);
		}
		else if( pRoomData->type == SNtlEventRBRoom::ROOM_LEAVE )
		{
			Show( false );
		}
	}*/
	if( msg.Id == g_EventRBJoin )
	{
		SNtlEventRBJoin* pJoin = reinterpret_cast<SNtlEventRBJoin*>( msg.pData );

		if( pJoin->eType == SNtlEventRBJoin::STATE_JOIN )
		{
			Show( true );			
			CSideIconGui::GetInstance()->OpenSideView( this, SIDEVIEW_RB, NULL );	

			Logic_PlayGUISound(GSD_SYSTEM_WINDOW_ALREAT);
		}
		else if( pJoin->eType == SNtlEventRBJoin::STATE_LEAVE  || 
				 pJoin->eType == SNtlEventRBJoin::STATE_CANCELED )
		{
			Show( false );
		}
	}
	//else if( msg.Id == g_EventPartyUpdateValue )
	//{
	//	SNtlEventPartyUpdate* pData = reinterpret_cast<SNtlEventPartyUpdate*>(msg.pData);

	//	if( pData->iMessage == PMT_PARTY_LEAVE )
	//	{
	//		if( GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle()->IsPartyBattle() )
	//			Show( false );
	//	}
	//}
	//else if( msg.Id == g_EventRBMatch )
	//{
	//	SNtlEventRBMatch* pMatchData = reinterpret_cast<SNtlEventRBMatch*>( msg.pData );

	//	if( pMatchData->type == SNtlEventRBMatch::MATCH_START )			
	//	{
	//		RwUInt16 wResultCode = *((RwUInt16*)pMatchData->pReadyData);

	//		if( wResultCode == GAME_SUCCESS )
	//			Show( false );
	//	}
	//	else if( pMatchData->type == SNtlEventRBMatch::MATCH_CANCELED ||
	//			 pMatchData->type == SNtlEventRBMatch::MATCH_READY )
	//	{
	//		CSideIconGui::GetInstance()->OpenSideView( this, SIDEVIEW_RB, NULL );
	//	}		
	//}	
///*	else if( msg.Id == g_EventRBBattleEnd )
//	{
//		if( GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle()->IsRoomOwner() )
//		{
//			Show( true );
//			Logic_PlayGUISound(GSD_SYSTEM_WINDOW_ALREAT);
//			CSideIconGui::GetInstance()->OpenSideView( this, SIDEVIEW_RB, NULL );	
//		}
//	}*/	
}

VOID CRBSideIconGui::OnIconButtonClicked( gui::CComponent* pComponent )
{
	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();

	if( pRankBattle->GetRBRequestState() == CNtlRankBattle::RB_REQUEST_STATE_REQUEST )
		CDboEventGenerator::NPCDialogOpen( INVALID_SERIAL_ID, DIALOG_RBBOARD );
	//else
		//CSideIconGui::GetInstance()->OpenSideView( this, SIDEVIEW_RB, NULL );	
}

CRBSideViewGui::CRBSideViewGui( const RwChar* pName )
: CSideViewBase( pName ), m_pbtnClose( NULL ), m_pstbInfo( NULL ), m_fCurrentTime( 0.0f ), m_nCurrentTime( 0 )
{

}

CRBSideViewGui::~CRBSideViewGui(VOID)
{

}

RwBool CRBSideViewGui::Create(VOID)
{
	NTL_FUNCTION( "CRBSideViewGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\RankBattleSide.srf", "gui\\RankBattleSideView.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	//m_pstbRemainTime = (gui::CStaticBox*)GetComponent( "stbRemainTime" );
	//m_pbtnCancelRB = (gui::CButton*)GetComponent( "btnCancel" );
	m_pbtnClose = (gui::CButton*)GetComponent( "btnClose" );
	//m_pmdbInfo = (gui::CMDStaticBox*)GetComponent( "mdbInfo" );
	m_pstbInfo = (gui::CStaticBox*)GetComponent( "stbInfo" );

	//m_slotCancelRankBattle = m_pbtnCancelRB->SigClicked().Connect( this, &CRBSideViewGui::OnClickCancelRankBattle );
	m_slotClose = m_pbtnClose->SigClicked().Connect( this, &CRBSideViewGui::OnClickClose );	
			
	LinkMsg( g_EventRBJoin );
	//LinkMsg( g_EventRBBattleEnd );
	//LinkMsg( g_EventPartyLeaderChange );
	//LinkMsg( g_EventPartyLeave );
		
	Show( false );

	NTL_RETURN( TRUE );
}

VOID CRBSideViewGui::Destroy(VOID)
{
	GetNtlGuiManager()->RemoveUpdateBeforeFuncWithoutRefCheck( this );

	UnLinkMsg( g_EventRBJoin );
	//UnLinkMsg( g_EventRBBattleEnd );
	//UnLinkMsg( g_EventPartyLeaderChange );
	//UnLinkMsg( g_EventPartyLeave );
	
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CRBSideViewGui::Update( RwReal fElapsed )
{
	SetCurrentTimeInfo( fElapsed );
}

VOID CRBSideViewGui::OnPressESC(VOID)
{

}

VOID CRBSideViewGui::OnSideViewOpen( const VOID* pData )
{
	m_pThis->SetSize( m_pstbInfo->GetWidth(), m_pstbInfo->GetHeight() + RANKBATTLE_SIDE_VIEW_BOTTOM_MARGIN );
	m_pbtnClose->SetPosition( m_pThis->GetWidth() - 19, 6 );

	Show( true );
}

VOID CRBSideViewGui::OnSideViewClose(VOID)
{
	Show( false );	
}

VOID CRBSideViewGui::OnSideViewLocate( const CRectangle& rtSideIcon )
{
	RwInt32 nWidth = m_pThis->GetWidth();
	RwInt32 nHeight = m_pThis->GetHeight();
	
	m_pThis->SetPosition( rtSideIcon.right - nWidth, rtSideIcon.top - nHeight );
}

VOID CRBSideViewGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventRBJoin )
	{
		SNtlEventRBJoin* pJoin = reinterpret_cast<SNtlEventRBJoin*>( msg.pData );

		if( pJoin->eType == SNtlEventRBJoin::STATE_JOIN )
		{
			m_pstbInfo->Format( GetDisplayStringManager()->GetString( "DST_RANKBATTLE_SIDEVIEW_MESSAGE_JOIN" ), GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle()->GetTblData()->wszName );
			SetLifeTimeInfo( RANKBATTLE_SIDE_VIEW_SHOW_TIME );
			OnSideViewOpen( NULL );
		}	
		else if( pJoin->eType == SNtlEventRBJoin::STATE_START )
		{
			if( pJoin->wResultCode == GAME_RANKBATTLE_CANNOT_FIND_OPPONENT )
			{
				m_pstbInfo->SetText( GetDisplayStringManager()->GetString( "DST_RANKBATTLE_SIDEVIEW_MESSAGE_MATCHFAIL" ) );
				SetLifeTimeInfo( RANKBATTLE_SIDE_VIEW_SHOW_TIME );
				OnSideViewOpen( NULL );
			}
		}
	}	
	//if( msg.Id == g_EventRBRoom )		
	//{
	//	SNtlEventRBRoom* pRoomData = reinterpret_cast<SNtlEventRBRoom*>( msg.pData );

	//	if( pRoomData->type == SNtlEventRBRoom::ROOM_CREATE ||
	//		pRoomData->type == SNtlEventRBRoom::ROOM_CHALLENGE ||
	//		pRoomData->type == SNtlEventRBRoom::ROOM_JOIN_OWNER ||
	//		pRoomData->type == SNtlEventRBRoom::ROOM_JOIN_CHALLENGE )
	//	{
	//		SetWaitData();			
	//	}				
	//}
	//else if( msg.Id == g_EventRBMatch )
	//{
	//	SNtlEventRBMatch* pMatchData = reinterpret_cast<SNtlEventRBMatch*>( msg.pData );

	//	if( pMatchData->type == SNtlEventRBMatch::MATCH_READY )
	//	{
	//		sRANKBATTLE_MEMBER_RESULT* paMemberResult = reinterpret_cast<sRANKBATTLE_MEMBER_RESULT*>( pMatchData->pReadyData );			
	//		RwUInt16 wResultCode = GAME_SUCCESS; 

	//		for( RwInt32 i = 0 ; i < pMatchData->byDataCount ; ++i )
	//		{
	//			if( paMemberResult[i].hPc == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
	//			{
	//				wResultCode = paMemberResult[i].wResultCode;
	//				break;
	//			}
	//		}

	//		if( wResultCode == GAME_SUCCESS )
	//		{
	//			SetReadyData( pMatchData->pReadyData, pMatchData->byDataCount );
	//			SetLifeTimeInfo( RANKBATTLE_SIDE_VIEW_ENTER_TIME );
	//		}
	//		else
	//		{				
	//			SetCancelData( wResultCode, CDisplayStringManager::SERVER_SYNC_RESULT_MSG );
	//		}				
	//	}
	//	// Match_Start에서 실패 한 경우만 들어옴.
	//	else if( pMatchData->type == SNtlEventRBMatch::MATCH_CANCELED )
	//	{
	//		SetWaitData();
	//		SetLifeTimeInfo( 0.0f );									
	//	}
	//	// 자신이 취소를 누른 경우 파티전인 경우 BattleEnd를 기다림. 
	//	// 개인전이거나 전원 취소인 경우는 RoomLeave에서 삭제.
	//	else if( pMatchData->type == SNtlEventRBMatch::MATCH_CANCEL )
	//	{
	//		SetCancelData( DST_RANKBATTLE_CANCELED, CDisplayStringManager::CLIENT_ONLY_MSG );
	//	}
	//	else if( pMatchData->type == SNtlEventRBMatch::MATCH_START )
	//	{
	//		RwUInt16 wResultCode = *((RwUInt16*)pMatchData->pReadyData);
	//		CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
	//		
	//		if( wResultCode != GAME_SUCCESS )
	//			SetCancelData( wResultCode, CDisplayStringManager::SERVER_SYNC_RESULT_MSG );			
	//	}
	//}	
	///*else*/ if( msg.Id == g_EventRBBattleEnd )
	//{
	//	if( GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle()->IsRoomOwner() )
	//	{
	//		SetWaitData();			
	//	}		
	//}	
	//else if( msg.Id == g_EventPartyLeaderChange || msg.Id == g_EventPartyLeave )
	//{
	//	SetState( m_eState );
	//}
}

//VOID CRBSideViewGui::SetWaitData(VOID)
//{
//	const WCHAR* pString = GetDisplayStringManager()->GetString( DST_RB_SIDE_WAIT_FOR_ENTER );
//	
//	sRANKBATTLE_TBLDAT* pTblData  = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle()->GetTblData();
//	NTL_ASSERT( pTblData, "CRBSideViewGui::SetWaitData : Invalid RankBattle Table Data" );
//
//	m_pmdbInfo->Clear();
//
//	m_pmdbInfo->Format( "title", 0, COMP_TEXT_LEFT, RANKBATTLE_SIDE_FONTCOLOR_NORMAL, 0, FALSE, pString, pTblData->byMinLevel, pTblData->byMaxLevel );
//	m_pmdbInfo->SetBlankLine();
//	m_pmdbInfo->DrawItem();
//
//	SetState( STATE_WAIT );
//}
//
//VOID CRBSideViewGui::SetReadyData( VOID* pData, RwInt32 nCount )
//{
//	sRANKBATTLE_MEMBER_RESULT* paMemberResult = reinterpret_cast<sRANKBATTLE_MEMBER_RESULT*>( pData );
//	const WCHAR* pString = NULL;
//	CHAR Buf[256];
//
//	sRANKBATTLE_TBLDAT* pTblData = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle()->GetTblData();
//	NTL_ASSERT( pTblData, "CRBSideViewGui::SetWaitData : Invalid RankBattle Table Data" );
//
//	m_pmdbInfo->Clear();
//
//	pString = GetDisplayStringManager()->GetString( DST_RB_SIDE_CHECK_FOR_ENTER );
//	m_pmdbInfo->Format( "title", 0, COMP_TEXT_LEFT, RANKBATTLE_SIDE_FONTCOLOR_NORMAL, 0, FALSE, pString, pTblData->byMinLevel, pTblData->byMaxLevel );
//	m_pmdbInfo->SetBlankLine();
//	pString = GetDisplayStringManager()->GetString( DST_RB_SIDE_DO_ENTER_RANKBATTLE );
//	m_pmdbInfo->SetItem( pString, "Question", 0, COMP_TEXT_LEFT, RANKBATTLE_SIDE_FONTCOLOR_NORMAL );
//	m_pmdbInfo->SetBlankLine();
//
//	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
//	
//	if( pAvatar->GetRankBattle()->IsPartyBattle() )
//	{
//		CNtlParty* pParty = pAvatar->GetParty();
//		COMMUNITY_ITER it = pParty->Begin();
//
//		for( RwInt32 i = 0 ; i < nCount ; ++i, ++it )
//		{
//			sPartyMember* pMember = reinterpret_cast<sPartyMember*>( *it );
//			NTL_ASSERT( pMember, "CRBSideViewGui::SetReadyDaya : There is no player in party" );
//
//			if( paMemberResult[i].wResultCode == GAME_SUCCESS )
//			{
//				sprintf_s( Buf, 256, "Name%d", i );	
//				m_pmdbInfo->SetItem( pMember->wszMemberName, Buf, 0, COMP_TEXT_LEFT, RANKBATTLE_SIDE_FONTCOLOR_NORMAL );
//				sprintf_s( Buf, 256, "Result%d", i );
//				pString = GetDisplayStringManager()->GetString( DST_RB_SIDE_ENTER_SUCESS );
//				m_pmdbInfo->SetItem( pString, Buf, 0, COMP_TEXT_RIGHT, RANKBATTLE_SIDE_FONTCOLOR_NORMAL, RANKBATTLE_SIDE_VIEW_RESULT_MARGIN, TRUE );
//			}
//			else 
//			{
//				sprintf_s( Buf, 256, "Name%d", i );	
//				m_pmdbInfo->SetItem( pMember->wszMemberName, Buf, 0, COMP_TEXT_LEFT, RANKBATTLE_SIDE_FONTCOLOR_ERROR );
//				sprintf_s( Buf, 256, "Result%d", i );
//				pString = GetDisplayStringManager()->GetString( DST_RB_SIDE_ENTER_FAIL );
//
//				const WCHAR* pStringResult = NULL;
//
//				if( paMemberResult[i].wResultCode >= GAME_ROOM_ENTER_TOO_LOW_LEVEL &&
//					paMemberResult[i].wResultCode <= GAME_ROOM_ENTER_MUST_HAVE_NEED_ZENNY )
//					pStringResult = GetDisplayStringManager()->GetString( DST_RB_SIDE_ENTER_TOO_LOW_LEVEL + paMemberResult[i].wResultCode - GAME_ROOM_ENTER_TOO_LOW_LEVEL );
//				else
//					pStringResult = GetDisplayStringManager()->GetString( DST_RB_SIDE_ENTER_STATE_ERROR );
//
//				m_pmdbInfo->Format( Buf, 0, COMP_TEXT_RIGHT, RANKBATTLE_SIDE_FONTCOLOR_ERROR, RANKBATTLE_SIDE_VIEW_RESULT_MARGIN, TRUE, L"%s %s", pString, pStringResult );						
//			}
//		}
//	}	
//	else
//	{
//		m_pmdbInfo->SetItem( Logic_GetName( pAvatar ), "Name", 0, COMP_TEXT_LEFT, RANKBATTLE_SIDE_FONTCOLOR_NORMAL );
//		pString = GetDisplayStringManager()->GetString( DST_RB_SIDE_ENTER_SUCESS );
//		m_pmdbInfo->SetItem( pString, "Result", 0, COMP_TEXT_RIGHT, RANKBATTLE_SIDE_FONTCOLOR_NORMAL, RANKBATTLE_SIDE_VIEW_RESULT_MARGIN, TRUE );
//	}
//
//	m_pmdbInfo->DrawItem();
//
//	SetState( STATE_READY );
//}
//
//VOID CRBSideViewGui::SetCancelData( RwUInt32 uiString, RwUInt32 eType )
//{
//	const WCHAR* pString = GetDisplayStringManager()->GetString( uiString, eType );
//
//	m_pmdbInfo->Clear();
//	m_pmdbInfo->SetBlankLine();
//	m_pmdbInfo->SetItem( pString, "text", 0, COMP_TEXT_CENTER, RANKBATTLE_SIDE_FONTCOLOR_NORMAL );
//	m_pmdbInfo->DrawItem();
//
//	SetState( STATE_CANCEL );
//}
//
//VOID CRBSideViewGui::SetState( RwInt32 eState )
//{
//	m_eState = eState;
//
//	m_pThis->SetSize( m_pmdbInfo->GetWidth(), m_pmdbInfo->GetHeight() + RANKBATTLE_SIDE_VIEW_BUTTON_YDELTA );
//	m_pbtnClose->SetPosition( m_pThis->GetWidth() - 19, 6 );
//
//	RwInt32 nYPos = m_pmdbInfo->GetHeight();
//	
//	if( eState == STATE_WAIT )
//	{
//		CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
//		if( ( pRankBattle->IsPartyBattle() && Logic_I_am_PartyLeader() ) || !pRankBattle->IsPartyBattle() )
//		{
//			m_pbtnCancelRB->Show( true );
//			m_pbtnCancelRB->SetPosition( m_pThis->GetPosition().GetWidth() / 2, nYPos );
//			//m_pbtnCancelRB->SetText( GetDisplayStringManager()->GetString( DST_RB_LEAVEROOM ) );
//		}
//		else
//		{
//			m_pbtnCancelRB->Show( false );
//		}
//		
//		m_pstbRemainTime->Show( false );		
//	}
//	else if( eState == STATE_READY )
//	{
//		m_pbtnCancelRB->Show( true );
//		m_pstbRemainTime->Show( true );
//		
//		m_pbtnCancelRB->SetPosition( m_pThis->GetWidth() - RANKBATTLE_SIDE_VIEW_BUTTON_XDELTA - m_pbtnCancelRB->GetWidth(), nYPos );									 
//		m_pstbRemainTime->SetPosition( RANKBATTLE_SIDE_VIEW_BUTTON_XDELTA, nYPos );		
//
//		//m_pbtnCancelRB->SetText( GetDisplayStringManager()->GetString( DST_RB_CANCEL_BTNTEXT ) );
//	}		
//	else // eState == STATE_CANCEL 
//	{
//		m_pbtnCancelRB->Show( false );
//		m_pstbRemainTime->Show( false );
//	}	
//
//	OnSideViewLocate( CSideIconGui::GetInstance()->GetSideIcon( SIDEICON_RB )->GetPosition() );
//}
//

VOID CRBSideViewGui::SetCurrentTimeInfo( RwReal fElapsedTIme )
{
	m_fCurrentTime -= fElapsedTIme;
	
	if( m_fCurrentTime <= 0.0f )
	{
		SetLifeTimeInfo( 0.0f );
		OnSideViewClose();

		//SetLifeTimeInfo( 0.0f );
		//m_pstbRemainTime->SetText( GetDisplayStringManager()->GetString( DST_RB_SIDE_ON_ENTER ) );
		//m_pbtnCancelRB->Show( false );
	}
	//else if( (RwInt32)( m_fCurrentTime + 0.99f ) != m_nCurrentTime )
	//{
	//	m_nCurrentTime = (RwInt32)( m_fCurrentTime + 0.99f );
	//	m_pstbRemainTime->Format( GetDisplayStringManager()->GetString( DST_RB_SIDE_ON_ENTER_SECOND ), m_nCurrentTime );
	//}	
}

VOID CRBSideViewGui::SetLifeTimeInfo( RwReal fLifeTime )
{
	m_fCurrentTime = fLifeTime;

	if( fLifeTime == 0.0f )
		GetNtlGuiManager()->RemoveUpdateBeforeFuncWithoutRefCheck( this );
	else
		GetNtlGuiManager()->AddUpdateFunc( this );	
}

//VOID CRBSideViewGui::OnClickCancelRankBattle( gui::CComponent* pComponent )
//{
//	//if( m_eState == STATE_WAIT )
//	//{
//	//	if( GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle()->IsRoomOwner() )
//	//		GetDboGlobal()->GetGamePacketGenerator()->SendRBRoomLeaveReq();		
//	//}
//	//else if( m_eState == STATE_READY )
//	//{
//	//	GetDboGlobal()->GetGamePacketGenerator()->SendRBMatchCancelReq();
//	//}
//	//else
//	//	NTL_ASSERTFAIL( "CRBSideViewGui::OnClickCancleRankBattle : Must not show at this state" );
//}

VOID CRBSideViewGui::OnClickClose( gui::CComponent* pComponent )
{
	OnSideViewClose();
}
