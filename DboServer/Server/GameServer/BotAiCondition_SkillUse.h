#ifndef __AI_DBOG_BOTCONDITION_SKILLUSE_H__
#define __AI_DBOG_BOTCONDITION_SKILLUSE_H__

#include "BotAiCondition.h"


class CBotAiCondition_SkillUse : public CBotAiCondition
{

public:
	CBotAiCondition_SkillUse(CNpc* pBot);
	virtual	~CBotAiCondition_SkillUse();


public:

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


private:

	DWORD m_dwTime;

};

#endif