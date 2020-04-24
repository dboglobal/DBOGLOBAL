#include "precomp_dboclient.h"
#include "LogInStage.h"

// shared
#include "NtlResultCode.h"

// network
#include "NtlClientNet.h"

// core
#include "NtlDebug.h"
#include "NtlEventQueue.h"
#include "NtlSystemInfo.h"

// simulation
#include "NtlNetSender.h"
#include "InputActionMap.h"
#include "NtlStorageManager.h"

// dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "LogInGuiGroup.h"
#include "LogInStageState.h"
#include "AlarmManager.h"

#include "DboApplication.h"
#include "MoviePlayer.h"

// discord
#ifdef USE_DISCORD
#include "Discord.h"
#endif




CLogInStage::CLogInStage(const char *pStageName)
:CNtlStage(pStageName),
m_bAutoLogIn(FALSE),
m_pState(NULL),
m_pGuiGroup(NULL)
{
	
}

CLogInStage::~CLogInStage()
{
}

bool CLogInStage::Create(void)
{
	NTL_FUNCTION("CLogInStage::Create" );

	// event 등록.
	LinkMsg(g_EventUpdateTick, 0);
	LinkMsg(g_EventLogInServerConnect, 0);
	LinkMsg(g_EventLobbyServerConnect, 0);
	LinkMsg(g_EventLogInStageStateEnter, 0);
	LinkMsg(g_EventLogInStageStateExit, 0);
	LinkMsg(g_EventLogInStageTimeOut, 0);
	LinkMsg(g_EventLoginMessage, 0);	

	m_pState	= NTL_NEW CLogInStageState;	

	m_pGuiGroup = NTL_NEW CLogInGuiGroup;
	if(!m_pGuiGroup->Create())
	{
		NTL_DELETE(m_pGuiGroup);
		NTL_RETURN(FALSE);
	}

	// avooo's commnet: The logo is visible only when the application is first run.
	// When I get back to the login screen during the game, I see the logo again.
	static RwBool bFirstRunLogo = TRUE;
	if( bFirstRunLogo )
	{
		GetLogInStageState()->ChangeState(LOGIN_STATE_SERVER_CONNECT);
		//GetLogInStageState()->ChangeState(LOGIN_STATE_LOGO);

#ifndef NDEBUG
		RwBool bWindowMode = GetNtlStorageManager()->GetBoolData( dSTORAGE_GRAPHIC_WINDOW_MODE );
		RwInt32 nScreenWidth = GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH );
		RwInt32 nScreenHeight = GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT );
		GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_WINDOW_MODE, B2b(!NTL_DEBUG_WINDOW_MODE) );
		GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH, DEFAULT_SCREEN_WIDTH );
		GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT, DEFAULT_SCREEN_HEIGHT );
		GetNtlStorageManager()->ApplyStorageType( eNTL_STORAGE_GRAPHIC, eNTL_STORAGE_APPLY_ALL );
		GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_WINDOW_MODE, B2b(bWindowMode) );
		GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH, nScreenWidth );
		GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT, nScreenHeight );
#else
		GetNtlStorageManager()->ApplyStorageType( eNTL_STORAGE_GRAPHIC, eNTL_STORAGE_APPLY_ALL );		
#endif

		GetNtlStorageManager()->ApplyStorageType( eNTL_STORAGE_SOUND, eNTL_STORAGE_APPLY_ALL );

		bFirstRunLogo = !bFirstRunLogo;
	}
	else
	{
		GetLogInStageState()->ChangeState(LOGIN_STATE_SERVER_CONNECT);
	}

	GetInputActionMap()->LinkGameExit(this, &CLogInStage::ActionGameExit);
	
	NTL_RETURN(TRUE);
}

void CLogInStage::Destroy(void)
{
	NTL_FUNCTION("CLogInStage::Destroy" );

	CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_NONE);

	// event 해제.
	UnLinkMsg(g_EventUpdateTick);
	UnLinkMsg(g_EventLogInServerConnect);
	UnLinkMsg(g_EventLobbyServerConnect);
	UnLinkMsg(g_EventLogInStageStateEnter);
	UnLinkMsg(g_EventLogInStageStateExit);
	UnLinkMsg(g_EventLogInStageTimeOut);
	UnLinkMsg(g_EventLoginMessage);

	if(m_pGuiGroup)
	{
		m_pGuiGroup->Destroy();
		NTL_DELETE(m_pGuiGroup);
	}

	if(m_pState)
	{
		NTL_DELETE(m_pState);
	}

	GetInputActionMap()->UnLinkGameExit();

	NTL_RETURNVOID();
}

