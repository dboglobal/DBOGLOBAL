#include "stdafx.h"
#include "GameServer.h"
#include "NtlRandom.h"

#include "SubNeighborServerInfoManager.h"
#include "EventMgr.h"

#include "ItemManager.h"
#include "freebattle.h"
#include "trade.h"
#include "Party.h"
#include "DungeonManager.h"
#include "TriggerManager.h"
#include "ObjectManager.h"
#include "privateshop.h"
#include "RankBattle.h"
#include "PartyMatching.h"
#include "ShenronManager.h"
#include "DojoManager.h"
#include "Guild.h"

#include "GameProcessor.h"
#include "GameData.h"
#include "GameMain.h"
#include "ActionPatternSystem.h"
#include "ug_opcodes.h"
#include "qg_opcodes.h"
#include "tg_opcodes.h"
#include "mg_opcodes.h"
#include "ScriptAlgoObjectManager.h"
#include "DynamicFieldSystemEvent.h"
#include "DragonballHunt.h"
#include "DragonballScramble.h"
#include "DojoWar.h"
#include "BudokaiManager.h"
#include "ExpEvent.h"
#include "BusSystem.h"
#include "scsManager.h"
#include "HoneyBeeEvent.h"



CGameServer::CGameServer()
{
	Init();
}


CGameServer::~CGameServer()
{
	Destroy();
}



