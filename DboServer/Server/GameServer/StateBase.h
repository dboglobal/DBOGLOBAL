#ifndef __SERVER_BASE_STATE__
#define __SERVER_BASE_STATE__

#include "NtlCharacter.h"

class CObjectMsg;

class CStateBase
{

public:
	CStateBase(BYTE stateID);
	virtual ~CStateBase();

public:

	virtual void OnEnter() {};
	virtual void OnExit() {};
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple) { return 0; };
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg) { return 0; };
	virtual bool CanTransition(BYTE stateID) { return true; };	//always true except if owner is PC
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail) { return 0; };
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail) {};
	virtual bool IsValidTarget() { return true; }

	virtual int CopyAspectTo(sASPECTSTATE_DETAIL* pStateDetail) { return 0; };
	virtual void CopyAspectFrom(sASPECTSTATE_DETAIL* pStateDetail) {};
	virtual const char* GetStateName() = 0;

public:

	inline BYTE GetStateID() { return this->m_stateID; }
	inline BYTE GetPrevStateID() { return this->m_prevStateID; }
	inline void SetPrevStateID(BYTE stateID) { this->m_prevStateID = stateID; }
	inline BYTE GetNextStateID() { return this->m_nextStateID; }
	inline void SetNextStateID(BYTE stateID) { this->m_nextStateID = stateID; }
	inline DWORD GetStateTime() { return this->m_dwTime; }
	inline void SetStateTime(DWORD dwTime) { this->m_dwTime = dwTime; }
	inline void SetFinish(bool bFinish) { this->m_bFinish = bFinish; }
	inline bool IsFinished() { return this->m_bFinish; }
	
public:

	inline void SetInitialData(void* pvInitialData) { this->m_pvInitialData = pvInitialData; }
	inline void ResetInitialData() { this->m_pvInitialData = NULL; }
	inline void SetReferenceDataForExiting(void* pvReferenceDataForExiting) { this->m_pvReferenceDataForExiting = pvReferenceDataForExiting; }
	inline void ResetReferenceDataForExiting() { this->m_pvReferenceDataForExiting = NULL; }

private:

	BYTE m_stateID;
	BYTE m_prevStateID;
	BYTE m_nextStateID;
	DWORD m_dwTime;
	bool m_bFinish;
	void *m_pvInitialData;
	void *m_pvReferenceDataForExiting;

};


#endif