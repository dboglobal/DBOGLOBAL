#include "stdafx.h"
#include "AISNodeScene.h"

#include "AISNodeState.h"



CAISNodeScene::CAISNodeScene(DWORD dwSceneNumber, const char* lpszSceneName)
: m_dwSceneNumber(dwSceneNumber), m_bIsDestroy(false), CControlScriptNodeState(AIS_NODE_STATE_SCENE, lpszSceneName)
{
}

CAISNodeScene::~CAISNodeScene()
{

}


bool CAISNodeScene::AddState(CAISNodeState* pState)
{
	auto it = m_mapState.find(pState->GetStateID());
	if (it != m_mapState.end())
	{
		printf("CAISNodeScene::AddState: %s dwNodexIndex %u already exist\n", this->GetName(), pState->GetStateID());
		return false;
	}

	m_mapState.insert(std::make_pair(pState->GetStateID(), pState));

	return true;
}

CAISNodeState* CAISNodeScene::FindState(int nStateID)
{
	auto it = m_mapState.find(nStateID);
	if (it == m_mapState.end())
	{
		return NULL;
	}

	return it->second;
}