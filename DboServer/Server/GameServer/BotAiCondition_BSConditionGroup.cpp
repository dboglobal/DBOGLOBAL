#include "stdafx.h"
#include "BotAiCondition_BSConditionGroup.h"
#include "SPSNodeCondition_BSConditionGroup.h"



CBotAiCondition_BSConditionGroup::CBotAiCondition_BSConditionGroup(CNpc* pBot)
:CBotAiBattleSceneCondition(pBot, BOTCONTROL_CONDITION_BS_CONDITION_GROUP, "BOTCONTROL_CONDITION_BS_CONDITION_GROUP")
{
	
}

CBotAiCondition_BSConditionGroup::~CBotAiCondition_BSConditionGroup()
{
}

bool CBotAiCondition_BSConditionGroup::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeCondition_BSConditionGroup* pCond = dynamic_cast<CSPSNodeCondition_BSConditionGroup*>(pControlScriptNode);
	if (pCond)
	{
		m_bOperatorAnd = pCond->m_bOperatorAnd;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CCSPSNodeCondition_BSConditionGroup", pControlScriptNode);
	return false;
}

void CBotAiCondition_BSConditionGroup::OnEnter()
{
}

void CBotAiCondition_BSConditionGroup::OnExit()
{
}

int CBotAiCondition_BSConditionGroup::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	int nResult = UpdateSubControlList(dwTickDiff, fMultiple);

	if (nResult != 9)
	{
		if (nResult != 10)
		{
			ERR_LOG(LOG_BOTAI, "invalid update 'bs condition group' result[%d]", nResult);
			m_status = SYSTEMERROR;
		}
	}

	return m_status;
}

