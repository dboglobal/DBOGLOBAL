#include "precomp_dboclient.h"
#include "QuestInventoryGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLEvent.h"
#include "NtlInventory.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobQuestItem.h"
#include "NtlSobQuestItemAttr.h"
#include "NtlSobIcon.h"

// Dbo
#include "DboDef.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboLogic.h"
#include "InfoWndManager.h"
#include "DialogManager.h"
#include "IconMoveManager.h"
#include "PopupManager.h"
#include "QuestListGui.h"
#include "AlarmManager.h"
#include "DisplayStringManager.h"

#include "GUISoundDefine.h"

CQuestInventoryGui::CQuestInventoryGui(VOID)
{
	Init();
}

CQuestInventoryGui::~CQuestInventoryGui(VOID)
{

}

VOID CQuestInventoryGui::Init(VOID)
{
	m_pNextInvenBtn = NULL;
	m_pPrevInvenBtn = NULL;
	m_pInvenInfo = NULL;

	m_nLSelectedSlotIdx = -1;
	m_nRSelectedSlotIdx = -1;
	m_nMouseOnIdx = -1;
	m_nPushDownIdx = -1;

	m_nInvenPage = 0;

	m_nShowFocus = -1;
	m_nShowPickedUp = -1;

	memset( m_pStackNum, 0, sizeof( m_pStackNum ) );

	m_pQuestListGui = NULL;
}

RwBool CQuestInventoryGui::Create( CQuestListGui* pQuestListGui )
{
	NTL_FUNCTION( "CQuestInventoryGui::Create" );

	m_pQuestListGui = pQuestListGui;
	m_pBack = (gui::CDialog*)pQuestListGui->GetComponent( "dlgInven" );

	m_pbtnDiscard = (gui::CButton*)pQuestListGui->GetComponent( "btnDiscard" );
	m_pNextInvenBtn = (gui::CButton*)pQuestListGui->GetComponent( "btnNextInven" );
	m_pPrevInvenBtn = (gui::CButton*)pQuestListGui->GetComponent( "btnPrevInven" );
	m_pInvenInfo = (gui::CStaticBox*)pQuestListGui->GetComponent( "stbInvenInfo" );

	m_pNextInvenBtn->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( "DST_QUEST_NEXT_INVENTORY" ) ) );
	m_pPrevInvenBtn->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( "DST_QUEST_PREV_INVENTORY" ) ) );
	m_pbtnDiscard->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( "DST_DISCARD_BTN" ) ) );
	
	m_slotMouseDown		= m_pBack->SigMouseDown().Connect( this, &CQuestInventoryGui::OnMouseDown );
	m_slotMouseUp		= m_pBack->SigMouseUp().Connect( this, &CQuestInventoryGui::OnMouseUp );
	m_slotMouseMove		= m_pBack->SigMouseMove().Connect( this, &CQuestInventoryGui::OnMouseMove );
	m_slotMouseOut		= m_pBack->SigMouseLeave().Connect( this, &CQuestInventoryGui::OnMouseOut );
	m_slotPaint			= m_pBack->SigPaint().Connect( this, &CQuestInventoryGui::OnPaint );
	m_slotMove			= m_pBack->SigMove().Connect( this, &CQuestInventoryGui::OnMove );
	m_slotClickNextInven= m_pNextInvenBtn->SigClicked().Connect( this, &CQuestInventoryGui::OnNextInvenBtnClick );
	m_slotClickPrevInven= m_pPrevInvenBtn->SigClicked().Connect( this, &CQuestInventoryGui::OnPrevInvenBtnClick );
	m_slotClickDiscardButton = m_pbtnDiscard->SigClicked().Connect( this, &CQuestInventoryGui::OnClickDiscardButton );

	SetSlotRectHardCode();
	UpdateInvenInfo();

	m_pbtnDiscard->Raise(); // raise so the button is click-able

	NTL_RETURN( TRUE );
}

