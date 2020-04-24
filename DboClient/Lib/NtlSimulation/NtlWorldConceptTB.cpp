#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptTB.h"

// shared
#include "WorldTable.h"
#include "TableContainer.h"
#include "NtlResultCode.h"

#include "PCTable.h"

#include "GUISoundDefine.h"
#include "NtlSoundEventGenerator.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLTBudokai.h"
#include "NtlSobAvatar.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlCameraController.h"
#include "NtlCameraManager.h"

CNtlWorldConceptTB::CNtlWorldConceptTB(void)
{
	m_pMatchTblDat = NULL;
	m_eMatchType = INVALID_MATCHWORLD_TYPE;
	m_eMatchState = INVALID_BYTE;	
	m_bEnterState = FALSE;
	m_wMyTeamType = INVALID_WORD;
	m_byMatchDepth = INVALID_BYTE;
	m_byMatchIndex = INVALID_BYTE;
	StateUpdate = &CNtlWorldConceptTB::UpdateState_Null;
}

CNtlWorldConceptTB::~CNtlWorldConceptTB(void)
{	
	ClearMemberData();
}

RwBool CNtlWorldConceptTB::IsEnableAction( EAvatarAction eAction )
{
	// 각 상태에 맞게 세팅
	switch(eAction)
	{
	case E_ACTION_CAN_COMMUNICATION:
	case E_ACTION_CHANGE_HEADING:		
	case E_ACTION_CHARGING:		
	case E_ACTION_CAMERA_CONTROL:
	case E_ACTION_MOVE:		
	case E_ACTION_PICK_WORLD:
	case E_ACTION_PICK_OBJECT:
	case E_ACTION_TARGETTING:
	case E_ACTION_LOOTING:		
	case E_ACTION_ATTACK:
		if( m_eMatchType == MATCHWORLD_TYPE_MINOR_MATCH && m_eMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_RUN ||
			m_eMatchType == MATCHWORLD_TYPE_MINOR_MATCH && m_eMatchState == BUDOKAI_MINORMATCH_STATE_WAIT_MINORMATCH ||
			m_eMatchType == MATCHWORLD_TYPE_MAJOR_MATCH && m_eMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_RUN ||
			m_eMatchType == MATCHWORLD_TYPE_FINAL_MATCH && m_eMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_RUN )
			return TRUE;
		else 
			return FALSE;		
	}

	return FALSE;
}

void CNtlWorldConceptTB::Update( RwReal fElapsed ) 
{
	(this->*StateUpdate)( fElapsed );
}

void CNtlWorldConceptTB::ChangeState( RwInt32 eState )
{
	CNtlWorldConceptController::ChangeState( eState );

	switch( eState )
	{
	case WORLD_STATE_ENTER: 
		// WorldConcept 초기에 해야할 세팅. 혹은 생성자.
		// 여기서 테이블을 읽어서 현재 월드타입을 세팅해야함.		
		break;
	case WORLD_STATE_EXIT:	
		// WorldConcept 마지막에 해야할 세팅. 혹은 소멸자.
		CNtlWorldConceptController::ChangeState( WORLD_STATE_NONE );
		break;
	}
}

void CNtlWorldConceptTB::ChangeTBType( RwUInt8 eTBType )
{
	m_eMatchType = eTBType;
}

