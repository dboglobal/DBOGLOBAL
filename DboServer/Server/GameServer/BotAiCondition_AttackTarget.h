#ifndef __AI_DBOG_BOTCONDITION_ATTACKTARGET_H__
#define __AI_DBOG_BOTCONDITION_ATTACKTARGET_H__

#include "BotAiCondition.h"


class CBotAiCondition_AttackTarget : public CBotAiCondition
{

public:
	CBotAiCondition_AttackTarget(CNpc* pBot);
	virtual	~CBotAiCondition_AttackTarget();


public:

	virtual void OnContinue();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

protected:

	bool CheckAndAttackTarget();

private:

	DWORD m_dwTime;
	bool m_bPartyTargetChecked;

};

#endif