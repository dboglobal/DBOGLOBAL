//***********************************************************************************
//
//	File		:	NtlBudokai.h
//
//	Begin		:	2008-04-22
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Ju-hyung Lee
//
//	Desc		:	천하제일무도회
//
//***********************************************************************************

#pragma once

#include "NtlParty.h"

// Find out the next cycle of budokai type.
BUDOKAITIME CalcBudokaiNextStartTime( BUDOKAITIME tmDefaultTime, BYTE byOpenTerm, BYTE byOpenDayOfWeek, BYTE byOpenHour, BYTE byOpenMinute );


// budokai type
enum eBUDOKAI_TYPE
{
	BUDOKAI_TYPE_JUNIOR,	// junior
	BUDOKAI_TYPE_ADULT,		// adult

	INVALID_BUDOKAI_TYPE = 0xff
};


// budokai match type
enum eBUDOKAI_MATCH_TYPE
{
	BUDOKAI_MATCH_TYPE_INDIVIDIAUL,
	BUDOKAI_MATCH_TYPE_TEAM,

	MAX_BUDOKAI_MATCH_TYPE,
	INVALID_BUDOKAI_MATCH_TYPE = 0xff,
};


// Icon Notification Type
enum eBUDOKAI_NOTICE_TYPE
{
	BUDOKAI_NOTICE_SYSTEM,					// Server sends table index
	BUDOKAI_NOTICE_GM,						// Server sends string
	BUDOKAI_NOTICE_ONLY_CLIENT,				// Client self-processing

	INVALID_BUDOKAI_NOTICE_TYPE = 0xff,
};


//
enum eBUDOKAI_STATE
{
	// junior
	BUDOKAI_STATE_JUNIOR_CLOSE,
	BUDOKAI_STATE_JUNIOR_OPEN_NOTICE,	// Start notice
	BUDOKAI_STATE_JUNIOR_DOJO_RECOMMEND,// dojo recommendation
	BUDOKAI_STATE_JUNIOR_OPEN,			// people can start register
	BUDOKAI_STATE_JUNIOR_CLEAR,

	// adult
	BUDOKAI_STATE_CLOSE,
	BUDOKAI_STATE_OPEN_NOTICE,		// Start notice
	BUDOKAI_STATE_DOJO_RECOMMEND,	// dojo recommendation
	BUDOKAI_STATE_OPEN,				//
	BUDOKAI_STATE_CLEAR,

	MAX_BUDOKAI_STATE,
	BUDOKAI_STATE_JUNIOR_FIRST = BUDOKAI_STATE_JUNIOR_CLOSE,
	BUDOKAI_STATE_JUNIOR_LAST = BUDOKAI_STATE_JUNIOR_CLEAR,
	BUDOKAI_STATE_ADULT_FIRST = BUDOKAI_STATE_CLOSE,
	BUDOKAI_STATE_ADULT_LAST = BUDOKAI_STATE_CLEAR,
	BUDOKAI_STATE_FIRST = BUDOKAI_STATE_JUNIOR_CLOSE,
	BUDOKAI_STATE_LAST = BUDOKAI_STATE_CLEAR,
	INVALID_BUDOKAI_STATE = 0xff,
};

// eBUDOKAI_STATE You need to change the following functions as well.
eBUDOKAI_TYPE GetBudokaiType( BYTE byState );	// eBUDOKAI_STATE

// is open
bool IsBudokaiOpen( BYTE byState );


//
enum eBUDOKAI_MATCHSTATE
{
	BUDOKAI_MATCHSTATE_REGISTER,				// Registration period
	BUDOKAI_MATCHSTATE_WAIT_MINOR_MATCH,		// Waiting for qualification
	BUDOKAI_MATCHSTATE_MINOR_MATCH,				// Preliminaries 
	BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_32,
	BUDOKAI_MATCHSTATE_MAJORMATCH_32,
	BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_16,
	BUDOKAI_MATCHSTATE_MAJORMATCH_16,
	BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_08,
	BUDOKAI_MATCHSTATE_MAJORMATCH_08,
	BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH,	// Semi-waiting
	BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH,			// semifinal
	BUDOKAI_MATCHSTATE_WAIT_FINAL_MATCH,		// Final Standby: Not used
	BUDOKAI_MATCHSTATE_FINAL_MATCH,				// finals
	BUDOKAI_MATCHSTATE_MATCH_END,				//End. Grace time

