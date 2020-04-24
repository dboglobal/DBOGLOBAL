#include "precomp_dboclient.h" 

// core
#include "NtlDebug.h"
#include "CEventHandler.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlSobAvatar.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPrivateShop.h"

// table
#include "ItemTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// dbo
#include "DboGlobal.h"
#include "DboDef.h"
#include "DboEvent.h"
#include "DboLogic.h"
#include "DboEventGenerator.h"
#include "DboPacketGenerator.h"
#include "InfoWndManager.h"
#include "DisplayStringManager.h"
#include "DialogManager.h"
#include "IconMoveManager.h"
#include "PrivateShopCartGui.h"

// etc

CPrivateShopCartGui::CPrivateShopCartGui(const RwChar* pName) : CNtlPLGui(pName)
,m_pTotalBuyMoneyTitle(NULL)
,m_pTotalBuyMoney(NULL)
,m_pBuyButton(NULL)
,m_pFocusItem(NULL)
,m_pClickItem(NULL)
,m_pRegItem(NULL)
,m_hOwner(INVALID_SERIAL_ID)
{
}

CPrivateShopCartGui::~CPrivateShopCartGui()
{
}

RwBool CPrivateShopCartGui::Create()
{
	NTL_FUNCTION("CPrivateShopCartGui::Create");

	if (!CNtlPLGui::Create("", "gui\\PrivateShopCart.srf", "gui\\PrivateShopCart.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Componenet
	m_pThis					= (gui::CDialog*)GetComponent("dlgMain");
	m_pTotalBuyMoneyTitle	= (gui::CStaticBox*)GetComponent("stbTotalBuyMoneyTitle");
	m_pTotalBuyMoney		= (gui::CStaticBox*)GetComponent("stbTotalBuyMoney");
	m_pBuyButton			= (gui::CButton*)GetComponent("btnBuy");
	
	// Priority
	m_pThis->SetPriority( dDIALOGPRIORITY_DEFAULT );
	
	// Item
	//RwInt32 iTop = dPRIVATESHOP_CART_UI_ITEM_TOP;
	//RwInt32 iGap = dPRIVATESHOP_CART_UI_ITEM_GAP;
	//CRectangle rect = GetPosition();
	//for (int i = 0; i < NTL_MAX_BUY_SHOPPING_CART; ++i)
	//{
	//	m_CartItem[i].slotCart.Create(m_pThis, DIALOG_PRIVATESHOP_CART, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
	//	m_CartItem[i].slotCart.SetPosition_fromParent(dPRIVATESHOP_CART_UI_ITEM_LEFT, iTop);
	//	
	//	m_CartItem[i].EffectSold.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PrivateShopCart.srf", "srfShopItemSold"));
	//	m_CartItem[i].EffectSold.SetPositionfromParent(dPRIVATESHOP_CART_UI_ITEM_LEFT, iTop);
	//	
	//	iTop += iGap;
	//}

	RwInt32 iTop = dPRIVATESHOP_CART_UI_ITEM_TOP;
	RwInt32 iGap;
	for (int i = 0; i < NTL_MAX_BUY_SHOPPING_CART; ++i)
	{
		if( (i%2) == 0 )
			iGap = 18;
		else
			iGap = 18 + dPRIVATESHOP_CART_UI_ITEM_HORI_GAP;

		m_CartItem[i].slotCart.Create(m_pThis, DIALOG_PRIVATESHOP_CART, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
		m_CartItem[i].slotCart.SetPosition_fromParent(iGap, iTop);

		m_CartItem[i].EffectSold.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PrivateShopCart.srf", "srfShopItemSold"));
		m_CartItem[i].EffectSold.SetPositionfromParent(iGap, iTop);
		
		if( (i%2) != 0 )
			iTop += dPRIVATESHOP_CART_UI_ITEM_VERTI_GAP;
	}

	//// Slots
	//RwInt32 iSlotX;
	//RwInt32 iSlotY = 57;
	//for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	//{	
	//	if( (i%2) == 0 )
	//		iSlotX = 18;
	//	else
	//		iSlotX = 18 + dGUI_SLOT_HORI_GAP;


	//	m_BuySlotInfo[i].slot.Create(m_pThis, DIALOG_SHOPING_CART, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
	//	m_BuySlotInfo[i].slot.SetPosition_fromParent(iSlotX, iSlotY);

	//	m_SellSlotInfo[i].slot.Create(m_pThis, DIALOG_SHOPING_CART, REGULAR_SLOT_ITEM_SOB, SDS_COUNT);
	//	m_SellSlotInfo[i].slot.SetPosition_fromParent(iSlotX + dGUI_BUY_SELL_SLOT_GAP, iSlotY);


	//	if( (i%2) != 0 )
	//iSlotY += dGUI_SLOT_VERT_GAP;
	//}

	// Effect
	m_EffectFocus.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));

	// Event
	m_slotMove				= m_pThis->SigMove().Connect(this, &CPrivateShopCartGui::OnMove);
	m_slotPaint				= m_pThis->SigPaint().Connect(this, &CPrivateShopCartGui::OnPaint);	
	m_slotMouseUp			= m_pThis->SigMouseUp().Connect(this, &CPrivateShopCartGui::OnMouseUp);
	m_slotMouseDown			= m_pThis->SigMouseDown().Connect(this, &CPrivateShopCartGui::OnMouseDown);
	m_slotMouseMove			= m_pThis->SigMouseMove().Connect(this, &CPrivateShopCartGui::OnMouseMove);
	m_slotMouseLeave		= m_pThis->SigMouseLeave().Connect(this, &CPrivateShopCartGui::OnMouseLeave);
	m_slotBuyButtonClicked	= m_pBuyButton->SigClicked().Connect(this, &CPrivateShopCartGui::OnBuyButtonClicked);
	m_slotCaptureMouseDown		= GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect(this, &CPrivateShopCartGui::OnCaptureMouseDown);
	
	m_pTotalBuyMoneyTitle->SetText(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_CART_PRICE"));
	m_pBuyButton->SetText(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_CART_BUY"));
	m_pTotalBuyMoney->SetText(L"0");

	LinkMsg(g_EventPrivateShopStateVisitor, 0);
	LinkMsg(g_EventPrivateShopItemBuying, 0);
	LinkMsg(g_EventRegPrivateShopCartItem, 0);

	Clear();
	Show(false);
	NTL_RETURN(TRUE);	
}

void CPrivateShopCartGui::Destroy()
{
	NTL_FUNCTION("CPrivateShopCartGui::Destroy");

	UnLinkMsg(g_EventPrivateShopStateVisitor);
	UnLinkMsg(g_EventPrivateShopItemBuying);
	UnLinkMsg(g_EventRegPrivateShopCartItem);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CPrivateShopCartGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	if (!bOpen)
	{
		Clear();
	}

	return TRUE;
}

void CPrivateShopCartGui::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CPrivateShopCartGui::HandleEvents");

	if (msg.Id == g_EventPrivateShopStateVisitor)
	{
		SNtlEventPrivateShopStateVisitor* pStateVisitor = reinterpret_cast<SNtlEventPrivateShopStateVisitor*>(msg.pData);

		switch (pStateVisitor->uiEventType)
		{
		case PRIVATESHOP_EVENT_ENTER:
			m_hOwner = pStateVisitor->pPrivateShopData->hOwner;
			GetDialogManager()->OpenDialog(DIALOG_PRIVATESHOP_CART);
			break;
		case PRIVATESHOP_EVENT_LEAVE:
			GetDialogManager()->CloseDialog(DIALOG_PRIVATESHOP_CART);
			break;
		}
	}
	else if (msg.Id == g_EventRegPrivateShopCartItem)
	{
		SDboEventRegPrivateShopCartItem* pShopItem = reinterpret_cast<SDboEventRegPrivateShopCartItem*>(msg.pData);

		if (pShopItem->uiPlace == PLACE_PRIVATESHOP && pShopItem->uiSerial != INVALID_SERIAL_ID)
		{
			if( m_pRegItem )
			{
				if( m_pRegItem->slotCart.GetSerial() != INVALID_SERIAL_ID )
				{
					if( m_pRegItem->uiPrivateShopPos == pShopItem->uiPos )
					{
						m_pRegItem = NULL;
						return;
					}
				}

				for( int i=0; i<NTL_MAX_BUY_SHOPPING_CART; ++i )
				{
					if( m_CartItem[i].slotCart.GetSerial() != INVALID_SERIAL_ID )
					{
						if( m_CartItem[i].uiPrivateShopPos == pShopItem->uiPos )
						{
							m_pRegItem = NULL;
							return;
						}
					}
				}

				m_pRegItem->SetItem( pShopItem->uiSerial, pShopItem->uiStackCount,
					pShopItem->uiPrice, (RwUInt8)pShopItem->uiPos, (sITEM_DATA*)pShopItem->pData);

				m_pRegItem = NULL;
			}
			else
			{
				for (int i = 0; i < NTL_MAX_BUY_SHOPPING_CART; ++i)
				{
					if (m_CartItem[i].slotCart.GetSerial() != INVALID_SERIAL_ID)
					{
						if (m_CartItem[i].uiPrivateShopPos == pShopItem->uiPos)
						{
							break;
						}

						continue;
					}

					m_CartItem[i].SetItem(pShopItem->uiSerial, pShopItem->uiStackCount,
						pShopItem->uiPrice,	(RwUInt8)pShopItem->uiPos, (sITEM_DATA*)pShopItem->pData);

					//GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopItemSelectReq(m_hOwner, pShopItem->uiPos, true);
					break;
				}
			}
			CalcPrice();
			CheckEnableBuy();
		}
	}
	else if (msg.Id == g_EventPrivateShopItemBuying)
	{
		SNtlEventPrivateShopItemBuying* pPrivateShopItemBuying = reinterpret_cast<SNtlEventPrivateShopItemBuying*>(msg.pData);

		if (pPrivateShopItemBuying->bIsBuyAvatar)
		{
			for (int i = 0; i < NTL_MAX_BUY_SHOPPING_CART; ++i)
			{
				if (!m_CartItem[i].bSold)
				{
					m_CartItem[i].Clear();
				}
			}
		}
		else
		{
			for (int i = 0; i < pPrivateShopItemBuying->uiBuyCount; ++i)
			{
				sPRIVATESHOP_ITEM_DATA& sPrivateShopItem = pPrivateShopItemBuying->asPrivateShopItemData[i];
				if (sPrivateShopItem.byPrivateShopInventorySlotPos != PRIVATESHOP_ITEMPOS_INVALID) 
				{
					for (int j = 0; j < NTL_MAX_BUY_SHOPPING_CART; ++j)
					{
						if (m_CartItem[j].uiPrivateShopPos == sPrivateShopItem.byPrivateShopInventorySlotPos)
						{
							m_CartItem[j].uiPrivateShopPos	= PRIVATESHOP_ITEMPOS_INVALID;
							m_CartItem[j].bSold				= TRUE;
						}
					}
				}
			}
		}
		CalcPrice();		
		CheckEnableBuy();
	}

	NTL_RETURNVOID();
}
	
void CPrivateShopCartGui::OnPaint()
{
	for (int i = 0; i < NTL_MAX_BUY_SHOPPING_CART; ++i)
	{
		m_CartItem[i].slotCart.Paint();
		if (m_CartItem[i].bSold)
		{
			m_CartItem[i].EffectSold.Render();
		}
	}

	if (m_pFocusItem)
	{
		m_EffectFocus.Render();
	}
}

void CPrivateShopCartGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = GetPosition();
	for (int i = 0; i < NTL_MAX_BUY_SHOPPING_CART; ++i)
	{
		m_CartItem[i].slotCart.SetParentPosition(rect.left, rect.top);
		m_CartItem[i].EffectSold.SetPositionbyParent(rect.left, rect.top);
	}

	SetCartItemFocus(m_pFocusItem);
}

void CPrivateShopCartGui::OnMouseUp(const CKey& key)
{
	if (m_pClickItem)
	{
		if (m_pFocusItem == m_pClickItem)
		{
			// 현재 마우스 왼쪽 버튼을 이용하여 아이템을 등록하는 것은 막혀 있음
			if (key.m_nID == UD_LEFT_BUTTON)
			{
				if (GetIconMoveManager()->IsActive() && GetIconMoveManager()->GetSrcPlace() == PLACE_PRIVATESHOP)
				{
					if( m_pClickItem->slotCart.GetSerial() == INVALID_SERIAL_ID )
					{
						GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopItemSelectReq(m_hOwner, (RwUInt8)GetIconMoveManager()->GetSrcSlotIdx(), true);
						m_pRegItem = m_pClickItem;
					}

					GetIconMoveManager()->IconMoveEnd();
				}
			}
			else if (key.m_nID == UD_RIGHT_BUTTON)
			{
				if (m_pClickItem->slotCart.GetSerial() != INVALID_SERIAL_ID)
				{
					if (!m_pClickItem->bSold)
					{ 
						GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopItemSelectReq(m_hOwner, m_pClickItem->uiPrivateShopPos, false);
					}
					m_pFocusItem->Clear();
				}
			}

			CalcPrice();
			CheckEnableBuy();
		}
		m_pClickItem->slotCart.ClickEffect(false);
		m_pThis->ReleaseMouse();
		m_pClickItem = NULL;
	}
}

void CPrivateShopCartGui::OnMouseDown(const CKey& key)
{
	m_pClickItem = m_pFocusItem;
	if ( m_pClickItem )
	{
		// 오른쪽 버튼만 효과를 준다.
		if( key.m_nID == UD_RIGHT_BUTTON )
			m_pClickItem->slotCart.ClickEffect(true);

		m_pThis->CaptureMouse();
	}
}

void CPrivateShopCartGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	for (int i = 0; i < NTL_MAX_BUY_SHOPPING_CART; ++i)
	{
		if (m_CartItem[i].slotCart.PtInRect(nX, nY))
		{
			SetCartItemFocus(&m_CartItem[i]);
			ShowInfoWindow(TRUE, i);
			
			
			
			return;
		}
	}
	SetCartItemFocus(NULL);
	ShowInfoWindow(FALSE);
}

