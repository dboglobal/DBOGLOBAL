#ifndef __AI_DBOG_BOTACTION_RANGEDATTACK_H__
#define __AI_DBOG_BOTACTION_RANGEDATTACK_H__

#include "BotAiAction.h"


class CBotAiAction_RangedAttack : public CBotAiAction
{

public:

	CBotAiAction_RangedAttack(CNpc* pBot, CNtlVector& vDestLoc);
	virtual	~CBotAiAction_RangedAttack();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD m_dwTime;
	CNtlVector m_vDestLoc;

};

#endif