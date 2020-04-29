#ifndef __BUDOKAI_MANAGER__
#define __BUDOKAI_MANAGER__

#include "NtlSingleton.h"
#include "BudokaiTable.h"

#include <unordered_map>


class CPlayer;
class CWorld;

class CBudokaiManager : public CNtlSingleton<CBudokaiManager>
{
	struct sPLAYER_INFO
	{
		sPLAYER_INFO(CHARACTERID id, bool bFlag)
		{
			charId = id;
			byKills = 0;
			dwTick = 0;
			bSpectator = bFlag;
		}

		CHARACTERID		charId;
		BYTE			byKills;
		DWORD			dwTick;
		bool			bSpectator;
	};

	struct sPARTY_INFO
	{
		sPARTY_INFO(JOINID id, bool bFlag, WCHAR* teamName)
		{
			joinId = id;
			memset(charId, INVALID_CHARACTERID, sizeof(charId));
			memset(charHandle, INVALID_HOBJECT, sizeof(charHandle));
			leaderCharId = INVALID_CHARACTERID;
			byKills = 0;
			byMemberCount = 0;
			teamType = INVALID_TEAMTYPE;
			bSpectator = bFlag;
			NTL_SAFE_WCSCPY(m_wszTeamName, teamName);
		}

		void Add(CHARACTERID charid, HOBJECT handle, bool bLeader)
		{
			if (bLeader)
			{
				leaderCharId = charid;
			//	charId[0] = charid;
			//	charHandle[0] = handle;
			}
		//	else
		//	{
		//		BYTE bySlot = (leaderCharId == INVALID_CHARACTERID) ? byMemberCount + 1 : byMemberCount;
				BYTE bySlot = byMemberCount;
				charId[bySlot] = charid;
				charHandle[bySlot] = handle;
		//	}

			++byMemberCount;
		}


		JOINID			joinId;
		CHARACTERID		charId[NTL_MAX_MEMBER_IN_PARTY];
		HOBJECT			charHandle[NTL_MAX_MEMBER_IN_PARTY];
		CHARACTERID		leaderCharId;
		BYTE			byKills;
		BYTE			byMemberCount;
		TEAMTYPE		teamType;
		WCHAR			m_wszTeamName[NTL_MAX_SIZE_PARTY_NAME + 1];
		bool			bSpectator;
	};

	struct sPRELIM_HEAD
	{
		virtual ~sPRELIM_HEAD();

		BYTE							m_byMatchState;
		DWORD							m_dwMatchTickCount;
		BYTE							m_byCount;
		BYTE							m_byTotalCount;

		JOINID							wPrelimWinner;

		CWorld*							m_pWorld;

		std::map<HOBJECT, sPLAYER_INFO>	m_mapIndividualPrelim;		//
		std::map<JOINID, sPARTY_INFO>	m_mapTeamPrelim;			//byte = kills

		std::map<unsigned int, BYTE>	m_mapSelection; //first = player/team handle, second = dice number
	};

	struct sTOURNAMENT_MATCH
	{
		sTOURNAMENT_MATCH::sTOURNAMENT_MATCH()
		{
			m_byMatchState = BUDOKAI_MAJORMATCH_STATE_WAIT;
			m_dwMatchTickCount = 2 * 60 * 1000;
			m_byTotalCount = 0;
			m_byCount = 0;
			m_byTeam1PlayerCount = 0;
			m_byTeam2PlayerCount = 0;
			m_byTeam1UnavailablePlayerCount = 0;
			m_byTeam2UnavailablePlayerCount = 0;
			m_pWorld = NULL;
			byStage = 0;
			data.byMatchResult = MATCH_RESULT_SUCCESS;
			data.wWinnerTeam = INVALID_JOINID;
			data.wJoinId1 = INVALID_JOINID;
			data.wJoinId2 = INVALID_JOINID;
			data.byScore1 = 0;
			data.byScore2 = 0;
			bFinishMatch = false;
			bEnd = false;
			wLoserJoinID = INVALID_JOINID;
		}

		virtual ~sTOURNAMENT_MATCH();

		BYTE							m_byMatchState;
		DWORD							m_dwMatchTickCount;
		BYTE							m_byTotalCount;
		BYTE							m_byCount;
		BYTE							m_byTeam1PlayerCount;
		BYTE							m_byTeam2PlayerCount;
		BYTE							m_byTeam1UnavailablePlayerCount;
		BYTE							m_byTeam2UnavailablePlayerCount;
		BYTE							byStage;			//stage is like the current round. 0 = round 1
		bool							bFinishMatch;
		bool							bEnd;

