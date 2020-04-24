#ifndef __AI_DBOG_BOTCONDITION_CHILD_H__
#define __AI_DBOG_BOTCONDITION_CHILD_H__

#include "BotAiCondition.h"


class CBotAiCondition_Child : public CBotAiCondition
{

public:
	CBotAiCondition_Child(CNpc* pBot);
	virtual	~CBotAiCondition_Child();


public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif