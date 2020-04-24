#include "precomp_dboclient.h"
#include "QuestListGui.h"

// core
#include "NtlDebug.h"
#include "NtlTSCoreDefine.h"
#include "DboTSCoreDefine.h"

// shared
#include "QuestTextDataTable.h"
#include "TableContainer.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlParty.h"
#include "NtlSLLogic.h"

#include "NtlStorageManager.h"
#include "NtlStorageGroupQuest.h"
	
// presentation
#include "NtlPLGuiManager.h"

// gui
#include "GuiUtil.h"

// dbo
#include "DialogManager.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "QuestGui.h"
#include "QuestIndicatorGui.h"
#include "QuestInventoryGui.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "AlarmManager.h"

#include "GUISoundDefine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Size Define
#define QUESTLIST_X						13
#define QUESTLIST_Y						38	
#define QUESTLIST_WIDTH					234
#define QUESTLIST_HEIGHT				240
#define QUESTLIST_LINE_HEIGHT			20
#define QUESTLIST_LINE_MARGIN			5
#define QUESTLIST_CHILD_XMARGIN			0
#define QUESTLIST_SCROLLBAR_WIDTH		12
#define QUESTLIST_SCROLLBAR_BTN_WIDTH	12
#define QUESTLIST_SCROLLBAR_BTN_HEIGHT	19
#define QUESTLIST_SCROLLBAR_SLIDER		39

#define QUEST_REGIONNODE_BUTTON_X		1
#define QUEST_REGIONNODE_BUTTON_Y		1
#define QUEST_REGIONNODE_BUTTON_WIDTH	18
#define QUEST_REGIONNODE_BUTTON_HEIGHT	18

#define QUEST_REGIONNODE_TITLE_X		23
#define QUEST_REGIONNODE_TITLE_Y		1
#define QUEST_REGIONNODE_TITLE_WIDTH	196
#define QUEST_REGIONNODE_TITLE_HEIGHT	20

#define QUEST_QUESTNODE_BACK_X			1
#define QUEST_QUESTNODE_BACK_Y			0
#define QUEST_QUESTNODE_BACK_WIDTH		203
#define QUEST_QUESTNODE_BACK_HEIGHT		20

#define QUEST_QUESTNODE_SHARE_X			3
#define QUEST_QUESTNODE_SHARE_Y			3
#define QUEST_QUESTNODE_SHARE_WIDTH		15
#define QUEST_QUESTNODE_SHARE_HEIGHT	15

#define QUEST_QUESTNODE_STATE_X			20
#define QUEST_QUESTNODE_STATE_Y			3
#define QUEST_QUESTNODE_STATE_WIDTH		15
#define QUEST_QUESTNODE_STATE_HEIGHT	15

#define QUEST_QUESTNODE_INDICATOR_X			204
#define QUEST_QUESTNODE_INDICATOR_Y			4
#define QUEST_QUESTNODE_INDICATOR_WIDTH		12
#define QUEST_QUESTNODE_INDICATOR_HEIGHT	12

#define QUESTNODE_DOWNCOORD_DIFFX			0
#define QUESTNODE_DOWNCOORD_DIFFY			1
#define QUESTNODE_TEXT_X					38

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CQuestListGui

CQuestListGui::CQuestListGui(VOID)
: m_pQuestList( NULL ), m_pbtnClose( NULL ), m_pQuestGui( NULL ), m_pstbTitle( NULL ), m_pstbInventoryTitle( NULL ), m_pQuestInventory( NULL )
{
	
}

CQuestListGui::CQuestListGui( const RwChar* pName )
: CNtlPLGui( pName ), m_pQuestList( NULL ), m_pbtnClose( NULL ), m_pQuestGui( NULL ), m_pstbTitle( NULL ), m_pstbInventoryTitle( NULL ), m_pQuestInventory( NULL )
{

}

CQuestListGui::~CQuestListGui(VOID)
{
	
}

