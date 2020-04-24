#pragma once

#include "ceventhandler.h"
#include "NtlPLGui.h"
#include "Windowby3.h"
#include "SlotGui.h"

#include "NtlMarket.h"

class CAuctionHouseGui : public CNtlPLGui, public RWS::CEventHandler
{
private:


#define AH_VISIBLE_ITEM_TYPE_BUTTONS 8
#define AH_MAX_PAGES_COUNT 10

protected:

	typedef std::map<unsigned short, const WCHAR*> ITEMTYPEMAP;

	// MAIN
	gui::CStaticBox*	m_pStbTitle;

	gui::CStaticBox*	m_pstdInfoText;

	gui::CButton*		m_pBtnClose;
	gui::CSlot			m_slotClickedBtnClose;

	gui::CButton*		m_pBtnBuyNot;

	gui::CButton*		m_pBtnBuy;
	gui::CSlot			m_slotClickedBtnBuy;

	gui::CComboBox*		m_cbbPage;
	gui::CSlot			m_slotSelectPage;

	CWindowby3			m_stfTop;
	CWindowby3			m_stfMid;
	CWindowby3			m_stfBot;

	gui::CDialog*		m_pPageDialog[eAUCTION_TAB_TYPE_COUNT];

	CSurfaceGui			m_srfBackgroundLT[eAUCTION_TAB_TYPE_COUNT];
	CSurfaceGui			m_srfBackgroundMT;	// Only in sell page
	CSurfaceGui			m_srfBackgroundRT[eAUCTION_TAB_TYPE_COUNT];

	CSurfaceGui			m_srfBackgroundLM[eAUCTION_TAB_TYPE_COUNT];
	CSurfaceGui			m_srfBackgroundMM[eAUCTION_TAB_TYPE_COUNT];
	CSurfaceGui			m_srfBackgroundRM[eAUCTION_TAB_TYPE_COUNT];

	CSurfaceGui			m_srfBackgroundLB[eAUCTION_TAB_TYPE_COUNT];
	CSurfaceGui			m_srfBackgroundMB[eAUCTION_TAB_TYPE_COUNT];
	CSurfaceGui			m_srfBackgroundRB[eAUCTION_TAB_TYPE_COUNT];


	gui::CTabButton*	m_ptabType;
	gui::CSlot			m_slotSelectTab;

	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMainMouseMove;
	gui::CSlot			m_slotSellMouseMove;
	gui::CSlot			m_slotCaptureMouseDown;

	CSurfaceGui			m_srfItemNameInput;
	gui::CStaticBox*	m_pstbItemNameInput;
	gui::CInputBox*		m_pItemNameInput;
	gui::CSlot			m_slotItemNameInputGotFocus;
	gui::CSlot			m_slotItemNameInputLostFocus;


	gui::CStaticBox*	m_pstbMinLevel;
	gui::CButton*		m_pbtnMinLevel;
	gui::CSlot			m_slotClickedBtnMinLevel;

	gui::CStaticBox*	m_pstbMaxLevel;
	gui::CButton*		m_pbtnMaxLevel;
	gui::CSlot			m_slotClickedBtnMaxLevel;

	gui::CButton*		m_pbtnSearch;
	gui::CSlot			m_slotClickedBtnSearch;

	gui::CButton*		m_pbtnBuyRefresh;
	gui::CSlot			m_slotClickedBtnBuyRefresh;

	gui::CButton*		m_pbtnArrowLeftItemType;
	gui::CSlot			m_slotClickedBtnArrowLeftItemType;

	gui::CButton*		m_pbtnArrowRightItemType;
	gui::CSlot			m_slotClickedBtnArrowRightItemType;

	gui::CComboBox*		m_pcbbItemRank;
	gui::CSlot			m_slotSelectedItemRank;

	gui::CComboBox*		m_pcbbItemClass;
	gui::CSlot			m_slotSelectedItemClass;

	gui::CButton*		m_pBtnItemType1;
	gui::CSlot			m_slotClickedBtnGloveItemType1;
	gui::CButton*		m_pBtnItemType2;
	gui::CSlot			m_slotClickedBtnGloveItemType2;
	gui::CButton*		m_pBtnItemType3;
	gui::CSlot			m_slotClickedBtnGloveItemType3;
	gui::CButton*		m_pBtnItemType4;
	gui::CSlot			m_slotClickedBtnGloveItemType4;
	gui::CButton*		m_pBtnItemType5;
	gui::CSlot			m_slotClickedBtnGloveItemType5;
	gui::CButton*		m_pBtnItemType6;
	gui::CSlot			m_slotClickedBtnGloveItemType6;
	gui::CButton*		m_pBtnItemType7;
	gui::CSlot			m_slotClickedBtnGloveItemType7;
	gui::CButton*		m_pBtnItemType8;
	gui::CSlot			m_slotClickedBtnGloveItemType8;

