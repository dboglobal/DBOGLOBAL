#include "precomp_dboclient.h"
#include "ItemSwapCartGui.h"

// core
#include "NtlDebug.h"

// shared
#include "ItemTable.h"
#include "NPCTable.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSLLogic.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"

// dbo
#include "IconMoveManager.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"
#include "InfoWndManager.h"
#include "DialogManager.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboPacketGenerator.h"
#include "DboEventGenerator.h"
#include "ItemSwapGui.h"

namespace
{
#define dGUI_BUY_SELL_SLOT_GAP				97
#define dGUI_SLOT_HORI_GAP					47
#define dGUI_SLOT_VERT_GAP					57
#define dGUI_BUTTON_HORI_GAP				19
}


CItemSwapCartGui::CItemSwapCartGui(const RwChar* pName)
	:CNtlPLGui(pName)
	, m_pSellButton(NULL)
	, m_pTotalSellMoney(NULL)
	, m_hNPCSerial(INVALID_SERIAL_ID)
	, m_uiTotalSellPrice(0)
	, m_byOthersEmptySlot(MAX_SLOT)
	, m_bFocus(FALSE)
	, m_bLeftLock(FALSE)
	, m_bRightLock(FALSE)
	, m_uiUserTradePacketCount(0)
	, m_byInfoWindowIndex(INVALID_BYTE)
	, m_pMoneyIconTexture(NULL)
{
	for (RwInt32 j = 0; j < MAX_SLOT; ++j)
	{
		m_pUpButton[j] = NULL;
		m_pDownButton[j] = NULL;
	}
}

CItemSwapCartGui::~CItemSwapCartGui()
{
	NTL_FUNCTION("CItemSwapCartGui::Destroy");

	Destroy();

	NTL_RETURNVOID();
}

