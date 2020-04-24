#ifndef __SERVER_BOT_STATE_FOLLOW__
#define __SERVER_BOT_STATE_FOLLOW__

#include "CharState.h"

class CBotState_Follow : public CCharState
{

public:
	CBotState_Follow(CCharacter* pParent);
	virtual ~CBotState_Follow();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

private:



};


#endif