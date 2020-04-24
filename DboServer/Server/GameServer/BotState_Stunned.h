#ifndef __SERVER_BOT_STATE_STUNNED__
#define __SERVER_BOT_STATE_STUNNED__

#include "CharState.h"

class CBotState_Stunned : public CCharState
{

public:
	CBotState_Stunned(CCharacter* pParent);
	virtual ~CBotState_Stunned();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

private:

	BYTE byStunType;
};


#endif