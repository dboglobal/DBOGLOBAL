#include "stdafx.h"
#include "ServerAIScript.h"

#include "AIScript.h"
#include "NtlLuaState.h"
#include "AISNodeState.h"


bool CServerAIScript::LuaBind()
{
	AISLua_Setup();

	return true;
}


CServerAIScript::CServerAIScript(CControlScriptManager* pManager, char const* lpszFileName)
: CControlScript(pManager, lpszFileName)
{
	Init();
}

CServerAIScript::~CServerAIScript()
{

}


void CServerAIScript::Init()
{
	m_sceneList.clear();
}


bool CServerAIScript::Load(const char* lpszScriptFile)
{
	s_pCurScript = this;
	s_pCurNode = NULL;

	if (GetAISLuaState()->RunScript(lpszScriptFile))
	{
		return true;
	}
	else
	{
		s_pCurScript = NULL;
		s_pCurNode = NULL;
		printf("AIS Run fail. script file[%s] \n", lpszScriptFile);
	}

	return false;
}

bool CServerAIScript::CheckIntegrity()
{
	for (std::map<DWORD, CAISNodeScene*>::iterator it = m_sceneList.begin(); it != m_sceneList.end(); it++)
	{
		CAISNodeScene* pScene = it->second;

		if (!pScene->CheckIntegrity())
			return false;
	}

	return true;
}


CAISNodeScene* CServerAIScript::Find(DWORD dwSceneNumber)
{
	std::map<DWORD, CAISNodeScene*>::iterator it = m_sceneList.find(dwSceneNumber);
	if (it == m_sceneList.end())
	{
		printf("CServerAIScript: Cant find dwScene %u from file %s \n", dwSceneNumber, GetFileName());
		return NULL;
	}

	return it->second;
}


bool CServerAIScript::AddScene(DWORD dwSceneNumber, CAISNodeScene * pScene)
{
	std::map<DWORD, CAISNodeScene*>::iterator it = m_sceneList.find(dwSceneNumber);
	if (it != m_sceneList.end())
	{
		printf("%s : Add Scene fail. SceneNumber[%d] [%s]\n", GetFileName(), dwSceneNumber, pScene->GetName());
		return false;
	}

	m_sceneList.insert(std::make_pair(dwSceneNumber, pScene));
	return true;
}

bool CServerAIScript::AttachScript(CControlState* pControlState, CControlStateFactory* pFactory, DWORD dwSceneNumber, eAIS_NODE eAttachAisNode)
{
	CAISNodeScene* pScene = Find(dwSceneNumber);
	if (pScene)
	{
		CAISNodeState* pState = pScene->FindState(eAttachAisNode);
		if (!pState || pState->AttachScript(pControlState, pFactory, true))
		{
			return true;
		}
		else
		{
			ERR_LOG(LOG_SCRIPT, "Attach Scene fail. SceneNumber[%u] And Attach State fail. StateId[%d]", dwSceneNumber, eAttachAisNode);
			return false;
		}
	}

	ERR_LOG(LOG_SCRIPT, "can not find scene. Scene Number[%u]", dwSceneNumber);
	return false;
}
