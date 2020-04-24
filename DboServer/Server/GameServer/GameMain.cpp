#include "stdafx.h"
#include "GameMain.h"
#include "GameData.h"
#include "GameServer.h"
#include "ServerNaviLog.h"

CGameMain::CGameMain()
{
	Init();
}

CGameMain::~CGameMain()
{
	Destroy();
}


bool CGameMain::Create(CGameData* pGameData)
{
	m_pNaviLog = new CServerNaviLog;
	if (!m_pNaviLog)
	{
		ERR_LOG(LOG_GENERAL, "new CNpcServerNaviLog failed.");
		return false;
	}

	if (!PrepareNavi())
	{
		ERR_LOG(LOG_GENERAL, "PrepareNavi failed.");
		Destroy();
		return false;
	}

	NTL_PRINT(PRINT_APP, "Create World Manager");
	if (m_worldManager.Create(GetWorldFactory(), pGameData->GetTableContainer()->GetWorldTable(), pGameData->GetTableContainer()->GetWorldZoneTable()) != NTL_SUCCESS)
	{
		Destroy();
		return false;
	}

	//m_pSpawnLocaitonCache = new CSpawnLocationCache;
	//if (!m_pSpawnLocaitonCache)
	//{
	//	ERR_LOG(LOG_GENERAL, "NULL == pSpawnLocCache");
	//	Destroy();
	//	return false;
	//}

	return true;
}


void CGameMain::Destroy()
{
	//SAFE_DELETE(m_pSpawnLocaitonCache);
	m_objectManager.Destroy();
	m_worldManager.Destroy();
	DestroyNavi();
	SAFE_DELETE(m_pNaviLog);
}


bool CGameMain::PrepareWorldAndObject()
{
	if (m_objectManager.Create())
	{
		m_bIsActive = true;
		return true;
	}
	else
		ERR_LOG(LOG_GENERAL, "false == m_objectManager.Create");

	return false;
}


void CGameMain::TickProcess(DWORD dwTickDiff, float fMultiple, LARGE_INTEGER *rWorld, LARGE_INTEGER *rObject, LARGE_INTEGER *rSpawn)
{
	if (m_bIsActive)
	{
		m_worldManager.TickProcess(dwTickDiff, fMultiple);
		QueryPerformanceCounter(rWorld);
		m_objectManager.TickProcess(dwTickDiff, fMultiple);
		QueryPerformanceCounter(rObject);
	//	m_pSpawnLocaitonCache->TickProcess(dwTickDiff, fMultiple);
		QueryPerformanceCounter(rSpawn);
	}
	else
	{
		QueryPerformanceCounter(rWorld);
		QueryPerformanceCounter(rObject);
		QueryPerformanceCounter(rSpawn);
	}
}


void CGameMain::Init()
{
	m_bIsActive = false;
	m_pNaviLog = NULL;
	//m_pSpawnLocaitonCache = 0i64;
}


bool CGameMain::PrepareNavi()
{
	CGameServer* app = (CGameServer*)g_pApp;
	if (m_pNaviLog)
	{
		NTL_PRINT(PRINT_APP, "Preparing Navi Engine.. This could take a bit...");

		if (GetNaviEngine()->Create(m_pNaviLog, app->m_config.m_strPathEngineDllName.c_str(), false))
		{
			std::vector<TBLIDX> vecWorldList;
			DWORD dwOldMemoryUsage = GetNaviEngine()->GetTotalMemory();

			DWORD dwLoadingTime = GetTickCount();

			BYTE ePathDataLoadFlag = ePATH_DATA_LOAD_FLAG_LOAD_ALL;

			if (app->m_config.m_bEnableNavigator == false)
				ePathDataLoadFlag = ePATH_DATA_LOAD_FLAG_LOAD_ONLY_PROPERTY;

			if (GetNaviEngine()->LoadPathEngineData(app->m_config.m_strNavDataPath.c_str(), vecWorldList, ePathDataLoadFlag))
			{
				DWORD dwLoadingEndTime = GetTickCount();

				NTL_PRINT(PRINT_APP, "Memory used for NaviEngine data.[%u bytes]", GetNaviEngine()->GetTotalMemory() - dwOldMemoryUsage);

				DWORD timeLength = dwLoadingEndTime - dwLoadingTime;

				sNTL_TIME_LENGTH pTimeLength;

				if (ConvertTimeLength(GetTickCount() - dwLoadingTime, &pTimeLength))
				{
					NTL_PRINT(PRINT_APP, "Time spent for NaviEngine data.[%u millisecs(%u days %u hours %u mins %u.%03u secs)]", pTimeLength.wMillisecs, pTimeLength.wDays, pTimeLength.byHours, pTimeLength.byMins, pTimeLength.bySecs);
				}

				return true;
			}
			else
			{
				NTL_PRINT(PRINT_APP, "INtlNavi object failed to be initialized.(GetNaviEngine()->LoadPathEngineData()), app->m_config.m_strNavDataPath = %s", app->m_config.m_strNavDataPath.c_str());
			}
		}
		else
		{
			NTL_PRINT(PRINT_APP, "INtlNavi object failed to be initialized.(GetNaviEngine()->LoadPathEngineData()), app->m_config.m_strPathEngineDllName = %s", app->m_config.m_strPathEngineDllName.c_str());
		}
	}
	else
	{
		NTL_PRINT(PRINT_APP, "INtlNaviLog object is not prepared.(NULL == m_pNaviLog)");
	}

	return false;
}


void CGameMain::DestroyNavi()
{
	GetNaviEngine()->Delete();
}