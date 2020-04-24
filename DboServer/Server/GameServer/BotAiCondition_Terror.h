#ifndef __AI_DBOG_BOTCONDITION_TERROR_H__
#define __AI_DBOG_BOTCONDITION_TERROR_H__

#include "BotAiCondition.h"


class CBotAiCondition_Terror : public CBotAiCondition
{

public:
	CBotAiCondition_Terror(CNpc* pBot);
	virtual	~CBotAiCondition_Terror();


public:

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif