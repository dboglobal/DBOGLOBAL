#ifndef __SERVER_BOT_STATE_DASHPASSIVE__
#define __SERVER_BOT_STATE_DASHPASSIVE__

#include "CharState.h"

class CBotState_DashPassive : public CCharState
{

public:
	CBotState_DashPassive(CCharacter* pParent);
	virtual ~CBotState_DashPassive();

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