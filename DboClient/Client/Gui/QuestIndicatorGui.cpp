#include "precomp_dboclient.h"
#include "QuestIndicatorGui.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// gui
#include "GuiUtil.h"

// shared
#include "QuestTextDataTable.h"
#include "MobTable.h"
#include "QuestItemTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// sound
#include "GUISoundDefine.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSobQuestItem.h"
#include "NtlSobQuestItemAttr.h"
#include "NtlSobIcon.h"
#include "NtlSLApi.h"
#include "NtlWorldConcept.h"

#include "NtlStorageManager.h"
#include "NtlStorageGroupQuest.h"

// Dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "InfoWndManager.h"
#include "QuestGui.h"
#include "QuestListGui.h"
#include "SideDialogManager.h"
#include "DialogManager.h"


#define QUEST_INDICATOR_Y					300

#define QUEST_INDICATOR_ITEM_MARGIN			5

#define QUEST_INDICATOR_FRAME_WIDTH			258//155

#define QUEST_INDICATOR_QUESTSTATE_WIDTH	15
#define QUEST_INDICATOR_QUESTSTATE_HEIGHT	15
#define QUEST_INDICATOR_QUESTTITLE_WIDTH	QUEST_INDICATOR_FRAME_WIDTH
#define QUEST_INDICATOR_QUESTTITLE_TEXTPOSX	0//( QUEST_INDICATOR_QUESTSTATE_WIDTH )
#define QUEST_INDICATOR_QUESTSTATE_POSY		( QUEST_INDICATOR_QUESTTITLE_HEIGHT - QUEST_INDICATOR_QUESTSTATE_HEIGHT ) / 2 + 5

#define QUEST_INDICATOR_QUESTINFO_POSX		80//20
#define QUEST_INDICATOR_QUESTINFO_WIDTH		242
#define QUEST_INDICATOR_QUESTINFO_HEIGHT	16

#define QUEST_INDICATOR_QUESTRESULT_WIDTH	33
#define QUEST_INDICATOR_QUESTRESULT_HEIGHT	QUEST_INDICATOR_QUESTINFO_HEIGHT

#define QUEST_INDICATOR_QUESTTIME_POSX		20
#define QUEST_INDICATOR_QUESTTIME_WIDTH		( QUEST_INDICATOR_QUESTINFO_WIDTH - QUEST_INDICATOR_QUESTTIME_POSX )
#define QUEST_INDICATOR_QUESTTIME_HEIGHT	QUEST_INDICATOR_QUESTINFO_HEIGHT

#define QUEST_INDICATOR_COLOR_GRAY			RGB( 168, 168, 168 )
#define QUEST_INDICATOR_COLOR_WHITE			RGB( 254, 254, 254 )
#define QUEST_INDICATOR_COLOR_RED			RGB( 254, 100, 100 )

#define QUEST_INDICATOR_SHADOW_COLOR		RGB( 0, 0, 0 )
#define QUEST_INDICATOR_SHADOW_OFFSET		1

#define QUEST_INDICATOR_QUESTTITLE_FONTSIZE	95//90

VOID stINDICATOR_ITEM::OnClickTitle( gui::CComponent* pComponent )
{
	CNtlSLEventGenerator::ShowQuestWindow( m_TSKey );	
}

VOID stINDICATOR_ITEM::OnMouseEnterTitle( gui::CComponent* pComponent )
{
	stINFOWND_QUESTINDICATOR stData;
	stData.uiQuestTitle = m_uiQuestTitle;
	stData.uiQuestGoal = m_uiQuestGoal;

	CRectangle rect = m_pTitle->GetScreenRect();
	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_QUEST_INDICATOR, rect.left, rect.top, &stData, SDIALOG_QUEST_INDICATOR );
}

VOID stINDICATOR_ITEM::OnMouseLeaveTitle( gui::CComponent* pComponent )
{
	if( GetInfoWndManager()->GetRequestGui() == SDIALOG_QUEST_INDICATOR )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

void stINDICATOR_ITEM::OnClickExpend(gui::CComponent * pComponent)
{
	// to do: show/hide quest info
	if (m_bQuestInfoShown)
	{
		m_bQuestInfoShown = false;


	}
	else
	{
		m_bQuestInfoShown = true;
	}
}

CQuestIndicatorGui::CQuestIndicatorGui(VOID)
{
	Init();
}

CQuestIndicatorGui::CQuestIndicatorGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();	
}

CQuestIndicatorGui::~CQuestIndicatorGui(VOID)
{

}

VOID CQuestIndicatorGui::Init(VOID)
{
	m_nTotalItemHeight = 0;

	m_pQuestGui = NULL;

	m_nScrollPos = 0;
}

