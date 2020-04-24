#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptTMQ.h"
#include "TimeQuestTable.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEventFunc.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSLTMQ.h"


CNtlWorldConceptTMQ::CNtlWorldConceptTMQ(void)
{
}

CNtlWorldConceptTMQ::~CNtlWorldConceptTMQ(void)
{
	CNtlSLEventGenerator::CameraTimeMachineArrive( FALSE, INVALID_SERIAL_ID );
}

RwBool CNtlWorldConceptTMQ::IsEnableAction(EAvatarAction eAction) 
{
	switch(m_byTMQState)
	{
	case WORLD_STATE_TMQ_BEGIN:
	case WORLD_STATE_TMQ_STAGE_READY:
	case WORLD_STATE_TMQ_STAGE_START:
	case WORLD_STATE_TMQ_STAGE_FINISH:
	case WORLD_STATE_TMQ_END:
		return TRUE;
	}

	return FALSE;
}

void CNtlWorldConceptTMQ::ChangeState(RwInt32 iState)
{
	CNtlWorldConceptController::ChangeState(iState);
}

void CNtlWorldConceptTMQ::BeforeLoadingDirect(void)
{
}

void CNtlWorldConceptTMQ::AfterLoadingDirect(void)
{
}

RwBool CNtlWorldConceptTMQ::IsBeforeLoadingDirect(void)
{
	return FALSE;
}

RwBool CNtlWorldConceptTMQ::IsAfterLoadingDirect(void)
{
	return FALSE;
}

void CNtlWorldConceptTMQ::SetTMQState(eTIMEQUEST_GAME_STATE eState)
{
	// Converting server state to client state
	switch(eState)
	{
		case TIMEQUEST_GAME_STATE_WAIT:				m_byTMQState = WORLD_STATE_TMQ_WAIT;					break;
		case TIMEQUEST_GAME_STATE_PREPARE:			m_byTMQState = WORLD_STATE_TMQ_PREPARE;				break;
		case TIMEQUEST_GAME_STATE_ARRIVE:			m_byTMQState = WORLD_STATE_TMQ_ARRIVE;				break;
		case TIMEQUEST_GAME_STATE_BEGIN:			m_byTMQState = WORLD_STATE_TMQ_BEGIN;				break;
		case TIMEQUEST_GAME_STATE_STAGE_READY:		m_byTMQState = WORLD_STATE_TMQ_STAGE_READY;			break;
		case TIMEQUEST_GAME_STATE_STAGE_START:		m_byTMQState = WORLD_STATE_TMQ_STAGE_START;			break;
		case TIMEQUEST_GAME_STATE_STAGE_FINISH:		m_byTMQState = WORLD_STATE_TMQ_STAGE_FINISH;			break;
		case TIMEQUEST_GAME_STATE_END:				m_byTMQState = WORLD_STATE_TMQ_END;					break;
		case TIMEQUEST_GAME_STATE_LEAVE:			m_byTMQState = WORLD_STATE_TMQ_LEAVE;
		case TIMEQUEST_GAME_STATE_FAIL:				m_byTMQState = WORLD_STATE_TMQ_FAIL;					break;
		case TIMEQUEST_GAME_STATE_CLOSE:			m_byTMQState = WORLD_STATE_TMQ_CLOSE;				break;
		default:	DBO_FAIL("Can not treat server tmq state : " << eState);
	}

	NotifyTMQStateUpdate();
}

void CNtlWorldConceptTMQ::SetTMQState_from_Cleint(RwUInt8 byState)
{
	m_byTMQState = byState;

	NotifyTMQStateUpdate();
}

void CNtlWorldConceptTMQ::NotifyTMQStateUpdate()
{
	CNtlSLEventGenerator::TMQCleintState(WORLD_PLAY_TIME_MACHINE, m_byTMQState);

	switch ( m_byTMQState )
	{
		case WORLD_STATE_TMQ_TERRAIN_READY:
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( !pAvatar )
			{
				DBO_FAIL("Not exist avatar instance");
				break;
			}

			CNtlTMQ* pTMQData = pAvatar->GetTMQ();

			if ( pTMQData->GetTMQTable() )
			{
				CNtlSLEventGenerator::CameraTimeMachineArrive( TRUE, GetNtlSobManager()->GetObjectTriggerSerialFromId( pTMQData->GetTMQTable()->arriveObjectIndex ) );
			}
		}
		break;

		case WORLD_STATE_TMQ_BEGIN:
		{
			CNtlSLEventGenerator::CameraTimeMachineArrive( FALSE, INVALID_SERIAL_ID );
		}
		break;
	}
}