	MAX_BUDOKAI_MATCHSTATE,
	INVALID_BUDOKAI_MATCHSTATE = 0xff,
};


// Player's Participation Status
enum eBUDOKAI_JOIN_STATE
{
	BUDOKAI_JOIN_STATE_PLAY = 0,		// Participating - It should not be changed since it is the default value that matches DB.
	BUDOKAI_JOIN_STATE_DROPOUT,			// It is a value that matches with DB - DB should not be changed.

	INVALID_BUDOKAI_JOIN_STATE = 0xff,	// Not participating
};


// Player's participation result
enum eBUDOKAI_JOIN_RESULT
{
	BUDOKAI_JOIN_RESULT_REGISTER = 0,	// Registering - It should not be changed since it is initial value adjusted with DB.
	BUDOKAI_JOIN_RESULT_MINORMATCH,		// Preliminary advance - It is a value matched with DB and should not be changed.
	BUDOKAI_JOIN_RESULT_ENTER_32,
	BUDOKAI_JOIN_RESULT_ENTER_16,
	BUDOKAI_JOIN_RESULT_ENTER_8,
	BUDOKAI_JOIN_RESULT_ENTER_4,
	BUDOKAI_JOIN_RESULT_ENTER_2,
	BUDOKAI_JOIN_RESULT_WINNER,

	INVALID_BUDOKAI_JOIN_RESULT = 0xff
};


enum eBUDOKAI_MATCH_DEPTH
{
	BUDOKAI_MATCH_DEPTH_2 = 0,	// 1	2^0
	BUDOKAI_MATCH_DEPTH_4,		// 2	2^1
	BUDOKAI_MATCH_DEPTH_8,		// 4	2^2
	BUDOKAI_MATCH_DEPTH_16,		// 8	2^3
	BUDOKAI_MATCH_DEPTH_32,		// 16	2^4

	BUDOKAI_MATCH_DEPTH_COUNT,
	MAX_BUDOKAI_MATCH_DEPTH_INDIVIDUAL = BUDOKAI_MATCH_DEPTH_32,
	MAX_BUDOKAI_MATCH_DEPTH_TEAM = BUDOKAI_MATCH_DEPTH_16,
	INVALID_BUDOKAI_MATCH_DEPTH = 0xff,
};


//--------------------------
// Budokai Condition

enum eBUDOKAI_PROGRESS_MESSAGE
{
	BUDOKAI_PROGRESS_MESSAGE_START = 0,					// The budokai will begin. News ....

	// 예선
	BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_ENTERING,		// Qualification starts to enter. Start your position ....
	BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_BEFORE_1MIN,	// The preliminary battle will begin soon. Standing ad ...
	BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_WINNER,			// You have decided to advance to the 32nd ...
	BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_LOSER,			// You failed to to advance to the 32nd ...
	BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_ALL_FINISHED,	//All qualifiers have been finished. Main Line Gene ....

	// 본선
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_32_BEFORE_10SEC,		// After 10 seconds, you will automatically enter the 32st Stadium.
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_32_WINNER,			// OOO님의 32강.... 축하합니다. 16강진출이.
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_32_LOSER,			// OOO님의 32강.... 안타깝게도, 16강에 준..
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_32_ALL_FINISHED,		// All the 32 rounds are finished.

	BUDOKAI_PROGRESS_MESSAGE_DEPTH_16_BEFORE_10SEC,		// After 10 seconds, it automatically enters the 16th Stadium.
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_16_WINNER,			// OOO님의 16강.... 축하합니다. 16강진출이.
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_16_LOSER,			// OOO님의 16강.... 안타깝게도, 16강에 준..
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_16_ALL_FINISHED,		// 16th round ends ... Semi finalists

	BUDOKAI_PROGRESS_MESSAGE_DEPTH_08_BEFORE_10SEC,		// After 10 seconds, it will automatically enter the 8th Stadium.
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_08_WINNER,			// OOO님의 8강.... 축하합니다. 8강진출이.
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_08_LOSER,			// OOO님의 8강.... 안타깝게도, 8강에 준..
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_08_ALL_FINISHED,		// 8th round ends ... Semi finalists

	//
	BUDOKAI_PROGRESS_MESSAGE_SEMI_FINAL_BEFORE_10SEC,
	BUDOkAI_PROGRESS_MESSAGE_SEMI_FINAL_RUNNING,
	BUDOkAI_PROGRESS_MESSAGE_FINAL_WAIT_LOSER,

	BUDOkAI_PROGRESS_MESSAGE_END,

	INVALID_BUDOKAI_PROGRESS_MESSAGE = 0xff,
};


//------------------------------------------------------------------------------------
// define

//-----------------------------------------------
// [8/4/2008 SGpro]
#define INVALID_MUDOSAPOINT INVALID_DWORD 

#define MAX_MUDOSA_POINT				(500000)

#define BUDOKAI_SERVER_INDEX			9


#define BUDOKAI_MAX_NOTICE_LENGTH		255


// Game <-> Query The maximum number of packets that can fit in a packet
#define BUDOKAI_MAX_INDIVIDUAL_DB_DATA_PACKET_COUNT	20
#define BUDOKAI_MAX_TEAM_DB_DATA_PACKET_COUNT		(int)(BUDOKAI_MAX_INDIVIDUAL_DB_DATA_PACKET_COUNT / NTL_MAX_MEMBER_IN_PARTY)


// Protocol related
#define	BUDOKAI_MAX_TOURNAMENT_INDIVIDUAL_ENTRY_PACKET_COUNT	(NTL_MAX_MEMBER_IN_PARTY * 4)
#define	BUDOKAI_MAX_TOURNAMENT_TEAM_ENTRY_PACKET_COUNT			4



//-----------------------------------------------
// Rule define

// Qualifying rounds, Main Line
#define	BUDOKAI_MAX_INDIVIDUAL_MATCH_COUNT			32
#define	BUDOKAI_MAX_TEAM_MATCH_COUNT				16

#if BUDOKAI_MAX_INDIVIDUAL_MATCH_COUNT > BUDOKAI_MAX_TEAM_MATCH_COUNT
#define BUDOKAI_MAX_MATCH_COUNT		BUDOKAI_MAX_INDIVIDUAL_MATCH_COUNT
#else
#define BUDOKAI_MAX_MATCH_COUNT		BUDOKAI_MAX_TEAM_MATCH_COUNT
#endif


// Maximum number of qualifiers and team members
#define BUDOKAI_MAX_INDIVIDUAL_REGISTER_COUNT		1000
#define BUDOKAI_MAX_TEAM_REGISTER_COUNT				200

// Minimum, maximum number of team members
#define BUDOKAI_MIN_TEAM_MEMBER_COUNT	NTL_MAX_MEMBER_IN_PARTY
#define BUDOKAI_MAX_TEAM_MEMBER_COUNT	NTL_MAX_MEMBER_IN_PARTY;


// Personnel and teams per qualification
#define BUDOKAI_MINOR_MATCH_INDIVIDUAL_COUNT		15
#define BUDOKAI_MINOR_MATCH_TEAM_COUNT				5

#if BUDOKAI_MINOR_MATCH_INDIVIDUAL_COUNT > BUDOKAI_MINOR_MATCH_TEAM_COUNT
#define BUDOKAI_MAX_MINORMATCH_TEAM_COUNT		BUDOKAI_MINOR_MATCH_INDIVIDUAL_COUNT
#else
#define BUDOKAI_MAX_MINORMATCH_TEAM_COUNT		BUDOKAI_MINOR_MATCH_TEAM_COUNT
#endif