	// ORDER Items button
	gui::CButton*		m_pBtnOrderItemName;
	gui::CSlot			m_slotClickedBtnOrderItemName;
	gui::CButton*		m_pBtnOrderItemRank;
	gui::CSlot			m_slotClickedBtnOrderItemRank;
	gui::CButton*		m_pBtnOrderItemRequiredLevel;
	gui::CSlot			m_slotClickedBtnOrderItemRequiredLevel;
	gui::CButton*		m_pBtnOrderItemPrice;
	gui::CSlot			m_slotClickedBtnOrderItemPrice;
	gui::CButton*		m_pBtnOrderItemTimeRemaining;
	gui::CSlot			m_slotClickedBtnOrderItemTimeRemaining;
	gui::CButton*		m_pBtnOrderItemSeller; // turn to "cancel" when in sell page

	struct sITEM
	{
		CSurfaceGui			m_srfline;

		CSurfaceGui			m_srfItemIcon;
		CRegularSlotGui		m_ItemSlot;

		gui::CButton*		m_pBtnBuy;	// this button is above all except the "cancel"
		gui::CSlot			m_slotMouseDownBtnBuy;
		gui::CSlot			m_slotMouseUpBtnBuy;

		gui::CStaticBox*	m_pstbItemName;
		gui::CStaticBox*	m_pstbItemRank;
		gui::CStaticBox*	m_pstbRequiredLevel;
		gui::CStaticBox*	m_pstbItemPrice;
		CSurfaceGui			m_srfZenny;
		gui::CStaticBox*	m_pstbTimeRemaining;
		gui::CStaticBox*	m_pstbItemSeller;	// hidden in "Sell" page

		gui::CButton*		m_pBtnCancel;	// only visible in "Sell" page
		gui::CSlot			m_slotClickedBtnCancel;
	};

	sITEM*				m_pItems[DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE];

	CSurfaceGui			m_srfLastline;

	// sell page
	gui::CStaticBox*	m_pstbItemNameTitle;
	gui::CStaticBox*	m_pstbRankTitle;
	gui::CStaticBox*	m_pstbUseLevelTitle;
	gui::CStaticBox*	m_pstbItemPriceTitle;
	gui::CStaticBox*	m_pstbLastingTimeTitle;
	gui::CStaticBox*	m_pstbSellPriceTitle;

	CSurfaceGui			m_srfSellline;

	CSurfaceGui			m_srfItemIcon;
	CRegularSlotGui		m_SellItemSlot;

	gui::CStaticBox*	m_pstbItemName;
	gui::CStaticBox*	m_pstbItemRank;
	gui::CStaticBox*	m_pstbUseLevel;
	gui::CButton*		m_pbtnItemPrice;
	gui::CSlot			m_slotClickedBtnItemPrice;
	gui::CComboBox*		m_cbbLastingTime;
	gui::CStaticBox*	m_pstbSellPrice;

	gui::CButton*		m_pBtnRegisterItem;
	gui::CSlot			m_slotClickedBtnRegisterItem;

	gui::CButton*		m_pBtnSellRefresh;
	gui::CSlot			m_slotClickedBtnSellRefresh;

	// 
	gui::CButton*		m_pbtnPrevPrev;
	gui::CSlot			m_slotClickedBtnPrevPrev;
	gui::CButton*		m_pbtnPrev;
	gui::CSlot			m_slotClickedBtnPrev;
	gui::CButton*		m_pbtnNext;
	gui::CSlot			m_slotClickedBtnNext;
	gui::CButton*		m_pbtnNextNext;
	gui::CSlot			m_slotClickedBtnNextNext;

	gui::CButton*		m_pBtnPageNum[AH_MAX_PAGES_COUNT];
	gui::CSlot			m_pSlotPageNum[AH_MAX_PAGES_COUNT];

public:

	CAuctionHouseGui(const RwChar* pName);
	virtual ~CAuctionHouseGui();

	RwBool		Create();
	void		Destroy();

	void		Update(RwReal fElapsed);

	void		HandleEvents(RWS::CMsg& msg);
	RwInt32		SwitchDialog(bool bOpen);


private:

	void		SetPage(BYTE byPage);

private:

	void		OnMove(RwInt32 iOldX, RwInt32 iOldY);
	void		OnPaint();
	void		OnMouseUp(const CKey& key);
	void		OnMouseDown(const CKey& key);
	void		OnMainMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	void		OnSellMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	void		OnCaptureMouseDown(const CKey& key);