RwBool CItemSwapCartGui::Create()
{
	NTL_FUNCTION("CItemSwapCartGui::Create");

	if (!CNtlPLGui::Create("gui\\ItemSwapCart.rsr", "gui\\ItemSwapCart.srf", "gui\\ItemSwapCart.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	m_pMoneyIconTexture = Logic_CreateTexture(MONEYICON_NAME);

	// Exit Button
	m_pExitButton = (gui::CButton*)GetComponent("ExitButton");

	// Sell button
	m_pSellButton = (gui::CButton*)GetComponent("SellButton");
	m_slotClickedSell = m_pSellButton->SigClicked().Connect(this, &CItemSwapCartGui::ClickedSellButton);

	// Item count plus / minus button
	RwInt32 iButtonX = 20;
	RwInt32 iButtonY = 93;

	for (RwInt32 j = 0; j < MAX_SLOT; ++j)
	{
		if ((j % 2) == 0)
			iButtonX = 20;
		else
			iButtonX = 20 + dGUI_SLOT_HORI_GAP;

		// Plus button
		rect.SetRectWH(iButtonX, iButtonY, 18, 15);
		m_pUpButton[j] = NTL_NEW gui::CButton(rect, "",
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "srfUpButtonUp"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "srfUpButtonDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "srfUpButtonDis"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "srfUpButtonFocus"),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		m_slotUpButton[j] = m_pUpButton[j]->SigClicked().Connect(this, &CItemSwapCartGui::ClickUpButton);


		// Minus button
		rect.SetRectWH(iButtonX + dGUI_BUTTON_HORI_GAP, iButtonY, 18, 15);
		m_pDownButton[j] = NTL_NEW gui::CButton(rect, "",
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "srfDownButtonUp"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "srfDownButtonDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "srfDownButtonDis"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "srfDownButtonFocus"),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		m_slotDownButton[j] = m_pDownButton[j]->SigClicked().Connect(this, &CItemSwapCartGui::ClickDownButton);

		if ((j % 2) != 0)
		{
			iButtonY += dGUI_SLOT_VERT_GAP;
		}
	}

	// Slots
	RwInt32 iSlotX = 23;
	RwInt32 iSlotY = 57;
	for (RwInt32 i = 0; i < MAX_SLOT; ++i)
	{
		if ((i % 2) == 0)
			iSlotX = 23;
		else
			iSlotX = 23 + dGUI_SLOT_HORI_GAP;


		m_SellSlotInfo[i].slot.Create(m_pThis, DIALOG_ITEM_EXCHANGE_CART, REGULAR_SLOT_ITEM_SOB, SDS_COUNT | SFF_USE_ITEM_OPTION_TABLE);
		m_SellSlotInfo[i].slot.SetPosition_fromParent(iSlotX, iSlotY);


		if ((i % 2) != 0)
			iSlotY += dGUI_SLOT_VERT_GAP;
	}

	// Total sales amount
	rect.SetRect(12, 407, 92, 423);
	m_pTotalSellMoney = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
	m_pTotalSellMoney->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pTotalSellMoney->SetText("1,234,567,890");
	m_pTotalSellMoney->Enable(false);

	// Slot focus effect
	m_FocusEffect.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));

	// Sell surface
	m_SellBar.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "srfSellbar"));
	m_SellBar.SetPositionfromParent(16, 28);


	// left-locked surface /// is required?
	m_LeftLockSurface.SetType(CWindowby3::WT_HORIZONTAL);
	m_LeftLockSurface.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "LockSurfaceUp"));
	m_LeftLockSurface.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "LockSurfaceCenter"));
	m_LeftLockSurface.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "LockSurfaceDown"));
	m_LeftLockSurface.SetSize(m_LeftLockSurface.GetWidth(), 415);
	m_LeftLockSurface.SetPositionfromParent(10, 46);

	// Right-locked surface /// is required?
	m_RightLockSurface.SetType(CWindowby3::WT_HORIZONTAL);
	m_RightLockSurface.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "LockSurfaceUp"));
	m_RightLockSurface.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "LockSurfaceCenter"));
	m_RightLockSurface.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ItemSwapCart.srf", "LockSurfaceDown"));
	m_RightLockSurface.SetSize(m_RightLockSurface.GetWidth(), 415);
	m_RightLockSurface.SetPositionfromParent(108, 46);

	// Sig
	m_slotMouseDown = m_pThis->SigMouseDown().Connect(this, &CItemSwapCartGui::OnMouseDown);
	m_slotMouseUp = m_pThis->SigMouseUp().Connect(this, &CItemSwapCartGui::OnMouseUp);
	m_slotMove = m_pThis->SigMove().Connect(this, &CItemSwapCartGui::OnMove);
	m_slotMouseMove = m_pThis->SigMouseMove().Connect(this, &CItemSwapCartGui::OnMouseMove);
	m_slotMouseLeave = m_pThis->SigMouseLeave().Connect(this, &CItemSwapCartGui::OnMouseLeave);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CItemSwapCartGui::OnPaint);
	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CItemSwapCartGui::OnCaptureWheelMove);
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect(this, &CItemSwapCartGui::OnCaptureMouseDown);

	LinkMsg(g_EventShop);
	LinkMsg(g_EventCharObjDelete);

	Show(false);

	Clear();

	NTL_RETURN(TRUE);
}

VOID CItemSwapCartGui::Destroy()
{
	NTL_FUNCTION("CItemSwapCartGui::Destroy");

	CheckInfoWindow();

	Logic_DeleteTexture(m_pMoneyIconTexture);
	m_pMoneyIconTexture = NULL;

	for (RwInt32 i = 0; i < MAX_SLOT; ++i)
	{
		m_SellSlotInfo[i].slot.Destroy();
	}

	UnLinkMsg(g_EventShop);
	UnLinkMsg(g_EventCharObjDelete);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CItemSwapCartGui::Clear()
{
	iMouseDownSlotIndex = -1;

	m_hNPCSerial = INVALID_SERIAL_ID;

	for (RwInt32 i = 0; i < MAX_SLOT; ++i)
	{
		ClearSlot(i);
	}

	CalcTotalSellPrice();
}

VOID CItemSwapCartGui::ClearSlot(RwInt32 iSlot)
{
	// Send event to bag
	if (m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace >= CONTAINER_TYPE_BAG_FIRST &&
		m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace <= CONTAINER_TYPE_BAG_LAST)
	{
		GetIconMoveManager()->IconMoveEnd();

		RwInt8 iBagIndex = Logic_ConvertContainerTypeToBagIdx(m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace);
		CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_TRADECART, PLACE_BAG,
			iBagIndex, m_SellSlotInfo[iSlot].NPCShopSellInfo.byPos, m_SellSlotInfo[iSlot].slot.GetSerial());
	}

	m_SellSlotInfo[iSlot].slot.Clear();
	memset((void*)&m_SellSlotInfo[iSlot].NPCShopSellInfo, 0, sizeof(m_SellSlotInfo[iSlot].NPCShopSellInfo));

	iMouseDownSlotIndex = -1;
}

