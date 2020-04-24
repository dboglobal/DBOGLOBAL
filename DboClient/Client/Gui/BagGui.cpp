#include "precomp_dboclient.h"

// shared
#include "NtlItem.h"

// core
#include "ceventhandler.h"
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"
#include "gui_renderer.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLDef.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobIcon.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "InputActionMap.h"
#include "NtlSlLogic.h"
#include "NtlWorldConceptTrade.h"

// Table
#include "ItemTable.h"

// DBO
#include "DboGlobal.h"
#include "SurfaceGui.h"
#include "DboLogic.h"	
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "IconMoveManager.h"
#include "PopupManager.h"
#include "InfoWndManager.h"
#include "ChatGui.h"
#include "DialogManager.h"
#include "GUISoundDefine.h"
#include "AlarmManager.h"
#include "DisplayStringManager.h"
#include "SideDialogManager.h"

// Self
#include "BagGui.h"

#define dFLASH_BOOM_EFFECT	"Tenkaichi_Boom.swf"
#define dFLASH_BOOM_WIDTH	64
#define dFLASH_BOOM_HEIGHT	64
#define BAG_CALC_OFFSET_X 104


////////////////////////////////////////////////////////////////////////////////
// BagBaseGui

CBagBaseGui::CBagBaseGui(VOID)
{
	Init();
}

CBagBaseGui::CBagBaseGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CBagBaseGui::~CBagBaseGui(VOID)
{

}

VOID CBagBaseGui::Init(VOID)
{
	m_hBagSerial = INVALID_SERIAL_ID;
	m_ppstbStackNum = NULL;
	m_pCloseButton = NULL;

	m_cBagPos = -1;

	m_prtSlots = NULL;
	m_psurIcons = NULL;
	m_psurFritzSlot = NULL;
	m_psurDisableSlot = NULL;
	m_nNumSlot = 0;
	m_nLSelectedSlotIdx = -1;
	m_nRSelectedSlotIdx = -1;
	m_nMouseOnIndex = -1;
	m_nPushDownIndex = -1;
	m_nCapsuleLockSlotIdx = -1;
	
	m_nShowPickedUp = -1;
		
	m_pCoolTimeEffect = NULL;	

	// Gamble
	m_ppGambleBoomEffect = NULL;
	m_pbIsGambleEffect = NULL;

	m_pPBox = NULL;

	m_bCEnable = TRUE;
}

RwBool CBagBaseGui::Create(VOID)
{
	NTL_FUNCTION( "CBagBaseGui::Create" );

	m_surFocusSlot.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_surPickedUp.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );
	m_surFocusSlot.Show( FALSE );
		
	if( m_nNumSlot <= 0 )
		NTL_RETURN( FALSE );

	m_prtSlots = NTL_NEW CRectangle[m_nNumSlot];
	m_psurIcons= NTL_NEW CSurfaceGui[m_nNumSlot];
	m_ppstbStackNum = NTL_NEW gui::CStaticBox*[m_nNumSlot];
	m_psurDisableSlot = NTL_NEW CSurfaceGui[m_nNumSlot];
	m_psurFritzSlot = NTL_NEW CSurfaceGui[m_nNumSlot];
	m_pCoolTimeEffect = NTL_NEW gui::CRadarEffect[m_nNumSlot];

	// Gamble Effect
	m_ppGambleBoomEffect = NTL_NEW gui::CFlash*[m_nNumSlot];	///< 겜블 붐 효과
	m_pSlotBoomEffectEnd = NTL_NEW gui::CSlot[m_nNumSlot];
	m_pbIsGambleEffect = NTL_NEW RwBool[m_nNumSlot];
	for( RwInt32 i = 0; i < m_nNumSlot; ++i )
	{
		m_ppGambleBoomEffect[i] = NTL_NEW gui::CFlash(m_pThis, GetNtlGuiManager()->GetSurfaceManager() );
		m_ppGambleBoomEffect[i]->SetSize( dFLASH_BOOM_WIDTH, dFLASH_BOOM_HEIGHT );
		m_ppGambleBoomEffect[i]->SetResizeType(gui::CFlash::EXACTFIT);
		m_ppGambleBoomEffect[i]->Enable( false );
		m_ppGambleBoomEffect[i]->Show( false );

		// Link
		m_pSlotBoomEffectEnd[i] = m_ppGambleBoomEffect[i]->SigMovieEnd().Connect( this, &CBagBaseGui::OnMovieEnd );

		// 플레이 안되고 있다고 초기화
		m_pbIsGambleEffect[i] = FALSE;
	}

	// Item Identify Particle
	m_pPBox = NTL_NEW gui::CParticleBox( m_pThis, GetNtlGuiManager()->GetSurfaceManager() );
	// Test
	m_pPBox->Load( "ItemEffect" );
	
	gui::CSurface surDisable = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotDisableEffect" );

	for( RwInt32 i = 0 ; i < m_nNumSlot ; ++i )
	{
		m_ppstbStackNum[i] = NULL;
		m_psurDisableSlot[i].SetSurface( surDisable );
		m_psurDisableSlot[i].Show( FALSE );
		m_psurIcons[i].Show( FALSE );
		m_psurFritzSlot[i].Show( FALSE );
	}

	m_pstbTitle = (gui::CStaticBox*)GetComponent( "stbTitle" );	

	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CBagBaseGui::OnCaptureMouseDown );

	LinkMsg( g_EventCalcPopupResult, 0 );
	LinkMsg( g_EventIconPopupResult, 0 );
	LinkMsg( g_EventPickedUpHide, 0 );
	LinkMsg( g_EventDialog, 0 );
	LinkMsg( g_EventSobCooling, 0 );
	LinkMsg( g_EventEnableItemIcon, 0 );
	LinkMsg( g_EventCapsuleLockItem, 0 );
	LinkMsg( g_EventGambleUseEffect, 0 );
	LinkMsg( g_EventGameServerChangeOut );
	LinkMsg( g_EventItemIdentifyEffect, 0 );
	LinkMsg( g_EventResize, 0 );

	// Update 연결
	GetNtlGuiManager()->AddUpdateFunc( this );
	
	NTL_RETURN( TRUE );
}

VOID CBagBaseGui::Destroy(VOID)
{
	NTL_FUNCTION( "CBagBaseGui::Destroy" );

	ClearInventory();

	GetNtlGuiManager()->RemoveUpdateFunc( this );
	
	UnLinkMsg( g_EventCalcPopupResult );
	UnLinkMsg( g_EventIconPopupResult );
	UnLinkMsg( g_EventPickedUpHide );
	UnLinkMsg( g_EventDialog );
	UnLinkMsg( g_EventSobCooling );
	UnLinkMsg( g_EventEnableItemIcon );
	UnLinkMsg( g_EventCapsuleLockItem );
	UnLinkMsg( g_EventGambleUseEffect );
	UnLinkMsg( g_EventGameServerChangeOut );
	UnLinkMsg( g_EventItemIdentifyEffect );
	UnLinkMsg( g_EventResize );

	NTL_ARRAY_DELETE( m_prtSlots );
	NTL_ARRAY_DELETE( m_psurIcons );
	NTL_ARRAY_DELETE( m_psurDisableSlot );
	NTL_ARRAY_DELETE( m_pCoolTimeEffect );
	NTL_ARRAY_DELETE( m_psurFritzSlot );

	// Gamble
	if( m_ppGambleBoomEffect )
	{
		for( RwInt32 i = 0 ; i < m_nNumSlot; ++i )
			NTL_DELETE( m_ppGambleBoomEffect[i] );	

		NTL_ARRAY_DELETE( m_ppGambleBoomEffect );
	}

	NTL_ARRAY_DELETE( m_pSlotBoomEffectEnd );
	NTL_ARRAY_DELETE( m_pbIsGambleEffect );
	
	if( m_ppstbStackNum )
	{
		for( RwInt32 i = 0 ; i < m_nNumSlot ; ++i )
		{
			DestroyStackNumber( i );
		}
		NTL_ARRAY_DELETE( m_ppstbStackNum );
	}

	NTL_DELETE( m_pPBox );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CBagBaseGui::Update( RwReal fElapsed )
{
	CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );
	if( !pBag )
		return;

	m_pPBox->Update( fElapsed );

	for( RwInt32 i = 0 ; i < m_nNumSlot ; ++i )
	{
		if( m_pCoolTimeEffect[i].IsWork() )
		{
			RwReal fCurrentTime;
			CNtlSobItem* pItem = pBag->GetChildItem( i );

			fCurrentTime = pItem->GetIcon()->GetCoolingTime();

			m_pCoolTimeEffect[i].Update( fCurrentTime );
		}

		// Flash가 연출중이라면 Update를 시켜준다.
		if( m_ppGambleBoomEffect[i]->IsPlayMovie() )
		{
			m_ppGambleBoomEffect[i]->Update( fElapsed );
		}
	}
}

VOID CBagBaseGui::SetBagSerial( SERIAL_HANDLE hSerial )
{
	m_hBagSerial = hSerial;
	SetBagName();
}

VOID CBagBaseGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );

		if( pUpdate->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			return;

		if( pUpdate->uiUpdateType & EVENT_AIUT_ITEM )
		{
			UpdateData();
		}
	}
	else if( msg.Id == g_EventCalcPopupResult )
	{
		SDboEventCalcPopupResult* pData = reinterpret_cast<SDboEventCalcPopupResult*>( msg.pData );

		if( pData->nSrcPlace == PLACE_BAG )
		{
			if( pData->uiValue == 0 )
				return;

			CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pData->uiSerial ) );
			if( pItem )
			{
				if( pItem->GetParentItemSerial() != m_hBagSerial )
					return;

				CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
				if( GetIconMoveManager()->IconMovePickUp( pItem->GetSerialID(), PLACE_BAG, pItem->GetItemSlotIdx(), pData->uiValue, pItem->GetIcon()->GetImage() ) )
					ShowPickedUp( TRUE, pItem->GetItemSlotIdx() );
			}				
		}		
	}
	else if( msg.Id == g_EventItemIdentifyEffect )
	{
		SDboEventItemIdentifyEffect* pData = reinterpret_cast< SDboEventItemIdentifyEffect* >( msg.pData );

		if( pData->bHandle )
		{
			CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );

			// Handle 제어
			for( RwInt32 i = 0; i < m_nNumSlot; ++i )
			{
				if( pData->hItemHandle == pBag->GetChildSerial( i ) )
				{	
					m_pPBox->SetPosition( m_prtSlots[i].left + (m_prtSlots[i].GetWidth()/2),
						m_prtSlots[i].top + (m_prtSlots[i].GetHeight()/2));
					m_pPBox->Play( FALSE );
					
					return;
				}
			}
		}
		else
		{
			// Place and Pos 제어
			RwUInt8 byBagIdx = Logic_ConvertContainerTypeToBagIdx( pData->byPlace );

			CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
			SERIAL_HANDLE hBag = pInventory->GetBagItem(byBagIdx);

			// not this bag
			if( hBag != m_hBagSerial )
				return;

			CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hBag ) );
			
			SERIAL_HANDLE hItem = pBag->GetChildSerial( pData->byPos );
			
			for( RwInt32 i = 0; i < m_nNumSlot; ++i )
			{
				if( hItem == pBag->GetChildSerial( i ) )
				{	
			/*		CRectangle rect = m_pThis->GetScreenRect();
					int nMoveX = m_prtSlots[i].top + rect.top;
					int nMoveY = m_prtSlots[i].left + rect.left;*/

					m_pPBox->SetPosition( m_prtSlots[i].left + (m_prtSlots[i].GetWidth()/2),
						m_prtSlots[i].top + (m_prtSlots[i].GetHeight()/2));
					m_pPBox->Play( FALSE );

					return;
				}
			}
		}
	}
	//else if( msg.Id == g_EventIconPopupResult )
	//{
	//	SDboEventIconPopupResult* pData = reinterpret_cast<SDboEventIconPopupResult*>( msg.pData );
	//	if( pData->nSrcPlace != PLACE_BAG )
	//		return;
	//	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pData->uiSerial ) );
	//	if( pItem->GetParentItemSerial() != m_hBagSerial )
	//		return;		

	//	if( pData->nWorkId == PMW_PICKUP )
	//	{
	//		if( IsEnableBagChildDragDrop( pData->nSrcSlotIdx ) )
	//		{
	//			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
	//			if( GetIconMoveManager()->IconMovePickUp( pData->uiSerial, PLACE_BAG, pData->nSrcSlotIdx, pItemAttr->GetStackNum(), pItem->GetIcon()->GetImage() ) )
	//				ShowPickedUp( TRUE, pData->nSrcSlotIdx );
	//		} 
	//	}
	//	else if( pData->nWorkId == PMW_EQUIP )
	//	{
	//		Logic_ItemDirectEquipProc( pData->uiSerial, PLACE_BAG, (RwUInt8)pData->nSrcSlotIdx );
	//	}
	//	else if( pData->nWorkId == PMW_USE )
	//	{
	//		Logic_UseProc( pData->uiSerial );
	//	}
	//	else if( pData->nWorkId == PMW_VIEW )
	//	{
	//		// 미정

	//	}
	//	else if( pData->nWorkId == PMW_SELL )
	//	{
	//		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
	//		CDboEventGenerator::ShopEvent(TRM_REG_ITEM_BY_BAG, pData->uiSerial, 0, NULL,
	//										pData->nSrcPlace, pData->nSrcSlotIdx, pItemAttr->GetStackNum() );
	//	}
	//	else
	//	{
	//		NTL_ASSERTFAIL( "CBagBaseGui::HandleEvents : Invalid Popup Work id" );
	//	}
	//}
	else if( msg.Id == g_EventPickedUpHide )
	{
		RwInt32 nPlace = (RwInt32)msg.pData;
		if( nPlace != PLACE_BAG )
			return;

		ShowPickedUp( FALSE );
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>( msg.pData );

		if( pData->iType == DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG ||
			pData->iType == DIALOGEVENT_BEGIN_UPGRADE_ITEM_IN_BAG ||
			pData->iType == DIALOGEVENT_BEGIN_ITEM_IN_OPTIONRECONSTRUCTION)
		{
			if( pData->iUserData != m_cBagPos )	
				return;
			
			CDboEventGenerator::EnableItemIcon( FALSE, PLACE_BAG, pData->iUserData2, m_cBagPos );			
		}
		else if( pData->iType == DIALOGEVENT_END_TRADING_ITEM_IN_BAG ||
				 pData->iType == DIALOGEVENT_END_UPGRADE_ITEM_IN_BAG ||
				 pData->iType == DIALOGEVENT_END_ITEM_IN_OPTIONRECONSTRUCTION)
		{
			if( pData->iUserData != m_cBagPos )	
				return;
			
			CDboEventGenerator::EnableItemIcon( TRUE, PLACE_BAG, pData->iUserData2, m_cBagPos );			
		}
	}
	else if( msg.Id == g_EventSobCooling )
	{
		SNtlEventSobCooling* pData = reinterpret_cast<SNtlEventSobCooling*>( msg.pData );

		CoolingEffectProc( pData->hSerialId, pData->bStartOrEnd );
	}
	else if( msg.Id == g_EventEnableItemIcon )
	{
		SDboEventEnableItemIcon* pData = reinterpret_cast<SDboEventEnableItemIcon*>( msg.pData );

		if( pData->ePlace != PLACE_BAG )
			return;

		if( pData->nPlaceIdx != m_cBagPos )
			return;

		ShowDisableSlot( !pData->bEnable, pData->uiSlotIdx );
	}
	else if( msg.Id == g_EventCapsuleLockItem )
	{
		// 현재 가방의 지정된 Slot에 캡슐 Lock을 걸고 유효한 아이템의 Serial로 가방이 아닌 공간에 있는 아이콘들을 제어한다.

		SDboEventCapsuleLockItem* pData = reinterpret_cast<SDboEventCapsuleLockItem*>( msg.pData );

		if( pData->ePlace != PLACE_BAG )
			return;

		if( pData->bEnable )
		{
			if( pData->nPlaceIdx != m_cBagPos )
				return;

			CapsuleLockSlot( TRUE, pData->uiSlotidx );
		}
		else
		{
			CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );

			CapsuleLockSlot( FALSE, -1 );
		}
	}
	else if( msg.Id == g_EventGambleUseEffect )
	{
		SDboEventGambleUseEffect* pData = reinterpret_cast<SDboEventGambleUseEffect*>(msg.pData);

		// Gamble Effect start
		BoomEffectProc( pData->hCreateSerial, pData->szEffectFile );
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		ClearInventory();
	}
	else if( msg.Id == g_EventResize )
	{
		Logic_LocateDialog_in_CleintRect(this);
	}
}

VOID CBagBaseGui::ClearInventory(VOID)
{
	for( RwInt32 i = 0 ; i < m_nNumSlot ; ++i )
	{
		ClearInventorySlot( i );
	}
}

