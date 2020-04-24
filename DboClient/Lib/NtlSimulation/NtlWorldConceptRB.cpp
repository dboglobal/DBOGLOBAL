#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptRB.h"

// shared
#include "NtlRankBattle.h"
#include "WorldTable.h"
#include "TableContainer.h"
#include "NtlResultCode.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLRankBattle.h"
#include "NtlSobAvatar.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlCameraController.h"
#include "NtlCameraManager.h"

CNtlWorldConceptRB::CNtlWorldConceptRB(void)
{
	ChangeRBState( RANKBATTLE_BATTLESTATE_NONE );
	m_bFirstMatchStart = false;
}

CNtlWorldConceptRB::~CNtlWorldConceptRB(void)
{
	
}

RwBool CNtlWorldConceptRB::IsEnableAction( EAvatarAction eAction )
{
	if (!IsFirstMatchStart())
		return FALSE;

	// Set for each state
	switch(eAction)
	{
		case E_ACTION_MOVE:
		case E_ACTION_ATTACK:
		case E_ACTION_LOOTING:
		case E_ACTION_PICK_WORLD:
		{
			if (m_eRBState == RANKBATTLE_BATTLESTATE_STAGE_RUN)
				return TRUE;
		}
		break;

		case E_ACTION_PICK_OBJECT:
		case E_ACTION_TARGETTING:
		{
			if (m_eRBState == RANKBATTLE_BATTLESTATE_STAGE_READY || m_eRBState == RANKBATTLE_BATTLESTATE_STAGE_RUN)
				return TRUE;
		}
		break;

		case E_ACTION_CHANGE_HEADING:
		case E_ACTION_CAMERA_CONTROL:
		case E_ACTION_CHARGING:
		case E_ACTION_CAN_COMMUNICATION:
			return TRUE;
	}

	return FALSE;
}

void CNtlWorldConceptRB::Update( RwReal fElapsed ) 
{
	(this->*StateUpdate)( fElapsed );
}

void CNtlWorldConceptRB::ChangeState( RwInt32 eState )
{
	CNtlWorldConceptController::ChangeState( eState );

	switch( eState )
	{
		case WORLD_STATE_ENTER: 
		{
			// WorldConcept Initial settings. Or constructor.
			ChangeRBState(RANKBATTLE_BATTLESTATE_NONE);
		}
		break;
		case WORLD_STATE_EXIT:
		{
			// WorldConcept The last setting to make. Or destructor.
			CNtlWorldConceptController::ChangeState(WORLD_STATE_NONE);
		}
		break;
	}
}

void CNtlWorldConceptRB::ChangeRBState( RwInt32 eRBState )
{
	switch( eRBState )
	{
		case RANKBATTLE_BATTLESTATE_WAIT:
		{
			StateUpdate = &CNtlWorldConceptRB::UpdateState_Wait;
		}
		break;
		case RANKBATTLE_BATTLESTATE_DIRECTION:
		{
			RwV3d vPos = { 0.0f, 0.0f, 0.0f };
			GetNtlGameCameraManager()->AddDTSplineSelfNode(vPos, 0.0f, 0.0f, 0.0f, "Battle_arena_001"); 

			StateUpdate = &CNtlWorldConceptRB::UpdateState_Direction;
		}
		break;
		case RANKBATTLE_BATTLESTATE_STAGE_PREPARE:
		{
			CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_DT);
			ResetCamera();
			StateUpdate = &CNtlWorldConceptRB::UpdateState_Stage_Ready;
		}
		break;
		case RANKBATTLE_BATTLESTATE_STAGE_READY:
		{
			StateUpdate = &CNtlWorldConceptRB::UpdateState_Stage_Run;
		}
		break;
		case RANKBATTLE_BATTLESTATE_STAGE_RUN:
		{
			StateUpdate = &CNtlWorldConceptRB::UpdateState_Stage_Finish;
		}
		break;
		case RANKBATTLE_BATTLESTATE_MATCH_FINISH:
		{
			StateUpdate = &CNtlWorldConceptRB::UpdateState_Boss_Direction;
		}
		break;
		case RANKBATTLE_BATTLESTATE_BOSS_READY:
		{
			ResetCamera();
			StateUpdate = &CNtlWorldConceptRB::UpdateState_Boss_Ready;
		}
		break;
		case RANKBATTLE_BATTLESTATE_BOSS_RUN:
		{
			StateUpdate = &CNtlWorldConceptRB::UpdateState_Boss_Run;
		}
		break;
		case RANKBATTLE_BATTLESTATE_BOSS_FINISH:
		{
			StateUpdate = &CNtlWorldConceptRB::UpdateState_Boss_Finish;
		}
		break;
		case RANKBATTLE_BATTLESTATE_END:
		{
			StateUpdate = &CNtlWorldConceptRB::UpdateState_End;
		}
		break;
		case RANKBATTLE_BATTLESTATE_NONE:
		case INVALID_RANKBATTLE_BATTLESTATE:	
		{
			StateUpdate = &CNtlWorldConceptRB::UpdateState_None;
		}
		break;
	}

	m_eRBState = eRBState;
}

