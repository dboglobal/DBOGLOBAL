#ifndef __SERVER_BOT_CONDITION_STATE_AFTEREFFECT__
#define __SERVER_BOT_CONDITION_STATE_AFTEREFFECT__

#include "ConditionState.h"

class CConditionState_AfterEffect : public CConditionState
{

public:

	CConditionState_AfterEffect(CCharacter *pParent);
	virtual ~CConditionState_AfterEffect();


public:

	virtual void	OnEnter();

	virtual int		OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD			m_dwDuration;

};

#endif
