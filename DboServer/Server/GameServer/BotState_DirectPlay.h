#ifndef __SERVER_BOT_STATE_DIRECTPLAY__
#define __SERVER_BOT_STATE_DIRECTPLAY__

#include "CharState.h"

class CBotState_DirectPlay : public CCharState
{

public:
	CBotState_DirectPlay(CCharacter* pParent);
	virtual ~CBotState_DirectPlay();

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

	BYTE				m_byDirectPlayType;
	TBLIDX				m_directTblidx;

};


#endif