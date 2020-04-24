#ifndef __SERVER_BOT_STATE__
#define __SERVER_BOT_STATE__

#include "StateBase.h"

class CCharacter;

class CCharState : public CStateBase
{

public:
	CCharState(BYTE stateID, CCharacter* pParent);
	virtual ~CCharState();

public:

	virtual void	OnEnter();
	virtual int		OnUpdate(DWORD dwTickDiff, float fMultiple);

	virtual int CopyAspectTo(sASPECTSTATE_DETAIL* pStateDetail) { return NULL; }
	virtual void CopyAspectFrom(sASPECTSTATE_DETAIL* pStateDetail) {}
	virtual const char* GetStateName() { return NtlGetCharStateString(GetStateID()); }

protected:

	CCharacter* m_pParent;

};


#endif