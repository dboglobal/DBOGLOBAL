/*****************************************************************************
* File			: DboPackethandler_GUBudokai.cpp
* Author		: Cho Haesung
* Copyright		: (주)NTL
* Date			: 2008. 5. 20
* Abstract		: 천하제일 무도회 관련 패킷 핸들
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// simulation
#include "NtlNetSender.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"

// Shared
#include "NtlBudokai.h"

void PacketHandler_GUMinorMatchStateUpdateNfy(void* pPacket)
{
	sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY* pNotify = reinterpret_cast<sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY*>( pPacket );

	CNtlSLEventGenerator::MinorMatchStateUpdate( pNotify->byMatchState, pNotify->byStage, pNotify->dwRemainTime, (RwBool)pNotify->bIsEnter );
}

void PacketHandler_GUMinorMatchTeamInfoNfy(void* pPacket)
{
	sGU_MATCH_MINORMATCH_TEAM_INFO_NFY* pNotify = reinterpret_cast<sGU_MATCH_MINORMATCH_TEAM_INFO_NFY*>( pPacket );
	
	CNtlSLEventGenerator::MinorMatchTeamInfo( pNotify->byMatchIndex, pNotify->byTeamCount, pNotify->wTeamInfo_var, &pNotify->sData );
}

void PacketHandler_GUMinorMatchPlayerStateNfy(void* pPacket)
{
	sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY* pNofity = reinterpret_cast<sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY*>( pPacket );

	CNtlSLEventGenerator::MinorMatchPlayerState( pNofity->hPc, pNofity->byPcState );
}

void PacketHandler_GUMinorMatchUpdateScoreNfy(void* pPacket)
{
	sGU_MATCH_MINORMATCH_UPDATE_SCORE_NFY* pNofity = reinterpret_cast<sGU_MATCH_MINORMATCH_UPDATE_SCORE_NFY*>( pPacket );

	CNtlSLEventGenerator::MinorMatchUpdateScore( pNofity->wTeamType, pNofity->byScore, pNofity->hSlayer, pNofity->hFainter );
}

void PacketHandler_GUMinorMatchTeamScoreNfy(void* pPacket)
{
	sGU_MATCH_MINORMATCH_TEAM_SCORE_NFY* pNotify = reinterpret_cast<sGU_MATCH_MINORMATCH_TEAM_SCORE_NFY*>( pPacket );

	CNtlSLEventGenerator::MinorMatchTeamScore( pNotify->byCount, pNotify->aTeamScore );
}

void PacketHandler_GUMinorMatchSelectionNfy(void* pPacket)
{
	sGU_MATCH_MINORMATCH_SELECTION_NFY* pNotify = reinterpret_cast<sGU_MATCH_MINORMATCH_SELECTION_NFY*>( pPacket );

	CNtlSLEventGenerator::MinorMatchSelection( pNotify->teamWinner, pNotify->byTeamCount, pNotify->aSelection );
}

void PacketHandler_GUMinorMatchStageFinishNfy(void* pPacket)
{
	sGU_MATCH_MINORMATCH_STAGE_FINISH_NFY* pNotify = reinterpret_cast<sGU_MATCH_MINORMATCH_STAGE_FINISH_NFY*>( pPacket );

	CNtlSLEventGenerator::MinorMatchStageFinish( pNotify->byStageResult, pNotify->wStageWinner );
}

void PacketHandler_GUMinorMatchMatchFinishNfy(void* pPacket)
{
	sGU_MATCH_MINORMATCH_MATCH_FINISH_NFY* pNotify = reinterpret_cast<sGU_MATCH_MINORMATCH_MATCH_FINISH_NFY*>( pPacket );

	CNtlSLEventGenerator::MinorMatchMatchFinish( pNotify->byMatchResult, pNotify->wMatchWinner, pNotify->byKillCount );
}

void PacketHandler_GUMajorMatchStateUpdateNfy(void* pPacket)
{
	sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY* pNotify = reinterpret_cast<sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY*>( pPacket );

	CNtlSLEventGenerator::MajorMatchStateUpdate( pNotify->byMatchState, pNotify->byStage, pNotify->dwRemainTime, (RwBool)pNotify->bIsEnter );
}

void PacketHandler_GUMajorMatchTeamInfoNfy(void* pPacket)
{
	sGU_MATCH_MAJORMATCH_TEAM_INFO_NFY* pNofity = reinterpret_cast<sGU_MATCH_MAJORMATCH_TEAM_INFO_NFY*>( pPacket );

	CNtlSLEventGenerator::MajorMatchTeamInfo( pNofity->byMatchDepth, pNofity->byMatchIndex, pNofity->byTeamCount, pNofity->wTeamInfo_var, &pNofity->sData );
}

void PacketHandler_GUMajorMatchPlayerStateNfy(void* pPacket)
{
	sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY* pNotify = reinterpret_cast<sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY*>( pPacket );

	CNtlSLEventGenerator::MajorMatchPlayerState( pNotify->hPc, pNotify->byPcState );
}

void PacketHandler_GUMajorMatchUpdateScoreNfy(void* pPacket)
{
	sGU_MATCH_MAJORMATCH_UPDATE_SCORE_NFY* pNotify = reinterpret_cast<sGU_MATCH_MAJORMATCH_UPDATE_SCORE_NFY*>( pPacket );

	CNtlSLEventGenerator::MajorMatchUpdateScore( &pNotify->sStageScore );
}

void PacketHandler_GUMajorMatchStageFinishNfy(void* pPacket)
{
	sGU_MATCH_MAJORMATCH_STAGE_FINISH_NFY* pNotify = reinterpret_cast<sGU_MATCH_MAJORMATCH_STAGE_FINISH_NFY*>( pPacket );

	CNtlSLEventGenerator::MajorMatchStageFinish( &pNotify->sMatchScore, pNotify->wStageWinner, pNotify->byStageResult, pNotify->bIsObserver );
}

void PacketHandler_GUMajorMatchMatchFinishNfy(void* pPacket)
{
	sGU_MATCH_MAJORMATCH_MATCH_FINISH_NFY* pNotify = reinterpret_cast<sGU_MATCH_MAJORMATCH_MATCH_FINISH_NFY*>( pPacket );

	CNtlSLEventGenerator::MajorMatchMatchFinish( &pNotify->sMatchScore, pNotify->wMatchWinner, pNotify->byMatchResult );		
}

void PacketHandler_GUFinalMatchStateUpdateNfy(void* pPacket)
{
	sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY* pNotify = reinterpret_cast<sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY*>( pPacket );

	CNtlSLEventGenerator::FinalMatchStateUpdate( pNotify->byMatchState, pNotify->byStage, pNotify->dwRemainTime, (RwBool)pNotify->bIsEnter );
}

void PacketHandler_GUFinalMatchTeamInfoNfy(void* pPacket)
{
	sGU_MATCH_FINALMATCH_TEAM_INFO_NFY* pNotify = reinterpret_cast<sGU_MATCH_FINALMATCH_TEAM_INFO_NFY*>( pPacket );

	CNtlSLEventGenerator::FinalMatchTeamInfo( pNotify->byMatchDepth, pNotify->byMatchIndex, pNotify->byTeamCount, pNotify->wTeamInfo_var, &pNotify->sData );
}

void PacketHandler_GUFinalMatchPlayerStateNfy(void* pPacket)
{	
	sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY* pNotify = reinterpret_cast<sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY*>( pPacket );

	CNtlSLEventGenerator::FinalMatchPlayerState( pNotify->hPc, pNotify->byPcState );
}

void PacketHandler_GUFinalMatchUpdateScoreNfy(void* pPacket)
{
	sGU_MATCH_FINALMATCH_UPDATE_SCORE_NFY* pNotify = reinterpret_cast<sGU_MATCH_FINALMATCH_UPDATE_SCORE_NFY*>( pPacket );

	CNtlSLEventGenerator::FinalMatchUpdateScore( &pNotify->sStageScore );
}

void PacketHandler_GUFinalMatchStageFinishNfy(void* pPacket)
{
	sGU_MATCH_FINALMATCH_STAGE_FINISH_NFY* pNotify = reinterpret_cast<sGU_MATCH_FINALMATCH_STAGE_FINISH_NFY*>( pPacket );

	CNtlSLEventGenerator::FinalMatchStageFinish( &pNotify->sMatchScore, pNotify->wStageWinner, pNotify->byStageResult, pNotify->bIsObserver );
}

void PacketHandler_GUFinalMatchMatchFinishNfy(void* pPacket)
{
	sGU_MATCH_FINALMATCH_MATCH_FINISH_NFY* pNotify = reinterpret_cast<sGU_MATCH_FINALMATCH_MATCH_FINISH_NFY*>( pPacket );

	CNtlSLEventGenerator::FinalMatchMatchFinish( &pNotify->sMatchScore, pNotify->wMatchWinner, pNotify->byMatchResult );		
}

void PacketHandler_GUAwardNfy(void* pPacket)
{
	sGU_MATCH_AWARDING_NFY* pNotify = reinterpret_cast<sGU_MATCH_AWARDING_NFY*>( pPacket );

	CNtlSLEventGenerator::AwardInfo( pNotify->awTeamName, &pNotify->sData );
}

/**
* \brief 천하제일 무도회의 현재 상태를 전송
*/
void PacketHandler_GUBudokaiStateInfo(void* pPacket)
{
	sGU_BUDOKAI_STATE_INFO_NFY* pNotify = reinterpret_cast<sGU_BUDOKAI_STATE_INFO_NFY*>(pPacket);

	STenkaichiBudokaiInfo* pBudokaiState = GetNtlSLGlobal()->GetTBudokaiStateInfo();

	memcpy( &pBudokaiState->sStateInfo, &pNotify->sStateInfo, sizeof(sBUDOKAI_UPDATE_STATE_INFO) );
	for( int i =0; i < MAX_BUDOKAI_MATCH_TYPE; ++i )
		memcpy( &pBudokaiState->sMatchStateInfo[i], &pNotify->aMatchStateInfo[i], sizeof(sBUDOKAI_UPDATE_MATCH_STATE_INFO) );
	memcpy( &pBudokaiState->sJoinInfo, &pNotify->sJoinInfo, sizeof(sBUDOKAI_JOIN_INFO) );

	CNtlSLEventGenerator::BudokaiStateInfoNfy( pNotify->sStateInfo.wSeasonCount, 
		pNotify->tmDefaultOpenTime, 
		&pNotify->sStateInfo,
		pNotify->aMatchStateInfo, 
		&pNotify->sJoinInfo );
}