RwBool CQuestListGui::Create( CQuestGui* pQuestGui )
{
	NTL_FUNCTION( "CQuestListGui::Create" );

	m_questCounter = 0;

	if( !CNtlPLGui::Create( "", "gui\\QuestList.srf", "gui\\QuestList.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pbtnClose = (gui::CButton*)GetComponent( "btnExit" );
	m_pstbTitle = (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pstbNoQuest = (gui::CStaticBox*)GetComponent( "stbNoQuest" );
	m_pstbQuestTotalCount = (gui::CStaticBox*)GetComponent("stbQuestTotalCount");
	m_pstbInventoryTitle = (gui::CStaticBox*)GetComponent( "stbInventoryTitle" );
	m_pBtnHelp = (gui::CButton*)GetComponent("btnHelp");

	m_slotClickedBtnHelp = m_pBtnHelp->SigClicked().Connect( this, &CQuestListGui::OnClickedBtnHelp );
	m_slotClickExitButton = m_pbtnClose->SigClicked().Connect( this, &CQuestListGui::OnClickExitButton );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CQuestListGui::OnCaptureMouseDown );
	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CQuestListGui::OnCaptureWheelMove );
		
	m_pQuestList = NTL_NEW CQuestList( pQuestGui );
	CRectangle rect;
	rect.SetRectWH( QUESTLIST_X, QUESTLIST_Y, QUESTLIST_WIDTH, QUESTLIST_HEIGHT );
	if( !m_pQuestList->Create( rect, m_pThis, QUESTLIST_LINE_HEIGHT, QUESTLIST_LINE_MARGIN, QUESTLIST_CHILD_XMARGIN, QUESTLIST_SCROLLBAR_WIDTH ) )
		NTL_RETURN( FALSE );

	// QuestInventory
	m_pQuestInventory = NTL_NEW	CQuestInventoryGui;
	m_pQuestInventory->Create( this );

	Show( false );

	m_pQuestGui = pQuestGui;

	m_pstbTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUESTLIST_TITLE" ) );
	
	m_pstbQuestTotalCount->SetMargin(m_pstbTitle->GetFitWidthToString() * 2, 0);
	//m_pstbQuestTotalCount->SetText("( 0 / 20 )");
	m_pstbQuestTotalCount->Format(L"( %i / %i )", m_questCounter, (int)eMAX_CAN_PROGRESS_QUEST_NUM);

	m_pstbInventoryTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUESTLIST_INVENTITLE" ) );
	m_pstbNoQuest->SetText( GetDisplayStringManager()->GetString( "DST_QUESTLIST_NOQUEST" ) );
	m_pstbNoQuest->Show( true );

	LinkMsg( g_EventSobInfoUpdate, 0 );
	LinkMsg( g_EventPickedUpHide, 0 );
	LinkMsg( g_EventEnableItemIcon, 0 );
	LinkMsg( g_EventTSRemovingTMQQuest_Nfy, 0 );
	
	NTL_RETURN( TRUE );
}

VOID CQuestListGui::Destroy(VOID)
{
	m_pQuestInventory->Destroy();
	NTL_DELETE( m_pQuestInventory );

	m_pQuestList->Destroy();
	NTL_DELETE( m_pQuestList );

	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventPickedUpHide );	
	UnLinkMsg( g_EventEnableItemIcon );
	UnLinkMsg( g_EventTSRemovingTMQQuest_Nfy );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CQuestListGui::SwitchDialog( bool bOpen )
{
	m_pThis->Show( bOpen );

	if( !bOpen )
	{
		if( GetInfoWndManager()->GetRequestGui() == DIALOG_QUESTLIST )
			GetInfoWndManager()->ShowInfoWindow( FALSE );
	}

	return 1;
}

VOID CQuestListGui::GetQuestList( std::list<stQUESTLISTDATA>* pList )
{
	CGuiLineTreeNode::LIST_LINENODE* m_listRegion = m_pQuestList->GetRootNode()->GetChildNodeAll();
	CGuiLineTreeNode::LIST_LINENODE::iterator it;

	for( it = m_listRegion->begin() ; it != m_listRegion->end() ; ++it )
	{
		CGuiLineTreeNode::LIST_LINENODE* m_listQuest = (*it)->GetChildNodeAll();
		CGuiLineTreeNode::LIST_LINENODE::iterator itq;

		for( itq = m_listQuest->begin() ; itq != m_listQuest->end() ; ++itq )
		{
			CQuestNode* pQuestNode = reinterpret_cast<CQuestNode*>( *itq );
			stQUESTLISTDATA stQuestData;
			stQuestData.m_TSKey = pQuestNode->GetQuestKey();
			stQuestData.m_uiSortType = pQuestNode->GetQuestSortType();

			pList->push_back( stQuestData );
		}
	}
}

VOID CQuestListGui::CancelIndicator( sTS_KEY& sTSKey, RwBool bAuto )
{
	CGuiLineTreeNode::LIST_LINENODE* m_listRegion = m_pQuestList->GetRootNode()->GetChildNodeAll();
	CGuiLineTreeNode::LIST_LINENODE::iterator it;

	for( it = m_listRegion->begin() ; it != m_listRegion->end() ; ++it )
	{
		CGuiLineTreeNode::LIST_LINENODE* m_listQuest = (*it)->GetChildNodeAll();
		CGuiLineTreeNode::LIST_LINENODE::iterator itq;

		for( itq = m_listQuest->begin() ; itq != m_listQuest->end() ; ++itq )
		{
			CQuestNode* pQuestNode = reinterpret_cast<CQuestNode*>( *itq );
			
			if( pQuestNode->GetQuestKey().tID == sTSKey.tID )
			{
				pQuestNode->SetDownIndicatorBtn( false );
				CNtlSLEventGenerator::HideIndicator( sTSKey );

				if( !GetNtlWorldConcept()->IsActivePlayConcept( WORLD_PLAY_TIME_MACHINE ) && !bAuto )
					GetAlarmManager()->AlarmMessage( "DST_QUEST_INDICATOR_CANNOT_ADD_INDICATE_ITEM" );
			}			
		}
	}	
}

VOID CQuestListGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventRegQuest_Nfy )
	{
		m_pQuestList->RegistQuest( msg.pData );
		m_pstbQuestTotalCount->Format(L" ( %i / %i )", ++m_questCounter, (int)eMAX_CAN_PROGRESS_QUEST_NUM);

		if( m_pstbNoQuest->IsVisible() )
			m_pstbNoQuest->Show( false );
	}
	else if( msg.Id == g_EventUnregQuest_Nfy )
	{
		SNtlEventUnregQuest_Nfy* pData = reinterpret_cast<SNtlEventUnregQuest_Nfy*>( msg.pData );
		
		m_pQuestList->UnRegistQuest( pData->sTSKey.tID );
		m_pstbQuestTotalCount->Format(L" ( %i / %i )", --m_questCounter, (int)eMAX_CAN_PROGRESS_QUEST_NUM);

		if( !m_pQuestList->GetRootNode()->IsChildExist() &&
			!m_pstbNoQuest->IsVisible() )
		{
			m_pstbNoQuest->Show( true );
		}
	}
	else if( msg.Id == g_EventTSRemovingTMQQuest_Nfy )
	{
		SNtlEventTSRemovingTMQQuest_Nfy* pData = reinterpret_cast<SNtlEventTSRemovingTMQQuest_Nfy*>( msg.pData );

		m_pQuestList->UnRegistQuest( pData->tRmvTSId );
		m_pstbQuestTotalCount->Format(L" ( %i / %i )", --m_questCounter, (int)eMAX_CAN_PROGRESS_QUEST_NUM);

		if( !m_pQuestList->GetRootNode()->IsChildExist() &&
			!m_pstbNoQuest->IsVisible() )
		{
			m_pstbNoQuest->Show( true );
		}
	}
	else if( msg.Id == g_EventUpdateQuestState_Nfy )
	{
		m_pQuestList->UpdateState( msg.pData );
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pData = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( pData->eWorldConcept == WORLD_PLAY_RANK_BATTLE ||
			pData->eWorldConcept == WORLD_PLAY_TIME_MACHINE )
		{
			if( pData->uiState == WORLD_STATE_EXIT )
			{
				RestoreIndicator();
			}
		}
	}
	else
	{
		m_pQuestInventory->HandleEvents( msg );
	}
}

