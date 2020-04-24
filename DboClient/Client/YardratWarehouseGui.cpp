#include "precomp_dboclient.h"
#include "YardratWarehouseGui.h"

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

#include "HLSItemTable.h"


#define dSLOT_ROW			4
#define dSLOT_COLUMN		4

#define dSLOT_START_X		27
#define dSLOT_START_Y		76

#define dSLOT_VER_GAP		40
#define dSLOT_HORI_GAP		40


CYardratWarehouseGui::CYardratWarehouseGui(const RwChar* pName)
	:CNtlPLGui(pName)
{
	Init();
}

CYardratWarehouseGui::~CYardratWarehouseGui()
{

}

void CYardratWarehouseGui::Init()
{
	m_pstbTitle = NULL;
	m_pstbCurrentPage = NULL;
	m_pstbPageSlash = NULL;
	m_pstbMaxPage = NULL;
	m_pbtnExit = NULL;
	m_pbtnPrev = NULL;
	m_pbtnNext = NULL;

	m_mapVisibleItems.clear();

	m_bFocus = false;
	m_nCurTab = 0;
	m_nCurPage = 1;
	m_nMaxPage = 1;
	m_iMouseDownSlot = INVALID_INDEX;
}

RwBool CYardratWarehouseGui::Create()
{
	NTL_FUNCTION("CYardratWarehouseGui::Create");

	// Load window
	if (!CNtlPLGui::Create("gui\\HLS.rsr", "gui\\YardratWarehouse.srf", "gui\\YardratWarehouse.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Get Component
	m_pThis = (gui::CDialog*)GetComponent("dlgMain");


	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_YARDRAT"));

	m_pstbCurrentPage = (gui::CStaticBox*)GetComponent("stbCurrentPage");
	m_pstbCurrentPage->SetText(L"1");

	m_pstbPageSlash = (gui::CStaticBox*)GetComponent("stbPageSlash");
	m_pstbPageSlash->SetText(L"/");

	m_pstbMaxPage = (gui::CStaticBox*)GetComponent("stbMaxPage");
	m_pstbMaxPage->SetText(L"1");

	m_pbtnExit = (gui::CButton*)GetComponent("btnExit");
	m_slotClickedBtnExit = m_pbtnExit->SigClicked().Connect(this, &CYardratWarehouseGui::OnClickedBtnExit);

	m_pTabButton = (gui::CTabButton*)GetComponent("TabButton");
	m_slotTab = m_pTabButton->SigSelectChanged().Connect(this, &CYardratWarehouseGui::OnSelectChangeTabButton);

	m_pbtnPrev = (gui::CButton*)GetComponent("btnPrev");
	m_slotClickedBtnPrev = m_pbtnPrev->SigClicked().Connect(this, &CYardratWarehouseGui::OnClickedBtnPrev);

	m_pbtnNext = (gui::CButton*)GetComponent("btnNext");
	m_slotClickedBtnNext = m_pbtnNext->SigClicked().Connect(this, &CYardratWarehouseGui::OnClickedBtnNext);

	// add tabs
	std::wstring wstrTabText = GetDisplayStringManager()->GetString("DST_YARDRAT_TAB_WAITING");
	m_pTabButton->AddTab(wstrTabText);

	wstrTabText = GetDisplayStringManager()->GetString("DST_YARDRAT_TAB_FUNCTIONAL");
	m_pTabButton->AddTab(wstrTabText);

	wstrTabText = GetDisplayStringManager()->GetString("DST_YARDRAT_TAB_EQUIPMENT");
	m_pTabButton->AddTab(wstrTabText);

	m_pTabButton->SelectTab(0);
	//


	m_FocusEffect.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));


	m_slotMove = m_pThis->SigMove().Connect(this, &CYardratWarehouseGui::OnMove);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CYardratWarehouseGui::OnPaint);
	m_slotMouseMove = m_pThis->SigMouseMove().Connect(this, &CYardratWarehouseGui::OnMouseMove);
	m_slotMouseLeave = m_pThis->SigMouseLeave().Connect(this, &CYardratWarehouseGui::OnMouseLeave);
	m_slotMouseDown = m_pThis->SigMouseDown().Connect(this, &CYardratWarehouseGui::OnMouseDown);
	m_slotMouseUp = m_pThis->SigMouseUp().Connect(this, &CYardratWarehouseGui::OnMouseUp);

	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_HLSHOP);

	LinkMsg(g_EventHLShopEvent);
	LinkMsg(g_EventHLShopEventItemInfo);
	LinkMsg(g_EventHLShopEventItemInfoRes);
	LinkMsg(g_EventHLShopEventItemAddNfy);
	LinkMsg(g_EventHLShopEventItemDelNfy);
	LinkMsg(g_EventHLShopEventItemMoveRes);
	LinkMsg(g_EventHLShopEventItemUseRes);

	Show(false);

	NTL_RETURN(TRUE);
}