/**
* \brief 천하제일 무도회의 Main state update
*/
void PacketHandler_GUBudokaiUpdateStateNfy(void* pPacket)
{
	sGU_BUDOKAI_UPDATE_STATE_NFY* pNotify = reinterpret_cast<sGU_BUDOKAI_UPDATE_STATE_NFY*>(pPacket);

	/*WCHAR buf[256];
	WCHAR* string[3] = { L"CLOSE", L"OPEN", L"CLEAR" };
	swprintf_s( buf, 256, L"((BudokaiUpdateStateNfy)) State = %s",
		string[pNotify->sStateInfo.byState] );
	CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );*/

	CNtlSLEventGenerator::BudokaiUpdateStateNfy( &pNotify->sStateInfo );
}

/**
* \brief 천하제일 무도회의 Match state update
*/
void PacketHandler_GUBudokaiUpdateMatchStateNfy(void* pPacket)
{
	sGU_BUDOKAI_UPDATE_MATCH_STATE_NFY* pNotify = reinterpret_cast<sGU_BUDOKAI_UPDATE_MATCH_STATE_NFY*>(pPacket);

	CNtlSLEventGenerator::BudokaiUpdateMatchStateNfy( pNotify->byMatchType, &pNotify->sStateInfo );
}

/**
* \brief 천하제일 무도회 개인 등록
*/
void PacketHandler_GUBudokaiJoinIndividualRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_JOIN_INDIVIDUAL_RES);

	sGU_BUDOKAI_JOIN_INDIVIDUAL_RES* pResult = reinterpret_cast<sGU_BUDOKAI_JOIN_INDIVIDUAL_RES*>(pPacket);

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_JOIN_INDIVIDUAL_RES"), TRUE );
	}

	CNtlSLEventGenerator::BudokaiJoinIndividualRes( pResult->wResultCode, pResult->wJoinId, pResult->bDojoRecommender );
}

