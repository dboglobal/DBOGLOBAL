#include "precomp_dboclient.h"
#include "ItemBindSealingGui.h"

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



CItemBindSealingGui::CItemBindSealingGui(const RwChar * pName)
	:CNtlPLGui(pName),
	m_pStbTitle(NULL),
	m_pBtnClose(NULL),
	m_pBtnSeal(NULL),
	m_pnlItemSlot(NULL),
	m_pnlCardSlot(NULL),
	m_pnlItemSealedSlot(NULL),
	m_stbItemTitleText(NULL),
	m_stbItemDescText(NULL),
	m_stbCardTitleText(NULL),
	m_stbCardDescText(NULL),
	m_stbMatchNotice(NULL),
	m_stbNeedCoinText(NULL),
	m_stbCardCoinText(NULL)
{
	m_bIsSealReady = false;
	m_byRequiredCoin = 0;
	m_byHasCoin = 0;
}

CItemBindSealingGui::~CItemBindSealingGui()
{
}

RwBool CItemBindSealingGui::Create()
{
	NTL_FUNCTION("CItemBindSealingGui::Create");

	if (!CNtlPLGui::Create("gui\\ItemBindSealingGui.rsr", "gui\\ItemBindSealingGui.srf", "gui\\ItemBindSealingGui.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	
	m_pBtnClose = (gui::CButton*)GetComponent("btnExit");
	m_pBtnSeal = (gui::CButton*)GetComponent("btnSealing");

	m_pnlItemSlot = (gui::CPanel*)GetComponent("pnlItemSlot");
	m_pnlCardSlot = (gui::CPanel*)GetComponent("pnlCardSlot");
	m_pnlItemSealedSlot = (gui::CPanel*)GetComponent("pnlItemSealedSlot");

	m_pStbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_stbItemTitleText = (gui::CStaticBox*)GetComponent("stbItemTitleText");
	m_stbItemDescText = (gui::CStaticBox*)GetComponent("stbItemDescText");
	m_stbCardTitleText = (gui::CStaticBox*)GetComponent("stbCardTitleText");
	m_stbCardDescText = (gui::CStaticBox*)GetComponent("stbCardDescText");
	m_stbMatchNotice = (gui::CStaticBox*)GetComponent("stbMatchNotice");
	m_stbNeedCoinText = (gui::CStaticBox*)GetComponent("stbNeedCoinText");
	m_stbCardCoinText = (gui::CStaticBox*)GetComponent("stbCardCoinText");


	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	m_slotClickedBtnClose = m_pBtnClose->SigClicked().Connect(this, &CItemBindSealingGui::OnClickedBtnClose);
	m_slotClickedBtnSeal = m_pBtnSeal->SigClicked().Connect(this, &CItemBindSealingGui::OnClickedBtnSeal);


	m_pStbTitle->SetText(GetDisplayStringManager()->GetString("DST_SEALING_TITLE"));
	m_stbItemTitleText->SetText(GetDisplayStringManager()->GetString("DST_REVERTED_ITEM"));
	m_stbItemDescText->SetText(GetDisplayStringManager()->GetString("DST_SET_REVERTED_ITEM"));
	m_stbCardTitleText->SetText(GetDisplayStringManager()->GetString("DST_COIN"));
	m_stbCardDescText->SetText(GetDisplayStringManager()->GetString("DST_SET_COIN"));
	m_stbMatchNotice->SetText(GetDisplayStringManager()->GetString("DST_SEALING_RULE_NOTICE"));

	m_pBtnSeal->SetText(GetDisplayStringManager()->GetString("DST_DISASSEMBLE_CLOSE"));

	m_stbNeedCoinText->SetText(0);
	m_stbCardCoinText->SetText(0);

	LinkMsg(g_EventRegBindSealingAttachItem);
	LinkMsg(g_EventSobItemRestrictionUpdate);

	m_slotMovePnlAttachItem = m_pnlItemSlot->SigMove().Connect(this, &CItemBindSealingGui::OnMovePnlAttachItem);
	m_slotPaintPnlAttachItem = m_pnlItemSlot->SigPaint().Connect(this, &CItemBindSealingGui::OnPaintPnlAttachItem);
	m_slotMouseDownPnlAttachItem = m_pnlItemSlot->SigMouseDown().Connect(this, &CItemBindSealingGui::OnMouseDownPnlAttachItem);
	m_slotMouseUpPnlAttachItem = m_pnlItemSlot->SigMouseUp().Connect(this, &CItemBindSealingGui::OnMouseUpPnlAttachItem);
	m_slotMouseEnterPnlAttachItem = m_pnlItemSlot->SigMouseEnter().Connect(this, &CItemBindSealingGui::OnMouseEnterPnlAttachItem);
	m_slotMouseLeavePnlAttachItem = m_pnlItemSlot->SigMouseLeave().Connect(this, &CItemBindSealingGui::OnMouseLeavePnlAttachItem);

	m_slotMovePnlAttachSealCoinItem = m_pnlCardSlot->SigMove().Connect(this, &CItemBindSealingGui::OnMovePnlAttachSealCoinItem);
	m_slotPaintPnlAttachSealCoinItem = m_pnlCardSlot->SigPaint().Connect(this, &CItemBindSealingGui::OnPaintPnlAttachSealCoinItem);
	m_slotMouseDownPnlAttachSealCoinItem = m_pnlCardSlot->SigMouseDown().Connect(this, &CItemBindSealingGui::OnMouseDownPnlAttachSealCoinItem);
	m_slotMouseUpPnlAttachSealCoinItem = m_pnlCardSlot->SigMouseUp().Connect(this, &CItemBindSealingGui::OnMouseUpPnlAttachSealCoinItem);
	m_slotMouseEnterPnlAttachSealCoinItem = m_pnlCardSlot->SigMouseEnter().Connect(this, &CItemBindSealingGui::OnMouseEnterPnlAttachSealCoinItem);
	m_slotMouseLeavePnlAttachSealCoinItem = m_pnlCardSlot->SigMouseLeave().Connect(this, &CItemBindSealingGui::OnMouseLeavePnlAttachSealCoinItem);

	m_slotMovePnlSealedItem = m_pnlItemSealedSlot->SigMove().Connect(this, &CItemBindSealingGui::OnMovePnlSealedItem);
	m_slotPaintPnlSealedItem = m_pnlItemSealedSlot->SigPaint().Connect(this, &CItemBindSealingGui::OnPaintPnlSealedItem);
	m_slotMouseUpPnlSealedItem = m_pnlItemSealedSlot->SigMouseUp().Connect(this, &CItemBindSealingGui::OnMouseUpPnlSealedItem);
	m_slotMouseEnterPnlSealedItem = m_pnlItemSealedSlot->SigMouseEnter().Connect(this, &CItemBindSealingGui::OnMouseEnterPnlSealedItem);
	m_slotMouseLeavePnlSealedItem = m_pnlItemSealedSlot->SigMouseLeave().Connect(this, &CItemBindSealingGui::OnMouseLeavePnlSealedItem);


	m_pFlash = NTL_NEW gui::CFlash(m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pFlash->Enable(false);
	m_pFlash->Show(false);

	m_pSlotSealSuccessEffectEnd = m_pFlash->SigMovieEnd().Connect(this, &CItemBindSealingGui::OnSealSuccessEffectEnd);


	SwitchDialog(false);
	GetNtlGuiManager()->AddUpdateFunc(this);

	NTL_RETURN(TRUE);
}

void CItemBindSealingGui::Destroy()
{
	NTL_FUNCTION("CItemBindSealingGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	UnLinkMsg(g_EventRegBindSealingAttachItem);
	UnLinkMsg(g_EventSobItemRestrictionUpdate);

	NTL_DELETE(m_pFlash);

	m_AttachItemSlot.Destroy();
	m_AttachSealItemSlot.Destroy();
	m_SealedItemSlot.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

void CItemBindSealingGui::Update(RwReal fElapsed)
{
	if (m_pFlash->IsPlayMovie())
	{
		m_pFlash->Update(fElapsed);
	}
}

void CItemBindSealingGui::HandleEvents(RWS::CMsg & msg)
{
	NTL_FUNCTION("CItemBindSealingGui::HandleEvents");

	if (msg.Id == g_EventRegBindSealingAttachItem)
	{
		SDboEventRegMailAttachItem* pData = (SDboEventRegMailAttachItem*)msg.pData;

		bool bInserSuccess = false;

		CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(pData->hHandle));
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

		sITEM_TBLDAT* pItemTbl = pItemAttr->GetItemTbl();

		if (pItemTbl->byItem_Type == ITEM_TYPE_SEAL)
		{
			if (m_AttachSealItemSlot.GetSerial() == INVALID_SERIAL_ID)
			{
				SetSealItem(pData->hHandle, (RwUInt8)Logic_WhichBagHasItem(pData->hHandle), (RwUInt8)pData->uiPos, pItemAttr->GetStackNum());
			}
		}
		else
		{
			if (m_AttachItemSlot.GetSerial() == INVALID_SERIAL_ID)
			{
				SetItem(pData->hHandle, (RwUInt8)Logic_WhichBagHasItem(pData->hHandle), (RwUInt8)pData->uiPos);
			}
		}
	}
	else if (msg.Id == g_EventSobItemRestrictionUpdate)
	{
		SNtlEventSobItemRestrictionUpdate* pData = reinterpret_cast<SNtlEventSobItemRestrictionUpdate*>(msg.pData);

		if (pData->byRestrictState == ITEM_RESTRICT_STATE_TYPE_SEAL)
		{
			GetAlarmManager()->AlarmMessage("DST_SEALING_SUCCESS");

			// remove item
			SetNeedCoinCount(0);
			m_AttachItemSlot.Clear();
			CheckReady();

			// update seal coin
			UpdateSealCoinsAfterSealing();
			
			// play effect
			m_pFlash->Load("ItemSeal_Success.swf");
			m_pFlash->SetSize(100, 100);

			CRectangle rect = m_pnlItemSealedSlot->GetPosition();
			m_pFlash->SetPosition(rect.left - 34, rect.top - 36);

			m_pFlash->PlayMovie(TRUE);
			m_pFlash->RestartMovie();
			m_pFlash->Show(true);

			// show sealed item
			SetSealedItem(m_sItem.hItem);
		}
	}
}

RwInt32 CItemBindSealingGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	if (!bOpen)
	{
		SetCoinCount(0);
		SetNeedCoinCount(0);

		UnsetItems();

		Logic_CancelNpcFacing();
	}

	NTL_RETURN(TRUE);
}

void CItemBindSealingGui::OnClickedBtnClose(gui::CComponent * pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_BIND_SEALING);
}

void CItemBindSealingGui::OnClickedBtnSeal(gui::CComponent * pComponent)
{
	if (m_bIsSealReady)
	{
		//send seal request
		GetDboGlobal()->GetGamePacketGenerator()->SendItemSealReq(m_sItem.byPlace, m_sItem.byPos, m_sSealCoinItem.byPlace, m_sSealCoinItem.byPos);
	}
	else
	{
		GetDialogManager()->CloseDialog(DIALOG_BIND_SEALING);
	}
}


void CItemBindSealingGui::OnPaintPnlAttachItem()
{
	m_AttachItemSlot.Paint();
}

void CItemBindSealingGui::OnMovePnlAttachItem(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pnlItemSlot->GetScreenRect();
	m_AttachItemSlot.SetParentPosition(rect.left, rect.top);
}

void CItemBindSealingGui::OnMouseDownPnlAttachItem(const CKey & key)
{
	if (!GetIconMoveManager()->IsActive())
	{
		return;
	}

	m_pnlItemSlot->CaptureMouse();
}

void CItemBindSealingGui::OnMouseUpPnlAttachItem(const CKey & key)
{
	m_pnlItemSlot->ReleaseMouse();
	if (!m_AttachItemSlot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fX))
	{
		return;
	}

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

			if (m_AttachItemSlot.GetSerial() == INVALID_SERIAL_ID && GetIconMoveManager()->GetSrcSerial() != INVALID_SERIAL_ID)
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
			SetItem(INVALID_SERIAL_ID);
			ShowItemInfoWindow(FALSE);
		}
		break;

		default: break;
	}

	GetIconMoveManager()->IconMoveEnd();
}

