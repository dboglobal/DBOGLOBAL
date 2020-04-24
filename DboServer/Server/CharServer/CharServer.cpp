//-----------------------------------------------------------------------------------
//		Char Server
//-----------------------------------------------------------------------------------

#include "stdafx.h"
#include "CharServer.h"
#include "NtlBitFlagManager.h"
#include "TableFileNameList.h"
#include "mysql.h"
#include "uc_opcodes.h"
#include "mc_opcodes.h"
#include "qc_opcodes.h"
#include "PacketEvent.h"



Database* db_char;
Database* db_acc;


CCharServer::CCharServer()
{
	m_pMasterServerSession = NULL;
}

CCharServer::~CCharServer()
{
}


int CCharServer::OnInitApp()
{
	m_nMaxSessionCount = m_config.nMaxConnection + 3;

	m_pPacketEvent = new CPacketEvent;

	NTL_PRINT(PRINT_APP, "Create OP CODES");
	CUC_Opcodes* cu_opcode = new CUC_Opcodes;
	UNREFERENCED_PARAMETER(cu_opcode);
	CMC_Opcodes* mu_opcode = new CMC_Opcodes;
	UNREFERENCED_PARAMETER(mu_opcode);
	CQC_Opcodes* qu_opcode = new CQC_Opcodes;
	UNREFERENCED_PARAMETER(qu_opcode);

	NTL_PRINT(PRINT_APP,"Create Player Manager");
	CPlayerManager* player_manager = new CPlayerManager;
	UNREFERENCED_PARAMETER(player_manager);

	NTL_PRINT(PRINT_APP, "Init Timed-Event Manager");
	EventMgr* m_pEventMgr = new EventMgr;
	UNREFERENCED_PARAMETER(m_pEventMgr);

	NTL_PRINT(PRINT_APP,"INIT NEIGHBOR SERVER INFO MANAGER");
	m_pNeighborServerInfoManager = new CSubNeighborServerInfoManager;
	g_pServerInfoManager->Create(NTL_SERVER_TYPE_CHARACTER);

	NTL_PRINT(PRINT_APP, "Create Queue Manager");
	CQueueManager* queue_manager = new CQueueManager;
	UNREFERENCED_PARAMETER(queue_manager);

	m_pSessionFactory = new CGameSessionFactory;
	if( NULL == m_pSessionFactory )
	{
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	return NTL_SUCCESS;
}


int CCharServer::OnAppStart()
{
	NTL_PRINT(PRINT_APP,"Create Main Tables");
	CreateTableContainer(m_config.LoadTableFormat);
	NTL_PRINT(PRINT_APP,"Load Localized Tables");
	LoadLocalizedTable();

	g_pServerInfoManager->StartEvents();

	return NTL_SUCCESS;
}


int CCharServer::OnCreate()
{
	int rc = NTL_SUCCESS;

	rc = m_clientAcceptor.Create(m_config.strClientAcceptAddr.c_str(), m_config.wClientAcceptPort, 1, m_config.wClientAcceptPort, SESSION_CLIENT, m_config.nMaxConnection, m_config.nMaxConnection, m_config.nMaxConnection, m_config.nMaxConnection);
	if ( NTL_SUCCESS != rc )
		return rc;
	
	rc = m_network.Associate( &m_clientAcceptor, true );
	if( NTL_SUCCESS != rc )
		return rc;


	rc = m_MasterServerConnector.Create( m_config.strMasterServerIP.c_str(), m_config.wMasterServerPort, SESSION_SERVER_CON_CHAR_TO_MASTER, 1000);
	if( NTL_SUCCESS != rc )
		return rc;
	rc = m_network.Associate( &m_MasterServerConnector, true );
	if( NTL_SUCCESS != rc )
		return rc;
	

	rc = m_QueryServerConnector.Create(m_config.strQueryServerIP.c_str(), m_config.wQueryServerPort, SESSION_SERVER_CON_QUERY_CHAR, 1000);
	if (NTL_SUCCESS != rc)
		return rc;
	rc = m_network.Associate(&m_QueryServerConnector, true);
	if (NTL_SUCCESS != rc)
		return rc;


	return NTL_SUCCESS;
}

void CCharServer::OnDestroy()
{
}




//-----------------------------------------------------------------------------------
//		Loading Tables
//-----------------------------------------------------------------------------------
bool CCharServer::CreateTableContainer(int LoadTableFormat)
{

	CNtlBitFlagManager flagManager;
    if (false == flagManager.Create(CTableContainer::TABLE_COUNT))
    {
        return false;
    }

    CTableFileNameList fileNameList;
    if (false == fileNameList.Create())
    {
        return false;
    }


	
	flagManager.Set(CTableContainer::TABLE_NEWBIE);
	fileNameList.SetFileName(CTableContainer::TABLE_NEWBIE,					"Table_Newbie_Data");
	flagManager.Set(CTableContainer::TABLE_ITEM); 
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM,					"Table_Item_Data");
	flagManager.Set(CTableContainer::TABLE_QUEST_TEXT_DATA);
	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_TEXT_DATA,		"table_quest_text_data");


	g_pTableContainer = new CTableContainer;

	//set load table method/format
	CTable::eLOADING_METHOD eLoadMethod = (CTable::eLOADING_METHOD)LoadTableFormat;

	return g_pTableContainer->Create(flagManager, (char*)m_config.TablePath.c_str(), &fileNameList, eLoadMethod, GetACP(), NULL);
}


