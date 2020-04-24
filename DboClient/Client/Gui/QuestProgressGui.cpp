#include "precomp_dboclient.h"
#include "QuestProgressGui.h"

// core
#include "NtlDebug.h"

// shared
#include "QuestTextDataTable.h"
#include "NPCTable.h"
#include "MobTable.h"
#include "QuestItemTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// Gui
#include "GuiUtil.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "GUISoundDefine.h"
#include "AlarmManager.h"

#define QUEST_PROGRESS_INFO_WIDTH					223
#define QUEST_PROGRESS_TIME_WIDTH					80
#define QUEST_PROGRESS_GOALINFO_LIMITWIDTH_TIME		( QUEST_PROGRESS_INFO_WIDTH - QUEST_PROGRESS_TIME_WIDTH )

#define QUEST_PROGRESS_AIM_HTML_X					18
#define QUEST_PROGRESS_AIM_HTML_Y					116
#define QUEST_PROGRESS_AIM_HTML_WIDTH				231
#define QUEST_PROGRESS_AIM_HTML_HEIGHT				147
#define QUEST_PROGRESS_AIM_HTML_Y_WITH_TARGETINFO		161
#define QUEST_PROGRESS_AIM_HTML_HEIGHT_WITH_TARGETINFO	102

RwInt32 g_GiveUp = 0;

CQuestProgressGui::CQuestProgressGui( const RwChar* pName )
: CQuestCommonGui( pName )
{
	m_ppnlConversationBack = NULL;
	m_phbxConversation = NULL;

	m_pstbAimTargetTitle = NULL;
	m_pstbAimTargetValue = NULL;
	m_pstbAimTitle = NULL;		

	m_ppnlAimBack = NULL;
	m_phbxAim = NULL;

	m_pstbProgressTitle = NULL;
	m_pstbTime = NULL;
	m_pstbTimeTitle = NULL;
	m_ppnlTimeBack = NULL;
	m_ppnlProgressBack = NULL;
	
	for( RwInt32 i = 0 ; i < QUEST_MAX_GOAL_COUNT ; ++i )
	{
		m_pstbProgressInfo[i] = NULL;
		m_pstbProgressResult[i] = NULL;
	}

	m_pbtnQuestGiveUp = NULL;
	m_ptbtTab = NULL;

	m_bShowTimeUI = FALSE;
}

CQuestProgressGui::~CQuestProgressGui(VOID)
{

}