int CGameServer::OnInitApp()
{
	m_tmCurrentTime = time(NULL);
	NtlRandInit(m_tmCurrentTime);

	m_nMaxSessionCount = m_config.nMaxConnection + 3;
	
	m_dwCurTickCount = 0;

	NTL_PRINT(PRINT_APP, "Init Session Factory ");
	m_pSessionFactory = new CGameSessionFactory;

	NTL_PRINT(PRINT_APP, "Init Timed-Event Manager");
	EventMgr* m_pEventMgr = new EventMgr;
	UNREFERENCED_PARAMETER(m_pEventMgr);

	NTL_PRINT(PRINT_APP, "Create Neighborserverinfomanager");
	CSubNeighborServerInfoManager* m_pNeighborServerInfoManager = new CSubNeighborServerInfoManager;
	UNREFERENCED_PARAMETER(m_pNeighborServerInfoManager);

	CScriptAlgoObjectManager* pScriptAlgoMgr = new CScriptAlgoObjectManager;
	UNREFERENCED_PARAMETER(pScriptAlgoMgr);

	NTL_PRINT(PRINT_APP, "Create Game Data");
	m_pGameData = new CGameData;
	m_pGameData->Create(0);

	NTL_PRINT(PRINT_APP, "Create Game Main");
	m_pGameMain = new CGameMain;
	m_pGameMain->Create(m_pGameData);

	NTL_PRINT(PRINT_APP, "Create Game Processor");
	m_pGameProcessor = new CGameProcessor;
	m_pGameProcessor->Create(100, m_pGameMain);

	NTL_PRINT(PRINT_APP, "Create Actionpatter System");
	m_pActionPatternSystem = new CActionPatternSystem;
	m_pActionPatternSystem->Create();

	NTL_PRINT(PRINT_APP,"Init Trigger Manager");
	CTriggerManager* trigger_manager = new CTriggerManager;
	trigger_manager->Init(m_config.bLoadTriggersEnc);

	NTL_PRINT(PRINT_APP,"Init Trade Manager ");
	CTradeManager* trade_manager = new CTradeManager;
	UNREFERENCED_PARAMETER(trade_manager);

	NTL_PRINT(PRINT_APP,"Init Dungeon Manager ");
	CDungeonManager* dungeon_manager = new CDungeonManager;
	UNREFERENCED_PARAMETER(dungeon_manager);

	NTL_PRINT(PRINT_APP, "Init Guild System");
	CGuildManager* guild = new CGuildManager;
	UNREFERENCED_PARAMETER(guild);

	NTL_PRINT(PRINT_APP,"Init Party Manager ");
	CPartyManager* party_manager = new CPartyManager;
	UNREFERENCED_PARAMETER(party_manager);

	NTL_PRINT(PRINT_APP,"Init Item Manager ");
	CItemManager* item_manager = new CItemManager;
	UNREFERENCED_PARAMETER(item_manager);

	NTL_PRINT(PRINT_APP,"Init Free Battle Manager ");
	CFreeBattleManager* freebattle_manager = new CFreeBattleManager;
	UNREFERENCED_PARAMETER(freebattle_manager);

	NTL_PRINT(PRINT_APP, "Init Private shop Manager ");
	CShopManager* privateshop_manager = new CShopManager;
	UNREFERENCED_PARAMETER(privateshop_manager);

	NTL_PRINT(PRINT_APP, "Init Rank Battle Manager ");
	CRankbattle* rankbattle_manager = new CRankbattle;
	UNREFERENCED_PARAMETER(rankbattle_manager);

	NTL_PRINT(PRINT_APP, "Init Party Matching");
	CPartyMatching* partymatching_manager = new CPartyMatching;
	UNREFERENCED_PARAMETER(partymatching_manager);

	NTL_PRINT(PRINT_APP, "Init Shenron Manager");
	CShenronManager* shenron_manager = new CShenronManager;
	UNREFERENCED_PARAMETER(shenron_manager);

	NTL_PRINT(PRINT_APP, "Prepare Opcodes System");
	CUG_Opcodes* cUG_opcodes = new CUG_Opcodes;
	UNREFERENCED_PARAMETER(cUG_opcodes);
	CQG_Opcodes* cQG_opcodes = new CQG_Opcodes;
	UNREFERENCED_PARAMETER(cQG_opcodes);
	CTG_Opcodes* cTG_opcodes = new CTG_Opcodes;
	UNREFERENCED_PARAMETER(cTG_opcodes);
	CMG_Opcodes* cMG_opcodes = new CMG_Opcodes;
	UNREFERENCED_PARAMETER(cMG_opcodes);

	CDynamicFieldSystemEvent* pDynEvent = new CDynamicFieldSystemEvent;
	UNREFERENCED_PARAMETER(pDynEvent);

	NTL_PRINT(PRINT_APP, "Prepare DragonballHunt Event System");
	CDragonballHunt* pDragonballHunt = new CDragonballHunt;
	UNREFERENCED_PARAMETER(pDragonballHunt);

	NTL_PRINT(PRINT_APP, "Prepare DragonballScramble Event System");
	CDragonballScramble* pDragonballScramble = new CDragonballScramble;
	UNREFERENCED_PARAMETER(pDragonballScramble);

	NTL_PRINT(PRINT_APP, "Prepare DojoWar System");
	CDojoWar* pDojoWar = new CDojoWar;
	UNREFERENCED_PARAMETER(pDojoWar);

	NTL_PRINT(PRINT_APP, "Exp Event System");
	CExpEvent* pExpEvent = new CExpEvent;
	UNREFERENCED_PARAMETER(pExpEvent);

	NTL_PRINT(PRINT_APP, "Bus System");
	CBusSystem* pBusSystem = new CBusSystem;
	UNREFERENCED_PARAMETER(pBusSystem);

	NTL_PRINT(PRINT_APP, "SCS System");
	CScsManager* pScs = new CScsManager;
	UNREFERENCED_PARAMETER(pScs);

	NTL_PRINT(PRINT_APP, "HoneyBee System");
	CHoneyBeeEvent* pHoneyBee = new CHoneyBeeEvent;
	UNREFERENCED_PARAMETER(pScs);

	if (!m_pGameMain->PrepareWorldAndObject())
	{
		NTL_PRINT(PRINT_APP, "m_pGameMain->PrepareWorldAndObject() == FALSE");
		return NTL_FAIL;
	}

	return NTL_SUCCESS;
}