RwUInt32 CItemSwapCartGui::GetTargetSerial()
{
	return m_hNPCSerial;
}

VOID CItemSwapCartGui::ResetCartTexture()
{
	CItemSwapGui* pNPCShopGui = reinterpret_cast<CItemSwapGui*>(GetDialogManager()->GetDialog(DIALOG_ITEM_EXCHANGE_SHOP));
	NTL_ASSERT(pNPCShopGui, "CItemSwapCartGui::ResetCartTexture, Not exist NPCShop gui");
	pNPCShopGui->UnLink(m_pThis);

	CRectangle rect = m_pThis->GetScreenRect();
	Destroy();

	Create();
	m_pThis->SetPosition(rect);

	pNPCShopGui->Link(m_pThis);
}

VOID CItemSwapCartGui::OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos)
{
	if (!IsShow())
		return;

	if (m_pThis->GetParent()->GetChildComponentReverseAt(pos.x, pos.y) != m_pThis)
		return;

	if (m_pThis->PosInRect(pos.x, pos.y) != gui::CComponent::INRECT)
		return;


	RwInt8 iSlot;
	CRectangle rtScreen = m_pThis->GetScreenRect();


	iSlot = (RwInt8)PtinSlot(pos.x - rtScreen.left, pos.y - rtScreen.top);
	if (iSlot != INVALID_INDEX)
	{
		if (sDelta > 0)
			AddItem(iSlot, 1, m_SellSlotInfo[iSlot].slot.GetItemTable()->tblidx);
		else
			SubItem(iSlot, 1, m_SellSlotInfo[iSlot].slot.GetItemTable()->tblidx);
	}
}

VOID CItemSwapCartGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE_TOP_LINKED(DIALOG_ITEM_EXCHANGE_SHOP, key.m_fX, key.m_fY);
}

VOID CItemSwapCartGui::ClickedSellButton(gui::CComponent* pComponent)
{
	sSHOP_SELL_CART* pSellInfo[MAX_SLOT];

	for (RwInt32 i = 0; i < MAX_SLOT; ++i)
	{
		pSellInfo[i] = &m_SellSlotInfo[i].NPCShopSellInfo;
	}

	GetDboGlobal()->GetGamePacketGenerator()->SendNPCShopSell(m_hNPCSerial, pSellInfo);
}

VOID CItemSwapCartGui::AddItemCount(RwInt32 iSlot, RwInt32 iCount, RwUInt32 itemSerial)
{
	sITEM_TBLDAT* pITEM_DATA = Logic_GetItemDataFromTable(itemSerial);
	if (!pITEM_DATA)
	{
		NTL_ASSERT(false, "CItemSwapCartGui::AddItemCount, Unknown tblidx " << itemSerial);
		return;
	}

	RwInt32 iResult = m_SellSlotInfo[iSlot].NPCShopSellInfo.byStack + iCount;

	if (iResult > pITEM_DATA->byMax_Stack)
		iResult = pITEM_DATA->byMax_Stack;

	if (iResult <= 0)
	{
		ClearSlot(iSlot);
		return;
	}

	m_SellSlotInfo[iSlot].NPCShopSellInfo.byStack = (BYTE)iResult;
	m_SellSlotInfo[iSlot].slot.SetCount(iResult);
}

VOID CItemSwapCartGui::UpdateCount(RwUInt8 bySlot, RwUInt8 byCount)
{
	sITEM_TBLDAT* pITEM_DATA = Logic_GetItemDataFromTable(GetIconMoveManager()->GetSrcSerial());
	if (!pITEM_DATA)
	{
		NTL_ASSERT(false, "CItemSwapCartGui::UpdateCount, Unknown tblidx " << GetIconMoveManager()->GetSrcSerial());
		return;
	}

	if (byCount > pITEM_DATA->byMax_Stack)
		byCount = pITEM_DATA->byMax_Stack;

	if (byCount <= 0)
	{
		ClearSlot(bySlot);
		return;
	}

	m_SellSlotInfo[bySlot].NPCShopSellInfo.byStack = byCount;
	m_SellSlotInfo[bySlot].slot.SetCount(byCount);
}

