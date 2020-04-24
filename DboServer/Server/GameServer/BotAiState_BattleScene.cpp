#include "stdafx.h"
#include "BotAiState_BattleScene.h"
#include "PlayScriptManager.h"
#include "PlayScriptControlFactoy.h"

CBotAiState_BattleScene::CBotAiState_BattleScene(CNpc* pBot, TBLIDX tblidxBattleScript, DWORD dwBSFlowNodeIndex)
	:CBotAiState(pBot, BOTCONTROL_BATTLESCENE, "BOTCONTROL_BATTLESCENE")
{
	m_tblidxBattleScript = tblidxBattleScript;
	m_dwBSFlowNodeIndex = dwBSFlowNodeIndex;
}

CBotAiState_BattleScene::~CBotAiState_BattleScene()
{
}


void CBotAiState_BattleScene::OnEnter()
{
	CControlScript* pControlScript = g_pPlayScriptManager->FindScript(m_tblidxBattleScript);
	if (pControlScript == NULL)
	{
		ERR_LOG(LOG_BOTAI, "can not find battle scene script. script[%u]", m_tblidxBattleScript);
		m_status = FAILED;
		return;
	}

	CControlScriptNode* pFlow = pControlScript->GetScriptNode(m_dwBSFlowNodeIndex);
	if (pFlow == NULL)
	{
		ERR_LOG(LOG_BOTAI, "can not find 'bs flow' script node. script[%u] node index[%u]", m_tblidxBattleScript, m_dwBSFlowNodeIndex);
		m_status = FAILED;
		return;
	}

	CPlayScriptControlFactoy factory(GetBot());

	if (pFlow->AttachScript(this, &factory, false) == false)
	{
		ERR_LOG(LOG_BOTAI, "could not attach script");
		m_status = FAILED;
	}

}

void CBotAiState_BattleScene::OnContinue()
{
}

void CBotAiState_BattleScene::OnPause()
{
}

void CBotAiState_BattleScene::OnExit()
{
}

int CBotAiState_BattleScene::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_status = CComplexState::OnUpdate(dwTickDiff, fMultiple);
	return m_status;
}
