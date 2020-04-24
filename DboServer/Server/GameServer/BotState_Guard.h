#ifndef __SERVER_BOT_STATE_GUARD__
#define __SERVER_BOT_STATE_GUARD__

#include "CharState.h"

class CBotState_Guard : public CCharState
{

public:
	CBotState_Guard(CCharacter* pParent);
	virtual ~CBotState_Guard();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

};


#endif