VOID CItemSwapCartGui::CalcTotalSellPrice()
{
	DWORD dwSellPrice = 0;
	m_uiTotalSellPrice = 0;

	for (RwInt32 i = 0; i < MAX_SLOT; ++i)
	{
		if (m_SellSlotInfo[i].slot.GetSerial() != INVALID_SERIAL_ID)
		{
			dwSellPrice = m_SellSlotInfo[i].slot.GetItemTable()->dwSell_Price * m_SellSlotInfo[i].NPCShopSellInfo.byStack;

			if (dwSellPrice <= 0)
				dwSellPrice = 0;

			m_uiTotalSellPrice += dwSellPrice;
		}
	}

	m_pTotalSellMoney->SetText(Logic_FormatZeni(m_uiTotalSellPrice));


	if (m_uiTotalSellPrice == 0)
		m_pSellButton->ClickEnable(false);
	else
		m_pSellButton->ClickEnable(true);
}

VOID CItemSwapCartGui::ClickUpButton(gui::CComponent* pComponent)
{
	for (RwUInt8 j = 0; j < MAX_SLOT; ++j)
	{
		if (m_pUpButton[j] == pComponent)
		{
			AddItem(j, 1, m_SellSlotInfo[j].slot.GetItemTable()->tblidx);

			break;
		}
	}
}

VOID CItemSwapCartGui::ClickDownButton(gui::CComponent* pComponent)
{
	for (RwUInt8 j = 0; j < MAX_SLOT; ++j)
	{
		if (m_pDownButton[j] == pComponent)
		{
			SubItem(j, 1, m_SellSlotInfo[j].slot.GetItemTable()->tblidx);

			break;
		}
	}
}

VOID CItemSwapCartGui::OnMouseDown(const CKey& key)
{
	iMouseDownSlotIndex = -1;

	if (GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN)
		return;

	for (RwInt32 i = 0; i < MAX_SLOT; ++i)
	{
		if (m_SellSlotInfo[i].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY))
		{
			iMouseDownSlotIndex = i;

			m_pThis->CaptureMouse();
			return;
		}
	}
}

VOID CItemSwapCartGui::OnMouseUp(const CKey& key)
{
	m_pThis->ReleaseMouse();

	if (!IsShow())
	{
		iMouseDownSlotIndex = INVALID_INDEX;
		return;
	}

	for (RwInt32 i = 0; i < MAX_SLOT; ++i)
	{
		// Sales Slot
		if (m_SellSlotInfo[i].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY))
		{
			if (key.m_nID == UD_LEFT_BUTTON)	// Left button
			{
				if (!GetIconMoveManager()->IsActive())
					break;

				if (GetIconMoveManager()->GetSrcPlace() == PLACE_BAG)
				{
					// When a message is sent from a character bag
					RegSellItemByDrag(i);
					GetIconMoveManager()->IconMoveEnd();
				}
			}
			else if (key.m_nID == UD_RIGHT_BUTTON) // Right button
			{
				if (GetIconMoveManager()->IsActive())
					break;

				if (m_SellSlotInfo[i].slot.GetSerial() == INVALID_SERIAL_ID)
					break;

				if (iMouseDownSlotIndex == i)
				{
					ClearSlot(iMouseDownSlotIndex);
					CalcTotalSellPrice();
				}
			}

			// If you have not processed any of the slots
			break;
		}
	}

	iMouseDownSlotIndex = INVALID_INDEX;
}

VOID CItemSwapCartGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = GetPosition();

	for (RwInt32 i = 0; i < MAX_SLOT; ++i)
	{
		m_SellSlotInfo[i].slot.SetParentPosition(rect.left, rect.top);
	}

	m_LeftLockSurface.SetPositionbyParent(rect.left, rect.top);
	m_RightLockSurface.SetPositionbyParent(rect.left, rect.top);

	m_SellBar.SetPositionbyParent(rect.left, rect.top);

	m_bFocus = FALSE;

	CheckInfoWindow();
}

