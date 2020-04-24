#include "precomp_dboclient.h"
#include "CharSelectGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// sound
#include "NtlSoundManager.h"
#include "GUISoundDefine.h"

// simulation
#include "NtlSLDef.h"

#include "NtlStorageManager.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "CharStageState.h"
#include "DboGlobal.h"
#include "DisplayStringManager.h"
#include "CharSelecterGui.h"
#include "ChannelSelectGui.h"
#include "CharInfoGui.h"
#include "CharAccountGui.h"
#include "AlarmManager.h"
#include "LobbyManager.h"

namespace
{
#define	dEDGE_ADJUST					4
#define dCHANNEL_HEIGHT_BUTTON_GAP		23
#define dCHANNEL_HEIGHT_GAP				23

#define dCHANNEL_BUTTON_X				26
#define dCHANNEL_BUTTON_START_Y			107
#define dCHANNEL_TEXT__CHANNEL_NUM_X	32
#define dCHANNEL_TEXT__CHANNEL_STATE_X	120
#define dCHANNEL_TEXT_START_Y			112
#define dCHANNEL_GAUGE_X				85
#define dCHANNEL_GAUGE_START_Y			115
#define dCHANNEL_GAUGE_BACK_X			85
#define dCHANNEL_GAUGE_BACK_START_Y		115

#define dMAX_VISIBLE_COUNT		9
};


CCharSelectGui::CCharSelectGui(const RwChar* pName)
	:CNtlPLGui(pName)
	, m_byRotationState(ER_STOP)
{
}

CCharSelectGui::~CCharSelectGui()
{

}

