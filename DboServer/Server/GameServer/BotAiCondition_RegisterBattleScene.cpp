#include "stdafx.h"
#include "BotAiCondition_RegisterBattleScene.h"
#include "SPSNodeCondition_RegisterBattleScene.h"


CBotAiCondition_RegisterBattleScene::CBotAiCondition_RegisterBattleScene(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_REGISTER_BATTLE_SCENE, "BOTCONTROL_CONDITION_REGISTER_BATTLE_SCENE")
{
	m_ePlayType = SPS_BS_PLAY_UNLIMIT;
}

CBotAiCondition_RegisterBattleScene::~CBotAiCondition_RegisterBattleScene()
{
}


bool CBotAiCondition_RegisterBattleScene::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeCondition_RegisterBattleScene* pCond = dynamic_cast<CSPSNodeCondition_RegisterBattleScene*>(pControlScriptNode);
	if (pCond)
	{
		m_ePlayType = pCond->m_ePlayType;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeCondition_RegisterBattleScene", pControlScriptNode);
	return false;
}

void CBotAiCondition_RegisterBattleScene::OnEnter()
{
}

void CBotAiCondition_RegisterBattleScene::OnExit()
{
}

int CBotAiCondition_RegisterBattleScene::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_status = COMPLETED;
	return m_status;
}

