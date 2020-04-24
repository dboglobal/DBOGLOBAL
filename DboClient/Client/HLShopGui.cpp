#include "precomp_dboclient.h"
#include "HLShopGui.h"

// core
#include "NtlDebug.h"

// presentation
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
#include "HLSItemTable.h"


#define HLS_ITEMS_PER_PAGE 10


CHLShopGui::CHLShopGui(const RwChar * pName)
	:CNtlPLGui(pName),
	m_pStbTitle(NULL),
	m_pBtnClose(NULL),
	m_pipbHLSSearch(NULL),
	m_pbtnSearch(NULL),
	m_pbtnInitSearchList(NULL),
	m_pstbHaveCashTitle(NULL),
	m_pstbHaveCash(NULL),
	m_ppnlHaveCashMark(NULL),
	m_pstbHaveWaguCoinTitle(NULL),
	m_pstbHaveWaguCoin(NULL),
	m_ppnlHaveWaguCoinMark(NULL),
	m_pstbHaveEventCoinTitle(NULL),
	m_pstbHaveEventCoin(NULL),
	m_ppnlHaveEventCoinMark(NULL),
	m_pbtnYadrat(NULL),
	m_pbtnCashRecharge(NULL),
	m_pbtnProductFirstList(NULL),
	m_pbtnProductPrevList(NULL),
	m_pstbPage(NULL),
	m_pbtnProductNextList(NULL),
	m_pbtnProductLastList(NULL)
{
	m_nCurrentCategory = -1;
	m_nCurrentPage = 1;
	m_nMaxPage = 1;

	for (int i = 0; i < eHLS_CATEGORY_NUM; i++)
	{
		m_pBtnCategory[i] = NULL;
		m_vecProducts[i].clear();
	}

	m_bSearch = false;
	m_vecSearch.clear();
}

CHLShopGui::~CHLShopGui()
{
}