void CItemBindSealingGui::OnMouseEnterPnlAttachItem(gui::CComponent * pComponent)
{
	ShowItemInfoWindow(TRUE);
}

void CItemBindSealingGui::OnMouseLeavePnlAttachItem(gui::CComponent * pComponent)
{
	ShowItemInfoWindow(FALSE);
}






void CItemBindSealingGui::OnPaintPnlAttachSealCoinItem()
{
	m_AttachSealItemSlot.Paint();
}

void CItemBindSealingGui::OnMovePnlAttachSealCoinItem(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pnlCardSlot->GetScreenRect();
	m_AttachSealItemSlot.SetParentPosition(rect.left, rect.top);
}

void CItemBindSealingGui::OnMouseDownPnlAttachSealCoinItem(const CKey & key)
{
	if (!GetIconMoveManager()->IsActive())
	{
		return;
	}

	m_pnlCardSlot->CaptureMouse();
}

void CItemBindSealingGui::OnMouseUpPnlAttachSealCoinItem(const CKey & key)
{
	m_pnlCardSlot->ReleaseMouse();
	if (!m_AttachSealItemSlot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fX))
	{
		return;
	}

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

		if (m_AttachSealItemSlot.GetSerial() == INVALID_SERIAL_ID && GetIconMoveManager()->GetSrcSerial() != INVALID_SERIAL_ID)
		{
			CNtlSobItem*		pSobItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(GetIconMoveManager()->GetSrcSerial()));
			CNtlSobItemAttr*	pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());

			if (pSobItemAttr->GetStackNum() == GetIconMoveManager()->GetStackCount())
			{
				sITEM_TBLDAT* pItemTbl = pSobItemAttr->GetItemTbl();

				if (pItemTbl->byItem_Type == ITEM_TYPE_SEAL)
				{
					SetSealItem(GetIconMoveManager()->GetSrcSerial(),
						(RwUInt8)Logic_WhichBagHasItem(GetIconMoveManager()->GetSrcSerial()),
						(RwUInt8)GetIconMoveManager()->GetSrcSlotIdx(),
						pSobItemAttr->GetStackNum());
				}
				else
				{
					GetAlarmManager()->AlarmMessage("DST_SEALING_NEED_GOLD_COIN");
				}
			}
		}
	}
	break;

	case UD_RIGHT_BUTTON:
	{
		SetSealItem(INVALID_SERIAL_ID);
		ShowSealItemInfoWindow(FALSE);
	}
	break;

	default: break;
	}

	GetIconMoveManager()->IconMoveEnd();
}

