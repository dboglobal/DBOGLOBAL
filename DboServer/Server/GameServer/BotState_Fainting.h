#ifndef __SERVER_BOT_STATE_FAINTING__
#define __SERVER_BOT_STATE_FAINTING__

#include "CharState.h"

class CBotState_Fainting : public CCharState
{

public:
	CBotState_Fainting(CCharacter* pParent);
	virtual ~CBotState_Fainting();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

	virtual bool IsValidTarget() { return false; }

private:

};


#endif