#include "precomp_dboclient.h"

// core
#include "QuickTeleportGui.h"
#include "NtlDebug.h"

// shared

// presentation
#include "NtlPLGuiManager.h"

// simulation 
#include "NtlSLEvent.h"
#include "NtlSobNpc.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobItemIcon.h"

// dbo
#include "DboGlobal.h"
#include "IconMoveManager.h"
#include "DialogManager.h"
#include "InfoWndManager.h"



CQuickTeleportGui::CQuickTeleportGui(const RwChar *pName)
	:CNtlPLGui(pName)
{
	Init();
}

CQuickTeleportGui::~CQuickTeleportGui()
{
	Init();
}

void CQuickTeleportGui::Init()
{
	m_nScrollPos = 0;
	SetItemHandle(INVALID_HOBJECT);
	m_bySelectedSlot = INVALID_BYTE;
	m_bLoaded = false;
}

RwBool CQuickTeleportGui::Create()
{
	NTL_FUNCTION("CQuickTeleportGui::Create");

	if (!CNtlPLGui::Create("gui\\QuickTeleport.rsr", "gui\\QuickTeleport.srf", "gui\\QuickTeleport.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_QUICK_TELEPORT"));

	m_pdlgClippingRect = (gui::CDialog*)GetComponent("dlgClippingRect");
	m_pdlgContentsScrolled = (gui::CDialog*)GetComponent("dlgContentsScrolled");

	m_pbtnClose = (gui::CButton*)GetComponent("btnClose");

	m_pbtnSave = (gui::CButton*)GetComponent("btnSave");
	m_pbtnSave->SetText(GetDisplayStringManager()->GetString("DST_SAVE"));

	m_pbtnMove = (gui::CButton*)GetComponent("btnMove");
	m_pbtnMove->SetText(GetDisplayStringManager()->GetString("DST_MOVE"));

	m_pbtnDelete = (gui::CButton*)GetComponent("btnDelete");
	m_pbtnDelete->SetText(GetDisplayStringManager()->GetString("DST_DELETE"));


	m_pScrollBar = (gui::CScrollBar*)GetComponent("scbScroll");
	m_pScrollBar->SetMaxValue(370);
	m_slotScrollChanged = m_pScrollBar->SigValueChanged().Connect(this, &CQuickTeleportGui::OnScrollChanged);
	m_slotScrollSliderMoved = m_pScrollBar->SigSliderMoved().Connect(this, &CQuickTeleportGui::OnScrollChanged);
	m_slotMouseWheel = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CQuickTeleportGui::OnCaptureWheelMove);

	char buf[64];
	for (int i = 0; i < NTL_QUICK_PORTAL_MAX_COUNT; i++)
	{
		int n = i + 1;

		sprintf_s(buf, 64, "btnPanel%d", n);
		m_pButtons[i].btnPanel = (gui::CButton*)GetComponent(buf);

		m_pButtons[i].m_slotPanelBtnClick = m_pButtons[i].btnPanel->SigClicked().Connect(this, &CQuickTeleportGui::OnClickedBtnPanel);

		sprintf_s(buf, 64, "pnlSlotIcon%d", n);
		m_pButtons[i].pnlSlotIcon = (gui::CPanel*)GetComponent(buf);

		sprintf_s(buf, 64, "stbPosition%d", n);
		m_pButtons[i].stbPosition = (gui::CStaticBox*)GetComponent(buf);

		sprintf_s(buf, 64, "stbDate%d", n);
		m_pButtons[i].stbDate = (gui::CStaticBox*)GetComponent(buf);

		sprintf_s(buf, 64, "stbEmpty%d", n);
		m_pButtons[i].stbEmpty = (gui::CStaticBox*)GetComponent(buf);

		if (i > 0)
		{
			m_pButtons[i].stbEmpty->SetText(GetDisplayStringManager()->GetString("DST_SLOT_EMPTY"));
		}
		else
		{
			m_pButtons[i].stbDeadPosition = (gui::CStaticBox*)GetComponent("stbDeadPosition");
			m_pButtons[i].stbDeadDate = (gui::CStaticBox*)GetComponent("stbDeadDate");

			m_pButtons[i].stbDeadPosition->SetText(GetDisplayStringManager()->GetString("DST_QUICK_TELEPORT_TO_FAINT_POS"));
		}

		m_pButtons[i].m_bEmpty = true;
	}


	m_slotCloseBtnClick = m_pbtnClose->SigClicked().Connect(this, &CQuickTeleportGui::OnClickedBtnClose);
	m_slotSaveBtnClick = m_pbtnSave->SigClicked().Connect(this, &CQuickTeleportGui::OnClickedBtnSave);
	m_slotMoveBtnClick = m_pbtnMove->SigClicked().Connect(this, &CQuickTeleportGui::OnClickedBtnMove);
	m_slotDeleteBtnClick = m_pbtnDelete->SigClicked().Connect(this, &CQuickTeleportGui::OnClickedBtnDelete);

	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	Show(false);

	// event register.
	LinkMsg(g_EventOpenQuickTeleport);
	LinkMsg(g_EventGameServerChangeOut);
	LinkMsg(g_EventQuickTeleportLoad);
	LinkMsg(g_EventMsgBoxResult);
	LinkMsg(g_EventQuickTeleportUpdate);
	LinkMsg(g_EventQuickTeleportDelete);
	LinkMsg(g_EventQuickTeleportMove);

	NTL_RETURN(TRUE);
}

