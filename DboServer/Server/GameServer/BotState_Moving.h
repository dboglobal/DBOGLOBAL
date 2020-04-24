#ifndef __SERVER_BOT_STATE_MOVING__
#define __SERVER_BOT_STATE_MOVING__

#include "CharState.h"

class CBotState_Moving : public CCharState
{

public:
	CBotState_Moving(CCharacter* pParent);
	virtual ~CBotState_Moving();

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