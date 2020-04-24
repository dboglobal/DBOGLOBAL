#include "stdafx.h"
#include "SPSNodeScene.h"



CSPSNodeScene::CSPSNodeScene(DWORD dwSceneNumber, const char* lpszSceneName)
: m_dwSceneNumber(dwSceneNumber), CControlScriptNodeState(SPS_NODE_STATE_SCENE, lpszSceneName)
{
}

CSPSNodeScene::~CSPSNodeScene()
{

}

