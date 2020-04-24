#include "precomp_dboclient.h"
#include "HLShopGiftGui.h"

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
#include "AlarmManager.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"

// table
#include "HLSItemTable.h"



CHLShopGiftGui::CHLShopGiftGui(const RwChar * pName)
	:CNtlPLGui(pName)
{
	Init();
}

CHLShopGiftGui::~CHLShopGiftGui()
{
}

void CHLShopGiftGui::Init()
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

RwBool CHLShopGiftGui::Create()
{
	NTL_FUNCTION("CHLShopGiftGui::Create");

	// Load window
	if (!CNtlPLGui::Create("gui\\HLS.rsr", "gui\\HLSConfirm.srf", "gui\\HLSGiftConfirm.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Get Component
	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_srfBackground.SetType(CWindowby3::WT_HORIZONTAL);
	m_srfBackground.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLSConfirm.srf", "srfConfirmBackTop"));
	m_srfBackground.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLSConfirm.srf", "srfConfirmBackMiddle"));
	m_srfBackground.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLSConfirm.srf", "srfConfirmBackBottom"));
	m_srfBackground.SetSize(334, 476);
	m_srfBackground.SetPositionfromParent(0, 0);

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_GIFT_TITLE"));

	m_pdlgEachBuy = (gui::CDialog*)GetComponent("dlgEachBuy");

	m_ppnlBlackSlot = (gui::CPanel*)GetComponent("pnlBlackSlot");
	m_surBackSlot.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfBackSlot"));
	m_surBackSlot.SetSize(m_ppnlBlackSlot->GetWidth(), m_ppnlBlackSlot->GetHeight());
	m_surBackSlot.SetPositionbyParent(m_ppnlBlackSlot->GetScreenRect().left, m_ppnlBlackSlot->GetScreenRect().top);

	m_pstbItemName = (gui::CStaticBox*)GetComponent("stbItemName");
	m_pstbItemInfo = (gui::CStaticBox*)GetComponent("stbItemInfo");

	m_ppnlGiftInfoBack = (gui::CPanel*)GetComponent("pnlGiftInfoBack");

	m_pstbSelectPlayer = (gui::CStaticBox*)GetComponent("stbSelectPlayer");
	m_pstbSelectPlayer->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_GIFT_WHO"));

	m_pstbCharacter = (gui::CStaticBox*)GetComponent("stbCharacter");
	m_pstbCharacter->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_GIFT_CHARACTER"));

	m_ppnlCashInfoBack = (gui::CPanel*)GetComponent("pnlCashInfoBack");

	m_pstbRequireCashTitle = (gui::CStaticBox*)GetComponent("stbNeedCashTitle");
	m_pstbRequireCashTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_NEED_CASH"));

	m_ppnlRequireCashBack = (gui::CPanel*)GetComponent("pnlNeedCashback");

	m_pstbRequireCash = (gui::CStaticBox*)GetComponent("stbNeedCash");
	m_pstbRequireCash->SetText(L"0");

	m_ppnlCashMark0 = (gui::CPanel*)GetComponent("pnlCashMark0");
	m_ppnlHaveCashBack = (gui::CPanel*)GetComponent("pnlHaveCashBack");
	m_pstbHaveCashTitle = (gui::CStaticBox*)GetComponent("stbHaveCashTitle");
	m_pstbHaveCashTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_HAVE_CASH"));

	m_pstbHaveCash = (gui::CStaticBox*)GetComponent("stbHaveCash");
	m_pstbHaveCash->SetText(L"0");

	m_ppnlCashMark1 = (gui::CPanel*)GetComponent("pnlCashMark1");

	m_phtmlGuide = (gui::CHtmlBox*)GetComponent("htmlGuide");
	const WCHAR* awchhtmlGuide = GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_HTML_GUIDE");
	m_phtmlGuide->SetHtmlFromMemory(awchhtmlGuide, wcslen(awchhtmlGuide));

	m_pbtnBuy = (gui::CButton*)GetComponent("btnBuy");
	m_pbtnBuy->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_BTN_BUY"));
	m_slotClickedBtnBuy = m_pbtnBuy->SigClicked().Connect(this, &CHLShopGiftGui::OnClickedBtnBuy);

	m_pbtnRecharge = (gui::CButton*)GetComponent("btnRecharge");
	m_pbtnRecharge->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_BTN_CASHCHARGE"));
	m_slotClickedBtnRecharge = m_pbtnRecharge->SigClicked().Connect(this, &CHLShopGiftGui::OnClickedBtnRecharge);

	m_pbtnClose = (gui::CButton*)GetComponent("btnClose");
	m_slotClickedBtnClose = m_pbtnClose->SigClicked().Connect(this, &CHLShopGiftGui::OnClickedBtnClose);

	m_pcbbHLSFriendList = (gui::CComboBox*)GetComponent("cbbHLSFriendList");
	m_pcbbHLSFriendList->GetButton()->SetToolTip(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_TOOLTIP_FRIEND_LIST"));
	m_slotSelectedOption = m_pcbbHLSFriendList->SigSelected().Connect(this, &CHLShopGiftGui::OnSelectedFriend);

	m_pIpbTargetName = (gui::CInputBox*)GetComponent("ipbTargetName");

	m_slotMove = m_pThis->SigMove().Connect(this, &CHLShopGiftGui::OnMove);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CHLShopGiftGui::OnPaint);
	m_slotMouseEnterItem = m_ppnlBlackSlot->SigMouseEnter().Connect(this, &CHLShopGiftGui::OnMouseEnterItem);
	m_slotMouseLeaveItem = m_ppnlBlackSlot->SigMouseLeave().Connect(this, &CHLShopGiftGui::OnMouseLeaveItem);


	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_HLSHOPBUY);

	LinkMsg(g_EventHLShopEventGift);
	LinkMsg(g_EventFriendList_Info);
	LinkMsg(g_EventFriendAdd_Res);
	LinkMsg(g_EventFriendDel_Res);
	LinkMsg(g_EventFriendMove_Res);

	Show(false);

	NTL_RETURN(TRUE);
}

void CHLShopGiftGui::Destroy()
{
	NTL_FUNCTION("CHLShopGiftGui::Destroy");

	UnLinkMsg(g_EventHLShopEventGift);
	UnLinkMsg(g_EventFriendList_Info);
	UnLinkMsg(g_EventFriendAdd_Res);
	UnLinkMsg(g_EventFriendDel_Res);
	UnLinkMsg(g_EventFriendMove_Res);

	m_ItemSlot.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CHLShopGiftGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	if (!bOpen)
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HLSHOP_GIFT)
			GetInfoWndManager()->ShowInfoWindow(FALSE);

		m_hlsItemTblidx = INVALID_TBLIDX;
		m_ItemSlot.Clear();
		m_pcbbHLSFriendList->SelectItem(0, false); // unselect friend
		m_pIpbTargetName->Clear();
	}
	else
	{

	}

	NTL_RETURN(TRUE);
}