VOID CQuestInventoryGui::Destroy(VOID)
{
	ClearInventory();
}

VOID CQuestInventoryGui::UpdateData(VOID)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlQuestInventory* pInventory = pAvatar->GetQuestInventory();

	for( RwInt32 i = 0 ; i < QUESTINVENTORY_MAXCOL ; ++i )
	{
		CNtlSobQuestItem* pItem = GetQuestItemFromIdx( i );

		if( pItem )
		{
			m_surSlot[i].SetTexture( (gui::CTexture*)pItem->GetIcon()->GetImage() );
			CNtlSobQuestItemAttr* pItemAttr = reinterpret_cast<CNtlSobQuestItemAttr*>( pItem->GetSobAttr() );
			RwInt32 nStackNumber = pItemAttr->GetStackNum();

			if( nStackNumber > 0 )
			{
				if( m_pStackNum[i] )
				{
					m_pStackNum[i]->SetText( nStackNumber );
				}
				else
				{
					CreateStackNumber( i, nStackNumber );
				}
			}
			else
			{
				if( m_pStackNum[i] )
				{
					DeleteStackNumber( i );
				}
			}
		}
		else
		{
			ClearInventorySlot( i, m_nInvenPage );			
		}
	}

	if( m_nMouseOnIdx >= 0 && GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_QUESTITEM )
	{
		CNtlSobQuestItem* pItem = pInventory->GetQuestItemFromIdx( (RwUInt8)m_nMouseOnIdx );

		if( pItem )
		{
			CRectangle rtScreen = m_pBack->GetScreenRect();
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_QUESTITEM, 
				m_rtSlot[m_nMouseOnIdx].left + rtScreen.left, m_rtSlot[m_nMouseOnIdx].top + rtScreen.top, pItem, DIALOG_QUESTLIST );
		}
		else
		{
			GetInfoWndManager()->ShowInfoWindow( FALSE );
		}
	}
}

VOID CQuestInventoryGui::ClearInventory(VOID)
{
	for( RwInt32 i = 0 ; i < QUESTINVENTORY_MAXROW ; ++i )
	{
		for( RwInt32 j = 0 ; j < QUESTINVENTORY_MAXCOL ; ++j )
		{
			ClearInventorySlot( j, i );
		}
	}
}

VOID CQuestInventoryGui::ClearInventorySlot( RwInt32 nSlotIdx, RwInt32 nRow )
{
	NTL_ASSERT( nSlotIdx >= 0 && nSlotIdx < QUESTINVENTORY_MAXCOL, "CQuestInventoryGui::ClearInventorySlot : Invalid Col Index" );
	NTL_ASSERT( nRow >= 0 && nRow < QUESTINVENTORY_MAXROW, "CQuestInventoryGui::ClearInventorySlot : Invalid Row Index" );

	if( m_nInvenPage == nRow )
	{
		m_surSlot[nSlotIdx].SetTexture( (gui::CTexture*)NULL );
		DeleteStackNumber( nSlotIdx );
	}

	m_surDisable[nSlotIdx][nRow].Show( FALSE );	
}

VOID CQuestInventoryGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pData = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
			return;		

		if( pData->hSerialId != pAvatar->GetSerialID() )
			return;

		if( pData->uiUpdateType & EVENT_AIUT_QUESTITEM )
		{
			UpdateData();
		}
	}
	else if( msg.Id == g_EventPickedUpHide )
	{
		RwInt32 nPlace = (RwInt32)msg.pData;
		if( nPlace != PLACE_QUESTBAG )
			return;

		ShowPickedUp( FALSE );
	}
	else if( msg.Id == g_EventEnableItemIcon )
	{
		SDboEventEnableItemIcon* pData = reinterpret_cast<SDboEventEnableItemIcon*>( msg.pData );

		if( pData->ePlace != PLACE_QUESTBAG )
			return;

		ShowDisable( !pData->bEnable, pData->uiSlotIdx );
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		ClearInventory();
	}
}