		CWorld*							m_pWorld;

		JOINID							wLoserJoinID;

		sBUDOKAI_TOURNAMENT_MATCH_DATA	data;

		std::set<JOINID>				m_setSpectator;
				
		std::map<HOBJECT, sPLAYER_INFO>	m_mapIndividual;	//
		std::map<JOINID, sPARTY_INFO>	m_mapTeam;			//
	};

	struct sTOURNAMENT_HEAD
	{
		BYTE								byMatchCount;
		BYTE								byMatchCountEnd;

		std::map<BYTE, sTOURNAMENT_MATCH*>	m_mapTournament;	//first = match number
	};


public:

	CBudokaiManager();
	virtual ~CBudokaiManager();

public:

	void								StartSoloAdultBudokai();
	void								StartSoloJuniorBudokai();
	void								StartPartyAdultBudokai();
	void								StartPartyJuniorBudokai();

	void								CloseBudokai();

private:

	void								Init();

	void								Destroy();

public:

	void								RecvInitInfo(WORD wSeasonCount, BUDOKAITIME tmDefaultOpenTime);

	void								TickProcess(DWORD dwTickDiff);//this function only called by budokai channel

	void								UpdateChannelStateInfo(BUDOKAITIME tmDefaultOpenTime, bool bRankbattleAllow, sBUDOKAI_UPDATE_STATE_INFO& sStateInfo, sBUDOKAI_UPDATE_MATCH_STATE_INFO* aMatchStateInfo);

	void								UpdateChannelState(BUDOKAITIME tmDefaultOpenTime, bool bRankbattleAllow, sBUDOKAI_UPDATE_STATE_INFO& sStateInfo);

	void								UpdateMatchState(BYTE byMatchType, sBUDOKAI_UPDATE_MATCH_STATE_INFO& sStateInfo);

	void								SendTournamentIndividualListToChat(CHARACTERID charId, BYTE byReceivedListCount, HOBJECT hSession);
	void								SendTournamentIndividualInfoToChat(CHARACTERID charId, WORD* awJoinId, HOBJECT hSession);

	void								SendTournamentTeamListToChat(CHARACTERID charId, BYTE byReceivedListCount, HOBJECT hSession);
	void								SendTournamentTeamInfoToChat(CHARACTERID charId, WORD* awJoinId, HOBJECT hSession);

private:

	void								CreateBudokai(eBUDOKAI_TYPE type, eBUDOKAI_MATCH_TYPE matchType, BUDOKAITIME curTime, sBUDOKAI_TBLINFO* pTblInfo); //this function only called by budokai channel

	bool								TickProcessBudokai(DWORD dwTickDiff, BUDOKAITIME curTime);//this function only called by budokai channel
	void								TickProcessMatch(DWORD dwTickDif, BUDOKAITIME curTime);//this function only called by budokai channel

	void								SendBudokaiState();
	void								SendMatchState();

	// ----
	void								StartPrelim();
	void								EndPrelim(); //here we create major match

	void								TickProcessPrelim(DWORD dwTickDif, BUDOKAITIME curTime);

	void								SendMinorMatchTeamInfo(BYTE byMatchIndex, sPRELIM_HEAD* prelim); // TODO
	void								MinorMatchUpdatePlayersState(sPRELIM_HEAD* prelim, BYTE byMatchIndex, BYTE byState);

	void								ProcessMinorMatch(sPRELIM_HEAD* prelim, BYTE byMatchIndex, DWORD dwTickDif);
	void								DecidePrelimWinner(sPRELIM_HEAD* prelim, BYTE byMatchIndex);
	void								MinorMatchSelection(sPRELIM_HEAD* prelim, BYTE byMatchIndex);
	void								MinorMatchStageFinish(sPRELIM_HEAD* prelim, BYTE byMatchIndex);
	void								MinorMatchMatchFinish(sPRELIM_HEAD* prelim, BYTE byMatchIndex);
	void								MinorMatchEnd(sPRELIM_HEAD* prelim, BYTE byMatchIndex);

	void								MinorMatchSpawnPlayerRandomPlace(CPlayer* pPlayer, bool bSpawnFromFaint);
	// ----

	void								TickProcessMajorMatch(DWORD dwTickDif, BUDOKAITIME curTime);

	void								StartMajorMatchTeleport();

