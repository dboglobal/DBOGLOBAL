#include "stdafx.h"
#include "BudokaiManager.h"
#include "GameServer.h"
#include "TableContainerManager.h"
#include "NtlPacketGU.h"
#include "NtlPacketGQ.h"
#include "NtlPacketGT.h"
#include "NtlResultCode.h"
#include "ObjectManager.h"
#include "CPlayer.h"
#include "DojoManager.h"
#include "GameMain.h"
#include "DiceManager.h"


struct sprelim_sorting
{
	inline bool operator() (const std::pair<JOINID, sBUDOKAI_REGISTER_INDIVIDUAL_DATA> a, const std::pair<JOINID, sBUDOKAI_REGISTER_INDIVIDUAL_DATA> b)
	{
		return (a.second.fPoint > b.second.fPoint);
	}
};


CBudokaiManager::CBudokaiManager()
{
	Init();
}

CBudokaiManager::~CBudokaiManager()
{
	Destroy();
}


void CBudokaiManager::StartSoloAdultBudokai()
{
	CGameServer* app = (CGameServer*)g_pApp;
	m_bAdultBudokaiBegan = true;

	CreateBudokai(BUDOKAI_TYPE_ADULT, BUDOKAI_MATCH_TYPE_INDIVIDIAUL, (BUDOKAITIME)app->GetTime(), m_pTableInfo);
}

void CBudokaiManager::StartSoloJuniorBudokai()
{
	CGameServer* app = (CGameServer*)g_pApp;
	m_bJuniorBudokaiBegan = true;

	CreateBudokai(BUDOKAI_TYPE_JUNIOR, BUDOKAI_MATCH_TYPE_INDIVIDIAUL, (BUDOKAITIME)app->GetTime(), m_pTableInfo);
}

void CBudokaiManager::StartPartyAdultBudokai()
{
	CGameServer* app = (CGameServer*)g_pApp;
	m_bPartyAdultBudokaiBegan = true;

	CreateBudokai(BUDOKAI_TYPE_ADULT, BUDOKAI_MATCH_TYPE_TEAM, (BUDOKAITIME)app->GetTime(), m_pTableInfo);
}

void CBudokaiManager::StartPartyJuniorBudokai()
{
	CGameServer* app = (CGameServer*)g_pApp;
	m_bPartyJuniorBudokaiBegan = true;

	CreateBudokai(BUDOKAI_TYPE_JUNIOR, BUDOKAI_MATCH_TYPE_TEAM, (BUDOKAITIME)app->GetTime(), m_pTableInfo);
}


CBudokaiManager::sPRELIM_HEAD::~sPRELIM_HEAD()
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (m_pWorld)
	{
		ERR_LOG(LOG_GENERAL, "BUDOKAI: Destroy Prelim-World ID %u \n", m_pWorld->GetID());
		app->GetGameMain()->GetWorldManager()->DestroyWorld(m_pWorld);
	}
}


CBudokaiManager::sTOURNAMENT_MATCH::~sTOURNAMENT_MATCH()
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (m_pWorld)
	{
		ERR_LOG(LOG_GENERAL, "BUDOKAI: Destroy Tournament-World ID %u \n", m_pWorld->GetID());
		app->GetGameMain()->GetWorldManager()->DestroyWorld(m_pWorld);
	}
}


void CBudokaiManager::Init()
{
	CGameServer* app = (CGameServer*)g_pApp;

	BUDOKAITIME curTime = (BUDOKAITIME)app->GetTime();

	m_dwTick = 0;

	m_bAdultBudokaiBegan = false;
	m_bJuniorBudokaiBegan = false;
	m_bPartyAdultBudokaiBegan = false;
	m_bPartyJuniorBudokaiBegan = false;

	memset(&m_stateInfo, NULL, sizeof(m_stateInfo));
	
	m_matchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].byState = INVALID_BUDOKAI_MATCHSTATE;
	m_matchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].tmNextStepTime = 0;
	m_matchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].tmRemainTime = 0;
	m_matchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].byState = INVALID_BUDOKAI_MATCHSTATE;
	m_matchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].tmNextStepTime = 0;
	m_matchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].tmRemainTime = 0;

	m_matchType = INVALID_BUDOKAI_MATCH_TYPE;

	m_type = INVALID_BUDOKAI_TYPE;

	m_pTableInfo = g_pTableContainer->GetBudokaiTable()->GetBudokaiTblInfo();

	/*printf("m_pTableInfo->byOpenTerm %u, m_pTableInfo->byOpenDayOfWeek %u, m_pTableInfo->byOpenHour %u, m_pTableInfo->byOpenMinute %u \n", 
		m_pTableInfo->byOpenTerm, m_pTableInfo->byOpenDayOfWeek, m_pTableInfo->byOpenHour, m_pTableInfo->byOpenMinute);

	printf("m_pTableInfo->byJiniorOpenTerm %u, m_pTableInfo->byJiniorOpenDayOfWeek %u, m_pTableInfo->byJiniorOpenHour %u, m_pTableInfo->byJiniorOpenMinute %u \n",
		m_pTableInfo->byJiniorOpenTerm, m_pTableInfo->byJiniorOpenDayOfWeek, m_pTableInfo->byJiniorOpenHour, m_pTableInfo->byJiniorOpenMinute);*/

	m_startAdultBudokaiTime = INVALID_BUDOKAITIME;// CalcBudokaiNextStartTime(curTime, m_pTableInfo->byOpenTerm, m_pTableInfo->byOpenDayOfWeek, m_pTableInfo->byOpenHour, m_pTableInfo->byOpenMinute);
	m_startJuniorBudokaiTime = INVALID_BUDOKAITIME;// CalcBudokaiNextStartTime(curTime, m_pTableInfo->byOpenTerm, m_pTableInfo->byOpenDayOfWeek, m_pTableInfo->byOpenHour, m_pTableInfo->byOpenMinute);
	m_startPartyAdultBudokaiTime = INVALID_BUDOKAITIME;// CalcBudokaiNextStartTime(curTime, m_pTableInfo->byJiniorOpenTerm, m_pTableInfo->byJiniorOpenDayOfWeek, m_pTableInfo->byJiniorOpenHour, m_pTableInfo->byJiniorOpenMinute);
	m_startPartyJuniorBudokaiTime = INVALID_BUDOKAITIME;// CalcBudokaiNextStartTime(curTime, m_pTableInfo->byJiniorOpenTerm, m_pTableInfo->byJiniorOpenDayOfWeek - 1, m_pTableInfo->byJiniorOpenHour, m_pTableInfo->byJiniorOpenMinute);
	//printf("curtime %u, m_startJuniorBudokaiTime %u, m_startPartyAdultBudokaiTime %u, m_startPartyJuniorBudokaiTime %u \n", curTime, m_startJuniorBudokaiTime, m_startPartyAdultBudokaiTime, m_startPartyJuniorBudokaiTime);

	m_byDojoRecommenders = 0;
	m_eMatchDepth = INVALID_BUDOKAI_MATCH_DEPTH;
}

void CBudokaiManager::Destroy()
{
	if (m_bAdultBudokaiBegan)
	{
		m_bAdultBudokaiBegan = false;
	}

	if (m_bJuniorBudokaiBegan)
	{
		m_bJuniorBudokaiBegan = false;
	}

	if (m_bPartyAdultBudokaiBegan)
	{
		m_bPartyAdultBudokaiBegan = false;
	}

	if (m_bPartyJuniorBudokaiBegan)
	{
		m_bPartyJuniorBudokaiBegan = false;
	}
}

void CBudokaiManager::RecvInitInfo(WORD wSeasonCount, BUDOKAITIME tmDefaultOpenTime)
{
	m_stateInfo.wSeasonCount = wSeasonCount;
	m_tmDefaultOpenTime = tmDefaultOpenTime;
}

void CBudokaiManager::TickProcess(DWORD dwTickDiff)
{
	CGameServer* app = (CGameServer*)g_pApp;

	DBOTIME curTime = app->GetTime();

	if (m_stateInfo.byState != BUDOKAI_STATE_JUNIOR_CLOSE || m_stateInfo.byState != BUDOKAI_STATE_CLOSE)
	{
		m_dwTick += dwTickDiff;

		if (m_dwTick >= 1000)
		{
			UnsignedSafeDecrease<DWORD>(m_stateInfo.tmRemainTime, 1);
			UnsignedSafeDecrease<DWORD>(m_matchStateInfo[m_matchType].tmRemainTime, 1);

			m_dwTick = 0;
		}
	}

	tm timeStruct = {};
	localtime_s(&timeStruct, &curTime);

	// -- ADULT SOLO BUDOKAI
	if (m_bAdultBudokaiBegan == false)
	{
		if ((timeStruct.tm_wday == 6 && timeStruct.tm_hour == 18)//check if its saturday 17 o clock
			|| (timeStruct.tm_wday == 4 && timeStruct.tm_hour == 23)) //check if its thursday 0 o clock
		{
			//start adult solo budokai
			m_bAdultBudokaiBegan = true;

			CreateBudokai(BUDOKAI_TYPE_ADULT, BUDOKAI_MATCH_TYPE_INDIVIDIAUL, (BUDOKAITIME)curTime, m_pTableInfo);
		}
	}
	else
	{
		if (TickProcessBudokai(dwTickDiff, (BUDOKAITIME)curTime) == true) //if true, then end the budokai
		{
			//end budokai
			ERR_LOG(LOG_GENERAL, "BUDOKAI: End Adult-Solo Budokai");
			m_bAdultBudokaiBegan = false;
		}
	}


	// -- JUNIOR SOLO BUDOKAI
	if (m_bJuniorBudokaiBegan == false)
	{
		if (timeStruct.tm_wday == 2 && timeStruct.tm_hour == 14) //check if its tuesday 14 o clock
		{
			//start junior solo budokai
			m_bJuniorBudokaiBegan = true;
			
			CreateBudokai(BUDOKAI_TYPE_JUNIOR, BUDOKAI_MATCH_TYPE_INDIVIDIAUL, (BUDOKAITIME)curTime, m_pTableInfo);
		}
	}
	else
	{
		if (TickProcessBudokai(dwTickDiff, (BUDOKAITIME)curTime) == true) //if true, then end the budokai
		{
			//end budokai
			ERR_LOG(LOG_GENERAL, "BUDOKAI: End Junior-Solo Budokai");
			m_bJuniorBudokaiBegan = false;
		}
	}

	// -- ADULT PARTY BUDOKAI
	if (m_bPartyAdultBudokaiBegan == false)
	{
		if (timeStruct.tm_wday == 0 && timeStruct.tm_hour == 13) //check if its sunday 13 o clock
		{
			//start adult party budokai
			m_bPartyAdultBudokaiBegan = true;
			CreateBudokai(BUDOKAI_TYPE_ADULT, BUDOKAI_MATCH_TYPE_TEAM, (BUDOKAITIME)curTime, m_pTableInfo);
		}
	}
	else 
	{
		if (TickProcessBudokai(dwTickDiff, (BUDOKAITIME)curTime) == true) //if true, then end the budokai
		{
			//end budokai
			ERR_LOG(LOG_GENERAL, "BUDOKAI: End Adult-Party Budokai");
			m_bPartyAdultBudokaiBegan = false;
		}
	}

	// -- JUNIOR PARTY BUDOKAI
	if (m_bPartyJuniorBudokaiBegan == false)
	{
		if (timeStruct.tm_wday == 5 && timeStruct.tm_hour == 17) //check if its friday 17 o clock
		{
			//start junior party budokai
			m_bPartyJuniorBudokaiBegan = true;
			CreateBudokai(BUDOKAI_TYPE_JUNIOR, BUDOKAI_MATCH_TYPE_TEAM, (BUDOKAITIME)curTime, m_pTableInfo);
		}
	}
	else
	{
		if (TickProcessBudokai(dwTickDiff, (BUDOKAITIME)curTime) == true) //if true, then end the budokai
		{
			//end budokai
			ERR_LOG(LOG_GENERAL, "BUDOKAI: End Junior-Party Budokai");
			m_bPartyJuniorBudokaiBegan = false;
		}
	}
}

void CBudokaiManager::UpdateChannelStateInfo(BUDOKAITIME tmDefaultOpenTime, bool bRankbattleAllow, sBUDOKAI_UPDATE_STATE_INFO & sStateInfo, sBUDOKAI_UPDATE_MATCH_STATE_INFO * aMatchStateInfo)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (app->m_config.byChannel == DOJO_CHANNEL_INDEX)
	{

	}
	else
	{
		ERR_LOG(LOG_GENERAL, "BUDOKAI: Receive Budokai began Info");

		m_tmDefaultOpenTime = tmDefaultOpenTime;
		m_bRankbattleAllow = bRankbattleAllow;

		memcpy(&m_stateInfo, &sStateInfo, sizeof(sBUDOKAI_UPDATE_STATE_INFO));
		memcpy(&m_matchStateInfo, aMatchStateInfo, sizeof(m_matchStateInfo));

		m_type = GetBudokaiType(sStateInfo.byState);

		if (aMatchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].tmNextStepTime == INVALID_BUDOKAITIME) //only the budokai we started has INVALID_BUDOKAITIME. All others are 0
		{
			m_matchType = BUDOKAI_MATCH_TYPE_INDIVIDIAUL;

			if (m_type == BUDOKAI_TYPE_JUNIOR)
				m_bJuniorBudokaiBegan = true;
			else
				m_bAdultBudokaiBegan = true;
		}
		else
		{
			m_matchType = BUDOKAI_MATCH_TYPE_TEAM;

			if (m_type == BUDOKAI_TYPE_JUNIOR)
				m_bPartyJuniorBudokaiBegan = true;
			else
				m_bPartyAdultBudokaiBegan = true;
		}

		//send budokai state. It will create an icon
		CNtlPacket packet(sizeof(sGU_BUDOKAI_STATE_INFO_NFY));
		sGU_BUDOKAI_STATE_INFO_NFY * res = (sGU_BUDOKAI_STATE_INFO_NFY *)packet.GetPacketData();
		res->wOpCode = GU_BUDOKAI_STATE_INFO_NFY;
		res->tmDefaultOpenTime = tmDefaultOpenTime;
		res->bRankbattleAllow = bRankbattleAllow;
		memcpy(&res->sStateInfo, &sStateInfo, sizeof(sBUDOKAI_UPDATE_STATE_INFO));
		memcpy(res->aMatchStateInfo, aMatchStateInfo, sizeof(res->aMatchStateInfo));

		//set these invalid because we just started budokai.. When player login, then we will set these data correctly.
		res->sJoinInfo.byJoinState = INVALID_BUDOKAI_JOIN_STATE;
		res->sJoinInfo.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;
		res->sJoinInfo.byMatchType = INVALID_BUDOKAI_MATCH_TYPE;

		packet.SetPacketLen(sizeof(sGU_BUDOKAI_STATE_INFO_NFY));
		g_pObjectManager->SendPacketToAll(&packet);


		//send announcement that budokai started
		CNtlPacket packet2(sizeof(sGU_BUDOKAI_PROGRESS_MESSAGE_NFY));
		sGU_BUDOKAI_PROGRESS_MESSAGE_NFY * res2 = (sGU_BUDOKAI_PROGRESS_MESSAGE_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_BUDOKAI_PROGRESS_MESSAGE_NFY;
		res2->byMsgId = BUDOKAI_PROGRESS_MESSAGE_START;
		packet2.SetPacketLen(sizeof(sGU_BUDOKAI_PROGRESS_MESSAGE_NFY));
		g_pObjectManager->SendPacketToAll(&packet2);
	}
}

void CBudokaiManager::UpdateChannelState(BUDOKAITIME tmDefaultOpenTime, bool bRankbattleAllow, sBUDOKAI_UPDATE_STATE_INFO & sStateInfo)
{
	CGameServer* app = (CGameServer*)g_pApp;

	// TODO: decide what we do with tmDefaultOpenTime

	if (app->m_config.byChannel == DOJO_CHANNEL_INDEX)
	{

	}
	else
	{
		ERR_LOG(LOG_GENERAL, "BUDOKAI: Receive channel state update. byState = %u, tmNextStepTime = %u, tmRemainTime = %u, wSeasonCount = %u", sStateInfo.byState, sStateInfo.tmNextStepTime, sStateInfo.tmRemainTime, sStateInfo.wSeasonCount);

		m_tmDefaultOpenTime = tmDefaultOpenTime;
		m_bRankbattleAllow = bRankbattleAllow;
		memcpy(&m_stateInfo, &sStateInfo, sizeof(sBUDOKAI_UPDATE_STATE_INFO));

		CNtlPacket packet(sizeof(sGU_BUDOKAI_UPDATE_STATE_NFY));
		sGU_BUDOKAI_UPDATE_STATE_NFY * res = (sGU_BUDOKAI_UPDATE_STATE_NFY *)packet.GetPacketData();
		res->wOpCode = GU_BUDOKAI_UPDATE_STATE_NFY;
		res->bRankbattleAllow = bRankbattleAllow;
		memcpy(&res->sStateInfo, &sStateInfo, sizeof(sBUDOKAI_UPDATE_STATE_INFO));
		packet.SetPacketLen(sizeof(sGU_BUDOKAI_UPDATE_STATE_NFY));
		g_pObjectManager->SendPacketToAll(&packet);
	}
}

void CBudokaiManager::UpdateMatchState(BYTE byMatchType, sBUDOKAI_UPDATE_MATCH_STATE_INFO & sStateInfo)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (app->m_config.byChannel == DOJO_CHANNEL_INDEX)
	{

	}
	else
	{
		ERR_LOG(LOG_GENERAL, "BUDOKAI: Receive match state update. byMatchType = %u, byState = %u, tmNextStepTime = %u, tmRemainTime = %u", byMatchType, sStateInfo.byState, sStateInfo.tmNextStepTime, sStateInfo.tmRemainTime);

		memcpy(&m_matchStateInfo[byMatchType], &sStateInfo, sizeof(sBUDOKAI_UPDATE_MATCH_STATE_INFO));

		CNtlPacket packet(sizeof(sGU_BUDOKAI_UPDATE_MATCH_STATE_NFY));
		sGU_BUDOKAI_UPDATE_MATCH_STATE_NFY * res = (sGU_BUDOKAI_UPDATE_MATCH_STATE_NFY *)packet.GetPacketData();
		res->wOpCode = GU_BUDOKAI_UPDATE_MATCH_STATE_NFY;
		res->byMatchType = byMatchType;
		memcpy(&res->sStateInfo, &sStateInfo, sizeof(sBUDOKAI_UPDATE_MATCH_STATE_INFO));
		packet.SetPacketLen(sizeof(sGU_BUDOKAI_UPDATE_MATCH_STATE_NFY));
		g_pObjectManager->SendPacketToAll(&packet);
	}
}

void CBudokaiManager::SendTournamentIndividualListToChat(CHARACTERID charId, BYTE byReceivedListCount, HOBJECT hSession)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES));
	sGT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES * res = (sGT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES *)packet.GetPacketData();
	res->wOpCode = GT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->charId = charId;
	res->byMatchDataCount = 0;
	res->bSetEntryTeam = false;
	memset(res->aEntryTeam, 0, sizeof(res->aEntryTeam));
	memset(res->aMatchData, 0, sizeof(res->aMatchData));

	// INFO: If byReceivedListCount == 255 open for the first time.
	BYTE bySkipCount = byReceivedListCount;

	if (IsBudokaiOpen(m_stateInfo.byState))
	{
		if (byReceivedListCount == INVALID_BYTE)
		{
			for (auto it = m_mapIndividualTournament.begin(); it != m_mapIndividualTournament.end(); it++)
			{
				res->aEntryTeam[it->second->byMinorMatch_MatchIndex].wJoinId = it->first;
				res->aEntryTeam[it->second->byMinorMatch_MatchIndex].bDojoRecommender = it->second->bDojoRecommender;
				NTL_SAFE_WCSCPY(res->aEntryTeam[it->second->byMinorMatch_MatchIndex].wszTeamName, it->second->wszCharName);
			}

			res->bSetEntryTeam = true;
			bySkipCount = 0;
		}

		if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
		{
			for (BYTE i = MAX_BUDOKAI_MATCH_DEPTH_INDIVIDUAL; i != INVALID_BUDOKAI_MATCH_DEPTH; i--)
			{
				for (auto it = m_aTournamentMatch[i].m_mapTournament.begin(); it != m_aTournamentMatch[i].m_mapTournament.end(); it++)
				{
					sTOURNAMENT_MATCH* pMatch = it->second;
					memcpy(&res->aMatchData[res->byMatchDataCount], &pMatch->data, sizeof(pMatch->data));

					res->byMatchDataCount++;
				}
			}
		}
	}
	else res->wResultCode = COMMUNITY_FAIL;

	packet.SetPacketLen(sizeof(sGT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES));
	app->Send(hSession, &packet);
}

void CBudokaiManager::SendTournamentIndividualInfoToChat(CHARACTERID charId, WORD * awJoinId, HOBJECT hSession)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES));
	sGT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES * res = (sGT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES *)packet.GetPacketData();
	res->wOpCode = GT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->charId = charId;
	memset(res->asEntryData, 0, sizeof(res->asEntryData));

	if (IsBudokaiOpen(m_stateInfo.byState))
	{
		auto it = m_mapIndividualTournament.find(awJoinId[MATCH_TEAM_TYPE_TEAM1]);
		if (it != m_mapIndividualTournament.end())
		{
			memcpy(&res->asEntryData[MATCH_TEAM_TYPE_TEAM1], it->second, sizeof(sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA));
		}

		auto it2 = m_mapIndividualTournament.find(awJoinId[MATCH_TEAM_TYPE_TEAM2]);
		if (it2 != m_mapIndividualTournament.end())
		{
			memcpy(&res->asEntryData[MATCH_TEAM_TYPE_TEAM2], it2->second, sizeof(sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA));
		}
	}
	else res->wResultCode = COMMUNITY_FAIL;

	packet.SetPacketLen(sizeof(sGT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES));
	app->Send(hSession, &packet);
}

void CBudokaiManager::SendTournamentTeamListToChat(CHARACTERID charId, BYTE byReceivedListCount, HOBJECT hSession)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGT_BUDOKAI_TOURNAMENT_TEAM_LIST_RES));
	sGT_BUDOKAI_TOURNAMENT_TEAM_LIST_RES * res = (sGT_BUDOKAI_TOURNAMENT_TEAM_LIST_RES *)packet.GetPacketData();
	res->wOpCode = GT_BUDOKAI_TOURNAMENT_TEAM_LIST_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->charId = charId;
	res->byMatchDataCount = 0;
	res->bSetEntryTeam = false;
	memset(res->aEntryTeam, 0, sizeof(res->aEntryTeam));
	memset(res->aMatchData, 0, sizeof(res->aMatchData));

	// INFO: If byReceivedListCount == 255 open for the first time.
	BYTE bySkipCount = byReceivedListCount;

	if (IsBudokaiOpen(m_stateInfo.byState))
	{
		if (byReceivedListCount == INVALID_BYTE)
		{
			for (auto it = m_mapTeamTournament.begin(); it != m_mapTeamTournament.end(); it++)
			{
				res->aEntryTeam[it->second->byMinorMatch_MatchIndex].wJoinId = it->first;
				res->aEntryTeam[it->second->byMinorMatch_MatchIndex].bDojoRecommender = false;
				NTL_SAFE_WCSCPY(res->aEntryTeam[it->second->byMinorMatch_MatchIndex].wszTeamName, it->second->wszTeamName);
			}

			res->bSetEntryTeam = true;
			bySkipCount = 0;
		}

		if (m_matchType == BUDOKAI_MATCH_TYPE_TEAM)
		{
			for (BYTE i = MAX_BUDOKAI_MATCH_DEPTH_TEAM; i != INVALID_BUDOKAI_MATCH_DEPTH; i--)
			{
				for (auto it = m_aTournamentMatch[i].m_mapTournament.begin(); it != m_aTournamentMatch[i].m_mapTournament.end(); it++)
				{
					sTOURNAMENT_MATCH* pMatch = it->second;
					memcpy(&res->aMatchData[res->byMatchDataCount], &pMatch->data, sizeof(pMatch->data));

					res->byMatchDataCount++;
				}
			}
		}
	}
	else res->wResultCode = COMMUNITY_FAIL;

	packet.SetPacketLen(sizeof(sGT_BUDOKAI_TOURNAMENT_TEAM_LIST_RES));
	app->Send(hSession, &packet);
}

void CBudokaiManager::SendTournamentTeamInfoToChat(CHARACTERID charId, WORD * awJoinId, HOBJECT hSession)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGT_BUDOKAI_TOURNAMENT_TEAM_INFO_RES));
	sGT_BUDOKAI_TOURNAMENT_TEAM_INFO_RES * res = (sGT_BUDOKAI_TOURNAMENT_TEAM_INFO_RES *)packet.GetPacketData();
	res->wOpCode = GT_BUDOKAI_TOURNAMENT_TEAM_INFO_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->charId = charId;
	memset(res->asEntryData, 0, sizeof(res->asEntryData));

	if (IsBudokaiOpen(m_stateInfo.byState))
	{
		auto it = m_mapTeamTournament.find(awJoinId[MATCH_TEAM_TYPE_TEAM1]);
		if (it != m_mapTeamTournament.end())
		{
			memcpy(&res->asEntryData[MATCH_TEAM_TYPE_TEAM1], it->second, sizeof(sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA));
		}

		auto it2 = m_mapTeamTournament.find(awJoinId[MATCH_TEAM_TYPE_TEAM2]);
		if (it2 != m_mapTeamTournament.end())
		{
			memcpy(&res->asEntryData[MATCH_TEAM_TYPE_TEAM2], it2->second, sizeof(sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA));
		}
	}
	else res->wResultCode = COMMUNITY_FAIL;

	packet.SetPacketLen(sizeof(sGT_BUDOKAI_TOURNAMENT_TEAM_INFO_RES));
	app->Send(hSession, &packet);
}

void CBudokaiManager::CreateBudokai(eBUDOKAI_TYPE type, eBUDOKAI_MATCH_TYPE matchType, BUDOKAITIME curTime, sBUDOKAI_TBLINFO * pTblInfo)
{
	ERR_LOG(LOG_GENERAL, "BUDOKAI: Create Budokai. m_type = %u, matchType = %u, curTime = %u", type, matchType, curTime);

	m_type = type;
	m_matchType = matchType;

	if (type == BUDOKAI_TYPE_JUNIOR)
		m_stateInfo.byState = BUDOKAI_STATE_JUNIOR_OPEN_NOTICE;
	else
		m_stateInfo.byState = BUDOKAI_STATE_OPEN_NOTICE;

	m_stateInfo.tmNextStepTime = curTime + pTblInfo->dwOpenNoticeTime;
	m_stateInfo.tmRemainTime = pTblInfo->dwOpenNoticeTime;

	m_matchStateInfo[m_matchType].byState = INVALID_BUDOKAI_MATCHSTATE;
	m_matchStateInfo[m_matchType].tmNextStepTime = INVALID_BUDOKAITIME;
	m_matchStateInfo[m_matchType].tmRemainTime = INVALID_BUDOKAITIME;

	m_pTableInfo = pTblInfo;

	m_tmDefaultOpenTime = curTime;
	m_bRankbattleAllow = true;

	CGameServer* app = (CGameServer*)g_pApp;

	//broadcast that budokai began
	CNtlPacket packet(sizeof(sGT_BUDOKAI_CHANNEL_STATE_INFO_NFY));
	sGT_BUDOKAI_CHANNEL_STATE_INFO_NFY * res = (sGT_BUDOKAI_CHANNEL_STATE_INFO_NFY *)packet.GetPacketData();
	res->wOpCode = GT_BUDOKAI_CHANNEL_STATE_INFO_NFY;
	res->tmDefaultOpenTime = m_tmDefaultOpenTime;
	res->bRankbattleAllow = m_bRankbattleAllow;
	memcpy(&res->sStateInfo, &m_stateInfo, sizeof(sBUDOKAI_UPDATE_STATE_INFO));
	res->aMatchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].byState = INVALID_BUDOKAI_MATCHSTATE;
	res->aMatchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].byState = INVALID_BUDOKAI_MATCHSTATE;
	memcpy(&res->aMatchStateInfo[m_matchType], &m_matchStateInfo, sizeof(sBUDOKAI_UPDATE_MATCH_STATE_INFO));
	packet.SetPacketLen(sizeof(sGT_BUDOKAI_CHANNEL_STATE_INFO_NFY));
	app->SendTo(app->GetChatServerSession(), &packet);
}

void CBudokaiManager::CloseBudokai()
{
	ERR_LOG(LOG_GENERAL, "BUDOKAI: Close Budokai. m_type = %u, m_matchType = %u, m_byDojoRecommenders = %u, m_eMatchDepth = %u", m_type, m_matchType, m_byDojoRecommenders, m_eMatchDepth);

	for (int i = 0; i < BUDOKAI_MATCH_DEPTH_COUNT; i++)
	{
		for (auto it = m_aTournamentMatch[i].m_mapTournament.begin(); it != m_aTournamentMatch[i].m_mapTournament.end(); )
		{
			sTOURNAMENT_MATCH* match = it->second;
			delete match;
			it = m_aTournamentMatch[i].m_mapTournament.erase(it);
		}
	}

	for (auto it = m_mapIndividualTournament.begin(); it != m_mapIndividualTournament.end(); )
	{
		sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA* entry = it->second;
		delete entry;
		it = m_mapIndividualTournament.erase(it);
	}
	m_mapIndividualTournament.clear();

	for (auto it = m_mapTeamTournament.begin(); it != m_mapTeamTournament.end(); )
	{
		sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* entry = it->second;
		delete entry;
		it = m_mapTeamTournament.erase(it);
	}
	m_mapTeamTournament.clear();

	m_mapPrelimWinners.clear();

	m_eMatchDepth = INVALID_BUDOKAI_MATCH_DEPTH;

	for (auto it = m_mapPrelims.begin(); it != m_mapPrelims.end(); )
	{
		sPRELIM_HEAD* prelim = it->second;
		delete prelim;
		it = m_mapPrelims.erase(it);
	}
	m_mapPrelims.clear();

	m_byDojoRecommenders = 0;
	m_mapJoinInfo.clear();


	for (auto it = m_mapIndividual.begin(); it != m_mapIndividual.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->FindByChar(it->second.charId);
		if (pPlayer)
			pPlayer->InitBudokai();
	}
	m_mapIndividual.clear();

	for (auto it = m_mapTeam.begin(); it != m_mapTeam.end(); it++)
	{
		for (BYTE i = 0; i < NTL_MAX_MEMBER_IN_PARTY; i++)
		{
			if (it->second.aMembers[i] == INVALID_CHARACTERID || it->second.aMembers[i] == 0)
				break;

			CPlayer* pPlayer = g_pObjectManager->FindByChar(it->second.aMembers[i]);
			if (pPlayer)
				pPlayer->InitBudokai();
		}
	}
	m_mapTeam.clear();



	memset(&m_stateInfo, NULL, sizeof(m_stateInfo));

	m_matchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].byState = INVALID_BUDOKAI_MATCHSTATE;
	m_matchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].tmNextStepTime = 0;
	m_matchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].tmRemainTime = 0;
	m_matchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].byState = INVALID_BUDOKAI_MATCHSTATE;
	m_matchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].tmNextStepTime = 0;
	m_matchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].tmRemainTime = 0;

	m_matchType = INVALID_BUDOKAI_MATCH_TYPE;

	m_type = INVALID_BUDOKAI_TYPE;

	m_mapTeamType.clear();
	m_mapMatchIndex.clear();
}

