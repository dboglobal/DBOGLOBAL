#ifndef __SERVER_BOT_STATE_CAMPING__
#define __SERVER_BOT_STATE_CAMPING__

#include "CharState.h"

class CBotState_Camping : public CCharState
{

public:
	CBotState_Camping(CCharacter* pParent);
	virtual ~CBotState_Camping();

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