#include "precomp_dboclient.h"
#include "NetPyShopCartGui.h"

// core
#include "NtlDebug.h"

// shared
#include "ItemTable.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSLLogic.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"

// dbo
#include "IconMoveManager.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"
#include "InfoWndManager.h"
#include "DialogManager.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboPacketGenerator.h"
#include "DboEventGenerator.h"
#include "NPCShop.h"

namespace
{
#define dMAX_TRADE_OVERLAP_COUNT			20	///< 한 슬롯당 거래할 수 있는 최대 아이템 갯수

#define dGUI_BUY_SELL_SLOT_GAP				97
#define dGUI_SLOT_HORI_GAP					42
#define dGUI_SLOT_VERT_GAP					57
#define dGUI_BUTTON_HORI_GAP				19
}


CNetPyShopCartGui::CNetPyShopCartGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pBuyButton(NULL)
,m_pTotalBuyMoney(NULL)
,m_pTotalSellMoney(NULL)
,m_uiTotalBuyPrice(0)
,m_bFocus(FALSE)
,m_byInfoWindowIndex(INVALID_BYTE)
,m_pMoneyIconTexture(NULL)
{
	for(RwInt32 i = 0 ; i < SLOTKIND_NUM ; ++i )
	{
		for(RwInt32 j = 0 ; j < MAX_SLOT ; ++j )
		{
			m_pUpButton[i][j] = NULL;
			m_pDownButton[i][j] = NULL;
		}
	}
}

CNetPyShopCartGui::~CNetPyShopCartGui()
{
	NTL_FUNCTION("CNetPyShopCartGui::Destroy");

	Destroy();

	NTL_RETURNVOID();
}