// Total Qualifying Selection
#define BUDOKAI_MINOR_MATCH_MAX_INDIVIDUAL_COUNT	(BUDOKAI_MINOR_MATCH_INDIVIDUAL_COUNT * BUDOKAI_MAX_INDIVIDUAL_MATCH_COUNT)
#define BUDOKAI_MINOR_MATCH_MAX_TEAM_COUNT			(BUDOKAI_MINOR_MATCH_TEAM_COUNT * BUDOKAI_MAX_TEAM_MATCH_COUNT)
#define BUDOKAI_MINOR_MATCH_MAX_TEAM_MEMBER_COUNT	(BUDOKAI_MINOR_MATCH_MAX_TEAM_COUNT * NTL_MAX_MEMBER_IN_PARTY)


// Number of individuals who will enter the Individual Finals: 
// Change QG_BUDOKAI_INDIVIDUAL_LIST_RES Check that the size of the packet does not exceed.
#define BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER			(BUDOKAI_MAX_INDIVIDUAL_MATCH_COUNT)


// 팀전 본선 진출 인원 : 변경할 경우 QG_BUDOKAI_TEAM_LIST_RES 패킷의 크기가 넘어가지 않는지 체크한다.
#define BUDOKAI_MAJOR_MATCH_TEAM_NUMBER				(BUDOKAI_MAX_TEAM_MATCH_COUNT)


// Automatic resurrection time (sec)
#define BUDOKAI_MINORMATCH_RESCUE_TIME				10

// STAGE SELECTION state Time
#define BUDOKAI_MINORMATCH_SELECTION_STATE_TIME		20

// Size of Random Number on Stage Selection
#define BUDOKAI_MINORMATCH_SELECTION_NUMBER			100


//-----------------------------------------------
// Mudosa

// Maximum number of masters information
#define BUDOKAI_MAX_MUDOSA_INFO_COUNT				5

// Maximum Capacity
#define BUDOKAI_MAX_MUDOSA_PLAYER_COUNT				200

// Maximum Length : DBO_MAX_LENGTH_WORLD_NAME_IN_UNICODE/
#define BUDOKAI_MAX_MUDOSA_NAME_IN_UNICODE			32




// 최대 토너먼트 개인전 대전 횟수
#define BUDOKAI_MAX_TOURNAMENT_INDIVIDUAL_MATCH_COUNT	(BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER - 1)

// 최대 토너먼트 팀 대전 횟수
#define BUDOKAI_MAX_TOURNAMENT_TEAM_MATCH_COUNT			(BUDOKAI_MAJOR_MATCH_TEAM_NUMBER - 1)


// FinalMatch Max Team Count
#define BUDOKAI_MAX_FINALMATCH_TEAM_COUNT			4



// DB 쿼리 관련
// 토너먼트 개인전 결과 최대 레코드 카운트
#define BUDOKAI_MAX_TOURNAMENT_INDIVIDUAL_MATCH_RECORD_COUNT	(BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER * 2)
// 토너먼트 팀전 결과 최대 레코드 카운트
#define BUDOKAI_MAX_TOURNAMENT_TEAM_MATCH_RECORD_COUNT			(BUDOKAI_MAJOR_MATCH_TEAM_NUMBER * 2)



//------------------------------------------------------------------------------------
// history 관련

// 개인전 입상자 카운트 : 결선 4명
#define	BUDOKAI_MAX_SEASON_HISTORY_INDIVIAUL_WINNER_COUNT		(4)
// 팀전 입상자 카운트 : 결선 4개팀 20명
#define	BUDOKAI_MAX_SEASON_HISTORY_TEAM_WINNER_COUNT			(NTL_MAX_MEMBER_IN_PARTY * 4)

// 개인전 및 팀전 입상자 카운트
#define	BUDOKAI_MAX_SEASON_HISTORY_WINNER_PLAYER_COUNT			(BUDOKAI_MAX_SEASON_HISTORY_INDIVIAUL_WINNER_COUNT + BUDOKAI_MAX_SEASON_HISTORY_TEAM_WINNER_COUNT)


