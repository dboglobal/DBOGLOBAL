#ifndef __DBOG_SKILL_CONDITION_ONLYLP__
#define __DBOG_SKILL_CONDITION_ONLYLP__

#include "SkillCondition.h"

class CSkillBot;

class CSkillCondition_OnlyLP : public CSkillCondition
{
public:

	CSkillCondition_OnlyLP()
		:m_bRunSkill(false) {}

	virtual ~CSkillCondition_OnlyLP() {}

public:

	virtual CSkillBot*	OnUpdate(DWORD dwTickTime);

private:

	bool				m_bRunSkill;

};


inline CSkillBot* CSkillCondition_OnlyLP::OnUpdate(DWORD dwTickTime)
{
	if (GetBot()->GetCurEP() < m_pSkill->GetOriginalTableData()->wRequire_EP)
		return NULL;

	if (m_pSkill->GetCoolTimeRemaining() > 0)
		return NULL;

	if (m_wUse_Skill_LP == INVALID_WORD)
	{
		ERR_LOG(LOG_GENERAL, "fail : INVALID_WORD == m_wUse_Skill_LP");
		return NULL;
	}
	else
	{
		if (GetBot()->ConsiderLPLow(m_wUse_Skill_LP))
		{
			if (!m_bRunSkill)
			{
				m_bRunSkill = true;
				return m_pSkill;
			}
		}
		else
			m_bRunSkill = false;
	}

	return NULL;
}

#endif