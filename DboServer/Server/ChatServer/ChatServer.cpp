//-----------------------------------------------------------------------------------
//		Chat Server
//-----------------------------------------------------------------------------------

#include "stdafx.h"
#include "ChatServer.h"
#include "TableContainerManager.h"
#include "mysql.h"
#include "Party.h"
#include "gt_opcodes.h"
#include "ut_opcodes.h"
#include "PacketEvent.h"
#include "DojoWar.h"
#include "HlsSlotMachine.h"
#include "NtlRandom.h"


Database* db_char;
Database* db_acc;
Database* db_log;

CChatServer::CChatServer()
{
	m_pMasterServerSession = NULL;
}

CChatServer::~CChatServer()
{
}


int CChatServer::OnInitApp()
{
	NtlRandInit(time(NULL));

	m_pGameServerSession = NULL;

	m_nMaxSessionCount = m_config.nMaxConnection + m_config.ServerAcceptLimit + 1;

	NTL_PRINT(PRINT_APP, "Init Timed-Event Manager");
	EventMgr* m_pEventMgr = new EventMgr;
	UNREFERENCED_PARAMETER(m_pEventMgr);

	NTL_PRINT(PRINT_APP, "Prepare Opcodes System");
	CGT_Opcodes* cgt_opcodes = new CGT_Opcodes;
	UNREFERENCED_PARAMETER(cgt_opcodes);
	CUT_Opcodes* cUT_opcodes = new CUT_Opcodes;
	UNREFERENCED_PARAMETER(cUT_opcodes);

	NTL_PRINT(PRINT_APP, "Prepare DojoWar Manager");
	CDojoWar* pDojoWarManager = new CDojoWar;
	UNREFERENCED_PARAMETER(pDojoWarManager);

	NTL_PRINT(PRINT_APP, "Prepare Packet Event");
	m_pPacketEvent = new CPacketEvent;

	NTL_PRINT(PRINT_APP, "Prepare Neihbor Server Manager");
	CSubNeighborServerInfoManager* pservermanager = new CSubNeighborServerInfoManager;
	g_pServerInfoManager->Create(NTL_SERVER_TYPE_COMMUNITY);

	NTL_PRINT(PRINT_APP, "Prepare Game Session Factory");
	m_pSessionFactory = new CGameSessionFactory;
	if( NULL == m_pSessionFactory )
	{
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	NTL_PRINT(PRINT_APP, "Init Main & Localized Tables ");
	CTableContainerManager* tc_manager = new CTableContainerManager;
	if (tc_manager->CreateTableContainer(m_config.LoadTableFormat, (char*)m_config.TablePath.c_str()) == false)
	{
		NTL_PRINT(PRINT_APP, "Failed loading main tables ");
		return NTL_FAIL;
	}

	NTL_PRINT(PRINT_APP, "Create Party Manager");
	CPartyManager* partymgr = new CPartyManager;
	UNREFERENCED_PARAMETER(partymgr);

	NTL_PRINT(PRINT_APP, "Create HLS-Slot Machine Manager");
	CHlsSlotMachine* pSlotMachine = new CHlsSlotMachine;
	UNREFERENCED_PARAMETER(pSlotMachine);

	NTL_PRINT(PRINT_APP, "Init Player Manager");
	CPlayerManager* player_manager = new CPlayerManager;
	g_pPlayerManager->Init();


	return NTL_SUCCESS;
}

int CChatServer::OnAppStart()
{
	g_pServerInfoManager->StartEvents();

	return NTL_SUCCESS;
}

int CChatServer::OnCreate()
{
	//Create network
	int rc = NTL_SUCCESS;

	//Accept server connections
	rc = m_serverAcceptor.Create(m_config.strServerAcceptAddr.c_str(), m_config.wServerAcceptPort, 1, m_config.wServerAcceptPort, SESSION_SERVER_PASSIVE, m_config.ServerAcceptLimit, m_config.ServerAcceptLimit, m_config.ServerAcceptLimit, m_config.ServerAcceptLimit);
	if (NTL_SUCCESS != rc)
	{
		return rc;
	}
	rc = m_network.Associate(&m_serverAcceptor, true);
	if (NTL_SUCCESS != rc)
	{
		return rc;
	}

	//Accept client connections
	rc = m_clientAcceptor.Create(m_config.strClientAcceptAddr.c_str(), m_config.wClientAcceptPort, 1, m_config.wClientAcceptPort, SESSION_CLIENT, m_config.nMaxConnection, m_config.nMaxConnection, m_config.nMaxConnection, m_config.nMaxConnection);
	if (NTL_SUCCESS != rc)
	{
		return rc;
	}
	rc = m_network.Associate(&m_clientAcceptor, true);
	if (NTL_SUCCESS != rc)
	{
		return rc;
	}

	//Query server connector
	rc = m_serverQueryConnector.Create(m_config.strQueryServerIP.c_str(), m_config.wQueryServerPort, SESSION_SERVER_CON_QUERY_CHAT);
	if (NTL_SUCCESS != rc)
		return rc;
	rc = m_network.Associate(&m_serverQueryConnector, true);
	if (NTL_SUCCESS != rc)
		return rc;

	//connect to master server
	rc = m_MasterServerConnector.Create(m_config.strMasterServerIP.c_str(), m_config.wMasterServerPort, SESSION_SERVER_CON_CHAT_TO_MASTER);
	if (NTL_SUCCESS != rc)
	{
		return rc;
	}
	rc = m_network.Associate(&m_MasterServerConnector, true);
	if (NTL_SUCCESS != rc)
	{
		return rc;
	}

	return rc;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CChatServer::OnCommandInput(std::string& sCmd)
{
	if (sCmd == "help")
	{
		std::cout << "playercount - return amount of players online" << std::endl;

		std::cout << "\n" << std::endl;

		std::cout << "\n" << std::endl;
	}
	else if (sCmd == "playercount")
	{
		printf("Players online: %i \n", g_pPlayerManager->GetPlayerCount());
	}

	return TRUE;
}


void	CChatServer::Run()
{
	DWORD m_dwTickCount = 0;
	DWORD m_dwQueryTaskRun = GetTickCount();

	while (IsRunnable())
	{
		m_dwTickCount = GetTickCount();

		m_pPacketEvent->ProcessEventQueue();

		if (m_dwTickCount - m_dwQueryTaskRun >= 3) //execute every ~3ms
		{
			g_pServerInfoManager->TickProcess(m_dwTickCount - m_dwQueryTaskRun);

			g_pPlayerManager->TickProcess(m_dwTickCount - m_dwQueryTaskRun);

			GetCharDB.QueryTaskRun();
			GetAccDB.QueryTaskRun();
			GetLogDB.QueryTaskRun();

			m_dwQueryTaskRun = m_dwTickCount;
		}

		Wait(1);
	}
}

void CChatServer::PostClientPacketEvent(CPacketEventObj * pEvent)
{
	m_pPacketEvent->PostEvent(pEvent);
}


//-----------------------------------------------------------------------------------
//		ChatServerMain
//-----------------------------------------------------------------------------------
int main(int argc, _TCHAR* argv[])
{
	CChatServer app;
	CNtlFileStream traceFileStream;

	SYSTEMTIME ti;
	GetLocalTime(&ti);

	SetConsoleTitle( TEXT("ChatServer") );

	int rc = app.Create(argc, argv, ".\\config\\ChatServer.ini");
	if (NTL_SUCCESS != rc)
	{
		NTL_PRINT(PRINT_APP, "Server Application Create Fail %d(%s)", rc, NtlGetErrorMessage(rc));
		return rc;
	}


	char m_LogFile[256];
	sprintf(m_LogFile, ".\\logs\\chatserver\\chatlog%02u-%02u-%02u.txt", ti.wYear, ti.wMonth, ti.wDay);
	
	rc = traceFileStream.Create(m_LogFile);
	if (NTL_SUCCESS != rc)
		return rc;

	app.m_log.AttachLogStream(traceFileStream.GetFilePtr());
	NtlSetPrintFlag(PRINT_APP | PRINT_SYSTEM);


	// CONNECT TO CHARACTER DATABASE
	db_char = Database::CreateDatabaseInterface(1);
	// Initialize it
	if (!GetCharDB.Initialize(app.GetDatabaseHost(), 3306, app.GetDatabaseUser(),
		app.GetDatabasePassword(), app.GetDatabaseDatabase(), 3))
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
	if (!GetLogDB.Initialize(app.GetDatabaseHost(), 3306, app.GetDatabaseUser(),
		app.GetDatabasePassword(), "dbo_log", 2))
	{
		NTL_PRINT(PRINT_APP, "sql : log database initialization failed. Exiting.");
		Sleep(5000);
		return 0;
	}

	NTL_PRINT(PRINT_APP, "CONNECT TO DATABASE SUCCESS");

	unsigned int mysqlthreadsafe = mysql_thread_safe();
	if (!mysqlthreadsafe)
		NTL_PRINT(PRINT_APP, "mysql lib is not a thread safe mode!!!!!!!!!");

	Database::StartThread();


	app.Start();
	NTL_PRINT(PRINT_APP, "Chat Server started");
	app.WaitCommandInput();
	app.WaitForTerminate();

	return 0;
}
