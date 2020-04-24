#include "precomp_ntlsimulation.h"
#include "NtlSLTMQ.h"

// share
#include "TimeQuestTable.h"
#include "TableContainer.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSLLogic.h"
#include "ntlCameraController.h"


CNtlTMQ::CNtlTMQ()
{
}

CNtlTMQ::~CNtlTMQ()
{

}

RwBool CNtlTMQ::Create(void)
{
	NTL_FUNCTION("CNtlTMQ::Create");

	NtlSLCommunityAgent::Create(COMMUNITY_AGENT_PARTY);

	ClearBoardInfo();
	ClearRunningInfo();

	NTL_RETURN(TRUE);
}

void CNtlTMQ::Destroy(void)
{
	NTL_FUNCTION("CNtlTMQ::Destroy");

	ClearBoardInfo();
	ClearRunningInfo();

	NTL_RETURNVOID();
}

VOID CNtlTMQ::ClearBoardInfo()
{
	m_WaitInfo.pTMQTable					= NULL;
	m_WaitInfo.fRemainTime					= 0.f;
	m_WaitInfo.bTeleport					= FALSE;
	m_WaitInfo.hNPCHandle					= INVALID_SERIAL_ID;
}

VOID CNtlTMQ::ClearRunningInfo()
{
	m_RunningInfo.pTMQTable					= NULL;
	m_RunningInfo.eTimeQuestMode			= INVALID_TIMEQUEST_MODE;
	m_RunningInfo.byDifficultyFlag			= INVALID_TIMEQUEST_DIFFICULTY;	

	m_RunningInfo.byStage					= (RwUInt8)(dINVALID_STAGE);
	m_RunningInfo.fLimitedTime				= 0.f;
	m_RunningInfo.bUpdateTime				= FALSE;
	
	m_RunningInfo.fLastDayRecord			= 0.f;
	m_RunningInfo.uiClearTime				= 0;
	m_RunningInfo.uiBonusTime				= 0;
	m_RunningInfo.bDayRecord				= FALSE;
	m_RunningInfo.bBestRecord				= FALSE;

	Clear();
}

VOID CNtlTMQ::Update(RwReal fElapsed)
{
	if(m_WaitInfo.fRemainTime > 0.f)
	{
		m_WaitInfo.fRemainTime -= fElapsed;

		if( m_WaitInfo.fRemainTime < 0.f )
			m_WaitInfo.fRemainTime = 0.f;
	}

	if( m_RunningInfo.fLimitedTime > 0.f && m_RunningInfo.bUpdateTime)
	{
		m_RunningInfo.fLimitedTime -= fElapsed;

		if( m_RunningInfo.fLimitedTime < 0.f )
			m_RunningInfo.fLimitedTime = 0.f;
	}
}

RwUInt8 CNtlTMQ::GetNearDifficult(TBLIDX tmqTblidx)
{		
	sTIMEQUEST_TBLDAT* pTIMEQUEST_TBLDAT = reinterpret_cast<sTIMEQUEST_TBLDAT*>( API_GetTableContainer()->GetTimeQuestTable()->FindData(tmqTblidx) );
	if( !pTIMEQUEST_TBLDAT )
	{
		DBO_FAIL("Not exist tmq table of index : " << tmqTblidx);
		return INVALID_TIMEQUEST_DIFFICULTY;
	}
	
	sTIMEQUEST_DATASET* pDATASET_EASY	= &pTIMEQUEST_TBLDAT->sTimeQuestDataset[TIMEQUEST_DIFFICULTY_EASY];
	sTIMEQUEST_DATASET* pDATASET_NORMAL	= &pTIMEQUEST_TBLDAT->sTimeQuestDataset[TIMEQUEST_DIFFICULTY_NORMAL];
	sTIMEQUEST_DATASET* pDATASET_HARD	= &pTIMEQUEST_TBLDAT->sTimeQuestDataset[TIMEQUEST_DIFFICULTY_HARD];
	struct sLeastLevel
	{
		RwUInt8 byLeasyLevel;
		RwUInt8 byDifficult;
	};
	std::map<RwUInt8, RwUInt8> mapDifficult;

	mapDifficult[pDATASET_EASY->byMinMemberLevel]	= TIMEQUEST_DIFFICULTY_EASY;
	mapDifficult[pDATASET_EASY->byMaxMemberLevel]	= TIMEQUEST_DIFFICULTY_EASY;
	mapDifficult[pDATASET_NORMAL->byMinMemberLevel] = TIMEQUEST_DIFFICULTY_NORMAL;
	mapDifficult[pDATASET_NORMAL->byMaxMemberLevel] = TIMEQUEST_DIFFICULTY_NORMAL;
	mapDifficult[pDATASET_HARD->byMinMemberLevel]	= TIMEQUEST_DIFFICULTY_HARD;
	mapDifficult[pDATASET_HARD->byMaxMemberLevel]	= TIMEQUEST_DIFFICULTY_HARD;
	
	RwUInt8 byLevelGap;
	sLeastLevel leastDifficult;
	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr());
	RwUInt8 byAvatarLevel = pAvatarAttr->GetLevel();
	std::map<RwUInt8, RwUInt8>::iterator it = mapDifficult.begin();

	leastDifficult.byLeasyLevel	= 255;
	leastDifficult.byDifficult	= INVALID_TIMEQUEST_DIFFICULTY;

	for( ; it != mapDifficult.end() ; ++it )
	{
		byLevelGap = (RwUInt8)( abs(it->first - byAvatarLevel) );
		if( byLevelGap < leastDifficult.byLeasyLevel )
		{
			leastDifficult.byLeasyLevel = byLevelGap;
			leastDifficult.byDifficult = it->second;
		}
	}
	
	return leastDifficult.byDifficult;
}