VOID CBagBaseGui::ClearInventorySlot( RwInt32 nSlotIdx )
{
	NTL_ASSERT( nSlotIdx >= 0 && nSlotIdx < m_nNumSlot, "CBagBaseGui::ClearInventorySlot : Invalid SlotIndex" );

	m_psurIcons[nSlotIdx].Show( FALSE );
	DestroyStackNumber( nSlotIdx );
	m_pCoolTimeEffect[nSlotIdx].EndProc();
	ShowDisableSlot( FALSE, nSlotIdx );	
	FritzEffect( FRITZ_NONE, nSlotIdx );
}

RwInt32 CBagBaseGui::GetChildSlotIdx( RwInt32 nX, RwInt32 nY )
{
	for( RwInt32 i = 0 ; i < m_nNumSlot ; ++i )
	{
		if( m_prtSlots[i].PtInRect( CPos( nX, nY ) ) )
			return i;
	}

	return -1;
}

CRectangle CBagBaseGui::GetChildSlotRect( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 || nSlotIdx >= m_nNumSlot )
		return CRectangle( 0, 0, 0, 0 );

	return m_prtSlots[nSlotIdx];
}

RwBool CBagBaseGui::IsEnableBagChildPickUp( RwInt32 nSlotIdx )
{
	// 1. Does the item exist ?
	// 2. Is it possible to move?
	// 3. Disabled.

	if( nSlotIdx < 0 )
		return FALSE;

	if( !GetIconMoveManager()->IsEnable() )
		return FALSE;

	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );
	CNtlSobItem* pChildItem = pSobItem->GetChildItem( nSlotIdx );

	if( !pChildItem )
		return FALSE;

	CNtlSobIcon* pChildIcon = pChildItem->GetIcon();
	if( !pChildIcon->IsDragAndDropPossible() )
		return FALSE;

	if( IsDisableSlot( nSlotIdx ) )
		return FALSE;

	return TRUE;
}

RwBool CBagBaseGui::IsEnableBagChildPutDown( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 )
		return FALSE;

	if( IsDisableSlot( nSlotIdx ) )
		return FALSE;

	return TRUE;
}

//RwBool CBagBaseGui::IsEnableBagChildPopup( RwInt32 nSlotIdx )
//{
//	if( nSlotIdx < 0 )
//		return FALSE;
//
//	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );
//	CNtlSobItem* pChildItem = pSobItem->GetChildItem( nSlotIdx );
//
//	if( !pChildItem )
//		return FALSE;
//
//	if( m_psurDisable[nSlotIdx].IsShow() )
//		return FALSE;
//
//	return TRUE;
//}

VOID CBagBaseGui::UpdateData(VOID)
{
	CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );	
	if( !pBag )
		return;
	
	for( RwInt32 i = 0 ; i < m_nNumSlot ; ++i )
	{
		CNtlSobItem* pItem = pBag->GetChildItem( i );
		if( pItem )
		{
			m_psurIcons[i].SetTexture( (gui::CTexture*)pItem->GetIcon()->GetImage() );
			m_psurIcons[i].Show( TRUE );

			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
			RwInt32 nStackNumber = pItemAttr->GetStackNum();
			if( pItem->IsStackable() && nStackNumber > 0 )
			{
				if( m_ppstbStackNum[i] )
				{
					m_ppstbStackNum[i]->SetText( nStackNumber );
				}
				else
				{
					CreateStackNumber( i, nStackNumber );
				}
			}
			else
			{
				if( m_ppstbStackNum[i] )
				{
					DestroyStackNumber( i );
				}				
			}

			CNtlSobIcon* pIcon = reinterpret_cast<CNtlSobIcon*>( pItem->GetIcon() );
			if( pIcon->GetIconState() == CNtlSobIcon::ICON_STATE_COOLING )
				m_pCoolTimeEffect[i].StartProc( pIcon->GetMaxCoolingTime() );
			else
				m_pCoolTimeEffect[i].EndProc();

			if( pItemAttr->IsExpired() || pItemAttr->GetDur() == 0 )
				FritzEffect( FRITZ_ABSOLUTE, i );
			else if( (RwReal)pItemAttr->GetDur() / (RwReal)pItemAttr->GetMaxDur() <= ITEM_DURATION_WARNING )
				FritzEffect( FRITZ_HALF, i );
			else
				FritzEffect( FRITZ_NONE, i );			
		}
		else
		{
			ClearInventorySlot( i );
		}
	}

	if( m_nMouseOnIndex >= 0 && GetInfoWndManager()->GetRequestGui() == DIALOG_CAPSULE_1 + GetBagPos() )
	{
		CNtlSobItem* pItem = pBag->GetChildItem( m_nMouseOnIndex );
		if( !pItem )
		{
			GetInfoWndManager()->ShowInfoWindow( FALSE );
			return;
		}
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
		CRectangle rtScreen = m_pThis->GetScreenRect();		
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM, m_prtSlots[m_nMouseOnIndex].left + rtScreen.left, m_prtSlots[m_nMouseOnIndex].top + rtScreen.top, pItem, DIALOG_CAPSULE_1 + GetBagPos() );
	}
}

RwInt32	CBagBaseGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		if( m_bCEnable )
		{
			RaiseTop();
			Show( true );
			UpdateData();			
			Logic_SendTutorialCondition(ETL_CONDITION_TYPE_CAPSULEKIT_OPEN);
			CDboEventGenerator::OpenBagGui();
		}
	}
	else
	{
		if( GetInfoWndManager()->GetRequestGui() == DIALOG_CAPSULE_1 + GetBagPos() )
			GetInfoWndManager()->ShowInfoWindow( FALSE );

		if( GetPopupManager()->IsVisiblePopupGui( PLACE_BAG ) )
			GetPopupManager()->AllPopupClose();

		if (GetDialogManager()->IsMode(DIALOGMODE_ITEM_DISASSEMBLE))
			GetDialogManager()->OffMode();

		if (GetDialogManager()->IsMode(DIALOGMODE_ITEM_BEAD))
			GetDialogManager()->OffMode();

		Show( false );
		Logic_SendTutorialCondition(ETL_CONDITION_TYPE_CAPSULEKIT_CLOSE);
	}

	GetDialogManager()->LocateBag( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );

	return 1;
}

VOID CBagBaseGui::SetBagEnable( RwBool bEnable )
{
	m_bCEnable = bEnable;

	if( IsShow() )
		GetDialogManager()->CloseDialog( DIALOG_CAPSULE_1 + GetBagPos() );
}

RwBool CBagBaseGui::CreateStackNumber( RwInt32 nSlotIdx, RwInt32 nValue )
{
	NTL_FUNCTION( "CBagBaseGui::CreateStackNumber" );

	CRectangle rect;
	rect.SetRect( m_prtSlots[nSlotIdx].left, m_prtSlots[nSlotIdx].bottom - DBOGUI_STACKNUM_HEIGHT,
				  m_prtSlots[nSlotIdx].left + DBOGUI_STACKNUM_WIDTH, m_prtSlots[nSlotIdx].bottom );
	m_ppstbStackNum[nSlotIdx] = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN );
	if( !m_ppstbStackNum[nSlotIdx] )
		NTL_RETURN( FALSE );

	m_ppstbStackNum[nSlotIdx]->CreateFontStd( DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR );
	m_ppstbStackNum[nSlotIdx]->SetEffectMode( DBOGUI_STACKNUM_FONTEFFECTMODE );
	m_ppstbStackNum[nSlotIdx]->SetText( nValue );
	m_ppstbStackNum[nSlotIdx]->Enable( false );

	NTL_RETURN( TRUE );
}

VOID CBagBaseGui::DestroyStackNumber( RwInt32 nSlotIdx )
{
	NTL_DELETE( m_ppstbStackNum[nSlotIdx] );
}

VOID CBagBaseGui::ClickEffect( RwBool bPush, RwInt32 nSlotIdx /* = -1  */)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	if( bPush )
	{
		m_psurIcons[nSlotIdx].SetRect( rtScreen.left + m_prtSlots[nSlotIdx].left + ICONPUSH_SIZEDIFF, rtScreen.top + m_prtSlots[nSlotIdx].top + ICONPUSH_SIZEDIFF,
									   rtScreen.left + m_prtSlots[nSlotIdx].right - ICONPUSH_SIZEDIFF, rtScreen.top + m_prtSlots[nSlotIdx].bottom - ICONPUSH_SIZEDIFF );
	}
	else if( m_nPushDownIndex >= 0 )
	{
		m_psurIcons[m_nPushDownIndex].SetRect( rtScreen.left + m_prtSlots[m_nPushDownIndex].left, rtScreen.top + m_prtSlots[m_nPushDownIndex].top,
											   rtScreen.left + m_prtSlots[m_nPushDownIndex].right, rtScreen.top + m_prtSlots[m_nPushDownIndex].bottom );
	}	

	m_nPushDownIndex = nSlotIdx;
}

VOID CBagBaseGui::FritzEffect( eFRITZEFFECT eFritzState, RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 || nSlotIdx >= m_nNumSlot )
		return;

	switch( eFritzState )
	{
	case FRITZ_NONE:
		m_psurFritzSlot[nSlotIdx].Show( FALSE );
		return;
	case FRITZ_HALF:
		m_psurFritzSlot[nSlotIdx].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfHalfFritzSlotEffect" ) );
		break;
	case FRITZ_ABSOLUTE:
		m_psurFritzSlot[nSlotIdx].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfFritzSlotEffect" ) );
		break;
	}

	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_psurFritzSlot[nSlotIdx].SetRect( rtScreen.left + m_prtSlots[nSlotIdx].left, rtScreen.top + m_prtSlots[nSlotIdx].top,
									   rtScreen.left + m_prtSlots[nSlotIdx].right, rtScreen.top + m_prtSlots[nSlotIdx].bottom );

	m_psurFritzSlot[nSlotIdx].Show( TRUE );
}

VOID CBagBaseGui::ShowPickedUp( RwBool bShow, RwInt32 nSlotIdx /* = 0 */)
{
	if( bShow )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();

		m_nShowPickedUp = nSlotIdx;
		m_surPickedUp.SetPosition( m_prtSlots[nSlotIdx].left + rtScreen.left, m_prtSlots[nSlotIdx].top + rtScreen.top );
	}
	else
	{
		m_nShowPickedUp = -1;
	}
}

VOID CBagBaseGui::ShowDisableSlot( RwBool bShow, RwInt32 nSlotIdx )
{
	m_psurDisableSlot[nSlotIdx].Show( bShow );
}

VOID CBagBaseGui::CapsuleLockSlot( RwBool bLock, RwInt32 nSlotIdx )
{
	if( bLock )
	{
		CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );

		SERIAL_HANDLE hItem = pBag->GetChildSerial( nSlotIdx );
		if( hItem == INVALID_SERIAL_ID )
			return;

		CDboEventGenerator::CapsuleLockItemWithoutBag( TRUE, hItem );

		m_nCapsuleLockSlotIdx = nSlotIdx;
		m_psurDisableSlot[nSlotIdx].Show( TRUE );
	}
	else
	{
		if( m_nCapsuleLockSlotIdx != -1 )
		{
			CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );

			SERIAL_HANDLE hItem = pBag->GetChildSerial( m_nCapsuleLockSlotIdx );
			if( hItem == INVALID_SERIAL_ID )
				return;

			CDboEventGenerator::CapsuleLockItemWithoutBag( TRUE, hItem );

			m_psurDisableSlot[m_nCapsuleLockSlotIdx].Show( FALSE );
			m_nCapsuleLockSlotIdx = -1;

			CDboEventGenerator::CapsuleLockItemWithoutBag( FALSE, hItem );
		}
	}	
}

RwBool CBagBaseGui::IsDisableSlot( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 || nSlotIdx >= m_nNumSlot ) // by daneos: changed && to ||
		return TRUE;

	return m_psurDisableSlot[nSlotIdx].IsShow() ? TRUE : FALSE;
}

VOID CBagBaseGui::CoolingEffectProc( SERIAL_HANDLE hSerial, BOOL bStart )
{
	CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );

	for( RwInt32 i = 0 ;i < m_nNumSlot ; ++i )
	{
		if( hSerial == pBag->GetChildSerial( i ) )
		{
			if( bStart )
			{
				CNtlSobItem* pItem = pBag->GetChildItem( i );
				CNtlSobIcon* pIcon = reinterpret_cast<CNtlSobIcon*>( pItem->GetIcon() );
				m_pCoolTimeEffect[i].StartProc( pIcon->GetMaxCoolingTime() );
			}
			else
			{
				m_pCoolTimeEffect[i].EndProc();
			}

			return;
		}
	}
}

