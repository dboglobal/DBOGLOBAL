#ifndef __SERVER_BOT_STATE_AIRDASHACCEL__
#define __SERVER_BOT_STATE_AIRDASHACCEL__

#include "CharState.h"

class CBotState_AirDashAccel : public CCharState
{

public:
	CBotState_AirDashAccel(CCharacter* pParent);
	virtual ~CBotState_AirDashAccel();

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