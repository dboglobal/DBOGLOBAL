#ifndef __AI_DBOG_BOTACTION_H__
#define __AI_DBOG_BOTACTION_H__

#include "BotAiBase.h"


class CBotAiAction : public CBotAiBase
{

public:
	CBotAiAction(CNpc* pBot, eBOTCONTROLID controlID, const char* lpszControlName);
	virtual	~CBotAiAction();



};

#endif