VOID CBagBaseGui::RightButtonProc( RwInt32 nSlotIdx )
{
	CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );
	if( !pBag )
		return;

	CNtlSobItem* pItem = pBag->GetChildItem( nSlotIdx );
	
	if( GetIconMoveManager()->IsActive() )
		return;

	if( !pItem )
		return;

	// Do not process if the Gamble effect is in production.
	if( m_pbIsGambleEffect[nSlotIdx] )
		return;
	
	SERIAL_HANDLE hItemHandle = pItem->GetSerialID();
	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
	RwBool bNeedToIdentifyItem = pItemAttr->IsNeedToIdentify();
	BYTE byRestrictState = pItemAttr->GetRestrictState();

	// Note: Unidentified items do not know the item table on the client
	sITEM_TBLDAT* pITEM_TBLDAT = pItemAttr->GetItemTbl();

	// Items that have expired are displayed with an extension message box.
	if( pItemAttr->IsExpired() && pITEM_TBLDAT->bIsCanRenewal)
	{
		CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

		sMsgBoxData data;
		data.sItemInfo.hSerial		= hItemHandle;
		data.sItemInfo.ePlace		= PLACE_BAG;
		data.sItemInfo.nPlaceIdx	= pItem->GetParentItemSlotIdx();
		data.sItemInfo.nSlotIdx		= pItem->GetItemSlotIdx();
		
		CDboEventGenerator::EnableItemIcon( FALSE, data.sItemInfo.ePlace, data.sItemInfo.nSlotIdx, data.sItemInfo.nPlaceIdx );
		GetAlarmManager()->FormattedAlarmMessage( "DST_COMMERCIAL_MB_USE_TERM_LIMETED_ITEM_MSG", FALSE, &data, pItemTextTable->GetText( pItemAttr->GetItemTbl()->Name ).c_str() );
	}		
	// If the warehouse is open,
	else if( GetDialogManager()->IsOpenDialog(DIALOG_WAREHOUSEBAR) )
	{
		RwUInt8 byWarehouseIndex, bySlot_of_warehouse;
		SERIAL_HANDLE hWarehouse;

		Logic_GetFirstEmptySlot_of_Warehouse(byWarehouseIndex, hWarehouse, bySlot_of_warehouse);

		if( byWarehouseIndex != INVALID_BYTE )
		{	
			Logic_ItemMoveSubProcBagChildToWarehouse( hItemHandle, nSlotIdx, hWarehouse, bySlot_of_warehouse, pItemAttr->GetStackNum() );
		}
		else
		{
			// Warehouse is full
			GetAlarmManager()->AlarmMessage( "DST_WAREHOUSE_FULL" );
		}
	}
	// If the guild warehouse is open,
	else if( GetDialogManager()->IsOpenDialog(DIALOG_GUILD_WAREHOUSEBAR) )
	{
		RwUInt8 byGuildWarehouseIndex, bySlot_of_GuildWarehouse;

		Logic_GetFirstEmptySlot_of_GuildWarehouse(byGuildWarehouseIndex, bySlot_of_GuildWarehouse);

		if( bySlot_of_GuildWarehouse != INVALID_BYTE )
		{	
			Logic_ItemMoveSubProcBagChildToGuildWarehouse( hItemHandle, nSlotIdx, byGuildWarehouseIndex, bySlot_of_GuildWarehouse, pItemAttr->GetStackNum() );
		}
		else
		{
			// 창고가 가득 찼습니다
			GetAlarmManager()->AlarmMessage( "DST_WAREHOUSE_FULL" );
		}
	}
	else if (GetDialogManager()->IsOpenDialog(DIALOG_PRIVATESHOP)
		//&& GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_PRIVATESHOP) // No state conversion yet.
		//&& GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_PRIVATESHOP)->GetState() == PRIVATESHOP_CLOSE // No detailed state
		)		// Individual stores can also register unidentified items.
	{		
		CDboEventGenerator::RegPrivateShopItem(PLACE_BAG, m_cBagPos, nSlotIdx);
	}
	// If Mail window is open, go directly to Mail window
	else if( GetDialogManager()->IsOpenDialog(DIALOG_MAILSYSTEM) )
	{
		CDboEventGenerator::RegMailAttatchItem( pItem->GetSerialID(), PLACE_BAG, m_cBagPos, nSlotIdx );
	}
	else if( pITEM_TBLDAT && ITEM_TYPE_HOIPOIROCK == pITEM_TBLDAT->byItem_Type && GetDialogManager()->IsOpenDialog( DIALOG_DOJO_UPGRADE ) )
	{
		CDboEventGenerator::DirectMoveIcon( hItemHandle, PLACE_BAG, PLACE_DOJO_UPRAGE, nSlotIdx, pItemAttr->GetStackNum() );
	}
	// If the auction house is open
	else if (GetDialogManager()->IsOpenDialog(DIALOG_AUCTION_HOUSE))
	{
		CDboEventGenerator::RegAuctionHouseItem(pItem->GetSerialID(), PLACE_BAG, m_cBagPos, nSlotIdx);
	}
	// If you are in a trade, you can trade-cart ...
	else if (GetDialogManager()->IsOpenDialog(DIALOG_TRADECART))
	{
		CNtlWorldConceptTrade* pTradeConcept = reinterpret_cast<CNtlWorldConceptTrade*>(GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_TRADE));
		NTL_ASSERT(pTradeConcept, "CBagBaseGui::RightButtonProc, Not exist trade concept");

		GetDboGlobal()->GetGamePacketGenerator()->SendTradeAddReq(pTradeConcept->GetOtherPCHandler(),
			hItemHandle,
			pItemAttr->GetStackNum());
	}
	// Other unidentified items are not applicable.
	else if (bNeedToIdentifyItem)
	{
		return;
	}
	// If the store is open, unconditional store line
	else if (GetDialogManager()->IsOpenDialog(DIALOG_NPCSHOP))
	{
		CDboEventGenerator::ShopEvent(TRM_REG_ITEM_BY_BAG, hItemHandle, 0, NULL, PLACE_BAG, nSlotIdx, pItemAttr->GetStackNum());
	}
	else if (GetDialogManager()->IsOpenDialog(DIALOG_ITEM_EXCHANGE_SHOP))
	{
		CDboEventGenerator::ShopEvent(TRM_REG_ITEM_BY_BAG, hItemHandle, 0, NULL, PLACE_BAG, nSlotIdx, pItemAttr->GetStackNum());
	}
	else if( pITEM_TBLDAT && ITEM_TYPE_DOGI == pITEM_TBLDAT->byItem_Type && GetDialogManager()->IsOpenDialog( DIALOG_DOGI ) )
	{
		CDboEventGenerator::DirectMoveIcon( hItemHandle, PLACE_BAG, PLACE_DOGI, nSlotIdx, pItemAttr->GetStackNum() );
	}
	else if (pITEM_TBLDAT && byRestrictState == ITEM_RESTRICT_STATE_TYPE_SEAL)
	{
		// open dialog
		sMsgBoxData data;
		data.sItemInfo.hSerial = hItemHandle;
		data.sItemInfo.ePlace = PLACE_BAG;
		data.sItemInfo.nPlaceIdx = Logic_ConvertBagIdxToContainerType(GetBagPos());
		data.sItemInfo.nSlotIdx = nSlotIdx;

		GetAlarmManager()->AlarmMessage("DST_SEALING_EXTRACT_CONFIRM_BOX", FALSE, &data);
	}
	else if (pITEM_TBLDAT && pITEM_TBLDAT->byItem_Type == ITEM_TYPE_BEAD)
	{
		GetDialogManager()->OffMode();
		GetDialogManager()->OnMode(DIALOGMODE_ITEM_BEAD);
		GetDialogManager()->SetClickedItem(pItem);

		CDboEventGenerator::EnableItemIcon(FALSE, PLACE_BAG, pItem->GetItemSlotIdx(), pItem->GetParentItemSlotIdx());
	}
	// If the upgrade window is open, navigate accordingly
	else if( GetDialogManager()->IsOpenDialog( DIALOG_ITEMUPGRADE ) )
	{
		CDboEventGenerator::RegistItemUpgrade( pItem->GetSerialID(), PLACE_BAG, nSlotIdx );
	}
	// If the upgrade window is open, navigate accordingly
	else if (GetDialogManager()->IsOpenDialog(DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE))
	{
		CDboEventGenerator::RegisterItemChangeBattleAttribute(pItem->GetSerialID(), PLACE_BAG, nSlotIdx);
	}
	else if( GetDialogManager()->IsOpenDialog(DIALOG_VEHICLE) && pITEM_TBLDAT && pITEM_TBLDAT->byItem_Type == ITEM_TYPE_FUEL)
	{
		// Fuel registration of vehicle
		CNtlSLEventGenerator::SobVehicleEngine(Logic_GetAvatarHandle(), VEHICLE_ENGINE_ASK_REG_FROM_BAG, hItemHandle);
	}
	// If the seal bind item is open
	else if (GetDialogManager()->IsOpenDialog(DIALOG_BIND_SEALING))
	{
		CDboEventGenerator::RegBindSealingItem(pItem->GetSerialID(), PLACE_BAG, m_cBagPos, nSlotIdx);
	}
	// If the CRAFTING is open
	else if (GetDialogManager()->IsOpenDialog(DIALOG_HOIPOIMIX_CRAFT))
	{
		CDboEventGenerator::RegHoiPoiMixMaterialItem(pItem->GetSerialID(), PLACE_BAG, m_cBagPos, nSlotIdx);
	}
	// If the ccbd board is open
	else if (GetDialogManager()->IsOpenDialog(DIALOG_CCBD_BOARD))
	{
		CDboEventGenerator::RegCCBDCouponItem(pItem->GetSerialID(), PLACE_BAG, m_cBagPos, nSlotIdx);
	}
	// If the item option reconstruction is open
	else if (GetDialogManager()->IsOpenDialog(DIALOG_ITEM_OPTIONRESTRUCTURE) ||
			(pITEM_TBLDAT && pITEM_TBLDAT->byItem_Type == ITEM_TYPE_CHANGE_OPTION_KIT))
	{
		CDboEventGenerator::RegOptionReconstructItem(pItem->GetSerialID(), PLACE_BAG, m_cBagPos, nSlotIdx);
	}
	// quick teleport gui
	else if (GetDialogManager()->IsOpenDialog(DIALOG_QUICK_TELEPORT) == FALSE && (pITEM_TBLDAT && pITEM_TBLDAT->byItem_Type == ITEM_TYPE_QUICK_TELEPORT))
	{	
		CDboEventGenerator::EventOpenQuickTeleport(pItem->GetSerialID(), PLACE_BAG, m_cBagPos, nSlotIdx);
	}
	// If the item is attached, the bag item is placed in the bag slot, and the item attached to the scout is attached to the scout.
	else if( pItemAttr->IsEquipItem() || pItemAttr->IsBagItem() )
	{
		Logic_ItemDirectEquipProc(hItemHandle, PLACE_BAG, (RwUInt8)nSlotIdx);
	}
	// Available items are used
	else if( pItem->IsUsePossible() )
	{
		// If it is ITEM_TYPE_CAPSULE among the available items, it locks.
		// Unlock ItemUseRes among Locked Items, and release Lock if it fails.
		// If the lock is released, create it according to the situation in each contents.
		// This lock does not synchronize between client and server.
		//
		// HOIPOIMIX MACHINE = Object is destroyed
		// VEHICLE = Vechcle Gui If the object is terminated
		// by Kell (2009.2.9)
		Logic_UseProc( pItem->GetSerialID() );
	}	
}

VOID CBagBaseGui::SetBagName(VOID)
{
	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );

	if( pSobItem )
	{
		CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
		CTextTable* pItemTextTbl = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

		m_pstbTitle->SetText( pItemTextTbl->GetText( pSobItemAttr->GetItemTbl()->Name ).c_str() );
	}
	else
		m_pstbTitle->SetText( GetDisplayStringManager()->GetString( "DST_BAG_TITLE" ) );
}

/**
* \brief 겜블 아이템 연출 시작
* \param hOldSerial	(RwUint32) 캡슐 아이템의 핸들
* \param hSerial	(RwUInt32) 캡슐에서 나온 아이템의 핸들
*/
VOID CBagBaseGui::BoomEffectProc( RwUInt32 hCreateSerial, RwChar* pcEffectFile )
{
	CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );

	for( RwInt32 i = 0; i < m_nNumSlot; ++i )
	{
		if( hCreateSerial == pBag->GetChildSerial( i ) )
		{	
			if( m_ppGambleBoomEffect[i]->Load( pcEffectFile ) == FALSE )
			{
				NTL_ASSERTFAIL( "CBagBaseGui::BoomEffectProc - not exist flash file" << pcEffectFile );
				return;
			}

			m_ppGambleBoomEffect[i]->PlayMovie(TRUE);
			m_ppGambleBoomEffect[i]->RestartMovie();
			m_ppGambleBoomEffect[i]->Show(true);
			m_pbIsGambleEffect[i] = TRUE;
			
			return;
		}
	}
}

VOID CBagBaseGui::OnMovieEnd( gui::CComponent* pComponent )
{
	gui::CFlash* pFlash = reinterpret_cast< gui::CFlash*>( pComponent );

	for( RwInt32 i = 0; i < m_nNumSlot; ++i )
	{
		// 일치한다.
		if( m_ppGambleBoomEffect[i]->GetScreenRect() == pFlash->GetScreenRect() )
		{
			// Item Create Effect
			CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );
			CDboEventGenerator::ItemCreatedEffect( pBag->GetChildSerial(i), GetBagPos(), i);

			// Flash Unload and Hide
			pFlash->Show(false);
			m_pbIsGambleEffect[i] = FALSE;
			m_ppGambleBoomEffect[i]->Unload();	

			return;
		}
	}

	// 여기까지 알고리즘이 온다면 잘못된 정보의 플래쉬 객체가 있다는 뜻.
	NTL_ASSERTFAIL( "CBagBaseGui::OnMovieEnd - Invalid flash component" );
}

VOID CBagBaseGui::MouseDown( const CKey& key )
{
	if( GetIconMoveManager()->IsActive() )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_CPASULE_KIT_WND_SLOTS_DROP_DOWN ) )
			return;
	}
	else
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_CPASULE_KIT_WND_SLOTS_PICK_UP ) )
			return;
	}	

	RwInt32 nClickIdx = GetChildSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );
	if( nClickIdx >= 0 && !GetIconMoveManager()->IsActive() )
		ClickEffect( TRUE, nClickIdx );

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		// 1. Select Src or Dest.
		if( !GetIconMoveManager()->IsActive() )
		{
			// 2. Item validation.
			if( IsEnableBagChildPickUp( nClickIdx ) )
			{
				m_nLSelectedSlotIdx = nClickIdx;
			}
		}
		else
		{
			if( IsEnableBagChildPutDown( nClickIdx ) )
			{
				m_nLSelectedSlotIdx = nClickIdx;
			}			
		}
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		// 1. 아이콘 픽업상태인가 아닌가
		if( !GetIconMoveManager()->IsActive() )
		{
			if( !IsDisableSlot( nClickIdx ) )
			{				
				m_nRSelectedSlotIdx = nClickIdx;
			}
			//2. 아이템 유효성검사.
			//if( IsEnableBagChildPopup( nClickIdx ) )
			//{
			//	m_nRSelectedSlotIdx = nClickIdx;
			//}
		}
	}

	m_pThis->CaptureMouse();

	// 동시에 눌릴때 무효처리.
	if( m_nLSelectedSlotIdx >= 0 && m_nRSelectedSlotIdx >= 0 )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		ClickEffect( FALSE );
		m_pThis->ReleaseMouse();
	}
}

VOID CBagBaseGui::MouseUp( const CKey& key )
{
	// 1. Left버튼인가 Right버튼인가
	// 2. Src선택인가 Dest선택인가.

	RwInt32 nSlotIdx = GetChildSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );
	ClickEffect( FALSE );
	
	m_pThis->ReleaseMouse();

	if( nSlotIdx < 0 || !IsShow() )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		return;
	}

	// 겜블 효과가 사용중이라면 입력을 막는다.
	if( m_pbIsGambleEffect[nSlotIdx] )
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
				GetIconMoveManager()->IconMovePutDown( PLACE_BAG, m_hBagSerial, nSlotIdx ); 
			}
			else
			{
				if( GetDialogManager()->IsMode( DIALOGMODE_ITEM_REPAIR ) )
				{
					Logic_ItemRepairProc( Logic_ConvertBagIdxToContainerType( GetBagPos() ), nSlotIdx );					
				}
				else if( GetDialogManager()->IsMode( DIALOGMODE_ITEM_IDENTIFICATION ) )
				{
					Logic_ItemIdentificationProc( GetBagPos(), nSlotIdx );					
				}
				else if( GetDialogManager()->IsMode( DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION ) )
				{
					Logic_ItemIdentification_in_NPCShop_Proc( GetBagPos(), nSlotIdx);
					
				}
				else if (GetDialogManager()->IsMode(DIALOGMODE_ITEM_DISASSEMBLE))
				{
					CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(m_hBagSerial));
					CNtlSobItem* pItem = pBagItem->GetChildItem(nSlotIdx);
					if (pItem)
					{
						CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

						if (pItemAttr->IsNeedToIdentify() == FALSE && IsSealItem(pItemAttr->GetRestrictState()) == false)
							GetDboGlobal()->GetGamePacketGenerator()->SendItemDisassembleReq(Logic_ConvertBagIdxToContainerType(GetBagPos()), nSlotIdx);
					}
				}
				else if (GetDialogManager()->IsMode(DIALOGMODE_ITEM_BEAD))
				{
					Logic_ItemBeadProc(GetBagPos(), nSlotIdx);

					GetDialogManager()->OffMode();
				}
				else if( key.m_dwVKey & UD_MK_CONTROL )
				{
					CRectangle rtScreen = m_pThis->GetScreenRect();
					CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );
					CNtlSobItem* pItem = pBagItem->GetChildItem( nSlotIdx );
					if( pItem )
					{
						CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
						
						if( pItemAttr->GetStackNum() > 1 )
							CDboEventGenerator::CalcPopupShow( TRUE, pItem->GetSerialID(), PLACE_BAG, rtScreen.left, rtScreen.top, pItemAttr->GetStackNum() );
					}					
				}
				else
				{
					CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );
					CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( pBagItem->GetChildItem( nSlotIdx ) );
					if( pItem )
					{
						CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
						if( GetIconMoveManager()->IconMovePickUp( pItem->GetSerialID(), PLACE_BAG, nSlotIdx, pItemAttr->GetStackNum(), pItem->GetIcon()->GetImage() ) )
							ShowPickedUp( TRUE, nSlotIdx );
					}					
				}
			}
		}
		
		m_nLSelectedSlotIdx = -1;
	}
	else if (key.m_nID == UD_RIGHT_BUTTON && key.m_dwVKey & UD_MK_SHIFT)
	{
		if (nSlotIdx == m_nRSelectedSlotIdx)
		{
			Logic_ItemBeadDestroyProc(GetBagPos(), nSlotIdx);
		}

		m_nRSelectedSlotIdx = -1;
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( nSlotIdx == m_nRSelectedSlotIdx )
		{
			RightButtonProc( nSlotIdx );
			//CRectangle rtScreen = m_pThis->GetScreenRect();
			//CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );
			//SERIAL_HANDLE hSerial = pBagItem->GetChildSerial( nSlotIdx );
			//CDboEventGenerator::IconPopupShow( TRUE, hSerial, PLACE_BAG, nSlotIdx, m_prtSlots[nSlotIdx].left + rtScreen.left, m_prtSlots[nSlotIdx].top + rtScreen.top );
		}

		m_nRSelectedSlotIdx = -1;
	}
}

VOID CBagBaseGui::MouseOut(VOID)
{
	m_surFocusSlot.Show( FALSE );

	if( m_nMouseOnIndex != -1 )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		if( m_nPushDownIndex >= 0 )
			ClickEffect( FALSE, m_nPushDownIndex );
		m_nMouseOnIndex = -1;
	}
}

VOID CBagBaseGui::MouseMove( INT nKey, INT nXPos, INT nYPos )
{
	RwInt32 nMouseOnIdx = GetChildSlotIdx( nXPos, nYPos );

	if( nMouseOnIdx >= 0 )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();

		m_surFocusSlot.SetRect( m_prtSlots[nMouseOnIdx].left + rtScreen.left, m_prtSlots[nMouseOnIdx].top + rtScreen.top,
								m_prtSlots[nMouseOnIdx].right + rtScreen.left, m_prtSlots[nMouseOnIdx].bottom + rtScreen.top );
		m_surFocusSlot.Show( TRUE );

		// 
		if( m_nMouseOnIndex != nMouseOnIdx )
		{
			CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial ) );
			CNtlSobItem* pItem = pBagItem->GetChildItem( nMouseOnIdx );

			if( pItem )
			{
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM,
					m_prtSlots[nMouseOnIdx].left + rtScreen.left, m_prtSlots[nMouseOnIdx].top + rtScreen.top, pItem, DIALOG_CAPSULE_1 + GetBagPos() );

				m_nMouseOnIndex = nMouseOnIdx;
			}

			if( nMouseOnIdx == m_nPushDownIndex )
				ClickEffect( TRUE, nMouseOnIdx );
			else if( m_nPushDownIndex >= 0 )
				ClickEffect( FALSE, m_nPushDownIndex );
		}
	}
	else
	{
		m_surFocusSlot.Show( FALSE );

		if( m_nMouseOnIndex != -1 )
		{
			GetInfoWndManager()->ShowInfoWindow( FALSE );
			if( m_nPushDownIndex >= 0 )
				ClickEffect( FALSE, m_nPushDownIndex );
			m_nMouseOnIndex = -1;
		}
	}
}