void CItemBindSealingGui::OnMouseEnterPnlAttachSealCoinItem(gui::CComponent * pComponent)
{
	ShowSealItemInfoWindow(TRUE);
}

void CItemBindSealingGui::OnMouseLeavePnlAttachSealCoinItem(gui::CComponent * pComponent)
{
	ShowSealItemInfoWindow(FALSE);
}

void CItemBindSealingGui::OnPaintPnlSealedItem()
{
	m_SealedItemSlot.Paint();
}

void CItemBindSealingGui::OnMovePnlSealedItem(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pnlItemSealedSlot->GetScreenRect();
	m_SealedItemSlot.SetParentPosition(rect.left, rect.top);
}

void CItemBindSealingGui::OnMouseUpPnlSealedItem(const CKey & key)
{
	m_pnlItemSealedSlot->ReleaseMouse();
	if (!m_SealedItemSlot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fX))
	{
		return;
	}

	switch (key.m_nID)
	{
		case UD_RIGHT_BUTTON:
		{
			SetSealedItem(INVALID_SERIAL_ID);
			ShowSealedItemInfoWindow(FALSE);
		}
		break;

		default: break;
	}
}

void CItemBindSealingGui::OnMouseEnterPnlSealedItem(gui::CComponent * pComponent)
{
	ShowSealedItemInfoWindow(TRUE);
}

