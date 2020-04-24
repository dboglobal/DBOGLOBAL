#include "stdafx.h"
#include "BotAiCondition_Defend.h"


CBotAiCondition_Defend::CBotAiCondition_Defend(CNpc* pBot, DWORD dwLimitTime)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_DEFEND, "BOTCONTROL_CONDITION_DEFEND")
{
	m_dwLimitTime = dwLimitTime;
	m_dwTime = 0;
}

CBotAiCondition_Defend::~CBotAiCondition_Defend()
{
}


int CBotAiCondition_Defend::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);

	if (m_dwTime >= m_dwLimitTime)
	{
		m_dwTime = 0;
		GetBot()->HelpDefend();
		m_status = COMPLETED;
	}

	return m_status;
}

