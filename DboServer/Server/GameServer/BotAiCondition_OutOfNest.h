#ifndef __AI_DBOG_BOTCONDITION_OUTOFNEST_H__
#define __AI_DBOG_BOTCONDITION_OUTOFNEST_H__

#include "BotAiCondition.h"

class CBotAiCondition_OutOfNest : public CBotAiCondition
{

public:
	CBotAiCondition_OutOfNest(CNpc* pBot);
	virtual	~CBotAiCondition_OutOfNest();


public:

	virtual void OnEnter();
	virtual void OnContinue();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


private:

	BYTE m_byMaximumRangeOfAwayFromNest;

};

#endif