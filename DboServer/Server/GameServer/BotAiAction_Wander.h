#ifndef __AI_DBOG_BOTACTION_WANDER_H__
#define __AI_DBOG_BOTACTION_WANDER_H__

#include "BotAiAction.h"


class CBotAiAction_Wander : public CBotAiAction
{

public:

	CBotAiAction_Wander(CNpc* pBot);
	virtual	~CBotAiAction_Wander();

public:

	virtual void OnEnter();
	virtual void OnContinue();
	virtual void OnPause();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

protected:

	void		Wander(DWORD dwTickTime);

private:

	DWORD		m_dwTime;

};

#endif