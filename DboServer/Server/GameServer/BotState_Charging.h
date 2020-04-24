#ifndef __SERVER_BOT_STATE_CHARGING__
#define __SERVER_BOT_STATE_CHARGING__

#include "CharState.h"

class CBotState_Charging : public CCharState
{

public:
	CBotState_Charging(CCharacter* pParent);
	virtual ~CBotState_Charging();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

private:

	float m_fCurrentValue;
	float m_fDestValue;
	float m_fRPChargingTime;
	float m_fCurrentRPChargingTime;

};


#endif