void CNtlWorldConceptTB::ChangeTBState( RwUInt8 eTBState, RwBool bEnterState )
{
	if( bEnterState )
	{
		switch( m_eMatchType )
		{
		case MATCHWORLD_TYPE_MINOR_MATCH :
			{
				switch( eTBState )
				{
				case BUDOKAI_MINORMATCH_STATE_NONE : StateUpdate = &CNtlWorldConceptTB::UpdateState_MinorMatch_None; break;
				case BUDOKAI_MINORMATCH_STATE_WAIT_MINORMATCH : StateUpdate = &CNtlWorldConceptTB::UpdateState_MinorMatch_WaitMinorMatch; break;
				case BUDOKAI_MINORMATCH_STATE_WAIT : StateUpdate = &CNtlWorldConceptTB::UpdateState_MinorMatch_Wait; break;
				case BUDOKAI_MINORMATCH_STATE_DIRECTION : StateUpdate = &CNtlWorldConceptTB::UpdateState_MinorMatch_Direction; break;		
				case BUDOKAI_MINORMATCH_STATE_MATCH_READY: StateUpdate = &CNtlWorldConceptTB::UpdateState_MinorMatch_Match_Ready; break;
				case BUDOKAI_MINORMATCH_STATE_STAGE_READY :	StateUpdate = &CNtlWorldConceptTB::UpdateState_MinorMatch_Stage_Ready; break;
				case BUDOKAI_MINORMATCH_STATE_STAGE_RUN : StateUpdate = &CNtlWorldConceptTB::UpdateState_MinorMatch_Stage_Run; break;		
				case BUDOKAI_MINORMATCH_STATE_STAGE_FINISH : StateUpdate = &CNtlWorldConceptTB::UpdateState_MinorMatch_Stage_Finish; break;	
				case BUDOKAI_MINORMATCH_STATE_MATCH_FINISH : StateUpdate = &CNtlWorldConceptTB::UpdateState_MinorMatch_Match_Finish; break;	
				case BUDOKAI_MINORMATCH_STATE_END : &CNtlWorldConceptTB::UpdateState_MinorMatch_End; break;
				default : StateUpdate = &CNtlWorldConceptTB::UpdateState_Null; break;
				}
			}
			break;
		case MATCHWORLD_TYPE_MAJOR_MATCH :
			{
				switch( eTBState )
				{
				case BUDOKAI_MAJORMATCH_STATE_NONE : StateUpdate = &CNtlWorldConceptTB::UpdateState_MajorMatch_None; break;
				case BUDOKAI_MAJORMATCH_STATE_WAIT : StateUpdate = &CNtlWorldConceptTB::UpdateState_MajorMatch_Wait; break;
				case BUDOKAI_MAJORMATCH_STATE_DIRECTION : StateUpdate = &CNtlWorldConceptTB::UpdateState_MajorMatch_Direction; break;
				case BUDOKAI_MAJORMATCH_STATE_MATCH_READY : StateUpdate = &CNtlWorldConceptTB::UpdateState_MajorMatch_Match_Ready; break;
				case BUDOKAI_MAJORMATCH_STATE_STAGE_READY :	StateUpdate = &CNtlWorldConceptTB::UpdateState_MajorMatch_Stage_Ready; break;
				case BUDOKAI_MAJORMATCH_STATE_STAGE_RUN : StateUpdate = &CNtlWorldConceptTB::UpdateState_MajorMatch_Stage_Run; break;		
				case BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH : StateUpdate = &CNtlWorldConceptTB::UpdateState_MajorMatch_Stage_Finish; break;	
				case BUDOKAI_MAJORMATCH_STATE_MATCH_FINISH : StateUpdate = &CNtlWorldConceptTB::UpdateState_MajorMatch_Match_Finish; break;	
				case BUDOKAI_MAJORMATCH_STATE_END : &CNtlWorldConceptTB::UpdateState_MajorMatch_End; break;
				default : StateUpdate = &CNtlWorldConceptTB::UpdateState_Null; break;
				}
			}
			break;
		case MATCHWORLD_TYPE_FINAL_MATCH :
			{
				switch( eTBState )
				{
				case BUDOKAI_FINALMATCH_STATE_NONE : StateUpdate = &CNtlWorldConceptTB::UpdateState_FinalMatch_None; break;
				case BUDOKAI_FINALMATCH_STATE_WAIT : StateUpdate = &CNtlWorldConceptTB::UpdateState_FinalMatch_Wait; break;
				case BUDOKAI_FINALMATCH_STATE_DIRECTION :StateUpdate = &CNtlWorldConceptTB::UpdateState_FinalMatch_Direction; break;	
				case BUDOKAI_FINALMATCH_STATE_MATCH_READY :	StateUpdate = &CNtlWorldConceptTB::UpdateState_FinalMatch_Match_Ready; break;
				case BUDOKAI_FINALMATCH_STATE_STAGE_READY :	StateUpdate = &CNtlWorldConceptTB::UpdateState_FinalMatch_Stage_Ready; break;
				case BUDOKAI_FINALMATCH_STATE_STAGE_RUN : StateUpdate = &CNtlWorldConceptTB::UpdateState_FinalMatch_Stage_Run; break;		
				case BUDOKAI_FINALMATCH_STATE_STAGE_FINISH : StateUpdate = &CNtlWorldConceptTB::UpdateState_FinalMatch_Stage_Finish; break;	
				case BUDOKAI_FINALMATCH_STATE_MATCH_FINISH : StateUpdate = &CNtlWorldConceptTB::UpdateState_FinalMatch_Match_Finish; break;	
				case BUDOKAI_FINALMATCH_STATE_AWARDING : StateUpdate = &CNtlWorldConceptTB::UpdateState_FinalMatch_Awarding; break;
				case BUDOKAI_FINALMATCH_STATE_END : &CNtlWorldConceptTB::UpdateState_FinalMatch_End; break;
				default : StateUpdate = &CNtlWorldConceptTB::UpdateState_Null; break;
				}
			}
			break;
		default:
			{
				StateUpdate = &CNtlWorldConceptTB::UpdateState_Null;
			}
			break;
		}

		m_eMatchState = eTBState;
	}
	
	m_bEnterState = bEnterState;
	
#ifdef BUDOKAI_TIME_DEBUG
	WCHAR buf[256];
	WCHAR* szType[MAX_MATCHWORLD_TYPE] = { L"MINOR", L"MAJOR", L"FINAL" };
	WCHAR* szState[MAX_MATCHWORLD_TYPE][MAX_BUDOKAI_FINALMATCH_STATE] = { L"NONE", L"WAIT_MINORMATCH", L"WAIT", L"DIRECTION", L"MATCH_READY", L"STAGE_READY", L"STAGE_RUN", L"STAGE_FINISH", L"MATCH_FINISH", L"END", L"ERROR",
																		  L"NONE", L"WAIT", L"DIRECTION", L"MATCH_READY", L"STAGE_READY", L"STAGE_RUN", L"STAGE_FINISH", L"MATCH_FINISH", L"END", L"ERROR", L"ERROR",
																		  L"NONE", L"WAIT", L"DIRECTION", L"MATCH_READY", L"STAGE_READY", L"STAGE_RUN", L"STAGE_FINISH", L"MATCH_FINISH", L"FINAL_DIRECTION", L"AWARDING", L"END" };
	
	swprintf_s( buf, 256, L"((MatchStateUpdate)) Type = %s, State = %s,(%d)", szType[m_eMatchType], szState[m_eMatchType][eTBState], eTBState );
	CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
#endif	
}