void CLogInStage::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventUpdateTick)
	{
		float fElapsed = *reinterpret_cast<float*>(pMsg.pData);
		EventProcUpdateTick(fElapsed);
	}
	else if(pMsg.Id == g_EventLogInServerConnect)
	{
		LogInServerConnect(pMsg);
	}
	else if(pMsg.Id == g_EventLobbyServerConnect)
	{
		LobbyServerConnect(pMsg);
	}
	else if(pMsg.Id == g_EventLogInStageStateEnter)
	{
		LoginStageStateEnter(pMsg);
	}
	else if(pMsg.Id == g_EventLogInStageStateExit)
	{
		LoginStageStateExit(pMsg);
	}
	else if(pMsg.Id == g_EventLogInStageTimeOut)
	{
		LogInStageTimeOutEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventLoginMessage)
	{
		LogInMessageEventHandler(pMsg);
	}
}

void CLogInStage::EventProcUpdateTick(RwReal fElapsed)
{
	GetLogInStageState()->Update(fElapsed);
}

void CLogInStage::LogInServerConnect(RWS::CMsg &pMsg)
{
	SDboEventServerConnect *pConnect = reinterpret_cast<SDboEventServerConnect*>( pMsg.pData );

	if(pConnect->byConnectType == SERVER_CONNECT_TYPE_CONNENCT_SUCCESS)
	{
#ifdef USE_DISCORD
		GetDiscordManager()->LoginAuthServer();
#endif

		GetLogInStageState()->ChangeState(LOGIN_STATE_IDLE);
	}
	else if(pConnect->byConnectType == SERVER_CONNECT_TYPE_CONNENCT_FAIL)
	{
		GetLogInStageState()->ChangeState(LOGIN_STATE_SERVER_CONNECT_FAIL);
		GetAlarmManager()->AlarmMessage( "DST_ACCOUNT_SERVER_CONNECT_FAIL" );
	}
	else if(pConnect->byConnectType == SERVER_CONNECT_TYPE_CONNENCT_DISCONNECT)
	{
		GetAlarmManager()->AlarmMessage( "DST_SERVER_DISCONNECT" );
	}
}

void CLogInStage::LobbyServerConnect(RWS::CMsg &pMsg)
{
	SDboEventServerConnect *pConnect = reinterpret_cast<SDboEventServerConnect*>( pMsg.pData );

	if( pConnect->byConnectType == SERVER_CONNECT_TYPE_CONNENCT_SUCCESS )
	{
		GetLogInStageState()->ChangeState(LOGIN_STATE_CHAR_SERVER_LOGIN);
	}
	else if( pConnect->byConnectType == SERVER_CONNECT_TYPE_CONNENCT_FAIL )
	{
		m_bAutoLogIn = TRUE;
		GetLogInStageState()->ChangeState(LOGIN_STATE_CHAR_SERVER_CONNECT);
	}
}

