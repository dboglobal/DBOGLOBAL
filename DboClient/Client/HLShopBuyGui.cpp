#include "precomp_dboclient.h"
#include "HLShopBuyGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DisplayStringManager.h"
#include "DboEvent.h"
#include "IconMoveManager.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "InfoWndManager.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"

// table
#include "HLSItemTable.h"



CHLShopBuyGui::CHLShopBuyGui(const RwChar * pName)
	:CNtlPLGui(pName)
{
	Init();
}

CHLShopBuyGui::~CHLShopBuyGui()
{
}

void CHLShopBuyGui::Init()
{
	m_pstbTitle = NULL;
	m_pdlgEachBuy = NULL;
	m_ppnlBlackSlot = NULL;
	m_pstbItemName = NULL;
	m_pstbItemInfo = NULL;
	m_ppnlCashInfoBack = NULL;
	m_pstbRequireCashTitle = NULL;
	m_ppnlRequireCashBack = NULL;
	m_ppnlCashMark0 = NULL;
	m_pstbRequireCash = NULL;
	m_pstbHaveCashTitle = NULL;
	m_ppnlHaveCashBack = NULL;
	m_ppnlCashMark1 = NULL;
	m_pstbHaveCash = NULL;
	m_phtmlGuide = NULL;
	m_pbtnBuy = NULL;
	m_pbtnRecharge = NULL;
	m_pbtnClose = NULL;

	m_hlsItemTblidx = INVALID_TBLIDX;
}

RwBool CHLShopBuyGui::Create()
{
	NTL_FUNCTION("CHLShopBuyGui::Create");

	// Load window
	if (!CNtlPLGui::Create("gui\\HLS.rsr", "gui\\HLSConfirm.srf", "gui\\HLSConfirm.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Get Component
	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_srfBackground.SetType(CWindowby3::WT_HORIZONTAL);
	m_srfBackground.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLSConfirm.srf", "srfConfirmBackTop"));
	m_srfBackground.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLSConfirm.srf", "srfConfirmBackMiddle"));
	m_srfBackground.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLSConfirm.srf", "srfConfirmBackBottom"));
	m_srfBackground.SetSize(334, 395);
	m_srfBackground.SetPositionfromParent(0, 0);

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_TITLE"));

	m_pdlgEachBuy = (gui::CDialog*)GetComponent("dlgEachBuy");

	m_ppnlBlackSlot = (gui::CPanel*)GetComponent("pnlBlackSlot");
	m_surBackSlot.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfBackSlot"));
	m_surBackSlot.SetSize(m_ppnlBlackSlot->GetWidth(), m_ppnlBlackSlot->GetHeight());
	m_surBackSlot.SetPositionbyParent(m_ppnlBlackSlot->GetScreenRect().left, m_ppnlBlackSlot->GetScreenRect().top);

	m_pstbItemName = (gui::CStaticBox*)GetComponent("stbItemName");
	m_pstbItemInfo = (gui::CStaticBox*)GetComponent("stbItemInfo");
	m_ppnlCashInfoBack = (gui::CPanel*)GetComponent("pnlCashInfoBack");

	m_pstbRequireCashTitle = (gui::CStaticBox*)GetComponent("stbRequireCashTitle");
	m_pstbRequireCashTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_NEED_CASH"));

	m_ppnlRequireCashBack = (gui::CPanel*)GetComponent("pnlRequireCashBack");
	m_ppnlCashMark0 = (gui::CPanel*)GetComponent("pnlCashMark0");

	m_pstbRequireCash = (gui::CStaticBox*)GetComponent("stbRequireCash");
	m_pstbRequireCash->SetText(L"0");

	m_pstbHaveCashTitle = (gui::CStaticBox*)GetComponent("stbHaveCashTitle");
	m_pstbHaveCashTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_HAVE_CASH"));

	m_ppnlHaveCashBack = (gui::CPanel*)GetComponent("pnlHaveCashBack");
	m_ppnlCashMark1 = (gui::CPanel*)GetComponent("pnlCashMark1");

	m_pstbHaveCash = (gui::CStaticBox*)GetComponent("stbHaveCash");
	m_pstbHaveCash->SetText(L"0");

	m_phtmlGuide = (gui::CHtmlBox*)GetComponent("htmlGuide");
	const WCHAR* awchhtmlGuide = GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_HTML_GUIDE");
	m_phtmlGuide->SetHtmlFromMemory(awchhtmlGuide, wcslen(awchhtmlGuide));

	m_pbtnBuy = (gui::CButton*)GetComponent("btnBuy");
	m_pbtnBuy->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_BTN_BUY"));
	m_slotClickedBtnBuy = m_pbtnBuy->SigClicked().Connect(this, &CHLShopBuyGui::OnClickedBtnBuy);

	m_pbtnRecharge = (gui::CButton*)GetComponent("btnRecharge");
	m_pbtnRecharge->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_BTN_CASHCHARGE"));
	m_slotClickedBtnRecharge = m_pbtnRecharge->SigClicked().Connect(this, &CHLShopBuyGui::OnClickedBtnRecharge);

	m_pbtnClose = (gui::CButton*)GetComponent("btnClose");
	m_slotClickedBtnClose = m_pbtnClose->SigClicked().Connect(this, &CHLShopBuyGui::OnClickedBtnClose);

	m_slotMove = m_pThis->SigMove().Connect(this, &CHLShopBuyGui::OnMove);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CHLShopBuyGui::OnPaint);
	m_slotMouseEnterItem = m_ppnlBlackSlot->SigMouseEnter().Connect(this, &CHLShopBuyGui::OnMouseEnterItem);
	m_slotMouseLeaveItem = m_ppnlBlackSlot->SigMouseLeave().Connect(this, &CHLShopBuyGui::OnMouseLeaveItem);


	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_HLSHOPBUY);

	LinkMsg(g_EventHLShopEventBuy);

	Show(false);

	NTL_RETURN(TRUE);
}

