#ifndef __AI_DBOG_BOTCONDITION_RANGEDATTACK_H__
#define __AI_DBOG_BOTCONDITION_RANGEDATTACK_H__

#include "BotAiCondition.h"


class CBotAiCondition_RangedAttack : public CBotAiCondition
{

public:
	CBotAiCondition_RangedAttack(CNpc* pBot);
	virtual	~CBotAiCondition_RangedAttack();


public:

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


private:

	DWORD m_dwTime;

};

#endif