RwBool CNtlWorldConceptTB::IsMoveableArea(CNtlSobActor *pActor, const RwV3d *pPos, const RwV3d *pDestPos)
{
	CWorldTable *pWorldTbl = API_GetTableContainer()->GetWorldTable();
	sWORLD_TBLDAT *pWorldTblData = reinterpret_cast<sWORLD_TBLDAT*>( pWorldTbl->FindData(Logic_GetActiveWorldTableId()) );
	if(pWorldTblData == NULL)
		return TRUE;

	if( pWorldTblData->byWorldRuleType != GAMERULE_MINORMATCH &&
		pWorldTblData->byWorldRuleType != GAMERULE_MAJORMATCH &&
		pWorldTblData->byWorldRuleType != GAMERULE_FINALMATCH )		
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

RwBool CNtlWorldConceptTB::CanUseSkill(RwUInt32 hSerialID, RwUInt32& uiResultCode)
{
	RwBool bRingOut = IsRingOut(hSerialID);

	if(bRingOut)
	{
		// result code를 적재한다.
		uiResultCode = GAME_MATCH_CAN_NOT_USE_SKILL_IN_OUTOFAREA;

		return FALSE;
	}

	return TRUE;
}

RwBool CNtlWorldConceptTB::IsRingOut( RwUInt32 hSerialID )
{
	if( hSerialID == INVALID_SERIAL_ID )
		return TRUE;

	stTBudokaiMember* pMember = FindMember( hSerialID );

	if( pMember )
	{
		if( pMember->byState == MATCH_MEMBER_STATE_OUTOFAREA )
			return TRUE;

		return FALSE;
	}

	return TRUE;
}

RwBool CNtlWorldConceptTB::IsAttackableEnemy( RwUInt32 hSerialId )
{
	MAP_TBMEMBER_ITER iter_member = m_mapMembers.find( hSerialId );

	if( iter_member != m_mapMembers.end() )
	{
		stTBudokaiMember* pMember = iter_member->second;

		if( pMember->pTeam->wTeamType == GetMyTeamType() )
			return FALSE;

		if( pMember->byState == MATCH_MEMBER_STATE_NORMAL )
			return TRUE;
	}

	return FALSE;
}


CNtlSobActor* CNtlWorldConceptTB::GetNearAttackableEnemy(CNtlSobActor* pActor)
{
	MAP_TBMEMBER_ITER iter_member; 

	RwReal fLen, fMinLen = FLT_MAX;
	RwV3d vPos = pActor->GetPosition();
	RwV3d vSub, vTarPos;
	CNtlSobActor* pTargetActor = NULL;
	CNtlSobActor* pSobActor;

	for( iter_member = m_mapMembers.begin() ; iter_member != m_mapMembers.end() ; ++iter_member )
	{
		stTBudokaiMember* pMember = iter_member->second;
		RwUInt32 hSerial = pMember->hSerial;

		if( pMember->pTeam->wTeamType == GetMyTeamType() )
			continue;

		if( pMember->byState != MATCH_MEMBER_STATE_NORMAL )
			continue;

		pSobActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject( hSerial ) );

        if(!Logic_CanTargetted(pSobActor) || !Logic_CanAttacked(pSobActor))
            continue;

		vTarPos = pSobActor->GetPosition();
		RwV3dSubMacro( &vSub, &vTarPos, &vPos );
		fLen = RwV3dLength( &vSub );

		if( fLen > 50.0f )
			continue;

		if( fLen < fMinLen )
		{
			fMinLen = fLen;
			pTargetActor = pSobActor;
		}
	}

	return pTargetActor;
}

RwBool CNtlWorldConceptTB::IsPartyBattle(VOID)
{
	if( !m_pMatchTblDat )
		return FALSE;

	return m_pMatchTblDat->byBattleMode;
}

stTBudokaiMember* CNtlWorldConceptTB::FindMember( RwUInt32 hSerialID )
{
	MAP_TBMEMBER_ITER iter = m_mapMembers.find( hSerialID );

	if( iter != m_mapMembers.end() )
		return iter->second;

	return NULL;	
}

stTBudokaiTeam* CNtlWorldConceptTB::FindTeam( RwUInt16 wTeamID )
{
	MAP_TBTEAM_ITER iter = m_mapTeams.find( wTeamID );

	if( iter != m_mapTeams.end() )
		return iter->second;

	return NULL;
}

stTBudokaiTeam* CNtlWorldConceptTB::GetMyTeam(VOID)
{
	MAP_TBTEAM_ITER iter = m_mapTeams.find( m_wMyTeamType );

	if( iter != m_mapTeams.end() )
		return iter->second;

	return NULL;
}

stTBudokaiTeam* CNtlWorldConceptTB::GetEnemyTeam(VOID)
{
	MAP_TBTEAM_ITER iter;
	
	for( iter = m_mapTeams.begin() ; iter != m_mapTeams.end() ; ++iter )
	{
		if( iter->first != m_wMyTeamType )
			return iter->second;
	}

	return NULL;
}

stTBudokaiTeam* CNtlWorldConceptTB::GetBlueTeam(VOID)
{
	MAP_TBTEAM_ITER iter;

	for( iter = m_mapTeams.begin() ; iter != m_mapTeams.end() ; ++iter )
	{
		if( IsBlueTeam( iter->first ) )
			return iter->second;		
	}

	return NULL;
}

stTBudokaiTeam* CNtlWorldConceptTB::GetRedTeam(VOID)
{
	MAP_TBTEAM_ITER iter;

	for( iter = m_mapTeams.begin() ; iter != m_mapTeams.end() ; ++iter )
	{
		if( IsRedTeam( iter->first ) )
			return iter->second;		
	}

	return NULL;
}

RwBool CNtlWorldConceptTB::MyTeamIsBlue(VOID)
{
	if( m_eMatchType == MATCHWORLD_TYPE_MAJOR_MATCH ||
		m_eMatchType == MATCHWORLD_TYPE_FINAL_MATCH )
	{
		stTBudokaiTeam* pMyTeam = GetMyTeam();

		if( pMyTeam && IsBlueTeam( pMyTeam->wTeamType ) )
			return TRUE;		
	}

	return FALSE;
}

RwBool CNtlWorldConceptTB::IsBlueTeam( RwUInt16 wTeamType )
{
	if( wTeamType == MATCH_TEAM_TYPE_TEAM1 )
		return TRUE;

	return FALSE;
}

RwBool CNtlWorldConceptTB::IsRedTeam( RwUInt16 wTeamType )
{
	if( wTeamType == MATCH_TEAM_TYPE_TEAM2 )
		return TRUE;

	return FALSE;
}

void CNtlWorldConceptTB::SetMatchTbl( RwUInt32 tblidx )
{
	if( tblidx == INVALID_SERIAL_ID )
		m_pMatchTblDat = NULL;
	else
		m_pMatchTblDat = reinterpret_cast<sRANKBATTLE_TBLDAT*>( API_GetTableContainer()->GetRankBattleTable()->FindData( tblidx ) );	
}

void CNtlWorldConceptTB::ClearMemberData(VOID)
{
	MAP_TBMEMBER_ITER iterMember;
	MAP_TBTEAM_ITER iterTeam;

	for( iterMember = m_mapMembers.begin() ; iterMember != m_mapMembers.end() ; ++iterMember )
	{
		stTBudokaiMember* pMember = iterMember->second;
		NTL_DELETE( pMember );		
	}

	for( iterTeam = m_mapTeams.begin() ; iterTeam != m_mapTeams.end() ; ++iterTeam )
	{
		stTBudokaiTeam* pTeam = iterTeam->second;
		pTeam->vecpMember.clear();
		NTL_DELETE( pTeam );
	}

	m_mapMembers.clear();
	m_mapTeams.clear();

	m_wMyTeamType = INVALID_WORD;
}

