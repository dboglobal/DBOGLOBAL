#ifndef __AI_DBOG_BOT_STATE_DESPAWN_H__
#define __AI_DBOG_BOT_STATE_DESPAWN_H__

#include "BotAiState.h"

class CBotAiState_Despawn : public CBotAiState
{

public:

	CBotAiState_Despawn(CNpc* pBot);
	virtual	~CBotAiState_Despawn();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD m_dwTime;

};

#endif