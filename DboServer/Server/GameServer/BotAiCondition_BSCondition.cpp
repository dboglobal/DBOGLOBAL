#include "stdafx.h"
#include "BotAiCondition_BSCondition.h"
#include "SPSNodeCondition_BSCondition.h"
#include "PlayScriptManager.h"
#include "PlayScriptControlFactoy.h"


CBotAiCondition_BSCondition::CBotAiCondition_BSCondition(CNpc* pBot)
:CBotAiBattleSceneCondition(pBot, BOTCONTROL_CONDITION_BS_CONDITION, "BOTCONTROL_CONDITION_BS_CONDITION")
{
	m_tblidxBattleScript = INVALID_TBLIDX;
	m_dwNodeIndex = INVALID_DWORD;
	m_ePlayType = SPS_BS_PLAY_UNLIMIT;
	m_dwPreCoolTime = 0;
	m_dwBSFlowNodeIndex = INVALID_DWORD;
	m_dwCheckPreCoolTime = 0;
	m_dwCheckCoolTime = 0;
	m_dwPlayCount = 0;
}

CBotAiCondition_BSCondition::~CBotAiCondition_BSCondition()
{
}

bool CBotAiCondition_BSCondition::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeCondition_BSCondition* pCond = dynamic_cast<CSPSNodeCondition_BSCondition*>(pControlScriptNode);
	if (pCond)
	{
		m_bOperatorAnd = pCond->m_bOperatorAnd;
		m_dwPreCoolTime = pCond->m_dwPreCoolTime;
		m_dwCoolTime = pCond->m_dwCoolTime;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeCondition_BSCondition", pControlScriptNode);
	return false;
}

void CBotAiCondition_BSCondition::OnEnter()
{
	CControlScriptNode* pPlayScript = g_pPlayScriptManager->GetPlayScriptNode(m_tblidxBattleScript, m_dwNodeIndex);
	if (pPlayScript == NULL)
	{
		ERR_LOG(LOG_BOTAI, "can not find 'register battle scene'. script[%u] node index[%u]", m_tblidxBattleScript, m_dwNodeIndex);
		m_status = SYSTEMERROR;
		return;
	}

	if (pPlayScript->GetID() != SPS_NODE_CONDITION_REGISTER_BATTLE_SCENE)
	{
		ERR_LOG(LOG_BOTAI, "is not 'register battle scene'. script[%u] node index[%u]", m_tblidxBattleScript, m_dwNodeIndex);
		m_status = SYSTEMERROR;
		return;
	}

	CControlScriptNode* pCondition = pPlayScript->GetChildNode(SPS_NODE_CONDITION_BS_CONDITION);
	if (pCondition == NULL)
	{
		ERR_LOG(LOG_BOTAI, "can not find 'bs condition'. script[%u] node index[%u]", m_tblidxBattleScript, m_dwNodeIndex);
		m_status = SYSTEMERROR;
		return;
	}

	DWORD dwFlowIndex = pPlayScript->GetChildNodeIndex(SPS_NODE_CONDITION_BS_FLOW);
	if (dwFlowIndex == INVALID_DWORD)
	{
		ERR_LOG(LOG_BOTAI, "can not find 'bs flow'. script[%u] node index[%u]", m_tblidxBattleScript, m_dwNodeIndex);
		m_status = SYSTEMERROR;
		return;
	}

	CPlayScriptControlFactoy factory(GetBot());

	if (pCondition->AttachScript(this, &factory, true))
	{
		m_dwBSFlowNodeIndex = dwFlowIndex;
		m_dwCheckPreCoolTime = 0;
		m_dwCheckCoolTime = m_dwCoolTime;
		m_dwPlayCount = 0;
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "could not attach script");
		m_status = SYSTEMERROR;
	}
}

void CBotAiCondition_BSCondition::OnContinue()
{
	m_dwCheckCoolTime = 0;
}

void CBotAiCondition_BSCondition::OnExit()
{
}

int CBotAiCondition_BSCondition::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_ePlayType)
	{
		if(m_ePlayType == INVALID_SPS_BS_PLAY_TYPE)
		{
			ERR_LOG(LOG_BOTAI, "invalid play type[%u]");
			m_status = SYSTEMERROR;
		}

		if (m_dwPlayCount >= 1)
			return m_status;
	}

	m_dwCheckPreCoolTime += dwTickDiff;

	if (m_dwCheckPreCoolTime >= m_dwPreCoolTime)
	{
		int nStatus = UpdateSubControlList(dwTickDiff, fMultiple);

		if (nStatus == 9)
		{
			if (GetBot()->GetBotController()->AddControlState_BattleScene(m_tblidxBattleScript, m_dwBSFlowNodeIndex))
			{
				m_status = CHANGED;
			}
			else
			{
				ERR_LOG(LOG_BOTAI, "false == GetBot()->GetBotController()->AddControlState_BattleScene( m_tblidxBattleScript, m_dwBSFlowNodeIndex )");
				m_status = SYSTEMERROR;
			}
		}
		else if (nStatus == 10)
			return m_status;
		else
		{
			ERR_LOG(LOG_BOTAI, "invalid update 'bs condition' result[%d]", nStatus);
			m_status = SYSTEMERROR;
		}
	}

	return m_status;
}


void CBotAiCondition_BSCondition::SetScript(TBLIDX tblidxBattleScript, DWORD dwNodeIndex, eSPS_BS_PLAY_TYPE ePlayType)
{
	m_tblidxBattleScript = tblidxBattleScript;
	m_dwNodeIndex = dwNodeIndex;
	m_ePlayType = ePlayType;
}
