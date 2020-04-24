#include "stdafx.h"
#include "BotAiCondition_RangedAttack.h"
#include "BotAiAction_RangedAttack.h"
#include "BotAiState_Fight.h"


CBotAiCondition_RangedAttack::CBotAiCondition_RangedAttack(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_RANGED_ATTACK, "BOTCONTROL_CONDITION_RANGED_ATTACK")
{
	m_dwTime = 0;
}

CBotAiCondition_RangedAttack::~CBotAiCondition_RangedAttack()
{
}


int CBotAiCondition_RangedAttack::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	//printf("CBotAiCondition_RangedAttack::OnUpdate TO DO \n");

	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);
	if (m_dwTime > 3000)
	{
		m_dwTime = 0;

		if(GetBot()->IsMoveAble()) //not sure
		{
			if (GetBot()->ConsiderRangedAttack())
			{
				CBotAiState_Fight* pFightState = GetBot()->GetBotController()->GetFightState();
				if (pFightState)
				{
					CNtlVector rDestLoc;
					if (GetBot()->GetRangedAttackLoc(rDestLoc))
					{
						CBotAiAction_RangedAttack* pRangedAttack = new CBotAiAction_RangedAttack(GetBot(), rDestLoc);
						if (!pFightState->AddSubControlQueue(pRangedAttack, true))
						{
							m_status = FAILED;
						}
					}
				}
			}
		}
	}

	return m_status;
}