void CYardratWarehouseGui::Destroy()
{
	NTL_FUNCTION("CYardratWarehouseGui::Destroy");

	UnLinkMsg(g_EventHLShopEvent);
	UnLinkMsg(g_EventHLShopEventItemInfo);
	UnLinkMsg(g_EventHLShopEventItemInfoRes);
	UnLinkMsg(g_EventHLShopEventItemMoveRes);
	UnLinkMsg(g_EventHLShopEventItemUseRes);
	UnLinkMsg(g_EventHLShopEventItemAddNfy);
	UnLinkMsg(g_EventHLShopEventItemDelNfy);

	for (int i = 0; i < dHLS_WAREHOUSE_TAB_COUNT; i++)
	{
		for (std::map<QWORD, sHLS_ITEM*>::iterator it = m_mapItems[i].begin(); it != m_mapItems[i].end(); )
		{
			sHLS_ITEM* pItem = it->second;

			++it;

			delete pItem;
		}

		m_mapItems[i].clear();
	}

	m_mapVisibleItems.clear();

	for (RwUInt8 i = 0; i < dYARDRAT_WAREHOUS_SLOTS_PER_PAGE; i++)
	{
		m_slot[i].Destroy();
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}


RwInt32 CYardratWarehouseGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	NTL_RETURN(TRUE);
}

void CYardratWarehouseGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventHLShopEvent)
	{
		SDboEventHLShopEvent* pData = reinterpret_cast<SDboEventHLShopEvent*>(msg.pData);

		switch (pData->byEventType)
		{
			case eHLSHOP_EVENT_START:
			{
				SetMovable(false);

				GetDialogManager()->OpenDialog(DIALOG_HLSHOP_WAREHOUSE);
			}
			break;
			case eHLSHOP_EVENT_END:
			{
				GetDialogManager()->CloseDialog(DIALOG_HLSHOP_WAREHOUSE);
			}
			break;

			default: break;
		}
	}
	else if (msg.Id == g_EventHLShopEventItemInfo)
	{
		SDboEventHLShopEventItemInfo* pData = reinterpret_cast<SDboEventHLShopEventItemInfo*>(msg.pData);

		LoadItems(pData->byCount, pData->aInfo);
	}
	else if (msg.Id == g_EventHLShopEventItemInfoRes)
	{
		UpdateTabContent(m_nCurTab);
	}
	else if (msg.Id == g_EventHLShopEventItemMoveRes)
	{
		SDboEventHLShopEventItemMoveRes* pData = reinterpret_cast<SDboEventHLShopEventItemMoveRes*>(msg.pData);

		DelItem(pData->qwProductId);
	}
	else if (msg.Id == g_EventHLShopEventItemUseRes)
	{
		SDboEventHLShopEventItemMoveRes* pData = reinterpret_cast<SDboEventHLShopEventItemMoveRes*>(msg.pData);

		DelItem(pData->qwProductId);
	}
	else if (msg.Id == g_EventHLShopEventItemAddNfy)
	{
		SDboEventHLShopEventItemAddNfy* pData = reinterpret_cast<SDboEventHLShopEventItemAddNfy*>(msg.pData);

		AddItem(pData->aInfo);
	}
	else if (msg.Id == g_EventHLShopEventItemDelNfy)
	{
		SDboEventHLShopEventItemDelNfy* pData = reinterpret_cast<SDboEventHLShopEventItemDelNfy*>(msg.pData);

		DelItem(pData->qwProductId);
	}
}

