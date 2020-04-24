#include "stdafx.h"
#include "BotAiCondition_Confusion.h"


CBotAiCondition_Confusion::CBotAiCondition_Confusion(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_CONFUSION, "BOTCONTROL_CONDITION_CONFUSION")
{

}

CBotAiCondition_Confusion::~CBotAiCondition_Confusion()
{
}


int CBotAiCondition_Confusion::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetBot()->IsActionStatusFlag(BOT_ACTIONSTATUS_CONFUSED) || !GetBot()->GetStateManager()->IsCharCondition(CHARCOND_CONFUSED))
	{
		if (GetBot()->IsActionStatusFlag(BOT_ACTIONSTATUS_CONFUSED))
		{
			if (!GetBot()->GetStateManager()->IsCharCondition(CHARCOND_CONFUSED))
			{
				GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_CONFUSED, false);
			//	GetBot()->ChangeTarget(INVALID_HOBJECT); //when enable this, then mob stops attacking
			}
		}
	}
	else
	{
		GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_CONFUSED, true);
	}

	return m_status;
}

