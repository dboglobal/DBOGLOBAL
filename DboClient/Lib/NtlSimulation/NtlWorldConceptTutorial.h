#pragma once

// share
#include "NtlTimeQuest.h"

// simulation
#include "NtlWorldConceptController.h"
#include "DboTLSystem.h"
#include "DboTLConditionCheck.h"

/**
 * \ingroup NtlSimulation
 * \brief Tutorial시의 제어 클래스
 * \date 2007-07-20
 * \author avooo
 * TMQ와 동일한 클라이언트 상태를 유지한다
 */
class CNtlWorldConceptTutorial : public CNtlWorldConceptController
{
public:
	CNtlWorldConceptTutorial(void);
	~CNtlWorldConceptTutorial(void);

	virtual void		Update(RwReal fElapsedTime);
	virtual RwBool		IsEnableAction(EAvatarAction eAction);
	virtual void		HandleEvents(RWS::CMsg &pMsg);

	void				BeforeLoadingDirect(void);
	void				AfterLoadingDirect(void);

	RwBool				IsBeforeLoadingDirect(void);
	RwBool				IsAfterLoadingDirect(void);
	RwBool				CanMouseInput(ETL_MOUSE_INPUT_TYPE byType);
	RwBool				CanKeybaordInput(ETL_KEYBOARD_INPUT_TYPE byType);
	RwBool				CanAttack(ETL_ATTACK_TYPE byType);
	RwBool				IsDisableMinimap();
	RwBool				IsStepSkipping();

	void					SetTMQState(eTIMEQUEST_GAME_STATE eState);
	void					SetTMQState_from_Cleint(RwUInt8 byState);	// 클라이언트 자체적으로 상태를 변경한다

	void					NotifyTMQStateUpdate();
	eTIMEQUEST_GAME_STATE	GetTMQState();

	void					NextStep(void);

	VOID					LoadScript(void);

protected:
	RwUInt8					m_byTMQState;
	RwBool					m_bDisableMinimap;

	CDboTLSystem*			m_pTLSystem;
	CDboTLConditionCheck*	m_pConditionCheck;
};

inline eTIMEQUEST_GAME_STATE CNtlWorldConceptTutorial::GetTMQState()
{
	return (eTIMEQUEST_GAME_STATE)m_byTMQState;
}