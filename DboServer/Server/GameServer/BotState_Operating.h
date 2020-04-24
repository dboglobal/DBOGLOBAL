#ifndef __SERVER_BOT_STATE_OPERATING__
#define __SERVER_BOT_STATE_OPERATING__

#include "CharState.h"

class CBotState_Operating : public CCharState
{

public:
	CBotState_Operating(CCharacter* pParent);
	virtual ~CBotState_Operating();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

public:

	HOBJECT				GetTarget() { return hTargetObject; }

private:

	HOBJECT				hTargetObject; // Handle of the target object
	DWORD				dwOperateTime; // Operating time
	TBLIDX				directTblidx; // production TBLIDX

};


#endif