RwBool CQuestIndicatorGui::Create( CQuestGui* pQuestGui )
{
	NTL_FUNCTION( "CQuestIndicatorGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\QuestIndicator.srf", "gui\\QuestIndicator.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	m_pbtnMinimize = (gui::CButton*)GetComponent( "btnMinimize" );
	m_pbtnMinimize->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTINDICATOR_TRANSFORM_MINI"));
	m_slotClickMinimize = m_pbtnMinimize->SigClicked().Connect(this, &CQuestIndicatorGui::OnClickMinimize);

	m_pbtnAllExtendOn = (gui::CButton*)GetComponent("btnAllExtendOn");
	m_pbtnAllExtendOn->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTINDICATOR_ALLEXTEND_ON"));
	m_slotClickAllExtendOn = m_pbtnAllExtendOn->SigClicked().Connect(this, &CQuestIndicatorGui::OnClickAllExtendOn);

	m_pbtnAllExtendOff = (gui::CButton*)GetComponent("btnAllExtendOff");
	m_pbtnAllExtendOff->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTINDICATOR_ALLEXTEND_OFF"));
	m_pbtnAllExtendOff->Show(false);
	m_slotClickAllExtendOff = m_pbtnAllExtendOff->SigClicked().Connect(this, &CQuestIndicatorGui::OnClickAllExtendOff);

	m_pstbTitle = (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_SIDEDLG_TITLE_QUESTINDICATOR"));

	m_ppnClippingArea = (gui::CPanel*)GetComponent("pnClippingArea");
	m_ppnClippingArea->Lower();
	m_ppnIndicatorList = (gui::CPanel*)GetComponent("pnIndicatorList");

	m_pscrBar = (gui::CScrollBar*)GetComponent("scrBar");
	m_pscrBar->Show(false);
	m_slotScrollChanged = m_pscrBar->SigValueChanged().Connect(this, &CQuestIndicatorGui::OnScrollChanged);
	m_slotScrollSliderMoved = m_pscrBar->SigSliderMoved().Connect(this, &CQuestIndicatorGui::OnScrollChanged);

	m_ppnTitlePanel = (gui::CPanel*)GetComponent("pnTitlePanel");

	m_pbtnQuestListOpen = (gui::CButton*)GetComponent("btnQuestListOpen");
	m_pbtnQuestListOpen->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTINDICATOR_QUESTLIST_BTN"));
	m_slotClickQuestListOpen = m_pbtnQuestListOpen->SigClicked().Connect(this, &CQuestIndicatorGui::OnClickQuestListOpen);

	
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();
	CRectangle rect;

	for(int i = 0 ; i < eMAX_CAN_PROGRESS_QUEST_NUM; i++)
	{
		rect.SetRectWH( 0, 0, QUEST_INDICATOR_FRAME_WIDTH, 0);
		m_IndicatorItem[i].m_pFrame = NTL_NEW gui::CDialog( rect, m_ppnIndicatorList, pSurfaceManager );

		m_IndicatorItem[i].m_Background.SetType(CWindowby3::WT_HORIZONTAL);
		m_IndicatorItem[i].m_Background.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestIndicator.srf", "srfItemBackTop"));
		m_IndicatorItem[i].m_Background.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestIndicator.srf", "srfItemBackCenter"));
		m_IndicatorItem[i].m_Background.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestIndicator.srf", "srfItemBackBottom"));
		m_IndicatorItem[i].m_Background.SetAlpha(50);

		rect.SetRect(30, 6, 200, 29);
		m_IndicatorItem[i].m_pTitle = NTL_NEW gui::CButton( rect, m_IndicatorItem[i].m_pFrame, pSurfaceManager );
		m_IndicatorItem[i].m_pTitle->SetTextFont( "detail", QUEST_INDICATOR_QUESTTITLE_FONTSIZE, 0 );
		m_IndicatorItem[i].m_pTitle->SetTextDownCoordDiff( 0, 1 );
		m_IndicatorItem[i].m_pTitle->SetTextStyle( COMP_TEXT_LEFT );
		m_IndicatorItem[i].m_pTitle->SetTextCoord( QUEST_INDICATOR_QUESTTITLE_TEXTPOSX, 0 );
		m_IndicatorItem[i].m_pTitle->AddSurfaceFocus( pSurfaceManager->GetSurface( "QuestIndicator.srf", "srfbtnTitleFocus" ) );
		m_IndicatorItem[i].m_pTitle->SetTextEffectMode(TE_OUTLINE);
		m_IndicatorItem[i].m_pTitle->SetTextUpColor( SIDE_DIALOG_FONT_YELLOW_COLOR );
		m_IndicatorItem[i].m_pTitle->SetTextDownColor( SIDE_DIALOG_FONT_YELLOW_COLOR );
		m_IndicatorItem[i].m_pTitle->SetTextFocusColor( SIDE_DIALOG_FONT_YELLOW_COLOR );
		m_IndicatorItem[i].m_pTitle->ApplyText();


		rect.SetRect(239, 1, 257, 19);
		m_IndicatorItem[i].m_pbtnExpend = NTL_NEW gui::CButton(rect, m_IndicatorItem[i].m_pFrame, pSurfaceManager);
		m_IndicatorItem[i].m_pbtnExpend->AddSurfaceFocus(pSurfaceManager->GetSurface("QuestIndicator.srf", "srfExpendBtnFoc"));
		m_IndicatorItem[i].m_pbtnExpend->AddSurfaceUp(pSurfaceManager->GetSurface("QuestIndicator.srf", "srfExpendBtnUp"));
		m_IndicatorItem[i].m_pbtnExpend->AddSurfaceDown(pSurfaceManager->GetSurface("QuestIndicator.srf", "srfExpendBtnDown"));
		m_IndicatorItem[i].m_pbtnExpend->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTINDICATOR_EXTEND"));

		rect.SetRectWH(10, QUEST_INDICATOR_QUESTSTATE_POSY, QUEST_INDICATOR_QUESTSTATE_WIDTH, QUEST_INDICATOR_QUESTSTATE_HEIGHT );
		m_IndicatorItem[i].m_pState = NTL_NEW gui::CPanel( rect, m_IndicatorItem[i].m_pFrame, GetNtlGuiManager()->GetSurfaceManager() );

		rect.SetRectWH( QUEST_INDICATOR_QUESTTIME_POSX, 0, QUEST_INDICATOR_QUESTTIME_WIDTH, QUEST_INDICATOR_QUESTTIME_HEIGHT );
		m_IndicatorItem[i].m_pTime = NTL_NEW gui::CStaticBox( rect, m_IndicatorItem[i].m_pFrame, pSurfaceManager, COMP_TEXT_LEFT );
		m_IndicatorItem[i].m_pTime->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
		m_IndicatorItem[i].m_pTime->Enable( false );
		
		for(int j = 0 ; j < QUEST_MAX_GOAL_COUNT; j++)
		{
			rect.SetRectWH( QUEST_INDICATOR_QUESTINFO_POSX, 0, QUEST_INDICATOR_QUESTINFO_WIDTH, QUEST_INDICATOR_QUESTINFO_HEIGHT );
			m_IndicatorItem[i].m_pGoalInfo[j] = NTL_NEW gui::CStaticBox( rect, m_IndicatorItem[i].m_pFrame, pSurfaceManager, COMP_TEXT_LEFT );
			m_IndicatorItem[i].m_pGoalInfo[j]->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
			m_IndicatorItem[i].m_pGoalInfo[j]->Enable( false );

			rect.SetRectWH( 0, 0, QUEST_INDICATOR_QUESTRESULT_WIDTH, QUEST_INDICATOR_QUESTRESULT_HEIGHT );
			m_IndicatorItem[i].m_pGoalResult[j] = NTL_NEW gui::CStaticBox( rect, m_IndicatorItem[i].m_pFrame, pSurfaceManager, COMP_TEXT_LEFT );			
			m_IndicatorItem[i].m_pGoalResult[j]->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
			m_IndicatorItem[i].m_pGoalResult[j]->Enable( false );
		}

		m_IndicatorItem[i].m_pFrame->Show( false );
		
		m_IndicatorItem[i].m_nFrameHeight = 0;		

		m_IndicatorItem[i].m_slotClickTitle = m_IndicatorItem[i].m_pTitle->SigClicked().Connect( &m_IndicatorItem[i], &stINDICATOR_ITEM::OnClickTitle );
		m_IndicatorItem[i].m_slotMouseEnter = m_IndicatorItem[i].m_pTitle->SigMouseEnter().Connect( &m_IndicatorItem[i], &stINDICATOR_ITEM::OnMouseEnterTitle );
		m_IndicatorItem[i].m_slotMouseLeave = m_IndicatorItem[i].m_pTitle->SigMouseLeave().Connect( &m_IndicatorItem[i], &stINDICATOR_ITEM::OnMouseLeaveTitle );

		m_IndicatorItem[i].m_slotClickExpend = m_IndicatorItem[i].m_pbtnExpend->SigClicked().Connect(&m_IndicatorItem[i], &stINDICATOR_ITEM::OnClickExpend);
		m_IndicatorItem[i].m_bQuestInfoShown = true;
	}

	m_slotPaint = m_ppnIndicatorList->SigPaint().Connect(this, &CQuestIndicatorGui::OnPaint);
	m_slotMouseWheel = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CQuestIndicatorGui::OnMouseWheel);

	Show( false );

	m_pQuestGui = pQuestGui;

	NTL_RETURN( TRUE );
}

VOID CQuestIndicatorGui::Destroy(VOID)
{
	DeleteAllData();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CQuestIndicatorGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		CNtlWorldConcept* pWorldConcept = GetNtlWorldConcept();

		if( m_listData.size() > 0 )
		{
			if( pWorldConcept->IsActivePlayConcept( WORLD_PLAY_TIME_MACHINE ) )			
			{
				if( pWorldConcept->GetWorldConceptController( WORLD_PLAY_TIME_MACHINE )->GetState() == WORLD_STATE_EXIT )
					Show( true );
				else
					return -1;
			}
			else
			{
				Show( true );
			}			
		}
		else 
			return -1;		
	}
	else
	{
		Show( false );
	}	

	return 1;
}