VOID CQuestListGui::ClearIndicator(VOID)
{
	CGuiLineTreeNode::LIST_LINENODE* m_listRegion = m_pQuestList->GetRootNode()->GetChildNodeAll();
	CGuiLineTreeNode::LIST_LINENODE::iterator it;

	for( it = m_listRegion->begin() ; it != m_listRegion->end() ; ++it )
	{
		CGuiLineTreeNode::LIST_LINENODE* m_listQuest = (*it)->GetChildNodeAll();
		CGuiLineTreeNode::LIST_LINENODE::iterator itq;

		for( itq = m_listQuest->begin() ; itq != m_listQuest->end() ; ++itq )
		{
			CQuestNode* pQuestNode = reinterpret_cast<CQuestNode*>( *itq );

			pQuestNode->SetEnableIndicatorBtn( FALSE );				
		}
	}
}

VOID CQuestListGui::RestoreIndicator(VOID)
{
	CGuiLineTreeNode::LIST_LINENODE* m_listRegion = m_pQuestList->GetRootNode()->GetChildNodeAll();
	CGuiLineTreeNode::LIST_LINENODE::iterator it;

	for( it = m_listRegion->begin() ; it != m_listRegion->end() ; ++it )
	{
		CGuiLineTreeNode::LIST_LINENODE* m_listQuest = (*it)->GetChildNodeAll();
		CGuiLineTreeNode::LIST_LINENODE::iterator itq;

		for( itq = m_listQuest->begin() ; itq != m_listQuest->end() ; ++itq )
		{
			CQuestNode* pQuestNode = reinterpret_cast<CQuestNode*>( *itq );
			pQuestNode->SetEnableIndicatorBtn( TRUE );

			if( pQuestNode->IsReservedIndicator() )
			{
				pQuestNode->SetDownIndicatorBtn( TRUE );
			}
		}
	}
}

VOID CQuestListGui::OnClickExitButton( gui::CComponent* pComponent )
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_QUEST_WND_CLOSE_BTN ) )
		return;

	GetDialogManager()->CloseDialog( DIALOG_QUESTLIST );	
}

VOID CQuestListGui::OnClickedBtnHelp( gui::CComponent* pComponent )
{
	CDboEventGenerator::OpenHelpContent( DIALOG_QUESTLIST );
}

VOID CQuestListGui::OnCaptureMouseDown( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_QUESTLIST, key.m_fX, key.m_fY );
}