void CItemBindSealingGui::OnMouseLeavePnlSealedItem(gui::CComponent * pComponent)
{
	ShowSealedItemInfoWindow(FALSE);
}

void CItemBindSealingGui::SetItem(SERIAL_HANDLE hItem, RwUInt8 uiPlace, RwUInt8 uiPos)
{
	if (hItem == INVALID_SERIAL_ID)
	{
		if (m_AttachItemSlot.GetSerial() != INVALID_SERIAL_ID)
		{
			CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_SUB_BIND_SEALING_ITEM, PLACE_BAG,
				Logic_ConvertContainerTypeToBagIdx(m_sItem.byPlace), m_sItem.byPos, m_sItem.hItem);

			m_AttachItemSlot.Clear();
		}

		SetNeedCoinCount(0);

		CheckReady();
	}
	else
	{
		CNtlSobItemAttr* pItemAttr = Logic_GetItemSobAttrFromSobSerial(hItem);

		sITEM_TBLDAT* pItemTbl = pItemAttr->GetItemTbl();

		if (IsSealing(pItemTbl->byRestrictType, pItemAttr->GetRestrictState(), pItemTbl->byEquip_Type))
		{
			SetSealedItem(INVALID_SERIAL_ID);

			CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG, PLACE_SUB_BIND_SEALING_ITEM, PLACE_BAG,
				Logic_ConvertContainerTypeToBagIdx(uiPlace), uiPos, hItem);

			if (m_AttachItemSlot.GetSerial() != INVALID_SERIAL_ID)
				m_AttachItemSlot.Clear();

			m_AttachItemSlot.Create(m_pnlItemSlot, DIALOG_BIND_SEALING, REGULAR_SLOT_ITEM_SOB, SDS_NONE);
			m_AttachItemSlot.SetPosition_fromParent(0, 0);
			m_AttachItemSlot.SetParentPosition(m_pnlItemSlot->GetScreenRect().left, m_pnlItemSlot->GetScreenRect().top);

			m_AttachItemSlot.SetIcon(hItem);

			SetNeedCoinCount(GetRequiredSealItemNum(pItemAttr->GetRank(), pItemAttr->GetGrade()));

			CheckReady();
		}
		else
		{
			GetAlarmManager()->AlarmMessage("DST_SEALING_NEED_RESTRICT_EQUIP");
			return;
		}
	}

	m_sItem.hItem = hItem;
	m_sItem.byPlace = uiPlace;
	m_sItem.byPos = uiPos;
}

