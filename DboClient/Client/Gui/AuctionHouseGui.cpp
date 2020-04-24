#include "precomp_dboclient.h"
#include "AuctionHouseGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

//  simul
#include "NtlCameraController.h"
#include "NtlSLEventFunc.h"
#include "NtlSobManager.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSLLogic.h"

// framework
#include "NtlApplication.h"


// dbo
#include "DisplayStringManager.h"
#include "DboEvent.h"
#include "IconMoveManager.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "InfoWndManager.h"
#include "AlarmManager.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"

// table




CAuctionHouseGui::CAuctionHouseGui(const RwChar * pName)
	:CNtlPLGui(pName),
	m_pStbTitle(NULL),
	m_pBtnClose(NULL)
{
	m_bUpdateItemTypeButtons = FALSE;
	m_byItemTypeMoveCounter = 0;

	m_byPage = eAUCTION_TAB_TYPE_BUY;
	m_byClassType = INVALID_BYTE;
	m_byTabType = eAUCTION_TAB_TYPE_ALL;
	m_byItemType = INVALID_BYTE;
	m_byMinLevel = INVALID_BYTE;
	m_byMaxLevel = INVALID_BYTE;
	m_byRank = INVALID_BYTE;
	m_uiPage = 1;
	m_bySortType = eAUCTION_SORT_TYPE_NONE;

	m_uiMaxPage = 1;
	m_nCancelSaleIndex = -1;
	m_nSelectedItemIndex = -1;

	m_dwSellPrice = 1;
}

CAuctionHouseGui::~CAuctionHouseGui()
{
}

RwBool CAuctionHouseGui::Create()
{
	NTL_FUNCTION("CAuctionHouseGui::Create");

	if (!CNtlPLGui::Create("gui\\Market.rsr", "gui\\Market.srf", "gui\\Market.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	m_stfTop.SetType(CWindowby3::WT_VERTICAL);
	m_stfTop.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfDialogTL"));
	m_stfTop.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfDialogTM"));
	m_stfTop.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfDialogTR"));
	m_stfTop.SetSize(m_stfTop.GetWidth(), m_stfTop.GetHeight());
	m_stfTop.SetPositionfromParent(0, 0);
	m_stfMid.SetType(CWindowby3::WT_VERTICAL);
	m_stfMid.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfDialogML"));
	m_stfMid.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfDialogMM"));
	m_stfMid.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfDialogMR"));
	m_stfMid.SetSize(m_stfMid.GetWidth(), m_stfMid.GetHeight());
	m_stfMid.SetPositionfromParent(0, 60);
	m_stfBot.SetType(CWindowby3::WT_VERTICAL);
	m_stfBot.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfDialogBL"));
	m_stfBot.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfDialogBM"));
	m_stfBot.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfDialogBR"));
	m_stfBot.SetSize(m_stfBot.GetWidth(), m_stfBot.GetHeight());
	m_stfBot.SetPositionfromParent(0, 630);

	// Title
	rect.SetRect(24, 6, 234, 24);
	m_pStbTitle = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pStbTitle->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pStbTitle->SetText(GetDisplayStringManager()->GetString("DST_MARKET_TITLE"));
	m_pStbTitle->Enable(false);

	// Info text
	rect.SetRect(77, 647, 590, 667);
	m_pstdInfoText = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pstdInfoText->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstdInfoText->SetTextStyle(COMP_TEXT_CENTER);
	m_pstdInfoText->SetText(GetDisplayStringManager()->GetString("DST_MARKET_BUYITEM_CLICK"));
	m_pstdInfoText->Enable(false);

	// close button
	rect.SetRect(765, 5, 779, 19);
	m_pBtnClose = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnClose->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("gui\\GameCommon.srf", "srfCloseBtnUp"));
	m_pBtnClose->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("gui\\GameCommon.srf", "srfCloseBtnFoc"));
	m_pBtnClose->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("gui\\GameCommon.srf", "srfCloseBtnDown"));
	m_pBtnClose->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("gui\\GameCommon.srf", "srfCloseBtnUp"));
	m_slotClickedBtnClose = m_pBtnClose->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_Close);

	// buy button ( must be inside main dialog, otherwise its not possible to click )
	rect.SetRect(654, 641, 753, 670);
	m_pBtnBuy = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnBuy->SetText(GetDisplayStringManager()->GetString("DST_MARKET_BUYTEXT"));
	m_pBtnBuy->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorBigUp"));
	m_pBtnBuy->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorBigFoc"));
	m_pBtnBuy->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorBigDwon"));
	m_pBtnBuy->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorBigDwon"));
	m_slotClickedBtnBuy = m_pBtnBuy->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_Buy);

	// order item by name button
	rect.SetRect(130, 168, 218, 192);
	m_pBtnOrderItemName = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnOrderItemName->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidUp"));
	m_pBtnOrderItemName->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidFoc"));
	m_pBtnOrderItemName->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidDown"));
	m_pBtnOrderItemName->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidDown"));
	m_pBtnOrderItemName->SetTextUpColor(INFOCOLOR_7);
	m_pBtnOrderItemName->SetTextDisableColor(INFOCOLOR_7);
	m_pBtnOrderItemName->SetTextDownColor(INFOCOLOR_7);
	m_pBtnOrderItemName->SetText(GetDisplayStringManager()->GetString("DST_MARKET_NAME"));
	m_pBtnOrderItemName->ClickEnable(TRUE);
	m_slotClickedBtnOrderItemName = m_pBtnOrderItemName->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_OrderItemName);

	// order item by rank button
	rect.SetRect(320, 168, 381, 192);
	m_pBtnOrderItemRank = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnOrderItemRank->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonShotUp"));
	m_pBtnOrderItemRank->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonShotFoc"));
	m_pBtnOrderItemRank->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonShotDown"));
	m_pBtnOrderItemRank->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonShotDown"));
	m_pBtnOrderItemRank->SetTextUpColor(INFOCOLOR_7);
	m_pBtnOrderItemRank->SetTextDisableColor(INFOCOLOR_7);
	m_pBtnOrderItemRank->SetTextDownColor(INFOCOLOR_7);
	m_pBtnOrderItemRank->SetText(GetDisplayStringManager()->GetString("DST_MARKET_GRADE"));
	m_pBtnOrderItemRank->ClickEnable(TRUE);
	m_slotClickedBtnOrderItemRank = m_pBtnOrderItemRank->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_OrderItemRank);

	// order item by required level button
	rect.SetRect(385, 168, 473, 192);
	m_pBtnOrderItemRequiredLevel = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnOrderItemRequiredLevel->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidUp"));
	m_pBtnOrderItemRequiredLevel->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidFoc"));
	m_pBtnOrderItemRequiredLevel->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidDown"));
	m_pBtnOrderItemRequiredLevel->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidDown"));
	m_pBtnOrderItemRequiredLevel->SetTextUpColor(INFOCOLOR_7);
	m_pBtnOrderItemRequiredLevel->SetTextDisableColor(INFOCOLOR_7);
	m_pBtnOrderItemRequiredLevel->SetTextDownColor(INFOCOLOR_7);
	m_pBtnOrderItemRequiredLevel->SetText(GetDisplayStringManager()->GetString("DST_MARKET_USELEVLE"));
	m_pBtnOrderItemRequiredLevel->ClickEnable(TRUE);
	m_slotClickedBtnOrderItemRequiredLevel = m_pBtnOrderItemRequiredLevel->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_OrderItemLevel);

	// order item by price button
	rect.SetRect(481, 168, 542, 192);
	m_pBtnOrderItemPrice = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnOrderItemPrice->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonShotUp"));
	m_pBtnOrderItemPrice->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonShotFoc"));
	m_pBtnOrderItemPrice->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonShotDown"));
	m_pBtnOrderItemPrice->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonShotDown"));
	m_pBtnOrderItemPrice->SetTextUpColor(INFOCOLOR_7);
	m_pBtnOrderItemPrice->SetTextDisableColor(INFOCOLOR_7);
	m_pBtnOrderItemPrice->SetTextDownColor(INFOCOLOR_7);
	m_pBtnOrderItemPrice->SetText(GetDisplayStringManager()->GetString("DST_MARKET_PRICE"));
	m_pBtnOrderItemPrice->ClickEnable(TRUE);
	m_slotClickedBtnOrderItemPrice = m_pBtnOrderItemPrice->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_OrderItemPrice);

	// order item by remaining time button
	rect.SetRect(560, 168, 648, 192);
	m_pBtnOrderItemTimeRemaining = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnOrderItemTimeRemaining->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidUp"));
	m_pBtnOrderItemTimeRemaining->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidFoc"));
	m_pBtnOrderItemTimeRemaining->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidDown"));
	m_pBtnOrderItemTimeRemaining->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidDown"));
	m_pBtnOrderItemTimeRemaining->SetTextUpColor(INFOCOLOR_7);
	m_pBtnOrderItemTimeRemaining->SetTextDisableColor(INFOCOLOR_7);
	m_pBtnOrderItemTimeRemaining->SetTextDownColor(INFOCOLOR_7);
	m_pBtnOrderItemTimeRemaining->SetText(GetDisplayStringManager()->GetString("DST_MARKET_LAST_TIME"));
	m_pBtnOrderItemTimeRemaining->ClickEnable(TRUE);
	m_slotClickedBtnOrderItemTimeRemaining = m_pBtnOrderItemTimeRemaining->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_OrderItemRemainingTime);

	// seller ( unable to click ! )
	rect.SetRect(655, 168, 743, 192);
	m_pBtnOrderItemSeller = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnOrderItemSeller->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidUp"));
	m_pBtnOrderItemSeller->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidUp"));
	m_pBtnOrderItemSeller->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidUp"));
	m_pBtnOrderItemSeller->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonMidUp"));
	m_pBtnOrderItemSeller->SetTextUpColor(INFOCOLOR_7);
	m_pBtnOrderItemSeller->SetTextDisableColor(INFOCOLOR_7);
	m_pBtnOrderItemSeller->SetTextDownColor(INFOCOLOR_7);
	m_pBtnOrderItemSeller->SetText(GetDisplayStringManager()->GetString("DST_MARKET_SELLER"));
	m_pBtnOrderItemSeller->ClickEnable(FALSE);

	//the actual items
	for (int i = 0; i < DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE; i++)
	{
		int nExtra = i * 40;

		m_pItems[i] = new CAuctionHouseGui::sITEM;

		m_pItems[i]->m_srfline.SetPositionfromParent(55, 200 + nExtra);
		m_pItems[i]->m_srfline.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfline"));

	//	m_pItems[i]->m_srfItemIcon.SetRect(13, 200 + nExtra, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
		m_pItems[i]->m_srfItemIcon.SetPositionfromParent(13, 200 + nExtra);
		m_pItems[i]->m_srfItemIcon.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfItemIcon"));

		// zeni icon
		m_pItems[i]->m_srfZenny.SetPositionfromParent(548, 200 + nExtra + 14);
		m_pItems[i]->m_srfZenny.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfZenny"));

		rect.SetRect(55, 200 + nExtra, 742, 200 + nExtra + 40);
		m_pItems[i]->m_pBtnBuy = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		m_pItems[i]->m_pBtnBuy->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "SelectFocBtn"));
		m_pItems[i]->m_pBtnBuy->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "SelectDwonBtn"));
	//	m_pItems[i]->m_pBtnBuy->SetToggleMode(true);
		m_pItems[i]->m_slotMouseDownBtnBuy = m_pItems[i]->m_pBtnBuy->SigMouseDown().Connect(this, &CAuctionHouseGui::OnMouseDown_ItemBuy);
		m_pItems[i]->m_slotMouseUpBtnBuy = m_pItems[i]->m_pBtnBuy->SigMouseUp().Connect(this, &CAuctionHouseGui::OnMouseUp_ItemBuy);


		// item name
		rect.SetRect(46, 200 + nExtra + 9, 331, 200 + nExtra + 33);
		m_pItems[i]->m_pstbItemName = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		m_pItems[i]->m_pstbItemName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_pItems[i]->m_pstbItemName->SetTextStyle(COMP_TEXT_CENTER);
		m_pItems[i]->m_pstbItemName->SetText(L"-");
		m_pItems[i]->m_pstbItemName->Enable(false);

		// item rank
		rect.SetRect(335, 200 + nExtra + 9, 375, 200 + nExtra + 33);
		m_pItems[i]->m_pstbItemRank = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		m_pItems[i]->m_pstbItemRank->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_pItems[i]->m_pstbItemRank->SetTextStyle(COMP_TEXT_CENTER);
		m_pItems[i]->m_pstbItemRank->SetText(L"-");
		m_pItems[i]->m_pstbItemRank->Enable(false);

		// required level
		rect.SetRect(410, 200 + nExtra + 9, 450, 200 + nExtra + 33);
		m_pItems[i]->m_pstbRequiredLevel = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		m_pItems[i]->m_pstbRequiredLevel->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_pItems[i]->m_pstbRequiredLevel->SetTextStyle(COMP_TEXT_CENTER);
		m_pItems[i]->m_pstbRequiredLevel->SetText(L"-");
		m_pItems[i]->m_pstbRequiredLevel->Enable(false);

		// item price
		rect.SetRect(460, 200 + nExtra + 9, 550, 200 + nExtra + 33);
		m_pItems[i]->m_pstbItemPrice = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		m_pItems[i]->m_pstbItemPrice->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_pItems[i]->m_pstbItemPrice->SetTextStyle(COMP_TEXT_CENTER);
		m_pItems[i]->m_pstbItemPrice->SetText(L"-");
		m_pItems[i]->m_pstbItemPrice->Enable(false);

		// time remaining
		rect.SetRect(575, 200 + nExtra + 9, 635, 200 + nExtra + 33);
		m_pItems[i]->m_pstbTimeRemaining = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		m_pItems[i]->m_pstbTimeRemaining->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_pItems[i]->m_pstbTimeRemaining->SetTextStyle(COMP_TEXT_CENTER);
		m_pItems[i]->m_pstbTimeRemaining->SetText(L"-");
		m_pItems[i]->m_pstbTimeRemaining->Enable(false);

		// seller
		rect.SetRect(645, 200 + nExtra + 9, 745, 200 + nExtra + 33);
		m_pItems[i]->m_pstbItemSeller = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		m_pItems[i]->m_pstbItemSeller->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_pItems[i]->m_pstbItemSeller->SetTextStyle(COMP_TEXT_CENTER);
		m_pItems[i]->m_pstbItemSeller->SetText(L"-");
		m_pItems[i]->m_pstbItemSeller->Enable(false);

		// cancel button (only visible in sell page)
		rect.SetRect(659, 200 + nExtra + 7, 739, 200 + nExtra + 31);
		m_pItems[i]->m_pBtnCancel = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		m_pItems[i]->m_pBtnCancel->SetText(GetDisplayStringManager()->GetString("DST_MARKET_SELL_NOT"));
		m_pItems[i]->m_pBtnCancel->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidUp"));
		m_pItems[i]->m_pBtnCancel->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidFoc"));
		m_pItems[i]->m_pBtnCancel->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDwon"));
		m_pItems[i]->m_pBtnCancel->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDis"));
		m_pItems[i]->m_pBtnCancel->Show(false);
		m_pItems[i]->m_slotClickedBtnCancel = m_pItems[i]->m_pBtnCancel->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_CancelItem);
	}

	m_srfLastline.SetPositionfromParent(13, 595);
	m_srfLastline.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfLastline"));


	// Init buy page
	InitBuyPage();

	// init sell page
	InitSellPage();
	m_pPageDialog[eAUCTION_TAB_TYPE_SELL]->Show(false); // hide sell page


	// cbb button. Info: below init and buy page, so the drop-down is above the sell/buy page stuff
	rect.SetRect(562, 18, 759, 41);
	m_cbbPage = NTL_NEW gui::CComboBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), 0, 0, 20, 20, 60, 2);
	m_cbbPage->SetTextFont(DEFAULT_FONT);
	m_cbbPage->SetListFont(DEFAULT_FONT);
	m_cbbPage->SetTextStyle(COMP_TEXT_CENTER);
	m_cbbPage->SetListTextStyle(COMP_TEXT_CENTER);
	m_cbbPage->SetTextColor(RGB(255, 255, 255));
	m_cbbPage->SetItemHeight(30);
	m_cbbPage->AddSurfaceButtonUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonUp"));
	m_cbbPage->AddSurfaceButtonDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonDown"));
	m_cbbPage->AddSurfaceButtonFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonFocus"));
	m_cbbPage->AddSurfaceButtonDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonDisable"));
	m_cbbPage->AddStaticBoxSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticBig"));
	m_cbbPage->AddListBoxMaskSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticColorBig"));
	m_cbbPage->AddLIstBoxSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfComboBoxBig"));
	m_cbbPage->AddItem(GetDisplayStringManager()->GetString("DST_MARKET_BUYTEXT"), eAUCTION_TAB_TYPE_BUY);
	m_cbbPage->AddItem(GetDisplayStringManager()->GetString("DST_MARKET_SELLTEXT"), eAUCTION_TAB_TYPE_SELL);
	m_cbbPage->SelectItem(0);
	m_slotSelectPage = m_cbbPage->SigSelected().Connect(this, &CAuctionHouseGui::OnSelectPage);


	// next/prev buttons
	rect.SetRect(150, 1 + 610, 150 + 33, 1 + 610 + 21); // 33 width, 21 height
	m_pbtnPrevPrev = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pbtnPrevPrev->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowNextLeftUp"));
	m_pbtnPrevPrev->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowNextLeftFoc"));
	m_pbtnPrevPrev->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowNextLeftDown"));
	m_pbtnPrevPrev->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowNextLeftDown"));
	m_slotClickedBtnPrevPrev = m_pbtnPrevPrev->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_PrevPrev);

	rect.SetRect(190, 1 + 610, 190 + 33, 1 + 610 + 21); // 33 width, 21 height
	m_pbtnPrev = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pbtnPrev->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowLeftUp"));
	m_pbtnPrev->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowLeftFoc"));
	m_pbtnPrev->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowLeftDown"));
	m_pbtnPrev->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowLeftDown"));
	m_slotClickedBtnPrev = m_pbtnPrev->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_Prev);

	rect.SetRect(561, 1 + 610, 561 + 33, 1 + 610 + 21); // 33 width, 21 height
	m_pbtnNext = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pbtnNext->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowRightUp"));
	m_pbtnNext->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowRightFoc"));
	m_pbtnNext->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowRightDown"));
	m_pbtnNext->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowRightDown"));
	m_slotClickedBtnNext = m_pbtnNext->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_Next);

	rect.SetRect(601, 1 + 610, 601 + 33, 1 + 610 + 21); // 33 width, 21 height
	m_pbtnNextNext = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pbtnNextNext->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowNextRightUp"));
	m_pbtnNextNext->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowNextRightFoc"));
	m_pbtnNextNext->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowNextRightDown"));
	m_pbtnNextNext->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowNextRightDown"));
	m_slotClickedBtnNextNext = m_pbtnNextNext->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_NextNext);

	for (int i = 0; i < AH_MAX_PAGES_COUNT; i++)
	{
		int nExtra = i * 30;

		rect.SetRect(245 + nExtra, 1 + 610, 245 + nExtra + 20, 1 + 610 + 21); // 20 width, 21 height
		m_pBtnPageNum[i] = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		m_pBtnPageNum[i]->SetText(i + 1); // +1 coz we start with number 1 and not 0
		m_pBtnPageNum[i]->SetTextDownColor(RGB(255, 0, 0));
		m_pBtnPageNum[i]->SetTextFocusColor(RGB(255, 0, 0));
		m_pSlotPageNum[i] = m_pBtnPageNum[i]->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_PageNumber);
	}


	m_slotMove = m_pThis->SigMove().Connect(this, &CAuctionHouseGui::OnMove);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CAuctionHouseGui::OnPaint);
	m_slotMouseUp = m_pPageDialog[eAUCTION_TAB_TYPE_SELL]->SigMouseUp().Connect(this, &CAuctionHouseGui::OnMouseUp);
	m_slotMouseDown = m_pPageDialog[eAUCTION_TAB_TYPE_SELL]->SigMouseDown().Connect(this, &CAuctionHouseGui::OnMouseDown);
	m_slotMainMouseMove = m_pThis->SigMouseMove().Connect(this, &CAuctionHouseGui::OnMainMouseMove);
	m_slotSellMouseMove = m_pPageDialog[eAUCTION_TAB_TYPE_SELL]->SigMouseMove().Connect(this, &CAuctionHouseGui::OnSellMouseMove);
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect(this, &CAuctionHouseGui::OnCaptureMouseDown); // required to put AH above other UI's

	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventAuctionHouseListRes);
	LinkMsg(g_EventAuctionHouseListData);
	LinkMsg(g_EventCalcPopupResult);
	LinkMsg(g_EventRegAuctionHouseItem);
	LinkMsg(g_EventMsgBoxResult);
	LinkMsg(g_EventAuctionHouseSellRes);
	LinkMsg(g_EventAuctionHouseSellCancelRes);
	LinkMsg(g_EventAuctionHouseBuyRes);
	LinkMsg(g_EventDialog);

	m_pThis->Show(false);

	NTL_RETURN(TRUE);
}

