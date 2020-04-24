#ifndef __SERVER_BOT_STATE_TURNING__
#define __SERVER_BOT_STATE_TURNING__

#include "CharState.h"

class CBotState_Turning : public CCharState
{

public:
	CBotState_Turning(CCharacter* pParent);
	virtual ~CBotState_Turning();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

private:

	TBLIDX m_directTblidx;
	sVECTOR3 m_vDestDir;

};


#endif