void CItemBindSealingGui::SetSealItem(SERIAL_HANDLE hItem, RwUInt8 uiPlace, RwUInt8 uiPos, RwUInt8 uiStackCount)
{
	if (hItem == INVALID_SERIAL_ID)
	{
		if (m_AttachSealItemSlot.GetSerial() != INVALID_SERIAL_ID)
		{
			CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_SUB_BIND_SEALING_ITEM, PLACE_BAG,
				Logic_ConvertContainerTypeToBagIdx(m_sSealCoinItem.byPlace), m_sSealCoinItem.byPos, m_sSealCoinItem.hItem);

			SetCoinCount(0);

			m_AttachSealItemSlot.Clear();
		}
	}
	else
	{
		CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG, PLACE_SUB_BIND_SEALING_ITEM, PLACE_BAG,
			Logic_ConvertContainerTypeToBagIdx(uiPlace), uiPos, hItem);

		if (m_AttachSealItemSlot.GetSerial() != INVALID_SERIAL_ID)
			m_AttachSealItemSlot.Clear();

		m_AttachSealItemSlot.Create(m_pnlCardSlot, DIALOG_BIND_SEALING, REGULAR_SLOT_ITEM_SOB, SDS_COUNT);
		m_AttachSealItemSlot.SetPosition_fromParent(0, 0);
		m_AttachSealItemSlot.SetParentPosition(m_pnlCardSlot->GetScreenRect().left, m_pnlCardSlot->GetScreenRect().top);

		m_AttachSealItemSlot.SetIcon(hItem, uiStackCount);

		SetCoinCount(uiStackCount);
	}

	m_sSealCoinItem.hItem = hItem;
	m_sSealCoinItem.byPlace = uiPlace;
	m_sSealCoinItem.byPos = uiPos;

	CheckReady();
}