void CAuctionHouseGui::Destroy()
{
	NTL_FUNCTION("CAuctionHouseGui::Destroy");

	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventAuctionHouseListRes);
	UnLinkMsg(g_EventAuctionHouseListData);
	UnLinkMsg(g_EventCalcPopupResult);
	UnLinkMsg(g_EventRegAuctionHouseItem);
	UnLinkMsg(g_EventMsgBoxResult);
	UnLinkMsg(g_EventAuctionHouseSellRes);
	UnLinkMsg(g_EventAuctionHouseSellCancelRes);
	UnLinkMsg(g_EventAuctionHouseBuyRes);
	UnLinkMsg(g_EventDialog);

	DeleteItems();// delete all current listed items

	ClearSellItem();

	for (int i = 0; i < DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE; i++)
	{
		m_pItems[i]->m_ItemSlot.Destroy();

		delete m_pItems[i]->m_pBtnBuy;
		delete m_pItems[i]->m_pstbItemName;
		delete m_pItems[i]->m_pstbItemRank;
		delete m_pItems[i]->m_pstbRequiredLevel;
		delete m_pItems[i]->m_pstbItemPrice;
		delete m_pItems[i]->m_pstbTimeRemaining;
		delete m_pItems[i]->m_pstbItemSeller;
		delete m_pItems[i]->m_pBtnCancel;

		delete m_pItems[i];
	}

	delete m_pStbTitle;
	delete m_pstdInfoText;
	delete m_pBtnClose;
	delete m_pBtnBuyNot;
	delete m_pBtnBuy;
	delete m_cbbPage;
	delete m_ptabType;
	delete m_pstbItemNameInput;
	delete m_pstbMinLevel;
	delete m_pstbMaxLevel;
	delete m_pbtnSearch;
	delete m_pbtnBuyRefresh;
	delete m_pbtnArrowLeftItemType;
	delete m_pbtnArrowRightItemType;
	delete m_pcbbItemRank;
	delete m_pcbbItemClass;
	delete m_pBtnItemType1;
	delete m_pBtnItemType2;
	delete m_pBtnItemType3;
	delete m_pBtnItemType4;
	delete m_pBtnItemType5;
	delete m_pBtnItemType6;
	delete m_pBtnItemType7;
	delete m_pBtnItemType8;
	delete m_pBtnOrderItemName;
	delete m_pBtnOrderItemRank;
	delete m_pBtnOrderItemRequiredLevel;
	delete m_pBtnOrderItemPrice;
	delete m_pBtnOrderItemTimeRemaining;
	delete m_pBtnOrderItemSeller;
	delete m_pstbItemNameTitle;
	delete m_pstbRankTitle;
	delete m_pstbUseLevelTitle;
	delete m_pstbItemPriceTitle;
	delete m_pstbLastingTimeTitle;
	delete m_pstbSellPriceTitle;
	delete m_pstbItemName;
	delete m_pstbItemRank;
	delete m_pstbUseLevel;
	delete m_pbtnItemPrice;
	delete m_cbbLastingTime;
	delete m_pstbSellPrice;
	delete m_pBtnRegisterItem;
	delete m_pBtnSellRefresh;
	delete m_pbtnPrevPrev;
	delete m_pbtnPrev;
	delete m_pbtnNext;
	delete m_pbtnNextNext;

	for (int i = 0; i < AH_MAX_PAGES_COUNT; i++)
	{
		delete m_pBtnPageNum[i];
	}

	delete m_pPageDialog[eAUCTION_TAB_TYPE_BUY];
	delete m_pPageDialog[eAUCTION_TAB_TYPE_SELL];

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

void CAuctionHouseGui::Update(RwReal fElapsed)
{
}

void CAuctionHouseGui::HandleEvents(RWS::CMsg & msg)
{
	NTL_FUNCTION("CAuctionHouseGui::HandleEvents");

	if (msg.Id == g_EventNPCDialogOpen)
	{
		if (IsShow() == TRUE)
			return;

		SDboNpcDialogOpen* pEvent = reinterpret_cast<SDboNpcDialogOpen*>(msg.pData);

		if (pEvent->eDialog == DIALOG_AUCTION_HOUSE)
		{
			bool bSuccess = true;
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

			if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, m_uiPage, m_bySortType, eAUCTION_TAB_TYPE_BUY))
			{
				if (!GetDialogManager()->OpenDialog(DIALOG_AUCTION_HOUSE, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID()))
					bSuccess = false;
			}
			else
			{
				GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
				bSuccess = false;
			}

			if (bSuccess == false)
			{
				Logic_CancelNpcFacing();
			}
			else
			{
				CNtlSLEventGenerator::CameraNpc(pEvent->hSerialId);
				GetNtlWorldConcept()->AddWorldPlayConcept(WORLD_PLAY_NPC_COMMU);
			}
		}
	}
	else if (msg.Id == g_EventAuctionHouseListRes)
	{
		if (IsShow() == FALSE)
			return;

		SNtlEventAuctionHouseListRes* pEvent = reinterpret_cast<SNtlEventAuctionHouseListRes*>(msg.pData);

		OnAuctionHouseListRes(pEvent->uiPage, pEvent->uiMaxPage, pEvent->byListNum, pEvent->byPage);
	}
	else if (msg.Id == g_EventAuctionHouseListData)
	{
		if (IsShow() == FALSE)
			return;

		SNtlEventAuctionHouseListData* pEvent = reinterpret_cast<SNtlEventAuctionHouseListData*>(msg.pData);

		sTENKAICHIDAISIJYOU_DATA_CLIENT* pData = (sTENKAICHIDAISIJYOU_DATA_CLIENT*)pEvent->pItemData;

		ParseItemData(pData);
	}
	else if (msg.Id == g_EventCalcPopupResult)
	{
		if (IsShow() == FALSE)
			return;

		SDboEventCalcPopupResult* pData = reinterpret_cast<SDboEventCalcPopupResult*>(msg.pData);

		if (pData->nSrcPlace == PLACE_SUB_AUCTIONHOUSE_MIN_LEVEL)
		{
			m_byMinLevel = (BYTE)pData->uiValue;

			if (pData->uiValue == 0)
			{
				m_pbtnMinLevel->ClearText();
				return;
			}
			
			m_pbtnMinLevel->SetText(pData->uiValue);
		}
		else if (pData->nSrcPlace == PLACE_SUB_AUCTIONHOUSE_MAX_LEVEL)
		{
			if (pData->uiValue == 0)
				return;

			m_byMaxLevel = (BYTE)pData->uiValue;

			if (pData->uiValue == 0)
			{
				m_pbtnMaxLevel->ClearText();
				return;
			}

			m_pbtnMaxLevel->SetText(pData->uiValue);
		}
		else if (pData->nSrcPlace == PLACE_SUB_AUCTIONHOUSE_PRICE)
		{
			if (pData->uiValue == 0)
				return;

			m_dwSellPrice = pData->uiValue;

			m_pbtnItemPrice->SetText(Logic_FormatZeni(m_dwSellPrice));

			// set fee
			m_pstbSellPrice->SetText(Logic_FormatZeni(Dbo_GetAuctionHouseSellFee(m_dwSellPrice)));
		}
	}
	else if (msg.Id == g_EventRegAuctionHouseItem)
	{
		if (IsShow() == FALSE)
			return;

		SDboEventRegMailAttachItem* pData = (SDboEventRegMailAttachItem*)msg.pData;

		CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(pData->hHandle));
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

		sITEM_TBLDAT* pItemTbl = pItemAttr->GetItemTbl();

		CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

		if (m_byPage == eAUCTION_TAB_TYPE_BUY)
		{
			if (pItemAttr->IsNeedToIdentify())
				return;

			m_pstbItemNameInput->Show(false);
			m_pItemNameInput->SetText(pItemTextTable->GetText(pItemTbl->Name).c_str());
		}
		else if (m_byPage == eAUCTION_TAB_TYPE_SELL)
		{
			if (Logic_IsCanUserAuctionItem(pItemAttr) == FALSE)
			{
				GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_SELL");
				return;
			}

			if (m_SellItemSlot.GetSerial() != INVALID_SERIAL_ID)
			{
				ClearSellItem();
			}

			// disable item in inventory
			CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG, PLACE_SUB_AUCTIONHOUSE_ITEM, PLACE_BAG, pItem->GetParentItemSlotIdx(), pItem->GetItemSlotIdx());

			// set item
			m_SellItemSlot.Create(m_pThis, DIALOG_AUCTION_HOUSE, REGULAR_SLOT_ITEM_SOB, SDS_COUNT);
			m_SellItemSlot.SetPosition_fromParent(13 + 12, 44 + 62);
			m_SellItemSlot.SetIcon(pItem->GetSerialID(), pItemAttr->GetStackNum());

			// set item name and rank text color
			UINT32 color = Logic_GetItemRankColor(pItemAttr->GetRank());
			m_pstbItemName->SetTextColor(color);
			m_pstbItemRank->SetTextColor(color);

			// set item name
			if(pItemAttr->GetGrade() > 0)
				m_pstbItemName->Format("%ls + %u", pItemTextTable->GetText(pItemTbl->Name).c_str(), pItemAttr->GetGrade());
			else
				m_pstbItemName->SetText(pItemTextTable->GetText(pItemTbl->Name).c_str());

			// set rank
			m_pstbItemRank->SetText(Logic_GetItemRankName(pItemAttr->GetRank()));

			// set reqired level
			m_pstbUseLevel->SetText(pItemTbl->byNeed_Min_Level);

			m_dwSellPrice = (pItemTbl->dwSell_Price == 0) ? 1 : pItemTbl->dwSell_Price * 2;

			// set price
			m_pbtnItemPrice->SetText(Logic_FormatZeni(m_dwSellPrice));

			// set fee
			m_pstbSellPrice->SetText(Logic_FormatZeni(Dbo_GetAuctionHouseSellFee(m_dwSellPrice)));
		}
	}
	else if (msg.Id == g_EventMsgBoxResult)
	{
		if (IsShow() == FALSE)
			return;

		SDboEventMsgBoxResult*	pEvent = reinterpret_cast<SDboEventMsgBoxResult*>(msg.pData);

		if (pEvent->eResult == MBR_OK)
		{
			if (pEvent->strID == "DST_MARKET_SELL")
			{
				if (m_SellItemSlot.GetSerial() == INVALID_SERIAL_ID)
					return;

				// required level check
				if (Logic_GetLevel(GetNtlSLGlobal()->GetSobAvatar()) < NTL_AUCTIONHOUSE_REQUIRED_LV)
				{
					GetAlarmManager()->FormattedAlarmMessage("DST_HOIPOIMIX_REQUIRED_LEVEL", FALSE, NULL, NTL_AUCTIONHOUSE_REQUIRED_LV);
					return;
				}

				CNtlSobItem* pSobItem = m_SellItemSlot.GetSobItem();
				
				// register item
				if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseSellReq(m_dwSellPrice, pSobItem->GetParentItemSlotIdx() + CONTAINER_TYPE_BAG1, pSobItem->GetItemSlotIdx(), m_SellItemSlot.GetCount(), m_cbbLastingTime->GetSelectedItemData()))
				{
					DisableAllButtons();
				}
				else
				{
					GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
				}
			}
			else if (pEvent->strID == "DST_MARKET_SELL_CANCEL")
			{
				std::map<int, sTENKAICHIDAISIJYOU_DATA_CLIENT*>::iterator it = m_mapItemData.find(m_nCancelSaleIndex);
				if (it != m_mapItemData.end())
				{
					// cancel item request
					if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseSellCancelReq(it->second->nItem))
					{
						DisableAllButtons();
					}
					else
					{
						GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
					}
				}

				m_nCancelSaleIndex = -1;
			}
			else if (pEvent->strID == "DST_MARKET_BUY")
			{
				// required level check
				if (Logic_GetLevel(GetNtlSLGlobal()->GetSobAvatar()) < NTL_AUCTIONHOUSE_REQUIRED_LV)
				{
					GetAlarmManager()->FormattedAlarmMessage("DST_HOIPOIMIX_REQUIRED_LEVEL", FALSE, NULL, NTL_AUCTIONHOUSE_REQUIRED_LV);
					return;
				}

				std::map<int, sTENKAICHIDAISIJYOU_DATA_CLIENT*>::iterator it = m_mapItemData.find(m_nSelectedItemIndex);
				if (it != m_mapItemData.end())
				{
					// cancel item request
					if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseBuyReq(it->second->nItem))
					{
						DisableAllButtons();
					}
					else
					{
						GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
					}
				}
			}
		}
	}
	else if (msg.Id == g_EventAuctionHouseSellRes)
	{
		SNtlEventAuctionHouseSellRes* pEvent = reinterpret_cast<SNtlEventAuctionHouseSellRes*>(msg.pData);

		CNtlSobItem* pItem = m_SellItemSlot.GetSobItem();
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

		// update inventory item
		Logic_SetItemStack(pItem, pItemAttr->GetStackNum() - pEvent->byCount);
		
		// clear sell item
		ClearSellItem();

		// after adding the item, we will reload the list
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), L"", m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, m_uiPage, m_bySortType, eAUCTION_TAB_TYPE_SELL))
		{
			DisableAllButtons();
			DeleteItems(); // delete all current listed items
		}
		else
		{
			GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
		}
	}
	else if (msg.Id == g_EventAuctionHouseSellCancelRes)
	{
		SNtlEventAuctionHouseSellCancelRes* pEvent = reinterpret_cast<SNtlEventAuctionHouseSellCancelRes*>(msg.pData);

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		UINT32 uiPage = m_uiPage;
		if (m_uiPage > 1 && m_mapItemData.size() == 1)
			uiPage -= 1;

		if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), L"", m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, uiPage, m_bySortType, eAUCTION_TAB_TYPE_SELL))
		{
			m_uiPage = uiPage;
			DisableAllButtons();
			DeleteItems(); // delete all current listed items
		}
		else
		{
			GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
		}
	}
	else if (msg.Id == g_EventAuctionHouseBuyRes)
	{
		SNtlEventAuctionHouseBuyRes* pEvent = reinterpret_cast<SNtlEventAuctionHouseBuyRes*>(msg.pData);

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		UINT32 uiPage = m_uiPage;
		if (m_uiPage > 1 && m_mapItemData.size() == 1)
			uiPage -= 1;

		if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, uiPage, m_bySortType, eAUCTION_TAB_TYPE_BUY))
		{
			m_uiPage = uiPage;
			DisableAllButtons();
			DeleteItems(); // delete all current listed items
		}
		else
		{
			GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
		}
	}
	else if (msg.Id == g_EventDialog)
	{
		SDboEventDialog* pPacket = (SDboEventDialog*)msg.pData;
		if (pPacket->iType == DIALOGEVENT_OPEN_FAIL_NPC_DIALOG &&
			pPacket->iDestDialog == DIALOG_AUCTION_HOUSE)
		{
			// Failed to open AH
			Logic_CancelNpcFacing();
		}
	}
}

RwInt32 CAuctionHouseGui::SwitchDialog(bool bOpen)
{
	if (!bOpen)
	{
		if(GetInfoWndManager()->GetRequestGui() == DIALOG_AUCTION_HOUSE)
			GetInfoWndManager()->ShowInfoWindow(FALSE);

		Logic_CancelNpcFacing();

		m_cbbPage->SelectItem(0);

		InitSearch();
		DeleteItems(); // delete all current listed items
		ClearSellItem();

		SetPage(eAUCTION_TAB_TYPE_BUY);
	}
	else
	{
	}

	m_pThis->Show(bOpen);

	NTL_RETURN(TRUE);
}

void CAuctionHouseGui::SetPage(BYTE byPage)
{
	if (m_byPage == byPage)
		return;

	m_byPage = byPage;

	switch (m_byPage)
	{
		case eAUCTION_TAB_TYPE_BUY:
		{
			m_pBtnBuy->Show(true);

			m_pBtnOrderItemSeller->SetText(GetDisplayStringManager()->GetString("DST_MARKET_SELLER")); // turn to "Seller"

			m_pPageDialog[eAUCTION_TAB_TYPE_SELL]->Show(false);
			m_pPageDialog[eAUCTION_TAB_TYPE_BUY]->Show(true);
		}
		break;

		case eAUCTION_TAB_TYPE_SELL:
		{
			m_pBtnBuy->Show(false);

			m_pBtnOrderItemSeller->SetText(GetDisplayStringManager()->GetString("DST_MARKET_SELL_NOT")); // turn to "Cancel"

			m_pPageDialog[eAUCTION_TAB_TYPE_BUY]->Show(false);
			m_pPageDialog[eAUCTION_TAB_TYPE_SELL]->Show(true);
		}
		break;


		default: DBO_ASSERT(FALSE, "Page: " << m_byPage << " does not exist."); break;
	}
}

void CAuctionHouseGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = GetPosition();

	m_stfTop.SetPositionbyParent(rect.left, rect.top);
	m_stfMid.SetPositionbyParent(rect.left, rect.top);
	m_stfBot.SetPositionbyParent(rect.left, rect.top);

	for (int i = 0; i < eAUCTION_TAB_TYPE_COUNT; i++)
	{
		m_srfBackgroundLT[i].SetPositionbyParent(rect.left, rect.top);
		m_srfBackgroundRT[i].SetPositionbyParent(rect.left, rect.top);
		m_srfBackgroundLM[i].SetPositionbyParent(rect.left, rect.top);
		m_srfBackgroundMM[i].SetPositionbyParent(rect.left, rect.top);
		m_srfBackgroundRM[i].SetPositionbyParent(rect.left, rect.top);
		m_srfBackgroundLB[i].SetPositionbyParent(rect.left, rect.top);
		m_srfBackgroundMB[i].SetPositionbyParent(rect.left, rect.top);
		m_srfBackgroundRB[i].SetPositionbyParent(rect.left, rect.top);
	}

	for (int i = 0; i < DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE; i++)
	{
		m_pItems[i]->m_srfline.SetPositionbyParent(rect.left, rect.top);
		m_pItems[i]->m_srfItemIcon.SetPositionbyParent(rect.left, rect.top);
		m_pItems[i]->m_srfZenny.SetPositionbyParent(rect.left, rect.top);
		m_pItems[i]->m_ItemSlot.SetParentPosition(rect.left, rect.top);
	}

	m_srfItemNameInput.SetPositionbyParent(rect.left, rect.top);

	m_srfLastline.SetPositionbyParent(rect.left, rect.top);

	m_srfSellline.SetPositionbyParent(rect.left, rect.top);
	m_srfBackgroundMT.SetPositionbyParent(rect.left, rect.top);

	m_srfItemIcon.SetPositionbyParent(rect.left, rect.top);

	m_SellItemSlot.SetParentPosition(rect.left, rect.top);

	//CheckInfoWindow(); TO DO
}

void CAuctionHouseGui::OnPaint()
{
	m_stfTop.Render();
	m_stfMid.Render();
	m_stfBot.Render();

	m_srfBackgroundLT[m_byPage].Render();
	m_srfBackgroundRT[m_byPage].Render();
	m_srfBackgroundLM[m_byPage].Render();
	m_srfBackgroundMM[m_byPage].Render();
	m_srfBackgroundRM[m_byPage].Render();
	m_srfBackgroundLB[m_byPage].Render();
	m_srfBackgroundMB[m_byPage].Render();
	m_srfBackgroundRB[m_byPage].Render();

	if (m_byPage == eAUCTION_TAB_TYPE_BUY)
	{
		m_srfItemNameInput.Render();
	}
	else
	{
		m_srfSellline.Render();
		m_srfBackgroundMT.Render();
		m_srfItemIcon.Render();

		m_SellItemSlot.Paint();
	}

	for (int i = 0; i < DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE; i++)
	{
		m_pItems[i]->m_srfline.Render();
		m_pItems[i]->m_srfItemIcon.Render();
		m_pItems[i]->m_srfZenny.Render();
		m_pItems[i]->m_ItemSlot.Paint();
	}

	m_srfLastline.Render();
}

void CAuctionHouseGui::OnMouseUp(const CKey & key)
{
	if (!IsShow())
	{
		return;
	}

	if (m_pPageDialog[eAUCTION_TAB_TYPE_SELL]->IsEnabled() == false)
		return;

	CRectangle mainRect = m_pPageDialog[eAUCTION_TAB_TYPE_SELL]->GetScreenRect();
	if (m_srfItemIcon.PtInRect((int)key.m_fX + mainRect.left, (int)key.m_fY + mainRect.top) == FALSE)
	{
		return;
	}

	if (key.m_nID == UD_LEFT_BUTTON)
	{
		if (GetIconMoveManager()->IsActive())
		{
			if (GetIconMoveManager()->GetSrcSerial() != INVALID_SERIAL_ID)
			{
				CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(GetIconMoveManager()->GetSrcSerial()));
				CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

				if (Logic_IsCanUserAuctionItem(pItemAttr) == FALSE)
				{
					GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_SELL");
					return;
				}

				if (m_SellItemSlot.GetSerial() != INVALID_SERIAL_ID)
				{
					ClearSellItem();
				}

				sITEM_TBLDAT* pItemTbl = pItemAttr->GetItemTbl();

				CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

				// disable item in inventory
				CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG, PLACE_SUB_AUCTIONHOUSE_ITEM, PLACE_BAG, pItem->GetParentItemSlotIdx(), pItem->GetItemSlotIdx());

				// set item
				m_SellItemSlot.Create(m_pThis, DIALOG_AUCTION_HOUSE, REGULAR_SLOT_ITEM_SOB, SDS_COUNT);
				m_SellItemSlot.SetPosition_fromParent(13 + 12, 44 + 62);
				m_SellItemSlot.SetIcon(pItem->GetSerialID(), GetIconMoveManager()->GetStackCount());

				// set item name and rank text color
				UINT32 color = Logic_GetItemRankColor(pItemAttr->GetRank());
				m_pstbItemName->SetTextColor(color);
				m_pstbItemRank->SetTextColor(color);

				// set item name
				if (pItemAttr->GetGrade() > 0)
					m_pstbItemName->Format("%ls + %u", pItemTextTable->GetText(pItemTbl->Name).c_str(), pItemAttr->GetGrade());
				else
					m_pstbItemName->SetText(pItemTextTable->GetText(pItemTbl->Name).c_str());

				// set rank
				m_pstbItemRank->SetText(Logic_GetItemRankName(pItemAttr->GetRank()));

				// set reuired level
				m_pstbUseLevel->SetText(pItemTbl->byNeed_Min_Level);

				m_dwSellPrice = (pItemTbl->dwSell_Price == 0) ? 1 : pItemTbl->dwSell_Price * 2;

				// set price
				m_pbtnItemPrice->SetText(Logic_FormatZeni(m_dwSellPrice));

				// set fee
				m_pstbSellPrice->SetText(Logic_FormatZeni(Dbo_GetAuctionHouseSellFee(m_dwSellPrice)));

				GetIconMoveManager()->IconMoveEnd();
			}
		}
	}
	else if (key.m_nID == UD_RIGHT_BUTTON)
	{
		if (!GetIconMoveManager()->IsActive())
		{
			if (m_SellItemSlot.GetItemTable() != NULL)
			{
				ClearSellItem();
			}
		}
	}
}

void CAuctionHouseGui::OnMouseDown(const CKey & key)
{
	if (!IsShow())
	{
		return;
	}

	if (m_pPageDialog[eAUCTION_TAB_TYPE_SELL]->IsEnabled() == false)
		return;

	CRectangle mainRect = m_pPageDialog[eAUCTION_TAB_TYPE_SELL]->GetScreenRect();

	if (m_srfItemIcon.PtInRect((int)key.m_fX + mainRect.left, (int)key.m_fY + mainRect.top) == FALSE)
	{
		DBO_WARNING_MESSAGE("OnMouseDown FAIL");
		return;
	}
}

void CAuctionHouseGui::OnMainMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	int nIndex = -1;
	
	for (RwInt32 i = 0; i < DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE; i++)
	{
		if (m_pItems[i]->m_ItemSlot.PtInRect(nX, nY))
		{
			if (m_pItems[i]->m_ItemSlot.GetItemTable() != NULL)
			{
				nIndex = i;
			}
			else
			{
				GetInfoWndManager()->ShowInfoWindow(FALSE);
			}

			break;
		}
	}

	if (nIndex != -1)
	{
		std::map<int, sTENKAICHIDAISIJYOU_DATA_CLIENT*>::iterator it = m_mapItemData.find(nIndex);
		if (it != m_mapItemData.end())
		{
			sTENKAICHIDAISIJYOU_DATA_CLIENT* pData = it->second;

			sITEM_DATA itemData;
			itemData.itemNo = pData->itemNo;
			itemData.byPlace = INVALID_BYTE;
			itemData.byPosition = INVALID_BYTE;
			itemData.byStackcount = pData->byCount;
			itemData.byRank = pData->byRank;
			itemData.byCurrentDurability = pData->byCurrentDurability;
			itemData.bNeedToIdentify = false;
			itemData.byGrade = pData->byGrade;
			itemData.byBattleAttribute = pData->byBattleAttribute;
			itemData.byRestrictState = pData->byRestrictState;
			wcscpy_s(itemData.awchMaker, NTL_MAX_SIZE_CHAR_NAME + 1, pData->awchMaker);
			memcpy(&itemData.sOptionSet, &pData->sOptionSet, sizeof(sITEM_OPTION_SET));
			itemData.byDurationType = eDURATIONTYPE_NORMAL;
			itemData.nUseStartTime = 0;
			itemData.nUseEndTime = 0;

			CRectangle rtScreen = m_pThis->GetScreenRect();
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM_DATA,
				rtScreen.left + m_pItems[nIndex]->m_ItemSlot.GetX_fromParent(),
				rtScreen.top + m_pItems[nIndex]->m_ItemSlot.GetY_fromParent() - 20,
				&itemData, DIALOG_AUCTION_HOUSE);
		}
	}
	else
	{
		GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

void CAuctionHouseGui::OnSellMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	CRectangle mainRect = m_pPageDialog[eAUCTION_TAB_TYPE_SELL]->GetScreenRect();
	if (m_srfItemIcon.PtInRect(nX + mainRect.left, nY + mainRect.top))
	{
		if (m_SellItemSlot.GetSerial() == INVALID_SERIAL_ID)
			return;
		CRectangle rtScreen = m_pThis->GetScreenRect();
		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM,
			rtScreen.left + m_SellItemSlot.GetX_fromParent(),
			rtScreen.top + m_SellItemSlot.GetY_fromParent() - 20,
			m_SellItemSlot.GetSobItem(), DIALOG_AUCTION_HOUSE);
	}
	else
	{
		GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

void CAuctionHouseGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_AUCTION_HOUSE, key.m_fX, key.m_fY);
}

void CAuctionHouseGui::OnSelectPage(INT nSelectIndex)
{
	if (nSelectIndex == m_byPage)
		return;

	InitSearch();

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	BYTE bySortType = (nSelectIndex == eAUCTION_TAB_TYPE_BUY) ? eAUCTION_SORT_TYPE_RANK_ASC : eAUCTION_SORT_TYPE_TIMELEFT_DESC;

	if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), L"", m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, m_uiPage, bySortType, (BYTE)nSelectIndex))
	{
		m_bySortType = bySortType;
		DisableAllButtons();
		DeleteItems(); // delete all current listed items
	}
	else
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}

void CAuctionHouseGui::OnClicked_Close(gui::CComponent * pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_AUCTION_HOUSE);
}

void CAuctionHouseGui::OnClicked_Search(gui::CComponent * pComponent)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if (m_byClassType == INVALID_BYTE && m_byItemType == INVALID_BYTE && m_byMinLevel == INVALID_BYTE && m_byMaxLevel == INVALID_BYTE && m_byRank == INVALID_BYTE && wcslen(m_pItemNameInput->GetText()) == 0)
	{
		GetAlarmManager()->AlarmMessage("DST_MARKET_ITEM_NOT_SERACH");
		return;
	}

	if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, 1, eAUCTION_SORT_TYPE_RANK_ASC, eAUCTION_TAB_TYPE_BUY))
	{
		m_uiPage = 1;
		m_bySortType = eAUCTION_SORT_TYPE_RANK_ASC;

		DisableAllButtons();
		DeleteItems(); // delete all current listed items
	}
	else
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}

void CAuctionHouseGui::OnClicked_BuyRefresh(gui::CComponent * pComponent)
{
	InitSearch();

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), L"", m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, m_uiPage, eAUCTION_SORT_TYPE_NONE, eAUCTION_TAB_TYPE_BUY))
	{
		m_bySortType = eAUCTION_SORT_TYPE_NONE;
		DisableAllButtons();
		DeleteItems(); // delete all current listed items

		m_bUpdateItemTypeButtons = TRUE;
	}
	else
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}

void CAuctionHouseGui::OnClicked_SellRefresh(gui::CComponent * pComponent)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	ClearSellItem();

	if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), L"", m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, 1, eAUCTION_SORT_TYPE_TIMELEFT_DESC, eAUCTION_TAB_TYPE_SELL))
	{
		m_uiPage = 1;
		m_bySortType = eAUCTION_SORT_TYPE_TIMELEFT_DESC;

		DisableAllButtons();
		DeleteItems(); // delete all current listed items
	}
	else
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}

void CAuctionHouseGui::OnClicked_LeftArrowItemType(gui::CComponent * pComponent)
{
	if (m_byItemTypeMoveCounter == 0)
		return;

	--m_byItemTypeMoveCounter;

	SetItemTypeButtonText();
}

void CAuctionHouseGui::OnClicked_RightArrowItemType(gui::CComponent * pComponent)
{
	size_t mapSize = m_mapItemType.size();

	if (mapSize < AH_VISIBLE_ITEM_TYPE_BUTTONS || m_byItemTypeMoveCounter == mapSize - 8)
		return;

	++m_byItemTypeMoveCounter;

	SetItemTypeButtonText();
}