RwBool CNetPyShopCartGui::Create()
{
	NTL_FUNCTION( "CNetPyShopCartGui::Create" );

	char acSurfaceName[64];

	if(!CNtlPLGui::Create("", "gui\\NetPyShopCartGui.srf", "gui\\NetPyShopCartGui.frm"))
		NTL_RETURN(FALSE);

	sprintf_s(acSurfaceName, 64, "NetPyShopCartGui.srf");

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	m_pMoneyIconTexture = Logic_CreateTexture( MONEYICON_NAME );

	// 다이얼로그 이름 스태틱
	rect.SetRectWH(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y, 130, 14);
	m_pDialogName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pDialogName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pDialogName->Enable(false);	

	// Exit Button
	m_pExitButton = (gui::CButton*)GetComponent( "ExitButton" );
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CNetPyShopCartGui::ClickedCloseButton);

	// 구입 버튼
	m_pBuyButton = (gui::CButton*)GetComponent( "BuyButton" );	
	m_slotClickedBuy = m_pBuyButton->SigClicked().Connect(this, &CNetPyShopCartGui::ClickedBuyButton);	

	// 아이템 갯수 더하기/빼기 버튼
	RwInt32 iButtonX = 16;
	for( RwInt32 i = 0 ; i < SLOTKIND_NUM ; ++i )
	{		
		RwInt32 iButtonY = 93;

		for( RwInt32 j = 0 ; j < MAX_SLOT ; ++j )
		{
			if( (j%2) == 0 )
				iButtonX = 16;
			else
				iButtonX = 16 + dGUI_SLOT_HORI_GAP;

			// 아이템 갯수 더하기 버튼
			rect.SetRectWH(iButtonX, iButtonY, 18, 15);
			m_pUpButton[i][j] = NTL_NEW gui::CButton(rect, "",
				GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfUpButtonUp" ),
				GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfUpButtonDown" ),
				GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfUpButtonDis" ),
				GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfUpButtonFocus" ),
				NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
				GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, m_pThis, GetNtlGuiManager()->GetSurfaceManager() );
			m_slotUpButton[i][j] = m_pUpButton[i][j]->SigClicked().Connect(this, &CNetPyShopCartGui::ClickUpButton);

			// 아이템 갯수 빼기 버튼
			rect.SetRectWH(iButtonX + dGUI_BUTTON_HORI_GAP, iButtonY, 18, 15);
			m_pDownButton[i][j] = NTL_NEW gui::CButton(rect, "",
				GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfDownButtonUp" ),
				GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfDownButtonDown" ),
				GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfDownButtonDis" ),
				GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfDownButtonFocus" ),
				NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
				GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, m_pThis, GetNtlGuiManager()->GetSurfaceManager() );

			m_slotDownButton[i][j] = m_pDownButton[i][j]->SigClicked().Connect(this, &CNetPyShopCartGui::ClickDownButton);

			if( (j%2) != 0 )
			{
				iButtonY += dGUI_SLOT_VERT_GAP;
			}
		}

		iButtonX = 113;
	}	

	// Slots
	RwInt32 iSlotX;
	RwInt32 iSlotY = 57;
	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{	
		if( (i%2) == 0 )
			iSlotX = 18;
		else
			iSlotX = 18 + dGUI_SLOT_HORI_GAP;


		m_BuySlotInfo[i].slot.Create(m_pThis, DIALOG_SHOPING_CART, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
		m_BuySlotInfo[i].slot.SetPosition_fromParent(iSlotX, iSlotY);

		if( (i%2) != 0 )
			iSlotY += dGUI_SLOT_VERT_GAP;
	}

	// 총 구입 금액
	rect.SetRectWH( 14, 411, 61, 16);
	m_pTotalBuyMoney = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
	m_pTotalBuyMoney->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pTotalBuyMoney->SetText( "0");
	m_pTotalBuyMoney->Enable(false);

	// 슬롯 포커스 이펙트
	m_FocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );

	// Sig
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CNetPyShopCartGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect(this, &CNetPyShopCartGui::OnMouseUp);
	m_slotMove			= m_pThis->SigMove().Connect( this, &CNetPyShopCartGui::OnMove );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CNetPyShopCartGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CNetPyShopCartGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CNetPyShopCartGui::OnPaint );
	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CNetPyShopCartGui::OnCaptureWheelMove );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CNetPyShopCartGui::OnCaptureMouseDown );

	LinkMsg(g_EventShop);
	LinkMsg(g_EventUserTrade);
	LinkMsg(g_EventCalcPopupResult);
	LinkMsg(g_EventCharObjDelete);
	LinkMsg( g_EventNetPyShopEvent );

	Show(false);

	Clear();

	NTL_RETURN(TRUE);
}

VOID CNetPyShopCartGui::Destroy()
{
	NTL_FUNCTION( "CNetPyShopCartGui::Destroy" );

	CheckInfoWindow();

	Logic_DeleteTexture(m_pMoneyIconTexture);
	m_pMoneyIconTexture = NULL;

	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		m_BuySlotInfo[i].slot.Destroy();
	}

	UnLinkMsg(g_EventShop);
	UnLinkMsg(g_EventUserTrade);
	UnLinkMsg(g_EventCalcPopupResult);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg( g_EventNetPyShopEvent );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CNetPyShopCartGui::Clear()
{
	m_MouseDownSlot.iSlotKind = dNETPYSHOP_CART_INVALID_INDEX;
	m_MouseDownSlot.iSlotIndex = dNETPYSHOP_CART_INVALID_INDEX;

	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		ClearSlot(BUY_SLOT, i);
	}

	CalcTotalBuyPrice();
}

VOID CNetPyShopCartGui::ClearSlot(RwInt32 iSlotKind, RwInt32 iSlot)
{
	m_BuySlotInfo[iSlot].slot.Clear();
	memset((void*)&m_BuySlotInfo[iSlot].NPCShopBuyInfo, 0, sizeof(sSHOP_BUY_CART) );
	m_BuySlotInfo[iSlot].NPCShopBuyInfo.byItemPos = INVALID_BYTE;
		
	m_MouseDownSlot.iSlotKind = dNETPYSHOP_CART_INVALID_INDEX;
	m_MouseDownSlot.iSlotIndex = dNETPYSHOP_CART_INVALID_INDEX;
}

