#include "precomp_dboclient.h"
#include "CCBDBoardGui.h"

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

// simul
#include "NtlCameraController.h"
#include "NtlSobItemAttr.h"



CCCBDBoardGui::CCCBDBoardGui(const RwChar * pName)
	:CNtlPLGui(pName)
{
	Init();
}

CCCBDBoardGui::~CCCBDBoardGui()
{
}

void CCCBDBoardGui::Init()
{
	m_bItemSlotFocus = false;
	m_hNPCSerialID = INVALID_HOBJECT;

	m_sItem.hItem = INVALID_HOBJECT;
	m_sItem.byPlace = INVALID_BYTE;
	m_sItem.byPos = INVALID_BYTE;
}

RwBool CCCBDBoardGui::Create()
{
	NTL_FUNCTION("CCCBDBoardGui::Create");

	// Load window
	if (!CNtlPLGui::Create("gui\\CCBDBoardGui.rsr", "gui\\CCBDBoardGui.srf", "gui\\CCBDBoardGui.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Get Component
	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_psttTitle = (gui::CStaticBox*)GetComponent("sttTitle");
	m_psttTitle->SetText(GetDisplayStringManager()->GetString("DST_CCBD_BOARD_SETUP"));

	m_pstbCCBDSetup = (gui::CStaticBox*)GetComponent("stbCCBDSetup");
	m_pstbCCBDSetup->SetText(GetDisplayStringManager()->GetString("DST_CCBD_BOARD_CHALLEGNE"));

	m_phbxExplain = (gui::CHtmlBox*)GetComponent("hbxExplain");
	const WCHAR* wchHtmlMsg = GetDisplayStringManager()->GetString("DST_CCBD_BOARD_EXPLAIN");
	m_phbxExplain->SetHtmlFromMemory(wchHtmlMsg, wcslen(wchHtmlMsg));


	m_surNeedItem.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CCBDBoardGui.srf", "srfNeedItem"));
	m_surNeedItem.SetSize(110, 90);
	m_surNeedItem.SetPositionfromParent(77, 217);

	m_ppnlAttachItem = (gui::CPanel*)GetComponent("pnlAttachItem");

	m_pstbBarText = (gui::CStaticBox*)GetComponent("stbBarText");

	m_ppnlBar = (gui::CPanel*)GetComponent("pnlBar");

	m_pbtnPartyMatching = (gui::CButton*)GetComponent("btnRequestPartyMatching");
	m_pbtnPartyMatching->SetText(GetDisplayStringManager()->GetString("DST_PARTY_MATCHING_REGIST"));
	m_slotClickedBtnPartyMatching = m_pbtnPartyMatching->SigClicked().Connect(this, &CCCBDBoardGui::OnClickedBtnPartyMatching);


	m_pbtnEnter = (gui::CButton*)GetComponent("btnDirectEnter");
	m_pbtnEnter->SetText(GetDisplayStringManager()->GetString("DST_DUNGEON_DIRECT_ENTER"));
	m_slotClickedBtnEnter = m_pbtnEnter->SigClicked().Connect(this, &CCCBDBoardGui::OnClickedBtnEnter);


	m_pbtnCancel = (gui::CButton*)GetComponent("btnCancel");
	m_pbtnCancel->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_CANCLE2"));
	m_slotClickedBtnCancel = m_pbtnCancel->SigClicked().Connect(this, &CCCBDBoardGui::OnClickedBtnCancel);


	m_EffectFocus.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));
	m_EffectFocus.SetPositionbyParent(114, 245);


	m_slotMove = m_pThis->SigMove().Connect(this, &CCCBDBoardGui::OnMove);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CCCBDBoardGui::OnPaint);
	m_slotMouseEnterItem = m_ppnlAttachItem->SigMouseEnter().Connect(this, &CCCBDBoardGui::OnMouseEnterItem);
	m_slotMouseLeaveItem = m_ppnlAttachItem->SigMouseLeave().Connect(this, &CCCBDBoardGui::OnMouseLeaveItem);
	m_slotMouseDownAttachItem = m_ppnlAttachItem->SigMouseDown().Connect(this, &CCCBDBoardGui::OnMouseDownAttachItem);
	m_slotMouseUpAttachItem = m_ppnlAttachItem->SigMouseUp().Connect(this, &CCCBDBoardGui::OnMouseUpAttachItem);

	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);


	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventDialog);
	LinkMsg(g_EventRegCCBDCouponItem);


	Show(false);

	NTL_RETURN(TRUE);
}

