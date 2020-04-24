#include "precomp_ntlsimulation.h"
#include "NtlSLRankBattle.h"

#include "NtlResultCode.h"
#include "NtlRankBattle.h"
#include "PCTable.h"

#include "NtlSoundEventGenerator.h"
#include "GUISoundDefine.h"

#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSobManager.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlWorldConceptRB.h"
#include "TableContainer.h"

CNtlRankBattle::CNtlRankBattle(void)
{
	m_mapMyTeam.clear();
	m_mapEnemyTeam.clear();

	Init();
}

CNtlRankBattle::~CNtlRankBattle(void)
{
	m_mapMyTeam.clear();
	m_mapEnemyTeam.clear();
}

void CNtlRankBattle::Init(void)
{
	m_eResult = NONE;
	m_bRoomOwner = FALSE;
	SetRBRequestState( RB_REQUEST_STATE_NONE );
	m_pTblDat = NULL;
	
	m_nMyTeamWinScore = 0;
	m_nEnemyTeamWinScore = 0;

	::ZeroMemory( &m_myRankScore, sizeof( sRANKBATTLE_SCORE_INFO ) );
}

void CNtlRankBattle::ResetMatchData(void)
{
	m_eResult = NONE;
	m_bRoomOwner = FALSE;
	SetRBRequestState( RB_REQUEST_STATE_NONE );
	m_pTblDat = NULL;

	m_nMyTeamWinScore = 0;
	m_nEnemyTeamWinScore = 0;
}

RwBool CNtlRankBattle::Create(void)
{
	SAvatarInfo* pInfo = GetNtlSLGlobal()->GetAvatarInfo();
	if( pInfo )
		memcpy( &m_myRankScore, &pInfo->sRankBattleScoreInfo, sizeof( sRANKBATTLE_SCORE_INFO ) );

	LinkMsg( g_EventRBJoin, (const RwChar*)0, 0x9000 );
	//LinkMsg( g_EventRBMatch, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventRBBattleStateUpdate, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventRBBattleTeamInfo, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventRBBattlePlayerState, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventRBBattleKillScoreUpdate, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventRBBattleStageFinish, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventRBBattleMatchFinish, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventRBTotalScoreUpdate, (const RwChar*)0, 0x9000 );
	//LinkMsg( g_EventRBBattleEnd, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventCharRankPointResetNfy, (const RwChar*)0, 0x9000 );
				
	return TRUE;
}

void CNtlRankBattle::Destroy(void)
{
	UnLinkMsg( g_EventRBJoin ); 
	//UnLinkMsg( g_EventRBMatch );
	UnLinkMsg( g_EventRBBattleStateUpdate ); 
	UnLinkMsg( g_EventRBBattleTeamInfo ); 
	UnLinkMsg( g_EventRBBattlePlayerState ); 
	UnLinkMsg( g_EventRBBattleKillScoreUpdate );
	UnLinkMsg( g_EventRBBattleStageFinish ); 
	UnLinkMsg( g_EventRBBattleMatchFinish ); 
	UnLinkMsg( g_EventRBTotalScoreUpdate ); 
	//UnLinkMsg( g_EventRBBattleEnd );
	UnLinkMsg( g_EventCharRankPointResetNfy );
}

