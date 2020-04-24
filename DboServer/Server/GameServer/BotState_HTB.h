#ifndef __SERVER_BOT_STATE_HTB__
#define __SERVER_BOT_STATE_HTB__

#include "CharState.h"

class CBotState_HTB : public CCharState
{

public:
	CBotState_HTB(CCharacter* pParent);
	virtual ~CBotState_HTB();

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

	sCHARSTATE_HTB	m_htb;

};


#endif