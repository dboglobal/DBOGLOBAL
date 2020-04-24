#ifndef __SERVER_BOT_STATE_RIDEON__
#define __SERVER_BOT_STATE_RIDEON__

#include "CharState.h"

class CBotState_RideOn : public CCharState
{

public:
	CBotState_RideOn(CCharacter* pParent);
	virtual ~CBotState_RideOn();

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

	HOBJECT m_hTarget;

};


#endif