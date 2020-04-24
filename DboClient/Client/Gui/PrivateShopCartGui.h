#pragma once

#include "SlotGui.h"

#define dPRIVATESHOP_CART_UI_ITEM_TOP	57
#define dPRIVATESHOP_CART_UI_ITEM_LEFT	39
#define dPRIVATESHOP_CART_UI_ITEM_GAP	48
#define dPRIVATESHOP_CART_UI_ITEM_HORI_GAP	42
#define dPRIVATESHOP_CART_UI_ITEM_VERTI_GAP 57
#define dPRIVATESHOP_CART_UI_ITEM_HEIGHT DBOGUI_ICON_SIZE
#define dPRIVATESHOP_CART_UI_ITEM_WIDTH	DBOGUI_ICON_SIZE

class CPrivateShopCartGui : public CNtlPLGui, public RWS::CEventHandler
{
	struct sCART_SLOT
	{
		CRegularSlotGui				slotCart;		
		CSurfaceGui				EffectSold;

		RwUInt8					uiPrivateShopPos;
		RwBool					bSold;
		sITEM_DATA 				sItem;

		sCART_SLOT() : uiPrivateShopPos(PRIVATESHOP_ITEMPOS_INVALID), bSold(FALSE) {}
		void SetItem(RwUInt32 uiSerial, RwUInt32 uiCount, RwUInt32 uiPrice, RwUInt8 uiPrivateShop, sITEM_DATA* pItem)
		{
			eREGULAR_SLOT_TYPE eSerialType;
			if (pItem->bNeedToIdentify)
			{
				eSerialType = REGULAR_SLOT_ITEM_NOT_IDENTIFICATION;
				
			}
			else
			{
				eSerialType = REGULAR_SLOT_ITEM_TABLE;
			}
			
			if (slotCart.GetSerialType() != eSerialType)
			{
				slotCart.Clear();
				slotCart.SetSerialType(eSerialType);
			}
			
			slotCart.SetIcon(uiSerial, uiCount);
			slotCart.SetPrice(uiPrice);

			uiPrivateShopPos	= uiPrivateShop;

			if (pItem == NULL)
			{
				sItem = sITEM_DATA();
			}
			else
			{
				memcpy(&sItem, pItem, sizeof(sITEM_DATA));
			}			
		}
		void Clear()
		{
			sItem				= sITEM_DATA();
			bSold				= FALSE;
			uiPrivateShopPos	= PRIVATESHOP_ITEMPOS_INVALID;
			slotCart.Clear();
		}		
	};

protected:
	gui::CStaticBox*	m_pTotalBuyMoneyTitle;
	gui::CStaticBox*	m_pTotalBuyMoney;
	gui::CButton*		m_pBuyButton;


	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotBuyButtonClicked;
	gui::CSlot			m_slotCaptureMouseDown;
	
	CSurfaceGui			m_EffectFocus;
	
	sCART_SLOT			m_CartItem[NTL_MAX_BUY_SHOPPING_CART];
	sCART_SLOT*			m_pFocusItem;
	sCART_SLOT*			m_pClickItem;
	sCART_SLOT*			m_pRegItem;

	SERIAL_HANDLE		m_hOwner;

public:
	CPrivateShopCartGui(const RwChar* pName);
	virtual ~CPrivateShopCartGui();

	RwBool	Create();
	void	Destroy();

	RwInt32	SwitchDialog(bool bOpen);
	void	HandleEvents(RWS::CMsg& msg);

	void	OnPaint();
	void	OnMove(RwInt32 iOldX, RwInt32 iOldY);
	void	OnMouseUp(const CKey& key);
	void	OnMouseDown(const CKey& key);
	void	OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	void	OnMouseLeave(gui::CComponent* pComponent);
	void	OnBuyButtonClicked(gui::CComponent* pComponent);
	void	OnCaptureMouseDown(const CKey& key);

	void	SetCartItemFocus(sCART_SLOT* pItem);
	void	CalcPrice();
	void	CheckEnableBuy();
	void	Clear();

	void	ShowInfoWindow(RwBool bIsShow, RwInt32 iIndex = -1);
};