void CNtlRankBattle::HandleEvents(RWS::CMsg& msg)
{
	if( msg.Id == g_EventRBBattleTeamInfo )
	{
		NTL_ASSERT( m_pTblDat, "CNtlRankBattle::HandleEvent : Room doesn't have valid tblidx" );

		SNtlEventRBBattleTeamInfo* pData = reinterpret_cast<SNtlEventRBBattleTeamInfo*>( msg.pData );
		sRANKBATTLE_MATCH_MEMBER_INFO* pMemberInfo = reinterpret_cast<sRANKBATTLE_MATCH_MEMBER_INFO*>( pData->pMemberList );		

		m_mapMyTeam.clear();
		m_mapEnemyTeam.clear();

		// peessitemp : 임시. 나중에 Myteam EnemyTeam 구조 자체를 바꾼다. 
		m_bRoomOwner = FALSE;
		for( RwInt32 i = 0 ; i < pData->byCount ; ++i )
		{
			if( pMemberInfo[i].hPc == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			{
				if( pMemberInfo[i].byTeam == RANKBATTLE_TEAM_OWNER )
					m_bRoomOwner = TRUE;
			}
		}
		//
		
		for( RwInt32 i = 0 ; i < pData->byCount ; ++i )
		{
			stRankBattleMember stRBMemberInfo;
			stRBMemberInfo.hSerial = pMemberInfo[i].hPc;	
			stRBMemberInfo.byState = pMemberInfo[i].byState;
									
			if( m_bRoomOwner )
			{
				if( pMemberInfo[i].byTeam == RANKBATTLE_TEAM_OWNER )
					m_mapMyTeam[pMemberInfo[i].hPc] = stRBMemberInfo;
				else if( pMemberInfo[i].byTeam == RANKBATTLE_TEAM_CHALLENGER )
					m_mapEnemyTeam[pMemberInfo[i].hPc] = stRBMemberInfo;
				else
					NTL_ASSERTFAIL( "CNtlRankBattle::HandleEvent : Wrong Team Info" );
			}
			else
			{
				if( pMemberInfo[i].byTeam == RANKBATTLE_TEAM_OWNER )
					m_mapEnemyTeam[pMemberInfo[i].hPc] = stRBMemberInfo;
				else if( pMemberInfo[i].byTeam == RANKBATTLE_TEAM_CHALLENGER )
					m_mapMyTeam[pMemberInfo[i].hPc] = stRBMemberInfo;
				else
					NTL_ASSERTFAIL( "CNtlRankBattle::HandleEvent : Wrong Team Info" );
			}
		}

		if( m_pTblDat->byBattleMode == RANKBATTLE_MODE_PARTY )
		{
			if( m_bRoomOwner )
			{
				m_MyTeamName = pData->wcOwnerPartyName;
				m_EnemyTeamName = pData->wcChallengerPartyName;
			}
			else
			{
				m_MyTeamName = pData->wcChallengerPartyName;
				m_EnemyTeamName = pData->wcOwnerPartyName;
			}
		}		

		m_nBlueTeamStraightWinScore = pData->wStraightWinCount;
		m_nBlueTeamPerfectWinScore = pData->wPerfectWinCount;

		SetRBRequestState( RB_REQUEST_STATE_IN_MATCH );
	}
	else if( msg.Id == g_EventRBJoin )
	{
		SNtlEventRBJoin* pJoinData = reinterpret_cast<SNtlEventRBJoin*>( msg.pData );

		if( pJoinData->eType == SNtlEventRBJoin::STATE_JOIN )
		{
			SetRBRequestState( RB_REQUEST_STATE_REQUEST );
			SetTblData( pJoinData->tblidx );
		}
		else if( pJoinData->eType == SNtlEventRBJoin::STATE_LEAVE )
		{
			SetRBRequestState( RB_REQUEST_STATE_NONE );
			ResetMatchData();
		}
		else if( pJoinData->eType == SNtlEventRBJoin::STATE_START )
		{
			//if( pJoinData->wResultCode == GAME_SUCCESS )
			//	SetRBRequestState( RB_REQUEST_STATE_WAIT_MATCH );
		}
		else if( pJoinData->eType == SNtlEventRBJoin::STATE_CANCELED )
		{
			SetRBRequestState( RB_REQUEST_STATE_NONE );
			ResetMatchData();
		}
	}
	//else if( msg.Id == g_EventRBRoom )
	//{
	//	SNtlEventRBRoom* pRoomData = reinterpret_cast<SNtlEventRBRoom*>( msg.pData );
	//	
	//	if( pRoomData->type == SNtlEventRBRoom::ROOM_CREATE || 
	//		pRoomData->type == SNtlEventRBRoom::ROOM_JOIN_OWNER )
	//	{
	//		m_bRoomOwner = TRUE;
	//		SetTblData( pRoomData->tblidx );
	//		SetRBRequestState( RB_REQUEST_STATE_REQUEST );
	//	}
	//	else if( pRoomData->type == SNtlEventRBRoom::ROOM_CHALLENGE ||
	//			 pRoomData->type == SNtlEventRBRoom::ROOM_JOIN_CHALLENGE )
	//	{
	//		m_bRoomOwner = FALSE;
	//		SetTblData( pRoomData->tblidx );
	//		SetRBRequestState( RB_REQUEST_STATE_REQUEST );
	//	}
	//	else if( pRoomData->type == SNtlEventRBRoom::ROOM_LEAVE )
	//	{
	//		Init();
	//	}		
	//}	
	else if( msg.Id == g_EventRBBattleStateUpdate )
	{
		CNtlWorldConceptRB* pRBWorldConcept = reinterpret_cast<CNtlWorldConceptRB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_RANK_BATTLE ) );
		//NTL_ASSERT( pRBWorldConcept, "CNtlRankBattle::HandleEvents : must World concept is valid" );		
		if( !pRBWorldConcept )
		{
			DBO_WARNING_MESSAGE( "CNtlRankBattle : g_EventRBBattleStateUpdate, no world concept. critical error" );
		
			RwUInt8 byServerState = 0, byClientState = 0;
			char chState[124];
			CNtlSLEventGenerator::SobGetState( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), byServerState, byClientState, chState);
			DBO_WARNING_MESSAGE( "AvatarServerState = " << byServerState << " , AvatarClientState = " << byClientState );
		
			return;	
		}		

		SNtlEventRBBattleStateUpdate* pData = reinterpret_cast<SNtlEventRBBattleStateUpdate*>( msg.pData );
		pRBWorldConcept->ChangeRBState( pData->byBattleState );

		// Enter the name here ... assuming all objects are loaded at this time.
		if( pData->byBattleState == RANKBATTLE_BATTLESTATE_DIRECTION )
		{
			CNtlSob* pSobObj = NULL;

			MAP_RBMEMBER_ITER it;
			for( it = m_mapMyTeam.begin() ; it != m_mapMyTeam.end() ; ++it )
			{
				stRankBattleMember* pMemberData = &(*it).second;

				if( pMemberData->byState == RANKBATTLE_MEMBER_STATE_NOAPPLY )
					continue;
					
				pSobObj = GetNtlSobManager()->GetSobObject( pMemberData->hSerial );
				NTL_ASSERT( pSobObj, "CNtlRankBattle::HandleEvents::DIRECTION : TeamInfo Handle is Null" );
				CNtlSobPlayerAttr* pPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( pSobObj->GetSobAttr() );
				
				pMemberData->wstrName = Logic_GetName( pSobObj );
				pMemberData->byClass = pPlayerAttr->GetPcTbl()->byClass;
			}

			for( it = m_mapEnemyTeam.begin() ; it != m_mapEnemyTeam.end() ; ++it )
			{
				stRankBattleMember* pMemberData = &(*it).second;

				if( pMemberData->byState == RANKBATTLE_MEMBER_STATE_NOAPPLY )
					continue;

				pSobObj = GetNtlSobManager()->GetSobObject( pMemberData->hSerial );
				NTL_ASSERT( pSobObj, "CNtlRankBattle::HandleEvents::DIRECTION : TeamInfo Handle is Null" );
				CNtlSobPlayerAttr* pPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( pSobObj->GetSobAttr() );

				pMemberData->wstrName = Logic_GetName( pSobObj );
				pMemberData->byClass = pPlayerAttr->GetPcTbl()->byClass;
			}

			if( m_pTblDat->byBattleMode == RANKBATTLE_MODE_INDIVIDUAL )
			{
				pSobObj = GetNtlSobManager()->GetSobObject( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
				NTL_ASSERT( pSobObj, "CNtlRankBattle::HandleEvents : Avatar is Null" );
				m_MyTeamName = Logic_GetName( pSobObj );

				if( m_mapEnemyTeam.size() )
				{
					stRankBattleMember* pMemberData = &(*m_mapEnemyTeam.begin()).second;
					if( pMemberData->byState != RANKBATTLE_MEMBER_STATE_NOAPPLY )
					{
						pSobObj = GetNtlSobManager()->GetSobObject( (*m_mapEnemyTeam.begin()).first );
						NTL_ASSERT( pSobObj, "CNtlRankBattle::HandleEvents : EnemyTeam FirstMember is Null" );
						m_EnemyTeamName = Logic_GetName( pSobObj );
					}				 
				}				
			}		

			if( m_pTblDat->byBattleMode == RANKBATTLE_MODE_INDIVIDUAL )
				CNtlSLEventGenerator::TSRankBattle( m_pTblDat->tblidx, eRB_COND_FLAG_PRIVATE_ENTER );
			else 
				CNtlSLEventGenerator::TSRankBattle( m_pTblDat->tblidx, eRB_COND_FLAG_PARTY_ENTER );
		}
		else if( pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_PREPARE)
		{
			/*
			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup	= CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE;
			tSoundParam.pcFileName		= GSD_JINGLE_PVP_START;
			GetSoundManager()->Play(&tSoundParam);
*/
			if( IsPartyBattle() )
			{
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_START_PARTY_RANKBATTLE_BGM);
			}
			else
			{
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_START_PRIVATE_RANKBATTLE_BGM);
			}
		}
		else if( pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_RUN)
		{
			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup	= CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE;
			tSoundParam.pcFileName		= GSD_JINGLE_PVP_END;
			GetSoundManager()->Play(&tSoundParam);

			if( IsPartyBattle() )
			{
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_END_PARTY_RANKBATTLE_BGM);
			}
			else
			{
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_END_PRIVATE_RANKBATTLE_BGM);
			}
		}
	}
	//else if( msg.Id == g_EventRBMatch )
	//{
	//	SNtlEventRBMatch* pMatchData = reinterpret_cast<SNtlEventRBMatch*>( msg.pData );

	//	if( pMatchData->type == SNtlEventRBMatch::MATCH_READY )
	//		SetRBRequestState( RB_REQUEST_STATE_WAIT_MATCH );		
	//}
	else if( msg.Id == g_EventRBBattleMatchFinish )
	{
		SNtlEventRBBattleMatchFinish* pMatchFinish = reinterpret_cast<SNtlEventRBBattleMatchFinish*>( msg.pData );
		sRANKBATTLE_MATCH_FINISH_INFO* pMatchResultData = reinterpret_cast<sRANKBATTLE_MATCH_FINISH_INFO*>( pMatchFinish->pMatchResult );

		if( IsRoomOwner() )
		{
			if( pMatchResultData->byMatchWinner == RANKBATTLE_MATCH_WIN_OWNER )
				m_eResult = WIN;
			else if( pMatchResultData->byMatchWinner == RANKBATTLE_MATCH_WIN_CHALLENGER )
				m_eResult = LOSE;
			else if( pMatchResultData->byMatchWinner == RANKBATTLE_MATCH_DRAW )
				m_eResult = DRAW;
			else
				m_eResult = NONE;
		}
		else
		{
			if( pMatchResultData->byMatchWinner == RANKBATTLE_MATCH_WIN_OWNER )
				m_eResult = LOSE;
			else if( pMatchResultData->byMatchWinner == RANKBATTLE_MATCH_WIN_CHALLENGER )
				m_eResult = WIN;			
			else if( pMatchResultData->byMatchWinner == RANKBATTLE_MATCH_DRAW )
				m_eResult = DRAW;
			else
				m_eResult = NONE;
		}

		if( m_eResult == WIN )
		{
			if( m_pTblDat->byBattleMode == RANKBATTLE_MODE_INDIVIDUAL )
				CNtlSLEventGenerator::TSRankBattle( m_pTblDat->tblidx, eRB_COND_FLAG_PRIVATE_WIN );
			else
				CNtlSLEventGenerator::TSRankBattle( m_pTblDat->tblidx, eRB_COND_FLAG_PARTY_WIN );
		}
	}
	else if( msg.Id == g_EventRBBattlePlayerState )
	{
		SNtlEventRBBattlePlayerState* pPlayerState = reinterpret_cast<SNtlEventRBBattlePlayerState*>( msg.pData );

		stRankBattleMember* pMemberData = FindMemberData( pPlayerState->hSerial );
		//NTL_ASSERT( pMemberData, "CNtlRankBattle::GetRankBattleMemberBySerial : There is no matched data" );
		
		if( pMemberData )		
			pMemberData->byState = pPlayerState->byState;
		else
		{
			CNtlWorldConceptRB* pRBWorldConcept = reinterpret_cast<CNtlWorldConceptRB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_RANK_BATTLE ) );
			RwInt32 eRBState = -1;

			if( pRBWorldConcept )
				eRBState = pRBWorldConcept->GetRBState();

			RwUInt8 byServerState = 0, byClientState = 0;
			char chState[124];
			CNtlSLEventGenerator::SobGetState( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), byServerState, byClientState, chState);

			DBO_WARNING_MESSAGE( "g_EventRBBattlePlayerStaet : Invalid MemberData in State [" << eRBState << "]" );
			DBO_WARNING_MESSAGE("AvatarServerState = " << byServerState << " , AvatarClientState = " << byClientState );
		}