RwBool CQuestIndicatorGui::IsIndicatedQuest( RwUInt32 nID )
{
	LIST_DATA::iterator it;

	for( it = m_listData.begin() ; it != m_listData.end() ; ++it )
	{
		if( (*it)->sTSKey.tID == nID )
			return TRUE;
	}

	return FALSE;
}

VOID CQuestIndicatorGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventUpdateQuestProgressInfo_Nfy )
	{
		SNtlEventUpdateQuestProgressInfo_Nfy* pData = reinterpret_cast<SNtlEventUpdateQuestProgressInfo_Nfy*>( msg.pData );

		UpdateData( pData );
		Refresh();
		
		// this hides the quest list whenever a quest progressed
		//int nSize = m_listData.size();
		//for (int i = 0; i < nSize; i++)
		//{
		//	SNtlEventShowIndicator_Nfy* pBackData = NULL;
		//	pBackData = m_listData.back();
		//	m_pQuestGui->GetQuestListGui()->CancelIndicator( pBackData->sTSKey, TRUE );	// 메시지를 띄우지 않기 위해 Auto플래그 임의 TRUE세팅.
		//}
	}
	else if( msg.Id == g_EventUpdateQuestState_Nfy )
	{
		SNtlEventUpdateQuestState_Nfy* pData = reinterpret_cast<SNtlEventUpdateQuestState_Nfy*>( msg.pData );

		UpdateState( pData->sTSKey, pData->uiQuestState );
	}
	else if( msg.Id == g_EventShowIndicator_Nfy )
	{
		SNtlEventShowIndicator_Nfy* pData = reinterpret_cast<SNtlEventShowIndicator_Nfy*>( msg.pData );

		// disabled by daneos
		//if( IsAvaliableHeight( pData ) )
		{
			SNtlEventShowIndicator_Nfy* pIndicatorData;
			FindData( pData->sTSKey.tID, &pIndicatorData );

			if( pIndicatorData )
				memcpy( pIndicatorData, pData, sizeof( SNtlEventShowIndicator_Nfy )	);
			else
				CreateData( pData );

			GetSideDialogManager()->OpenDialog( SDIALOG_QUEST_INDICATOR, TRUE );
			Refresh();
		}		
		//else
		//{
		//	m_pQuestGui->GetQuestListGui()->CancelIndicator( pData->sTSKey, pData->bAuto );
		//}
	}
	else if( msg.Id == g_EventHideIndicator )
	{
		SNtlEventHideIndicator* pData = reinterpret_cast<SNtlEventHideIndicator*>( msg.pData );

		DeleteData( pData->sTSKey.tID );
		Refresh();
		
		if( m_listData.size() == 0 )
			GetSideDialogManager()->CloseDialog( SDIALOG_QUEST_INDICATOR, TRUE );
	}
	else if( msg.Id == g_EventUnregQuest_Nfy )
	{
		SNtlEventUnregQuest_Nfy* pData = reinterpret_cast<SNtlEventUnregQuest_Nfy*>( msg.pData );

		DeleteData( pData->sTSKey.tID );
		Refresh();

		if( m_listData.size() == 0 )
			GetSideDialogManager()->CloseDialog( SDIALOG_QUEST_INDICATOR, TRUE );
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pData = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( pData->eWorldConcept == WORLD_PLAY_TIME_MACHINE )
		{
			if( pData->uiState == WORLD_STATE_EXIT )
				GetSideDialogManager()->OpenDialog( SDIALOG_QUEST_INDICATOR, FALSE );
		}
	}
	//else if( msg.Id == g_EventRBMatch )
	//{
	//	SNtlEventRBMatch* pData = reinterpret_cast<SNtlEventRBMatch*>( msg.pData );

	//	if( pData->type == SNtlEventRBMatch::MATCH_START )
	//		GetSideDialogManager()->CloseDialog( SDIALOG_QUEST_INDICATOR, FALSE );
	//}
	else if( msg.Id == g_EventTSRemovingTMQQuest_Nfy )
	{
		SNtlEventTSRemovingTMQQuest_Nfy* pData = reinterpret_cast<SNtlEventTSRemovingTMQQuest_Nfy*>( msg.pData );
		DeleteData( pData->tRmvTSId );
		Refresh();
	}
	else if (msg.Id == g_EventResize)
	{
		Refresh();
	}
}

