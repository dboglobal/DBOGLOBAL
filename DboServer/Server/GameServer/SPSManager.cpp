#include "stdafx.h"
#include "SPSManager.h"

#include "ServerPlayScript.h"
#include "SPSNodeCondition_RegisterBattleScene.h"

CSPSManager::CSPSManager()
{

}

CSPSManager::~CSPSManager()
{

}


bool CSPSManager::Create(const char* lpszScriptPath, eFILE_TYPE eLoadFileType, const char* lpszScriptFileExt, const char* lpszScriptPackFileExt, const char* lpszScriptEncryptFileExt)
{
	m_strPath = lpszScriptPath;
	m_strScriptFileExt = lpszScriptFileExt; //sps
	m_strScriptPackFileExt = lpszScriptPackFileExt; //spe
	m_strScriptEncryptFileExt = lpszScriptEncryptFileExt; //spc

	if (CServerPlayScript::LuaBind())
	{
		if (Load(lpszScriptPath, eLoadFileType))
			return true;
		else
			ERR_LOG(LOG_SCRIPT, "CSPSManager::Create fail");

		return false;
	}

	return false;
}

CControlScript* CSPSManager::CreateControlScript(const char* lpszScriptFileName)
{
	return new CServerPlayScript(this, lpszScriptFileName);
}


bool CSPSManager::AttachScript(CControlState* pControlState, CControlStateFactory * pFactory, DWORD dwScriptNumber, DWORD dwSceneNumber)
{
	CControlScript* pScript = FindScript(dwScriptNumber);
	if (pScript)
	{
		CServerPlayScript* pServerPlayScript = static_cast<CServerPlayScript*>(pScript);
		if (pServerPlayScript)
		{
			return pServerPlayScript->AttachScript(pControlState, pFactory, dwSceneNumber);
		}
		else ERR_LOG(LOG_SCRIPT, "CServerPlayScript is NULL");
	}
	else ERR_LOG(LOG_SCRIPT, "CControlScript is NULL");

	return false;
}

CSPSNodeCondition_RegisterBattleScene * CSPSManager::GetFirstRegBattleScene(unsigned int scriptNumber, DWORD dwSceneNumber)
{
	CControlScriptNode* pNode = GetPlayScriptScene(scriptNumber, dwSceneNumber);
	if (pNode)
	{
		CSPSNodeCondition_RegisterBattleScene* pScene = (CSPSNodeCondition_RegisterBattleScene*)pNode->GetChildNode(SPS_NODE_CONDITION_REGISTER_BATTLE_SCENE);
		return pScene;
	}

	return nullptr;
}

CSPSNodeCondition_RegisterBattleScene * CSPSManager::GetNextRegBattleScene(CSPSNodeCondition_RegisterBattleScene * pBattleScene)
{
	if (pBattleScene)
	{
		int nId = pBattleScene->GetID();

		for (CSPSNodeCondition_RegisterBattleScene* j = (CSPSNodeCondition_RegisterBattleScene*)pBattleScene->GetNext();
			j;
			j = (CSPSNodeCondition_RegisterBattleScene*)j->GetNext())
		{
			if (j->GetID() == nId)
				return j;
		}
	}

	return nullptr;
}


CSPSNodeScene* CSPSManager::GetPlayScriptScene(DWORD scriptNumber, DWORD dwSceneNumber)
{
	CControlScript* pScript = FindScript(scriptNumber);
	if (pScript)
	{
		CServerPlayScript* pServerPlayScript = static_cast<CServerPlayScript*>(pScript);
		if (pServerPlayScript)
		{
			return pServerPlayScript->Find(dwSceneNumber);
		}
		else ERR_LOG(LOG_SCRIPT,"CServerPlayScript is NULL");
	}
	else ERR_LOG(LOG_SCRIPT, "CControlScript is NULL");

	return NULL;
}

CControlScriptNode* CSPSManager::GetPlayScriptNode(DWORD scriptNumber, DWORD dwNodeIndex)
{
	CControlScript* pScript = FindScript(scriptNumber);
	if (pScript)
	{
		CServerPlayScript* pServerPlayScript = static_cast<CServerPlayScript*>(pScript);
		if (pServerPlayScript)
		{
			return pServerPlayScript->GetScriptNode(dwNodeIndex);
		}
		else ERR_LOG(LOG_SCRIPT, "CServerPlayScript is NULL");
	}
	else ERR_LOG(LOG_SCRIPT, "CSPSManager::GetPlayScriptScene: CControlScript is NULL");

	return NULL;
}