VOID CQuestListGui::OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos )
{
	CAPTURE_MOUSEWHEELMOVE_SCROLLBAR( IsShow(), m_pQuestList->GetParentGui(), m_pQuestList->GetParentGui(), m_pQuestList->GetScrollBar(), sDelta, pos );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CQuestList

CQuestList::CQuestList( CQuestGui* pQuestGui )
{
	m_pQuestGui = pQuestGui;
}

CQuestList::~CQuestList(VOID)
{

}

RwBool CQuestList::Create( CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth )
{
	if( !CGuiLineTree::Create( rect, pParent, nLineHeight, nLineMargin, nChildXMargin, nScrollBarWidth ) )
		return FALSE;

	m_pScroll->AddSurfaceLayout( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollLayout" ) );
	m_pScroll->AddSurfaceSlider( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollMedium" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnUp" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnDown" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnFoc" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnUp" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnDown" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnUp" ) );
	m_pScroll->SetButtonSize( QUESTLIST_SCROLLBAR_BTN_WIDTH, QUESTLIST_SCROLLBAR_BTN_HEIGHT );
	m_pScroll->SetSliderSize( QUESTLIST_SCROLLBAR_WIDTH, QUESTLIST_SCROLLBAR_SLIDER );

	return TRUE;
}

VOID CQuestList::RegistQuest( VOID* pData )
{
	SNtlEventRegQuest_Nfy* pAddData = reinterpret_cast<SNtlEventRegQuest_Nfy*>( pData );

	// 같은 아이디가 있다면 먼저 지운다.///////////////////
	UnRegistQuest( pAddData->sTSKey.tID );
	///////////////////////////////////////////////////////
	
	CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
	sQUEST_TEXT_DATA_TBLDAT* pQuestNodeText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pAddData->uiQuestTitle ) );

	std::wstring wstr1;
	
	switch( pAddData->eQuestSortType )
	{
	case eQUEST_SORT_TYPE_TASK : 
		{
			sQUEST_TEXT_DATA_TBLDAT* pRegionNodeText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pAddData->uiQuestAreaName ) );

			if( pRegionNodeText )
				wstr1 = pRegionNodeText->wstrText;
			else
			{
				if( pAddData->uiQuestAreaName != INVALID_TBLIDX )
				{
					WCHAR wBuf[256];
					swprintf_s( wBuf, 256, L"%u QuestTextData is Missing", pAddData->uiQuestAreaName );
					wstr1 = wBuf;
				}
			}
		}
		break;
	case eQUEST_SORT_TYPE_SCENARIO :
		wstr1 = GetDisplayStringManager()->GetString( "DST_QUEST_CAT_SCENARIO" );
		break;
	case eQUEST_SORT_TYPE_TIMEMACHINE :
		wstr1 = GetDisplayStringManager()->GetString( "DST_QUEST_CAT_TIMEMACHINE" );
		break;
	case eQUEST_SORT_TYPE_CHANGEJOB :
		wstr1 = GetDisplayStringManager()->GetString( "DST_QUEST_CAT_JOBCHANGE" );
		break;
	case eQUEST_SORT_TYPE_DRAGONBALL :
		wstr1 = GetDisplayStringManager()->GetString( "DST_QUEST_CAT_DRAGONBALL" );
		break;
	case eQUEST_SORT_TYPE_TIMELEAPQUEST :
		wstr1 = GetDisplayStringManager()->GetString( "DST_QUEST_CAT_TLQ" );
		break;
	case eQUEST_SORT_TYPE_SYSTEM:
		wstr1 = GetDisplayStringManager()->GetString("DST_QUEST_CAT_SYSTEM");
		break;
	case eQUEST_SORT_TYPE_EVENT:
		wstr1 = GetDisplayStringManager()->GetString("DST_QUEST_CAT_EVENT");
		break;
	case eQUEST_SORT_TYPE_ELITE:
		wstr1 = GetDisplayStringManager()->GetString("DST_QUEST_CAT_ELITE");
		break;
	default :
		wstr1 = L"QuestSortTypeError";
		break;
	}
	
	RwInt32 nID = RegionNameToID( wstr1 );
	CRegionNode* pRegionNode = reinterpret_cast<CRegionNode*>( FindNode( nID ) );

	std::wstring wstr2;
	if( pQuestNodeText )
		wstr2 = pQuestNodeText->wstrText;	
	else
	{
		if( pAddData->uiQuestTitle != INVALID_TBLIDX )
		{
			WCHAR wBuf[256];
			swprintf_s( wBuf, 256, L"%u QuestTextData is Missing", pAddData->uiQuestTitle );
			wstr2 = wBuf;
		}			
	}	
	 
	CQuestNode* pQuestNode = NTL_NEW CQuestNode( this, pAddData->sTSKey, wstr2, pAddData->eQuestSortType, pAddData->uiQuestState, pAddData->bQuestShare, pAddData->bNewRegister );

	if( pRegionNode )
	{
		pRegionNode->AddChildNode( pQuestNode );
	}
	else
	{
		pRegionNode = NTL_NEW CRegionNode( this, nID, wstr1 ); // strText1 <- pRegionNodeText->wstrText
		pRegionNode->AddChildNode( pQuestNode );
		AddNode( pRegionNode );
	}

	pRegionNode->Expand( TRUE );	

	// If it is new or registered in the last game, it is automatically registered.
	
	CNtlStorageGroupQuest* pGroupQuest = (CNtlStorageGroupQuest*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_QUEST );
	SMemoryQuestData* pMemoryData = pGroupQuest->GetQuestData( pAddData->sTSKey.tID );

	if( pAddData->bNewRegister ||
		( pMemoryData && pMemoryData->bIsIndicated ) )		
	{
		pQuestNode->SetDownIndicatorBtn( true );
		ToggleIndicator( pAddData->sTSKey, true, TRUE );		
	}		
}

