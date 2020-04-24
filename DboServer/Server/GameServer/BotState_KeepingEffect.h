#ifndef __SERVER_BOT_STATE_KEEPINGEFFECT__
#define __SERVER_BOT_STATE_KEEPINGEFFECT__

#include "CharState.h"

class CBotState_KeepingEffect : public CCharState
{

public:
	CBotState_KeepingEffect(CCharacter* pParent);
	virtual ~CBotState_KeepingEffect();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

private:
	TBLIDX		m_skillId;
};


#endif