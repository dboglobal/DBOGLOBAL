#ifndef __AI_DBOG_BOTCONDITION_DEFEND_H__
#define __AI_DBOG_BOTCONDITION_DEFEND_H__

#include "BotAiCondition.h"


class CBotAiCondition_Defend : public CBotAiCondition
{

public:
	CBotAiCondition_Defend(CNpc* pBot, DWORD dwLimitTime);
	virtual	~CBotAiCondition_Defend();


public:

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD m_dwLimitTime;
	DWORD m_dwTime;

};

#endif