	void		OnSelectPage(INT nSelectIndex);

	void		OnClicked_Close(gui::CComponent* pComponent);
	void		OnClicked_Search(gui::CComponent* pComponent);
	void		OnClicked_BuyRefresh(gui::CComponent* pComponent);
	void		OnClicked_SellRefresh(gui::CComponent* pComponent);
	void		OnClicked_LeftArrowItemType(gui::CComponent* pComponent);
	void		OnClicked_RightArrowItemType(gui::CComponent* pComponent);
	void		OnClicked_ItemType1(gui::CComponent* pComponent);
	void		OnClicked_ItemType2(gui::CComponent* pComponent);
	void		OnClicked_ItemType3(gui::CComponent* pComponent);
	void		OnClicked_ItemType4(gui::CComponent* pComponent);
	void		OnClicked_ItemType5(gui::CComponent* pComponent);
	void		OnClicked_ItemType6(gui::CComponent* pComponent);
	void		OnClicked_ItemType7(gui::CComponent* pComponent);
	void		OnClicked_ItemType8(gui::CComponent* pComponent);

	void		OnClicked_MinLevel(gui::CComponent* pComponent);
	void		OnClicked_MaxLevel(gui::CComponent* pComponent);

	void		OnClicked_PrevPrev(gui::CComponent* pComponent);
	void		OnClicked_Prev(gui::CComponent* pComponent);
	void		OnClicked_NextNext(gui::CComponent* pComponent);
	void		OnClicked_Next(gui::CComponent* pComponent);
	void		OnClicked_PageNumber(gui::CComponent* pComponent);

	void		OnClicked_SetItemPrice(gui::CComponent* pComponent);

	void		OnClicked_RegisterItem(gui::CComponent* pComponent);
	void		OnClicked_CancelItem(gui::CComponent* pComponent);

	void		OnMouseDown_ItemBuy(const CKey& key);
	void		OnMouseUp_ItemBuy(const CKey& key);

	void		OnClicked_Buy(gui::CComponent* pComponent);

	void		OnClicked_OrderItemName(gui::CComponent* pComponent);
	void		OnClicked_OrderItemRank(gui::CComponent* pComponent);
	void		OnClicked_OrderItemLevel(gui::CComponent* pComponent);
	void		OnClicked_OrderItemPrice(gui::CComponent* pComponent);
	void		OnClicked_OrderItemRemainingTime(gui::CComponent* pComponent);

	void		OnItemNameInput_GotFocus();
	void		OnItemNameInput_LostFocus();

	void		OnSelectedTab(RwInt32 nSelectIndex, RwInt32 nPreviousIndex);

	void		OnSelectedItemRank(INT nSelectIndex);
	void		OnSelectedItemClass(INT nSelectIndex);


private:

	void		InitBuyPage();
	void		InitSellPage();

	void		DisableAllButtons(); // disable everything in current page

	void		OnAuctionHouseListRes(RwUInt32 uiPage, RwUInt32 uiMaxPage, BYTE byListNum, BYTE byPage);
	void		ParseItemData(sTENKAICHIDAISIJYOU_DATA_CLIENT* pData);

	void		LoadItemTypeButtons();
	void		SetItemTypeButtonText();

	BYTE		GetItemTypeByButton(const WCHAR* wchText);
	void		SetItemTypeButtonActive();

	void		InitSearch();
	void		InitOrder();

	void		SetSite(unsigned int uiSite, unsigned int uiMaxSite);

	void		DeleteItems();
	void		ClearItemList();

	void		ClearSellItem();
	void		ClearSelectItem();

private:


	std::map<int, sTENKAICHIDAISIJYOU_DATA_CLIENT*>	m_mapItemData;

	RwBool		m_bUpdateItemTypeButtons;

	BYTE		m_byItemTypeMoveCounter; // - 1 when press left arrow, + 1 when press right arrow

	ITEMTYPEMAP	m_mapItemType;

	bool		m_bUseSearch;

	int			m_nCancelSaleIndex;
	int			m_nSelectedItemIndex;
	int			m_nMouseDownItemIndex;

	DWORD		m_dwSellPrice;

private:

	BYTE		m_byPage; // sell / buy page

	RwUInt32	m_uiMaxPage; // max sites
	BYTE		m_byListNum; // amount of items in current site

	// sorting
	BYTE		m_byClassType;
	BYTE		m_byTabType;
	BYTE		m_byItemType;
	BYTE		m_byMinLevel;
	BYTE		m_byMaxLevel;
	BYTE		m_byRank;
	RwUInt32	m_uiPage;	// current site
	BYTE		m_bySortType;

};