VOID CQuestList::UnRegistQuest( RwUInt16 tID )
{
	CQuestNode* pQuestNode = reinterpret_cast<CQuestNode*>( FindNode( tID ) );
	
	if( !pQuestNode )
		return;

	CRegionNode* pRegionNode = reinterpret_cast<CRegionNode*>( pQuestNode->GetParentNode() );
	
	pRegionNode->DeleteChildNode( tID );

	if( !pRegionNode->IsChildExist() )
		m_pRootNode->DeleteChildNode( pRegionNode->GetID() );
}

VOID CQuestList::UpdateState( VOID* pData )
{
	SNtlEventUpdateQuestState_Nfy* pUpdateData = reinterpret_cast<SNtlEventUpdateQuestState_Nfy*>( pData );

	CQuestNode* pNode = reinterpret_cast<CQuestNode*>( FindNode( pUpdateData->sTSKey.tID ) );
	if( pNode )
		pNode->SetState( pUpdateData->uiQuestState );
}

VOID CQuestList::ToggleIndicator( sTS_KEY& stTSKey, bool bToggle, RwBool bAuto /* = TRUE  */ )
{
	if( bToggle )
	{
		CNtlSLEventGenerator::ShowIndicator( stTSKey, bAuto );
	}
	else
	{
		CNtlSLEventGenerator::HideIndicator( stTSKey );
	}	
}
	