RwBool CNtlWorldConceptRB::IsMoveableArea(CNtlSobActor *pActor, const RwV3d *pPos, const RwV3d *pDestPos)
{
	CWorldTable *pWorldTbl = API_GetTableContainer()->GetWorldTable();
	sWORLD_TBLDAT *pWorldTblData = reinterpret_cast<sWORLD_TBLDAT*>( pWorldTbl->FindData(Logic_GetActiveWorldTableId()) );
	if(pWorldTblData == NULL)
		return TRUE;

	if(pWorldTblData->byWorldRuleType != GAMERULE_RANKBATTLE)
		return TRUE;

	RwReal fOutStartX	= pWorldTblData->vOutSideBattleStartLoc.x;
	RwReal fOutEndX		= pWorldTblData->vOutSideBattleEndLoc.x;

	RwReal fOutStartZ	= pWorldTblData->vOutSideBattleStartLoc.z;
	RwReal fOutEndZ		= pWorldTblData->vOutSideBattleEndLoc.z;

	RwReal fBattleStartX	= pWorldTblData->vBattleStartLoc.x;
	RwReal fBattleEndX		= pWorldTblData->vBattleEndLoc.x;

	RwReal fBattleStartZ	= pWorldTblData->vBattleStartLoc.z;
	RwReal fBattleEndZ		= pWorldTblData->vBattleEndLoc.z;

	if(fBattleStartX > fBattleEndX)
	{
		fBattleStartX	= pWorldTblData->vBattleEndLoc.x;
		fBattleEndX		= pWorldTblData->vBattleStartLoc.x;
	}

	if(fBattleStartZ > fBattleEndZ)
	{
		fBattleStartZ	= pWorldTblData->vBattleEndLoc.z; 
		fBattleEndZ		= pWorldTblData->vBattleStartLoc.z;
	}

	if(fOutStartX > fOutEndX)
	{
		fOutStartX	= pWorldTblData->vOutSideBattleEndLoc.x;
		fOutEndX	= pWorldTblData->vOutSideBattleStartLoc.x;
	}

	if(fOutStartZ > fOutEndZ)
	{
		fOutStartZ	= pWorldTblData->vOutSideBattleEndLoc.z;
		fOutEndZ	= pWorldTblData->vOutSideBattleStartLoc.z;
	}


	if(pDestPos->x < fOutStartX || pDestPos->x > fOutEndX ||
		pDestPos->z < fOutStartZ || pDestPos->z > fOutEndZ)
		return FALSE;

	RwV2d vCenter;
	vCenter.x = (fBattleEndX - fBattleStartX)/2.0f + fBattleStartX;
	vCenter.y = (fBattleEndZ - fBattleStartZ)/2.0f + fBattleStartZ;

	RwBool bRingOut = IsRingOut(pActor->GetSerialID());
	if(bRingOut)
	{
		if(pDestPos->x >= fBattleStartX && pDestPos->x <= fBattleEndX &&
			pDestPos->z >= fBattleStartZ && pDestPos->z <= fBattleEndZ)
		{
			RwV2d vCurrXDelta, vCurrYDelta, vDestXDelta, vDestYDelta;

			vCurrXDelta.x = vCenter.x - pPos->x;
			vCurrXDelta.y = 0.0f;

			vCurrYDelta.x = 0.0f;
			vCurrYDelta.y = vCenter.y - pPos->z;

			vDestXDelta.x = vCenter.x - pDestPos->x;
			vDestXDelta.y = 0.0f;

			vDestYDelta.x = 0.0f;
			vDestYDelta.y = vCenter.y - pDestPos->z;			

			RwReal fCurrXLen = RwV2dLength(&vCurrXDelta);
			RwReal fCurrYLen = RwV2dLength(&vCurrYDelta);
			RwReal fDestXLen = RwV2dLength(&vDestXDelta);
			RwReal fDestYLen = RwV2dLength(&vDestYDelta);

			if( fDestXLen <= fCurrXLen || fDestYLen <= fCurrYLen )
				return FALSE;

			return TRUE;
		}
		else
		{
			return TRUE;
		}
	}
	
	return TRUE;
}

