#ifndef __SERVER_CONDITION_STATE__
#define __SERVER_CONDITION_STATE__

#include "StateBase.h" //here: #include "NtlCharacter.h"

class CCharacter;

class CConditionState : public CStateBase
{

public:
	CConditionState(BYTE stateID, CCharacter *pParent);
	virtual ~CConditionState();

public:

	virtual const char* GetStateName() { return NtlGetConditionStateString(GetStateID()); }

	inline DWORD GetReferCount() { return m_dwReferCount; }
	inline void SetReferCount(DWORD dwReferCount) { m_dwReferCount = dwReferCount; }

	inline void IncreaseReferCount() { ++m_dwReferCount; }
	inline void DecreaseReferCount() { --m_dwReferCount; }

	virtual bool IsStackable() { return true; }

private:

	DWORD m_dwReferCount;

protected:

	CCharacter* m_pParent;

};


#endif