VOID CNetPyShopCartGui::OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos)
{
	if( !IsShow() )
		return;	

	if( m_pThis->GetParent()->GetChildComponentReverseAt( pos.x, pos.y ) != m_pThis )
		return;

	if( m_pThis->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT )
		return;


	RwInt32 iSlot;
	CRectangle rtScreen = m_pThis->GetScreenRect();

	iSlot = PtinSlot(BUY_SLOT, pos.x - rtScreen.left, pos.y - rtScreen.top);
	if( iSlot != dNETPYSHOP_CART_INVALID_INDEX )
	{
		if( sDelta > 0 )
			AddItem(BUY_SLOT, iSlot, 1);
		else
			SubItem(BUY_SLOT, iSlot, 1);
		return;
	}
}

VOID CNetPyShopCartGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE_TOP_LINKED(DIALOG_NETPYSHOP_TRADE, key.m_fX, key.m_fY);
}

VOID CNetPyShopCartGui::ClickedCloseButton(gui::CComponent* pComponent)
{
	GetDboGlobal()->GetGamePacketGenerator()->SendShopNetPyItemEndReq();
}

VOID CNetPyShopCartGui::ClickedBuyButton(gui::CComponent* pComponent)
{
	sSHOP_BUY_CART aBuyCart[MAX_SLOT];
	RwUInt8 byCount = 0;
	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( m_BuySlotInfo[i].NPCShopBuyInfo.byItemPos != INVALID_BYTE )
		{
			memcpy( &aBuyCart[byCount], &m_BuySlotInfo[i].NPCShopBuyInfo, sizeof( sSHOP_BUY_CART ) );
			byCount++;
		}
	}

	// 카운트가 없다면 아무것도 사지 않는 다는 것이다.
	if( byCount == 0 )
		return;

	// 사는 패킷 보내기
	GetDboGlobal()->GetGamePacketGenerator()->SendShopNetPyItemBuyReq( byCount, aBuyCart );	
}

VOID CNetPyShopCartGui::AddItemCount(RwInt32 iSlotKind, RwInt32 iSlot, RwInt32 iCount)
{
	if( iSlotKind == BUY_SLOT )
	{
		RwInt32 iResult = m_BuySlotInfo[iSlot].NPCShopBuyInfo.byStack + iCount;

		if( iResult > dMAX_TRADE_OVERLAP_COUNT )
			iResult = dMAX_TRADE_OVERLAP_COUNT;

		if( iResult <= 0 )
		{
			ClearSlot(BUY_SLOT, iSlot);
			return;
		}

		m_BuySlotInfo[iSlot].NPCShopBuyInfo.byStack = (RwUInt8)iResult;
		m_BuySlotInfo[iSlot].slot.SetCount(iResult);
	}
}

VOID CNetPyShopCartGui::UpdateCount(RwUInt8 bySlotKind, RwUInt8 bySlot, RwUInt8 byCount)
{
	if( bySlotKind == BUY_SLOT )
	{
		if( byCount > dMAX_TRADE_OVERLAP_COUNT )
			byCount = dMAX_TRADE_OVERLAP_COUNT;

		if( byCount <= 0 )
		{
			ClearSlot(BUY_SLOT, bySlot);
			return;
		}

		m_BuySlotInfo[bySlot].NPCShopBuyInfo.byStack = byCount;
		m_BuySlotInfo[bySlot].slot.SetCount(byCount);
	}
}

