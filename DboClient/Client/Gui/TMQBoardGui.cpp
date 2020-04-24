#include "precomp_dboclient.h"
#include "TMQBoardGui.h"

// core
#include "NtlDebug.h"

// share
#include "TextAllTable.h"
#include "TimeQuestTable.h"
#include "NPCTable.h"
#include "TableContainer.h"
#include "QuestTextDataTable.h"


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



CTMQBoardGui::CTMQBoardGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_hNPCSerial(INVALID_SERIAL_ID)
,m_pTIMEQUEST_TBLDAT(NULL)
, m_byDifficulty(INVALID_TIMEQUEST_DIFFICULTY)
{
}

CTMQBoardGui::~CTMQBoardGui()
{

}

RwBool CTMQBoardGui::Create()
{
	NTL_FUNCTION( "CTMQBoardGui::Create" );

	if(!CNtlPLGui::Create("gui\\TMQBoard.rsr", "gui\\TMQBoard.srf", "gui\\TMQBoard.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;	

	m_pTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pTitle->SetText(GetDisplayStringManager()->GetString("DST_TMQ_STATUS_TITLE"));

	m_pExitButton = (gui::CButton*)GetComponent( "btnClose" );
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CTMQBoardGui::ClickedCloseButton);

	m_phtbQuestSenario = (gui::CHtmlBox*)GetComponent("htbQuestSenario");

	m_pLimitedLevel = (gui::CStaticBox*)GetComponent("stbLimitedLevel");
	m_pLimitedLevel->SetTextColor(INFOCOLOR_6);
	m_pLimitedLevel->SetText(GetDisplayStringManager()->GetString("DST_TMQ_BOARD_LIMITE_LEVEL"));

	m_pLimitedLevelValue = (gui::CStaticBox*)GetComponent("stbLimitedLevelValue");

	m_pstbQuestName = (gui::CStaticBox*)GetComponent("stbQuestName");

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

	m_pbtnDifficultyCombo = (gui::CComboBox*)GetComponent("btnDifficultyCombo");


	m_pbtnRequestPartyMatching = (gui::CButton*)GetComponent( "btnRequestPartyMatching" );
	m_pbtnRequestPartyMatching->SetText(GetDisplayStringManager()->GetString("DST_PARTY_MATCHING_REGIST"));
	m_slotbtnRequestPartyMatching = m_pbtnRequestPartyMatching->SigClicked().Connect(this, &CTMQBoardGui::ClickedRequestPartyMatchingButton);

	m_pbtnDirectEnter = (gui::CButton*)GetComponent( "btnDirectEnter" );
	m_pbtnDirectEnter->SetText(GetDisplayStringManager()->GetString("DST_DUNGEON_DIRECT_ENTER"));
	m_slotbtnDirectEnter = m_pbtnDirectEnter->SigClicked().Connect(this, &CTMQBoardGui::ClickedDirectEnterButton);

	m_pbtnBottomClose = (gui::CButton*)GetComponent( "btnBottomClose" );
	m_pbtnBottomClose->SetText(GetDisplayStringManager()->GetString("DST_QUEST_CLOSE_BTN"));
	m_slotbtnBottomClose = m_pbtnBottomClose->SigClicked().Connect(this, &CTMQBoardGui::ClickedCloseButton);




	// sig	
	m_slotPaint = m_pThis->SigPaint().Connect( this, &CTMQBoardGui::OnPaint );
	m_slotMove = m_pThis->SigMove().Connect( this, &CTMQBoardGui::OnMove );
	m_slotMouseMove = m_pThis->SigMouseMove().Connect( this, &CTMQBoardGui::OnMouseMove );
	m_slotMouseLeave = m_pThis->SigMouseLeave().Connect( this, &CTMQBoardGui::OnMouseLeave );
	m_slotbtnDifficultyCombo = m_pbtnDifficultyCombo->SigSelected().Connect(this, &CTMQBoardGui::OnSelectDifficulty);

	Show(false);

	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventCharObjDelete);
	LinkMsg(g_EventDialog);
	LinkMsg(g_EventTMQEnterRes);

	NTL_RETURN(TRUE);
}

VOID CTMQBoardGui::Destroy()
{
	NTL_FUNCTION("CTMQBoardGui::Destroy");

	CheckInfoWindow();

	m_NeedItem.Destroy();
	
	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventTMQEnterRes);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTMQBoardGui::Clear()
{
}

SERIAL_HANDLE CTMQBoardGui::GetNPCHandle()
{
	return m_hNPCSerial;
}

void CTMQBoardGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_TMQBOARD )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

void CTMQBoardGui::ClickedCloseButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_TMQBOARD);
}

