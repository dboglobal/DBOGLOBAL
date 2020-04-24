#ifndef __AI_DBOG_BOTACTION_PETFOLLOW_H__
#define __AI_DBOG_BOTACTION_PETFOLLOW_H__

#include "BotAiAction.h"


class CBotAiAction_PetFollow : public CBotAiAction
{

public:

	CBotAiAction_PetFollow(CNpc* pBot, HOBJECT hTarget, CNtlVector& vDestLoc);
	virtual	~CBotAiAction_PetFollow();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	HOBJECT		m_hTarget;
	CNtlVector	m_vDestLoc;
	DWORD		m_dwElapsedTimeSincedNextMovementApplied;
	bool		m_bIsPathFindingUsed;

};

#endif