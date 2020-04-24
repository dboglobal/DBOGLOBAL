#include "stdafx.h"
#include "ServerWorldPlayScript.h"

#include "WPSScript.h"
#include "NtlLuaState.h"


bool CServerWorldPlayScript::LuaBind()
{
	WPSLua_Setup();

	return true;
}


CServerWorldPlayScript::CServerWorldPlayScript(CControlScriptManager* pManager, char const* lpszFileName)
: CControlScript(pManager, lpszFileName)
{
	Init();
}

CServerWorldPlayScript::~CServerWorldPlayScript()
{

}

void CServerWorldPlayScript::Init()
{
	m_gameStageList.clear();
}


bool CServerWorldPlayScript::Load(const char* lpszScriptFile)
{
	s_pCurScriptWPS = this;
	s_pCurNodeWPS = NULL;

	if (GetWPSLuaState()->RunScript(lpszScriptFile))
	{
		if (s_pCurNodeWPS)
		{
			CControlScriptNode* pNode = NULL;

			if (s_pCurNodeWPS->GetParent())
				pNode = s_pCurNodeWPS->GetParent();
			else
				pNode = s_pCurNodeWPS;

			printf("CServerWorldPlayScript: %s: The node is not over. (Make sure that the node that is to be closed, End().) \n", pNode->GetName());

			return false;
		}

		return true;
	}
	else
	{
		s_pCurScriptWPS = NULL;
		s_pCurNodeWPS = NULL;
		printf("WPS Run fail. script file[%s] \n", lpszScriptFile);
	}

	return false;
}

bool CServerWorldPlayScript::CheckIntegrity()
{
	for (std::map<BYTE, CControlScriptNodeState*>::iterator it = m_gameStageList.begin(); it != m_gameStageList.end(); it++)
	{
		CControlScriptNodeState* pStage = it->second;

		if (!pStage->CheckIntegrity())
			return false;
	}

	return true;
}


CControlScriptNodeState* CServerWorldPlayScript::Find(BYTE byStageNumber)
{
	std::map<BYTE, CControlScriptNodeState*>::iterator it = m_gameStageList.find(byStageNumber);
	if (it == m_gameStageList.end())
	{
	//	printf("CServerWorldPlayScript: Cant find dwStageNumber %u from file %s \n", byStageNumber, GetFileName());
		return NULL;
	}

	return it->second;
}

bool CServerWorldPlayScript::AddStage(BYTE byStageNumber, CControlScriptNodeState * pScene)
{
	std::map<BYTE, CControlScriptNodeState*>::iterator it = m_gameStageList.find(byStageNumber);
	if (it != m_gameStageList.end())
	{
		printf("CServerWorldPlayScript: %s : Add Stage fail. StageNumber[%d]\n", GetFileName(), byStageNumber);
		return false;
	}

	m_gameStageList.insert(std::make_pair(byStageNumber, pScene));
	return true;
}


bool CServerWorldPlayScript::AttachScript(CControlState* pControlState, CControlStateFactory* pFactory, BYTE byStageNumber)
{
	CControlScriptNodeState* pScene = Find(byStageNumber);
	if (pScene)
	{
		if (!pScene->AttachScript(pControlState, pFactory, false))
		{
			ERR_LOG(LOG_SCRIPT, "Attach stage fail. StageNumber[%u]", byStageNumber);
			return false;
		}

		return true;
	}

	return false;
}