RwBool CHLShopGui::Create()
{
	NTL_FUNCTION("CHLShopGui::Create");

	if (!CNtlPLGui::Create("gui\\HLS.rsr", "gui\\HLS.srf", "gui\\HLS.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pStbTitle = (gui::CStaticBox*)GetComponent("stbHLSTitle");
	m_pStbTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_TITLE"));

	m_pipbHLSSearch = (gui::CInputBox*)GetComponent("ipbHLSSearch");
	m_pipbHLSSearch->SetMaxLength(NTL_MAX_LENGTH_ITEM_NAME_TEXT);
	m_pipbHLSSearch->SetText(L"");

	m_pbtnSearch = (gui::CButton*)GetComponent("btnSearch");
	m_pbtnSearch->SetText(GetDisplayStringManager()->GetString("DST_HLS_SEARCH"));
	m_slotClickedBtnSearch = m_pbtnSearch->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnSearch);

	m_pbtnInitSearchList = (gui::CButton*)GetComponent("btnInitSearchList");
	m_pbtnInitSearchList->SetText(GetDisplayStringManager()->GetString("DST_HLS_INIT_SEARCH"));
	m_slotClickedBtnInitSearchList = m_pbtnInitSearchList->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnInitSearchList);

	m_pBtnClose = (gui::CButton*)GetComponent("btnClose");
	m_slotClickedBtnClose = m_pBtnClose->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnClose);

	m_pstbHaveCashTitle = (gui::CStaticBox*)GetComponent("stbHaveCashTitle");
	m_pstbHaveCashTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_HAVE_CASH"));

	m_pstbHaveCash = (gui::CStaticBox*)GetComponent("stbHaveCash");
	m_pstbHaveCash->SetText(0);

	m_ppnlHaveCashMark = (gui::CPanel*)GetComponent("pnlHaveCashMark");

	m_pstbHaveWaguCoinTitle = (gui::CStaticBox*)GetComponent("stbHaveWaguCoinTitle");
	m_pstbHaveWaguCoinTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_HAVE_COIN"));

	m_pstbHaveWaguCoin = (gui::CStaticBox*)GetComponent("stbHaveWaguCoin");
	m_pstbHaveWaguCoin->SetText(0);

	m_ppnlHaveWaguCoinMark = (gui::CPanel*)GetComponent("pnlHaveWaguCoinMark");

	m_pstbHaveEventCoinTitle = (gui::CStaticBox*)GetComponent("stbHaveEventCoinTitle");
	m_pstbHaveEventCoinTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_HAVE_EVENTCOIN"));
	m_pstbHaveEventCoinTitle->Show(false); // hide event coin by default

	m_pstbHaveEventCoin = (gui::CStaticBox*)GetComponent("stbHaveEventCoin");
	m_pstbHaveEventCoin->SetText(0);
	m_pstbHaveEventCoin->Show(false); // hide event coin by default

	m_ppnlHaveEventCoinMark = (gui::CPanel*)GetComponent("pnlHaveEventCoinMark");
	m_ppnlHaveEventCoinMark->Show(false); // hide event coin by default

	m_pbtnYadrat = (gui::CButton*)GetComponent("btnYadrat");
	m_pbtnYadrat->SetText(GetDisplayStringManager()->GetString("DST_HLS_REFRESH"));
	m_slotClickedBtnYadrat = m_pbtnYadrat->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnYadrat);

	m_pbtnCashRecharge = (gui::CButton*)GetComponent("btnCashRecharge");
	m_pbtnCashRecharge->SetText(GetDisplayStringManager()->GetString("DST_HLS_CASHCHARGE"));
	m_slotClickedBtnCashRecharge = m_pbtnCashRecharge->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnCashRecharge);

	m_pbtnProductFirstList = (gui::CButton*)GetComponent("btnProductFirstList");
	m_slotClickedBtnProductFirstList = m_pbtnProductFirstList->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnProductFirstList);

	m_pbtnProductPrevList = (gui::CButton*)GetComponent("btnProductPrevList");
	m_slotClickedBtnProductPrevList = m_pbtnProductPrevList->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnProductPrevList);

	m_pstbPage = (gui::CStaticBox*)GetComponent("stbPage");

	m_pbtnProductNextList = (gui::CButton*)GetComponent("btnProductNextList");
	m_slotClickedBtnProductNextList = m_pbtnProductNextList->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnProductNextList);

	m_pbtnProductLastList = (gui::CButton*)GetComponent("btnProductLastList");
	m_slotClickedBtnProductLastList = m_pbtnProductLastList->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnProductLastList);


	//m_pdlgBanner = (gui::CDialog*)GetComponent("dlgBanner");
	m_slotMove = m_pThis->SigMove().Connect(this, &CHLShopGui::OnMove);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CHLShopGui::OnPaint);

	CreateCategoryButton();
	CreateItems();
	SelectCategory(eHLS_CATEGORY_AVATAR);

	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_HLSHOP);

	LinkMsg(g_EventHLShopEvent);
	LinkMsg(g_EventHLShopEventItemBuyRes);
	LinkMsg(g_EventHLShopEventItemGiftRes);

	Show(false);

	NTL_RETURN(TRUE);
}