void CHLShopBuyGui::Destroy()
{
	NTL_FUNCTION("CHLShopBuyGui::Destroy");

	UnLinkMsg(g_EventHLShopEventBuy);

	m_ItemSlot.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CHLShopBuyGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	if (!bOpen)
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HLSHOP_BUY)
			GetInfoWndManager()->ShowInfoWindow(FALSE);

		m_hlsItemTblidx = INVALID_TBLIDX;
		m_ItemSlot.Clear();
	}
	else
	{

	}

	NTL_RETURN(TRUE);
}

void CHLShopBuyGui::HandleEvents(RWS::CMsg & msg)
{
	if (msg.Id == g_EventHLShopEventBuy)
	{
		SDboEventHLShopEventBuy* pData = reinterpret_cast<SDboEventHLShopEventBuy*>(msg.pData);
		if (pData->bConfirm == false)
		{
			m_hlsItemTblidx = pData->hlsItemIdx;

			CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

			// get hls item
			sHLS_ITEM_TBLDAT* pHlsItemTbldat = (sHLS_ITEM_TBLDAT*)API_GetTableContainer()->GetHLSItemTable()->FindData(pData->hlsItemIdx);

			// get item
			sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(pHlsItemTbldat->itemTblidx);

			// set icon
			m_ItemSlot.Create(m_ppnlBlackSlot, DIALOG_HLSHOP_BUY, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
			m_ItemSlot.SetPosition_fromParent(2, 2);
			m_ItemSlot.SetParentPosition(m_ppnlBlackSlot->GetScreenRect().left, m_ppnlBlackSlot->GetScreenRect().top);

			m_ItemSlot.SetIcon(pItemTbldat->tblidx, pHlsItemTbldat->byStackCount);

			// set item name
			m_pstbItemName->SetText(pItemTextTable->GetText(pItemTbldat->Name).c_str());

			// set item info
			m_pstbItemInfo->SetText(pItemTextTable->GetText(pItemTbldat->Note).c_str());

			// set required cash
			m_pstbRequireCash->SetText(Logic_FormatZeni(pHlsItemTbldat->dwCash));
			
			//set have cash
			m_pstbHaveCash->SetText(Logic_FormatZeni(Logic_GetHlsCash()));

			// open the dialog
			GetDialogManager()->OpenDialog(DIALOG_HLSHOP_BUY);
		}
	}
}

void CHLShopBuyGui::OnPaint()
{
	m_srfBackground.Render();
	m_surBackSlot.Render();
	m_ItemSlot.Paint();
}

void CHLShopBuyGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pThis->GetScreenRect();

	m_srfBackground.SetPositionbyParent(rect.left, rect.top);

	CRectangle itemRect = m_ppnlBlackSlot->GetScreenRect();

	m_surBackSlot.SetPositionbyParent(itemRect.left, itemRect.top);
	m_ItemSlot.SetParentPosition(itemRect.left, itemRect.top);
}

void CHLShopBuyGui::OnClickedBtnBuy(gui::CComponent * pComponent)
{
	if (m_hlsItemTblidx != INVALID_TBLIDX)
	{
		CDboEventGenerator::HLShopEventBuy(true, m_hlsItemTblidx);
	}
}

void CHLShopBuyGui::OnClickedBtnRecharge(gui::CComponent * pComponent)
{
	DBO_WARNING_MESSAGE("OnClickedBtnRecharge TO DO");
}

void CHLShopBuyGui::OnClickedBtnClose(gui::CComponent * pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_HLSHOP_BUY);
}

void CHLShopBuyGui::OnMouseEnterItem(gui::CComponent * pComponent)
{
	ShowItemInfoWindow(true);
}

void CHLShopBuyGui::OnMouseLeaveItem(gui::CComponent * pComponent)
{
	ShowItemInfoWindow(false);
}

void CHLShopBuyGui::ShowItemInfoWindow(bool bIsShow)
{
	if (bIsShow && m_ItemSlot.GetItemTable() != NULL)
	{
		CRectangle rect = m_ppnlBlackSlot->GetScreenRect();

		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM, rect.left, rect.top, m_ItemSlot.GetItemTable(), DIALOG_HLSHOP_BUY);
	}
	else
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HLSHOP_BUY)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}
