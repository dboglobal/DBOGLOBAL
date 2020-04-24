#include "precomp_dboclient.h"
#include "QuestRewardGui.h"

// core
#include "NtlDebug.h"

// shared
#include "QuestTextDataTable.h"
#include "ItemTable.h"
#include "QuestItemTable.h"
#include "SkillTable.h"
#include "TableContainer.h"

// gui
#include "GuiUtil.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "DboTSCQAgency.h"
#include "NtlSLApi.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"

// dbo
#include "DboEvent.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "InfoWndManager.h"
#include "DisplayStringManager.h"
#include "GUISoundDefine.h"
#include "AlarmManager.h"

CQuestRewardGui::CQuestRewardGui( const RwChar* pName )
: CQuestCommonGui( pName )
{
	m_pstbConversationTitle = NULL;
	m_phbxConversation = NULL;

	m_pbtnQuestReward = NULL;
	m_pbtnRewardCancel = NULL;

	m_bCanSelectReward = FALSE;

	m_nSelectIndex = -1;

	m_bResult = false;
}

CQuestRewardGui::~CQuestRewardGui(VOID)
{
	
}

RwBool CQuestRewardGui::Create( CQuestGui* pQuestGui )
{
	NTL_FUNCTION( "CQuestListGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\QuestWnd.srf", "gui\\QuestReward.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	m_pstbTitle = (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pbtnClose = (gui::CButton*)GetComponent( "btnExit" );
	m_pstbQuestTitle = (gui::CStaticBox*)GetComponent( "stbQuestTitle" );
	m_pstbQuestGrade = (gui::CStaticBox*)GetComponent( "stbQuestGrade" );
	m_pstbQuestLevel = (gui::CStaticBox*)GetComponent("stbQuestLevel");

	m_pstbStandardRewardTitle = (gui::CStaticBox*)GetComponent( "stbBasicRewardText" );;
	m_pstbSelectRewardTitle = (gui::CStaticBox*)GetComponent( "stbSelectRewardText" );

	m_pstbRewardGetExpText = (gui::CStaticBox*)GetComponent("stbRewardGetExpText");
	m_pstbRewardGetExpAmountText = (gui::CStaticBox*)GetComponent("stbRewardGetExpAmountText");
	m_pstbRewardGetZennyText = (gui::CStaticBox*)GetComponent("stbRewardGetZennyText");
	m_pstbRewardGetZennyAmountText = (gui::CStaticBox*)GetComponent("stbRewardGetZennyAmountText");
	m_pstbRewardSelectText = (gui::CStaticBox*)GetComponent("stbRewardSelectText");
	m_pstbRewardSelectAmountText = (gui::CStaticBox*)GetComponent("stbRewardSelectAmountText");

	m_pstbConversationTitle = (gui::CStaticBox*)GetComponent( "stbConvTitle" );
	m_phbxConversation = (gui::CHtmlBox*)GetComponent( "hbxConv" );
	
	m_pbtnQuestReward = (gui::CButton*)GetComponent( "btnReward" );
	m_pbtnRewardCancel = (gui::CButton*)GetComponent( "btnCancel" );

	m_slotMouseDown = m_pThis->SigMouseDown().Connect( this, &CQuestRewardGui::OnMouseDown );
	m_slotMouseUp = m_pThis->SigMouseUp().Connect( this, &CQuestRewardGui::OnMouseUp );
	m_slotMouseMove = m_pThis->SigMouseMove().Connect( this, &CQuestRewardGui::OnMouseMove );
	m_slotMouseOut = m_pThis->SigMouseLeave().Connect( this, &CQuestRewardGui::OnMouseOut );
	m_slotPaint = m_pbtnClose->SigPaint().Connect( this, &CQuestRewardGui::OnPaint );
	m_slotMove = m_pThis->SigMove().Connect( this, &CQuestRewardGui::OnMove );

	m_slotClickExitButton = m_pbtnClose->SigClicked().Connect( this, &CQuestRewardGui::OnClickExitButton );
	m_slotClickRewardButton = m_pbtnQuestReward->SigClicked().Connect( this, &CQuestRewardGui::OnClickRewardButton );
	m_slotClickCancelButton = m_pbtnRewardCancel->SigClicked().Connect( this, &CQuestRewardGui::OnClickExitButton );
	m_slotWheelMoveConv = m_phbxConversation->SigMouseWheel().Connect( this, &CQuestRewardGui::OnWheelMoveConv );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CQuestRewardGui::OnCaptureMouseDown );


	m_surSelectEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_surSelectEffect.SetRectWH( 0, 0, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );

	m_phbxConversation->SetLineSpace( 7 );

	// 보상영역 설정.
	SetRewardRectHardcode();

	// 기본설정 세팅
	SetBasicUISetting();

	SelectIndex( -1 );

	Show( false );

	m_pQuestGui = pQuestGui;

	NTL_RETURN( TRUE );
}

