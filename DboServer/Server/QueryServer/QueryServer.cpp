//-----------------------------------------------------------------------------------
//		Query Server
//-----------------------------------------------------------------------------------

#include "stdafx.h"
#include "QueryServer.h"
#include "ItemManager.h"
#include "CashshopManager.h"
#include "mysql.h"
#include "CharacterManager.h"
#include "PlayerCache.h"
#include "AuctionHouse.h"
#include "Guild.h"
#include "Dojo.h"
#include "DynamicFieldSystem.h"
#include "DatabaseTaskRun.h"
#include "BudokaiManager.h"


Database* db_char;
Database* db_acc;
Database* db_log;


int CQueryServer::OnInitApp()
{
	m_bDragonballScrambleBegin = false;

	m_nMaxSessionCount = 100;

	NTL_PRINT(PRINT_APP, "Init Server Manager");
	CServerManager* server_manager = new CServerManager;
	UNREFERENCED_PARAMETER(server_manager);

	CPlayerCacheManager* playercache_manager = new CPlayerCacheManager;
	UNREFERENCED_PARAMETER(playercache_manager);

	m_pSessionFactory = new CSessionFactory;
	if( NULL == m_pSessionFactory )
	{
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	return NTL_SUCCESS;
}

int CQueryServer::OnAppStart()
{
	CCharacterManager* character_manager = new CCharacterManager;
	UNREFERENCED_PARAMETER(character_manager);
	CItemManager* item_manager = new CItemManager;
	UNREFERENCED_PARAMETER(item_manager);
	CCashshopManager* cashshop_manager = new CCashshopManager;
	UNREFERENCED_PARAMETER(cashshop_manager);
	CAutionhouse* auctionhouse_manager = new CAutionhouse;
	UNREFERENCED_PARAMETER(auctionhouse_manager);
	CGuild* guild_manager = new CGuild;
	UNREFERENCED_PARAMETER(guild_manager);
	CDojo* dojo_manager = new CDojo;
	UNREFERENCED_PARAMETER(dojo_manager);
	CDynamicFieldSystem* pDynamicFieldSystem_manager = new CDynamicFieldSystem;
	UNREFERENCED_PARAMETER(pDynamicFieldSystem_manager);
	CBudokaiManager* budokai_manager = new CBudokaiManager;
	UNREFERENCED_PARAMETER(budokai_manager);


	return NTL_SUCCESS;
}

int CQueryServer::OnCreate()
{
	int rc = NTL_SUCCESS;

	//ACCEPT CHAT SERVER CONNECTION
	rc = m_ChatServerAcceptor.Create(m_config.strChatServerAcceptAddr.c_str(), m_config.wChatServerAcceptPort, 1, m_config.wChatServerAcceptPort, SESSION_SERVER_CON_QUERY_CHAT, 3, 3, 3, 3);
	if (NTL_SUCCESS != rc)
	{
		return rc;
	}
	rc = m_network.Associate(&m_ChatServerAcceptor, true);
	if (NTL_SUCCESS != rc)
	{
		return rc;
	}

	//ACCEPT GAME SERVER CONNECTION
	rc = m_GameServerAcceptor.Create(m_config.strGameServerAcceptAddr.c_str(), m_config.wGameServerAcceptPort, 1, m_config.wGameServerAcceptPort, SESSION_SERVER_CON_QUERY_GAME, 20, 20, 20, 20);
	if (NTL_SUCCESS != rc)
	{
		return rc;
	}
	rc = m_network.Associate(&m_GameServerAcceptor, true);
	if (NTL_SUCCESS != rc)
	{
		return rc;
	}

	//ACCEPT CHAR SERVER CONNECTION
	rc = m_CharServerAcceptor.Create(m_config.strCharServerAcceptAddr.c_str(), m_config.wCharServerAcceptPort, 1, m_config.wCharServerAcceptPort, SESSION_SERVER_CON_QUERY_CHAR, 20, 20, 20, 20);
	if (NTL_SUCCESS != rc)
	{
		return rc;
	}
	rc = m_network.Associate(&m_CharServerAcceptor, true);
	if (NTL_SUCCESS != rc)
	{
		return rc;
	}

	//Connect to master server
	//rc = m_serverConnector.Create(m_config.strMasterServerIP.c_str(), m_config.wMasterServerPort, SESSION_SERVER_CON_QUERY_TO_MASTER, 5000);
	//if (NTL_SUCCESS != rc)
	//{
	//	return rc;
	//}
	//rc = m_network.Associate(&m_serverConnector, true);
	//if (NTL_SUCCESS != rc)
	//{
	//	return rc;
	//}

	return NTL_SUCCESS;
}

void CQueryServer::OnDestroy()
{
}

void CQueryServer::Run()
{
	while (IsRunnable())
	{
		Wait(1);
	}
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CQueryServer::OnCommandInput(std::string& sCmd)
{
	if (sCmd == "querycount")
	{
		printf("Currently queries to process %u. \n", GetCharDB.GetQueueSize() + GetAccDB.GetQueueSize());
	}

	return TRUE;
}

void CQueryServer::CreateQueryTaskRunThread(int nAmount)
{
	for (int i = 0; i < nAmount; i++)
	{
		CNtlString strName;
		strName.Format("QueryTask %i Processor", i);

		CDatabaseTaskRun* pTask = new CDatabaseTaskRun;
		if (NULL == pTask)
		{
			NTL_PRINT(PRINT_SYSTEM, "\"new CDatabaseTaskRun()\" failed.");
		}

		int rc = pTask->Create();
		if (NTL_SUCCESS != rc)
		{
			NTL_PRINT(PRINT_SYSTEM, "CDatabaseTaskRun->Create() failed.(NTL_SUCCESS != rc) rc = %d", rc);
		}

		CNtlThread * pThread = tThreadFactory::Instance().CreateThread(pTask, strName.c_str());
		if (NULL == pThread)
		{
			NTL_PRINT(PRINT_SYSTEM, "CNtlThreadFactory::CreateThread( pTask, strName, false ) failed.(NULL == pThread)");
			SAFE_DELETE(pTask);
		}

		pThread->Start();
	}
}



//-----------------------------------------------------------------------------------
//		QueryServerMain
//-----------------------------------------------------------------------------------
int main(int argc, _TCHAR* argv[])
{
	CQueryServer app;
	CNtlFileStream traceFileStream;

	SYSTEMTIME ti;
	GetLocalTime(&ti);

	SetConsoleTitle(TEXT("QueryServer"));


	// CHECK INI FILE AND START PROGRAM

	int rc = app.Create(argc, argv, ".\\config\\QueryServer.ini");
	if (NTL_SUCCESS != rc)
		return rc;

	// LOG FILE

	char m_LogFile[256];
	sprintf(m_LogFile, ".\\logs\\queryserver\\log_%02u-%02u-%02u.txt", ti.wYear, ti.wMonth, ti.wDay);

	rc = traceFileStream.Create(m_LogFile);
	if (NTL_SUCCESS != rc)
		return rc;

	app.m_log.AttachLogStream(traceFileStream.GetFilePtr());
	NtlSetPrintFlag(PRINT_APP | PRINT_SYSTEM);



	NTL_PRINT(PRINT_APP, "CONNECT TO DATABASE...");

	// CONNECT TO CHARACTER DATABASE
	db_char = Database::CreateDatabaseInterface(1);
	// Initialize it
	if (!GetCharDB.Initialize(app.GetDatabaseHost(), 3306, app.GetDatabaseUser(),
		app.GetDatabasePassword(), app.GetDatabase(), 5))
	{
		NTL_PRINT(PRINT_APP, "sql : char database initialization failed. Exiting.");
		Sleep(5000);
		return 0;
	}


	// CONNECT TO ACCOUNT DATABASE
	db_acc = Database::CreateDatabaseInterface(1);
	// Initialize it
	if (!GetAccDB.Initialize(app.GetAccDbHost(), 3306, app.GetAccDbUser(),
		app.GetAccDbPass(), app.GetAccDbDatabase(), 2))
	{
		NTL_PRINT(PRINT_APP, "sql : account database initialization failed. Exiting.");
		Sleep(5000);
		return 0;
	}


	// CONNECT TO LOG DATABASE
	db_log = Database::CreateDatabaseInterface(1);
	// Initialize it
	if (!GetLogDB.Initialize(app.GetAccDbHost(), 3306, app.GetAccDbUser(),
		app.GetAccDbPass(), "dbo_log", 2))
	{
		NTL_PRINT(PRINT_APP, "sql : log database initialization failed. Exiting.");
		Sleep(5000);
		return 0;
	}

	unsigned int mysqlthreadsafe = mysql_thread_safe();
	if (!mysqlthreadsafe)
		NTL_PRINT(PRINT_APP, "mysql lib is not a thread safe mode!!!!!!!!!");

	Database::StartThread();
	
	app.Start();

	app.CreateQueryTaskRunThread(1);

	NTL_PRINT(PRINT_APP,"QUERY SERVER READY!");
	app.WaitCommandInput();
	app.WaitForTerminate();

	return NTL_SUCCESS;
}
