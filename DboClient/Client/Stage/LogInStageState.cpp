#include "precomp_dboclient.h"
#include "LogInStageState.h"

// core
#include "NtlDebug.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"
#include "MsgBoxManager.h"
#include "DboGlobal.h"

CLogInStageState* CLogInStageState::m_pLoginState = NULL;

CLogInStageState::CLogInStageState()
{
	m_eState = LOGIN_STATE_NONE;

	for(RwInt32 i = 0; i < (RwInt32)LOGIN_STATE_END; i++)
		m_fnUpdate[i] = NULL;

	m_fnUpdate[LOGIN_STATE_SERVER_CONNECT]		= &CLogInStageState::UpdateServerConnect;
	m_fnUpdate[LOGIN_STATE_LOGINREQ]			= &CLogInStageState::UpdateLoginReq;
	m_fnUpdate[LOGIN_STATE_CHAR_SERVER_LOGIN]	= &CLogInStageState::UpdateLoginReq;
	m_fnUpdate[LOGIN_STATE_CHAR_SERVER_CONNECT]	= &CLogInStageState::UpdateServerConnect;

	NTL_ASSERT(m_pLoginState == NULL, "CLogInStageState::CLogInStageState");

	m_pLoginState = this;

	m_tCHAR_SERVER_LOGIN.bDisconnectedAuthServer	= FALSE;
	m_tCHAR_SERVER_LOGIN.bConnectedCharServer		= FALSE;
	m_tCHAR_SERVER_LOGIN.bAskedLoginCharServer		= FALSE;

	m_fNotifyTime = 0.0f;	
	m_bNotifyShow = FALSE;

	LinkMsg(g_EventLoginMessage);
}

CLogInStageState::~CLogInStageState()
{
	if(m_hTimer.m_bActive)
		CEventTimer::RemoveTimer(m_hTimer);

	m_pLoginState = NULL;

	UnLinkMsg(g_EventLoginMessage);
}

CLogInStageState* CLogInStageState::GetInstance(void)
{
	return m_pLoginState;
}

void CLogInStageState::UpdateLoginReq(RwReal fElapsed)
{
	if(!m_hTimer.m_bActive)
		return;

	m_fNotifyTime += fElapsed;

	if(!m_bNotifyShow)
	{
		if(m_fNotifyTime >= 3.0f)
		{
			m_bNotifyShow = TRUE;
			RwReal fRemainTime = (RwReal)(LSS_TIMER_LOGIN_REQ/1000) - 3.0f;
			GetAlarmManager()->AlarmMessage( "DST_SERVER_CONNECTING", FALSE, NULL, fRemainTime );
		}
	}
}

void CLogInStageState::UpdateServerConnect(RwReal fElapsed)
{
	if(!m_hTimer.m_bActive)
		return;

	m_fNotifyTime += fElapsed;

	if(!m_bNotifyShow)
	{
		if(m_fNotifyTime >= 3.0f)
		{
			m_bNotifyShow = TRUE;
			RwReal fRemainTime = (RwReal)(LSS_TIME_SERVER_CONNECT/1000) - 3.0f;
			GetAlarmManager()->AlarmMessage( "DST_SERVER_CONNECTING", FALSE, NULL, fRemainTime );
		}
	}
}

void CLogInStageState::Update(RwReal fElapsed)
{
	if( m_tCHAR_SERVER_LOGIN.bDisconnectedAuthServer	&&
		m_tCHAR_SERVER_LOGIN.bConnectedCharServer		&&
		FALSE == m_tCHAR_SERVER_LOGIN.bAskedLoginCharServer	)
	{
		SConnectData*			pConData				= GetDboGlobal()->GetConnectData();  
		CLobbyPacketGenerator*	pLobbyPacketGenerator	= GetDboGlobal()->GetLobbyPacketGenerator(); 

		pLobbyPacketGenerator->SendCharLogInReq(pConData->uiAccountId, pConData->sCharCon.chAuthKey, pConData->byLastServerFarmID);

		m_tCHAR_SERVER_LOGIN.bAskedLoginCharServer = TRUE;
	}

	if(m_fnUpdate[m_eState])
	{
		(this->*m_fnUpdate[m_eState])(fElapsed);
	}
}