VOID CNetPyShopCartGui::CalcTotalBuyPrice()
{
	RwInt32 iTotalPrice = 0;
	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( m_BuySlotInfo[i].slot.GetSerial() != INVALID_SERIAL_ID )
		{
			sITEM_TBLDAT* pITEM_DATA = m_BuySlotInfo[i].slot.GetItemTable();
			iTotalPrice += pITEM_DATA->dwCost * m_BuySlotInfo[i].NPCShopBuyInfo.byStack;
		}
	}		

	m_uiTotalBuyPrice = iTotalPrice;
	m_pTotalBuyMoney->SetText(Logic_FormatZeni(m_uiTotalBuyPrice));

	if( m_uiTotalBuyPrice == 0 )
		m_pBuyButton->ClickEnable(false);
	else
		m_pBuyButton->ClickEnable(true);
}

VOID CNetPyShopCartGui::ClickUpButton(gui::CComponent* pComponent)
{
	for( RwUInt8 i = 0 ; i< SLOTKIND_NUM ; ++i )
	{
		for( RwUInt8 j = 0 ; j < MAX_SLOT ; ++j )
		{
			if( m_pUpButton[i][j] == pComponent )
			{
				AddItem(i, j, 1);
				break;
			}
		}
	}
}

VOID CNetPyShopCartGui::ClickDownButton(gui::CComponent* pComponent)
{
	for( RwUInt8 i = 0 ; i< SLOTKIND_NUM ; ++i )
	{
		for( RwUInt8 j = 0 ; j < MAX_SLOT ; ++j )
		{
			if( m_pDownButton[i][j] == pComponent )
			{
				SubItem(i, j, 1);
				break;
			}
		}
	}
}

VOID CNetPyShopCartGui::OnMouseDown(const CKey& key)
{
	m_MouseDownSlot.iSlotKind = dNETPYSHOP_CART_INVALID_INDEX;
	m_MouseDownSlot.iSlotIndex = dNETPYSHOP_CART_INVALID_INDEX;

	if( GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN )
		return;

	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( m_BuySlotInfo[i].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
		{
			m_MouseDownSlot.iSlotKind = BUY_SLOT;
			m_MouseDownSlot.iSlotIndex = i;

			m_pThis->CaptureMouse();
			return;
		}
	}
}

VOID CNetPyShopCartGui::OnMouseUp(const CKey& key)
{	
	m_pThis->ReleaseMouse();

	if( !IsShow() )
	{
		m_MouseDownSlot.iSlotKind = dNETPYSHOP_CART_INVALID_INDEX;
		m_MouseDownSlot.iSlotIndex = dNETPYSHOP_CART_INVALID_INDEX;
		return;
	}

	for(RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		// 구입 슬롯
		if( m_BuySlotInfo[i].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
		{				
			if( key.m_nID == UD_LEFT_BUTTON )	// 좌버튼
			{
				if( !GetIconMoveManager()->IsActive() )
					break;

				// NPC 상점에서 메세지를 보낸 경우
				if( GetIconMoveManager()->GetSrcPlace() == PLACE_NPCSHOP )
				{
					RegBuyItemByDrag(i);
					GetIconMoveManager()->IconMoveEnd();
				}					
			}
			else if( key.m_nID == UD_RIGHT_BUTTON )	// 우버튼
			{
				if( GetIconMoveManager()->IsActive() )
					break;

				if( m_MouseDownSlot.iSlotKind == BUY_SLOT &&
					m_MouseDownSlot.iSlotIndex == i )
				{
					ClearSlot(BUY_SLOT, m_MouseDownSlot.iSlotIndex);
					CalcTotalBuyPrice();
				}					
			}	

			// 해당슬롯의 아무런 처리도 하지 않았을 때
			break;
		}

	}

	m_MouseDownSlot.iSlotKind = dNETPYSHOP_CART_INVALID_INDEX;
	m_MouseDownSlot.iSlotIndex = dNETPYSHOP_CART_INVALID_INDEX;
}

VOID CNetPyShopCartGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rect = GetPosition();

	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		m_BuySlotInfo[i].slot.SetParentPosition(rect.left, rect.top);
	}	

	m_bFocus = FALSE;

	CheckInfoWindow();
}

VOID CNetPyShopCartGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	FocusEffect(FALSE);	

	// 아이템 정보를 표시하고 포커스 이펙트를 보여준다
	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{		
		if( m_BuySlotInfo[i].slot.PtInRect(nX, nY) )
		{
			// 셀렉트 포커스
			FocusEffect(TRUE, BUY_SLOT, i);

			if( GetIconMoveManager()->IsActive() )
				return;

			if( m_BuySlotInfo[i].slot.GetSerialType() == REGULAR_SLOT_ITEM_NOT_IDENTIFICATION )
			{
				if( m_byInfoWindowIndex != i )
				{
					// 미확인 아이템
					CRectangle rtScreen = m_pThis->GetScreenRect();
					GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM,
						rtScreen.left + m_BuySlotInfo[i].slot.GetX_fromParent(),
						rtScreen.top + m_BuySlotInfo[i].slot.GetY_fromParent(), NULL, DIALOG_SHOPING_CART );
					m_byInfoWindowIndex = (RwInt8)i;
				}
			}
			else if( m_BuySlotInfo[i].slot.GetSerial() != INVALID_SERIAL_ID )
			{

				if( m_byInfoWindowIndex != i )
				{
					CRectangle rtScreen = m_pThis->GetScreenRect();
					GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
						rtScreen.left + m_BuySlotInfo[i].slot.GetX_fromParent(),
						rtScreen.top + m_BuySlotInfo[i].slot.GetY_fromParent(),
						m_BuySlotInfo[i].slot.GetItemTable(), DIALOG_SHOPING_CART );
					m_byInfoWindowIndex = (RwInt8)i;
				}

			}
			else if( m_BuySlotInfo[i].slot.IsOnlyUsableIcon() )					
			{
				if( m_byInfoWindowIndex != i )
				{
					// 당신의 가방에 빈 공간이 모자랍니다
					CRectangle rtScreen = m_pThis->GetScreenRect();
					const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_TRADE_YOUR_BAG_NOT_ENOUGH");
					GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
						rtScreen.left + m_BuySlotInfo[i].slot.GetX_fromParent(),
						rtScreen.top + m_BuySlotInfo[i].slot.GetY_fromParent(),
						(VOID*)pwcText, DIALOG_SHOPING_CART );
					m_byInfoWindowIndex = (RwUInt8)i;
				}
			}

			return;
		}
	}

	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CNetPyShopCartGui::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(FALSE);
	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CNetPyShopCartGui::FocusEffect( RwBool bPush, RwInt32 iSlotKind /* =0 */, RwInt32 iSlotIdx /* = -1 */)
{
	if( bPush)
	{
		RwInt32 iX = 18;
		RwInt32 iY = 57 + (iSlotIdx/2) * dGUI_SLOT_VERT_GAP;
		CRectangle rect = m_pThis->GetScreenRect();

		if( (iSlotIdx%2) != 0 )
			iX += dGUI_SLOT_HORI_GAP;

		m_FocusEffect.SetRectWH(rect.left + iX, rect.top + iY, 32, 32);
		m_bFocus = TRUE;
	}
	else
	{
		m_bFocus = FALSE;
	}
}

VOID CNetPyShopCartGui::OpenCart()
{
	// TradeCart를 연다.
	GetDialogManager()->OpenDialog(DIALOG_NETPYSHOP_TRADE);

	// 가방을 연다
	GetDialogManager()->SwitchBag( TRUE );
}

VOID CNetPyShopCartGui::CloseCart()
{	
	GetDialogManager()->CloseDialog(DIALOG_NETPYSHOP_TRADE);

	Clear();

	m_uiTotalBuyPrice = 0;
}

