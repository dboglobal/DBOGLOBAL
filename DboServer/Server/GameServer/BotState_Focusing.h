#ifndef __SERVER_BOT_STATE_FOCUSING__
#define __SERVER_BOT_STATE_FOCUSING__

#include "CharState.h"

class CBotState_Focusing : public CCharState
{

public:
	CBotState_Focusing(CCharacter* pParent);
	virtual ~CBotState_Focusing();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

private:

	DWORD			m_dwTime;

};


#endif