VOID CItemSwapCartGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	FocusEffect(FALSE);

	// Displays item information and shows focus effect
	for (RwInt8 i = 0; i < MAX_SLOT; ++i)
	{
		if (m_SellSlotInfo[i].slot.PtInRect(nX, nY))
		{
			// Select Focus
			FocusEffect(TRUE, i);

			if (GetIconMoveManager()->IsActive())
				return;

			CRectangle rect = m_pThis->GetScreenRect();

			if (m_SellSlotInfo[i].slot.GetSerial() != INVALID_SERIAL_ID)
			{
				if (m_byInfoWindowIndex != i)
				{
					CRectangle rtScreen = m_pThis->GetScreenRect();
					GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM,
						rtScreen.left + m_SellSlotInfo[i].slot.GetX_fromParent(),
						rtScreen.top + m_SellSlotInfo[i].slot.GetY_fromParent(),
						m_SellSlotInfo[i].slot.GetSobItem(), DIALOG_ITEM_EXCHANGE_CART);
					m_byInfoWindowIndex = i;
				}
			}

			return;
		}
	}

	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow(FALSE);
}

VOID CItemSwapCartGui::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(FALSE);
	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow(FALSE);
}

VOID CItemSwapCartGui::FocusEffect(RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
	if (bPush)
	{
		RwInt32 iX = 23;
		RwInt32 iY = 57 + (iSlotIdx / 2) * dGUI_SLOT_VERT_GAP;
		CRectangle rect = m_pThis->GetScreenRect();

		if ((iSlotIdx % 2) != 0)
			iX += dGUI_SLOT_HORI_GAP;

		m_FocusEffect.SetRectWH(rect.left + iX, rect.top + iY, 32, 32);
		m_bFocus = TRUE;
	}
	else
	{
		m_bFocus = FALSE;
	}
}

VOID CItemSwapCartGui::OpenCart(RwUInt32 uiSerial)
{
	// The serial number of the entity that opened the trade cart
	m_hNPCSerial = uiSerial;

	GetDialogManager()->OpenDialog(DIALOG_ITEM_EXCHANGE_CART, INVALID_SERIAL_ID, FALSE);

	// Open a bag
	GetDialogManager()->SwitchBag(TRUE);
}

VOID CItemSwapCartGui::CloseCart()
{
	Clear();

	GetDialogManager()->CloseDialog(DIALOG_ITEM_EXCHANGE_CART, FALSE);

	m_uiTotalSellPrice = 0;
	m_byOthersEmptySlot = MAX_SLOT;
	m_uiUserTradePacketCount = 0;

	m_bLeftLock = FALSE;
	m_bRightLock = FALSE;
}

VOID CItemSwapCartGui::AddItem(RwInt32 iSlotY, RwInt32 iCount, RwUInt32 itemTblidx)
{
	if (iSlotY < 0 || iSlotY >= MAX_SLOT)
	{
		NTL_ASSERT(false, "CItemSwapCartGui::AddItem, Unknown index " << iSlotY);
		return;
	}

	if (m_SellSlotInfo[iSlotY].slot.GetSerial() != INVALID_SERIAL_ID)
	{
		RwInt32 iNewCount = iCount + m_SellSlotInfo[iSlotY].slot.GetCount();
		CNtlSobItemAttr* pSobItemAttr = m_SellSlotInfo[iSlotY].slot.GetSobItemAttr();

		// If the new number is larger than the actual number of items you have,
		if (iNewCount > pSobItemAttr->GetStackNum())
			return;

		AddItemCount(iSlotY, iCount, itemTblidx);
		CalcTotalSellPrice();
	}
}

VOID CItemSwapCartGui::SubItem(RwInt32 iSlotY, RwInt32 iCount, RwUInt32 itemTblidx)
{
	if (iSlotY < 0 || iSlotY >= MAX_SLOT)
	{
		NTL_ASSERT(false, "CItemSwapCartGui::SubItem, Unknown index " << iSlotY);
		return;
	}

	if (m_SellSlotInfo[iSlotY].slot.GetSerial() != INVALID_SERIAL_ID)
	{
		AddItemCount(iSlotY, -iCount, itemTblidx);
		CalcTotalSellPrice();
	}
}

