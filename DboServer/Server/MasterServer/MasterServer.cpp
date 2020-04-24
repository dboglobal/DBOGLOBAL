//-----------------------------------------------------------------------------------
//		Master Server
//-----------------------------------------------------------------------------------

#include "stdafx.h"
#include "MasterServer.h"

int	CMasterServer::OnInitApp()
{
	m_nMaxSessionCount = m_config.ServerAcceptLimit;

	NTL_PRINT(PRINT_APP, "Init Timed-Event Manager");
	EventMgr* m_pEventMgr = new EventMgr;
	UNREFERENCED_PARAMETER(m_pEventMgr);

	NTL_PRINT(PRINT_APP,"INIT SERVER MANAGER");
	CSubNeighborServerInfoManager* server_manager = new CSubNeighborServerInfoManager;
	UNREFERENCED_PARAMETER(server_manager);
	g_pSrvMgr->Create(NTL_SERVER_TYPE_MASTER);

	m_pSessionFactory =  new CMasterSessionFactory;
	if( NULL == m_pSessionFactory )
	{
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	return NTL_SUCCESS;
}

int CMasterServer::OnAppStart()
{
	g_pSrvMgr->StartEvents();

	return NTL_SUCCESS;
}

int	CMasterServer::OnCreate()
{
	int rc = NTL_SUCCESS;

	rc = m_AuthServerAcceptor.Create(m_config.strAuthServerAcceptIP.c_str(), m_config.wAuthServerAcceptPort, 1, m_config.wAuthServerAcceptPort, SESSION_SERVER_CON_AUTH_TO_MASTER, 1, 1, 1, 1);
	if( NTL_SUCCESS != rc ) 
		return rc;
	rc = m_network.Associate( &m_AuthServerAcceptor, true );
	if( NTL_SUCCESS != rc )
		return rc;
	rc = m_CharServerAcceptor.Create(m_config.strCharServerAcceptIP.c_str(), m_config.wCharServerAcceptPort, 1, m_config.wCharServerAcceptPort, SESSION_SERVER_CON_CHAR_TO_MASTER, 10, 10, 10, 10);
	if( NTL_SUCCESS != rc ) 
		return rc;
	rc = m_network.Associate( &m_CharServerAcceptor, true );
	if( NTL_SUCCESS != rc )
		return rc;
	rc = m_ChatServerAcceptor.Create(m_config.strChatServerAcceptIP.c_str(), m_config.wChatServerAcceptPort, 1, m_config.wChatServerAcceptPort, SESSION_SERVER_CON_CHAT_TO_MASTER, 5, 5, 2, 5);
	if( NTL_SUCCESS != rc ) 
		return rc;
	rc = m_network.Associate( &m_ChatServerAcceptor, true );
	if( NTL_SUCCESS != rc )
		return rc;
	rc = m_GameServerAcceptor.Create(m_config.strGameServerAcceptIP.c_str(), m_config.wGameServerAcceptPort, 1, m_config.wGameServerAcceptPort, SESSION_SERVER_CON_GAME_TO_MASTER, 10, 10, 10, 10);
	if( NTL_SUCCESS != rc ) 
		return rc;
	rc = m_network.Associate( &m_GameServerAcceptor, true );
	if( NTL_SUCCESS != rc )
		return rc;

	rc = m_WebServerAcceptor.Create(m_config.strWebServerAcceptIP.c_str(), m_config.wWebServerAcceptPort, 1, m_config.wWebServerAcceptPort, SESSION_SERVER_CON_WEB, 1, 1, 1, 1);
	if (NTL_SUCCESS != rc)
		return rc;
	rc = m_network.Associate(&m_WebServerAcceptor, true);
	if (NTL_SUCCESS != rc)
		return rc;

	return NTL_SUCCESS;

}

void	CMasterServer::OnDestroy()
{
}


void	CMasterServer::Run()
{
	DWORD m_dwTickCount, dwLastLoop = 0;
	DWORD m_dwLastTimeGameMainUpdated = GetTickCount();

	while (IsRunnable())
	{
		m_dwTickCount = GetTickCount();

		if (dwLastLoop && m_dwTickCount - dwLastLoop > 1000)
		{
			NTL_PRINT(PRINT_APP, "m_dwTickCount - dwLastLoop %u > 1000", m_dwTickCount - dwLastLoop);
			ERR_LOG(LOG_GENERAL, "m_dwTickCount - dwLastLoop %u > 1000", m_dwTickCount - dwLastLoop);
		}

		if (m_dwTickCount - m_dwLastTimeGameMainUpdated >= 1000) //update events every 1000 second
		{
			DWORD dwTickDiff = m_dwTickCount - m_dwLastTimeGameMainUpdated;

			g_pSrvMgr->TickProcess(dwTickDiff);

			m_dwLastTimeGameMainUpdated = m_dwTickCount;
		}

		dwLastLoop = GetTickCount();
		Wait(1);
	}
}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CMasterServer::OnCommandInput(std::string& sCmd)
{
	if (sCmd == "printplayers")
	{
		g_pSrvMgr->PrintOnlinePlayers();
	}
	else if (sCmd == "playercount")
	{
		NTL_PRINT(PRINT_APP,"Players in CharServers: %u, Players in GameServers: %u", g_pSrvMgr->GetPlayerInCharServer(), g_pSrvMgr->GetPlayerInGameServer());
	}

	return TRUE;
}


int main(int argc, _TCHAR* argv[])
{
	CMasterServer app;
	CNtlFileStream traceFileStream;

	SYSTEMTIME ti;
	GetLocalTime( &ti );

	SetConsoleTitle( TEXT("MasterServer") );

	int rc = app.Create(argc, argv, ".\\config\\MasterServer.ini");

	if( NTL_SUCCESS != rc )
	{
		printf("Server Application Create Fail %d(%s)", rc, NtlGetErrorMessage(rc) );
		Sleep(20000);
		return rc;
	}
	
	// LOG FILE
	char m_LogFile[256];
	sprintf(m_LogFile,".\\logs\\masterserver\\log_%02d-%02d-%02d.txt", ti.wYear, ti.wMonth, ti.wDay);

	rc = traceFileStream.Create(m_LogFile);
	if (NTL_SUCCESS != rc)
		return rc;

	app.m_log.AttachLogStream(traceFileStream.GetFilePtr());
	NtlSetPrintFlag(PRINT_APP | PRINT_SYSTEM);

	app.Start();
	NTL_PRINT(PRINT_APP, "MASTER SERVER STARTED");

	app.WaitCommandInput();
	app.WaitForTerminate();
	return 0;
}