void CCCBDBoardGui::Destroy()
{
	NTL_FUNCTION("CCCBDBoardGui::Destroy");

	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventRegCCBDCouponItem);

	m_ItemSlot.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CCCBDBoardGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	SetItem(INVALID_SERIAL_ID, INVALID_BYTE, INVALID_BYTE);

	if (!bOpen)
	{
		ShowItemInfoWindow(false);
		
		Init();

		Logic_CancelNpcFacing();
	}
	else
	{

	}

	NTL_RETURN(TRUE);
}

void CCCBDBoardGui::HandleEvents(RWS::CMsg & msg)
{
	if (msg.Id == g_EventNPCDialogOpen)     // Dialog Open
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>(msg.pData);

		if (pData->eDialog != DIALOG_CCBD_BOARD)
			return;

		if (GetDialogManager()->IsOpenDialog(DIALOG_CCBD_BOARD))
			return;

		m_hNPCSerialID = pData->hSerialId;

		GetDialogManager()->OpenDialog(DIALOG_CCBD_BOARD, m_hNPCSerialID);
	}
	else if (msg.Id == g_EventDialog)       // Processing for Dialog Fail
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>(msg.pData);
		if (pData->iDestDialog == DIALOG_CCBD_BOARD)
		{
			if (pData->iType == DIALOGEVENT_OPEN_FAIL_NPC_DIALOG)
			{
				Logic_CancelNpcFacing();

				GetDialogManager()->CloseDialog(DIALOG_CCBD_BOARD);
			}
			else if (pData->iType == DIALOGEVENT_NPC_BYEBYE)
			{
				GetDialogManager()->CloseDialog(DIALOG_CCBD_BOARD);
			}
		}
	}
	else if (msg.Id == g_EventRegCCBDCouponItem)
	{
		SDboEventRegMailAttachItem* pData = (SDboEventRegMailAttachItem*)msg.pData;

		SetItem(pData->hHandle, (RwUInt8)Logic_WhichBagHasItem(pData->hHandle), (RwUInt8)pData->uiPos);
	}
}

void CCCBDBoardGui::OnPaint()
{
	m_surNeedItem.Render();

	if (m_ItemSlot.GetSerial() != INVALID_SERIAL_ID)
		m_ItemSlot.Paint();

	if (m_bItemSlotFocus)
	{
		m_EffectFocus.Render();
	}
}

void CCCBDBoardGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pThis->GetScreenRect();

	m_surNeedItem.SetPositionbyParent(rect.left, rect.top);

	CRectangle itemRect = m_ppnlAttachItem->GetScreenRect();

	m_ItemSlot.SetParentPosition(itemRect.left, itemRect.top);

	m_EffectFocus.SetPositionbyParent(itemRect.left - 2, itemRect.top - 2);
}

void CCCBDBoardGui::OnClickedBtnPartyMatching(gui::CComponent * pComponent)
{
	// to do
}

void CCCBDBoardGui::OnClickedBtnEnter(gui::CComponent * pComponent)
{
	GetDboGlobal()->GetGamePacketGenerator()->SendBattleDungeonEnterReq(m_hNPCSerialID, m_sItem.hItem);
}

void CCCBDBoardGui::OnClickedBtnCancel(gui::CComponent * pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_CCBD_BOARD);
}

void CCCBDBoardGui::OnMouseEnterItem(gui::CComponent * pComponent)
{
	m_bItemSlotFocus = true;
	ShowItemInfoWindow(true);
}

void CCCBDBoardGui::OnMouseLeaveItem(gui::CComponent * pComponent)
{
	m_bItemSlotFocus = false;
	ShowItemInfoWindow(false);
}

void CCCBDBoardGui::OnMouseDownAttachItem(const CKey & key)
{
	if (!GetIconMoveManager()->IsActive())
	{
		return;
	}

	m_ppnlAttachItem->CaptureMouse();
}