/**
* \brief 천하제일 무도회 개인 등록 취소
*/
void PacketHandler_GUBudokaiLeaveIndividualRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_LEAVE_INDIVIDUAL_RES);

	sGU_BUDOKAI_LEAVE_INDIVIDUAL_RES* pResult = reinterpret_cast<sGU_BUDOKAI_LEAVE_INDIVIDUAL_RES*>(pPacket);

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_LEAVE_INDIVIDUAL_RES"), TRUE );
	}

	CNtlSLEventGenerator::BudokaiLeaveIndividualRes( pResult->wResultCode );
}

/**
* \brief 천하제일 무도회 팀 등록정보 결과
*/
void PacketHandler_GUBudokaiJoinTeamInfoRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_JOIN_TEAM_INFO_RES);

	sGU_BUDOKAI_JOIN_TEAM_INFO_RES* pResult = reinterpret_cast<sGU_BUDOKAI_JOIN_TEAM_INFO_RES*>(pPacket);

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_JOIN_TEAM_INFO_RES"), TRUE );
	}

	CNtlSLEventGenerator::BudokaiJoinTeamInfoRes( pResult->wResultCode, pResult->byMemberCount, pResult->asPointInfo );
}

/**
* \brief 천하제일 무도회 팀 등록 결과
*/
void PacketHandler_GUBudokaiJoinTeamRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_JOIN_TEAM_RES);

	sGU_BUDOKAI_JOIN_TEAM_RES* pResult = reinterpret_cast<sGU_BUDOKAI_JOIN_TEAM_RES*>(pPacket);

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_JOIN_TEAM_RES"), TRUE );
	}

	CNtlSLEventGenerator::BudokaiJoinTeamRes( pResult->wResultCode, 
		pResult->hTroublePc, pResult->fTotalRankPoint, pResult->wJoinId, 
		pResult->wszTeamName, pResult->byMemberCount, pResult->aTeamInfo );
}