// join state를 보낼때 한패킷에 넣을수 있는 최대 갯수
const BYTE BUDOkAI_MAX_PLAYER_JOIN_STATE_DATA_COUNT = 100;


//------------------------------------------------------------------------------------


#pragma pack(1)


// 상태 정보 : DB 저장용
struct sBUDOKAI_STATE_DATA
{
	BYTE		byState;		// eBUDOKAI_STATE or eBUDOKAI_MATCHSTATE
	BUDOKAITIME	tmNextStepTime;
};

#define sBUDOKAI_MATCHSTATE_DATA sBUDOKAI_STATE_DATA	


struct sBUDOKAI_UPDATEDB_INITIALIZE
{
	WORD						wSeasonCount;				// How many times is the budokai in the world?
	BUDOKAITIME					tmDefaultOpenTime;			// Default start time

	bool						bRankPointInitialized;

	sBUDOKAI_STATE_DATA			sStateData;
	sBUDOKAI_MATCHSTATE_DATA	sIndividualStateData;
	sBUDOKAI_MATCHSTATE_DATA	sTeamStateData;
};

//-----------------------------------------------------------------------------------


// Status information: Enabled
struct sBUDOKAI_STATE_INFO
{
	eBUDOKAI_STATE	eState;		// eBUDOKAI_STATE
	BUDOKAITIME		tmNextStepTime;
//	BUDOKAITIME		tmRemainTime;
};


//
struct sBUDOKAI_MATCHSTATE_INFO
{
	eBUDOKAI_MATCHSTATE	eState;		// eBUDOKAI_MATCHSTATE
	BUDOKAITIME			tmNextStepTime;
//	BUDOKAITIME			tmRemainTime;
};


//---------------------------------------------------------------------------------------

// Personal information information of player who applied for the budokai
struct sBUDOKAI_PLAYER_DATA
{
//	sBUDOKAI_PLAYER_DATA() : charId(INVALID_CHARACTERID), byClass(INVALID_BYTE), byLevel(INVALID_BYTE), fPoint(0)
//	{
//		memset( wszName, 0x00, sizeof(wszName));
//	}

	CHARACTERID	charId;
	BYTE		byClass;
	BYTE		byLevel;
	float		fPoint;
	WCHAR		wszName[NTL_MAX_SIZE_CHAR_NAME + 1];
};



//---------------------------------------------------------------------------------------
// List of qualifiers

//
struct sBUDOKAI_REGISTER_DATA
{
	JOINID			wJoinId;
	WORD			wRanking;
	BYTE			byMinorMatch_MatchIndex;
	float			fPoint;
};

// DB's private data: If changed, make sure that QG_BUDOKAI_INDIVIDUAL_LIST_DATA does not exceed the size of the packet.
struct sBUDOKAI_REGISTER_INDIVIDUAL_DATA : public sBUDOKAI_REGISTER_DATA
{
	CHARACTERID		charId;
	bool			bDojoRecommender;
};

// DB's private data: Check if the size of QG_BUDOKAI_TEAM_LIST_RES packet is not exceeded when changing.
struct sBUDOKAI_REGISTER_TEAM_DATA : public sBUDOKAI_REGISTER_DATA
{
	WCHAR					wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
	CHARACTERID				aMembers[NTL_MAX_MEMBER_IN_PARTY];
};


//---------------------------------------------------------------------------------------
// List of finalists

struct sBUDOKAI_TOURNAMENT_ENTRY_DATA
{
	JOINID			wJoinId;
	BYTE			byMinorMatch_MatchIndex;
};


struct sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA : public sBUDOKAI_TOURNAMENT_ENTRY_DATA
{
	CHARACTERID		charId;
	WCHAR			wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	BYTE			byClass;
	BYTE			byLevel;
	float			fRankPoint;
	bool			bDojoRecommender;
	WORD			wRankBattleWinCount;
	WORD			wRankBattleDrawCount;
	WORD			wRankBattleLoseCount;
	WCHAR			wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
};


