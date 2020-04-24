#include "precomp_dboclient.h"
#include "QuestProposalGui.h"

// shared
#include "QuestTextDataTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// gui
#include "GuiUtil.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "DboTSCQAgency.h"
#include "NtlSLApi.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "DisplayStringManager.h"
#include "GUISoundDefine.h"

CQuestProposalGui::CQuestProposalGui( const RwChar* pName )
: CQuestCommonGui( pName )
{
	m_pstbConversationTitle = NULL;
	m_phbxConversation = NULL;

	m_pstbAimTitle = NULL;
	m_phbxAim = NULL;

	m_pbtnQuestConfirm = NULL;
	m_pbtnQuestReject = NULL;

	m_bResult = false;
}

CQuestProposalGui::~CQuestProposalGui(VOID)
{

}

RwBool CQuestProposalGui::Create( CQuestGui* pQuestGui )
{
	NTL_FUNCTION( "CQuestListGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\QuestWnd.srf", "gui\\QuestProposal.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	m_pstbTitle = (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pbtnClose = (gui::CButton*)GetComponent( "btnExit" );
	m_pstbStandardRewardTitle = (gui::CStaticBox*)GetComponent( "stbBasicRewardText" );
	m_pstbSelectRewardTitle = (gui::CStaticBox*)GetComponent( "stbSelectRewardText" );

	m_pstbRewardGetExpText = (gui::CStaticBox*)GetComponent("stbRewardGetExpText");
	m_pstbRewardGetExpAmountText = (gui::CStaticBox*)GetComponent("stbRewardGetExpAmountText");
	m_pstbRewardGetZennyText = (gui::CStaticBox*)GetComponent("stbRewardGetZennyText");
	m_pstbRewardGetZennyAmountText = (gui::CStaticBox*)GetComponent("stbRewardGetZennyAmountText");
	m_pstbRewardSelectText = (gui::CStaticBox*)GetComponent("stbRewardSelectText");
	m_pstbRewardSelectAmountText = (gui::CStaticBox*)GetComponent("stbRewardSelectAmountText");

	m_pstbQuestTitle = (gui::CStaticBox*)GetComponent( "stbQuestTitle" );
	m_pstbQuestGrade = (gui::CStaticBox*)GetComponent( "stbQuestGrade" );
	m_pstbQuestLevel = (gui::CStaticBox*)GetComponent("stbQuestLevel");

	m_pstbConversationTitle = (gui::CStaticBox*)GetComponent( "stbConvTitle" );
	m_phbxConversation = (gui::CHtmlBox*)GetComponent( "hbxConv" );
	m_pstbAimTitle = (gui::CStaticBox*)GetComponent( "stbAimTitle" );		
	m_phbxAim = (gui::CHtmlBox*)GetComponent( "hbxAim" );
		
	m_pbtnQuestConfirm = (gui::CButton*)GetComponent( "btnConfirm" );
	m_pbtnQuestReject = (gui::CButton*)GetComponent( "btnReject" );

	m_pBtnHelp = (gui::CButton*)GetComponent( "btnHelp" );
	
	m_slotClickedBtnHelp = m_pBtnHelp->SigClicked().Connect( this, &CQuestProposalGui::OnClickedBtnHelp );
	m_slotClickExitButton = m_pbtnClose->SigClicked().Connect( this, &CQuestProposalGui::OnClickRejectButton );
	m_slotClickConfirmButton = m_pbtnQuestConfirm->SigClicked().Connect( this, &CQuestProposalGui::OnClickCinfirmButton );
	m_slotClickRejectButton = m_pbtnQuestReject->SigClicked().Connect( this, &CQuestProposalGui::OnClickRejectButton );
	
	m_slotMouseMove = m_pThis->SigMouseMove().Connect( this, &CQuestProposalGui::OnMouseMove );
	m_slotMouseOut = m_pThis->SigMouseLeave().Connect( this, &CQuestProposalGui::OnMouseOut );
	m_slotPaint = m_pbtnClose->SigPaint().Connect( this, &CQuestProposalGui::OnPaint );
	m_slotMove = m_pThis->SigMove().Connect( this, &CQuestProposalGui::OnMove );
	m_slotWheelMoveConv = m_phbxConversation->SigMouseWheel().Connect( this, &CQuestProposalGui::OnWheelMoveConv );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CQuestProposalGui::OnCaptureMouseDown );

	m_phbxConversation->SetLineSpace( 7 );
	m_phbxAim->SetLineSpace( 7 );

	// 보상영역 설정.
	SetRewardRectHardcode();

	// 기본설정 세팅
	SetBasicUISetting();
	
	Show( false );

	m_pQuestGui = pQuestGui;

	NTL_RETURN( TRUE );
}