RwBool CQuestProgressGui::Create( CQuestGui* pQuestGui )
{
	NTL_FUNCTION( "CQuestProgressGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\QuestWnd.srf", "gui\\QuestProgress.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	
	m_pstbTitle = (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pbtnClose = (gui::CButton*)GetComponent( "btnExit" );
	m_pstbQuestTitle = (gui::CStaticBox*)GetComponent( "stbQuestTitle" );
	m_pstbQuestGrade = (gui::CStaticBox*)GetComponent( "stbQuestGrade" );
	m_pstbQuestLevel = (gui::CStaticBox*)GetComponent("stbQuestLevel");

	m_pstbRewardGetExpText = (gui::CStaticBox*)GetComponent("stbRewardGetExpText");
	m_pstbRewardGetExpAmountText = (gui::CStaticBox*)GetComponent("stbRewardGetExpAmountText");
	m_pstbRewardGetZennyText = (gui::CStaticBox*)GetComponent("stbRewardGetZennyText");
	m_pstbRewardGetZennyAmountText = (gui::CStaticBox*)GetComponent("stbRewardGetZennyAmountText");
	m_pstbRewardSelectText = (gui::CStaticBox*)GetComponent("stbRewardSelectText");
	m_pstbRewardSelectAmountText = (gui::CStaticBox*)GetComponent("stbRewardSelectAmountText");

	m_ppnlConversationBack = (gui::CPanel*)GetComponent( "pnlConvBack" );
	m_phbxConversation = (gui::CHtmlBox*)GetComponent( "hbxConv" );

	m_pstbAimTargetTitle = (gui::CStaticBox*)GetComponent( "stbTargetInfoTitle" );
	m_pstbAimTargetValue = (gui::CStaticBox*)GetComponent( "stbTargetInfoValue" );
	m_pstbAimTitle = (gui::CStaticBox*)GetComponent( "stbAimTitle" );
	
	m_phbxAim = (gui::CHtmlBox*)GetComponent( "hbxAim" );
	m_ppnlAimBack = (gui::CPanel*)GetComponent( "pnlAimBack" );
	m_pstbProgressTitle = (gui::CStaticBox*)GetComponent( "stbInfoTitle" );
	m_pstbTime = (gui::CStaticBox*)GetComponent( "stbLimitTime" );
	m_pstbTimeTitle = (gui::CStaticBox*)GetComponent( "stbLimitTimeTitle" );
	m_ppnlTimeBack = (gui::CPanel*)GetComponent( "pnlTimeBack" );
	
	m_ppnlProgressBack = (gui::CPanel*)GetComponent( "pnlInfoBack" );
	
	RwChar buf[GUI_TEXT_BUFFER_SIZE];
	for( RwInt32 i = 0 ; i < QUEST_MAX_GOAL_COUNT ; ++i )
	{
		sprintf_s( buf, GUI_TEXT_BUFFER_SIZE, "stbGoalInfo%d", i + 1 );
		m_pstbProgressInfo[i] = (gui::CStaticBox*)GetComponent( buf );
		sprintf_s( buf, GUI_TEXT_BUFFER_SIZE, "stbGoalResult%d", i + 1 );
		m_pstbProgressResult[i] = (gui::CStaticBox*)GetComponent( buf );
	}
	
	m_pstbStandardRewardTitle = (gui::CStaticBox*)GetComponent( "stbBasicRewardText" );
	m_pstbSelectRewardTitle = (gui::CStaticBox*)GetComponent( "stbSelectRewardText" );

	m_pbtnQuestClose = (gui::CButton*)GetComponent( "btnCancel" );
	m_pbtnQuestGiveUp = (gui::CButton*)GetComponent( "btnGiveUp" );
	m_ptbtTab = (gui::CTabButton*)GetComponent( "tabQuest" );

	m_pBtnHelp = (gui::CButton*)GetComponent("btnHelp");

	m_slotClickedBtnHelp = m_pBtnHelp->SigClicked().Connect( this, &CQuestProgressGui::OnClickedBtnHelp );
	m_slotClickExitButton = m_pbtnClose->SigClicked().Connect( this, &CQuestProgressGui::OnClickExitButton );
	m_slotClickCloseButton = m_pbtnQuestClose->SigClicked().Connect( this, &CQuestProgressGui::OnClickExitButton );
	m_slotClickGiveUpButton = m_pbtnQuestGiveUp->SigClicked().Connect( this, &CQuestProgressGui::OnClickGiveUpButton );
	m_slotSelectChangeTabButton = m_ptbtTab->SigSelectChanged().Connect( this, &CQuestProgressGui::OnSelectChangeTabButton );
	m_slotWheelMoveConv = m_phbxConversation->SigMouseWheel().Connect( this, &CQuestProgressGui::OnWheelMoveConv );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CQuestProgressGui::OnCaptureMouseDown );
	
	m_slotMouseMove = m_pThis->SigMouseMove().Connect( this, &CQuestProgressGui::OnMouseMove );
	m_slotMouseOut = m_pThis->SigMouseLeave().Connect( this, &CQuestProgressGui::OnMouseOut );
	m_slotPaint = m_pbtnClose->SigPaint().Connect( this, &CQuestProgressGui::OnPaint );
	m_slotMove = m_pThis->SigMove().Connect( this, &CQuestProgressGui::OnMove );

	m_phbxConversation->SetLineSpace( 7 );
	m_phbxAim->SetLineSpace( 7 );
	
	// 보상영역 설정.
	SetRewardRectHardcode();

	SetBasicUISetting();

	Show( false );

	m_pQuestGui = pQuestGui;

	NTL_RETURN( TRUE );
}

VOID CQuestProgressGui::Destroy(VOID)
{
	DeleteAllRewardSlot();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CQuestProgressGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	if( bOpen )
	{

	}
	else
	{
	
	}

	return 1;
}

VOID CQuestProgressGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventShowQuestWindow_Nfy )
	{
		SNtlEventShowQuestWindow_Nfy* pData = reinterpret_cast<SNtlEventShowQuestWindow_Nfy*>( msg.pData );	
		m_TSKey = pData->sTSKey;

		SetSenderRecieverData( pData->uiEventType, pData->uiEventID, pData->bIsEventStarter );
		SetQuestData( pData );
		UpdateUIData( pData->eEvtInfoType, &pData->uEvtInfoData, pData->uiTimeLimit );

		if( pData->uiTimeLimit != 0xFFFFFFFF )
			m_bShowTimeUI = TRUE;			
		else
			m_bShowTimeUI = FALSE;

		CalcResultWidth();

		m_ptbtTab->SelectTab( (RwInt32)AIM_TAB );
		SelectTabPage( AIM_TAB );

		GetDialogManager()->OpenDialog( DIALOG_QUESTPROGRESS );
	}
	else if( msg.Id == g_EventUpdateQuestProgressInfo_Nfy )
	{
		SNtlEventUpdateQuestProgressInfo_Nfy* pData = reinterpret_cast<SNtlEventUpdateQuestProgressInfo_Nfy*>( msg.pData );

		if( IsShow() && pData->sTSKey.tID == m_TSKey.tID )
		{
			UpdateUIData( pData->eEvtInfoType, &pData->uEvtInfoData, pData->uiTimeLimit );
			CalcResultWidth();
		}
	}
	else if( msg.Id == g_EventUpdateQuestState_Nfy )
	{
		SNtlEventUpdateQuestState_Nfy* pData = reinterpret_cast<SNtlEventUpdateQuestState_Nfy*>( msg.pData );

		if( IsShow() && pData->sTSKey.tID == m_TSKey.tID )
		{
			SetSenderRecieverData( pData->uiEventType, pData->uiEventID, pData->bIsEventStarter );
		}
	}

	else if( msg.Id == g_EventQuestDirectForward )
	{
		SNtlEventQuestDirect_Forward* pData = reinterpret_cast<SNtlEventQuestDirect_Forward*>( msg.pData );

		if( pData->eTDType == ETD_QuestReward )
		{
			if( pData->sReward.sTSKey.tID == m_TSKey.tID )
				GetDialogManager()->CloseDialog( DIALOG_QUESTPROGRESS );
		}
	}
	else if( msg.Id == g_EventUnregQuest_Nfy )
	{
		SNtlEventUnregQuest_Nfy* pDeleteData = reinterpret_cast<SNtlEventUnregQuest_Nfy*>( msg.pData );

		if( pDeleteData->sTSKey.tID == m_TSKey.tID )
			GetDialogManager()->CloseDialog( DIALOG_QUESTPROGRESS );		
	}
	else if( msg.Id == g_EventRegQuest_Nfy )
	{
		// 창이 열려있는 상태에서 같은 아이디면 창을 닫고 다시 띄운다. 
		if( !IsShow() )
			return;

		SNtlEventRegQuest_Nfy* pData = reinterpret_cast<SNtlEventRegQuest_Nfy*>( msg.pData );

		if( m_TSKey.tID == pData->sTSKey.tID )
			GetDialogManager()->CloseDialog( DIALOG_QUESTPROGRESS );

		CNtlSLEventGenerator::ShowQuestWindow( pData->sTSKey );	
	}	
}

VOID CQuestProgressGui::SetBasicUISetting(VOID)
{
	m_ptbtTab->SelectTab( (RwInt32)CONV_TAB );
	SelectTabPage( CONV_TAB );
	
	// UI 세팅
	m_pstbTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_PROGRESS_WINDOW_TITLE" ) );
	m_pstbTimeTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_LIMIT_TIME" ) );
	m_pstbProgressTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_PROGRESS_INFO" ) );
	m_pstbAimTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_TEXT_AIM" ) );

	m_pbtnQuestGiveUp->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_GIVEUP_BTN" ) );
	m_pbtnQuestGiveUp->SetTextDownColor( NTL_BUTTON_DOWN_COLOR );
	m_pbtnQuestGiveUp->SetTextFocusColor( NTL_BUTTON_FOCUS_COLOR );
	m_pbtnQuestGiveUp->SetTextUpColor( NTL_BUTTON_UP_COLOR );
	m_pbtnQuestGiveUp->SetTextDownCoordDiff( NTL_BUTTON_CLICK_DIFFX, NTL_BUTTON_CLICK_DIFFY );
	m_pbtnQuestGiveUp->ApplyText();

	m_pbtnQuestClose->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_CLOSE_BTN" ) );
	m_pbtnQuestClose->SetTextDownColor( NTL_BUTTON_DOWN_COLOR );
	m_pbtnQuestClose->SetTextFocusColor( NTL_BUTTON_FOCUS_COLOR );
	m_pbtnQuestClose->SetTextUpColor( NTL_BUTTON_UP_COLOR );
	m_pbtnQuestClose->SetTextDownCoordDiff( NTL_BUTTON_CLICK_DIFFX, NTL_BUTTON_CLICK_DIFFY );
	m_pbtnQuestClose->ApplyText();

	m_ptbtTab->AddTab( std::wstring( GetDisplayStringManager()->GetString( "DST_QUEST_TEXT_CONVERSATION" ) ) );
	m_ptbtTab->AddTab( std::wstring( GetDisplayStringManager()->GetString( "DST_QUEST_TEXT_AIM" ) ) );
	m_ptbtTab->SelectTab( 0 );	
}

VOID CQuestProgressGui::SetQuestData( SNtlEventShowQuestWindow_Nfy* pData )
{
	// 제목
	SetQuestTitle( pData->uiQuestTitle, pData->uiQuestSort, pData->eGradeType );

	CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
	sQUEST_TEXT_DATA_TBLDAT* pQuestText;

	// 내용
	pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pData->uiQuestContents ) );
	if( pQuestText )
		m_phbxConversation->SetHtmlFromMemory( pQuestText->wstrText.c_str(), pQuestText->wstrText.size() );
	else
		m_phbxConversation->Clear();

	// 목표
	pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pData->uiQuestGoal ) );
	if( pQuestText )
		m_phbxAim->SetHtmlFromMemory( pQuestText->wstrText.c_str(), pQuestText->wstrText.size() );
	else
		m_phbxAim->Clear();

	ClearProgressInfo();

	for( RwInt32 i = 0 ; i < QUEST_MAX_REWARD_SLOT ; ++i )
	{
		SetRewardSlot( i, &pData->sSelectReward[i]);
		SetRewardSlot( i + QUEST_MAX_REWARD_SLOT, &pData->sDefaultReward[i]);
	}

	m_pstbRewardGetExpAmountText->SetText(pData->uiRewardExp);
	m_pstbRewardGetZennyAmountText->SetText(pData->uiRewardZeni);

	// TMQ Quest는 포기할 수 없음
	if( Logic_IsNormalQuest( pData->sTSKey.tID ) )
		m_pbtnQuestGiveUp->Show( true );
	else
		m_pbtnQuestGiveUp->Show( false );

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