void CNtlWorldConceptTB::ResetCamera(VOID)
{
	GetNtlGameCameraManager()->ResetCamera();
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	RwV3d vDir	= pSobAvatar->GetDirection();
	RwReal fRotY = CNtlMath::LineToAngleY( &vDir );

	GetNtlGameCameraManager()->SetYaw( fRotY );
	GetNtlGameCameraManager()->SetPitch( 10.0f );		
	GetNtlGameCameraManager()->SetDist( 20.0f );
}

void CNtlWorldConceptTB::MajorResetCamera( VOID )
{
	CBudokaiTable* pTable = API_GetTableContainer()->GetBudokaiTable();
	if( !pTable )
		return;

	sBUDOKAI_TBLINFO* pTblInfo = pTable->GetBudokaiTblInfo();
	if( !pTblInfo )
		return;

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pSobAvatar )
		return;

	stTBudokaiMember* pMember = FindMember( pSobAvatar->GetSerialID() );
	if( !pMember )
		return;

	// 멤버의 Index를 알아야 한다.
	// 멤버의 Index와 TeamType으로 aMajorLoc의 좌표를 읽어온다.

	// MajocLoc 참조
	// FinalResetCamera 역시 동일 세팅
	RwV3d vDir;
	if( pMember->pTeam->wTeamType == MATCH_TEAM_TYPE_TEAM1 )
	{
		vDir.x = pTblInfo->aMajorLoc[pMember->byIndex].vDir.x;
		vDir.y = pTblInfo->aMajorLoc[pMember->byIndex].vDir.y;
		vDir.z = pTblInfo->aMajorLoc[pMember->byIndex].vDir.z;
	}
	else
	{
		vDir.x = pTblInfo->aMajorLoc[pMember->byIndex+NTL_MAX_MEMBER_IN_PARTY].vDir.x;
		vDir.y = pTblInfo->aMajorLoc[pMember->byIndex+NTL_MAX_MEMBER_IN_PARTY].vDir.y;
		vDir.z = pTblInfo->aMajorLoc[pMember->byIndex+NTL_MAX_MEMBER_IN_PARTY].vDir.z;
	}

	RwReal fRotY = CNtlMath::LineToAngleY( &vDir );

	GetNtlGameCameraManager()->SetYaw( fRotY );
	GetNtlGameCameraManager()->SetPitch( 10.0f );		
	GetNtlGameCameraManager()->SetDist( 20.0f );
}

void CNtlWorldConceptTB::FinalResetCamera( VOID )
{
	CBudokaiTable* pTable = API_GetTableContainer()->GetBudokaiTable();
	if( !pTable )
		return;

	sBUDOKAI_TBLINFO* pTblInfo = pTable->GetBudokaiTblInfo();
	if( !pTblInfo )
		return;

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pSobAvatar )
		return;

	stTBudokaiMember* pMember = FindMember( pSobAvatar->GetSerialID() );
	if( !pMember )
		return;

	// 멤버의 Index를 알아야 한다.
	// 멤버의 Index와 TeamType으로 aMajorLoc의 좌표를 읽어온다.

	// MajocLoc 참조
	// FinalResetCamera 역시 동일 세팅
	RwV3d vDir;
	if( pMember->pTeam->wTeamType == MATCH_TEAM_TYPE_TEAM1 )
	{
		vDir.x = pTblInfo->aFinalLoc[pMember->byIndex].vDir.x;
		vDir.y = pTblInfo->aFinalLoc[pMember->byIndex].vDir.y;
		vDir.z = pTblInfo->aFinalLoc[pMember->byIndex].vDir.z;
	}
	else
	{
		vDir.x = pTblInfo->aFinalLoc[pMember->byIndex+NTL_MAX_MEMBER_IN_PARTY].vDir.x;
		vDir.y = pTblInfo->aFinalLoc[pMember->byIndex+NTL_MAX_MEMBER_IN_PARTY].vDir.y;
		vDir.z = pTblInfo->aFinalLoc[pMember->byIndex+NTL_MAX_MEMBER_IN_PARTY].vDir.z;
	}

	RwReal fRotY = CNtlMath::LineToAngleY( &vDir );

	GetNtlGameCameraManager()->SetYaw( fRotY );
	GetNtlGameCameraManager()->SetPitch( 10.0f );		
	GetNtlGameCameraManager()->SetDist( 20.0f );
}

void CNtlWorldConceptTB::EventHandler_MinorMatchStateUpdate(void* pData) 
{
	SNtlEventMinorMatchStateUpdate* pStateUpdate = reinterpret_cast<SNtlEventMinorMatchStateUpdate*>( pData );
	ChangeTBState( pStateUpdate->byMatchState, pStateUpdate->bEnterState );

	if( pStateUpdate->bEnterState )
	{
		if( pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_DIRECTION )
		{
			CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_DIRECTION_BGM_PLAY);
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_READY )
		{
			ResetCamera();

			CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_DIRECTION_BGM_STOP);
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_FINISH )
		{
			if( IsPartyBattle() )
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_PARTY_BGM_STOP);
			else
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_PRIVATE_BGM_STOP);

			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup = CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE;
			tSoundParam.pcFileName = GSD_TB_ROUND_END;
			GetSoundManager()->Play(&tSoundParam);
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_RUN )
		{
			if( IsPartyBattle() )
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_PARTY_BGM_PLAY);
			else
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_PRIVATE_BGM_PLAY);
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_FINISH )
		{
			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup = CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE;
			tSoundParam.pcFileName = GSD_TB_MATCH_RESULT;
			GetSoundManager()->Play(&tSoundParam);
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_WAIT_MINORMATCH ||
				 pStateUpdate->byMatchState == BUDOKAI_MINORMATCH_STATE_WAIT )
		{
			
		}
	}

	//// peessitemp : for debug
	//WCHAR buf[256];
	//swprintf_s( buf, 256, L"((MatchStateUpdate)) Stage = %d", pStateUpdate->byStage );
	//CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}

