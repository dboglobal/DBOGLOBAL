#ifndef __AI_DBOG_BOTCONDITION_PETFOLLOW_H__
#define __AI_DBOG_BOTCONDITION_PETFOLLOW_H__

#include "BotAiCondition.h"


class CBotAiCondition_PetFollow : public CBotAiCondition
{

public:
	CBotAiCondition_PetFollow(CNpc* pBot);
	virtual	~CBotAiCondition_PetFollow();


public:

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


};

#endif