#include "precomp_dboclient.h"
#include "UltimateDungeonGui.h"

// core
#include "NtlDebug.h"

// share
#include "TextAllTable.h"
#include "NPCTable.h"
#include "TableContainer.h"
#include "DungeonTable.h"


// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "ntlCameraController.h"
#include "NtlSLTMQ.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "IconMoveManager.h"
#include "InfoWndManager.h"
#include "DboGlobal.h"
#include "DisplayStringManager.h"



CUltimateDungeonGui::CUltimateDungeonGui(const RwChar* pName)
	:CNtlPLGui(pName)
	, m_hNPCSerial(INVALID_SERIAL_ID)
	, m_byDifficulty(INVALID_BYTE)
{
}

CUltimateDungeonGui::~CUltimateDungeonGui()
{

}

RwBool CUltimateDungeonGui::Create()
{
	NTL_FUNCTION("CUltimateDungeonGui::Create");

	if (!CNtlPLGui::Create("gui\\UltimateDungeon.rsr", "gui\\UltimateDungeon.srf", "gui\\UltimateDungeon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	m_pTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pTitle->SetText(GetDisplayStringManager()->GetString("DST_ULTIMATE_DUNGEON_TITLE"));

	m_pLimitedLevel = (gui::CStaticBox*)GetComponent("stbLimitedLevel");
	m_pLimitedLevel->SetTextColor(INFOCOLOR_6);
	m_pLimitedLevel->SetText(GetDisplayStringManager()->GetString("DST_TMQ_BOARD_LIMITE_LEVEL"));

	m_pLimitedLevelValue = (gui::CStaticBox*)GetComponent("stbLimitedLevelValue");

	m_pstbDungeonName = (gui::CStaticBox*)GetComponent("stbDungeonName");

	m_pstbDifficult = (gui::CStaticBox*)GetComponent("stbDifficulty");
	m_pstbDifficult->SetTextColor(INFOCOLOR_6);
	m_pstbDifficult->SetText(GetDisplayStringManager()->GetString("DST_LEVEL_OF_DIFFICULT"));

	m_pstbLimitedTime = (gui::CStaticBox*)GetComponent("stbLimitedTime");
	m_pstbLimitedTime->SetTextColor(INFOCOLOR_6);
	m_pstbLimitedTime->SetText(GetDisplayStringManager()->GetString("DST_TMQ_BOARD_LIMITE_TIME"));

	m_pstbFatigability = (gui::CStaticBox*)GetComponent("stbFatigability");
	m_pstbFatigability->SetTextColor(INFOCOLOR_6);
	m_pstbFatigability->SetText(GetDisplayStringManager()->GetString("DST_TMQ_BOARD_TMQ_LIMITED_POINT"));

	m_pstbLimitedTimeValue = (gui::CStaticBox*)GetComponent("stbLimitedTimeValue");

	m_pstbFatigabilityValue = (gui::CStaticBox*)GetComponent("stbFatigabilityValue");

	m_pExitButton = (gui::CButton*)GetComponent("btnClose");
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CUltimateDungeonGui::ClickedCloseButton);

	m_pbtnRequestPartyMatching = (gui::CButton*)GetComponent("btnRequestPartyMatching");
	m_pbtnRequestPartyMatching->SetText(GetDisplayStringManager()->GetString("DST_PARTY_MATCHING_REGIST"));
	m_slotbtnRequestPartyMatching = m_pbtnRequestPartyMatching->SigClicked().Connect(this, &CUltimateDungeonGui::ClickedRequestPartyMatchingButton);

	m_pbtnDirectEnter = (gui::CButton*)GetComponent("btnDirectEnter");
	m_pbtnDirectEnter->SetText(GetDisplayStringManager()->GetString("DST_DUNGEON_DIRECT_ENTER"));
	m_slotbtnDirectEnter = m_pbtnDirectEnter->SigClicked().Connect(this, &CUltimateDungeonGui::ClickedDirectEnterButton);

	m_pbtnBottomClose = (gui::CButton*)GetComponent("btnBottomClose");
	m_pbtnBottomClose->SetText(GetDisplayStringManager()->GetString("DST_QUEST_CLOSE_BTN"));
	m_slotbtnBottomClose = m_pbtnBottomClose->SigClicked().Connect(this, &CUltimateDungeonGui::ClickedCloseButton);

	m_phtbDungeonInfo = (gui::CHtmlBox*)GetComponent("htbDungeonInfo");

	m_pbtnDifficultyCombo = (gui::CComboBox*)GetComponent("btnDifficultyCombo");



	// sig
	m_slotbtnDifficultyCombo = m_pbtnDifficultyCombo->SigSelected().Connect(this, &CUltimateDungeonGui::OnSelectDifficulty);

	Show(false);

	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventCharObjDelete);
	LinkMsg(g_EventDialog);
	LinkMsg(g_EventUDEnterRes);

	NTL_RETURN(TRUE);
}