//-----------------------------------------------------------------------------------
//		LOAD LOCALIZED TABLE (update tables)
//-----------------------------------------------------------------------------------
bool CCharServer::LoadLocalizedTable()
{

	CNtlBitFlagManager flagManager;
    if (false == flagManager.Create(CTableContainer::TABLE_COUNT))
    {
        return false;
    }

    CTableFileNameList fileNameList;
    if (false == fileNameList.Create())
    {
        return false;
    }


	flagManager.Set(CTableContainer::TABLE_ITEM);
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM,					"o_Table_Item_Data");


	//load tables
	this->g_pTableContainer->Update(flagManager, &fileNameList);

	return TRUE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CCharServer::OnCommandInput(std::string& sCmd)
{

	return TRUE;
}


void CCharServer::Run()
{
	DWORD m_dwTickCount = 0;
	DWORD m_dwLastTimeGameMainUpdated = GetTickCount();
	DWORD m_dwQueryTaskRun = GetTickCount();

	while (IsRunnable())
	{
		m_dwTickCount = GetTickCount();

		m_pPacketEvent->ProcessEventQueue();

		GetCharDB.QueryTaskRun();
		GetAccDB.QueryTaskRun();


		if (m_dwTickCount - m_dwLastTimeGameMainUpdated >= 100) //execute every ~100ms
		{
			g_pServerInfoManager->TickProcess(m_dwTickCount - m_dwLastTimeGameMainUpdated);

			m_dwLastTimeGameMainUpdated = m_dwTickCount;
		}


		g_pQueueManager->TickProcess(m_dwTickCount - m_dwQueryTaskRun);

		g_PlrMgr->TickProcess(m_dwTickCount - m_dwQueryTaskRun);

		Wait(1);
	}
}

void CCharServer::PostClientPacketEvent(CPacketEventObj * pEvent)
{
	m_pPacketEvent->PostEvent(pEvent);
}


//-----------------------------------------------------------------------------------
//		CharServerMain
//-----------------------------------------------------------------------------------
int main(int argc, _TCHAR* argv[])
{
	CCharServer app;
	CNtlFileStream traceFileStream;
	
	SYSTEMTIME ti;
	GetLocalTime( &ti );

	// CHECK INI FILE AND START PROGRAM
	int rc = app.Create(argc, argv, argv[1]);
	if (NTL_SUCCESS != rc)
		return rc;

	CNtlString consolename;
	consolename.Format("DBOG Character Server - Server Index %u", app.GetServerIndex());
	SetConsoleTitle(consolename.c_str());


	// LOG FILE
	char m_LogFile[256];
	sprintf_s(m_LogFile,".\\logs\\charserver\\index_%u\\log_%02d-%02d-%02d.txt", app.GetServerIndex(), ti.wYear, ti.wMonth, ti.wDay);

	rc = traceFileStream.Create(m_LogFile);
	if (NTL_SUCCESS != rc)
		return rc;

	app.m_log.AttachLogStream(traceFileStream.GetFilePtr());
	NtlSetPrintFlag(PRINT_APP | PRINT_SYSTEM);


	// CONNECT TO MYSQL DATABASE
	NTL_PRINT(PRINT_APP, "CONNECTING TO DATABASE");

	db_char = Database::CreateDatabaseInterface(1);

	// Initialize it
	if (!GetCharDB.Initialize(app.GetDatabaseHost(), 3306, app.GetDatabaseUser(),
		app.GetDatabasePassword(), app.GetDatabaseName(), 5))
	{
		NTL_PRINT(PRINT_APP, "sql : character database initialization failed. Exiting.");
		Sleep(5000);
		return 0;
	}

	db_acc = Database::CreateDatabaseInterface(1);

	// Initialize it
	if (!GetAccDB.Initialize(app.GetAccDatabaseHost(), 3306, app.GetAccDatabaseUser(),
		app.GetAccDatabasePassword(), app.GetAccDatabaseName(), 5))
	{
		NTL_PRINT(PRINT_APP, "sql : account database initialization failed. Exiting.");
		Sleep(5000);
		return 0;
	}

	NTL_PRINT(PRINT_APP, "CONNECT TO DATABASE SUCCESS");

	unsigned int mysqlthreadsafe = mysql_thread_safe();
	if (!mysqlthreadsafe)
		NTL_PRINT(PRINT_APP, "mysql lib is not a thread safe mode!!!!!!!!!");

	Database::StartThread();


	app.Start();
	app.WaitCommandInput();
	app.WaitForTerminate();
	return 0;
}