void CNtlWorldConceptTB::EventHandler_MinorMatchTeamInfo(void* pData)
{
	ClearMemberData();

	SNtlEventMinorMatchTeamInfo* pTeamData = reinterpret_cast<SNtlEventMinorMatchTeamInfo*>( pData );
	sMATCH_MINORMATCH_TEAM_INFO_VAR* pTeamInfo = reinterpret_cast<sMATCH_MINORMATCH_TEAM_INFO_VAR*>( pTeamData->pData->Read( pTeamData->wTeamInfo_var ) );

	m_byMatchIndex = pTeamData->byMatchIndex;

	NTL_ASSERT( m_pMatchTblDat, "CNtlWorldConceptTB::EventHandler_MinorMatchTeamInfo, Invalid Tbldat" );			

	for( RwInt32 i = 0 ; i < pTeamData->byTeamCount ; ++i )
	{
		stTBudokaiTeam*	pTeam = NTL_NEW stTBudokaiTeam;
		sMATCH_MEMBER_STATE_INFO* pMemberList = reinterpret_cast<sMATCH_MEMBER_STATE_INFO*>( pTeamData->pData->Read( pTeamInfo[i].wMembers_var ) );		
				
		WCHAR wBuf[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE+1];
		RwUInt16 wNameLenth = pTeamData->pData->GetSize( pTeamInfo[i].wTeamName_var );
		memset( wBuf, 0, sizeof( WCHAR ) *( NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1 ) );
		memcpy( wBuf, reinterpret_cast<WCHAR*>( pTeamData->pData->Read( pTeamInfo[i].wTeamName_var ) ), wNameLenth );

		pTeam->wstrTeamName = wBuf;
		pTeam->wTeamType = pTeamInfo[i].wTeamType;		

		for( RwInt32 j = 0 ; j < pTeamInfo[i].byMemberCount ; ++j )
		{
			stTBudokaiMember* pMember = NTL_NEW stTBudokaiMember;

			RwUInt32 hSerial = pMemberList[j].handle;
			pMember->hSerial = hSerial;
			pMember->byState = pMemberList[j].byState;
			pMember->byLevel = pMemberList[j].byLevel;
			pMember->byClass = pMemberList[j].byClass;
			pMember->wWinCount = pMemberList[j].wWinCount;
			pMember->wLoseCount = pMemberList[j].wLoseCount;
			pMember->wDrawCount = pMemberList[j].wDrawCount;
			pMember->byIndex = j;

			if( pMemberList[j].wGuildName != INVALID_WORD )
			{
				memset( wBuf, 0, sizeof( WCHAR ) *( NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1 ) );
				memcpy( wBuf, reinterpret_cast<WCHAR*>( pTeamData->pData->Read( pMemberList[j].wGuildName ) ), pTeamData->pData->GetSize( pMemberList[j].wGuildName ) );
				pMember->wstrGuildName = wBuf;
			}

			memset( wBuf, 0, sizeof( WCHAR ) *( NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1 ) );
			memcpy( wBuf, reinterpret_cast<WCHAR*>( pTeamData->pData->Read( pMemberList[j].wCharName ) ), pTeamData->pData->GetSize( pMemberList[j].wCharName ) );
			pMember->wstrName = wBuf;

			pMember->pTeam = pTeam;

			m_mapMembers[hSerial] = pMember;
			pTeam->vecpMember.push_back( pMember );
			
			if( hSerial == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
				m_wMyTeamType = pTeamInfo[i].wTeamType;
		}

		m_mapTeams[pTeamInfo[i].wTeamType] = pTeam;		
	}

	//// peessitemp : for debug
	//CNtlSLEventGenerator::SysMsg( L"((MinorMatchTeamInfo))", SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}

void CNtlWorldConceptTB::EventHandler_MinorMatchPlayerState(void* pData)
{
	SNtlEventMinorMatchPlayerState* pPlayerState = reinterpret_cast<SNtlEventMinorMatchPlayerState*>( pData );

	stTBudokaiMember* pMember = FindMember( pPlayerState->hPlayer );

	if( pMember )
		pMember->byState = pPlayerState->byPcState;

	//// peessitemp : for debug
	//WCHAR buf[256];
	//WCHAR* string[6] = { L"NONE", L"NORMAL", L"FAINT", L"OUTOFAREA", L"GIVEUP", L"NOAPPLY" };
	//swprintf_s( buf, 256, L"((MinorMatchPlayerState)) hPC = %u, State = %s", pPlayerState->hPlayer, string[pPlayerState->byPcState] );
	//CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}

void CNtlWorldConceptTB::EventHandler_MinorMatchUpdateScore(void* pData)
{
	SNtlEventMinorMatchUpdateScore* pUpdateScore = reinterpret_cast<SNtlEventMinorMatchUpdateScore*>( pData );

	stTBudokaiTeam* pTeam = FindTeam( pUpdateScore->wTeamType );

	if( pTeam )
		pTeam->byTeamScore = pUpdateScore->byScore;

	//// peessitemp : for debug
	//WCHAR buf[256];
	//swprintf_s( buf, 256, L"((MinorMatchUpdateScore)) wTeamType = %u, byScore = %u, hSlayer = %u, hFainter = %u", pUpdateScore->wTeamType, pUpdateScore->byScore, pUpdateScore->hSlayer, pUpdateScore->hFainter );
	//CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}

void CNtlWorldConceptTB::EventHandler_MinorMatchTeamScore( void* pData )
{
	SNtlEventMinorMatchTeamScore* pTeamScore = reinterpret_cast<SNtlEventMinorMatchTeamScore*>( pData );

	stTBudokaiTeam* pTeam = NULL;
	for( RwInt32 i=0; i< pTeamScore->byCount; ++i )
	{
		pTeam = FindTeam( pTeamScore->aTeamScore[i].teamType );

		if( pTeam )
			pTeam->byTeamScore = pTeamScore->aTeamScore[i].byScore;
	}
}

void CNtlWorldConceptTB::EventHandler_MinorMatchStageFinish(void* pData)
{
	//SNtlEventMinorMatchStageFinish* pStageFinish = reinterpret_cast<SNtlEventMinorMatchStageFinish*>( pData );

	//// peessitemp : for debug
	//WCHAR buf[256];
	//WCHAR* string[4] = { L"SUCCESS", L"DRAW", L"WIN", L"TIMEOVER" };
	//swprintf_s( buf, 256, L"((MinorMatchStageFinish)) Result = %s, TeamType = %d", string[pStageFinish->byStageResult], pStageFinish->wStageWinner );
	//CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );

	////
}

void CNtlWorldConceptTB::EventHandler_MinorMatchMatchFinish(void* pData)
{
	//// peessitemp : for debug
	//CNtlSLEventGenerator::SysMsg( L"((MinorMatchFinish))", SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}

void CNtlWorldConceptTB::EventHandler_MajorMatchStateUpdate(void* pData)
{
	SNtlEventMajorMatchStateUpdate* pStateUpdate = reinterpret_cast<SNtlEventMajorMatchStateUpdate*>( pData );
	ChangeTBState( pStateUpdate->byMatchState, pStateUpdate->bEnterState );

	if( pStateUpdate->bEnterState )
	{
		if( pStateUpdate->byMatchState == BUDOKAI_MAJORMATCH_STATE_DIRECTION )
		{
			CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_DIRECTION_BGM_PLAY);
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_READY )
		{
			MajorResetCamera();

			CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_DIRECTION_BGM_STOP);			
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_RUN )
		{
			if( IsPartyBattle() )
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_PARTY_BGM_PLAY);
			else
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_PRIVATE_BGM_PLAY);
		}		
		else if( pStateUpdate->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH )
		{
			if( IsPartyBattle() )
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_PARTY_BGM_STOP);
			else
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_PRIVATE_BGM_STOP);

			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup = CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE;
			tSoundParam.pcFileName = GSD_TB_ROUND_END;
			GetSoundManager()->Play(&tSoundParam);
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MAJORMATCH_STATE_MATCH_FINISH )
		{
			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup = CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE;
			tSoundParam.pcFileName = GSD_TB_MATCH_RESULT;
			GetSoundManager()->Play(&tSoundParam);
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_MAJORMATCH_STATE_WAIT )
		{
			MajorResetCamera();
		}
	}	
	
	//// peessitemp : for debug
	//WCHAR buf[256];
	//swprintf_s( buf, 256, L"((MajorMatchStateUpdate)) Stage = %d", pStateUpdate->byStage );
	//CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////	
}