VOID CQuestRewardGui::Destroy(VOID)
{
	DeleteAllRewardSlot();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CQuestRewardGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	if( bOpen )
	{
		m_bResult = false;
	}
	else
	{
		SendEchoEvent( m_bResult );
	}

	return 1;
}

VOID CQuestRewardGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventQuestDirectForward )
	{
		SNtlEventQuestDirect_Forward* pData = reinterpret_cast<SNtlEventQuestDirect_Forward*>( msg.pData );

		// peessi : 중복해서 Proposal이 날라오면 새로들어온 녀석을 Fail 응답처리.		
		if( m_pTCUnit )
		{
			SNtlEventQuestDirect_Echo stEcho;
			stEcho.sProposal.bResult = false;
			stEcho.sProposal.sTSKey	= pData->sReward.sTSKey;
			stEcho.eTDType = ETD_QuestReward;
			stEcho.pTCUnit = pData->pTCUnit;

			CNtlSLEventGenerator::QuestTriggerDirectEcho( &stEcho );
			return;
		}

		SetQuestData( pData );

		if( !IsShow() && !GetDialogManager()->OpenDialog( DIALOG_QUESTREWARD, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() ) )
			SendEchoEvent( false );
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>( msg.pData );

		if( pData->iType == DIALOGEVENT_NPC_BYEBYE && pData->iDestDialog == DIALOG_QUESTREWARD )
		{
			OnClickExitButton( NULL );
		}
	}
	else if( msg.Id == g_EventUnregQuest_Nfy )
	{
		SNtlEventUnregQuest_Nfy* pDeleteData = reinterpret_cast<SNtlEventUnregQuest_Nfy*>( msg.pData );

		if( pDeleteData->sTSKey.tID == m_TSKey.tID )
			GetDialogManager()->CloseDialog( DIALOG_QUESTREWARD );		
	}
	else if( msg.Id == g_EventTSRemovingTMQQuest_Nfy )
	{
		SNtlEventTSRemovingTMQQuest_Nfy* pData = reinterpret_cast<SNtlEventTSRemovingTMQQuest_Nfy*>( msg.pData );

		if( pData->tRmvTSId == m_TSKey.tID )
			GetDialogManager()->CloseDialog( DIALOG_QUESTREWARD );
	}
}

VOID CQuestRewardGui::SetBasicUISetting(VOID)
{
	m_pstbTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_REWARD_WINDOW_TITLE" ) );

	m_pbtnQuestReward->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_REWARD_BTN" ) );
	m_pbtnQuestReward->SetTextDownColor( NTL_BUTTON_DOWN_COLOR );
	m_pbtnQuestReward->SetTextFocusColor( NTL_BUTTON_FOCUS_COLOR );
	m_pbtnQuestReward->SetTextUpColor( NTL_BUTTON_UP_COLOR );
	m_pbtnQuestReward->SetTextDownCoordDiff( NTL_BUTTON_CLICK_DIFFX, NTL_BUTTON_CLICK_DIFFY );
	m_pbtnQuestReward->ApplyText();	

	m_pbtnRewardCancel->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_CANCEL_BTN" ) );
	m_pbtnRewardCancel->SetTextDownColor( NTL_BUTTON_DOWN_COLOR );
	m_pbtnRewardCancel->SetTextFocusColor( NTL_BUTTON_FOCUS_COLOR );
	m_pbtnRewardCancel->SetTextUpColor( NTL_BUTTON_UP_COLOR );
	m_pbtnRewardCancel->SetTextDownCoordDiff( NTL_BUTTON_CLICK_DIFFX, NTL_BUTTON_CLICK_DIFFY );
	m_pbtnRewardCancel->ApplyText();	

	m_pstbConversationTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_TEXT_CONVERSATION" ) );
}

VOID CQuestRewardGui::SetQuestData( SNtlEventQuestDirect_Forward* pData )
{
	// 다시 보내주어야할 Data
	m_pTCUnit = pData->pTCUnit;
	m_TSKey = pData->sReward.sTSKey;

	// Text Info
	SNtlEventQuestRewardDialog_Req* pRewardData = &pData->sReward;
	
	// Title
	SetQuestTitle( pRewardData->uiQuestTitle, pRewardData->uiQuestSort, pRewardData->eGradeType );

	// Body
	CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
	sQUEST_TEXT_DATA_TBLDAT* pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pRewardData->uiQuestContents ) );
	if( pQuestText )
		m_phbxConversation->SetHtmlFromMemory( pQuestText->wstrText.c_str(), pQuestText->wstrText.size() );
	else
		m_phbxConversation->Clear();

	// Reward Info
	SelectIndex( -1 );
	
	for( RwInt32 i = 0 ; i < QUEST_MAX_REWARD_SLOT; ++i )
	{
		SetRewardSlot( i, &pRewardData->sSelectReward[i]);
		SetRewardSlot( i + QUEST_MAX_REWARD_SLOT, &pRewardData->sDefaultReward[i]);
	}			

	m_pstbRewardGetExpAmountText->SetText(pRewardData->uiRewardExp);
	m_pstbRewardGetZennyAmountText->SetText(pRewardData->uiRewardZeni);

	if (IsAvailableSelect(0))
	{
		m_bCanSelectReward = TRUE;

		m_pstbRewardSelectText->Show();
		m_pstbRewardSelectAmountText->Show();
	}
	else
	{
		m_bCanSelectReward = FALSE;

		m_pstbRewardSelectText->Show(false);
		m_pstbRewardSelectAmountText->Show(false);
	}
}

