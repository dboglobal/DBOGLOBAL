#include "precomp_dboclient.h"
#include "CharAccountGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLDef.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DisplayStringManager.h"
#include "CharStageState.h"
#include "LobbyManager.h"


CAccountGui::CAccountGui(const RwChar* pName)
	:CNtlPLGui(pName)
{

}

CAccountGui::~CAccountGui()
{

}

RwBool CAccountGui::Create()
{
	NTL_FUNCTION("CAccountGui::Create");

	if (!CNtlPLGui::Create("", "gui\\CharSelect_Account.srf", "gui\\CharSelect_Account.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// 배경
	m_Background.SetType(CWindowby3::WT_HORIZONTAL);
	m_Background.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CharSelect_Account.srf", "srfBackgroundUp"));
	m_Background.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CharSelect_Account.srf", "srfBackgroundCenter"));
	m_Background.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CharSelect_Account.srf", "srfBackgroundBottom"));
	m_Background.SetSize(233, 172);

	// 계정 정보 배경
	m_srtAccountBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CharSelect_Account.srf", "srfBar"));

	// 'title box'
	rect.SetRect(52, 6, 172, 26);
	m_pTitleStatic = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pTitleStatic->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pTitleStatic->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_INFO"));
	m_pTitleStatic->Enable(false);

	// '서버 이름'
	rect.SetRect(30, 70, 130, 84);
	m_pServerNameStatic = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pServerNameStatic->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pServerNameStatic->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_NAME"));
	m_pServerNameStatic->Enable(false);

	// '서버 상태'
	rect.SetRect(30, 90, 130, 104);
	m_pServerStateStatic = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pServerStateStatic->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pServerStateStatic->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_STATE"));
	m_pServerStateStatic->Enable(false);

	// 서버 이름
	rect.SetRect(90, 70, 210, 84);
	m_pServerName = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pServerName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pServerName->Enable(false);

	// 서버 상태
	rect.SetRect(90, 90, 210, 104);
	m_pServerState = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pServerState->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pServerState->Enable(false);

	// 서버 선택 버튼
	m_pServerSelectButton = (gui::CButton*)GetComponent("btnServerSelect");
	m_pServerSelectButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pServerSelectButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pServerSelectButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_CHOICE_SERVER"));
	m_slotSelectServer = m_pServerSelectButton->SigClicked().Connect(this, &CAccountGui::OnClicked_ServerSelectButton);

	// sig
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CAccountGui::OnPaint);

	LinkMsg(g_EventCharStageStateEnter);
	LinkMsg(g_EventLobbyMessage);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CAccountGui::Destroy()
{
	NTL_FUNCTION("CAccountGui::Destroy");

	UnLinkMsg(g_EventCharStageStateEnter);
	UnLinkMsg(g_EventLobbyMessage);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CAccountGui::SetPosition(RwInt32 iXPos, RwInt32 iYPos)
{
	m_pThis->SetPosition(iXPos, iYPos);

	m_Background.SetPosition(iXPos, iYPos);
	m_srtAccountBack.SetPosition(iXPos + 11, iYPos + 53);
}

VOID CAccountGui::SetServerInfo()
{
	SERVER_HANDLE	hServer = GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby = GetLobbyManager()->GetLobby(hServer);
	if (!pLobby)
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	m_pServerName->Format(L"%s", pLobby->GetServerName());
}

VOID CAccountGui::SetServerState(RwUInt8 byState, RwUInt8 byRate)
{
	switch (byState)
	{
	case DBO_SERVER_STATUS_UP:
	{
		if (byRate < 50)
		{
			m_pServerState->SetTextColor(RGB(94, 255, 89), TRUE);
			m_pServerState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_NOT_BUSY"));
		}
		else if (byRate < 80)
		{
			m_pServerState->SetTextColor(RGB(94, 255, 89), TRUE);
			m_pServerState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_NORMAL"));
		}
		else if (byRate < 100)
		{
			m_pServerState->SetTextColor(RGB(94, 255, 89), TRUE);
			m_pServerState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_BUSY"));
		}
		else if (byRate >= 100)
		{
			m_pServerState->SetTextColor(RGB(94, 255, 89), TRUE);
			m_pServerState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_EXCESS"));
		}

		break;
	}
	case DBO_SERVER_STATUS_DOWN:
	{
		m_pServerState->SetTextColor(RGB(212, 15, 45), TRUE);
		m_pServerState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_CHECK"));

		break;
	}
	case DBO_SERVER_STATUS_LOCKED:
	{
		m_pServerState->SetTextColor(RGB(23, 125, 212), TRUE);
		m_pServerState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_LOCK"));

		break;
	}
	default:
	{
		DBO_FAIL("Not exist server status : " << byState);
		break;
	}
	}
}

VOID CAccountGui::EnableButtons(bool bEnable)
{
	m_pServerSelectButton->ClickEnable(bEnable);
}

VOID CAccountGui::OnClicked_ServerSelectButton(gui::CComponent* pComponent)
{
#ifdef NET_SERVER_SYNC

	if (GetCharStageState()->GetCurrentState() != CHAR_STATE_SELECT_IDLE)
		return;

	if (GetLobbyManager()->IsWaitingServerListInfo())
	{
		// 아직 서버 리스트를 받지 않았다면
		GetDboGlobal()->GetLobbyPacketGenerator()->SendCharServerListReq();
	}
	else
	{
		// 서버 셀렉트 화면으로 전환
		GetCharStageState()->ReservateState(CHAR_STATE_SERVER_ENTER);
		GetCharStageState()->ChangeState(CHAR_STATE_SELECT_EXIT);
	}

#endif
}

VOID CAccountGui::OnPaint()
{
	m_Background.Render();
	m_srtAccountBack.Render();
}

VOID CAccountGui::HandleEvents(RWS::CMsg &msg)
{
	NTL_FUNCTION("CAccountGui::HandleEvents");

	if (msg.Id == g_EventCharStageStateEnter)
	{
		SDboEventCharStageStateEnter* pEvent = reinterpret_cast<SDboEventCharStageStateEnter*>(msg.pData);

		if (pEvent->byState == CHAR_STATE_SELECT_IDLE)
		{
			EnableButtons(true);
		}
		else
		{
			EnableButtons(false);

			if (pEvent->byState == CHAR_STATE_SELECT_ENTER)
				SetServerInfo();
		}
	}
	else if (msg.Id == g_EventLobbyMessage)
	{
		SDboEventLobbyMessage* pEvent = reinterpret_cast<SDboEventLobbyMessage*>(msg.pData);

		// disabld by daneos because it seems to be useless to set channel-status because we only need server status..
		/*if (pEvent->byMessage == LMT_SELECT_CHANNEL)
		{
			SetServerInfo();

			SERVER_HANDLE	hServer = GetLobbyManager()->GetSelectedServerHandle();
			CLobby*			pLobby = GetLobbyManager()->GetLobby(hServer);
			if (!pLobby)
			{
				DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
				NTL_RETURNVOID();
			}

			RwUInt8 byIndex = (RwUInt8)pEvent->fValue;

			if (Logic_IsUsableIndex(byIndex, NTL_MAX_SERVER_CHANNEL_COUNT_IN_SERVER_FARM, INVALID_BYTE))
			{
				sDBO_GAME_SERVER_CHANNEL_INFO* pDBO_GAME_SERVER_CHANNEL_INFO = pLobby->GetChannel(byIndex);
				if (!pDBO_GAME_SERVER_CHANNEL_INFO)
				{
					DBO_FAIL("Not exist channel infomation of index : " << byIndex);
					NTL_RETURNVOID();
				}

				SetServerState(pDBO_GAME_SERVER_CHANNEL_INFO->byServerStatus,
					(RwUInt8)pDBO_GAME_SERVER_CHANNEL_INFO->dwLoad);
			}
		}
		else if (pEvent->byMessage == LMT_REFRESH_CHANEL)
		{
			SERVER_HANDLE	hServer = GetLobbyManager()->GetSelectedServerHandle();
			CLobby*			pLobby = GetLobbyManager()->GetLobby(hServer);
			if (!pLobby)
			{
				DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
				NTL_RETURNVOID();
			}

			for (RwUInt8 i = 0; i < NTL_MAX_SERVER_CHANNEL_COUNT_IN_SERVER_FARM; ++i)
			{
				sDBO_GAME_SERVER_CHANNEL_INFO* pDBO_GAME_SERVER_CHANNEL_INFO = pLobby->GetChannel(i);
				if (!pDBO_GAME_SERVER_CHANNEL_INFO)
				{
					DBO_FAIL("Not exist channel infomation of index : " << i);
					continue;
				}

				if (pDBO_GAME_SERVER_CHANNEL_INFO->byServerChannelIndex == pEvent->uiExData)
				{
					SetServerState(pDBO_GAME_SERVER_CHANNEL_INFO->byServerStatus, (RwUInt8)pDBO_GAME_SERVER_CHANNEL_INFO->dwLoad);
					break;
				}
			}
		}
		else*/ 
		if (pEvent->byMessage == LMT_UPDATE_SERVER || pEvent->byMessage == LMT_REFRESH_SERVER)
		{
			SERVER_HANDLE	hServer = GetLobbyManager()->GetSelectedServerHandle();
			CLobby*			pLobby = GetLobbyManager()->GetLobby(hServer);
			if (!pLobby)
			{
				DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
				NTL_RETURNVOID();
			}

			SetServerState(pLobby->GetServerStatus(), pLobby->GetServerUseRate());
		}
	}

	NTL_RETURNVOID();
}