// DebugInfo
//#ifndef NDEBUG
//		WCHAR buf[256];
//		WCHAR* szState;
//		
//		switch( pPlayerState->byState )
//		{
//		case RANKBATTLE_MEMBER_STATE_NONE : szState = L"NONE"; break;
//		case RANKBATTLE_MEMBER_STATE_NORMAL : szState = L"NORMAL"; break;
//		case RANKBATTLE_MEMBER_STATE_FAINT : szState = L"FAINT"; break;
//		case RANKBATTLE_MEMBER_STATE_OUTOFAREA : szState = L"OUTOFAREA"; break;
//		case RANKBATTLE_MEMBER_STATE_GIVEUP : szState = L"GIVEUP"; break;
//		case RANKBATTLE_MEMBER_STATE_NOAPPLY : szState = L"NOAPPLY"; break;	
//		default: szState = L"ERROR"; break;
//		}
//
//		CNtlSobPlayer* pPlayer = reinterpret_cast<CNtlSobPlayer*>( GetNtlSobManager()->GetSobObject( pPlayerState->hSerial ) );
//		if( pPlayer )
//		{
//			swprintf_s( buf, 256, L"GU_RANKBATTLE_PLAYER_STATE_NFY\\n%s : [ %s state ]", Logic_GetName( pPlayer ), szState );				
//			CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_WARN );
//		}
//#endif
	}
	else if( msg.Id == g_EventRBBattleKillScoreUpdate )
	{
		//SNtlEventRBBattleKillScoreUpdate* pData = reinterpret_cast<SNtlEventRBBattleKillScoreUpdate*>( msg.pData );

//#ifndef NDEBUG
//		WCHAR buf[256];
//		swprintf_s( buf, 256, L"GU_RANKBATTLE_KILLSCORE_UPDATE_NFY\\nOwnerKill : %u, ChallengerKill : %u", pData->byOwnerKillScore, pData->byChallengerKillScore );
//		CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_WARN );
//#endif	
	}
	else if( msg.Id == g_EventRBBattleStageFinish )
	{
		SNtlEventRBBattleStageFinish* pData = reinterpret_cast<SNtlEventRBBattleStageFinish*>( msg.pData );
		sRANKBATTLE_SCORE* pScore = reinterpret_cast<sRANKBATTLE_SCORE*>( pData->pStageScoreData );

		if( m_bRoomOwner )
		{
			m_nMyTeamWinScore = pScore->byOwner;
			m_nEnemyTeamWinScore = pScore->byChallenger;
		}
		else
		{
			m_nMyTeamWinScore = pScore->byChallenger;
			m_nEnemyTeamWinScore = pScore->byOwner;
		}

//#ifndef NDEBUG
//		WCHAR buf[256];
//		swprintf_s( buf, 256, L"GU_RANKBATTLE_BATTLE_STAGE_FINISH_NFY\\nOwner : %u, Challenger : %u", pScore->byOwner, pScore->byChallenger );
//		CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_WARN );
//#endif	
	}
	else if( msg.Id == g_EventRBTotalScoreUpdate )
	{
		SNtlEventRBTotalScoreUpdate* pData = reinterpret_cast<SNtlEventRBTotalScoreUpdate*>( msg.pData );
		sRANKBATTLE_SCORE_INFO* pTotalScore = reinterpret_cast<sRANKBATTLE_SCORE_INFO*>( pData->pTotalScoreData );

		memcpy( &m_myRankScore, pTotalScore, sizeof( sRANKBATTLE_SCORE_INFO ) );

//#ifndef NDEBUG
//		WCHAR buf[256];
//		swprintf_s( buf, 256, L"GU_RANKBATTLE_TOTAL_SCORE_NFY\\n BM:%u MSW:%u, SW:%u, MSKO:%u, SKO:%u, WC:%u, DC:%u, LC:%u, P:%f",
//			pData->byBattleMode, pTotalScore->wMaxStraightWin, pTotalScore->wStraightWin, pTotalScore->wMaxStraightKOWin, pTotalScore->wStraightKOWin,
//			pTotalScore->dwWin, pTotalScore->dwDraw, pTotalScore->dwLose, pTotalScore->fPoint );
//		CNtlSLEventGenerator::SysMsg( buf, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_WARN );
//#endif	
	}
	// 랭크 포인트 초기화
	else if( msg.Id == g_EventCharRankPointResetNfy )
	{
		m_myRankScore.fPoint = 0.0f;
	}
	//else if( msg.Id == g_EventRBBattleEnd )
	//{
	//	// 방의 승계는 이곳에서 정한다. 
	//	SNtlEventRBBattleEnd* pData = reinterpret_cast<SNtlEventRBBattleEnd*>( msg.pData );

	//	if( pData->byWinner == RANKBATTLE_MATCH_WIN_CHALLENGER )
	//	{
	//		if( IsRoomOwner() )
	//			m_bRoomOwner = FALSE;
	//		else 
	//			m_bRoomOwner = TRUE;
	//	}		

	//	// 일단 요청상태로 변경. 진팀은 자동으로 RoomLeave에서 리셋된다.
	//	SetRBRequestState( RB_REQUEST_STATE_REQUEST );
	//}
}