RwInt32 CQuestList::RegionNameToID( std::wstring wstrRegion )
{
	static RwInt32 sRegionID = 0x00010000;		// QuestID는 Short;

	MAP_REGIONID::iterator it;

	it = m_mapRegionID.find( wstrRegion );
	
	if( it != m_mapRegionID.end() )
	{
		return (*it).second;
	}
	else
	{
		m_mapRegionID[wstrRegion] = sRegionID;
		return sRegionID++;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRegionNode

CRegionNode::CRegionNode( CGuiLineTree* pTree, RwInt32 nRegionID, std::wstring wstrRegionName )
: CGuiLineTreeNode( pTree, nRegionID ), m_pbtnExpand( NULL ), m_pbtnReduce( NULL )
{
	CRectangle rect;

	rect.SetRectWH( QUEST_REGIONNODE_TITLE_X, QUEST_REGIONNODE_TITLE_Y, QUEST_REGIONNODE_TITLE_WIDTH, QUEST_REGIONNODE_TITLE_HEIGHT );
	m_pstbTitle = NTL_NEW gui::CStaticBox( rect, pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pstbTitle->SetTextColor( QUEST_TITLE_COLOR );
	m_pstbTitle->SetText( gui::GetHtmlFromMemoryString( wstrRegionName.c_str(), wstrRegionName.size() ).c_str() );
	if( m_pstbTitle->GetFitWidthToString() > m_pstbTitle->GetWidth() )
		m_pstbTitle->SetToolTip( m_pstbTitle->GetText() );
	else
		m_pstbTitle->DeleteToolTip();

	rect.SetRectWH( QUEST_REGIONNODE_BUTTON_X, QUEST_REGIONNODE_BUTTON_Y, QUEST_REGIONNODE_BUTTON_WIDTH, QUEST_REGIONNODE_BUTTON_HEIGHT );
	m_pbtnExpand = NTL_NEW gui::CButton( rect, std::string(),pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pbtnExpand->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfExpandBtnUp" ) );
	m_pbtnExpand->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfExpandBtnFoc" ) );
	m_pbtnExpand->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfExpandBtnDown" ) );

	m_pbtnReduce = NTL_NEW gui::CButton( rect, std::string(),pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pbtnReduce->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfReduceBtnUp" ) );
	m_pbtnReduce->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfReduceBtnFoc" ) );
	m_pbtnReduce->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfReduceBtnDown" ) );
	
	m_pbtnExpand->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( "DST_QUEST_SHOW_QUEST_CATALOG" ) ) );
	m_pbtnReduce->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( "DST_QUEST_HIDE_QUEST_CATALOG" ) ) );

	m_slotClickExpand = m_pbtnExpand->SigClicked().Connect( this, &CRegionNode::OnClickExpand );
	m_slotClickReduce = m_pbtnReduce->SigClicked().Connect( this, &CRegionNode::OnClickReduce );
}

CRegionNode::~CRegionNode(VOID)
{
	NTL_DELETE( m_pbtnExpand );
	NTL_DELETE( m_pbtnReduce );
	NTL_DELETE( m_pstbTitle );
}

VOID CRegionNode::ShowProc(VOID)
{
	CGuiLineTreeNode::ShowProc();

	m_pstbTitle->Show( true );
	if( IsExpand() )
	{
		m_pbtnReduce->Show( true );		
		m_pbtnExpand->Show( false );
	}
	else
	{
		m_pbtnExpand->Show( true );
		m_pbtnReduce->Show( false );		
	}

	m_pbtnExpand->SetPosition( m_nPosX + QUEST_REGIONNODE_BUTTON_X, m_nPosY + QUEST_REGIONNODE_TITLE_Y );
	m_pbtnReduce->SetPosition( m_nPosX + QUEST_REGIONNODE_BUTTON_X, m_nPosY + QUEST_REGIONNODE_TITLE_Y );
	m_pstbTitle->SetPosition( m_nPosX + QUEST_REGIONNODE_TITLE_X, m_nPosY + QUEST_REGIONNODE_TITLE_Y );
}

VOID CRegionNode::HideProc(VOID)
{
	CGuiLineTreeNode::HideProc();

	m_pstbTitle->Show( false );
	m_pbtnExpand->Show( false );
	m_pbtnReduce->Show( false );
}

VOID CRegionNode::Expand( RwBool bExpand )
{
	if( bExpand )
		CGuiLineTreeNode::Expand( true );
	else
		CGuiLineTreeNode::Expand( false );
}

VOID CRegionNode::OnClickExpand( gui::CComponent* pComponent )
{
	CGuiLineTreeNode::Expand( TRUE );	
	m_pbtnReduce->ShowToolTip( true );
}

VOID CRegionNode::OnClickReduce( gui::CComponent* pComponent )
{
	CGuiLineTreeNode::Expand( FALSE );
	m_pbtnExpand->ShowToolTip( true );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CQuestNode

CQuestNode::CQuestNode( CGuiLineTree* pTree, sTS_KEY stTSKey, std::wstring wstrTitle, RwUInt32 uiType, RwUInt32 uiState, bool bShare, bool bIsNew )
: CGuiLineTreeNode( pTree, (RwInt32)stTSKey.tID ), m_pbtnIndicator( NULL ), m_pbtnTitle( NULL ), m_bShare( bShare ), 
  m_dwClickTime( 0 ), m_ppnlState( NULL ), m_uiState( 0xffffffff ), m_bReserveIndicator( FALSE )
{
	CRectangle rect;
	rect.SetRectWH( QUEST_QUESTNODE_BACK_X, QUEST_QUESTNODE_BACK_Y, QUEST_QUESTNODE_BACK_WIDTH, QUEST_QUESTNODE_BACK_HEIGHT );
	m_pbtnTitle = NTL_NEW gui::CButton( rect, std::string(), pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pbtnTitle->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestItemBack" ) );
	m_pbtnTitle->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestItemBack" ) );
	m_pbtnTitle->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestItemBackFoc" ) );
	m_pbtnTitle->SetTextDownCoordDiff( QUESTNODE_DOWNCOORD_DIFFX, QUESTNODE_DOWNCOORD_DIFFY );
	m_pbtnTitle->SetTextCoord( QUESTNODE_TEXT_X, 0 );
	m_pbtnTitle->SetTextStyle( COMP_TEXT_LEFT );

	// 새로 생긴 녀석이거나, 이전 게임에서 새로 생긴 후 한번도 안내를 보지 않았으면 새퀘스트 처리
		CNtlStorageGroupQuest* pGroupQuest = (CNtlStorageGroupQuest*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_QUEST );
	SMemoryQuestData* pMemoryData = pGroupQuest->GetQuestData( stTSKey.tID );
	m_wstrTitle = gui::GetHtmlFromMemoryString( wstrTitle.c_str(), wstrTitle.size() );
	if( bIsNew ||
		( pMemoryData && pMemoryData->bIsNew ) )
	{
		m_pbtnTitle->SetTextUpColor( INFOCOLOR_7 );		
		m_pbtnTitle->SetText( m_wstrTitle.c_str() );
		m_bIsNew = TRUE;

		if( bIsNew )
			pGroupQuest->NewQuest( stTSKey.tID, TRUE );
	}
	else
	{
		m_pbtnTitle->SetTextUpColor( INFOCOLOR_0 );
		m_pbtnTitle->SetText( m_wstrTitle.c_str() );
		m_bIsNew = FALSE;
	}
	m_pbtnTitle->ApplyText();
		
	if( m_pbtnTitle->GetTextComponent()->GetFitWidthToString() > m_pbtnTitle->GetTextComponent()->GetWidth() )
		m_pbtnTitle->SetToolTip( m_wstrTitle.c_str() );
	else
		m_pbtnTitle->DeleteToolTip();
	
	rect.SetRectWH( QUEST_QUESTNODE_INDICATOR_X, QUEST_QUESTNODE_INDICATOR_Y, QUEST_QUESTNODE_INDICATOR_WIDTH, QUEST_QUESTNODE_INDICATOR_HEIGHT );
	m_pbtnIndicator = NTL_NEW gui::CButton( rect, std::string(), pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager(), true );
	m_pbtnIndicator->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestIndicatorUp" ) );
	m_pbtnIndicator->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestIndicatorDown" ) );
	m_pbtnIndicator->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestIndicatorFoc" ) );
	m_pbtnIndicator->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( "DST_QUEST_INDICATOR" ) ) );

	CNtlWorldConcept* pWorldConcept = GetNtlWorldConcept();
	if( pWorldConcept->IsActivePlayConcept( WORLD_PLAY_TIME_MACHINE ) )			
		m_pbtnIndicator->Enable( false );

	rect.SetRectWH( QUEST_QUESTNODE_SHARE_X, QUEST_QUESTNODE_SHARE_Y, QUEST_QUESTNODE_SHARE_WIDTH, QUEST_QUESTNODE_SHARE_HEIGHT );
	m_pbtnShare = NTL_NEW gui::CButton( rect, std::string(), pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pbtnShare->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestShareBtnUp" ) );
	m_pbtnShare->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestShareBtnDown" ) );
	m_pbtnShare->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestShareBtnFoc" ) );
	m_pbtnShare->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( "DST_QUEST_SHARE" ) ) );
		
	rect.SetRectWH( QUEST_QUESTNODE_STATE_X, QUEST_QUESTNODE_STATE_Y, QUEST_QUESTNODE_STATE_WIDTH, QUEST_QUESTNODE_STATE_HEIGHT );
	m_ppnlState = NTL_NEW gui::CPanel( rect, pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	
	m_slotTitleClick = m_pbtnTitle->SigClicked().Connect( this, &CQuestNode::OnTitleClick );
	m_slotShareClick = m_pbtnShare->SigClicked().Connect( this, &CQuestNode::OnClickShare );
	m_slotToggleIndicator = m_pbtnIndicator->SigToggled().Connect( this, &CQuestNode::OnToggleIndicator );
	m_slotTitlePressed	= m_pbtnTitle->SigPressed().Connect( this, &CQuestNode::OnPressedCommand );
	m_slotTitleReleased	= m_pbtnTitle->SigReleased().Connect( this, &CQuestNode::OnReleasedCommand );

	// Key Data 입력.
	m_TSKey = stTSKey;
	m_uiSortType = uiType;
	SetState( uiState );
		
	LinkMsg( g_EventPartyUpdateValue, 0 );
	LinkMsg( g_EventShowQuestWindow_Nfy );
}

CQuestNode::~CQuestNode(VOID)
{
	UnLinkMsg( g_EventPartyUpdateValue );
	UnLinkMsg( g_EventShowQuestWindow_Nfy );

	NTL_DELETE( m_pbtnShare );
	NTL_DELETE( m_pbtnIndicator );
	NTL_DELETE( m_pbtnTitle );
	NTL_DELETE( m_ppnlState );		
}

VOID CQuestNode::SetState( RwUInt32 uiState )
{
	if( uiState == m_uiState )   
		return;

	m_uiState = uiState;

	m_ppnlState->GetSurface()->clear();

	if( uiState & eTS_PROG_STATE_PROGRESS_FAILED )
	{
		m_ppnlState->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestStateFail" ) );
		m_ppnlState->SetToolTip( GetDisplayStringManager()->GetString( "DST_QUESTLIST_TIP_FAILD_QUEST" ) );
	}
	else if( uiState & eTS_PROG_STATE_ERROR_BLOCK )
	{
		m_ppnlState->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestStateFail" ) );
		m_ppnlState->SetToolTip( GetDisplayStringManager()->GetString( "DST_QUESTLIST_TIP_ERROR_BLOCK" ) );
	}
	else if( uiState & eTS_PROG_STATE_CLEARED )
	{
		m_ppnlState->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestStateReward" ) );
		m_ppnlState->SetToolTip( GetDisplayStringManager()->GetString( "DST_QUESTLIST_TIP_CLEARD_QUEST" ) );
	}
	else
	{
		m_ppnlState->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuestList.srf", "srfQuestStateProgress" ) );
		m_ppnlState->SetToolTip( GetDisplayStringManager()->GetString( "DST_QUESTLIST_TIP_PROGRESS_QUEST" ) );
	}
}

VOID CQuestNode::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventPartyUpdateValue )
	{
		SNtlEventPartyUpdateValue* pData = reinterpret_cast<SNtlEventPartyUpdateValue*>( msg.pData );

		if( pData->iValueType == PMT_PARTY_MEMBER_ADD || pData->iValueType == PMT_PARTY_MEMBER_DEL || pData->iValueType == PMT_PARTY_LEAVE )
		{
			if( m_bShow )
			{
				if( CanShowShareButton() )
					m_pbtnShare->Show( true );
				else
					m_pbtnShare->Show( false );
			}
		}
	}
	else if( msg.Id == g_EventShowQuestWindow_Nfy )
	{
		SNtlEventShowQuestWindow_Nfy* pData = reinterpret_cast<SNtlEventShowQuestWindow_Nfy*>( msg.pData );	
		
		if( pData->sTSKey.tID == m_TSKey.tID && m_bIsNew )
		{
			m_pbtnTitle->SetTextUpColor( INFOCOLOR_0 );
			m_pbtnTitle->SetText( m_wstrTitle.c_str() );
			m_pbtnTitle->ApplyText();

			CNtlStorageGroupQuest* pGroupQuest = (CNtlStorageGroupQuest*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_QUEST );
			pGroupQuest->NewQuest( m_TSKey.tID, FALSE );
			m_bIsNew = FALSE;			
		}
	}
}

