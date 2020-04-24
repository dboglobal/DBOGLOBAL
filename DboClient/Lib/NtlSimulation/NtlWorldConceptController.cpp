#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptController.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlWorldConcept.h"

#include "NtlSob.h"
#include "NtlSobGroup.h"
#include "NtlSobManager.h"


CNtlWorldConceptController::CNtlWorldConceptController(void)
{
	m_fStateTime	= 0.0f;
	m_iWorldState	= WORLD_STATE_NONE;
	m_eType			= WORLD_PLAY_NONE;
	m_bPause		= FALSE;
}

CNtlWorldConceptController::~CNtlWorldConceptController(void)
{
}

/**
 * 디폴트는 모든 액션이 가능한 상태이다. 각 컨셉에 따라 하면 안되는 액션이 있는 경우에는 오버라이딩한다.
 * \param eAction 액션 가능 유무 판별 플래그
 * return 액션 가능 유무
 */
RwBool CNtlWorldConceptController::IsEnableAction( EAvatarAction eAction ) 
{
	return TRUE;
}

/**
 * 디폴트는 아무것도 하지 않는다. 컨셉에 따라 하위 클래스에서 오버라이딩한다. 
 */
void CNtlWorldConceptController::Update( RwReal fElapsedTime ) 
{
	switch(m_iWorldState)
	{
		case WORLD_STATE_ENTER:
		case WORLD_STATE_IDLE:
			break;
		case WORLD_STATE_EXIT:
			ChangeState(WORLD_STATE_NONE);		
			break;
	}
}

void CNtlWorldConceptController::ChangeState( RwInt32 iState ) 
{
	m_fStateTime = 0.0f;
	m_iWorldState = iState;

	CNtlSLEventGenerator::ChangeWorldConceptState(m_eType, iState);
}

void CNtlWorldConceptController::SetConceptType(EWorldPlayConcept eType)
{
	m_eType = eType;
}

EWorldPlayConcept CNtlWorldConceptController::GetConceptType(void)
{
	return m_eType;
}

void CNtlWorldConceptController::Pause(RwBool bPause)
{
	m_bPause = bPause;
}