void CAuctionHouseGui::OnClicked_ItemType1(gui::CComponent * pComponent)
{
	m_byItemType = GetItemTypeByButton(m_pBtnItemType1->GetTextComponent()->GetText().c_str());

	if (m_byItemType != INVALID_BYTE)
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, 1, eAUCTION_SORT_TYPE_RANK_ASC, eAUCTION_TAB_TYPE_BUY))
		{
			m_bySortType = eAUCTION_SORT_TYPE_RANK_ASC;

			DisableAllButtons();
			DeleteItems(); // delete all current listed items

		}
		else
		{
			GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
		}
	}
}

void CAuctionHouseGui::OnClicked_ItemType2(gui::CComponent * pComponent)
{
	m_byItemType = GetItemTypeByButton(m_pBtnItemType2->GetTextComponent()->GetText().c_str());

	if (m_byItemType != INVALID_BYTE)
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, 1, eAUCTION_SORT_TYPE_RANK_ASC, eAUCTION_TAB_TYPE_BUY))
		{
			m_bySortType = eAUCTION_SORT_TYPE_RANK_ASC;
			DisableAllButtons();
			DeleteItems(); // delete all current listed items
			
		}
		else
		{
			GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
		}
	}
}

void CAuctionHouseGui::OnClicked_ItemType3(gui::CComponent * pComponent)
{
	m_byItemType = GetItemTypeByButton(m_pBtnItemType3->GetTextComponent()->GetText().c_str());

	if (m_byItemType != INVALID_BYTE)
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, 1, eAUCTION_SORT_TYPE_RANK_ASC, eAUCTION_TAB_TYPE_BUY))
		{
			m_bySortType = eAUCTION_SORT_TYPE_RANK_ASC;
			DisableAllButtons();
			DeleteItems(); // delete all current listed items
		}
		else
		{
			GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
		}
	}
}

void CAuctionHouseGui::OnClicked_ItemType4(gui::CComponent * pComponent)
{
	m_byItemType = GetItemTypeByButton(m_pBtnItemType4->GetTextComponent()->GetText().c_str());

	if (m_byItemType != INVALID_BYTE)
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, 1, eAUCTION_SORT_TYPE_RANK_ASC, eAUCTION_TAB_TYPE_BUY))
		{
			m_bySortType = eAUCTION_SORT_TYPE_RANK_ASC;
			DisableAllButtons();
			DeleteItems(); // delete all current listed items
		}
		else
		{
			GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
		}
	}
}

void CAuctionHouseGui::OnClicked_ItemType5(gui::CComponent * pComponent)
{
	m_byItemType = GetItemTypeByButton(m_pBtnItemType5->GetTextComponent()->GetText().c_str());

	if (m_byItemType != INVALID_BYTE)
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, 1, eAUCTION_SORT_TYPE_RANK_ASC, eAUCTION_TAB_TYPE_BUY))
		{
			m_bySortType = eAUCTION_SORT_TYPE_RANK_ASC;
			DisableAllButtons();
			DeleteItems(); // delete all current listed items
		}
		else
		{
			GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
		}
	}
}

void CAuctionHouseGui::OnClicked_ItemType6(gui::CComponent * pComponent)
{
	m_byItemType = GetItemTypeByButton(m_pBtnItemType6->GetTextComponent()->GetText().c_str());

	if (m_byItemType != INVALID_BYTE)
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, 1, eAUCTION_SORT_TYPE_RANK_ASC, eAUCTION_TAB_TYPE_BUY))
		{
			m_bySortType = eAUCTION_SORT_TYPE_RANK_ASC;
			DisableAllButtons();
			DeleteItems(); // delete all current listed items
		}
		else
		{
			GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
		}
	}
}

void CAuctionHouseGui::OnClicked_ItemType7(gui::CComponent * pComponent)
{
	m_byItemType = GetItemTypeByButton(m_pBtnItemType7->GetTextComponent()->GetText().c_str());

	if (m_byItemType != INVALID_BYTE)
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, 1, eAUCTION_SORT_TYPE_RANK_ASC, eAUCTION_TAB_TYPE_BUY))
		{
			m_bySortType = eAUCTION_SORT_TYPE_RANK_ASC;

			DisableAllButtons();
			DeleteItems(); // delete all current listed items
		}
		else
		{
			GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
		}
	}
}

void CAuctionHouseGui::OnClicked_ItemType8(gui::CComponent * pComponent)
{
	m_byItemType = GetItemTypeByButton(m_pBtnItemType8->GetTextComponent()->GetText().c_str());

	if (m_byItemType != INVALID_BYTE)
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, 1, eAUCTION_SORT_TYPE_RANK_ASC, eAUCTION_TAB_TYPE_BUY))
		{
			m_bySortType = eAUCTION_SORT_TYPE_RANK_ASC;

			DisableAllButtons();
			DeleteItems(); // delete all current listed items
		}
		else
		{
			GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
		}
	}
}

void CAuctionHouseGui::OnClicked_MinLevel(gui::CComponent * pComponent)
{
	CRectangle rect = m_pbtnMinLevel->GetScreenRect();
	CDboEventGenerator::CalcPopupShow(TRUE, INVALID_SERIAL_ID, PLACE_SUB_AUCTIONHOUSE_MIN_LEVEL, rect.left, rect.top + 10, PLAYER_MAX_LEVEL, true);
}

void CAuctionHouseGui::OnClicked_MaxLevel(gui::CComponent * pComponent)
{
	CRectangle rect = m_pbtnMaxLevel->GetScreenRect();
	CDboEventGenerator::CalcPopupShow(TRUE, INVALID_SERIAL_ID, PLACE_SUB_AUCTIONHOUSE_MAX_LEVEL, rect.left, rect.top + 10, PLAYER_MAX_LEVEL, true);
}

void CAuctionHouseGui::OnClicked_PrevPrev(gui::CComponent * pComponent)
{
	if (m_uiPage <= AH_MAX_PAGES_COUNT)
		return;

	int a = ((float)m_uiPage / 10.f) + 0.9f;
	int showMaxPage = (a * AH_MAX_PAGES_COUNT);

	UINT32 uiPage = (showMaxPage - AH_MAX_PAGES_COUNT) + 1;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, uiPage, m_bySortType, m_byPage))
	{
		m_uiPage = uiPage;
		DisableAllButtons();
		DeleteItems(); // delete all current listed items
	}
	else
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}

void CAuctionHouseGui::OnClicked_Prev(gui::CComponent * pComponent)
{
	if (m_uiPage == 1)
		return;

	UINT32 uiPage = m_uiPage - 1;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, uiPage, m_bySortType, m_byPage))
	{
		m_uiPage = uiPage;
		DisableAllButtons();
		DeleteItems(); // delete all current listed items
	}
	else
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}

void CAuctionHouseGui::OnClicked_NextNext(gui::CComponent * pComponent)
{
	if (m_uiPage == m_uiMaxPage)
		return;

	int a = ((float)m_uiPage / 10.f) + 0.9f;
	UINT32 showMaxPage = (a * AH_MAX_PAGES_COUNT);

	if (m_uiMaxPage > showMaxPage)
	{
		UINT32 uiPage = showMaxPage + 1;

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, uiPage, m_bySortType, m_byPage))
		{
			m_uiPage = uiPage;
			DisableAllButtons();
			DeleteItems(); // delete all current listed items
		}
		else
		{
			GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
		}
	}
}

void CAuctionHouseGui::OnClicked_Next(gui::CComponent * pComponent)
{
	if (m_uiPage == m_uiMaxPage)
		return;

	UINT32 uiPage = m_uiPage + 1;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, uiPage, m_bySortType, m_byPage))
	{
		m_uiPage = uiPage;
		DisableAllButtons();
		DeleteItems(); // delete all current listed items
	}
	else
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}

void CAuctionHouseGui::OnClicked_PageNumber(gui::CComponent * pComponent)
{
	gui::CButton* pButton = (gui::CButton*)pComponent;

	UINT32 nPage = (UINT32)_wtoi(pButton->GetTextComponent()->GetText().c_str());

	if (nPage < 0 || nPage > m_uiMaxPage)
	{
		DBO_WARNING_MESSAGE("Invalid page: " << nPage << " max page: " << m_uiMaxPage);
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, nPage, m_bySortType, m_byPage))
	{
		m_uiPage = nPage;

		DisableAllButtons();
		DeleteItems(); // delete all current listed items
	}
	else
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}

void CAuctionHouseGui::OnClicked_SetItemPrice(gui::CComponent * pComponent)
{
	CRectangle rect = m_pbtnItemPrice->GetScreenRect();
	CDboEventGenerator::CalcPopupShow(TRUE, INVALID_SERIAL_ID, PLACE_SUB_AUCTIONHOUSE_PRICE, rect.left, rect.top + 10, NTL_MAX_USE_ZENI);
}

void CAuctionHouseGui::OnClicked_RegisterItem(gui::CComponent * pComponent)
{
	if (m_SellItemSlot.GetSerial() == INVALID_SERIAL_ID)
		return;

	GetAlarmManager()->FormattedAlarmMessage("DST_MARKET_SELL", FALSE, NULL, m_SellItemSlot.GetCount(), m_pstbSellPrice->GetText().c_str());
}

void CAuctionHouseGui::OnClicked_CancelItem(gui::CComponent * pComponent)
{
	m_nCancelSaleIndex = -1;

	for (int i = 0; i < DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE; i++)
	{
		if (m_pItems[i]->m_pBtnCancel == pComponent)
		{
			m_nCancelSaleIndex = i;
			GetAlarmManager()->FormattedAlarmMessage("DST_MARKET_SELL_CANCEL", FALSE, NULL, m_pItems[i]->m_pstbItemName->GetText().c_str(), m_pItems[i]->m_pstbItemPrice->GetText().c_str());

			break;
		}
	}
}

void CAuctionHouseGui::OnMouseDown_ItemBuy(const CKey& key)
{
	for (int i = 0; i < DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE; i++)
	{
		if (m_pItems[i]->m_pBtnBuy->HasMouseOver())
		{
			if (m_pItems[i]->m_ItemSlot.GetItemTable() == NULL)
			{
				return;
			}

			if (key.m_nID == UD_RIGHT_BUTTON)
			{
				m_nMouseDownItemIndex = -1;

				if (m_nSelectedItemIndex != -1) // check if another item already selected
				{
					m_pItems[m_nSelectedItemIndex]->m_pBtnBuy->SetDown(false); // unselect old item
				}

				m_pItems[i]->m_pBtnBuy->SetDown(true); // select new item
				m_nSelectedItemIndex = i;

				if (m_byPage == eAUCTION_TAB_TYPE_BUY)
				{
					GetAlarmManager()->FormattedAlarmMessage("DST_MARKET_BUY", FALSE, NULL, m_pItems[i]->m_pstbItemName->GetText().c_str(), m_pItems[i]->m_pstbItemPrice->GetText().c_str());
				}
				else
				{
					m_nCancelSaleIndex = i;
					GetAlarmManager()->FormattedAlarmMessage("DST_MARKET_SELL_CANCEL", FALSE, NULL, m_pItems[i]->m_pstbItemName->GetText().c_str(), m_pItems[i]->m_pstbItemPrice->GetText().c_str());
				}
			}
			else
			{
				m_nMouseDownItemIndex = i;
			}

			break;
		}
	}
}

void CAuctionHouseGui::OnMouseUp_ItemBuy(const CKey & key)
{
	if (m_nMouseDownItemIndex == -1)
		return;

	if (key.m_nID == UD_LEFT_BUTTON)
	{
		for (int i = 0; i < DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE; i++)
		{
			if (m_pItems[i]->m_pBtnBuy->HasMouseOver()) // check if we over this item
			{
				if (m_nMouseDownItemIndex == i)
				{
					if (m_pItems[m_nMouseDownItemIndex]->m_ItemSlot.GetItemTable() == NULL)
					{
						m_pItems[m_nMouseDownItemIndex]->m_pBtnBuy->SetDown(false); // unselect
						m_nMouseDownItemIndex = -1;
						return;
					}
					
					if (m_nSelectedItemIndex != -1 && m_nMouseDownItemIndex != m_nSelectedItemIndex) // check if another item already selected
					{
						m_pItems[m_nSelectedItemIndex]->m_pBtnBuy->SetDown(false); // unselect old item
					}
					
					m_pItems[i]->m_pBtnBuy->SetDown(true); // select

					m_nSelectedItemIndex = i;
				}

				break;
			}
		}
	}

	m_nMouseDownItemIndex = -1;
}

void CAuctionHouseGui::OnClicked_Buy(gui::CComponent * pComponent)
{
	if (m_nSelectedItemIndex == -1)
		return;

	GetAlarmManager()->FormattedAlarmMessage("DST_MARKET_BUY", FALSE, NULL, m_pItems[m_nSelectedItemIndex]->m_pstbItemName->GetText().c_str(), m_pItems[m_nSelectedItemIndex]->m_pstbItemPrice->GetText().c_str());
}

void CAuctionHouseGui::OnClicked_OrderItemName(gui::CComponent * pComponent)
{
	InitOrder();

	m_pBtnOrderItemName->SetText(GetDisplayStringManager()->GetString("DST_MARKET_NAME"));

	if (m_bySortType == eAUCTION_SORT_TYPE_NAME_DESC)
	{
		m_bySortType = eAUCTION_SORT_TYPE_NAME_ASC;
		m_pBtnOrderItemName->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_UP_MARK"));
	}
	else if (m_bySortType == eAUCTION_SORT_TYPE_NAME_ASC)
	{
		m_bySortType = eAUCTION_SORT_TYPE_NAME_DESC;
		m_pBtnOrderItemName->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_DOWN_MARK"));
	}
	else
	{
		m_bySortType = eAUCTION_SORT_TYPE_NAME_ASC;
		m_pBtnOrderItemName->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_UP_MARK"));
	}


	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, m_uiPage, m_bySortType, m_byPage))
	{
		DisableAllButtons();
		DeleteItems(); // delete all current listed items
	}
	else
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}

void CAuctionHouseGui::OnClicked_OrderItemRank(gui::CComponent * pComponent)
{
	InitOrder();

	m_pBtnOrderItemRank->SetText(GetDisplayStringManager()->GetString("DST_MARKET_GRADE"));

	if (m_bySortType == eAUCTION_SORT_TYPE_RANK_DESC)
	{
		m_bySortType = eAUCTION_SORT_TYPE_RANK_ASC;
		m_pBtnOrderItemRank->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_UP_MARK"));
	}
	else if (m_bySortType == eAUCTION_SORT_TYPE_RANK_ASC)
	{
		m_bySortType = eAUCTION_SORT_TYPE_RANK_DESC;
		m_pBtnOrderItemRank->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_DOWN_MARK"));
	}
	else
	{
		m_bySortType = eAUCTION_SORT_TYPE_RANK_ASC;
		m_pBtnOrderItemRank->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_UP_MARK"));
	}


	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, m_uiPage, m_bySortType, m_byPage))
	{
		DisableAllButtons();
		DeleteItems(); // delete all current listed items
	}
	else
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}

void CAuctionHouseGui::OnClicked_OrderItemLevel(gui::CComponent * pComponent)
{
	InitOrder();

	m_pBtnOrderItemRequiredLevel->SetText(GetDisplayStringManager()->GetString("DST_MARKET_USELEVLE"));

	if (m_bySortType == eAUCTION_SORT_TYPE_LEVEL_DESC)
	{
		m_bySortType = eAUCTION_SORT_TYPE_LEVEL_ASC;
		m_pBtnOrderItemRequiredLevel->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_UP_MARK"));
	}
	else if (m_bySortType == eAUCTION_SORT_TYPE_LEVEL_ASC)
	{
		m_bySortType = eAUCTION_SORT_TYPE_LEVEL_DESC;
		m_pBtnOrderItemRequiredLevel->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_DOWN_MARK"));
	}
	else
	{
		m_bySortType = eAUCTION_SORT_TYPE_LEVEL_ASC;
		m_pBtnOrderItemRequiredLevel->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_UP_MARK"));
	}


	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, m_uiPage, m_bySortType, m_byPage))
	{
		DisableAllButtons();
		DeleteItems(); // delete all current listed items
	}
	else
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}

void CAuctionHouseGui::OnClicked_OrderItemPrice(gui::CComponent * pComponent)
{
	InitOrder();

	m_pBtnOrderItemPrice->SetText(GetDisplayStringManager()->GetString("DST_MARKET_PRICE"));

	if (m_bySortType == eAUCTION_SORT_TYPE_PRICE_DESC)
	{
		m_bySortType = eAUCTION_SORT_TYPE_PRICE_ASC;
		m_pBtnOrderItemPrice->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_UP_MARK"));
	}
	else if (m_bySortType == eAUCTION_SORT_TYPE_PRICE_ASC)
	{
		m_bySortType = eAUCTION_SORT_TYPE_PRICE_DESC;
		m_pBtnOrderItemPrice->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_DOWN_MARK"));
	}
	else
	{
		m_bySortType = eAUCTION_SORT_TYPE_PRICE_ASC;
		m_pBtnOrderItemPrice->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_UP_MARK"));
	}


	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, m_uiPage, m_bySortType, m_byPage))
	{
		DisableAllButtons();
		DeleteItems(); // delete all current listed items
	}
	else
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}