void CItemBindSealingGui::SetSealedItem(SERIAL_HANDLE hItem)
{
	if (m_SealedItemSlot.GetSerial() != INVALID_SERIAL_ID)
	{
		CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_SUB_BIND_SEALING_ITEM, PLACE_BAG,
			Logic_ConvertContainerTypeToBagIdx(m_sItem.byPlace), m_sItem.byPos, m_sItem.hItem);

		m_SealedItemSlot.Clear();
	}

	if (hItem == INVALID_SERIAL_ID)
	{
		m_sItem.byPlace = INVALID_BYTE;
		m_sItem.byPos = INVALID_BYTE;
	}
	else
	{
		m_SealedItemSlot.Create(m_pnlItemSealedSlot, DIALOG_BIND_SEALING, REGULAR_SLOT_ITEM_SOB, SDS_NONE);
		m_SealedItemSlot.SetPosition_fromParent(0, 0);
		m_SealedItemSlot.SetParentPosition(m_pnlItemSealedSlot->GetScreenRect().left, m_pnlItemSealedSlot->GetScreenRect().top);

		m_SealedItemSlot.SetIcon(hItem);
	}

	m_sItem.hItem = hItem;
}

void CItemBindSealingGui::UnsetItems()
{
	if (m_AttachItemSlot.GetSerial() != INVALID_SERIAL_ID)
	{
		CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_SUB_BIND_SEALING_ITEM, PLACE_BAG,
			Logic_ConvertContainerTypeToBagIdx(m_sItem.byPlace), m_sItem.byPos, m_sItem.hItem);

		m_AttachItemSlot.Clear();
	}

	if (m_AttachSealItemSlot.GetSerial() != INVALID_SERIAL_ID)
	{
		CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_SUB_BIND_SEALING_ITEM, PLACE_BAG,
			Logic_ConvertContainerTypeToBagIdx(m_sSealCoinItem.byPlace), m_sSealCoinItem.byPos, m_sSealCoinItem.hItem);
	
		m_AttachSealItemSlot.Clear();
	}

	if (m_SealedItemSlot.GetSerial() != INVALID_SERIAL_ID)
	{
		CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_SUB_BIND_SEALING_ITEM, PLACE_BAG,
			Logic_ConvertContainerTypeToBagIdx(m_sItem.byPlace), m_sItem.byPos, m_sItem.hItem);
	
		m_SealedItemSlot.Clear();
	}

	m_sSealCoinItem.hItem = INVALID_SERIAL_ID;
	m_sSealCoinItem.byPlace = INVALID_BYTE;
	m_sSealCoinItem.byPos = INVALID_BYTE;

	m_sItem.hItem = INVALID_SERIAL_ID;
	m_sItem.byPlace = INVALID_BYTE;
	m_sItem.byPos = INVALID_BYTE;

	CheckReady();
}

void CItemBindSealingGui::UpdateSealCoinsAfterSealing()
{
	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(m_sSealCoinItem.hItem));
	if (!pItem)
	{
		SetSealItem(INVALID_SERIAL_ID);

		return;
	}

	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

	sITEM_TBLDAT* pItemTbl = pItemAttr->GetItemTbl();

	if (pItemTbl->byItem_Type == ITEM_TYPE_SEAL)
	{
		if (m_AttachSealItemSlot.GetSerial() != INVALID_SERIAL_ID)
		{
			SetSealItem(m_sSealCoinItem.hItem, m_sSealCoinItem.byPlace, m_sSealCoinItem.byPos, pItemAttr->GetStackNum());
		}
	}
	else
	{
		NTL_ASSERTFAIL("CItemBindSealingGui::UpdateSealCoinsAfterSealing - Invalid Seal coin item");
	}
}

