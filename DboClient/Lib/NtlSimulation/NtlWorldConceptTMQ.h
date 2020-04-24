#pragma once

// share
#include "NtlTimeQuest.h"

// simulation
#include "NtlWorldConceptController.h"

/**
 * \ingroup NtlSimulation
 * \brief Time machine quest 시의 제어 클래스
 * \date 2007-07-20
 * \author avooo
 */
class CNtlWorldConceptTMQ : public CNtlWorldConceptController
{
public:
	CNtlWorldConceptTMQ(void);
	~CNtlWorldConceptTMQ(void);

	virtual RwBool		IsEnableAction(EAvatarAction eAction);
	virtual void		ChangeState(RwInt32 iState);

	void				BeforeLoadingDirect(void);
	void				AfterLoadingDirect(void);

	RwBool				IsBeforeLoadingDirect(void);
	RwBool				IsAfterLoadingDirect(void);

	
	void					SetTMQState(eTIMEQUEST_GAME_STATE eState);	// 서버로 부터 상태가 내려온다	
	void					SetTMQState_from_Cleint(RwUInt8 byState);	// 클라이언트 자체적으로 상태를 변경한다

	void					NotifyTMQStateUpdate();
	RwUInt8					GetTMQState();


protected:
	RwUInt8				m_byTMQState;
};

inline RwUInt8 CNtlWorldConceptTMQ::GetTMQState()
{
	return m_byTMQState;
}