void CYardratWarehouseGui::LoadItems(BYTE byCount, sCASHITEM_BRIEF * aInfo)
{
	int nTab = 0;

	for (BYTE i = 0; i < byCount; i++)
	{
		// get item
		sHLS_ITEM_TBLDAT* pHlsItemTbldat = (sHLS_ITEM_TBLDAT*)API_GetTableContainer()->GetHLSItemTable()->FindData(aInfo[i].HLSitemTblidx);

		// check if item exist
		if (pHlsItemTbldat == NULL)
		{
			DBO_WARNING_MESSAGE("Could not find hlsitem tbldat: " << aInfo[i].HLSitemTblidx);
			continue;
		}

		// get item
		sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(pHlsItemTbldat->itemTblidx);

		// check if item exist
		if (pItemTbldat == NULL)
		{
			DBO_WARNING_MESSAGE("Could not item tbldat: " << pHlsItemTbldat->itemTblidx << ", hlsitem tblidx: " << aInfo[i].HLSitemTblidx);
			continue;
		}

		sHLS_ITEM* pItem = new sHLS_ITEM;

		pItem->itemTblidx = pItemTbldat->tblidx;
		memcpy(&pItem->sBrief, &aInfo[i], sizeof(sCASHITEM_BRIEF));

		nTab = GetTabType(pItemTbldat->byItem_Type);

		m_mapItems[nTab].insert(std::make_pair(pItem->sBrief.qwProductId, pItem));
	}
}

void CYardratWarehouseGui::AddItem(sCASHITEM_BRIEF* pBrief)
{
	int nTab = 1;

	// get item
	sHLS_ITEM_TBLDAT* pHlsItemTbldat = (sHLS_ITEM_TBLDAT*)API_GetTableContainer()->GetHLSItemTable()->FindData(pBrief->HLSitemTblidx);

	// check if item exist
	if (pHlsItemTbldat == NULL)
	{
		DBO_WARNING_MESSAGE("Could not find hlsitem tbldat: " << pBrief->HLSitemTblidx);
		return;
	}

	// get item
	sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(pHlsItemTbldat->itemTblidx);

	// check if item exist
	if (pItemTbldat == NULL)
	{
		DBO_WARNING_MESSAGE("Could not item tbldat: " << pHlsItemTbldat->itemTblidx << ", hlsitem tblidx: " << pBrief->HLSitemTblidx);
		return;
	}

	sHLS_ITEM* pItem = new sHLS_ITEM;

	pItem->itemTblidx = pItemTbldat->tblidx;
	memcpy(&pItem->sBrief, pBrief, sizeof(sCASHITEM_BRIEF));

	nTab = GetTabType(pItemTbldat->byItem_Type);

	m_mapItems[nTab].insert(std::make_pair(pItem->sBrief.qwProductId, pItem));

	if (m_nCurTab == nTab) // only do if we are currently watching the same tab as the item goes in
	{
		size_t visibleCnt = m_mapVisibleItems.size();

		// insert item to slot
		if (m_mapVisibleItems.size() < dYARDRAT_WAREHOUS_SLOTS_PER_PAGE)
		{
			int nSlot = m_mapVisibleItems.size();

			m_slot[nSlot].SetIcon(pItem->itemTblidx, pItem->sBrief.byStackCount);

			m_mapVisibleItems.insert(std::make_pair(pItem->sBrief.qwProductId, nSlot));
		}
		else // if more item than visible exist, reload max page
		{
			SetPage(m_nCurPage);
		}
	}
}

void CYardratWarehouseGui::DelItem(QWORD qwProductId)
{
	std::map<QWORD, sHLS_ITEM*>::iterator it = m_mapItems[m_nCurTab].find(qwProductId);
	if (it == m_mapItems[m_nCurTab].end())
	{
		DBO_WARNING_MESSAGE("Could not find hlsitem: " << qwProductId);
		return;
	}

	sHLS_ITEM* pItem = it->second;

	delete pItem;
	m_mapItems[m_nCurTab].erase(it);

	size_t visibleCnt = m_mapVisibleItems.size();

	if (visibleCnt == 1) // check if only one item visible
	{
		m_slot[0].Clear();
		m_mapVisibleItems.clear();

		if (m_nCurPage > 1) // if we are on a page above 1
		{
			OnClickedBtnPrev(NULL);
		}
	}
	else // if more then 1 item visible, reload the current page
	{
		// re-create slots
		RecreateSlots();

		// clear visible items map
		m_mapVisibleItems.clear();

		int nToSkip = (m_nCurPage - 1) * dYARDRAT_WAREHOUS_SLOTS_PER_PAGE;
		int i = 0;
		int nSkip = 0;

		for (std::map<QWORD, sHLS_ITEM*>::iterator it = m_mapItems[m_nCurTab].begin(); it != m_mapItems[m_nCurTab].end(); it++)
		{
			if (nSkip++ < nToSkip)
				continue;

			sHLS_ITEM* pItem = it->second;

			m_slot[i].SetIcon(pItem->itemTblidx, pItem->sBrief.byStackCount);

			m_mapVisibleItems.insert(std::make_pair(pItem->sBrief.qwProductId, i));

			if (++i >= dYARDRAT_WAREHOUS_SLOTS_PER_PAGE)
				break;
		}
	}
}

