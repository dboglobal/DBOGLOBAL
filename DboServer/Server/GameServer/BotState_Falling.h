#ifndef __SERVER_BOT_STATE_FALLING__
#define __SERVER_BOT_STATE_FALLING__

#include "CharState.h"

class CBotState_Falling : public CCharState
{

public:
	CBotState_Falling(CCharacter* pParent);
	virtual ~CBotState_Falling();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

	virtual bool IsValidTarget() { return true; } //if this is false, then monsters will lose aggro when player is falling after getting attacked
};


#endif