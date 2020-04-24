#ifndef __AI_DBOG_BOTSTATESPAWN_H__
#define __AI_DBOG_BOTSTATESPAWN_H__

#include "BotAiState.h"

class CBotAiState_Spawn : public CBotAiState
{

public:

	CBotAiState_Spawn(CNpc* pBot);
	virtual	~CBotAiState_Spawn();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD		m_dwTime;
	DWORD		m_dwAniTime;

};

#endif