VOID CQuestNode::SetDownIndicatorBtn( bool bDown )
{
	m_pbtnIndicator->SetDown( bDown );
}

VOID CQuestNode::SetEnableIndicatorBtn( bool bEnable )
{
	m_pbtnIndicator->Enable( bEnable );
}

RwBool CQuestNode::IsIndicatorBtnDown(VOID)
{
	return m_pbtnIndicator->IsDown();
}

RwBool CQuestNode::CanShowShareButton(VOID)
{
	if( !m_bShare )
		return FALSE;

	if( GetNtlSLGlobal()->GetSobAvatar()->GetParty()->GetMemberCount() <= 1 )
		return FALSE;

	return TRUE;
}

VOID CQuestNode::ShowProc(VOID)
{
	CGuiLineTreeNode::ShowProc();	// m_nPosY 계산

	if( CanShowShareButton() )
		m_pbtnShare->Show( true );
	else
		m_pbtnShare->Show( false );
	
	m_pbtnIndicator->Show( true );
	m_pbtnTitle->Show( true );
	m_ppnlState->Show( true );
	
	m_pbtnIndicator->SetPosition( m_nPosX + QUEST_QUESTNODE_INDICATOR_X, m_nPosY + QUEST_QUESTNODE_INDICATOR_Y );
	m_pbtnTitle->SetPosition( m_nPosX + QUEST_QUESTNODE_BACK_X, m_nPosY + QUEST_QUESTNODE_BACK_Y );
	m_pbtnShare->SetPosition( m_nPosX + QUEST_QUESTNODE_SHARE_X, m_nPosY + QUEST_QUESTNODE_SHARE_Y );
	m_ppnlState->SetPosition( m_nPosX + QUEST_QUESTNODE_STATE_X, m_nPosY + QUEST_QUESTNODE_STATE_Y );	
}

