#ifndef __AI_DBOG_BOTSTATEIDLE_H__
#define __AI_DBOG_BOTSTATEIDLE_H__

#include "BotAiState.h"

class CBotAiState_Idle : public CBotAiState
{

public:

	CBotAiState_Idle(CNpc* pBot);
	virtual	~CBotAiState_Idle();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif