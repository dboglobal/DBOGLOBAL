#include "stdafx.h"
#include "BotAiCondition_Child.h"


CBotAiCondition_Child::CBotAiCondition_Child(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_CHILD, "BOTCONTROL_CONDITION_CHILD")
{

}

CBotAiCondition_Child::~CBotAiCondition_Child()
{
}

void CBotAiCondition_Child::OnEnter()
{
}

void CBotAiCondition_Child::OnExit()
{
}

int CBotAiCondition_Child::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	int nStatusList = CComplexState::UpdateSubControlList(dwTickDiff, fMultiple);
	int nStatusQueue = CComplexState::UpdateSubControlQueue(dwTickDiff, fMultiple);
	if (nStatusList != COMPLETED || nStatusQueue != COMPLETED)
	{
		if (nStatusList == FAILED || nStatusQueue == FAILED)
			m_status = FAILED;
	}
	else
	{
		m_status = COMPLETED;
	}
	return m_status;
}

