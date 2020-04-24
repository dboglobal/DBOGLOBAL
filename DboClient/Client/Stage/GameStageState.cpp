#include "precomp_dboclient.h"
#include "GameStageState.h"

// core
#include "NtlDebug.h"

// PL
#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "DboGlobal.h"
#include "AlarmManager.h"
#include "MsgBoxManager.h"


#define NP_CHECK_TIME		(0.5f)

CGameStageState* CGameStageState::m_pGameState = NULL;

CGameStageState::CGameStageState()
{
	m_eState = GAME_STATE_NONE;
	
	for(RwInt32 i = 0; i < (RwInt32)GAME_STATE_END; i++)
		m_fnUpdate[i] = NULL;
	
	m_fnUpdate[GAME_STATE_CHAR_SERVER_CONNECT]	= &CGameStageState::UpdateServerConnect;
	m_fnUpdate[GAME_STATE_CHAR_SERVER_LOGIN]	= &CGameStageState::UpdateServerConnect;
    m_fnUpdate[GAME_STATE_CHAR_EXIT]            = &CGameStageState::UpdateServerConnect;

	NTL_ASSERT(m_pGameState == NULL, "CGameStageState::CGameStageState");

	m_pGameState = this;

	m_fNotifyTime = 0.0f;	
	m_bNotifyShow = FALSE;

	m_fNPCheckTime = NP_CHECK_TIME;

	m_bNPCheck = FALSE;

	LinkMsg(g_EventLobbyServerConnect, 0);
}

CGameStageState::~CGameStageState()
{
	UnLinkMsg(g_EventLobbyServerConnect);

	if(m_hTimer.m_bActive)
		CEventTimer::RemoveTimer(m_hTimer);

	m_pGameState = NULL;
}

CGameStageState* CGameStageState::GetInstance(void)
{
	return m_pGameState;
}

void CGameStageState::Update(RwReal fElapsed)
{
	if(m_fnUpdate[m_eState])
	{
		(this->*m_fnUpdate[m_eState])(fElapsed);
	}

	m_fNPCheckTime -= fElapsed;

}

void CGameStageState::ChangeState(EGameStageState eState)
{
	ExitState(m_eState);

	EnterState(eState);
}

EGameStageState CGameStageState::GetCurrentState()
{
	return m_eState;
}

RwBool CGameStageState::IsState(EGameStageState eState)
{
	if( m_eState == eState )
		return TRUE;

	return FALSE;
}

void CGameStageState::SetNPCheck(RwBool bCheck)
{
	m_bNPCheck = bCheck;
}

void CGameStageState::OnTimer(void)
{
	CEventTimer::RemoveTimer(m_hTimer);
	GetMsgBoxManager()->DeleteNetConnectBox();
}

void CGameStageState::CreateTimeOutTimer(EGameStageState eState)
{
	switch(eState)
	{
	case GAME_STATE_CHAR_SERVER_CONNECT:
        if(m_hTimer.m_bActive)
        {
            CEventTimer::RemoveTimer(m_hTimer);
        }

		m_hTimer = CEventTimer::AddTimer(GSS_TIME_SERVER_CONNECT, this, &CGameStageState::OnTimer); 
		break;
	}
}

void CGameStageState::EnterState(EGameStageState eState)
{

#ifdef NET_SERVER_SYNC

	CreateTimeOutTimer(eState);

#endif

	m_eState = eState;

	if(m_eState == GAME_STATE_CHAR_SERVER_CONNECT)
	{
		CharServerConnectEnter();
	}
	else if(m_eState == GAME_STATE_CHAR_SERVER_LOGIN)
	{
		CharServerLoginEnter();
	}
}

void CGameStageState::CharServerConnectEnter(void)
{
#ifdef NET_SERVER_SYNC

	SConnectData *pConData = GetDboGlobal()->GetConnectData();  
	CNtlClientNet *pNet = GetDboGlobal()->GetNetwork(); 

	SCharServerInfo sCharServerInfo;
	RwBool bExist = pConData->PopCharacterServer(&sCharServerInfo);
	if(bExist)
		pNet->Connect( (char*)sCharServerInfo.chServerIP, sCharServerInfo.wServerPort, &pConData->sCharCon.hSocket, true, NETCLIENT_ENCODE_NONE);
#endif
}

void CGameStageState::CharServerLoginEnter(void)
{
	SConnectData *pConData = GetDboGlobal()->GetConnectData();  
	CLobbyPacketGenerator *pLobbyPacketGenerator = GetDboGlobal()->GetLobbyPacketGenerator(); 
	pLobbyPacketGenerator->SendCharLogInReq(pConData->uiAccountId, pConData->sCharCon.chAuthKey, pConData->byLastServerFarmID);
}

void CGameStageState::ExitState(EGameStageState eState)
{
    GetMsgBoxManager()->DeleteNetConnectBox();

	m_fNotifyTime = 0.0f;	
	m_bNotifyShow = FALSE;

	if(m_hTimer.m_bActive)
		GetMsgBoxManager()->DeleteNetConnectBox();

	switch(eState)
	{
	case GAME_STATE_CHAR_SERVER_CONNECT:
		if(m_hTimer.m_bActive)
			CEventTimer::RemoveTimer(m_hTimer);
		break;

	case GAME_STATE_CHAR_SERVER_LOGIN:
	{
		if (m_hTimer.m_bActive)
			CEventTimer::RemoveTimer(m_hTimer);
	}
	break;

	}
}

void CGameStageState::UpdateServerConnect(RwReal fElapsed)
{
	m_fNotifyTime += fElapsed;

	if(!m_bNotifyShow)
	{
		if(m_fNotifyTime >= dDELAY_BEFORE_VISIBLE_MESSAGEBOX)
		{
			m_bNotifyShow = TRUE;
			RwReal fRemainTime = (RwReal)(GSS_TIME_SERVER_CONNECT/1000) - dDELAY_BEFORE_VISIBLE_MESSAGEBOX;
			GetAlarmManager()->AlarmMessage( "DST_SERVER_CONNECTING", FALSE, NULL, fRemainTime );
         //   CNtlPLEventGenerator::CinematicFadeIn(CINEMATIC_FADEINOUT_SCREEN_PANEL, 0.1f, 0, 0, 0, 0);
		}
	}
    else if(m_fNotifyTime * 1000.0f >= GSS_TIME_SERVER_CONNECT)
    {
		if( GetMsgBoxManager()->GetMsgBox("DST_LOBBY_NO_RELPY_SERVER") )
			return;

        // When the waiting time (30 seconds) is over, clear the existing message box and jump the message box to end the game.
        GetMsgBoxManager()->DeleteNetConnectBox();
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
    }
}

void CGameStageState::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventLobbyServerConnect)
	{
		SDboEventServerConnect* pEvent = reinterpret_cast<SDboEventServerConnect*>( pMsg.pData );

		if( pEvent->byConnectType == SERVER_CONNECT_TYPE_CONNENCT_SUCCESS )
			ChangeState(GAME_STATE_CHAR_SERVER_LOGIN);
	}
}