VOID CQuestProposalGui::Destroy(VOID)
{
	DeleteAllRewardSlot();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CQuestProposalGui::SwitchDialog( bool bOpen )
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

VOID CQuestProposalGui::SendEchoEvent( bool bConfirm )
{
	if( m_pTCUnit )
	{
		SNtlEventQuestDirect_Echo stEcho;
		stEcho.sProposal.bResult = bConfirm;
		stEcho.sProposal.sTSKey	= m_TSKey;
		stEcho.eTDType = ETD_QuestProposal;
		stEcho.pTCUnit = m_pTCUnit;

		CNtlSLEventGenerator::QuestTriggerDirectEcho( &stEcho );
		InitQuestData();
	}	
}

VOID CQuestProposalGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventQuestDirectForward )
	{
		SNtlEventQuestDirect_Forward* pData = reinterpret_cast<SNtlEventQuestDirect_Forward*>( msg.pData );
		
		// peessi : 중복해서 Proposal이 날라오면 새로들어온 녀석을 Fail 응답처리.		
		if( m_pTCUnit )
		{
			SNtlEventQuestDirect_Echo stEcho;
			stEcho.sProposal.bResult = false;
			stEcho.sProposal.sTSKey	= pData->sProposal.sTSKey;
			stEcho.eTDType = ETD_QuestProposal;
			stEcho.pTCUnit = pData->pTCUnit;

			CNtlSLEventGenerator::QuestTriggerDirectEcho( &stEcho );
			return;
		}

		SetQuestData( pData );

		if( !pData->sProposal.bDisplaySummary )
		{
			if( !IsShow() && !GetDialogManager()->OpenDialog( DIALOG_QUESTPROPOSAL, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() ) )
				SendEchoEvent( false );
		}		
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>( msg.pData );

		if( pData->iType == DIALOGEVENT_NPC_BYEBYE && pData->iDestDialog == DIALOG_QUESTPROPOSAL )
		{
			OnClickRejectButton( NULL );
		}
	}
	else if( msg.Id == g_EventUnregQuest_Nfy )
	{
		SNtlEventUnregQuest_Nfy* pDeleteData = reinterpret_cast<SNtlEventUnregQuest_Nfy*>( msg.pData );

		if( pDeleteData->sTSKey.tID == m_TSKey.tID )
			GetDialogManager()->CloseDialog( DIALOG_QUESTPROPOSAL );		
	}
	else if( msg.Id == g_EventTSRemovingTMQQuest_Nfy )
	{
		SNtlEventTSRemovingTMQQuest_Nfy* pData = reinterpret_cast<SNtlEventTSRemovingTMQQuest_Nfy*>( msg.pData );

		if( pData->tRmvTSId == m_TSKey.tID )
			GetDialogManager()->CloseDialog( DIALOG_QUESTPROPOSAL );
	}
}

VOID CQuestProposalGui::SetBasicUISetting(VOID)
{
	m_pstbTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_PROPOSAL_WINDOW_TITLE" ) );

	m_pstbConversationTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_TEXT_CONVERSATION" ) );
	m_pstbAimTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_TEXT_AIM" )  );
	
	m_pbtnQuestConfirm->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_ACCEPT_BTN" ) );
	m_pbtnQuestConfirm->SetTextDownColor( NTL_BUTTON_DOWN_COLOR );
	m_pbtnQuestConfirm->SetTextFocusColor( NTL_BUTTON_FOCUS_COLOR );
	m_pbtnQuestConfirm->SetTextUpColor( NTL_BUTTON_UP_COLOR );
	m_pbtnQuestConfirm->SetTextDownCoordDiff( NTL_BUTTON_CLICK_DIFFX, NTL_BUTTON_CLICK_DIFFY );
	m_pbtnQuestConfirm->ApplyText();

	m_pbtnQuestReject->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_REJECT_BTN" ) );
	m_pbtnQuestReject->SetTextDownColor( NTL_BUTTON_DOWN_COLOR );
	m_pbtnQuestReject->SetTextFocusColor( NTL_BUTTON_FOCUS_COLOR );
	m_pbtnQuestReject->SetTextUpColor( NTL_BUTTON_UP_COLOR );
	m_pbtnQuestReject->SetTextDownCoordDiff( NTL_BUTTON_CLICK_DIFFY, NTL_BUTTON_CLICK_DIFFY );
	m_pbtnQuestReject->ApplyText();	
}

