#include "stdafx.h"
#include "BotAiCondition_AllianceHelpMe.h"
#include "NtlRandom.h"


CBotAiCondition_AllianceHelpMe::CBotAiCondition_AllianceHelpMe(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_ALLIANCE_HELPME, "BOTCONTROL_CONDITION_ALLIANCE_HELPME")
{
}

CBotAiCondition_AllianceHelpMe::~CBotAiCondition_AllianceHelpMe()
{
}


void CBotAiCondition_AllianceHelpMe::OnEnter()
{
}

int CBotAiCondition_AllianceHelpMe::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetBot()->GetTargetListManager()->IsCalledByHelp())
		m_status = COMPLETED;
	else
	{
		if (GetBot()->ConsiderLPLow(30.0f))
		{
			if (Dbo_CheckProbability(30))
			{
				GetBot()->HelpAlliance();
			}
			m_status = COMPLETED;
		}
	}

	return m_status;
}