void CHLShopGiftGui::HandleEvents(RWS::CMsg & msg)
{
	if (msg.Id == g_EventHLShopEventGift)
	{
		SDboEventHLShopEventGift* pData = reinterpret_cast<SDboEventHLShopEventGift*>(msg.pData);

		if (pData->bConfirm == false)
		{
			m_hlsItemTblidx = pData->hlsItemIdx;

			CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

			// get hls item
			sHLS_ITEM_TBLDAT* pHlsItemTbldat = (sHLS_ITEM_TBLDAT*)API_GetTableContainer()->GetHLSItemTable()->FindData(pData->hlsItemIdx);

			// get item
			sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(pHlsItemTbldat->itemTblidx);

			// set icon
			m_ItemSlot.Create(m_ppnlBlackSlot, DIALOG_HLSHOP_GIFT, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
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
			GetDialogManager()->OpenDialog(DIALOG_HLSHOP_GIFT);
		}
	}
	else if (msg.Id == g_EventFriendList_Info)
	{
		SNtlEventFriendListInfo* pData = reinterpret_cast<SNtlEventFriendListInfo*>(msg.pData);

		for (int i = 0; i < pData->byCount; i++)
		{
			if (!pData->asInfo[i].bIsBlack)
			{
				m_pcbbHLSFriendList->AddItem(pData->asInfo[i].wchName, pData->asInfo[i].charID);
			}
		}
	}
	else if (msg.Id == g_EventFriendAdd_Res)
	{
		SNtlEventFriendAddRes* pData = reinterpret_cast<SNtlEventFriendAddRes*>(msg.pData);

		m_pcbbHLSFriendList->AddItem(pData->wchName, pData->uiTargetID);
	}
	else if (msg.Id == g_EventFriendDel_Res)
	{
		SNtlEventFriendDelRes* pData = reinterpret_cast<SNtlEventFriendDelRes*>(msg.pData);

		int nCount = m_pcbbHLSFriendList->GetListBox()->GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			if (m_pcbbHLSFriendList->GetItemData(i) == pData->uiTargetID)
			{
				m_pcbbHLSFriendList->RemoveItem(i);
				break;
			}
		}
	}
	else if (msg.Id == g_EventFriendMove_Res)
	{
		SNtlEventFriendMoveRes* pData = reinterpret_cast<SNtlEventFriendMoveRes*>(msg.pData);

		int nCount = m_pcbbHLSFriendList->GetListBox()->GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			if (m_pcbbHLSFriendList->GetItemData(i) == pData->uiTargetID)
			{
				m_pcbbHLSFriendList->RemoveItem(i);
				break;
			}
		}
	}
}