void CLogInStageState::EnterState(ELogInStageState eState)
{
#ifdef NET_SERVER_SYNC

	CreateTimeOutTimer(eState);

#endif

	m_eState = eState;

	switch(m_eState)
	{
		case LOGIN_STATE_SERVER_CONNECT:
		{
			CDboEventGenerator::LoginEvent(LOGIN_EVENT_FINISH_LOADING_AND_MOVIE);
		}
		break;
		case LOGIN_STATE_CHAR_SERVER_LOGIN:
		{
			m_tCHAR_SERVER_LOGIN.bConnectedCharServer = TRUE;
			break;
		}
	}

	CDboEventGenerator::LogInStageStateEnter( (RwUInt8 )eState);
}

void CLogInStageState::ExitState(ELogInStageState eState)
{
	m_fNotifyTime = 0.0f;	
	m_bNotifyShow = FALSE;

	if(m_hTimer.m_bActive)
		GetMsgBoxManager()->DeleteNetConnectBox();

	switch(eState)
	{
	case LOGIN_STATE_LOGO:
		break;
	case LOGIN_STATE_SERVER_CONNECT:
		if(m_hTimer.m_bActive)
			CEventTimer::RemoveTimer(m_hTimer);
		break;
	case LOGIN_STATE_SERVER_CONNECT_FAIL:
		break;
	case LOGIN_STATE_IDLE:
		break;
	case LOGIN_STATE_CONTRACT:
		break;
	case LOGIN_STATE_LOGINREQ:
		if(m_hTimer.m_bActive)
			CEventTimer::RemoveTimer(m_hTimer);
		break;
	case LOGIN_STATE_CHAR_SERVER_CONNECT:
		if(m_hTimer.m_bActive)
			CEventTimer::RemoveTimer(m_hTimer);
		break;
	case LOGIN_STATE_CHAR_SERVER_LOGIN:
		if(m_hTimer.m_bActive)
			CEventTimer::RemoveTimer(m_hTimer);
		break;
	case LOGIN_STATE_EXIT:
		if(m_hTimer.m_bActive)
			CEventTimer::RemoveTimer(m_hTimer);
		break;
	}

	CDboEventGenerator::LogInStageStateExit( (RwUInt8 )m_eState);
}


void CLogInStageState::ChangeState(ELogInStageState eState)
{
	ExitState(m_eState);

	EnterState(eState);
}

ELogInStageState CLogInStageState::GetCurrentState()
{
	return m_eState;
}

RwBool CLogInStageState::IsState(ELogInStageState eState)
{
	if( m_eState == eState )
		return TRUE;

	return FALSE;
}

void CLogInStageState::OnTimer(void)
{
	CEventTimer::RemoveTimer(m_hTimer);

	GetMsgBoxManager()->DeleteNetConnectBox();

	CDboEventGenerator::LogInStageTimeOut( (RwUInt8 )m_eState);
}

void CLogInStageState::CreateTimeOutTimer(ELogInStageState eState)
{
	switch(eState)
	{
	case LOGIN_STATE_LOGO:
		break;
	case LOGIN_STATE_SERVER_CONNECT:
		m_hTimer = CEventTimer::AddTimer(LSS_TIME_SERVER_CONNECT, this, &CLogInStageState::OnTimer); 
		break;
	case LOGIN_STATE_SERVER_CONNECT_FAIL:
		break;
	case LOGIN_STATE_CONTRACT:
		break;
	case LOGIN_STATE_IDLE:
		break;
	case LOGIN_STATE_LOGINREQ:
		m_hTimer = CEventTimer::AddTimer(LSS_TIMER_LOGIN_REQ, this, &CLogInStageState::OnTimer); 
		break;
	case LOGIN_STATE_CHAR_SERVER_CONNECT:
		m_hTimer = CEventTimer::AddTimer(LSS_TIME_SERVER_CONNECT, this, &CLogInStageState::OnTimer); 
		break;
	case LOGIN_STATE_CHAR_SERVER_LOGIN:
		m_hTimer = CEventTimer::AddTimer(LSS_TIMER_LOGIN_REQ, this, &CLogInStageState::OnTimer); 
		break;
	}
}

void CLogInStageState::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventLoginMessage)
	{
		SDboEventLoginMessage* pEvent = reinterpret_cast<SDboEventLoginMessage*>( pMsg.pData );

		if( LOGIN_EVENT_DISCONNECTED_AUTH_SERVER == pEvent->byMessage )
		{
			m_tCHAR_SERVER_LOGIN.bDisconnectedAuthServer = TRUE;
		}
	}
}