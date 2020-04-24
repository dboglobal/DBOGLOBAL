#ifndef __AI_DBOG_BOTSTATE_H__
#define __AI_DBOG_BOTSTATE_H__

#include "BotAiBase.h"

class CBotAiState : public CBotAiBase
{

public:
	CBotAiState(CNpc* pBot, eBOTCONTROLID controlID, const char* lpszControlName);
	virtual	~CBotAiState();



};

#endif