VOID CQuestProgressGui::UpdateUIData( eSTOC_EVT_DATA_TYPE eEvtInfoType, uSTOC_EVT_DATA* puEvtInfoData, RwUInt32 uiTimeLimit )
{
	ClearProgressInfo();

	if( eEvtInfoType == eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT )
	{
		CMobTable* pMobTable = API_GetTableContainer()->GetMobTable();
		CTextTable* pMobText = API_GetTableContainer()->GetTextAllTable()->GetMobTbl();

		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_MOB_KILL ; ++i )
		{
			RwUInt32 nTblIdx = pMobTable->FindTblidxByGroup( puEvtInfoData->sMobKillCnt[i].uiMobIdx );
			sMOB_TBLDAT* pMobData = reinterpret_cast<sMOB_TBLDAT*>( pMobTable->FindData( nTblIdx ) );
			if( !pMobData )
				continue;

			UpdateResultItem( pMobText->GetText( pMobData->Name ).c_str(), puEvtInfoData->sMobKillCnt[i].nCurMobCnt, puEvtInfoData->sMobKillCnt[i].nMobCnt, i );			
		}
	}
	else if( eEvtInfoType == eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT )
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_MOB_KILL_ITEM ; ++i )
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pItemTable->FindData( puEvtInfoData->sMobKillItemCnt[i].uiMobLIIdx ) );
			
			if( !pItemData )
				continue;
				
			UpdateResultItem( pQuestItemText->GetText( pItemData->ItemName ).c_str(), puEvtInfoData->sMobKillItemCnt[i].nCurMobLICnt, puEvtInfoData->sMobKillItemCnt[i].nMobLICnt, i );			
		}
	}
	else if( eEvtInfoType == eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM )
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_DELIVERY_ITEM ; ++i )
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pItemTable->FindData( puEvtInfoData->sDeliveryItemCnt[i].uiItemIdx ) );
			
			if( !pItemData )
				continue;

			UpdateResultItem( pQuestItemText->GetText( pItemData->ItemName ).c_str(), puEvtInfoData->sDeliveryItemCnt[i].nCurItemCnt, puEvtInfoData->sDeliveryItemCnt[i].nItemCnt, i );			
		}
	}
	else if( eEvtInfoType == eSTOC_EVT_DATA_TYPE_OBJECT_ITEM )
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_OBJECT_ITEM ; ++i )
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pItemTable->FindData( puEvtInfoData->sObjectItemCnt[i].uiItemIdx ) );

			if( !pItemData )
				continue;

			UpdateResultItem( pQuestItemText->GetText( pItemData->ItemName ).c_str(), puEvtInfoData->sObjectItemCnt[i].nCurItemCnt, puEvtInfoData->sObjectItemCnt[i].nItemCnt, i );			
		}
	}
	else if( eEvtInfoType == eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT )
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_PUBLIC_MOB_ITEM ; ++i )
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pItemTable->FindData( puEvtInfoData->sPublicMobItemCnt[i].uiItemIdx ) );
			
			if( !pItemData )
				continue;

			UpdateResultItem( pQuestItemText->GetText( pItemData->ItemName ).c_str(), puEvtInfoData->sPublicMobItemCnt[i].nCurItemCnt, puEvtInfoData->sPublicMobItemCnt[i].nItemCnt, i );			
		}
	}
	else if( eEvtInfoType == eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT )
	{
		CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();

		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_CUSTOM_EVT_CNT ; ++i )
		{
			sQUEST_TEXT_DATA_TBLDAT* pQuestTextData = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( puEvtInfoData->sCustomEvtCnt[i].uiQTextTblIdx ) );

			if( !pQuestTextData )
				continue;

			std::wstring strQuestText = gui::GetHtmlFromMemoryString( pQuestTextData->wstrText.c_str(), pQuestTextData->wstrText.size() );
			UpdateResultItem( strQuestText.c_str(), puEvtInfoData->sCustomEvtCnt[i].nCurCnt, puEvtInfoData->sCustomEvtCnt[i].nMaxCnt, i );
		}
	}
	else if( eEvtInfoType == eSTOC_EVT_DATA_TYPE_VISIT )
	{
		CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();

		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_VISIT_EVT ; ++i )
		{
			sQUEST_TEXT_DATA_TBLDAT* pQuestTextData = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( puEvtInfoData->sVisitEvt[i].uiIndicatorQText ) );

			if( !pQuestTextData )
				continue;

			std::wstring wstrText = gui::GetHtmlFromMemoryString( pQuestTextData->wstrText.c_str(), pQuestTextData->wstrText.size() );
			UpdateResultItem( wstrText.c_str(), puEvtInfoData->sVisitEvt[i].bCompleted, i );
		}
	}

	// 시간.
	if( uiTimeLimit != 0xffffffff )
	{
		RwUInt32 uiTime;
		RwUInt32 uiMinute;
		RwUInt32 uiSecond;

		if( uiTimeLimit <= 0 )
		{
			uiMinute = 0;
			uiSecond = 0;
		}
		else
		{
			uiTime = uiTimeLimit / 1000;
			uiMinute = uiTime / 60;
			uiSecond = uiTime % 60;
		}

		m_pstbTime->Format( "%02d : %02d", uiMinute, uiSecond );		
	}
	else
	{
		m_pstbTime->Clear();
	}
}

