#include "precomp_ntlsimulation.h"
#include "NtlSLTBudokai.h"

#include "NtlBudokai.h"
#include "NtlMatch.h"
#include "PCTable.h"

#include "NtlSysEvent.h"
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSobAvatar.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSobManager.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlWorldConceptTB.h"
#include "TableContainer.h"
#include "NtlResultCode.h"

CNtlTBudokai::CNtlTBudokai(void)
{
	Init();
}

CNtlTBudokai::~CNtlTBudokai(void)
{
	Init();
}

void CNtlTBudokai::Init(void)
{
	m_pBudokaiTblDat = NULL;
	m_wSeasonCount = INVALID_WORD;
	::ZeroMemory(&m_sStateInfo, sizeof( sBUDOKAI_UPDATE_STATE_INFO ) );
	::ZeroMemory(&m_aMatchStateInfo, sizeof( sBUDOKAI_UPDATE_MATCH_STATE_INFO ) * MAX_BUDOKAI_MATCH_TYPE );
	::ZeroMemory(&m_sJoinInfo, sizeof( sBUDOKAI_JOIN_INFO ) );

	m_sJoinInfo.byJoinState = INVALID_BUDOKAI_JOIN_STATE;
	m_sJoinInfo.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;
	m_sJoinInfo.byMatchType = INVALID_BUDOKAI_MATCH_TYPE;

	m_fCurrentTime = 0.0f;
}

RwBool CNtlTBudokai::Create(void)
{
	m_pBudokaiTblDat = reinterpret_cast<sBUDOKAI_TBLINFO*>( API_GetTableContainer()->GetBudokaiTable()->GetBudokaiTblInfo() );

	LinkMsg( g_EventMinorMatchStateUpdate, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventMinorMatchTeamInfo, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventMinorMatchPlayerState, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventMinorMatchUpdateScore, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventMinorMatchTeamScore, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventMinorMatchStageFinish, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventMinorMatchMatchFinish, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventMajorMatchStateUpdate, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventMajorMatchTeamInfo, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventMajorMatchPlayerState, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventMajorMatchUpdateScore, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventMajorMatchStageFinish, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventMajorMatchMatchFinish, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventFinalMatchStateUpdate, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventFinalMatchTeamInfo, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventFinalMatchPlayerState, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventFinalMatchUpdateScore, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventFinalMatchStageFinish, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventFinalMatchMatchFinish, (const RwChar*)0, 0x9000 );

	LinkMsg( g_EventBudokaiStateInfoNfy, (const RwChar*)0, 0x9000 );			
	LinkMsg( g_EventBudokaiUpdateStateNfy, (const RwChar*)0, 0x9000 );			
	LinkMsg( g_EventBudokaiUpdateMatchStateNfy, (const RwChar*)0, 0x9000 );		
	LinkMsg( g_EventBudokaiJoinIndividualRes, (const RwChar*)0, 0x9000 );		
	LinkMsg( g_EventBudokaiLeaveIndividualRes, (const RwChar*)0, 0x9000 );		
	LinkMsg( g_EventBudokaiJoinTeamInfoRes, (const RwChar*)0, 0x9000 );			
	LinkMsg( g_EventBudokaiJoinTeamRes, (const RwChar*)0, 0x9000 );				
	LinkMsg( g_EventBudokaiJoinTeamNfy, (const RwChar*)0, 0x9000 );				
	LinkMsg( g_EventBudokaiLeaveTeamRes, (const RwChar*)0, 0x9000 );			
	LinkMsg( g_EventBudokaiLeaveTeamNfy, (const RwChar*)0, 0x9000 );			
	LinkMsg( g_EventBudokaiLeaveTeamMemberRes, (const RwChar*)0, 0x9000 );		
	LinkMsg( g_EventBudokaiLeaveTeamMemberNfy, (const RwChar*)0, 0x9000 );		
	LinkMsg( g_EventBudokaiJoinInfoRes, (const RwChar*)0, 0x9000 );				
	LinkMsg( g_EventBudokaiJoinStateNfy, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventBudokaiJoinStateRes, (const RwChar*)0, 0x9000 );

	LinkMsg( g_EventBudokaiProgressMessageNfy, (const RwChar*)0, 0x9000 );

	LinkMsg( g_EventUpdateTick, 0 );

	// 천하제일 무도회의 상태 ( 게임 접속 초기 )
	STenkaichiBudokaiInfo* pBudokaiState = GetNtlSLGlobal()->GetTBudokaiStateInfo();
	m_wSeasonCount = pBudokaiState->sStateInfo.wSeasonCount; 
	::CopyMemory( &m_sStateInfo, &pBudokaiState->sStateInfo, sizeof( sBUDOKAI_UPDATE_STATE_INFO ) );
	::CopyMemory( m_aMatchStateInfo, pBudokaiState->sMatchStateInfo, sizeof(sBUDOKAI_UPDATE_MATCH_STATE_INFO) * MAX_BUDOKAI_MATCH_TYPE );
	::CopyMemory( &m_sJoinInfo, &pBudokaiState->sJoinInfo, sizeof(sBUDOKAI_JOIN_INFO) );

	return TRUE;
}