VOID CNetPyShopCartGui::AddItem(RwInt32 iSlotKind, RwInt32 iSlotY, RwInt32 iCount)
{
	if( iSlotY < 0 || iSlotY >= MAX_SLOT )
	{
		NTL_ASSERT(false, "CNetPyShopCartGui::AddItem, Unknown index " << iSlotY);
		return;
	}

	if( iSlotKind == BUY_SLOT )
	{
		if( m_BuySlotInfo[iSlotY].slot.GetSerial() != INVALID_SERIAL_ID )
		{
			if( m_BuySlotInfo[iSlotY].slot.GetItemTable()->byMax_Stack >= m_BuySlotInfo[iSlotY].slot.GetCount() + iCount )
			{
				AddItemCount(BUY_SLOT, iSlotY, iCount);
				CalcTotalBuyPrice();
			}			
		}		
	}	

}

VOID CNetPyShopCartGui::SubItem(RwInt32 iSlotKind, RwInt32 iSlotY, RwInt32 iCount)
{
	if( iSlotY < 0 || iSlotY >= MAX_SLOT )
	{
		NTL_ASSERT(false, "CNetPyShopCartGui::SubItem, Unknown index " << iSlotY);
		return;
	}

	if( iSlotKind == BUY_SLOT )
	{
		if( m_BuySlotInfo[iSlotY].slot.GetSerial() != INVALID_SERIAL_ID )
		{			
			//RwInt32 iPrice = m_BuySlotInfo[iSlotY].slot.GetPrice();

			AddItemCount(BUY_SLOT, iSlotY, -iCount);			

			CalcTotalBuyPrice();
		}
	}	
}

VOID CNetPyShopCartGui::RegBuyItemByDrag(RwInt32 iSlot)
{
	sITEM_TBLDAT* pITEM_DATA = Logic_GetItemDataFromTable( GetIconMoveManager()->GetSrcSerial() );
	if(!pITEM_DATA)
		return;

	RwUInt8 byMerchantTab	= (BYTE)GetIconMoveManager()->GetEXData1();
	RwUInt8 byItemPos		= (BYTE)GetIconMoveManager()->GetSrcSlotIdx();

	// 기간제 아이템
	if( eDURATIONTYPE_FLATSUM		== pITEM_DATA->byDurationType ||
		eDURATIONTYPE_METERRATE		== pITEM_DATA->byDurationType )
	{
		TBLIDX idxMerchant;

		switch( byMerchantTab )
		{
		case 0:			idxMerchant = 1001;			break;
		case 1:			idxMerchant = 1002;			break;
		case 2:			idxMerchant = 1003;			break;
		case 3:			idxMerchant = 1004;			break;
		default:		DBO_FAIL("Wrong merchant tab index : " << byMerchantTab); return;
		}

		CDboEventGenerator::StartProcessBuyDurItemNetpy(pITEM_DATA->tblidx, idxMerchant, byItemPos);
		return;
	}

	if( m_BuySlotInfo[iSlot].slot.GetCount() <= 0 )
	{
		// 빈 슬롯이면 등록
		RwInt32 iCount = GetIconMoveManager()->GetStackCount();	
		RwInt32 iPrice = GetIconMoveManager()->GetEXData2();

		CRectangle rect = m_pThis->GetScreenRect();					
		m_BuySlotInfo[iSlot].slot.SetParentPosition(rect.left, rect.top);						
		m_BuySlotInfo[iSlot].slot.SetIcon(GetIconMoveManager()->GetSrcSerial(), iCount);
		m_BuySlotInfo[iSlot].slot.SetPrice(iPrice);

		m_BuySlotInfo[iSlot].NPCShopBuyInfo.byMerchantTab	= byMerchantTab;
		m_BuySlotInfo[iSlot].NPCShopBuyInfo.byItemPos		= byItemPos;

		AddItemCount(BUY_SLOT, iSlot, iCount);
		CalcTotalBuyPrice();

		return;						
	}
	else if( m_BuySlotInfo[iSlot].slot.GetSerial() == GetIconMoveManager()->GetSrcSerial() )
	{
		if( pITEM_DATA->byMax_Stack > m_BuySlotInfo[iSlot].NPCShopBuyInfo.byStack)
		{
			// 이전에 등록된 아이템과 같은 아이템이 슬롯에 놓일 수 있는 최대 갯수를 초과하지 않았다면
			AddItem(BUY_SLOT, iSlot, 1);
		}			
	}
}

