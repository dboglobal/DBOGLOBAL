#pragma once


#include "SlotGui.h"
#include "ceventhandler.h"

enum eHLS_CATEGORY
{
	eHLS_CATEGORY_AVATAR,
	eHLS_CATEGORY_CONSUMABLE,
	eHLS_CATEGORY_VEHICLE,
	eHLS_CATEGORY_DOGI,
	eHLS_CATEGORY_DOGI_HAIR,
	eHLS_CATEGORY_DOGI_MASK,
	eHLS_CATEGORY_DOGI_RICHNESS,
	eHLS_CATEGORY_DOGI_BACKPACK,
	eHLS_CATEGORY_OTHER,
	eHLS_CATEGORY_WAGU_COIN,
	eHLS_CATEGORY_WAGU_MACHINE,
	eHLS_CATEGORY_EVENT_MACHINE,

	eHLS_CATEGORY_NUM,
};

static bool abIsCategoryActice[eHLS_CATEGORY_NUM] =
{
	// eHLS_CATEGORY_AVATAR
	true,
	// eHLS_CATEGORY_CONSUMABLE
	true,
	// eHLS_CATEGORY_VEHICLE
	true,
	// eHLS_CATEGORY_DOGI
	true,
	// eHLS_CATEGORY_DOGI_HAIR
	true,
	// eHLS_CATEGORY_DOGI_MASK
	true,
	// eHLS_CATEGORY_DOGI_RICHNESS
	true,
	// eHLS_CATEGORY_DOGI_BACKPACK
	true,
	// eHLS_CATEGORY_OTHER
	true,
	// eHLS_CATEGORY_WAGU_COIN
	false,
	// eHLS_CATEGORY_WAGU_MACHINE
	false,
	// eHLS_CATEGORY_EVENT_MACHINE
	false
};

class CHLShopGui : public CNtlPLGui, public RWS::CEventHandler
{

	struct sHLS_PRODUCTS
	{
		TBLIDX				hlsItemTblidx;

		gui::CPanel*		pDialog; // parent of all
		int					nDialogX;
		int					nDialogY;

		CRegularSlotGui		ItemSlot;
		gui::CPanel*		ppnlItemSlot;
		gui::CSlot			slotMouseEnterItem;
		gui::CSlot			slotMouseLeaveItem;

		CSurfaceGui			mSurface;

		gui::CStaticBox*	pstbItemName;
		gui::CStaticBox*	pstbPrice;
		gui::CPanel*		ppnlCoinMark;

		gui::CButton*		pBtnBuy;
		gui::CSlot			slotClickedBtnBuy;

		gui::CButton*		pBtnGift;
		gui::CSlot			slotClickedBtnGift;
	};

protected:

	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;

	// MAIN
	gui::CStaticBox*	m_pStbTitle;

	gui::CButton*		m_pBtnClose;
	gui::CSlot			m_slotClickedBtnClose;

	gui::CButton*		m_pbtnSearch;
	gui::CSlot			m_slotClickedBtnSearch;

	gui::CButton*		m_pbtnInitSearchList;
	gui::CSlot			m_slotClickedBtnInitSearchList;

	gui::CInputBox*		m_pipbHLSSearch;

	gui::CStaticBox*	m_pstbHaveCashTitle;
	gui::CStaticBox*	m_pstbHaveCash;
	gui::CPanel*		m_ppnlHaveCashMark;

	gui::CStaticBox*	m_pstbHaveWaguCoinTitle;
	gui::CStaticBox*	m_pstbHaveWaguCoin;
	gui::CPanel*		m_ppnlHaveWaguCoinMark;

	gui::CStaticBox*	m_pstbHaveEventCoinTitle;
	gui::CStaticBox*	m_pstbHaveEventCoin;
	gui::CPanel*		m_ppnlHaveEventCoinMark;

	gui::CButton*		m_pbtnYadrat;
	gui::CSlot			m_slotClickedBtnYadrat;

	gui::CButton*		m_pbtnCashRecharge;
	gui::CSlot			m_slotClickedBtnCashRecharge;

	gui::CButton*		m_pbtnProductFirstList;
	gui::CSlot			m_slotClickedBtnProductFirstList;

	gui::CButton*		m_pbtnProductPrevList;
	gui::CSlot			m_slotClickedBtnProductPrevList;

	gui::CStaticBox*	m_pstbPage;

	gui::CButton*		m_pbtnProductNextList;
	gui::CSlot			m_slotClickedBtnProductNextList;

	gui::CButton*		m_pbtnProductLastList;
	gui::CSlot			m_slotClickedBtnProductLastList;

//	gui::CDialog*		m_pdlgBanner;

	gui::CButton*		m_pBtnCategory[eHLS_CATEGORY_NUM];
	gui::CSlot			m_slotClickedBtnCategory[eHLS_CATEGORY_NUM];

private:

	int					m_nCurrentCategory;
	int					m_nCurrentPage;
	int					m_nMaxPage;

	std::vector<sHLS_PRODUCTS*>	m_vecProducts[eHLS_CATEGORY_NUM];
	std::vector<sHLS_PRODUCTS*>	m_vecVisibleProducts;

	bool				m_bSearch;

	std::vector<sHLS_PRODUCTS*>	m_vecSearch;

public:

	CHLShopGui(const RwChar* pName);
	virtual ~CHLShopGui();

	RwBool		Create();
	void		Destroy();

private:

	void		CreateCategoryButton();
	void		CreateItems();
	void		SelectCategory(int iCategory);
	void		SetPage(int nCurPage, int nMaxPage);

	void		ClearVisibleItems();

public:

	void		HandleEvents(RWS::CMsg& msg);
	RwInt32		SwitchDialog(bool bOpen);

	void		OnPaint();
	void		OnMove(RwInt32 iOldX, RwInt32 iOldY);

	void		OnClickedBtnClose(gui::CComponent* pComponent);
	void		OnClickedBtnSearch(gui::CComponent* pComponent);
	void		OnClickedBtnInitSearchList(gui::CComponent* pComponent);
	void		OnClickedBtnYadrat(gui::CComponent* pComponent);
	void		OnClickedBtnCashRecharge(gui::CComponent* pComponent);

	void		OnClickedBtnProductFirstList(gui::CComponent* pComponent);
	void		OnClickedBtnProductPrevList(gui::CComponent* pComponent);
	void		OnClickedBtnProductNextList(gui::CComponent* pComponent);
	void		OnClickedBtnProductLastList(gui::CComponent* pComponent);

	void		OnClickUpButtonCategory(gui::CComponent* pComponent);
	void		OnClickBuyButton(gui::CComponent* pComponent);
	void		OnClickGiftButton(gui::CComponent* pComponent);

	void		OnMouseEnterItem(gui::CComponent* pComponent);
	void		OnMouseLeaveItem(gui::CComponent* pComponent);

private:

	void		ShowItemInfoWindow(bool bIsShow, sHLS_PRODUCTS* pProduct);

	void		InitSearch();

};