void CPrivateShopCartGui::ShowInfoWindow(RwBool bIsShow, RwInt32 iIndex)
{
	if (bIsShow && iIndex >= 0 && m_CartItem[iIndex].slotCart.GetSerial() != INVALID_SERIAL_ID)
	{
		CRectangle	rect = m_pThis->GetScreenRect();
		RwInt32		iX = m_CartItem[iIndex].slotCart.GetX_fromParent();
		RwInt32		iY = m_CartItem[iIndex].slotCart.GetY_fromParent();

		if (m_CartItem[iIndex].sItem.bNeedToIdentify)
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM, rect.left + iX, rect.top + iY, NULL, DIALOG_PRIVATESHOP_CART);
		}
		else
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM_DATA, rect.left + iX, rect.top + iY, &m_CartItem[iIndex].sItem, DIALOG_PRIVATESHOP_CART);
		}
	}
	else
	{
		// 자신의 Dialog에서 요청한 InfoWindow인지 검사하고 닫아준다.
		if( DIALOG_PRIVATESHOP_CART == GetInfoWndManager()->GetRequestGui() )
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

void CPrivateShopCartGui::OnMouseLeave(gui::CComponent* pComponent)
{
	SetCartItemFocus(NULL);
	ShowInfoWindow(FALSE);
}

void CPrivateShopCartGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE_TOP_LINKED(DIALOG_PRIVATESHOP, key.m_fX, key.m_fY);
}

