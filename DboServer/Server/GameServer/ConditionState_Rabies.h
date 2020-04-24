#ifndef __SERVER_BOT_CONDITION_STATE_RABIES__
#define __SERVER_BOT_CONDITION_STATE_RABIES__

#include "ConditionState.h"

class CConditionState_Rabies : public CConditionState
{

public:

	CConditionState_Rabies(CCharacter *pParent);
	virtual ~CConditionState_Rabies();


public:

	virtual void	OnEnter();
	virtual void	OnExit();

	virtual int		OnUpdate(DWORD dwTickDiff, float fMultiple);


private:


};

#endif
