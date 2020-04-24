#include "stdafx.h"
#include "TriggerManager.h"
#include "GameServer.h"

CTriggerManager::CTriggerManager()
{

}

CTriggerManager::~CTriggerManager()
{
}



void CTriggerManager::Init(BOOL bLoadEncrypt)
{
	CGameServer* app = (CGameServer*)g_pApp;
	if(bLoadEncrypt == TRUE)
	{
		m_strQuestPath.Format("%s\\quest.e", app->GetTsPath().c_str());
		m_strTriggerPath.Format("%s\\pctrigger.e", app->GetTsPath().c_str());
		m_strObjectPath.Format("%s\\objtrigger.e", app->GetTsPath().c_str());
		LoadAllEncTriggers();
	}
	else
	{
		m_strQuestPath.Format("%s\\quests", app->GetTsPath().c_str());
		m_strTriggerPath.Format("%s\\pctrigger", app->GetTsPath().c_str());
		m_strObjectPath.Format("%s\\objtrigger", app->GetTsPath().c_str());
		LoadAllTriggers();
	}

}

BOOL CTriggerManager::LoadAllEncTriggers()
{
	if(Create())
	{
		if ( !LoadTSCryptoData( m_strQuestPath.GetString(), m_defQuest ) )
		{
			ERR_LOG(LOG_ASSERT, "Failed loading Quest Triggers...");
			return FALSE;
		}

		if (!LoadTSCryptoData(m_strObjectPath.GetString(), m_defObject))
		{
			ERR_LOG(LOG_ASSERT, "Failed loading OBJ Triggers...");
			return FALSE;
		}

		if (!LoadTSCryptoData(m_strTriggerPath.GetString(), m_defTrigger))
		{
			ERR_LOG(LOG_ASSERT, "Failed loading PC Triggers...");
			return FALSE;
		}

	}
	else
	{
		ERR_LOG(LOG_ASSERT, "Failed to create CDboTSMain (TRIGGER SYSTEM)");
		return FALSE;
	}
	
	return TRUE;
}

BOOL CTriggerManager::LoadAllTriggers()
{
	if(Create())
	{
		if (!LoadTSPath(m_strQuestPath.GetString(), m_defQuest))
		{
			ERR_LOG(LOG_ASSERT, "Failed loading Quests...\n");
			return FALSE;
		}

		if (!LoadTSPath(m_strObjectPath.GetString(), m_defObject))
		{
			ERR_LOG(LOG_ASSERT, "Failed loading OBJ Trigger...\n");
			return FALSE;
		}

		if (!LoadTSPath(m_strTriggerPath.GetString(), m_defTrigger))
		{
			ERR_LOG(LOG_ASSERT,"Failed loading PC Trigger...\n");
			return FALSE;
		}

	}
	else
	{
		ERR_LOG(LOG_ASSERT, "Failed to create CDboTSMain (TRIGGER SYSTEM)\n");
		return FALSE;
	}
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------//
//		FIND QUEST FROM MAP
//--------------------------------------------------------------------------------------//
CNtlTSTrigger* CTriggerManager::FindQuestFromTS( QUESTID tId )
{
	CNtlTSMain::mapdef_TLIST::iterator it = m_defQuest.find(tId);

	if ( it == m_defQuest.end() )
	{
		NTL_PRINT(PRINT_APP,"Couldnt find quest id %d", tId);
		return NULL;
	}

	return it->second;
}

//--------------------------------------------------------------------------------------//
//		FIND OBJECT FROM MAP
//--------------------------------------------------------------------------------------//
CNtlTSTrigger* CTriggerManager::FindObjectFromTS( QUESTID tId )
{
	CNtlTSMain::mapdef_TLIST::iterator it = m_defObject.find(tId);

	if ( it == m_defObject.end() )
	{
	//	NTL_PRINT(PRINT_APP,"Couldnt find object id %d", tId);
		return NULL;
	}

	return it->second;
}

//--------------------------------------------------------------------------------------//
//		FIND PC TRIGGER FROM MAP
//--------------------------------------------------------------------------------------//
CNtlTSTrigger* CTriggerManager::FindPcFromTS( QUESTID tId )
{
	CNtlTSMain::mapdef_TLIST::iterator it = m_defTrigger.find(tId);

	if ( it == m_defTrigger.end() )
	{
		NTL_PRINT(PRINT_APP,"Couldnt find pc trigger id %d", tId);
		return NULL;
	}

	return it->second;
}