void CCCBDBoardGui::OnMouseUpAttachItem(const CKey & key)
{
	m_ppnlAttachItem->ReleaseMouse();

	switch (key.m_nID)
	{
		case UD_LEFT_BUTTON:
		{
			if (!GetIconMoveManager()->IsActive() && GetIconMoveManager()->GetSrcPlace() == PLACE_BAG)
			{
				return;
			}

			// If it is not a bag, modify it so that it does not attach.
			if (GetIconMoveManager()->IsActive() && GetIconMoveManager()->GetSrcPlace() != PLACE_BAG)
			{
				return;
			}

			if (m_ItemSlot.GetSerial() == INVALID_SERIAL_ID && GetIconMoveManager()->GetSrcSerial() != INVALID_SERIAL_ID)
			{
				CNtlSobItem*		pSobItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(GetIconMoveManager()->GetSrcSerial()));
				CNtlSobItemAttr*	pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());

				if (pSobItemAttr->GetStackNum() == GetIconMoveManager()->GetStackCount())
				{
					SetItem(GetIconMoveManager()->GetSrcSerial(),
						(RwUInt8)Logic_WhichBagHasItem(GetIconMoveManager()->GetSrcSerial()),
						(RwUInt8)GetIconMoveManager()->GetSrcSlotIdx());
				}
			}
		}
		break;

		case UD_RIGHT_BUTTON:
		{
			SetItem(INVALID_SERIAL_ID, INVALID_BYTE, INVALID_BYTE);
			m_bItemSlotFocus = false;
			ShowItemInfoWindow(false);
		}
		break;

		default: break;
	}

	GetIconMoveManager()->IconMoveEnd();
}

void CCCBDBoardGui::SetBarText(int nLevel)
{
	m_pstbBarText->Format(GetDisplayStringManager()->GetString("DST_CCBD_BOARD_YOU_CAN_ENTER_CCBD"), nLevel);
}

void CCCBDBoardGui::SetItem(HOBJECT hItem, BYTE byPlace, BYTE byPos)
{
	if (hItem == INVALID_SERIAL_ID)
	{
		if (m_ItemSlot.GetSerial() != INVALID_SERIAL_ID)
		{
			CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_CCBD_DIALOG, PLACE_BAG,
				Logic_ConvertContainerTypeToBagIdx(m_sItem.byPlace), m_sItem.byPos, m_sItem.hItem);
		
			m_ItemSlot.Clear();
		}

		SetBarText(1);
	}
	else
	{
		CNtlSobItemAttr* pItemAttr = Logic_GetItemSobAttrFromSobSerial(hItem);

		sITEM_TBLDAT* pItemTbl = pItemAttr->GetItemTbl();

		if (pItemTbl->byItem_Type == ITEM_TYPE_BATTLE_DUNGEON_TICKET)
		{
			CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG, PLACE_CCBD_DIALOG, PLACE_BAG,
				Logic_ConvertContainerTypeToBagIdx(byPlace), byPos, hItem);

			if(m_ItemSlot.GetSerial() != INVALID_SERIAL_ID)
				m_ItemSlot.Clear();

			m_ItemSlot.Create(m_ppnlAttachItem, DIALOG_CCBD_BOARD, REGULAR_SLOT_ITEM_SOB, SDS_COUNT);
			m_ItemSlot.SetPosition_fromParent(0, 0);

			m_ItemSlot.SetIcon(hItem, pItemAttr->GetStackNum());

			SetBarText((int)pItemTbl->contentsTblidx);
		}
		else
		{
			GetAlarmManager()->AlarmMessage("DST_CCBD_BOARD_INAVLID_ITEM");
			return;
		}
	}


	m_sItem.hItem = hItem;
	m_sItem.byPlace = byPlace;
	m_sItem.byPos = byPos;
}

void CCCBDBoardGui::ShowItemInfoWindow(bool bIsShow)
{
	if (bIsShow && m_ItemSlot.GetSerial() != INVALID_SERIAL_ID)
	{
		CRectangle rect = m_ppnlAttachItem->GetScreenRect();

		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM, rect.left, rect.top, m_ItemSlot.GetSobItem(), DIALOG_CCBD_BOARD);
	}
	else
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_CCBD_BOARD)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}