void CPrivateShopCartGui::OnBuyButtonClicked(gui::CComponent* pComponent)
{
	RwUInt8 auiBuy[NTL_MAX_BUY_SHOPPING_CART];
	memset( auiBuy, INVALID_BYTE, sizeof(RwUInt8) * NTL_MAX_BUY_SHOPPING_CART );
	RwUInt8 byCount = 0;

	// autBuy 배열에 순서대로 쌓아서 서버로 보낸다. ( 중간에 빈 곳이 있으면 구입이 되지 않는다. )
	for (int i = 0; i < NTL_MAX_BUY_SHOPPING_CART; ++i)
	{
		if( m_CartItem[i].uiPrivateShopPos != INVALID_BYTE )
			auiBuy[byCount++] = m_CartItem[i].uiPrivateShopPos;
	}

	GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopItemBuyingReq(m_hOwner, auiBuy);
}

void CPrivateShopCartGui::SetCartItemFocus(sCART_SLOT* pItem)
{
	m_pFocusItem = pItem;
	if (m_pFocusItem)
	{
		CRectangle	rect = m_pThis->GetScreenRect();

		rect.left	= rect.left + m_pFocusItem->slotCart.GetX_fromParent();
		rect.top	= rect.top + m_pFocusItem->slotCart.GetY_fromParent();
		rect.right	= rect.left + dPRIVATESHOP_CART_UI_ITEM_WIDTH;	
		rect.bottom	= rect.top + dPRIVATESHOP_CART_UI_ITEM_HEIGHT;

		m_EffectFocus.SetRect(rect);
	}
}

