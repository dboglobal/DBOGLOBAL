#ifndef __SERVER_BOT_STATE_LEAVING__
#define __SERVER_BOT_STATE_LEAVING__

#include "CharState.h"

class CBotState_Leaving : public CCharState
{

public:
	CBotState_Leaving(CCharacter* pParent);
	virtual ~CBotState_Leaving();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

	virtual bool IsValidTarget() { return false; }

private:

	BYTE			byLeavingType;

};


#endif