VOID CNtlTMQ::HandleEvents(RWS::CMsg &pMsg)
{	
	NTL_FUNCTION("CNtlTMQ::HandleEvents");

	if( pMsg.Id == g_EventTMQ )
	{
		SNtlEventTMQ* pEvent = reinterpret_cast<SNtlEventTMQ*>( pMsg.pData );

		switch(pEvent->uiEventType)
		{
		case TET_REWARD:
			{
				CNtlSLEventGenerator::SLTMQNotify(TET_REWARD, pEvent->uiValue);
				break;
			}
		case TBT_COUNTDOWN:
			{
				m_RunningInfo.bUpdateTime = pEvent->uiValue;
				CNtlSLEventGenerator::SLTMQNotify(TBT_COUNTDOWN, pEvent->uiValue);
				break;
			}
		case TET_COUPON_COUNT:
			{
				CNtlSLEventGenerator::SLTMQNotify(TET_COUPON_COUNT, pEvent->uiValue);
				break;
			}
		case TET_TMQ_POINT:
			{
				CNtlSLEventGenerator::SLTMQNotify(TET_TMQ_POINT, pEvent->uiValue);
				break;
			}
		case TET_GAME_TIME:
			{
				CNtlSLEventGenerator::SLTMQNotify(TET_GAME_TIME, pEvent->uiValue);
				break;
			}
		case TET_CLEAR_TIME:
			{
				m_RunningInfo.uiClearTime	= pEvent->uiValue/1000;
				break;
			}
		case TET_BONUS_TIME:
			{
				m_RunningInfo.uiBonusTime	= pEvent->uiValue/1000;
					break;
			}
		case TET_DAY_RECORD:
			{
				m_RunningInfo.bDayRecord	= pEvent->uiValue;
					break;
			}
		case TET_IS_BEST_RECORD:
			{
				m_RunningInfo.bBestRecord	= pEvent->uiValue;
					break;
			}
		}
	}
	else if( pMsg.Id == g_EventTMQCleintState )
	{
		SNtlEventTMQServerState* pEvent = reinterpret_cast<SNtlEventTMQServerState*>( pMsg.pData );

		if( pEvent->eWorldConcept != WORLD_PLAY_TIME_MACHINE )
			NTL_RETURNVOID();

		if( pEvent->uiState == WORLD_STATE_TMQ_WAIT || pEvent->uiState == WORLD_STATE_TMQ_TERRAIN_READY)
		{
			sGAME_RULE_INFO& rGameRuleInfo = GetNtlSLGlobal()->GetAvatarInfo()->sWorldInfo.sRuleInfo;

			if( GAMERULE_TIMEQUEST != rGameRuleInfo.byRuleType )
			{
				DBO_FAIL("wrong world rule type : " << rGameRuleInfo.byRuleType);
				NTL_RETURNVOID();
			}
			
			sTIMEQUEST_RULE_INFO& rTimeQuestRuleInfo = rGameRuleInfo.sTimeQuestRuleInfo;

			m_RunningInfo.pTMQTable			= reinterpret_cast<sTIMEQUEST_TBLDAT*>( API_GetTableContainer()->GetTimeQuestTable()->FindData(rTimeQuestRuleInfo.timeQuestTblidx) );
			m_RunningInfo.eTimeQuestMode	= (eTIMEQUEST_MODE)rTimeQuestRuleInfo.byTimeQuestMode;
			m_RunningInfo.byDifficultyFlag	= (eTIMEQUEST_DIFFICULTY)rTimeQuestRuleInfo.byDifficulty;
			m_RunningInfo.fLastDayRecord	= 0.f;// (RwReal)rTimeQuestRuleInfo.dwDayRecord / 1000.f;//밀리세컨드 단위로 온다 //by daneos
			m_RunningInfo.fLimitedTime		= 0.f;// (RwReal)rTimeQuestRuleInfo.dwLimitTime; //by daneos
			m_RunningInfo.bUpdateTime		= rTimeQuestRuleInfo.bCountDown;

			// 서버쪽에서 기록이 매일 갱신된다
			//by daneos
			//if( rTimeQuestRuleInfo.dwDayRecord == INVALID_SERIAL_ID )
			//	m_RunningInfo.fLastDayRecord = 0.f;

			SetAgentType(NONE_COMMUNITY_AGENT);
		}
	}
	else if( pMsg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( pMsg.pData );

		if( pEvent->eWorldConcept != WORLD_PLAY_TIME_MACHINE )
			NTL_RETURNVOID();

		if( pEvent->uiState != WORLD_STATE_EXIT )
			NTL_RETURNVOID();

		ClearRunningInfo();
	}

	NTL_RETURNVOID();
}