void CPrivateShopCartGui::CalcPrice()
{
	RwInt32 iPrice = 0;
	for (int i = 0; i < NTL_MAX_BUY_SHOPPING_CART; ++i)
	{
		if (!m_CartItem[i].bSold && m_CartItem[i].uiPrivateShopPos != PRIVATESHOP_ITEMPOS_INVALID)
		{
			iPrice += m_CartItem[i].slotCart.GetPrice();		
		}
	}
	m_pTotalBuyMoney->SetText(iPrice);
}

void CPrivateShopCartGui::Clear()
{
	for (int i = 0; i < NTL_MAX_BUY_SHOPPING_CART; ++i)
	{
		m_CartItem[i].Clear();
	}
	SetCartItemFocus(NULL);
	CalcPrice();
	m_pBuyButton->Enable(false);
	m_hOwner = INVALID_SERIAL_ID;
}

void CPrivateShopCartGui::CheckEnableBuy()
{
	for (int i = 0; i < NTL_MAX_BUY_SHOPPING_CART; ++i)
	{
		if (!m_CartItem[i].bSold && m_CartItem[i].uiPrivateShopPos != PRIVATESHOP_ITEMPOS_INVALID)
		{
			m_pBuyButton->Enable(true);
			return;
		}
	}
	m_pBuyButton->Enable(false);
}