void CHLShopGiftGui::OnPaint()
{
	m_srfBackground.Render();
	m_surBackSlot.Render();
	m_ItemSlot.Paint();
}

void CHLShopGiftGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pThis->GetScreenRect();

	m_srfBackground.SetPositionbyParent(rect.left, rect.top);

	CRectangle itemRect = m_ppnlBlackSlot->GetScreenRect();

	m_surBackSlot.SetPositionbyParent(itemRect.left, itemRect.top);
	m_ItemSlot.SetParentPosition(itemRect.left, itemRect.top);
}

void CHLShopGiftGui::OnClickedBtnBuy(gui::CComponent * pComponent)
{
	if (m_pIpbTargetName->GetLength() <= 0)
	{
		GetAlarmManager()->AlarmMessage("DST_HLS_GIFT_CHAR_NAME_GUIDE");
		return;
	}

	if (m_hlsItemTblidx != INVALID_TBLIDX)
	{
		CDboEventGenerator::HLShopEventGift(true, m_hlsItemTblidx, m_pIpbTargetName->GetText());
	}
}

void CHLShopGiftGui::OnClickedBtnRecharge(gui::CComponent * pComponent)
{
	DBO_WARNING_MESSAGE("OnClickedBtnRecharge TO DO");
}

void CHLShopGiftGui::OnClickedBtnClose(gui::CComponent * pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_HLSHOP_GIFT);
}

void CHLShopGiftGui::OnMouseEnterItem(gui::CComponent * pComponent)
{
	ShowItemInfoWindow(true);
}

void CHLShopGiftGui::OnMouseLeaveItem(gui::CComponent * pComponent)
{
	ShowItemInfoWindow(false);
}

void CHLShopGiftGui::OnSelectedFriend(INT nSelectIndex)
{
	m_pIpbTargetName->SetText(m_pcbbHLSFriendList->GetSelectedItemText().c_str());
}

void CHLShopGiftGui::ShowItemInfoWindow(bool bIsShow)
{
	if (bIsShow && m_ItemSlot.GetItemTable() != NULL)
	{
		CRectangle rect = m_ppnlBlackSlot->GetScreenRect();

		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM, rect.left, rect.top, m_ItemSlot.GetItemTable(), DIALOG_HLSHOP_GIFT);
	}
	else
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HLSHOP_GIFT)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}
