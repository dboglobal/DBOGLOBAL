#ifndef __SERVER_BOT_STATE_SPAWNING__
#define __SERVER_BOT_STATE_SPAWNING__

#include "CharState.h"

class CBotState_Spawning : public CCharState
{

public:
	CBotState_Spawning(CCharacter* pParent);
	virtual ~CBotState_Spawning();

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

	BYTE m_byTeleportType;
	bool m_bSpawnDirection;
	bool m_bIsFaint; //if true, then client thinks the player is dead while spawn

};


#endif