#include "stdafx.h"
#include "ServerPlayScript.h"

#include "SPSScript.h"
#include "NtlLuaState.h"



bool CServerPlayScript::LuaBind()
{
	SPSLua_Setup();

	return true;
}


CServerPlayScript::CServerPlayScript(CControlScriptManager* pManager, char const* lpszFileName)
: CControlScript(pManager, lpszFileName)
{
	Init();
}

CServerPlayScript::~CServerPlayScript()
{

}


void CServerPlayScript::Init()
{
	m_sceneList.clear();
}

bool CServerPlayScript::Load(const char* lpszScriptFile)
{
	s_pCurScriptSPS = this;
	s_pCurNodeSPS = NULL;

	if (GetSPSLuaState()->RunScript(lpszScriptFile))
	{
		if (s_pCurNodeSPS)
		{
			CControlScriptNode* pNode = NULL;

			if (s_pCurNodeSPS->GetParent())
				pNode = s_pCurNodeSPS->GetParent();
			else
				pNode = s_pCurNodeSPS;

			printf("%s: The node is not over. (Make sure that the node that is to be closed, End().) \n", pNode->GetName());

			return false;
		}

		return true;
	}
	else
	{
		s_pCurScriptSPS = NULL;
		s_pCurNodeSPS = NULL;
		printf("SPS Run fail. script file[%s] \n", lpszScriptFile);
	}

	return false;
}

bool CServerPlayScript::CheckIntegrity()
{
	for (std::map<DWORD, CSPSNodeScene*>::iterator it = m_sceneList.begin(); it != m_sceneList.end(); it++)
	{
		CSPSNodeScene* pScene = it->second;

		if (!pScene->CheckIntegrity())
			return false;
	}

	return true;
}

CSPSNodeScene* CServerPlayScript::Find(DWORD dwSceneNumber)
{
	std::map<DWORD, CSPSNodeScene*>::iterator it = m_sceneList.find(dwSceneNumber);
	if (it == m_sceneList.end())
	{
		printf("CServerPlayScript: Cant find dwScene %u from file %s \n", dwSceneNumber, GetFileName());
		return NULL;
	}

	return it->second;
}


bool CServerPlayScript::AddScene(DWORD dwSceneNumber, CSPSNodeScene * pScene)
{
	std::map<DWORD, CSPSNodeScene*>::iterator it = m_sceneList.find(dwSceneNumber);
	if (it != m_sceneList.end())
	{
		printf("CServerPlayScript: %s : Add Scene fail. SceneNumber[%d]\n", GetFileName(), dwSceneNumber);
		return false;
	}

	m_sceneList.insert(std::make_pair(dwSceneNumber, pScene));
	return true;
}

bool CServerPlayScript::AttachScript(CControlState* pControlState, CControlStateFactory* pFactory, DWORD dwSceneNumber)
{
	CSPSNodeScene* pScene = Find(dwSceneNumber);
	if (pScene)
	{
		if (!pScene->AttachScript(pControlState, pFactory, true))
		{
			ERR_LOG(LOG_SCRIPT, "Attach Scene fail. SceneNumber[%u]", dwSceneNumber);
			return false;
		}

		return true;
	}
	else ERR_LOG(LOG_SCRIPT, "can not find scene. Scene Number[%u]", dwSceneNumber);

	return false;
}