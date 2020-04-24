#ifndef __AI_DBOG_BOT_STATE_FIGHT_H__
#define __AI_DBOG_BOT_STATE_FIGHT_H__

#include "BotAiState.h"
#include "BotAiBase_FirstProcList.h"

class CBotAiState_Fight : public CBotAiState
{

public:

	CBotAiState_Fight(CNpc* pBot, HOBJECT hTarget);
	virtual	~CBotAiState_Fight();

public:

	virtual void OnEnter();
	virtual void OnContinue();
	virtual void OnPause();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

public:

	void	SetCompulsionTarget(HOBJECT hTarget) { m_hCompulsionTarget = hTarget; }
	HOBJECT GetCompulsionTarget() { return m_hCompulsionTarget; }

private:

	CBotAiBase_FirstProcList m_cFirstProcList;
	HOBJECT m_hTarget;
	HOBJECT m_hCompulsionTarget;
	DWORD m_dwTime;
	DWORD m_dwDelayTime;
	bool m_bIsOnlySkill;

};

#endif