VOID CQuestRewardGui::SendEchoEvent( bool bConfirm )
{
	if( m_pTCUnit )
	{
		SNtlEventQuestDirect_Echo stEcho;
		stEcho.sReward.bResult		= bConfirm;
		stEcho.sReward.sTSKey		= m_TSKey;
		stEcho.sReward.nSelRwdIdx	= m_nSelectIndex;
		stEcho.eTDType		= ETD_QuestReward;
		stEcho.pTCUnit		= m_pTCUnit;

		CNtlSLEventGenerator::QuestTriggerDirectEcho( &stEcho );
		InitQuestData();
	}	
}

VOID CQuestRewardGui::SelectIndex( RwInt32 nIdx )
{
	if( !IsAvailableSelect( nIdx ) )
	{
		m_pstbRewardSelectAmountText->SetText(L"0 / 1");
		m_surSelectEffect.Show( FALSE );
		m_nSelectIndex = -1;
		return;
	}
	
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_nSelectIndex = nIdx;
	m_surSelectEffect.SetPosition( m_rtReward[nIdx].left + rtScreen.left, m_rtReward[nIdx].top + rtScreen.top );
	m_surSelectEffect.Show( TRUE );

	m_pstbRewardSelectAmountText->SetText(L"1 / 1");
}

// CallBack Message
VOID CQuestRewardGui::OnMouseDown( const CKey& key )
{
	RwInt32 nClickIdx = GetRewardSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( IsAvailableSelect( nClickIdx ) )
		{
			m_nLClickIdx = nClickIdx;
		}
	}
	
	m_pThis->CaptureMouse();
}

VOID CQuestRewardGui::OnMouseUp( const CKey& key )
{
	RwInt32 nClickIdx = GetRewardSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );

	m_pThis->ReleaseMouse();

	if( nClickIdx < 0 || !IsShow() )
	{
		m_nLClickIdx = -1;
		return;
	}

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( m_nLClickIdx == nClickIdx )
		{	
			SelectIndex( nClickIdx );
		}

		m_nLClickIdx = -1;
	}
}

VOID CQuestRewardGui::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	CQuestCommonGui::OnMouseMove( nKey, nXPos, nYPos );
}

VOID CQuestRewardGui::OnMouseOut( gui::CComponent* pComponent )
{
	CQuestCommonGui::OnMouseOut( pComponent );
}

VOID CQuestRewardGui::OnPaint(VOID)
{
	CQuestCommonGui::OnPaint();

	if( m_nSelectIndex >= 0 )
		m_surSelectEffect.Render();
}

VOID CQuestRewardGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	CQuestCommonGui::OnMove( nX, nY );

	CRectangle rtScreen = m_pThis->GetScreenRect();

	if( m_nSelectIndex >= 0 )
		m_surSelectEffect.SetPosition( m_rtReward[m_nSelectIndex].left + rtScreen.left, m_rtReward[m_nSelectIndex].top + rtScreen.top );
}

VOID CQuestRewardGui::OnClickExitButton( gui::CComponent* pComponent )
{
	m_bResult = false;
	GetDialogManager()->CloseDialog( DIALOG_QUESTREWARD );
}

VOID CQuestRewardGui::OnClickRewardButton( gui::CComponent* pComponent )
{
	if( m_bCanSelectReward && m_nSelectIndex < 0 )
	{
		GetAlarmManager()->AlarmMessage( "DST_MUST_SELECT_REWARD" );
		return;
	}	

	m_bResult = true;
	GetDialogManager()->CloseDialog( DIALOG_QUESTREWARD );
}


VOID CQuestRewardGui::OnWheelMoveConv( RwInt32 nFlag, RwInt16 sDelta, CPos& pos )
{
	gui::CScrollBar* pScrollBar = m_phbxConversation->GetScrollBar();

	RwInt32 nValue = pScrollBar->GetValue();
	RwInt32 nMaxValue = pScrollBar->GetMaxValue();
	RwInt32 nDelta = nValue - ( sDelta * 18 ) / GUI_MOUSE_WHEEL_DELTA;

	if( nDelta < 0 )
		nDelta = 0;
	else if( nDelta > nMaxValue )
		nDelta = nMaxValue;

	pScrollBar->SetValue( nDelta );
}

VOID CQuestRewardGui::OnCaptureMouseDown( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_QUESTREWARD, key.m_fX, key.m_fY );
}