void CNtlTBudokai::Destroy(void)
{
	UnLinkMsg( g_EventMinorMatchStateUpdate );
	UnLinkMsg( g_EventMinorMatchTeamInfo );
	UnLinkMsg( g_EventMinorMatchPlayerState );
	UnLinkMsg( g_EventMinorMatchUpdateScore );
	UnLinkMsg( g_EventMinorMatchTeamScore );
	UnLinkMsg( g_EventMinorMatchStageFinish );
	UnLinkMsg( g_EventMinorMatchMatchFinish );
	UnLinkMsg( g_EventMajorMatchStateUpdate );
	UnLinkMsg( g_EventMajorMatchTeamInfo );
	UnLinkMsg( g_EventMajorMatchPlayerState );
	UnLinkMsg( g_EventMajorMatchUpdateScore );
	UnLinkMsg( g_EventMajorMatchStageFinish );
	UnLinkMsg( g_EventMajorMatchMatchFinish );
	UnLinkMsg( g_EventFinalMatchStateUpdate );
	UnLinkMsg( g_EventFinalMatchTeamInfo );
	UnLinkMsg( g_EventFinalMatchPlayerState );
	UnLinkMsg( g_EventFinalMatchUpdateScore );
	UnLinkMsg( g_EventFinalMatchStageFinish );
	UnLinkMsg( g_EventFinalMatchMatchFinish );

	UnLinkMsg( g_EventBudokaiStateInfoNfy );			
	UnLinkMsg( g_EventBudokaiUpdateStateNfy );			
	UnLinkMsg( g_EventBudokaiUpdateMatchStateNfy );		
	UnLinkMsg( g_EventBudokaiJoinIndividualRes );		
	UnLinkMsg( g_EventBudokaiLeaveIndividualRes );		
	UnLinkMsg( g_EventBudokaiJoinTeamInfoRes );			
	UnLinkMsg( g_EventBudokaiJoinTeamRes );				
	UnLinkMsg( g_EventBudokaiJoinTeamNfy );				
	UnLinkMsg( g_EventBudokaiLeaveTeamRes );			
	UnLinkMsg( g_EventBudokaiLeaveTeamNfy );			
	UnLinkMsg( g_EventBudokaiLeaveTeamMemberRes );		
	UnLinkMsg( g_EventBudokaiLeaveTeamMemberNfy );		
	UnLinkMsg( g_EventBudokaiJoinInfoRes );
	UnLinkMsg( g_EventBudokaiJoinStateNfy );
	UnLinkMsg( g_EventBudokaiJoinStateRes );

	UnLinkMsg( g_EventBudokaiProgressMessageNfy );

	UnLinkMsg( g_EventUpdateTick );
}