VOID CItemSwapCartGui::RegSellItemByDrag(RwInt32 iSlot)
{
	// The client and server have different Bag indexes and convert.		
	RwUInt32 uiSerial = GetIconMoveManager()->GetSrcSerial();
	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(uiSerial));
	NTL_ASSERT(pSobItem, "CItemSwapCartGui::RegSellItemByDrag, Can not found sob item of the handle : " << uiSerial);
	CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
	NTL_ASSERT(pSobItemAttr, "CItemSwapCartGui::RegSellItemByDrag, Can not found sob item attr of the handle : " << uiSerial);

	if (pSobItemAttr->IsNeedToIdentify())
	{
		// This item can not be sold
		GetAlarmManager()->AlarmMessage("DST_TRADECART_CANNOT_SELL_ITEM");

		return;
	}

	sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();
	NTL_ASSERT(pITEM_TBLDAT, "CItemSwapCartGui::RegSellItemByDrag, Not exist item table of item handle " << uiSerial);

	if (Logic_IsCanSellItem(pSobItemAttr) == FALSE)
	{
		// This item can not be sold
		GetAlarmManager()->AlarmMessage("DST_TRADECART_CANNOT_SELL_ITEM");

		return;
	}

	if (pSobItemAttr->GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_SEAL)
	{
		GetAlarmManager()->AlarmMessage("DST_SEALING_NOT_EXTRACT");
		return;
	}

	// An item already registered in a sales slot can not be registered in another slot.
	for (RwInt32 i = 0; i < MAX_SLOT; ++i)
	{
		if (i != iSlot)
		{
			if (m_SellSlotInfo[i].slot.GetSerial() == uiSerial)
				return;
		}
	}

	if (m_SellSlotInfo[iSlot].slot.GetCount() <= 0)
	{
		// Registration if empty slot
		CRectangle rect = m_pThis->GetScreenRect();
		RwInt32 iCount = GetIconMoveManager()->GetStackCount();
		RwUInt32 uiPrice = pITEM_TBLDAT->dwSell_Price;

		m_SellSlotInfo[iSlot].slot.SetParentPosition(rect.left, rect.top);
		m_SellSlotInfo[iSlot].slot.SetIcon(uiSerial, iCount);
		m_SellSlotInfo[iSlot].slot.SetPrice(uiPrice);

		// Bag number
		m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace = (BYTE)Logic_WhichBagHasItem(uiSerial);

		// Slot index in bag
		m_SellSlotInfo[iSlot].NPCShopSellInfo.byPos = (BYTE)GetIconMoveManager()->GetSrcSlotIdx();

		// item count
		AddItemCount(iSlot, iCount, pITEM_TBLDAT->tblidx);

		// sell price
		CalcTotalSellPrice();

		// Send event to bag
		RwInt8 iBagIndex = Logic_ConvertContainerTypeToBagIdx(m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace);
		CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG, PLACE_TRADECART, PLACE_BAG,
			iBagIndex, m_SellSlotInfo[iSlot].NPCShopSellInfo.byPos, uiSerial);
	}
	else if (m_SellSlotInfo[iSlot].slot.GetSerial() == uiSerial)
	{
		AddItem(iSlot, 1, pITEM_TBLDAT->tblidx);
	}
}

VOID CItemSwapCartGui::RegBuyItemByEvent(SDboEventShop& TradeInfo)
{
	sITEM_TBLDAT* pITEM_DATA = Logic_GetItemDataFromTable(TradeInfo.uiSerial);
	if (!pITEM_DATA)
	{
		DBO_FAIL("Not exist item table of index : " << TradeInfo.uiSerial);
		return;
	}

	const WCHAR* wchItemName = Logic_GetItemName(pITEM_DATA->tblidx);

	sMsgBoxData sMsg;
	sMsg.hExtendHandle = m_hNPCSerial;
	sMsg.sShopBuy.byItemPos = (BYTE)TradeInfo.iPosition;
	sMsg.sShopBuy.byMerchantTab = (BYTE)TradeInfo.iPlace;
	sMsg.sShopBuy.byStack = (BYTE)TradeInfo.iOverlapCount;

	if (TradeInfo.needItemCount == 0)
	{
		GetAlarmManager()->FormattedAlarmMessage("DST_NPCSHOP_FREE_ITEMSWAP_BUY_CONFIRM", FALSE, &sMsg, wchItemName, TradeInfo.iOverlapCount, TradeInfo.ulPrice);
	}
	else
	{
		GetAlarmManager()->FormattedAlarmMessage("DST_NPCSHOP_ITEMSWAP_BUY_CONFIRM", FALSE, &sMsg, wchItemName, TradeInfo.iOverlapCount, TradeInfo.pcItemName,
			TradeInfo.needItemCount, TradeInfo.ulPrice);
	}
}

