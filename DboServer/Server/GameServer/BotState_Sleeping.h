#ifndef __SERVER_BOT_STATE_SLEEPING__
#define __SERVER_BOT_STATE_SLEEPING__

#include "CharState.h"

class CBotState_Sleeping : public CCharState
{

public:
	CBotState_Sleeping(CCharacter* pParent);
	virtual ~CBotState_Sleeping();

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