RwBool CNtlRankBattle::IsAttackableEnemy(RwUInt32 hSerialId)
{
	MAP_RBMEMBER_ITER it = m_mapEnemyTeam.find( hSerialId );
	
	if( it == m_mapEnemyTeam.end() )
		return FALSE;

	if( (*it).second.byState == RANKBATTLE_MEMBER_STATE_ATTACKABLE)
		return TRUE;

	return FALSE;
}

CNtlSobActor* CNtlRankBattle::GetNearAttackableEnemy(CNtlSobActor* pActor)
{
	MAP_RBMEMBER_ITER it;

	RwReal fLen, fMinLen = FLT_MAX;
	RwV3d vPos = pActor->GetPosition();
	RwV3d vSub, vTarPos;
	CNtlSobActor* pTargetActor = NULL;
	CNtlSobActor* pSobActor;

	for( it = m_mapEnemyTeam.begin() ; it != m_mapEnemyTeam.end() ; ++it )
	{
		if( (*it).second.byState != RANKBATTLE_MEMBER_STATE_ATTACKABLE)
			continue;

		pSobActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject( (*it).second.hSerial ) );
        
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

void CNtlRankBattle::SetTblData( RwUInt32 tblidx )
{
	if( tblidx == INVALID_SERIAL_ID )
		m_pTblDat = NULL;
	else
		m_pTblDat = reinterpret_cast<sRANKBATTLE_TBLDAT*>( API_GetTableContainer()->GetRankBattleTable()->FindData( tblidx ) );	
}

RwBool CNtlRankBattle::IsPartyBattle(VOID)
{
	if( !m_pTblDat )
		return FALSE;

	return m_pTblDat->byBattleMode;
}

RwBool CNtlRankBattle::IsEnableRBBoard(VOID)
{
	if( GetRBRequestState() == RB_REQUEST_STATE_WAIT_MATCH ||
		GetRBRequestState() == RB_REQUEST_STATE_IN_MATCH )
		return FALSE;
	else
		return TRUE;
}

CNtlRankBattle::stRankBattleMember* CNtlRankBattle::FindMemberData(RwUInt32 hSerialID)
{
	MAP_RBMEMBER_ITER it = m_mapMyTeam.find( hSerialID );

	if( it == m_mapMyTeam.end() )
	{
		it = m_mapEnemyTeam.find( hSerialID );

		if( it == m_mapEnemyTeam.end() )
			return NULL;		
	}

	return &(*it).second;
}