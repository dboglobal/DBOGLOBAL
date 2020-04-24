#include "stdafx.h"
#include "ScriptAlgoObjectManager.h"
#include "WpsAlgoObject.h"
#include "TqsAlgoObject.h"
#include "TableContainerManager.h"
#include "World.h"
#include "WorldPlayScriptManager.h"
#include "TimeQuestScriptManager.h"
#include "CPlayer.h"


CScriptAlgoObjectManager::CScriptAlgoObjectManager()
{
	m_dwHandle = 0;
	m_setScriptObjects.clear();
}

CScriptAlgoObjectManager::~CScriptAlgoObjectManager()
{
}



void CScriptAlgoObjectManager::TickProcess(DWORD dwTickDiff)
{
	for (std::vector<CScriptAlgoObject*>::iterator it = m_setRemoveScriptObjects.begin(); it != m_setRemoveScriptObjects.end(); it++)
	{
		CScriptAlgoObject* pScript = *it;
		
		m_setScriptObjects.erase(pScript->GetHandle());
		m_vecHandle.push_back(pScript->GetHandle());
		SAFE_DELETE(pScript);
	}
	m_setRemoveScriptObjects.clear();

	for (std::map<DWORD, CScriptAlgoObject*>::iterator it = m_setScriptObjects.begin(); it != m_setScriptObjects.end(); it++)
	{
		CScriptAlgoObject* pObject = it->second;
		if (!pObject->IsPaused())
			pObject->OnUpdate(dwTickDiff);
	}
}

DWORD CScriptAlgoObjectManager::GenerateHandle()
{
	DWORD dwHandle = INVALID_DWORD;

	if (m_vecHandle.size() > 0)
	{
		dwHandle = m_vecHandle.front();
		m_vecHandle.erase(m_vecHandle.begin());
	}
	else
		dwHandle = ++m_dwHandle;

	if (dwHandle == INVALID_DWORD)
	{
		ERR_LOG(LOG_SCRIPT, "ERROR: FAILED TO CREATE HANDLE !!!");
	}
	
	return dwHandle;
}


CWpsAlgoObject* CScriptAlgoObjectManager::CreateWpsObject(TBLIDX scriptTblidx, bool & rbStart, CPlayer* pPlayer/* = NULL*/)
{
	sWORLDPLAY_TBLDAT* pWorldPlayTbldat = (sWORLDPLAY_TBLDAT*)g_pTableContainer->GetWorldPlayTable()->FindData(scriptTblidx); //check if script exist in table
	if (pWorldPlayTbldat == NULL)
	{
		ERR_LOG(LOG_SYSTEM, "ERROR: WPS-Script %u does not exist in table\n", scriptTblidx);
		return NULL;
	}

	if (g_pWorldPlayScriptManager->FindScript(scriptTblidx) == NULL)
	{
		ERR_LOG(LOG_SYSTEM, "ERROR: WPS-Script %u is not loaded\n", scriptTblidx);
		return NULL;
	}

	CWpsAlgoObject* pObject = NULL;

	pObject = GetWpsObject(scriptTblidx); //check if script already exist and is shared
	if (pObject)
	{
		if (pObject->GetExecuterType() == WORLDPLAY_EXECUTER_TS && pObject->GetWorld() && pObject->GetWorld()->GetTbldat()->bDynamic == false) //only share if not in dynamic(dungeon) world. If share in dungeon then summoning the objects at master class(wps 6005) does not work
			return pObject;
		else if(pPlayer && pPlayer->GetParty() && pPlayer->GetTMQ()) // To avoid quest start multiple WPS script for TMQ.
		{
			if (pPlayer->GetParty()->MemberHasScript((DWORD)scriptTblidx, SSD_SCRIPT_TYPE_WPS))
				return pObject;
		}
	}

	DWORD dwHandle = GenerateHandle();
	if (dwHandle == INVALID_DWORD)
		return NULL;

	pObject = new CWpsAlgoObject(scriptTblidx, pWorldPlayTbldat, dwHandle);
	if (pObject == NULL)
		return NULL;
	
	rbStart = true;
//	ERR_LOG(LOG_SYSTEM, "CScriptAlgoObjectManager::CreateWpsObject: scriptTblidx %u \n", scriptTblidx);
	m_setScriptObjects.insert({ dwHandle, pObject });

	return pObject;
}


CTqsAlgoObject * CScriptAlgoObjectManager::CreateTqsObject(TBLIDX scriptTblidx, sTIMEQUEST_TBLDAT* pTimeQuestTbldat)
{
	if (g_pTimeQuestScriptManager->FindScript(scriptTblidx) == NULL)
	{
		ERR_LOG(LOG_SYSTEM, "ERROR: TQS-Script %u is not loaded\n", scriptTblidx);
		return NULL;
	}

	CTqsAlgoObject* pObject = NULL;

	DWORD dwHandle = GenerateHandle();
	if (dwHandle == INVALID_DWORD)
		return NULL;

	pObject = new CTqsAlgoObject(scriptTblidx, pTimeQuestTbldat, dwHandle);
	if (pObject == NULL)
		return NULL;

	
	//ERR_LOG(LOG_SYSTEM, "CScriptAlgoObjectManager::CreateTqsObject: scriptTblidx %u \n", scriptTblidx);
	m_setScriptObjects.insert({ dwHandle, pObject });

	return pObject;
}


void CScriptAlgoObjectManager::DestroyScript(CScriptAlgoObject* pScript)
{
	if (pScript)
	{
		if (pScript->GetPlayersCount() > 0)
			ERR_LOG(LOG_GENERAL, "IMPORTANT-ERROR: TRYING TO DELETE SCRIPT WHILE PLAYERS STILL INSIDE");
		m_setRemoveScriptObjects.push_back(pScript);
	}
}


CWpsAlgoObject* CScriptAlgoObjectManager::GetWpsObject(TBLIDX scriptTblidx)
{
	for (std::map<DWORD, CScriptAlgoObject*>::iterator it = m_setScriptObjects.begin(); it != m_setScriptObjects.end(); it++)
	{
		CWpsAlgoObject* pObject = (CWpsAlgoObject*)it->second;
		if (pObject)
		{
			if (pObject->GetScriptID() == scriptTblidx)
			{
				if (pObject->GetShareType() == WORLDPLAY_SHARE_PARTY) //return script only if it is shared
				{
					return pObject;
				}

				break;
			}
		}
	}

	return NULL;
}

CScriptAlgoObject * CScriptAlgoObjectManager::Find(DWORD dwHandle)
{
	std::map<DWORD, CScriptAlgoObject*>::iterator it = m_setScriptObjects.find(dwHandle);
	if (it != m_setScriptObjects.end())
		return it->second;

	return NULL;
}

#include "World.h"
void CScriptAlgoObjectManager::LogAllActiveScripts()
{
	for (std::map<DWORD, CScriptAlgoObject*>::iterator it = m_setScriptObjects.begin(); it != m_setScriptObjects.end(); it++)
	{
		CScriptAlgoObject* pObject = it->second;
		if (pObject)
		{
			if(pObject->GetWorld())
				ERR_LOG(LOG_GENERAL, "WPS: %u Running. Playercount %u, WorldTblidx %u, WorldID %u", pObject->GetScriptID(), pObject->GetPlayersCount(), pObject->GetWorld()->GetIdx(), pObject->GetWorld()->GetID());
			else
				ERR_LOG(LOG_GENERAL, "WPS: %u Running. Playercount %u", pObject->GetScriptID(), pObject->GetPlayersCount());
		}
	}
}
