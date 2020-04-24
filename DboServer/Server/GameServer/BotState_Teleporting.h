#ifndef __SERVER_BOT_STATE_TELEPORTING__
#define __SERVER_BOT_STATE_TELEPORTING__

#include "CharState.h"

class CBotState_Teleporting : public CCharState
{

public:
	CBotState_Teleporting(CCharacter* pParent);
	virtual ~CBotState_Teleporting();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

	virtual bool IsValidTarget() { return false; }

};


#endif