VOID CItemSwapCartGui::RegSellItemByEvent(RwInt32 iSlot, SDboEventShop& TradeInfo)
{
	CRectangle rect = m_pThis->GetScreenRect();
	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(TradeInfo.uiSerial));
	NTL_ASSERT(pSobItem, "CItemSwapCartGui::RegSellItemByEvent, Can not found sob item of the handle : " << TradeInfo.uiSerial);
	CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
	NTL_ASSERT(pSobItemAttr, "CItemSwapCartGui::RegSellItemByEvent, Can not found sob item attr of the handle : " << TradeInfo.uiSerial);

	if (pSobItemAttr->IsNeedToIdentify())
	{
		// Items you can not sell
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_SELL");
		return;
	}

	sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();
	NTL_ASSERT(pITEM_TBLDAT, "CItemSwapCartGui::RegSellItemByEvent, Not exist item table of item handle " << TradeInfo.uiSerial);

	if (Logic_IsCanSellItem(pSobItemAttr) == FALSE)
	{
		// Items you can not sell
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_SELL");
		return;
	}

	if (pSobItemAttr->GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_SEAL)
	{
		GetAlarmManager()->AlarmMessage("DST_SEALING_NOT_EXTRACT");
		return;
	}

	m_SellSlotInfo[iSlot].slot.SetSerialType(REGULAR_SLOT_ITEM_SOB);
	m_SellSlotInfo[iSlot].slot.SetParentPosition(rect.left, rect.top);
	m_SellSlotInfo[iSlot].slot.SetIcon(TradeInfo.uiSerial, TradeInfo.iOverlapCount);

	// item place
	m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace = (BYTE)Logic_WhichBagHasItem(TradeInfo.uiSerial);

	// Slot index in bag
	m_SellSlotInfo[iSlot].NPCShopSellInfo.byPos = (BYTE)TradeInfo.iPosition;

	// amount
	// If you use the AddItemCount (..) function to add a sales item to an event, do not add the number
	// Set the number of TradeInfo.iOverlapCount as it is.
	if (TradeInfo.iOverlapCount > m_SellSlotInfo[iSlot].slot.GetItemTable()->byMax_Stack)
		TradeInfo.iOverlapCount = m_SellSlotInfo[iSlot].slot.GetItemTable()->byMax_Stack;

	m_SellSlotInfo[iSlot].NPCShopSellInfo.byStack = (BYTE)TradeInfo.iOverlapCount;
	m_SellSlotInfo[iSlot].slot.SetCount(TradeInfo.iOverlapCount);

	// total price
	CalcTotalSellPrice();

	// Send event to bag
	RwInt8 iBagIndex = Logic_ConvertContainerTypeToBagIdx(m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace);
	CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG, PLACE_TRADECART, PLACE_BAG,
		iBagIndex, m_SellSlotInfo[iSlot].NPCShopSellInfo.byPos, m_SellSlotInfo[iSlot].slot.GetSerial());
}

RwInt32 CItemSwapCartGui::FindEmptySlot()
{
	for (RwInt32 i = 0; i < MAX_SLOT; ++i)
	{
		if (m_SellSlotInfo[i].slot.GetCount() <= 0)
			return i;
	}

	return -1;
}

RwUInt8 CItemSwapCartGui::FindSlot(RwUInt32 uiSerial)
{
	for (RwUInt8 i = 0; i < MAX_SLOT; ++i)
	{
		if (m_SellSlotInfo[i].slot.GetSerial() == uiSerial)
			return i;
	}

	return INVALID_SLOTID;
}

RwInt32	 CItemSwapCartGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	for (RwInt32 i = 0; i < MAX_SLOT; ++i)
	{
		if (m_SellSlotInfo[i].slot.PtInRect(iX, iY))
			return i;
	}

	return INVALID_INDEX;
}