bool CBudokaiManager::TickProcessBudokai(DWORD dwTickDiff, BUDOKAITIME curTime)
{
	switch (m_stateInfo.byState) //budokai state
	{
		case BUDOKAI_STATE_JUNIOR_CLOSE:
		case BUDOKAI_STATE_CLOSE: //this state is the last one. //When this is received by client then information "all battle have ended" appear. So it seems like this is called in the end of budokai.
		{
			CGameServer* app = (CGameServer*)g_pApp;

			CNtlPacket packet(sizeof(sGQ_BUDOKAI_HISTORY_WRITE_REQ));
			sGQ_BUDOKAI_HISTORY_WRITE_REQ * res = (sGQ_BUDOKAI_HISTORY_WRITE_REQ *)packet.GetPacketData();
			res->wOpCode = GQ_BUDOKAI_HISTORY_WRITE_REQ;
			res->byBudokaiType = m_type;
			res->byMatchType = m_matchType;
			packet.SetPacketLen(sizeof(sGQ_BUDOKAI_HISTORY_WRITE_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet);

			CloseBudokai();

			return true;
		}
		break;
		case BUDOKAI_STATE_JUNIOR_OPEN_NOTICE:
		{
			if (m_stateInfo.tmNextStepTime <= curTime)
			{
				m_stateInfo.byState = BUDOKAI_STATE_JUNIOR_DOJO_RECOMMEND;
				m_stateInfo.tmNextStepTime = curTime + m_pTableInfo->dwDojoRecommendTime;
				m_stateInfo.tmRemainTime = m_pTableInfo->dwDojoRecommendTime;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update State BUDOKAI_STATE_JUNIOR_OPEN_NOTICE -> BUDOKAI_STATE_JUNIOR_DOJO_RECOMMEND, tmNextStepTime = %u, tmRemainTime = %u", m_stateInfo.tmNextStepTime, m_stateInfo.tmRemainTime);

				SendBudokaiState();
			}
		}
		break;
		case BUDOKAI_STATE_OPEN_NOTICE:
		{
			if (m_stateInfo.tmNextStepTime <= curTime)
			{
				m_stateInfo.byState = BUDOKAI_STATE_DOJO_RECOMMEND;
				m_stateInfo.tmNextStepTime = curTime + m_pTableInfo->dwDojoRecommendTime;
				m_stateInfo.tmRemainTime = m_pTableInfo->dwDojoRecommendTime;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update State BUDOKAI_STATE_OPEN_NOTICE -> BUDOKAI_STATE_DOJO_RECOMMEND, tmNextStepTime = %u, tmRemainTime = %u", m_stateInfo.tmNextStepTime, m_stateInfo.tmRemainTime);

				SendBudokaiState();
			}
		}
		break;
		case BUDOKAI_STATE_JUNIOR_DOJO_RECOMMEND:
		{
			if (m_stateInfo.tmNextStepTime <= curTime)
			{
				m_stateInfo.byState = BUDOKAI_STATE_JUNIOR_OPEN;
				m_stateInfo.tmNextStepTime = curTime + m_pTableInfo->dwBudokaiEndTime;
				m_stateInfo.tmRemainTime = m_pTableInfo->dwBudokaiEndTime;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update State BUDOKAI_STATE_JUNIOR_DOJO_RECOMMEND -> BUDOKAI_STATE_JUNIOR_OPEN, tmNextStepTime = %u, tmRemainTime = %u", m_stateInfo.tmNextStepTime, m_stateInfo.tmRemainTime);

				SendBudokaiState();

				m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_REGISTER;
				m_matchStateInfo[m_matchType].tmNextStepTime = curTime + m_pTableInfo->dwRegisterTime;
				m_matchStateInfo[m_matchType].tmRemainTime = m_pTableInfo->dwRegisterTime;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update MatchType %u State %u, tmNextStepTime = %u, tmRemainTime = %u",
					m_matchType, BUDOKAI_MATCHSTATE_REGISTER, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();
			}
		}
		break;
		case BUDOKAI_STATE_DOJO_RECOMMEND:
		{
			if (m_stateInfo.tmNextStepTime <= curTime)
			{
				m_stateInfo.byState = BUDOKAI_STATE_OPEN;
				m_stateInfo.tmNextStepTime = curTime + m_pTableInfo->dwBudokaiEndTime;
				m_stateInfo.tmRemainTime = m_pTableInfo->dwBudokaiEndTime;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update State BUDOKAI_STATE_DOJO_RECOMMEND -> BUDOKAI_STATE_OPEN, tmNextStepTime = %u, tmRemainTime = %u", m_stateInfo.tmNextStepTime, m_stateInfo.tmRemainTime);

				SendBudokaiState();

				m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_REGISTER;
				m_matchStateInfo[m_matchType].tmNextStepTime = curTime + m_pTableInfo->dwRegisterTime;
				m_matchStateInfo[m_matchType].tmRemainTime = m_pTableInfo->dwRegisterTime;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update MatchType %u State %u, tmNextStepTime = %u, tmRemainTime = %u", 
					m_matchType, BUDOKAI_MATCHSTATE_REGISTER, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();
			}
		}
		break;
		case BUDOKAI_STATE_JUNIOR_OPEN:
		{
			//progress match
			TickProcessMatch(dwTickDiff, curTime);

			if (m_stateInfo.tmNextStepTime <= curTime)
			{
				//change budokai to clear state. 
				m_stateInfo.byState = BUDOKAI_STATE_JUNIOR_CLEAR;
				m_stateInfo.tmNextStepTime = curTime + 0;
				m_stateInfo.tmRemainTime = 0;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update State BUDOKAI_STATE_JUNIOR_OPEN -> BUDOKAI_STATE_JUNIOR_CLEAR, tmNextStepTime = %u, tmRemainTime = %u", m_stateInfo.tmNextStepTime, m_stateInfo.tmRemainTime);

				SendBudokaiState();
			}
		}
		break;
		case BUDOKAI_STATE_OPEN:
		{
			//progress match
			TickProcessMatch(dwTickDiff, curTime);

			if (m_stateInfo.tmNextStepTime <= curTime)
			{
				//change budokai to clear state. 
				m_stateInfo.byState = BUDOKAI_STATE_CLEAR;
				m_stateInfo.tmNextStepTime = curTime + 0;
				m_stateInfo.tmRemainTime = 0;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update State BUDOKAI_STATE_OPEN -> BUDOKAI_STATE_CLEAR, tmNextStepTime = %u, tmRemainTime = %u", m_stateInfo.tmNextStepTime, m_stateInfo.tmRemainTime);

				SendBudokaiState();
			}
		}
		break;
		case BUDOKAI_STATE_JUNIOR_CLEAR:
		{
			if (m_stateInfo.tmNextStepTime <= curTime)
			{
				//change to close state
				m_stateInfo.byState = BUDOKAI_STATE_JUNIOR_CLOSE;
				m_stateInfo.tmNextStepTime = curTime;
				m_stateInfo.tmRemainTime = 0;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update State BUDOKAI_STATE_JUNIOR_CLEAR -> BUDOKAI_STATE_JUNIOR_CLOSE, tmNextStepTime = %u, tmRemainTime = %u", m_stateInfo.tmNextStepTime, m_stateInfo.tmRemainTime);

				SendBudokaiState();
			}
		}
		break;
		case BUDOKAI_STATE_CLEAR:
		{
			if (m_stateInfo.tmNextStepTime <= curTime)
			{
				//change to close state
				m_stateInfo.byState = BUDOKAI_STATE_CLOSE;
				m_stateInfo.tmNextStepTime = curTime;
				m_stateInfo.tmRemainTime = 0;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update State BUDOKAI_STATE_CLEAR -> BUDOKAI_STATE_CLOSE, tmNextStepTime = %u, tmRemainTime = %u", m_stateInfo.tmNextStepTime, m_stateInfo.tmRemainTime);

				SendBudokaiState();
			}
		}
		break;

		default: ERR_LOG(LOG_SYSTEM, "Unknown budokai-state %u", m_stateInfo.byState); break;
	}

	return false;
}

void CBudokaiManager::TickProcessMatch(DWORD dwTickDif, BUDOKAITIME curTime)
{
	switch (m_matchStateInfo[m_matchType].byState)
	{
		case BUDOKAI_MATCHSTATE_REGISTER:
		{
			if (m_matchStateInfo[m_matchType].tmNextStepTime <= curTime)
			{
				m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_WAIT_MINOR_MATCH;
				m_matchStateInfo[m_matchType].tmNextStepTime = curTime + m_pTableInfo->dwMinorMatch_WaitTime;
				m_matchStateInfo[m_matchType].tmRemainTime = m_pTableInfo->dwMinorMatch_WaitTime;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Match-State. Type %u, byState BUDOKAI_MATCHSTATE_REGISTER -> BUDOKAI_MATCHSTATE_WAIT_MINOR_MATCH, tmNextStepTime = %u, tmRemainTime = %u", 
					m_matchType, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();
			}
		}
		break;

		case BUDOKAI_MATCHSTATE_WAIT_MINOR_MATCH:
		{
			if (m_matchStateInfo[m_matchType].tmNextStepTime <= curTime)
			{
				m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_MINOR_MATCH;
				m_matchStateInfo[m_matchType].tmNextStepTime = INVALID_BUDOKAITIME;
				m_matchStateInfo[m_matchType].tmRemainTime = 0;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Match-State. Type %u, byState BUDOKAI_MATCHSTATE_WAIT_MINOR_MATCH -> BUDOKAI_MATCHSTATE_MINOR_MATCH, tmNextStepTime = %u, tmRemainTime = %u",
					m_matchType, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();

				//start prelim
				StartPrelim();
			}
		}
		break;

		case BUDOKAI_MATCHSTATE_MINOR_MATCH:
		{
			//progress prelim
			TickProcessPrelim(dwTickDif, curTime);

			if (m_mapPrelims.size() == 0) //check if all prelims done
			{
				EndPrelim();

				if(m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
					m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_32;
				else
					m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_16;

				m_matchStateInfo[m_matchType].tmNextStepTime = curTime + m_pTableInfo->dwMajorMatch_WaitTime;
				m_matchStateInfo[m_matchType].tmRemainTime = m_pTableInfo->dwMajorMatch_WaitTime;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Match-State. Type %u, byState BUDOKAI_MATCHSTATE_MINOR_MATCH -> BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_32, tmNextStepTime = %u, tmRemainTime = %u",
					m_matchType, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();
			}
		}
		break;

		case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_32:
		{
			if (m_matchStateInfo[m_matchType].tmNextStepTime <= curTime)
			{
				m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_MAJORMATCH_32;
				m_matchStateInfo[m_matchType].tmNextStepTime = INVALID_BUDOKAITIME;
				m_matchStateInfo[m_matchType].tmRemainTime = 0;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Match-State. Type %u, byState BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_32 -> BUDOKAI_MATCHSTATE_MAJORMATCH_32, tmNextStepTime = %u, tmRemainTime = %u",
					m_matchType, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();

				StartMajorMatchTeleport();
			}
		}
		break;

		case BUDOKAI_MATCHSTATE_MAJORMATCH_32:
		{
			//progress depth 32
			TickProcessMajorMatch(dwTickDif, curTime);

			if(m_aTournamentMatch[BUDOKAI_MATCH_DEPTH_32].byMatchCount == m_aTournamentMatch[BUDOKAI_MATCH_DEPTH_32].byMatchCountEnd)//check if all 32th done
			{
				EndMatchDepth(BUDOKAI_MATCH_DEPTH_32);

				m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_16;
				m_matchStateInfo[m_matchType].tmNextStepTime = curTime + m_pTableInfo->dwMajorMatch_WaitTime;
				m_matchStateInfo[m_matchType].tmRemainTime = m_pTableInfo->dwMajorMatch_WaitTime;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Match-State. Type %u, byState BUDOKAI_MATCHSTATE_MAJORMATCH_32 -> BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_16, tmNextStepTime = %u, tmRemainTime = %u",
					m_matchType, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();
			}
		}
		break;

		case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_16:
		{
			if (m_matchStateInfo[m_matchType].tmNextStepTime <= curTime)
			{
				m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_MAJORMATCH_16;
				m_matchStateInfo[m_matchType].tmNextStepTime = INVALID_BUDOKAITIME;
				m_matchStateInfo[m_matchType].tmRemainTime = 0;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Match-State. Type %u, byState BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_16 -> BUDOKAI_MATCHSTATE_MAJORMATCH_16, tmNextStepTime = %u, tmRemainTime = %u",
					m_matchType, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();

				StartMajorMatchTeleport();
			}
		}
		break;

		case BUDOKAI_MATCHSTATE_MAJORMATCH_16:
		{
			//progress depth 16
			TickProcessMajorMatch(dwTickDif, curTime);

			if (m_aTournamentMatch[BUDOKAI_MATCH_DEPTH_16].byMatchCount == m_aTournamentMatch[BUDOKAI_MATCH_DEPTH_16].byMatchCountEnd) //check if all 16th done
			{
				EndMatchDepth(BUDOKAI_MATCH_DEPTH_16);

				m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_08;
				m_matchStateInfo[m_matchType].tmNextStepTime = curTime + m_pTableInfo->dwMajorMatch_WaitTime;
				m_matchStateInfo[m_matchType].tmRemainTime = m_pTableInfo->dwMajorMatch_WaitTime;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Match-State. Type %u, byState BUDOKAI_MATCHSTATE_MAJORMATCH_16 -> BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_08, tmNextStepTime = %u, tmRemainTime = %u",
					m_matchType, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();
			}
		}
		break;

		case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_08:
		{
			if (m_matchStateInfo[m_matchType].tmNextStepTime <= curTime)
			{
				m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_MAJORMATCH_08;
				m_matchStateInfo[m_matchType].tmNextStepTime = INVALID_BUDOKAITIME;
				m_matchStateInfo[m_matchType].tmRemainTime = 0;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Match-State. Type %u, byState BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_08 -> BUDOKAI_MATCHSTATE_MAJORMATCH_08, tmNextStepTime = %u, tmRemainTime = %u",
					m_matchType, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();

				StartMajorMatchTeleport();
			}
		}
		break;

		case BUDOKAI_MATCHSTATE_MAJORMATCH_08:
		{
			//progress depth 8
			TickProcessMajorMatch(dwTickDif, curTime);

			if (m_aTournamentMatch[BUDOKAI_MATCH_DEPTH_8].byMatchCount == m_aTournamentMatch[BUDOKAI_MATCH_DEPTH_8].byMatchCountEnd) //check if all 8th done
			{
				EndMatchDepth(BUDOKAI_MATCH_DEPTH_8);

				m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH;
				m_matchStateInfo[m_matchType].tmNextStepTime = curTime + m_pTableInfo->dwFinalMatch_WaitTime;
				m_matchStateInfo[m_matchType].tmRemainTime = m_pTableInfo->dwFinalMatch_WaitTime;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Match-State. Type %u, byState BUDOKAI_MATCHSTATE_MAJORMATCH_08 -> BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH, tmNextStepTime = %u, tmRemainTime = %u",
					m_matchType, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();
			}
		}
		break;

		case BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH:
		{
			if (m_matchStateInfo[m_matchType].tmNextStepTime <= curTime)
			{
				m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH;
				m_matchStateInfo[m_matchType].tmNextStepTime = INVALID_BUDOKAITIME;
				m_matchStateInfo[m_matchType].tmRemainTime = 0;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Match-State. Type %u, byState BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH -> BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH, tmNextStepTime = %u, tmRemainTime = %u",
					m_matchType, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();

				StartFinalMatchTeleport();
			}
		}
		break;

		case BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH:
		{
			//progress semi finals
			TickProcessFinalMatch(dwTickDif, curTime);

			if (m_aTournamentMatch[BUDOKAI_MATCH_DEPTH_4].byMatchCount == m_aTournamentMatch[BUDOKAI_MATCH_DEPTH_4].byMatchCountEnd) //check if all semi final done
			{
				EndMatchDepth(BUDOKAI_MATCH_DEPTH_4);

				m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_WAIT_FINAL_MATCH;
				m_matchStateInfo[m_matchType].tmNextStepTime = curTime + m_pTableInfo->dwFinalMatch_WaitTime;
				m_matchStateInfo[m_matchType].tmRemainTime = m_pTableInfo->dwFinalMatch_WaitTime;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Match-State. Type %u, byState BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH -> BUDOKAI_MATCHSTATE_WAIT_FINAL_MATCH, tmNextStepTime = %u, tmRemainTime = %u",
					m_matchType, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();
			}
		}
		break;

		case BUDOKAI_MATCHSTATE_WAIT_FINAL_MATCH:
		{
			if (m_matchStateInfo[m_matchType].tmNextStepTime <= curTime)
			{
				m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_FINAL_MATCH;
				m_matchStateInfo[m_matchType].tmNextStepTime = INVALID_BUDOKAITIME;
				m_matchStateInfo[m_matchType].tmRemainTime = 0;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Match-State. Type %u, byState BUDOKAI_MATCHSTATE_WAIT_FINAL_MATCH -> BUDOKAI_MATCHSTATE_FINAL_MATCH, tmNextStepTime = %u, tmRemainTime = %u",
					m_matchType, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();

				StartFinalMatchTeleport();
			}
		}
		break;

		case BUDOKAI_MATCHSTATE_FINAL_MATCH:
		{
			TickProcessFinalMatch(dwTickDif, curTime);

			if (m_aTournamentMatch[BUDOKAI_MATCH_DEPTH_2].byMatchCount == m_aTournamentMatch[BUDOKAI_MATCH_DEPTH_2].byMatchCountEnd) //check if all final done
			{
				m_matchStateInfo[m_matchType].byState = BUDOKAI_MATCHSTATE_MATCH_END;
				m_matchStateInfo[m_matchType].tmNextStepTime = curTime + m_pTableInfo->dwEndingWaitTime;
				m_matchStateInfo[m_matchType].tmRemainTime = m_pTableInfo->dwEndingWaitTime;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Match-State. Type %u, byState BUDOKAI_MATCHSTATE_FINAL_MATCH -> BUDOKAI_MATCHSTATE_MATCH_END, tmNextStepTime = %u, tmRemainTime = %u",
					m_matchType, m_matchStateInfo[m_matchType].tmNextStepTime, m_matchStateInfo[m_matchType].tmRemainTime);

				SendMatchState();
			}
		}
		break;

		case BUDOKAI_MATCHSTATE_MATCH_END:
		{
			if (m_matchStateInfo[m_matchType].tmNextStepTime <= curTime)
			{
				m_stateInfo.tmNextStepTime = 0;
			}
		}
		break;

		default: ERR_LOG(LOG_SYSTEM, "Invalid budokai match state %u", m_matchStateInfo[m_matchType].byState); break;
	}
}


void CBudokaiManager::SendBudokaiState()
{
	CGameServer* app = (CGameServer*)g_pApp;

	//send to query server
	CNtlPacket packet2(sizeof(sGQ_BUDOKAI_UPDATE_STATE_REQ));
	sGQ_BUDOKAI_UPDATE_STATE_REQ * res2 = (sGQ_BUDOKAI_UPDATE_STATE_REQ *)packet2.GetPacketData();
	res2->wOpCode = GQ_BUDOKAI_UPDATE_STATE_REQ;
	res2->sStateData.byState = m_stateInfo.byState;
	res2->sStateData.tmNextStepTime = m_stateInfo.tmNextStepTime;
	packet2.SetPacketLen(sizeof(sGQ_BUDOKAI_UPDATE_STATE_REQ));
	app->SendTo(app->GetQueryServerSession(), &packet2);

	CNtlPacket packet(sizeof(sGT_BUDOKAI_CHANNEL_UPDATE_STATE));
	sGT_BUDOKAI_CHANNEL_UPDATE_STATE * res = (sGT_BUDOKAI_CHANNEL_UPDATE_STATE *)packet.GetPacketData();
	res->wOpCode = GT_BUDOKAI_CHANNEL_UPDATE_STATE;
	res->tmDefaultOpenTime = m_tmDefaultOpenTime;
	res->bRankbattleAllow = m_bRankbattleAllow;
	memcpy(&res->sStateInfo, &m_stateInfo, sizeof(sBUDOKAI_UPDATE_STATE_INFO));
	packet.SetPacketLen(sizeof(sGT_BUDOKAI_CHANNEL_UPDATE_STATE));
	app->SendTo(app->GetChatServerSession(), &packet);
}

void CBudokaiManager::SendMatchState()
{
	CGameServer* app = (CGameServer*)g_pApp;

	//send to query server
	CNtlPacket packet2(sizeof(sGQ_BUDOKAI_UPDATE_MATCH_STATE_REQ));
	sGQ_BUDOKAI_UPDATE_MATCH_STATE_REQ * res2 = (sGQ_BUDOKAI_UPDATE_MATCH_STATE_REQ *)packet2.GetPacketData();
	res2->wOpCode = GQ_BUDOKAI_UPDATE_MATCH_STATE_REQ;
	res2->byMatchType = m_matchType;
	res2->sStateData.byState = m_matchStateInfo[m_matchType].byState;
	res2->sStateData.tmNextStepTime = m_matchStateInfo[m_matchType].tmNextStepTime;
	packet2.SetPacketLen(sizeof(sGQ_BUDOKAI_UPDATE_MATCH_STATE_REQ));
	app->SendTo(app->GetQueryServerSession(), &packet2);

	CNtlPacket packet(sizeof(sGT_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE));
	sGT_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE * res = (sGT_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE *)packet.GetPacketData();
	res->wOpCode = GT_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE;
	res->byMatchType = m_matchType;
	memcpy(&res->sStateInfo, &m_matchStateInfo[m_matchType], sizeof(sBUDOKAI_UPDATE_MATCH_STATE_INFO));
	packet.SetPacketLen(sizeof(sGT_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE));
	app->SendTo(app->GetChatServerSession(), &packet);
}


void CBudokaiManager::StartPrelim()
{
	CGameServer* app = (CGameServer*)g_pApp;

	//send announcement that players can enter prelims
	CNtlPacket packet2(sizeof(sGU_BUDOKAI_PROGRESS_MESSAGE_NFY));
	sGU_BUDOKAI_PROGRESS_MESSAGE_NFY * res2 = (sGU_BUDOKAI_PROGRESS_MESSAGE_NFY *)packet2.GetPacketData();
	res2->wOpCode = GU_BUDOKAI_PROGRESS_MESSAGE_NFY;
	res2->byMsgId = BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_ENTERING;
	packet2.SetPacketLen(sizeof(sGU_BUDOKAI_PROGRESS_MESSAGE_NFY));
	g_pObjectManager->SendPacketToAll(&packet2);

	BYTE byIndex = 0;
	BYTE byMaxPrelims = (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL) ? BUDOKAI_MAX_INDIVIDUAL_MATCH_COUNT - m_byDojoRecommenders : BUDOKAI_MAX_TEAM_MATCH_COUNT;
	int nMaxTotalCount = (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL) ? byMaxPrelims * BUDOKAI_MINOR_MATCH_INDIVIDUAL_COUNT : BUDOKAI_MAX_TEAM_MATCH_COUNT * BUDOKAI_MINOR_MATCH_TEAM_COUNT;
	int nTotalCount = 0;

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		ERR_LOG(LOG_GENERAL, "BUDOKAI: Start create Individual Prelims. m_byDojoRecommenders = %u, byMaxPrelims = %u, RegistrarCount %u",
			m_byDojoRecommenders, byMaxPrelims, (int)m_mapIndividual.size());

		CNtlPacket packet(sizeof(sGT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY));
		sGT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY * res = (sGT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY *)packet.GetPacketData();
		res->wOpCode = GT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY;
		res->byServerChannelId = DOJO_CHANNEL_INDEX;
		res->byServerIndex = 0;
		memset(res->aTeleportData, 0, sizeof(res->aTeleportData));

		// --- 1. LOOP -> ALL DOJO RECOMMENDERS
		for (auto it = m_mapIndividual.begin(); it != m_mapIndividual.end(); it++)
		{
			if (it->second.bDojoRecommender)
			{
				CWorld* pWorld = app->GetGameMain()->GetWorldManager()->CreateWorld((sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(m_pTableInfo->sIndividualWorldTblidx.minorMatch));
				if (pWorld)
				{
					res->aTeleportData[byIndex].worldTblidx = m_pTableInfo->sIndividualWorldTblidx.minorMatch;
					res->aTeleportData[byIndex].byPlayerCount = 1;
					res->aTeleportData[byIndex].aPlayers[0] = it->second.charId;

					it->second.byMinorMatch_MatchIndex = byIndex;

					//set join result
					SetJoinResult(it->first, BUDOKAI_JOIN_RESULT_MINORMATCH);

					res->aTeleportData[byIndex].worldId = pWorld->GetID();

					sPRELIM_HEAD* prelim = new sPRELIM_HEAD;
					prelim->m_byMatchState = BUDOKAI_MINORMATCH_STATE_WAIT;
					prelim->m_dwMatchTickCount = 2 * 60 * 1000; //2 minutes
					prelim->m_byTotalCount = 1;
					prelim->m_byCount = 0;
					prelim->m_pWorld = pWorld;
					m_mapPrelims.insert({ byIndex , prelim });

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Created Individual Prelim. Index = %u, WorldID = %u, m_byTotalPlayerCount = %u ",
						byIndex, pWorld->GetID(), 1);

					++byIndex;
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "Could not create world with tblidx %u", m_pTableInfo->sIndividualWorldTblidx.minorMatch);
				}
			}
		}

		BYTE byNonDojoIndex = byIndex;

		std::vector<std::pair<JOINID, sBUDOKAI_REGISTER_INDIVIDUAL_DATA>> elems(m_mapIndividual.begin(), m_mapIndividual.end());

		std::sort(elems.begin(), elems.end(), sprelim_sorting());

		// --- 2. LOOP -> ALL NON-DOJO RECOMMENDERS
		for (auto it = elems.begin(); it != elems.end(); it++)
		{
		//	printf("it->first: %u, charId:%u, fPoint %f \n", it->first, it->second.charId, it->second.fPoint);
			if (it->second.bDojoRecommender == false)
			{
				auto realIt = m_mapIndividual.find(it->first);
				if (realIt != m_mapIndividual.end())
				{
					res->aTeleportData[byNonDojoIndex].worldTblidx = m_pTableInfo->sIndividualWorldTblidx.minorMatch;
					res->aTeleportData[byNonDojoIndex].aPlayers[res->aTeleportData[byNonDojoIndex].byPlayerCount++] = realIt->second.charId;

					realIt->second.byMinorMatch_MatchIndex = byNonDojoIndex;

					//set join result
					SetJoinResult(realIt->first, BUDOKAI_JOIN_RESULT_MINORMATCH);

					CWorld* pWorld = NULL;

					if (res->aTeleportData[byNonDojoIndex].byPlayerCount == 1) //if we set first player, then create prelim
					{
						pWorld = app->GetGameMain()->GetWorldManager()->CreateWorld((sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(m_pTableInfo->sIndividualWorldTblidx.minorMatch));

						res->aTeleportData[byNonDojoIndex].worldId = pWorld->GetID();

						sPRELIM_HEAD* prelim = new sPRELIM_HEAD;
						prelim->m_byMatchState = BUDOKAI_MINORMATCH_STATE_WAIT;
						prelim->m_dwMatchTickCount = 2 * 60 * 1000; //2 minutes
						prelim->m_byTotalCount = 1;
						prelim->m_byCount = 0;
						prelim->m_pWorld = pWorld;
						m_mapPrelims.insert({ byNonDojoIndex , prelim });

						ERR_LOG(LOG_GENERAL, "BUDOKAI: Created Individual Prelim. Index = %u, WorldID = %u",
							byNonDojoIndex, pWorld->GetID());
					}
					else
					{
						auto itPrelim = m_mapPrelims.find(byNonDojoIndex);
						if (itPrelim != m_mapPrelims.end())
						{
							itPrelim->second->m_byTotalCount += 1;

							ERR_LOG(LOG_GENERAL, "BUDOKAI: Add Player to Individual Prelim. Index = %u, WorldID = %u, m_byTotalPlayerCount = %u",
								byNonDojoIndex, itPrelim->second->m_pWorld->GetID(), itPrelim->second->m_byTotalCount);
						}
						else
						{
							ERR_LOG(LOG_GENERAL, "BUDOKAI: COULD NOT FIND PRELIM INDEX %u", byNonDojoIndex);
						}
					}

					++byNonDojoIndex;

					++nTotalCount;

					if (nTotalCount >= nMaxTotalCount)
						break;

					if (byNonDojoIndex == BUDOKAI_MAX_INDIVIDUAL_MATCH_COUNT)
					{
						byNonDojoIndex = byIndex;
					}
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "Could not find realit.. joinid: %u, tempsize %u, map size %u", it->first, (int)elems.size(), (int)m_mapIndividual.size());
				}
			}
		}


		ERR_LOG(LOG_GENERAL, "BUDOKAI: Individual Prelims %u out of max %u created. Players = %u",
			byNonDojoIndex, byMaxPrelims, nTotalCount + m_byDojoRecommenders);

		packet.SetPacketLen(sizeof(sGT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY));
		app->SendTo(app->GetChatServerSession(), &packet);
	}
	else
	{
		ERR_LOG(LOG_GENERAL, "BUDOKAI: Start create Party Prelims. byMaxPrelims = %u, RegistrarCount %u", byMaxPrelims, (int)m_mapIndividual.size());

		CNtlPacket packet(sizeof(sGT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY));
		sGT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY * res = (sGT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY *)packet.GetPacketData();
		res->wOpCode = GT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY;
		res->byServerChannelId = DOJO_CHANNEL_INDEX;
		res->byServerIndex = 0;
		memset(res->aTeleportData, 0, sizeof(res->aTeleportData));

		BYTE byNonDojoIndex = byIndex;

		// --- 1. LOOP
		for (auto it = m_mapTeam.begin(); it != m_mapTeam.end(); it++)
		{
			res->aTeleportData[byNonDojoIndex].worldTblidx = m_pTableInfo->sTeamWorldTblidx.minorMatch;

			for (BYTE memberCount = 0; memberCount < NTL_MAX_MEMBER_IN_PARTY; memberCount++)
			{
				res->aTeleportData[byNonDojoIndex].aTeamList[res->aTeleportData[byNonDojoIndex].byTeamCount].charId[memberCount] = it->second.aMembers[memberCount];
				res->aTeleportData[byNonDojoIndex].aTeamList[res->aTeleportData[byNonDojoIndex].byTeamCount].byCount++;
			}

			res->aTeleportData[byNonDojoIndex].byTeamCount++;

			it->second.byMinorMatch_MatchIndex = byNonDojoIndex;

			//set join result
			SetJoinResult(it->first, BUDOKAI_JOIN_RESULT_MINORMATCH);

			CWorld* pWorld = NULL;

			if (res->aTeleportData[byNonDojoIndex].byTeamCount == 1) //if we set first player, then create prelim
			{
				pWorld = app->GetGameMain()->GetWorldManager()->CreateWorld((sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(m_pTableInfo->sTeamWorldTblidx.minorMatch));

				res->aTeleportData[byNonDojoIndex].worldId = pWorld->GetID();

				sPRELIM_HEAD* prelim = new sPRELIM_HEAD;
				prelim->m_byMatchState = BUDOKAI_MINORMATCH_STATE_WAIT;
				prelim->m_dwMatchTickCount = 2 * 60 * 1000; //2 minutes
				prelim->m_byTotalCount = 5;
				prelim->m_byCount = 0;
				prelim->m_pWorld = pWorld;
				m_mapPrelims.insert({ byNonDojoIndex , prelim });

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Created Party Prelim. Index = %u, WorldID = %u",
					byNonDojoIndex, pWorld->GetID());
			}
			else
			{
				auto itPrelim = m_mapPrelims.find(byNonDojoIndex);
				if (itPrelim != m_mapPrelims.end())
				{
					itPrelim->second->m_byTotalCount += 5;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Add Team to Party Prelim. Index = %u, WorldID = %u, m_byTotalTeamCount = %u",
						byNonDojoIndex, itPrelim->second->m_pWorld->GetID(), itPrelim->second->m_byTotalCount);
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: COULD NOT FIND PRELIM INDEX %u", byNonDojoIndex);
				}
			}

			++byNonDojoIndex;

			++nTotalCount;

			if (nTotalCount >= nMaxTotalCount)
				break;

			if (byNonDojoIndex == BUDOKAI_MAX_TEAM_MATCH_COUNT)
			{
				byNonDojoIndex = byIndex;
			}
		}


		ERR_LOG(LOG_GENERAL, "BUDOKAI: Party Prelims %u out of max %u created. Players = %u",
			byNonDojoIndex, byMaxPrelims, nTotalCount);

		packet.SetPacketLen(sizeof(sGT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY));
		app->SendTo(app->GetChatServerSession(), &packet);
	}
}

void CBudokaiManager::EndPrelim()
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		m_eMatchDepth = MAX_BUDOKAI_MATCH_DEPTH_INDIVIDUAL;

		ERR_LOG(LOG_GENERAL, "BUDOKAI: Individual Prelim End! Set m_eMatchDepth = %u. Start Creating tournament matches", m_eMatchDepth);

		for (BYTE depth = MAX_BUDOKAI_MATCH_DEPTH_INDIVIDUAL; depth != INVALID_BUDOKAI_MATCH_DEPTH; depth--)
		{
			m_aTournamentMatch[depth].byMatchCount = 0;
			m_aTournamentMatch[depth].byMatchCountEnd = 0;

			BYTE byCreateCount = (BYTE)std::pow(2, depth);

			for (BYTE i = 0; i < byCreateCount; i++)
			{
				sTOURNAMENT_MATCH* tournament = new sTOURNAMENT_MATCH;
				tournament->data.byDepth = depth;
				tournament->data.byMatchNumber = i;

				if (depth == MAX_BUDOKAI_MATCH_DEPTH_INDIVIDUAL) //add prelim winners
				{
					for (auto it = m_mapPrelimWinners.begin(); it != m_mapPrelimWinners.end(); )
					{
						if (BYTE(it->first / 2) == i) //set player into the 32th slot same as prelim index. Do prelim index / 2. Because 0 and 1 = match index 0. 2 and 3 = match index 1...
						{
							SetMatchIndex(it->second.first, i);

							if (tournament->data.wJoinId1 == INVALID_JOINID)
							{
								tournament->m_byTotalCount++;
								tournament->data.wJoinId1 = it->second.first;
								SetIndividualTeamType(it->second.second, MATCH_TEAM_TYPE_TEAM1);

								ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim End. Match Index %u, Add JoinID1 %u to 32th", i, it->second.first);

								it = m_mapPrelimWinners.erase(it);
								continue;
							}
							else
							{
								tournament->m_byTotalCount++;
								tournament->data.wJoinId2 = it->second.first;
								SetIndividualTeamType(it->second.second, MATCH_TEAM_TYPE_TEAM2);

								ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim End. Match Index %u, Add JoinID2 %u to 32th", i, it->second.first);

								it = m_mapPrelimWinners.erase(it);
								break;
							}
						} 
						else ++it;
					}
				}

				//create world
				if (depth <= BUDOKAI_MATCH_DEPTH_4)
					tournament->m_pWorld = app->GetGameMain()->GetWorldManager()->CreateWorld((sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(m_pTableInfo->sIndividualWorldTblidx.finalMatch));
				else
					tournament->m_pWorld = app->GetGameMain()->GetWorldManager()->CreateWorld((sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(m_pTableInfo->sIndividualWorldTblidx.majorMatch));

				if (tournament->m_byTotalCount > 0)
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: Created Match %u, depth %u, Created WorldID = %u, Players %u", i, depth, tournament->m_pWorld->GetID(), tournament->m_byTotalCount);
				}

				m_aTournamentMatch[depth].byMatchCount += 1;
				m_aTournamentMatch[depth].m_mapTournament.insert({ i , tournament });
			}
		}
	}
	else
	{
		m_eMatchDepth = MAX_BUDOKAI_MATCH_DEPTH_TEAM;

		ERR_LOG(LOG_GENERAL, "BUDOKAI: Party Prelim End! Set m_eMatchDepth = %u. Start Creating tournament matches", m_eMatchDepth);

		for (BYTE depth = MAX_BUDOKAI_MATCH_DEPTH_TEAM; depth != INVALID_BUDOKAI_MATCH_DEPTH; depth--)
		{
			m_aTournamentMatch[depth].byMatchCount = 0;
			m_aTournamentMatch[depth].byMatchCountEnd = 0;

			BYTE byCreateCount = (BYTE)std::pow(2, depth);

			for (BYTE i = 0; i < byCreateCount; i++)
			{
				sTOURNAMENT_MATCH* tournament = new sTOURNAMENT_MATCH;
				tournament->data.byDepth = depth;
				tournament->data.byMatchNumber = i;

				if (depth == MAX_BUDOKAI_MATCH_DEPTH_TEAM) // add prelim winners
				{
					for (auto it = m_mapPrelimWinners.begin(); it != m_mapPrelimWinners.end(); )
					{
						if (BYTE(it->first / 2) == i) //set party into the 16th slot same as prelim index. Do prelim index / 2. Because 0 and 1 = match index 0. 2 and 3 = match index 1...
						{
							SetMatchIndex(it->second.first, i);

							if (tournament->data.wJoinId1 == INVALID_JOINID)
							{
								tournament->m_byTotalCount += 5;
								tournament->data.wJoinId1 = it->second.first;
								SetIndividualTeamType(it->second.second, MATCH_TEAM_TYPE_TEAM1);

								ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim End. Match Index %u, Add JoinID1 %u to 16th", i, it->second.first);

								it = m_mapPrelimWinners.erase(it);
								continue;
							}
							else
							{
								tournament->m_byTotalCount += 5;
								tournament->data.wJoinId2 = it->second.first;
								SetIndividualTeamType(it->second.second, MATCH_TEAM_TYPE_TEAM2);

								ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim End. Match Index %u, Add JoinID2 %u to 16th", i, it->second.first);

								it = m_mapPrelimWinners.erase(it);
								break;
							}
						}
						else ++it;
					}
				}

				//create world
				if (depth <= BUDOKAI_MATCH_DEPTH_4)
					tournament->m_pWorld = app->GetGameMain()->GetWorldManager()->CreateWorld((sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(m_pTableInfo->sTeamWorldTblidx.finalMatch));
				else
					tournament->m_pWorld = app->GetGameMain()->GetWorldManager()->CreateWorld((sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(m_pTableInfo->sTeamWorldTblidx.majorMatch));

				if (tournament->m_byTotalCount > 0)
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: Created Match %u, depth %u, Created WorldID = %u, Teams %u", i, depth, tournament->m_pWorld->GetID(), tournament->m_byTotalCount);
				}

				m_aTournamentMatch[depth].byMatchCount += 1;
				m_aTournamentMatch[depth].m_mapTournament.insert({ i , tournament });
			}
		}
	}

	ERR_LOG(LOG_GENERAL, "BUDOKAI: Finished Creating Tournament Matches");
}

void CBudokaiManager::TickProcessPrelim(DWORD dwTickDif, BUDOKAITIME curTime)
{
	for (auto it = m_mapPrelims.begin(); it != m_mapPrelims.end(); )
	{
		sPRELIM_HEAD* prelim = it->second;

		prelim->m_dwMatchTickCount = UnsignedSafeDecrease<DWORD>(prelim->m_dwMatchTickCount, dwTickDif);

		switch (prelim->m_byMatchState)
		{
			case BUDOKAI_MINORMATCH_STATE_NONE:
			{
			}
			break;
			case BUDOKAI_MINORMATCH_STATE_WAIT_MINORMATCH:
			{
			}
			break;
			case BUDOKAI_MINORMATCH_STATE_WAIT:
			{
				if (prelim->m_dwMatchTickCount == 0 || prelim->m_byCount >= prelim->m_byTotalCount)
				{
					if (prelim->m_byCount == 0 || prelim->m_byTotalCount == 0) //if none inside, then end directly //if none inside, then end directly
					{
						prelim->m_byMatchState = BUDOKAI_MINORMATCH_STATE_END;
						ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u no players connected. Go to end..", it->first);
					}
					else //normal prelim begin
					{
						ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u Started. PlayerCount %u, TotalPlayer %u", it->first, prelim->m_byCount, prelim->m_byTotalCount);

						prelim->m_byMatchState = BUDOKAI_MINORMATCH_STATE_MATCH_READY;
						prelim->m_dwMatchTickCount = 5000;

						CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
						sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
						res2->wOpCode = GU_MATCH_MINORMATCH_STATE_UPDATE_NFY;
						res2->byMatchState = BUDOKAI_MINORMATCH_STATE_MATCH_READY;
						res2->bIsEnter = true;
						res2->byStage = 0;
						res2->dwRemainTime = prelim->m_dwMatchTickCount;
						packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
						prelim->m_pWorld->Broadcast(&packet2);
					}
				}
			}
			break;
			case BUDOKAI_MINORMATCH_STATE_DIRECTION:
			{
			}
			break;
			case BUDOKAI_MINORMATCH_STATE_MATCH_READY:
			{
				if (prelim->m_dwMatchTickCount == 0)
				{
					//send player info
					SendMinorMatchTeamInfo(it->first, prelim);

					//send player state
					MinorMatchUpdatePlayersState(prelim, it->first, MATCH_MEMBER_STATE_NORMAL);

					bool bDefaultWin = false;

					if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL && prelim->m_byTotalCount == 1)
						bDefaultWin = true;
					else if (m_matchType == BUDOKAI_MATCH_TYPE_TEAM && prelim->m_byTotalCount == 5)
						bDefaultWin = true;

					if (bDefaultWin) // if only 1 in prelim, then finish
					{
						ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Only one player/team in prelim.. Add as default winner", it->first);

						if(m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
							prelim->m_mapSelection.insert({ prelim->m_mapIndividualPrelim.begin()->first, 0 });
						else
							prelim->m_mapSelection.insert({ prelim->m_mapTeamPrelim.begin()->first, 0 });

						prelim->m_byMatchState = BUDOKAI_MINORMATCH_STATE_STAGE_FINISH;
						prelim->m_dwMatchTickCount = 3000;

						CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
						sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
						res2->wOpCode = GU_MATCH_MINORMATCH_STATE_UPDATE_NFY;
						res2->byMatchState = BUDOKAI_MINORMATCH_STATE_STAGE_FINISH;
						res2->bIsEnter = true;
						res2->byStage = 0;
						res2->dwRemainTime = prelim->m_dwMatchTickCount;
						packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
						prelim->m_pWorld->Broadcast(&packet2);
					}
					else
					{
						prelim->m_byMatchState = BUDOKAI_MINORMATCH_STATE_STAGE_READY;
						prelim->m_dwMatchTickCount = 5000;

						//this packet will show "READY"
						CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
						sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
						res2->wOpCode = GU_MATCH_MINORMATCH_STATE_UPDATE_NFY;
						res2->byMatchState = BUDOKAI_MINORMATCH_STATE_STAGE_READY;
						res2->bIsEnter = true;
						res2->byStage = 0;
						res2->dwRemainTime = prelim->m_dwMatchTickCount;
						packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
						prelim->m_pWorld->Broadcast(&packet2);
					}

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Update State %u -> %u. m_dwMatchTickCount = %u", it->first, BUDOKAI_MINORMATCH_STATE_MATCH_READY, prelim->m_byMatchState, prelim->m_dwMatchTickCount);
				}
			}
			break;
			case BUDOKAI_MINORMATCH_STATE_STAGE_READY:
			{
				if (prelim->m_dwMatchTickCount == 0)
				{
					prelim->m_byMatchState = BUDOKAI_MINORMATCH_STATE_STAGE_RUN;
					prelim->m_dwMatchTickCount = m_pTableInfo->dwMatchIntervalTime * 1000;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Update State %u -> %u. m_dwMatchTickCount = %u", it->first, BUDOKAI_MINORMATCH_STATE_STAGE_READY, prelim->m_byMatchState, prelim->m_dwMatchTickCount);

					//this packet will show "START"
					CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_MINORMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_MINORMATCH_STATE_STAGE_RUN;
					res2->bIsEnter = true;
					res2->byStage = 0;
					res2->dwRemainTime = prelim->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
					prelim->m_pWorld->Broadcast(&packet2);
				}
			}
			break;
			case BUDOKAI_MINORMATCH_STATE_STAGE_RUN:
			{
				//progress fight
				ProcessMinorMatch(prelim, it->first, dwTickDif);

				if (prelim->m_dwMatchTickCount == 0)
				{
					prelim->m_dwMatchTickCount = 5000;

					DecidePrelimWinner(prelim, it->first);

					CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_MINORMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = prelim->m_byMatchState; //in "DecidePrelimWinner" we decide the next state
					res2->bIsEnter = true;
					res2->byStage = 0;
					res2->dwRemainTime = prelim->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
					prelim->m_pWorld->Broadcast(&packet2);

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Update State %u -> %u. m_dwMatchTickCount = %u", it->first, BUDOKAI_MINORMATCH_STATE_STAGE_RUN, prelim->m_byMatchState, prelim->m_dwMatchTickCount);
				}
			}
			break;
			case BUDOKAI_MINORMATCH_STATE_STAGE_SELECTION:
			{
				if (prelim->m_dwMatchTickCount == 0)
				{
					MinorMatchSelection(prelim, it->first);

					prelim->m_byMatchState = BUDOKAI_MINORMATCH_STATE_MATCH_FINISH;
					prelim->m_dwMatchTickCount = 3000;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Update State %u -> %u. m_dwMatchTickCount = %u", it->first, BUDOKAI_MINORMATCH_STATE_STAGE_SELECTION, prelim->m_byMatchState, prelim->m_dwMatchTickCount);

					CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_MINORMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_MINORMATCH_STATE_MATCH_FINISH;
					res2->bIsEnter = true;
					res2->byStage = 0;
					res2->dwRemainTime = prelim->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
					prelim->m_pWorld->Broadcast(&packet2);
				}
			}
			break;
			case BUDOKAI_MINORMATCH_STATE_STAGE_FINISH:
			{
				if (prelim->m_dwMatchTickCount == 0)
				{
					MinorMatchStageFinish(prelim, it->first);

					prelim->m_byMatchState = BUDOKAI_MINORMATCH_STATE_MATCH_FINISH;
					prelim->m_dwMatchTickCount = 1000;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Update State %u -> %u. m_dwMatchTickCount = %u", it->first, BUDOKAI_MINORMATCH_STATE_STAGE_FINISH, prelim->m_byMatchState, prelim->m_dwMatchTickCount);

					CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_MINORMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_MINORMATCH_STATE_MATCH_FINISH;
					res2->bIsEnter = true;
					res2->byStage = 0;
					res2->dwRemainTime = prelim->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
					prelim->m_pWorld->Broadcast(&packet2);
				}
			}
			break;
			case BUDOKAI_MINORMATCH_STATE_MATCH_FINISH:
			{
				if (prelim->m_dwMatchTickCount == 0)
				{
					MinorMatchMatchFinish(prelim, it->first);

					prelim->m_byMatchState = BUDOKAI_MINORMATCH_STATE_END;
					prelim->m_dwMatchTickCount = 15000;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Update State %u -> %u. m_dwMatchTickCount = %u", it->first, BUDOKAI_MINORMATCH_STATE_MATCH_FINISH, prelim->m_byMatchState, prelim->m_dwMatchTickCount);

					CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_MINORMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_MINORMATCH_STATE_END;
					res2->bIsEnter = true;
					res2->byStage = 0;
					res2->dwRemainTime = prelim->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
					prelim->m_pWorld->Broadcast(&packet2);
				}
			}
			break;

			case BUDOKAI_MINORMATCH_STATE_END:
			{
				if (prelim->m_dwMatchTickCount == 0)
				{
					MinorMatchEnd(prelim, it->first);

					delete prelim;
					it = m_mapPrelims.erase(it);
					continue;
				}
			}
			break;

			default: ERR_LOG(LOG_GENERAL, "Unknown match state %u", prelim->m_byMatchState);
		}

		++it;
	}
}


