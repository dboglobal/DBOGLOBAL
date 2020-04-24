#ifndef __SERVER_BOT_ASPECT_STATE__
#define __SERVER_BOT_ASPECT_STATE__

#include "StateBase.h"

class CCharacter;

class CAspectState : public CStateBase
{

public:
	CAspectState(BYTE stateID, CCharacter *pParent);
	virtual ~CAspectState();

public:

	virtual void OnEnter() { m_dwLastTransformEffect = 0; m_dwDuration = INVALID_DWORD; }
	virtual void OnExit() { m_dwLastTransformEffect = 0; m_dwDuration = INVALID_DWORD; }
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple) { return NULL; }
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg) { return NULL; }

	virtual bool CanTransition(BYTE stateID)  { return false; } //check if current state can change into stateID
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail) { return NULL; }
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail) {}

	virtual int CopyAspectTo(sASPECTSTATE_DETAIL* pStateDetail) { return NULL; }
	virtual void CopyAspectFrom(sASPECTSTATE_DETAIL* pStateDetail) {}
	virtual const char* GetStateName() { return NtlGetAspectStateString(GetStateID()); }

protected:

	CCharacter* m_pParent;

	DWORD		m_dwLastTransformEffect;

	DWORD		m_dwDuration;

};


#endif