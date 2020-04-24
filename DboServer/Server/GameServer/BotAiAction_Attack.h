#ifndef __AI_DBOG_BOTACTION_ATTACK_H__
#define __AI_DBOG_BOTACTION_ATTACK_H__

#include "BotAiAction.h"
#include "AISNodeAction_AddMob.h"

class CCharacter;

class CBotAiAction_Attack : public CBotAiAction
{

public:

	CBotAiAction_Attack(CNpc* pBot, HOBJECT hTarget);
	virtual	~CBotAiAction_Attack();

public:

	virtual void OnEnter();
	virtual void OnContinue();
	virtual void OnPause();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

public:

	int BreakUp(DWORD dwTickTime, float fMultiple);

	int ObjTypeBreakUp(CCharacter* pTarget);

private:

	HOBJECT m_hTarget;
	DWORD m_dwAttackTime;
	DWORD m_dwMoveTime;
	bool m_bAttackCanceled;

};

#endif