void CBudokaiManager::SendMinorMatchTeamInfo(BYTE byMatchIndex, sPRELIM_HEAD* prelim)
{
	sVARIABLE_DATA sData;
	sData.Init(2000);

	BYTE i = 0;
	WORD wTeamInfo_var = 0;

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		sMATCH_MINORMATCH_TEAM_INFO_VAR info[BUDOKAI_MAX_TOURNAMENT_TEAM_MATCH_COUNT];

		for (auto it = prelim->m_mapIndividualPrelim.begin(); it != prelim->m_mapIndividualPrelim.end(); it++)
		{
			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer && pPlayer->GetMatchIndex() == byMatchIndex)
			{
				auto it2 = m_mapIndividual.find(pPlayer->GetJoinID());
				if (it2 != m_mapIndividual.end())
				{
					info[i].wTeamType = (WORD) MAX_MATCH_TEAM_TYPE_COUNT + i;

					pPlayer->SetBudokaiTeamType(info[i].wTeamType);

					info[i].wTeamName_var = 0;
					info[i].bDojoRecommender = it2->second.bDojoRecommender;
					info[i].byMemberCount = 1;

					WCHAR			m_wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
					WCHAR			m_wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];

					NTL_SAFE_WCSCPY(m_wszCharName, pPlayer->GetCharName());
					NTL_SAFE_WCSCPY(m_wszGuildName, pPlayer->GetGuildName());

					sMATCH_MEMBER_STATE_INFO memberInfo[1];
					memberInfo[0].handle = pPlayer->GetID();
					memberInfo[0].byState = pPlayer->GetBudokaiPcState();
					memberInfo[0].byLevel = pPlayer->GetLevel();
					memberInfo[0].byClass = pPlayer->GetClass();
					memberInfo[0].wGuildName = sData.Write(sizeof(m_wszGuildName), m_wszGuildName);
					memberInfo[0].wWinCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwWin;
					memberInfo[0].wDrawCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwDraw;
					memberInfo[0].wLoseCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwLose;
					memberInfo[0].wCharName = sData.Write(sizeof(m_wszCharName), m_wszCharName);

					info[i].wTeamName_var = sData.Write(sizeof(m_wszCharName), m_wszCharName);
					info[i].wMembers_var = sData.Write(sizeof(memberInfo), memberInfo);

					++i;

					if (prelim->m_byTotalCount > 1)
						MinorMatchSpawnPlayerRandomPlace(pPlayer, false);
				}
			}
		}

		wTeamInfo_var = sData.Write(i * sizeof(sMATCH_MINORMATCH_TEAM_INFO_VAR), info);
	}
	else
	{
		sMATCH_MINORMATCH_TEAM_INFO_VAR info[BUDOKAI_MINOR_MATCH_TEAM_COUNT];
		WCHAR			charName[NTL_MAX_SIZE_CHAR_NAME + 1];
		WCHAR			partyName[NTL_MAX_SIZE_PARTY_NAME + 1];

		for (std::map<JOINID, sPARTY_INFO>::iterator it = prelim->m_mapTeamPrelim.begin(); it != prelim->m_mapTeamPrelim.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;

			sMATCH_MEMBER_STATE_INFO memberInfo[NTL_MAX_MEMBER_IN_PARTY];
			memset(memberInfo, -1, sizeof(memberInfo));

			info[i].wTeamType = (WORD)MAX_MATCH_TEAM_TYPE_COUNT + i;

			partyInfo->teamType = info[i].wTeamType;

			info[i].wTeamName_var = 0;
			info[i].bDojoRecommender = false;
			info[i].byMemberCount = 0;
			
			NTL_SAFE_WCSCPY(partyName, partyInfo->m_wszTeamName);
			info[i].wTeamName_var = sData.Write(sizeof(partyName), partyName);

			for (BYTE memberCnt = 0; memberCnt < partyInfo->byMemberCount; memberCnt++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[memberCnt]);
				if (pPlayer && pPlayer->GetCharID() == partyInfo->charId[memberCnt])
				{
					NTL_SAFE_WCSCPY(charName, pPlayer->GetCharName());

					pPlayer->SetBudokaiTeamType(info[i].wTeamType);

					memberInfo[info[i].byMemberCount].handle = pPlayer->GetID();
					memberInfo[info[i].byMemberCount].byState = pPlayer->GetBudokaiPcState();
					memberInfo[info[i].byMemberCount].byLevel = pPlayer->GetLevel();
					memberInfo[info[i].byMemberCount].byClass = pPlayer->GetClass();
					memberInfo[info[i].byMemberCount].wGuildName = INVALID_WORD;
					memberInfo[info[i].byMemberCount].wWinCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwWin;
					memberInfo[info[i].byMemberCount].wDrawCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwDraw;
					memberInfo[info[i].byMemberCount].wLoseCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwLose;
					memberInfo[info[i].byMemberCount++].wCharName = sData.Write(sizeof(charName), charName);

					if (prelim->m_byTotalCount > 5)
						MinorMatchSpawnPlayerRandomPlace(pPlayer, false);
				}
			}

			info[i].wMembers_var = sData.Write(info[i].byMemberCount * sizeof(sMATCH_MEMBER_STATE_INFO), memberInfo);

			++i;
		}

		wTeamInfo_var = sData.Write(i * sizeof(sMATCH_MINORMATCH_TEAM_INFO_VAR), info);
	}

	WORD wPacketSize = sData.GetPacketSize(sizeof(sGU_MATCH_MINORMATCH_TEAM_INFO_NFY));

	CNtlPacket packet(wPacketSize);
	sGU_MATCH_MINORMATCH_TEAM_INFO_NFY * res = (sGU_MATCH_MINORMATCH_TEAM_INFO_NFY *)packet.GetPacketData();
	res->wOpCode = GU_MATCH_MINORMATCH_TEAM_INFO_NFY;
	res->byMatchIndex = byMatchIndex;
	res->byTeamCount = i;
	res->wTeamInfo_var = wTeamInfo_var;
	sData.CopyTo(&res->sData, sData.GetDataSize());
	prelim->m_pWorld->Broadcast(&packet);

	ERR_LOG(LOG_GENERAL, "BUDOKAI: wPacketSize %u, wTeamInfo_var %u, GetDataSize() %u, GetTotalSize %u, byTeamCount %u", wPacketSize, wTeamInfo_var, sData.GetDataSize(), sData.GetTotalSize(), i);
}

void CBudokaiManager::MinorMatchUpdatePlayersState(sPRELIM_HEAD* prelim, BYTE byMatchIndex, BYTE byState)
{
	ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Update all players state to %u", byMatchIndex, byState);

	for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = prelim->m_mapIndividualPrelim.begin(); it != prelim->m_mapIndividualPrelim.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
		if (pPlayer)
		{
			if (pPlayer->GetCharID() == it->second.charId)
			{
				if (pPlayer->GetMatchIndex() == byMatchIndex)
				{
					pPlayer->SetBudokaiPcState(byState);

					CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY));
					sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_MINORMATCH_PLAYER_STATE_NFY;
					res2->hPc = it->first;
					res2->byPcState = byState;
					packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY));
					prelim->m_pWorld->Broadcast(&packet2);
				}
				else ERR_LOG(LOG_GENERAL, "Player has different match index %u != %u", pPlayer->GetMatchIndex(), byMatchIndex);
			}
			else ERR_LOG(LOG_GENERAL, "Player has different char id %u != %u", pPlayer->GetCharID(), it->second.charId);
		}
		else ERR_LOG(LOG_GENERAL, "Could not find player");
	}
}

void CBudokaiManager::ProcessMinorMatch(sPRELIM_HEAD* prelim, BYTE byMatchIndex, DWORD dwTickDif)
{
	for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = prelim->m_mapIndividualPrelim.begin(); it != prelim->m_mapIndividualPrelim.end(); it++)
	{
		sPLAYER_INFO& info = it->second;

		CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
		if (pPlayer)
		{
			if (pPlayer->GetCharID() == info.charId)
			{
				if (pPlayer->GetMatchIndex() == byMatchIndex)
				{
					if (info.dwTick == 0 && pPlayer->GetBudokaiPcState() == MATCH_MEMBER_STATE_NORMAL)
					{
						if (IsInsideArena(pPlayer->GetCurLoc(), pPlayer->GetCurWorld()) == false)
						{
							info.dwTick = 9000;

							pPlayer->SetBudokaiPcState(MATCH_MEMBER_STATE_OUTOFAREA);

							CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY));
							sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
							res2->wOpCode = GU_MATCH_MINORMATCH_PLAYER_STATE_NFY;
							res2->hPc = pPlayer->GetID();
							res2->byPcState = MATCH_MEMBER_STATE_OUTOFAREA;
							packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY));
							prelim->m_pWorld->Broadcast(&packet2);
						}
					}
					else if (info.dwTick > 0)
					{
						info.dwTick = UnsignedSafeDecrease<DWORD>(info.dwTick, dwTickDif);

						if (info.dwTick == 0)
						{
							MinorMatchSpawnPlayerRandomPlace(pPlayer, true);
						}
					}
				}
			}
		}
	}
}

void CBudokaiManager::DecidePrelimWinner(sPRELIM_HEAD* prelim, BYTE byMatchIndex)
{
	ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Time Out. Decide Winner !", byMatchIndex);

	BYTE byCount = 0;
	std::map<unsigned int, BYTE> mapKills;
	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = prelim->m_mapIndividualPrelim.begin(); it != prelim->m_mapIndividualPrelim.end(); it++)
		{
			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer)
			{
				if (pPlayer->GetCharID() == it->second.charId)
				{
					if (pPlayer->GetMatchIndex() == byMatchIndex)
					{
						mapKills.insert({ it->first, it->second.byKills });

						//update players state to normal
						pPlayer->SetBudokaiPcState(MATCH_MEMBER_STATE_NONE);
						CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY));
						sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
						res2->wOpCode = GU_MATCH_MINORMATCH_PLAYER_STATE_NFY;
						res2->hPc = it->first;
						res2->byPcState = MATCH_MEMBER_STATE_NONE;
						packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY));
						prelim->m_pWorld->Broadcast(&packet2);
					}
				}
			}
		}

		// -- Check if there are people with same amount of kills
		auto x = std::max_element(mapKills.begin(), mapKills.end(),
			[](const std::pair<HOBJECT, BYTE>& p1, const std::pair<HOBJECT, BYTE>& p2) {
			return p1.second < p2.second; });

		prelim->m_mapSelection.insert({x->first, 0});

		ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Player %u has the most kills %u", byMatchIndex, x->first, x->second);
		
		//check if some other have the same amount of kills... Then add to selection
		for (std::map<HOBJECT, BYTE>::iterator it = mapKills.begin(); it != mapKills.end(); it++)
		{
			if (it->first != x->first && it->second == x->second)
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Player %u has the same amount of kills %u as the leader %u", byMatchIndex, it->first, x->second, x->first);
				prelim->m_mapSelection.insert({ it->first, 0 });
			}
		}
	}
	else if (m_matchType == BUDOKAI_MATCH_TYPE_TEAM)
	{
		std::map<unsigned int, BYTE> mapMemberCount;

		for (std::map<JOINID, sPARTY_INFO>::iterator it = prelim->m_mapTeamPrelim.begin(); it != prelim->m_mapTeamPrelim.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;

			mapKills.insert({ it->first, partyInfo->byKills });
			mapMemberCount.insert({ it->first, partyInfo->byMemberCount });

			for (BYTE i = 0; i < partyInfo->byMemberCount; i++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[i]);
				if (pPlayer)
				{
					if (pPlayer->GetCharID() == partyInfo->charId[i])
					{
						if (pPlayer->GetMatchIndex() == byMatchIndex)
						{
							//update players state to normal
							pPlayer->SetBudokaiPcState(MATCH_MEMBER_STATE_NONE);

							CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY));
							sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
							res2->wOpCode = GU_MATCH_MINORMATCH_PLAYER_STATE_NFY;
							res2->hPc = it->first;
							res2->byPcState = MATCH_MEMBER_STATE_NONE;
							packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY));
							prelim->m_pWorld->Broadcast(&packet2);
						}
					}
				}
			}
		}

		// -- Check if there are people with same amount of kills
		auto x = std::max_element(mapKills.begin(), mapKills.end(),
			[](const std::pair<unsigned int, BYTE>& p1, const std::pair<unsigned int, BYTE>& p2) {
			return p1.second < p2.second; });

		prelim->m_mapSelection.insert({ x->first, 0 });

		ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Party %u has the most kills %u", byMatchIndex, x->first, x->second);

		//check if some other have the same amount of kills... Then add to selection
		for (std::map<unsigned int, BYTE>::iterator it = mapKills.begin(); it != mapKills.end(); it++)
		{
			if (it->first != x->first && it->second == x->second)
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Party %u has the same amount of kills %u as the leader %u", byMatchIndex, it->first, x->second, x->first);
				prelim->m_mapSelection.insert({ it->first, 0 });
			}
		}

		if (prelim->m_mapSelection.size() > 1 && x->second == 0) // if multiple team inside and everyone has 0 kills then filter m_mapSelection with who have the most members
		{
			prelim->m_mapSelection.clear();

			// -- Get team with most members
			auto y = std::max_element(mapMemberCount.begin(), mapMemberCount.end(),
				[](const std::pair<unsigned int, BYTE>& p1, const std::pair<unsigned int, BYTE>& p2) {
				return p1.second < p2.second; });

			prelim->m_mapSelection.insert({ y->first, 0 });

			//check if some other have the same amount of members... Then add to selection
			for (std::map<unsigned int, BYTE>::iterator it = mapMemberCount.begin(); it != mapMemberCount.end(); it++)
			{
				if (it->first != y->first && it->second == y->second)
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Party %u has the same amount of members %u as the leader %u", byMatchIndex, it->first, y->second, y->first);
					prelim->m_mapSelection.insert({ it->first, 0 });
				}
			}
		}
	}
	else
	{
		// DEBUG
	}

	// -- Decide if we go selection or nah
	if (prelim->m_mapSelection.size() == 1)
	{
		prelim->m_byMatchState = BUDOKAI_MINORMATCH_STATE_STAGE_FINISH;
	}
	else
	{
		prelim->m_byMatchState = BUDOKAI_MINORMATCH_STATE_STAGE_SELECTION;
	}
}

void CBudokaiManager::MinorMatchSelection(sPRELIM_HEAD* prelim, BYTE byMatchIndex)
{
	CNtlPacket packet(sizeof(sGU_MATCH_MINORMATCH_SELECTION_NFY));
	sGU_MATCH_MINORMATCH_SELECTION_NFY * res = (sGU_MATCH_MINORMATCH_SELECTION_NFY *)packet.GetPacketData();
	res->wOpCode = GU_MATCH_MINORMATCH_SELECTION_NFY;
	res->byTeamCount = 0;
	res->teamWinner = INVALID_TEAMTYPE;

	//dice manager
	CDiceManager dice;

	std::map<unsigned int, BYTE>::iterator winner = prelim->m_mapSelection.begin();

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		for (std::map<unsigned int, BYTE>::iterator it = winner; it != prelim->m_mapSelection.end(); it++)
		{
			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer)
			{
				if (pPlayer->GetMatchIndex() == byMatchIndex)
				{
					it->second = dice.RollDice();

					if (it->second >= winner->second)
					{
						winner = it;
						res->teamWinner = pPlayer->GetBudokaiTeamType();
						prelim->wPrelimWinner = pPlayer->GetJoinID();
					}

					res->aSelection[res->byTeamCount].teamType = pPlayer->GetBudokaiTeamType();
					res->aSelection[res->byTeamCount++].byNumber = it->second;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Selection: Player %u, dice %u", byMatchIndex, it->first, it->second);
				}
			}
		}

		ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Selection Winner: Player = %u, dice = %u, byTeamCount = %u", byMatchIndex, winner->first, winner->second, res->byTeamCount);
	}
	else
	{
		for (std::map<unsigned int, BYTE>::iterator it = winner; it != prelim->m_mapSelection.end(); it++)
		{
			std::map<JOINID, sPARTY_INFO>::iterator it2 = prelim->m_mapTeamPrelim.find(it->first);
			if (it2 != prelim->m_mapTeamPrelim.end())
			{
				sPARTY_INFO* partyInfo = &it2->second;

				it->second = dice.RollDice();

				if (it->second >= winner->second)
				{
					winner = it;
					res->teamWinner = partyInfo->teamType;
					prelim->wPrelimWinner = partyInfo->joinId;
				}

				res->aSelection[res->byTeamCount].teamType = partyInfo->teamType;
				res->aSelection[res->byTeamCount++].byNumber = it->second;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Selection: Player %u, dice %u", byMatchIndex, it->first, it->second);
			}
		}

		ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Selection Winner: Party = %u, dice = %u, byTeamCount = %u", byMatchIndex, winner->first, winner->second, res->byTeamCount);
	}
	
	packet.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_SELECTION_NFY));
	prelim->m_pWorld->Broadcast(&packet);
}


void CBudokaiManager::MinorMatchStageFinish(sPRELIM_HEAD* prelim, BYTE byMatchIndex)
{
	CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_STAGE_FINISH_NFY));
	sGU_MATCH_MINORMATCH_STAGE_FINISH_NFY * res2 = (sGU_MATCH_MINORMATCH_STAGE_FINISH_NFY *)packet2.GetPacketData();
	res2->wOpCode = GU_MATCH_MINORMATCH_STAGE_FINISH_NFY;
	res2->wStageWinner = INVALID_TEAMTYPE;
	res2->byStageResult = INVALID_MATCH_RESULT;

	std::map<unsigned int, BYTE>::iterator winner = prelim->m_mapSelection.begin();

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(winner->first);
		if (pPlayer)
		{
			if (pPlayer->GetMatchIndex() == byMatchIndex)
			{
				res2->wStageWinner = pPlayer->GetBudokaiTeamType();
				res2->byStageResult = MATCH_RESULT_WIN;

				prelim->wPrelimWinner = pPlayer->GetJoinID();
			}
		}
	}
	else if (m_matchType == BUDOKAI_MATCH_TYPE_TEAM)
	{
		std::map<JOINID, sPARTY_INFO>::iterator it2 = prelim->m_mapTeamPrelim.find(winner->first);
		if (it2 != prelim->m_mapTeamPrelim.end())
		{
			sPARTY_INFO* partyInfo = &it2->second;

			res2->wStageWinner = partyInfo->teamType;
			res2->byStageResult = MATCH_RESULT_WIN;

			prelim->wPrelimWinner = partyInfo->joinId;
		}
	}
	else
	{
		// debug
	}

	ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Winner = %u, TeamType = %u", byMatchIndex, prelim->wPrelimWinner, res2->wStageWinner);

	packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_STAGE_FINISH_NFY));
	prelim->m_pWorld->Broadcast(&packet2);
}

void CBudokaiManager::MinorMatchMatchFinish(sPRELIM_HEAD* prelim, BYTE byMatchIndex)
{
	CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_MATCH_FINISH_NFY));
	sGU_MATCH_MINORMATCH_MATCH_FINISH_NFY * res2 = (sGU_MATCH_MINORMATCH_MATCH_FINISH_NFY *)packet2.GetPacketData();
	res2->wOpCode = GU_MATCH_MINORMATCH_MATCH_FINISH_NFY;
	res2->wMatchWinner = INVALID_MATCH_TEAM_TYPE;
	res2->byMatchResult = INVALID_MATCH_RESULT;
	res2->byKillCount = 0;

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		auto it2 = m_mapIndividual.find(prelim->wPrelimWinner);
		if (it2 != m_mapIndividual.end())
		{
			CPlayer* pPlayer = g_pObjectManager->FindByChar(it2->second.charId);
			if (pPlayer)
			{
				if (pPlayer->GetMatchIndex() == byMatchIndex)
				{
					res2->wMatchWinner = pPlayer->GetBudokaiTeamType();

					//Add prelim winner to tournament
					sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA* data = new sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA;
					data->bDojoRecommender = it2->second.bDojoRecommender;
					data->byClass = pPlayer->GetClass();
					data->byLevel = pPlayer->GetLevel();
					data->byMinorMatch_MatchIndex = it2->second.byMinorMatch_MatchIndex;
					data->charId = pPlayer->GetCharID();
					data->fRankPoint = it2->second.fPoint;
					data->wJoinId = it2->first;
					data->wRankBattleDrawCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwDraw;
					data->wRankBattleLoseCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwLose;
					data->wRankBattleWinCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwWin;
					NTL_SAFE_WCSCPY(data->wszCharName, pPlayer->GetCharName());
					NTL_SAFE_WCSCPY(data->wszGuildName, pPlayer->GetGuildName());

					m_mapIndividualTournament.insert({ data->wJoinId , data });

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Add Player to Tournament. CharID = %u, bDojoRecommender = %u, fRankPoint = %f, wJoinId = %u",
						byMatchIndex, data->charId, data->bDojoRecommender, data->fRankPoint, data->wJoinId);

					std::map<HOBJECT, sPLAYER_INFO>::iterator it = prelim->m_mapIndividualPrelim.find(pPlayer->GetID());
					if (it != prelim->m_mapIndividualPrelim.end())
					{
						res2->byMatchResult = MATCH_RESULT_WIN;
						res2->byKillCount = it->second.byKills;
					}
				}
			}
		}
	}
	else if (m_matchType == BUDOKAI_MATCH_TYPE_TEAM)
	{
		auto it2 = m_mapTeam.find(prelim->wPrelimWinner);
		if (it2 != m_mapTeam.end())
		{
			sBUDOKAI_JOIN_INFO* pJoinInfo = &m_mapJoinInfo.find(prelim->wPrelimWinner)->second;

			//Add prelim winner to tournament
			sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* data = new sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA;
			data->byMinorMatch_MatchIndex = it2->second.byMinorMatch_MatchIndex;
			data->wJoinId = it2->second.wJoinId;
			NTL_SAFE_WCSCPY(data->wszTeamName, it2->second.wszTeamName);
			data->byMemberCount = 0;

			for (BYTE byMemberCount = 0; byMemberCount < NTL_MAX_MEMBER_IN_PARTY; byMemberCount++)
			{
				if (it2->second.aMembers[byMemberCount] == INVALID_CHARACTERID || it2->second.aMembers[byMemberCount] == 0)
					break;

				data->aMembers[data->byMemberCount].charId = it2->second.aMembers[byMemberCount];
				data->aMembers[data->byMemberCount].byClass = pJoinInfo->sTeamInfo.aMemberInfo[byMemberCount].byClass;
				data->aMembers[data->byMemberCount].byLevel = pJoinInfo->sTeamInfo.aMemberInfo[byMemberCount].byLevel;
				data->aMembers[data->byMemberCount].fRankPoint = pJoinInfo->sTeamInfo.aMemberInfo[byMemberCount].fPoint;
				NTL_SAFE_WCSCPY(data->aMembers[data->byMemberCount].wszCharName, pJoinInfo->sTeamInfo.aMemberInfo[byMemberCount].wszName);

				++data->byMemberCount;
			}

			m_mapTeamTournament.insert({ data->wJoinId , data });

			ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Add Party to Tournament. wJoinId = %u", byMatchIndex, data->wJoinId);

			std::map<JOINID, sPARTY_INFO>::iterator it = prelim->m_mapTeamPrelim.find(prelim->wPrelimWinner);
			if (it != prelim->m_mapTeamPrelim.end())
			{
				res2->byMatchResult = MATCH_RESULT_WIN;
				res2->byKillCount = it->second.byKills;
				res2->wMatchWinner = it->second.teamType;
			}
		}
	}
	else
	{
		// debug
	}

	packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_MATCH_FINISH_NFY));
	prelim->m_pWorld->Broadcast(&packet2);


	// -- Give Winner Reward
	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = prelim->m_mapIndividualPrelim.begin(); it != prelim->m_mapIndividualPrelim.end(); it++)
		{
			sPLAYER_INFO& info = it->second;

			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer)
			{
				if (pPlayer->GetMatchIndex() == byMatchIndex)
				{
					DWORD dwMudosaPoints = 0;
					TBLIDX itemIdx = INVALID_TBLIDX;
					BYTE byItemCount = 0;

					if (pPlayer->GetJoinID() == prelim->wPrelimWinner) //prelim winner receives different reward
					{
						m_mapPrelimWinners.insert({ byMatchIndex, std::make_pair(pPlayer->GetJoinID(), pPlayer->GetCharID()) });

						if (m_type == BUDOKAI_TYPE_JUNIOR)
						{
							dwMudosaPoints = m_pTableInfo->sJuniorIndividualReward.sMinorMatch.dwBaseMudosaPoint + m_pTableInfo->sJuniorIndividualReward.sMinorMatch.dwWinnerMudosaPoint + (m_pTableInfo->sJuniorIndividualReward.sMinorMatch.dwKillCountPoint * (DWORD)info.byKills);
							itemIdx = m_pTableInfo->sJuniorIndividualReward.sMinorMatch.winnerItem;
							byItemCount = m_pTableInfo->sJuniorIndividualReward.sMinorMatch.byWinerItemStackCount;
						}
						else
						{
							dwMudosaPoints = m_pTableInfo->sIndividualReward.sMinorMatch.dwBaseMudosaPoint + m_pTableInfo->sIndividualReward.sMinorMatch.dwWinnerMudosaPoint + (m_pTableInfo->sIndividualReward.sMinorMatch.dwKillCountPoint * (DWORD)info.byKills);
							itemIdx = m_pTableInfo->sIndividualReward.sMinorMatch.winnerItem;
							byItemCount = m_pTableInfo->sIndividualReward.sMinorMatch.byWinerItemStackCount;
						}
					}
					else
					{
						if (m_type == BUDOKAI_TYPE_JUNIOR)
						{
							dwMudosaPoints = m_pTableInfo->sJuniorIndividualReward.sMinorMatch.dwBaseMudosaPoint + (m_pTableInfo->sJuniorIndividualReward.sMinorMatch.dwKillCountPoint * (DWORD)info.byKills);
							itemIdx = m_pTableInfo->sJuniorIndividualReward.sMinorMatch.loserItem;
							byItemCount = m_pTableInfo->sJuniorIndividualReward.sMinorMatch.byLoserItemStackCount;
						}
						else
						{
							dwMudosaPoints = m_pTableInfo->sIndividualReward.sMinorMatch.dwBaseMudosaPoint + (m_pTableInfo->sIndividualReward.sMinorMatch.dwKillCountPoint * (DWORD)info.byKills);
							itemIdx = m_pTableInfo->sIndividualReward.sMinorMatch.loserItem;
							byItemCount = m_pTableInfo->sIndividualReward.sMinorMatch.byLoserItemStackCount;
						}
					}

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. CharID %u Receive Prelim Reward. dwMudosaPoints = %u, itemIdx = %u, byItemCount = %u, byKills = %u",
						byMatchIndex, pPlayer->GetCharID(), dwMudosaPoints, itemIdx, byItemCount, info.byKills);

					pPlayer->UpdateMudosaPoints(pPlayer->GetMudosaPoints() + dwMudosaPoints, true);

					if(itemIdx != INVALID_TBLIDX)
						g_pItemManager->CreateQuestRewardItem(pPlayer, itemIdx, byItemCount);
				}
			}
		}
	}
	else if (m_matchType == BUDOKAI_MATCH_TYPE_TEAM)
	{
		for (std::map<JOINID, sPARTY_INFO>::iterator it = prelim->m_mapTeamPrelim.begin(); it != prelim->m_mapTeamPrelim.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;
			
			DWORD dwMudosaPoints = 0;
			TBLIDX itemIdx = INVALID_TBLIDX;
			BYTE byItemCount = 0;

			if (partyInfo->joinId == prelim->wPrelimWinner) //prelim winner receives different reward
			{
				m_mapPrelimWinners.insert({ byMatchIndex, std::make_pair(partyInfo->joinId, partyInfo->joinId) });

				if (m_type == BUDOKAI_TYPE_JUNIOR)
				{
					dwMudosaPoints = m_pTableInfo->sJuniorTeamReward.sMinorMatch.dwBaseMudosaPoint + m_pTableInfo->sJuniorTeamReward.sMinorMatch.dwWinnerMudosaPoint + (m_pTableInfo->sJuniorTeamReward.sMinorMatch.dwKillCountPoint * (DWORD)partyInfo->byKills);
					itemIdx = m_pTableInfo->sJuniorTeamReward.sMinorMatch.winnerItem;
					byItemCount = m_pTableInfo->sJuniorTeamReward.sMinorMatch.byWinerItemStackCount;
				}
				else
				{
					dwMudosaPoints = m_pTableInfo->sTeamReward.sMinorMatch.dwBaseMudosaPoint + m_pTableInfo->sTeamReward.sMinorMatch.dwWinnerMudosaPoint + (m_pTableInfo->sTeamReward.sMinorMatch.dwKillCountPoint * (DWORD)partyInfo->byKills);
					itemIdx = m_pTableInfo->sTeamReward.sMinorMatch.winnerItem;
					byItemCount = m_pTableInfo->sTeamReward.sMinorMatch.byWinerItemStackCount;
				}
			}
			else
			{
				if (m_type == BUDOKAI_TYPE_JUNIOR)
				{
					dwMudosaPoints = m_pTableInfo->sJuniorTeamReward.sMinorMatch.dwBaseMudosaPoint + (m_pTableInfo->sJuniorTeamReward.sMinorMatch.dwKillCountPoint * (DWORD)partyInfo->byKills);
					itemIdx = m_pTableInfo->sJuniorTeamReward.sMinorMatch.loserItem;
					byItemCount = m_pTableInfo->sJuniorTeamReward.sMinorMatch.byLoserItemStackCount;
				}
				else
				{
					dwMudosaPoints = m_pTableInfo->sTeamReward.sMinorMatch.dwBaseMudosaPoint + (m_pTableInfo->sTeamReward.sMinorMatch.dwKillCountPoint * (DWORD)partyInfo->byKills);
					itemIdx = m_pTableInfo->sTeamReward.sMinorMatch.loserItem;
					byItemCount = m_pTableInfo->sTeamReward.sMinorMatch.byLoserItemStackCount;
				}
			}

			ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Party %u Receive Prelim Reward. dwMudosaPoints = %u, itemIdx = %u, byItemCount = %u, byKills = %u",
				byMatchIndex, partyInfo->joinId, dwMudosaPoints, itemIdx, byItemCount, it->second);

			for (BYTE byMemberCount = 0; byMemberCount < partyInfo->byMemberCount; byMemberCount++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[byMemberCount]);
				if (pPlayer && partyInfo->charId[byMemberCount] == pPlayer->GetCharID() && pPlayer->GetMatchIndex() == byMatchIndex)
				{
					pPlayer->UpdateMudosaPoints(pPlayer->GetMudosaPoints() + dwMudosaPoints, true);

					if (itemIdx != INVALID_TBLIDX)
						g_pItemManager->CreateQuestRewardItem(pPlayer, itemIdx, byItemCount);
				}
			}
		}
	}
	else
	{
		// debug
	}
}

void CBudokaiManager::MinorMatchEnd(sPRELIM_HEAD* prelim, BYTE byMatchIndex)
{
	CGameServer* app = (CGameServer*)g_pApp;

	ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. End. ", byMatchIndex);

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = prelim->m_mapIndividualPrelim.begin(); it != prelim->m_mapIndividualPrelim.end(); it++)
		{
			sPLAYER_INFO& info = it->second;

			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer)
			{
				if (pPlayer->GetMatchIndex() == byMatchIndex)
				{
					CNtlPacket packet(sizeof(sGQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ));
					sGQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ * res = (sGQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ *)packet.GetPacketData();
					res->wOpCode = GQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ;
					res->wJoinId = pPlayer->GetJoinID();
					res->byMatchIndex = byMatchIndex;
					res->byWinnerJoinResult = INVALID_BUDOKAI_JOIN_RESULT;

					if (pPlayer->GetJoinID() == prelim->wPrelimWinner) //prelim winner will stay in budokai channel and add to torunament
					{
						std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(pPlayer->GetJoinID());
						if (it != m_mapJoinInfo.end())
						{
							ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Player %u JoinID %u. Set JoinResult = BUDOKAI_JOIN_RESULT_ENTER_32 ", byMatchIndex, pPlayer->GetCharID(), prelim->wPrelimWinner);

							it->second.byJoinResult = BUDOKAI_JOIN_RESULT_ENTER_32;
							res->byWinnerJoinResult = BUDOKAI_JOIN_RESULT_ENTER_32;
						}
						else ERR_LOG(LOG_GENERAL, "Could not find prelim winner in m_mapJoinInfo");
					}
					else
					{
						std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(pPlayer->GetJoinID());
						if (it != m_mapJoinInfo.end())
						{
							ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Player %u. Set byJoinState = BUDOKAI_JOIN_STATE_DROPOUT, JoinResult = INVALID_BUDOKAI_JOIN_RESULT ", byMatchIndex, pPlayer->GetCharID());

							it->second.byJoinState = BUDOKAI_JOIN_STATE_DROPOUT;
							it->second.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;

							res->byLoserJoinState = BUDOKAI_JOIN_STATE_DROPOUT;
							res->byLoserResultCondition = INVALID_BUDOKAI_JOIN_RESULT;
						}
						else ERR_LOG(LOG_GENERAL, "Could not find player in m_mapJoinInfo");
					}

					packet.SetPacketLen(sizeof(sGQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ));
					app->SendTo(app->GetQueryServerSession(), &packet);

					pPlayer->TeleportAnotherServer(prelim->m_pWorld->GetTbldat()->outWorldLoc, pPlayer->GetCurDir(), prelim->m_pWorld->GetTbldat()->outWorldTblidx, prelim->m_pWorld->GetTbldat()->outWorldTblidx, TELEPORT_TYPE_BUDOKAI, pPlayer->GetPrevChannelID(), 0, false);
				}
			}
		}
	}
	else if (m_matchType == BUDOKAI_MATCH_TYPE_TEAM)
	{
		for (std::map<JOINID, sPARTY_INFO>::iterator it = prelim->m_mapTeamPrelim.begin(); it != prelim->m_mapTeamPrelim.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;

			CNtlPacket packet(sizeof(sGQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ));
			sGQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ * res = (sGQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ *)packet.GetPacketData();
			res->wOpCode = GQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ;
			res->wJoinId = partyInfo->joinId;
			res->byMatchIndex = byMatchIndex;
			res->byWinnerJoinResult = INVALID_BUDOKAI_JOIN_RESULT;

			if (partyInfo->joinId == prelim->wPrelimWinner) //prelim winner will stay in budokai channel and add to torunament
			{
				std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(partyInfo->joinId);
				if (it != m_mapJoinInfo.end())
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Party %u Handle %u. Set JoinResult = BUDOKAI_JOIN_RESULT_ENTER_32 ", byMatchIndex, partyInfo->joinId, prelim->wPrelimWinner);

					it->second.byJoinResult = BUDOKAI_JOIN_RESULT_ENTER_32;
					res->byWinnerJoinResult = BUDOKAI_JOIN_RESULT_ENTER_32;
				}
				else ERR_LOG(LOG_GENERAL, "Could not find prelim winner in m_mapJoinInfo");
			}
			else
			{
				std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(partyInfo->joinId);
				if (it != m_mapJoinInfo.end())
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Party %u. Set byJoinState = BUDOKAI_JOIN_STATE_DROPOUT, JoinResult = INVALID_BUDOKAI_JOIN_RESULT ", byMatchIndex, partyInfo->joinId);

					it->second.byJoinState = BUDOKAI_JOIN_STATE_DROPOUT;
					it->second.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;

					res->byLoserJoinState = BUDOKAI_JOIN_STATE_DROPOUT;
					res->byLoserResultCondition = INVALID_BUDOKAI_JOIN_RESULT;
				}
				else ERR_LOG(LOG_GENERAL, "Could not find player in m_mapJoinInfo");
			}

			packet.SetPacketLen(sizeof(sGQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet);


			for (BYTE byMemberCount = 0; byMemberCount < partyInfo->byMemberCount; byMemberCount++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[byMemberCount]);
				if (pPlayer && partyInfo->charId[byMemberCount] == pPlayer->GetCharID() && pPlayer->GetMatchIndex() == byMatchIndex)
				{
					pPlayer->TeleportAnotherServer(prelim->m_pWorld->GetTbldat()->outWorldLoc, pPlayer->GetCurDir(), prelim->m_pWorld->GetTbldat()->outWorldTblidx, prelim->m_pWorld->GetTbldat()->outWorldTblidx, TELEPORT_TYPE_BUDOKAI, pPlayer->GetPrevChannelID(), 0, false);
				}
			}
		}
	}
	else
	{
		// debug
	}
}


void CBudokaiManager::MinorMatchSpawnPlayerRandomPlace(CPlayer * pPlayer, bool bSpawnFromFaint)
{
	if (pPlayer->GetCurWorld())
	{
		pPlayer->ChangeAttackProgress(false);
		pPlayer->SetAttackTarget(INVALID_HOBJECT);

		//remove buffs
		pPlayer->GetBuffManager()->RemoveAllBuff();

		//if transformed then cancel transformation
		if (pPlayer->GetTransformationTbldat())
			pPlayer->CancelTransformation();

		if (pPlayer->GetStateManager()->GetConditionState() > 0)
		{
			pPlayer->GetStateManager()->RemoveConditionFlags(INVALID_QWORD, true);
		}

		//re-calculate all stats
		pPlayer->GetCharAtt()->CalculateAll();

		//restore lp/rp/rp/rp balls
		pPlayer->UpdateCurLP(pPlayer->GetMaxLP(), true, false);
		pPlayer->UpdateCurEP(pPlayer->GetMaxEP(), true, false);
		pPlayer->UpdateRpBall(pPlayer->GetMaxRPBall(), false, false);
		pPlayer->UpdateCurRP(pPlayer->GetCharAtt()->GetMaxRP(), false, false);

		//remove target 
		pPlayer->ChangeTarget(INVALID_HOBJECT);

		//clear cc reduction
		pPlayer->ClearCrowdControlReduction();


		CNtlVector vRandLoc(pPlayer->GetCurLoc());
		vRandLoc.x = RandomRangeF(pPlayer->GetCurWorld()->GetTbldat()->vBattleStartLoc.x, pPlayer->GetCurWorld()->GetTbldat()->vBattleEndLoc.x);
		vRandLoc.z = RandomRangeF(pPlayer->GetCurWorld()->GetTbldat()->vBattleStartLoc.z, pPlayer->GetCurWorld()->GetTbldat()->vBattleEndLoc.z);

		if (bSpawnFromFaint)
		{
			pPlayer->SetIsReviving(true);

			pPlayer->SetBudokaiPcState(MATCH_MEMBER_STATE_RESCUE);

			CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY));
			sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
			res2->wOpCode = GU_MATCH_MINORMATCH_PLAYER_STATE_NFY;
			res2->hPc = pPlayer->GetID();
			res2->byPcState = pPlayer->GetBudokaiPcState();
			packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY));
			pPlayer->GetCurWorld()->Broadcast(&packet2);

			pPlayer->StartTeleport(vRandLoc, pPlayer->GetCurDir(), pPlayer->GetWorldID(), TELEPORT_TYPE_MINORMATCH);
		}
		else
		{
			pPlayer->SetCurLoc(vRandLoc);
			pPlayer->SendCharStateSpawning(TELEPORT_TYPE_MINORMATCH, true, false);

			pPlayer->SetBudokaiPcState(MATCH_MEMBER_STATE_NORMAL);
		}
	}
}