void CNtlTBudokai::HandleEvents(RWS::CMsg& msg)
{
	if(msg.Id == g_EventUpdateTick)
	{
		float fElapsed = *reinterpret_cast<float*>(msg.pData);

		m_fCurrentTime += fElapsed;
		
		// 천하제일 무도회 시간을 업데이트 해준다.
		if( m_sStateInfo.byState != BUDOKAI_STATE_JUNIOR_CLOSE ||
			m_sStateInfo.byState != BUDOKAI_STATE_CLOSE )
		{
			if( m_fCurrentTime < 1.0f )
				return;

			--m_sStateInfo.tmRemainTime;
			if( m_sStateInfo.tmRemainTime < 0 )
				m_sStateInfo.tmRemainTime = 0;

			--m_aMatchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].tmRemainTime;
			if( m_aMatchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].tmRemainTime < 0 )
				m_aMatchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].tmRemainTime = 0;
			
			--m_aMatchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].tmRemainTime;
			if( m_aMatchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].tmRemainTime < 0 )
				m_aMatchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].tmRemainTime = 0;

			m_fCurrentTime -= 1.0f;
		}
		else
		{
			m_fCurrentTime = 0.0f;
		}
	}
	else if( msg.Id == g_EventMinorMatchStateUpdate )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_MinorMatchStateUpdate( msg.pData );
	}
	else if( msg.Id == g_EventMinorMatchTeamInfo )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_MinorMatchTeamInfo( msg.pData );
	}
	else if( msg.Id == g_EventMinorMatchPlayerState )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_MinorMatchPlayerState( msg.pData );
	}
	else if( msg.Id == g_EventMinorMatchUpdateScore )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_MinorMatchUpdateScore( msg.pData );
	}
	else if( msg.Id == g_EventMinorMatchTeamScore )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World conept is valid" );

		pTBWorldConcept->EventHandler_MinorMatchTeamScore( msg.pData );
	}
	else if( msg.Id == g_EventMinorMatchStageFinish )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_MinorMatchStageFinish( msg.pData );
	}
	else if( msg.Id == g_EventMinorMatchMatchFinish )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_MinorMatchMatchFinish( msg.pData );
	}
	else if( msg.Id == g_EventMajorMatchStateUpdate )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_MajorMatchStateUpdate( msg.pData );
	}
	else if( msg.Id == g_EventMajorMatchTeamInfo )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_MajorMatchTeamInfo( msg.pData );
	}
	else if( msg.Id == g_EventMajorMatchPlayerState )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_MajorMatchPlayerState( msg.pData );
	}
	else if( msg.Id == g_EventMajorMatchUpdateScore )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_MajorMatchUpdateScore( msg.pData );
	}
	else if( msg.Id == g_EventMajorMatchStageFinish )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_MajorMatchStageFinish( msg.pData );
	}
	else if( msg.Id == g_EventMajorMatchMatchFinish )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_MajorMatchMatchFinish( msg.pData );
	}
	else if( msg.Id == g_EventFinalMatchStateUpdate )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_FinalMatchStateUpdate( msg.pData );
	}
	else if( msg.Id == g_EventFinalMatchTeamInfo )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_FinalMatchTeamInfo( msg.pData );
	}
	else if( msg.Id == g_EventFinalMatchPlayerState )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_FinalMatchPlayerState( msg.pData );
	}
	else if( msg.Id == g_EventFinalMatchUpdateScore )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_FinalMatchUpdateScore( msg.pData );
	}
	else if( msg.Id == g_EventFinalMatchStageFinish )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_FinalMatchStageFinish( msg.pData );
	}
	else if( msg.Id == g_EventFinalMatchMatchFinish )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		pTBWorldConcept->EventHandler_FinalMatchMatchFinish( msg.pData );
	}
	else if( msg.Id == g_EventBudokaiStateInfoNfy )
	{
		// 천하제일 무도회의 상태 ( 게임 접속 초기 )
		sNtlEventBudokaiStateInfoNfy* pNotify = reinterpret_cast<sNtlEventBudokaiStateInfoNfy*>(msg.pData);

		m_wSeasonCount = pNotify->wSeasonCount; 

		::CopyMemory( &m_sStateInfo, &pNotify->sStateInfo, sizeof( sBUDOKAI_UPDATE_STATE_INFO ) );
		::CopyMemory( m_aMatchStateInfo, pNotify->aMatchStateInfo, sizeof(sBUDOKAI_UPDATE_MATCH_STATE_INFO) * MAX_BUDOKAI_MATCH_TYPE );
		::CopyMemory( &m_sJoinInfo, &pNotify->sJoinInfo, sizeof(sBUDOKAI_JOIN_INFO) );
	}		
	else if( msg.Id == g_EventBudokaiUpdateStateNfy )
	{		
		// 천하제일 무도회 MainState 의 Update
		sNtlEventBudokaiUpdateStateNfy* pNotify = reinterpret_cast<sNtlEventBudokaiUpdateStateNfy*>( msg.pData );
		
		m_wSeasonCount = pNotify->sStateInfo.wSeasonCount;
		::CopyMemory( &m_sStateInfo, &pNotify->sStateInfo, sizeof( sBUDOKAI_UPDATE_STATE_INFO ) );

		if( m_sStateInfo.byState == BUDOKAI_STATE_JUNIOR_CLOSE ||
			m_sStateInfo.byState == BUDOKAI_STATE_CLOSE )
		{
			::ZeroMemory(&m_sStateInfo, sizeof( sBUDOKAI_UPDATE_STATE_INFO ) );
			::ZeroMemory(&m_aMatchStateInfo, sizeof( sBUDOKAI_UPDATE_MATCH_STATE_INFO ) * MAX_BUDOKAI_MATCH_TYPE );
			::ZeroMemory(&m_sJoinInfo, sizeof( sBUDOKAI_JOIN_INFO ) );

			m_sJoinInfo.byMatchType = INVALID_BUDOKAI_MATCH_TYPE;
			m_sJoinInfo.byJoinState	= INVALID_BUDOKAI_JOIN_STATE;
			m_sJoinInfo.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;
		}
	}		
	else if( msg.Id == g_EventBudokaiUpdateMatchStateNfy )
	{
		// 천하제일 무도회의 MatchState Update
		sNtlEventBudokaiUpdateMatchStateNfy* pNotify = reinterpret_cast<sNtlEventBudokaiUpdateMatchStateNfy*>( msg.pData );

		if( pNotify->byMatchType == INVALID_BUDOKAI_MATCH_TYPE )
			return;

		::CopyMemory( &m_aMatchStateInfo[ pNotify->byMatchType ] , &pNotify->sStateInfo, sizeof( sBUDOKAI_UPDATE_MATCH_STATE_INFO ) );
		
		// 천하제일 무도회 참가 신청한 상태이고 자신이 신청한 매치 타입이 등록기간 -> 예선전 대기 상태로 업데이트 되면
		// 서버로 자신이 예선전에 뽑혔는지 안 뽑혔는지 정보 확인을 요청한다.
		if( m_sJoinInfo.byJoinState == BUDOKAI_JOIN_STATE_PLAY )
		{
			if( m_sJoinInfo.byMatchType == pNotify->byMatchType )
			{
				API_GetSLPacketGenerator()->SendBudokaiJoinStateReq();
			}
		}
	}	
	else if( msg.Id == g_EventBudokaiJoinIndividualRes )
	{
		// 개인전 참가 결과
		SNtlEventBudokaiJoinIndividualRes* pResult = reinterpret_cast<SNtlEventBudokaiJoinIndividualRes*>( msg.pData );

		if( pResult->wResultCode == GAME_SUCCESS )
		{
			m_sJoinInfo.byMatchType = BUDOKAI_MATCH_TYPE_INDIVIDIAUL;		// 개인전
			m_sJoinInfo.byJoinResult = BUDOKAI_JOIN_RESULT_REGISTER;		// 등록자
			m_sJoinInfo.byJoinState = BUDOKAI_JOIN_STATE_PLAY;				// 참가중
			m_sJoinInfo.sIndividualInfo.wJoinId = pResult->wJoinId;			// 참가번호
			m_sJoinInfo.sIndividualInfo.bDojoRecommender = B2b(pResult->bDojoRecommender);	// 도장 추천자

			SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
			swprintf_s( m_sJoinInfo.sIndividualInfo.wszCharName, NTL_MAX_SIZE_CHAR_NAME + 1, L"%s", pAvatarInfo->sCharPf.awchName );
		}
	}	
	else if( msg.Id == g_EventBudokaiLeaveIndividualRes )
	{
		// 개인전 취소 결과
		SNtlEventBudokaiLeaveIndividualRes* pResult = reinterpret_cast<SNtlEventBudokaiLeaveIndividualRes*>( msg.pData );

		if( pResult->wResultCode == GAME_SUCCESS )
		{
			m_sJoinInfo.byMatchType = INVALID_BUDOKAI_MATCH_TYPE;		// 타입 모름
			m_sJoinInfo.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;		// 참가 결과 모름
			m_sJoinInfo.byJoinState = INVALID_BUDOKAI_JOIN_STATE;		// 참가 안함
			m_sJoinInfo.sIndividualInfo.wJoinId = INVALID_WORD;			// 접수 번호 없음

			SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
			swprintf_s( m_sJoinInfo.sIndividualInfo.wszCharName, NTL_MAX_SIZE_CHAR_NAME + 1, L"%s", pAvatarInfo->sCharPf.awchName );
		}
	}	
	else if( msg.Id == g_EventBudokaiJoinTeamInfoRes )
	{
		// 이 이벤트는 파티전을 신청할 때 신청서에 자신이 속한 파티의 정보를 출력하는데 사용된다.
		// ( 정보를 Simulation에 저장할 필요가 없다 )
	}		
	else if( msg.Id == g_EventBudokaiJoinTeamRes )
	{
		// 파티전 등록 결과
		SNtlEventBudokaiJoinTeamRes* pResult = reinterpret_cast<SNtlEventBudokaiJoinTeamRes*>( msg.pData );
		
		if( pResult->wResultCode == GAME_SUCCESS )
		{
			m_sJoinInfo.byMatchType = BUDOKAI_MATCH_TYPE_TEAM;			// 파티전
			m_sJoinInfo.byJoinResult = BUDOKAI_JOIN_RESULT_REGISTER;	// 등록중
			m_sJoinInfo.byJoinState = BUDOKAI_JOIN_STATE_PLAY;			// 참가중
			m_sJoinInfo.sTeamInfo.wJoinId = pResult->wJoinId;			// 참가번호

			// 팀 이름과 멤버들의 정보
			swprintf_s( m_sJoinInfo.sTeamInfo.wszTeamName, NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1, L"%s", pResult->wszTeamName );
			::CopyMemory( m_sJoinInfo.sTeamInfo.aMemberInfo, pResult->aTeamInfo, sizeof( sBUDOKAI_TEAM_POINT_INFO ) * pResult->byMemberCount );
		}
	}			
	else if( msg.Id == g_EventBudokaiJoinTeamNfy )
	{	
		// 파티전 등록 결과 ( 파티장이 아닌 다른 멤버들에게 전달되는 이벤트, 성공일때만 온다. )
		SNtlEventBudokaiJoinTeamNfy* pNotify = reinterpret_cast<SNtlEventBudokaiJoinTeamNfy*>( msg.pData );

		m_sJoinInfo.byMatchType = BUDOKAI_MATCH_TYPE_TEAM;				// 파티전
		m_sJoinInfo.byJoinResult = BUDOKAI_JOIN_RESULT_REGISTER;		// 등록중
		m_sJoinInfo.byJoinState = BUDOKAI_JOIN_STATE_PLAY;				// 참가중
		m_sJoinInfo.sTeamInfo.wJoinId = pNotify->wJoinId;				// 참가번호

		// 팀 이름과 멤버들의 정보
		swprintf_s( m_sJoinInfo.sTeamInfo.wszTeamName, NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1, L"%s", pNotify->wszTeamName );
		::CopyMemory( m_sJoinInfo.sTeamInfo.aMemberInfo, pNotify->aTeamInfo, sizeof( sBUDOKAI_TEAM_POINT_INFO ) * pNotify->byMemberCount );
	}			
	else if( msg.Id == g_EventBudokaiLeaveTeamRes )
	{
		// 파티전 전체 취소 결과 ( 파티장 )
		SNtlEventBudokaiLeaveTeamRes* pResult = reinterpret_cast< SNtlEventBudokaiLeaveTeamRes*>( msg.pData );

		if( pResult->wResultCode == GAME_SUCCESS )
		{
			m_sJoinInfo.byMatchType = INVALID_BUDOKAI_MATCH_TYPE;
			m_sJoinInfo.byJoinState = INVALID_BUDOKAI_JOIN_STATE;
			m_sJoinInfo.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;
			m_sJoinInfo.sTeamInfo.wJoinId = INVALID_WORD;

			::ZeroMemory( m_sJoinInfo.sTeamInfo.wszTeamName, sizeof( WCHAR ) * ( NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1 ) );
			::ZeroMemory( m_sJoinInfo.sTeamInfo.aMemberInfo, sizeof( sBUDOKAI_TEAM_POINT_INFO ) * NTL_MAX_MEMBER_IN_PARTY );
		}
	}		
	else if( msg.Id == g_EventBudokaiLeaveTeamNfy )
	{
		// 파티전 전체 취소 결과 ( 파티장 외의 멤버들 )

		m_sJoinInfo.byMatchType = INVALID_BUDOKAI_MATCH_TYPE;
		m_sJoinInfo.byJoinState = INVALID_BUDOKAI_JOIN_STATE;
		m_sJoinInfo.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;
		m_sJoinInfo.sTeamInfo.wJoinId = INVALID_WORD;

		::ZeroMemory( m_sJoinInfo.sTeamInfo.wszTeamName, sizeof( WCHAR ) * ( NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1 ) );
		::ZeroMemory( m_sJoinInfo.sTeamInfo.aMemberInfo, sizeof( sBUDOKAI_TEAM_POINT_INFO ) * NTL_MAX_MEMBER_IN_PARTY );
	}		
	else if( msg.Id == g_EventBudokaiLeaveTeamMemberRes )
	{
		// 멤버 취소 결과
		SNtlEventBudokaiLeaveTeamMemberRes* pResult = reinterpret_cast<SNtlEventBudokaiLeaveTeamMemberRes*>( msg.pData );

		if( pResult->wResultCode == GAME_SUCCESS )
		{
			m_sJoinInfo.byMatchType = INVALID_BUDOKAI_MATCH_TYPE;
			m_sJoinInfo.byJoinState = INVALID_BUDOKAI_JOIN_STATE;
			m_sJoinInfo.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;
			m_sJoinInfo.sTeamInfo.wJoinId = INVALID_WORD;

			::ZeroMemory( m_sJoinInfo.sTeamInfo.wszTeamName, sizeof( WCHAR ) * ( NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1 ) );
			::ZeroMemory( m_sJoinInfo.sTeamInfo.aMemberInfo, sizeof( sBUDOKAI_TEAM_POINT_INFO ) * NTL_MAX_MEMBER_IN_PARTY );
		}
	}	
	else if( msg.Id == g_EventBudokaiLeaveTeamMemberNfy )
	{
		// 멤버 취소 알림 ( 타 멤버들에게 )
		SNtlEventBudokaiLeaveTeamMemberNfy* pNotify = reinterpret_cast<SNtlEventBudokaiLeaveTeamMemberNfy*>( msg.pData );

		// 파티전일 경우만 유효하다.
		if( m_sJoinInfo.byMatchType != BUDOKAI_MATCH_TYPE_TEAM )
			return;

		// 이름이 맞는 파티 멤버를 찾아서 정보를 삭제해준다.
		for( RwInt32 i = 0; i < NTL_MAX_MEMBER_IN_PARTY; ++i )
		{
			if( wcscmp( m_sJoinInfo.sTeamInfo.aMemberInfo[i].wszName, pNotify->pwcMemberName ) == 0 )
			{
				::ZeroMemory( &m_sJoinInfo.sTeamInfo.aMemberInfo[i], sizeof( sBUDOKAI_TEAM_POINT_INFO ) );
				break;
			}
		}
	}	
	else if( msg.Id == g_EventBudokaiJoinInfoRes )
	{
	}
	else if( msg.Id == g_EventBudokaiJoinStateNfy )
	{
		SNtlEventBudokaiJoinStateNfy* pNotify = reinterpret_cast<SNtlEventBudokaiJoinStateNfy*>( msg.pData );

		m_sJoinInfo.byMatchType = pNotify->byMatchType;
		m_sJoinInfo.byJoinState = pNotify->byJoinState;
		m_sJoinInfo.byJoinResult = pNotify->byJoinResult;
	}
	else if( msg.Id == g_EventBudokaiJoinStateRes )
	{
		SNtlEventBudokaiJoinStateRes* pResult = reinterpret_cast<SNtlEventBudokaiJoinStateRes*>( msg.pData );

		if( pResult->wResultCode == GAME_SUCCESS )
		{
			m_sJoinInfo.byMatchType = pResult->byMatchType;
			m_sJoinInfo.byJoinState = pResult->byJoinState;
			m_sJoinInfo.byJoinResult = pResult->byJoinResult;
		}
	}
}



