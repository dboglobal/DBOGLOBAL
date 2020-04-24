#include "stdafx.h"
#include "WPSManager.h"

#include "ServerWorldPlayScript.h"

CWPSManager::CWPSManager()
{
}

CWPSManager::~CWPSManager()
{
}


bool CWPSManager::Create(const char* lpszScriptPath, eFILE_TYPE eLoadFileType, const char* lpszScriptFileExt, const char* lpszScriptPackFileExt, const char* lpszScriptEncryptFileExt)
{
	m_strPath = lpszScriptPath;
	m_strScriptFileExt = lpszScriptFileExt; //sps
	m_strScriptPackFileExt = lpszScriptPackFileExt; //spe
	m_strScriptEncryptFileExt = lpszScriptEncryptFileExt; //spc

	if (CServerWorldPlayScript::LuaBind())
	{
		if (!Load(lpszScriptPath, eLoadFileType))
			return false;

		return true;
	}

	return false;
}


CControlScript* CWPSManager::CreateControlScript(const char* lpszScriptFileName)
{
	return new CServerWorldPlayScript(this, lpszScriptFileName);
}


bool CWPSManager::AttachScript(CControlState* pControlState, CControlStateFactory * pFactory, DWORD dwScriptNumber, DWORD dwStageNumber)
{
	CControlScript* pScript = FindScript(dwScriptNumber);
	if (pScript)
	{
		CServerWorldPlayScript* pServerPlayScript = static_cast<CServerWorldPlayScript*>(pScript);
		if (pServerPlayScript)
		{
			return pServerPlayScript->AttachScript(pControlState, pFactory, (BYTE)dwStageNumber);
		}
		else ERR_LOG(LOG_SCRIPT, "CServerPlayScript is NULL");
	}
	else ERR_LOG(LOG_SCRIPT, "CControlScript is NULL");

	return false;
}

CControlScriptNode * CWPSManager::GetWorldPlayScriptNode(DWORD scriptNumber, DWORD dwNodeIndex)
{
	CControlScript* pScript = FindScript(scriptNumber);
	if (pScript)
	{
		CServerWorldPlayScript* pServerPlayScript = static_cast<CServerWorldPlayScript*>(pScript);
		if (pServerPlayScript)
		{
			return pServerPlayScript->GetScriptNode(dwNodeIndex);
		}
		else ERR_LOG(LOG_SCRIPT, "CServerWorldPlayScript is NULL");
	}
	else ERR_LOG(LOG_SCRIPT, "* CWPSManager::GetPlayScriptScene: CControlScript is NULL");

	return nullptr;
}