void CBudokaiManager::TickProcessMajorMatch(DWORD dwTickDif, BUDOKAITIME curTime)
{
	for (std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.begin(); it != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end(); it++)
	{
		sTOURNAMENT_MATCH* match = it->second;

		if (match->bEnd == true)
			continue;

		match->m_dwMatchTickCount = UnsignedSafeDecrease<DWORD>(match->m_dwMatchTickCount, dwTickDif);

		switch (match->m_byMatchState)
		{
		case BUDOKAI_MAJORMATCH_STATE_NONE:
		{
			if (match->m_dwMatchTickCount == 0)
			{
				match->m_byMatchState = BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Major Match. Index %u, byState BUDOKAI_MAJORMATCH_STATE_NONE -> BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH, m_dwMatchTickCount = %u",
					it->first, match->m_dwMatchTickCount);

				CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
				sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
				res2->wOpCode = GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY;
				res2->byMatchState = BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH;
				res2->bIsEnter = false; //false = the score picture goes away
				res2->byStage = match->byStage;
				res2->dwRemainTime = match->m_dwMatchTickCount;
				packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
				match->m_pWorld->Broadcast(&packet2);
			}
		}
		break;
		case BUDOKAI_MAJORMATCH_STATE_WAIT:
		{
			if (match->m_dwMatchTickCount == 0 || match->m_byCount >= match->m_byTotalCount)
			{
				if (match->m_byCount == 0 || match->m_byTotalCount == 0) //if none inside, then end directly
				{
					match->m_byMatchState = BUDOKAI_MAJORMATCH_STATE_END;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Major Match. Index %u, byState BUDOKAI_MAJORMATCH_STATE_WAIT -> BUDOKAI_MAJORMATCH_STATE_END, m_dwMatchTickCount = %u, m_byCount = %u, m_byTotalCount = %u",
						it->first, match->m_dwMatchTickCount, match->m_byCount, match->m_byTotalCount);
				}
				else //begin
				{
					match->m_byMatchState = BUDOKAI_MAJORMATCH_STATE_MATCH_READY;
					match->m_dwMatchTickCount = 5000;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Major Match. Index %u, byState BUDOKAI_MAJORMATCH_STATE_WAIT -> BUDOKAI_MAJORMATCH_STATE_MATCH_READY, m_dwMatchTickCount = %u",
						it->first, match->m_dwMatchTickCount);

					//send player info
					SendMajorMatchTeamInfo(it->first, match);

					//reset player shit
					ResetPlayerMajorMatch(match, it->first, false);

					CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_MAJORMATCH_STATE_MATCH_READY;
					res2->bIsEnter = true;
					res2->byStage = 0;
					res2->dwRemainTime = match->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
					match->m_pWorld->Broadcast(&packet2);
				}
			}
		}
		break;
		case BUDOKAI_MAJORMATCH_STATE_DIRECTION:
		{
			if (match->m_dwMatchTickCount == 0)
			{
				if(MajorMatchCheckPlayerCount(match, it->first) == true)
				{
					match->m_byMatchState = BUDOKAI_MAJORMATCH_STATE_STAGE_READY;
					match->m_dwMatchTickCount = 5000;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Major Match. Index %u, byState BUDOKAI_MAJORMATCH_STATE_DIRECTION -> BUDOKAI_MAJORMATCH_STATE_STAGE_READY, m_dwMatchTickCount = %u",
						it->first, match->m_dwMatchTickCount);

					CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_MAJORMATCH_STATE_STAGE_READY;
					res2->bIsEnter = true;
					res2->byStage = match->byStage;
					res2->dwRemainTime = match->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
					match->m_pWorld->Broadcast(&packet2);
				}
			}
		}
		break;
		case BUDOKAI_MAJORMATCH_STATE_MATCH_READY:
		{
			if (match->m_dwMatchTickCount == 0)
			{
				match->m_byMatchState = BUDOKAI_MAJORMATCH_STATE_DIRECTION;
				match->m_dwMatchTickCount = 25000;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Major Match. Index %u, byState BUDOKAI_MAJORMATCH_STATE_MATCH_READY -> BUDOKAI_MAJORMATCH_STATE_DIRECTION, m_dwMatchTickCount = %u",
					it->first, match->m_dwMatchTickCount);

				CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
				sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
				res2->wOpCode = GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY;
				res2->byMatchState = BUDOKAI_MAJORMATCH_STATE_DIRECTION;
				res2->bIsEnter = true;
				res2->byStage = match->byStage;
				res2->dwRemainTime = match->m_dwMatchTickCount;
				packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
				match->m_pWorld->Broadcast(&packet2);
			}
		}
		break;
		case BUDOKAI_MAJORMATCH_STATE_STAGE_READY:
		{
			if (match->m_dwMatchTickCount == 0)
			{
				match->m_byMatchState = BUDOKAI_MAJORMATCH_STATE_STAGE_RUN;

				if(m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
					match->m_dwMatchTickCount = 3 * 60 * 1000; // 3 minutes
				else
					match->m_dwMatchTickCount = 4 * 60 * 1000; // 4 minutes


				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Major Match. Index %u, byState BUDOKAI_MAJORMATCH_STATE_STAGE_READY -> BUDOKAI_MAJORMATCH_STATE_STAGE_RUN, m_dwMatchTickCount = %u",
					it->first, match->m_dwMatchTickCount);

				//update players state to normal
				MajorMatchUpdatePlayersState(match, it->first, MATCH_MEMBER_STATE_NORMAL);

				//this packet will show "START"
				CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
				sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
				res2->wOpCode = GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY;
				res2->byMatchState = BUDOKAI_MAJORMATCH_STATE_STAGE_RUN;
				res2->bIsEnter = true;
				res2->byStage = match->byStage;
				res2->dwRemainTime = match->m_dwMatchTickCount;
				packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
				match->m_pWorld->Broadcast(&packet2);
			}
		}
		break;
		case BUDOKAI_MAJORMATCH_STATE_STAGE_RUN:
		{
			if (MajorMatchCheckPlayerCount(match, it->first) == true)
			{
				//progress fight
				if (ProcessMajorMatch(match, it->first, dwTickDif) == true)
				{
					if (match->m_dwMatchTickCount == 0)
					{
						ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Major Match. Index %u, byState BUDOKAI_MAJORMATCH_STATE_STAGE_RUN -> BUDOKAI_MAJORMATCH_STATE_NONE, m_dwMatchTickCount = %u",
							it->first, match->m_dwMatchTickCount);

						MajorMatchTimeEnd(match, it->first);
					}
				}
			}
		}
		break;
		case BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH:
		{
			if (match->m_dwMatchTickCount == 0)
			{
				ResetPlayerMajorMatch(match, it->first, true);

				//check if match done. Otherwise continue
				if (match->bFinishMatch == false)
				{
					match->m_byMatchState = BUDOKAI_MAJORMATCH_STATE_STAGE_READY;
					match->m_dwMatchTickCount = 3000;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Major Match. Index %u, byState BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH -> BUDOKAI_MAJORMATCH_STATE_STAGE_READY, m_dwMatchTickCount = %u",
						it->first, match->m_dwMatchTickCount);

					CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_MAJORMATCH_STATE_STAGE_READY;
					res2->bIsEnter = true;
					res2->byStage = match->byStage;
					res2->dwRemainTime = match->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
					match->m_pWorld->Broadcast(&packet2);
				}
				else
				{
					match->m_byMatchState = BUDOKAI_MAJORMATCH_STATE_MATCH_FINISH;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Major Match. Index %u, byState BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH -> BUDOKAI_MAJORMATCH_STATE_MATCH_FINISH, m_dwMatchTickCount = %u",
						it->first, match->m_dwMatchTickCount);

					CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_MAJORMATCH_STATE_MATCH_FINISH;
					res2->bIsEnter = true; 
					res2->byStage = match->byStage;
					res2->dwRemainTime = match->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
					match->m_pWorld->Broadcast(&packet2);
				}
			}
		}
		break;
		case BUDOKAI_MAJORMATCH_STATE_MATCH_FINISH:
		{
			if (match->m_dwMatchTickCount == 0)
			{
				match->m_byMatchState = BUDOKAI_MAJORMATCH_STATE_END;
				match->m_dwMatchTickCount = 15000;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Major Match. Index %u, byState BUDOKAI_MAJORMATCH_STATE_MATCH_FINISH -> BUDOKAI_MAJORMATCH_STATE_END, m_dwMatchTickCount = %u",
					it->first, match->m_dwMatchTickCount);

				CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
				sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
				res2->wOpCode = GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY;
				res2->byMatchState = BUDOKAI_MAJORMATCH_STATE_END;
				res2->bIsEnter = true;
				res2->byStage = match->byStage;
				res2->dwRemainTime = match->m_dwMatchTickCount;
				packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
				match->m_pWorld->Broadcast(&packet2);

				MajorMatchMatchFinish(match, it->first);

				AddIndividualToNextRound(match, it->first, match->data.wWinnerTeam);
			}
		}
		break;

		case BUDOKAI_MAJORMATCH_STATE_END:
		{
			if (match->m_dwMatchTickCount == 0)
			{
				match->bEnd = true;
				MajorMatchEnd(match, it->first);

				m_aTournamentMatch[m_eMatchDepth].byMatchCountEnd += 1;
			}
		}
		break;

		default: ERR_LOG(LOG_GENERAL, "Unknown match state %u", match->m_byMatchState);
		}
	}
}

void CBudokaiManager::StartMajorMatchTeleport()
{
	CGameServer* app = (CGameServer*)g_pApp;

	ERR_LOG(LOG_GENERAL, "BUDOKAI: StartMajorMatchTeleport: m_eMatchDepth = %u", m_eMatchDepth);

	int i = 0;

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		CNtlPacket packet(sizeof(sGT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY));
		sGT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY * res = (sGT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY *)packet.GetPacketData();
		res->wOpCode = GT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY;
		res->byServerChannelId = DOJO_CHANNEL_INDEX;
		res->byServerIndex = 0;
		memset(res->aTeleportData, 0, sizeof(res->aTeleportData));

		for (std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.begin(); it != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end(); it++)
		{
			std::map<JOINID, sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA*>::iterator it2 = m_mapIndividualTournament.find(it->second->data.wJoinId1);
			if (it2 != m_mapIndividualTournament.end())
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: Success StartMajorMatchTeleport: i = %u, WorldTblix = %u, CharID = %u, WorldId = %u, JoinID1 = %u",
					i, m_pTableInfo->sIndividualWorldTblidx.majorMatch, it2->second->charId, it->second->m_pWorld->GetID(), it->second->data.wJoinId1);

				res->aTeleportData[i].worldTblidx = m_pTableInfo->sIndividualWorldTblidx.majorMatch;
				res->aTeleportData[i].byPlayerCount = 1;
				res->aTeleportData[i].aPlayers[0] = it2->second->charId;
				res->aTeleportData[i++].worldId = it->second->m_pWorld->GetID();
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: Failed StartMajorMatchTeleport: i = %u, WorldTblix = %u, JoinID1 = %u",
					i, m_pTableInfo->sIndividualWorldTblidx.majorMatch, it->second->data.wJoinId1);
			}

			if (it->second->data.wJoinId2 != INVALID_TEAMTYPE)
			{
				std::map<JOINID, sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA*>::iterator it3 = m_mapIndividualTournament.find(it->second->data.wJoinId2);
				if (it3 != m_mapIndividualTournament.end())
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: Success StartMajorMatchTeleport: i = %u, WorldTblix = %u, CharID = %u, WorldId = %u, JoinID2 = %u",
						i, m_pTableInfo->sIndividualWorldTblidx.majorMatch, it3->second->charId, it->second->m_pWorld->GetID(), it->second->data.wJoinId2);

					res->aTeleportData[i].worldTblidx = m_pTableInfo->sIndividualWorldTblidx.majorMatch;
					res->aTeleportData[i].byPlayerCount = 1;
					res->aTeleportData[i].aPlayers[0] = it3->second->charId;
					res->aTeleportData[i++].worldId = it->second->m_pWorld->GetID();
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: Failed StartMajorMatchTeleport: i = %u, WorldTblix = %u, JoinID2 = %u",
						i, m_pTableInfo->sIndividualWorldTblidx.majorMatch, it->second->data.wJoinId2);
				}
			}

			if (i == BUDOKAI_MAX_INDIVIDUAL_MATCH_COUNT)
				break;
		}

		packet.SetPacketLen(sizeof(sGT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY));
		app->SendTo(app->GetChatServerSession(), &packet);
	}
	else
	{
		CNtlPacket packet(sizeof(sGT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY));
		sGT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY * res = (sGT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY *)packet.GetPacketData();
		res->wOpCode = GT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY;
		res->byServerChannelId = DOJO_CHANNEL_INDEX;
		res->byServerIndex = 0;
		memset(res->aTeleportData, 0, sizeof(res->aTeleportData));

		for (std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.begin(); it != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end(); it++)
		{
			std::map<JOINID, sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA*>::iterator it2 = m_mapTeamTournament.find(it->second->data.wJoinId1);
			if (it2 != m_mapTeamTournament.end())
			{
				res->aTeleportData[i].worldTblidx = m_pTableInfo->sTeamWorldTblidx.majorMatch;
				res->aTeleportData[i].byTeamCount = 1;

				for (BYTE memberCnt = 0; memberCnt < it2->second->byMemberCount; memberCnt++)
				{
					res->aTeleportData[i].aTeamList[0].charId[res->aTeleportData[i].aTeamList[0].byCount] = it2->second->aMembers[memberCnt].charId;
					res->aTeleportData[i].aTeamList[0].byCount++;
				}

				res->aTeleportData[i++].worldId = it->second->m_pWorld->GetID();
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: Failed StartMajorMatchTeleport: i = %u, WorldTblix = %u, JoinID1 = %u",
					i, m_pTableInfo->sTeamWorldTblidx.majorMatch, it->second->data.wJoinId1);
			}

			if (it->second->data.wJoinId2 != INVALID_TEAMTYPE)
			{
				std::map<JOINID, sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA*>::iterator it3 = m_mapTeamTournament.find(it->second->data.wJoinId2);
				if (it3 != m_mapTeamTournament.end())
				{
					res->aTeleportData[i].worldTblidx = m_pTableInfo->sTeamWorldTblidx.majorMatch;
					res->aTeleportData[i].byTeamCount = 1;

					for (BYTE memberCnt = 0; memberCnt < it3->second->byMemberCount; memberCnt++)
					{
						res->aTeleportData[i].aTeamList[0].charId[res->aTeleportData[i].aTeamList[0].byCount] = it3->second->aMembers[memberCnt].charId;
						res->aTeleportData[i].aTeamList[0].byCount++;
					}

					res->aTeleportData[i++].worldId = it->second->m_pWorld->GetID();
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: Failed StartMajorMatchTeleport: i = %u, WorldTblix = %u, JoinID2 = %u",
						i, m_pTableInfo->sTeamWorldTblidx.majorMatch, it->second->data.wJoinId2);
				}
			}

			if (i == BUDOKAI_MAX_TEAM_MATCH_COUNT)
				break;
		}

		packet.SetPacketLen(sizeof(sGT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY));
		app->SendTo(app->GetChatServerSession(), &packet);
	}
}

void CBudokaiManager::SendMajorMatchTeamInfo(BYTE byMatchIndex, sTOURNAMENT_MATCH * match)
{
	sVARIABLE_DATA sData;
	sData.Init(1024);

	BYTE i = 0;
	WORD wTeamInfo_var = 0;

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		sMATCH_MINORMATCH_TEAM_INFO_VAR info[2];

		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer && pPlayer->GetMatchIndex() == byMatchIndex)
			{
				auto it2 = m_mapIndividual.find(pPlayer->GetJoinID());
				if (it2 != m_mapIndividual.end())
				{
					info[i].wTeamType = GetIndividualTeamType(it2->second.charId);

					pPlayer->SetBudokaiTeamType(info[i].wTeamType);

					if (info[i].wTeamType == MATCH_TEAM_TYPE_TEAM1)
						match->m_byTeam1PlayerCount += 1;
					else if (info[i].wTeamType == MATCH_TEAM_TYPE_TEAM2)
						match->m_byTeam2PlayerCount += 1;

					info[i].wTeamName_var = 0;
					info[i].bDojoRecommender = it2->second.bDojoRecommender;
					info[i].byMemberCount = 1;

					WCHAR			m_wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
					WCHAR			m_wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];

					NTL_SAFE_WCSCPY(m_wszCharName, pPlayer->GetCharName());
					NTL_SAFE_WCSCPY(m_wszGuildName, pPlayer->GetGuildName());

					sMATCH_MEMBER_STATE_INFO memberInfo[1];
					memberInfo[0].handle = pPlayer->GetID();
					memberInfo[0].byState = pPlayer->GetBudokaiPcState();
					memberInfo[0].byLevel = pPlayer->GetLevel();
					memberInfo[0].byClass = pPlayer->GetClass();
					memberInfo[0].wGuildName = sData.Write(sizeof(m_wszGuildName), m_wszGuildName);
					memberInfo[0].wWinCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwWin;
					memberInfo[0].wDrawCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwDraw;
					memberInfo[0].wLoseCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwLose;
					memberInfo[0].wCharName = sData.Write(sizeof(m_wszCharName), m_wszCharName);

					info[i].wTeamName_var = sData.Write(sizeof(m_wszCharName), m_wszCharName);
					info[i].wMembers_var = sData.Write(sizeof(memberInfo), memberInfo);

					++i;
				}
			}
		}

		wTeamInfo_var = sData.Write(sizeof(info), info);
	}
	else
	{
		sMATCH_MINORMATCH_TEAM_INFO_VAR info[2];
		WCHAR			charName[NTL_MAX_SIZE_CHAR_NAME + 1];
		WCHAR			partyName[NTL_MAX_SIZE_PARTY_NAME + 1];

		for (std::map<JOINID, sPARTY_INFO>::iterator it = match->m_mapTeam.begin(); it != match->m_mapTeam.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;

			sMATCH_MEMBER_STATE_INFO memberInfo[NTL_MAX_MEMBER_IN_PARTY];
			memset(memberInfo, -1, sizeof(memberInfo));

			info[i].wTeamType = GetIndividualTeamType(it->first);

			partyInfo->teamType = info[i].wTeamType;

			info[i].wTeamName_var = 0;
			info[i].bDojoRecommender = false;
			info[i].byMemberCount = 0;

			NTL_SAFE_WCSCPY(partyName, partyInfo->m_wszTeamName);
			info[i].wTeamName_var = sData.Write(sizeof(partyName), partyName);

			for (BYTE memberCnt = 0; memberCnt < partyInfo->byMemberCount; memberCnt++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[memberCnt]);
				if (pPlayer && pPlayer->GetCharID() == partyInfo->charId[memberCnt])
				{
					NTL_SAFE_WCSCPY(charName, pPlayer->GetCharName());

					pPlayer->SetBudokaiTeamType(info[i].wTeamType);

					if (info[i].wTeamType == MATCH_TEAM_TYPE_TEAM1)
						match->m_byTeam1PlayerCount += 1;
					else if (info[i].wTeamType == MATCH_TEAM_TYPE_TEAM2)
						match->m_byTeam2PlayerCount += 1;

					memberInfo[info[i].byMemberCount].handle = pPlayer->GetID();
					memberInfo[info[i].byMemberCount].byState = pPlayer->GetBudokaiPcState();
					memberInfo[info[i].byMemberCount].byLevel = pPlayer->GetLevel();
					memberInfo[info[i].byMemberCount].byClass = pPlayer->GetClass();
					memberInfo[info[i].byMemberCount].wGuildName = INVALID_WORD;
					memberInfo[info[i].byMemberCount].wWinCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwWin;
					memberInfo[info[i].byMemberCount].wDrawCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwDraw;
					memberInfo[info[i].byMemberCount].wLoseCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwLose;
					memberInfo[info[i].byMemberCount++].wCharName = sData.Write(sizeof(charName), charName);
				}
			}

			info[i].wMembers_var = sData.Write(sizeof(memberInfo), memberInfo);

			++i;
		}

		wTeamInfo_var = sData.Write(sizeof(info), info);
	}

	WORD wPacketSize = sData.GetPacketSize(sizeof(sGU_MATCH_MAJORMATCH_TEAM_INFO_NFY));

	CNtlPacket packet(wPacketSize);
	sGU_MATCH_MAJORMATCH_TEAM_INFO_NFY * res = (sGU_MATCH_MAJORMATCH_TEAM_INFO_NFY *)packet.GetPacketData();
	res->wOpCode = GU_MATCH_MAJORMATCH_TEAM_INFO_NFY;
	res->byMatchIndex = byMatchIndex;
	res->byMatchDepth = m_eMatchDepth;
	res->byTeamCount = i;
	res->wTeamInfo_var = wTeamInfo_var;
	sData.CopyTo(&res->sData, sData.GetDataSize());
	match->m_pWorld->Broadcast(&packet);

	ERR_LOG(LOG_GENERAL,"Budokai: wPacketSize %u, sData.GetDataSize() %u, byTeamCount %u \n", wPacketSize, sData.GetDataSize(), res->byTeamCount);
}

void CBudokaiManager::MajorMatchUpdatePlayersState(sTOURNAMENT_MATCH * match, BYTE byMatchIndex, BYTE byState)
{
	for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
		if (pPlayer)
		{
			if (pPlayer->GetCharID() == it->second.charId)
			{
				if (pPlayer->GetMatchIndex() == byMatchIndex)
				{
					pPlayer->SetBudokaiPcState(byState);

					CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
					sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_MAJORMATCH_PLAYER_STATE_NFY;
					res2->hPc = it->first;
					res2->byPcState = byState;
					packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
					match->m_pWorld->Broadcast(&packet2);
				}
				else ERR_LOG(LOG_GENERAL, "Player has different match index %u != %u", pPlayer->GetMatchIndex(), byMatchIndex);
			}
			else ERR_LOG(LOG_GENERAL, "Player has different char id %u != %u", pPlayer->GetCharID(), it->second.charId);
		}
		else ERR_LOG(LOG_GENERAL, "Could not find player");
	}
}

bool CBudokaiManager::ProcessMajorMatch(sTOURNAMENT_MATCH * match, BYTE byMatchIndex, DWORD dwTickDif)
{
	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			sPLAYER_INFO& info = it->second;

			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer)
			{
				if (pPlayer->GetCharID() == info.charId)
				{
					if (pPlayer->GetMatchIndex() == byMatchIndex)
					{
						if (pPlayer->GetBudokaiPcState() == MATCH_MEMBER_STATE_NORMAL)
						{
							if (IsInsideArena(pPlayer->GetCurLoc(), pPlayer->GetCurWorld()) == false)
							{
								pPlayer->SetBudokaiPcState(MATCH_MEMBER_STATE_OUTOFAREA);

								CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
								sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
								res2->wOpCode = GU_MATCH_MAJORMATCH_PLAYER_STATE_NFY;
								res2->hPc = pPlayer->GetID();
								res2->byPcState = MATCH_MEMBER_STATE_OUTOFAREA;
								packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
								match->m_pWorld->Broadcast(&packet2);

								// --- Lose Round
								TEAMTYPE wMatchWinner = (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1) ? MATCH_TEAM_TYPE_TEAM2 : MATCH_TEAM_TYPE_TEAM1;

								UpdateMajorMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, wMatchWinner);

								return false;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		for (std::map<JOINID, sPARTY_INFO>::iterator it = match->m_mapTeam.begin(); it != match->m_mapTeam.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;

			for (BYTE i = 0; i < partyInfo->byMemberCount; i++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[i]);
				if (pPlayer)
				{
					if (pPlayer->GetCharID() == partyInfo->charId[i])
					{
						if (pPlayer->GetMatchIndex() == byMatchIndex)
						{
							if (pPlayer->GetBudokaiPcState() == MATCH_MEMBER_STATE_NORMAL)
							{
								if (IsInsideArena(pPlayer->GetCurLoc(), pPlayer->GetCurWorld()) == false)
								{
									pPlayer->SetBudokaiPcState(MATCH_MEMBER_STATE_OUTOFAREA);

									CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
									sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
									res2->wOpCode = GU_MATCH_MAJORMATCH_PLAYER_STATE_NFY;
									res2->hPc = pPlayer->GetID();
									res2->byPcState = MATCH_MEMBER_STATE_OUTOFAREA;
									packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
									match->m_pWorld->Broadcast(&packet2);

									if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1)
									{
										match->m_byTeam1UnavailablePlayerCount += 1;

										if (match->m_byTeam1UnavailablePlayerCount >= match->m_byTeam1PlayerCount)
										{
											UpdateMajorMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, MATCH_TEAM_TYPE_TEAM2);
											return false;
										}
									}
									else if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM2)
									{
										match->m_byTeam2UnavailablePlayerCount += 1;

										if (match->m_byTeam2UnavailablePlayerCount >= match->m_byTeam2PlayerCount)
										{
											UpdateMajorMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, MATCH_TEAM_TYPE_TEAM1);
											return false;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return true;
}

void CBudokaiManager::UpdateMajorMatchScore(sTOURNAMENT_MATCH * match, BYTE byMatchIndex, BYTE byMatchResult, TEAMTYPE wMatchWinner, BYTE byWins/* = 1*/)
{
	if (wMatchWinner == MATCH_TEAM_TYPE_TEAM1)
	{
		match->data.byScore1 += byWins;
		if (match->data.byScore1 > 2)
			match->data.byScore1 = 2;
	}
	else
	{
		match->data.byScore2 += byWins;
		if (match->data.byScore2 > 2)
			match->data.byScore2 = 2;
	}

	CNtlPacket packet(sizeof(sGU_MATCH_MAJORMATCH_STAGE_FINISH_NFY));
	sGU_MATCH_MAJORMATCH_STAGE_FINISH_NFY * res = (sGU_MATCH_MAJORMATCH_STAGE_FINISH_NFY *)packet.GetPacketData();
	res->wOpCode = GU_MATCH_MAJORMATCH_STAGE_FINISH_NFY;
	res->sMatchScore.byTeam1 = match->data.byScore1;
	res->sMatchScore.byTeam2 = match->data.byScore2;
	res->byStageResult = byMatchResult;
	res->wStageWinner = wMatchWinner;
	res->bIsObserver = false;
	res->nGetPoint = FALSE;
	packet.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_STAGE_FINISH_NFY));
	match->m_pWorld->Broadcast(&packet);

	match->byStage += byWins;

	//update players state to none
	MajorMatchUpdatePlayersState(match, byMatchIndex, MATCH_MEMBER_STATE_NONE);

	//check if match finish
	if (match->data.byScore1 >= 2 || match->data.byScore2 >= 2)
	{
		ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Major Match. Index %u. Winner-Team = %u. Score1 = %u, Score2 = %u, byMatchResult = %u ",
			byMatchIndex, wMatchWinner, match->data.byScore1, match->data.byScore2, byMatchResult);

		match->bFinishMatch = true;
	}

	match->m_byMatchState = BUDOKAI_MAJORMATCH_STATE_NONE;
	match->m_dwMatchTickCount = 5000;

	CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
	sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
	res2->wOpCode = GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY;
	res2->byMatchState = BUDOKAI_MAJORMATCH_STATE_NONE;
	res2->bIsEnter = true;
	res2->byStage = match->byStage;
	res2->dwRemainTime = match->m_dwMatchTickCount;
	packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
	match->m_pWorld->Broadcast(&packet2);
}


void CBudokaiManager::ResetPlayerMajorMatch(sTOURNAMENT_MATCH * match, BYTE byMatchIndex, bool bSpawn)
{
	match->m_byTeam1UnavailablePlayerCount = 0;
	match->m_byTeam2UnavailablePlayerCount = 0;

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			sPLAYER_INFO& info = it->second;

			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer)
			{
				if (pPlayer->GetCharID() == info.charId)
				{
					if (pPlayer->GetMatchIndex() == byMatchIndex)
					{
						pPlayer->ChangeAttackProgress(false);
						pPlayer->SetAttackTarget(INVALID_HOBJECT);

						//remove buffs
						pPlayer->GetBuffManager()->RemoveAllBuff();

						//if transformed then cancel transformation
						if (pPlayer->GetTransformationTbldat())
							pPlayer->CancelTransformation();

						if (pPlayer->GetStateManager()->GetConditionState() > 0)
						{
							pPlayer->GetStateManager()->RemoveConditionFlags(INVALID_QWORD, true);
						}

						//re-calculate all stats
						pPlayer->GetCharAtt()->CalculateAll();

						CNtlVector vNewLoc;

						if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1)
						{
							vNewLoc = m_pTableInfo->aMajorLoc[0].vLoc;
							pPlayer->SetCurDir(m_pTableInfo->aMajorLoc[0].vDir);
						}
						else if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM2)
						{
							vNewLoc = m_pTableInfo->aMajorLoc[5].vLoc;
							pPlayer->SetCurDir(m_pTableInfo->aMajorLoc[5].vDir);
						}
						pPlayer->SetIsReviving(false);

						//restore lp/rp/rp/rp balls
						pPlayer->UpdateCurLP(pPlayer->GetMaxLP(), true, false);
						pPlayer->UpdateCurEP(pPlayer->GetMaxEP(), true, false);
						pPlayer->UpdateRpBall(pPlayer->GetMaxRPBall(), false, false);
						pPlayer->UpdateCurRP(pPlayer->GetCharAtt()->GetMaxRP(), false, false);

						//remove target 
						pPlayer->ChangeTarget(INVALID_HOBJECT);

						//clear cc reduction
						pPlayer->ClearCrowdControlReduction();

						if (bSpawn)
							pPlayer->StartTeleport(vNewLoc, pPlayer->GetCurDir(), match->m_pWorld->GetID(), TELEPORT_TYPE_MAJORMATCH);
					}
				}
			}
		}
	}
	else
	{
		for (std::map<JOINID, sPARTY_INFO>::iterator it = match->m_mapTeam.begin(); it != match->m_mapTeam.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;

			for (BYTE i = 0; i < partyInfo->byMemberCount; i++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[i]);
				if (pPlayer)
				{
					if (pPlayer->GetCharID() == partyInfo->charId[i])
					{
						if (pPlayer->GetMatchIndex() == byMatchIndex)
						{
							pPlayer->ChangeAttackProgress(false);
							pPlayer->SetAttackTarget(INVALID_HOBJECT);

							//remove buffs
							pPlayer->GetBuffManager()->RemoveAllBuff();

							//if transformed then cancel transformation
							if (pPlayer->GetTransformationTbldat())
								pPlayer->CancelTransformation();

							if (pPlayer->GetStateManager()->GetConditionState() > 0)
							{
								pPlayer->GetStateManager()->RemoveConditionFlags(INVALID_QWORD, true);
							}

							//re-calculate all stats
							pPlayer->GetCharAtt()->CalculateAll();

							CNtlVector vNewLoc;

							if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1)
							{
								vNewLoc = m_pTableInfo->aMajorLoc[i].vLoc;
								pPlayer->SetCurDir(m_pTableInfo->aMajorLoc[i].vDir);
							}
							else if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM2)
							{
								vNewLoc = m_pTableInfo->aMajorLoc[i + 5].vLoc;
								pPlayer->SetCurDir(m_pTableInfo->aMajorLoc[i + 5].vDir);
							}

							pPlayer->SetIsReviving(false);

							//restore lp/rp/rp/rp balls
							pPlayer->UpdateCurLP(pPlayer->GetMaxLP(), true, false);
							pPlayer->UpdateCurEP(pPlayer->GetMaxEP(), true, false);
							pPlayer->UpdateRpBall(pPlayer->GetMaxRPBall(), false, false);
							pPlayer->UpdateCurRP(pPlayer->GetCharAtt()->GetMaxRP(), false, false);

							//remove target 
							pPlayer->ChangeTarget(INVALID_HOBJECT);

							//clear cc reduction
							pPlayer->ClearCrowdControlReduction();

							if (bSpawn)
								pPlayer->StartTeleport(vNewLoc, pPlayer->GetCurDir(), match->m_pWorld->GetID(), TELEPORT_TYPE_MAJORMATCH);
						}
					}
				}
			}
		}
	}
}

void CBudokaiManager::MajorMatchMatchFinish(sTOURNAMENT_MATCH * match, BYTE byMatchIndex)
{
	CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_MATCH_FINISH_NFY));
	sGU_MATCH_MAJORMATCH_MATCH_FINISH_NFY * res2 = (sGU_MATCH_MAJORMATCH_MATCH_FINISH_NFY *)packet2.GetPacketData();
	res2->wOpCode = GU_MATCH_MAJORMATCH_MATCH_FINISH_NFY;
	res2->byMatchResult = MATCH_RESULT_WIN;
	res2->sMatchScore.byTeam1 = match->data.byScore1;
	res2->sMatchScore.byTeam2 = match->data.byScore2;

	if (match->data.byScore1 > match->data.byScore2)
	{
		match->data.wWinnerTeam = match->data.wJoinId1;
		res2->wMatchWinner = MATCH_TEAM_TYPE_TEAM1;
	}
	else
	{
		match->data.wWinnerTeam = match->data.wJoinId2;
		res2->wMatchWinner = MATCH_TEAM_TYPE_TEAM2;
	}

	packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_MATCH_FINISH_NFY));
	match->m_pWorld->Broadcast(&packet2);

	// -- Give Winner Reward
	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			sPLAYER_INFO& info = it->second;

			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer)
			{
				pPlayer->SetBudokaiPcState(MATCH_MEMBER_STATE_NORMAL);

				CNtlPacket packet3(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
				sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY * res3 = (sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY *)packet3.GetPacketData();
				res3->wOpCode = GU_MATCH_MAJORMATCH_PLAYER_STATE_NFY;
				res3->hPc = it->first;
				res3->byPcState = MATCH_MEMBER_STATE_NORMAL;
				packet3.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
				pPlayer->SendPacket(&packet3);

				if (pPlayer->GetCharID() == it->second.charId && pPlayer->GetMatchIndex() == byMatchIndex)
				{
					DWORD dwMudosaPoints = 0;
					TBLIDX itemIdx = INVALID_TBLIDX;
					BYTE byItemCount = 0;

					if (pPlayer->GetJoinID() == match->data.wWinnerTeam)
					{
						ERR_LOG(LOG_GENERAL, "BUDOKAI: Tournament Major Match Depth %u, Index %u Finish. Add Winner CharID = %u, JoinId = %u, WinnerTeam = %u", m_eMatchDepth, byMatchIndex, pPlayer->GetCharID(), pPlayer->GetJoinID(), match->data.wWinnerTeam);

						match->data.byMatchResult = MATCH_RESULT_WIN;

						if (m_type == BUDOKAI_TYPE_JUNIOR)
						{
							dwMudosaPoints = m_pTableInfo->sJuniorIndividualReward.aMajorMatch[m_eMatchDepth].dwBaseMudosaPoint + m_pTableInfo->sJuniorIndividualReward.aMajorMatch[m_eMatchDepth].dwWinnerMudosaPoint;
							itemIdx = m_pTableInfo->sJuniorIndividualReward.aMajorMatch[m_eMatchDepth].winnerItem;
							byItemCount = m_pTableInfo->sJuniorIndividualReward.aMajorMatch[m_eMatchDepth].byWinerItemStackCount;
						}
						else
						{
							dwMudosaPoints = m_pTableInfo->sIndividualReward.aMajorMatch[m_eMatchDepth].dwBaseMudosaPoint + m_pTableInfo->sIndividualReward.aMajorMatch[m_eMatchDepth].dwWinnerMudosaPoint;
							itemIdx = m_pTableInfo->sIndividualReward.aMajorMatch[m_eMatchDepth].winnerItem;
							byItemCount = m_pTableInfo->sIndividualReward.aMajorMatch[m_eMatchDepth].byWinerItemStackCount;
						}
					}
					else
					{
						if (m_type == BUDOKAI_TYPE_JUNIOR)
						{
							dwMudosaPoints = m_pTableInfo->sJuniorIndividualReward.aMajorMatch[m_eMatchDepth].dwBaseMudosaPoint;
							itemIdx = m_pTableInfo->sJuniorIndividualReward.aMajorMatch[m_eMatchDepth].loserItem;
							byItemCount = m_pTableInfo->sJuniorIndividualReward.aMajorMatch[m_eMatchDepth].byLoserItemStackCount;
						}
						else
						{
							dwMudosaPoints = m_pTableInfo->sIndividualReward.aMajorMatch[m_eMatchDepth].dwBaseMudosaPoint;
							itemIdx = m_pTableInfo->sIndividualReward.aMajorMatch[m_eMatchDepth].loserItem;
							byItemCount = m_pTableInfo->sIndividualReward.aMajorMatch[m_eMatchDepth].byLoserItemStackCount;
						}
					}

					ERR_LOG(LOG_GENERAL, "Budokai Major match reward: CharID = %u, Item Idx = %u, Item Count = %u, Mudosa Points = %u", pPlayer->GetCharID(), itemIdx, byItemCount, dwMudosaPoints);

					pPlayer->UpdateMudosaPoints(pPlayer->GetMudosaPoints() + dwMudosaPoints, true);

					if (itemIdx != INVALID_TBLIDX)
						g_pItemManager->CreateQuestRewardItem(pPlayer, itemIdx, byItemCount);
				}
			}
		}
	}
	else if (m_matchType == BUDOKAI_MATCH_TYPE_TEAM)
	{
		for (std::map<JOINID, sPARTY_INFO>::iterator it = match->m_mapTeam.begin(); it != match->m_mapTeam.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;

			DWORD dwMudosaPoints = 0;
			TBLIDX itemIdx = INVALID_TBLIDX;
			BYTE byItemCount = 0;

			if (partyInfo->joinId == match->data.wWinnerTeam)
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: Tournament Major Match Depth %u, Index %u Finish. Add Winner JoinId = %u, WinnerTeam = %u", m_eMatchDepth, byMatchIndex, partyInfo->joinId, match->data.wWinnerTeam);

				match->data.byMatchResult = MATCH_RESULT_WIN;

				if (m_type == BUDOKAI_TYPE_JUNIOR)
				{
					dwMudosaPoints = m_pTableInfo->sJuniorTeamReward.aMajorMatch[m_eMatchDepth].dwBaseMudosaPoint + m_pTableInfo->sJuniorTeamReward.aMajorMatch[m_eMatchDepth].dwWinnerMudosaPoint;
					itemIdx = m_pTableInfo->sJuniorTeamReward.aMajorMatch[m_eMatchDepth].winnerItem;
					byItemCount = m_pTableInfo->sJuniorTeamReward.aMajorMatch[m_eMatchDepth].byWinerItemStackCount;
				}
				else
				{
					dwMudosaPoints = m_pTableInfo->sTeamReward.aMajorMatch[m_eMatchDepth].dwBaseMudosaPoint + m_pTableInfo->sTeamReward.aMajorMatch[m_eMatchDepth].dwWinnerMudosaPoint;
					itemIdx = m_pTableInfo->sTeamReward.aMajorMatch[m_eMatchDepth].winnerItem;
					byItemCount = m_pTableInfo->sTeamReward.aMajorMatch[m_eMatchDepth].byWinerItemStackCount;
				}
			}
			else
			{
				if (m_type == BUDOKAI_TYPE_JUNIOR)
				{
					dwMudosaPoints = m_pTableInfo->sJuniorTeamReward.aMajorMatch[m_eMatchDepth].dwBaseMudosaPoint;
					itemIdx = m_pTableInfo->sJuniorTeamReward.aMajorMatch[m_eMatchDepth].loserItem;
					byItemCount = m_pTableInfo->sJuniorTeamReward.aMajorMatch[m_eMatchDepth].byLoserItemStackCount;
				}
				else
				{
					dwMudosaPoints = m_pTableInfo->sTeamReward.aMajorMatch[m_eMatchDepth].dwBaseMudosaPoint;
					itemIdx = m_pTableInfo->sTeamReward.aMajorMatch[m_eMatchDepth].loserItem;
					byItemCount = m_pTableInfo->sTeamReward.aMajorMatch[m_eMatchDepth].byLoserItemStackCount;
				}
			}

			for (BYTE byMemberCount = 0; byMemberCount < partyInfo->byMemberCount; byMemberCount++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[byMemberCount]);
				if (pPlayer && partyInfo->charId[byMemberCount] == pPlayer->GetCharID() && pPlayer->GetMatchIndex() == byMatchIndex)
				{
					pPlayer->SetBudokaiPcState(MATCH_MEMBER_STATE_NORMAL);

					CNtlPacket packet3(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
					sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY * res3 = (sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY *)packet3.GetPacketData();
					res3->wOpCode = GU_MATCH_MAJORMATCH_PLAYER_STATE_NFY;
					res3->hPc = it->first;
					res3->byPcState = MATCH_MEMBER_STATE_NORMAL;
					packet3.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
					pPlayer->SendPacket(&packet3);

					pPlayer->UpdateMudosaPoints(pPlayer->GetMudosaPoints() + dwMudosaPoints, true);

					if (itemIdx != INVALID_TBLIDX)
						g_pItemManager->CreateQuestRewardItem(pPlayer, itemIdx, byItemCount);
				}
			}
		}
	}
	else
	{
		// debug
	}
}

