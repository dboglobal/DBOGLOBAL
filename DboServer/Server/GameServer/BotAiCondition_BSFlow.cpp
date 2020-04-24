#include "stdafx.h"
#include "BotAiCondition_BSFlow.h"
#include "SPSNodeCondition_BSFlow.h"


CBotAiCondition_BSFlow::CBotAiCondition_BSFlow(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_BS_FLOW, "BOTCONTROL_CONDITION_BS_FLOW")
{

}

CBotAiCondition_BSFlow::~CBotAiCondition_BSFlow()
{
}

bool CBotAiCondition_BSFlow::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeCondition_BSFlow* pCond = dynamic_cast<CSPSNodeCondition_BSFlow*>(pControlScriptNode);
	if (pCond)
	{

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeCondition_BSFlow", pControlScriptNode);
	return false;
}

void CBotAiCondition_BSFlow::OnEnter()
{
}

void CBotAiCondition_BSFlow::OnExit()
{
}

int CBotAiCondition_BSFlow::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_status = COMPLETED;
	return m_status;
}