	void								SendMajorMatchTeamInfo(BYTE byMatchIndex, sTOURNAMENT_MATCH* match);
	void								MajorMatchUpdatePlayersState(sTOURNAMENT_MATCH* match, BYTE byMatchIndex, BYTE byState);

	bool								ProcessMajorMatch(sTOURNAMENT_MATCH* match, BYTE byMatchIndex, DWORD dwTickDif);
	void								UpdateMajorMatchScore(sTOURNAMENT_MATCH* match, BYTE byMatchIndex, BYTE byMatchResult, TEAMTYPE wMatchWinner, BYTE byWins = 1);
	void								ResetPlayerMajorMatch(sTOURNAMENT_MATCH* match, BYTE byMatchIndex, bool bSpawn);
	void								MajorMatchMatchFinish(sTOURNAMENT_MATCH* match, BYTE byMatchIndex);
	void								MajorMatchEnd(sTOURNAMENT_MATCH* match, BYTE byMatchIndex);

	void								EndMatchDepth(eBUDOKAI_MATCH_DEPTH matchDepth);

	void								MajorMatchTimeEnd(sTOURNAMENT_MATCH* match, BYTE byMatchIndex);

	bool								MajorMatchCheckPlayerCount(sTOURNAMENT_MATCH* match, BYTE byMatchIndex);

	// ----

	void								TickProcessFinalMatch(DWORD dwTickDif, BUDOKAITIME curTime);

	void								StartFinalMatchTeleport();

	void								SendFinalMatchTeamInfo(BYTE byMatchIndex, sTOURNAMENT_MATCH* match);
	void								FinalMatchUpdatePlayersState(sTOURNAMENT_MATCH* match, BYTE byMatchIndex, BYTE byState);

	bool								ProcessFinalMatch(sTOURNAMENT_MATCH* match, BYTE byMatchIndex, DWORD dwTickDif);
	void								UpdateFinalMatchScore(sTOURNAMENT_MATCH* match, BYTE byMatchIndex, BYTE byMatchResult, TEAMTYPE wMatchWinner, BYTE byWins = 1);
	void								FinalMatchMatchFinish(sTOURNAMENT_MATCH* match, BYTE byMatchIndex);

	void								ResetPlayerFinalMatch(sTOURNAMENT_MATCH* match, BYTE byMatchIndex, bool bSpawn);

	void								FinalMatchTimeEnd(sTOURNAMENT_MATCH* match, BYTE byMatchIndex);
	void								FinalMatchEnd(sTOURNAMENT_MATCH* match, BYTE byMatchIndex);

	bool								FinalMatchCheckPlayerCount(sTOURNAMENT_MATCH* match, BYTE byMatchIndex);

	// ----

	void								FinalMatchFinalDirection(sTOURNAMENT_MATCH* match);
	void								FinalMatchAwarding(sTOURNAMENT_MATCH* match);

	void								AddIndividualToNextRound(sTOURNAMENT_MATCH * match, BYTE byMatchIndex, JOINID joinId, JOINID spectatorJoinId = INVALID_JOINID);

public:

	void								MinorMatchUpdatePlayerState(BYTE byState, CPlayer* pPlayer);
	void								MajorMatchUpdatePlayerState(BYTE byState, CPlayer* pPlayer);
	void								FinalMatchUpdatePlayerState(BYTE byState, CPlayer* pPlayer);

	void								LoadBudokaiStateInfo(CPlayer* pPlayer);

	void								JoinStateReq(CPlayer* pPlayer);

	void								JoinIndividualReq(CPlayer* pPlayer);
	void								LeaveIndividualReq(CPlayer* pPlayer);

	void								JoinTeamReq(CPlayer* pPlayer, WCHAR * wszTeamName);
	void								LeaveTeamReq(CPlayer* pPlayer);

	void								JoinIndividual(CHARACTERID charId, float fPoint, JOINID wJoinId, bool bDojoRecommender, WCHAR * wszCharName);
	void								LeaveIndividual(JOINID joinid);

	void								JoinTeam(CHARACTERID charId, WCHAR* wszTeamName, BYTE byMemberCount, CHARACTERID* aMembers, float fPoint, JOINID wJoinId, sBUDOKAI_TEAM_POINT_INFO* aTeamInfo);
	void								LeaveTeam(JOINID joinid);

	void								TeleportPrelimReq(CHARACTERID charId, HOBJECT handle, BYTE byServerChannelId, BYTE byServerIndex, BYTE byMatchType, JOINID joinId); //only used by budokai channel

