#ifndef __SERVER_BOT_STATE_KNOCKDOWN__
#define __SERVER_BOT_STATE_KNOCKDOWN__

#include "CharState.h"

class CBotState_Knockdown : public CCharState
{

public:
	CBotState_Knockdown(CCharacter* pParent);
	virtual ~CBotState_Knockdown();

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