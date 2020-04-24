#include "stdafx.h"
#include "BotAiCondition_SkillUse.h"
#include "BotAiState.h"
#include "BotAiAction_SkillUse.h"
#include "SkillCondition.h"


CBotAiCondition_SkillUse::CBotAiCondition_SkillUse(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_SKILL_USE, "BOTCONTROL_CONDITION_SKILL_USE")
{
	m_dwTime = 0;
}

CBotAiCondition_SkillUse::~CBotAiCondition_SkillUse()
{
}


int CBotAiCondition_SkillUse::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);
	if (m_dwTime >= 1000)
	{
		m_dwTime = 0;

		if (GetBot()->HasNearbyPlayer(false))
		{
			if (!GetBot()->GetStateManager()->IsCharCondition(CHARCOND_CONFUSED))
			{
				CBotAiState* pCurState = GetBot()->GetBotController()->GetCurrentState();
				if (pCurState)
				{
					CComplexState* pCurAction = pCurState->GetCurrentAction();
					if (pCurAction && pCurAction->GetControlStateID() == BOTCONTROL_ACTION_LOOK)
						return m_status;
					else
					{
						CSkillManagerBot* pBotSkillManager = (CSkillManagerBot*)GetBot()->GetSkillManager();
						if (!pBotSkillManager->IsSkillUseLock())
						{
							CSkillCondition* pSkillCondition = NULL;
							if (pBotSkillManager->GetNumberOfSkill() > 0)
								pSkillCondition = pBotSkillManager->GetSkill(dwTickDiff);

							if (pSkillCondition && pSkillCondition->GetCanUseSkill())
							{
								CBotAiAction_SkillUse* pSkillUse = new CBotAiAction_SkillUse(GetBot(), pSkillCondition->GetSkillConditionIdx());
								if (!pCurState->AddSubControlQueue(pSkillUse, true))
								{
									m_status = FAILED;
								}
							}
						}
					}
				}
			}
		}

	}

	return m_status;
}