void CAuctionHouseGui::OnClicked_OrderItemRemainingTime(gui::CComponent * pComponent)
{
	InitOrder();

	m_pBtnOrderItemTimeRemaining->SetText(GetDisplayStringManager()->GetString("DST_MARKET_LAST_TIME"));

	if (m_bySortType == eAUCTION_SORT_TYPE_TIMELEFT_DESC)
	{
		m_bySortType = eAUCTION_SORT_TYPE_TIMELEFT_ASC;
		m_pBtnOrderItemTimeRemaining->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_UP_MARK"));
	}
	else if (m_bySortType == eAUCTION_SORT_TYPE_TIMELEFT_ASC)
	{
		m_bySortType = eAUCTION_SORT_TYPE_TIMELEFT_DESC;
		m_pBtnOrderItemTimeRemaining->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_DOWN_MARK"));
	}
	else
	{
		m_bySortType = eAUCTION_SORT_TYPE_TIMELEFT_ASC;
		m_pBtnOrderItemTimeRemaining->GetTextComponent()->AddText(GetDisplayStringManager()->GetString("DST_MARKET_UP_MARK"));
	}


	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, m_byTabType, m_byItemType, m_byMinLevel, m_byMaxLevel, m_byRank, m_uiPage, m_bySortType, m_byPage))
	{
		DisableAllButtons();
		DeleteItems(); // delete all current listed items
	}
	else
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}

void CAuctionHouseGui::OnItemNameInput_GotFocus()
{
	m_pstbItemNameInput->Show(false);
}

void CAuctionHouseGui::OnItemNameInput_LostFocus()
{
	if(wcslen(m_pItemNameInput->GetText()) == 0)
		m_pstbItemNameInput->Show();
}

void CAuctionHouseGui::OnSelectedTab(RwInt32 nSelectIndex, RwInt32 nPreviousIndex)
{
	switch (nSelectIndex)
	{
		case eAUCTION_TAB_TYPE_NONE:
		case eAUCTION_TAB_TYPE_WEAPON:
		case eAUCTION_TAB_TYPE_SUB_WEAPON:
		case eAUCTION_TAB_TYPE_ARMOR:
		case eAUCTION_TAB_TYPE_ACCESSORY:
		case eAUCTION_TAB_TYPE_SCOUTER:
		case eAUCTION_TAB_TYPE_RESTORE:
		case eAUCTION_TAB_TYPE_RECIPE:
		case eAUCTION_TAB_TYPE_OTHER:
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

			BYTE byTabType = nSelectIndex;

			if (nSelectIndex == eAUCTION_TAB_TYPE_NONE)
			{
				byTabType = eAUCTION_TAB_TYPE_ALL;
			}

			if (GetDboGlobal()->GetGamePacketGenerator()->SendAuctionHouseListReq(pAvatar->GetCharID(), m_pItemNameInput->GetText(), m_byClassType, byTabType, INVALID_BYTE, m_byMinLevel, m_byMaxLevel, m_byRank, 1, eAUCTION_SORT_TYPE_RANK_ASC, eAUCTION_TAB_TYPE_BUY))
			{
				DisableAllButtons();
				DeleteItems(); // delete all current listed items

				m_bUpdateItemTypeButtons = TRUE;

				m_byItemType = INVALID_BYTE;
				m_byTabType = byTabType;
				m_bySortType = eAUCTION_SORT_TYPE_RANK_ASC;

				InitOrder();
			}
			else
			{
				GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
			}
		}
		break;

		default: DBO_ASSERT(FALSE, "Index: " << nSelectIndex << " does not exist."); break;
	}
}

void CAuctionHouseGui::OnSelectedItemRank(INT nSelectIndex)
{
	m_byRank = (BYTE)m_pcbbItemRank->GetSelectedItemData();
}

void CAuctionHouseGui::OnSelectedItemClass(INT nSelectIndex)
{
	m_byClassType = (BYTE)m_pcbbItemClass->GetSelectedItemData();

	if (m_pbtnMinLevel->GetTextComponent()->GetText().length() == 0)
	{
		if (m_byClassType != INVALID_BYTE)
			m_byMinLevel = 1;
		else
			m_byMinLevel = INVALID_BYTE;
	}
}

void CAuctionHouseGui::InitBuyPage()
{
	m_pPageDialog[eAUCTION_TAB_TYPE_BUY] = NTL_NEW gui::CDialog(CRectangle(10, 49, 762, 165), m_pThis, GetNtlGuiManager()->GetSurfaceManager());

	m_srfBackgroundLT[eAUCTION_TAB_TYPE_BUY].SetPositionfromParent(0 + 10, 83 + 49);
	m_srfBackgroundLT[eAUCTION_TAB_TYPE_BUY].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfBackgroundLT"));

	m_srfBackgroundRT[eAUCTION_TAB_TYPE_BUY].SetPositionfromParent(757 + 10, 83 + 49);
	m_srfBackgroundRT[eAUCTION_TAB_TYPE_BUY].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfBackgroundRT"));

	m_srfBackgroundLM[eAUCTION_TAB_TYPE_BUY].SetPositionfromParent(0 + 10, 95 + 49);
	m_srfBackgroundLM[eAUCTION_TAB_TYPE_BUY].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfBackgroundLM"));

	m_srfBackgroundMM[eAUCTION_TAB_TYPE_BUY].SetPositionfromParent(10 + 10, 95 + 49);
	m_srfBackgroundMM[eAUCTION_TAB_TYPE_BUY].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfBackgroundMM"));

	m_srfBackgroundRM[eAUCTION_TAB_TYPE_BUY].SetPositionfromParent(757 + 10, 95 + 49);
	m_srfBackgroundRM[eAUCTION_TAB_TYPE_BUY].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfBackgroundRM"));

	m_srfBackgroundLB[eAUCTION_TAB_TYPE_BUY].SetPositionfromParent(0 + 10, 544 + 49);
	m_srfBackgroundLB[eAUCTION_TAB_TYPE_BUY].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfBackgroundLB"));

	m_srfBackgroundMB[eAUCTION_TAB_TYPE_BUY].SetPositionfromParent(10 + 10, 544 + 49);
	m_srfBackgroundMB[eAUCTION_TAB_TYPE_BUY].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfBackgroundMB"));

	m_srfBackgroundRB[eAUCTION_TAB_TYPE_BUY].SetPositionfromParent(757 + 10, 544 + 49);
	m_srfBackgroundRB[eAUCTION_TAB_TYPE_BUY].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfBackgroundRB"));
	
	CRectangle rect;

	rect.SetRect(10, 60, 762, 86);
	m_ptabType = NTL_NEW gui::CTabButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_ptabType->SetTabSurface(
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfCategoryLeftUp"),
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfCategoryMidUp"),
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfCategoryRightUp"),
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfCategoryLeftDown"),
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfCategoryMidDown"),
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfCategoryRightDown"),
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfCategoryLeftFoc"),
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfCategoryMidFoc"),
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfCategoryRightFoc"),
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfCategoryLine")
	);

	m_ptabType->SetUnselTabWidth(82);

	m_ptabType->AddTab(std::wstring(GetDisplayStringManager()->GetString("DST_MARKET_ALLCATEGORY")));
	m_ptabType->AddTab(std::wstring(GetDisplayStringManager()->GetString("DST_MARKET_WEAPON")));
	m_ptabType->AddTab(std::wstring(GetDisplayStringManager()->GetString("DST_MARKET_SUBWEAPON")));
	m_ptabType->AddTab(std::wstring(GetDisplayStringManager()->GetString("DST_MARKET_ARMOR")));
	m_ptabType->AddTab(std::wstring(GetDisplayStringManager()->GetString("DST_MARKET_ACCESSORY")));
	m_ptabType->AddTab(std::wstring(GetDisplayStringManager()->GetString("DST_MARKET_SCOUTER")));
	m_ptabType->AddTab(std::wstring(GetDisplayStringManager()->GetString("DST_MARKET_RESORATIVE")));
	m_ptabType->AddTab(std::wstring(GetDisplayStringManager()->GetString("DST_MARKET_RECIPE")));
	m_ptabType->AddTab(std::wstring(GetDisplayStringManager()->GetString("DST_MARKET_ETC")));

	m_slotSelectTab = m_ptabType->SigSelectChanged().Connect(this, &CAuctionHouseGui::OnSelectedTab);
	m_ptabType->SelectTab(0);

	rect.SetRect(47, 89, 127, 115);
	m_pBtnItemType1 = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnItemType1->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidUp"));
	m_pBtnItemType1->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidFoc"));
	m_pBtnItemType1->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDwon"));
	m_pBtnItemType1->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDis"));
	m_slotClickedBtnGloveItemType1 = m_pBtnItemType1->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_ItemType1);

	rect.SetRect(131, 89, 211, 115);
	m_pBtnItemType2 = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnItemType2->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidUp"));
	m_pBtnItemType2->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidFoc"));
	m_pBtnItemType2->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDwon"));
	m_pBtnItemType2->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDis"));
	m_slotClickedBtnGloveItemType2 = m_pBtnItemType2->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_ItemType2);

	rect.SetRect(215, 89, 295, 115);
	m_pBtnItemType3 = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnItemType3->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidUp"));
	m_pBtnItemType3->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidFoc"));
	m_pBtnItemType3->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDwon"));
	m_pBtnItemType3->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDis"));
	m_slotClickedBtnGloveItemType3 = m_pBtnItemType3->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_ItemType3);

	rect.SetRect(299, 89, 379, 115);
	m_pBtnItemType4 = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnItemType4->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidUp"));
	m_pBtnItemType4->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidFoc"));
	m_pBtnItemType4->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDwon"));
	m_pBtnItemType4->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDis"));
	m_slotClickedBtnGloveItemType4 = m_pBtnItemType4->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_ItemType4);

	rect.SetRect(383, 89, 463, 115);
	m_pBtnItemType5 = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnItemType5->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidUp"));
	m_pBtnItemType5->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidFoc"));
	m_pBtnItemType5->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDwon"));
	m_pBtnItemType5->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDis"));
	m_slotClickedBtnGloveItemType5 = m_pBtnItemType5->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_ItemType5);

	rect.SetRect(467, 89, 547, 115);
	m_pBtnItemType6 = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnItemType6->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidUp"));
	m_pBtnItemType6->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidFoc"));
	m_pBtnItemType6->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDwon"));
	m_pBtnItemType6->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDis"));
	m_slotClickedBtnGloveItemType6 = m_pBtnItemType6->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_ItemType6);

	rect.SetRect(551, 89, 631, 115);
	m_pBtnItemType7 = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnItemType7->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidUp"));
	m_pBtnItemType7->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidFoc"));
	m_pBtnItemType7->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDwon"));
	m_pBtnItemType7->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDis"));
	m_slotClickedBtnGloveItemType7 = m_pBtnItemType7->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_ItemType7);

	rect.SetRect(636, 89, 716, 115);
	m_pBtnItemType8 = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnItemType8->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidUp"));
	m_pBtnItemType8->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidFoc"));
	m_pBtnItemType8->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDwon"));
	m_pBtnItemType8->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDis"));
	m_slotClickedBtnGloveItemType8 = m_pBtnItemType8->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_ItemType8);

	// search item name surface
	m_srfItemNameInput.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticBigName"));
	m_srfItemNameInput.SetPositionfromParent(41, 3 + 49);

	//search item name title
	rect.SetRect(48, 4, 263, 26);
	m_pstbItemNameInput = NTL_NEW gui::CStaticBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pstbItemNameInput->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbItemNameInput->SetText(GetDisplayStringManager()->GetString("DST_MARKET_INPUT_NAME"));
	m_pstbItemNameInput->Enable(false);

	// search item name input
	rect.SetRect(48, 7, 263, 29);
	m_pItemNameInput = NTL_NEW gui::CInputBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pItemNameInput->SetMaxLength(DBO_MAX_LENGTH_ITEM_NAME_TEXT);
	m_pItemNameInput->SetHeight(20);
	m_pItemNameInput->SetWidth(200);
	m_pItemNameInput->Enable(true);
	m_pItemNameInput->SetReadOnly(FALSE);
	m_pItemNameInput->SetText(L"");
	m_slotItemNameInputGotFocus = m_pItemNameInput->SigGotFocus().Connect(this, &CAuctionHouseGui::OnItemNameInput_GotFocus);
	m_slotItemNameInputLostFocus = m_pItemNameInput->SigLostFocus().Connect(this, &CAuctionHouseGui::OnItemNameInput_LostFocus);


	// min level search title
	rect.SetRect(288, 31, 358, 54);
	m_pstbMinLevel = NTL_NEW gui::CStaticBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pstbMinLevel->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbMinLevel->SetText(GetDisplayStringManager()->GetString("DST_MARKET_MINLEVEL"));
	m_pstbMinLevel->Enable(false);

	rect.SetRect(356, 31, 394, 53);
	m_pbtnMinLevel = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pbtnMinLevel->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticShot"));
	m_pbtnMinLevel->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticShot"));
	m_pbtnMinLevel->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticShot"));
	m_pbtnMinLevel->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticShot"));
	m_pbtnMinLevel->SetText(L"");
	m_slotClickedBtnMinLevel = m_pbtnMinLevel->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_MinLevel);


	// max level search title
	rect.SetRect(415, 31, 485, 54);
	m_pstbMaxLevel = NTL_NEW gui::CStaticBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pstbMaxLevel->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbMaxLevel->SetText(GetDisplayStringManager()->GetString("DST_MARKET_MAXLEVEL"));
	m_pstbMaxLevel->Enable(false);

	rect.SetRect(481, 31, 519, 53);
	m_pbtnMaxLevel = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pbtnMaxLevel->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticShot"));
	m_pbtnMaxLevel->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticShot"));
	m_pbtnMaxLevel->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticShot"));
	m_pbtnMaxLevel->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticShot"));
	m_pbtnMaxLevel->SetText(L"");
	m_slotClickedBtnMaxLevel = m_pbtnMaxLevel->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_MaxLevel);


	// search button
	rect.SetRect(574, 17, 654, 43);
	m_pbtnSearch = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pbtnSearch->SetText(GetDisplayStringManager()->GetString("DST_MARKET_SERACH_ITEM"));
	m_pbtnSearch->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidUp"));
	m_pbtnSearch->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidFoc"));
	m_pbtnSearch->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDwon"));
	m_pbtnSearch->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDwon"));
	m_slotClickedBtnSearch = m_pbtnSearch->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_Search);


	// refresh button
	rect.SetRect(667, 17, 747, 43);
	m_pbtnBuyRefresh = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pbtnBuyRefresh->SetText(GetDisplayStringManager()->GetString("DST_MARKET_SERACH_REFRESH"));
	m_pbtnBuyRefresh->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidUp"));
	m_pbtnBuyRefresh->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidFoc"));
	m_pbtnBuyRefresh->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDwon"));
	m_pbtnBuyRefresh->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorMidDwon"));
	m_slotClickedBtnBuyRefresh = m_pbtnBuyRefresh->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_BuyRefresh);


	// arrow left - item type
	rect.SetRect(9, 91, 42, 112);
	m_pbtnArrowLeftItemType = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pbtnArrowLeftItemType->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowLeftUp"));
	m_pbtnArrowLeftItemType->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowLeftFoc"));
	m_pbtnArrowLeftItemType->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowLeftDown"));
	m_pbtnArrowLeftItemType->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowLeftDisable"));
	m_slotClickedBtnArrowLeftItemType = m_pbtnArrowLeftItemType->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_LeftArrowItemType);

	// arrow right - item type
	rect.SetRect(720, 91, 753, 112);
	m_pbtnArrowRightItemType = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager());
	m_pbtnArrowRightItemType->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowRightUp"));
	m_pbtnArrowRightItemType->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowRightFoc"));
	m_pbtnArrowRightItemType->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowRightDown"));
	m_pbtnArrowRightItemType->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfArrowRightDisable"));
	m_slotClickedBtnArrowRightItemType = m_pbtnArrowRightItemType->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_RightArrowItemType);


	// item rank combo box
	rect.SetRect(422, 3, 519, 26);
	m_pcbbItemRank = NTL_NEW gui::CComboBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager(), 0, -1, 20, 20, 116, 6);
	m_pcbbItemRank->SetTextFont(DEFAULT_FONT);
	m_pcbbItemRank->SetListFont(DEFAULT_FONT);
	m_pcbbItemRank->SetTextStyle(COMP_TEXT_CENTER);
	m_pcbbItemRank->SetListTextStyle(COMP_TEXT_CENTER);
	m_pcbbItemRank->SetTextColor(RGB(255, 255, 255));
	m_pcbbItemRank->SetItemHeight(19);
	m_pcbbItemRank->AddSurfaceButtonUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonUp"));
	m_pcbbItemRank->AddSurfaceButtonDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonDown"));
	m_pcbbItemRank->AddSurfaceButtonFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonFocus"));
	m_pcbbItemRank->AddSurfaceButtonDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonDisable"));
	m_pcbbItemRank->AddStaticBoxSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticMidMidStd"));
	m_pcbbItemRank->AddListBoxMaskSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticMidMid"));
	m_pcbbItemRank->AddLIstBoxSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfComboBoxShort"));
	m_pcbbItemRank->AddItem(GetDisplayStringManager()->GetString("DST_MARKET_ALLTYPE"), INVALID_BYTE);
	m_pcbbItemRank->AddItem(GetDisplayStringManager()->GetString("DST_MARKET_NORMALTYPE"), ITEM_RANK_NORMAL);
	m_pcbbItemRank->AddItem(GetDisplayStringManager()->GetString("DST_MARKET_SUPERIORTYPE"), ITEM_RANK_SUPERIOR);
	m_pcbbItemRank->AddItem(GetDisplayStringManager()->GetString("DST_MARKET_EXCELLENTTYPE"), ITEM_RANK_EXCELLENT);
	m_pcbbItemRank->AddItem(GetDisplayStringManager()->GetString("DST_MARKET_RARETYPE"), ITEM_RANK_RARE);
	m_pcbbItemRank->AddItem(GetDisplayStringManager()->GetString("DST_MARKET_LEGENDARYTYPE"), ITEM_RANK_LEGENDARY);
	m_pcbbItemRank->SelectItem(0);
	m_slotSelectedItemRank = m_pcbbItemRank->SigSelected().Connect(this, &CAuctionHouseGui::OnSelectedItemRank);


	// item class combo box
	rect.SetRect(297, 3, 394, 26);
	m_pcbbItemClass = NTL_NEW gui::CComboBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_BUY], GetNtlGuiManager()->GetSurfaceManager(), 0, -1, 20, 20, 439, 19);
	m_pcbbItemClass->SetTextFont(DEFAULT_FONT);
	m_pcbbItemClass->SetListFont(DEFAULT_FONT);
	m_pcbbItemClass->SetTextStyle(COMP_TEXT_CENTER);
	m_pcbbItemClass->SetListTextStyle(COMP_TEXT_CENTER);
	m_pcbbItemClass->SetTextColor(RGB(255, 255, 255));
	m_pcbbItemClass->SetItemHeight(23);
	m_pcbbItemClass->AddSurfaceButtonUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonUp"));
	m_pcbbItemClass->AddSurfaceButtonDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonDown"));
	m_pcbbItemClass->AddSurfaceButtonFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonFocus"));
	m_pcbbItemClass->AddSurfaceButtonDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonDisable"));
	m_pcbbItemClass->AddStaticBoxSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticMidMidStd"));
	m_pcbbItemClass->AddListBoxMaskSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticMidMid"));
	m_pcbbItemClass->AddLIstBoxSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfComboBoxlong"));
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_MARKET_ALLCLASS"), INVALID_BYTE);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_HUMAN_FIGHTER"), PC_CLASS_HUMAN_FIGHTER);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_HUMAN_MYSTIC"), PC_CLASS_HUMAN_MYSTIC);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_NAMEK_FIGHTER"), PC_CLASS_NAMEK_FIGHTER);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_NAMEK_MYSTIC"), PC_CLASS_NAMEK_MYSTIC);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_MIGHTY_MAJIN"), PC_CLASS_MIGHTY_MAJIN);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_WONDER_MAJIN"), PC_CLASS_WONDER_MAJIN);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_STREET_FIGHTER"), PC_CLASS_STREET_FIGHTER);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_SWORD_MASTER"), PC_CLASS_SWORD_MASTER);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_CRANE_ROSHI"), PC_CLASS_CRANE_ROSHI);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_TURTLE_ROSHI"), PC_CLASS_TURTLE_ROSHI);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_DARK_WARRIOR"), PC_CLASS_DARK_WARRIOR);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_SHADOW_KNIGHT"), PC_CLASS_SHADOW_KNIGHT);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_DENDEN_HEALER"), PC_CLASS_DENDEN_HEALER);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_POCO_SUMMONER"), PC_CLASS_POCO_SUMMONER);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_ULTI_MA"), PC_CLASS_ULTI_MA);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_GRAND_MA"), PC_CLASS_GRAND_MA);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_PLAS_MA"), PC_CLASS_PLAS_MA);
	m_pcbbItemClass->AddItem(GetDisplayStringManager()->GetString("DST_KAR_MA"), PC_CLASS_KAR_MA);
	m_pcbbItemClass->SelectItem(0);
	m_slotSelectedItemClass = m_pcbbItemClass->SigSelected().Connect(this, &CAuctionHouseGui::OnSelectedItemClass);

	LoadItemTypeButtons();
}

