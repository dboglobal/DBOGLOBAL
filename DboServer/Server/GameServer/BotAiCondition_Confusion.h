#ifndef __AI_DBOG_BOTCONDITION_CONFUSION_H__
#define __AI_DBOG_BOTCONDITION_CONFUSION_H__

#include "BotAiCondition.h"


class CBotAiCondition_Confusion : public CBotAiCondition
{

public:
	CBotAiCondition_Confusion(CNpc* pBot);
	virtual	~CBotAiCondition_Confusion();


public:

	virtual int		OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif