#ifndef __AI_DBOG_BOTACTION_FIXLOC_H__
#define __AI_DBOG_BOTACTION_FIXLOC_H__

#include "BotAiAction.h"

class CBotAiAction_FixLoc : public CBotAiAction
{

public:

	CBotAiAction_FixLoc(CNpc* pBot);
	virtual	~CBotAiAction_FixLoc();

public:

	virtual void OnEnter();
	virtual void OnContinue();
	virtual void OnPause();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

protected:

	void		FixLoc(DWORD dwTickTime);

private:

	DWORD		m_dwTime;

};

#endif