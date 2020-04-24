#ifndef __SERVER_BOT_STATE_DESTMOVE__
#define __SERVER_BOT_STATE_DESTMOVE__

#include "CharState.h"

class CBotState_DestMove : public CCharState
{

public:
	CBotState_DestMove(CCharacter* pParent);
	virtual ~CBotState_DestMove();

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