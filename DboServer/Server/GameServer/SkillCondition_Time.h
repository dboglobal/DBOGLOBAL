#ifndef __DBOG_SKILL_CONDITION_TIME__
#define __DBOG_SKILL_CONDITION_TIME__

#include "SkillCondition.h"

class CSkillBot;

class CSkillCondition_Time : public CSkillCondition
{
public:

	CSkillCondition_Time() {}
	virtual ~CSkillCondition_Time() {}

public:

	virtual CSkillBot*		OnUpdate(DWORD dwTickTime);

};


inline CSkillBot* CSkillCondition_Time::OnUpdate(DWORD dwTickTime)
{
	if (Dbo_CheckProbability(m_wUse_Skill_LP))
	{
		return CSkillCondition::OnUpdate(dwTickTime);
	}

	return NULL;
}

#endif