RwBool CQuestInventoryGui::CreateStackNumber( RwInt32 nSlotIdx, RwInt32 nValue )
{
	NTL_FUNCTION( "CQuestInventoryGui::CreateStackNubmer" );

	CRectangle rect;
	rect.SetRect( m_rtSlot[nSlotIdx].left, m_rtSlot[nSlotIdx].bottom - DBOGUI_STACKNUM_HEIGHT,
				  m_rtSlot[nSlotIdx].left + DBOGUI_STACKNUM_WIDTH, m_rtSlot[nSlotIdx].bottom );
	m_pStackNum[nSlotIdx] = NTL_NEW gui::CStaticBox( rect, m_pBack, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN );
	if( !m_pStackNum[nSlotIdx] )
		NTL_RETURN( FALSE );

	m_pStackNum[nSlotIdx]->CreateFontStd( DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR );
	m_pStackNum[nSlotIdx]->SetEffectMode( DBOGUI_STACKNUM_FONTEFFECTMODE );
	m_pStackNum[nSlotIdx]->SetText( nValue );
	m_pStackNum[nSlotIdx]->Enable( false );

	NTL_RETURN( TRUE );
}

VOID CQuestInventoryGui::DeleteStackNumber( RwInt32 nSlotIdx )
{
	if( m_pStackNum[nSlotIdx] )
		NTL_DELETE( m_pStackNum[nSlotIdx] );
}

VOID CQuestInventoryGui::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pBack->GetScreenRect();
	
	for( RwInt32 i = 0 ; i < QUESTINVENTORY_MAXCOL ; ++i )
	{	
		RwInt32 nCol = i % 5;
		RwInt32 nRow = i / 5;

		m_rtSlot[i].left	= nCol * 40 + 13;
		m_rtSlot[i].top		= nRow * 38 + 7;
		m_rtSlot[i].right	= m_rtSlot[i].left + DBOGUI_ICON_SIZE;
		m_rtSlot[i].bottom	= m_rtSlot[i].top + DBOGUI_ICON_SIZE;

		m_surSlot[i].SetRectWH( rtScreen.left + m_rtSlot[i].left, rtScreen.top + m_rtSlot[i].top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );

		for( RwInt32 j = 0 ; j < QUESTINVENTORY_MAXROW ; ++j )
		{
			m_surDisable[i][j].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotDisableEffect" ) );		
			m_surDisable[i][j].SetRectWH( m_rtSlot[i].left, m_rtSlot[i].top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
			m_surDisable[i][j].Show( FALSE );
		}
	}

	m_surFocusSlot.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_surFocusSlot.SetRectWH( 0, 0, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_surPickedUp.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );
}

VOID CQuestInventoryGui::ClickEffect( RwBool bPush, RwInt32 nSlotIdx /* = -1  */)
{
	CRectangle rtScreen = m_pBack->GetScreenRect();

	if( bPush )
	{
		m_surSlot[nSlotIdx].SetRect( rtScreen.left + m_rtSlot[nSlotIdx].left + ICONPUSH_SIZEDIFF, rtScreen.top + m_rtSlot[nSlotIdx].top + ICONPUSH_SIZEDIFF,
									 rtScreen.left + m_rtSlot[nSlotIdx].right - ICONPUSH_SIZEDIFF, rtScreen.top + m_rtSlot[nSlotIdx].bottom - ICONPUSH_SIZEDIFF );
	}
	else if( m_nPushDownIdx >= 0 )
	{
		m_surSlot[m_nPushDownIdx].SetRect( rtScreen.left + m_rtSlot[m_nPushDownIdx].left, rtScreen.top + m_rtSlot[m_nPushDownIdx].top,
										   rtScreen.left + m_rtSlot[m_nPushDownIdx].right, rtScreen.top + m_rtSlot[m_nPushDownIdx].bottom );
	}

	m_nPushDownIdx = nSlotIdx;
}