	void								MinorMatchUpdateScore(BYTE byMatchIndex, TEAMTYPE wTeamType, HOBJECT hSlayer, HOBJECT hFainter);
	void								MajorMatchUpdateScore(BYTE byMatchIndex, TEAMTYPE wTeamType, HOBJECT hFainter, JOINID fainterJoinId);
	void								FinalMatchUpdateScore(BYTE byMatchIndex, TEAMTYPE wTeamType, HOBJECT hFainter, JOINID fainterJoinId);

	void								PlayerDisconnect(CHARACTERID charId, HOBJECT hPlayer, JOINID joinId, BYTE byMatchIndex, TEAMTYPE wTeamType);

private:

	WORD								CheckBudokaiOpen(CPlayer* pPlayer);

	bool								IsInsideArena(CNtlVector& vLoc, CWorld* pWorld);

public:

	BYTE								GetType() { return m_type; }

	BYTE								GetMatchType() { return m_matchType; }

	eBUDOKAI_MATCH_DEPTH				GetMatchDepth() { return m_eMatchDepth; }

	void								SetJoinResult(JOINID joinid, BYTE byResult);

	void								SetJoinData(JOINID joinid, BYTE byJoinState, BYTE byJoinResult);

	bool								CanTeleportPrelim(CPlayer* pPlayer);
	bool								CanTeleportMajorMatch(CPlayer* pPlayer);

private:

	void								SetIndividualTeamType(CHARACTERID charId, TEAMTYPE teamType);
	TEAMTYPE							GetIndividualTeamType(CHARACTERID charId);

	void								SetMatchIndex(JOINID joinId, BYTE byIndex);
	BYTE								GetMatchIndex(JOINID joinId);

	TEAMTYPE							DecideTeamType(BYTE nextMatchDepth, BYTE byPrelimIndex);



private:

	DWORD								m_dwTick;

	bool								m_bAdultBudokaiBegan;
	BUDOKAITIME							m_startAdultBudokaiTime;

	bool								m_bJuniorBudokaiBegan;
	BUDOKAITIME							m_startJuniorBudokaiTime;


	bool								m_bPartyAdultBudokaiBegan;
	BUDOKAITIME							m_startPartyAdultBudokaiTime;

	bool								m_bPartyJuniorBudokaiBegan;
	BUDOKAITIME							m_startPartyJuniorBudokaiTime;
	
	// ---- 

	eBUDOKAI_TYPE						m_type;

	eBUDOKAI_MATCH_TYPE					m_matchType;

	sBUDOKAI_UPDATE_STATE_INFO			m_stateInfo;

	sBUDOKAI_UPDATE_MATCH_STATE_INFO	m_matchStateInfo[MAX_BUDOKAI_MATCH_TYPE];

	BUDOKAITIME							m_tmDefaultOpenTime;

	bool								m_bRankbattleAllow;

	sBUDOKAI_TBLINFO*					m_pTableInfo;

	// ----

	std::unordered_map<JOINID, sBUDOKAI_REGISTER_INDIVIDUAL_DATA>	m_mapIndividual;

	std::unordered_map<JOINID, sBUDOKAI_REGISTER_TEAM_DATA>			m_mapTeam;

	std::map<JOINID, sBUDOKAI_JOIN_INFO>							m_mapJoinInfo;

	// ----

	BYTE								m_byDojoRecommenders;	//amount of players which go straight to tournament without participating in prelim

	std::map<BYTE, sPRELIM_HEAD*>		m_mapPrelims; //BYTE = byMinorMatch_MatchIndex

	// ----

	eBUDOKAI_MATCH_DEPTH				m_eMatchDepth;

	std::map<JOINID, sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA*>	m_mapIndividualTournament;
	std::map<JOINID, sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA*>			m_mapTeamTournament;

	sTOURNAMENT_HEAD					m_aTournamentMatch[BUDOKAI_MATCH_DEPTH_COUNT];


	// ----

	std::map<BYTE, std::pair<JOINID,CHARACTERID>>		m_mapPrelimWinners; //use map index as key to have proper sorting

	// ----

	std::map<CHARACTERID, TEAMTYPE>			m_mapTeamType;
	std::map<JOINID, BYTE>					m_mapMatchIndex;


};

#define GetBudokaiManager()				CBudokaiManager::GetInstance()
#define g_pBudokaiManager				GetBudokaiManager()

#endif