void CItemBindSealingGui::CheckReady()
{
	if(m_bIsSealReady == false)
	{
		if (m_AttachItemSlot.GetSerial() != INVALID_SERIAL_ID && m_AttachSealItemSlot.GetSerial() != INVALID_SERIAL_ID)
		{
			if (m_byHasCoin >= m_byRequiredCoin)
			{
				m_bIsSealReady = true;
				m_pBtnSeal->SetText(GetDisplayStringManager()->GetString("DST_SEALING_REVERTED"));
			}
			else
			{
				GetAlarmManager()->AlarmMessage("DST_SEALING_NEED_MORE_COIN");
			}
		}
	}
	else
	{
		m_bIsSealReady = false;
		m_pBtnSeal->SetText(GetDisplayStringManager()->GetString("DST_DISASSEMBLE_CLOSE"));
	}
}

void CItemBindSealingGui::ShowItemInfoWindow(RwBool bIsShow)
{
	if (bIsShow && m_AttachItemSlot.GetSerial() != INVALID_SERIAL_ID)
	{
		CRectangle rect = m_pnlItemSlot->GetScreenRect();
		if (Logic_GetItemDataFromSob(m_AttachItemSlot.GetSerial()))
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM, rect.left, rect.top, m_AttachItemSlot.GetSobItem(), DIALOG_BIND_SEALING);
		}
		else
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM, rect.left, rect.top, NULL, DIALOG_BIND_SEALING);
		}
	}
	else
	{
		if (DIALOG_BIND_SEALING == GetInfoWndManager()->GetRequestGui())
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

void CItemBindSealingGui::ShowSealItemInfoWindow(RwBool bIsShow)
{
	if (bIsShow && m_AttachSealItemSlot.GetSerial() != INVALID_SERIAL_ID)
	{
		CRectangle rect = m_pnlCardSlot->GetScreenRect();
		if (Logic_GetItemDataFromSob(m_AttachSealItemSlot.GetSerial()))
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM, rect.left, rect.top, m_AttachSealItemSlot.GetSobItem(), DIALOG_BIND_SEALING);
		}
		else
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM, rect.left, rect.top, NULL, DIALOG_BIND_SEALING);
		}
	}
	else
	{
		if (DIALOG_BIND_SEALING == GetInfoWndManager()->GetRequestGui())
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

void CItemBindSealingGui::ShowSealedItemInfoWindow(RwBool bIsShow)
{
	if (bIsShow && m_SealedItemSlot.GetSerial() != INVALID_SERIAL_ID)
	{
		CRectangle rect = m_pnlItemSealedSlot->GetScreenRect();
		if (Logic_GetItemDataFromSob(m_SealedItemSlot.GetSerial()))
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM, rect.left, rect.top, m_SealedItemSlot.GetSobItem(), DIALOG_BIND_SEALING);
		}
		else
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM, rect.left, rect.top, NULL, DIALOG_BIND_SEALING);
		}
	}
	else
	{
		if (DIALOG_BIND_SEALING == GetInfoWndManager()->GetRequestGui())
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

void CItemBindSealingGui::SetCoinCount(BYTE byVal)
{
	m_byHasCoin = byVal;
	m_stbCardCoinText->SetText(m_byHasCoin);
}

void CItemBindSealingGui::SetNeedCoinCount(BYTE byVal)
{
	m_byRequiredCoin = byVal;
	m_stbNeedCoinText->SetText(m_byRequiredCoin);
}

void CItemBindSealingGui::OnSealSuccessEffectEnd(gui::CComponent * pComponent)
{
	gui::CFlash* pFlash = reinterpret_cast<gui::CFlash*>(pComponent);

	if (m_pFlash->GetScreenRect() == pFlash->GetScreenRect())
	{
		// Item Create Effect

		// Flash Unload and Hide
		pFlash->Show(false);
		m_pFlash->Unload();

		return;
	}

	// If the algorithm comes up here, it means that there is a flash object with the wrong information.
	NTL_ASSERTFAIL("CItemBindSealingGui::OnSealSuccessEffectEnd - Invalid flash component");
}
