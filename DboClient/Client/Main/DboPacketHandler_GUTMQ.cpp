/*****************************************************************************
* File			: DboPackethandler_GUTMQ.cpp
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2007. 1. 16
* Abstract		: Time Machine Quest 관련 패킷 핸들
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// simulation
#include "NtlNetSender.h"
#include "NtlSLPacketGenerator.h"
#include "NtlWorldConceptTutorial.h"
#include "NtlWorldConceptTMQ.h"

// dbo
#include "DboTSCTAgency.h"
#include "DboTSCQAgency.h"

#include "DirectionLinkTable.h"
#include "NtlSLLuaGlue.h"
#include "NtlLuaState.h"
#include "TimeQuestTable.h"



void PacketHandler_GUTimeQuest_Enter_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_TIMEQUEST_ENTER_RES);

	sGU_TIMEQUEST_ENTER_RES* pResult = (sGU_TIMEQUEST_ENTER_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TIMEQUEST_ENTER_RES"), TRUE);
		return;
	}

	// Event
	CNtlSLEventGenerator::SLTMQEnterRes();
}

void PacketHandler_GUTimeQuest_EnterDirection_Nfy(void * pPacket)
{
	sGU_TIMEQUEST_ENTER_DIRECTION_NFY* pResult = (sGU_TIMEQUEST_ENTER_DIRECTION_NFY*)pPacket;

	// play direction
	sTIMEQUEST_TBLDAT* pTIMEQUEST_TBLDAT = reinterpret_cast<sTIMEQUEST_TBLDAT*>(API_GetTableContainer()->GetTimeQuestTable()->FindData(pResult->tmqTblidx));
	if (pTIMEQUEST_TBLDAT == NULL)
		return;

	CDirectionLinkTable* pLinkTbl = API_GetTableContainer()->GetDirectionLinkTable();
	if (pLinkTbl == NULL)
		return;

	sDIRECTION_LINK_TBLDAT* pLinkTblData = reinterpret_cast<sDIRECTION_LINK_TBLDAT*>(pLinkTbl->FindData(pTIMEQUEST_TBLDAT->startCharacterDirection));
	if (pLinkTblData == NULL)
		return;

	DBO_WARNING_MESSAGE("pTIMEQUEST_TBLDAT->startCharacterDirection: " << pTIMEQUEST_TBLDAT->startCharacterDirection);
	DBO_WARNING_MESSAGE("pLinkTblData->szNote: " << pLinkTblData->szNote);

	std::string strFunc = pLinkTblData->szFunctionName;
	strFunc += "()";
	DBO_WARNING_MESSAGE("FUNC: " << strFunc.c_str());
	CNtlLuaState *pLuaState = GetLuaState();
	pLuaState->RunString(strFunc.c_str());
}

void PacketHandler_GUTimeQuest_Start_Nfy(void *pPacket)
{
	sGU_TIMEQUEST_START_NFY* pResult = (sGU_TIMEQUEST_START_NFY*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TIMEQUEST_START_NFY"), TRUE);
		return;
	}
}

void PacketHandler_GUTimeQuest_Start_Exception_Nfy(void *pPacket)
{
	sGU_TIMEQUEST_START_EXCEPTION_NFY* pResult = (sGU_TIMEQUEST_START_EXCEPTION_NFY*)pPacket;

	GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TIMEQUEST_START_EXCEPTION_NFY"), TRUE);
}

void PacketHandler_GUTimeQuest_Leave_Nfy(void *pPacket)
{
	sGU_TIMEQUEST_LEAVE_NFY* pResult = (sGU_TIMEQUEST_LEAVE_NFY*)pPacket;

	if( pResult->wResultCode == GAME_SUCCESS )
		return;

	if( pResult->handle == INVALID_SERIAL_ID )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TIMEQUEST_LEAVE_NFY"), TRUE);
	}
	else
	{
		CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
		NTL_ASSERT(pTMQ->GetTMQMode() == GAMERULE_TIMEQUEST, "PacketHandler_GUTimeQuest_Leave_Nfy, wrong world rule type : " << pTMQ->GetTMQMode());
		sPartyMember* pSM_Member = reinterpret_cast<sPartyMember*>( pTMQ->GetMemberbyKey(pResult->handle) );
		NTL_ASSERT(pSM_Member, "PacketHandler_GUTimeQuest_Leave_Nfy, not exist member of handle : " << pResult->handle);

		if( pSM_Member )
			GetAlarmManager()->FormattedAlarmMessage("DST_TMQ_BALLOON_LEAVE_MEMVER", FALSE, NULL, pSM_Member->wszMemberName);
	}
}

void PacketHandler_GUTimeQuest_Reward_Nfy(void *pPacket)
{
	sGU_TIMEQUEST_REWARD_NFY* pResult = (sGU_TIMEQUEST_REWARD_NFY*)pPacket;

	// TMQ 정보 갱신
	CNtlSLEventGenerator::SLTMQEvent(TET_DAY_RECORD, INVALID_SERIAL_ID, pResult->bIsDayRecord);
	CNtlSLEventGenerator::SLTMQEvent(TET_IS_BEST_RECORD, INVALID_SERIAL_ID, pResult->bIsBestRecord);

	CNtlSLEventGenerator::SLTMQNotify(TET_TIME_UNTIL_TELEPORT_TO_MAINWORLD, pResult->dwWaitTime);

	// 트리거 실행
	switch ( pResult->byTriggerType )
	{
		case TS_TYPE_QUEST_CS:
		{
			CDboTSCQAgency* pQuestAgency = GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency();
			if ( pQuestAgency )
				pQuestAgency->GU_TS_Update_Event_Nfy( pResult->teid );
		}
		break;

		case TS_TYPE_PC_TRIGGER_CS:
		{
			CDboTSCTAgency* pTriggerAgency = GetNtlSLGlobal()->GetSobAvatar()->GetPCTriggerAgency();
			if ( pTriggerAgency )
				pTriggerAgency->GU_TS_Update_Event_Nfy( pResult->teid );
		}
		break;

	default:
		{
			NTL_ASSERTFAIL( "Not supported trigger type" );
		}
	}
}

void PacketHandler_GUTimeQuest_Infomation_Nfy(void *pPacket)
{
	sGU_TIMEQUEST_INFORMATION_NFY* pResult = (sGU_TIMEQUEST_INFORMATION_NFY*)pPacket;

	CNtlSLEventGenerator::SLTMQNotify(TET_INFOMATION_NFY, pResult->byInfoType);
}

void PacketHandler_GUTimeQuest_Countdown_Nfy(void *pPacket)
{
	sGU_TIMEQUEST_COUNTDOWN_NFY* pResult = (sGU_TIMEQUEST_COUNTDOWN_NFY*)pPacket;

	CNtlSLEventGenerator::SLTMQEvent(TBT_COUNTDOWN, INVALID_SERIAL_ID, pResult->bCountDown);
}

void PacketHandler_GUTimeQuest_GameState(void *pPacket)
{
	sGU_TIMEQUEST_UPDATE_GAME_STATE* pResult = (sGU_TIMEQUEST_UPDATE_GAME_STATE*)pPacket;

	CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);
	NTL_ASSERT(pWorldConcept, "PacketHandler_GUTimeQuest_GameState, Wrong first grade world concept");

	if( pWorldConcept->GetConceptType() == WORLD_PLAY_TIME_MACHINE )
	{
		CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
		CNtlWorldConceptTMQ* pTMQConcept = reinterpret_cast<CNtlWorldConceptTMQ*>(pWorldConcept);

		pTMQ->SetStage(pResult->sTimeQuestState.sTimeQuestStage.byStageNumber);
		pTMQConcept->SetTMQState((eTIMEQUEST_GAME_STATE)pResult->sTimeQuestState.byGameState);
	}
	else if( pWorldConcept->GetConceptType() == WORLD_PLAY_TUTORIAL )
	{
		CNtlWorldConceptTutorial* pTutorialConcept = reinterpret_cast<CNtlWorldConceptTutorial*>(pWorldConcept);

		pTutorialConcept->SetTMQState((eTIMEQUEST_GAME_STATE)pResult->sTimeQuestState.byGameState);
	}
	else
	{
		NTL_ASSERT(false, "PacketHandler_GUTimeQuest_GameState, wrong world concept : " << pWorldConcept->GetConceptType());
	}
}

void PacketHandler_GUTimeQuest_CouponCount(void *pPacket)
{
	// TMQ 쿠폰 정보
	sGU_TIMEQUEST_UPDATE_COUPON_COUNT* pResult = (sGU_TIMEQUEST_UPDATE_COUPON_COUNT*)pPacket;

	CNtlSLEventGenerator::SLTMQEvent(TET_COUPON_COUNT, INVALID_SERIAL_ID, pResult->byCouponCount);
}

void PacketHandler_GUTimeQuest_TMQPoint(void *pPacket)
{
	// TMQ 포인트
	sGU_TIMEQUEST_UPDATE_TMQ_POINT* pResult = (sGU_TIMEQUEST_UPDATE_TMQ_POINT*)pPacket;

	CNtlSLEventGenerator::SLTMQEvent(TET_TMQ_POINT, INVALID_SERIAL_ID, pResult->wTmqPoint);
}

void PacketHandler_GUTimeQuest_GameTime(void *pPacket)
{
	sGU_TIMEQUEST_UPDATE_GAME_TIME* pResult = (sGU_TIMEQUEST_UPDATE_GAME_TIME*)pPacket;

	CNtlSLEventGenerator::SLTMQEvent(TET_GAME_TIME, INVALID_SERIAL_ID, pResult->dwBonusTime);
}