VOID CQuestNode::HideProc(VOID)
{
	CGuiLineTreeNode::HideProc();

	m_pbtnShare->Show( false );
	m_pbtnIndicator->Show( false );
	m_pbtnTitle->Show( false );
	m_ppnlState->Show( false );	
}
					
VOID CQuestNode::OnToggleIndicator( gui::CComponent* pComponent, bool bToggle )
{
	reinterpret_cast<CQuestList*>( m_pMgr )->ToggleIndicator( m_TSKey, bToggle, FALSE );	
}

VOID CQuestNode::OnTitleClick( gui::CComponent* pComponent )
{
	CNtlSLEventGenerator::ShowQuestWindow( m_TSKey );	
}

VOID CQuestNode::OnClickShare( gui::CComponent* pComponent )
{
	DWORD dwTime = GetTickCount();

	if( dwTime - m_dwClickTime <= 5000 )
		return;

	m_dwClickTime = dwTime;

	CNtlSLEventGenerator::QuestShare_Nfy( m_TSKey );
}

VOID CQuestNode::OnPressedCommand( gui::CComponent* pComponent )
{
	CRectangle rect = m_pbtnIndicator->GetPosition();
	m_pbtnIndicator->SetPosition( rect.left + QUESTNODE_DOWNCOORD_DIFFX, rect.top + QUESTNODE_DOWNCOORD_DIFFY );
}

VOID CQuestNode::OnReleasedCommand( gui::CComponent* pComponent )
{
	CRectangle rect = m_pbtnIndicator->GetPosition();
	m_pbtnIndicator->SetPosition( rect.left - QUESTNODE_DOWNCOORD_DIFFX, rect.top - QUESTNODE_DOWNCOORD_DIFFY );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////