#include "stdafx.h"
#include "ControlScriptManager.h"


CControlScriptManager::CControlScriptManager()
{

}

CControlScriptManager::~CControlScriptManager()
{

}

bool CControlScriptManager::Load(const char* lpszScriptPath, eFILE_TYPE eLoadFileType)
{
	if (lpszScriptPath == NULL)
	{
		printf("script path is null.");
		return false;
	}

	std::string strSearchSpec = lpszScriptPath;
	std::string strPath = lpszScriptPath;

	strSearchSpec += "/";
	strPath += "/";

	if (eLoadFileType == FILE_SCRIPT)
	{
		strPath += m_strScriptFileExt.c_str();

		strSearchSpec += m_strScriptFileExt.c_str(); // example: path + ais = path/ais
		strSearchSpec += "/*."; // example: path + ais + *, = path/ais/*.
		strSearchSpec += m_strScriptFileExt.c_str(); // example: "path" + "ais" + "/*," + "ais" = path/ais/*.ais
	}

	if (LoadFile(strPath.c_str(), strSearchSpec.c_str(), NULL))
		return CheckScript();
	else
		return false;
}


void CControlScriptManager::Unload()
{

}


bool CControlScriptManager::Reload()
{
	return true;
}


bool CControlScriptManager::AttachScript(CControlState* pControlState, CControlStateFactory* pFactory, DWORD dwScriptNumber)
{
	CControlScript* pScript = FindScript(dwScriptNumber);
	if (!pScript)
	{
		ERR_LOG(LOG_SCRIPT, "Could not find script %u", dwScriptNumber);
		return false;
	}

	return pScript->AttachScript(pControlState, pFactory);
}


bool CControlScriptManager::LoadScript(const char* lpszScriptPath, const char* lpszScriptFile, const char* lpszSSEPath)
{
	std::string strFullName = lpszScriptFile;
	std::string strFileName = strFullName.substr(0, strFullName.find_last_of("."));
	std::string strExtName = strFullName.substr(strFullName.find_last_of(".")+1, 3);

	//if (strFileName != "999999")
	//	return true;

	//printf("load script: strFullName [%s] strFileName [%s] strExtName [%s]\n", strFullName.c_str(), strFileName.c_str(), strExtName.c_str());

	bool bResult = false;

	if (strExtName.compare("sps") == 0 || strExtName.compare("wps") == 0 || strExtName.compare("tqs") == 0 || strExtName.compare("ais") == 0)
		bResult = LoadScriptFile(lpszScriptPath, lpszScriptFile, lpszSSEPath);
	else if (strExtName.compare("spe") == 0 || strExtName.compare("wpe") == 0 || strExtName.compare("tqe") == 0 || strExtName.compare("aie") == 0)
		bResult = LoadScriptPackFile(lpszScriptPath, strFileName.c_str());
	else if (strExtName.compare("spc") == 0 || strExtName.compare("wpc") == 0 || strExtName.compare("tqc") == 0 || strExtName.compare("aic") == 0)
		bResult = LoadScriptEncryptFile(lpszScriptPath, strFileName.c_str());

	return bResult;
}


CControlScript* CControlScriptManager::FindScript(DWORD dwScriptNumber)
{
	SCRIPTLIST::iterator it = m_scriptList.find(dwScriptNumber);
	if (it == m_scriptList.end())
		return NULL;

	return it->second;
}



bool CControlScriptManager::LoadScriptFile(const char* lpszScriptPath, const char* lpszFileName, const char* lpszSSEPath)
{
	std::string strFullName = lpszScriptPath;
	strFullName += "/";
	strFullName += lpszFileName;

	std::string strFileName = lpszFileName;
	strFileName = strFileName.substr(0, strFileName.find_last_of("."));

	DWORD dwScriptNumber = std::stoul(strFileName);

	//printf("LoadScriptFile: dwScriptNumber [%u] strFullName[%s] strFileName[%s]\n", dwScriptNumber, strFullName.c_str(), strFileName.c_str());

	if (m_scriptList.find(dwScriptNumber) != m_scriptList.end())
	{
		printf("LoadScriptFile: dwScriptNumber [%u] already loaded. Path [%s] \n", dwScriptNumber, lpszScriptPath);
		return false;
	}

	CControlScript* pControlScript = CreateControlScript(strFileName.c_str());
	if (pControlScript)
	{
		pControlScript->SetSSEPath(lpszSSEPath);

		if (pControlScript->Load(strFullName.c_str()))
		{
			m_scriptList.insert(std::make_pair(dwScriptNumber, pControlScript));
			return true;
		}
	}

	delete pControlScript;

	return false;
}


bool CControlScriptManager::CheckScript()
{
	for (SCRIPTLIST::iterator it = m_scriptList.begin(); it != m_scriptList.end(); it++)
	{
		CControlScript* pScript = it->second;

		if (!pScript->CheckIntegrity())
		{
			ERR_LOG(LOG_SCRIPT, "The contents of the script file [%s] is not correct.", pScript->GetFileName());
			return false;
		}
	}

	return true;
}


bool CControlScriptManager::LoadFile(const char* lpszScriptPath, const char* lpszSearchFileName, const char* lpszSSEPath)
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA FindFileData;

	hFind = FindFirstFile(lpszSearchFileName, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		printf("Can not find the scripts to load. Info[%s]. \n", lpszSearchFileName);
		return false;
	}

	do
	{
		if (FILE_ATTRIBUTE_DIRECTORY != (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			'.' != FindFileData.cFileName[0])
		{
			LoadScript(lpszScriptPath, FindFileData.cFileName, lpszSSEPath);
		}
	} while (FindNextFile(hFind, &FindFileData) != 0);

	FindClose(hFind);

	return true;
}