VOID CQuestInventoryGui::ShowPickedUp( RwBool bShow, RwInt32 nSlotIdx /* = -1  */)
{
	if( bShow )
	{
		CRectangle rtScreen = m_pBack->GetScreenRect();

		m_nShowPickedUp = GetQuestItemIdxFromIdx( nSlotIdx );
		m_surPickedUp.SetPosition( m_rtSlot[nSlotIdx].left + rtScreen.left, m_rtSlot[nSlotIdx].top + rtScreen.top );
	}
	else
	{
		m_nShowPickedUp = -1;
	}
}

VOID CQuestInventoryGui::ShowFocus( RwBool bShow, RwInt32 nSlotIdx /* = -1  */)
{
	if( bShow )
	{
		CRectangle rtScreen = m_pBack->GetScreenRect();

		m_nShowFocus = nSlotIdx;
		m_surFocusSlot.SetPosition( m_rtSlot[nSlotIdx].left + rtScreen.left, m_rtSlot[nSlotIdx].top + rtScreen.top );									
	}
	else
	{
		m_nShowFocus = nSlotIdx;
	}
}

VOID CQuestInventoryGui::ShowDisable( RwBool bShow, RwInt32 nSlotIdx )
{
	RwInt32 nRow = nSlotIdx / QUESTINVENTORY_MAXCOL;
	RwInt32 nCol = nSlotIdx % QUESTINVENTORY_MAXCOL;

	m_surDisable[nCol][nRow].Show( bShow );
}

RwInt32 CQuestInventoryGui::GetChildSlotIdx( RwInt32 nX, RwInt32 nY )
{
	for( RwInt32 i = 0 ; i < QUESTINVENTORY_MAXCOL ; ++i )
	{
		if( m_rtSlot[i].PtInRect( nX, nY ) )
			return i;
	}

	return -1;
}

RwBool CQuestInventoryGui::IsEnablePickUp( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 || nSlotIdx >= QUESTINVENTORY_MAXCOL )
		return FALSE;

	if( m_surDisable[nSlotIdx][m_nInvenPage].IsShow() )
		return FALSE;

	if( !GetIconMoveManager()->IsEnable() )
		return FALSE;

	CNtlSobQuestItem* pItem = GetQuestItemFromIdx( nSlotIdx );

	if( pItem )
		return TRUE;

	return FALSE;
}

RwBool CQuestInventoryGui::IsEnablePutDown( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 || nSlotIdx >= QUESTINVENTORY_MAXCOL )
		return FALSE;

	if( m_surDisable[nSlotIdx][m_nInvenPage].IsShow() )
		return FALSE;

	return TRUE;
}

VOID CQuestInventoryGui::UpdateInvenInfo(VOID)
{
	m_pInvenInfo->Format( "%d / %d", m_nInvenPage + 1, QUESTINVENTORY_MAXROW );
	
	if( m_nInvenPage <= 0 )
		m_pPrevInvenBtn->ClickEnable( FALSE );
	else
		m_pPrevInvenBtn->ClickEnable( TRUE );

	if( m_nInvenPage + 1 >= QUESTINVENTORY_MAXROW )
		m_pNextInvenBtn->ClickEnable( FALSE );
	else
		m_pNextInvenBtn->ClickEnable( TRUE );

}

CNtlSobQuestItem* CQuestInventoryGui::GetQuestItemFromIdx( RwInt32 nIdx )
{ 
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return NULL;

	return pAvatar->GetQuestInventory()->GetQuestItemFromIdx((RwUInt8)GetQuestItemIdxFromIdx(nIdx)  );
}

RwUInt32 CQuestInventoryGui::GetQuestItemSerialFromIdx( RwInt32 nIdx )
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return NULL;

	return pAvatar->GetQuestInventory()->GetQuestItemSerial( (RwUInt8)GetQuestItemIdxFromIdx( nIdx ) );
}

RwInt32 CQuestInventoryGui::GetQuestItemIdxFromIdx( RwInt32 nIdx )
{
	return nIdx + ( m_nInvenPage * QUESTINVENTORY_MAXCOL );
}

RwInt32	CQuestInventoryGui::GetCurrentPageFromIdx( RwInt32 nIdx )
{
	if( nIdx < 0 )
		return -1;

	return nIdx / QUESTINVENTORY_MAXCOL;
}

VOID CQuestInventoryGui::OnMouseDown( const CKey& key )
{
	RwInt32 nClickIdx = GetChildSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );
	
	if( nClickIdx >= 0 && !GetIconMoveManager()->IsActive() )
		ClickEffect( TRUE, nClickIdx );
	
	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( GetIconMoveManager()->IsActive() )
		{
			if( IsEnablePutDown( nClickIdx ) )
				m_nLSelectedSlotIdx = nClickIdx;
		}
		else
		{
			if( IsEnablePickUp( nClickIdx ) )
				m_nLSelectedSlotIdx = nClickIdx;			
		}
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( !GetIconMoveManager()->IsActive() )
		{
			m_nRSelectedSlotIdx = nClickIdx;	
		}
	}

	m_pBack->CaptureMouse();

	if( m_nLSelectedSlotIdx >= 0 && m_nRSelectedSlotIdx >= 0 )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		ClickEffect( FALSE );
		m_pBack->ReleaseMouse();
	}
}

VOID CQuestInventoryGui::OnMouseUp( const CKey& key )
{
	RwInt32 nSlotIdx = GetChildSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );
	ClickEffect( FALSE );

	m_pBack->ReleaseMouse();

	if( nSlotIdx < 0 || !m_pBack->IsVisibleTruly() )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		return;
	}

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( nSlotIdx == m_nLSelectedSlotIdx )
		{
			if( GetIconMoveManager()->IsActive() )
			{
				GetIconMoveManager()->IconMovePutDown( PLACE_QUESTBAG, INVALID_SERIAL_ID, GetQuestItemIdxFromIdx( nSlotIdx ) );
			}
			else
			{
				CNtlSobQuestItem* pItem = GetQuestItemFromIdx( nSlotIdx );
				if( pItem )
				{
					if( GetIconMoveManager()->IconMovePickUp( pItem->GetSerialID(), PLACE_QUESTBAG, GetQuestItemIdxFromIdx( nSlotIdx ), 0, pItem->GetIcon()->GetImage() ) )
						ShowPickedUp( TRUE, nSlotIdx );
				}						
			}
		}

		m_nLSelectedSlotIdx = -1;
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		//if( nSlotIdx == m_nRSelectedSlotIdx )
		//{
		//	CRectangle rtScreen = m_pBack->GetScreenRect();
		//	SERIAL_HANDLE hSerial = GetQuestItemSerialFromIdx( nSlotIdx );
		//	CDboEventGenerator::IconPopupShow( TRUE, hSerial, PLACE_QUESTBAG, GetQuestItemIdxFromIdx( nSlotIdx ), m_rtSlot[nSlotIdx].left + rtScreen.left, m_rtSlot[nSlotIdx].top + rtScreen.top );
		//}

		m_nRSelectedSlotIdx = -1;
	}
}