struct sBUDOKAI_TOURNAMENT_TEAM_ENTRY_MEMBER_DATA
{
	CHARACTERID		charId;
	WCHAR			wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	BYTE			byClass;
	BYTE			byLevel;
	float			fRankPoint;
};

struct sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA : public sBUDOKAI_TOURNAMENT_ENTRY_DATA
{
	WCHAR			wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
	BYTE			byMemberCount;
	sBUDOKAI_TOURNAMENT_TEAM_ENTRY_MEMBER_DATA	aMembers[NTL_MAX_MEMBER_IN_PARTY];
};


// variable data ----

struct sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA_VAR : sBUDOKAI_TOURNAMENT_ENTRY_DATA
{
	CHARACTERID		charId;
	WORD			wCharName;				//WCHAR			wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	BYTE			byClass;
	BYTE			byLevel;
	float			fRankPoint;
	bool			bDojoRecommender;		// dojo recommended
	WORD			wRankBattleWinCount;
	WORD			wRankBattleDrawCount;
	WORD			wRankBattleLoseCount;
	WORD			wGuildName;				//WCHAR			wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
};


struct sBUDOKAI_TOURNAMENT_TEAM_ENTRY_MEMBER_DATA_VAR
{
	CHARACTERID		charId;
	WORD			wCharName;				//WCHAR			wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	BYTE			byClass;
	BYTE			byLevel;
	float			fRankPoint;
};

struct sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA_VAR : public sBUDOKAI_TOURNAMENT_ENTRY_DATA
{
	WORD			wTeamName;				//WCHAR			wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
	BYTE			byMemberCount;
	WORD			wMembers;				//sBUDOKAI_TOURNAMENT_TEAM_ENTRY_MEMBER_DATA	aMembers[NTL_MAX_MEMBER_IN_PARTY];
};


//---------------------------------------------------------------------------------------
// Game Server <-> Client



// Request information from the application team when applying for a team
struct sBUDOKAI_TEAM_POINT_INFO
{
	BYTE		byClass;
	WCHAR		wszName[NTL_MAX_SIZE_CHAR_NAME + 1];
	BYTE		byLevel;
	float		fPoint;
};


//
struct sBUDOKAI_REGISTER_INDIVIDUAL_INFO
{
	bool			bDojoRecommender;	// Acknowledgment
	JOINID			wJoinId;
	WCHAR			wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
};


//
struct sBUDOKAI_REGISTER_TEAM_INFO
{
	JOINID			wJoinId;
	WCHAR			wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];

	sBUDOKAI_TEAM_POINT_INFO	aMemberInfo[NTL_MAX_MEMBER_IN_PARTY];
};


// Personal information sent to client upon connection
struct sBUDOKAI_JOIN_INFO
{
	BYTE					byMatchType;		// eBUDOKAI_MATCH_TYPE
	BYTE					byJoinState;		// eBUDOKAI_JOIN_STATE
	BYTE					byJoinResult;		// eBUDOKAI_JOIN_RESULT

	union
	{
		sBUDOKAI_REGISTER_INDIVIDUAL_INFO	sIndividualInfo;
		sBUDOKAI_REGISTER_TEAM_INFO			sTeamInfo;
	};
};



//-----------------------
// List of finalists


struct sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO
{
	JOINID	wJoinId;
	bool	bDojoRecommender;
	WCHAR	wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
};


struct sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO_VAR
{
	JOINID	wJoinId;
	bool	bDojoRecommender;
	WORD	wTeamName;		// WCHAR	wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
};



//-----------------------

struct sBUDOKAI_TOURNAMENT_MATCH_DATA
{
	BYTE		byDepth;
	BYTE		byMatchNumber;
	BYTE		byMatchResult;		// eMATCH_RESULT
	JOINID		wWinnerTeam;		// Winner Team JoinID
	JOINID		wJoinId1;
	JOINID		wJoinId2;
	BYTE		byScore1;
	BYTE		byScore2;
};


