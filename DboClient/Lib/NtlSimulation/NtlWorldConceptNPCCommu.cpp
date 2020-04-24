#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptNPCCommu.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLEventFunc.h"

CNtlWorldConceptNPCCOMMU::CNtlWorldConceptNPCCOMMU(void)
{
	
}

CNtlWorldConceptNPCCOMMU::~CNtlWorldConceptNPCCOMMU(void)
{
}

RwBool CNtlWorldConceptNPCCOMMU::IsEnableAction(EAvatarAction eAction) 
{
	switch(eAction)
	{
	case E_ACTION_MOVE:
	case E_ACTION_PICK_WORLD:
	case E_ACTION_PICK_OBJECT:
	case E_ACTION_TARGETTING:
	case E_ACTION_ATTACK:
	case E_ACTION_CHANGE_HEADING:
	case E_ACTION_CAMERA_CONTROL:
	case E_ACTION_CHARGING:
	case E_ACTION_LOOTING:	
	case E_ACTION_CAN_COMMUNICATION:
		return FALSE;
	}

	return FALSE;
}

void CNtlWorldConceptNPCCOMMU::ChangeState(RwInt32 iState)
{
	CNtlWorldConceptController::ChangeState(iState);

	switch(iState)
	{
	case WORLD_STATE_ENTER:
		{
			CNtlSLEventGenerator::ActionMapHalt();
			break;
		}
	case WORLD_STATE_EXIT:
		{
			CNtlWorldConceptController::ChangeState(WORLD_PLAY_NONE);

			break;
		}
	}
}