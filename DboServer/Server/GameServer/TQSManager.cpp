#include "stdafx.h"
#include "TQSManager.h"

#include "ServerTimeQuestScript.h"

CTQSManager::CTQSManager()
{
}

CTQSManager::~CTQSManager()
{
}


bool CTQSManager::Create(const char* lpszScriptPath, eFILE_TYPE eLoadFileType, const char* lpszScriptFileExt, const char* lpszScriptPackFileExt, const char* lpszScriptEncryptFileExt)
{
	m_strPath = lpszScriptPath;
	m_strScriptFileExt = lpszScriptFileExt; //tqs
	m_strScriptPackFileExt = lpszScriptPackFileExt; //tqe
	m_strScriptEncryptFileExt = lpszScriptEncryptFileExt; //tqc

	if (CServerTimeQuestScript::LuaBind())
	{
		if (!Load(lpszScriptPath, eLoadFileType))
			return false;

		return true;
	}

	return false;
}

CControlScript* CTQSManager::CreateControlScript(const char* lpszScriptFileName)
{
	return new CServerTimeQuestScript(this, lpszScriptFileName);
}


bool CTQSManager::AttachScript(CControlState* pControlState, CControlStateFactory * pFactory, DWORD dwScriptNumber, DWORD dwStageNumber)
{
	CControlScript* pScript = FindScript(dwScriptNumber);
	if (pScript)
	{
		CServerTimeQuestScript* pServerScript = static_cast<CServerTimeQuestScript*>(pScript);
		if (pServerScript)
		{
			return pServerScript->AttachScript(pControlState, pFactory, dwStageNumber);
		}
		else ERR_LOG(LOG_SCRIPT, "CServerTimeQuestScript is NULL");
	}
	else ERR_LOG(LOG_SCRIPT, "CControlScript is NULL");

	return false;
}