void CAuctionHouseGui::InitSellPage()
{
	m_pPageDialog[eAUCTION_TAB_TYPE_SELL] = NTL_NEW gui::CDialog(CRectangle(10, 60, 777, 156), m_pThis, GetNtlGuiManager()->GetSurfaceManager());

	m_srfBackgroundLT[eAUCTION_TAB_TYPE_SELL].SetPositionfromParent(1 + 10, 1 + 60);
	m_srfBackgroundLT[eAUCTION_TAB_TYPE_SELL].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfSellBackgroundLT"));

	m_srfBackgroundMT.SetPositionfromParent(10 + 10, 1 + 60);
	m_srfBackgroundMT.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfSellBackgroundMT"));

	m_srfBackgroundRT[eAUCTION_TAB_TYPE_SELL].SetPositionfromParent(755 + 10, 1 + 60);
	m_srfBackgroundRT[eAUCTION_TAB_TYPE_SELL].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfSellBackgroundRT"));

	m_srfBackgroundLM[eAUCTION_TAB_TYPE_SELL].SetPositionfromParent(1 + 10, 13 + 60);
	m_srfBackgroundLM[eAUCTION_TAB_TYPE_SELL].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfSellBackgroundLM"));

	m_srfBackgroundMM[eAUCTION_TAB_TYPE_SELL].SetPositionfromParent(1 + 10, 13 + 60);
	m_srfBackgroundMM[eAUCTION_TAB_TYPE_SELL].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfSellBackgroundMM"));

	m_srfBackgroundRM[eAUCTION_TAB_TYPE_SELL].SetPositionfromParent(755 + 10, 13 + 60);
	m_srfBackgroundRM[eAUCTION_TAB_TYPE_SELL].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfSellBackgroundRM"));

	m_srfBackgroundLB[eAUCTION_TAB_TYPE_SELL].SetPositionfromParent(1 + 10, 83 + 60);
	m_srfBackgroundLB[eAUCTION_TAB_TYPE_SELL].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfSellBackgroundLB"));

	m_srfBackgroundMB[eAUCTION_TAB_TYPE_SELL].SetPositionfromParent(10 + 10, 83 + 60);
	m_srfBackgroundMB[eAUCTION_TAB_TYPE_SELL].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfSellBackgroundMB"));

	m_srfBackgroundRB[eAUCTION_TAB_TYPE_SELL].SetPositionfromParent(755 + 10, 83 + 60);
	m_srfBackgroundRB[eAUCTION_TAB_TYPE_SELL].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfSellBackgroundRB"));

	CRectangle rect;

	// hide-buy button
	rect.SetRect(637, 578, 746, 603);
	m_pBtnBuyNot = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnBuyNot->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "BuyNotButton"));
	m_pBtnBuyNot->Enable(false);

	// item name
	rect.SetRect(90, 8, 178, 32);
	m_pstbItemNameTitle = NTL_NEW gui::CStaticBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager());
	m_pstbItemNameTitle->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbItemNameTitle->SetTextColor(INFOCOLOR_7);
	m_pstbItemNameTitle->SetTextStyle(COMP_TEXT_CENTER);
	m_pstbItemNameTitle->SetText(GetDisplayStringManager()->GetString("DST_MARKET_ITEMNAME"));
	m_pstbItemNameTitle->Enable(false);

	// item rank
	rect.SetRect(228, 8, 289, 32);
	m_pstbRankTitle = NTL_NEW gui::CStaticBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager());
	m_pstbRankTitle->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbRankTitle->SetTextColor(INFOCOLOR_7);
	m_pstbRankTitle->SetTextStyle(COMP_TEXT_CENTER);
	m_pstbRankTitle->SetText(GetDisplayStringManager()->GetString("DST_MARKET_GRADE"));
	m_pstbRankTitle->Enable(false);

	// item required level level
	rect.SetRect(269, 8, 357, 32);
	m_pstbUseLevelTitle = NTL_NEW gui::CStaticBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager());
	m_pstbUseLevelTitle->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbUseLevelTitle->SetTextColor(INFOCOLOR_7);
	m_pstbUseLevelTitle->SetTextStyle(COMP_TEXT_CENTER);
	m_pstbUseLevelTitle->SetText(GetDisplayStringManager()->GetString("DST_MARKET_USELEVLE"));
	m_pstbUseLevelTitle->Enable(false);

	// sell price
	rect.SetRect(350, 8, 438, 32);
	m_pstbItemPriceTitle = NTL_NEW gui::CStaticBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager());
	m_pstbItemPriceTitle->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbItemPriceTitle->SetTextColor(INFOCOLOR_7);
	m_pstbItemPriceTitle->SetTextStyle(COMP_TEXT_CENTER);
	m_pstbItemPriceTitle->SetText(GetDisplayStringManager()->GetString("DST_MARKET_SELL_ITMEPRICE"));
	m_pstbItemPriceTitle->Enable(false);

	// duration inside AH
	rect.SetRect(461, 8, 549, 32);
	m_pstbLastingTimeTitle = NTL_NEW gui::CStaticBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager());
	m_pstbLastingTimeTitle->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbLastingTimeTitle->SetTextColor(INFOCOLOR_7);
	m_pstbLastingTimeTitle->SetTextStyle(COMP_TEXT_CENTER);
	m_pstbLastingTimeTitle->SetText(GetDisplayStringManager()->GetString("DST_MARKET_LAST_TIME"));
	m_pstbLastingTimeTitle->Enable(false);

	// fee
	rect.SetRect(569, 8, 657, 32);
	m_pstbSellPriceTitle = NTL_NEW gui::CStaticBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager());
	m_pstbSellPriceTitle->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbSellPriceTitle->SetTextColor(INFOCOLOR_7);
	m_pstbSellPriceTitle->SetTextStyle(COMP_TEXT_CENTER);
	m_pstbSellPriceTitle->SetText(GetDisplayStringManager()->GetString("DST_MARKET_SELL_PRICE"));
	m_pstbSellPriceTitle->Enable(false);


	m_srfSellline.SetPositionfromParent(53 + 10, 34 + 60);
	m_srfSellline.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfSellline"));

	m_srfItemIcon.SetRectWH(13 + 10, 44 + 60, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
	m_srfItemIcon.SetPositionfromParent(13 + 10, 44 + 60);
	m_srfItemIcon.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfItemIcon"));

	
	rect.SetRect(56, 49, 216, 69);
	m_pstbItemName = NTL_NEW gui::CStaticBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager());
	m_pstbItemName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbItemName->SetTextStyle(COMP_TEXT_CENTER);
	m_pstbItemName->Enable(false);


	rect.SetRect(226, 49, 287, 72);
	m_pstbItemRank = NTL_NEW gui::CStaticBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager());
	m_pstbItemRank->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbItemRank->SetTextStyle(COMP_TEXT_CENTER);
	m_pstbItemRank->Enable(false);


	rect.SetRect(271, 49, 351, 72);
	m_pstbUseLevel = NTL_NEW gui::CStaticBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager());
	m_pstbUseLevel->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbUseLevel->SetTextStyle(COMP_TEXT_CENTER);
	m_pstbUseLevel->Enable(false);


	rect.SetRect(340, 50, 455, 70);
	m_pbtnItemPrice = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager());
	m_pbtnItemPrice->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticZenny"));
	m_pbtnItemPrice->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticZenny"));
	m_pbtnItemPrice->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticZenny"));
	m_pbtnItemPrice->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticZennyDis"));
	m_slotClickedBtnItemPrice = m_pbtnItemPrice->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_SetItemPrice);


	rect.SetRect(468, 49, 565, 71);
	m_cbbLastingTime = NTL_NEW gui::CComboBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager(), 0, -1, 20, 20, 70, 1);
	m_cbbLastingTime->SetTextFont(DEFAULT_FONT);
	m_cbbLastingTime->SetListFont(DEFAULT_FONT);
	m_cbbLastingTime->SetTextStyle(COMP_TEXT_CENTER);
	m_cbbLastingTime->SetListTextStyle(COMP_TEXT_CENTER);
	m_cbbLastingTime->SetTextColor(RGB(255, 255, 255));
	m_cbbLastingTime->SetItemHeight(19);
	m_cbbLastingTime->AddSurfaceButtonUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonUp"));
	m_cbbLastingTime->AddSurfaceButtonDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonDown"));
	m_cbbLastingTime->AddSurfaceButtonFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonFocus"));
	m_cbbLastingTime->AddSurfaceButtonDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfcbbButtonDisable"));
	m_cbbLastingTime->AddStaticBoxSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticMidMidStd"));
	m_cbbLastingTime->AddListBoxMaskSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfStaticMidMid"));
	m_cbbLastingTime->AddLIstBoxSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfComboBoxShortEmpty"));
	m_cbbLastingTime->AddItem(GetDisplayStringManager()->GetString("DST_MARKET_ENDTIME"), eAUCTION_ITEM_SALE_DURATION_ONE_DAY);
	m_cbbLastingTime->SelectItem(0);


	rect.SetRect(569, 49, 649, 72);
	m_pstbSellPrice = NTL_NEW gui::CStaticBox(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager());
	m_pstbSellPrice->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbSellPrice->SetTextStyle(COMP_TEXT_CENTER);
	m_pstbSellPrice->Enable(false);


	rect.SetRect(661, 16, 760, 45);
	m_pBtnRegisterItem = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnRegisterItem->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorBigUp"));
	m_pBtnRegisterItem->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorBigFoc"));
	m_pBtnRegisterItem->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorBigDwon"));
	m_pBtnRegisterItem->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorBigDwon"));
	m_pBtnRegisterItem->SetText(GetDisplayStringManager()->GetString("DST_MARKET_REGISTRATION"));
	m_slotClickedBtnRegisterItem = m_pBtnRegisterItem->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_RegisterItem);


	rect.SetRect(661, 50, 760, 79);
	m_pBtnSellRefresh = NTL_NEW gui::CButton(rect, m_pPageDialog[eAUCTION_TAB_TYPE_SELL], GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnSellRefresh->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorBigUp"));
	m_pBtnSellRefresh->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorBigFoc"));
	m_pBtnSellRefresh->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorBigDwon"));
	m_pBtnSellRefresh->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Market.srf", "srfButtonColorBigDwon"));
	m_pBtnSellRefresh->SetText(GetDisplayStringManager()->GetString("DST_MARKET_REFRESH"));
	m_slotClickedBtnSellRefresh = m_pBtnSellRefresh->SigClicked().Connect(this, &CAuctionHouseGui::OnClicked_SellRefresh);

}

void CAuctionHouseGui::DisableAllButtons()
{
	m_pPageDialog[eAUCTION_TAB_TYPE_BUY]->Enable(false);
	m_pPageDialog[eAUCTION_TAB_TYPE_SELL]->Enable(false);
}

void CAuctionHouseGui::OnAuctionHouseListRes(RwUInt32 uiPage, RwUInt32 uiMaxPage, BYTE byListNum, BYTE byPage)
{
	SetPage(byPage);

	switch (m_byPage)
	{
		case eAUCTION_TAB_TYPE_BUY:
		{
			ClearSellItem(); // CLEAR SELL item in case we switch from sell to buy

			for (int i = 0; i < DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE; i++)
			{
				m_pItems[i]->m_pBtnCancel->Show(false);
				m_pItems[i]->m_pstbItemSeller->Show(true);
			}

			m_pPageDialog[eAUCTION_TAB_TYPE_BUY]->Enable(true);

			m_pBtnItemType1->SetDown(false);
			m_pBtnItemType2->SetDown(false);
			m_pBtnItemType3->SetDown(false);
			m_pBtnItemType4->SetDown(false);
			m_pBtnItemType5->SetDown(false);
			m_pBtnItemType6->SetDown(false);
			m_pBtnItemType7->SetDown(false);
			m_pBtnItemType8->SetDown(false);

			if (m_bUpdateItemTypeButtons)
			{
				m_bUpdateItemTypeButtons = FALSE;

				LoadItemTypeButtons();
			}

			if (m_byItemType != INVALID_BYTE)
			{
				SetItemTypeButtonActive();
			}
		}
		break;

		case eAUCTION_TAB_TYPE_SELL:
		{
			m_pPageDialog[eAUCTION_TAB_TYPE_SELL]->Enable(true);

			for (int i = 0; i < DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE; i++)
			{
				m_pItems[i]->m_pstbItemSeller->Show(false);

				if (m_pItems[i]->m_ItemSlot.GetItemTable() != NULL)
				{
					m_pItems[i]->m_pBtnCancel->Show(true);
				}
				else
				{
					m_pItems[i]->m_pBtnCancel->Show(false);
				}
			}
		}
		break;


		default: DBO_ASSERT(FALSE, "Page: " << m_byPage << " does not exist."); break;
	}

	SetSite(uiPage, uiMaxPage);

	m_byListNum = byListNum;
}

