#ifndef __SERVER_BOT_STATE_SITTING__
#define __SERVER_BOT_STATE_SITTING__

#include "CharState.h"

class CBotState_Sitting : public CCharState
{

public:
	CBotState_Sitting(CCharacter* pParent);
	virtual ~CBotState_Sitting();

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