VOID CUltimateDungeonGui::Destroy()
{
	NTL_FUNCTION("CUltimateDungeonGui::Destroy");

	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventUDEnterRes);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CUltimateDungeonGui::Clear()
{
}

SERIAL_HANDLE CUltimateDungeonGui::GetNPCHandle()
{
	return m_hNPCSerial;
}

void CUltimateDungeonGui::ClickedCloseButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_ULTIMATEDUNGEON);
}

void CUltimateDungeonGui::ClickedRequestPartyMatchingButton(gui::CComponent* pComponent)
{
	// to do
}

void CUltimateDungeonGui::ClickedDirectEnterButton(gui::CComponent* pComponent)
{
	if (GetDboGlobal()->GetGamePacketGenerator()->SendUltimateDungeonEnterReq(m_hNPCSerial, m_byDifficulty) == false)
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}

void CUltimateDungeonGui::OnSelectDifficulty(INT nSelectIndex)
{
	// set difficulty
	m_byDifficulty = (BYTE)m_pbtnDifficultyCombo->GetSelectedItemData();

	sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob(m_hNPCSerial);
	if (!pNPC_TBLDAT)
	{
		DBO_WARNING_MESSAGE("Unable to get NPC table");
		return;
	}

	sDUNGEON_TBLDAT* pDUNGEON_TBLDAT = (sDUNGEON_TBLDAT*)API_GetTableContainer()->GetDungeonTable()->FindDataByGroupIdx(pNPC_TBLDAT->amerchant_Tblidx[0], m_byDifficulty);
	if (!pDUNGEON_TBLDAT)
	{
		DBO_WARNING_MESSAGE("Unable to get dungeon table");
		return;
	}

	// set required level
	m_pLimitedLevelValue->Format(GetDisplayStringManager()->GetString("DST_TMQBOARD_LIMITEDLEVEL"), pDUNGEON_TBLDAT->byMinLevel);
}

bool CUltimateDungeonGui::LoadUdInfo(SERIAL_HANDLE hNPCSerial)
{
	sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob(hNPCSerial);
	if (!pNPC_TBLDAT)
	{
		DBO_WARNING_MESSAGE("Unable to get NPC table");
		return false;
	}

	CDungeonTable* pDungeonTable = API_GetTableContainer()->GetDungeonTable();

	for (CDungeonTable::TABLEIT iter = pDungeonTable->Begin(); iter != pDungeonTable->End(); iter++)
	{
		sDUNGEON_TBLDAT * pData = (sDUNGEON_TBLDAT*)iter->second;

		if (pData && pData->groupIdx == pNPC_TBLDAT->amerchant_Tblidx[0])
		{
			char chDifficulty[128];
			sprintf_s(chDifficulty, 128, "DST_TMQ_DEGREE_OF_DIFFICULTY_%d", pData->byDungeonType + 1);

			m_pbtnDifficultyCombo->AddItem(GetDisplayStringManager()->GetString(chDifficulty), (DWORD)pData->byDungeonType - 1);
		}
	}

	// select first item
	m_pbtnDifficultyCombo->SelectItem(0);

	// get selected difficulty
	m_byDifficulty = (BYTE)m_pbtnDifficultyCombo->GetSelectedItemData();
	
	sDUNGEON_TBLDAT* pDUNGEON_TBLDAT = (sDUNGEON_TBLDAT*)pDungeonTable->FindDataByGroupIdx(pNPC_TBLDAT->amerchant_Tblidx[0], m_byDifficulty);
	if (!pDUNGEON_TBLDAT)
	{
		DBO_WARNING_MESSAGE("Unable to get dungeon table");
		return false;
	}

	// set required level
	m_pLimitedLevelValue->Format(GetDisplayStringManager()->GetString("DST_TMQBOARD_LIMITEDLEVEL"), pDUNGEON_TBLDAT->byMinLevel);


	CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetPartyDungeonTbl();

	// set ud name
	m_pstbDungeonName->SetText(pTextTable->GetText(pDUNGEON_TBLDAT->groupIdx + 1000).c_str());

	// set ud description
	//m_phtbDungeonInfo->SetHtmlFromMemory(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), (int)pQUEST_TEXT_DATA_TBLDAT->wstrText.length());

	// set limited time
	m_pstbLimitedTimeValue->SetText(GetDisplayStringManager()->GetString("DST_TMQBOARD_LIMITEDTIME"));

	// set fatigability count (how often you can do the dungeon)
	m_pstbFatigabilityValue->SetText(GetDisplayStringManager()->GetString("DST_TMQBOARD_COUNTTEXT"));


	return true;
}

int CUltimateDungeonGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		Show(true);
	}
	else
	{
		Show(false);

		Logic_CancelNpcFacing();

		Clear();

		m_hNPCSerial = INVALID_SERIAL_ID;
		m_byDifficulty = INVALID_BYTE;

		m_pbtnDifficultyCombo->ClearAllItems();
	}

	return 1;
}

VOID CUltimateDungeonGui::HandleEvents(RWS::CMsg &msg)
{
	NTL_FUNCTION("CUltimateDungeonGui::HandleEvents");

	if (msg.Id == g_EventNPCDialogOpen)
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>(msg.pData);

		if (pData->eDialog != DIALOG_ULTIMATEDUNGEON)
			return;

		// Same NPC check
		if (m_hNPCSerial == pData->hSerialId)
			return;

		// If you are having a conversation with another NPC
		if (m_hNPCSerial != INVALID_SERIAL_ID)
		{
			GetDialogManager()->CloseDialog(DIALOG_ULTIMATEDUNGEON);
			return;
		}

		// set UD info
		if (LoadUdInfo(pData->hSerialId) == false)
		{
			Logic_CancelNpcFacing();
			return;
		}

		RwBool bOpenDialog = GetDialogManager()->OpenDialog(DIALOG_ULTIMATEDUNGEON, pData->hSerialId);

		if (!bOpenDialog)
		{
			Logic_CancelNpcFacing();
			return;
		}

		CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();

		if (pTMQ->GetNPCHandle() == INVALID_SERIAL_ID)
			pTMQ->SetNPCHandle(pData->hSerialId);

		m_hNPCSerial = pData->hSerialId;
	}
	else if (msg.Id == g_EventCharObjDelete)
	{
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>(msg.pData);

		if (m_hNPCSerial == *pDeleteSerial)
		{
			// NPC server is down or suddenly NPC disappears
			GetDialogManager()->CloseDialog(DIALOG_ULTIMATEDUNGEON);
		}
	}
	else if (msg.Id == g_EventDialog)
	{
		SDboEventDialog* pPacket = (SDboEventDialog*)msg.pData;
		if (pPacket->iType == DIALOGEVENT_OPEN_FAIL_NPC_DIALOG &&
			pPacket->iDestDialog == DIALOG_ULTIMATEDUNGEON)
		{
			// Failed to open TMQ bulletin board and resultcode from server
			Logic_CancelNpcFacing();

			m_hNPCSerial = INVALID_SERIAL_ID;
		}
	}
	else if (msg.Id == g_EventUDEnterRes)
	{
		if (!IsShow())
			return;

		if (m_hNPCSerial == INVALID_SERIAL_ID)
			return;

		GetDialogManager()->CloseDialog(DIALOG_ULTIMATEDUNGEON);
	}

	NTL_RETURNVOID();
}