VOID CQuestIndicatorGui::Refresh(VOID)
{
	RwInt32 i;
	LIST_DATA::iterator it;
	
	for( i = 0 ; i < eMAX_CAN_PROGRESS_QUEST_NUM; i++)
	{
		m_IndicatorItem[i].m_pFrame->Show( false );
	}

	for( i = 0, it = m_listData.begin() ; it != m_listData.end(); ++it, ++i )
	{
		SNtlEventShowIndicator_Nfy* pIndicatorData = (*it);
		DrawData( i, pIndicatorData );
	}

	CalculatePosition();
}

VOID CQuestIndicatorGui::DrawData( RwInt32 nIdx, SNtlEventShowIndicator_Nfy* pIndicatorData )
{
	RwInt32 j;
	RwUInt32 color;

	m_IndicatorItem[nIdx].m_nFrameHeight = 0;
	m_IndicatorItem[nIdx].m_pFrame->Show( true );

	m_IndicatorItem[nIdx].m_pTitle->GetTextComponent()->Clear();
	m_IndicatorItem[nIdx].m_pTime->Clear();

	for( j = 0 ; j < QUEST_MAX_GOAL_COUNT ; ++j )
	{
		m_IndicatorItem[nIdx].m_pGoalInfo[j]->Clear();
		m_IndicatorItem[nIdx].m_pGoalResult[j]->Clear();
		m_IndicatorItem[nIdx].m_pGoalInfo[j]->Show( false );
		m_IndicatorItem[nIdx].m_pGoalResult[j]->Show( false );			
	}

	// Data입력
	m_IndicatorItem[nIdx].m_TSKey = pIndicatorData->sTSKey;
	m_IndicatorItem[nIdx].m_uiQuestTitle = pIndicatorData->uiQuestTitle;
	m_IndicatorItem[nIdx].m_uiQuestGoal = pIndicatorData->uiQuestGoal;

	// 제목
	CQuestTextDataTable* pTable = API_GetTableContainer()->GetQuestTextDataTable();
	sQUEST_TEXT_DATA_TBLDAT* pData = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pTable->FindData( pIndicatorData->uiQuestTitle ) );
	std::wstring wstrTitle;
	
	if( pData )
		wstrTitle = gui::GetHtmlFromMemoryString( pData->wstrText.c_str(), pData->wstrText.size() );
	else
	{
		if( pIndicatorData->uiQuestTitle != INVALID_TBLIDX )
		{
			RwChar buf[256];
			sprintf_s( buf, 256, "%u QuestTextData is Missing", pIndicatorData->uiQuestTitle );
		}		
	}

	m_IndicatorItem[nIdx].m_pTitle->SetText( wstrTitle.c_str() );
	m_IndicatorItem[nIdx].m_nFrameHeight += m_IndicatorItem[nIdx].m_pTitle->GetHeight() + 5;

	UpdateState( pIndicatorData->sTSKey, pIndicatorData->uiQState );

	if( pIndicatorData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT )
	{
		CMobTable* pMobTable = API_GetTableContainer()->GetMobTable();
		CTextTable* pMobText = API_GetTableContainer()->GetTextAllTable()->GetMobTbl();

		for( j = 0 ; j < uSTOC_EVT_DATA::MAX_MOB_KILL ; ++j )
		{
			RwUInt32 nTblIdx = pMobTable->FindTblidxByGroup( pIndicatorData->uEvtInfoData.sMobKillCnt[j].uiMobIdx );
			sMOB_TBLDAT* pMobData = reinterpret_cast<sMOB_TBLDAT*>( pMobTable->FindData( nTblIdx ) );
			if( !pMobData )
				continue;

			UpdateResultItem( pMobText->GetText( pMobData->Name ).c_str(), pIndicatorData->uEvtInfoData.sMobKillCnt[j].nCurMobCnt, pIndicatorData->uEvtInfoData.sMobKillCnt[j].nMobCnt, nIdx, j );
		}
	}
	else if( pIndicatorData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT )
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for( j = 0 ; j < uSTOC_EVT_DATA::MAX_MOB_KILL_ITEM ; ++j )
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pItemTable->FindData( pIndicatorData->uEvtInfoData.sMobKillItemCnt[j].uiMobLIIdx ) );

			if( !pItemData )
				continue;

			UpdateResultItem( pQuestItemText->GetText( pItemData->ItemName ).c_str(), pIndicatorData->uEvtInfoData.sMobKillItemCnt[j].nCurMobLICnt, pIndicatorData->uEvtInfoData.sMobKillItemCnt[j].nMobLICnt, nIdx, j );
		}			
	}
	else if( pIndicatorData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM )
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for( j = 0 ; j < uSTOC_EVT_DATA::MAX_DELIVERY_ITEM ; ++j )
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pItemTable->FindData( pIndicatorData->uEvtInfoData.sDeliveryItemCnt[j].uiItemIdx ) );

			if( !pItemData )
				continue;

			UpdateResultItem( pQuestItemText->GetText( pItemData->ItemName ).c_str(), pIndicatorData->uEvtInfoData.sDeliveryItemCnt[j].nCurItemCnt, pIndicatorData->uEvtInfoData.sDeliveryItemCnt[j].nItemCnt, nIdx, j );
		}
	}
	else if( pIndicatorData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_OBJECT_ITEM )
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for( j = 0 ; j < uSTOC_EVT_DATA::MAX_OBJECT_ITEM ; ++j )
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pItemTable->FindData( pIndicatorData->uEvtInfoData.sObjectItemCnt[j].uiItemIdx ) );

			if( !pItemData )
				continue;

			UpdateResultItem( pQuestItemText->GetText( pItemData->ItemName ).c_str(), pIndicatorData->uEvtInfoData.sObjectItemCnt[j].nCurItemCnt, pIndicatorData->uEvtInfoData.sObjectItemCnt[j].nItemCnt, nIdx, j );
		}
	}
	else if( pIndicatorData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT )
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for( j = 0 ; j < uSTOC_EVT_DATA::MAX_PUBLIC_MOB_ITEM ; ++j )
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pItemTable->FindData( pIndicatorData->uEvtInfoData.sPublicMobItemCnt[j].uiItemIdx ) );

			if( !pItemData )
				continue;

			UpdateResultItem( pQuestItemText->GetText( pItemData->ItemName ).c_str(), pIndicatorData->uEvtInfoData.sPublicMobItemCnt[j].nCurItemCnt, pIndicatorData->uEvtInfoData.sPublicMobItemCnt[j].nItemCnt, nIdx, j );
		}
	}
	else if( pIndicatorData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT )
	{
		CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();

		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_CUSTOM_EVT_CNT ; ++i )
		{
			sQUEST_TEXT_DATA_TBLDAT* pQuestTextData = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pIndicatorData->uEvtInfoData.sCustomEvtCnt[i].uiQTextTblIdx ) );

			if( !pQuestTextData )
				continue;

			std::wstring strQuestText = gui::GetHtmlFromMemoryString( pQuestTextData->wstrText.c_str(), pQuestTextData->wstrText.size() );
			UpdateResultItem( strQuestText.c_str(), pIndicatorData->uEvtInfoData.sCustomEvtCnt[i].nCurCnt, pIndicatorData->uEvtInfoData.sCustomEvtCnt[i].nMaxCnt, nIdx, i );
		}
	}
	else if( pIndicatorData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_VISIT )
	{
		CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();

		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_VISIT_EVT ; ++i )
		{
			sQUEST_TEXT_DATA_TBLDAT* pQuestTextData = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pIndicatorData->uEvtInfoData.sVisitEvt[i].uiIndicatorQText ) );

			if( !pQuestTextData )
				continue;

			std::wstring wstrText = gui::GetHtmlFromMemoryString( pQuestTextData->wstrText.c_str(), pQuestTextData->wstrText.size() );
			UpdateResultItem( wstrText.c_str(), pIndicatorData->uEvtInfoData.sVisitEvt[i].bCompleted, nIdx, i );
		}
	}

	// 시간.
	if( pIndicatorData->uiTimeLimit != INVALID_DWORD )
	{
		RwUInt32 uiTime;
		RwUInt32 uiMinute;
		RwUInt32 uiSecond;

		if( pIndicatorData->uiTimeLimit <= 0 )
		{
			uiMinute = 0;
			uiSecond = 0;
			color = QUEST_INDICATOR_COLOR_GRAY;
		}
		else
		{
			uiTime = pIndicatorData->uiTimeLimit / 1000;
			uiMinute = uiTime / 60;
			uiSecond = uiTime % 60;

			if( pIndicatorData->uiTimeLimit <= 10 )
				color = QUEST_INDICATOR_COLOR_RED;
			else
				color = QUEST_INDICATOR_COLOR_WHITE;
		}

		m_IndicatorItem[nIdx].m_pTime->SetTextColor( color );
		m_IndicatorItem[nIdx].m_pTime->Format( GetDisplayStringManager()->GetString( "DST_QUEST_REMAIN_TIME" ), uiMinute, uiSecond );			
		m_IndicatorItem[nIdx].m_pTime->SetPosition( m_IndicatorItem[nIdx].m_pTime->GetPosition().left, m_IndicatorItem[nIdx].m_nFrameHeight );
		m_IndicatorItem[nIdx].m_nFrameHeight += m_IndicatorItem[nIdx].m_pTime->GetHeight();
	}		
}

