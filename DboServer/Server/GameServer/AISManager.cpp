#include "stdafx.h"
#include "AISManager.h"

#include "ServerAIScript.h"


CAISManager::CAISManager()
{

}

CAISManager::~CAISManager()
{

}

bool CAISManager::Create(const char* lpszScriptPath, eFILE_TYPE eLoadFileType, const char* lpszScriptFileExt, const char* lpszScriptPackFileExt, const char* lpszScriptEncryptFileExt)
{
	m_strPath = lpszScriptPath;
	m_strScriptFileExt = lpszScriptFileExt; //ais
	m_strScriptPackFileExt = lpszScriptPackFileExt; //aie
	m_strScriptEncryptFileExt = lpszScriptEncryptFileExt; //aic

	if (CServerAIScript::LuaBind())
	{
		if (!Load(lpszScriptPath, eLoadFileType))
			return false;

		return true;
	}

	return false;
}

CControlScript* CAISManager::CreateControlScript(const char* lpszScriptFileName)
{
	return new CServerAIScript(this, lpszScriptFileName);
}

bool CAISManager::AttachScript(CControlState* pControlState, CControlStateFactory* pFactory, DWORD dwScriptNumber, DWORD dwSceneNumber, eAIS_NODE eAttachAisNode)
{
	CControlScript* pScript = FindScript(dwScriptNumber);
	if (pScript)
	{

		CServerAIScript* pAiScript = dynamic_cast<CServerAIScript *>(pScript);
		if (pAiScript)
		{
			if(pAiScript->AttachScript(pControlState, pFactory, dwSceneNumber, eAttachAisNode))
				return true;
			else
			{
				ERR_LOG(LOG_SCRIPT, "AIS Attach fail. script number[%u] dwSceneNumber [%u]", dwScriptNumber, dwSceneNumber);
				return false;
			}
		}
		else
		{
			ERR_LOG(LOG_SCRIPT, "cant cast CControlScript to CServerAiScript. script number[%u] dwSceneNumber [%u]", dwScriptNumber, dwSceneNumber);
			return false;
		}
	}

	ERR_LOG(LOG_SCRIPT, "can not find AIS script. script number[%u]", dwScriptNumber);
	return false;
}