void CBudokaiManager::MajorMatchEnd(sTOURNAMENT_MATCH * match, BYTE byMatchIndex)
{
	CGameServer* app = (CGameServer*)g_pApp;

	ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Major Match. Index %u END", byMatchIndex);

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			sPLAYER_INFO& info = it->second;

			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer)
			{
				if (pPlayer->GetMatchIndex() == byMatchIndex && pPlayer->GetCharID() == info.charId)
				{
					CNtlPacket packet(sizeof(sGQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ));
					sGQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ * res = (sGQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ *)packet.GetPacketData();
					res->wOpCode = GQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ;
					res->wJoinId = pPlayer->GetJoinID();
					res->byMatchIndex = byMatchIndex;
					res->byWinnerJoinResult = INVALID_BUDOKAI_JOIN_RESULT;

					if (pPlayer->GetJoinID() == match->data.wWinnerTeam) //check if winner
					{
						std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(pPlayer->GetJoinID());
						if (it != m_mapJoinInfo.end())
						{
							if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_32)
							{
								it->second.byJoinResult = BUDOKAI_JOIN_RESULT_ENTER_16;
								res->byWinnerJoinResult = BUDOKAI_JOIN_RESULT_ENTER_16;
							}
							else if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_16)
							{
								it->second.byJoinResult = BUDOKAI_JOIN_RESULT_ENTER_8;
								res->byWinnerJoinResult = BUDOKAI_JOIN_RESULT_ENTER_8;
							}
							else if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_8)
							{
								it->second.byJoinResult = BUDOKAI_JOIN_RESULT_ENTER_4;
								res->byWinnerJoinResult = BUDOKAI_JOIN_RESULT_ENTER_4;
							}

							ERR_LOG(LOG_GENERAL, "BUDOKAI: Match-Depth = %u, Update CharID = %u, JoinResult = %u", m_eMatchDepth, pPlayer->GetCharID(), it->second.byJoinResult);
						}
						else ERR_LOG(LOG_GENERAL, "Could not find prelim winner in m_mapJoinInfo");
					}
					else
					{
						std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(pPlayer->GetJoinID());
						if (it != m_mapJoinInfo.end())
						{
							it->second.byJoinState = BUDOKAI_JOIN_STATE_DROPOUT;
							it->second.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;

							res->byLoserJoinState = BUDOKAI_JOIN_STATE_DROPOUT;
							res->byLoserResultCondition = INVALID_BUDOKAI_JOIN_RESULT;

							ERR_LOG(LOG_GENERAL, "BUDOKAI: Match-Depth = %u, Update CharID = %u, JoinResult = %u, byJoinState = %u", m_eMatchDepth, pPlayer->GetCharID(), it->second.byJoinResult, it->second.byJoinState);
						}
						else ERR_LOG(LOG_GENERAL, "Could not find player in m_mapJoinInfo");
					}
			
					packet.SetPacketLen(sizeof(sGQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ));
					app->SendTo(app->GetQueryServerSession(), &packet);

					pPlayer->TeleportAnotherServer(match->m_pWorld->GetTbldat()->outWorldLoc, pPlayer->GetCurDir(), match->m_pWorld->GetTbldat()->outWorldTblidx, match->m_pWorld->GetTbldat()->outWorldTblidx, TELEPORT_TYPE_BUDOKAI, pPlayer->GetPrevChannelID(), 0, false);
				}
			}
		}
	}
	else
	{
		for (std::map<JOINID, sPARTY_INFO>::iterator it = match->m_mapTeam.begin(); it != match->m_mapTeam.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;

			CNtlPacket packet(sizeof(sGQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ));
			sGQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ * res = (sGQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ *)packet.GetPacketData();
			res->wOpCode = GQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ;
			res->wJoinId = partyInfo->joinId;
			res->byMatchIndex = byMatchIndex;
			res->byWinnerJoinResult = INVALID_BUDOKAI_JOIN_RESULT;

			if (partyInfo->joinId == match->data.wWinnerTeam)
			{
				std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(partyInfo->joinId);
				if (it != m_mapJoinInfo.end())
				{
					if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_16)
					{
						it->second.byJoinResult = BUDOKAI_JOIN_RESULT_ENTER_8;
						res->byWinnerJoinResult = BUDOKAI_JOIN_RESULT_ENTER_8;
					}
					else if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_8)
					{
						it->second.byJoinResult = BUDOKAI_JOIN_RESULT_ENTER_4;
						res->byWinnerJoinResult = BUDOKAI_JOIN_RESULT_ENTER_4;
					}
				}
				else ERR_LOG(LOG_GENERAL, "Could not find winner in m_mapJoinInfo");
			}
			else
			{
				std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(partyInfo->joinId);
				if (it != m_mapJoinInfo.end())
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. Party %u. Set byJoinState = BUDOKAI_JOIN_STATE_DROPOUT, JoinResult = INVALID_BUDOKAI_JOIN_RESULT ", byMatchIndex, partyInfo->joinId);

					it->second.byJoinState = BUDOKAI_JOIN_STATE_DROPOUT;
					it->second.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;

					res->byLoserJoinState = BUDOKAI_JOIN_STATE_DROPOUT;
					res->byLoserResultCondition = INVALID_BUDOKAI_JOIN_RESULT;
				}
				else ERR_LOG(LOG_GENERAL, "Could not find player in m_mapJoinInfo");
			}

			packet.SetPacketLen(sizeof(sGQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet);


			for (BYTE byMemberCount = 0; byMemberCount < partyInfo->byMemberCount; byMemberCount++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[byMemberCount]);
				if (pPlayer && partyInfo->charId[byMemberCount] == pPlayer->GetCharID() && pPlayer->GetMatchIndex() == byMatchIndex)
				{
					pPlayer->TeleportAnotherServer(match->m_pWorld->GetTbldat()->outWorldLoc, pPlayer->GetCurDir(), match->m_pWorld->GetTbldat()->outWorldTblidx, match->m_pWorld->GetTbldat()->outWorldTblidx, TELEPORT_TYPE_BUDOKAI, pPlayer->GetPrevChannelID(), 0, false);
				}
			}
		}
	}
}

void CBudokaiManager::EndMatchDepth(eBUDOKAI_MATCH_DEPTH matchDepth)
{
	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		if (matchDepth == BUDOKAI_MATCH_DEPTH_32)
			m_eMatchDepth = BUDOKAI_MATCH_DEPTH_16;
		else if (matchDepth == BUDOKAI_MATCH_DEPTH_16)
			m_eMatchDepth = BUDOKAI_MATCH_DEPTH_8;
		else if (matchDepth == BUDOKAI_MATCH_DEPTH_8)
			m_eMatchDepth = BUDOKAI_MATCH_DEPTH_4;
		else if (matchDepth == BUDOKAI_MATCH_DEPTH_4)
			m_eMatchDepth = BUDOKAI_MATCH_DEPTH_2;
	}
	else
	{
		if (matchDepth == BUDOKAI_MATCH_DEPTH_16)
			m_eMatchDepth = BUDOKAI_MATCH_DEPTH_8;
		else if (matchDepth == BUDOKAI_MATCH_DEPTH_8)
			m_eMatchDepth = BUDOKAI_MATCH_DEPTH_4;
		else if (matchDepth == BUDOKAI_MATCH_DEPTH_4)
			m_eMatchDepth = BUDOKAI_MATCH_DEPTH_2;
	}
}

void CBudokaiManager::MajorMatchTimeEnd(sTOURNAMENT_MATCH * match, BYTE byMatchIndex)
{
	//if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		float fTotalLifePercentOwner = 0.0f;
		float fTotalLifePercentChallenger = 0.0f;

		CPlayer* pPlayerOwner = NULL;
		CPlayer* pPlayerChallenger = NULL;

		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer && pPlayer->IsInitialized() && pPlayer->GetMatchIndex() == byMatchIndex)
			{
				if (pPlayer->GetBudokaiPcState() == MATCH_MEMBER_STATE_NORMAL)
				{
					if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1)
					{
						fTotalLifePercentOwner += pPlayer->GetCurLpInPercent();
						pPlayerOwner = pPlayer;
					}
					else if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM2)
					{
						fTotalLifePercentChallenger += pPlayer->GetCurLpInPercent();
						pPlayerChallenger = pPlayer;
					}
				}
			}
		}

		ERR_LOG(LOG_GENERAL, "fTotalLifePercentOwner %f, fTotalLifePercentChallenger %f", fTotalLifePercentOwner, fTotalLifePercentChallenger);

		if (fTotalLifePercentOwner > fTotalLifePercentChallenger)
			UpdateMajorMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, pPlayerOwner->GetBudokaiTeamType());
		else if (fTotalLifePercentChallenger > fTotalLifePercentOwner)
			UpdateMajorMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, pPlayerChallenger->GetBudokaiTeamType());
		else
		{
			//dice manager
			CDiceManager dice;

			fTotalLifePercentOwner = (float)dice.RollDice();
			fTotalLifePercentChallenger = (float)dice.RollDice();

			if (pPlayerOwner)
			{
				CNtlPacket packet2(sizeof(sGU_DICE_ROLLED_NFY));
				sGU_DICE_ROLLED_NFY* res2 = (sGU_DICE_ROLLED_NFY*)packet2.GetPacketData();
				res2->wOpCode = GU_DICE_ROLLED_NFY;
				res2->hSubject = pPlayerOwner->GetID();
				res2->wDiceResult = (WORD)fTotalLifePercentOwner;
				packet2.SetPacketLen(sizeof(sGU_DICE_ROLLED_NFY));
				pPlayerOwner->Broadcast(&packet2);
			}

			if (pPlayerChallenger)
			{
				CNtlPacket packet4(sizeof(sGU_DICE_ROLLED_NFY));
				sGU_DICE_ROLLED_NFY* res4 = (sGU_DICE_ROLLED_NFY*)packet4.GetPacketData();
				res4->wOpCode = GU_DICE_ROLLED_NFY;
				res4->hSubject = pPlayerChallenger->GetID();
				res4->wDiceResult = (WORD)fTotalLifePercentChallenger;
				packet4.SetPacketLen(sizeof(sGU_DICE_ROLLED_NFY));
				pPlayerChallenger->Broadcast(&packet4);
			}

			if (pPlayerOwner && pPlayerChallenger == NULL)
			{
				UpdateMajorMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, pPlayerOwner->GetBudokaiTeamType());

				return;
			}
			else if (pPlayerChallenger && pPlayerOwner == NULL)
			{
				UpdateMajorMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, pPlayerChallenger->GetBudokaiTeamType());

				return;
			}

			if (pPlayerOwner && fTotalLifePercentOwner >= fTotalLifePercentChallenger)
				UpdateMajorMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, pPlayerOwner->GetBudokaiTeamType());
			else if (pPlayerChallenger && fTotalLifePercentChallenger > fTotalLifePercentOwner)
				UpdateMajorMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, pPlayerChallenger->GetBudokaiTeamType());
		}
	}
}

bool CBudokaiManager::MajorMatchCheckPlayerCount(sTOURNAMENT_MATCH* match, BYTE byMatchIndex)
{
	if (match->m_byTeam1PlayerCount == 0)
	{
		UpdateMajorMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, MATCH_TEAM_TYPE_TEAM2, 2); //give instant 2 wins
		return false;
	}

	if(match->m_byTeam2PlayerCount == 0)
	{
		UpdateMajorMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, MATCH_TEAM_TYPE_TEAM1, 2); //give instant 2 wins
		return false;
	}

	return true;
}

void CBudokaiManager::TickProcessFinalMatch(DWORD dwTickDif, BUDOKAITIME curTime)
{
	for (std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.begin(); it != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end(); it++)
	{
		sTOURNAMENT_MATCH* match = it->second;

		if (match->bEnd == true)
			continue;

		match->m_dwMatchTickCount = UnsignedSafeDecrease<DWORD>(match->m_dwMatchTickCount, dwTickDif);

		switch (match->m_byMatchState)
		{
		case BUDOKAI_FINALMATCH_STATE_NONE:
		{
			if (match->m_dwMatchTickCount == 0)
			{
				match->m_byMatchState = BUDOKAI_FINALMATCH_STATE_STAGE_FINISH;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Final Match. Index %u, byState BUDOKAI_FINALMATCH_STATE_NONE -> BUDOKAI_FINALMATCH_STATE_STAGE_FINISH, m_dwMatchTickCount = %u",
					it->first, match->m_dwMatchTickCount);

				CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
				sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
				res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
				res2->byMatchState = BUDOKAI_FINALMATCH_STATE_STAGE_FINISH;
				res2->bIsEnter = false; //false = the score picture goes away
				res2->byStage = match->byStage;
				res2->dwRemainTime = match->m_dwMatchTickCount;
				packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
				match->m_pWorld->Broadcast(&packet2);
			}
		}
		break;
		case BUDOKAI_FINALMATCH_STATE_WAIT:
		{
			if (match->m_dwMatchTickCount == 0 || match->m_byCount >= match->m_byTotalCount)
			{
				if (match->m_byCount == 0 || match->m_byTotalCount == 0) //if none inside, then end directly //if none inside, then end directly
				{
					match->m_byMatchState = BUDOKAI_FINALMATCH_STATE_END;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Final Match. Index %u, byState BUDOKAI_FINALMATCH_STATE_WAIT -> BUDOKAI_FINALMATCH_STATE_END, m_dwMatchTickCount = %u, m_byCount = %u, m_byTotalCount = %u",
						it->first, match->m_dwMatchTickCount, match->m_byCount, match->m_byTotalCount);
				}
				else //begin
				{
					match->m_byMatchState = BUDOKAI_FINALMATCH_STATE_MATCH_READY;
					match->m_dwMatchTickCount = 5000;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Final Match. Index %u, byState BUDOKAI_FINALMATCH_STATE_WAIT -> BUDOKAI_FINALMATCH_STATE_MATCH_READY, m_dwMatchTickCount = %u, m_byCount = %u, m_byTotalCount = %u",
						it->first, match->m_dwMatchTickCount, match->m_byCount, match->m_byTotalCount);

					//send player info
					SendFinalMatchTeamInfo(it->first, match);

					//reset player shit
					ResetPlayerFinalMatch(match, it->first, false);

					CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_FINALMATCH_STATE_MATCH_READY;
					res2->bIsEnter = true;
					res2->byStage = 0;
					res2->dwRemainTime = match->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					match->m_pWorld->Broadcast(&packet2);
				}
			}
		}
		break;
		case BUDOKAI_FINALMATCH_STATE_DIRECTION: // only used in semi final
		{
			if (match->m_dwMatchTickCount == 0)
			{
				if (FinalMatchCheckPlayerCount(match, it->first) == true)
				{
					match->m_byMatchState = BUDOKAI_FINALMATCH_STATE_STAGE_READY;
					match->m_dwMatchTickCount = 5000;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Final Match. Index %u, byState BUDOKAI_FINALMATCH_STATE_DIRECTION -> BUDOKAI_FINALMATCH_STATE_STAGE_READY, m_dwMatchTickCount = %u",
						it->first, match->m_dwMatchTickCount);

					CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_FINALMATCH_STATE_STAGE_READY;
					res2->bIsEnter = true;
					res2->byStage = match->byStage;
					res2->dwRemainTime = match->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					match->m_pWorld->Broadcast(&packet2);
				}
			}
		}
		break;
		case BUDOKAI_FINALMATCH_STATE_MATCH_READY:
		{
			if (match->m_dwMatchTickCount == 0)
			{
				if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_2) //if final
				{
					match->m_byMatchState = BUDOKAI_FINALMATCH_STATE_FINALDIRECTION;
					match->m_dwMatchTickCount = m_pTableInfo->dwFinalMatchDirectionTime * 1000;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Final Match. Index %u, byState BUDOKAI_FINALMATCH_STATE_MATCH_READY -> BUDOKAI_FINALMATCH_STATE_FINALDIRECTION, m_dwMatchTickCount = %u",
						it->first, match->m_dwMatchTickCount);

					CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_FINALMATCH_STATE_FINALDIRECTION;
					res2->bIsEnter = true;
					res2->byStage = match->byStage;
					res2->dwRemainTime = match->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					match->m_pWorld->Broadcast(&packet2);
				}
				else
				{
					match->m_byMatchState = BUDOKAI_FINALMATCH_STATE_DIRECTION;
					match->m_dwMatchTickCount = 25000;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Final Match. Index %u, byState BUDOKAI_FINALMATCH_STATE_MATCH_READY -> BUDOKAI_FINALMATCH_STATE_DIRECTION, m_dwMatchTickCount = %u",
						it->first, match->m_dwMatchTickCount);

					CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_FINALMATCH_STATE_DIRECTION;
					res2->bIsEnter = true;
					res2->byStage = match->byStage;
					res2->dwRemainTime = match->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					match->m_pWorld->Broadcast(&packet2);
				}
			}
		}
		break;
		case BUDOKAI_FINALMATCH_STATE_STAGE_READY:
		{
			if (match->m_dwMatchTickCount == 0)
			{
				match->m_byMatchState = BUDOKAI_FINALMATCH_STATE_STAGE_RUN;
				
				if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
					match->m_dwMatchTickCount = 3 * 60 * 1000; // 3 minutes
				else
					match->m_dwMatchTickCount = 4 * 60 * 1000; // 4 minutes

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Final Match. Index %u, byState BUDOKAI_FINALMATCH_STATE_STAGE_READY -> BUDOKAI_FINALMATCH_STATE_STAGE_RUN, m_dwMatchTickCount = %u",
					it->first, match->m_dwMatchTickCount);

				//update players state to normal
				FinalMatchUpdatePlayersState(match, it->first, MATCH_MEMBER_STATE_NORMAL);

				//this packet will show "START"
				CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
				sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
				res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
				res2->byMatchState = BUDOKAI_FINALMATCH_STATE_STAGE_RUN;
				res2->bIsEnter = true;
				res2->byStage = match->byStage;
				res2->dwRemainTime = match->m_dwMatchTickCount;
				match->m_pWorld->Broadcast(&packet2);
			}
		}
		break;
		case BUDOKAI_FINALMATCH_STATE_STAGE_RUN:
		{
			if (FinalMatchCheckPlayerCount(match, it->first) == true)
			{
				//progress fight
				if (ProcessFinalMatch(match, it->first, dwTickDif) == true)
				{
					if (match->m_dwMatchTickCount == 0)
					{
						FinalMatchTimeEnd(match, it->first);
					}
				}
			}
		}
		break;
		case BUDOKAI_FINALMATCH_STATE_STAGE_FINISH:
		{
			if (match->m_dwMatchTickCount == 0)
			{
				ResetPlayerFinalMatch(match, it->first, true);

				//check if match done. Otherwise continue
				if (match->bFinishMatch == false)
				{
					match->m_byMatchState = BUDOKAI_FINALMATCH_STATE_STAGE_READY;
					match->m_dwMatchTickCount = 3000;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Final Match. Index %u, byState BUDOKAI_FINALMATCH_STATE_STAGE_FINISH -> BUDOKAI_FINALMATCH_STATE_STAGE_READY, m_dwMatchTickCount = %u",
						it->first, match->m_dwMatchTickCount);

					CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_FINALMATCH_STATE_STAGE_READY;
					res2->bIsEnter = true;
					res2->byStage = match->byStage;
					res2->dwRemainTime = match->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					match->m_pWorld->Broadcast(&packet2);
				}
				else
				{
					match->m_byMatchState = BUDOKAI_FINALMATCH_STATE_MATCH_FINISH;
					match->m_dwMatchTickCount = 15000;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Final Match. Index %u, byState BUDOKAI_FINALMATCH_STATE_STAGE_FINISH -> BUDOKAI_FINALMATCH_STATE_MATCH_FINISH, m_dwMatchTickCount = %u",
						it->first, match->m_dwMatchTickCount);

					CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_FINALMATCH_STATE_MATCH_FINISH;
					res2->bIsEnter = true;
					res2->byStage = match->byStage;
					res2->dwRemainTime = match->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					match->m_pWorld->Broadcast(&packet2);

					//send shit that finish..And show winners
					FinalMatchMatchFinish(match, it->first);
				}
			}
		}
		break;
		case BUDOKAI_FINALMATCH_STATE_MATCH_FINISH:
		{
			if (match->m_dwMatchTickCount == 0)
			{
				if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_2)
				{
					match->m_byMatchState = BUDOKAI_FINALMATCH_STATE_AWARDING;
					match->m_dwMatchTickCount = m_pTableInfo->dwAwardingTime * 1000;

					FinalMatchFinalDirection(match);

					FinalMatchAwarding(match);

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Final Match. Index %u, byState BUDOKAI_FINALMATCH_STATE_MATCH_FINISH -> BUDOKAI_FINALMATCH_STATE_AWARDING, m_dwMatchTickCount = %u",
						it->first, match->m_dwMatchTickCount);

					CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_FINALMATCH_STATE_AWARDING;
					res2->bIsEnter = true;
					res2->byStage = match->byStage;
					res2->dwRemainTime = match->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					match->m_pWorld->Broadcast(&packet2);
				}
				else
				{
					match->m_byMatchState = BUDOKAI_FINALMATCH_STATE_END;
					match->m_dwMatchTickCount = 0;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Final Match. Index %u, byState BUDOKAI_FINALMATCH_STATE_MATCH_FINISH -> BUDOKAI_FINALMATCH_STATE_END, m_dwMatchTickCount = %u",
						it->first, match->m_dwMatchTickCount);

					CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_FINALMATCH_STATE_END;
					res2->bIsEnter = true;
					res2->byStage = match->byStage;
					res2->dwRemainTime = match->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					match->m_pWorld->Broadcast(&packet2);

					AddIndividualToNextRound(match, it->first, match->data.wWinnerTeam, match->wLoserJoinID);
				}
			}
		}
		break;

		case BUDOKAI_FINALMATCH_STATE_FINALDIRECTION: //direction for finals. Used only in final match
		{
			if (match->m_dwMatchTickCount == 0)
			{
				if (FinalMatchCheckPlayerCount(match, it->first) == true)
				{
					match->m_byMatchState = BUDOKAI_FINALMATCH_STATE_STAGE_READY;
					match->m_dwMatchTickCount = 5000;

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Final Match. Index %u, byState BUDOKAI_FINALMATCH_STATE_FINALDIRECTION -> BUDOKAI_FINALMATCH_STATE_STAGE_READY, m_dwMatchTickCount = %u",
						it->first, match->m_dwMatchTickCount);

					CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = BUDOKAI_FINALMATCH_STATE_STAGE_READY;
					res2->bIsEnter = true;
					res2->byStage = match->byStage;
					res2->dwRemainTime = match->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
					match->m_pWorld->Broadcast(&packet2);
				}
			}
		}
		break;

		case BUDOKAI_FINALMATCH_STATE_AWARDING:
		{
			if (match->m_dwMatchTickCount == 0)
			{
				match->m_byMatchState = BUDOKAI_FINALMATCH_STATE_END;
				match->m_dwMatchTickCount = 0;

				ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Final Match. Index %u, byState BUDOKAI_FINALMATCH_STATE_AWARDING -> BUDOKAI_FINALMATCH_STATE_END, m_dwMatchTickCount = %u",
					it->first, match->m_dwMatchTickCount);

				CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
				sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
				res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
				res2->byMatchState = BUDOKAI_FINALMATCH_STATE_END;
				res2->bIsEnter = true;
				res2->byStage = match->byStage;
				res2->dwRemainTime = match->m_dwMatchTickCount;
				packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
				match->m_pWorld->Broadcast(&packet2);
			}
		}
		break;

		case BUDOKAI_FINALMATCH_STATE_END:
		{
			if (match->m_dwMatchTickCount == 0)
			{
				match->bEnd = true;
				FinalMatchEnd(match, it->first);

				m_aTournamentMatch[m_eMatchDepth].byMatchCountEnd += 1;
			}
		}
		break;

		default: ERR_LOG(LOG_GENERAL, "Unknown match state %u", match->m_byMatchState);
		}
	}
}

void CBudokaiManager::StartFinalMatchTeleport()
{
	CGameServer* app = (CGameServer*)g_pApp;

	ERR_LOG(LOG_GENERAL, "BUDOKAI: StartFinalMatchTeleport: m_eMatchDepth = %u", m_eMatchDepth);

	int i = 0;

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		CNtlPacket packet(sizeof(sGT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY));
		sGT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY * res = (sGT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY *)packet.GetPacketData();
		res->wOpCode = GT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY;
		res->byServerChannelId = DOJO_CHANNEL_INDEX;
		res->byServerIndex = 0;
		memset(res->aTeleportData, 0, sizeof(res->aTeleportData));

		for (std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.begin(); it != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end(); it++)
		{
			std::map<JOINID, sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA*>::iterator it2 = m_mapIndividualTournament.find(it->second->data.wJoinId1);
			if (it2 != m_mapIndividualTournament.end())
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: Success StartFinalMatchTeleport: i = %u, WorldTblix = %u, CharID = %u, WorldId = %u, JoinID1 = %u",
					i, m_pTableInfo->sIndividualWorldTblidx.finalMatch, it2->second->charId, it->second->m_pWorld->GetID(), it->second->data.wJoinId1);

				res->aTeleportData[i].worldTblidx = m_pTableInfo->sIndividualWorldTblidx.finalMatch;
				res->aTeleportData[i].byPlayerCount = 1;
				res->aTeleportData[i].aPlayers[0] = it2->second->charId;
				res->aTeleportData[i++].worldId = it->second->m_pWorld->GetID();
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: Failed StartFinalMatchTeleport: i = %u, WorldTblix = %u, JoinID1 = %u",
					i, m_pTableInfo->sIndividualWorldTblidx.finalMatch, it->second->data.wJoinId1);
			}

			std::map<JOINID, sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA*>::iterator it3 = m_mapIndividualTournament.find(it->second->data.wJoinId2);
			if (it3 != m_mapIndividualTournament.end())
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: Success StartFinalMatchTeleport: i = %u, WorldTblix = %u, CharID = %u, WorldId = %u, JoinID2 = %u",
					i, m_pTableInfo->sIndividualWorldTblidx.finalMatch, it3->second->charId, it->second->m_pWorld->GetID(), it->second->data.wJoinId2);

				res->aTeleportData[i].worldTblidx = m_pTableInfo->sIndividualWorldTblidx.finalMatch;
				res->aTeleportData[i].byPlayerCount = 1;
				res->aTeleportData[i].aPlayers[0] = it3->second->charId;
				res->aTeleportData[i++].worldId = it->second->m_pWorld->GetID();
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: Failed StartFinalMatchTeleport: i = %u, WorldTblix = %u, JoinID2 = %u",
					i, m_pTableInfo->sIndividualWorldTblidx.finalMatch, it->second->data.wJoinId2);
			}

			//add spectators
			for (std::set<JOINID>::iterator it4 = it->second->m_setSpectator.begin(); it4 != it->second->m_setSpectator.end(); it4++)
			{
				std::map<JOINID, sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA*>::iterator it5 = m_mapIndividualTournament.find(*it4);
				if (it5 != m_mapIndividualTournament.end())
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: Success StartFinalMatchTeleport <ADD SPECTATOR>: i = %u, WorldTblix = %u, CharID = %u, WorldId = %u, JoinID = %u",
						i, m_pTableInfo->sIndividualWorldTblidx.finalMatch, it5->second->charId, it->second->m_pWorld->GetID(), *it4);

					res->aTeleportData[i].worldTblidx = m_pTableInfo->sIndividualWorldTblidx.finalMatch;
					res->aTeleportData[i].byPlayerCount = 1;
					res->aTeleportData[i].aPlayers[0] = it5->second->charId;
					res->aTeleportData[i++].worldId = it->second->m_pWorld->GetID();
				}
			}
		}

		packet.SetPacketLen(sizeof(sGT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY));
		app->SendTo(app->GetChatServerSession(), &packet);
	}
	else
	{
		CNtlPacket packet(sizeof(sGT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY));
		sGT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY * res = (sGT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY *)packet.GetPacketData();
		res->wOpCode = GT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY;
		res->byServerChannelId = DOJO_CHANNEL_INDEX;
		res->byServerIndex = 0;
		memset(res->aTeleportData, 0, sizeof(res->aTeleportData));

		for (std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.begin(); it != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end(); it++)
		{
			std::map<JOINID, sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA*>::iterator it2 = m_mapTeamTournament.find(it->second->data.wJoinId1);
			if (it2 != m_mapTeamTournament.end())
			{
				res->aTeleportData[i].worldTblidx = m_pTableInfo->sTeamWorldTblidx.finalMatch;
				res->aTeleportData[i].byTeamCount = 1;

				for (BYTE memberCnt = 0; memberCnt < it2->second->byMemberCount; memberCnt++)
				{
					res->aTeleportData[i].aTeamList[0].charId[res->aTeleportData[i].aTeamList[0].byCount] = it2->second->aMembers[memberCnt].charId;
					res->aTeleportData[i].aTeamList[0].byCount++;
				}

				res->aTeleportData[i++].worldId = it->second->m_pWorld->GetID();
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: Failed StartFinalMatchTeleport: i = %u, WorldTblix = %u, JoinID1 = %u",
					i, m_pTableInfo->sTeamWorldTblidx.finalMatch, it->second->data.wJoinId1);
			}

			if (it->second->data.wJoinId2 != INVALID_TEAMTYPE)
			{
				std::map<JOINID, sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA*>::iterator it3 = m_mapTeamTournament.find(it->second->data.wJoinId2);
				if (it3 != m_mapTeamTournament.end())
				{
					res->aTeleportData[i].worldTblidx = m_pTableInfo->sTeamWorldTblidx.finalMatch;
					res->aTeleportData[i].byTeamCount = 1;

					for (BYTE memberCnt = 0; memberCnt < it3->second->byMemberCount; memberCnt++)
					{
						res->aTeleportData[i].aTeamList[0].charId[res->aTeleportData[i].aTeamList[0].byCount] = it3->second->aMembers[memberCnt].charId;
						res->aTeleportData[i].aTeamList[0].byCount++;
					}

					res->aTeleportData[i++].worldId = it->second->m_pWorld->GetID();
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: Failed StartFinalMatchTeleport: i = %u, WorldTblix = %u, JoinID2 = %u",
						i, m_pTableInfo->sTeamWorldTblidx.finalMatch, it->second->data.wJoinId2);
				}
			}

			//add spectators
			for (std::set<JOINID>::iterator it4 = it->second->m_setSpectator.begin(); it4 != it->second->m_setSpectator.end(); it4++)
			{
				std::map<JOINID, sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA*>::iterator it5 = m_mapTeamTournament.find(*it4);
				if (it5 != m_mapTeamTournament.end())
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: Success StartFinalMatchTeleport <ADD SPECTATOR>: i = %u, WorldTblix = %u,, WorldId = %u, JoinID = %u",
						i, m_pTableInfo->sIndividualWorldTblidx.finalMatch, it->second->m_pWorld->GetID(), *it4);

					res->aTeleportData[i].worldTblidx = m_pTableInfo->sTeamWorldTblidx.finalMatch;
					res->aTeleportData[i].byTeamCount = 1;

					for (BYTE memberCnt = 0; memberCnt < it5->second->byMemberCount; memberCnt++)
					{
						res->aTeleportData[i].aTeamList[0].charId[res->aTeleportData[i].aTeamList[0].byCount] = it5->second->aMembers[memberCnt].charId;
						res->aTeleportData[i].aTeamList[0].byCount++;
					}

					res->aTeleportData[i++].worldId = it->second->m_pWorld->GetID();
				}
			}

			if (i == BUDOKAI_MAX_TEAM_MATCH_COUNT)
				break;
		}

		packet.SetPacketLen(sizeof(sGT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY));
		app->SendTo(app->GetChatServerSession(), &packet);
	}
}

void CBudokaiManager::SendFinalMatchTeamInfo(BYTE byMatchIndex, sTOURNAMENT_MATCH * match)
{
	sVARIABLE_DATA sData;
	sData.Init(1024);

	BYTE i = 0;
	WORD wTeamInfo_var = 0;

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		sMATCH_FINALMATCH_TEAM_INFO_VAR info[2]; // 4 because 2 outside and 2 inside

		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer && pPlayer->GetMatchIndex() == byMatchIndex)
			{
				auto it2 = m_mapIndividual.find(pPlayer->GetJoinID());
				if (it2 != m_mapIndividual.end())
				{
					info[i].wTeamType = GetIndividualTeamType(it2->second.charId);

					pPlayer->SetBudokaiTeamType(info[i].wTeamType);

					if (it->second.bSpectator)
						continue;

					if (info[i].wTeamType == MATCH_TEAM_TYPE_TEAM1)
						match->m_byTeam1PlayerCount += 1;
					else if (info[i].wTeamType == MATCH_TEAM_TYPE_TEAM2)
						match->m_byTeam2PlayerCount += 1;

					info[i].wTeamName_var = 0;
					info[i].bDojoRecommender = it2->second.bDojoRecommender;
					info[i].byMemberCount = 1;

					WCHAR			m_wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
					WCHAR			m_wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];

					NTL_SAFE_WCSCPY(m_wszCharName, pPlayer->GetCharName());
					NTL_SAFE_WCSCPY(m_wszGuildName, pPlayer->GetGuildName());

					sMATCH_MEMBER_STATE_INFO memberInfo[1];
					memberInfo[0].handle = pPlayer->GetID();
					memberInfo[0].byState = pPlayer->GetBudokaiPcState();
					memberInfo[0].byLevel = pPlayer->GetLevel();
					memberInfo[0].byClass = pPlayer->GetClass();
					memberInfo[0].wGuildName = sData.Write(sizeof(m_wszGuildName), m_wszGuildName);
					memberInfo[0].wWinCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwWin;
					memberInfo[0].wDrawCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwDraw;
					memberInfo[0].wLoseCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwLose;
					memberInfo[0].wCharName = sData.Write(sizeof(m_wszCharName), m_wszCharName);

					info[i].wTeamName_var = sData.Write(sizeof(m_wszCharName), m_wszCharName);
					info[i].wMembers_var = sData.Write(sizeof(memberInfo), memberInfo);

					++i;
				}
			}
		}

		wTeamInfo_var = sData.Write(sizeof(info), info);
	}
	else
	{
		sMATCH_FINALMATCH_TEAM_INFO_VAR info[2];
		WCHAR			charName[NTL_MAX_SIZE_CHAR_NAME + 1];
		WCHAR			guildName[NTL_MAX_SIZE_GUILD_NAME + 1];
		WCHAR			partyName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];

		for (std::map<JOINID, sPARTY_INFO>::iterator it = match->m_mapTeam.begin(); it != match->m_mapTeam.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;

			if (partyInfo->bSpectator)
			{
				for (BYTE memberCnt = 0; memberCnt < partyInfo->byMemberCount; memberCnt++)
				{
					CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[memberCnt]);
					if (pPlayer && pPlayer->GetCharID() == partyInfo->charId[memberCnt])
					{
						pPlayer->SetBudokaiTeamType(GetIndividualTeamType(it->first));
					}
				}

				continue;
			}

			sMATCH_MEMBER_STATE_INFO memberInfo[NTL_MAX_MEMBER_IN_PARTY];
			memset(memberInfo, -1, sizeof(memberInfo));

			info[i].wTeamType = GetIndividualTeamType(it->first);

			partyInfo->teamType = info[i].wTeamType;

			info[i].wTeamName_var = 0;
			info[i].bDojoRecommender = false;
			info[i].byMemberCount = 0;

			NTL_SAFE_WCSCPY(partyName, partyInfo->m_wszTeamName);
			info[i].wTeamName_var = sData.Write(sizeof(partyName), partyName);

			for (BYTE memberCnt = 0; memberCnt < partyInfo->byMemberCount; memberCnt++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[memberCnt]);
				if (pPlayer && pPlayer->GetCharID() == partyInfo->charId[memberCnt])
				{
					NTL_SAFE_WCSCPY(charName, pPlayer->GetCharName());
					NTL_SAFE_WCSCPY(guildName, pPlayer->GetGuildName());

					pPlayer->SetBudokaiTeamType(info[i].wTeamType);

					if (info[i].wTeamType == MATCH_TEAM_TYPE_TEAM1)
						match->m_byTeam1PlayerCount += 1;
					else if (info[i].wTeamType == MATCH_TEAM_TYPE_TEAM2)
						match->m_byTeam2PlayerCount += 1;

					memberInfo[info[i].byMemberCount].handle = pPlayer->GetID();
					memberInfo[info[i].byMemberCount].byState = pPlayer->GetBudokaiPcState();
					memberInfo[info[i].byMemberCount].byLevel = pPlayer->GetLevel();
					memberInfo[info[i].byMemberCount].byClass = pPlayer->GetClass();
					memberInfo[info[i].byMemberCount].wGuildName = sData.Write(sizeof(guildName), guildName);
					memberInfo[info[i].byMemberCount].wWinCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwWin;
					memberInfo[info[i].byMemberCount].wDrawCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwDraw;
					memberInfo[info[i].byMemberCount].wLoseCount = (WORD)pPlayer->GetRankBattleScoreInfo()->dwLose;
					memberInfo[info[i].byMemberCount++].wCharName = sData.Write(sizeof(charName), charName);
				}
			}

			info[i].wMembers_var = sData.Write(sizeof(memberInfo), memberInfo);

			++i;
		}

		wTeamInfo_var = sData.Write(sizeof(info), info);
	}

	WORD wPacketSize = sData.GetPacketSize(sizeof(sGU_MATCH_FINALMATCH_TEAM_INFO_NFY));

	CNtlPacket packet(wPacketSize);
	sGU_MATCH_FINALMATCH_TEAM_INFO_NFY * res = (sGU_MATCH_FINALMATCH_TEAM_INFO_NFY *)packet.GetPacketData();
	res->wOpCode = GU_MATCH_FINALMATCH_TEAM_INFO_NFY;
	res->byMatchIndex = byMatchIndex;
	res->byMatchDepth = m_eMatchDepth;
	res->byTeamCount = i;
	res->wTeamInfo_var = wTeamInfo_var;
	sData.CopyTo(&res->sData, sData.GetDataSize());
	match->m_pWorld->Broadcast(&packet);

	ERR_LOG(LOG_GENERAL, "Budokai: Final wPacketSize %u, sData.GetDataSize() %u, byTeamCount %u \n", wPacketSize, sData.GetDataSize(), res->byTeamCount);
}

