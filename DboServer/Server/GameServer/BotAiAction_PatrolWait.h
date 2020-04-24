#ifndef __AI_DBOG_BOTACTION_PATROLWAIT_H__
#define __AI_DBOG_BOTACTION_PATROLWAIT_H__

#include "BotAiAction.h"

class CBotAiAction_PatrolWait : public CBotAiAction
{

public:

	CBotAiAction_PatrolWait(CNpc* pBot, HOBJECT hTarget);
	virtual	~CBotAiAction_PatrolWait();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg* pObjMsg);

private:

	DWORD m_dwTime;
	HOBJECT m_hTarget;
};

#endif