void CTMQBoardGui::ClickedRequestPartyMatchingButton(gui::CComponent* pComponent)
{
	// to do
}

void CTMQBoardGui::ClickedDirectEnterButton(gui::CComponent* pComponent)
{
	if (m_NeedItem.IsShowDisable())
	{
		GetAlarmManager()->AlarmMessage("GAME_TIMEQUEST_MUST_HAVE_NEED_ITEM", TRUE);
		return;
	}

	if (GetDboGlobal()->GetGamePacketGenerator()->SendTimeQuestEnterReq(m_hNPCSerial, m_byDifficulty) == false)
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
}


void CTMQBoardGui::OnPaint()
{
	if (m_NeedItem.GetSerial() != INVALID_SERIAL_ID)
		m_NeedItem.Paint();
}

void CTMQBoardGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_NeedItem.SetParentPosition(rtScreen.left, rtScreen.top);

	CheckInfoWindow();
}

void CTMQBoardGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( m_NeedItem.PtInRect(nX, nY) && m_NeedItem.GetSerial() != INVALID_SERIAL_ID )
	{
		if( GetInfoWndManager()->GetInfoWndState() != CInfoWndManager::INFOWND_TABLE_ITEM )
		{
			CRectangle rtScreen = m_pThis->GetScreenRect();
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
												rtScreen.left + m_NeedItem.GetX_fromParent(),
												rtScreen.top + m_NeedItem.GetY_fromParent(),
												m_NeedItem.GetItemTable(), DIALOG_TMQBOARD );
		}

		return;
	}

	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

void CTMQBoardGui::OnMouseLeave(gui::CComponent* pComponent)
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

void CTMQBoardGui::OnSelectDifficulty(INT nSelectIndex)
{
	// set difficulty
	m_byDifficulty = (BYTE)m_pbtnDifficultyCombo->GetSelectedItemData();

	// set required level
	m_pLimitedLevelValue->Format(GetDisplayStringManager()->GetString("DST_TMQBOARD_LIMITEDLEVEL"), m_pTIMEQUEST_TBLDAT->sTimeQuestDataset[m_byDifficulty].byMinMemberLevel);

	// load item
	LoadItem();
}

bool CTMQBoardGui::LoadTmqInfo(HOBJECT hNPCSerial)
{
	sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob(hNPCSerial);
	if (!pNPC_TBLDAT)
	{
		DBO_WARNING_MESSAGE("Unable to get NPC table");
		return false;
	}

	sTIMEQUEST_TBLDAT* pTIMEQUEST_TBLDAT = (sTIMEQUEST_TBLDAT*)API_GetTableContainer()->GetTimeQuestTable()->FindData(pNPC_TBLDAT->amerchant_Tblidx[0]);
	if (!pTIMEQUEST_TBLDAT)
	{
		DBO_WARNING_MESSAGE("Unable to get time quest table");
		return false;
	}

	m_pTIMEQUEST_TBLDAT = pTIMEQUEST_TBLDAT;

	for (BYTE i = 0; i < MAX_TIMEQUEST_DIFFICULTY; i++)
	{
		if (BIT_FLAG_TEST(pTIMEQUEST_TBLDAT->byDifficultyFlag, MAKE_BIT_FLAG(i)))
		{
			char chDifficulty[128];
			sprintf_s(chDifficulty, 128, "DST_TMQ_DEGREE_OF_DIFFICULTY_%d", i + 1);

			m_pbtnDifficultyCombo->AddItem(GetDisplayStringManager()->GetString(chDifficulty), (DWORD)i);
		}
	}
	// select first item
	m_pbtnDifficultyCombo->SelectItem(0);

	// get selected difficulty
	m_byDifficulty = (BYTE)m_pbtnDifficultyCombo->GetSelectedItemData();

	// set required level
	m_pLimitedLevelValue->Format(GetDisplayStringManager()->GetString("DST_TMQBOARD_LIMITEDLEVEL"), pTIMEQUEST_TBLDAT->sTimeQuestDataset[m_byDifficulty].byMinMemberLevel);

	CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();

	sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT = NULL;

	// set tmq name
	pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>(pQuestTextTable->FindData(pTIMEQUEST_TBLDAT->sTimeQuestDataset[m_byDifficulty].nameTblidx));
	if (pQUEST_TEXT_DATA_TBLDAT)
	{
		std::wstring& wstrText = gui::GetHtmlFromMemoryString(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size());
		m_pstbQuestName->SetText(wstrText.c_str());
	}

	pQUEST_TEXT_DATA_TBLDAT = NULL;

	// set tmq description
	pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>(pQuestTextTable->FindData(pTIMEQUEST_TBLDAT->sTimeQuestDataset[m_byDifficulty].questStringTblidx));
	if (pQUEST_TEXT_DATA_TBLDAT)
	{
		m_phtbQuestSenario->SetHtmlFromMemory(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), (int)pQUEST_TEXT_DATA_TBLDAT->wstrText.length());
	}

	// set limited time
	m_pstbLimitedTimeValue->SetText(GetDisplayStringManager()->GetString("DST_TMQBOARD_LIMITEDTIME"));

	// set fatigability count (how often you can do the dungeon)
	m_pstbFatigabilityValue->SetText(GetDisplayStringManager()->GetString("DST_TMQBOARD_COUNTTEXT"));

	// load item
	LoadItem();

	return true;
}

