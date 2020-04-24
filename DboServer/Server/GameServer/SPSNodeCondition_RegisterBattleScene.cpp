#include "stdafx.h"
#include "SPSNodeCondition_RegisterBattleScene.h"
#include "ControlScriptNodeParam_String.h"



CSPSNodeCondition_RegisterBattleScene::CSPSNodeCondition_RegisterBattleScene(const char* lpszName)
:CControlScriptNodeCondition(SPS_NODE_CONDITION_REGISTER_BATTLE_SCENE, lpszName)
{
	m_ePlayType = SPS_BS_PLAY_UNLIMIT;
}

CSPSNodeCondition_RegisterBattleScene::~CSPSNodeCondition_RegisterBattleScene()
{
}


bool CSPSNodeCondition_RegisterBattleScene::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "play type") == 0)
	{
		const char* sub = pNode->GetName();

		if (_stricmp(sub, "unlimit") == 0)
		{
			m_ePlayType = SPS_BS_PLAY_UNLIMIT;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "once") == 0)
		{
			m_ePlayType = SPS_BS_PLAY_ONCE;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeCondition_RegisterBattleScene: sub [%s] not exist \n", sub);
		return false;
	}

	printf("CSPSNodeCondition_RegisterBattleScene: [%s] not exist \n", name);
	return false;
}