RwBool CNtlWorldConceptRB::CanUseSkill(RwUInt32 hSerialID, RwUInt32& uiResultCode)
{
	RwBool bRingOut = IsRingOut(hSerialID);

	if(bRingOut)
	{
		uiResultCode = GAME_MATCH_CAN_NOT_USE_SKILL_IN_OUTOFAREA;

		return FALSE;
	}
	else
	{
		if (m_eRBState != RANKBATTLE_BATTLESTATE_STAGE_RUN)
		{
			uiResultCode = GAME_SKILL_INVALID_TARGET_APPOINTED;
			return FALSE;
		}
	}

	return TRUE;
}

RwBool CNtlWorldConceptRB::IsRingOut( RwUInt32 hSerialID )
{
	if( hSerialID == INVALID_SERIAL_ID )
		return FALSE;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	NTL_ASSERT(pAvatar, "Not exist sob avatar instance");

	CNtlRankBattle::stRankBattleMember* pMember = pAvatar->GetRankBattle()->FindMemberData( hSerialID );

	if( pMember )
	{
		if( pMember->byState == RANKBATTLE_MEMBER_STATE_OUTOFAREA )
			return TRUE;
	}	
	else
	{
		DBO_WARNING_MESSAGE("Member: " << hSerialID << " not found");
	}	

	return FALSE;
}

void CNtlWorldConceptRB::ResetCamera(VOID)
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	RwV3d vDir	= pSobAvatar->GetDirection();
	RwReal fRotY = CNtlMath::LineToAngleY( &vDir );

	GetNtlGameCameraManager()->SetYaw( fRotY );
	GetNtlGameCameraManager()->SetPitch( 10.0f );		
	GetNtlGameCameraManager()->SetDist( 20.0f );
}

void CNtlWorldConceptRB::UpdateState_None( RwReal fElapsed )
{

}
		
void CNtlWorldConceptRB::UpdateState_Wait( RwReal fElapsed )
{

}

void CNtlWorldConceptRB::UpdateState_Direction( RwReal fElapsed )
{

}

void CNtlWorldConceptRB::UpdateState_Stage_Ready( RwReal fElapsed )
{

}

void CNtlWorldConceptRB::UpdateState_Stage_Run( RwReal fElapsed )
{

}

void CNtlWorldConceptRB::UpdateState_Stage_Finish( RwReal fElapsed )
{

}

void CNtlWorldConceptRB::UpdateState_Boss_Direction( RwReal fElapsed )
{

}

void CNtlWorldConceptRB::UpdateState_Boss_Ready( RwReal fElapsed )
{

}

void CNtlWorldConceptRB::UpdateState_Boss_Run( RwReal fElapsed )
{

}

void CNtlWorldConceptRB::UpdateState_Boss_Finish( RwReal fElapsed )
{

}

void CNtlWorldConceptRB::UpdateState_End( RwReal fElapsed )
{

}