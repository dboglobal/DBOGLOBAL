#ifndef __SERVER_BOT_CONDITION_STATE_FAKEDEATH__
#define __SERVER_BOT_CONDITION_STATE_FAKEDEATH__

#include "ConditionState.h"

class CConditionState_FakeDeath : public CConditionState
{

public:

	CConditionState_FakeDeath(CCharacter *pParent);
	virtual ~CConditionState_FakeDeath();


public:

	//virtual void		OnExit();

	//virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:


};

#endif
