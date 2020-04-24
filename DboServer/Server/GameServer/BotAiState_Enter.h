#ifndef __AI_DBOG_BOTSTATEENTER_H__
#define __AI_DBOG_BOTSTATEENTER_H__

#include "BotAiState.h"

class CBotAiState_Enter : public CBotAiState
{

public:

	CBotAiState_Enter(CNpc* pBot);
	virtual	~CBotAiState_Enter();

public:

	virtual void OnEnter();
	virtual void OnExit();

};

#endif