void CHLShopGui::Destroy()
{
	NTL_FUNCTION("CHLShopGui::Destroy");


	UnLinkMsg(g_EventHLShopEvent);
	UnLinkMsg(g_EventHLShopEventItemBuyRes);
	UnLinkMsg(g_EventHLShopEventItemGiftRes);

	m_vecVisibleProducts.clear();

	m_bSearch = false;
	m_vecSearch.clear();

	for (int i = 0; i < eHLS_CATEGORY_NUM; i++)
	{
		NTL_DELETE(m_pBtnCategory[i]);

		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecProducts[i].begin(); it != m_vecProducts[i].end(); )
		{
			sHLS_PRODUCTS* pProduct = *it;

			++it;

			pProduct->ItemSlot.Destroy();
			delete pProduct->ppnlItemSlot;
			delete pProduct->pstbItemName;
			delete pProduct->pstbPrice;
			delete pProduct->ppnlCoinMark;
			delete pProduct->pBtnBuy;
			delete pProduct->pBtnGift;
			delete pProduct->pDialog;
			delete pProduct;
		}

		m_vecProducts[i].clear();
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

void CHLShopGui::CreateCategoryButton()
{
	CTextTable* pMerchantTextTable = API_GetTableContainer()->GetTextAllTable()->GetMerchantTbl();

	CRectangle rect;
	RwInt32 iButtonY = 55;

	for (int i = 0; i < eHLS_CATEGORY_NUM; i++)
	{
		//check if category is active
		if (abIsCategoryActice[i] == false)
			continue;

		std::wstring wstrMerchantName = pMerchantTextTable->GetText(i + 1 + 100000); // category names start at ID 100001

		rect.SetRectWH(3, iButtonY, 97, 36);
		m_pBtnCategory[i] = NTL_NEW gui::CButton(rect, wstrMerchantName,
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfTabBtnUp"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfTabBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfTabBtnDisable"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfTabBtnFocus"),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_UP_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, m_pThis, GetNtlGuiManager()->GetSurfaceManager());

		m_slotClickedBtnCategory[i] = m_pBtnCategory[i]->SigClicked().Connect(this, &CHLShopGui::OnClickUpButtonCategory);

		iButtonY += 35;
	}
}

void CHLShopGui::CreateItems()
{
	CTextTable* pMerchantTextTable = API_GetTableContainer()->GetTextAllTable()->GetMerchantTbl();
	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	CRectangle rect;

	int ai[eHLS_CATEGORY_NUM];
	int iDialogX[eHLS_CATEGORY_NUM]; //left/right
	int iDialogY[eHLS_CATEGORY_NUM]; //top/down

	for (int i = 0; i < eHLS_CATEGORY_NUM; i++)
	{
		ai[i] = 0;
		iDialogX[i] = 105;
		iDialogY[i] = 50;
	}

	CHLSItemTable* pHlsItemTable = API_GetTableContainer()->GetHLSItemTable();
	for (std::map<TBLIDX, sTBLDAT *>::iterator it = pHlsItemTable->Begin(); it != pHlsItemTable->End(); it++)
	{
		sHLS_ITEM_TBLDAT* pHlsItem = (sHLS_ITEM_TBLDAT*)it->second;

		// check if on sale
		if (pHlsItem->bOnSale == false)
			continue;

		// check if valid category
		if (pHlsItem->byCategory >= eHLS_CATEGORY_NUM)
			continue;

		//check if category is active
		if (abIsCategoryActice[pHlsItem->byCategory] == false)
			continue;

		// get item
		sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(pHlsItem->itemTblidx);

		// check if item exist
		if (pItemTbldat == NULL)
			continue;

		sHLS_PRODUCTS* pProduct = new sHLS_PRODUCTS;


		pProduct->hlsItemTblidx = pHlsItem->tblidx;

		if ((ai[pHlsItem->byCategory] % 2) == 0)
		{
			iDialogX[pHlsItem->byCategory] = 105;
		}
		else
		{
			iDialogX[pHlsItem->byCategory] = 105 + 233;
		}

		if (ai[pHlsItem->byCategory] == HLS_ITEMS_PER_PAGE)
		{
			ai[pHlsItem->byCategory] = 0;
			iDialogY[pHlsItem->byCategory] = 50;
		}

		rect.SetRectWH(iDialogX[pHlsItem->byCategory], iDialogY[pHlsItem->byCategory], 229, 86);
		pProduct->pDialog = NTL_NEW gui::CPanel(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());

		pProduct->nDialogX = iDialogX[pHlsItem->byCategory];
		pProduct->nDialogY = iDialogY[pHlsItem->byCategory];

		rect.SetRectWH(12, 18, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE);
		pProduct->ppnlItemSlot = NTL_NEW gui::CPanel(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager());
		pProduct->slotMouseEnterItem = pProduct->ppnlItemSlot->SigMouseEnter().Connect(this, &CHLShopGui::OnMouseEnterItem);
		pProduct->slotMouseLeaveItem = pProduct->ppnlItemSlot->SigMouseLeave().Connect(this, &CHLShopGui::OnMouseLeaveItem);

		pProduct->ItemSlot.Create(pProduct->ppnlItemSlot, DIALOG_HLSHOP, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
		pProduct->ItemSlot.SetSize(NTL_ITEM_ICON_SIZE);
		pProduct->ItemSlot.SetPosition_fromParent(0, 0);
		pProduct->ItemSlot.SetParentPosition(pProduct->ppnlItemSlot->GetScreenRect().left, pProduct->ppnlItemSlot->GetScreenRect().top);
		pProduct->ItemSlot.SetIcon(pItemTbldat->tblidx, pHlsItem->byStackCount);

		pProduct->mSurface.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfSlotBack"));

		rect.SetRectWH(50, 15, 165, 14);
		pProduct->pstbItemName = NTL_NEW gui::CStaticBox(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
		pProduct->pstbItemName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		pProduct->pstbItemName->SetText(pItemTextTable->GetText(pItemTbldat->Name).c_str());
		pProduct->pstbItemName->Enable(false);
		pProduct->pstbItemName->SetColor(255, 218, 75); // INFOCOLOR_7

		rect.SetRectWH(168, 36, 30, 14);
		pProduct->pstbPrice = NTL_NEW gui::CStaticBox(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
		pProduct->pstbPrice->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		pProduct->pstbPrice->SetText(Logic_FormatZeni(pHlsItem->dwCash));
		pProduct->pstbPrice->Enable(false);
		pProduct->pstbPrice->SetColor(255, 168, 68); //INFOCOLOR_6

		rect.SetRectWH(203, 35, 16, 16);
		pProduct->ppnlCoinMark = NTL_NEW gui::CPanel(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfCashMark"));
		pProduct->ppnlCoinMark->Enable(false);

		rect.SetRectWH(83, 58, 67, 22);
		pProduct->pBtnBuy = NTL_NEW gui::CButton(rect, GetDisplayStringManager()->GetString("DST_HLS_SLOT_BUY"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnUp"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnFocus"),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_UP_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager());
		pProduct->slotClickedBtnBuy = pProduct->pBtnBuy->SigClicked().Connect(this, &CHLShopGui::OnClickBuyButton);

		rect.SetRectWH(153, 58, 67, 22);
		pProduct->pBtnGift = NTL_NEW gui::CButton(rect, GetDisplayStringManager()->GetString("DST_HLS_SLOT_GIFT"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnUp"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnFocus"),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_UP_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager());
		pProduct->slotClickedBtnGift = pProduct->pBtnGift->SigClicked().Connect(this, &CHLShopGui::OnClickGiftButton);

		pProduct->pDialog->Show(false); // hide by default

		m_vecProducts[pHlsItem->byCategory].push_back(pProduct); // push into "category" list

		if ((ai[pHlsItem->byCategory] % 2) != 0)
		{
			iDialogY[pHlsItem->byCategory] += 90;
		}

		++ai[pHlsItem->byCategory];
	}
}

void CHLShopGui::SelectCategory(int iCategory)
{
	if (m_nCurrentCategory == iCategory)
		return;

	// hide current active button
	if (m_nCurrentCategory != -1) // when we init the variable, its -1
	{
		m_pBtnCategory[m_nCurrentCategory]->SetDown(false);
	}

	// hide current shown items
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		pProduct->pDialog->Show(false);
	}
	m_vecVisibleProducts.clear();

	// show new items
	int i = 0;
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecProducts[iCategory].begin(); it != m_vecProducts[iCategory].end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		pProduct->pDialog->Show(true);

		m_vecVisibleProducts.push_back(pProduct);

		if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
			break;
	}

	m_nCurrentCategory = iCategory;
	m_pBtnCategory[m_nCurrentCategory]->SetDown(true);

	// get max pages
	float fPages = ((float)m_vecProducts[m_nCurrentCategory].size() / (float)HLS_ITEMS_PER_PAGE) + 0.9;
	if (fPages < 1)
		fPages = 1;

	SetPage(1, (int)fPages);
}

void CHLShopGui::SetPage(int nCurPage, int nMaxPage)
{
	m_nCurrentPage = nCurPage;
	m_nMaxPage = nMaxPage;

	m_pstbPage->Format(L"%d / %d", nCurPage, nMaxPage);

	//do onmove to refresh image position
	OnMove(0,0);
}

void CHLShopGui::ClearVisibleItems()
{
	// hide current shown items
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		pProduct->pDialog->Show(false);
	}
	m_vecVisibleProducts.clear();
}

void CHLShopGui::HandleEvents(RWS::CMsg & msg)
{
	NTL_FUNCTION("CHLShopGui::HandleEvents");

	if (msg.Id == g_EventHLShopEvent)
	{
		SDboEventHLShopEvent* pData = reinterpret_cast<SDboEventHLShopEvent*>(msg.pData);

		switch (pData->byEventType)
		{
			case eHLSHOP_EVENT_START:
			{
				Logic_SetHlsCash(pData->dwCash);
				m_pstbHaveCash->SetText(Logic_FormatZeni(pData->dwCash));

				// set storage position
				CRectangle rect = GetPosition();
				CNtlPLGui* pPLGui = GetDialogManager()->GetDialog(DIALOG_HLSHOP_WAREHOUSE);
				pPLGui->SetPosition(rect.left - pPLGui->GetWidth() - NTL_LINKED_DIALOG_GAP, rect.top);

				GetDialogManager()->OpenDialog(DIALOG_HLSHOP);
			}
			break;
			case eHLSHOP_EVENT_REFRESH:
			{
				Logic_SetHlsCash(pData->dwCash);
				m_pstbHaveCash->SetText(Logic_FormatZeni(pData->dwCash));
			}
			break;
			case eHLSHOP_EVENT_END:
			{
				GetDialogManager()->CloseDialog(DIALOG_HLSHOP_BUY);
				GetDialogManager()->CloseDialog(DIALOG_HLSHOP_BUY_CONFIRM);
				GetDialogManager()->CloseDialog(DIALOG_HLSHOP_GIFT);
				GetDialogManager()->CloseDialog(DIALOG_HLSHOP_GIFT_CONFIRM);
				GetDialogManager()->CloseDialog(DIALOG_HLSHOP);
			}
			break;
		}
	}
	else if (msg.Id == g_EventHLShopEventItemBuyRes)
	{
		SDboEventHLShopEventItemBuyRes* pData = reinterpret_cast<SDboEventHLShopEventItemBuyRes*>(msg.pData);

		Logic_SetHlsCash(pData->dwCash);
		m_pstbHaveCash->SetText(Logic_FormatZeni(pData->dwCash));
	}
	else if (msg.Id == g_EventHLShopEventItemGiftRes)
	{
		SDboEventHLShopEventItemGiftRes* pData = reinterpret_cast<SDboEventHLShopEventItemGiftRes*>(msg.pData);

		Logic_SetHlsCash(pData->dwCash);
		m_pstbHaveCash->SetText(Logic_FormatZeni(pData->dwCash));
	}
}

RwInt32 CHLShopGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	if (!bOpen)
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HLSHOP)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
	else
	{
		
	}

	NTL_RETURN(TRUE);
}

void CHLShopGui::OnPaint()
{
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		pProduct->mSurface.Render();
		pProduct->ItemSlot.Paint();
	}
}

void CHLShopGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		CRectangle rect = pProduct->ppnlItemSlot->GetScreenRect();

		pProduct->ItemSlot.SetParentPosition(rect.left, rect.top);

		CRectangle drect = pProduct->pDialog->GetScreenRect();

		pProduct->mSurface.SetPositionbyParent(drect.left, drect.top);
	}

	CRectangle rtScreen = m_pThis->GetScreenRect();

	if(iOldX !=  0 && iOldY != 0)
		MoveLinkedPLGui(rtScreen.left - iOldX, rtScreen.top - iOldY);
}

void CHLShopGui::OnClickedBtnClose(gui::CComponent * pComponent)
{
	GetDboGlobal()->GetGamePacketGenerator()->SendCashItemHLShopEndReq();
}

void CHLShopGui::OnClickedBtnSearch(gui::CComponent * pComponent)
{
	const WCHAR* searchText = m_pipbHLSSearch->GetText();
	if (wcslen(searchText) == 0)
	{
		GetAlarmManager()->AlarmMessage("DST_HLS_SEARCH_GUIDE");
		return;
	}

	//init
	InitSearch();

	m_bSearch = true;

	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	// hide current active button
	if (m_nCurrentCategory != -1) // when we init the variable, its -1
	{
		m_pBtnCategory[m_nCurrentCategory]->SetDown(false);
	}

	// hide current shown items
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		pProduct->pDialog->Show(false);
	}
	m_vecVisibleProducts.clear();


	// show new items
	int iDialogX = 105;
	int iDialogY = 50;

	int showCount = 0, totalCount = 0;

	for (int i = 0; i < eHLS_CATEGORY_NUM; i++)
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecProducts[i].begin(); it != m_vecProducts[i].end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			std::wstring wstrItemName = pItemTextTable->GetText(pProduct->ItemSlot.GetItemTable()->Name);

			if(wstrItemName.find(searchText) != string::npos) // search
			{
				if ((totalCount % 2) == 0)
				{
					iDialogX = 105;
				}
				else
				{
					iDialogX = 105 + 233;
				}

				if (totalCount == HLS_ITEMS_PER_PAGE)
				{
					totalCount = 0;
					iDialogY = 50;
				}

				CRectangle rect;
				rect.SetRectWH(iDialogX, iDialogY, 229, 86);

				pProduct->pDialog->SetPosition(rect);

				if (showCount++ < HLS_ITEMS_PER_PAGE) // only show first 10 items
				{
					pProduct->pDialog->Show(true);
					m_vecVisibleProducts.push_back(pProduct);
				}

				m_vecSearch.push_back(pProduct);

				if ((totalCount % 2) != 0)
				{
					iDialogY += 90;
				}

				++totalCount;
			}
		}
	}

	// get max pages
	float fPages = ((float)m_vecSearch.size() / (float)HLS_ITEMS_PER_PAGE) + 0.9;
	if (fPages < 1)
		fPages = 1;

	SetPage(1, (int)fPages);
}