void CNtlWorldConceptTB::EventHandler_MajorMatchTeamInfo(void* pData)
{
	ClearMemberData();

	SNtlEventMajorMatchTeamInfo* pTeamData = reinterpret_cast<SNtlEventMajorMatchTeamInfo*>( pData );
	sMATCH_MAJORMATCH_TEAM_INFO_VAR* pTeamInfo = reinterpret_cast<sMATCH_MAJORMATCH_TEAM_INFO_VAR*>( pTeamData->pData->Read( pTeamData->wTeamInfo_var ) );

	m_byMatchDepth = pTeamData->byMatchDepth;
	m_byMatchIndex = pTeamData->byMatchIndex;	

	NTL_ASSERT( m_pMatchTblDat, "CNtlWorldConceptTB::EventHandler_MajorMatchTeamInfo, Invalid Tbldat" );

	for( RwInt32 i = 0 ; i < pTeamData->byTeamCount ; ++i )
	{
		stTBudokaiTeam*	pTeam = NTL_NEW stTBudokaiTeam;
		sMATCH_MEMBER_STATE_INFO* pMemberList = reinterpret_cast<sMATCH_MEMBER_STATE_INFO*>( pTeamData->pData->Read( pTeamInfo[i].wMembers_var ) );		

		WCHAR wBuf[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE+1];
		RwUInt16 wNameLenth = pTeamData->pData->GetSize( pTeamInfo[i].wTeamName_var );
		memset( wBuf, 0, sizeof( WCHAR ) *( NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1 ) );
		memcpy( wBuf, reinterpret_cast<WCHAR*>( pTeamData->pData->Read( pTeamInfo[i].wTeamName_var ) ), wNameLenth );

		pTeam->wstrTeamName = wBuf;
		pTeam->wTeamType = pTeamInfo[i].wTeamType;		

		for( RwInt32 j = 0 ; j < pTeamInfo[i].byMemberCount ; ++j )
		{
			stTBudokaiMember* pMember = NTL_NEW stTBudokaiMember;

			RwUInt32 hSerial = pMemberList[j].handle;
			pMember->hSerial = hSerial;
			pMember->byState = pMemberList[j].byState;
			pMember->byLevel = pMemberList[j].byLevel;
			pMember->byClass = pMemberList[j].byClass;
			pMember->wWinCount = pMemberList[j].wWinCount;
			pMember->wLoseCount = pMemberList[j].wLoseCount;
			pMember->wDrawCount = pMemberList[j].wDrawCount;
			pMember->byIndex = j;
			
			if( pMemberList[j].wGuildName != INVALID_WORD )
			{
				memset( wBuf, 0, sizeof( WCHAR ) *( NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1 ) );
				memcpy( wBuf, reinterpret_cast<WCHAR*>( pTeamData->pData->Read( pMemberList[j].wGuildName ) ), pTeamData->pData->GetSize( pMemberList[j].wGuildName ) );
				pMember->wstrGuildName = wBuf;
			}					

			memset( wBuf, 0, sizeof( WCHAR ) *( NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1 ) );
			memcpy( wBuf, reinterpret_cast<WCHAR*>( pTeamData->pData->Read( pMemberList[j].wCharName ) ), pTeamData->pData->GetSize( pMemberList[j].wCharName ) );
			pMember->wstrName = wBuf;
			
			pMember->pTeam = pTeam;

			m_mapMembers[hSerial] = pMember;
			pTeam->vecpMember.push_back( pMember );

			if( hSerial == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
				m_wMyTeamType = pTeamInfo[i].wTeamType;
		}

		m_mapTeams[pTeamInfo[i].wTeamType] = pTeam;
	}	

	//// peessitemp : for debug
	//CNtlSLEventGenerator::SysMsg( L"((MajorMatchTeamInfo))", SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}

void CNtlWorldConceptTB::EventHandler_MajorMatchPlayerState(void* pData)
{
	SNtlEventMajorMatchPlayerState* pPlayerState = reinterpret_cast<SNtlEventMajorMatchPlayerState*>( pData );

	stTBudokaiMember* pMember = FindMember( pPlayerState->hPlayer );

	if( pMember )
		pMember->byState = pPlayerState->byPcState;

	//// peessitemp : for debug
	//WCHAR buf[256];
	//WCHAR* string[6] = { L"NONE", L"NORMAL", L"FAINT", L"OUTOFAREA", L"GIVEUP", L"NOAPPLY" };
	//swprintf_s( buf, 256, L"((MajorMatchPlayerState)) hPC = %u, State = %s", pPlayerState->hPlayer, string[pPlayerState->byPcState] );
	//CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}

void CNtlWorldConceptTB::EventHandler_MajorMatchUpdateScore(void* pData)
{
	//SNtlEventMajorMatchUpdateScore* pUpdateScore = reinterpret_cast<SNtlEventMajorMatchUpdateScore*>( pData );

	//// peessitemp : for debug
	//WCHAR buf[256];
	//swprintf_s( buf, 256, L"((MajorMatchUpdateScore) Team1 = %u, Team2 = %u", pUpdateScore->pStageScore->byTeam1, pUpdateScore->pStageScore->byTeam2 );
	//CNtlSLEventGenerator::SysMsg( L"((MajorMatchUpdateScore))", SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}

void CNtlWorldConceptTB::EventHandler_MajorMatchStageFinish(void* pData)
{
	//SNtlEventMajorMatchStageFinish* pStageFinish = reinterpret_cast<SNtlEventMajorMatchStageFinish*>( pData );
	
	//// peessitemp : for debug
	//WCHAR buf[256];
	//swprintf_s( buf, 256, L"((MajorMatchStageFinish) Result = %d, SWinner = %u, Team1 = %u, Team2 = %u", pStageFinish->byStageResult, pStageFinish->wStageWinner, pStageFinish->pStageScore->byTeam1, pStageFinish->pStageScore->byTeam2 );
	//CNtlSLEventGenerator::SysMsg( L"((MajorMatchTeamInfo))", SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}

void CNtlWorldConceptTB::EventHandler_MajorMatchMatchFinish(void* pData)
{
	//SNtlEventMajorMatchMatchFinish* pMatchFinish = reinterpret_cast<SNtlEventMajorMatchMatchFinish*>( pData );

	//// peessitemp : for debug
	//WCHAR buf[256];
	//swprintf_s( buf, 256, L"((MajorMatchMatchFinish) Result = %d, MWinner = %u, Team1 = %u, Team2 = %u", pMatchFinish->byMatchResult, pMatchFinish->wMatchWinner, pMatchFinish->pStageScore->byTeam1, pMatchFinish->pStageScore->byTeam2 );
	//CNtlSLEventGenerator::SysMsg( L"((MajorMatchTeamInfo))", SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}

void CNtlWorldConceptTB::EventHandler_FinalMatchStateUpdate(void* pData)
{
	SNtlEventFinalMatchStateUpdate* pStateUpdate = reinterpret_cast<SNtlEventFinalMatchStateUpdate*>( pData );
	ChangeTBState( pStateUpdate->byMatchState, pStateUpdate->bEnterState );

	if( GetEnterState() )
	{
		// peessitemp:
		if( pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_DIRECTION )
		{
			m_byMatchDepth = BUDOKAI_MATCH_DEPTH_4;
			CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_DIRECTION_BGM_PLAY);
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_READY )
		{
			FinalResetCamera();

			CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_DIRECTION_BGM_STOP);
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_FINALDIRECTION )
		{
			m_byMatchDepth = BUDOKAI_MATCH_DEPTH_2;
			CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_DIRECTION_BGM_PLAY);
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_RUN )
		{
			if( IsPartyBattle() )
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_PARTY_BGM_PLAY);
			else
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_PRIVATE_BGM_PLAY);
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_FINISH )
		{
			if( IsPartyBattle() )
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_PARTY_BGM_STOP);
			else
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TB_PRIVATE_BGM_STOP);

			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup = CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE;
			tSoundParam.pcFileName = GSD_TB_ROUND_END;
			GetSoundManager()->Play(&tSoundParam);
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_MATCH_FINISH )
		{
			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup = CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE;
			tSoundParam.pcFileName = GSD_TB_MATCH_RESULT;
			GetSoundManager()->Play(&tSoundParam);
		}
		else if( pStateUpdate->byMatchState == BUDOKAI_FINALMATCH_STATE_WAIT )
		{
			FinalResetCamera();
		}
	}	

	//// peessitemp : for debug
	//WCHAR buf[256];
	//swprintf_s( buf, 256, L"((FinalMatchStateUpdate)) Stage = %d", pStateUpdate->byStage );
	//CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////	
}

