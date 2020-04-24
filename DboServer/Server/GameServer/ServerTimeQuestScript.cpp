#include "stdafx.h"
#include "ServerTimeQuestScript.h"

#include "TQSScript.h"
#include "NtlLuaState.h"



bool CServerTimeQuestScript::LuaBind()
{
	TQSLua_Setup();

	return true;
}


CServerTimeQuestScript::CServerTimeQuestScript(CControlScriptManager* pManager, char const* lpszFileName)
: CControlScript(pManager, lpszFileName)
{
	Init();
}

CServerTimeQuestScript::~CServerTimeQuestScript()
{

}


void CServerTimeQuestScript::Init()
{
	m_gameStageList.clear();
}

bool CServerTimeQuestScript::Load(const char* lpszScriptFile)
{
	s_pCurScriptTQS = this;
	s_pCurNodeTQS = NULL;

	if (GetTQSLuaState()->RunScript(lpszScriptFile))
	{
		if (s_pCurNodeTQS)
		{
			CControlScriptNode* pNode = NULL;

			if (s_pCurNodeTQS->GetParent())
				pNode = s_pCurNodeTQS->GetParent();
			else
				pNode = s_pCurNodeTQS;

			printf("CServerTimeQuestScript: %s: The node is not over. (Make sure that the node that is to be closed, End().) \n", pNode->GetName());

			return false;
		}

		return true;
	}
	else
	{
		s_pCurScriptTQS = NULL;
		s_pCurNodeTQS = NULL;
		printf("TQS Run fail. script file[%s] \n", lpszScriptFile);
	}

	return false;
}

bool CServerTimeQuestScript::CheckIntegrity()
{
	for (std::map<DWORD, CControlScriptNodeState*>::iterator it = m_gameStageList.begin(); it != m_gameStageList.end(); it++)
	{
		CControlScriptNodeState* pStage = it->second;

		if (!pStage->CheckIntegrity())
			return false;
	}

	return true;
}


CControlScriptNodeState* CServerTimeQuestScript::Find(DWORD dwStageNumber)
{
	std::map<DWORD, CControlScriptNodeState*>::iterator it = m_gameStageList.find(dwStageNumber);
	if (it == m_gameStageList.end())
	{
	//	printf("CServerTimeQuestScript: Cant find dwStageNumber %u from file %s \n", dwStageNumber, GetFileName());
		return NULL;
	}

	return it->second;
}

bool CServerTimeQuestScript::AddStage(DWORD dwStageNumber, CControlScriptNodeState * pScene)
{
	std::map<DWORD, CControlScriptNodeState*>::iterator it = m_gameStageList.find(dwStageNumber);
	if (it != m_gameStageList.end())
	{
		printf("CServerTimeQuestScript: %s : Add Stage fail. StageNumber[%d]\n", GetFileName(), dwStageNumber);
		return false;
	}

	m_gameStageList.insert(std::make_pair(dwStageNumber, pScene));
	return true;
}


bool CServerTimeQuestScript::AttachScript(CControlState* pControlState, CControlStateFactory* pFactory, DWORD dwStageNumber)
{
	CControlScriptNodeState* pScene = Find(dwStageNumber);
	if (pScene)
	{
		if (!pScene->AttachScript(pControlState, pFactory, false))
		{
			ERR_LOG(LOG_SCRIPT, "Attach stage fail. StageNumber[%u]", dwStageNumber);
			return false;
		}

		return true;
	}
	else ERR_LOG(LOG_SCRIPT, "can not find stage. Stage Number[%u]", dwStageNumber);

	return false;
}