void CBudokaiManager::FinalMatchUpdatePlayersState(sTOURNAMENT_MATCH * match, BYTE byMatchIndex, BYTE byState)
{
	for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
	{
		if (it->second.bSpectator == false) //dont need to update spectators state
		{
			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer)
			{
				if (pPlayer->GetCharID() == it->second.charId)
				{
					if (pPlayer->GetMatchIndex() == byMatchIndex)
					{
						pPlayer->SetBudokaiPcState(byState);

						CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY));
						sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
						res2->wOpCode = GU_MATCH_FINALMATCH_PLAYER_STATE_NFY;
						res2->hPc = it->first;
						res2->byPcState = byState;
						match->m_pWorld->Broadcast(&packet2);
					}
					else ERR_LOG(LOG_GENERAL, "Player has different match index %u != %u", pPlayer->GetMatchIndex(), byMatchIndex);
				}
				else ERR_LOG(LOG_GENERAL, "Player has different char id %u != %u", pPlayer->GetCharID(), it->second.charId);
			}
			else ERR_LOG(LOG_GENERAL, "Could not find player");
		}
	}
}

bool CBudokaiManager::ProcessFinalMatch(sTOURNAMENT_MATCH * match, BYTE byMatchIndex, DWORD dwTickDif)
{
	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			sPLAYER_INFO& info = it->second;
			if (info.bSpectator == false) // no need to check spectrators
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
				if (pPlayer)
				{
					if (pPlayer->GetCharID() == info.charId)
					{
						if (pPlayer->GetMatchIndex() == byMatchIndex)
						{
							if (pPlayer->GetBudokaiPcState() == MATCH_MEMBER_STATE_NORMAL)
							{
								if (IsInsideArena(pPlayer->GetCurLoc(), pPlayer->GetCurWorld()) == false)
								{
									pPlayer->SetBudokaiPcState(MATCH_MEMBER_STATE_OUTOFAREA);

									CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY));
									sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
									res2->wOpCode = GU_MATCH_FINALMATCH_PLAYER_STATE_NFY;
									res2->hPc = pPlayer->GetID();
									res2->byPcState = MATCH_MEMBER_STATE_OUTOFAREA;
									packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY));
									match->m_pWorld->Broadcast(&packet2);

									// --- Lose Round

									TEAMTYPE wMatchWinner = (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1) ? MATCH_TEAM_TYPE_TEAM2 : MATCH_TEAM_TYPE_TEAM1;

									UpdateFinalMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, wMatchWinner);

									return false;
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		for (std::map<JOINID, sPARTY_INFO>::iterator it = match->m_mapTeam.begin(); it != match->m_mapTeam.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;

			if (partyInfo->bSpectator)
				continue;

			for (BYTE i = 0; i < partyInfo->byMemberCount; i++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[i]);
				if (pPlayer)
				{
					if (pPlayer->GetCharID() == partyInfo->charId[i])
					{
						if (pPlayer->GetMatchIndex() == byMatchIndex)
						{
							if (pPlayer->GetBudokaiPcState() == MATCH_MEMBER_STATE_NORMAL)
							{
								if (IsInsideArena(pPlayer->GetCurLoc(), pPlayer->GetCurWorld()) == false)
								{
									pPlayer->SetBudokaiPcState(MATCH_MEMBER_STATE_OUTOFAREA);

									CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
									sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
									res2->wOpCode = GU_MATCH_MAJORMATCH_PLAYER_STATE_NFY;
									res2->hPc = pPlayer->GetID();
									res2->byPcState = MATCH_MEMBER_STATE_OUTOFAREA;
									packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
									match->m_pWorld->Broadcast(&packet2);

									if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1)
									{
										match->m_byTeam1UnavailablePlayerCount += 1;

										if (match->m_byTeam1UnavailablePlayerCount >= match->m_byTeam1PlayerCount)
										{
											UpdateFinalMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, MATCH_TEAM_TYPE_TEAM2);
											return false;
										}
									}
									else if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM2)
									{
										match->m_byTeam2UnavailablePlayerCount += 1;

										if (match->m_byTeam2UnavailablePlayerCount >= match->m_byTeam2PlayerCount)
										{
											UpdateFinalMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, MATCH_TEAM_TYPE_TEAM1);
											return false;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return true;
}

void CBudokaiManager::UpdateFinalMatchScore(sTOURNAMENT_MATCH * match, BYTE byMatchIndex, BYTE byMatchResult, TEAMTYPE wMatchWinner, BYTE byWins/* = 1*/)
{
	if (wMatchWinner == MATCH_TEAM_TYPE_TEAM1)
	{
		match->data.byScore1 += byWins;
		if (match->data.byScore1 > 3)
			match->data.byScore1 = 3;
	}
	else
	{
		match->data.byScore2 += byWins;
		if (match->data.byScore2 > 3)
			match->data.byScore2 = 3;
	}

	CNtlPacket packet(sizeof(sGU_MATCH_FINALMATCH_STAGE_FINISH_NFY));
	sGU_MATCH_FINALMATCH_STAGE_FINISH_NFY * res = (sGU_MATCH_FINALMATCH_STAGE_FINISH_NFY *)packet.GetPacketData();
	res->wOpCode = GU_MATCH_FINALMATCH_STAGE_FINISH_NFY;
	res->sMatchScore.byTeam1 = match->data.byScore1;
	res->sMatchScore.byTeam2 = match->data.byScore2;
	res->byStageResult = byMatchResult;
	res->wStageWinner = wMatchWinner;
	res->bIsObserver = false;
	res->nGetPoint = FALSE;
	packet.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STAGE_FINISH_NFY));
	match->m_pWorld->Broadcast(&packet);

	match->byStage += byWins;

	//update players state to none
	FinalMatchUpdatePlayersState(match, byMatchIndex, MATCH_MEMBER_STATE_NONE);

	//check if match finish
	if (match->data.byScore1 >= 3 || match->data.byScore2 >= 3)
	{
		ERR_LOG(LOG_GENERAL, "BUDOKAI: Update Tournament Final Match. Index %u. Winner-Team = %u. Score1 = %u, Score2 = %u, byMatchResult = %u ",
			byMatchIndex, wMatchWinner, match->data.byScore1, match->data.byScore2, byMatchResult);

		match->bFinishMatch = true;
	}

	match->m_byMatchState = BUDOKAI_FINALMATCH_STATE_NONE;
	match->m_dwMatchTickCount = 5000;

	CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
	sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
	res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
	res2->byMatchState = BUDOKAI_FINALMATCH_STATE_NONE;
	res2->bIsEnter = true;
	res2->byStage = match->byStage;
	res2->dwRemainTime = match->m_dwMatchTickCount;
	packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
	match->m_pWorld->Broadcast(&packet2);
}

void CBudokaiManager::FinalMatchMatchFinish(sTOURNAMENT_MATCH * match, BYTE byMatchIndex)
{
	CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_MATCH_FINISH_NFY));
	sGU_MATCH_FINALMATCH_MATCH_FINISH_NFY * res2 = (sGU_MATCH_FINALMATCH_MATCH_FINISH_NFY *)packet2.GetPacketData();
	res2->wOpCode = GU_MATCH_FINALMATCH_MATCH_FINISH_NFY;
	res2->byMatchResult = MATCH_RESULT_WIN;
	res2->sMatchScore.byTeam1 = match->data.byScore1;
	res2->sMatchScore.byTeam2 = match->data.byScore2;

	if (match->data.byScore1 > match->data.byScore2)
	{
		match->wLoserJoinID = match->data.wJoinId2;
		match->data.wWinnerTeam = match->data.wJoinId1;
		res2->wMatchWinner = MATCH_TEAM_TYPE_TEAM1;
	}
	else
	{
		match->wLoserJoinID = match->data.wJoinId1;
		match->data.wWinnerTeam = match->data.wJoinId2;
		res2->wMatchWinner = MATCH_TEAM_TYPE_TEAM2;
	}

	packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_MATCH_FINISH_NFY));
	match->m_pWorld->Broadcast(&packet2);

	// ----

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			sPLAYER_INFO& info = it->second;

			if (info.bSpectator == false) //spectators do not receive reward
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
				if (pPlayer)
				{
					pPlayer->SetBudokaiPcState(MATCH_MEMBER_STATE_NORMAL);

					CNtlPacket packet3(sizeof(sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY));
					sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY * res3 = (sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY *)packet3.GetPacketData();
					res3->wOpCode = GU_MATCH_FINALMATCH_PLAYER_STATE_NFY;
					res3->hPc = it->first;
					res3->byPcState = MATCH_MEMBER_STATE_NORMAL;
					packet3.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY));
					pPlayer->SendPacket(&packet3);

					if (pPlayer->GetCharID() == it->second.charId && pPlayer->GetMatchIndex() == byMatchIndex)
					{
						DWORD dwMudosaPoints = 0;
						TBLIDX itemIdx = INVALID_TBLIDX;
						BYTE byItemCount = 0;


						if (pPlayer->GetJoinID() == match->data.wWinnerTeam)
						{
							match->data.byMatchResult = MATCH_RESULT_WIN;

							if (m_type == BUDOKAI_TYPE_JUNIOR)
							{
								dwMudosaPoints = m_pTableInfo->sJuniorIndividualReward.aMajorMatch[m_eMatchDepth].dwBaseMudosaPoint + m_pTableInfo->sJuniorIndividualReward.aMajorMatch[m_eMatchDepth].dwWinnerMudosaPoint;
								itemIdx = m_pTableInfo->sJuniorIndividualReward.aMajorMatch[m_eMatchDepth].winnerItem;
								byItemCount = m_pTableInfo->sJuniorIndividualReward.aMajorMatch[m_eMatchDepth].byWinerItemStackCount;
							}
							else
							{
								dwMudosaPoints = m_pTableInfo->sIndividualReward.aMajorMatch[m_eMatchDepth].dwBaseMudosaPoint + m_pTableInfo->sIndividualReward.aMajorMatch[m_eMatchDepth].dwWinnerMudosaPoint;
								itemIdx = m_pTableInfo->sIndividualReward.aMajorMatch[m_eMatchDepth].winnerItem;
								byItemCount = m_pTableInfo->sIndividualReward.aMajorMatch[m_eMatchDepth].byWinerItemStackCount;
							}
						}
						else if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1 || pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM2)
						{
							if (m_type == BUDOKAI_TYPE_JUNIOR)
							{
								dwMudosaPoints = m_pTableInfo->sJuniorIndividualReward.aMajorMatch[m_eMatchDepth].dwBaseMudosaPoint;
								itemIdx = m_pTableInfo->sJuniorIndividualReward.aMajorMatch[m_eMatchDepth].loserItem;
								byItemCount = m_pTableInfo->sJuniorIndividualReward.aMajorMatch[m_eMatchDepth].byLoserItemStackCount;
							}
							else
							{
								dwMudosaPoints = m_pTableInfo->sIndividualReward.aMajorMatch[m_eMatchDepth].dwBaseMudosaPoint;
								itemIdx = m_pTableInfo->sIndividualReward.aMajorMatch[m_eMatchDepth].loserItem;
								byItemCount = m_pTableInfo->sIndividualReward.aMajorMatch[m_eMatchDepth].byLoserItemStackCount;
							}
						}

						ERR_LOG(LOG_GENERAL, "Budokai Final match reward: CharID = %u, Item Idx = %u, Item Count = %u, Mudosa Points = %u", pPlayer->GetCharID(), itemIdx, byItemCount, dwMudosaPoints);

						pPlayer->UpdateMudosaPoints(pPlayer->GetMudosaPoints() + dwMudosaPoints, true);

						if (itemIdx != INVALID_TBLIDX)
							g_pItemManager->CreateQuestRewardItem(pPlayer, itemIdx, byItemCount);
					}
				}
			}
		}
	}
	else
	{
		for (std::map<JOINID, sPARTY_INFO>::iterator it = match->m_mapTeam.begin(); it != match->m_mapTeam.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;

			DWORD dwMudosaPoints = 0;
			TBLIDX itemIdx = INVALID_TBLIDX;
			BYTE byItemCount = 0;

			if (partyInfo->joinId == match->data.wWinnerTeam)
			{
				match->data.byMatchResult = MATCH_RESULT_WIN;

				if (m_type == BUDOKAI_TYPE_JUNIOR)
				{
					dwMudosaPoints = m_pTableInfo->sJuniorTeamReward.sMinorMatch.dwBaseMudosaPoint + m_pTableInfo->sJuniorTeamReward.sMinorMatch.dwWinnerMudosaPoint + (m_pTableInfo->sJuniorTeamReward.sMinorMatch.dwKillCountPoint * (DWORD)partyInfo->byKills);
					itemIdx = m_pTableInfo->sJuniorTeamReward.sMinorMatch.winnerItem;
					byItemCount = m_pTableInfo->sJuniorTeamReward.sMinorMatch.byWinerItemStackCount;
				}
				else
				{
					dwMudosaPoints = m_pTableInfo->sTeamReward.sMinorMatch.dwBaseMudosaPoint + m_pTableInfo->sTeamReward.sMinorMatch.dwWinnerMudosaPoint + (m_pTableInfo->sTeamReward.sMinorMatch.dwKillCountPoint * (DWORD)partyInfo->byKills);
					itemIdx = m_pTableInfo->sTeamReward.sMinorMatch.winnerItem;
					byItemCount = m_pTableInfo->sTeamReward.sMinorMatch.byWinerItemStackCount;
				}
			}
			else
			{
				if (m_type == BUDOKAI_TYPE_JUNIOR)
				{
					dwMudosaPoints = m_pTableInfo->sJuniorTeamReward.sMinorMatch.dwBaseMudosaPoint + (m_pTableInfo->sJuniorTeamReward.sMinorMatch.dwKillCountPoint * (DWORD)partyInfo->byKills);
					itemIdx = m_pTableInfo->sJuniorTeamReward.sMinorMatch.loserItem;
					byItemCount = m_pTableInfo->sJuniorTeamReward.sMinorMatch.byLoserItemStackCount;
				}
				else
				{
					dwMudosaPoints = m_pTableInfo->sTeamReward.sMinorMatch.dwBaseMudosaPoint + (m_pTableInfo->sTeamReward.sMinorMatch.dwKillCountPoint * (DWORD)partyInfo->byKills);
					itemIdx = m_pTableInfo->sTeamReward.sMinorMatch.loserItem;
					byItemCount = m_pTableInfo->sTeamReward.sMinorMatch.byLoserItemStackCount;
				}
			}

			for (BYTE byMemberCount = 0; byMemberCount < partyInfo->byMemberCount; byMemberCount++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[byMemberCount]);
				if (pPlayer && partyInfo->charId[byMemberCount] == pPlayer->GetCharID() && pPlayer->GetMatchIndex() == byMatchIndex)
				{
					pPlayer->SetBudokaiPcState(MATCH_MEMBER_STATE_NORMAL);

					CNtlPacket packet3(sizeof(sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY));
					sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY * res3 = (sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY *)packet3.GetPacketData();
					res3->wOpCode = GU_MATCH_FINALMATCH_PLAYER_STATE_NFY;
					res3->hPc = it->first;
					res3->byPcState = MATCH_MEMBER_STATE_NORMAL;
					packet3.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY));
					pPlayer->SendPacket(&packet3);

					pPlayer->UpdateMudosaPoints(pPlayer->GetMudosaPoints() + dwMudosaPoints, true);

					if (itemIdx != INVALID_TBLIDX)
						g_pItemManager->CreateQuestRewardItem(pPlayer, itemIdx, byItemCount);
				}
			}
		}
	}
}


void CBudokaiManager::ResetPlayerFinalMatch(sTOURNAMENT_MATCH * match, BYTE byMatchIndex, bool bSpawn)
{
	match->m_byTeam1UnavailablePlayerCount = 0;
	match->m_byTeam2UnavailablePlayerCount = 0;

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			sPLAYER_INFO& info = it->second;

			if (info.bSpectator == false) //no need reset spectrators after a match
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
				if (pPlayer)
				{
					if (pPlayer->GetCharID() == info.charId)
					{
						if (pPlayer->GetMatchIndex() == byMatchIndex)
						{
							pPlayer->ChangeAttackProgress(false);
							pPlayer->SetAttackTarget(INVALID_HOBJECT);

							//remove buffs
							pPlayer->GetBuffManager()->RemoveAllBuff();

							//if transformed then cancel transformation
							if (pPlayer->GetTransformationTbldat())
								pPlayer->CancelTransformation();

							if (pPlayer->GetStateManager()->GetConditionState() > 0)
							{
								pPlayer->GetStateManager()->RemoveConditionFlags(INVALID_QWORD, true);
							}

							//re-calculate all stats
							pPlayer->GetCharAtt()->CalculateAll();

							CNtlVector vNewLoc;

							if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1)
							{
								vNewLoc = m_pTableInfo->aFinalLoc[0].vLoc;
								pPlayer->SetCurDir(m_pTableInfo->aFinalLoc[0].vDir);
							}
							else if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM2)
							{
								vNewLoc = m_pTableInfo->aFinalLoc[5].vLoc;
								pPlayer->SetCurDir(m_pTableInfo->aFinalLoc[5].vDir);
							}

							pPlayer->SetIsReviving(false);

							//restore lp/rp/rp/rp balls
							pPlayer->UpdateCurLP(pPlayer->GetMaxLP(), true, false);
							pPlayer->UpdateCurEP(pPlayer->GetMaxEP(), true, false);
							pPlayer->UpdateRpBall(pPlayer->GetMaxRPBall(), false, false);
							pPlayer->UpdateCurRP(pPlayer->GetCharAtt()->GetMaxRP(), false, false);

							//remove target 
							pPlayer->ChangeTarget(INVALID_HOBJECT);

							//clear cc reduction
							pPlayer->ClearCrowdControlReduction();

							if (bSpawn)
								pPlayer->StartTeleport(vNewLoc, pPlayer->GetCurDir(), match->m_pWorld->GetID(), TELEPORT_TYPE_FINALMATCH);
						}
					}
				}
			}
		}
	}
	else
	{
		for (std::map<JOINID, sPARTY_INFO>::iterator it = match->m_mapTeam.begin(); it != match->m_mapTeam.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;

			if (partyInfo->bSpectator)
				continue;

			for (BYTE i = 0; i < partyInfo->byMemberCount; i++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[i]);
				if (pPlayer)
				{
					if (pPlayer->GetCharID() == partyInfo->charId[i])
					{
						if (pPlayer->GetMatchIndex() == byMatchIndex)
						{
							pPlayer->ChangeAttackProgress(false);
							pPlayer->SetAttackTarget(INVALID_HOBJECT);

							//remove buffs
							pPlayer->GetBuffManager()->RemoveAllBuff();

							//if transformed then cancel transformation
							if (pPlayer->GetTransformationTbldat())
								pPlayer->CancelTransformation();

							if (pPlayer->GetStateManager()->GetConditionState() > 0)
							{
								pPlayer->GetStateManager()->RemoveConditionFlags(INVALID_QWORD, true);
							}

							//re-calculate all stats
							pPlayer->GetCharAtt()->CalculateAll();

							CNtlVector vNewLoc;

							if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1)
							{
								vNewLoc = m_pTableInfo->aFinalLoc[i].vLoc;
								pPlayer->SetCurDir(m_pTableInfo->aFinalLoc[i].vDir);
							}
							else if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM2)
							{
								vNewLoc = m_pTableInfo->aFinalLoc[i + 5].vLoc;
								pPlayer->SetCurDir(m_pTableInfo->aFinalLoc[i + 5].vDir);
							}

							pPlayer->SetIsReviving(false);

							//restore lp/rp/rp/rp balls
							pPlayer->UpdateCurLP(pPlayer->GetMaxLP(), true, false);
							pPlayer->UpdateCurEP(pPlayer->GetMaxEP(), true, false);
							pPlayer->UpdateRpBall(pPlayer->GetMaxRPBall(), false, false);
							pPlayer->UpdateCurRP(pPlayer->GetCharAtt()->GetMaxRP(), false, false);

							//remove target 
							pPlayer->ChangeTarget(INVALID_HOBJECT);

							//clear cc reduction
							pPlayer->ClearCrowdControlReduction();

							if (bSpawn)
								pPlayer->StartTeleport(vNewLoc, pPlayer->GetCurDir(), match->m_pWorld->GetID(), TELEPORT_TYPE_FINALMATCH);
						}
					}
				}
			}
		}
	}
}

void CBudokaiManager::FinalMatchTimeEnd(sTOURNAMENT_MATCH * match, BYTE byMatchIndex)
{
	//if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		float fTotalLifePercentOwner = 0;
		float fTotalLifePercentChallenger = 0;

		CPlayer* pPlayerOwner = NULL;
		CPlayer* pPlayerChallenger = NULL;

		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			if (it->second.bSpectator == false) // no need to include spectators
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
				if (pPlayer && pPlayer->IsInitialized() && pPlayer->GetMatchIndex() == byMatchIndex)
				{
					if (pPlayer->GetBudokaiPcState() == MATCH_MEMBER_STATE_NORMAL)
					{
						if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1)
						{
							fTotalLifePercentOwner += pPlayer->GetCurLpInPercent();
							pPlayerOwner = pPlayer;
						}
						else if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM2)
						{
							fTotalLifePercentChallenger += pPlayer->GetCurLpInPercent();
							pPlayerChallenger = pPlayer;
						}
					}
				}
			}
		}

		ERR_LOG(LOG_GENERAL, "fTotalLifePercentOwner %f, fTotalLifePercentChallenger %f", fTotalLifePercentOwner, fTotalLifePercentChallenger);

		if (fTotalLifePercentOwner > fTotalLifePercentChallenger)
			UpdateFinalMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, pPlayerOwner->GetBudokaiTeamType());
		else if (fTotalLifePercentChallenger > fTotalLifePercentOwner)
			UpdateFinalMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, pPlayerChallenger->GetBudokaiTeamType());
		else
		{
			//dice manager
			CDiceManager dice;

			fTotalLifePercentOwner = (float)dice.RollDice();
			fTotalLifePercentChallenger = (float)dice.RollDice();

			if (pPlayerOwner)
			{
				CNtlPacket packet2(sizeof(sGU_DICE_ROLLED_NFY));
				sGU_DICE_ROLLED_NFY* res2 = (sGU_DICE_ROLLED_NFY*)packet2.GetPacketData();
				res2->wOpCode = GU_DICE_ROLLED_NFY;
				res2->hSubject = pPlayerOwner->GetID();
				res2->wDiceResult = (WORD)fTotalLifePercentOwner;
				packet2.SetPacketLen(sizeof(sGU_DICE_ROLLED_NFY));
				pPlayerOwner->Broadcast(&packet2);
			}

			if (pPlayerChallenger)
			{
				CNtlPacket packet4(sizeof(sGU_DICE_ROLLED_NFY));
				sGU_DICE_ROLLED_NFY* res4 = (sGU_DICE_ROLLED_NFY*)packet4.GetPacketData();
				res4->wOpCode = GU_DICE_ROLLED_NFY;
				res4->hSubject = pPlayerChallenger->GetID();
				res4->wDiceResult = (WORD)fTotalLifePercentChallenger;
				packet4.SetPacketLen(sizeof(sGU_DICE_ROLLED_NFY));
				pPlayerChallenger->Broadcast(&packet4);
			}

			if (pPlayerOwner && pPlayerChallenger == NULL)
			{
				UpdateFinalMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, pPlayerOwner->GetBudokaiTeamType());

				return;
			}
			else if (pPlayerChallenger && pPlayerOwner == NULL)
			{
				UpdateFinalMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, pPlayerChallenger->GetBudokaiTeamType());

				return;
			}

			if (pPlayerOwner && fTotalLifePercentOwner >= fTotalLifePercentChallenger)
				UpdateFinalMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, pPlayerOwner->GetBudokaiTeamType());
			else if (pPlayerChallenger && fTotalLifePercentChallenger > fTotalLifePercentOwner)
				UpdateFinalMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, pPlayerChallenger->GetBudokaiTeamType());
		}
	}
}

void CBudokaiManager::FinalMatchEnd(sTOURNAMENT_MATCH * match, BYTE byMatchIndex)
{
	CGameServer* app = (CGameServer*)g_pApp;

	ERR_LOG(LOG_GENERAL, "BUDOKAI: Tournament Final Match Index %u END", byMatchIndex);

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			sPLAYER_INFO& info = it->second;

			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer)
			{
				if (pPlayer->GetMatchIndex() == byMatchIndex && pPlayer->GetCharID() == info.charId)
				{
					CNtlPacket packet(sizeof(sGQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ));
					sGQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ * res = (sGQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ *)packet.GetPacketData();
					res->wOpCode = GQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ;
					res->wJoinId = pPlayer->GetJoinID();
					res->byMatchIndex = byMatchIndex;
					res->byWinnerJoinResult = INVALID_BUDOKAI_JOIN_RESULT;

					if (pPlayer->GetJoinID() == match->data.wWinnerTeam) //check if winner
					{
						std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(pPlayer->GetJoinID());
						if (it != m_mapJoinInfo.end())
						{
							if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_4) //if semi match
							{
								it->second.byJoinResult = BUDOKAI_JOIN_RESULT_ENTER_2;
								res->byWinnerJoinResult = BUDOKAI_JOIN_RESULT_ENTER_2;
							}
							else
							{
								it->second.byJoinResult = BUDOKAI_JOIN_RESULT_WINNER;
								res->byWinnerJoinResult = BUDOKAI_JOIN_RESULT_WINNER;
							}

							ERR_LOG(LOG_GENERAL, "BUDOKAI: Tournament Final Match Index %u, Winner CharID = %u, JoinResult = BUDOKAI_JOIN_RESULT_WINNER", byMatchIndex, pPlayer->GetCharID());
						}
						else ERR_LOG(LOG_GENERAL, "Could not find prelim winner in m_mapJoinInfo");
					}
					else
					{
						std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(pPlayer->GetJoinID());
						if (it != m_mapJoinInfo.end())
						{
							if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_4) //if semi match
							{
								it->second.byJoinResult = BUDOKAI_JOIN_RESULT_ENTER_2;
								res->byWinnerJoinResult = BUDOKAI_JOIN_RESULT_ENTER_2;
							}
							else
							{
								it->second.byJoinState = BUDOKAI_JOIN_STATE_DROPOUT;
								it->second.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;

								res->byLoserJoinState = BUDOKAI_JOIN_STATE_DROPOUT;
								res->byLoserResultCondition = INVALID_BUDOKAI_JOIN_RESULT;
							}

							ERR_LOG(LOG_GENERAL, "BUDOKAI: Tournament Final Match Index %u, Loser CharID = %u, byJoinState = BUDOKAI_JOIN_STATE_DROPOUT", byMatchIndex, pPlayer->GetCharID());
						}
						else ERR_LOG(LOG_GENERAL, "Could not find player in m_mapJoinInfo");
					}
					
					packet.SetPacketLen(sizeof(sGQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ));
					app->SendTo(app->GetQueryServerSession(), &packet);

					pPlayer->TeleportAnotherServer(match->m_pWorld->GetTbldat()->outWorldLoc, pPlayer->GetCurDir(), match->m_pWorld->GetTbldat()->outWorldTblidx, match->m_pWorld->GetTbldat()->outWorldTblidx, TELEPORT_TYPE_BUDOKAI, pPlayer->GetPrevChannelID(), 0, false);
				}
			}
		}
	}
	else
	{
		for (std::map<JOINID, sPARTY_INFO>::iterator it = match->m_mapTeam.begin(); it != match->m_mapTeam.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;

			CNtlPacket packet(sizeof(sGQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ));
			sGQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ * res = (sGQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ *)packet.GetPacketData();
			res->wOpCode = GQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ;
			res->wJoinId = partyInfo->joinId;
			res->byMatchIndex = byMatchIndex;
			res->byWinnerJoinResult = INVALID_BUDOKAI_JOIN_RESULT;

			if (partyInfo->joinId == match->data.wWinnerTeam)
			{
				std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(partyInfo->joinId);
				if (it != m_mapJoinInfo.end())
				{
					if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_4) //if semi match
					{
						it->second.byJoinResult = BUDOKAI_JOIN_RESULT_ENTER_2;
						res->byWinnerJoinResult = BUDOKAI_JOIN_RESULT_ENTER_2;
					}
					else
					{
						it->second.byJoinResult = BUDOKAI_JOIN_RESULT_WINNER;
						res->byWinnerJoinResult = BUDOKAI_JOIN_RESULT_WINNER;
					}

					ERR_LOG(LOG_GENERAL, "BUDOKAI: Tournament Final Match Index %u, Winner = %u, JoinResult = %u", byMatchIndex, partyInfo->joinId, it->second.byJoinResult);
				}
				else ERR_LOG(LOG_GENERAL, "Could not find winner in m_mapJoinInfo");
			}
			else
			{
				std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(partyInfo->joinId);
				if (it != m_mapJoinInfo.end())
				{
					if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_4) //if semi match
					{
						it->second.byJoinResult = BUDOKAI_JOIN_RESULT_ENTER_2;
						res->byWinnerJoinResult = BUDOKAI_JOIN_RESULT_ENTER_2;
					}
					else
					{
						it->second.byJoinState = BUDOKAI_JOIN_STATE_DROPOUT;
						it->second.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;

						res->byLoserJoinState = BUDOKAI_JOIN_STATE_DROPOUT;
						res->byLoserResultCondition = INVALID_BUDOKAI_JOIN_RESULT;
					}
					ERR_LOG(LOG_GENERAL, "BUDOKAI: Tournament Final Match Index %u, Loser = %u, byJoinResult = %u", byMatchIndex, partyInfo->joinId, it->second.byJoinResult);
				}
				else ERR_LOG(LOG_GENERAL, "Could not find player in m_mapJoinInfo");
			}

			packet.SetPacketLen(sizeof(sGQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet);


			for (BYTE byMemberCount = 0; byMemberCount < partyInfo->byMemberCount; byMemberCount++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[byMemberCount]);
				if (pPlayer && partyInfo->charId[byMemberCount] == pPlayer->GetCharID() && pPlayer->GetMatchIndex() == byMatchIndex)
				{
					pPlayer->TeleportAnotherServer(match->m_pWorld->GetTbldat()->outWorldLoc, pPlayer->GetCurDir(), match->m_pWorld->GetTbldat()->outWorldTblidx, match->m_pWorld->GetTbldat()->outWorldTblidx, TELEPORT_TYPE_BUDOKAI, pPlayer->GetPrevChannelID(), 0, false);
				}
			}
		}
	}
}

bool CBudokaiManager::FinalMatchCheckPlayerCount(sTOURNAMENT_MATCH * match, BYTE byMatchIndex)
{
	if (match->m_byTeam1PlayerCount == 0)
	{
		UpdateFinalMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, MATCH_TEAM_TYPE_TEAM2, 3); //give instant 3 wins
		return false;
	}

	if (match->m_byTeam2PlayerCount == 0)
	{
		UpdateFinalMatchScore(match, byMatchIndex, MATCH_RESULT_WIN, MATCH_TEAM_TYPE_TEAM1, 3); //give instant 3 wins
		return false;
	}

	return true;
}

void CBudokaiManager::FinalMatchFinalDirection(sTOURNAMENT_MATCH * match)
{
	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		int nLoc = 10;

		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			sPLAYER_INFO& info = it->second;

			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer)
			{
				if (pPlayer->GetCharID() == info.charId)
				{
					pPlayer->ChangeAttackProgress(false);
					pPlayer->SetAttackTarget(INVALID_HOBJECT);

					//remove buffs
					pPlayer->GetBuffManager()->RemoveAllBuff();

					//if transformed then cancel transformation
					if (pPlayer->GetTransformationTbldat())
						pPlayer->CancelTransformation();

					if (pPlayer->GetStateManager()->GetConditionState() > 0)
					{
						pPlayer->GetStateManager()->RemoveConditionFlags(INVALID_QWORD, true);
					}

					//re-calculate all stats
					pPlayer->GetCharAtt()->CalculateAll();

					CNtlVector vNewLoc;

					if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1 || pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM2)
					{
						if (pPlayer->GetJoinID() == match->data.wWinnerTeam)
						{
							vNewLoc = m_pTableInfo->aAwardingLoc[0].vLoc;
							pPlayer->SetCurDir(m_pTableInfo->aAwardingLoc[0].vDir);
						}
						else
						{
							vNewLoc = m_pTableInfo->aAwardingLoc[5].vLoc;
							pPlayer->SetCurDir(m_pTableInfo->aAwardingLoc[5].vDir);
						}
					}
					else
					{
						vNewLoc = m_pTableInfo->aAwardingLoc[nLoc].vLoc;
						pPlayer->SetCurDir(m_pTableInfo->aAwardingLoc[nLoc].vDir);

						nLoc += 4;
					}

					pPlayer->SetIsReviving(false);

					//restore lp/rp/rp/rp balls
					pPlayer->UpdateCurLP(pPlayer->GetMaxLP(), true, false);
					pPlayer->UpdateCurEP(pPlayer->GetMaxEP(), true, false);
					pPlayer->UpdateRpBall(pPlayer->GetMaxRPBall(), false, false);
					pPlayer->UpdateCurRP(pPlayer->GetCharAtt()->GetMaxRP(), false, false);

					//remove target 
					pPlayer->ChangeTarget(INVALID_HOBJECT);

					//clear cc reduction
					pPlayer->ClearCrowdControlReduction();

					pPlayer->StartTeleport(vNewLoc, pPlayer->GetCurDir(), match->m_pWorld->GetID(), TELEPORT_TYPE_FINALMATCH);
				}
			}
		}
	}
	else
	{
		int nLocT1 = 0;
		int nLocT2 = 5;
		int nLocT3 = 10;
		int nLocT4 = 15;

		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			sPLAYER_INFO& info = it->second;

			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer)
			{
				if (pPlayer->GetCharID() == info.charId)
				{
					pPlayer->ChangeAttackProgress(false);
					pPlayer->SetAttackTarget(INVALID_HOBJECT);

					//remove buffs
					pPlayer->GetBuffManager()->RemoveAllBuff();

					//if transformed then cancel transformation
					if (pPlayer->GetTransformationTbldat())
						pPlayer->CancelTransformation();

					if (pPlayer->GetStateManager()->GetConditionState() > 0)
					{
						pPlayer->GetStateManager()->RemoveConditionFlags(INVALID_QWORD, true);
					}

					//re-calculate all stats
					pPlayer->GetCharAtt()->CalculateAll();

					CNtlVector vNewLoc(pPlayer->GetCurLoc());

					if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1 || pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM2)
					{
						if (pPlayer->GetJoinID() == match->data.wWinnerTeam)
						{
							vNewLoc = m_pTableInfo->aAwardingLoc[nLocT1].vLoc;
							pPlayer->SetCurDir(m_pTableInfo->aAwardingLoc[nLocT1++].vDir);
						}
						else
						{
							vNewLoc = m_pTableInfo->aAwardingLoc[nLocT2].vLoc;
							pPlayer->SetCurDir(m_pTableInfo->aAwardingLoc[nLocT2++].vDir);
						}
					}
					else if (pPlayer->GetBudokaiTeamType() == 2)
					{
						vNewLoc = m_pTableInfo->aAwardingLoc[nLocT3].vLoc;
						pPlayer->SetCurDir(m_pTableInfo->aAwardingLoc[nLocT3++].vDir);
					}
					else if (pPlayer->GetBudokaiTeamType() == 3 && nLocT4 < BUDOKAI_MAX_AWARDING_LOCATION_COUNT)
					{
						vNewLoc = m_pTableInfo->aAwardingLoc[nLocT4].vLoc;
						pPlayer->SetCurDir(m_pTableInfo->aAwardingLoc[nLocT4++].vDir);
					}
					else
					{
						ERR_LOG(LOG_GENERAL, "BUDOKAI AWARDING: Player has wrong team type of something wrong with location count. Teamtype: %u, nLocT1: %u, nLocT2: %u, nLocT3: %u, nLocT4: %u", pPlayer->GetBudokaiTeamType(), nLocT1, nLocT2, nLocT3, nLocT4);
					}

					pPlayer->SetIsReviving(false);

					//restore lp/rp/rp/rp balls
					pPlayer->UpdateCurLP(pPlayer->GetMaxLP(), true, false);
					pPlayer->UpdateCurEP(pPlayer->GetMaxEP(), true, false);
					pPlayer->UpdateRpBall(pPlayer->GetMaxRPBall(), false, false);
					pPlayer->UpdateCurRP(pPlayer->GetCharAtt()->GetMaxRP(), false, false);

					//remove target 
					pPlayer->ChangeTarget(INVALID_HOBJECT);

					//clear cc reduction
					pPlayer->ClearCrowdControlReduction();

					pPlayer->StartTeleport(vNewLoc, pPlayer->GetCurDir(), match->m_pWorld->GetID(), TELEPORT_TYPE_FINALMATCH);
				}
			}
		}
	}
}