void CNtlWorldConceptTB::EventHandler_FinalMatchTeamInfo(void* pData)
{
	ClearMemberData();

	SNtlEventFinalMatchTeamInfo* pTeamData = reinterpret_cast<SNtlEventFinalMatchTeamInfo*>( pData );
	sMATCH_FINALMATCH_TEAM_INFO_VAR* pTeamInfo = reinterpret_cast<sMATCH_FINALMATCH_TEAM_INFO_VAR*>( pTeamData->pData->Read( pTeamData->wTeamInfo_var ) );

	m_byMatchDepth = pTeamData->byMatchDepth;
	m_byMatchIndex = pTeamData->byMatchIndex;	

	NTL_ASSERT( m_pMatchTblDat, "CNtlWorldConceptTB::EventHandler_FinalMatchTeamInfo, Invalid Tbldat" );

	for( RwInt32 i = 0 ; i < pTeamData->byTeamCount ; ++i )
	{
		stTBudokaiTeam*	pTeam = NTL_NEW stTBudokaiTeam;
		sMATCH_MEMBER_STATE_INFO* pMemberList = reinterpret_cast<sMATCH_MEMBER_STATE_INFO*>( pTeamData->pData->Read( pTeamInfo[i].wMembers_var ) );		

		WCHAR wBuf[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE+1];
		RwUInt16 wNameLenth = pTeamData->pData->GetSize( pTeamInfo[i].wTeamName_var );
		memset( wBuf, 0, sizeof( WCHAR ) *( NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1 ) );
		memcpy( wBuf, reinterpret_cast<WCHAR*>( pTeamData->pData->Read( pTeamInfo[i].wTeamName_var ) ), wNameLenth );

		pTeam->wstrTeamName = wBuf;
		pTeam->wTeamType = pTeamInfo[i].wTeamType;		

		for( RwInt32 j = 0 ; j < pTeamInfo[i].byMemberCount ; ++j )
		{
			stTBudokaiMember* pMember = NTL_NEW stTBudokaiMember;

			RwUInt32 hSerial = pMemberList[j].handle;
			pMember->hSerial = hSerial;
			pMember->byState = pMemberList[j].byState;
			pMember->byLevel = pMemberList[j].byLevel;
			pMember->byClass = pMemberList[j].byClass;
			pMember->wWinCount = pMemberList[j].wWinCount;
			pMember->wLoseCount = pMemberList[j].wLoseCount;
			pMember->wDrawCount = pMemberList[j].wDrawCount;
			pMember->byIndex = j;

			if( pMemberList[j].wGuildName != INVALID_WORD )
			{
				memset( wBuf, 0, sizeof( WCHAR ) *( NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1 ) );
				memcpy( wBuf, reinterpret_cast<WCHAR*>( pTeamData->pData->Read( pMemberList[j].wGuildName ) ), pTeamData->pData->GetSize( pMemberList[j].wGuildName ) );
				pMember->wstrGuildName = wBuf;
			}

			memset( wBuf, 0, sizeof( WCHAR ) *( NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1 ) );
			memcpy( wBuf, reinterpret_cast<WCHAR*>( pTeamData->pData->Read( pMemberList[j].wCharName ) ), pTeamData->pData->GetSize( pMemberList[j].wCharName ) );
			pMember->wstrName = wBuf;

			pMember->pTeam = pTeam;

			m_mapMembers[hSerial] = pMember;
			pTeam->vecpMember.push_back( pMember );

			if( hSerial == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
				m_wMyTeamType = pTeamInfo[i].wTeamType;
		}

		m_mapTeams[pTeamInfo[i].wTeamType] = pTeam;
	}	

	//// peessitemp : for debug
	//CNtlSLEventGenerator::SysMsg( L"((FinalMatchTeamInfo))", SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}

void CNtlWorldConceptTB::EventHandler_FinalMatchPlayerState(void* pData)
{
	SNtlEventFinalMatchPlayerState* pPlayerState = reinterpret_cast<SNtlEventFinalMatchPlayerState*>( pData );

	stTBudokaiMember* pMember = FindMember( pPlayerState->hPlayer );

	if( pMember )
		pMember->byState = pPlayerState->byPcState;

	//// peessitemp : for debug
	//WCHAR buf[256];
	//WCHAR* string[6] = { L"NONE", L"NORMAL", L"FAINT", L"OUTOFAREA", L"GIVEUP", L"NOAPPLY" };
	//swprintf_s( buf, 256, L"((FinalMatchPlayerState)) hPC = %u, State = %s", pPlayerState->hPlayer, string[pPlayerState->byPcState] );
	//CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}

void CNtlWorldConceptTB::EventHandler_FinalMatchUpdateScore(void* pData)
{
	//SNtlEventFinalMatchUpdateScore* pUpdateScore = reinterpret_cast<SNtlEventFinalMatchUpdateScore*>( pData );

	//// peessitemp : for debug
	//WCHAR buf[256];
	//swprintf_s( buf, 256, L"((FinalMatchUpdateScore) Team1 = %u, Team2 = %u", pUpdateScore->pStageScore->byTeam1, pUpdateScore->pStageScore->byTeam2 );
	//CNtlSLEventGenerator::SysMsg( L"((FinalMatchUpdateScore))", SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}

void CNtlWorldConceptTB::EventHandler_FinalMatchStageFinish(void* pData)
{
	//SNtlEventFinalMatchStageFinish* pStageFinish = reinterpret_cast<SNtlEventFinalMatchStageFinish*>( pData );

	//// peessitemp : for debug
	//WCHAR buf[256];
	//swprintf_s( buf, 256, L"((FinalMatchStageFinish) Result = %d, SWinner = %u, Team1 = %u, Team2 = %u", pStageFinish->byStageResult, pStageFinish->wStageWinner, pStageFinish->pStageScore->byTeam1, pStageFinish->pStageScore->byTeam2 );
	//CNtlSLEventGenerator::SysMsg( L"((FinalMatchTeamInfo))", SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}

void CNtlWorldConceptTB::EventHandler_FinalMatchMatchFinish(void* pData)
{
	//SNtlEventFinalMatchMatchFinish* pMatchFinish = reinterpret_cast<SNtlEventFinalMatchMatchFinish*>( pData );

	//// peessitemp : for debug
	//WCHAR buf[256];
	//swprintf_s( buf, 256, L"((FinalMatchMatchFinish) Result = %d, MWinner = %u, Team1 = %u, Team2 = %u", pMatchFinish->byMatchResult, pMatchFinish->wMatchWinner, pMatchFinish->pStageScore->byTeam1, pMatchFinish->pStageScore->byTeam2 );
	//CNtlSLEventGenerator::SysMsg( L"((FinalMatchTeamInfo))", SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
	////
}
void	CNtlWorldConceptTB::UpdateState_Null( RwReal fElpased ){}

void	CNtlWorldConceptTB::UpdateState_MinorMatch_None( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MinorMatch_WaitMinorMatch( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MinorMatch_Wait( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MinorMatch_Direction( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MinorMatch_Match_Ready( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MinorMatch_Stage_Ready( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MinorMatch_Stage_Run( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MinorMatch_Stage_Finish( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MinorMatch_Match_Finish( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MinorMatch_End( RwReal fElapsed )
{
	// lleo52
	CNtlSLEventGenerator::TSBudokai();
}

void	CNtlWorldConceptTB::UpdateState_MajorMatch_None( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MajorMatch_Wait( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MajorMatch_Direction( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MajorMatch_Match_Ready( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MajorMatch_Stage_Ready( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MajorMatch_Stage_Run( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MajorMatch_Stage_Finish( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MajorMatch_Match_Finish( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_MajorMatch_End( RwReal fElapsed )
{
	// lleo52
	CNtlSLEventGenerator::TSBudokai();
}

void	CNtlWorldConceptTB::UpdateState_FinalMatch_None( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_FinalMatch_Wait( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_FinalMatch_Direction( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_FinalMatch_Match_Ready( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_FinalMatch_Stage_Ready( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_FinalMatch_Stage_Run( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_FinalMatch_Stage_Finish( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_FinalMatch_Match_Finish( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_FinalMatch_Final_Direction( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_FinalMatch_Awarding( RwReal fElapsed ){}
void	CNtlWorldConceptTB::UpdateState_FinalMatch_End( RwReal fElapsed )
{
	// lleo52
	CNtlSLEventGenerator::TSBudokai();
}