RwBool CCharSelectGui::Create()
{
	NTL_FUNCTION("CCharSelectGui::Create");

	if (!CNtlPLGui::Create("", "gui\\CharSelect.srf", "gui\\CharSelect.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// Left rotate button
	m_pLeftRotButton = (gui::CButton*)GetComponent("LeftRotButton");
	m_pLeftRotButton->SetClickSound(GSD_SYSTEM_CHARACTER_ROTATE);
	m_slotPressLeftButton = m_pLeftRotButton->SigPressed().Connect(this, &CCharSelectGui::OnPress_LeftTurnButton);
	m_slotReleaseLeftButton = m_pLeftRotButton->SigReleased().Connect(this, &CCharSelectGui::OnRelease_TurnButton);

	// Right rotate button
	m_pRightRotButton = (gui::CButton*)GetComponent("RightRotButton");
	m_pRightRotButton->SetClickSound(GSD_SYSTEM_CHARACTER_ROTATE);
	m_slotPressRightButton = m_pRightRotButton->SigPressed().Connect(this, &CCharSelectGui::OnPress_RightTurnButton);
	m_slotReleaseRightButton = m_pRightRotButton->SigReleased().Connect(this, &CCharSelectGui::OnRelease_TurnButton);

	// Start game button
	m_pGameStartButton = (gui::CButton*)GetComponent("btnGameStart");
	m_pGameStartButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pGameStartButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pGameStartButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_GAME_START"));
	m_slotGameStartButton = m_pGameStartButton->SigClicked().Connect(this, &CCharSelectGui::OnClickedGameStartButton);

	// Exit button
	m_pExitButton = (gui::CButton*)GetComponent("btnExit");
	m_pExitButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pExitButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pExitButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_EXIT"));
	m_slotExitButton = m_pExitButton->SigClicked().Connect(this, &CCharSelectGui::OnClickedExitButton);

	// Account information GUI
	m_pAccountGui = NTL_NEW CAccountGui("AccountGui");
	m_pAccountGui->Create();
	m_pAccountGui->SetPosition(dEDGE_ADJUST, 48);

	// Character information GUI
	m_pCharacterInfoGui = NTL_NEW CCharacterInfoGui("CharacterInfoGui");
	m_pCharacterInfoGui->Create();
	m_pCharacterInfoGui->SetPosition(dEDGE_ADJUST, 238);

	// Character Selector GUI
	m_pCharSelecterGui = NTL_NEW CCharSelecterGui("CharacterInfoGui");
	m_pCharSelecterGui->Create();

	// Channel Select GUI
	m_pChannelSelectGui = NTL_NEW CChannelSelectGui("ChannelSelectGui");
	m_pChannelSelectGui->Create();

	LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

	// default value
	m_tMouseRol.bPressLeft = FALSE;
	m_tMouseRol.iOldX = 0;

	// sig	
	m_slotMouseDown = m_pThis->SigMouseDown().Connect(this, &CCharSelectGui::OnMouseDown);
	m_slotMouseUp = m_pThis->SigMouseUp().Connect(this, &CCharSelectGui::OnMouseUp);
	m_slotMouseMove = m_pThis->SigMouseMove().Connect(this, &CCharSelectGui::OnMouseMove);
	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CCharSelectGui::OnCaptureWheelMove);
	m_slotCaptureMouseUp = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseUp().Connect(this, &CCharSelectGui::OnCaptureMouseUp);

	LinkMsg(g_EventCharStageStateEnter);
	LinkMsg(g_EventCharStageStateExit);
	LinkMsg(g_EventResize);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CCharSelectGui::Destroy()
{
	NTL_FUNCTION("CCharSelectGui::Destroy");

	if (m_pAccountGui)
	{
		m_pAccountGui->Destroy();
		NTL_DELETE(m_pAccountGui);
	}

	if (m_pCharacterInfoGui)
	{
		m_pCharacterInfoGui->Destroy();
		NTL_DELETE(m_pCharacterInfoGui);
	}

	if (m_pCharSelecterGui)
	{
		m_pCharSelecterGui->Destroy();
		NTL_DELETE(m_pCharSelecterGui);
	}

	if (m_pChannelSelectGui)
	{
		m_pChannelSelectGui->Destroy();
		NTL_DELETE(m_pChannelSelectGui);
	}

	UnLinkMsg(g_EventCharStageStateEnter);
	UnLinkMsg(g_EventCharStageStateExit);
	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CCharSelectGui::SwitchDialog(bool bShow)
{
	m_pAccountGui->Show(bShow);
	m_pCharacterInfoGui->Show(bShow);
	m_pCharSelecterGui->Show(bShow);

	Show(bShow);
}

RwUInt8 CCharSelectGui::GetSelectedChannelIndex()
{
	if (!m_pChannelSelectGui)
		return INVALID_BYTE;

	return m_pChannelSelectGui->GetSelectedChannelIndex();
}

VOID CCharSelectGui::EnableButtons(bool bEnable)
{
	m_pLeftRotButton->ClickEnable(bEnable);
	m_pRightRotButton->ClickEnable(bEnable);
	m_pGameStartButton->ClickEnable(bEnable);
	m_pExitButton->ClickEnable(bEnable);
}

VOID CCharSelectGui::LocateComponent(RwInt32 iWidth, RwInt32 iHeight)
{
	m_pThis->SetPosition(CRectangle(0, 0, iWidth, iHeight));

	m_pCharSelecterGui->SetPosition(iWidth - m_pCharSelecterGui->GetWidth() - dEDGE_ADJUST, 48);
	m_pChannelSelectGui->SetPosition((iWidth - m_pCharSelecterGui->GetWidth()) / 2,
		(iHeight - m_pCharSelecterGui->GetHeight()) / 2);

	m_pGameStartButton->SetPosition((iWidth - m_pGameStartButton->GetWidth()) / 2, iHeight - 75);
	m_pExitButton->SetPosition(iWidth - 185, iHeight - 70);

	RwInt32 iButtonCenterX = iWidth / 2;
	m_pLeftRotButton->SetPosition(iButtonCenterX - 206, iHeight - 194);
	m_pRightRotButton->SetPosition(iButtonCenterX + 206 - m_pRightRotButton->GetWidth(), iHeight - 194);
}

VOID CCharSelectGui::OnPress_LeftTurnButton(gui::CComponent* pComponent)
{
	if (GetCharStageState()->GetCurrentState() != CHAR_STATE_SELECT_IDLE)
		return;

	m_byRotationState = ER_TURN_LEFT;
	CDboEventGenerator::LobbyEvent(LMT_ROTATION_LEFT, 100.0f);
}

VOID CCharSelectGui::OnPress_RightTurnButton(gui::CComponent* pComponent)
{
	if (GetCharStageState()->GetCurrentState() != CHAR_STATE_SELECT_IDLE)
		return;

	m_byRotationState = ER_TURN_RIGHT;
	CDboEventGenerator::LobbyEvent(LMT_ROTATION_RIGHT, 100.0f);
}

VOID CCharSelectGui::OnRelease_TurnButton(gui::CComponent* pComponent)
{
	if (GetCharStageState()->GetCurrentState() != CHAR_STATE_SELECT_IDLE)
		return;

	m_byRotationState = ER_STOP;
	CDboEventGenerator::LobbyEvent(LMT_ROTATION_STOP);
}

VOID CCharSelectGui::OnClickedGameStartButton(gui::CComponent* pComponent)
{
	if (GetCharStageState()->GetCurrentState() != CHAR_STATE_SELECT_IDLE)
		return;


	SERVER_HANDLE	hServer = GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby = GetLobbyManager()->GetLobby(hServer);
	if (!pLobby)
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	RwUInt8 byCharIndex = pLobby->GetSelectedCharacterIndex();

	if (FALSE == Logic_IsUsableIndex(byCharIndex, NTL_MAX_COUNT_USER_CHAR_SLOT, INVALID_BYTE) ||
		pLobby->GetCharacterCount() == 0)
	{
		// 캐릭터를 선택하십시요
		GetAlarmManager()->AlarmMessage("DST_LOBBY_MUST_CHOICE_CHAR");
		return;
	}

	sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter(byCharIndex);
	if (!pLOBBY_CHARACTER)
	{
		DBO_FAIL("Not exist character of index : " << byCharIndex);
		return;
	}

	if (pLOBBY_CHARACTER->bReservatedDelete)
	{
		GetAlarmManager()->AlarmMessage("DST_LOBBY_RESERVATED_CHAR");
		return;
	}

	if (pLOBBY_CHARACTER->tSummary.bNeedNameChange)
	{
		GetAlarmManager()->AlarmMessage("DST_LOBBY_CAN_NOT_USE_CHAR_UNTIL_RENAME");
		return;
	}

	// Open channel dialog
	m_pChannelSelectGui->Show(true);
	m_pChannelSelectGui->GetDialog()->Popup(true);
}

VOID CCharSelectGui::OnClickedExitButton(gui::CComponent* pComponent)
{
	// I go back to the lobby.
	GetDboGlobal()->GetLobbyPacketGenerator()->SendCharDisconnectReq(false);
	GetCharStageState()->ReservateState(CHAR_STATE_RETURN_LOGIN);
	GetCharStageState()->ChangeState(CHAR_STATE_SELECT_EXIT);
}

VOID CCharSelectGui::OnMouseDown(const CKey& key)
{
	if (key.m_nID != UD_LEFT_BUTTON)
		return;

	m_tMouseRol.bPressLeft = TRUE;
	m_tMouseRol.iOldX = (RwInt32)key.m_fX;
}

VOID CCharSelectGui::OnMouseUp(const CKey& key)
{
	if (key.m_nID != UD_LEFT_BUTTON)
		return;

	m_tMouseRol.bPressLeft = FALSE;
}

VOID CCharSelectGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if (!m_tMouseRol.bPressLeft)
		return;

	if (m_tMouseRol.iOldX == nX)
		return;

	RwReal fDeltaAngle = (RwReal)(nX - m_tMouseRol.iOldX) * 1.5f;
	CDboEventGenerator::LobbyEvent(LMT_ROTATION, fDeltaAngle);

	m_tMouseRol.iOldX = nX;
}

VOID CCharSelectGui::OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
	if (!IsShow())
		return;

	if (sDelta < 0)
		CDboEventGenerator::LobbyEvent(LMT_ZOOM_OUT);
	else
		CDboEventGenerator::LobbyEvent(LMT_ZOOM_IN);
}

VOID CCharSelectGui::OnCaptureMouseUp(const CKey& key)
{
	if (key.m_nID != UD_LEFT_BUTTON)
		return;

	m_tMouseRol.bPressLeft = FALSE;
}

VOID CCharSelectGui::HandleEvents(RWS::CMsg &msg)
{
	NTL_FUNCTION("CCharSelectGui::HandleEvents");

	if (msg.Id == g_EventCharStageStateEnter)
	{
		SDboEventCharStageStateEnter* pEvent = reinterpret_cast<SDboEventCharStageStateEnter*>(msg.pData);

		if (pEvent->byState == CHAR_STATE_SELECT_IDLE)
			EnableButtons(true);
		else
			EnableButtons(false);

		if (pEvent->byState == CHAR_STATE_SELECT_ENTER)
		{
			SERVER_HANDLE	hServer = GetLobbyManager()->GetSelectedServerHandle();
			CLobby*			pLobby = GetLobbyManager()->GetLobby(hServer);
			if (!pLobby)
			{
				DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
				NTL_RETURNVOID();
			}

			// 캐릭터 모델 바꾸기			
			if (pLobby->GetCharacterCount() > 0)
			{
				if (pLobby->GetSelectedCharacterIndex() != INVALID_BYTE)
				{
					// 현재 서버에서 선택된 캐릭터가 있다
					CDboEventGenerator::LobbyEvent(LMT_SELECT_CHARACTER, (RwReal)pLobby->GetSelectedCharacterIndex());
				}
				else if (!pLobby->IsLoadedLastCharacter())
				{
					// 마지막으로 접속했던 캐릭터 찾기
					std::string strFinalCharName = GetNtlStorageManager()->GetStringData(dSTORAGE_ACCOUNT_LASTCHAR);
					WCHAR awcBuffer[NTL_MAX_SIZE_CHAR_NAME + 1];
					::MultiByteToWideChar(GetACP(), 0, strFinalCharName.c_str(), -1, awcBuffer, NTL_MAX_SIZE_CHAR_NAME + 1);

					for (RwUInt8 i = 0; i < pLobby->GetCharacterCount(); ++i)
					{
						sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter(i);
						if (!pLOBBY_CHARACTER)
							continue;

						if (wcscmp(pLOBBY_CHARACTER->tSummary.awchName, awcBuffer) == 0)
						{
							pLobby->SetLoadedLastCharacter(TRUE);
							pLobby->SetSelectedCharacterIndex(i);
							CDboEventGenerator::LobbyEvent(LMT_SELECT_CHARACTER, i);
							break;
						}
					}

					if (!pLobby->IsLoadedLastCharacter())
					{
						// 마지막으로 접속했던 캐릭터의 정보를 제대로 찾지 못했다
						pLobby->SetSelectedCharacterIndex(0);
						CDboEventGenerator::LobbyEvent(LMT_SELECT_CHARACTER, 0);
					}
				}
				else
				{
					// 마지막으로 접속했던 캐릭터의 정보가 없다
					pLobby->SetSelectedCharacterIndex(0);
					CDboEventGenerator::LobbyEvent(LMT_SELECT_CHARACTER, 0);
				}
			}
			else
			{
				// 캐릭터가 없다
				pLobby->SetSelectedCharacterIndex(INVALID_BYTE);
				CDboEventGenerator::LobbyEvent(LMT_SELECT_CHARACTER, (RwReal)INVALID_BYTE);
			}

			SwitchDialog(true);
		}
	}
	else if (msg.Id == g_EventCharStageStateExit)
	{
		SDboEventCharStageStateExit* pEvent = reinterpret_cast<SDboEventCharStageStateExit*>(msg.pData);
		if (pEvent->byState == CHAR_STATE_SELECT_EXIT)
		{
			ECharacterStageState eReservateState = GetCharStageState()->GetReservateState();

			if (eReservateState == CHAR_STATE_SUCCESS_ENTER_GAME ||
				eReservateState == CHAR_STATE_RETURN_LOGIN ||
				eReservateState == CHAR_STATE_IMMEDIATLY_RETURN_LOGIN_ENTER)
			{
				CDboEventGenerator::LobbyEvent(LMT_DESTORY_LOBBY_WORLD);
			}
			else
				SwitchDialog(false);

			m_byRotationState = ER_STOP;
			CDboEventGenerator::LobbyEvent(LMT_ROTATION_FOREWARD);
		}
	}
	else if (msg.Id == g_EventResize)
	{
		SNtlPLEventResize* pEvent = reinterpret_cast<SNtlPLEventResize*>(msg.pData);
		LocateComponent(pEvent->iWidht, pEvent->iHeight);
	}

	NTL_RETURNVOID();
}