void CHLShopGui::OnClickedBtnInitSearchList(gui::CComponent * pComponent)
{
	if (m_bSearch)
	{
		m_pipbHLSSearch->SetText(L"");
		InitSearch();
	}

	int curCat = m_nCurrentCategory;
	m_nCurrentCategory = -1;

	SelectCategory(curCat);
}

void CHLShopGui::OnClickedBtnYadrat(gui::CComponent * pComponent)
{
	GetDboGlobal()->GetGamePacketGenerator()->SendCashItemHLShopRefreshReq();
}

void CHLShopGui::OnClickedBtnCashRecharge(gui::CComponent * pComponent)
{
}

void CHLShopGui::OnClickedBtnProductFirstList(gui::CComponent * pComponent)
{
	if (m_nCurrentPage == 1)
		return;

	// hide current shown items
	ClearVisibleItems();

	// show new items
	int i = 0;

	if (m_bSearch)
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecSearch.begin(); it != m_vecSearch.end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}
	else
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecProducts[m_nCurrentCategory].begin(); it != m_vecProducts[m_nCurrentCategory].end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}

	SetPage(1, m_nMaxPage);
}

void CHLShopGui::OnClickedBtnProductPrevList(gui::CComponent * pComponent)
{
	if (m_nCurrentPage == 1)
		return;

	// hide current shown items
	ClearVisibleItems();

	int nToSkip = (m_nCurrentPage - 2) * HLS_ITEMS_PER_PAGE;
	if (nToSkip < HLS_ITEMS_PER_PAGE)
		nToSkip = 0;

	int i = 0;
	int nSkip = 0;

	if (m_bSearch)
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecSearch.begin(); it != m_vecSearch.end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			if (nSkip++ < nToSkip)
				continue;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}
	else
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecProducts[m_nCurrentCategory].begin(); it != m_vecProducts[m_nCurrentCategory].end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			if (nSkip++ < nToSkip)
				continue;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}


	SetPage(m_nCurrentPage - 1, m_nMaxPage);
}

