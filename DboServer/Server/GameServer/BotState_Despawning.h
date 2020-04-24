#ifndef __SERVER_BOT_STATE_DESPAWNING__
#define __SERVER_BOT_STATE_DESPAWNING__

#include "CharState.h"

class CBotState_Despawning : public CCharState
{

public:
	CBotState_Despawning(CCharacter* pParent);
	virtual ~CBotState_Despawning();

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