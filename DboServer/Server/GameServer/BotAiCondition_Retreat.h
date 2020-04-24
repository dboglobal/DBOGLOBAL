#ifndef __AI_DBOG_BOTCONDITION_RETREAT_H__
#define __AI_DBOG_BOTCONDITION_RETREAT_H__

#include "BotAiCondition.h"


class CBotAiCondition_Retreat : public CBotAiCondition
{

public:
	CBotAiCondition_Retreat(CNpc* pBot);
	virtual	~CBotAiCondition_Retreat();


public:

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


private:

	DWORD m_dwTime;

};

#endif