VOID CBagBaseGui::Paint(VOID)
{
	// Icon 출력
	for( RwInt32 i = 0 ; i < m_nNumSlot ; ++i )
	{
		m_psurIcons[i].Render();
		m_pCoolTimeEffect[i].Render();
		m_psurFritzSlot[i].Render();
		m_psurDisableSlot[i].Render();
	}

	if( m_nShowPickedUp >= 0 )
	{
		m_surPickedUp.Render();
	}

	m_surFocusSlot.Render();
}

VOID CBagBaseGui::Move( RwInt32 nX, RwInt32 nY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 i = 0 ; i < m_nNumSlot ; ++i )
	{
		m_psurIcons[i].SetPosition( rtScreen.left + m_prtSlots[i].left, rtScreen.top + m_prtSlots[i].top );
		m_psurDisableSlot[i].SetPosition( rtScreen.left + m_prtSlots[i].left, rtScreen.top + m_prtSlots[i].top );
		m_psurFritzSlot[i].SetPosition( rtScreen.left + m_prtSlots[i].left, rtScreen.top + m_prtSlots[i].top );
		m_pCoolTimeEffect[i].SetPosition( rtScreen.left + m_prtSlots[i].left, rtScreen.top + m_prtSlots[i].top );
		m_ppGambleBoomEffect[i]->SetPosition( m_prtSlots[i].left - ((m_ppGambleBoomEffect[i]->GetWidth() - m_prtSlots[i].GetWidth())/2)
			, m_prtSlots[i].top - ((m_ppGambleBoomEffect[i]->GetHeight() - m_prtSlots[i].GetHeight())/2) );
	}

	if( m_nShowPickedUp >= 0 )
		m_surPickedUp.SetPosition( rtScreen.left + m_prtSlots[m_nShowPickedUp].left, rtScreen.top + m_prtSlots[m_nShowPickedUp].top );
}

VOID CBagBaseGui::OnCaptureMouseDown( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_CAPSULE_1 + m_cBagPos, key.m_fX, key.m_fY );
}

VOID CBagBaseGui::ExitButtonClick( gui::CComponent* pComponent )
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_CPASULE_KIT_WND_CLOSE_BTN ) )
		return;

	Logic_PlayGUISound(GSD_SYSTEM_WINDOW_CLOSE);

	GetDialogManager()->SwitchDialog( m_cBagPos + DIALOG_CAPSULE_1 );
}

////////////////////////////////////////////////////////////////////////////////
// BasicBagGui

CBasicBagGui::CBasicBagGui(VOID)
: m_pstbMoney( NULL )
,m_pMoneyIconTexture(NULL)
{
	
}

CBasicBagGui::CBasicBagGui( const RwChar* pName )
: CBagBaseGui( pName )
,m_pstbMoney( NULL )
,m_pMoneyIconTexture(NULL)
{

}

CBasicBagGui::~CBasicBagGui(VOID)
{

}

RwBool CBasicBagGui::Create(VOID)
{
	NTL_FUNCTION( "CBasicBagGui" );

	if( !CNtlPLGui::Create( "", "gui\\BasicBag.srf", "gui\\BasicBag.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pCloseButton = (gui::CButton*)GetComponent( "btnBagExit" );
	m_pbtnPopupMenu = (gui::CButton*)GetComponent("btnPopupMenu");
	m_pstbMoney = (gui::CStaticBox*)GetComponent( "sttZenny" );
	m_pbtnZenny = (gui::CButton*)GetComponent( "btnZenny" );
	m_pbtnDiscard = (gui::CButton*)GetComponent( "btnDiscard" );
	m_ppnlZennyFocus = (gui::CPanel*)GetComponent( "pnlZennyFocus" );
	m_ppnlZennyFocus->Show( false );
	m_pstbMoney->Enable(false);
	
	m_slotMouseDown = m_pThis->SigMouseDown().Connect( this, &CBasicBagGui::OnMouseDown );
	m_slotMouseUp	= m_pThis->SigMouseUp().Connect( this, &CBasicBagGui::OnMouseUp );
	m_slotMouseMove	= m_pThis->SigMouseMove().Connect( this, &CBasicBagGui::OnMouseMove );
	m_slotMouseOut  = m_pThis->SigMouseLeave().Connect( this, &CBasicBagGui::OnMouseOut );
	m_slotPaint		= m_pThis->SigPaint().Connect( this, &CBasicBagGui::OnPaint );
	m_slotMove		= m_pThis->SigMove().Connect( this, &CBasicBagGui::OnMove );
	m_slotClikedExit= m_pCloseButton->SigClicked().Connect( this, &CBasicBagGui::OnExitButtonClick );
	m_slotClickZennyBtn = m_pbtnZenny->SigClicked().Connect( this, &CBasicBagGui::OnClickZennyBtn );
	m_slotClickDiscardBtn = m_pbtnDiscard->SigClicked().Connect( this, &CBasicBagGui::OnClickDiscardBtn );
	m_slotClickPopupMenuBtn = m_pbtnPopupMenu->SigClicked().Connect(this, &CBasicBagGui::OnClickPopupMenuBtn);

	// 제니 아이콘
	m_pMoneyIconTexture = Logic_CreateTexture( MONEYICON_NAME );

	// ToolTIp
	m_pbtnZenny->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( "DST_ZENNY_BTN" ) ) );
	m_pbtnDiscard->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( "DST_DISCARD_BTN" ) ) );
	m_pbtnPopupMenu->SetToolTip(GetDisplayStringManager()->GetString("DST_POPUPMENU_BUTTON_TOOLTIP"));

	m_nNumSlot = 16;
	m_pstbMoney->SetText( 0 );
	if( !CBagBaseGui::Create() )
		NTL_RETURN( FALSE );

	SetSlotRectHardCode();

	LinkMsg( g_EventIconMoveClick );

	NTL_RETURN( TRUE );
}

VOID CBasicBagGui::Destroy(VOID)
{
	NTL_FUNCTION( "CBasicBagGui::Destroy" );

	UnLinkMsg( g_EventIconMoveClick );

	// 가방에서 만들어진 제니 아이콘을 삭제한다
	Logic_DeleteTexture(m_pMoneyIconTexture);
	m_pMoneyIconTexture = NULL;
	
	CBagBaseGui::Destroy();

	NTL_RETURNVOID();
}

VOID CBasicBagGui::UpdateData(VOID)
{
	CBagBaseGui::UpdateData();
	UpdateZenny();	
}

VOID CBasicBagGui::HandleEvents( RWS::CMsg& msg )
{
	CBagBaseGui::HandleEvents( msg );

	if( msg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );

		if( pUpdate->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			return;

		if( pUpdate->uiUpdateType & EVENT_AIUT_ZENNY )
		{
			UpdateZenny();
		}
	}
	else if( msg.Id == g_EventIconMoveClick )
	{
		RwBool bPickUpState = (RwBool)msg.pData;

		if( bPickUpState )
		{
			if( GetIconMoveManager()->IsActiveZenny() )
				m_ppnlZennyFocus->Show( true );
		}
		else
		{
			m_ppnlZennyFocus->Show( false );
		}
	}
	else if( msg.Id == g_EventCalcPopupResult )
	{
		SDboEventCalcPopupResult* pData = reinterpret_cast<SDboEventCalcPopupResult*>( msg.pData );

		if( pData->nSrcPlace == PLACE_SUB_BAG_ZENNY )
		{
			if( pData->uiValue == 0 )
				return;

			GetIconMoveManager()->IconMovePickUp( INVALID_SERIAL_ID, PLACE_SUB_BAG_ZENNY, INVALID_INDEX, pData->uiValue, m_pMoneyIconTexture);
		}		
	}
}

VOID CBasicBagGui::UpdateZenny(VOID)
{
	m_pstbMoney->SetText(Logic_FormatZeni(Logic_GetZenny()) );
}

VOID CBasicBagGui::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 i = 0 ; i < m_nNumSlot ; ++i )
	{
		RwInt32 j = i % 4;
		RwInt32 k = i / 4;
		m_prtSlots[i].left	= j * 40 + 19;
		m_prtSlots[i].top	= k * 40 + 61;
		m_prtSlots[i].right	= m_prtSlots[i].left + 32;
		m_prtSlots[i].bottom= m_prtSlots[i].top + 32;

		RwInt32 nLeft = rtScreen.left + m_prtSlots[i].left;
		RwInt32 nTop = rtScreen.top + m_prtSlots[i].top;
		RwInt32 nRight = rtScreen.left + m_prtSlots[i].right;
		RwInt32 nBottom = rtScreen.top + m_prtSlots[i].bottom;

		m_psurIcons[i].SetRect( nLeft, nTop, nRight, nBottom );
		m_psurDisableSlot[i].SetRect( nLeft, nTop, nRight, nBottom );

		m_pCoolTimeEffect[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );
		m_pCoolTimeEffect[i].SetPosition( nLeft, nTop );		
	}
}

VOID CBasicBagGui::OnMouseDown( const CKey& key )
{
	MouseDown( key );
}

VOID CBasicBagGui::OnMouseUp( const CKey& key )
{
	MouseUp( key );
}

VOID CBasicBagGui::OnMouseOut( gui::CComponent* pComponent )
{
	MouseOut();
}

VOID CBasicBagGui::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	MouseMove( nKey, nXPos, nYPos );
}

VOID CBasicBagGui::OnPaint()
{
	Paint();
}

VOID CBasicBagGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	Move( nX, nY );
}

VOID CBasicBagGui::OnExitButtonClick( gui::CComponent* pComponent )
{
	ExitButtonClick( pComponent );
}

VOID CBasicBagGui::OnClickZennyBtn( gui::CComponent* pComponent )
{
	if( GetIconMoveManager()->IsActive() )
	{
		GetIconMoveManager()->IconMovePutDown( PLACE_SUB_BAG_ZENNY, INVALID_SERIAL_ID, INVALID_DWORD );		
	}
	else
	{
		CRectangle rect = m_pbtnZenny->GetScreenRect();
		CDboEventGenerator::CalcPopupShow( TRUE, INVALID_SERIAL_ID, PLACE_SUB_BAG_ZENNY, rect.left - BAG_CALC_OFFSET_X, rect.top, Logic_GetZenny() );
	}
}

VOID CBasicBagGui::OnClickDiscardBtn( gui::CComponent* pComponent )
{
	Logic_ItemDropConfirmProc();
}

VOID CBasicBagGui::OnClickPopupMenuBtn(gui::CComponent * pComponent)
{
	CRectangle rtRect = m_pThis->GetScreenRect();
	CDboEventGenerator::IconPopupShow(TRUE, INVALID_SERIAL_ID, PLACE_SUB_BAG, 0,
		rtRect.left - 35, rtRect.top - 145);
}

////////////////////////////////////////////////////////////////////////////////
// Bag4SpaceGui

CBag4SpaceGui::CBag4SpaceGui(VOID)
{

}

CBag4SpaceGui::CBag4SpaceGui( const RwChar* pName )
: CBagBaseGui( pName )
{

}

CBag4SpaceGui::~CBag4SpaceGui(VOID)
{

}

RwBool CBag4SpaceGui::Create(VOID)
{
	NTL_FUNCTION( "CBag4SpaceGui" );

	if( !CNtlPLGui::Create( "", "gui\\4Bag.srf", "gui\\4Bag.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pCloseButton = (gui::CButton*)GetComponent( "btnBagExit" );

	m_slotMouseDown = m_pThis->SigMouseDown().Connect( this, &CBag4SpaceGui::OnMouseDown );
	m_slotMouseUp	= m_pThis->SigMouseUp().Connect( this, &CBag4SpaceGui::OnMouseUp );
	m_slotMouseMove	= m_pThis->SigMouseMove().Connect( this, &CBag4SpaceGui::OnMouseMove );
	m_slotMouseOut  = m_pThis->SigMouseLeave().Connect( this, &CBag4SpaceGui::OnMouseOut );
	m_slotPaint		= m_pThis->SigPaint().Connect( this, &CBag4SpaceGui::OnPaint );
	m_slotMove		= m_pThis->SigMove().Connect( this, &CBag4SpaceGui::OnMove );
	m_slotClikedExit= m_pCloseButton->SigClicked().Connect( this, &CBag4SpaceGui::OnExitButtonClick );
	
	m_nNumSlot = 4;
	if( !CBagBaseGui::Create() )
		NTL_RETURN( FALSE );

	SetSlotRectHardCode();

	NTL_RETURN( TRUE );
}

VOID CBag4SpaceGui::Destroy(VOID)
{
	NTL_FUNCTION( "CBasicBagGui::Destroy" );

	CBagBaseGui::Destroy();

	NTL_RETURNVOID();
}

VOID CBag4SpaceGui::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 i = 0 ; i < m_nNumSlot ; ++i )
	{
		RwInt32 j = i % 4;
		RwInt32 k = i / 4;
		m_prtSlots[i].left	= j * 40 + 19;
		m_prtSlots[i].top	= k * 40 + 31;
		m_prtSlots[i].right	= m_prtSlots[i].left + 32;
		m_prtSlots[i].bottom= m_prtSlots[i].top + 32;

		RwInt32 nLeft = rtScreen.left + m_prtSlots[i].left;
		RwInt32 nTop = rtScreen.top + m_prtSlots[i].top;
		RwInt32 nRight = rtScreen.left + m_prtSlots[i].right;
		RwInt32 nBottom = rtScreen.top + m_prtSlots[i].bottom;

		m_psurIcons[i].SetRect( nLeft, nTop, nRight, nBottom );
		m_psurDisableSlot[i].SetRect( nLeft, nTop, nRight, nBottom );

		m_pCoolTimeEffect[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );
		m_pCoolTimeEffect[i].SetPosition( nLeft, nTop );

		m_ppGambleBoomEffect[i]->SetPosition( m_prtSlots[i].left - ((m_ppGambleBoomEffect[i]->GetWidth() - m_prtSlots[i].GetWidth())/2)
			, m_prtSlots[i].top - ((m_ppGambleBoomEffect[i]->GetHeight() - m_prtSlots[i].GetHeight())/2) );
	}
}

VOID CBag4SpaceGui::OnMouseDown( const CKey& key )
{
	MouseDown( key );
}

VOID CBag4SpaceGui::OnMouseUp( const CKey& key )
{
	MouseUp( key );
}

VOID CBag4SpaceGui::OnMouseOut( gui::CComponent* pComponent )
{
	MouseOut();
}

VOID CBag4SpaceGui::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	MouseMove( nKey, nXPos, nYPos );
}

VOID CBag4SpaceGui::OnPaint()
{
	Paint();
}

VOID CBag4SpaceGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	Move( nX, nY );
}

VOID CBag4SpaceGui::OnExitButtonClick( gui::CComponent* pComponent )
{
	ExitButtonClick( pComponent );
}

////////////////////////////////////////////////////////////////////////////////
// Bag8SpaceGui

CBag8SpaceGui::CBag8SpaceGui(VOID)
{

}

CBag8SpaceGui::CBag8SpaceGui( const RwChar* pName )
: CBagBaseGui( pName )
{

}

CBag8SpaceGui::~CBag8SpaceGui(VOID)
{

}