void CYardratWarehouseGui::FocusEffect(bool bPush, int iSlotIdx)
{
	if (bPush)
	{
		int iX = (iSlotIdx % dSLOT_COLUMN) * dSLOT_VER_GAP + dSLOT_START_X;
		int iY = (iSlotIdx / dSLOT_ROW) * dSLOT_HORI_GAP + dSLOT_START_Y;
		CRectangle rect = m_pThis->GetScreenRect();

		m_FocusEffect.SetRectWH(rect.left + iX, rect.top + iY, 32, 32);
		m_bFocus = true;
	}
	else
		m_bFocus = false;
}

int CYardratWarehouseGui::PtinSlot(int iX, int iY)
{
	for (RwUInt8 i = 0; i < dYARDRAT_WAREHOUS_SLOTS_PER_PAGE; i++)
	{
		if (m_slot[i].PtInRect(iX, iY))
		{
			return i;
		}
	}

	return INVALID_INDEX;
}

void CYardratWarehouseGui::SetPage(int nPage)
{
	m_nCurPage = nPage;

	// get max pages
	float fPages = ((float)m_mapItems[m_nCurTab].size() / (float)dYARDRAT_WAREHOUS_SLOTS_PER_PAGE) + 0.99999;
	if (fPages < 1)
		fPages = 1;

	m_nMaxPage = (int)fPages;

	m_pstbCurrentPage->SetText(m_nCurPage);
	m_pstbMaxPage->SetText(m_nMaxPage);
}

void CYardratWarehouseGui::UpdateTabContent(int nIndex)
{
	// re-create slots
	RecreateSlots();

	// clear visible items map
	m_mapVisibleItems.clear();

	m_nCurTab = nIndex;
	m_nCurPage = 1;

	// show items
	int i = 0;
	for (std::map<QWORD, sHLS_ITEM*>::iterator it = m_mapItems[nIndex].begin(); it != m_mapItems[nIndex].end(); it++)
	{
		sHLS_ITEM* pItem = it->second;

		if (i < dYARDRAT_WAREHOUS_SLOTS_PER_PAGE)
		{
			m_slot[i].SetIcon(pItem->itemTblidx, pItem->sBrief.byStackCount);

			m_mapVisibleItems.insert(std::make_pair(pItem->sBrief.qwProductId, i));
			++i;
		}
		else
		{
			break;
		}
	}

	//set page
	SetPage(1);
}

void CYardratWarehouseGui::RecreateSlots()
{
	CRectangle rtRect = m_pThis->GetScreenRect();
	for (int i = 0; i < dYARDRAT_WAREHOUS_SLOTS_PER_PAGE; i++)
	{
		m_slot[i].Clear();

		m_slot[i].Create(m_pThis, DIALOG_HLSHOP_WAREHOUSE, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT | SDS_LOCK);
		m_slot[i].SetPosition_fromParent((i % dSLOT_COLUMN) * dSLOT_VER_GAP + dSLOT_START_X, (i / dSLOT_ROW) * dSLOT_HORI_GAP + dSLOT_START_Y);
		m_slot[i].SetParentPosition(rtRect.left, rtRect.top);
	}
}

