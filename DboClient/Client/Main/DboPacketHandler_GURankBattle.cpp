/*****************************************************************************
* File			: DboPackethandler_GURankBattle.cpp
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2007. 1. 16
* Abstract		: 랭크 배틀 관련 패킷 핸들
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// simulation
#include "NtlNetSender.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"


//void PacketHandler_GSRBRoomListRes(void* pPacket)
//{
//	API_GetSLPacketLockManager()->Unlock( GU_RANKBATTLE_ROOM_LIST_RES );
//	
//	sGU_RANKBATTLE_ROOM_LIST_RES* pResult = (sGU_RANKBATTLE_ROOM_LIST_RES*)pPacket;
//
//	if( pResult->wResultCode != GAME_SUCCESS )
//	{
//		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, pResult->wResultCode);
//		return;
//	}
//
////	CNtlSLEventGenerator::RBBoardUpdate(pResult->rankBattleTblidx, pResult->wTotalPage, pResult->wCurPage, 
////										pResult->byRoomCount, pResult->byRoomMode, (void*)&pResult->uRoomList);
//}

//void PacketHandler_GSRBRoomInfoRes(void* pPacket)
//{
//	API_GetSLPacketLockManager()->Unlock( GU_RANKBATTLE_ROOM_INFO_RES );
//
//	sGU_RANKBATTLE_ROOM_INFO_RES* pResult = (sGU_RANKBATTLE_ROOM_INFO_RES*)pPacket;
//
//	if( pResult->wResultCode != GAME_SUCCESS )
//	{
//		CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, pResult->wResultCode );
//		return;
//	}
//
////	CNtlSLEventGenerator::RBRoomInfo( pResult->byRoomMode, (void*)&pResult->uRoom );
//}

//void PacketHandler_GSRBRoomCreateRes(void* pPacket)
//{
//	API_GetSLPacketLockManager()->Unlock( GU_RANKBATTLE_ROOM_CREATE_RES );
//
//	sGU_RANKBATTLE_ROOM_CREATE_RES* pResult = (sGU_RANKBATTLE_ROOM_CREATE_RES*)pPacket;
//
//	// 생성되지 않았을때 메세지 표시, 파티원 누구때문인지도 표시.
//	if( pResult->wResultCode != GAME_SUCCESS )
//	{
//		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, pResult->wResultCode);
//		
//		sRANKBATTLE_TBLDAT* pRankBattleTbl = reinterpret_cast<sRANKBATTLE_TBLDAT*>( API_GetTableContainer()->GetRankBattleTable()->FindData( pResult->rankBattleTblidx ) );
//		
//		CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
//		std::wstring wBuf;
//
//		for( RwUInt32 i = 0 ; i < pResult->byTroubleCount ; ++i )
//		{
//			if( pRankBattleTbl->byBattleMode == RANKBATTLE_MODE_PARTY )
//			{
//				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey( pResult->asTroublePc[i].hPc ) );
//				NTL_ASSERT( pMember, "PacketHandler_GSRBRoomCreateRes : There is no player in party" );
//				wBuf = pMember->wszMemberName;
//			}
//			else
//				wBuf = Logic_GetName( GetNtlSLGlobal()->GetSobAvatar() );			
//
//			if( pResult->asTroublePc[i].wResultCode >= GAME_ROOM_ENTER_TOO_LOW_LEVEL &&
//				pResult->asTroublePc[i].wResultCode <= GAME_ROOM_ENTER_MUST_HAVE_NEED_ZENNY )
//				wBuf += GetDisplayStringManager()->GetString( DST_RB_SIDE_ENTER_TOO_LOW_LEVEL + pResult->asTroublePc[i].wResultCode - GAME_ROOM_ENTER_TOO_LOW_LEVEL );
//			else
//				wBuf += GetDisplayStringManager()->GetString( DST_RB_SIDE_ENTER_STATE_ERROR );
//			
//			CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, SENDERNAME_SYSTEM, (RwUInt16)wBuf.size(), wBuf.c_str() );	
//		}	
//		return;
//	}
//	
//	// 사이드 아이콘 표시
//	//CNtlSLEventGenerator::RBRoomEvent(SNtlEventRBRoom::ROOM_CREATE, pResult->rankBattleTblidx, pResult->roomId);
//}

//void PacketHandler_GSRBRoomCreateNfy(void* pPacket)
//{
//	sGU_RANKBATTLE_ROOM_CREATE_NFY* pResult = (sGU_RANKBATTLE_ROOM_CREATE_NFY*)pPacket;
//
//	// 생성되지 않았을때 메세지 표시
//	if( pResult->wResultCode != GAME_SUCCESS )
//	{
//		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, pResult->wResultCode);
//		return;
//	}
//
//	// 사이드 아이콘 표시
//	//CNtlSLEventGenerator::RBRoomEvent(SNtlEventRBRoom::ROOM_CREATE, pResult->rankBattleTblidx, pResult->roomId);
//}

//void PacketHandler_GSRBRoomLeaveRes(void* pPacket)
//{
//	API_GetSLPacketLockManager()->Unlock( GU_RANKBATTLE_ROOM_LEAVE_RES );
//
//	sGU_RANKBATTLE_ROOM_LEAVE_RES* pResult = (sGU_RANKBATTLE_ROOM_LEAVE_RES*)pPacket;
//
//	if( pResult->wResultCode != GAME_SUCCESS )
//	{
//		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, pResult->wResultCode);
//		return;
//	}
//
//	// 방에서 나갔다는 메세지를 메세지 박스로 표현
//	//CNtlSLEventGenerator::RBRoomEvent(SNtlEventRBRoom::ROOM_LEAVE);
//}
//
//void PacketHandler_GSRBRoomLeaveNfy(void* pPacket)
//{
//	sGU_RANKBATTLE_ROOM_LEAVE_NFY* pResult = (sGU_RANKBATTLE_ROOM_LEAVE_NFY*)pPacket;
//
//	// 방에서 나갔다는 메세지를 메세지 박스로 표현
//	//CNtlSLEventGenerator::RBRoomEvent(SNtlEventRBRoom::ROOM_LEAVE);
//}
//
//void PacketHandler_GSRBRoomJoinInfoNfy(void* pPacket)
//{
//	sGU_RANKBATTLE_ROOM_JOIN_INFO_NFY* pResult = (sGU_RANKBATTLE_ROOM_JOIN_INFO_NFY*)pPacket;
//
//	//if( pResult->byTeamType == RANKBATTLE_TEAM_OWNER )
//	//	CNtlSLEventGenerator::RBRoomEvent(SNtlEventRBRoom::ROOM_JOIN_OWNER, pResult->rankBattleTblidx, pResult->roomId);
//	//else if( pResult->byTeamType == RANKBATTLE_TEAM_CHALLENGER )
//	//	CNtlSLEventGenerator::RBRoomEvent(SNtlEventRBRoom::ROOM_JOIN_CHALLENGE, pResult->rankBattleTblidx, pResult->roomId);
//	
//	// 다른 경우는 무시.
//}

//void PacketHandler_GSRBRoomChallengeRes(void* pPacket)
//{
//	API_GetSLPacketLockManager()->Unlock( GU_RANKBATTLE_ROOM_CHALLENGE_RES );
//
//	sGU_RANKBATTLE_ROOM_CHALLENGE_RES* pResult = (sGU_RANKBATTLE_ROOM_CHALLENGE_RES*)pPacket;
//
//	// 생성되지 않았을때 메세지 표시, 파티원 누구때문인지도 표시.
//	if( pResult->wResultCode != GAME_SUCCESS )
//	{
//		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, pResult->wResultCode);
//
//		sRANKBATTLE_TBLDAT* pRankBattleTbl = reinterpret_cast<sRANKBATTLE_TBLDAT*>( API_GetTableContainer()->GetRankBattleTable()->FindData( pResult->rankBattleTblidx ) );
//
//		CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
//		std::wstring wBuf;
//
//		for( RwUInt32 i = 0 ; i < pResult->byTroubleCount ; ++i )
//		{
//			if( pRankBattleTbl->byBattleMode == RANKBATTLE_MODE_PARTY )
//			{
//				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey( pResult->asTroublePc[i].hPc ) );
//				NTL_ASSERT( pMember, "PacketHandler_GSRBRoomCreateRes : There is no player in party" );
//
//				wBuf = pMember->wszMemberName;
//			}
//			else
//				wBuf = Logic_GetName( GetNtlSLGlobal()->GetSobAvatar() );						
//
//			if( pResult->asTroublePc[i].wResultCode >= GAME_ROOM_ENTER_TOO_LOW_LEVEL &&
//				pResult->asTroublePc[i].wResultCode <= GAME_ROOM_ENTER_MUST_HAVE_NEED_ZENNY )
//				wBuf += GetDisplayStringManager()->GetString( DST_RB_SIDE_ENTER_TOO_LOW_LEVEL + pResult->asTroublePc[i].wResultCode - GAME_ROOM_ENTER_TOO_LOW_LEVEL );
//			else
//				wBuf += GetDisplayStringManager()->GetString( DST_RB_SIDE_ENTER_STATE_ERROR );
//
//			CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, SENDERNAME_SYSTEM, (RwUInt16)wBuf.size(), wBuf.c_str() );	
//		}	
//
//		return;
//	}
//
//	// 사이드 아이콘 생성
//	//CNtlSLEventGenerator::RBRoomEvent(SNtlEventRBRoom::ROOM_CHALLENGE, pResult->rankBattleTblidx, pResult->roomId);
//}
//	 
//void PacketHandler_GSRBRoomChallengeNfy(void* pPacket)
//{
//	sGU_RANKBATTLE_ROOM_CHALLENGE_NFY* pResult = (sGU_RANKBATTLE_ROOM_CHALLENGE_NFY*)pPacket;
//
//	// 생성되지 않았을때 메세지 표시
//	if( pResult->wResultCode != GAME_SUCCESS )
//	{
//		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, pResult->wResultCode);
//		return;
//	}
//
//	// 사이드 아이콘 생성
//	//CNtlSLEventGenerator::RBRoomEvent(SNtlEventRBRoom::ROOM_CHALLENGE, pResult->rankBattleTblidx, pResult->roomId);
//}
//
//void PacketHandler_GSRBRoomMatchReadyNfy(void* pPacket)
//{
//	sGU_RANKBATTLE_ROOM_MATCH_READY_NFY* pResult = (sGU_RANKBATTLE_ROOM_MATCH_READY_NFY*)pPacket;
//
//	if( pResult->wResultCode != GAME_SUCCESS )
//	{
//		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, pResult->wResultCode);		
//	}
//
//	// 진입 대기창 띄우기
//	//CNtlSLEventGenerator::RBMatchEvent(SNtlEventRBMatch::MATCH_READY, pResult->asMemberResult, pResult->byMemberCount);
//}
//
//void PacketHandler_GSRBRoomMatchStartNfy(void* pPacket)
//{
//	sGU_RANKBATTLE_ROOM_MATCH_START_NFY* pResult = (sGU_RANKBATTLE_ROOM_MATCH_START_NFY*)pPacket;
//
//	if( pResult->wResultCode != GAME_SUCCESS )
//		CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, pResult->wResultCode );	
//
//	//CNtlSLEventGenerator::RBMatchEvent( SNtlEventRBMatch::MATCH_START, (void*)&pResult->wResultCode );
//}
//
//void PacketHandler_GSRBRoomMatchCancelRes(void* pPacket)
//{
//	API_GetSLPacketLockManager()->Unlock( GU_RANKBATTLE_ROOM_MATCH_CANCEL_RES );
//	sGU_RANKBATTLE_ROOM_MATCH_CANCEL_RES* pResult = (sGU_RANKBATTLE_ROOM_MATCH_CANCEL_RES*)pPacket;
//
//	if( pResult->wResultCode != GAME_SUCCESS )
//	{
//		CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, pResult->wResultCode );
//		return;
//	}	
//
//	// 사이드 아이콘 원래 상태로 되돌림. 메세지 출력
//	//CNtlSLEventGenerator::RBMatchEvent( SNtlEventRBMatch::MATCH_CANCEL );
//	GetAlarmManager()->AlarmMessage( DST_RANKBATTLE_CANCELED );
//}
//
//void PacketHandler_GSRBRoomMatchCanceledNfy(void* pPacket)
//{
//	//CNtlSLEventGenerator::RBMatchEvent( SNtlEventRBMatch::MATCH_CANCELED );
//}

void PacketHandler_GSRBInfoRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock( GU_RANKBATTLE_INFO_RES );
//	DBO_WARNING_MESSAGE("GU_RANKBATTLE_INFO_RES");
	sGU_RANKBATTLE_INFO_RES* pInfo = reinterpret_cast<sGU_RANKBATTLE_INFO_RES*>( pPacket );

	if( pInfo->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, Logic_GetResultCodeString(pInfo->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::RBBoardInfo(pInfo->byBattleMode, pInfo->dwRemainTime, pInfo->byArenaCount, pInfo->asArenaInfo );
}

void PacketHandler_GSRBJoinRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock( GU_RANKBATTLE_JOIN_RES );
	sGU_RANKBATTLE_JOIN_RES* pJoin = reinterpret_cast<sGU_RANKBATTLE_JOIN_RES*>( pPacket );
//	DBO_WARNING_MESSAGE("GU_RANKBATTLE_JOIN_RES");
	if( pJoin->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, Logic_GetResultCodeString(pJoin->wResultCode, "").c_str());
		CNtlSLEventGenerator::RBJoinFail();
		return;
	}

	CNtlSLEventGenerator::RBJoin( TRUE, pJoin->hBoardObject, pJoin->rankBattleTblidx, pJoin->dwRemainTime, pJoin->byArenaCount, pJoin->asArenaInfo );
}

void PacketHandler_GSRBJoinNfy( void* pPacket )
{
	sGU_RANKBATTLE_JOIN_NFY* pJoin = reinterpret_cast<sGU_RANKBATTLE_JOIN_NFY*>( pPacket );
//	DBO_WARNING_MESSAGE("sGU_RANKBATTLE_JOIN_NFY");
	CNtlSLEventGenerator::RBJoin( FALSE, INVALID_SERIAL_ID, pJoin->rankBattleTblidx, 0, 0, NULL );
}

void PacketHandler_GSRBLeaveRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock( GU_RANKBATTLE_LEAVE_RES );
	sGU_RANKBATTLE_LEAVE_RES* pLeave = reinterpret_cast<sGU_RANKBATTLE_LEAVE_RES*>( pPacket );
//	DBO_WARNING_MESSAGE("sGU_RANKBATTLE_LEAVE_RES");
	if( pLeave->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, Logic_GetResultCodeString(pLeave->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::RBLeave();
}

void PacketHandler_GSRBLeaveNfy( void* pPacket )
{
//	DBO_WARNING_MESSAGE("PacketHandler_GSRBLeaveNfy");
	CNtlSLEventGenerator::RBLeave();
}

void PacketHandler_GSRBMatchStartNfy( void* pPacket )
{
	sGU_RANKBATTLE_MATCH_START_NFY* pStart = reinterpret_cast<sGU_RANKBATTLE_MATCH_START_NFY*>( pPacket );
	//DBO_WARNING_MESSAGE("sGU_RANKBATTLE_MATCH_START_NFY");
	if( pStart->wResultCode != GAME_SUCCESS )
		CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, Logic_GetResultCodeString(pStart->wResultCode, "").c_str());

	CNtlSLEventGenerator::RBStart( pStart->wResultCode );	
}

void PacketHandler_GSRBMatchCanceledNfy( void* pPacket )
{
	//DBO_WARNING_MESSAGE("PacketHandler_GSRBMatchCanceledNfy");
	CNtlSLEventGenerator::RBCancled();

	GetAlarmManager()->AlarmMessage( "DST_RANKBATTLE_CANCELED" );
}

void PacketHandler_GSRBBattleStateUpdateNfy(void* pPacket)
{
	sGU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY* pResult = (sGU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY*)pPacket;
//	DBO_WARNING_MESSAGE("sGU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY: byBattleState= " << (int)pResult->byBattleState << " byStage= " << (int)pResult->byStage);
	CNtlSLEventGenerator::RBBattleStateUpdate(pResult->byBattleState, pResult->byStage);	
}

void PacketHandler_GSRBBattleTeamInfoNfy(void* pPacket)
{
	sGU_RANKBATTLE_BATTLE_TEAM_INFO_NFY* pResult = (sGU_RANKBATTLE_BATTLE_TEAM_INFO_NFY*)pPacket;
	sRANKBATTLE_MATCH_MEMBER_INFO* pMemberInfo = reinterpret_cast<sRANKBATTLE_MATCH_MEMBER_INFO*>( pResult->sData.Read( pResult->wMemberInfo ) );
	WCHAR* pOwnerPartyName = reinterpret_cast<WCHAR*>( pResult->sData.Read( pResult->wOwnerPartyName ) );
	WCHAR* pChallengerPartyName = reinterpret_cast<WCHAR*>( pResult->sData.Read( pResult->wChallengerPartyName ) );
	RwUInt32 uiOwnerPartyNameSize = pResult->sData.GetSize( pResult->wOwnerPartyName );
	RwUInt32 uiChallengerPartyNameSize = pResult->sData.GetSize( pResult->wChallengerPartyName );
//	DBO_WARNING_MESSAGE("sGU_RANKBATTLE_BATTLE_TEAM_INFO_NFY");
	CNtlSLEventGenerator::RBBattleTeamInfo( pResult->wStraightWinCount, pResult->wStraightKOWinCount, pResult->byCount, pMemberInfo, pOwnerPartyName, pChallengerPartyName, uiOwnerPartyNameSize, uiChallengerPartyNameSize );
}

void PacketHandler_GSRBBattlePlayerStateNfy(void* pPacket)
{
	sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY* pResult = (sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY*)pPacket;
//	DBO_WARNING_MESSAGE("sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY: pResult->byPCState = " << (int)pResult->byPCState);
	// 점수 업데이트.
	CNtlSLEventGenerator::RBBattlePlayerState( pResult->hPc, pResult->byPCState );
}

void PacketHandler_GSRBBattleKillScoreUpdateNfy(void* pPacket)
{
	sGU_RANKBATTLE_BATTLE_KILLSCORE_UPDATE_NFY* pResult = (sGU_RANKBATTLE_BATTLE_KILLSCORE_UPDATE_NFY*)pPacket;
//	DBO_WARNING_MESSAGE("sGU_RANKBATTLE_BATTLE_KILLSCORE_UPDATE_NFY");
	// 킬수 업데이트.
	CNtlSLEventGenerator::RBBattleKillScoreUpdate( pResult->byOwnerKillScore, pResult->byChallengerKillScore );	
}

void PacketHandler_GSRBBattleStageFinishNfy(void* pPacket)
{
	sGU_RANKBATTLE_BATTLE_STAGE_FINISH_NFY* pResult = (sGU_RANKBATTLE_BATTLE_STAGE_FINISH_NFY*)pPacket;
//	DBO_WARNING_MESSAGE("sGU_RANKBATTLE_BATTLE_STAGE_FINISH_NFY");
	// 점수 업데이트.
	CNtlSLEventGenerator::RBBattleStageFinish( pResult->byBattleResult, (void*)&pResult->sStageScore );	
}

void PacketHandler_GSRBBattleMatchFinishNfy(void* pPacket)
{
	sGU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY* pResult = (sGU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY*)pPacket;
//	DBO_WARNING_MESSAGE("sGU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY");
	// 점수 업데이트.
	CNtlSLEventGenerator::RBBattleMatchFinish( (void*)&pResult->sMatchResult );
}

void PacketHandler_GSRBBattleEndNfy(void* pPacket)
{
	sGU_RANKBATTLE_BATTLE_END_NFY* pResult = (sGU_RANKBATTLE_BATTLE_END_NFY*)pPacket;	
//	DBO_WARNING_MESSAGE("sGU_RANKBATTLE_BATTLE_END_NFY");
	CNtlSLEventGenerator::RBBattleEnd( pResult->byMatchWinner );
}

void PAcketHandler_GSRBForcedEndNfy(void* pPacket)
{
//	DBO_WARNING_MESSAGE("PAcketHandler_GSRBForcedEndNfy");
	CNtlSLEventGenerator::RBForcedEnd();
}

void PacketHandler_GSRBTotalScoreUpdateNfy(void* pPacket)
{
	sGU_RANKBATTLE_TOTAL_SCORE_UPDATE_NFY* pResult = (sGU_RANKBATTLE_TOTAL_SCORE_UPDATE_NFY*)pPacket;
//	DBO_WARNING_MESSAGE("sGU_RANKBATTLE_TOTAL_SCORE_UPDATE_NFY");
	// 토탈 점수 업데이트. 
	CNtlSLEventGenerator::RBTotalScoreUpdate( pResult->byBattlemode, (void*)&pResult->sTotalScore );	
}