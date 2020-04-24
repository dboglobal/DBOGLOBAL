#include "stdafx.h"
#include "BotAiAction_SetCharCondition.h"
#include "SPSNodeAction_SetCharCondition.h"


CBotAiAction_SetCharCondition::CBotAiAction_SetCharCondition(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_SET_CHAR_CONDITION, "BOTCONTROL_ACTION_SET_CHAR_CONDITION")
{
	m_qwAddCharConditionFlag = 0;
	m_qwRemoveCharConditionFlag = 0;
}


CBotAiAction_SetCharCondition::~CBotAiAction_SetCharCondition()
{
}


bool CBotAiAction_SetCharCondition::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_SetCharCondition* pAction = dynamic_cast<CSPSNodeAction_SetCharCondition*>(pControlScriptNode);
	if (pAction)
	{
		m_qwAddCharConditionFlag = pAction->m_qwAddCharConditionFlag;
		m_qwRemoveCharConditionFlag = pAction->m_qwRemoveCharConditionFlag;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_SetCharCondition", pControlScriptNode);
	return false;
}

void CBotAiAction_SetCharCondition::CopyTo(CControlState* pTo)
{
	CBotAiAction_SetCharCondition* pAction = dynamic_cast<CBotAiAction_SetCharCondition*>(pTo);
	if (pAction)
	{
		pAction->m_qwAddCharConditionFlag = m_qwAddCharConditionFlag;
		pAction->m_qwRemoveCharConditionFlag = m_qwRemoveCharConditionFlag;
	}
}

void CBotAiAction_SetCharCondition::OnEnter()
{
}

void CBotAiAction_SetCharCondition::OnExit()
{
}


int CBotAiAction_SetCharCondition::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_qwRemoveCharConditionFlag > 0)
	{
		GetBot()->GetStateManager()->RemoveConditionFlags(m_qwRemoveCharConditionFlag, true);
	}
	if (m_qwAddCharConditionFlag > 0)
	{
		GetBot()->GetStateManager()->AddConditionFlags(m_qwAddCharConditionFlag, true);
	}
	//printf("m_qwRemoveCharConditionFlag: %u , m_qwAddCharConditionFlag: %u, GetConditionState: %u \n", m_qwRemoveCharConditionFlag, m_qwAddCharConditionFlag, GetBot()->GetStateManager()->GetConditionState());

	m_status = COMPLETED;
	return m_status;
}