VOID CNetPyShopCartGui::RegBuyItemByEvent(RwInt32 iSlot, SDboEventNetPyShopEvent& TradeInfo)
{
	sITEM_TBLDAT* pITEM_DATA = Logic_GetItemDataFromTable( TradeInfo.uiSerial );
	if( !pITEM_DATA )
	{
		DBO_FAIL("Not exist item table of index : " << TradeInfo.uiSerial);
		return;
	}


	// 기간제 아이템
	if( eDURATIONTYPE_FLATSUM		== pITEM_DATA->byDurationType ||
		eDURATIONTYPE_METERRATE		== pITEM_DATA->byDurationType )
	{
		TBLIDX idxMerchant;

		switch( TradeInfo.nPlace )
		{
		case 0:			idxMerchant = 1001;			break;
		case 1:			idxMerchant = 1002;			break;
		case 2:			idxMerchant = 1003;			break;
		case 3:			idxMerchant = 1004;			break;
		default:		DBO_FAIL("Wrong merchant tab index : " << TradeInfo.nPlace); return;
		}

		CDboEventGenerator::StartProcessBuyDurItemNetpy(pITEM_DATA->tblidx, idxMerchant, TradeInfo.nPosition);
		return;
	}


	CRectangle rect = m_pThis->GetScreenRect();	

	m_BuySlotInfo[iSlot].slot.SetSerialType(REGULAR_SLOT_ITEM_TABLE);
	m_BuySlotInfo[iSlot].slot.SetParentPosition(rect.left, rect.top);						
	m_BuySlotInfo[iSlot].slot.SetIcon(TradeInfo.uiSerial, TradeInfo.nOverlapCount);
	m_BuySlotInfo[iSlot].slot.SetPrice(TradeInfo.ulPrice);

	m_BuySlotInfo[iSlot].NPCShopBuyInfo.byMerchantTab = (RwUInt8)TradeInfo.nPlace;
	m_BuySlotInfo[iSlot].NPCShopBuyInfo.byItemPos = (RwUInt8)TradeInfo.nPosition;

	AddItemCount(BUY_SLOT, iSlot, TradeInfo.nOverlapCount);
	CalcTotalBuyPrice();
}

RwInt32 CNetPyShopCartGui::FindEmptySlot(RwInt32 iSlotType)
{
	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( iSlotType == BUY_SLOT )
		{
			if( m_BuySlotInfo[i].slot.GetCount() <= 0 )
				return i;
		}
	}

	return dNETPYSHOP_CART_INVALID_INDEX;
}

RwInt32 CNetPyShopCartGui::FindSlot(RwUInt8 bySlotType, RwUInt32 uiSerial)
{
	if( bySlotType == BUY_SLOT )
	{
		for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
		{
			if( m_BuySlotInfo[i].slot.GetSerial() == uiSerial )
				return i;
		}
	}

	return dNETPYSHOP_CART_INVALID_INDEX;
}

RwInt32	 CNetPyShopCartGui::PtinSlot(RwInt32 iSlotKind, RwInt32 iX, RwInt32 iY)
{
	if( iSlotKind == BUY_SLOT )
	{
		for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
		{
			if( m_BuySlotInfo[i].slot.PtInRect(iX, iY)  )
				return i;
		}
	}
	
	return dNETPYSHOP_CART_INVALID_INDEX;
}

VOID CNetPyShopCartGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_SHOPING_CART ||
		GetInfoWndManager()->GetRequestGui() == DIALOG_TRADECART )
	{
		m_byInfoWindowIndex = INVALID_BYTE;
		GetInfoWndManager()->ShowInfoWindow( FALSE );		
	}
}

RwInt32 CNetPyShopCartGui::SwitchDialog(bool bOpen)
{	
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_TRADE) )
		{
			ClickedCloseButton(NULL);
		}
		else
		{
			Show(false);
			CheckInfoWindow();
		}
	}

	return 1;
}

