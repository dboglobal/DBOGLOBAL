#ifndef __AI_DBOG_BOTACTION_FOLLOWMOVE_H__
#define __AI_DBOG_BOTACTION_FOLLOWMOVE_H__

#include "BotAiAction.h"

class CNpc;

class CBotAiAction_FollowMove : public CBotAiAction
{

public:

	CBotAiAction_FollowMove(CNpc* pBot, HOBJECT hTarget, CNtlVector& vDestLoc, bool bIsFormationType);
	virtual	~CBotAiAction_FollowMove();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	int		MarineType(CNpc * pLeader);
	int		FormationType(CNpc * pLeader);

private:

	HOBJECT		m_hTarget;
	DWORD		m_dwTime;
	CNtlVector	m_vDestLoc;
	bool		m_bIsFormationType;

};

#endif