RwBool CBag8SpaceGui::Create(VOID)
{
	NTL_FUNCTION( "CBag8SpaceGui" );

	if( !CNtlPLGui::Create( "", "gui\\8Bag.srf", "gui\\8Bag.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pCloseButton = (gui::CButton*)GetComponent( "btnBagExit" );

	m_slotMouseDown = m_pThis->SigMouseDown().Connect( this, &CBag8SpaceGui::OnMouseDown );
	m_slotMouseUp	= m_pThis->SigMouseUp().Connect( this, &CBag8SpaceGui::OnMouseUp );
	m_slotMouseMove	= m_pThis->SigMouseMove().Connect( this, &CBag8SpaceGui::OnMouseMove );
	m_slotMouseOut  = m_pThis->SigMouseLeave().Connect( this, &CBag8SpaceGui::OnMouseOut );
	m_slotPaint		= m_pThis->SigPaint().Connect( this, &CBag8SpaceGui::OnPaint );
	m_slotMove		= m_pThis->SigMove().Connect( this, &CBag8SpaceGui::OnMove );
	m_slotClikedExit= m_pCloseButton->SigClicked().Connect( this, &CBag8SpaceGui::OnExitButtonClick );
	
	m_nNumSlot = 8;
	if( !CBagBaseGui::Create() )
		NTL_RETURN( FALSE );

	SetSlotRectHardCode();

	NTL_RETURN( TRUE );
}

VOID CBag8SpaceGui::Destroy(VOID)
{
	NTL_FUNCTION( "CBasicBagGui::Destroy" );

	CBagBaseGui::Destroy();

	NTL_RETURNVOID();
}

VOID CBag8SpaceGui::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 i = 0 ; i < m_nNumSlot ; ++i )
	{
		RwInt32 j = i % 4;
		RwInt32 k = i / 4;
		m_prtSlots[i].left	= j * 40 + 19;
		m_prtSlots[i].top	= k * 40 + 31;
		m_prtSlots[i].right	= m_prtSlots[i].left + 32;
		m_prtSlots[i].bottom= m_prtSlots[i].top + 32;

		RwInt32 nLeft = rtScreen.left + m_prtSlots[i].left;
		RwInt32 nTop = rtScreen.top + m_prtSlots[i].top;
		RwInt32 nRight = rtScreen.left + m_prtSlots[i].right;
		RwInt32 nBottom = rtScreen.top + m_prtSlots[i].bottom;

		m_psurIcons[i].SetRect( nLeft, nTop, nRight, nBottom );
		m_psurDisableSlot[i].SetRect( nLeft, nTop, nRight, nBottom );

		m_pCoolTimeEffect[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );
		m_pCoolTimeEffect[i].SetPosition( nLeft, nTop );	

		m_ppGambleBoomEffect[i]->SetPosition( m_prtSlots[i].left - ((m_ppGambleBoomEffect[i]->GetWidth() - m_prtSlots[i].GetWidth())/2)
			, m_prtSlots[i].top - ((m_ppGambleBoomEffect[i]->GetHeight() - m_prtSlots[i].GetHeight())/2) );
	}
}

VOID CBag8SpaceGui::OnMouseDown( const CKey& key )
{
	MouseDown( key );
}

VOID CBag8SpaceGui::OnMouseUp( const CKey& key )
{
	MouseUp( key );
}

VOID CBag8SpaceGui::OnMouseOut( gui::CComponent* pComponent )
{
	MouseOut();
}

VOID CBag8SpaceGui::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	MouseMove( nKey, nXPos, nYPos );
}

VOID CBag8SpaceGui::OnPaint()
{
	Paint();
}

VOID CBag8SpaceGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	Move( nX, nY );
}

VOID CBag8SpaceGui::OnExitButtonClick( gui::CComponent* pComponent )
{
	ExitButtonClick( pComponent );
}
////////////////////////////////////////////////////////////////////////////////
// Bag12SpaceGui

CBag12SpaceGui::CBag12SpaceGui(VOID)
{

}

CBag12SpaceGui::CBag12SpaceGui( const RwChar* pName )
: CBagBaseGui( pName )
{
	
}

CBag12SpaceGui::~CBag12SpaceGui(VOID)
{

}

RwBool CBag12SpaceGui::Create(VOID)
{
	NTL_FUNCTION( "CBag12SpaceGui" );

	if( !CNtlPLGui::Create( "", "gui\\12Bag.srf", "gui\\12Bag.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pCloseButton = (gui::CButton*)GetComponent( "btnBagExit" );

	m_slotMouseDown = m_pThis->SigMouseDown().Connect( this, &CBag12SpaceGui::OnMouseDown );
	m_slotMouseUp	= m_pThis->SigMouseUp().Connect( this, &CBag12SpaceGui::OnMouseUp );
	m_slotMouseMove	= m_pThis->SigMouseMove().Connect( this, &CBag12SpaceGui::OnMouseMove );
	m_slotMouseOut  = m_pThis->SigMouseLeave().Connect( this, &CBag12SpaceGui::OnMouseOut );
	m_slotPaint		= m_pThis->SigPaint().Connect( this, &CBag12SpaceGui::OnPaint );
	m_slotMove		= m_pThis->SigMove().Connect( this, &CBag12SpaceGui::OnMove );
	m_slotClikedExit= m_pCloseButton->SigClicked().Connect( this, &CBag12SpaceGui::OnExitButtonClick );
	
	m_nNumSlot = 12;
	if( !CBagBaseGui::Create() )
		NTL_RETURN( FALSE );

	SetSlotRectHardCode();

	NTL_RETURN( TRUE );
}	

VOID CBag12SpaceGui::Destroy(VOID)
{
	NTL_FUNCTION( "CBasicBagGui::Destroy" );

	CBagBaseGui::Destroy();

	NTL_RETURNVOID();
}

VOID CBag12SpaceGui::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 i = 0 ; i < m_nNumSlot ; ++i )
	{
		RwInt32 j = i % 4;
		RwInt32 k = i / 4;
		m_prtSlots[i].left	= j * 40 + 19;
		m_prtSlots[i].top	= k * 40 + 31;
		m_prtSlots[i].right	= m_prtSlots[i].left + 32;
		m_prtSlots[i].bottom= m_prtSlots[i].top + 32;

		RwInt32 nLeft = rtScreen.left + m_prtSlots[i].left;
		RwInt32 nTop = rtScreen.top + m_prtSlots[i].top;
		RwInt32 nRight = rtScreen.left + m_prtSlots[i].right;
		RwInt32 nBottom = rtScreen.top + m_prtSlots[i].bottom;

		m_psurIcons[i].SetRect( nLeft, nTop, nRight, nBottom );
		m_psurDisableSlot[i].SetRect( nLeft, nTop, nRight, nBottom );

		m_pCoolTimeEffect[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );
		m_pCoolTimeEffect[i].SetPosition( nLeft, nTop );

		m_ppGambleBoomEffect[i]->SetPosition( m_prtSlots[i].left - ((m_ppGambleBoomEffect[i]->GetWidth() - m_prtSlots[i].GetWidth())/2)
			, m_prtSlots[i].top - ((m_ppGambleBoomEffect[i]->GetHeight() - m_prtSlots[i].GetHeight())/2) );
	}
}

VOID CBag12SpaceGui::OnMouseDown( const CKey& key )
{
	MouseDown( key );
}

VOID CBag12SpaceGui::OnMouseUp( const CKey& key )
{
	MouseUp( key );
}

VOID CBag12SpaceGui::OnMouseOut( gui::CComponent* pComponent )
{
	MouseOut();
}

VOID CBag12SpaceGui::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	MouseMove( nKey, nXPos, nYPos );
}

VOID CBag12SpaceGui::OnPaint()
{
	Paint();
}

VOID CBag12SpaceGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	Move( nX, nY );
}

VOID CBag12SpaceGui::OnExitButtonClick( gui::CComponent* pComponent )
{
	ExitButtonClick( pComponent );
}

////////////////////////////////////////////////////////////////////////////////
// Bag16SpaceGui

CBag16SpaceGui::CBag16SpaceGui(VOID)
{

}

CBag16SpaceGui::CBag16SpaceGui( const RwChar* pName )
: CBagBaseGui( pName )
{

}

CBag16SpaceGui::~CBag16SpaceGui(VOID)
{

}

RwBool CBag16SpaceGui::Create(VOID)
{
	NTL_FUNCTION( "CBag16SpaceGui" );

	if( !CNtlPLGui::Create( "", "gui\\16Bag.srf", "gui\\16Bag.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pCloseButton = (gui::CButton*)GetComponent( "btnBagExit" );

	m_slotMouseDown = m_pThis->SigMouseDown().Connect( this, &CBag16SpaceGui::OnMouseDown );
	m_slotMouseUp	= m_pThis->SigMouseUp().Connect( this, &CBag16SpaceGui::OnMouseUp );
	m_slotMouseMove	= m_pThis->SigMouseMove().Connect( this, &CBag16SpaceGui::OnMouseMove );
	m_slotMouseOut  = m_pThis->SigMouseLeave().Connect( this, &CBag16SpaceGui::OnMouseOut );
	m_slotPaint		= m_pThis->SigPaint().Connect( this, &CBag16SpaceGui::OnPaint );
	m_slotMove		= m_pThis->SigMove().Connect( this, &CBag16SpaceGui::OnMove );
	m_slotClikedExit= m_pCloseButton->SigClicked().Connect( this, &CBag16SpaceGui::OnExitButtonClick );

	m_nNumSlot = 16;
	if( !CBagBaseGui::Create() )
		NTL_RETURN( FALSE );

	SetSlotRectHardCode();

	NTL_RETURN( TRUE );
}	

VOID CBag16SpaceGui::Destroy(VOID)
{
	NTL_FUNCTION( "CBasicBagGui::Destroy" );

	CBagBaseGui::Destroy();

	NTL_RETURNVOID();
}

VOID CBag16SpaceGui::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 i = 0 ; i < m_nNumSlot ; ++i )
	{
		RwInt32 j = i % 4;
		RwInt32 k = i / 4;
		m_prtSlots[i].left	= j * 40 + 19;
		m_prtSlots[i].top	= k * 40 + 31;
		m_prtSlots[i].right	= m_prtSlots[i].left + 32;
		m_prtSlots[i].bottom= m_prtSlots[i].top + 32;

		RwInt32 nLeft = rtScreen.left + m_prtSlots[i].left;
		RwInt32 nTop = rtScreen.top + m_prtSlots[i].top;
		RwInt32 nRight = rtScreen.left + m_prtSlots[i].right;
		RwInt32 nBottom = rtScreen.top + m_prtSlots[i].bottom;

		m_psurIcons[i].SetRect( nLeft, nTop, nRight, nBottom );
		m_psurDisableSlot[i].SetRect( nLeft, nTop, nRight, nBottom );

		m_pCoolTimeEffect[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );
		m_pCoolTimeEffect[i].SetPosition( nLeft, nTop );	

		m_ppGambleBoomEffect[i]->SetPosition( m_prtSlots[i].left - ((m_ppGambleBoomEffect[i]->GetWidth() - m_prtSlots[i].GetWidth())/2)
			, m_prtSlots[i].top - ((m_ppGambleBoomEffect[i]->GetHeight() - m_prtSlots[i].GetHeight())/2) );
	}
}

VOID CBag16SpaceGui::OnMouseDown( const CKey& key )
{
	MouseDown( key );
}

VOID CBag16SpaceGui::OnMouseUp( const CKey& key )
{
	MouseUp( key );
}

VOID CBag16SpaceGui::OnMouseOut( gui::CComponent* pComponent )
{
	MouseOut();
}

VOID CBag16SpaceGui::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	MouseMove( nKey, nXPos, nYPos );
}

VOID CBag16SpaceGui::OnPaint()
{
	Paint();
}

VOID CBag16SpaceGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	Move( nX, nY );
}

VOID CBag16SpaceGui::OnExitButtonClick( gui::CComponent* pComponent )
{
	ExitButtonClick( pComponent );
}

////////////////////////////////////////////////////////////////////////////////
// Bag20SpaceGui

CBag20SpaceGui::CBag20SpaceGui(VOID)
{

}

CBag20SpaceGui::CBag20SpaceGui(const RwChar* pName)
	: CBagBaseGui(pName)
{

}

CBag20SpaceGui::~CBag20SpaceGui(VOID)
{

}

RwBool CBag20SpaceGui::Create(VOID)
{
	NTL_FUNCTION("CBag20SpaceGui");

	if (!CNtlPLGui::Create("", "gui\\20Bag.srf", "gui\\20Bag.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pCloseButton = (gui::CButton*)GetComponent("btnBagExit");

	m_slotMouseDown = m_pThis->SigMouseDown().Connect(this, &CBag20SpaceGui::OnMouseDown);
	m_slotMouseUp = m_pThis->SigMouseUp().Connect(this, &CBag20SpaceGui::OnMouseUp);
	m_slotMouseMove = m_pThis->SigMouseMove().Connect(this, &CBag20SpaceGui::OnMouseMove);
	m_slotMouseOut = m_pThis->SigMouseLeave().Connect(this, &CBag20SpaceGui::OnMouseOut);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CBag20SpaceGui::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CBag20SpaceGui::OnMove);
	m_slotClikedExit = m_pCloseButton->SigClicked().Connect(this, &CBag20SpaceGui::OnExitButtonClick);

	m_nNumSlot = 20;
	if (!CBagBaseGui::Create())
		NTL_RETURN(FALSE);

	SetSlotRectHardCode();

	NTL_RETURN(TRUE);
}

VOID CBag20SpaceGui::Destroy(VOID)
{
	NTL_FUNCTION("CBasicBagGui::Destroy");

	CBagBaseGui::Destroy();

	NTL_RETURNVOID();
}

VOID CBag20SpaceGui::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for (RwInt32 i = 0; i < m_nNumSlot; ++i)
	{
		RwInt32 j = i % 4;
		RwInt32 k = i / 4;
		m_prtSlots[i].left = j * 40 + 19;
		m_prtSlots[i].top = k * 40 + 31;
		m_prtSlots[i].right = m_prtSlots[i].left + 32;
		m_prtSlots[i].bottom = m_prtSlots[i].top + 32;

		RwInt32 nLeft = rtScreen.left + m_prtSlots[i].left;
		RwInt32 nTop = rtScreen.top + m_prtSlots[i].top;
		RwInt32 nRight = rtScreen.left + m_prtSlots[i].right;
		RwInt32 nBottom = rtScreen.top + m_prtSlots[i].bottom;

		m_psurIcons[i].SetRect(nLeft, nTop, nRight, nBottom);
		m_psurDisableSlot[i].SetRect(nLeft, nTop, nRight, nBottom);

		m_pCoolTimeEffect[i].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotGrayedEffect"));
		m_pCoolTimeEffect[i].SetPosition(nLeft, nTop);

		m_ppGambleBoomEffect[i]->SetPosition(m_prtSlots[i].left - ((m_ppGambleBoomEffect[i]->GetWidth() - m_prtSlots[i].GetWidth()) / 2)
			, m_prtSlots[i].top - ((m_ppGambleBoomEffect[i]->GetHeight() - m_prtSlots[i].GetHeight()) / 2));
	}
}

VOID CBag20SpaceGui::OnMouseDown(const CKey& key)
{
	MouseDown(key);
}

VOID CBag20SpaceGui::OnMouseUp(const CKey& key)
{
	MouseUp(key);
}

VOID CBag20SpaceGui::OnMouseOut(gui::CComponent* pComponent)
{
	MouseOut();
}

VOID CBag20SpaceGui::OnMouseMove(RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos)
{
	MouseMove(nKey, nXPos, nYPos);
}

VOID CBag20SpaceGui::OnPaint()
{
	Paint();
}

VOID CBag20SpaceGui::OnMove(RwInt32 nX, RwInt32 nY)
{
	Move(nX, nY);
}

VOID CBag20SpaceGui::OnExitButtonClick(gui::CComponent* pComponent)
{
	ExitButtonClick(pComponent);
}

////////////////////////////////////////////////////////////////////////////////
// Bag24SpaceGui

CBag24SpaceGui::CBag24SpaceGui(VOID)
{

}

CBag24SpaceGui::CBag24SpaceGui(const RwChar* pName)
	: CBagBaseGui(pName)
{

}

CBag24SpaceGui::~CBag24SpaceGui(VOID)
{

}

RwBool CBag24SpaceGui::Create(VOID)
{
	NTL_FUNCTION("CBag24SpaceGui");

	if (!CNtlPLGui::Create("", "gui\\24Bag.srf", "gui\\24Bag.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pCloseButton = (gui::CButton*)GetComponent("btnBagExit");

	m_slotMouseDown = m_pThis->SigMouseDown().Connect(this, &CBag24SpaceGui::OnMouseDown);
	m_slotMouseUp = m_pThis->SigMouseUp().Connect(this, &CBag24SpaceGui::OnMouseUp);
	m_slotMouseMove = m_pThis->SigMouseMove().Connect(this, &CBag24SpaceGui::OnMouseMove);
	m_slotMouseOut = m_pThis->SigMouseLeave().Connect(this, &CBag24SpaceGui::OnMouseOut);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CBag24SpaceGui::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CBag24SpaceGui::OnMove);
	m_slotClikedExit = m_pCloseButton->SigClicked().Connect(this, &CBag24SpaceGui::OnExitButtonClick);

	m_nNumSlot = 24;
	if (!CBagBaseGui::Create())
		NTL_RETURN(FALSE);

	SetSlotRectHardCode();

	NTL_RETURN(TRUE);
}

VOID CBag24SpaceGui::Destroy(VOID)
{
	NTL_FUNCTION("CBag24SpaceGui::Destroy");

	CBagBaseGui::Destroy();

	NTL_RETURNVOID();
}

VOID CBag24SpaceGui::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for (RwInt32 i = 0; i < m_nNumSlot; ++i)
	{
		RwInt32 j = i % 4;
		RwInt32 k = i / 4;
		m_prtSlots[i].left = j * 40 + 19;
		m_prtSlots[i].top = k * 40 + 31;
		m_prtSlots[i].right = m_prtSlots[i].left + 32;
		m_prtSlots[i].bottom = m_prtSlots[i].top + 32;

		RwInt32 nLeft = rtScreen.left + m_prtSlots[i].left;
		RwInt32 nTop = rtScreen.top + m_prtSlots[i].top;
		RwInt32 nRight = rtScreen.left + m_prtSlots[i].right;
		RwInt32 nBottom = rtScreen.top + m_prtSlots[i].bottom;

		m_psurIcons[i].SetRect(nLeft, nTop, nRight, nBottom);
		m_psurDisableSlot[i].SetRect(nLeft, nTop, nRight, nBottom);

		m_pCoolTimeEffect[i].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotGrayedEffect"));
		m_pCoolTimeEffect[i].SetPosition(nLeft, nTop);

		m_ppGambleBoomEffect[i]->SetPosition(m_prtSlots[i].left - ((m_ppGambleBoomEffect[i]->GetWidth() - m_prtSlots[i].GetWidth()) / 2)
			, m_prtSlots[i].top - ((m_ppGambleBoomEffect[i]->GetHeight() - m_prtSlots[i].GetHeight()) / 2));
	}
}

VOID CBag24SpaceGui::OnMouseDown(const CKey& key)
{
	MouseDown(key);
}

VOID CBag24SpaceGui::OnMouseUp(const CKey& key)
{
	MouseUp(key);
}

VOID CBag24SpaceGui::OnMouseOut(gui::CComponent* pComponent)
{
	MouseOut();
}

VOID CBag24SpaceGui::OnMouseMove(RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos)
{
	MouseMove(nKey, nXPos, nYPos);
}

VOID CBag24SpaceGui::OnPaint()
{
	Paint();
}

VOID CBag24SpaceGui::OnMove(RwInt32 nX, RwInt32 nY)
{
	Move(nX, nY);
}

VOID CBag24SpaceGui::OnExitButtonClick(gui::CComponent* pComponent)
{
	ExitButtonClick(pComponent);
}


////////////////////////////////////////////////////////////////////////////////
// Bag28SpaceGui

CBag28SpaceGui::CBag28SpaceGui(VOID)
{

}

CBag28SpaceGui::CBag28SpaceGui(const RwChar* pName)
	: CBagBaseGui(pName)
{

}

CBag28SpaceGui::~CBag28SpaceGui(VOID)
{

}

RwBool CBag28SpaceGui::Create(VOID)
{
	NTL_FUNCTION("CBag28SpaceGui");

	if (!CNtlPLGui::Create("", "gui\\28Bag.srf", "gui\\28Bag.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pCloseButton = (gui::CButton*)GetComponent("btnBagExit");

	m_slotMouseDown = m_pThis->SigMouseDown().Connect(this, &CBag28SpaceGui::OnMouseDown);
	m_slotMouseUp = m_pThis->SigMouseUp().Connect(this, &CBag28SpaceGui::OnMouseUp);
	m_slotMouseMove = m_pThis->SigMouseMove().Connect(this, &CBag28SpaceGui::OnMouseMove);
	m_slotMouseOut = m_pThis->SigMouseLeave().Connect(this, &CBag28SpaceGui::OnMouseOut);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CBag28SpaceGui::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CBag28SpaceGui::OnMove);
	m_slotClikedExit = m_pCloseButton->SigClicked().Connect(this, &CBag28SpaceGui::OnExitButtonClick);

	m_nNumSlot = 28;
	if (!CBagBaseGui::Create())
		NTL_RETURN(FALSE);

	SetSlotRectHardCode();

	NTL_RETURN(TRUE);
}

VOID CBag28SpaceGui::Destroy(VOID)
{
	NTL_FUNCTION("CBag28SpaceGui::Destroy");

	CBagBaseGui::Destroy();

	NTL_RETURNVOID();
}

VOID CBag28SpaceGui::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for (RwInt32 i = 0; i < m_nNumSlot; ++i)
	{
		RwInt32 j = i % 4;
		RwInt32 k = i / 4;
		m_prtSlots[i].left = j * 40 + 19;
		m_prtSlots[i].top = k * 40 + 31;
		m_prtSlots[i].right = m_prtSlots[i].left + 32;
		m_prtSlots[i].bottom = m_prtSlots[i].top + 32;

		RwInt32 nLeft = rtScreen.left + m_prtSlots[i].left;
		RwInt32 nTop = rtScreen.top + m_prtSlots[i].top;
		RwInt32 nRight = rtScreen.left + m_prtSlots[i].right;
		RwInt32 nBottom = rtScreen.top + m_prtSlots[i].bottom;

		m_psurIcons[i].SetRect(nLeft, nTop, nRight, nBottom);
		m_psurDisableSlot[i].SetRect(nLeft, nTop, nRight, nBottom);

		m_pCoolTimeEffect[i].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotGrayedEffect"));
		m_pCoolTimeEffect[i].SetPosition(nLeft, nTop);

		m_ppGambleBoomEffect[i]->SetPosition(m_prtSlots[i].left - ((m_ppGambleBoomEffect[i]->GetWidth() - m_prtSlots[i].GetWidth()) / 2)
			, m_prtSlots[i].top - ((m_ppGambleBoomEffect[i]->GetHeight() - m_prtSlots[i].GetHeight()) / 2));
	}
}

VOID CBag28SpaceGui::OnMouseDown(const CKey& key)
{
	MouseDown(key);
}

VOID CBag28SpaceGui::OnMouseUp(const CKey& key)
{
	MouseUp(key);
}

VOID CBag28SpaceGui::OnMouseOut(gui::CComponent* pComponent)
{
	MouseOut();
}

VOID CBag28SpaceGui::OnMouseMove(RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos)
{
	MouseMove(nKey, nXPos, nYPos);
}

VOID CBag28SpaceGui::OnPaint()
{
	Paint();
}

VOID CBag28SpaceGui::OnMove(RwInt32 nX, RwInt32 nY)
{
	Move(nX, nY);
}

VOID CBag28SpaceGui::OnExitButtonClick(gui::CComponent* pComponent)
{
	ExitButtonClick(pComponent);
}


////////////////////////////////////////////////////////////////////////////////
// Bag32SpaceGui

CBag32SpaceGui::CBag32SpaceGui(VOID)
{

}

CBag32SpaceGui::CBag32SpaceGui(const RwChar* pName)
	: CBagBaseGui(pName)
{

}

CBag32SpaceGui::~CBag32SpaceGui(VOID)
{

}

RwBool CBag32SpaceGui::Create(VOID)
{
	NTL_FUNCTION("CBag32SpaceGui");

	if (!CNtlPLGui::Create("", "gui\\32Bag.srf", "gui\\32Bag.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pCloseButton = (gui::CButton*)GetComponent("btnBagExit");

	m_slotMouseDown = m_pThis->SigMouseDown().Connect(this, &CBag32SpaceGui::OnMouseDown);
	m_slotMouseUp = m_pThis->SigMouseUp().Connect(this, &CBag32SpaceGui::OnMouseUp);
	m_slotMouseMove = m_pThis->SigMouseMove().Connect(this, &CBag32SpaceGui::OnMouseMove);
	m_slotMouseOut = m_pThis->SigMouseLeave().Connect(this, &CBag32SpaceGui::OnMouseOut);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CBag32SpaceGui::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CBag32SpaceGui::OnMove);
	m_slotClikedExit = m_pCloseButton->SigClicked().Connect(this, &CBag32SpaceGui::OnExitButtonClick);

	m_nNumSlot = 32;
	if (!CBagBaseGui::Create())
		NTL_RETURN(FALSE);

	SetSlotRectHardCode();

	NTL_RETURN(TRUE);
}

VOID CBag32SpaceGui::Destroy(VOID)
{
	NTL_FUNCTION("CBag32SpaceGui::Destroy");

	CBagBaseGui::Destroy();

	NTL_RETURNVOID();
}

VOID CBag32SpaceGui::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for (RwInt32 i = 0; i < m_nNumSlot; ++i)
	{
		RwInt32 j = i % 4;
		RwInt32 k = i / 4;
		m_prtSlots[i].left = j * 40 + 19;
		m_prtSlots[i].top = k * 40 + 31;
		m_prtSlots[i].right = m_prtSlots[i].left + 32;
		m_prtSlots[i].bottom = m_prtSlots[i].top + 32;

		RwInt32 nLeft = rtScreen.left + m_prtSlots[i].left;
		RwInt32 nTop = rtScreen.top + m_prtSlots[i].top;
		RwInt32 nRight = rtScreen.left + m_prtSlots[i].right;
		RwInt32 nBottom = rtScreen.top + m_prtSlots[i].bottom;

		m_psurIcons[i].SetRect(nLeft, nTop, nRight, nBottom);
		m_psurDisableSlot[i].SetRect(nLeft, nTop, nRight, nBottom);

		m_pCoolTimeEffect[i].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotGrayedEffect"));
		m_pCoolTimeEffect[i].SetPosition(nLeft, nTop);

		m_ppGambleBoomEffect[i]->SetPosition(m_prtSlots[i].left - ((m_ppGambleBoomEffect[i]->GetWidth() - m_prtSlots[i].GetWidth()) / 2)
			, m_prtSlots[i].top - ((m_ppGambleBoomEffect[i]->GetHeight() - m_prtSlots[i].GetHeight()) / 2));
	}
}

VOID CBag32SpaceGui::OnMouseDown(const CKey& key)
{
	MouseDown(key);
}

VOID CBag32SpaceGui::OnMouseUp(const CKey& key)
{
	MouseUp(key);
}

VOID CBag32SpaceGui::OnMouseOut(gui::CComponent* pComponent)
{
	MouseOut();
}

VOID CBag32SpaceGui::OnMouseMove(RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos)
{
	MouseMove(nKey, nXPos, nYPos);
}

VOID CBag32SpaceGui::OnPaint()
{
	Paint();
}

VOID CBag32SpaceGui::OnMove(RwInt32 nX, RwInt32 nY)
{
	Move(nX, nY);
}

VOID CBag32SpaceGui::OnExitButtonClick(gui::CComponent* pComponent)
{
	ExitButtonClick(pComponent);
}

////////////////////////////////////////////////////////////////////////////////
// GetItemEffect
#define BAG_ITEM_GET_EFFECT_YDELTA			64
#define BAG_ITEM_GET_EFFECT_MOVE_TIME		1.0f
#define BAG_ITEM_GET_EFFECT_DELAY_TIME		0.2f
#define BAG_ITEM_GET_EFFECT_MIN_ALPHA		0
#define BAG_ITEM_GET_EFFECT_MAX_ALPHA		255

CGetItemEffect::CGetItemEffect(VOID)
: m_bShowEffect( FALSE ), m_fCurrentTime( 0.0f ), m_nInitXPos( 0 ), m_nInitYPos( 0 ), m_nDestXPos( 0 ), m_nDestYPos( 0 ), m_hSerial( INVALID_SERIAL_ID )
{

}

CGetItemEffect::~CGetItemEffect(VOID)
{

}

VOID CGetItemEffect::Create(VOID)
{
	
}

VOID CGetItemEffect::Destroy(VOID)
{

}

VOID CGetItemEffect::StartProc( RwInt32 nScreenXPos, RwInt32 nScreenYPos, gui::CTexture* pTexture, RwReal fDelayTime, RwUInt32 hSerial )
{
	CSurfaceGui surface;
	surface.SetRectWH( 0, 0, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE );
	surface.SetTexture( pTexture );

	m_bShowEffect = TRUE;
	m_fCurrentTime = -fDelayTime;
	m_nInitXPos = nScreenXPos + NTL_ITEM_ICON_SIZE / 2;
	m_nInitYPos = nScreenYPos - BAG_ITEM_GET_EFFECT_YDELTA + NTL_ITEM_ICON_SIZE / 2;
	m_nDestXPos = nScreenXPos + NTL_ITEM_ICON_SIZE / 2;
	m_nDestYPos = nScreenYPos + NTL_ITEM_ICON_SIZE / 2;
	m_hSerial = hSerial;

	m_FlickerEffect.SetSurface( *surface.GetSurface() );
	m_FlickerEffect.SetCenterPos( m_nInitXPos, m_nInitYPos );
	m_FlickerEffect.SetAlpha( BAG_ITEM_GET_EFFECT_MAX_ALPHA, BAG_ITEM_GET_EFFECT_MIN_ALPHA );
	m_FlickerEffect.SetSize( NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE / 2, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE / 2 );
	m_FlickerEffect.SetTime( 1.0f, 1.0f );	
	m_FlickerEffect.StartProc();
}

VOID CGetItemEffect::EndProc(VOID)
{
	m_bShowEffect = FALSE;
	m_fCurrentTime = 0.0f;
	m_FlickerEffect.EndProc();
}

RwBool CGetItemEffect::Update( RwReal fElapsed )
{
	if( m_bShowEffect )
	{
		m_fCurrentTime += fElapsed;
		
		if( m_fCurrentTime >= 0.0f )
		{
			m_FlickerEffect.Update( fElapsed );

			if( m_fCurrentTime < BAG_ITEM_GET_EFFECT_MOVE_TIME )
			{
				RwReal fYPosControl1 = gui::GetValueByPercentOfStart( (RwReal)m_nInitYPos, (RwReal)m_nDestYPos, 80 );
				RwReal fYPosControl2 = gui::GetValueByPercentOfStart( (RwReal)m_nInitYPos, (RwReal)m_nDestYPos, 90 );
				RwInt32 nYPos = (RwInt32)( gui::GetResult3BezierInterpolation( m_fCurrentTime, BAG_ITEM_GET_EFFECT_MOVE_TIME, (RwReal)m_nInitYPos, fYPosControl1, fYPosControl2, (RwReal)m_nDestYPos ) );			
				m_FlickerEffect.SetCenterPos( m_nDestXPos, nYPos );
			}
			else
			{
				EndProc();
				return FALSE;
			}
		}
	}

	return TRUE;
}

VOID CGetItemEffect::Render(VOID)
{
	if( m_bShowEffect )
		m_FlickerEffect.Render();
}

VOID CGetItemEffect::SetInitXPos( RwInt32 nScreenXPos )
{
	m_nInitXPos = nScreenXPos + NTL_ITEM_ICON_SIZE / 2;
	m_nDestXPos = nScreenXPos + NTL_ITEM_ICON_SIZE / 2;
}

VOID CGetItemEffect::SetInitYPos( RwInt32 nScreenYPos )
{
	m_nInitYPos = nScreenYPos - BAG_ITEM_GET_EFFECT_YDELTA + NTL_ITEM_ICON_SIZE / 2;
	m_nDestYPos = nScreenYPos + NTL_ITEM_ICON_SIZE / 2;
}

////////////////////////////////////////////////////////////////////////////////
// BagSlotGui

CBagSlotGui::CBagSlotGui(VOID)
{
	Init();
}

CBagSlotGui::CBagSlotGui( const RwChar* pName )
: CNtlPLGui( pName )
{	
	Init();
}

CBagSlotGui::~CBagSlotGui(VOID)
{
	
}

VOID CBagSlotGui::Init(VOID)
{
	for( RwInt32 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; i++ )
	{
		m_pBag[i] = NULL;
		m_hBagSerial[i] = INVALID_SERIAL_ID;

		if( i < VISIBLE_BAGSLOT_COUNT )
		{
			m_arrFocusEffect[i] = 0x00;
			m_apstbStackNum[i] = NULL;
		}		
	}

	m_nLSelectedSlotIdx = -1;
	m_nRSelectedSlotIdx = -1;
	m_nMouseOnIndex = -1;
	m_nPushDownIndex = -1;
}