VOID CQuestIndicatorGui::UpdateData( SNtlEventUpdateQuestProgressInfo_Nfy* pData )
{
	SNtlEventShowIndicator_Nfy* pIndicatorData;
	RwInt32 nIdx = FindData( pData->sTSKey.tID, &pIndicatorData );

	if( pIndicatorData )
	{
		pIndicatorData->eEvtInfoType = pData->eEvtInfoType;
		pIndicatorData->sTSKey = pData->sTSKey;
		pIndicatorData->uEvtInfoData = pData->uEvtInfoData;
		pIndicatorData->uiTimeLimit = pData->uiTimeLimit;

		DrawData( nIdx, pIndicatorData );		
	}	
}

VOID CQuestIndicatorGui::UpdateState( sTS_KEY& sTSKey, RwUInt32 uiQuestState )
{
	SNtlEventShowIndicator_Nfy* pIndicatorData;
	RwInt32 nIdx = FindData( sTSKey.tID, &pIndicatorData );

	if( pIndicatorData )
	{
		pIndicatorData->uiQState = uiQuestState;
		m_IndicatorItem[nIdx].m_pState->GetSurface()->clear();

		if( uiQuestState & eTS_PROG_STATE_PROGRESS_FAILED )
		{
			m_IndicatorItem[nIdx].m_pState->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestStateFail" ) );
			m_IndicatorItem[nIdx].m_pState->SetToolTip( GetDisplayStringManager()->GetString( "DST_QUESTLIST_TIP_FAILD_QUEST" ) );
		}
		else if( uiQuestState & eTS_PROG_STATE_ERROR_BLOCK )
		{
			m_IndicatorItem[nIdx].m_pState->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestStateFail" ) );
			m_IndicatorItem[nIdx].m_pState->SetToolTip( GetDisplayStringManager()->GetString( "DST_QUESTLIST_TIP_ERROR_BLOCK" ) );
		}
		else if( uiQuestState & eTS_PROG_STATE_CLEARED )
		{
			m_IndicatorItem[nIdx].m_pState->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestStateReward" ) );
			m_IndicatorItem[nIdx].m_pState->SetToolTip( GetDisplayStringManager()->GetString( "DST_QUESTLIST_TIP_CLEARD_QUEST" ) );
		}
		else
		{
			m_IndicatorItem[nIdx].m_pState->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestStateProgress" ) );
			m_IndicatorItem[nIdx].m_pState->SetToolTip( GetDisplayStringManager()->GetString( "DST_QUESTLIST_TIP_PROGRESS_QUEST" ) );
		}		
	}
}