VOID CQuestProposalGui::SetQuestData( SNtlEventQuestDirect_Forward* pData )
{
	// 다시 보내주어야할 Data
	m_pTCUnit = pData->pTCUnit;
	m_TSKey = pData->sProposal.sTSKey;

	SNtlEventQuestProposalDialog_Req* pProposalData = &pData->sProposal;

	// 제목
	SetQuestTitle( pProposalData->uiQuestTitle, pProposalData->uiQuestSort, pProposalData->eGradeType );

	CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
	sQUEST_TEXT_DATA_TBLDAT* pQuestText;
	
	// 내용
	pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pProposalData->uiQuestContents ) );
	if( pQuestText )
		m_phbxConversation->SetHtmlFromMemory( pQuestText->wstrText.c_str(), pQuestText->wstrText.size() );
	else
		m_phbxConversation->Clear();

	// 목표
	pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pProposalData->uiQuestGoal ) );
	if( pQuestText )
		m_phbxAim->SetHtmlFromMemory( pQuestText->wstrText.c_str(), pQuestText->wstrText.size() );
	else
		m_phbxAim->Clear();

	// 보상정보
	for( RwInt32 i = 0 ; i < QUEST_MAX_REWARD_SLOT ; ++i )
	{
		SetRewardSlot( i, &pProposalData->sSelectReward[i]);
		SetRewardSlot( i + QUEST_MAX_REWARD_SLOT, &pProposalData->sDefaultReward[i]);
	}

	m_pstbRewardGetExpAmountText->SetText(pProposalData->uiRewardExp);
	m_pstbRewardGetZennyAmountText->SetText(pProposalData->uiRewardZeni);

	if (IsAvailableSelect(0))
	{
		m_pstbRewardSelectText->Show();
		m_pstbRewardSelectAmountText->Show();
	}
	else
	{
		m_pstbRewardSelectText->Show(false);
		m_pstbRewardSelectAmountText->Show(false);
	}
}

// CallBack Message

VOID CQuestProposalGui::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	CQuestCommonGui::OnMouseMove( nKey, nXPos, nYPos );
}

VOID CQuestProposalGui::OnMouseOut( gui::CComponent* pComponent )
{
	CQuestCommonGui::OnMouseOut( pComponent );
}

VOID CQuestProposalGui::OnPaint(VOID)
{
	CQuestCommonGui::OnPaint();
}

VOID CQuestProposalGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	CQuestCommonGui::OnMove( nX, nY );
}

VOID CQuestProposalGui::OnClickCinfirmButton( gui::CComponent* pComponent )
{
	m_bResult = true;	
	GetDialogManager()->CloseDialog( DIALOG_QUESTPROPOSAL );
}

VOID CQuestProposalGui::OnClickRejectButton( gui::CComponent* pComponent )
{
	m_bResult = false;
	GetDialogManager()->CloseDialog( DIALOG_QUESTPROPOSAL );
}

VOID CQuestProposalGui::OnWheelMoveConv( RwInt32 nFlag, RwInt16 sDelta, CPos& pos )
{
	gui::CScrollBar* pScrollBar = m_phbxConversation->GetScrollBar();

	RwInt32 nValue = pScrollBar->GetValue();
	RwInt32 nMaxValue = pScrollBar->GetMaxValue();
	RwInt32 nDelta = nValue - ( sDelta * 18 )/ GUI_MOUSE_WHEEL_DELTA;

	if( nDelta < 0 )
		nDelta = 0;
	else if( nDelta > nMaxValue )
		nDelta = nMaxValue;

	pScrollBar->SetValue( nDelta );	
}

VOID CQuestProposalGui::OnCaptureMouseDown( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_QUESTPROPOSAL, key.m_fX, key.m_fY );
}

VOID CQuestProposalGui::OnClickedBtnHelp( gui::CComponent* pComponent )
{
	CDboEventGenerator::OpenHelpContent( DIALOG_QUESTPROPOSAL );
}