void CQuickTeleportGui::Destroy()
{
	NTL_FUNCTION("CQuickTeleportGui::Destroy");

	UnLinkMsg(g_EventOpenQuickTeleport);
	UnLinkMsg(g_EventGameServerChangeOut);
	UnLinkMsg(g_EventQuickTeleportLoad);
	UnLinkMsg(g_EventMsgBoxResult);
	UnLinkMsg(g_EventQuickTeleportUpdate);
	UnLinkMsg(g_EventQuickTeleportDelete);
	UnLinkMsg(g_EventQuickTeleportMove);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

int CQuickTeleportGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		Show(bOpen);
	}
	else
	{
		if (m_bySelectedSlot < NTL_QUICK_PORTAL_MAX_COUNT)
		{
			m_pButtons[m_bySelectedSlot].btnPanel->SetDown(false); // set down false
		}

		m_bySelectedSlot = INVALID_BYTE;

		SetItemHandle(INVALID_HOBJECT);

		Show(bOpen);
	}

	return 1;
}

void CQuickTeleportGui::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CQuickTeleportGui::HandleEvents");

	if (pMsg.Id == g_EventOpenQuickTeleport)
	{
		SDboEventRegMailAttachItem* pData = (SDboEventRegMailAttachItem*)pMsg.pData;

		if (m_bLoaded == false)
		{
			CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(pData->hHandle));
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

			RwUInt8 byPlace = pItem->GetParentItemSlotIdx() + CONTAINER_TYPE_BAG1;
			RwUInt8 byPos = pItem->GetItemSlotIdx();

			GetDboGlobal()->GetGamePacketGenerator()->SendQuickTeleportLoadReq(byPlace, byPos);
		}
		else
		{
			GetDialogManager()->OpenDialog(DIALOG_QUICK_TELEPORT);
		}

		SetItemHandle(pData->hHandle);
	}
	else if (pMsg.Id == g_EventGameServerChangeOut)
	{
		GetDialogManager()->CloseDialog(DIALOG_QUICK_TELEPORT);
	}
	else if (pMsg.Id == g_EventQuickTeleportLoad)
	{
		SDboEventQuickTeleportLoad* pData = (SDboEventQuickTeleportLoad*)pMsg.pData;

		// to do: load data
		for (BYTE i = 0; i < pData->byCount; i++)
		{
			sQUICK_TELEPORT_INFO* pInfo = (sQUICK_TELEPORT_INFO*)&pData->asData[i];

			memcpy(&m_pButtons[pInfo->bySlotNum].teleportInfo, pInfo, sizeof(sQUICK_TELEPORT_INFO));

			// update panel button
			m_pButtons[pInfo->bySlotNum].stbEmpty->Clear();

			WCHAR buf[128] = { 0, };

			std::wstring strAreaName;
			Logic_GetAreaNameFromTblIDX(pInfo->mapNameTblidx, &strAreaName);
			swprintf_s(buf, L"%s : %s", GetDisplayStringManager()->GetString("DST_QUICK_TELEPORT_SAVE_POS"), strAreaName.c_str());
			m_pButtons[pInfo->bySlotNum].stbPosition->Format(GetDisplayStringManager()->GetString("DST_QUICK_TELEPORT_SAVE_POS"), strAreaName.c_str());

			m_pButtons[pInfo->bySlotNum].stbDate->Format(GetDisplayStringManager()->GetString("DST_QUICK_TELEPORT_SAVE_TIME"),
				pInfo->tSaveTime.year, pInfo->tSaveTime.month, pInfo->tSaveTime.day, pInfo->tSaveTime.hour, pInfo->tSaveTime.minute);

			m_pButtons[pInfo->bySlotNum].m_bEmpty = false;
		}

		m_bLoaded = true;

		GetDialogManager()->OpenDialog(DIALOG_QUICK_TELEPORT);
	}
	else if (pMsg.Id == g_EventMsgBoxResult)
	{
		HandleEventsSubMsgBox(pMsg);
	}
	else if (pMsg.Id == g_EventQuickTeleportUpdate)
	{
		SDboEventQuickTeleportUpdate* pData = (SDboEventQuickTeleportUpdate*)pMsg.pData;

		sQUICK_TELEPORT_INFO* pInfo = (sQUICK_TELEPORT_INFO*)pData->asData;

		memcpy(&m_pButtons[pInfo->bySlotNum].teleportInfo, pInfo, sizeof(sQUICK_TELEPORT_INFO));

		// update panel button
		m_pButtons[pInfo->bySlotNum].stbEmpty->Clear();

		WCHAR buf[128] = { 0, };

		std::wstring strAreaName;
		Logic_GetAreaNameFromTblIDX(pInfo->mapNameTblidx, &strAreaName);
		swprintf_s(buf, L"%s : %s", GetDisplayStringManager()->GetString("DST_QUICK_TELEPORT_SAVE_POS"), strAreaName.c_str());
		m_pButtons[pInfo->bySlotNum].stbPosition->Format(GetDisplayStringManager()->GetString("DST_QUICK_TELEPORT_SAVE_POS"), strAreaName.c_str());

		m_pButtons[pInfo->bySlotNum].stbDate->Format(GetDisplayStringManager()->GetString("DST_QUICK_TELEPORT_SAVE_TIME"), 
			pInfo->tSaveTime.year, pInfo->tSaveTime.month, pInfo->tSaveTime.day, pInfo->tSaveTime.hour, pInfo->tSaveTime.minute);

		m_pButtons[pInfo->bySlotNum].m_bEmpty = false;
	}
	else if (pMsg.Id == g_EventQuickTeleportDelete)
	{
		SDboEventQuickTeleportDeleteAndMove* pData = (SDboEventQuickTeleportDeleteAndMove*)pMsg.pData;

		m_pButtons[pData->bySlot].teleportInfo.bySlotNum = INVALID_BYTE;

		// update panel button
		m_pButtons[pData->bySlot].stbEmpty->SetText(GetDisplayStringManager()->GetString("DST_SLOT_EMPTY"));
		m_pButtons[pData->bySlot].stbPosition->Clear();
		m_pButtons[pData->bySlot].stbDate->Clear();

		m_pButtons[pData->bySlot].m_bEmpty = true;
	}
	else if (pMsg.Id == g_EventQuickTeleportMove)
	{
		SDboEventQuickTeleportDeleteAndMove* pData = (SDboEventQuickTeleportDeleteAndMove*)pMsg.pData;

		GetDialogManager()->CloseDialog(DIALOG_QUICK_TELEPORT);
	}

	NTL_RETURNVOID();
}

