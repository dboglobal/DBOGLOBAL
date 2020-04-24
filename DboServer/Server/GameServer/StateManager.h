#ifndef __SERVER_STATE_MANAGER__
#define __SERVER_STATE_MANAGER__

#include "ConditionState.h" // HERE: #include "StateBase.h"

class CCharState;
class CAspectState;

class CCharacter;
class CObjectMsg;

class CStateManager
{

public:
	CStateManager();
	virtual ~CStateManager();

public:

	virtual bool			Create(CCharacter* pParentRef, eAIR_STATE eAirState);

protected:

	virtual void			Init();
	virtual void			Destroy();

public:

	CCharState*				GetCharStateInstance(BYTE stateID);
	CAspectState*			GetAspectInstance(BYTE stateID);
	CConditionState*		GetConditionInstance(BYTE stateID);

	virtual int				OnUpdate(DWORD dwTickDiff, float fMultiple);

public:

	bool					ChangeCharState(BYTE stateID, void *pvInitialData, bool bUpdate);
	bool					CanCharStateTransition(BYTE stateID);

	BYTE					GetCharStateID();

	DWORD					GetCharStateTime();

	bool					IsCharState(BYTE stateID);

	CCharState*				GetCharState();

	bool					ChangeAspectState(BYTE stateID, void *pvInitialData, bool bUpdate);

	BYTE					GetAspectStateID();

	bool					AddConditionState(BYTE stateID, void *pvInitialData, bool bUpdate);
	bool					RemoveConditionState(BYTE stateID, void *pvReferenceDataForExiting, bool bUpdate);
	bool					AddConditionFlags(QWORD qwCharConditionFlag, bool bUpdate);
	bool					RemoveConditionFlags(QWORD qwCharConditionFlag, bool bUpdate);
	bool					IsCharCondition(BYTE stateID);

	inline QWORD			GetConditionState() { return this->m_qwConditionFlag; }
	inline void				SetConditionState(QWORD qwConditionFlag) { this->m_qwConditionFlag = qwConditionFlag; }

	bool					IsValidTarget(DWORD dwNoCheckStateFlag);

	int						OnObjectMsg(CObjectMsg* pObjectMsg);

	int						CopyTo(sCHARSTATE* pCharState);
	bool					CopyFrom(sCHARSTATE* pCharState);

	int						CopyAspectTo(sASPECTSTATE* pAspectState);
	void					CopyAspectFrom(sASPECTSTATE* pAspectState);

	eAIR_STATE				GetAirState() { return this->m_eAirState; }
	void					SetAirState(eAIR_STATE eAirState) { this->m_eAirState = eAirState; }


private:

	void					RegisterState(CCharState *pState);
	void					RegisterAspectState(CAspectState *pState);
	void					RegisterConditionState(CConditionState *pState);

protected:

	CCharacter*				m_pParentRef;

private:

	CCharState*				m_pCharState;
	CAspectState*			m_pAspectState;
	QWORD					m_qwConditionFlag;
	eAIR_STATE				m_eAirState;
	CConditionState*		m_apConditionState[CHARCONDITION_COUNT];


	CCharState*				m_apStateInstance[CHARSTATE_COUNT];
	CAspectState*			m_apAspectStateInstance[ASPECTSTATE_COUNT];
	CConditionState*		m_apConditionStateInstance[CHARCONDITION_COUNT];
};


#endif