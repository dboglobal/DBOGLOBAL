#include "stdafx.h"

#include "ServerAIScript.h"
#include "ControlScriptManager.h"
#include "ControlScriptNodeFactory.h"



CControlScript::CControlScript(CControlScriptManager * pManager, const char* lpszFileName)
{
	m_pScriptManager = pManager;
	m_strScriptFileName.operator=(lpszFileName);
	m_strSSEPath.clear();
	m_mapScriptNode.clear();
}

CControlScript::~CControlScript()
{
	m_mapScriptNode.clear();
	m_strSSEPath.clear();
	m_strScriptFileName.clear();
}


CControlScriptNodeAction* CControlScript::CreateControlScriptNodeAction(const char* lpszScriptNodeName)
{
	if (m_pScriptManager && m_pScriptManager->GetControlScriptNodeFactory())
		return m_pScriptManager->GetControlScriptNodeFactory()->CreateControlNodeAction(lpszScriptNodeName);

	return NULL;
}

CControlScriptNodeCondition* CControlScript::CreateControlScriptNodeCondition(const char* lpszScriptNodeName)
{
	if (m_pScriptManager && m_pScriptManager->GetControlScriptNodeFactory())
		return m_pScriptManager->GetControlScriptNodeFactory()->CreateControlNodeCondition(lpszScriptNodeName);

	return NULL;
}

bool CControlScript::Save()
{
	return true;
}
bool CControlScript::AttachScript(CControlState * pControlState, CControlStateFactory * pFactory)
{
	return true;
}
bool CControlScript::Load(char * achBuffer)
{
	return true;
}
bool CControlScript::Load(const char* lpszScriptFile)
{
	return true;
}
bool CControlScript::CheckIntegrity()
{
	return true;
}


bool CControlScript::AddScriptNode(CControlScriptNode * pScriptNode)
{
	DWORD dwNodeIndex = GetScriptNodeCount();

	std::map<DWORD, CControlScriptNode *>::iterator it = m_mapScriptNode.find(dwNodeIndex);
	if (it != m_mapScriptNode.end())
	{
		printf("AddScriptNode: %s dwNodexIndex %u already exist\n", pScriptNode->GetName(), dwNodeIndex);
		return false;
	}

	pScriptNode->SetScriptManager(GetScriptManager());
	pScriptNode->SetScript(this);
	pScriptNode->SetNodeIndex(dwNodeIndex);

	m_mapScriptNode.insert(std::make_pair(dwNodeIndex, pScriptNode));

	return true;
}

CControlScriptNode* CControlScript::GetScriptNode(DWORD dwNodeIndex)
{
	std::map<DWORD, CControlScriptNode *>::iterator it = m_mapScriptNode.find(dwNodeIndex);
	if (it == m_mapScriptNode.end())
	{
		printf("GetScriptNode: %s dwNodexIndex %u not found\n", this->GetFileName(), dwNodeIndex);
		return NULL;
	}

	return it->second;
}