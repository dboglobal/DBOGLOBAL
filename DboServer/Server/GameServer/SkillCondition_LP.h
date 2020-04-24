#ifndef __DBOG_SKILL_CONDITION_LP__
#define __DBOG_SKILL_CONDITION_LP__

#include "SkillCondition.h"

class CSkillBot;

class CSkillCondition_LP : public CSkillCondition
{
public:

	CSkillCondition_LP() {}
	virtual ~CSkillCondition_LP() {}

public:

	virtual CSkillBot*		OnUpdate(DWORD dwTickTime);

};


inline CSkillBot* CSkillCondition_LP::OnUpdate(DWORD dwTickTime)
{
	CSkillBot* pSkill = CSkillCondition::OnUpdate(dwTickTime);
	if (pSkill)
	{
		if (m_wUse_Skill_LP == INVALID_WORD)
		{
			ERR_LOG(LOG_GENERAL, "fail : INVALID_WORD == m_wUse_Skill_LP");
			return NULL;
		}
		else
		{
			if (GetBot()->ConsiderLPLow(m_wUse_Skill_LP))
				return pSkill;
		}
	}

	return NULL;
}

#endif