void CBudokaiManager::FinalMatchAwarding(sTOURNAMENT_MATCH * match)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sVARIABLE_DATA sData;
	sData.Init(1024);

	WORD awTeamName[MAX_FINALMATCH_RESULT];
	memset(awTeamName, INVALID_WORD, sizeof(awTeamName));

	int i = 0;

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		for (std::map<HOBJECT, sPLAYER_INFO>::iterator it = match->m_mapIndividual.begin(); it != match->m_mapIndividual.end(); it++)
		{
			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer && pPlayer->GetCharID() == it->second.charId)
			{
				DWORD dwMudosaPoints = 0;
				TBLIDX itemIdx = INVALID_TBLIDX;
				BYTE byItemCount = 0;
				TBLIDX titleId = INVALID_TBLIDX;

				WCHAR wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
				NTL_SAFE_WCSCPY(wszTeamName, pPlayer->GetCharName());

				if (pPlayer->GetJoinID() == match->data.wWinnerTeam) //give final winner reward
				{
					awTeamName[0] = sData.Write(sizeof(wszTeamName), wszTeamName); //first place

					// --- ANNOUNCE THE FIRST PLACE WINNER
					CNtlPacket pChat(sizeof(sGT_BROADCASTING_SYSTEM_NFY));
					sGT_BROADCASTING_SYSTEM_NFY * rChat = (sGT_BROADCASTING_SYSTEM_NFY *)pChat.GetPacketData();
					rChat->wOpCode = GT_BROADCASTING_SYSTEM_NFY;
					rChat->byMsgType = DBO_BROADCASTING_MSG_TYPE_BUDOKAI;
					rChat->sData.sBudokaiRecord.wSeason = m_stateInfo.wSeasonCount + 1;
					rChat->sData.sBudokaiRecord.byRank = 0;
					rChat->sData.sBudokaiRecord.byBudokaiType = m_type;
					rChat->sData.sBudokaiRecord.byMatchType = m_matchType;
					rChat->sData.sBudokaiRecord.byMemberCount = 1;
					memset(rChat->sData.sBudokaiRecord.awszMember, NULL, sizeof(rChat->sData.sBudokaiRecord.awszMember));
					NTL_SAFE_WCSCPY(rChat->sData.sBudokaiRecord.awszMember[0], pPlayer->GetCharName());
					pChat.SetPacketLen(sizeof(sGT_BROADCASTING_SYSTEM_NFY));
					app->SendTo(app->GetChatServerSession(), &pChat);

					if (m_type == BUDOKAI_TYPE_JUNIOR)
					{
						titleId = 501;//WMAT Prodigy

						dwMudosaPoints = m_pTableInfo->sJuniorIndividualReward.sAwarding.sWinner.dwMudosaPoint;
						itemIdx = m_pTableInfo->sJuniorIndividualReward.sAwarding.sWinner.itemTblidx;
						byItemCount = m_pTableInfo->sJuniorIndividualReward.sAwarding.sWinner.byStackCount;
					}
					else
					{
						titleId = 507;//WMAT Solo Champion

						dwMudosaPoints = m_pTableInfo->sIndividualReward.sAwarding.sWinner.dwMudosaPoint;
						itemIdx = m_pTableInfo->sIndividualReward.sAwarding.sWinner.itemTblidx;
						byItemCount = m_pTableInfo->sIndividualReward.sAwarding.sWinner.byStackCount;
					}
				}
				else if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1 || pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM2) //give 2nd reward
				{
					awTeamName[1] = sData.Write(sizeof(wszTeamName), wszTeamName); //second place

					if (m_type == BUDOKAI_TYPE_JUNIOR)
					{
						titleId = 502;// Cosmic Supernova

						dwMudosaPoints = m_pTableInfo->sJuniorIndividualReward.sAwarding.sSemiWinner.dwMudosaPoint;
						itemIdx = m_pTableInfo->sJuniorIndividualReward.sAwarding.sSemiWinner.itemTblidx;
						byItemCount = m_pTableInfo->sJuniorIndividualReward.sAwarding.sSemiWinner.byStackCount;
					}
					else
					{
						titleId = 508;// Uncrowned King

						dwMudosaPoints = m_pTableInfo->sIndividualReward.sAwarding.sSemiWinner.dwMudosaPoint;
						itemIdx = m_pTableInfo->sIndividualReward.sAwarding.sSemiWinner.itemTblidx;
						byItemCount = m_pTableInfo->sIndividualReward.sAwarding.sSemiWinner.byStackCount;
					}
				}
				else //give 3th and 4th reward
				{
					awTeamName[2 + i] = sData.Write(sizeof(wszTeamName), wszTeamName); //3rd/4th place
					++i;

					if (m_type == BUDOKAI_TYPE_JUNIOR)
					{
						titleId = 503;// Star of Tomorrow

						dwMudosaPoints = m_pTableInfo->sJuniorIndividualReward.sAwarding.sOther.dwMudosaPoint;
						itemIdx = m_pTableInfo->sJuniorIndividualReward.sAwarding.sOther.itemTblidx;
						byItemCount = m_pTableInfo->sJuniorIndividualReward.sAwarding.sOther.byStackCount;
					}
					else
					{
						titleId = 509;// Flaming Fist

						dwMudosaPoints = m_pTableInfo->sIndividualReward.sAwarding.sOther.dwMudosaPoint;
						itemIdx = m_pTableInfo->sIndividualReward.sAwarding.sOther.itemTblidx;
						byItemCount = m_pTableInfo->sIndividualReward.sAwarding.sOther.byStackCount;
					}
				}

				ERR_LOG(LOG_GENERAL, "Budokai Final reward: CharID = %u, Item Idx = %u, Item Count = %u, Mudosa Points = %u, TitleID = %u", pPlayer->GetCharID(), itemIdx, byItemCount, dwMudosaPoints, titleId);

				pPlayer->UpdateMudosaPoints(pPlayer->GetMudosaPoints() + dwMudosaPoints, true);

				if (itemIdx != INVALID_TBLIDX)
					g_pItemManager->CreateQuestRewardItem(pPlayer, itemIdx, byItemCount);

				//add title
				if (titleId != INVALID_TBLIDX && pPlayer->CheckCharTitle(titleId - 1) == false)
				{
					pPlayer->AddCharTitle(titleId - 1);
				}
			}
		}
	}
	else
	{
		for (std::map<JOINID, sPARTY_INFO>::iterator it = match->m_mapTeam.begin(); it != match->m_mapTeam.end(); it++)
		{
			sPARTY_INFO* partyInfo = &it->second;

			DWORD dwMudosaPoints = 0;
			TBLIDX itemIdx = INVALID_TBLIDX;
			BYTE byItemCount = 0;
			TBLIDX titleId = INVALID_TBLIDX;

			WCHAR wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
			NTL_SAFE_WCSCPY(wszTeamName, partyInfo->m_wszTeamName);

			if (partyInfo->joinId == match->data.wWinnerTeam)//give final winner reward
			{
				awTeamName[0] = sData.Write(sizeof(wszTeamName), wszTeamName); //first place

				// --- ANNOUNCE THE FIRST PLACE WINNER
				CNtlPacket pChat(sizeof(sGT_BROADCASTING_SYSTEM_NFY));
				sGT_BROADCASTING_SYSTEM_NFY * rChat = (sGT_BROADCASTING_SYSTEM_NFY *)pChat.GetPacketData();
				rChat->wOpCode = GT_BROADCASTING_SYSTEM_NFY;
				rChat->byMsgType = DBO_BROADCASTING_MSG_TYPE_BUDOKAI;
				rChat->sData.sBudokaiRecord.wSeason = m_stateInfo.wSeasonCount + 1;
				rChat->sData.sBudokaiRecord.byRank = 0;
				rChat->sData.sBudokaiRecord.byBudokaiType = m_type;
				rChat->sData.sBudokaiRecord.byMatchType = m_matchType;
				rChat->sData.sBudokaiRecord.byMemberCount = 0;
				memset(rChat->sData.sBudokaiRecord.awszMember, NULL, sizeof(rChat->sData.sBudokaiRecord.awszMember));

				for (BYTE byMemberCount = 0; byMemberCount < partyInfo->byMemberCount; byMemberCount++)
				{
					CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[byMemberCount]);
					if (pPlayer && partyInfo->charId[byMemberCount] == pPlayer->GetCharID())
					{
						NTL_SAFE_WCSCPY(rChat->sData.sBudokaiRecord.awszMember[byMemberCount], pPlayer->GetCharName());
						rChat->sData.sBudokaiRecord.byMemberCount += 1;
					}
				}

				pChat.SetPacketLen(sizeof(sGT_BROADCASTING_SYSTEM_NFY));
				app->SendTo(app->GetChatServerSession(), &pChat);

				if (m_type == BUDOKAI_TYPE_JUNIOR)
				{
					titleId = 504;//WMAT Group Champion

					dwMudosaPoints = m_pTableInfo->sJuniorTeamReward.sAwarding.sWinner.dwMudosaPoint;
					itemIdx = m_pTableInfo->sJuniorTeamReward.sAwarding.sWinner.itemTblidx;
					byItemCount = m_pTableInfo->sJuniorTeamReward.sAwarding.sWinner.byStackCount;
				}
				else
				{
					titleId = 510;//WMAT Team Champion

					dwMudosaPoints = m_pTableInfo->sTeamReward.sAwarding.sWinner.dwMudosaPoint;
					itemIdx = m_pTableInfo->sTeamReward.sAwarding.sWinner.itemTblidx;
					byItemCount = m_pTableInfo->sTeamReward.sAwarding.sWinner.byStackCount;
				}
			}
			else if (partyInfo->teamType == MATCH_TEAM_TYPE_TEAM1 || partyInfo->teamType == MATCH_TEAM_TYPE_TEAM2) //give 2nd reward
			{
				awTeamName[1] = sData.Write(sizeof(wszTeamName), wszTeamName); //second place

				if (m_type == BUDOKAI_TYPE_JUNIOR)
				{
					titleId = 505;// Fantastical Partner

					dwMudosaPoints = m_pTableInfo->sJuniorTeamReward.sAwarding.sSemiWinner.dwMudosaPoint;
					itemIdx = m_pTableInfo->sJuniorTeamReward.sAwarding.sSemiWinner.itemTblidx;
					byItemCount = m_pTableInfo->sJuniorTeamReward.sAwarding.sSemiWinner.byStackCount;
				}
				else
				{
					titleId = 511;// Perfect Partner

					dwMudosaPoints = m_pTableInfo->sTeamReward.sAwarding.sSemiWinner.dwMudosaPoint;
					itemIdx = m_pTableInfo->sTeamReward.sAwarding.sSemiWinner.itemTblidx;
					byItemCount = m_pTableInfo->sTeamReward.sAwarding.sSemiWinner.byStackCount;
				}
			}
			else //give 3th and 4th reward
			{
				awTeamName[2 + i] = sData.Write(sizeof(wszTeamName), wszTeamName); //3rd/4th place
				++i;

				if (m_type == BUDOKAI_TYPE_JUNIOR)
				{
					titleId = 506;// Not Harmonic

					dwMudosaPoints = m_pTableInfo->sJuniorTeamReward.sAwarding.sOther.dwMudosaPoint;
					itemIdx = m_pTableInfo->sJuniorTeamReward.sAwarding.sOther.itemTblidx;
					byItemCount = m_pTableInfo->sJuniorTeamReward.sAwarding.sOther.byStackCount;
				}
				else
				{
					titleId = 512;// Screech

					dwMudosaPoints = m_pTableInfo->sTeamReward.sAwarding.sOther.dwMudosaPoint;
					itemIdx = m_pTableInfo->sTeamReward.sAwarding.sOther.itemTblidx;
					byItemCount = m_pTableInfo->sTeamReward.sAwarding.sOther.byStackCount;
				}
			}

			for (BYTE byMemberCount = 0; byMemberCount < partyInfo->byMemberCount; byMemberCount++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(partyInfo->charHandle[byMemberCount]);
				if (pPlayer && partyInfo->charId[byMemberCount] == pPlayer->GetCharID())
				{
					pPlayer->UpdateMudosaPoints(pPlayer->GetMudosaPoints() + dwMudosaPoints, true);

					if (itemIdx != INVALID_TBLIDX)
						g_pItemManager->CreateQuestRewardItem(pPlayer, itemIdx, byItemCount);

					//add title
					if (titleId != INVALID_TBLIDX && pPlayer->CheckCharTitle(titleId - 1) == false)
					{
						pPlayer->AddCharTitle(titleId - 1);
					}
				}
			}
		}
	}

	WORD wPacketSize = sData.GetPacketSize(sizeof(sGU_MATCH_AWARDING_NFY));

	CNtlPacket packet(wPacketSize);
	sGU_MATCH_AWARDING_NFY * res = (sGU_MATCH_AWARDING_NFY *)packet.GetPacketData();
	res->wOpCode = GU_MATCH_AWARDING_NFY;
	memcpy(res->awTeamName, awTeamName, sizeof(awTeamName));
	sData.CopyTo(&res->sData, sData.GetDataSize());
	match->m_pWorld->Broadcast(&packet);

	ERR_LOG(LOG_GENERAL, "wPacketSize %u, sData.GetDataSize() %u \n", wPacketSize, sData.GetDataSize());
}

void CBudokaiManager::AddIndividualToNextRound(sTOURNAMENT_MATCH * match, BYTE byMatchIndex, JOINID joinId, JOINID spectatorJoinId/* = INVALID_JOINID*/)
{
	CGameServer* app = (CGameServer*)g_pApp;

	BYTE nextMatchDepth = 0;
	BYTE byPlayersNextMatchIndex = byMatchIndex / 2;

	if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_32)
		nextMatchDepth = BUDOKAI_MATCH_DEPTH_16;
	else if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_16)
		nextMatchDepth = BUDOKAI_MATCH_DEPTH_8;
	else if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_8)
		nextMatchDepth = BUDOKAI_MATCH_DEPTH_4;
	else if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_4)
		nextMatchDepth = BUDOKAI_MATCH_DEPTH_2;

	m_aTournamentMatch[nextMatchDepth].byMatchCountEnd = 0;


	// -- First we check if we can add the winner to next match
	std::map<BYTE, sTOURNAMENT_MATCH*>::iterator itMatch = m_aTournamentMatch[nextMatchDepth].m_mapTournament.find(byPlayersNextMatchIndex);
	if (itMatch != m_aTournamentMatch[nextMatchDepth].m_mapTournament.end())
	{
		sTOURNAMENT_MATCH* pMatch = itMatch->second;

		if (spectatorJoinId != INVALID_JOINID)
		{
			pMatch->m_setSpectator.insert(spectatorJoinId);

			BYTE byTeamType = (BYTE)pMatch->m_setSpectator.size() + 1;

			if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
			{
				std::map<JOINID, sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA*>::iterator it6 = m_mapIndividualTournament.find(spectatorJoinId);
				if (it6 != m_mapIndividualTournament.end())
				{
					SetIndividualTeamType(it6->second->charId, byTeamType);
					SetMatchIndex(it6->first, pMatch->data.byMatchNumber);
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "Could not find user in m_mapIndividualTournament");
				}
			}
			else
			{
				std::map<JOINID, sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA*>::iterator it6 = m_mapTeamTournament.find(spectatorJoinId);
				if (it6 != m_mapTeamTournament.end())
				{
					SetIndividualTeamType(it6->first, byTeamType);
					SetMatchIndex(it6->first, pMatch->data.byMatchNumber);
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "Could not find user in m_mapTeamTournament");
				}
			}
		}

		//INFO: DONT DO "else" HERE OTHERWISE FINALISTS WONT GET TELEPORTED IF THERE IS A SPECTATOR
		
		if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
		{
			std::map<JOINID, sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA*>::iterator it5 = m_mapIndividualTournament.find(joinId);
			if (it5 != m_mapIndividualTournament.end())
			{
				TEAMTYPE teamType = DecideTeamType(nextMatchDepth, it5->second->byMinorMatch_MatchIndex);

				if (teamType == MATCH_TEAM_TYPE_TEAM1)
				{
					pMatch->data.wJoinId1 = joinId;
					pMatch->m_byTotalCount += 1;

					SetIndividualTeamType(it5->second->charId, MATCH_TEAM_TYPE_TEAM1);
					SetMatchIndex(it5->first, pMatch->data.byMatchNumber);
				}
				else
				{
					pMatch->data.wJoinId2 = joinId;
					pMatch->m_byTotalCount += 1;

					SetMatchIndex(it5->first, pMatch->data.byMatchNumber);
					SetIndividualTeamType(it5->second->charId, MATCH_TEAM_TYPE_TEAM2);
				}
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "Could not find user in m_mapIndividualTournament");
			}
		}
		else
		{
			std::map<JOINID, sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA*>::iterator it5 = m_mapTeamTournament.find(joinId);
			if (it5 != m_mapTeamTournament.end())
			{
				TEAMTYPE teamType = DecideTeamType(nextMatchDepth, it5->second->byMinorMatch_MatchIndex);

				if (teamType == MATCH_TEAM_TYPE_TEAM1)
				{
					pMatch->data.wJoinId1 = joinId;
					pMatch->m_byTotalCount += 5;

					SetIndividualTeamType(it5->first, MATCH_TEAM_TYPE_TEAM1);
					SetMatchIndex(it5->first, pMatch->data.byMatchNumber);
				}
				else
				{
					pMatch->data.wJoinId2 = joinId;
					pMatch->m_byTotalCount += 5;

					SetMatchIndex(it5->first, pMatch->data.byMatchNumber);
					SetIndividualTeamType(it5->first, MATCH_TEAM_TYPE_TEAM2);
				}
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "Could not find user in m_mapIndividualTournament");
			}
		}	
	}
	else printf("AddIndividualToNextRound: Could not find match...");
}


void CBudokaiManager::MinorMatchUpdatePlayerState(BYTE byState, CPlayer * pPlayer)
{
	std::map<BYTE, sPRELIM_HEAD*>::iterator iter = m_mapPrelims.find(pPlayer->GetMatchIndex());
	if (iter != m_mapPrelims.end())
	{
		sPRELIM_HEAD* prelim = iter->second;

		pPlayer->SetBudokaiPcState(byState);

		CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY));
		sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_MATCH_MINORMATCH_PLAYER_STATE_NFY;
		res2->hPc = pPlayer->GetID();
		res2->byPcState = byState;
		packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY));
		prelim->m_pWorld->Broadcast(&packet2);
	}
}

void CBudokaiManager::MajorMatchUpdatePlayerState(BYTE byState, CPlayer * pPlayer)
{
	std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it4 = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.find(pPlayer->GetMatchIndex());
	if (it4 != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end())
	{
		pPlayer->SetBudokaiPcState(byState);

		//if we revive, then decrease unavailable counter
		if (pPlayer->GetBudokaiPcState() == MATCH_MEMBER_STATE_RESCUE)
		{
			if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1)
			{
				it4->second->m_byTeam1UnavailablePlayerCount = UnsignedSafeDecrease<BYTE>(it4->second->m_byTeam1UnavailablePlayerCount, 1);
			}
			else if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM2)
			{
				it4->second->m_byTeam2UnavailablePlayerCount = UnsignedSafeDecrease<BYTE>(it4->second->m_byTeam2UnavailablePlayerCount, 1);
			}
		}

		CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
		sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_MATCH_MAJORMATCH_PLAYER_STATE_NFY;
		res2->hPc = pPlayer->GetID();
		res2->byPcState = byState;
		packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
		it4->second->m_pWorld->Broadcast(&packet2);
	}
}

void CBudokaiManager::FinalMatchUpdatePlayerState(BYTE byState, CPlayer * pPlayer)
{
	std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it4 = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.find(pPlayer->GetMatchIndex());
	if (it4 != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end())
	{
		pPlayer->SetBudokaiPcState(byState);

		//if we revive, then decrease unavailable counter
		if (pPlayer->GetBudokaiPcState() == MATCH_MEMBER_STATE_RESCUE)
		{
			if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM1)
			{
				it4->second->m_byTeam1UnavailablePlayerCount = UnsignedSafeDecrease<BYTE>(it4->second->m_byTeam1UnavailablePlayerCount, 1);
			}
			else if (pPlayer->GetBudokaiTeamType() == MATCH_TEAM_TYPE_TEAM2)
			{
				it4->second->m_byTeam2UnavailablePlayerCount = UnsignedSafeDecrease<BYTE>(it4->second->m_byTeam2UnavailablePlayerCount, 1);
			}
		}

		CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY));
		sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_MATCH_FINALMATCH_PLAYER_STATE_NFY;
		res2->hPc = pPlayer->GetID();
		res2->byPcState = byState;
		packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY));
		it4->second->m_pWorld->Broadcast(&packet2);
	}
}

void CBudokaiManager::LoadBudokaiStateInfo(CPlayer * pPlayer)
{
	if (m_stateInfo.byState == BUDOKAI_STATE_JUNIOR_CLOSE || m_stateInfo.byState == BUDOKAI_STATE_CLOSE)
		return;

	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGU_BUDOKAI_STATE_INFO_NFY));
	sGU_BUDOKAI_STATE_INFO_NFY * res = (sGU_BUDOKAI_STATE_INFO_NFY *)packet.GetPacketData();
	res->wOpCode = GU_BUDOKAI_STATE_INFO_NFY;
	res->tmDefaultOpenTime = m_tmDefaultOpenTime;
	res->bRankbattleAllow = m_bRankbattleAllow;
	memcpy(&res->sStateInfo, &m_stateInfo, sizeof(sBUDOKAI_UPDATE_STATE_INFO));
	memcpy(res->aMatchStateInfo, m_matchStateInfo, sizeof(m_matchStateInfo));
	//set these invalid as default
	res->sJoinInfo.byJoinState = INVALID_BUDOKAI_JOIN_STATE;
	res->sJoinInfo.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;
	res->sJoinInfo.byMatchType = m_matchType;

	if (pPlayer->GetJoinID() != INVALID_JOINID)
	{
		if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
		{
			std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(pPlayer->GetJoinID());
			if (it != m_mapJoinInfo.end())
			{
				res->sJoinInfo.byJoinResult = it->second.byJoinResult;
				res->sJoinInfo.byJoinState = it->second.byJoinState;

				memcpy(&res->sJoinInfo.sIndividualInfo, &it->second.sIndividualInfo, sizeof(sBUDOKAI_REGISTER_INDIVIDUAL_INFO));
			}
		}
		else if (m_matchType == BUDOKAI_MATCH_TYPE_TEAM)
		{
			std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(pPlayer->GetJoinID());
			if (it != m_mapJoinInfo.end())
			{
				res->sJoinInfo.byJoinResult = it->second.byJoinResult;
				res->sJoinInfo.byJoinState = it->second.byJoinState;

				memcpy(&res->sJoinInfo.sTeamInfo, &it->second.sTeamInfo, sizeof(sBUDOKAI_REGISTER_TEAM_INFO));
			}
		}
		else
		{
			// debug
		}
	}

	packet.SetPacketLen(sizeof(sGU_BUDOKAI_STATE_INFO_NFY));
	pPlayer->SendPacket(&packet);


	if (app->m_config.byChannel != DOJO_CHANNEL_INDEX)
		return;

	if (pPlayer->GetJoinID() == INVALID_JOINID)
		return;

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		if (m_matchStateInfo[m_matchType].byState == BUDOKAI_MATCHSTATE_MINOR_MATCH)
		{
			auto it3 = m_mapIndividual.find(pPlayer->GetJoinID());
			if (it3 != m_mapIndividual.end())
			{
				std::map<BYTE, sPRELIM_HEAD*>::iterator it4 = m_mapPrelims.find(it3->second.byMinorMatch_MatchIndex);
				if (it4 != m_mapPrelims.end())
				{
					pPlayer->SetMatchIndex(it4->first);

					it4->second->m_mapIndividualPrelim.insert({ pPlayer->GetID(), sPLAYER_INFO(pPlayer->GetCharID(), false) });
					it4->second->m_byCount += 1;

					CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_MINORMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = it4->second->m_byMatchState;
					res2->bIsEnter = true;
					res2->byStage = 0;
					res2->dwRemainTime = it4->second->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
					pPlayer->SendPacket(&packet2);
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: COULD NOT FIND byMinorMatch_MatchIndex %u", it3->second.byMinorMatch_MatchIndex);
				}
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: COULD NOT FIND CHARACTER %u", pPlayer->GetCharID());
			}
		}
		else if (m_matchStateInfo[m_matchType].byState > BUDOKAI_MATCHSTATE_MINOR_MATCH && m_matchStateInfo[m_matchType].byState != BUDOKAI_MATCHSTATE_MATCH_END)
		{
			std::map<JOINID, sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA*>::iterator it3 = m_mapIndividualTournament.find(pPlayer->GetJoinID());
			if (it3 != m_mapIndividualTournament.end())
			{
				BYTE byMatchIndex = GetMatchIndex(pPlayer->GetJoinID());

				std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it4 = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.find(byMatchIndex);
				if (it4 != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end())
				{
					pPlayer->SetMatchIndex(byMatchIndex);

					bool bSpectator = false;

					if (it4->second->m_setSpectator.find(pPlayer->GetJoinID()) != it4->second->m_setSpectator.end()) //is spectator? Then do not add to players
					{
						bSpectator = true;
					}

					it4->second->m_mapIndividual.insert({ pPlayer->GetID(), sPLAYER_INFO(pPlayer->GetCharID(), bSpectator) });

					if(bSpectator == false) //only increase counter if not false
						it4->second->m_byCount += 1;

					if (m_matchStateInfo[m_matchType].byState < BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH)
					{
						CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
						sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
						res2->wOpCode = GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY;
						res2->byMatchState = it4->second->m_byMatchState;
						res2->bIsEnter = true;
						res2->byStage = 0;
						res2->dwRemainTime = it4->second->m_dwMatchTickCount;
						packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
						pPlayer->SendPacket(&packet2);
					}
					else
					{
						CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
						sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
						res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
						res2->byMatchState = it4->second->m_byMatchState;
						res2->bIsEnter = true;
						res2->byStage = 0;
						res2->dwRemainTime = it4->second->m_dwMatchTickCount;
						packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
						pPlayer->SendPacket(&packet2);
					}
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: COULD NOT FIND byMatchIndex %u, m_eMatchDepth %u", byMatchIndex, m_eMatchDepth);
				}
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: COULD NOT FIND JOINID %u", pPlayer->GetJoinID());
			}
		}
	}
	else if (m_matchType == BUDOKAI_MATCH_TYPE_TEAM)
	{
		if (m_matchStateInfo[m_matchType].byState == BUDOKAI_MATCHSTATE_MINOR_MATCH)
		{
			auto it3 = m_mapTeam.find(pPlayer->GetJoinID());
			if (it3 != m_mapTeam.end())
			{
				std::map<BYTE, sPRELIM_HEAD*>::iterator it4 = m_mapPrelims.find(it3->second.byMinorMatch_MatchIndex);
				if (it4 != m_mapPrelims.end())
				{
					pPlayer->SetMatchIndex(it4->first);

					it4->second->m_mapIndividualPrelim.insert({ pPlayer->GetID(), sPLAYER_INFO(pPlayer->GetCharID(), false) });

					// Add Player to team
					if (it4->second->m_mapTeamPrelim.find(pPlayer->GetJoinID()) == it4->second->m_mapTeamPrelim.end())
					{
						sPARTY_INFO partyInfo(pPlayer->GetJoinID(), false, it3->second.wszTeamName);
						partyInfo.Add(pPlayer->GetCharID(), pPlayer->GetID(), pPlayer->GetCharID() == it3->second.aMembers[0]);

						it4->second->m_mapTeamPrelim.insert({ pPlayer->GetJoinID(), partyInfo });
					}
					else
					{
						sPARTY_INFO* partyInfo = &it4->second->m_mapTeamPrelim.find(pPlayer->GetJoinID())->second;
						partyInfo->Add(pPlayer->GetCharID(), pPlayer->GetID(), pPlayer->GetCharID() == it3->second.aMembers[0]);
					}

					//add player to party. If dont exist, new party is created.
					g_pPartyManager->BudokaiCreateParty(pPlayer, it3->second.wszTeamName, it3->second.aMembers[0]);

					it4->second->m_byCount += 1;

					CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
					sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_MINORMATCH_STATE_UPDATE_NFY;
					res2->byMatchState = it4->second->m_byMatchState;
					res2->bIsEnter = true;
					res2->byStage = 0;
					res2->dwRemainTime = it4->second->m_dwMatchTickCount;
					packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_STATE_UPDATE_NFY));
					pPlayer->SendPacket(&packet2);
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: COULD NOT FIND byMinorMatch_MatchIndex %u", it3->second.byMinorMatch_MatchIndex);
				}
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: COULD NOT FIND CHARACTER %u", pPlayer->GetCharID());
			}
		}
		else if (m_matchStateInfo[m_matchType].byState > BUDOKAI_MATCHSTATE_MINOR_MATCH && m_matchStateInfo[m_matchType].byState != BUDOKAI_MATCHSTATE_MATCH_END)
		{
			std::map<JOINID, sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA*>::iterator it3 = m_mapTeamTournament.find(pPlayer->GetJoinID());
			if (it3 != m_mapTeamTournament.end())
			{
				BYTE byMatchIndex = GetMatchIndex(pPlayer->GetJoinID());

				std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it4 = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.find(byMatchIndex);
				if (it4 != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end())
				{
					pPlayer->SetMatchIndex(byMatchIndex);

					bool bSpectator = false;

					if (it4->second->m_setSpectator.find(pPlayer->GetJoinID()) != it4->second->m_setSpectator.end()) //is spectator? Then do not add to players
					{
						bSpectator = true;
					}

					it4->second->m_mapIndividual.insert({ pPlayer->GetID(), sPLAYER_INFO(pPlayer->GetCharID(), bSpectator) });

					// Add Player to team
					if (it4->second->m_mapTeam.find(pPlayer->GetJoinID()) == it4->second->m_mapTeam.end())
					{
						sPARTY_INFO partyInfo(pPlayer->GetJoinID(), bSpectator, it3->second->wszTeamName);
						partyInfo.Add(pPlayer->GetCharID(), pPlayer->GetID(), pPlayer->GetCharID() == it3->second->aMembers[0].charId);

						it4->second->m_mapTeam.insert({ pPlayer->GetJoinID(), partyInfo });
					}
					else
					{
						sPARTY_INFO* partyInfo = &it4->second->m_mapTeam.find(pPlayer->GetJoinID())->second;
						partyInfo->Add(pPlayer->GetCharID(), pPlayer->GetID(), pPlayer->GetCharID() == it3->second->aMembers[0].charId);
					}

					//add player to party. If dont exist, new party is created.
					g_pPartyManager->BudokaiCreateParty(pPlayer, it3->second->wszTeamName, it3->second->aMembers[0].charId);

					if (bSpectator == false) //only increase counter if not false
						it4->second->m_byCount += 1;

					if (m_matchStateInfo[m_matchType].byState < BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH)
					{
						CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
						sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
						res2->wOpCode = GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY;
						res2->byMatchState = it4->second->m_byMatchState;
						res2->bIsEnter = true;
						res2->byStage = 0;
						res2->dwRemainTime = it4->second->m_dwMatchTickCount;
						packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_STATE_UPDATE_NFY));
						pPlayer->SendPacket(&packet2);
					}
					else
					{
						CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
						sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY * res2 = (sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY *)packet2.GetPacketData();
						res2->wOpCode = GU_MATCH_FINALMATCH_STATE_UPDATE_NFY;
						res2->byMatchState = it4->second->m_byMatchState;
						res2->bIsEnter = true;
						res2->byStage = 0;
						res2->dwRemainTime = it4->second->m_dwMatchTickCount;
						packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_STATE_UPDATE_NFY));
						pPlayer->SendPacket(&packet2);
					}
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "BUDOKAI: COULD NOT FIND byMatchIndex %u, m_eMatchDepth %u", byMatchIndex, m_eMatchDepth);
				}
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "BUDOKAI: COULD NOT FIND JOINID %u", pPlayer->GetJoinID());
			}
		}
	}
	else
	{
		// debug
	}
}

void CBudokaiManager::JoinStateReq(CPlayer * pPlayer)
{
	CNtlPacket packet(sizeof(sGU_BUDOKAI_JOIN_STATE_RES));
	sGU_BUDOKAI_JOIN_STATE_RES * res = (sGU_BUDOKAI_JOIN_STATE_RES *)packet.GetPacketData();
	res->wOpCode = GU_BUDOKAI_JOIN_STATE_RES;
	res->wResultCode = GAME_SUCCESS;

	std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(pPlayer->GetJoinID());
	if (it != m_mapJoinInfo.end())
	{
		res->byMatchType = m_matchType;
		res->byJoinState = it->second.byJoinState;
		res->byJoinResult = it->second.byJoinResult;
	}
	else res->wResultCode = GAME_BUDOKAI_CHARACTER_NOT_JOINED;

	packet.SetPacketLen(sizeof(sGU_BUDOKAI_JOIN_STATE_RES));
	pPlayer->SendPacket(&packet);
}