void CTMQBoardGui::LoadItem()
{
	TBLIDX itemTblidx = m_pTIMEQUEST_TBLDAT->sTimeQuestDataset[m_byDifficulty].needItemTblidx;

	sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(itemTblidx);
	if (!pItemTbldat)
	{
		DBO_WARNING_MESSAGE("Could not find item tblidx = " << itemTblidx);
		return;
	}

	if (m_NeedItem.GetItemTable())
		m_NeedItem.Clear();

	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_NeedItem.Create(m_pThis, DIALOG_TMQBOARD, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT | SDS_DISABLE);
	m_NeedItem.SetSize(NTL_ITEM_ICON_SIZE);
	m_NeedItem.SetPosition_fromParent(265, 284);
	m_NeedItem.SetParentPosition(rtScreen.left, rtScreen.top);
	m_NeedItem.SetIcon(pItemTbldat->tblidx, m_pTIMEQUEST_TBLDAT->sTimeQuestDataset[m_byDifficulty].byNeedLimitCount);


	CNtlSobItem* pItem = Logic_FindInventoryItemByTableId(itemTblidx);
	if (pItem)
	{
		m_NeedItem.ShowDisable(false);
	}
	else
	{
		m_NeedItem.ShowDisable(true);
	}
}


int CTMQBoardGui::SwitchDialog(bool bOpen)
{	
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		Show(false);		

		Logic_CancelNpcFacing();

		Clear();

		m_hNPCSerial = INVALID_SERIAL_ID;
		m_byDifficulty = INVALID_TIMEQUEST_DIFFICULTY;
		m_pTIMEQUEST_TBLDAT = NULL;
		m_pbtnDifficultyCombo->ClearAllItems();
		CheckInfoWindow();
	}
	
	return 1;
}

VOID CTMQBoardGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CTMQBoardGui::HandleEvents");

	if( msg.Id == g_EventNPCDialogOpen )
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>( msg.pData );

		if( pData->eDialog != DIALOG_TMQBOARD )
			return;

		// Same NPC check
		if( m_hNPCSerial == pData->hSerialId )
			return;

		// If you are having a conversation with another NPC
		if( m_hNPCSerial != INVALID_SERIAL_ID )
		{
			GetDialogManager()->CloseDialog(DIALOG_TMQBOARD);
			return;
		}

		// set TMQ info
		if (LoadTmqInfo(pData->hSerialId) == false)
		{
			Logic_CancelNpcFacing();
			return;
		}

		RwBool bOpenDialog = GetDialogManager()->OpenDialog(DIALOG_TMQBOARD, pData->hSerialId);

		if(!bOpenDialog)
		{
			Logic_CancelNpcFacing();
			return;
		}

		CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();

		if( pTMQ->GetNPCHandle() == INVALID_SERIAL_ID )
			pTMQ->SetNPCHandle(pData->hSerialId);

		m_hNPCSerial = pData->hSerialId;
	}
	else if( msg.Id == g_EventCharObjDelete )
	{
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>( msg.pData );

		if( m_hNPCSerial == *pDeleteSerial )
		{
			// NPC server is down or suddenly NPC disappears
			GetDialogManager()->CloseDialog(DIALOG_TMQBOARD);
		}
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pPacket = (SDboEventDialog*)msg.pData;
		if( pPacket->iType == DIALOGEVENT_OPEN_FAIL_NPC_DIALOG &&
			pPacket->iDestDialog == DIALOG_TMQBOARD )
		{
			// Failed to open TMQ bulletin board and resultcode from server
			Logic_CancelNpcFacing();

			m_hNPCSerial = INVALID_SERIAL_ID;
		}
	}
	else if (msg.Id == g_EventTMQEnterRes)
	{
		if (!IsShow())
			return;

		if (m_hNPCSerial == INVALID_SERIAL_ID)
			return;

		GetDialogManager()->CloseDialog(DIALOG_TMQBOARD);
	}

	NTL_RETURNVOID();
}