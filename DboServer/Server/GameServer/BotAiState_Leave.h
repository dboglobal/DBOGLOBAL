#ifndef __AI_DBOG_BOT_STATE_LEAVE_H__
#define __AI_DBOG_BOT_STATE_LEAVE_H__

#include "BotAiState.h"

class CBotAiState_Leave : public CBotAiState
{

public:

	CBotAiState_Leave(CNpc* pBot);
	virtual	~CBotAiState_Leave();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif