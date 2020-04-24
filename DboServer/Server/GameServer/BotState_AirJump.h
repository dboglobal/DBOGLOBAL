#ifndef __SERVER_BOT_STATE_AIRJUMP__
#define __SERVER_BOT_STATE_AIRJUMP__

#include "CharState.h"

class CBotState_AirJump : public CCharState
{

public:
	CBotState_AirJump(CCharacter* pParent);
	virtual ~CBotState_AirJump();

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