void CQuickTeleportGui::HandleEventsSubMsgBox(RWS::CMsg & pMsg)
{
	SDboEventMsgBoxResult* pEvent = reinterpret_cast<SDboEventMsgBoxResult*>(pMsg.pData);

	if (pEvent->strID == "DST_COMMERCIAL_MB_QUICK_TELEPORT_SLOT_WRITE")
	{
		if (pEvent->eResult == MBR_OK)
		{
			// send packet
			GetDboGlobal()->GetGamePacketGenerator()->SendQuickTeleportUpdateReq(m_hItemHandle, m_bySelectedSlot);
		}
	}
	else if (pEvent->strID == "DST_COMMERCIAL_MB_QUICK_TELEPORT_SLOT_OVERWRITE")
	{
		if (pEvent->eResult == MBR_OK)
		{
			// send packet
			GetDboGlobal()->GetGamePacketGenerator()->SendQuickTeleportUpdateReq(m_hItemHandle, m_bySelectedSlot);
		}
	}
	else if (pEvent->strID == "DST_COMMERCIAL_MB_QUICK_TELEPORT_SLOT_DELETE")
	{
		if (pEvent->eResult == MBR_OK)
		{
			// send packet
			GetDboGlobal()->GetGamePacketGenerator()->SendQuickTeleportDelReq(m_hItemHandle, m_bySelectedSlot);
		}
	}
	else if (pEvent->strID == "DST_COMMERCIAL_MB_QUICK_TELEPORT_SLOT_MOVE")
	{
		if (pEvent->eResult == MBR_OK)
		{
			// send packet
			GetDboGlobal()->GetGamePacketGenerator()->SendQuickTeleportUseReq(m_hItemHandle, m_bySelectedSlot);
		}
	}
}