RwBool CBagSlotGui::Create(VOID)
{
	NTL_FUNCTION( "CBagSlotGui::Create" );

	if( !CNtlPLGui::Create( "", "", "gui\\BagSlot.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );
	
	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pThis->SetPriority(dDIALOGPRIORITY_QUICKSLOT);

	m_slotMouseDown = m_pThis->SigMouseDown().Connect( this, &CBagSlotGui::OnMouseDown );
	m_slotMouseUp	= m_pThis->SigMouseUp().Connect( this, &CBagSlotGui::OnMouseUp );
	m_slotMouseOut	= m_pThis->SigMouseLeave().Connect( this, &CBagSlotGui::OnMouseOut );
	m_slotMouseMove	= m_pThis->SigMouseMove().Connect( this, &CBagSlotGui::OnMouseMove );
	m_slotPaint		= m_pThis->SigPaint().Connect( this, &CBagSlotGui::OnPaint );
	m_slotMove		= m_pThis->SigMove().Connect( this, &CBagSlotGui::OnMove );

	SetSlotRectHardCode();

	LinkMsg( g_EventSobInfoUpdate, 0 );
	LinkMsg( g_EventIconMoveClick, 0 );
	LinkMsg( g_EventIconPopupResult, 0 );
	LinkMsg( g_EventEnableItemIcon, 0 );
	LinkMsg( g_EventItemCreatedEffect, 0 );
	LinkMsg( g_EventZennyLootingEffect, 0 );
	LinkMsg( g_EventGameServerChangeOut );
	LinkMsg( g_EventSobItemDelete );

	GetNtlGuiManager()->AddUpdateFunc( this );

	NTL_RETURN( TRUE );
}

VOID CBagSlotGui::Destroy(VOID)
{
	NTL_FUNCTION( "CBagSlotGui::Destroy" );

	ClearGetItemEffect();
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventIconMoveClick );
	UnLinkMsg( g_EventIconPopupResult );
	UnLinkMsg( g_EventEnableItemIcon );
	UnLinkMsg( g_EventSobItemAdd );
	UnLinkMsg( g_EventItemCreatedEffect );
	UnLinkMsg( g_EventZennyLootingEffect );
	UnLinkMsg( g_EventGameServerChangeOut );
	UnLinkMsg( g_EventSobItemDelete );

	for( RwInt32 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
		if( m_pBag[i] )
		{
			m_pBag[i]->Destroy();
			NTL_DELETE( m_pBag[i] );
		}
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CBagSlotGui::Update( RwReal fElapsed )
{
	std::list<CGetItemEffect*>::iterator it;
	for( it = m_listGetItemEffect.begin() ; it != m_listGetItemEffect.end() ; )
	{
		CGetItemEffect* pGetItemEffect = *it;
		if( pGetItemEffect->Update( fElapsed ) )
		{
			++it;
		}
		else
		{
			NTL_DELETE( pGetItemEffect );
			it = m_listGetItemEffect.erase( it );
		}
	}
}

RwInt32 CBagSlotGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		//RaiseTop();
		Show( true );
		//UpdateData();
	}
	else
	{
		Show( false );
	}

	return 1;
}

VOID CBagSlotGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );

		if( pUpdate->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			return;

		if( pUpdate->uiUpdateType & EVENT_AIUT_ITEM )
		{
			UpdateData();

			for( RwInt32 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
			{
				if( m_pBag[i] )
					m_pBag[i]->HandleEvents( msg );
			}
		}
		else if( pUpdate->uiUpdateType & EVENT_AIUT_ZENNY )
		{
			if( m_pBag[0] )
				m_pBag[0]->HandleEvents( msg );
		}
	}
	else if( msg.Id == g_EventIconMoveClick &&
		     ( GetIconMoveManager()->GetSrcPlace() == PLACE_BAGSLOT ||
			   GetIconMoveManager()->GetSrcPlace() == PLACE_BAG ) )
	{
		ShowIconDestination();
	}
	//else if( msg.Id == g_EventIconPopupResult )
	//{
	//	SDboEventIconPopupResult* pData = reinterpret_cast<SDboEventIconPopupResult*>( msg.pData );

	//	if( pData->nSrcPlace != PLACE_BAGSLOT )
	//		return;

	//	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pData->uiSerial ) );

	//	if( pData->nWorkId == PMW_PICKUP )
	//	{
	//		if( IsEnableBagPickUp( pData->nSrcSlotIdx ) )
	//		{
	//			CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pData->uiSerial ) );
	//			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
	//			GetIconMoveManager()->IconMovePickUp( pData->uiSerial, PLACE_BAGSLOT, pData->nSrcSlotIdx, pItemAttr->GetStackNum(), pItem->GetIcon()->GetImage() );
	//		}
	//	}
	//	else if( pData->nWorkId == PMW_CLEAR )
	//	{
	//		Logic_ItemBagClearProc( pData->uiSerial, pData->nSrcSlotIdx );
	//	}	
	//	else if( pData->nWorkId == PMW_OPEN )
	//	{
	//		if( !GetDialogManager()->IsOpenDialog( pData->nSrcSlotIdx + DIALOG_CAPSULE_1 ) )
	//			GetDialogManager()->SwitchDialog( pData->nSrcSlotIdx + DIALOG_CAPSULE_1 );
	//	}
	//}
	else if( msg.Id == g_EventEnableItemIcon )
	{
		SDboEventEnableItemIcon* pData = reinterpret_cast<SDboEventEnableItemIcon*>( msg.pData );

		if( pData->ePlace != PLACE_BAGSLOT )
			return;

		ShowDisableSlot( !pData->bEnable, pData->uiSlotIdx );
	}
	else if( msg.Id == g_EventItemCreatedEffect )
	{
		SDboEventItemCreatedEffect* pData = reinterpret_cast<SDboEventItemCreatedEffect*>( msg.pData );

		if( pData->ucBagIdx >= VISIBLE_BAGSLOT_COUNT )
			return;

		CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

		// 가방 체크는 패킷핸들러에서 미리 함.
		CNtlSobItem* pGetItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pData->hSerial ) );
		if( pGetItem )
		{
			CNtlSobIcon* pGetItemIcon = pGetItem->GetIcon();
			CRectangle* pRect = &m_rtIcon[pData->ucBagIdx];
			CRectangle rtScreen = m_pThis->GetScreenRect();

			CGetItemEffect* pGetItemEffect = NTL_NEW CGetItemEffect();
			pGetItemEffect->Create();
			pGetItemEffect->StartProc( rtScreen.left + pRect->left, rtScreen.top + pRect->top, (gui::CTexture*)pGetItemIcon->GetImage(), m_listGetItemEffect.size() * BAG_ITEM_GET_EFFECT_DELAY_TIME, pData->hSerial );
			m_listGetItemEffect.push_back( pGetItemEffect );
		}		
	}
	else if( msg.Id == g_EventZennyLootingEffect )
	{
		SDboEventZennyLootingEffect* pData = reinterpret_cast<SDboEventZennyLootingEffect*>( msg.pData );

		if( m_pBag[0] )
		{
			// 돈은 기본 슬롯으로만 들어간다.
			CRectangle* pRect = &m_rtIcon[0];
			CRectangle rtScreen = m_pThis->GetScreenRect();
			CBasicBagGui* pBasicBag = reinterpret_cast<CBasicBagGui*>( m_pBag[0] );

			CGetItemEffect* pGetItemEffect = NTL_NEW CGetItemEffect();
			pGetItemEffect->Create();
			pGetItemEffect->StartProc( rtScreen.left + pRect->left, rtScreen.top + pRect->top, pBasicBag->GetMoneyIcon(), m_listGetItemEffect.size() * BAG_ITEM_GET_EFFECT_DELAY_TIME, INVALID_SERIAL_ID );
			m_listGetItemEffect.push_back( pGetItemEffect );
		}		
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		ClearBag();
	}
	else if( msg.Id == g_EventSobItemDelete )
	{
		SNtlEventSobItemDelete* pSobItemDelete = reinterpret_cast<SNtlEventSobItemDelete*>( msg.pData );

		if( pSobItemDelete->byPlace >= CONTAINER_TYPE_BAG_FIRST && pSobItemDelete->byPlace <= CONTAINER_TYPE_BAG_LAST )
			RemoveGetItemEffect( pSobItemDelete->hItemSerialId );
	}
}

VOID CBagSlotGui::ClearBag(VOID)
{
	for( RwInt32 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
		ClearBagSlot( i );
	}
}

VOID CBagSlotGui::ClearBagSlot( RwInt32 nSlotIdx )
{
	NTL_ASSERT( nSlotIdx >= 0 && nSlotIdx < NTL_MAX_BAGSLOT_COUNT, "CBasicBagGui::ClearBagSlot : Invalid BagSlot" );

	if( m_pBag[nSlotIdx] )
	{
		m_pBag[nSlotIdx]->Destroy();
		NTL_DELETE( m_pBag[nSlotIdx] );	
		GetDialogManager()->UnRegistDialog( nSlotIdx + DIALOG_CAPSULE_1 );	
		m_hBagSerial[nSlotIdx] = INVALID_SERIAL_ID;

		if( nSlotIdx < VISIBLE_BAGSLOT_COUNT )
		{	
			m_surIcon[nSlotIdx].SetTexture( (gui::CTexture*)NULL );
			ShowDisableSlot( FALSE, nSlotIdx );				
			DestroyStackNumber( nSlotIdx );							
		}				
	}	
}

VOID CBagSlotGui::SetSlotRectHardCode(VOID)
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();
	CRectangle rtScreen = m_pThis->GetScreenRect();
	
	for( RwInt32 i = 0 ; i < VISIBLE_BAGSLOT_COUNT; i++ )
	{
		m_rtIcon[i].SetRectWH( i * 35 + 1, 3, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE );
		m_surIcon[i].SetRect( rtScreen.left + m_rtIcon[i].left, rtScreen.top + m_rtIcon[i].top,
							  rtScreen.left + m_rtIcon[i].right, rtScreen.top + m_rtIcon[i].bottom );

		m_surSlot[i].SetSurface( pSurfaceManager->GetSurface( "GameCommon.srf", "srfIconSlotBack" ) );
		m_surSlot[i].SetRect( rtScreen.left + m_rtIcon[i].left, rtScreen.top + m_rtIcon[i].top,
							  rtScreen.left + m_rtIcon[i].right + 2, rtScreen.top + m_rtIcon[i].bottom + 4 );

		m_surFocusSlot[i].SetSurface( pSurfaceManager->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
		m_surFocusSlot[i].SetRect( rtScreen.left + m_rtIcon[i].left, rtScreen.top + m_rtIcon[i].top,
							   rtScreen.left + m_rtIcon[i].right, rtScreen.top + m_rtIcon[i].bottom );

		m_surDisableSlot[i].SetSurface( pSurfaceManager->GetSurface( "GameCommon.srf", "srfSlotDisableEffect" ) );
		m_surDisableSlot[i].SetRect( rtScreen.left + m_rtIcon[i].left, rtScreen.top + m_rtIcon[i].top,
							   rtScreen.left + m_rtIcon[i].right, rtScreen.top + m_rtIcon[i].bottom );

		m_surDisableSlot[i].Show( false );
		
		m_surFritzSlot[i].Show( false );
	}
}

RwInt32 CBagSlotGui::GetBagSlotIdx( RwInt32 nX, RwInt32 nY )
{
	for( RwInt32 i = 0 ; i < VISIBLE_BAGSLOT_COUNT ; ++i )
	{
		if( m_rtIcon[i].PtInRect( CPos( nX, nY ) ) )
			return i;
	}

	return -1;
}

CRectangle CBagSlotGui::GetBagSlotRect( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 || nSlotIdx >= VISIBLE_BAGSLOT_COUNT )
		return CRectangle( 0, 0, 0, 0 );

	return m_rtIcon[nSlotIdx];
}

RwBool CBagSlotGui::IsEnableBagPickUp( RwInt32 nSlotIdx )
{
	// 0. BasicItem은 이동불가
	// 1. 가방이 슬롯안에 존재하는가
	// 3. 이동이 가능한가
	if( nSlotIdx < 0 || nSlotIdx >= VISIBLE_BAGSLOT_COUNT )
		return FALSE;

	if (nSlotIdx == BAGSLOT_POSITION_BAGSLOT_POSITION_0)
	{
		GetAlarmManager()->AlarmMessage("DST_MAIN_CAPSULEKIT_CANT_MOVE");
		return FALSE;
	}

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	SERIAL_HANDLE hSlotSerial = pInventory->GetBagItem( nSlotIdx );
	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSlotSerial ) );

	if( !pSobItem )
		return FALSE;

	if (!pSobItem->EmptyChild())
	{
		GetAlarmManager()->AlarmMessage("DST_DO_EMPTY_CAPSULEKIT");
		return FALSE;
	}

	// 4. Disable상태가 아니어야
	if( IsDisableSlot( nSlotIdx ) )
		return FALSE;

	CNtlSobIcon* pIcon = pSobItem->GetIcon();
	if( !pIcon->IsDragAndDropPossible() )
		return FALSE;

	return TRUE;
}

RwBool CBagSlotGui::IsEnableBagPutDown( RwInt32 nSlotIdx )
{
	if( !GetIconMoveManager()->IsEnable() )
		return FALSE;

	if( nSlotIdx < 0 || nSlotIdx >= VISIBLE_BAGSLOT_COUNT )
		return FALSE;

	if( IsDisableSlot( nSlotIdx ) )
		return FALSE;

	return TRUE;
}

RwBool CBagSlotGui::IsEnableBagOpenClose( RwInt32 nSlotIdx )
{
	if( !IsEnableBagPutDown( nSlotIdx ) )
		return FALSE;

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

	if( pInventory->GetBagItem( nSlotIdx ) == INVALID_SERIAL_ID )
		return FALSE;

	return TRUE;
}

//RwBool CBagSlotGui::IsEnableBagPopup( RwInt32 nSlotIdx )
//{
//	if( nSlotIdx < 0 )
//		return FALSE;
//
//	if( m_surDisable[nSlotIdx].IsShow() )
//		return FALSE;
//
//	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
//	SERIAL_HANDLE hSlotSerial = pInventory->GetBagItem( nSlotIdx );
//	
//	if( hSlotSerial == INVALID_SERIAL_ID )
//		return FALSE;
//
//	return TRUE;
//}

RwBool CBagSlotGui::UpdateData(VOID)
{
	NTL_FUNCTION( "CBagSlotGui::UpdateData" );

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	if( !pInventory )
		NTL_RETURN( FALSE );

	for( RwInt32 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
		SERIAL_HANDLE hSerial = pInventory->GetBagItem( i );
		
		// 변경된 경우. 생성 삭제
		if( m_hBagSerial[i] != hSerial )
		{
			m_hBagSerial[i] = hSerial;
	
			if( m_pBag[i] )
			{
				m_pBag[i]->Destroy();
				NTL_DELETE( m_pBag[i] );	
				GetDialogManager()->UnRegistDialog( i + DIALOG_CAPSULE_1 );				
			}

			if( hSerial == INVALID_SERIAL_ID )
			{
				if( i < VISIBLE_BAGSLOT_COUNT )
				{
					m_surIcon[i].SetTexture( (gui::CTexture*)NULL );
					ShowDisableSlot( FALSE, i );								
				}				
			}
			else
			{
				CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSerial ) );
				CNtlSobItemAttr* pBagAttr = reinterpret_cast<CNtlSobItemAttr*>( pBag->GetSobAttr() );

				if( i < VISIBLE_BAGSLOT_COUNT )
					m_surIcon[i].SetTexture( (gui::CTexture*)pBag->GetIcon()->GetImage() );

				switch( pBagAttr->GetChildSlotSize() )
				{
					case ITEM_BAG_SIZE_4_BY_1: m_pBag[i] = NTL_NEW CBag4SpaceGui; break;
					case ITEM_BAG_SIZE_4_BY_2: m_pBag[i] = NTL_NEW CBag8SpaceGui; break;
					case ITEM_BAG_SIZE_4_BY_3: m_pBag[i] = NTL_NEW CBag12SpaceGui; break;

					case ITEM_BAG_SIZE_4_BY_4:
						if( i == BAGSLOT_POSITION_BAGSLOT_POSITION_0 )	
							m_pBag[i] = NTL_NEW CBasicBagGui; 
						else 
							m_pBag[i] = NTL_NEW CBag16SpaceGui; 										
						break;

					case ITEM_BAG_SIZE_4_BY_5: m_pBag[i] = NTL_NEW CBag20SpaceGui; break;
					case ITEM_BAG_SIZE_4_BY_6: m_pBag[i] = NTL_NEW CBag24SpaceGui; break;
					case ITEM_BAG_SIZE_4_BY_7: m_pBag[i] = NTL_NEW CBag28SpaceGui; break;
					case ITEM_BAG_SIZE_4_BY_8: m_pBag[i] = NTL_NEW CBag32SpaceGui; break;

					default: NTL_ASSERT( NULL, "Invalid Bag Size" ); NTL_RETURN( FALSE );
				}

				if( !m_pBag[i]->Create() )
				{
					m_pBag[i]->Destroy();
					NTL_DELETE( m_pBag[i] );
					NTL_RETURN( FALSE );
				}

				m_pBag[i]->Show( false );
				GetDialogManager()->RegistDialog( i + DIALOG_CAPSULE_1, m_pBag[i], &CBagBaseGui::SwitchDialog );
				m_pBag[i]->SetBagSerial( hSerial );
				m_pBag[i]->SetBagPos( i );								
			}
		}		

		if( i < VISIBLE_BAGSLOT_COUNT )
		{
			// 가방의 상태
			if( hSerial == INVALID_SERIAL_ID )
			{
				DestroyStackNumber( i );
				FritzEffect( FRITZ_NONE, i );
			}
			else
			{
				CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSerial ) );
				CNtlSobItemAttr* pBagAttr = reinterpret_cast<CNtlSobItemAttr*>( pBag->GetSobAttr() );

				if( m_apstbStackNum[i] )
				{
					m_apstbStackNum[i]->Format( L"%d/%d", pBag->GetValidChildNum(), pBag->GetChildNum() );
				}
				else
				{
					CreateStackNumber( i, pBag->GetChildNum(), pBag->GetValidChildNum() );
				}

				if( pBagAttr->IsExpired() )
					FritzEffect( FRITZ_ABSOLUTE, i );
				else
					FritzEffect( FRITZ_NONE, i );
			}
		}		
	}

	if( m_nMouseOnIndex >= 0 && GetInfoWndManager()->GetRequestGui() == DIALOG_BAGSLOT )
	{
		CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pInventory->GetBagItem( m_nMouseOnIndex ) ) );
		CRectangle rtScreen = m_pThis->GetScreenRect();

		RwInt32 nX = m_rtIcon[m_nMouseOnIndex].left + rtScreen.left;
		RwInt32 nY = m_rtIcon[m_nMouseOnIndex].top + rtScreen.top;

		if( pBag )
		{
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, nX, nY,	(void*)GetDisplayStringManager()->GetString( "DST_BAG_SLOT" ), DIALOG_BAGSLOT );												
		}
		else
		{
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM, nX, nY, (void*)pBag, DIALOG_BAGSLOT );
		}
	}

	NTL_RETURN( TRUE );
}