VOID CQuestInventoryGui::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	RwInt32 nMouseOnIdx = GetChildSlotIdx( nXPos, nYPos );

	if( nMouseOnIdx >= 0 )
	{
		ShowFocus( TRUE, nMouseOnIdx );

		if( m_nMouseOnIdx != nMouseOnIdx )
		{
			CNtlSobQuestItem* pItem = GetQuestItemFromIdx( nMouseOnIdx );

			if( pItem )
			{
				CRectangle rtScreen = m_pBack->GetScreenRect();
				CNtlSobQuestItemAttr* pAttr = reinterpret_cast<CNtlSobQuestItemAttr*>( pItem->GetSobAttr() );

				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_QUESTITEM, m_rtSlot[nMouseOnIdx].left + rtScreen.left,
													 m_rtSlot[nMouseOnIdx].top + rtScreen.top, pAttr->GetQuestItemTbl(), DIALOG_QUESTLIST );
				m_nMouseOnIdx = nMouseOnIdx;
			}

			if( nMouseOnIdx == m_nPushDownIdx )
				ClickEffect( TRUE, nMouseOnIdx );
			else if( m_nPushDownIdx >= 0 )
				ClickEffect( FALSE );
		}
	}
	else
	{
		ShowFocus( FALSE );

		if( m_nMouseOnIdx >= 0 )
		{
			GetInfoWndManager()->ShowInfoWindow( FALSE );
			if( m_nPushDownIdx >= 0 )
				ClickEffect( FALSE );
			m_nMouseOnIdx = -1;
		}
	}
}

VOID CQuestInventoryGui::OnMouseOut( gui::CComponent* pComonent )
{
	ShowFocus( FALSE );

	if( m_nMouseOnIdx >= 0 )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );

		if( m_nPushDownIdx >= 0 )
			ClickEffect( FALSE );
		m_nMouseOnIdx = -1;
	}
}

VOID CQuestInventoryGui::OnPaint(VOID)
{
	for( RwInt32 i = 0 ; i < QUESTINVENTORY_MAXCOL ; ++i )
	{
		if( m_surSlot[i].GetTexture() )
			m_surSlot[i].Render();

		m_surDisable[i][m_nInvenPage].Render();
	}

	if( GetCurrentPageFromIdx( m_nShowPickedUp ) == m_nInvenPage )
		m_surPickedUp.Render();
	
	if( m_nShowFocus >= 0 )
		m_surFocusSlot.Render();
}

VOID CQuestInventoryGui::OnMove( RwInt32 nXPos, RwInt32 nYPos )
{
	CRectangle rtScreen = m_pBack->GetScreenRect();
	CRectangle rect;
	
	for( RwInt32 i = 0 ; i < QUESTINVENTORY_MAXCOL ; ++i )
	{
		m_surSlot[i].SetPosition( rtScreen.left + m_rtSlot[i].left, rtScreen.top + m_rtSlot[i].top );

		for( RwInt32 j = 0 ; j < QUESTINVENTORY_MAXROW ; ++j )
			m_surDisable[i][j].SetPosition( rtScreen.left + m_rtSlot[i].left, rtScreen.top + m_rtSlot[i].top );							
	}

	if( m_nShowPickedUp >= 0 )
		m_surPickedUp.SetPosition( rtScreen.left + m_rtSlot[GetQuestItemIdxFromIdx( m_nShowPickedUp )].left, rtScreen.top + m_rtSlot[GetQuestItemIdxFromIdx(m_nShowPickedUp)].top );

	if( m_nShowFocus >= 0)
		m_surFocusSlot.SetPosition( rtScreen.left + m_rtSlot[m_nShowFocus].left, rtScreen.top + m_rtSlot[m_nShowFocus].top );

}

VOID CQuestInventoryGui::OnNextInvenBtnClick( gui::CComponent* pComponent )
{
	if( ++m_nInvenPage >= QUESTINVENTORY_MAXROW )
		m_nInvenPage = QUESTINVENTORY_MAXROW - 1;

	UpdateData();
	UpdateInvenInfo();
}

VOID CQuestInventoryGui::OnPrevInvenBtnClick( gui::CComponent* pComponent )
{
	if( --m_nInvenPage < 0 )
		m_nInvenPage = 0;

	UpdateData();
	UpdateInvenInfo();
}

VOID CQuestInventoryGui::OnClickDiscardButton( gui::CComponent* pComponent )
{
	Logic_ItemDropConfirmProc();
}