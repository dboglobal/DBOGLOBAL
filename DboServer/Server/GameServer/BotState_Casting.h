#ifndef __SERVER_BOT_STATE_CASTING__
#define __SERVER_BOT_STATE_CASTING__

#include "CharState.h"

class CBotState_Casting : public CCharState
{

public:
	CBotState_Casting(CCharacter* pParent);
	virtual ~CBotState_Casting();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

private:

	TBLIDX			m_skillId;
	HOBJECT			m_hTarget;
	DWORD			m_dwCastingTime;
	DWORD			m_dwCastingTimeRemaining;

};


#endif