void CQuickTeleportGui::OnPaint()
{
}

void CQuickTeleportGui::OnMove(RwInt32 nXPos, RwInt32 nYPos)
{
}

void CQuickTeleportGui::OnClickedBtnClose(gui::CComponent * pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_QUICK_TELEPORT);
}

void CQuickTeleportGui::OnClickedBtnSave(gui::CComponent * pComponent)
{
	if (m_bySelectedSlot >= NTL_QUICK_PORTAL_MAX_COUNT || m_bySelectedSlot == 0)
		return;

	if (m_pButtons[m_bySelectedSlot].m_bEmpty)
		GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_QUICK_TELEPORT_SLOT_WRITE");
	else
		GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_QUICK_TELEPORT_SLOT_OVERWRITE");
}

void CQuickTeleportGui::OnClickedBtnMove(gui::CComponent * pComponent)
{
	if (m_bySelectedSlot >= NTL_QUICK_PORTAL_MAX_COUNT)
		return;

	if(m_pButtons[m_bySelectedSlot].m_bEmpty == false)
		GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_QUICK_TELEPORT_SLOT_MOVE");
}

void CQuickTeleportGui::OnClickedBtnDelete(gui::CComponent * pComponent)
{
	if (m_bySelectedSlot >= NTL_QUICK_PORTAL_MAX_COUNT || m_bySelectedSlot == 0)
		return;

	if (m_pButtons[m_bySelectedSlot].m_bEmpty == false)
		GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_QUICK_TELEPORT_SLOT_DELETE");
}

void CQuickTeleportGui::OnClickedBtnPanel(gui::CComponent * pComponent)
{
	for (int i = 0; i < NTL_QUICK_PORTAL_MAX_COUNT; i++)
	{
		if (m_pButtons[i].btnPanel == pComponent)
		{
			// do something
			if (m_bySelectedSlot == i)
			{
				m_pButtons[i].btnPanel->SetDown(false);
				m_bySelectedSlot = INVALID_BYTE;
			}
			else
			{
				m_pButtons[i].btnPanel->SetDown(true);
				m_bySelectedSlot = i;
			}
		}
		else
		{
			m_pButtons[i].btnPanel->SetDown(false); // set down false
		}
	}
}

void CQuickTeleportGui::OnScrollChanged(int nNewOffset)
{
	m_nScrollPos = nNewOffset;

	CRectangle rtPos = m_pdlgContentsScrolled->GetPosition();
	m_pdlgContentsScrolled->SetPosition(rtPos.left, -m_nScrollPos);
}

void CQuickTeleportGui::OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos & pos)
{
	int nOffset = (int)((float)m_pScrollBar->GetMaxValue() * 0.2f) * sDelta;

	CAPTURE_MOUSEWHEELMOVE_SCROLLBAR(IsShow(), m_pdlgClippingRect, m_pdlgContentsScrolled, m_pScrollBar, nOffset, pos);
}

void CQuickTeleportGui::SetItemHandle(HOBJECT hHandle)
{
	m_hItemHandle = hHandle;
}
