#ifndef __SERVER_BOT_STATE_PARALYZED__
#define __SERVER_BOT_STATE_PARALYZED__

#include "CharState.h"

class CBotState_Paralyzed : public CCharState
{

public:
	CBotState_Paralyzed(CCharacter* pParent);
	virtual ~CBotState_Paralyzed();

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