#ifndef __AI_DBOG_BOTCONDITION_H__
#define __AI_DBOG_BOTCONDITION_H__

#include "BotAiBase.h"

class CBotAiCondition : public CBotAiBase
{

public:
	CBotAiCondition(CNpc* pBot, eBOTCONTROLID controlID, const char* lpszControlName);
	virtual	~CBotAiCondition();

};

#endif