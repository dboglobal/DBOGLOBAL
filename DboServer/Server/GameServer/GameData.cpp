#include "stdafx.h"
#include "GameData.h"
#include "GameServer.h"


CGameData::CGameData()
{
	Init();
}

CGameData::~CGameData()
{
	Destroy();
}


bool CGameData::Create(DWORD dwCodePage)
{
	CGameServer* app = (CGameServer*)g_pApp;

	NTL_PRINT(PRINT_APP, "Create Table Container");
	if (!m_tableContainer.CreateTableContainer(app->m_config.LoadTableFormat, (char*)app->m_config.TablePath.c_str()))
	{
		ERR_LOG(LOG_GENERAL, "CreateTableContainer == false");
		Destroy();
		return false;
	}

	if (app->IsDojoChannel() == false) //check if not dojo channel
	{
		NTL_PRINT(PRINT_APP, "Init Play Script Manager");
		if (!m_playScriptManager.Create(app->m_config.strPlayScriptPath.c_str(), CControlScriptManager::FILE_SCRIPT, "sps", "spe", "spc"))
		{
			ERR_LOG(LOG_GENERAL, "m_playScriptManager.Create == false");
			return false;
		}

		NTL_PRINT(PRINT_APP, "Init AI Script Manager");
		if (!m_AiScriptManager.Create(app->m_config.strAIScriptPath.c_str(), CControlScriptManager::FILE_SCRIPT, "ais", "aie", "aic"))
		{
			ERR_LOG(LOG_GENERAL, "m_AiScriptManager.Create == false");
			return false;
		}

		NTL_PRINT(PRINT_APP, "Init Time Quest Script Manager");
		if (!m_timeQuestScriptManager.Create(app->m_config.strTimeQuestScriptPath.c_str(), CControlScriptManager::FILE_SCRIPT, "tqs", "tqe", "tqc"))
		{
			ERR_LOG(LOG_GENERAL, "m_timeQuestScriptManager.Create == false");
			return false;
		}

		NTL_PRINT(PRINT_APP, "Init World Play Script Manager");
		if (!m_WorldPlayScriptManager.Create(app->m_config.strWorldPlayScriptPath.c_str(), CControlScriptManager::FILE_SCRIPT, "wps", "wpe", "wpc"))
		{
			ERR_LOG(LOG_GENERAL, "m_WorldPlayScriptManager.Create == false");
		}
	}

	return true;
}


void CGameData::Destroy()
{
	m_tableContainer.Destroy();
	m_playScriptManager.Destroy();
	m_AiScriptManager.Destroy();
	m_timeQuestScriptManager.Destroy();
	m_WorldPlayScriptManager.Destroy();
}


void CGameData::Init()
{
}