void CYardratWarehouseGui::ShowItemInfoWindow(bool bIsShow, int nSlot, sCASHITEM_BRIEF * pInfo)
{
	if (bIsShow && m_slot[nSlot].GetItemTable() != NULL)
	{
		sCASHITEM cashItem;
		cashItem.byStackCount = pInfo->byStackCount;
		cashItem.HLSitemTblidx = pInfo->HLSitemTblidx;
		cashItem.itemTblidx = m_slot[nSlot].GetItemTable()->tblidx;
		cashItem.qwProductId = pInfo->qwProductId;
		cashItem.tRegTime = pInfo->tRegTime;
		NTL_SAFE_WCSCPY(cashItem.wchSenderName, pInfo->wchSenderName);

		CRectangle rtScreen = m_pThis->GetScreenRect();

		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_HLS_ITEM, 
			rtScreen.left + m_slot[nSlot].GetX_fromParent(), rtScreen.top + m_slot[nSlot].GetY_fromParent(),
			&cashItem, DIALOG_HLSHOP_WAREHOUSE);
	}
	else
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HLSHOP_WAREHOUSE)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

CYardratWarehouseGui::sHLS_ITEM * CYardratWarehouseGui::GetItem(QWORD qwProductId)
{
	std::map<QWORD, sHLS_ITEM*>::iterator it = m_mapItems[m_nCurTab].find(qwProductId);
	if (it == m_mapItems[m_nCurTab].end())
		return NULL;

	return it->second;
}

QWORD CYardratWarehouseGui::GetProductId(int nSlot)
{
	for (std::map<QWORD, int>::iterator it = m_mapVisibleItems.begin(); it != m_mapVisibleItems.end(); it++)
	{
		if (it->second == nSlot)
		{
			return it->first;
		}
	}

	return INVALID_QWORD;
}

int CYardratWarehouseGui::GetTabType(BYTE byItemType)
{
	if (byItemType == ITEM_TYPE_WAREHOUSE || byItemType == ITEM_TYPE_EVENT_COIN)
	{
		return eTAB_CONSUMABLE;
	}
	else if (byItemType == ITEM_TYPE_CAPSULE || byItemType == ITEM_TYPE_COSTUME_SET || byItemType == ITEM_TYPE_COSTUME_HAIR_STYLE || 
		byItemType == ITEM_TYPE_COSTUME_MASK || byItemType == ITEM_TYPE_COSTUME_HAIR_ACCESSORY || byItemType == ITEM_TYPE_COSTUME_BACK_ACCESSORY)
	{
		return eTAB_EQUIPMENT;
	}

	return eTAB_CRAFT;
}

void CYardratWarehouseGui::OnPaint()
{
	for (RwUInt8 i = 0; i < dYARDRAT_WAREHOUS_SLOTS_PER_PAGE; i++)
	{
		m_slot[i].Paint();
	}

	if (m_bFocus)
		m_FocusEffect.Render();
}

void CYardratWarehouseGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for (RwUInt8 i = 0; i < dYARDRAT_WAREHOUS_SLOTS_PER_PAGE; i++)
	{
		m_slot[i].SetParentPosition(rtScreen.left, rtScreen.top);
	}

	m_FocusEffect.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_bFocus = false;
}

void CYardratWarehouseGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	int iPtinSlot = PtinSlot(nX, nY);

	if (iPtinSlot != INVALID_INDEX)
	{
		FocusEffect(true, iPtinSlot);

		if (m_slot[iPtinSlot].GetItemTable() == NULL)
			return;

		for (std::map<QWORD, int>::iterator it = m_mapVisibleItems.begin(); it != m_mapVisibleItems.end(); it++)
		{
			if (it->second == iPtinSlot)
			{
				sHLS_ITEM* pItem = GetItem(it->first);
				if (pItem)
				{
					ShowItemInfoWindow(true, iPtinSlot, &pItem->sBrief);
				}

				break;
			}
		}
	}
	else
	{
		FocusEffect(false);

		ShowItemInfoWindow(false, 0, 0);
	}
}

void CYardratWarehouseGui::OnMouseLeave(gui::CComponent * pComponent)
{
	FocusEffect(false);
	ShowItemInfoWindow(false, 0, 0);
}

void CYardratWarehouseGui::OnMouseDown(const CKey & key)
{
	if (key.m_nID == UD_RIGHT_BUTTON)
	{
		if (GetIconMoveManager()->IsActive())
			return;

		if (GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN)
			return;

		for (RwUInt8 i = 0; i < dYARDRAT_WAREHOUS_SLOTS_PER_PAGE; i++)
		{
			if (m_slot[i].PtInRect((int)key.m_fX, (int)key.m_fY))
			{
				if (m_slot[i].GetItemTable() != NULL)
				{
					m_iMouseDownSlot = i;
				}

				break;
			}
		}
	}
}

