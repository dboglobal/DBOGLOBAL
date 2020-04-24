#ifndef __SERVER_BOT_STATE_SLIDING__
#define __SERVER_BOT_STATE_SLIDING__

#include "CharState.h"

class CBotState_Sliding : public CCharState
{

public:
	CBotState_Sliding(CCharacter* pParent);
	virtual ~CBotState_Sliding();

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