void CBudokaiManager::JoinIndividualReq(CPlayer* pPlayer)
{
	CGameServer* app = (CGameServer*)g_pApp;

	WORD wResultcode = CheckBudokaiOpen(pPlayer);

	if (pPlayer->GetJoinID() != INVALID_JOINID)
		wResultcode = GAME_BUDOKAI_CHARACTER_ALREADY_JOIN;

	if (wResultcode == GAME_SUCCESS)
	{
		CNtlPacket packet(sizeof(sGQ_BUDOKAI_JOIN_INDIVIDUAL_REQ));
		sGQ_BUDOKAI_JOIN_INDIVIDUAL_REQ * res = (sGQ_BUDOKAI_JOIN_INDIVIDUAL_REQ *)packet.GetPacketData();
		res->wOpCode = GQ_BUDOKAI_JOIN_INDIVIDUAL_REQ;
		res->charId = pPlayer->GetCharID();
		res->fPoint = pPlayer->GetRankBattleScoreInfo()->fPoint;
		res->handle = pPlayer->GetID();
		packet.SetPacketLen(sizeof(sGQ_BUDOKAI_JOIN_INDIVIDUAL_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);
	}
	else
	{
		CNtlPacket packet(sizeof(sGU_BUDOKAI_JOIN_INDIVIDUAL_RES));
		sGU_BUDOKAI_JOIN_INDIVIDUAL_RES * res = (sGU_BUDOKAI_JOIN_INDIVIDUAL_RES *)packet.GetPacketData();
		res->wOpCode = GU_BUDOKAI_JOIN_INDIVIDUAL_RES;
		res->wResultCode = wResultcode;
		packet.SetPacketLen(sizeof(sGU_BUDOKAI_JOIN_INDIVIDUAL_RES));
		pPlayer->SendPacket(&packet);
	}
}

void CBudokaiManager::LeaveIndividualReq(CPlayer * pPlayer)
{
	CGameServer* app = (CGameServer*)g_pApp;

	WORD wResultcode = IsBudokaiOpen(m_stateInfo.byState) == true ? GAME_SUCCESS : GAME_BUDOKAI_NOT_OPENED;

	if (wResultcode == GAME_SUCCESS)
	{
		CNtlPacket packet(sizeof(sGQ_BUDOKAI_LEAVE_INDIVIDUAL_REQ));
		sGQ_BUDOKAI_LEAVE_INDIVIDUAL_REQ * res = (sGQ_BUDOKAI_LEAVE_INDIVIDUAL_REQ *)packet.GetPacketData();
		res->wOpCode = GQ_BUDOKAI_LEAVE_INDIVIDUAL_REQ;
		res->charId = pPlayer->GetCharID();
		res->handle = pPlayer->GetID();
		packet.SetPacketLen(sizeof(sGQ_BUDOKAI_LEAVE_INDIVIDUAL_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);
	}
	else
	{
		CNtlPacket packet(sizeof(sGU_BUDOKAI_LEAVE_INDIVIDUAL_RES));
		sGU_BUDOKAI_LEAVE_INDIVIDUAL_RES * res = (sGU_BUDOKAI_LEAVE_INDIVIDUAL_RES *)packet.GetPacketData();
		res->wOpCode = GU_BUDOKAI_LEAVE_INDIVIDUAL_RES;
		res->wResultCode = wResultcode;
		packet.SetPacketLen(sizeof(sGU_BUDOKAI_LEAVE_INDIVIDUAL_RES));
		pPlayer->SendPacket(&packet);
	}
}

void CBudokaiManager::JoinTeamReq(CPlayer * pPlayer, WCHAR * wszTeamName)
{
	CGameServer* app = (CGameServer*)g_pApp;

	WORD wResultcode = CheckBudokaiOpen(pPlayer);

	std::wstring wstrTeamName = wszTeamName;

	int nameLen = (int)wstrTeamName.length();

	if (pPlayer->GetJoinID() != INVALID_JOINID)
		wResultcode = GAME_BUDOKAI_CHARACTER_ALREADY_JOIN;
	else if(nameLen > NTL_MAX_SIZE_PARTY_NAME)
		wResultcode = GAME_BUDOKAI_TOO_LONG_TEAM_NAME;
	else if (nameLen < NTL_MIN_SIZE_PARTY_NAME)
		wResultcode = GAME_BUDOKAI_TOO_SHORT_TEAM_NAME;
	else if (wstrTeamName.find_first_not_of(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890") != std::wstring::npos)
		wResultcode = GAME_BUDOKAI_INVALID_TEAM_NAME;

	if (wResultcode == GAME_SUCCESS)
	{
		CParty* pParty = pPlayer->GetParty();

		CNtlPacket packet(sizeof(sGQ_BUDOKAI_JOIN_TEAM_REQ));
		sGQ_BUDOKAI_JOIN_TEAM_REQ * res = (sGQ_BUDOKAI_JOIN_TEAM_REQ *)packet.GetPacketData();
		res->wOpCode = GQ_BUDOKAI_JOIN_TEAM_REQ;
		res->handle = pPlayer->GetID();
		res->charId = pPlayer->GetCharID();
		NTL_SAFE_WCSCPY(res->wszTeamName, wszTeamName);
		res->byMemberCount = pParty->GetPartyMemberCount();
		res->fPoint = 0.0f;

		for (BYTE i = 0; i < res->byMemberCount; i++)
		{
			CPlayer* pMember = g_pObjectManager->GetPC(pParty->GetMemberInfo(i).hHandle);

			res->aMembers[i] = pMember->GetCharID();
			res->aTeamInfo[i].byClass = pMember->GetClass();
			NTL_SAFE_WCSCPY(res->aTeamInfo[i].wszName, pMember->GetCharName());
			res->aTeamInfo[i].byLevel = pMember->GetLevel();
			res->aTeamInfo[i].fPoint = pMember->GetRankBattleScoreInfo()->fPoint;

			res->fPoint += res->aTeamInfo[i].fPoint;
		}
		
		packet.SetPacketLen(sizeof(sGQ_BUDOKAI_JOIN_TEAM_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);
	}
	else
	{
		CNtlPacket packet(sizeof(sGU_BUDOKAI_JOIN_TEAM_RES));
		sGU_BUDOKAI_JOIN_TEAM_RES * res = (sGU_BUDOKAI_JOIN_TEAM_RES *)packet.GetPacketData();
		res->wOpCode = GU_BUDOKAI_JOIN_TEAM_RES;
		res->wResultCode = wResultcode;
		packet.SetPacketLen(sizeof(sGU_BUDOKAI_JOIN_TEAM_RES));
		pPlayer->SendPacket(&packet);
	}
}

void CBudokaiManager::LeaveTeamReq(CPlayer * pPlayer)
{
	CGameServer* app = (CGameServer*)g_pApp;

	WORD wResultcode = IsBudokaiOpen(m_stateInfo.byState) == true ? GAME_SUCCESS : GAME_BUDOKAI_NOT_OPENED;

	if (wResultcode == GAME_SUCCESS)
	{
		auto it = m_mapTeam.find(pPlayer->GetJoinID());
		if (it != m_mapTeam.end())
		{
			if(it->second.aMembers[0] != pPlayer->GetCharID())
				wResultcode = GAME_BUDOKAI_YOU_ARE_NOT_A_TEAM_LEADER;
		}
		else wResultcode = GAME_BUDOKAI_CHARACTER_NOT_JOINED;
	}

	if (wResultcode == GAME_SUCCESS)
	{
		CNtlPacket packet(sizeof(sGQ_BUDOKAI_LEAVE_TEAM_REQ));
		sGQ_BUDOKAI_LEAVE_TEAM_REQ * res = (sGQ_BUDOKAI_LEAVE_TEAM_REQ *)packet.GetPacketData();
		res->wOpCode = GQ_BUDOKAI_LEAVE_TEAM_REQ;
		res->charId = pPlayer->GetCharID();
		res->handle = pPlayer->GetID();
		packet.SetPacketLen(sizeof(sGQ_BUDOKAI_LEAVE_TEAM_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);
	}
	else
	{
		CNtlPacket packet(sizeof(sGU_BUDOKAI_LEAVE_TEAM_RES));
		sGU_BUDOKAI_LEAVE_TEAM_RES * res = (sGU_BUDOKAI_LEAVE_TEAM_RES *)packet.GetPacketData();
		res->wOpCode = GU_BUDOKAI_LEAVE_TEAM_RES;
		res->wResultCode = wResultcode;
		packet.SetPacketLen(sizeof(sGU_BUDOKAI_LEAVE_TEAM_RES));
		pPlayer->SendPacket(&packet);
	}
}

void CBudokaiManager::JoinIndividual(CHARACTERID charId, float fPoint, JOINID wJoinId, bool bDojoRecommender, WCHAR * wszCharName)
{
	if(bDojoRecommender)
		++m_byDojoRecommenders;

	sBUDOKAI_REGISTER_INDIVIDUAL_DATA data;
	data.bDojoRecommender = bDojoRecommender;
	data.byMinorMatch_MatchIndex = INVALID_BYTE;
	data.charId = charId;
	data.fPoint = fPoint;
	data.wJoinId = wJoinId;
	data.wRanking = 4544; //todo
	m_mapIndividual.insert(std::make_pair(wJoinId, data));	//insert into map

	sBUDOKAI_JOIN_INFO joinInfo;
	joinInfo.byMatchType = BUDOKAI_MATCH_TYPE_INDIVIDIAUL;
	joinInfo.byJoinState = BUDOKAI_JOIN_STATE_PLAY;
	joinInfo.byJoinResult = BUDOKAI_JOIN_RESULT_REGISTER;
	joinInfo.sIndividualInfo.bDojoRecommender = bDojoRecommender;
	joinInfo.sIndividualInfo.wJoinId = wJoinId;
	NTL_SAFE_WCSCPY(joinInfo.sIndividualInfo.wszCharName, wszCharName);
	m_mapJoinInfo.insert(std::make_pair(wJoinId, joinInfo));	//insert into map
}

void CBudokaiManager::LeaveIndividual(JOINID joinid)
{
	auto it = m_mapIndividual.find(joinid);
	if (it != m_mapIndividual.end())
	{
		if (it->second.bDojoRecommender)
			--m_byDojoRecommenders;

		m_mapJoinInfo.erase(joinid);

		m_mapIndividual.erase(it);
	}
}

void CBudokaiManager::JoinTeam(CHARACTERID charId, WCHAR * wszTeamName, BYTE byMemberCount, CHARACTERID * aMembers, float fPoint, JOINID wJoinId, sBUDOKAI_TEAM_POINT_INFO * aTeamInfo)
{
	sBUDOKAI_REGISTER_TEAM_DATA data;
	NTL_SAFE_WCSCPY(data.wszTeamName, wszTeamName);
	memcpy(data.aMembers, aMembers, sizeof(data.aMembers));
	data.wJoinId = wJoinId;
	data.wRanking = 4544; //todo
	data.byMinorMatch_MatchIndex = INVALID_BYTE;
	data.fPoint = fPoint;
	m_mapTeam.insert(std::make_pair(wJoinId, data));	//insert into map

	sBUDOKAI_JOIN_INFO joinInfo;
	joinInfo.byMatchType = BUDOKAI_MATCH_TYPE_INDIVIDIAUL;
	joinInfo.byJoinState = BUDOKAI_JOIN_STATE_PLAY;
	joinInfo.byJoinResult = BUDOKAI_JOIN_RESULT_REGISTER;
	joinInfo.sTeamInfo.wJoinId = wJoinId;
	NTL_SAFE_WCSCPY(joinInfo.sTeamInfo.wszTeamName, wszTeamName);
	memcpy(joinInfo.sTeamInfo.aMemberInfo, aTeamInfo, sizeof(joinInfo.sTeamInfo.aMemberInfo));
	m_mapJoinInfo.insert(std::make_pair(wJoinId, joinInfo));	//insert into map
}

void CBudokaiManager::LeaveTeam(JOINID joinid)
{
	auto it = m_mapTeam.find(joinid);
	if (it != m_mapTeam.end())
	{
		m_mapJoinInfo.erase(joinid);

		m_mapTeam.erase(it);
	}
}

void CBudokaiManager::TeleportPrelimReq(CHARACTERID charId, HOBJECT handle, BYTE byServerChannelId, BYTE byServerIndex, BYTE byMatchType, JOINID joinId)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES));
	sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES * res = (sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES *)packet.GetPacketData();
	res->wOpCode = GT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES;
	res->charId = charId;
	res->handle = handle;
	res->byServerChannelId = byServerChannelId;
	res->byServerIndex = byServerIndex;
	res->byMatchType = byMatchType;
	res->joinId = joinId;
	res->wResultCode = GAME_SUCCESS;
	res->byDestServerChannelId = DOJO_CHANNEL_INDEX;
	res->byDestServerIndex = 0;

	if (IsBudokaiOpen(m_stateInfo.byState))
	{
		if (byMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
		{
			if (m_matchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].byState == BUDOKAI_MATCHSTATE_MINOR_MATCH)
			{
				res->byTeleportType = TELEPORT_TYPE_MINORMATCH;
				res->worldTblidx = m_pTableInfo->sIndividualWorldTblidx.minorMatch;

				auto it = m_mapIndividual.find(joinId);
				if (it != m_mapIndividual.end())
				{
					std::map<BYTE, sPRELIM_HEAD*>::iterator it2 = m_mapPrelims.find(it->second.byMinorMatch_MatchIndex);
					if (it2 != m_mapPrelims.end())
					{
						res->worldId = it2->second->m_pWorld->GetID();

						it2->second->m_pWorld->GetTbldat()->vStart1Loc.CopyTo(res->vLoc);
						it2->second->m_pWorld->GetTbldat()->vStart1Dir.CopyTo(res->vDir);

						res->vLoc.x = res->vLoc.x + RandomRangeF(-10.0f, 10.0f);
						res->vLoc.z = res->vLoc.z + RandomRangeF(-10.0f, 10.0f);
					}
					else res->wResultCode = GAME_WORLD_NOT_EXIST;
				}
				else res->wResultCode = GAME_BUDOKAI_CHARACTER_NOT_JOINED;
			}
			else if (m_matchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].byState == BUDOKAI_MATCHSTATE_MAJORMATCH_32 
				|| m_matchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].byState == BUDOKAI_MATCHSTATE_MAJORMATCH_16
				|| m_matchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].byState == BUDOKAI_MATCHSTATE_MAJORMATCH_08)
			{
				res->byTeleportType = TELEPORT_TYPE_MAJORMATCH;
				res->worldTblidx = m_pTableInfo->sIndividualWorldTblidx.majorMatch;

				std::map<JOINID, sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA*>::iterator it = m_mapIndividualTournament.find(joinId);
				if (it != m_mapIndividualTournament.end())
				{
					BYTE byMatchIndex = GetMatchIndex(joinId);

					std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it4 = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.find(byMatchIndex);
					if (it4 != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end())
					{
						res->worldId = it4->second->m_pWorld->GetID();

						if (joinId == it4->second->data.wJoinId1)
						{
							m_pTableInfo->aMajorLoc[0].vLoc.CopyTo(res->vLoc);
							m_pTableInfo->aMajorLoc[0].vDir.CopyTo(res->vDir);
						}
						else if (joinId == it4->second->data.wJoinId2)
						{
							m_pTableInfo->aMajorLoc[5].vLoc.CopyTo(res->vLoc);
							m_pTableInfo->aMajorLoc[5].vDir.CopyTo(res->vDir);
						}
						else res->wResultCode = GAME_BUDOKAI_CHARACTER_NOT_JOINED;
					}
					else res->wResultCode = GAME_WORLD_NOT_EXIST;
				}
				else res->wResultCode = GAME_BUDOKAI_CHARACTER_NOT_JOINED;
			}
			else if (m_matchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].byState == BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH
				|| m_matchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].byState == BUDOKAI_MATCHSTATE_FINAL_MATCH)
			{
				res->byTeleportType = TELEPORT_TYPE_FINALMATCH;
				res->worldTblidx = m_pTableInfo->sIndividualWorldTblidx.finalMatch;

				std::map<JOINID, sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA*>::iterator it = m_mapIndividualTournament.find(joinId);
				if (it != m_mapIndividualTournament.end())
				{
					BYTE byMatchIndex = GetMatchIndex(joinId);

					std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it4 = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.find(byMatchIndex);
					if (it4 != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end())
					{
						res->worldId = it4->second->m_pWorld->GetID();

						if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_2)
						{
							if (it4->second->m_setSpectator.find(joinId) != it4->second->m_setSpectator.end()) //is spectator?
							{
								res->vLoc.x = it4->second->m_pWorld->GetTbldat()->vDefaultLoc.x + RandomRangeF(-10.0f, 10.0f);
								res->vLoc.z = it4->second->m_pWorld->GetTbldat()->vDefaultLoc.z + RandomRangeF(-10.0f, 10.0f);
								res->vLoc.y = it4->second->m_pWorld->GetTbldat()->vDefaultLoc.y;

								m_pTableInfo->aFinalLoc[0].vDir.CopyTo(res->vDir);
							}
						}

						if (joinId == it4->second->data.wJoinId1)
						{
							m_pTableInfo->aFinalLoc[0].vLoc.CopyTo(res->vLoc);
							m_pTableInfo->aFinalLoc[0].vDir.CopyTo(res->vDir);
						}
						else if (joinId == it4->second->data.wJoinId2)
						{
							m_pTableInfo->aFinalLoc[5].vLoc.CopyTo(res->vLoc);
							m_pTableInfo->aFinalLoc[5].vDir.CopyTo(res->vDir);
						}
					}
					else res->wResultCode = GAME_WORLD_NOT_EXIST;
				}
				else res->wResultCode = GAME_BUDOKAI_CHARACTER_NOT_JOINED;
			}
		}
		else if(byMatchType == BUDOKAI_MATCH_TYPE_TEAM)
		{
			if (m_matchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].byState == BUDOKAI_MATCHSTATE_MINOR_MATCH)
			{
				res->byTeleportType = TELEPORT_TYPE_MINORMATCH;
				res->worldTblidx = m_pTableInfo->sTeamWorldTblidx.minorMatch;

				auto it = m_mapTeam.find(joinId);
				if (it != m_mapTeam.end())
				{
					std::map<BYTE, sPRELIM_HEAD*>::iterator it2 = m_mapPrelims.find(it->second.byMinorMatch_MatchIndex);
					if (it2 != m_mapPrelims.end())
					{
						res->worldId = it2->second->m_pWorld->GetID();

						it2->second->m_pWorld->GetTbldat()->vStart1Loc.CopyTo(res->vLoc);
						it2->second->m_pWorld->GetTbldat()->vStart1Dir.CopyTo(res->vDir);

						res->vLoc.x = res->vLoc.x + RandomRangeF(-10.0f, 10.0f);
						res->vLoc.z = res->vLoc.z + RandomRangeF(-10.0f, 10.0f);
					}
					else res->wResultCode = GAME_WORLD_NOT_EXIST;
				}
				else res->wResultCode = GAME_BUDOKAI_CHARACTER_NOT_JOINED;
			}
			else if (m_matchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].byState == BUDOKAI_MATCHSTATE_MAJORMATCH_16
				|| m_matchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].byState == BUDOKAI_MATCHSTATE_MAJORMATCH_08)
			{
				res->byTeleportType = TELEPORT_TYPE_MAJORMATCH;
				res->worldTblidx = m_pTableInfo->sTeamWorldTblidx.majorMatch;

				std::map<JOINID, sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA*>::iterator it = m_mapTeamTournament.find(joinId);
				if (it != m_mapTeamTournament.end())
				{
					BYTE byMatchIndex = GetMatchIndex(joinId);

					std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it4 = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.find(byMatchIndex);
					if (it4 != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end())
					{
						res->worldId = it4->second->m_pWorld->GetID();

						BYTE bySlot = 0;
						for (BYTE i = 0; i < NTL_MAX_MEMBER_IN_PARTY; i++)
						{
							if (it->second->aMembers[i].charId == charId)
								bySlot = i;
						}

						if (joinId == it4->second->data.wJoinId1)
						{
							m_pTableInfo->aMajorLoc[bySlot].vLoc.CopyTo(res->vLoc);
							m_pTableInfo->aMajorLoc[bySlot].vDir.CopyTo(res->vDir);
						}
						else if (joinId == it4->second->data.wJoinId2)
						{
							m_pTableInfo->aMajorLoc[bySlot + 5].vLoc.CopyTo(res->vLoc);
							m_pTableInfo->aMajorLoc[bySlot + 5].vDir.CopyTo(res->vDir);
						}
						else res->wResultCode = GAME_BUDOKAI_CHARACTER_NOT_JOINED;
					}
					else res->wResultCode = GAME_WORLD_NOT_EXIST;
				}
				else res->wResultCode = GAME_BUDOKAI_CHARACTER_NOT_JOINED;
			}
			else if (m_matchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].byState == BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH
				|| m_matchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].byState == BUDOKAI_MATCHSTATE_FINAL_MATCH)
			{
				res->byTeleportType = TELEPORT_TYPE_FINALMATCH;
				res->worldTblidx = m_pTableInfo->sTeamWorldTblidx.finalMatch;

				std::map<JOINID, sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA*>::iterator it = m_mapTeamTournament.find(joinId);
				if (it != m_mapTeamTournament.end())
				{
					BYTE byMatchIndex = GetMatchIndex(joinId);

					std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it4 = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.find(byMatchIndex);
					if (it4 != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end())
					{
						res->worldId = it4->second->m_pWorld->GetID();

						if (m_eMatchDepth == BUDOKAI_MATCH_DEPTH_2)
						{
							if (it4->second->m_setSpectator.find(joinId) != it4->second->m_setSpectator.end()) //is spectator?
							{
								res->vLoc.x = it4->second->m_pWorld->GetTbldat()->vDefaultLoc.x + RandomRangeF(-10.0f, 10.0f);
								res->vLoc.z = it4->second->m_pWorld->GetTbldat()->vDefaultLoc.z + RandomRangeF(-10.0f, 10.0f);
								res->vLoc.y = it4->second->m_pWorld->GetTbldat()->vDefaultLoc.y;

								m_pTableInfo->aFinalLoc[0].vDir.CopyTo(res->vDir);
							}
						}

						BYTE bySlot = 0;
						for (BYTE i = 0; i < NTL_MAX_MEMBER_IN_PARTY; i++)
						{
							if (it->second->aMembers[i].charId == charId)
								bySlot = i;
						}

						if (joinId == it4->second->data.wJoinId1)
						{
							m_pTableInfo->aFinalLoc[bySlot].vLoc.CopyTo(res->vLoc);
							m_pTableInfo->aFinalLoc[bySlot].vDir.CopyTo(res->vDir);
						}
						else if (joinId == it4->second->data.wJoinId2)
						{
							m_pTableInfo->aFinalLoc[bySlot + 5].vLoc.CopyTo(res->vLoc);
							m_pTableInfo->aFinalLoc[bySlot + 5].vDir.CopyTo(res->vDir);
						}
					}
					else res->wResultCode = GAME_WORLD_NOT_EXIST;
				}
				else res->wResultCode = GAME_BUDOKAI_CHARACTER_NOT_JOINED;
			}
		}
		else
		{
			// DEBUG
		}
	}
	else res->wResultCode = GAME_BUDOKAI_NOT_OPENED;

	packet.SetPacketLen(sizeof(sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES));
	app->SendTo(app->GetChatServerSession(), &packet);
}

void CBudokaiManager::MinorMatchUpdateScore(BYTE byMatchIndex, TEAMTYPE wTeamType, HOBJECT hSlayer, HOBJECT hFainter)
{
	std::map<BYTE, sPRELIM_HEAD*>::iterator it = m_mapPrelims.find(byMatchIndex);
	if (it != m_mapPrelims.end())
	{
		std::map<HOBJECT, sPLAYER_INFO>::iterator it3 = it->second->m_mapIndividualPrelim.find(hFainter);
		if (it3 != it->second->m_mapIndividualPrelim.end())
		{
			//set respawn time
			it3->second.dwTick = 9000;

			CNtlPacket packet2(sizeof(sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY));
			sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
			res2->wOpCode = GU_MATCH_MINORMATCH_PLAYER_STATE_NFY;
			res2->hPc = hFainter;
			res2->byPcState = MATCH_MEMBER_STATE_FAINT;
			packet2.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_PLAYER_STATE_NFY));
			it->second->m_pWorld->Broadcast(&packet2);
		}

		if (wTeamType != INVALID_TEAMTYPE)
		{
			CNtlPacket packet(sizeof(sGU_MATCH_MINORMATCH_UPDATE_SCORE_NFY));
			sGU_MATCH_MINORMATCH_UPDATE_SCORE_NFY * res = (sGU_MATCH_MINORMATCH_UPDATE_SCORE_NFY *)packet.GetPacketData();
			res->wOpCode = GU_MATCH_MINORMATCH_UPDATE_SCORE_NFY;
			res->wTeamType = wTeamType;
			res->hSlayer = hSlayer;
			res->hFainter = hFainter;

			if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
			{
				std::map<HOBJECT, sPLAYER_INFO>::iterator it2 = it->second->m_mapIndividualPrelim.find(hSlayer);
				if (it2 != it->second->m_mapIndividualPrelim.end())
				{
					it2->second.byKills += 1;

					res->byScore = it2->second.byKills;
				}
			}
			else if (m_matchType == BUDOKAI_MATCH_TYPE_TEAM)
			{
				CPlayer* pSlayer = g_pObjectManager->GetPC(hSlayer);
				if (pSlayer)
				{
					std::map<JOINID, sPARTY_INFO>::iterator it2 = it->second->m_mapTeamPrelim.find(pSlayer->GetJoinID());
					if (it2 != it->second->m_mapTeamPrelim.end())
					{
						it2->second.byKills += 1;

						res->byScore = it2->second.byKills;
					}
				}
			}

			packet.SetPacketLen(sizeof(sGU_MATCH_MINORMATCH_UPDATE_SCORE_NFY));
			it->second->m_pWorld->Broadcast(&packet);
		}
	}
	else ERR_LOG(LOG_GENERAL, "Could not update prelim score. byMatchIndex %u, wTeamType %u, hSlayer %u, hFainter %u", byMatchIndex, wTeamType, hSlayer, hFainter);
}

void CBudokaiManager::MajorMatchUpdateScore(BYTE byMatchIndex, TEAMTYPE wTeamType, HOBJECT hFainter, JOINID fainterJoinId)
{
	if (m_eMatchDepth >= BUDOKAI_MATCH_DEPTH_COUNT)
		return;

	std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.find(byMatchIndex);
	if (it != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end())
	{
		if (it->second->m_byMatchState != BUDOKAI_MAJORMATCH_STATE_STAGE_RUN)
			return;

		std::map<HOBJECT, sPLAYER_INFO>::iterator it3 = it->second->m_mapIndividual.find(hFainter);
		if (it3 != it->second->m_mapIndividual.end())
		{
			CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
			sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
			res2->wOpCode = GU_MATCH_MAJORMATCH_PLAYER_STATE_NFY;
			res2->hPc = hFainter;
			res2->byPcState = MATCH_MEMBER_STATE_FAINT;
			packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
			it->second->m_pWorld->Broadcast(&packet2);
		}


		if (wTeamType == MATCH_TEAM_TYPE_TEAM1)
		{
			it->second->m_byTeam1UnavailablePlayerCount += 1;

			if (it->second->m_byTeam1UnavailablePlayerCount >= it->second->m_byTeam1PlayerCount)
			{
				UpdateMajorMatchScore(it->second, byMatchIndex, MATCH_RESULT_WIN, MATCH_TEAM_TYPE_TEAM2);
				return;
			}
		}
		else if (wTeamType == MATCH_TEAM_TYPE_TEAM2)
		{
			it->second->m_byTeam2UnavailablePlayerCount += 1;

			if (it->second->m_byTeam2UnavailablePlayerCount >= it->second->m_byTeam2PlayerCount)
			{
				UpdateMajorMatchScore(it->second, byMatchIndex, MATCH_RESULT_WIN, MATCH_TEAM_TYPE_TEAM1);
				return;
			}
		}
	}
	else ERR_LOG(LOG_GENERAL, "Could not update match score. byMatchIndex %u, wTeamType %u, m_eMatchDepth %u", byMatchIndex, wTeamType, m_eMatchDepth);
}

void CBudokaiManager::FinalMatchUpdateScore(BYTE byMatchIndex, TEAMTYPE wTeamType, HOBJECT hFainter, JOINID fainterJoinId)
{
	if (m_eMatchDepth >= BUDOKAI_MATCH_DEPTH_COUNT)
		return;

	std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.find(byMatchIndex);
	if (it != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end())
	{
		if (it->second->m_byMatchState != BUDOKAI_FINALMATCH_STATE_STAGE_RUN)
			return;

		std::map<HOBJECT, sPLAYER_INFO>::iterator it3 = it->second->m_mapIndividual.find(hFainter);
		if (it3 != it->second->m_mapIndividual.end())
		{
			CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY));
			sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
			res2->wOpCode = GU_MATCH_FINALMATCH_PLAYER_STATE_NFY;
			res2->hPc = hFainter;
			res2->byPcState = MATCH_MEMBER_STATE_FAINT;
			packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY));
			it->second->m_pWorld->Broadcast(&packet2);
		}


		if (wTeamType == MATCH_TEAM_TYPE_TEAM1)
		{
			it->second->m_byTeam1UnavailablePlayerCount += 1;

			if (it->second->m_byTeam1UnavailablePlayerCount >= it->second->m_byTeam1PlayerCount)
			{
				UpdateFinalMatchScore(it->second, byMatchIndex, MATCH_RESULT_WIN, MATCH_TEAM_TYPE_TEAM2);
				return;
			}
		}
		else if (wTeamType == MATCH_TEAM_TYPE_TEAM2)
		{
			it->second->m_byTeam2UnavailablePlayerCount += 1;

			if (it->second->m_byTeam2UnavailablePlayerCount >= it->second->m_byTeam2PlayerCount)
			{
				UpdateFinalMatchScore(it->second, byMatchIndex, MATCH_RESULT_WIN, MATCH_TEAM_TYPE_TEAM1);
				return;
			}
		}

	}
	else ERR_LOG(LOG_GENERAL, "Could not update final match score. byMatchIndex %u, wTeamType %u, m_eMatchDepth %u", byMatchIndex, wTeamType, m_eMatchDepth);
}

void CBudokaiManager::PlayerDisconnect(CHARACTERID charId, HOBJECT hPlayer, JOINID joinId, BYTE byMatchIndex, TEAMTYPE wTeamType)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (m_eMatchDepth != INVALID_BUDOKAI_MATCH_DEPTH) //check if logout during torunament
	{
		std::map<BYTE, sTOURNAMENT_MATCH*>::iterator it = m_aTournamentMatch[m_eMatchDepth].m_mapTournament.find(byMatchIndex);
		if (it != m_aTournamentMatch[m_eMatchDepth].m_mapTournament.end())
		{
			if (it->second->bFinishMatch)
				return;

			std::map<HOBJECT, sPLAYER_INFO>::iterator it3 = it->second->m_mapIndividual.find(hPlayer);
			if (it3 != it->second->m_mapIndividual.end())
			{
				if (m_eMatchDepth <= BUDOKAI_MATCH_DEPTH_4)
				{
					CNtlPacket packet2(sizeof(sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY));
					sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_FINALMATCH_PLAYER_STATE_NFY;
					res2->hPc = hPlayer;
					res2->byPcState = MATCH_MEMBER_STATE_GIVEUP;
					packet2.SetPacketLen(sizeof(sGU_MATCH_FINALMATCH_PLAYER_STATE_NFY));
					it->second->m_pWorld->Broadcast(&packet2);
				}
				else
				{
					CNtlPacket packet2(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
					sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY * res2 = (sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_MATCH_MAJORMATCH_PLAYER_STATE_NFY;
					res2->hPc = hPlayer;
					res2->byPcState = MATCH_MEMBER_STATE_GIVEUP;
					packet2.SetPacketLen(sizeof(sGU_MATCH_MAJORMATCH_PLAYER_STATE_NFY));
					it->second->m_pWorld->Broadcast(&packet2);
				}

				if (it3->second.bSpectator == false)
					it->second->m_byCount = UnsignedSafeDecrease<BYTE>(it->second->m_byCount, 1);

				it->second->m_mapIndividual.erase(it3); //this is required. If we dont have this, then it bugs in party if someone dc		

				if (wTeamType == MATCH_TEAM_TYPE_TEAM1)
				{
					it->second->m_byTeam1PlayerCount = UnsignedSafeDecrease<BYTE>(it->second->m_byTeam1PlayerCount, 1);
					if (it->second->m_byTeam1UnavailablePlayerCount >= it->second->m_byTeam1PlayerCount)
					{
						UpdateMajorMatchScore(it->second, byMatchIndex, MATCH_RESULT_WIN, MATCH_TEAM_TYPE_TEAM2);
						return;
					}
				}
				else if (wTeamType == MATCH_TEAM_TYPE_TEAM2)
				{
					it->second->m_byTeam2PlayerCount = UnsignedSafeDecrease<BYTE>(it->second->m_byTeam2PlayerCount, 1);
					if (it->second->m_byTeam2UnavailablePlayerCount >= it->second->m_byTeam2PlayerCount)
					{
						UpdateMajorMatchScore(it->second, byMatchIndex, MATCH_RESULT_WIN, MATCH_TEAM_TYPE_TEAM1);
						return;
					}
				}
			}
		}
	}
	else //else logout during prelim
	{
		if (m_matchType == BUDOKAI_MATCH_TYPE_TEAM)
		{
			std::map<BYTE, sPRELIM_HEAD*>::iterator it = m_mapPrelims.find(byMatchIndex);
			if (it != m_mapPrelims.end())
			{
				std::map<JOINID, sPARTY_INFO>::iterator it2 = it->second->m_mapTeamPrelim.find(joinId);
				if (it2 != it->second->m_mapTeamPrelim.end())
				{
					it2->second.byMemberCount = UnsignedSafeDecrease<BYTE>(it2->second.byMemberCount, 1);

					if (it2->second.byMemberCount == 0) // if member count is 0
					{
						ERR_LOG(LOG_GENERAL, "BUDOKAI: Prelim Index %u. All players of team %u with join-id left..", byMatchIndex, joinId);

						if (it->second->m_byMatchState == BUDOKAI_MINORMATCH_STATE_WAIT || it->second->m_byMatchState == BUDOKAI_MINORMATCH_STATE_DIRECTION
							|| it->second->m_byMatchState == BUDOKAI_MINORMATCH_STATE_MATCH_READY)
						{
							it->second->m_byTotalCount = UnsignedSafeDecrease<BYTE>(it->second->m_byTotalCount, 5); // after BUDOKAI_MINORMATCH_STATE_MATCH_READY finish, the totalcount will be checked and decided what to do
						}
					}
				}
				else
				{
					ERR_LOG(LOG_WARNING, "Party Prelim match index %u could not find team with joinid %u", byMatchIndex, joinId);
				}
			}
			else
			{
				ERR_LOG(LOG_WARNING, "Party Prelim match index %u not found", byMatchIndex);
			}
		}
	}
}

WORD CBudokaiManager::CheckBudokaiOpen(CPlayer * pPlayer)
{
	//printf("%u %u %u %u \n", m_pTableInfo->byJuniorLevelMin, m_pTableInfo->byJuniorLevelMax, m_pTableInfo->byAdultLevelMin, m_pTableInfo->byAdultLevelMax);
	if (m_type == BUDOKAI_TYPE_JUNIOR)
	{
		if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
		{
			if (m_bJuniorBudokaiBegan)
			{
				if (pPlayer->GetLevel() < m_pTableInfo->byJuniorLevelMin || pPlayer->GetLevel() > m_pTableInfo->byJuniorLevelMax)
					return GAME_CHAR_LEVEL_FAIL;
			}
			else return GAME_BUDOKAI_NOT_OPENED;
		}
		else
		{
			if (m_bPartyJuniorBudokaiBegan)
			{
				CParty* pParty = pPlayer->GetParty();

				if (pParty == NULL)
					return GAME_PARTY_YOU_ARE_NOT_IN_PARTY;
				if(pParty->GetPartyLeaderID() != pPlayer->GetID())
					return GAME_BUDOKAI_YOU_ARE_NOT_A_TEAM_LEADER;
				if(pParty->GetPartyMemberCount() < NTL_MAX_MEMBER_IN_PARTY)
					return GAME_BUDOKAI_NEED_MORE_MEMBER;

				for (BYTE i = 0; i < pParty->GetPartyMemberCount(); i++)
				{
					if(g_pObjectManager->GetPC(pParty->GetMemberInfo(i).hHandle) == NULL)
						return GAME_BUDOKAI_NEED_MORE_MEMBER;

					if (pParty->GetMemberInfo(i).byLevel < m_pTableInfo->byJuniorLevelMin || pParty->GetMemberInfo(i).byLevel > m_pTableInfo->byJuniorLevelMax)
						return GAME_CHAR_LEVEL_FAIL;
				}
			}
			else return GAME_BUDOKAI_NOT_OPENED;
		}
	}
	else
	{
		if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
		{
			if (m_bAdultBudokaiBegan)
			{
				if (pPlayer->GetLevel() < m_pTableInfo->byAdultLevelMin || pPlayer->GetLevel() > m_pTableInfo->byAdultLevelMax)
					return GAME_CHAR_LEVEL_FAIL;
			}
			else return GAME_BUDOKAI_NOT_OPENED;
		}
		else
		{
			if (m_bPartyAdultBudokaiBegan)
			{
				CParty* pParty = pPlayer->GetParty();

				if (pParty == NULL)
					return GAME_PARTY_YOU_ARE_NOT_IN_PARTY;
				if (pParty->GetPartyLeaderID() != pPlayer->GetID())
					return GAME_BUDOKAI_YOU_ARE_NOT_A_TEAM_LEADER;
				if (pParty->GetPartyMemberCount() < NTL_MAX_MEMBER_IN_PARTY)
					return GAME_BUDOKAI_NEED_MORE_MEMBER;

				for (BYTE i = 0; i < pParty->GetPartyMemberCount(); i++)
				{
					if (g_pObjectManager->GetPC(pParty->GetMemberInfo(i).hHandle) == NULL)
						return GAME_BUDOKAI_NEED_MORE_MEMBER;

					if (pParty->GetMemberInfo(i).byLevel < m_pTableInfo->byAdultLevelMin || pParty->GetMemberInfo(i).byLevel > m_pTableInfo->byAdultLevelMax)
						return GAME_CHAR_LEVEL_FAIL;
				}
			}
			else return GAME_BUDOKAI_NOT_OPENED;
		}
	}

	if (IsBudokaiOpen(m_stateInfo.byState) == false || m_matchStateInfo[m_matchType].byState != BUDOKAI_MATCHSTATE_REGISTER)
		return GAME_BUDOKAI_NOT_OPENED;

	return GAME_SUCCESS;
}

bool CBudokaiManager::IsInsideArena(CNtlVector & vLoc, CWorld* pWorld)
{
	if (pWorld)
	{
		if (vLoc.x < pWorld->GetTbldat()->vBattleStartLoc.x && vLoc.z < pWorld->GetTbldat()->vBattleStartLoc.z && vLoc.x > pWorld->GetTbldat()->vBattleEndLoc.x && vLoc.z > pWorld->GetTbldat()->vBattleEndLoc.z)
			return true;
	}

	return false;
}

void CBudokaiManager::SetJoinResult(JOINID joinid, BYTE byResult)
{
	std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it2 = m_mapJoinInfo.find(joinid);
	if (it2 != m_mapJoinInfo.end())
	{
		it2->second.byJoinResult = byResult;
	}
}

void CBudokaiManager::SetJoinData(JOINID joinid, BYTE byJoinState, BYTE byJoinResult)
{
	std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it2 = m_mapJoinInfo.find(joinid);
	if (it2 != m_mapJoinInfo.end())
	{
		it2->second.byJoinResult = byJoinResult;
		it2->second.byJoinState = byJoinState;
	}
}

bool CBudokaiManager::CanTeleportPrelim(CPlayer * pPlayer)
{
	if (m_matchStateInfo[m_matchType].byState != BUDOKAI_MATCHSTATE_MINOR_MATCH)
		return false;

	std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it2 = m_mapJoinInfo.find(pPlayer->GetJoinID());
	if (it2 != m_mapJoinInfo.end())
	{
		if (it2->second.byJoinResult == BUDOKAI_JOIN_RESULT_MINORMATCH)
			return true;
	}

	return false;
}

bool CBudokaiManager::CanTeleportMajorMatch(CPlayer * pPlayer)
{
	if (m_matchStateInfo[m_matchType].byState < BUDOKAI_MATCHSTATE_MINOR_MATCH)
		return false;

	std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it2 = m_mapJoinInfo.find(pPlayer->GetJoinID());
	if (it2 != m_mapJoinInfo.end())
	{
		if (it2->second.byJoinResult > BUDOKAI_JOIN_RESULT_MINORMATCH && it2->second.byJoinState == BUDOKAI_JOIN_STATE_PLAY)
			return true;
	}
	
	return false;
}

void CBudokaiManager::SetIndividualTeamType(CHARACTERID charId, TEAMTYPE teamType)
{
	std::map<CHARACTERID, TEAMTYPE>::iterator it = m_mapTeamType.find(charId);
	if (it != m_mapTeamType.end())
		it->second = teamType;
	else
		m_mapTeamType.insert({ charId , teamType });
}

TEAMTYPE CBudokaiManager::GetIndividualTeamType(CHARACTERID charId)
{
	std::map<CHARACTERID, TEAMTYPE>::iterator it = m_mapTeamType.find(charId);
	if (it != m_mapTeamType.end())
		return it->second;

	return INVALID_MATCH_TEAM_TYPE;
}

void CBudokaiManager::SetMatchIndex(JOINID joinId, BYTE byIndex)
{
	std::map<JOINID, BYTE>::iterator it = m_mapMatchIndex.find(joinId);
	if (it != m_mapMatchIndex.end())
		it->second = byIndex;
	else
		m_mapMatchIndex.insert({ joinId , byIndex });
}

BYTE CBudokaiManager::GetMatchIndex(JOINID joinId)
{
	std::map<JOINID, BYTE>::iterator it = m_mapMatchIndex.find(joinId);
	if (it != m_mapMatchIndex.end())
		return it->second;

	return INVALID_BYTE;
}

TEAMTYPE CBudokaiManager::DecideTeamType(BYTE nextMatchDepth, BYTE byPrelimIndex)
{
	TEAMTYPE teamType = MATCH_TEAM_TYPE_TEAM2;

	if (m_matchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		switch (nextMatchDepth)
		{
			case BUDOKAI_MATCH_DEPTH_16:
			{
				if (byPrelimIndex == 0 || byPrelimIndex == 1
					|| byPrelimIndex == 4 || byPrelimIndex == 5
					|| byPrelimIndex == 8 || byPrelimIndex == 9
					|| byPrelimIndex == 12 || byPrelimIndex == 13
					|| byPrelimIndex == 16 || byPrelimIndex == 17
					|| byPrelimIndex == 20 || byPrelimIndex == 21
					|| byPrelimIndex == 24 || byPrelimIndex == 25
					|| byPrelimIndex == 28 || byPrelimIndex == 29)
					return MATCH_TEAM_TYPE_TEAM1;
			}
			break;
			case BUDOKAI_MATCH_DEPTH_8:
			{
				if (byPrelimIndex == 0 || byPrelimIndex == 1 || byPrelimIndex == 2 || byPrelimIndex == 3
					|| byPrelimIndex == 8 || byPrelimIndex == 9 || byPrelimIndex == 10 || byPrelimIndex == 11
					|| byPrelimIndex == 16 || byPrelimIndex == 17 || byPrelimIndex == 18 || byPrelimIndex == 19
					|| byPrelimIndex == 24 || byPrelimIndex == 25 || byPrelimIndex == 26 || byPrelimIndex == 27)
					return MATCH_TEAM_TYPE_TEAM1;
			}
			break;
			case BUDOKAI_MATCH_DEPTH_4:
			{
				if (byPrelimIndex == 0 || byPrelimIndex == 1 || byPrelimIndex == 2 || byPrelimIndex == 3 || byPrelimIndex == 4 || byPrelimIndex == 5 || byPrelimIndex == 6 || byPrelimIndex == 7
					|| byPrelimIndex == 16 || byPrelimIndex == 17 || byPrelimIndex == 18 || byPrelimIndex == 19 || byPrelimIndex == 20 || byPrelimIndex == 21 || byPrelimIndex == 22 || byPrelimIndex == 23)
					return MATCH_TEAM_TYPE_TEAM1;
			}
			break;
			case BUDOKAI_MATCH_DEPTH_2:
			{
				if (byPrelimIndex >= 0 && byPrelimIndex <= 15)
					return MATCH_TEAM_TYPE_TEAM1;
			}
			break;
		}
	}
	else
	{
		switch (nextMatchDepth)
		{
			case BUDOKAI_MATCH_DEPTH_8:
			{
				if (byPrelimIndex == 0 || byPrelimIndex == 1 
					|| byPrelimIndex == 4 || byPrelimIndex == 5 
					|| byPrelimIndex == 8 || byPrelimIndex == 9
					|| byPrelimIndex == 12 || byPrelimIndex == 13)
					return MATCH_TEAM_TYPE_TEAM1;
			}
			break;
			case BUDOKAI_MATCH_DEPTH_4:
			{
				if (byPrelimIndex == 0 || byPrelimIndex == 1 || byPrelimIndex == 2 || byPrelimIndex == 3 
					|| byPrelimIndex == 8 || byPrelimIndex == 9 || byPrelimIndex == 10 || byPrelimIndex == 11)
					return MATCH_TEAM_TYPE_TEAM1;
			}
			break;
			case BUDOKAI_MATCH_DEPTH_2:
			{
				if (byPrelimIndex >= 0 && byPrelimIndex <= 7)
					return MATCH_TEAM_TYPE_TEAM1;
			}
			break;
		}
	}

	return teamType;
}


