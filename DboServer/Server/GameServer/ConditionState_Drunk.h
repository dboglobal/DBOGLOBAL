#ifndef __SERVER_BOT_CONDITION_STATE_DRUNK__
#define __SERVER_BOT_CONDITION_STATE_DRUNK__

#include "ConditionState.h"

class CConditionState_Drunk : public CConditionState
{

public:

	CConditionState_Drunk(CCharacter *pParent);
	virtual ~CConditionState_Drunk();


public:

	virtual void		OnExit();

private:


};

#endif