VOID CItemSwapCartGui::CheckInfoWindow()
{
	if (GetInfoWndManager()->GetRequestGui() == DIALOG_ITEM_EXCHANGE_CART)
	{
		m_byInfoWindowIndex = INVALID_BYTE;
		GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

RwInt32 CItemSwapCartGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		Show(true);
	}
	else
	{
		Show(false);
		CheckInfoWindow();
	}

	return 1;
}

VOID CItemSwapCartGui::OnPaint()
{
	for (RwInt32 i = 0; i < MAX_SLOT; ++i)
	{
		m_SellSlotInfo[i].slot.Paint();
	}

	if (m_bFocus)
		m_FocusEffect.Render();

	if (m_bLeftLock)
		m_LeftLockSurface.Render();

	if (m_bRightLock)
		m_RightLockSurface.Render();

	m_SellBar.Render();
}

VOID CItemSwapCartGui::HandleEvents(RWS::CMsg &msg)
{
	NTL_FUNCTION("CItemSwapCartGui::HandleEvents");

	if (msg.Id == g_EventShop)
	{
		SDboEventShop* pPacket = (SDboEventShop*)msg.pData;
		switch (pPacket->iType)
		{
			case TRM_ITEM_EXCHANGE_SHOP_OPEN:
			{
				sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob(pPacket->uiSerial);
				if (!pNPC_TBLDAT)
				{
					DBO_FAIL("Not exist npc table of sob handle : " << pPacket->uiSerial);
					NTL_RETURNVOID();
				}

				ResetCartTexture();

				m_pSellButton->SetText(GetDisplayStringManager()->GetString("DST_TRADECART_SELL"));

				m_pSellButton->ClickEnable(false);

				m_pExitButton->Show(false);

				for (RwUInt8 i = 0; i < MAX_SLOT; ++i)
				{
					m_pUpButton[i]->Show(true);
					m_pDownButton[i]->Show(true);
				}

				SetMovable(false);

				OpenCart(pPacket->uiSerial);
			}
			break;
			case TRM_REG_ITEM_BY_NPCSHOP:
			{
				if (m_hNPCSerial == INVALID_SERIAL_ID)
					NTL_RETURNVOID();

				RegBuyItemByEvent(*pPacket);
			}
			break;
			case TRM_REG_ITEM_BY_BAG:
			{
				if (m_hNPCSerial == INVALID_SERIAL_ID)
					NTL_RETURNVOID();

				RwInt32 iSlot = INVALID_INDEX;

				// If the item is already registered, re-register it.
				for (RwInt32 i = 0; i < MAX_SLOT; ++i)
				{
					if (m_SellSlotInfo[i].slot.GetSerial() == pPacket->uiSerial)
					{
						// I handed you an arm item from the bag.
						// means to sell all the items in that slot.
						RegSellItemByEvent(i, *pPacket);
						iSlot = i;

						break;
					}
				}

				// Registration if empty slot			
				if (iSlot == INVALID_INDEX)
				{
					iSlot = FindEmptySlot();

					if (iSlot >= 0)
						RegSellItemByEvent(iSlot, *pPacket);
					else
					{
						// Cart lacks slots
						GetAlarmManager()->AlarmMessage("DST_TRADECART_NO_MORE_SLOT");
					}
				}
			}
			break;
			case TRM_BUY_SUCCESS:
			{
				if (m_hNPCSerial == INVALID_SERIAL_ID)
					NTL_RETURNVOID();

				CheckInfoWindow();
			}
			break;
			case TRM_SELL_SUCCESS:
			{
				if (m_hNPCSerial == INVALID_SERIAL_ID)
					NTL_RETURNVOID();

				for (RwInt32 i = 0; i < MAX_SLOT; ++i)
					ClearSlot(i);

				CalcTotalSellPrice();

				CheckInfoWindow();
			}
			break;
			case TRM_CLOSE:
			{
				if (m_hNPCSerial == INVALID_SERIAL_ID)
					NTL_RETURNVOID();

				CloseCart();
			}
			break;

			default: break;
		}

	}
	else if (msg.Id == g_EventCharObjDelete)
	{
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>(msg.pData);

		if (m_hNPCSerial == *pDeleteSerial)
		{
			// Suddenly your opponent disappears.
			if (GetDialogManager()->IsOpenDialog(DIALOG_ITEM_EXCHANGE_CART))
				CloseCart();
		}
	}

	NTL_RETURNVOID();
}