void CLogInStage::LoginStageStateEnter(RWS::CMsg &pMsg)
{
	SDboEventLogInStageStateEnter *pStageStateEnter = reinterpret_cast<SDboEventLogInStageStateEnter*>(pMsg.pData);

	ELogInStageState eState = (ELogInStageState)pStageStateEnter->byState;
	switch(eState)
	{
	case LOGIN_STATE_LOGO:
		break;
	case LOGIN_STATE_SERVER_CONNECT:
		{
			// account server connect...
			SConfigData *pConfigData = GetDboGlobal()->GetConfigData();  
			SConnectData *pConnectData = GetDboGlobal()->GetConnectData();  
			CNtlClientNet *pNet = GetDboGlobal()->GetNetwork(); 


#ifdef NET_SERVER_SYNC
			pNet->Connect( (char*)pConfigData->strAddr.c_str(), (WORD)pConfigData->dwPort, &pConnectData->sAuthCon.hSocket, true, NETCLIENT_ENCODE_NONE);
#else
            CLoginPacketGenerator *pLoginPackGenerator = CDboGlobal::GetInstance()->GetLoginPacketGenerator(); 
            CNtlNetSender *pNetSender = pLoginPackGenerator->GetNetSender();
			pNetSender->SetData(INVALID_HSERVER, NULL); 
			GetLogInStageState()->ChangeState(LOGIN_STATE_IDLE);
#endif
		}
		break;
	case LOGIN_STATE_SERVER_CONNECT_FAIL:
		break;
	case LOGIN_STATE_CONTRACT:
	{
		m_bAutoLogIn = TRUE;
	}
	break;
	case LOGIN_STATE_IDLE:
	{
		if (m_bAutoLogIn)
		{
			m_bAutoLogIn = FALSE;

			// Request login to server
			SUserData *pUserData = GetDboGlobal()->GetUserData();
			const BYTE* pMacAddress = GetSystemMacAddress();

			if (pMacAddress)
			{
				GetDboGlobal()->GetLoginPacketGenerator()->SendLonInReq(pUserData->wchUserID, pUserData->wchPassword, CLIENT_LVER, CLIENT_RVER, pMacAddress);
				GetLogInStageState()->ChangeState(LOGIN_STATE_LOGINREQ);
			}
			else
			{
				GetAlarmManager()->AlarmMessage(L"Fail read MAC address", CAlarmManager::ALARM_TYPE_LOBBY_MSGBOX);
			}
		}
	}
	break;
	case LOGIN_STATE_LOGINREQ:
		break;
	case LOGIN_STATE_CHAR_SERVER_CONNECT:
		{
#ifdef NET_SERVER_SYNC

			SConnectData *pConData = GetDboGlobal()->GetConnectData();  
			CNtlClientNet *pNet = GetDboGlobal()->GetNetwork(); 

			SCharServerInfo sCharServerInfo;
			RwBool bExist = pConData->PopCharacterServer(&sCharServerInfo);
			if(bExist)
				pNet->Connect( (char*)sCharServerInfo.chServerIP, sCharServerInfo.wServerPort, &pConData->sCharCon.hSocket, TRUE, NETCLIENT_ENCODE_NONE);
			else
			{
				GetAlarmManager()->AlarmMessage( "DST_CHAR_SERVER_CONNECT_FAIL" );
				GetLogInStageState()->ChangeState(LOGIN_STATE_SERVER_CONNECT_FAIL);
			}
#else
			CLobbyPacketGenerator *pLobbyPacketGenerator = GetDboGlobal()->GetLobbyPacketGenerator(); 
			CNtlNetSender *pNetSender = pLobbyPacketGenerator->GetNetSender();
			pNetSender->SetData(INVALID_HSERVER, NULL); 

			GetLogInStageState()->ChangeState(LOGIN_STATE_CHAR_SERVER_LOGIN);
#endif
		}
		break;
	}
}

void CLogInStage::LoginStageStateExit(RWS::CMsg &pMsg)
{
}

void CLogInStage::LogInStageTimeOutEventHandler(RWS::CMsg &msg)
{
	SDboEventLogInStageTimeOut *pStageTimeOut = reinterpret_cast<SDboEventLogInStageTimeOut*>(msg.pData);

	ELogInStageState eState = (ELogInStageState)pStageTimeOut->byState;
	switch(eState)
	{
	case LOGIN_STATE_LOGO:
		break;
	case LOGIN_STATE_SERVER_CONNECT: 
		GetAlarmManager()->AlarmMessage( "DST_ACCOUNT_SERVER_CONNECT_FAIL" );
		break;
	case LOGIN_STATE_SERVER_CONNECT_FAIL:
		break;
	case LOGIN_STATE_IDLE:
		break;
	case LOGIN_STATE_LOGINREQ:
		GetAlarmManager()->AlarmMessage( "AUTH_AUTHKEY_FAIL", TRUE );
		GetLogInStageState()->ChangeState(LOGIN_STATE_IDLE);
		break;
	case LOGIN_STATE_CHAR_SERVER_CONNECT:
		GetLogInStageState()->ChangeState(LOGIN_STATE_CHAR_SERVER_CONNECT);
		break;
	case LOGIN_STATE_CHAR_SERVER_LOGIN:
		GetAlarmManager()->AlarmMessage( "CHARACTER_AUTHKEY_FAIL", TRUE );
		m_bAutoLogIn = FALSE;
		GetLogInStageState()->ChangeState(LOGIN_STATE_SERVER_CONNECT);
		break;
	}
}

void CLogInStage::LogInMessageEventHandler(RWS::CMsg &msg)
{
	SDboEventLoginMessage* pEvent = reinterpret_cast<SDboEventLoginMessage*>( msg.pData );

	switch(pEvent->byMessage)
	{
		case LOGIN_EVENT_FINISH_LOGO_PLAY:
		{
			Logic_SetOptionMovieStop();
			GetLogInStageState()->ChangeState(LOGIN_STATE_SERVER_CONNECT);
		}
		break;

		default: break;
	}
}

int CLogInStage::ActionGameExit(void)
{
	CDboApplication::GetInstance()->SetRenderEnable(TRUE);
	/*CDboApplication::GetInstance()->SetToggleScreenMode(TRUE);*/
	
	GetInputActionMap()->SetActive(FALSE);
	
	return TRUE;
}