/**
* \brief 천하제일 무도회 팀 등록 알림
*/
void PacketHandler_GUBudokaiJoinTeamNfy(void* pPacket)
{
	sGU_BUDOKAI_JOIN_TEAM_NFY* pNotify = reinterpret_cast<sGU_BUDOKAI_JOIN_TEAM_NFY*>(pPacket);

	CNtlSLEventGenerator::BudokaiJoinTeamNfy( pNotify->fTotalRankPoint, pNotify->wJoinId, pNotify->wszTeamName, pNotify->byMemberCount, pNotify->aTeamInfo );
}

/**
* \brief 천하제일 무도회 팀 등록 취소 결과
*/
void PacketHandler_GUBudokaiLeaveTeamRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_LEAVE_TEAM_RES);

	sGU_BUDOKAI_LEAVE_TEAM_RES* pResult = reinterpret_cast<sGU_BUDOKAI_LEAVE_TEAM_RES*>(pPacket);

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_LEAVE_TEAM_RES"), TRUE );
	}

	CNtlSLEventGenerator::BudokaiLeaveTeamRes( pResult->wResultCode );
}

/**
* \brief 천하제일 무도회 팀 등록 취소 알림
*/
void PacketHandler_GUBudokaiLeaveTeamNfy(void* pPacket)
{
	CNtlSLEventGenerator::BudokaiLeaveTeamNfy();
}

/**
* \brief 천하제일 무도회 팀 멤버 등록 취소 결과
*/
void PacketHandler_GUBudokaiLeaveTeamMemberRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_LEAVE_TEAM_MEMBER_RES);

	sGU_BUDOKAI_LEAVE_TEAM_RES* pResult = reinterpret_cast<sGU_BUDOKAI_LEAVE_TEAM_RES*>(pPacket);

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_LEAVE_TEAM_MEMBER_RES"), TRUE );
	}

	CNtlSLEventGenerator::BudokaiLeaveTeamMemberRes( pResult->wResultCode );
}

/**
* \brief 천하제일 무도회 팀 멤버 등록 취소 알림
*/
void PacketHandler_GUBudokaiLeaveTeamMemberNfy(void* pPacket)
{
	sGU_BUDOKAI_LEAVE_TEAM_MEMBER_NFY* pNotify = reinterpret_cast<sGU_BUDOKAI_LEAVE_TEAM_MEMBER_NFY*>(pPacket);

	CNtlSLEventGenerator::BudokaiLeaveTeamMemberNfy( pNotify->wszMemberName );
}

/**
* \brief 천하제일 무도회 등록 정보
*/
void PacketHandler_GUBudokaiJoinInfoRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_JOIN_INFO_RES);
	
	sGU_BUDOKAI_JOIN_INFO_RES* pResult = reinterpret_cast<sGU_BUDOKAI_JOIN_INFO_RES*>(pPacket);

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_JOIN_INFO_RES"), TRUE );
	}

	CNtlSLEventGenerator::BudokaiJoinInfoRes( (RwUInt8)pResult->wResultCode, pResult->byMatchType, &pResult->sIndividualInfo, &pResult->sTeamInfo );
}


void PacketHandler_GUBudokaiMudosaInfoRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_MUDOSA_INFO_RES);

	sGU_BUDOKAI_MUDOSA_INFO_RES* pResult = reinterpret_cast<sGU_BUDOKAI_MUDOSA_INFO_RES*>(pPacket);

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_MUDOSA_INFO_RES"), TRUE );
	}

	CNtlSLEventGenerator::BudokaiMudosaInfoRes( pResult->wResultCode, pResult->byMudosaCount, pResult->aMudosaInfo );
}


void PacketHandler_GUBudokaiMudosaTeleportRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_MUDOSA_TELEPORT_RES);

	sGU_BUDOKAI_MUDOSA_TELEPORT_RES* pResult = reinterpret_cast<sGU_BUDOKAI_MUDOSA_TELEPORT_RES*>(pPacket);

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_MUDOSA_TELEPORT_RES"), TRUE );
	}
}

void PacketHandler_GUBudokaiJoinStateNfy(void* pPacket)
{
	sGU_BUDOKAI_JOIN_STATE_NFY* pNotify = reinterpret_cast<sGU_BUDOKAI_JOIN_STATE_NFY*>(pPacket);

	/*WCHAR buf[256];
	WCHAR* stringMatch[256] = { L"BUDOKAI_MATCHSTATE_REGISTER", 
		L"BUDOKAI_MATCHSTATE_WAIT_MINOR_MATCH", 
		L"BUDOKAI_MATCHSTATE_MINOR_MATCH", 
		L"BUDOKAI_MATCHSTATE_WAIT_MAJOR_MATCH",
		L"BUDOKAI_MATCHSTATE_MAJOR_MATCH",
		L"BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH",
		L"BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH",
		L"BUDOKAI_MATCHSTATE_WAIT_FINAL_MATCH",
		L"BUDOKAI_MATCHSTATE_FINAL_MATCH",
		L"BUDOKAI_MATCHSTATE_MATCH_END", 0 };
	stringMatch[255] = L"INVALID_MATCHSTATE";
	
	WCHAR* stringJoinState[256] = { L"BUDOKAI_JOIN_STATE_PLAY",
		L"BUDOKAI_JOIN_STATE_DROPOUT", 0 };	
	stringJoinState[255] = L"INVALID_BUDOKAI_JOIN_STATE";

	WCHAR* stringJoinResult[256] = { L"BUDOKAI_JOIN_RESULT_REGISTER",
		L"BUDOKAI_JOIN_RESULT_MINORMATCH",
		L"BUDOKAI_JOIN_RESULT_ENTER_32",
		L"BUDOKAI_JOIN_RESULT_ENTER_16",
		L"BUDOKAI_JOIN_RESULT_ENTER_8",
		L"BUDOKAI_JOIN_RESULT_ENTER_4",
		L"BUDOKAI_JOIN_RESULT_ENTER_2",
		L"BUDOKAI_JOIN_RESULT_WINNER", 0 };
	stringJoinResult[255] = L"INVALID_BUDOKAI_JOIN_RESULT";

	swprintf_s( buf, 256, L"((BudokaiJoinStateNfy)) MatchState = %s JoinState = %s JoinResult = %s",
		stringMatch[pNotify->byMatchType],
		stringJoinState[pNotify->byJoinState],
		stringJoinResult[pNotify->byJoinResult] );
	CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );*/

	CNtlSLEventGenerator::BudokaiJoinStateNfy( pNotify->byMatchType, pNotify->byJoinState, pNotify->byJoinResult );
}

void PacketHandler_GUBudokaiJoinStateRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_JOIN_STATE_RES);

	sGU_BUDOKAI_JOIN_STATE_RES* pResult = reinterpret_cast<sGU_BUDOKAI_JOIN_STATE_RES*>(pPacket);

	/*WCHAR buf[256];
	WCHAR* stringMatch[256] = { L"BUDOKAI_MATCHSTATE_REGISTER", 
		L"BUDOKAI_MATCHSTATE_WAIT_MINOR_MATCH", 
		L"BUDOKAI_MATCHSTATE_MINOR_MATCH", 
		L"BUDOKAI_MATCHSTATE_WAIT_MAJOR_MATCH",
		L"BUDOKAI_MATCHSTATE_MAJOR_MATCH",
		L"BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH",
		L"BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH",
		L"BUDOKAI_MATCHSTATE_WAIT_FINAL_MATCH",
		L"BUDOKAI_MATCHSTATE_FINAL_MATCH",
		L"BUDOKAI_MATCHSTATE_MATCH_END", 0 };
	stringMatch[255] = L"INVALID_MATCHSTATE";

	WCHAR* stringJoinState[256] = { L"BUDOKAI_JOIN_STATE_PLAY",
		L"BUDOKAI_JOIN_STATE_DROPOUT", 0 };	
	stringJoinState[255] = L"INVALID_BUDOKAI_JOIN_STATE";

	WCHAR* stringJoinResult[256] = { L"BUDOKAI_JOIN_RESULT_REGISTER",
		L"BUDOKAI_JOIN_RESULT_MINORMATCH",
		L"BUDOKAI_JOIN_RESULT_ENTER_32",
		L"BUDOKAI_JOIN_RESULT_ENTER_16",
		L"BUDOKAI_JOIN_RESULT_ENTER_8",
		L"BUDOKAI_JOIN_RESULT_ENTER_4",
		L"BUDOKAI_JOIN_RESULT_ENTER_2",
		L"BUDOKAI_JOIN_RESULT_WINNER", 0 };
	stringJoinResult[255] = L"INVALID_BUDOKAI_JOIN_RESULT";

	swprintf_s( buf, 256, L"((BudokaiJoinStateRes)) MatchState = %s JoinState = %s JoinResult = %s",
		stringMatch[pResult->byMatchType],
		stringJoinState[pResult->byJoinState],
		stringJoinResult[pResult->byJoinResult] );
	CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );*/

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_JOIN_STATE_RES"), TRUE );
	}

	CNtlSLEventGenerator::BudokaiJoinStateRes( pResult->wResultCode, pResult->byMatchType, pResult->byJoinState,
		pResult->byJoinResult );
}

void PacketHandler_GUBudokaiMarkingNfy(void* pPacket)
{
   /* sGU_CHAR_MARKING_NFY* pData = (sGU_CHAR_MARKING_NFY*)pPacket;

    CNtlSLEventGenerator::TenkaichiMarking(pData->handle, pData->sMarking.byCode);*/
}

void PacketHandler_GUBudokaiProgressMessageNfy(void* pPacket)
{
	sGU_BUDOKAI_PROGRESS_MESSAGE_NFY* pNotify = (sGU_BUDOKAI_PROGRESS_MESSAGE_NFY*)pPacket;

	CNtlSLEventGenerator::BudokaiProgressMessageNfy( pNotify->byMsgId );
}

void PacketHandler_GUBudokaiSocialAction(void* pPacket)
{
	sGU_BUDOKAI_SOCIAL_ACTION_NFY* pNotify = (sGU_BUDOKAI_SOCIAL_ACTION_NFY*)pPacket;

	CNtlSLEventGenerator::SobAnimPlay(pNotify->hSubject, pNotify->socialAction, FALSE, 0.0f);
}

void PacketHandler_GUBudokaiPrizeWinnerNameRes(void* pPacket)
{
	API_GetSLPacketLockManager()->IndividualUnlock(GU_BUDOKAI_PRIZEWINNER_NAME_RES);

	sGU_BUDOKAI_PRIZEWINNER_NAME_RES* pResult = (sGU_BUDOKAI_PRIZEWINNER_NAME_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_PRIZEWINNER_NAME_RES"), TRUE );
	}

	//// Event 보냄
	CDboEventGenerator::BudokaiPrizeWinnerName( BUDOKAI_MATCH_TYPE_INDIVIDIAUL, pResult->wWinner, pResult->wSecondWinner, reinterpret_cast<void*>( &pResult->sData ) );
}

void PacketHandler_GUBudokaiPrizeWinnerTeamNameRes(void* pPacket)
{
	API_GetSLPacketLockManager()->IndividualUnlock(GU_BUDOKAI_PRIZEWINNER_TEAM_NAME_RES);

	sGU_BUDOKAI_PRIZEWINNER_TEAM_NAME_RES* pResult = (sGU_BUDOKAI_PRIZEWINNER_TEAM_NAME_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_PRIZEWINNER_TEAM_NAME_RES"), TRUE );
	}

	CDboEventGenerator::BudokaiPrizeWinnerName( BUDOKAI_MATCH_TYPE_TEAM, pResult->wWinner, pResult->wSecondWinner, reinterpret_cast<void*>( &pResult->sData ) );
}