//---------------------------------------------------------------------------------------
struct sBUDOKAI_UPDATE_STATE_INFO
{
	BYTE			byState;		// eBUDOKAI_STATE
	BUDOKAITIME		tmNextStepTime;
	BUDOKAITIME		tmRemainTime;
	WORD			wSeasonCount;	//budokai counter. 0 = first budokai. 10 = 11th budokai
};

struct sBUDOKAI_UPDATE_MATCH_STATE_INFO
{
	BYTE			byState;		// eBUDOKAI_MATCHSTATE
	BUDOKAITIME		tmNextStepTime;
	BUDOKAITIME		tmRemainTime;
};


//---------------------------------------------------------------------------------------

struct sBUDOKAI_MUDOSA_INFO
{
	WCHAR			wszMudosaName[BUDOKAI_MAX_MUDOSA_NAME_IN_UNICODE + 1];
	WORD			wCurrentUserCount;
};


// Information of the player of the previous season
struct sBUDOKAI_PREV_SEASON_PLAYER_INFO
{
	BYTE			byMatchType;		// eBUDOKAI_MATCH_TYPE
	CHARACTERID		charId;
	WCHAR			wszName[NTL_MAX_SIZE_CHAR_NAME + 1];
	BYTE			byClass;
	BYTE			byLevel;
	BYTE			byJoinResult;		// eBUDOKAI_JOIN_RESULT
};


//---------------------------------------------------------------------------------------
// Information about the contestants who are notified by the server

// Preliminary Individual Exhibition Teleport Information
struct sMINORMATCH_TELEPORT_INDIVIDUAL_DATA
{
	WORLDID		worldId;
	TBLIDX		worldTblidx;
	BYTE		byPlayerCount;
	CHARACTERID	aPlayers[BUDOKAI_MINOR_MATCH_INDIVIDUAL_COUNT];
};


// Preliminary Match Team Teleport Information
struct sMINORMATCH_TELEPORT_TEAM_MEMBER_LIST
{
	BYTE		byCount;
	CHARACTERID	charId[NTL_MAX_MEMBER_IN_PARTY];
};

struct sMINORMATCH_TELEPORT_TEAM_DATA
{
	WORLDID									worldId;
	TBLIDX									worldTblidx;
	BYTE									byTeamCount;
	sMINORMATCH_TELEPORT_TEAM_MEMBER_LIST	aTeamList[BUDOKAI_MINOR_MATCH_TEAM_COUNT];
};



enum eBUDOKAI_GM_MATCH_PROGRESS_STATE
{
	BUDOKAI_GM_MATCH_PROGRESS_STATE_READY,		// Preparing default
	BUDOKAI_GM_MATCH_PROGRESS_STATE_RUN,		// Proceeding
	BUDOKAI_GM_MATCH_PROGRESS_STATE_FINISHED,	// complete
};


// GM information output
struct sBUDOKAI_GM_MATCH_PROGRESS_STATE
{
	BYTE		byMatchIndex;
	BYTE		byProgressState;		// eBUDOKAI_GM_MATCH_PROGRESS_STATE

	BYTE		byMatchResult;			// eMATCH_RESULT
	TEAMTYPE	winnerTeamType;			// eMATCH_TEAM_TYPE

	// Team1
	WCHAR		wszTeamName1[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
	BYTE		byScore1;

	// Team2
	WCHAR		wszTeamName2[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
	BYTE		byScore2;
};


// Preliminary match winner lottery data
struct sMINORMATCH_SELECTION_DATA
{
	TEAMTYPE	teamType;
	BYTE		byNumber;
};


struct sBUDOKAI_JOIN_STATE_DATA
{
	CHARACTERID				charId;
	JOINID					joinId;
	BYTE					byJoinState;	// eBUDOKAI_JOIN_STATE
	BYTE					byJoinResult;	// eBUDOKAI_JOIN_RESULT
};

#pragma pack()