void CHLShopGui::OnClickedBtnProductNextList(gui::CComponent * pComponent)
{
	if (m_nCurrentPage == m_nMaxPage)
		return;

	// hide current shown items
	ClearVisibleItems();

	int nToSkip = m_nCurrentPage * HLS_ITEMS_PER_PAGE;

	int i = 0;
	int nSkip = 0;

	if (m_bSearch)
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecSearch.begin(); it != m_vecSearch.end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			if (nSkip++ < nToSkip)
				continue;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}
	else
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecProducts[m_nCurrentCategory].begin(); it != m_vecProducts[m_nCurrentCategory].end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			if (nSkip++ < nToSkip)
				continue;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}

	SetPage(m_nCurrentPage + 1, m_nMaxPage);
}

void CHLShopGui::OnClickedBtnProductLastList(gui::CComponent * pComponent)
{
	if (m_nCurrentPage == m_nMaxPage)
		return;

	int nToSkip = (m_nMaxPage - 1) * HLS_ITEMS_PER_PAGE;
	if (nToSkip < 0)
		nToSkip = 0;

	// hide current shown items
	ClearVisibleItems();

	int i = 0;
	int nSkip = 0;

	if (m_bSearch)
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecSearch.begin(); it != m_vecSearch.end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			if (nSkip++ < nToSkip)
				continue;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}
	else
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecProducts[m_nCurrentCategory].begin(); it != m_vecProducts[m_nCurrentCategory].end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			if (nSkip++ < nToSkip)
				continue;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}

	SetPage(m_nMaxPage, m_nMaxPage);
}