void CYardratWarehouseGui::OnMouseUp(const CKey & key)
{
	if (key.m_nID != UD_RIGHT_BUTTON)
		return;

	if (!IsShow())
	{
		m_iMouseDownSlot = INVALID_INDEX;
		return;
	}

	if (m_iMouseDownSlot < 0 || m_iMouseDownSlot >= dYARDRAT_WAREHOUS_SLOTS_PER_PAGE)
		return;

	// I pressed the mouse in the icon area.
	if (m_slot[m_iMouseDownSlot].GetItemTable() != NULL &&
		m_slot[m_iMouseDownSlot].PtInRect((int)key.m_fX, (int)key.m_fY))
	{
		QWORD productId = GetProductId(m_iMouseDownSlot);

		if (productId != INVALID_QWORD)
		{
			sMsgBoxData data;
			data.uiParam = productId;

			sHLS_ITEM* pItem = GetItem(productId);
			if (pItem)
			{
				sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(pItem->itemTblidx);
				if (!pItemTbldat)
					return;

				int nTabType = GetTabType(pItemTbldat->byItem_Type);

				if (nTabType == 0)
				{
					CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

					const WCHAR* wchName = pItemTextTable->GetText(pItemTbldat->Name).c_str();

					GetAlarmManager()->FormattedAlarmMessage("DST_YADRAT_MB_ITEM_USE_HLS_ITEM_TYPE_DEFAULT", FALSE, &data, wchName);
				}
				else
				{
					GetAlarmManager()->AlarmMessage("DST_YADRAT_MB_GET_ITEM", FALSE, &data);
				}
			}
		}
	}

	m_iMouseDownSlot = INVALID_INDEX;
}

void CYardratWarehouseGui::OnSelectChangeTabButton(INT nCurIndex, INT nPrevIndex)
{
	UpdateTabContent(nCurIndex);
}

void CYardratWarehouseGui::OnClickedBtnExit(gui::CComponent * pComponent)
{
	GetDboGlobal()->GetGamePacketGenerator()->SendCashItemHLShopEndReq();
}

void CYardratWarehouseGui::OnClickedBtnPrev(gui::CComponent * pComponent)
{
	if (m_nCurPage == 1)
		return;

	// re-create slots
	RecreateSlots();

	// clear visible items map
	m_mapVisibleItems.clear();

	int nToSkip = (m_nCurPage - 2) * dYARDRAT_WAREHOUS_SLOTS_PER_PAGE;
	if (nToSkip < dYARDRAT_WAREHOUS_SLOTS_PER_PAGE)
		nToSkip = 0;

	int i = 0;
	int nSkip = 0;

	for (std::map<QWORD, sHLS_ITEM*>::iterator it = m_mapItems[m_nCurTab].begin(); it != m_mapItems[m_nCurTab].end(); it++)
	{
		if (nSkip++ < nToSkip)
			continue;

		sHLS_ITEM* pItem = it->second;

		m_slot[i].SetIcon(pItem->itemTblidx, pItem->sBrief.byStackCount);

		m_mapVisibleItems.insert(std::make_pair(pItem->sBrief.qwProductId, i));
			
		if (++i >= dYARDRAT_WAREHOUS_SLOTS_PER_PAGE)
			break;
	}

	SetPage(m_nCurPage - 1);
}

void CYardratWarehouseGui::OnClickedBtnNext(gui::CComponent * pComponent)
{
	if (m_nCurPage == m_nMaxPage)
		return;

	// re-create slots
	RecreateSlots();

	// clear visible items map
	m_mapVisibleItems.clear();

	int nToSkip = m_nCurPage * dYARDRAT_WAREHOUS_SLOTS_PER_PAGE;
	int i = 0;
	int nSkip = 0;

	for (std::map<QWORD, sHLS_ITEM*>::iterator it = m_mapItems[m_nCurTab].begin(); it != m_mapItems[m_nCurTab].end(); it++)
	{
		if (nSkip++ < nToSkip)
			continue;

		sHLS_ITEM* pItem = it->second;

		m_slot[i].SetIcon(pItem->itemTblidx, pItem->sBrief.byStackCount);

		m_mapVisibleItems.insert(std::make_pair(pItem->sBrief.qwProductId, i));

		if (++i >= dYARDRAT_WAREHOUS_SLOTS_PER_PAGE)
			break;
	}

	SetPage(m_nCurPage + 1);
}
