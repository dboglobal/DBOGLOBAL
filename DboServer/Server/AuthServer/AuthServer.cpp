//-----------------------------------------------------------------------------------
//		Auth Server
//-----------------------------------------------------------------------------------

#include "stdafx.h"
#include "AuthServer.h"
#include "mysql.h"

Database* db_acc;
Database* db_log;


CAuthServer::CAuthServer()
{
	m_pMasterServerSession = NULL;
	m_pClientAcceptor = NULL;
	m_pServerConnector = NULL;
}

CAuthServer::~CAuthServer()
{

}


int		CAuthServer::OnInitApp()
{
	m_nMaxSessionCount = m_config.nMaxConnection + 2;

	NTL_PRINT(PRINT_APP, "Init Timed-Event Manager");
	EventMgr* m_pEventMgr = new EventMgr;
	UNREFERENCED_PARAMETER(m_pEventMgr);

	NTL_PRINT(PRINT_APP,"INIT NEIGHBOR SERVER INFO MANAGER");
	m_pNeighborServerInfoManager = new CSubNeighborServerInfoManager;
	g_pServerInfoManager->Create(NTL_SERVER_TYPE_AUTH);

	m_pSessionFactory =  new CAuthSessionFactory;
	if( NULL == m_pSessionFactory )
	{
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	return NTL_SUCCESS;
}

int		CAuthServer::OnCreate()
{
	return NTL_SUCCESS;
}

void	CAuthServer::OnDestroy()
{
}


int		CAuthServer::OnAppStart()
{
	int rc = NTL_SUCCESS;

	//client acceptor
	{
		std::unique_ptr<CNtlAcceptor> clientAcceptor(new CNtlAcceptor);
		if (clientAcceptor.get())
		{
			rc = clientAcceptor->Create(m_config.strClientAcceptAddr.c_str(), m_config.wClientAcceptPort, 1, m_config.wClientAcceptPort, SESSION_CLIENT, m_config.nMaxConnection, m_config.nMaxConnection, m_config.nMaxConnection, m_config.nMaxConnection);
			if (NTL_SUCCESS != rc)
			{
				return rc;
			}

			rc = GetNetwork()->Associate(clientAcceptor.get(), true);
			if (NTL_SUCCESS != rc)
			{
				return rc;
			}

			m_pClientAcceptor = clientAcceptor.release();
		}
		else
		{
			return 100003;
		}
	}

	//connector to master server
	{
		std::unique_ptr<CNtlConnector> masterConnector(new CNtlConnector);
		if (masterConnector.get())
		{
			rc = masterConnector->Create(m_config.strMasterServerIP.c_str(), m_config.wMasterServerPort, SESSION_SERVER_CON_AUTH_TO_MASTER, INVALID_DWORD, INVALID_DWORD);
			if (NTL_SUCCESS != rc)
			{
				return rc;
			}

			rc = GetNetwork()->Associate(masterConnector.get(), true);
			if (NTL_SUCCESS != rc)
			{
				return rc;
			}

			m_pServerConnector = masterConnector.release();
		}
		else
		{
			return 100003;
		}
	}

	g_pServerInfoManager->StartEvents();

	return NTL_SUCCESS;
}


bool	CAuthServer::AddPlayer(ACCOUNTID AccID, CClientSession* session)
{
	auto it = m_map_Players.find(AccID);
	if (it == m_map_Players.end()) 
	{
		m_map_Players[AccID] = session;
		return true;
	}

	return false;
}
void	CAuthServer::DelPlayer(ACCOUNTID AccID)
{
	auto it = m_map_Players.find(AccID);
	if (it == m_map_Players.end()) {
		return;
	}

	m_map_Players.erase(AccID);
}
CClientSession*	CAuthServer::FindPlayer(ACCOUNTID AccID)
{
	auto it = m_map_Players.find(AccID);
	if (it == m_map_Players.end()) {
	//	NTL_PRINT(PRINT_APP,"[CAuthServer::FindPlayer] %d not found", AccID);
		return NULL;
	}

	return it->second;
}

bool CAuthServer::IsAccountTempBlocked(const char* strUsername)
{
	std::map<std::string, QWORD>::iterator it = m_mapBlockedAccounts.find(strUsername);
	if (it != m_mapBlockedAccounts.end())
	{
		QWORD curTick64 = GetTickCount64();

		if (curTick64 < it->second)
		{
			return true;
		}
		else
		{
			m_mapBlockedAccounts.erase(it);
		}
	}

	return false;
}

void CAuthServer::RegisterAccountTempBann(const char* strUsername)
{
	m_mapBlockedAccounts.insert({ strUsername, GetTickCount64() + 300000 }); //5 minutes
}


void CAuthServer::Run()
{
	DWORD m_dwTickCount, dwLastLoop = 0;
	DWORD m_dwLastTimeGameMainUpdated = GetTickCount(), m_dwQueryTaskRun = GetTickCount();

	while (IsRunnable())
	{
		m_dwTickCount = GetTickCount();

		if (m_dwTickCount - m_dwQueryTaskRun >= 3) //execute every ~3ms
		{
			GetAccDB.QueryTaskRun();

			m_dwQueryTaskRun = m_dwTickCount;
		}

		if (m_dwTickCount - m_dwLastTimeGameMainUpdated >= 1000) //update events every 1000 second
		{
			DWORD dwTickDiff = m_dwTickCount - m_dwLastTimeGameMainUpdated;

			g_pServerInfoManager->TickProcess(dwTickDiff);

			m_dwLastTimeGameMainUpdated = m_dwTickCount;
		}

		dwLastLoop = GetTickCount();

		if (dwLastLoop - m_dwTickCount > 100)
		{
			NTL_PRINT(PRINT_APP, "dwNow %u - dwLastLoop %u = %u > 1000. timeGetTime = %u", m_dwTickCount, dwLastLoop, dwLastLoop - m_dwTickCount, timeGetTime());
			ERR_LOG(LOG_GENERAL, "dwNow %u - dwLastLoop %u = %u > 1000. timeGetTime = %u", m_dwTickCount, dwLastLoop, dwLastLoop - m_dwTickCount, timeGetTime());
		}


		Wait(1);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------

BOOL CAuthServer::OnCommandInput(std::string& sCmd)
{
	UNREFERENCED_PARAMETER(sCmd);
	//if (sCmd == "networktest")
	//{
	//	NTL_PRINT(PRINT_APP, "START SENDING PACKETS");

	//	for (int i = 0; i < 1000; i++)
	//	{
	//		//send server data to master server
	//		CNtlPacket packet(sizeof(sAM_NOTIFY_SERVER_BEGIN));
	//		sAM_NOTIFY_SERVER_BEGIN * res = (sAM_NOTIFY_SERVER_BEGIN *)packet.GetPacketData();
	//		res->wOpCode = AM_NOTIFY_SERVER_BEGIN;
	//		res->sServerInfo.bIsOn = true;
	//		res->sServerInfo.byRunningState = DBO_SERVER_RUNNING_STATE_RUNNING;
	//		res->sServerInfo.byServerIndex = m_config.byServerId;
	//		res->sServerInfo.dwLoad = 0;
	//		res->sServerInfo.dwMaxLoad = (float)m_config.nMaxConnection * 0.9f;
	//		res->sServerInfo.wPortForClient = m_config.wClientAcceptPort;
	//		strcpy_s(res->sServerInfo.achPublicAddress, NTL_MAX_LENGTH_OF_IP + 1, m_config.strClientAcceptAddr.c_str());
	//		res->sServerInfo.byServerType = NTL_SERVER_TYPE_AUTH;
	//		packet.SetPacketLen(sizeof(sAM_NOTIFY_SERVER_BEGIN));
	//		Send(GetMasterServerHandle(), &packet);
	//	}
	//}

	return TRUE;
}



int main(int argc, _TCHAR* argv[])
{
	CAuthServer app;
	CNtlFileStream traceFileStream;

	SYSTEMTIME ti;
	GetLocalTime(&ti);

	SetConsoleTitle( TEXT("AuthServer") );

	// CHECK INI FILE AND START PROGRAM
	int rc = app.Create(argc, argv, ".\\config\\AuthServer.ini");
	if (NTL_SUCCESS != rc)
	{
		NTL_PRINT(PRINT_APP, "Server Application Create Fail %d(%s)", rc, NtlGetErrorMessage(rc));
		return rc;
	}


	// LOG FILE
	char m_LogFile[256];
	sprintf(m_LogFile, ".\\logs\\authserver\\log_%02u-%02u-%02u.txt", ti.wYear, ti.wMonth, ti.wDay);

	rc = traceFileStream.Create(m_LogFile);
	if (NTL_SUCCESS != rc)
		return rc;

	app.m_log.AttachLogStream(traceFileStream.GetFilePtr());
	NtlSetPrintFlag(PRINT_APP | PRINT_SYSTEM);

	// CONNECT TO MYSQL DATABASE
	NTL_PRINT(PRINT_APP, "CONNECTING TO DATABASE");
	
	db_acc = Database::CreateDatabaseInterface(1);
	if (!GetAccDB.Initialize(app.GetDatabaseHost(), 3306, app.GetDatabaseUser(),
		app.GetDatabasePassword(), app.GetDatabaseName(), 5))
	{
		NTL_PRINT(PRINT_APP, "sql : dbo_acc database initialization failed. Exiting.");
		Sleep(5000);
		return 0;
	}

	db_log = Database::CreateDatabaseInterface(1);
	if (!GetLogDB.Initialize(app.GetDatabaseHost(), 3306, app.GetDatabaseUser(),
		app.GetDatabasePassword(), "dbo_log", 5))
	{
		NTL_PRINT(PRINT_APP, "sql : dbo_log database initialization failed. Exiting.");
		Sleep(5000);
		return 0;
	}

	NTL_PRINT(PRINT_APP, "CONNECT TO DATABASE SUCCESS");

	unsigned int mysqlthreadsafe = mysql_thread_safe();
	if (!mysqlthreadsafe)
		NTL_PRINT(PRINT_APP, "mysql lib is not a thread safe mode!!!!!!!!!");

	Database::StartThread();

	app.Start();
	NTL_PRINT(PRINT_APP, "AUTH SERVER STARTED");
	app.WaitCommandInput();
	app.WaitForTerminate();
	return 0;
}