int CGameServer::OnAppStart()
{
	NTL_PRINT(PRINT_APP, "Init Dojo Manager");
	CDojoManager* dojo_manager = new CDojoManager;
	UNREFERENCED_PARAMETER(dojo_manager);

	NTL_PRINT(PRINT_APP, "Prepare Budokai System");
	CBudokaiManager* pBudokaiManager = new CBudokaiManager;
	UNREFERENCED_PARAMETER(pBudokaiManager);

	int rc = NTL_SUCCESS;

	rc = m_clientAcceptor.Create(m_config.strClientAcceptAddr.c_str(), m_config.wClientAcceptPort, 1, m_config.wClientAcceptPort, SESSION_CLIENT, m_config.nMaxConnection, m_config.nMaxConnection, m_config.nMaxConnection, m_config.nMaxConnection);
	if (NTL_SUCCESS != rc)
		return rc;
	rc = m_network.Associate(&m_clientAcceptor, true);
	if (NTL_SUCCESS != rc)
		return rc;

	//Connect chat server
	rc = m_serverChatConnector.Create(m_config.strChatServerConAddr.c_str(), m_config.wChatServerConPort, SESSION_SERVER_CON_CHAT, 1000);
	if (NTL_SUCCESS != rc)
		return rc;
	rc = m_network.Associate(&m_serverChatConnector, true);
	if (NTL_SUCCESS != rc)
		return rc;

	//Query server connector
	rc = m_serverQueryConnector.Create(m_config.strQueryServerIP.c_str(), m_config.wQueryServerPort, SESSION_SERVER_CON_QUERY_GAME, INVALID_DWORD, INVALID_DWORD); //dont try to reconnect. Server will shut down once dc
	if (NTL_SUCCESS != rc)
		return rc;
	rc = m_network.Associate(&m_serverQueryConnector, true);
	if (NTL_SUCCESS != rc)
		return rc;

	//Connect master server
	rc = m_serverMasterConnector.Create(m_config.strMasterServerIP.c_str(), m_config.wMasterServerPort, SESSION_SERVER_CON_GAME_TO_MASTER, INVALID_DWORD, INVALID_DWORD); //dont try to reconnect. Server will shut down once dc
	if (NTL_SUCCESS != rc)
		return rc;
	rc = m_network.Associate(&m_serverMasterConnector, true);
	if (NTL_SUCCESS != rc)
		return rc;


	NTL_PRINT(PRINT_APP, "GAME SERVER READY ");

	return NTL_SUCCESS;
}


int CGameServer::OnCreate()
{
	return NTL_SUCCESS;
}


void CGameServer::Run()
{
	DWORD dwNow, dwLastLoop = 0;

	while (true)
	{
		LARGE_INTEGER m_freq, rStart, rEnd, rLoadReport, rMemoryUsage;

		QueryPerformanceFrequency(&m_freq);
		QueryPerformanceCounter(&rStart);

		dwNow = GetTickCount();
		m_dwCurTickCount = dwNow;
		m_tmCurrentTime = time(NULL);

	//	DoUpdatePerformanceLog(dwNow); //requires too much time
		DoReportLoad(dwNow);
		QueryPerformanceCounter(&rLoadReport);

		DoUpdateMemoryUseLog( dwNow);
		QueryPerformanceCounter(&rMemoryUsage);

		if(GetMasterServerSession())		//master server is the last one we connect.. So only loop when we are connected to master server
			m_pGameProcessor->Run(dwNow);

		dwLastLoop = GetTickCount();
		QueryPerformanceCounter(&rEnd);

		float fDur = ((float)(rEnd.QuadPart - rStart.QuadPart)) * 1000.f / ((float)m_freq.QuadPart);

		if (fDur > 200.f)
		{
			NTL_PRINT(PRINT_APP, "dwLastLoop %u - m_dwCurTickCount %u = %u > 200.", m_dwCurTickCount, dwLastLoop, dwLastLoop - m_dwCurTickCount);
			ERR_LOG(LOG_SYSTEM, "MainLoop: Total %f, LoadReport %f, MemoryUsage %f, GameProcess %f",
				fDur,
				((float)(rLoadReport.QuadPart - rStart.QuadPart)) * 1000.f / ((float)m_freq.QuadPart),
				((float)(rMemoryUsage.QuadPart - rLoadReport.QuadPart)) * 1000.f / ((float)m_freq.QuadPart),
				((float)(rEnd.QuadPart - rMemoryUsage.QuadPart)) * 1000.f / ((float)m_freq.QuadPart)
			);
		}

		Wait(1);
	}

	ERR_LOG(LOG_SYSTEM, "CGameServer::Run(): IsRunnable() == false");
}


int	CGameServer::OnConfiguration(const char * lpszConfigFile)
{
	CNtlIniFile file;
	int rc = file.Create(lpszConfigFile);
	if (NTL_SUCCESS != rc)
		return rc;

	if (!file.Read("Game Server", "Address", m_config.strClientAcceptAddr))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("Game Server", "PublicAddress", m_config.strPublicClientAcceptAddr))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("Game Server", "Port", m_config.wClientAcceptPort))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("Game Server", "ServerID", m_config.byServerID))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("Game Server", "Channel", m_config.byChannel))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("Game Server", "Servername", m_config.ServerName))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	file.Read("Game Server", "Channelname", m_config.ChannelName);
	


	//CONNECT CHAT SERVER
	if (!file.Read("Chat Connect", "Address", m_config.strChatServerIP))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("Chat Connect", "Port", m_config.wChatServerPort))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("Chat Connect", "ServerConAddr", m_config.strChatServerConAddr))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("Chat Connect", "ServerConPort", m_config.wChatServerConPort))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}

	//CONNECT MASTER SERVER
	if (!file.Read("Master Connect", "Address", m_config.strMasterServerIP))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("Master Connect", "Port", m_config.wMasterServerPort))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}


	if (!file.Read("Query Connect", "Address", m_config.strQueryServerIP))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("Query Connect", "Port", m_config.wQueryServerPort))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}

	//DATABASE
	if (!file.Read("DATABASE_CHARACTER", "Host", m_config.DatabaseHost))
	{
		return NTL_ERR_DBC_HANDLE_ALREADY_ALLOCATED;
	}
	if (!file.Read("DATABASE_CHARACTER", "User", m_config.DatabaseUser))
	{
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}
	if (!file.Read("DATABASE_CHARACTER", "Password", m_config.DatabasePassword))
	{
		return NTL_ERR_SYS_LOG_SYSTEM_INITIALIZE_FAIL;
	}
	if (!file.Read("DATABASE_CHARACTER", "Db", m_config.Database))
	{
		return NTL_ERR_DBC_CONNECTION_CONNECT_FAIL;
	}
	if (!file.Read("DATABASE_ACCOUNT", "Host", m_config.AccDatabaseHost))
	{
		return NTL_ERR_DBC_HANDLE_ALREADY_ALLOCATED;
	}
	if (!file.Read("DATABASE_ACCOUNT", "User", m_config.AccDatabaseUser))
	{
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}
	if (!file.Read("DATABASE_ACCOUNT", "Password", m_config.AccDatabasePassword))
	{
		return NTL_ERR_SYS_LOG_SYSTEM_INITIALIZE_FAIL;
	}
	if (!file.Read("DATABASE_ACCOUNT", "Db", m_config.AccDatabase))
	{
		return NTL_ERR_DBC_CONNECTION_CONNECT_FAIL;
	}
	// CONFIG
	if (!file.Read("CONFIG", "MaxConnection", m_config.nMaxConnection))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}

	// TABLES
	if (!file.Read("TABLE", "LoadTableFormat", m_config.LoadTableFormat))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("TABLE", "Path", m_config.TablePath))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}

	//SETTINGS
	if (!file.Read("SETTINGS", "TestServer", m_config.bTestServer))
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	if (!file.Read("SETTINGS", "LoadTriggersEnc", m_config.bLoadTriggersEnc))
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	if (!file.Read("SETTINGS", "LogPath", m_config.strLogPath))
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	if (!file.Read("SETTINGS", "TsPath", m_config.strTsPath))
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;


	if (!file.Read("Play Script", "DataPath", m_config.strPlayScriptPath))
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;

	if (!file.Read("AI Script", "DataPath", m_config.strAIScriptPath))
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;

	if (!file.Read("World Play Script", "DataPath", m_config.strWorldPlayScriptPath))
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;

	if (!file.Read("Time Quest Script", "DataPath", m_config.strTimeQuestScriptPath))
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;


	//GRAPHIC
	if (!file.Read("Graphic Data", "CharacterDataBinaryPath", m_config.m_strGraphicCharacterDataPath))
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	if (!file.Read("Graphic Data", "ObjectDataBinaryPath", m_config.m_strGraphicObjectDataPath))
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	if (!file.Read("Graphic Data", "WorldDataPath", m_config.m_strWorldDataPath))
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;

	//NAVI
	if (!file.Read("Navigator", "PathEngineDllName", m_config.m_strPathEngineDllName))
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	if (!file.Read("Navigator", "DataPath", m_config.m_strNavDataPath))
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	if (!file.Read("Navigator", "EnableNavigator", m_config.m_bEnableNavigator))
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;

	//GAME CONFIGS
	if (!file.Read("GAMECONFIG", "MaxLevel", m_config.MaxLevel))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("GAMECONFIG", "SoloExpRate", m_config.SoloExpRate))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("GAMECONFIG", "PartyExpRate", m_config.PartyExpRate))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("GAMECONFIG", "ItemDropRate", m_config.ItemDropRate))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("GAMECONFIG", "QuestExpRate", m_config.QuestExpRate))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("GAMECONFIG", "ZeniDropRate", m_config.ZeniDropRate))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("GAMECONFIG", "ZeniBonusRate", m_config.ZeniBonusRate))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("GAMECONFIG", "ZeniPartyBonusRate", m_config.ZeniPartyBonusRate))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}
	if (!file.Read("GAMECONFIG", "QuestMoneyRate", m_config.QuestMoneyRate))
	{
		return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CGameServer::OnCommandInput(std::string& sCmd)
{
	if (sCmd == "help")
	{
		printf("shutdown - Shutdown the server after 30 seconds \n");

		printf("\n");

		printf("playercount - return amount of players online\n");

		printf("\n");

		printf("startdbhunt - start dragonball hunt event\n");
		printf("stopdbhunt - stop dragonball hunt event\n");
		printf("startbossspawnevent - start boss spawn event\n");

		printf("\n");
	}
	else if (sCmd == "shutdown")
	{
		printf("Server shut down begin\n");
		g_pGameProcessor->StartServerShutdownEvent();
	}
	else if (sCmd == "playercount")
	{
		printf("Currently %zu players online \n", g_pObjectManager->GetPlayerCount());
	}
	else if (sCmd == "logwpsscript")
	{
		g_pScriptAlgoManager->LogAllActiveScripts();
	}
	else if (sCmd == "startdbhunt")
	{
		g_pDragonballHuntEvent->StartEvent();
		NTL_PRINT(PRINT_APP, "Dragonball Hunt Event Started");
	}
	else if (sCmd == "stopdbhunt")
	{
		g_pDragonballHuntEvent->EndEvent();
		NTL_PRINT(PRINT_APP, "Dragonball Hunt Event Stopped");
	}
	else if (sCmd == "dumpthreads")
	{
		tThreadFactory::Instance().AllThreadDump();
	}
	else if (sCmd == "StartAdultSolo")
	{
		g_pBudokaiManager->StartSoloAdultBudokai();
	}
	else if (sCmd == "StartAdultTeam")
	{
		g_pBudokaiManager->StartPartyAdultBudokai();
	}
	else if (sCmd == "StartJuniorSolo")
	{
		g_pBudokaiManager->StartSoloJuniorBudokai();
	}
	else if (sCmd == "StartJuniorTeam")
	{
		g_pBudokaiManager->StartPartyJuniorBudokai();
	}
	else if (sCmd == "startdbscramble")
	{
		g_pDragonballScramble->StartEvent();
		NTL_PRINT(PRINT_APP, "DragonballScramble Started");
	}
	else if (sCmd == "stopdbscramble")
	{
		g_pDragonballScramble->EndEvent(true);
		NTL_PRINT(PRINT_APP, "DragonballScramble Stopped");
	}
	else if (sCmd == "startbeeevent")
	{
		g_pHoneyBeeEvent->StartEvent();
		NTL_PRINT(PRINT_APP, "HoneybeeEvent Started");
	}
	else if (sCmd == "stopbeeevent")
	{
		g_pHoneyBeeEvent->EndEvent();
		NTL_PRINT(PRINT_APP, "HoneybeeEvent Stopped");
	}

	return TRUE;
}


void CGameServer::Init()
{
	m_dwCurTickCount = 0;
	m_dwLastTimePerformanceLogged = 0;
	m_dwLastTimeLoadReported = 0;
	m_dwLastTimeMemoryUseLogged = 0;
	m_pChatServerSession = NULL;
	m_pMasterServerSession = NULL;
	m_pQueryServerSession = NULL;
	m_pGameProcessor = NULL;
	m_pGameMain = NULL;
	m_pGameData = NULL;
	m_pActionPatternSystem = NULL;
}


void CGameServer::Destroy()
{
	SAFE_DELETE(m_pSessionFactory);

	m_pChatServerSession = NULL;
	m_pMasterServerSession = NULL;
	m_pQueryServerSession = NULL;

	SAFE_DELETE(m_pGameProcessor);
	SAFE_DELETE(m_pGameMain);
	SAFE_DELETE(m_pGameData);
}


void CGameServer::DoUpdatePerformanceLog(DWORD dwNow)
{
	if (dwNow - m_dwLastTimePerformanceLogged >= 10000)
	{
	//	m_performance.UpdateLog();

		m_dwLastTimePerformanceLogged = dwNow;
	}
}


void CGameServer::DoReportLoad(DWORD dwNow)
{
	if (dwNow - m_dwLastTimeLoadReported >= 10000)
	{
		if (m_dwLastTimeLoadReported)
		{
		//	ERR_LOG(LOG_SYSTEM, "GetProcessProcessorLoad() = %u, GetSystemProcessorLoad() = %u, GetProcessMemoryUsage() = %u", m_performance.GetProcessProcessorLoad(), m_performance.GetSystemProcessorLoad(), m_performance.GetProcessMemoryUsage());
			g_pServerInfoManager->SendMasterServerAlive();
		}

		m_dwLastTimeLoadReported = dwNow;
	}
}


void CGameServer::DoUpdateMemoryUseLog(DWORD dwNow)
{
	if (dwNow - m_dwLastTimeMemoryUseLogged >= 60000)
	{


		m_dwLastTimeMemoryUseLogged = dwNow;
	}
}


//-----------------------------------------------------------------------------------
//		GameServerMain
//-----------------------------------------------------------------------------------
int main(int argc, _TCHAR* argv[])
{
	CGameServer app;
	CNtlFileStream traceFileStream;

	SYSTEMTIME ti;
	GetLocalTime( &ti );

// CHECK INI FILE AND START PROGRAM
	int rc = app.Create(argc, argv, argv[1]);
	if( NTL_SUCCESS != rc )
		return rc;

	CNtlString consolename;
	consolename.Format("DBOG Game Server - Server %d Channel %d", app.GetGsServerId(), app.GetGsChannel());

	SetConsoleTitle(consolename.c_str());

	// LOG FILE
	char m_LogFile[256];
	sprintf(m_LogFile,"%s\\channel%u\\gamelog_%02u-%02u-%02u.txt", app.GetLogPath().c_str() ,app.GetGsChannel(), ti.wYear, ti.wMonth, ti.wDay);

	rc = traceFileStream.Create( m_LogFile );
	if( NTL_SUCCESS != rc )
		return rc;

	app.GetLog()->AttachLogStream(traceFileStream.GetFilePtr());


	NtlSetPrintFlag( PRINT_APP | PRINT_SYSTEM );

	app.Start();
	app.WaitCommandInput();
	app.WaitForTerminate();	

	return 0;
}