VOID CQuestProgressGui::SetSenderRecieverData( RwUInt32 uiEventType, RwUInt32 uiEventID, RwBool bStarter )
{
	CRectangle rectAim;
	if( uiEventType == eEVENT_GEN_TYPE_CLICK_NPC )
	{
		m_pstbAimTargetTitle->Show( true );
		rectAim.SetRectWH( QUEST_PROGRESS_AIM_HTML_X, QUEST_PROGRESS_AIM_HTML_Y_WITH_TARGETINFO, QUEST_PROGRESS_AIM_HTML_WIDTH, QUEST_PROGRESS_AIM_HTML_HEIGHT_WITH_TARGETINFO );

		if( bStarter )
			m_pstbAimTargetTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_SENDER" ) );
		else
			m_pstbAimTargetTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUEST_RECIEVER" ) );

		CNPCTable* pNpcTable = API_GetTableContainer()->GetNpcTable();
		CTextTable* pNpcTextTable = API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();
		sNPC_TBLDAT* pNpcData = reinterpret_cast<sNPC_TBLDAT*>( pNpcTable->FindData( uiEventID ) );

		m_pstbAimTargetValue->SetText( pNpcTextTable->GetText( pNpcData->Name ).c_str() );
	}
	else
	{
		m_pstbAimTargetTitle->Show( false );
		rectAim.SetRectWH( QUEST_PROGRESS_AIM_HTML_X, QUEST_PROGRESS_AIM_HTML_Y, QUEST_PROGRESS_AIM_HTML_WIDTH, QUEST_PROGRESS_AIM_HTML_HEIGHT );
	}
	m_phbxAim->SetPosition( rectAim );
}

VOID CQuestProgressGui::UpdateResultItem( const WCHAR* pInfo, RwInt32 nCurVal, RwInt32 nTargetVal, RwInt32 i )
{
	m_pstbProgressInfo[i]->SetText( pInfo );
	m_pstbProgressResult[i]->Format( L" : %d / %d", nCurVal, nTargetVal );									 			

	RwInt32 nFitWidth = m_pstbProgressResult[i]->GetFitWidthToString();
	m_pstbProgressResult[i]->SetWidth( nFitWidth );
}

VOID CQuestProgressGui::UpdateResultItem( const WCHAR* pInfo, RwBool bComplete, RwInt32 i )
{
	if( i >= QUEST_MAX_GOAL_COUNT )
		return;

	m_pstbProgressInfo[i]->SetText( pInfo );
	m_pstbProgressResult[i]->Clear();

	RwInt32 nFitWidth = m_pstbProgressResult[i]->GetFitWidthToString();
	m_pstbProgressResult[i]->SetWidth( nFitWidth );
}

VOID CQuestProgressGui::CalcResultWidth(VOID)
{
	if( m_bShowTimeUI )
	{
		for( RwInt32 i = 0 ; i < QUEST_MAX_GOAL_COUNT ; ++i )
		{
			RwInt32 nWidth = m_pstbProgressInfo[i]->GetFitWidthToString();
			RwInt32 nResultWidth = m_pstbProgressResult[i]->GetWidth();

			if( nWidth > QUEST_PROGRESS_GOALINFO_LIMITWIDTH_TIME - nResultWidth )
			{
				nWidth = QUEST_PROGRESS_INFO_WIDTH - nResultWidth;
				m_pstbProgressInfo[i]->SetToolTip( m_pstbProgressInfo[i]->GetText() );
			}
			else
			{
				m_pstbProgressInfo[i]->DeleteToolTip();
			}

			m_pstbProgressInfo[i]->SetWidth( nWidth );
			m_pstbProgressResult[i]->SetPosition( m_pstbProgressInfo[i]->GetPosition().left + nWidth, m_pstbProgressResult[i]->GetPosition().top );			
		}

		//if( m_ptbtTab->GetSelectedButtonIndex() == AIM_TAB )
		//{
		//	m_pstbTimeTitle->Show( true );
		//	m_pstbTime->Show( true );
		//	m_ppnlTimeBack->Show( true );		
		//}
		//else
		//{
		//	m_pstbTimeTitle->Show( false );
		//	m_pstbTime->Show( false );
		//	m_ppnlTimeBack->Show( false );
		//}
	}
	else
	{
		for( RwInt32 i = 0 ; i < QUEST_MAX_GOAL_COUNT ; ++i )
		{
			RwInt32 nWidth = m_pstbProgressInfo[i]->GetFitWidthToString();
			RwInt32 nResultWidth = m_pstbProgressResult[i]->GetWidth();
			
			if( nWidth > QUEST_PROGRESS_INFO_WIDTH - nResultWidth )
			{
				nWidth = QUEST_PROGRESS_INFO_WIDTH - nResultWidth;
				m_pstbProgressInfo[i]->SetToolTip( m_pstbProgressInfo[i]->GetText() );
			}
			else
			{
				m_pstbProgressInfo[i]->DeleteToolTip();
			}

			m_pstbProgressInfo[i]->SetWidth( nWidth );
			m_pstbProgressResult[i]->SetPosition( m_pstbProgressInfo[i]->GetPosition().left + nWidth, m_pstbProgressResult[i]->GetPosition().top );
		}

		//m_pstbTimeTitle->Show( false );
		//m_pstbTime->Show( false );
		//m_ppnlTimeBack->Show( false );
	}
}

