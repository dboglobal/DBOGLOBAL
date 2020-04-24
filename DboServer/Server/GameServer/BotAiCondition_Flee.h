#ifndef __AI_DBOG_BOTCONDITION_FLEE_H__
#define __AI_DBOG_BOTCONDITION_FLEE_H__

#include "BotAiCondition.h"


class CBotAiCondition_Flee : public CBotAiCondition
{

public:
	CBotAiCondition_Flee(CNpc* pBot, float fFleeFactor);
	virtual	~CBotAiCondition_Flee();


public:

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD m_dwTime;
	float m_fFleeFactor;

};

#endif