void CAuctionHouseGui::ParseItemData(sTENKAICHIDAISIJYOU_DATA_CLIENT * pData)
{
	sTENKAICHIDAISIJYOU_DATA_CLIENT* pNew = new sTENKAICHIDAISIJYOU_DATA_CLIENT;

	memcpy(pNew, pData, sizeof(sTENKAICHIDAISIJYOU_DATA_CLIENT));

	int nIndex = (int)m_mapItemData.size();
	if (nIndex == DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE)
	{
		DBO_WARNING_MESSAGE("Max Items amount reached");
		return;
	}

	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	m_mapItemData.insert(std::make_pair(nIndex, pNew));

	sITEM* pItem = m_pItems[nIndex];

	int nExtra = 40 * nIndex;

	// set item
	pItem->m_ItemSlot.Create(m_pThis, DIALOG_AUCTION_HOUSE, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
	pItem->m_ItemSlot.SetPosition_fromParent(13 + 2, 200 + nExtra + 2);
	pItem->m_ItemSlot.SetIcon(pData->itemNo, pData->byCount);

	// set item name and rank text color
	UINT32 color = Logic_GetItemRankColor(pData->byRank);
	pItem->m_pstbItemName->SetTextColor(color);
	pItem->m_pstbItemRank->SetTextColor(color);

	sITEM_TBLDAT* pItemTbldat = pItem->m_ItemSlot.GetItemTable();

	// set item name
	if (pNew->byGrade > 0)
		pItem->m_pstbItemName->Format("%ls + %u", pItemTextTable->GetText(pItem->m_ItemSlot.GetItemTable()->Name).c_str(), pNew->byGrade);
	else
		pItem->m_pstbItemName->SetText(pItemTextTable->GetText(pItem->m_ItemSlot.GetItemTable()->Name).c_str());

	// set rank
	pItem->m_pstbItemRank->SetText(Logic_GetItemRankName(pData->byRank));

	// set reuired level
	pItem->m_pstbRequiredLevel->SetText(pData->byItemLevel);

	// set price
	pItem->m_pstbItemPrice->SetText(Logic_FormatZeni(pData->dwPrice));

	// set remaining time
	pItem->m_pstbTimeRemaining->SetText(Logic_GetTimeRemainString(pData->nEndSellTime));

	// set seller name
	pItem->m_pstbItemSeller->SetText(pData->awchSeller);
}

void CAuctionHouseGui::LoadItemTypeButtons()
{
	m_byItemTypeMoveCounter = 0;
	m_mapItemType.clear();

	switch (m_byTabType)
	{
		case eAUCTION_TAB_TYPE_ALL:
		{
			m_pBtnItemType1->SetText(GetDisplayStringManager()->GetString("DST_MARKET_GROVE_KNUCKLE"));
			m_pBtnItemType1->Enable(true);
			m_pBtnItemType2->SetText(GetDisplayStringManager()->GetString("DST_STAFF"));
			m_pBtnItemType2->Enable(true);
			m_pBtnItemType3->SetText(GetDisplayStringManager()->GetString("DST_CLAW"));
			m_pBtnItemType3->Enable(true);
			m_pBtnItemType4->SetText(GetDisplayStringManager()->GetString("DST_AXE"));
			m_pBtnItemType4->Enable(true);
			m_pBtnItemType5->SetText(GetDisplayStringManager()->GetString("DST_SCROLL"));
			m_pBtnItemType5->Enable(true);
			m_pBtnItemType6->SetText(GetDisplayStringManager()->GetString("DST_GEM"));
			m_pBtnItemType6->Enable(true);
			m_pBtnItemType7->SetText(GetDisplayStringManager()->GetString("DST_STICK"));
			m_pBtnItemType7->Enable(true);
			m_pBtnItemType8->SetText(GetDisplayStringManager()->GetString("DST_SWORD"));
			m_pBtnItemType8->Enable(true);

			m_mapItemType[ITEM_TYPE_GLOVE] = GetDisplayStringManager()->GetString("DST_MARKET_GROVE_KNUCKLE");
			m_mapItemType[ITEM_TYPE_STAFF] = GetDisplayStringManager()->GetString("DST_STAFF");
			m_mapItemType[ITEM_TYPE_CLAW] = GetDisplayStringManager()->GetString("DST_CLAW");
			m_mapItemType[ITEM_TYPE_AXE] = GetDisplayStringManager()->GetString("DST_AXE");
			m_mapItemType[ITEM_TYPE_SCROLL] = GetDisplayStringManager()->GetString("DST_SCROLL");
			m_mapItemType[ITEM_TYPE_GEM] = GetDisplayStringManager()->GetString("DST_GEM");
			m_mapItemType[ITEM_TYPE_STICK] = GetDisplayStringManager()->GetString("DST_STICK");
			m_mapItemType[ITEM_TYPE_SWORD] = GetDisplayStringManager()->GetString("DST_SWORD");
			m_mapItemType[ITEM_TYPE_FAN] = GetDisplayStringManager()->GetString("DST_FAN");
			m_mapItemType[ITEM_TYPE_WAND] = GetDisplayStringManager()->GetString("DST_WAND");
			m_mapItemType[ITEM_TYPE_INSTRUMENT] = GetDisplayStringManager()->GetString("DST_INSTRUMENT");
			m_mapItemType[ITEM_TYPE_CLUB] = GetDisplayStringManager()->GetString("DST_CLUB");
			m_mapItemType[ITEM_TYPE_DRUM] = GetDisplayStringManager()->GetString("DST_DRUM");
			m_mapItemType[ITEM_TYPE_MASK] = GetDisplayStringManager()->GetString("DST_MASK");
			m_mapItemType[ITEM_TYPE_JACKET] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_JACKET");
			m_mapItemType[ITEM_TYPE_PANTS] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_PANTS");
			m_mapItemType[ITEM_TYPE_BOOTS] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_BOOTS");
			m_mapItemType[ITEM_TYPE_COSTUME_SET] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_DOGI");
			m_mapItemType[ITEM_TYPE_DOGI] = GetDisplayStringManager()->GetString("DST_MARKET_DOJO");
			m_mapItemType[ITEM_TYPE_COSTUME_HAIR_STYLE] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_HAIR");
			m_mapItemType[ITEM_TYPE_COSTUME_MASK] = GetDisplayStringManager()->GetString("DST_MARKET_ACCESSORY1");
			m_mapItemType[ITEM_TYPE_COSTUME_HAIR_ACCESSORY] = GetDisplayStringManager()->GetString("DST_MARKET_ACCESSORY2");
			m_mapItemType[ITEM_TYPE_COSTUME_BACK_ACCESSORY] = GetDisplayStringManager()->GetString("DST_MARKET_ACCESSORY3");
			m_mapItemType[ITEM_TYPE_NECKLACE] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_NECKLACE");
			m_mapItemType[ITEM_TYPE_EARRING] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_EARRING");
			m_mapItemType[ITEM_TYPE_RING] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_RING");
			m_mapItemType[ITEM_TYPE_SCOUTER] = GetDisplayStringManager()->GetString("DST_MARKET_SCOUTER");
			m_mapItemType[ITEM_TYPE_SCOUTER_PART] = GetDisplayStringManager()->GetString("DST_MARKET_SCOUTER_PART");
			m_mapItemType[ITEM_TYPE_RECOVER] = GetDisplayStringManager()->GetString("DST_MARKET_RESORATIVE_ITEM");
			m_mapItemType[ITEM_TYPE_FOOD] = GetDisplayStringManager()->GetString("DST_MARKET_FOOD");
			m_mapItemType[ITEM_TYPE_NEW_FOOD] = GetDisplayStringManager()->GetString("DST_MARKET_NEW_FOOD");
			m_mapItemType[ITEM_TYPE_JUNK] = GetDisplayStringManager()->GetString("DST_MARKET_STUFF");
			m_mapItemType[ITEM_TYPE_RECIPE] = GetDisplayStringManager()->GetString("DST_MARKET_RECIPE_ITEM");
			m_mapItemType[ITEM_TYPE_UPGRADE_STONE_WEAPON] = GetDisplayStringManager()->GetString("DST_MARKET_UPGRADE_STONE_WEAPON");
			m_mapItemType[ITEM_TYPE_UPGRADE_STONE_ARMOR] = GetDisplayStringManager()->GetString("DST_MARKET_UPGRADE_STONE_ARMOR");
			m_mapItemType[ITEM_TYPE_GREATER_UPGRADE_STONE_WEAPON] = GetDisplayStringManager()->GetString("DST_MARKET_GREATER_UPGRADE_STONE_WEAPON");
			m_mapItemType[ITEM_TYPE_GREATER_UPGRADE_STONE_ARMOR] = GetDisplayStringManager()->GetString("DST_MARKET_GREATER_UPGRADE_STONE_ARMOR");
			m_mapItemType[ITEM_TYPE_DOWNGRADE_STONE_WEAPON] = GetDisplayStringManager()->GetString("DST_MARKET_DOWNGRADE_STONE_WEAPON");
			m_mapItemType[ITEM_TYPE_DOWNGRADE_STONE_ARMOR] = GetDisplayStringManager()->GetString("DST_MARKET_DOWNGRADE_STONE_ARMOR");
			m_mapItemType[ITEM_TYPE_BAG] = GetDisplayStringManager()->GetString("DST_MARKET_CAPSULE_KIT");
			m_mapItemType[ITEM_TYPE_MASCOT_BODY] = GetDisplayStringManager()->GetString("DST_MARKET_MASCOT");
			m_mapItemType[ITEM_TYPE_MASCOT_FOOD] = GetDisplayStringManager()->GetString("DST_MARKET_MASCOT_FOOD");
			m_mapItemType[ITEM_TYPE_SKILL_POINT_RESET] = GetDisplayStringManager()->GetString("DST_MARKET_FORGET_BOOK");
			m_mapItemType[ITEM_TYPE_SKILL_POINT_RESET_PLUS] = GetDisplayStringManager()->GetString("DST_MARKET_FORGET_BOOKPLUS");
			m_mapItemType[ITEM_TYPE_COMMERCIAL_BUFF] = GetDisplayStringManager()->GetString("DST_MARKET_EXP_BUFF");
		}
		break;

		case eAUCTION_TAB_TYPE_WEAPON:
		{
			m_pBtnItemType1->SetText(GetDisplayStringManager()->GetString("DST_MARKET_GROVE_KNUCKLE"));
			m_pBtnItemType1->Enable(true);
			m_pBtnItemType2->SetText(GetDisplayStringManager()->GetString("DST_STAFF"));
			m_pBtnItemType2->Enable(true);

			m_pBtnItemType3->SetText(L"");
			m_pBtnItemType4->SetText(L"");
			m_pBtnItemType5->SetText(L"");
			m_pBtnItemType6->SetText(L"");
			m_pBtnItemType7->SetText(L"");
			m_pBtnItemType8->SetText(L"");

			m_mapItemType[ITEM_TYPE_GLOVE] = GetDisplayStringManager()->GetString("DST_MARKET_GROVE_KNUCKLE");
			m_mapItemType[ITEM_TYPE_STAFF] = GetDisplayStringManager()->GetString("DST_STAFF");
		}
		break;
		case eAUCTION_TAB_TYPE_SUB_WEAPON:
		{
			m_pBtnItemType1->SetText(GetDisplayStringManager()->GetString("DST_CLAW"));
			m_pBtnItemType1->Enable(true);
			m_pBtnItemType2->SetText(GetDisplayStringManager()->GetString("DST_AXE"));
			m_pBtnItemType2->Enable(true);
			m_pBtnItemType3->SetText(GetDisplayStringManager()->GetString("DST_SCROLL"));
			m_pBtnItemType3->Enable(true);
			m_pBtnItemType4->SetText(GetDisplayStringManager()->GetString("DST_GEM"));
			m_pBtnItemType4->Enable(true);
			m_pBtnItemType5->SetText(GetDisplayStringManager()->GetString("DST_STICK"));
			m_pBtnItemType5->Enable(true);
			m_pBtnItemType6->SetText(GetDisplayStringManager()->GetString("DST_SWORD"));
			m_pBtnItemType6->Enable(true);
			m_pBtnItemType7->SetText(GetDisplayStringManager()->GetString("DST_FAN"));
			m_pBtnItemType7->Enable(true);
			m_pBtnItemType8->SetText(GetDisplayStringManager()->GetString("DST_WAND"));
			m_pBtnItemType8->Enable(true);

			m_mapItemType[ITEM_TYPE_CLAW] = GetDisplayStringManager()->GetString("DST_CLAW");
			m_mapItemType[ITEM_TYPE_AXE] = GetDisplayStringManager()->GetString("DST_AXE");
			m_mapItemType[ITEM_TYPE_SCROLL] = GetDisplayStringManager()->GetString("DST_SCROLL");
			m_mapItemType[ITEM_TYPE_GEM] = GetDisplayStringManager()->GetString("DST_GEM");
			m_mapItemType[ITEM_TYPE_STICK] = GetDisplayStringManager()->GetString("DST_STICK");
			m_mapItemType[ITEM_TYPE_SWORD] = GetDisplayStringManager()->GetString("DST_SWORD");
			m_mapItemType[ITEM_TYPE_FAN] = GetDisplayStringManager()->GetString("DST_FAN");
			m_mapItemType[ITEM_TYPE_WAND] = GetDisplayStringManager()->GetString("DST_WAND");
			m_mapItemType[ITEM_TYPE_INSTRUMENT] = GetDisplayStringManager()->GetString("DST_INSTRUMENT");
			m_mapItemType[ITEM_TYPE_CLUB] = GetDisplayStringManager()->GetString("DST_CLUB");
			m_mapItemType[ITEM_TYPE_DRUM] = GetDisplayStringManager()->GetString("DST_DRUM");
			m_mapItemType[ITEM_TYPE_MASK] = GetDisplayStringManager()->GetString("DST_MASK");
		}
		break;
		case eAUCTION_TAB_TYPE_ARMOR:
		{
			m_pBtnItemType1->SetText(GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_JACKET"));
			m_pBtnItemType1->Enable(true);
			m_pBtnItemType2->SetText(GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_PANTS"));
			m_pBtnItemType2->Enable(true);
			m_pBtnItemType3->SetText(GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_BOOTS"));
			m_pBtnItemType3->Enable(true);
			m_pBtnItemType4->SetText(GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_DOGI"));
			m_pBtnItemType4->Enable(true);
			m_pBtnItemType5->SetText(GetDisplayStringManager()->GetString("DST_MARKET_DOJO"));
			m_pBtnItemType5->Enable(true);
			m_pBtnItemType6->SetText(GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_HAIR"));
			m_pBtnItemType6->Enable(true);
			m_pBtnItemType7->SetText(GetDisplayStringManager()->GetString("DST_MARKET_ACCESSORY1"));
			m_pBtnItemType7->Enable(true);
			m_pBtnItemType8->SetText(GetDisplayStringManager()->GetString("DST_MARKET_ACCESSORY2"));
			m_pBtnItemType8->Enable(true);

			m_mapItemType[ITEM_TYPE_JACKET] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_JACKET");
			m_mapItemType[ITEM_TYPE_PANTS] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_PANTS");
			m_mapItemType[ITEM_TYPE_BOOTS] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_BOOTS");
			m_mapItemType[ITEM_TYPE_COSTUME_SET] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_DOGI");
			m_mapItemType[ITEM_TYPE_DOGI] = GetDisplayStringManager()->GetString("DST_MARKET_DOJO");
			m_mapItemType[ITEM_TYPE_COSTUME_HAIR_STYLE] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_HAIR");
			m_mapItemType[ITEM_TYPE_COSTUME_MASK] = GetDisplayStringManager()->GetString("DST_MARKET_ACCESSORY1");
			m_mapItemType[ITEM_TYPE_COSTUME_HAIR_ACCESSORY] = GetDisplayStringManager()->GetString("DST_MARKET_ACCESSORY2");
			m_mapItemType[ITEM_TYPE_COSTUME_BACK_ACCESSORY] = GetDisplayStringManager()->GetString("DST_MARKET_ACCESSORY3");
		}
		break;
		case eAUCTION_TAB_TYPE_ACCESSORY:
		{
			m_pBtnItemType1->SetText(GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_NECKLACE"));
			m_pBtnItemType1->Enable(true);
			m_pBtnItemType2->SetText(GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_EARRING"));
			m_pBtnItemType2->Enable(true);
			m_pBtnItemType3->SetText(GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_RING"));
			m_pBtnItemType3->Enable(true);
			m_pBtnItemType4->SetText(L"");
			m_pBtnItemType5->SetText(L"");
			m_pBtnItemType6->SetText(L"");
			m_pBtnItemType7->SetText(L"");
			m_pBtnItemType8->SetText(L"");

			m_mapItemType[ITEM_TYPE_NECKLACE] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_NECKLACE");
			m_mapItemType[ITEM_TYPE_EARRING] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_EARRING");
			m_mapItemType[ITEM_TYPE_RING] = GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_RING");
		}
		break;
		case eAUCTION_TAB_TYPE_SCOUTER:
		{
			m_pBtnItemType1->SetText(GetDisplayStringManager()->GetString("DST_MARKET_SCOUTER"));
			m_pBtnItemType1->Enable(true);
			m_pBtnItemType2->SetText(GetDisplayStringManager()->GetString("DST_MARKET_SCOUTER_PART"));
			m_pBtnItemType2->Enable(true);
			m_pBtnItemType3->SetText(L"");
			m_pBtnItemType4->SetText(L"");
			m_pBtnItemType5->SetText(L"");
			m_pBtnItemType6->SetText(L"");
			m_pBtnItemType7->SetText(L"");
			m_pBtnItemType8->SetText(L"");

			m_mapItemType[ITEM_TYPE_SCOUTER] = GetDisplayStringManager()->GetString("DST_MARKET_SCOUTER");
			m_mapItemType[ITEM_TYPE_SCOUTER_PART] = GetDisplayStringManager()->GetString("DST_MARKET_SCOUTER_PART");
		}
		break;
		case eAUCTION_TAB_TYPE_RESTORE:
		{
			m_pBtnItemType1->SetText(GetDisplayStringManager()->GetString("DST_MARKET_RESORATIVE_ITEM"));
			m_pBtnItemType1->Enable(true);
			m_pBtnItemType2->SetText(GetDisplayStringManager()->GetString("DST_MARKET_FOOD"));
			m_pBtnItemType2->Enable(true);
			m_pBtnItemType3->SetText(GetDisplayStringManager()->GetString("DST_MARKET_NEW_FOOD"));
			m_pBtnItemType3->Enable(true);
			m_pBtnItemType4->SetText(L"");
			m_pBtnItemType5->SetText(L"");
			m_pBtnItemType6->SetText(L"");
			m_pBtnItemType7->SetText(L"");
			m_pBtnItemType8->SetText(L"");

			m_mapItemType[ITEM_TYPE_RECOVER] = GetDisplayStringManager()->GetString("DST_MARKET_RESORATIVE_ITEM");
			m_mapItemType[ITEM_TYPE_FOOD] = GetDisplayStringManager()->GetString("DST_MARKET_FOOD");
			m_mapItemType[ITEM_TYPE_NEW_FOOD] = GetDisplayStringManager()->GetString("DST_MARKET_NEW_FOOD");
		}
		break;
		case eAUCTION_TAB_TYPE_RECIPE:
		{
			m_pBtnItemType1->SetText(GetDisplayStringManager()->GetString("DST_MARKET_STUFF"));
			m_pBtnItemType1->Enable(true);
			m_pBtnItemType2->SetText(GetDisplayStringManager()->GetString("DST_MARKET_RECIPE_ITEM"));
			m_pBtnItemType2->Enable(true);
			m_pBtnItemType3->SetText(L"");
			m_pBtnItemType4->SetText(L"");
			m_pBtnItemType5->SetText(L"");
			m_pBtnItemType6->SetText(L"");
			m_pBtnItemType7->SetText(L"");
			m_pBtnItemType8->SetText(L"");

			m_mapItemType[ITEM_TYPE_JUNK] = GetDisplayStringManager()->GetString("DST_MARKET_STUFF");
			m_mapItemType[ITEM_TYPE_RECIPE] = GetDisplayStringManager()->GetString("DST_MARKET_RECIPE_ITEM");

		}
		break;
		case eAUCTION_TAB_TYPE_OTHER:
		{
			m_pBtnItemType1->SetText(GetDisplayStringManager()->GetString("DST_MARKET_UPGRADE_STONE_WEAPON"));
			m_pBtnItemType1->Enable(true);
			m_pBtnItemType2->SetText(GetDisplayStringManager()->GetString("DST_MARKET_UPGRADE_STONE_ARMOR"));
			m_pBtnItemType2->Enable(true);
			m_pBtnItemType3->SetText(GetDisplayStringManager()->GetString("DST_MARKET_GREATER_UPGRADE_STONE_WEAPON"));
			m_pBtnItemType3->Enable(true);
			m_pBtnItemType4->SetText(GetDisplayStringManager()->GetString("DST_MARKET_GREATER_UPGRADE_STONE_ARMOR"));
			m_pBtnItemType4->Enable(true);
			m_pBtnItemType5->SetText(GetDisplayStringManager()->GetString("DST_MARKET_DOWNGRADE_STONE_WEAPON"));
			m_pBtnItemType5->Enable(true);
			m_pBtnItemType6->SetText(GetDisplayStringManager()->GetString("DST_MARKET_DOWNGRADE_STONE_ARMOR"));
			m_pBtnItemType6->Enable(true);
			m_pBtnItemType7->SetText(GetDisplayStringManager()->GetString("DST_MARKET_CAPSULE_KIT"));
			m_pBtnItemType7->Enable(true);
			m_pBtnItemType8->SetText(GetDisplayStringManager()->GetString("DST_MARKET_MASCOT"));
			m_pBtnItemType8->Enable(true);

			m_mapItemType[ITEM_TYPE_UPGRADE_STONE_WEAPON] = GetDisplayStringManager()->GetString("DST_MARKET_UPGRADE_STONE_WEAPON");
			m_mapItemType[ITEM_TYPE_UPGRADE_STONE_ARMOR] = GetDisplayStringManager()->GetString("DST_MARKET_UPGRADE_STONE_ARMOR");
			m_mapItemType[ITEM_TYPE_GREATER_UPGRADE_STONE_WEAPON] = GetDisplayStringManager()->GetString("DST_MARKET_GREATER_UPGRADE_STONE_WEAPON");
			m_mapItemType[ITEM_TYPE_GREATER_UPGRADE_STONE_ARMOR] = GetDisplayStringManager()->GetString("DST_MARKET_GREATER_UPGRADE_STONE_ARMOR");
			m_mapItemType[ITEM_TYPE_DOWNGRADE_STONE_WEAPON] = GetDisplayStringManager()->GetString("DST_MARKET_DOWNGRADE_STONE_WEAPON");
			m_mapItemType[ITEM_TYPE_DOWNGRADE_STONE_ARMOR] = GetDisplayStringManager()->GetString("DST_MARKET_DOWNGRADE_STONE_ARMOR");
			m_mapItemType[ITEM_TYPE_BAG] = GetDisplayStringManager()->GetString("DST_MARKET_CAPSULE_KIT");
			m_mapItemType[ITEM_TYPE_MASCOT_BODY] = GetDisplayStringManager()->GetString("DST_MARKET_MASCOT");
			m_mapItemType[ITEM_TYPE_MASCOT_FOOD] = GetDisplayStringManager()->GetString("DST_MARKET_MASCOT_FOOD");
			m_mapItemType[ITEM_TYPE_SKILL_POINT_RESET] = GetDisplayStringManager()->GetString("DST_MARKET_FORGET_BOOK");
			m_mapItemType[ITEM_TYPE_SKILL_POINT_RESET_PLUS] = GetDisplayStringManager()->GetString("DST_MARKET_FORGET_BOOKPLUS");
			m_mapItemType[ITEM_TYPE_COMMERCIAL_BUFF] = GetDisplayStringManager()->GetString("DST_MARKET_EXP_BUFF");
		}
		break;

		default: DBO_ASSERT(FALSE, "Tab type: " << m_byTabType << " does not exist."); break;
	}
}

void CAuctionHouseGui::SetItemTypeButtonText()
{
	m_pBtnItemType1->SetDown(false);
	m_pBtnItemType2->SetDown(false);
	m_pBtnItemType3->SetDown(false);
	m_pBtnItemType4->SetDown(false);
	m_pBtnItemType5->SetDown(false);
	m_pBtnItemType6->SetDown(false);
	m_pBtnItemType7->SetDown(false);
	m_pBtnItemType8->SetDown(false);

	ITEMTYPEMAP::iterator it = m_mapItemType.begin();

	for (BYTE i = 0; i < m_byItemTypeMoveCounter; i++)
	{
		++it;
	}

	m_pBtnItemType1->SetText(it->second);
	++it;
	m_pBtnItemType2->SetText(it->second);
	++it;
	m_pBtnItemType3->SetText(it->second);
	++it;
	m_pBtnItemType4->SetText(it->second);
	++it;
	m_pBtnItemType5->SetText(it->second);
	++it;
	m_pBtnItemType6->SetText(it->second);
	++it;
	m_pBtnItemType7->SetText(it->second);
	++it;
	m_pBtnItemType8->SetText(it->second);
}

BYTE CAuctionHouseGui::GetItemTypeByButton(const WCHAR* wchText)
{
	for (ITEMTYPEMAP::iterator it = m_mapItemType.begin(); it != m_mapItemType.end(); it++)
	{
		if (_wcsicmp(wchText, it->second) == 0)
		{
			return (BYTE)it->first;
		}
	}

	return INVALID_BYTE;
}

void CAuctionHouseGui::SetItemTypeButtonActive()
{
	ITEMTYPEMAP::iterator it = m_mapItemType.find(m_byItemType);
	if (it != m_mapItemType.end())
	{
		if (_wcsicmp(m_pBtnItemType1->GetTextComponent()->GetText().c_str(), it->second) == 0)
		{
			m_pBtnItemType1->SetDown(true);
			return;
		}

		if (_wcsicmp(m_pBtnItemType2->GetTextComponent()->GetText().c_str(), it->second) == 0)
		{
			m_pBtnItemType2->SetDown(true);
			return;
		}

		if (_wcsicmp(m_pBtnItemType3->GetTextComponent()->GetText().c_str(), it->second) == 0)
		{
			m_pBtnItemType3->SetDown(true);
			return;
		}

		if (_wcsicmp(m_pBtnItemType4->GetTextComponent()->GetText().c_str(), it->second) == 0)
		{
			m_pBtnItemType4->SetDown(true);
			return;
		}

		if (_wcsicmp(m_pBtnItemType5->GetTextComponent()->GetText().c_str(), it->second) == 0)
		{
			m_pBtnItemType5->SetDown(true);
			return;
		}

		if (_wcsicmp(m_pBtnItemType6->GetTextComponent()->GetText().c_str(), it->second) == 0)
		{
			m_pBtnItemType6->SetDown(true);
			return;
		}

		if (_wcsicmp(m_pBtnItemType7->GetTextComponent()->GetText().c_str(), it->second) == 0)
		{
			m_pBtnItemType7->SetDown(true);
			return;
		}

		if (_wcsicmp(m_pBtnItemType8->GetTextComponent()->GetText().c_str(), it->second) == 0)
		{
			m_pBtnItemType8->SetDown(true);
			return;
		}
	}
}

void CAuctionHouseGui::InitSearch()
{
	m_byClassType = INVALID_BYTE;
	m_byTabType = eAUCTION_TAB_TYPE_ALL;
	m_byItemType = INVALID_BYTE;
	m_byMinLevel = INVALID_BYTE;
	m_byMaxLevel = INVALID_BYTE;
	m_byRank = INVALID_BYTE;
	m_uiPage = 1;
	m_bySortType = eAUCTION_SORT_TYPE_NONE;

	InitOrder();

	// search
	m_pItemNameInput->SetText(L"");
	m_pstbItemNameInput->Show(true);

	m_ptabType->SelectTab(0);

	m_pbtnMinLevel->ClearText();
	m_pbtnMaxLevel->ClearText();

	m_pcbbItemRank->SelectItem(0);
	m_pcbbItemClass->SelectItem(0);
}

void CAuctionHouseGui::InitOrder()
{
	// clear the order
	m_pBtnOrderItemName->SetText(GetDisplayStringManager()->GetString("DST_MARKET_NAME"));
	m_pBtnOrderItemRank->SetText(GetDisplayStringManager()->GetString("DST_MARKET_GRADE"));
	m_pBtnOrderItemRequiredLevel->SetText(GetDisplayStringManager()->GetString("DST_MARKET_USELEVLE"));
	m_pBtnOrderItemPrice->SetText(GetDisplayStringManager()->GetString("DST_MARKET_PRICE"));
	m_pBtnOrderItemTimeRemaining->SetText(GetDisplayStringManager()->GetString("DST_MARKET_LAST_TIME"));
}

void CAuctionHouseGui::SetSite(unsigned int uiSite, unsigned int uiMaxSite)
{
	m_uiPage = uiSite;
	m_uiMaxPage = uiMaxSite;

	if (m_uiPage > m_uiMaxPage)
	{
		m_uiPage = m_uiMaxPage;
	}

	// do the prev/next buttons
	if (m_uiPage == 1) // check if we disable the prev button
		m_pbtnPrev->Enable(false);
	else
		m_pbtnPrev->Enable(true);

	if (m_uiPage <= AH_MAX_PAGES_COUNT) // check if we disable the 10*Prev button
		m_pbtnPrevPrev->Enable(false);
	else
		m_pbtnPrevPrev->Enable(true);

	if (m_uiPage == m_uiMaxPage) // check if we disable next button
		m_pbtnNext->Enable(false);
	else
		m_pbtnNext->Enable(true);

	if (m_uiPage + 10 > m_uiMaxPage)
		m_pbtnNextNext->Enable(false);
	else
		m_pbtnNextNext->Enable(true);

	// hide all site numbers first
	for (int i = 0; i < AH_MAX_PAGES_COUNT; i++)
	{
		m_pBtnPageNum[i]->Show(false);
	}

	// do the site numbers
	int nLimit = 0;

	UINT32 a = ((float)m_uiPage / 10.f) + 0.9f;
	UINT32 showMaxPage = (a * AH_MAX_PAGES_COUNT);

	for (UINT32 i = showMaxPage - AH_MAX_PAGES_COUNT; i < m_uiMaxPage; i++, nLimit++) // because page start at 1 we have to do -1
	{
		if (nLimit >= AH_MAX_PAGES_COUNT)
			break;

		m_pBtnPageNum[nLimit]->SetText(i+1);
		m_pBtnPageNum[nLimit]->Show(true);

		if (i == m_uiPage-1)
		{
			m_pBtnPageNum[nLimit]->SetDown(true);
		}
		else
		{
			m_pBtnPageNum[nLimit]->SetDown(false);
		}
	}
}


void CAuctionHouseGui::DeleteItems()
{
	for (std::map<int, sTENKAICHIDAISIJYOU_DATA_CLIENT*>::iterator it = m_mapItemData.begin(); it != m_mapItemData.end(); it++)
	{
		sTENKAICHIDAISIJYOU_DATA_CLIENT* pData = it->second;

		delete pData;
	}

	m_mapItemData.clear();

	// clear the list
	ClearItemList();

	// reset selected item
	ClearSelectItem();
}

void CAuctionHouseGui::ClearItemList()
{
	for (int i = 0; i < DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE; i++)
	{
		m_pItems[i]->m_ItemSlot.Clear();

		m_pItems[i]->m_pstbItemName->SetText(L"-");
		m_pItems[i]->m_pstbItemName->SetTextColor(RGB(255,255,255));

		m_pItems[i]->m_pstbItemRank->SetText(L"-");
		m_pItems[i]->m_pstbItemRank->SetTextColor(RGB(255, 255, 255));

		m_pItems[i]->m_pstbRequiredLevel->SetText(L"-");
		m_pItems[i]->m_pstbItemPrice->SetText(L"-");
		m_pItems[i]->m_pstbTimeRemaining->SetText(L"-");
		m_pItems[i]->m_pstbItemSeller->SetText(L"-");
	}
}

void CAuctionHouseGui::ClearSellItem()
{
	if (m_SellItemSlot.GetSerial() != INVALID_SERIAL_ID)
	{
		CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_SUB_AUCTIONHOUSE_ITEM, PLACE_BAG,
			m_SellItemSlot.GetSobItem()->GetParentItemSlotIdx(), m_SellItemSlot.GetSobItem()->GetItemSlotIdx());
	}

	m_SellItemSlot.Clear();

	m_pstbItemName->Clear();
	m_pstbItemName->SetTextColor(RGB(255, 255, 255));

	m_pstbItemRank->Clear();
	m_pstbItemRank->SetTextColor(RGB(255, 255, 255));

	m_pstbUseLevel->Clear();
	m_pbtnItemPrice->ClearText();
	m_cbbLastingTime->SelectItem(0);
	m_pstbSellPrice->Clear();
}

void CAuctionHouseGui::ClearSelectItem()
{
	if (m_nSelectedItemIndex == -1)
		return;

	m_pItems[m_nSelectedItemIndex]->m_pBtnBuy->SetDown(false);

	m_nSelectedItemIndex = -1;
	m_nMouseDownItemIndex = -1;
}