VOID CQuestIndicatorGui::UpdateResultItem( const WCHAR* pInfo, RwInt32 nCurVal, RwInt32 nTargetVal, RwInt32 i, RwInt32 j )
{
	if( nCurVal >= nTargetVal )
	{
		m_IndicatorItem[i].m_pGoalInfo[j]->SetTextColor( QUEST_INDICATOR_COLOR_GRAY );
		m_IndicatorItem[i].m_pGoalResult[j]->SetTextColor( QUEST_INDICATOR_COLOR_GRAY );					
	}
	else
	{
		m_IndicatorItem[i].m_pGoalInfo[j]->SetTextColor( QUEST_INDICATOR_COLOR_WHITE );
		m_IndicatorItem[i].m_pGoalResult[j]->SetTextColor( QUEST_INDICATOR_COLOR_WHITE );		
	}

	m_IndicatorItem[i].m_pGoalInfo[j]->SetText( pInfo );
	m_IndicatorItem[i].m_pGoalResult[j]->Format( L" : %d / %d", nCurVal, nTargetVal );				

	RwInt32 nResultFitWidth = m_IndicatorItem[i].m_pGoalResult[j]->GetFitWidthToString();
	m_IndicatorItem[i].m_pGoalResult[j]->SetWidth( nResultFitWidth );

	RwInt32 nWidth = m_IndicatorItem[i].m_pGoalInfo[j]->GetFitWidthToString();
	if( nWidth > QUEST_INDICATOR_QUESTINFO_WIDTH - nResultFitWidth )
		nWidth = QUEST_INDICATOR_QUESTINFO_WIDTH - nResultFitWidth;
	m_IndicatorItem[i].m_pGoalInfo[j]->SetWidth( nWidth );

	m_IndicatorItem[i].m_pGoalInfo[j]->SetPosition(30, m_IndicatorItem[i].m_nFrameHeight );
	m_IndicatorItem[i].m_pGoalResult[j]->SetPosition(m_IndicatorItem[i].m_pGoalInfo[j]->GetPosition().right, m_IndicatorItem[i].m_nFrameHeight );
	m_IndicatorItem[i].m_nFrameHeight += m_IndicatorItem[i].m_pGoalInfo[j]->GetHeight();

	m_IndicatorItem[i].m_pGoalInfo[j]->Show( true );
	m_IndicatorItem[i].m_pGoalResult[j]->Show( true );	
}

