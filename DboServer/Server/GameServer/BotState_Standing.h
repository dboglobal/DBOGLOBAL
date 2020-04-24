#ifndef __SERVER_BOT_STATE_STANDING__
#define __SERVER_BOT_STATE_STANDING__

#include "CharState.h"

class CBotState_Standing : public CCharState
{

public:
	CBotState_Standing(CCharacter* pParent);
	virtual ~CBotState_Standing();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

private:

	DWORD m_dwWakeUpTime;

};


#endif