VOID CNetPyShopCartGui::OnPaint()
{
	for( RwInt32 i = 0 ; i< MAX_SLOT ; ++i )
	{	
		m_BuySlotInfo[i].slot.Paint();
	}	

	if( m_bFocus )
		m_FocusEffect.Render();
}

VOID CNetPyShopCartGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION( "CNetPyShopCartGui::HandleEvents" );

	if( msg.Id == g_EventNetPyShopEvent )
	{
		SDboEventNetPyShopEvent* pData = reinterpret_cast<SDboEventNetPyShopEvent*>( msg.pData );

		switch( pData->byEventType )
		{
		case eNETPYSHOP_EVENT_START:
			{
				m_pBuyButton->SetText(GetDisplayStringManager()->GetString("DST_TRADECART_BUY"));
				m_pBuyButton->ClickEnable(false);

				m_pExitButton->Show(false);
				m_pBuyButton->Show(true);				

				for(RwUInt8 i = 0 ; i < MAX_SLOT ; ++i)
				{
					m_pUpButton[BUY_SLOT][i]->Show(true);
					m_pDownButton[BUY_SLOT][i]->Show(true);
				}

				SetMovable(false);

				OpenCart();

				break;
			}
			break;
		case eNETPYSHOP_EVENT_REG_ITEM:
			{
				RwInt32 iSlot = dNETPYSHOP_CART_INVALID_INDEX;

				// 기존에 등록된 아이템이면 더한다.				
				for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
				{
					if( m_BuySlotInfo[i].slot.GetSerial() == pData->uiSerial )
					{
						if(m_BuySlotInfo[i].slot.GetItemTable()->byMax_Stack > m_BuySlotInfo[i].NPCShopBuyInfo.byStack)
						{
							// 최대 스택갯수를 초과하지 않았다면
							AddItem(BUY_SLOT, i, 1);
							iSlot = i;

							break;
						}
					}
				}

				// 기존에 등록된 아이템이 아니면 빈 슬롯을 찾아 등록한다.
				if(iSlot == dNETPYSHOP_CART_INVALID_INDEX)
				{
					iSlot = (RwInt8)FindEmptySlot(BUY_SLOT);

					if( iSlot >= 0 )
						RegBuyItemByEvent(iSlot, *pData);
					else
					{
						// 카트에 슬롯이 부족합니다
						GetAlarmManager()->AlarmMessage("DST_TRADECART_NO_MORE_SLOT");
					}
				}		
			}
			break;
		case eNETPYSHOP_EVENT_REG_ITEM_MAX:
			{
				RwInt8 iSlot = (RwInt8)FindEmptySlot(BUY_SLOT);
				if( iSlot >= 0 )
				{
					// 빈 슬롯을 찾아서 20개를 등록한다
					RegBuyItemByEvent(iSlot, *pData);
				}
				else
				{					
					for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
					{
						if( m_BuySlotInfo[i].slot.GetSerial() == pData->uiSerial )
						{
							// 최대 스택갯수를 초과하지 않았다면
							if(m_BuySlotInfo[i].slot.GetItemTable()->byMax_Stack > m_BuySlotInfo[i].slot.GetCount())
							{
								// 마저 더해서 20개를 채운다
								AddItem(BUY_SLOT, i, m_BuySlotInfo[i].slot.GetItemTable()->byMax_Stack - m_BuySlotInfo[i].slot.GetCount());
								break;
							}							
						}
					}
				}
			}
			break;
		case eNETPYSHOP_EVENT_BUY_SUCCESS:
			{
				for(RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
					ClearSlot(BUY_SLOT, i);

				CalcTotalBuyPrice();

				CheckInfoWindow();
			}
			break;
		case eNETPYSHOP_EVENT_END:
			{
				CloseCart();
			}
			break;
		default:
			break;
		}
	}
	
	NTL_RETURNVOID();
}