VOID CBagSlotGui::ShowIconDestination(VOID)
{
	if( GetIconMoveManager()->IsActive() )
	{
		if( GetIconMoveManager()->GetSrcPlace() == PLACE_SKILL )
			return;

		CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( GetIconMoveManager()->GetSrcSerial() ) );
		CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
		
		if( pSobItem->IsBagItem() )
		{
			for( RwInt32 i = 0 ; i < VISIBLE_BAGSLOT_COUNT ; ++i )
			{
				if( pInventory->GetBagItem( i ) == INVALID_SERIAL_ID ||
					pInventory->GetBagItem( i ) == GetIconMoveManager()->GetSrcSerial() )
				{
					m_arrFocusEffect[i] = m_arrFocusEffect[i] | SLOT_FOCUS_CAN_MOVE;
				}
				else 
				{
					m_arrFocusEffect[i] = m_arrFocusEffect[i] & !SLOT_FOCUS_CAN_MOVE;
				}
			}
		}
		else
		{
			for( RwInt32 i = 0 ; i < VISIBLE_BAGSLOT_COUNT ; ++i )
			{
				SERIAL_HANDLE hBagItem = pInventory->GetBagItem( i );
				if( hBagItem == INVALID_SERIAL_ID )
					continue;

				CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hBagItem ) );
												
				if( !pBagItem->FullChild() && pBagItem->GetSerialID() != pSobItem->GetParentItemSerial() )
				{
					m_arrFocusEffect[i] = m_arrFocusEffect[i] | SLOT_FOCUS_CAN_MOVE;
				}
				else
				{
					m_arrFocusEffect[i] = m_arrFocusEffect[i] & !SLOT_FOCUS_CAN_MOVE;
				}
			}
		}
	}
	else
	{
		for( RwInt32 i = 0 ; i < VISIBLE_BAGSLOT_COUNT ; ++i )
		{
			m_arrFocusEffect[i] = m_arrFocusEffect[i] & !SLOT_FOCUS_CAN_MOVE;
		}
	}
}

RwBool CBagSlotGui::CreateStackNumber( RwInt32 nSlotIdx, RwInt32 nSlotCount, RwInt32 nItemCount )
{
	DBO_ASSERT( nSlotIdx >= 0 && nSlotIdx < VISIBLE_BAGSLOT_COUNT, "CBagSlotGui::CreateStackNumbert : Invalid array index" );

	NTL_FUNCTION( "CBagSlotGui::CreateStackNumber" );

	CRectangle rect;
	rect.SetRect( m_rtIcon[nSlotIdx].left, m_rtIcon[nSlotIdx].bottom - DBOGUI_STACKNUM_HEIGHT, m_rtIcon[nSlotIdx].left + DBOGUI_ICON_SIZE, m_rtIcon[nSlotIdx].bottom );
	m_apstbStackNum[nSlotIdx] = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN );
			
	if( !m_apstbStackNum[nSlotIdx] )
		NTL_RETURN( FALSE );

	m_apstbStackNum[nSlotIdx]->CreateFontStd( DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR );
	m_apstbStackNum[nSlotIdx]->SetEffectMode( TE_OUTLINE );
	m_apstbStackNum[nSlotIdx]->Format( L"%d/%d", nItemCount, nSlotCount );
	m_apstbStackNum[nSlotIdx]->Enable( false );

	NTL_RETURN( TRUE );
}

VOID CBagSlotGui::DestroyStackNumber( RwInt32 nSlotIdx )
{
	DBO_ASSERT( nSlotIdx >= 0 && nSlotIdx < VISIBLE_BAGSLOT_COUNT, "CBagSlotGui::DestroyStackNumber : Invalid array index" );

	if( m_apstbStackNum[nSlotIdx] )
		NTL_DELETE( m_apstbStackNum[nSlotIdx] );
}

VOID CBagSlotGui::ClickEffect( RwBool bPush, RwInt32 nSlotIdx /* = -1  */)
{
	if( bPush )
		DBO_ASSERT( nSlotIdx >= 0 && nSlotIdx < VISIBLE_BAGSLOT_COUNT, "CBagSlotGui::ClickEffect : Invalid array index" );

	CRectangle rtScreen = m_pThis->GetScreenRect();

	if( bPush )
	{
		m_surIcon[nSlotIdx].SetRect( rtScreen.left + m_rtIcon[nSlotIdx].left + ICONPUSH_SIZEDIFF, rtScreen.top + m_rtIcon[nSlotIdx].top + ICONPUSH_SIZEDIFF,
									 rtScreen.left + m_rtIcon[nSlotIdx].right - ICONPUSH_SIZEDIFF, rtScreen.top + m_rtIcon[nSlotIdx].bottom - ICONPUSH_SIZEDIFF );
	}
	else if( m_nPushDownIndex >= 0 )
	{
		m_surIcon[m_nPushDownIndex].SetRect( rtScreen.left + m_rtIcon[m_nPushDownIndex].left, rtScreen.top + m_rtIcon[m_nPushDownIndex].top,
											 rtScreen.left + m_rtIcon[m_nPushDownIndex].right, rtScreen.top + m_rtIcon[m_nPushDownIndex].bottom );
	}	

	m_nPushDownIndex = nSlotIdx;
}

VOID CBagSlotGui::FritzEffect( eFRITZEFFECT eFritzState, RwInt32 nSlotIdx )
{
	DBO_ASSERT( nSlotIdx >= 0 && nSlotIdx < VISIBLE_BAGSLOT_COUNT, "CBagSlotGui::FritzEffect : Invalid array index" );

	switch( eFritzState )
	{
	case FRITZ_NONE:
		m_surFritzSlot[nSlotIdx].Show( FALSE );
		return;
	case FRITZ_HALF:
		m_surFritzSlot[nSlotIdx].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfHalfFritzSlotEffect" ) );
		break;
	case FRITZ_ABSOLUTE:
		m_surFritzSlot[nSlotIdx].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfFritzSlotEffect" ) );
		break;
	}

	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_surFritzSlot[nSlotIdx].SetRect( rtScreen.left + m_rtIcon[nSlotIdx].left, rtScreen.top + m_rtIcon[nSlotIdx].top,
									  rtScreen.left + m_rtIcon[nSlotIdx].right, rtScreen.top + m_rtIcon[nSlotIdx].bottom );

	m_surFritzSlot[nSlotIdx].Show( TRUE );
}

VOID CBagSlotGui::ShowDisableSlot( RwBool bShow, RwInt32 nSlotIdx )
{
	DBO_ASSERT( nSlotIdx >= 0 && nSlotIdx < NTL_MAX_BAGSLOT_COUNT, "CBagSlotGui::ShowDisableSlot : Invalid array index" );

	if( nSlotIdx >= 0 && nSlotIdx < VISIBLE_BAGSLOT_COUNT )
		m_surDisableSlot[nSlotIdx].Show( bShow );	

	if( m_pBag[nSlotIdx] )
		m_pBag[nSlotIdx]->SetBagEnable( !bShow );
}

RwBool CBagSlotGui::IsDisableSlot( RwInt32 nSlotIdx )
{
	DBO_ASSERT( nSlotIdx >= 0 && nSlotIdx < VISIBLE_BAGSLOT_COUNT, "CBagSlotGui::IsDisableSlot : Invalid array index" );

	return m_surDisableSlot[nSlotIdx].IsShow() ? TRUE : FALSE;
}

VOID CBagSlotGui::ClearGetItemEffect(VOID)
{
	std::list<CGetItemEffect*>::iterator it;
	for( it = m_listGetItemEffect.begin() ; it != m_listGetItemEffect.end() ; ++it )
	{
		CGetItemEffect* pGetItemEffect = (*it);
		NTL_DELETE( pGetItemEffect );
	}
	m_listGetItemEffect.clear();
}

VOID CBagSlotGui::RemoveGetItemEffect( RwUInt32 hSerial )
{
	std::list<CGetItemEffect*>::iterator it;
	for( it = m_listGetItemEffect.begin() ; it != m_listGetItemEffect.end() ; ++it )
	{
		CGetItemEffect* pGetItemEffect = (*it);
		if( pGetItemEffect->GetSerial() == hSerial )
		{
			NTL_DELETE( pGetItemEffect );
			m_listGetItemEffect.erase( it );
			return;
		}
	}	
}

VOID CBagSlotGui::OnMouseDown( const CKey& key )
{
	RwInt32 nClickIdx = GetBagSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );
	if( nClickIdx >= 0 && !GetIconMoveManager()->IsActive() )
		ClickEffect( TRUE, nClickIdx );

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( GetIconMoveManager()->IsActive() )
		{
			if( IsEnableBagPutDown( nClickIdx ) )
				m_nLSelectedSlotIdx = nClickIdx;
		}
		else
		{
			if( IsEnableBagPickUp( nClickIdx ) )
				m_nLSelectedSlotIdx = nClickIdx;
		}
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( !GetIconMoveManager()->IsActive() )
		{
			if( IsEnableBagOpenClose( nClickIdx ) )
				m_nRSelectedSlotIdx = nClickIdx;
		}

	}

	m_pThis->CaptureMouse();

	if( m_nLSelectedSlotIdx >= 0 && m_nRSelectedSlotIdx >= 0 )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		ClickEffect( FALSE );
		m_pThis->ReleaseMouse();
	}
}

VOID CBagSlotGui::OnMouseUp( const CKey& key )
{
	// 1. Left버튼인가 Right버튼인가
	// 2. Src선택인가 Dest선택인가.

	RwInt32 nSlotIdx = GetBagSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );
	ClickEffect( FALSE );

	m_pThis->ReleaseMouse();

	if( nSlotIdx < 0 || !IsShow() )
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
				GetIconMoveManager()->IconMovePutDown( PLACE_BAGSLOT, INVALID_SERIAL_ID, nSlotIdx );					
			}
			else
			{
				CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagSerial[nSlotIdx] ) );
				if( pItem )
				{
					CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
					GetIconMoveManager()->IconMovePickUp( m_hBagSerial[nSlotIdx], PLACE_BAGSLOT, nSlotIdx, pItemAttr->GetStackNum(), pItem->GetIcon()->GetImage() );				
				}			
			}
		}

		m_nLSelectedSlotIdx = -1;
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( nSlotIdx == m_nRSelectedSlotIdx )
		{
			if( GetDialogManager()->IsOpenDialog( nSlotIdx + DIALOG_CAPSULE_1 ) )
			{
				if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_CPASULE_SLOT_CLOSE ) )
					return;

				GetDialogManager()->CloseDialog( nSlotIdx + DIALOG_CAPSULE_1 );
			}
			else
			{
				if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_CPASULE_SLOT_OPEN ) )
					return;

				GetDialogManager()->OpenDialog( nSlotIdx + DIALOG_CAPSULE_1 );
			}

			//CRectangle rtScreen = m_pThis->GetScreenRect();
			//SERIAL_HANDLE hSrcSerial = GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetBagItem( nSlotIdx );
			//CDboEventGenerator::IconPopupShow( TRUE, hSrcSerial, PLACE_BAGSLOT, nSlotIdx, m_rtIcon[nSlotIdx].left + rtScreen.left, m_rtIcon[nSlotIdx].top + rtScreen.top );
		}

		m_nRSelectedSlotIdx = -1;
	}
}

VOID CBagSlotGui::OnMouseOut( gui::CComponent* pComponent )
{
	for( RwInt32 i = 0 ; i < VISIBLE_BAGSLOT_COUNT ; ++i )
	{
		m_arrFocusEffect[i] = m_arrFocusEffect[i] & ~SLOT_FOCUS;
	}

	if( m_nMouseOnIndex != -1 )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		if( m_nPushDownIndex >= 0 )
			ClickEffect( FALSE, m_nPushDownIndex );
		m_nMouseOnIndex = -1;
	}
}

VOID CBagSlotGui::OnMouseMove( INT nKey, INT nXPos, INT nYPos )
{
	RwBool bFlag = FALSE;

	for( RwInt32 i = 0 ; i < VISIBLE_BAGSLOT_COUNT ; ++i )
	{
		if( m_rtIcon[i].PtInRect( nXPos, nYPos ) )
		{
			m_arrFocusEffect[i] = m_arrFocusEffect[i] | SLOT_FOCUS;
			
			if( m_nMouseOnIndex != i )
			{
				SERIAL_HANDLE hSerial = GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetBagItem( i );
				CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSerial ) );
				CRectangle rtScreen = m_pThis->GetScreenRect();
				RwInt32 nX = m_rtIcon[i].left + rtScreen.left;
				RwInt32 nY = m_rtIcon[i].top + rtScreen.top;
				
				if( pBag )
				{
					GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM, nX, nY, pBag, DIALOG_BAGSLOT );
				}
				else
				{
					GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, nX, nY,	(void*)GetDisplayStringManager()->GetString( "DST_BAG_SLOT" ), DIALOG_BAGSLOT );																	
				}

				if( i == m_nPushDownIndex )
					ClickEffect( TRUE, i );
				else if( m_nPushDownIndex >= 0 )
					ClickEffect( FALSE, m_nPushDownIndex );

				m_nMouseOnIndex = i;
			}
			bFlag = TRUE;
		}
		else
		{
			m_arrFocusEffect[i] = m_arrFocusEffect[i] & ~SLOT_FOCUS;
		}
	}

	if( !bFlag && m_nMouseOnIndex != -1 )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		if( m_nPushDownIndex >= 0 )
			ClickEffect( FALSE, m_nPushDownIndex );
		m_nMouseOnIndex = -1;
	}
}

VOID CBagSlotGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 i = 0 ; i < VISIBLE_BAGSLOT_COUNT ; ++i )
	{
		m_surSlot[i].SetPosition( rtScreen.left + m_rtIcon[i].left, rtScreen.top + m_rtIcon[i].top );
		m_surIcon[i].SetPosition( rtScreen.left + m_rtIcon[i].left, rtScreen.top + m_rtIcon[i].top );
		m_surFocusSlot[i].SetPosition( rtScreen.left + m_rtIcon[i].left, rtScreen.top + m_rtIcon[i].top );
		m_surDisableSlot[i].SetPosition( rtScreen.left + m_rtIcon[i].left, rtScreen.top + m_rtIcon[i].top );
		m_surFritzSlot[i].SetPosition( rtScreen.left + m_rtIcon[i].left, rtScreen.top + m_rtIcon[i].top );
	}
}

VOID CBagSlotGui::OnPaint(VOID)
{
	for( RwInt32 i = 0 ; i < VISIBLE_BAGSLOT_COUNT ; ++i )
	{
		m_surSlot[i].Render();

		if( m_surIcon[i].GetTexture() )
			m_surIcon[i].Render();

		m_surFritzSlot[i].Render();
		m_surDisableSlot[i].Render();
		
		if( m_arrFocusEffect[i] )
			m_surFocusSlot[i].Render();				
	}

	std::list<CGetItemEffect*>::iterator it;
	for( it = m_listGetItemEffect.begin() ; it != m_listGetItemEffect.end() ; ++it )
	{
		CGetItemEffect* pGetItemEffect = *it;
		pGetItemEffect->Render();
	}
}