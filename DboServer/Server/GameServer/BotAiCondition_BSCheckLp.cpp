#include "stdafx.h"
#include "BotAiCondition_BSCheckLp.h"
#include "SPSNodeCondition_BSCheckLp.h"


CBotAiCondition_BSCheckLp::CBotAiCondition_BSCheckLp(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_BS_CHECK_LP, "BOTCONTROL_CONDITION_BS_CHECK_LP")
{
	m_byMaxPercent = 100;
	m_byMinPercent = 0;
	m_bIsInRange = false;
}

CBotAiCondition_BSCheckLp::~CBotAiCondition_BSCheckLp()
{
}

bool CBotAiCondition_BSCheckLp::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeCondition_BSCheckLp* pCond = dynamic_cast<CSPSNodeCondition_BSCheckLp*>(pControlScriptNode);
	if (pCond)
	{
		m_byMaxPercent = pCond->m_byMaxPercent;
		m_byMinPercent = pCond->m_byMinPercent;
		m_bIsInRange = pCond->m_bIsInRange;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeCondition_BSCheckLp", pControlScriptNode);
	return false;
}

void CBotAiCondition_BSCheckLp::OnEnter()
{
}

void CBotAiCondition_BSCheckLp::OnExit()
{
}

int CBotAiCondition_BSCheckLp::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetBot()->GetMaxLP() > 0)
	{
		BYTE byLpInPercent = (BYTE)floor(GetBot()->GetCurLpInPercent());
		if (m_bIsInRange)
		{
			if (m_byMinPercent <= byLpInPercent && m_byMaxPercent >= byLpInPercent)
				return 9;
		}
		else if (m_byMinPercent >= byLpInPercent || m_byMaxPercent <= byLpInPercent)
		{
			return 9;
		}

		return 10;
	}

	ERR_LOG(LOG_BOTAI, "0 >= GetBot()->GetMaxLP()");

	return 6;
}
