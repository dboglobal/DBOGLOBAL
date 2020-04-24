#ifndef __AI_DBOG_BOT_STATE_FAINT_H__
#define __AI_DBOG_BOT_STATE_FAINT_H__

#include "BotAiState.h"

class CBotAiState_Faint : public CBotAiState
{

public:

	CBotAiState_Faint(CNpc* pBot);
	virtual	~CBotAiState_Faint();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD m_dwTime;
	DWORD m_dwFaint_Leave_Time;

};

#endif