#ifndef __AI_DBOG_BOTCONDITION_TIMID_H__
#define __AI_DBOG_BOTCONDITION_TIMID_H__

#include "BotAiCondition.h"


class CBotAiCondition_Timid : public CBotAiCondition
{

public:
	CBotAiCondition_Timid(CNpc* pBot);
	virtual	~CBotAiCondition_Timid();


public:

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


private:

	DWORD m_dwTime;

};

#endif