VOID CQuestIndicatorGui::UpdateResultItem( const WCHAR* pInfo, RwBool bComplete, RwInt32 i, RwInt32 j )
{
	if( bComplete )
	{
		m_IndicatorItem[i].m_pGoalInfo[j]->SetTextColor( QUEST_INDICATOR_COLOR_GRAY );
		m_IndicatorItem[i].m_pGoalResult[j]->SetTextColor( QUEST_INDICATOR_COLOR_GRAY );					
	}
	else
	{
		m_IndicatorItem[i].m_pGoalInfo[j]->SetTextColor( QUEST_INDICATOR_COLOR_WHITE );
		m_IndicatorItem[i].m_pGoalResult[j]->SetTextColor( QUEST_INDICATOR_COLOR_WHITE );		
	}

	m_IndicatorItem[i].m_pGoalInfo[j]->SetText( pInfo );

	RwInt32 nResultFitWidth = m_IndicatorItem[i].m_pGoalResult[j]->GetFitWidthToString();
	m_IndicatorItem[i].m_pGoalResult[j]->SetWidth( nResultFitWidth );

	RwInt32 nWidth = m_IndicatorItem[i].m_pGoalInfo[j]->GetFitWidthToString();
	if( nWidth > QUEST_INDICATOR_QUESTINFO_WIDTH - nResultFitWidth )
		nWidth = QUEST_INDICATOR_QUESTINFO_WIDTH - nResultFitWidth;
	m_IndicatorItem[i].m_pGoalInfo[j]->SetWidth( nWidth );

	m_IndicatorItem[i].m_pGoalInfo[j]->SetPosition( m_IndicatorItem[i].m_pGoalInfo[j]->GetPosition().left, m_IndicatorItem[i].m_nFrameHeight );
	m_IndicatorItem[i].m_pGoalResult[j]->SetPosition( m_IndicatorItem[i].m_pGoalInfo[j]->GetPosition().right, m_IndicatorItem[i].m_nFrameHeight );
	m_IndicatorItem[i].m_nFrameHeight += m_IndicatorItem[i].m_pGoalInfo[j]->GetHeight();

	m_IndicatorItem[i].m_pGoalInfo[j]->Show( true );
	m_IndicatorItem[i].m_pGoalResult[j]->Show( true );	
}

VOID CQuestIndicatorGui::CalculatePosition(VOID)
{
	CRectangle listRect = GetPosition();
	int nYPos = 0;

	for(int i = 0; i < eMAX_CAN_PROGRESS_QUEST_NUM; i++)
	{
		if( !m_IndicatorItem[i].m_pFrame->IsVisible() )
			break;

		m_IndicatorItem[i].m_pFrame->SetHeight(m_IndicatorItem[i].m_nFrameHeight);
		m_IndicatorItem[i].m_Background.SetSize(QUEST_INDICATOR_FRAME_WIDTH, m_IndicatorItem[i].m_nFrameHeight);
;
		m_IndicatorItem[i].m_pFrame->SetPosition(0, nYPos);

		CRectangle rect = m_IndicatorItem[i].m_pFrame->GetPosition();
		CRectangle rect2 = m_ppnIndicatorList->GetPosition();

		m_IndicatorItem[i].m_Background.SetPosition(listRect.left + rect.left, listRect.top + rect.top + 25 + rect2.top);
		nYPos += rect.GetHeight() + QUEST_INDICATOR_ITEM_MARGIN;		
	}

	m_nTotalItemHeight = nYPos;

	if (m_nTotalItemHeight > QUEST_INDICATOR_MAX_HEIGHT)
	{
		m_ppnIndicatorList->SetHeight(QUEST_INDICATOR_MAX_HEIGHT);

		m_pscrBar->SetRange(0, m_nTotalItemHeight - QUEST_INDICATOR_MAX_HEIGHT);
		m_pscrBar->SetValue(m_nScrollPos);
		m_pscrBar->Show(true);
	}
	else
	{
		m_ppnIndicatorList->SetHeight(m_nTotalItemHeight);
		m_pscrBar->Show(false);
	}
}

RwBool	CQuestIndicatorGui::IsAvaliableHeight( SNtlEventShowIndicator_Nfy* pData )
{
	RwInt32 nItemHeight = QUEST_INDICATOR_QUESTTITLE_HEIGHT + QUEST_INDICATOR_ITEM_MARGIN;

	if( pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT )
	{
		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_MOB_KILL ; ++i )
		{
			if( pData->uEvtInfoData.sMobKillCnt[i].uiMobIdx == INVALID_TBLIDX )
				break;

			nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;
		}
	}
	else if( pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT )
	{
		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_MOB_KILL_ITEM ; ++i )
		{
			if( pData->uEvtInfoData.sMobKillItemCnt[i].uiMobLIIdx == INVALID_TBLIDX )
				break;

			nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;
		}
	}
	else if( pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM )
	{
		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_DELIVERY_ITEM ; ++i )
		{
			if( pData->uEvtInfoData.sDeliveryItemCnt[i].uiItemIdx == INVALID_TBLIDX )
				break;

			nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;
		}
	}
	else if( pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_OBJECT_ITEM )
	{
		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_OBJECT_ITEM ; ++i )
		{
			if( pData->uEvtInfoData.sObjectItemCnt[i].uiItemIdx == INVALID_TBLIDX )
				break;

			nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;
		}
	}
	else if( pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT )
	{
		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_PUBLIC_MOB_ITEM ; ++i )
		{
			if( pData->uEvtInfoData.sPublicMobItemCnt[i].uiItemIdx == INVALID_TBLIDX )
				break;

			nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;
		}
	}
	else if( pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT )
	{
		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_CUSTOM_EVT_CNT ; ++i )
		{
			if( pData->uEvtInfoData.sCustomEvtCnt[i].uiQTextTblIdx == INVALID_TBLIDX )
				break;

			nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;
		}
	}		
	else if( pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_VISIT )
	{
		for( RwInt32 i = 0 ; i < uSTOC_EVT_DATA::MAX_VISIT_EVT ; ++i )
		{
			if( pData->uEvtInfoData.sVisitEvt[i].uiIndicatorQText == INVALID_TBLIDX )
				break;

			nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;
		}
	}

	if( pData->uiTimeLimit != INVALID_DWORD )
		nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;

	//
	if( QUEST_INDICATOR_MAX_HEIGHT - m_nTotalItemHeight >= nItemHeight )
		return TRUE;
	else 
		return FALSE;
}

RwBool CQuestIndicatorGui::IsAvaliableHeight(VOID)
{
	if( QUEST_INDICATOR_MAX_HEIGHT - m_nTotalItemHeight >= 0 )
		return TRUE;
	else
		return FALSE;
}

RwInt32 CQuestIndicatorGui::FindData( RwUInt32 nID, SNtlEventShowIndicator_Nfy** ppData )
{
	RwInt32 i;
	LIST_DATA::iterator it;

	for( i = 0, it = m_listData.begin() ; it != m_listData.end() ; ++it, ++i )
	{
		if( (*it)->sTSKey.tID == nID )
		{
			*ppData = (*it);
			return i;
		}
	}

	*ppData = NULL;
	return -1;
}

VOID CQuestIndicatorGui::CreateData( SNtlEventShowIndicator_Nfy* pData )
{
	SNtlEventShowIndicator_Nfy* pIndiData = NTL_NEW SNtlEventShowIndicator_Nfy;
	memcpy( pIndiData, pData, sizeof( SNtlEventShowIndicator_Nfy ) );

	m_listData.push_back( pIndiData );

	CNtlStorageGroupQuest* pGroup = (CNtlStorageGroupQuest*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_QUEST );
	pGroup->IndicateQuest( pData->sTSKey.tID, TRUE );
}

VOID CQuestIndicatorGui::DeleteData( RwUInt32 nID )
{
	LIST_DATA::iterator it;

	CNtlStorageGroupQuest* pGroupQuest = (CNtlStorageGroupQuest*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_QUEST );

	for( it = m_listData.begin() ; it != m_listData.end() ; ++it )
	{
		if( (*it)->sTSKey.tID == nID )
		{
			pGroupQuest->IndicateQuest( (*it)->sTSKey.tID, FALSE );
			NTL_DELETE( (*it) );
			m_listData.erase( it );
			break;
		}
	}
}

VOID CQuestIndicatorGui::DeleteAllData(VOID)
{
	LIST_DATA::iterator it;

	for( it = m_listData.begin() ; it != m_listData.end() ; ++it )
	{
		NTL_DELETE( (*it) );
	}

	m_listData.clear();
}

void CQuestIndicatorGui::SetListItem(int nOffset)
{
	m_nScrollPos = nOffset;

	CRectangle rtPos = m_ppnIndicatorList->GetPosition();
	m_ppnIndicatorList->SetPosition(rtPos.left, -m_nScrollPos);

	CalculatePosition();
}

void CQuestIndicatorGui::OnPaint()
{
	for (int i = 0; i < eMAX_CAN_PROGRESS_QUEST_NUM; i++)
	{
		if (m_IndicatorItem[i].m_pFrame->IsVisible() == false)
			break;

		m_IndicatorItem[i].m_Background.Render();
	}
}

void CQuestIndicatorGui::OnClickMinimize( gui::CComponent* pComponent )
{
	CDboEventGenerator::SideDialogEvent( SDIALOG_MESSAGE_MINIMIZE, SDIALOG_QUEST_INDICATOR, DIALOG_SIDEDIALOG_CONTROLLER );
}

void CQuestIndicatorGui::OnClickAllExtendOn(gui::CComponent * pComponent)
{
	m_pbtnAllExtendOn->Show(false);
	m_pbtnAllExtendOff->Show(true);

	// to do: hide quest info


}

void CQuestIndicatorGui::OnClickAllExtendOff(gui::CComponent * pComponent)
{
	m_pbtnAllExtendOff->Show(false);
	m_pbtnAllExtendOn->Show(true);

	// to do: show quest info


}

void CQuestIndicatorGui::OnClickQuestListOpen(gui::CComponent * pComponent)
{
	if (GetDialogManager()->IsOpenDialog(DIALOG_QUESTLIST))
		GetDialogManager()->CloseDialog(DIALOG_QUESTLIST);
	else
		GetDialogManager()->OpenDialog(DIALOG_QUESTLIST);
}

void CQuestIndicatorGui::OnScrollChanged(int nNewOffset)
{
	SetListItem(nNewOffset);
}

void CQuestIndicatorGui::OnMouseWheel(int nFlag, RwInt16 sDelta, CPos & pos)
{
	if (m_pscrBar->IsVisible() == false)
		return;

	int nOffset = (int)((float)m_pscrBar->GetMaxValue() * 0.5f) * sDelta;
	CAPTURE_MOUSEWHEELMOVE_SCROLLBAR(IsShow(), m_pThis, m_pThis, m_pscrBar, nOffset, pos);
}
