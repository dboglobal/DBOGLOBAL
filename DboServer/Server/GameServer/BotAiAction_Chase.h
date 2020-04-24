#ifndef __AI_DBOG_BOTACTION_CHASE_H__
#define __AI_DBOG_BOTACTION_CHASE_H__

#include "BotAiAction.h"

class CBotAiAction_Chase : public CBotAiAction
{

public:

	enum eATTACKTYPE
	{
		ATTACKTYPE_ATTACK = 0x0,
		ATTACKTYPE_SKILL = 0x1,
	};

	CBotAiAction_Chase(CNpc* pBot, CBotAiAction_Chase::eATTACKTYPE AttackType, float fSkillAttackRange);
	virtual	~CBotAiAction_Chase();

public:

	virtual void OnEnter();
	virtual void OnContinue();
	virtual void OnPause();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool BreakupAttack(CCharacter* pTarget);
	bool BreakupAttack_ShortDistance(BYTE byBotCount);
	bool BreakupAttack_GreatDistance(BYTE byBotCount);
	bool BreakupSkill(CCharacter* pTarget);
	bool BreakupSkill_ShortDistance(BYTE byBotCount);
	bool BreakupSkill_GreatDistance(BYTE byBotCount);
	bool IsBreakupAIRun_ShortDistance();
	bool IsBreakupAIRun_GreatDistance();
	bool Following(CCharacter* pTarget);

private:

	DWORD m_dwElapsedTimeSincedNextMovementApplied;
	CBotAiAction_Chase::eATTACKTYPE m_AttackType;
	float m_fSkillAttackRange;
	bool m_bIsPathFindingUsed;
	float m_fBreakupStartPos;
	bool m_bIsBreakup;

};

#endif