void CHLShopGui::OnClickUpButtonCategory(gui::CComponent * pComponent)
{
	if (m_bSearch)
	{
		InitSearch();
		m_nCurrentCategory = -1;
	}

	for (int i = 0; i < eHLS_CATEGORY_NUM; i++)
	{
		if (m_pBtnCategory[i] == pComponent)
		{
			SelectCategory(i);
			break;
		}
	}
}

void CHLShopGui::OnClickBuyButton(gui::CComponent * pComponent)
{
	// buy window check
	if (GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_BUY) || GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_BUY_CONFIRM))
	{
		GetAlarmManager()->AlarmMessage("DST_HLS_ALREADY_CONFIRM");
		return;
	}
	// gift window check
	if (GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_GIFT) || GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_GIFT_CONFIRM))
	{
		GetAlarmManager()->AlarmMessage("DST_HLS_ALREADY_GIFT");
		return;
	}

	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		if (pProduct->pBtnBuy == pComponent)
		{
			CDboEventGenerator::HLShopEventBuy(false, pProduct->hlsItemTblidx);

			break;
		}
	}
}

void CHLShopGui::OnClickGiftButton(gui::CComponent * pComponent)
{
	// buy window check
	if (GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_BUY) || GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_BUY_CONFIRM))
	{
		GetAlarmManager()->AlarmMessage("DST_HLS_ALREADY_CONFIRM");
		return;
	}
	// gift window check
	if (GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_GIFT) || GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_GIFT_CONFIRM))
	{
		GetAlarmManager()->AlarmMessage("DST_HLS_ALREADY_GIFT");
		return;
	}

	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		if (pProduct->pBtnGift == pComponent)
		{
			CDboEventGenerator::HLShopEventGift(false, pProduct->hlsItemTblidx, L"");

			break;
		}
	}
}

void CHLShopGui::OnMouseEnterItem(gui::CComponent * pComponent)
{
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		if (pProduct->ppnlItemSlot == pComponent)
		{
			ShowItemInfoWindow(true, pProduct);

			break;
		}
	}
}

void CHLShopGui::OnMouseLeaveItem(gui::CComponent * pComponent)
{
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		if (pProduct->ppnlItemSlot == pComponent)
		{
			ShowItemInfoWindow(false, pProduct);

			break;
		}
	}
}

void CHLShopGui::ShowItemInfoWindow(bool bIsShow, sHLS_PRODUCTS* pProduct)
{
	if (bIsShow && pProduct->ItemSlot.GetItemTable() != NULL)
	{
		CRectangle rect = pProduct->ppnlItemSlot->GetScreenRect();

		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM, rect.left, rect.top, pProduct->ItemSlot.GetItemTable(), DIALOG_HLSHOP);
	}
	else
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HLSHOP)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

void CHLShopGui::InitSearch()
{
	m_bSearch = false;

	// reset location
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecSearch.begin(); it != m_vecSearch.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		CRectangle rect;
		rect.SetRectWH(pProduct->nDialogX, pProduct->nDialogY, 229, 86);

		pProduct->pDialog->SetPosition(rect);
	}

	m_vecSearch.clear();
}