VOID CQuestProgressGui::SelectTabPage( TAB eTab )
{
	if( eTab == CONV_TAB )
	{
		m_phbxConversation->Show( true );

		m_phbxAim->Show( false );

		m_pstbProgressTitle->Show( false );
			
		for( RwInt32 i = 0 ; i < QUEST_MAX_GOAL_COUNT ; ++i )
		{
			m_pstbProgressInfo[i]->Show( false );
			m_pstbProgressResult[i]->Show( false );
		}	

		m_ppnlConversationBack->Show( true );
		m_ppnlAimBack->Show( false );
		m_ppnlProgressBack->Show( false );		

		m_pstbTimeTitle->Show( false );
		m_pstbTime->Show( false );
		m_ppnlTimeBack->Show( false );
	}
	else
	{
		m_phbxConversation->Show( false );

		m_phbxAim->Show( true );

		m_pstbProgressTitle->Show( true );
				
		for( RwInt32 i = 0 ; i < QUEST_MAX_GOAL_COUNT ; ++i )
		{
			m_pstbProgressInfo[i]->Show( true );
			m_pstbProgressResult[i]->Show( true );
		}		

		m_ppnlConversationBack->Show( false );
		m_ppnlAimBack->Show( true );
		m_ppnlProgressBack->Show( true );		

		if( m_bShowTimeUI )
		{
			m_pstbTimeTitle->Show( true );
			m_pstbTime->Show( true );
			m_ppnlTimeBack->Show( true );
		}
		else
		{
			m_pstbTimeTitle->Show( false );
			m_pstbTime->Show( false );
			m_ppnlTimeBack->Show( false );
		}
	}
}

VOID CQuestProgressGui::ClearProgressInfo(VOID)
{
	for( RwInt32 i = 0 ; i < QUEST_MAX_GOAL_COUNT ; ++i )
	{
		m_pstbProgressInfo[i]->Clear();
		m_pstbProgressInfo[i]->DeleteToolTip();
		m_pstbProgressResult[i]->Clear();
		m_pstbProgressResult[i]->DeleteToolTip();
	}
}

// CallBack Message
VOID CQuestProgressGui::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	CQuestCommonGui::OnMouseMove( nKey, nXPos, nYPos );
}

VOID CQuestProgressGui::OnMouseOut( gui::CComponent* pComponent )
{
	CQuestCommonGui::OnMouseOut( pComponent );
}

VOID CQuestProgressGui::OnPaint(VOID)
{
	CQuestCommonGui::OnPaint();
}

VOID CQuestProgressGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	CQuestCommonGui::OnMove( nX, nY );
}

VOID CQuestProgressGui::OnClickExitButton( gui::CComponent* pComponent )
{
	GetDialogManager()->CloseDialog( DIALOG_QUESTPROGRESS );
}

VOID CQuestProgressGui::OnClickGiveUpButton( gui::CComponent* pComponent )
{
	sMsgBoxData data;
	data.sTSKey = m_TSKey;
	data.uiQuestTitle = m_uiTitleIdx;
	GetAlarmManager()->FormattedAlarmMessage( "DST_QUEST_GIVEUP_MESSAGE", FALSE, &data, m_pstbQuestTitle->GetText().c_str() );
	GetDialogManager()->CloseDialog( DIALOG_QUESTPROGRESS );	
}

VOID CQuestProgressGui::OnSelectChangeTabButton( INT nCurIndex, INT nPrevIndex )
{
	SelectTabPage( (TAB)nCurIndex );
}

VOID CQuestProgressGui::OnWheelMoveConv( RwInt32 nFlag, RwInt16 sDelta, CPos& pos )
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

VOID CQuestProgressGui::OnCaptureMouseDown( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_QUESTPROGRESS, key.m_fX, key.m_fY );
}

VOID CQuestProgressGui::OnClickedBtnHelp( gui::CComponent* pComponent )
{
	CDboEventGenerator::OpenHelpContent( DIALOG_QUESTPROGRESS );
}