//***********************************************************************************
//
//	File		:	NtlRankBattle.h
//
//	Begin		:	2007-05-31
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlSharedType.h"
#include "NtlSharedDef.h"
#include "NtlParty.h"

//-----------------------------------------------------------------------------------
// Enumeration
//-----------------------------------------------------------------------------------

// Room State
enum eRANKBATTLE_ROOMSTATE
{
	RANKBATTLE_ROOMSTATE_NONE,			// Early
	RANKBATTLE_ROOMSTATE_WAIT,			// Waiting for opponent
	RANKBATTLE_ROOMSTATE_MATCH_READY,	// War preparation
	RANKBATTLE_ROOMSTATE_WAIT_MATCH_WORLD,	// The stadium waiting to be created
	RANKBATTLE_ROOMSTATE_MATCH_RUN,		// War begins
	RANKBATTLE_ROOMSTATE_MATCH_END,		// War of Exit -> after the state changed to WAIT again

	RANKBATTLE_ROOMSTATE_COUNT,
	INVALID_RANKBATTLE_ROOMSTATE = INVALID_BYTE,
};


// Rank Battle Mode
enum eRANKBATTLE_MODE
{
	RANKBATTLE_MODE_INDIVIDUAL, // Solo
	RANKBATTLE_MODE_PARTY, // Party

	MAX_RANKBATTLE_MODE,
	INVALID_RANKBATTLE_MODE = INVALID_BYTE
};

enum eRANKBATTLE_TEAM_TYPE
{
	RANKBATTLE_TEAM_NONE		= 0,
	RANKBATTLE_TEAM_OWNER		= 0x01 << 0,
	RANKBATTLE_TEAM_CHALLENGER	= 0x01 << 1,
	RANKBATTLE_TEAM_OTHER		= 0x01 << 2,				// If you are not a team, ex.GM

	RANKBATTLE_TEAM_ALL			= RANKBATTLE_TEAM_OWNER | RANKBATTLE_TEAM_CHALLENGER,		// Except RANKBATTLE_TEAM_OTHER
};


// status
enum eRANKBATTLE_BATTLESTATE
{
	RANKBATTLE_BATTLESTATE_NONE,			// The initial state
	RANKBATTLE_BATTLESTATE_WAIT,			// Waiting for the players
	RANKBATTLE_BATTLESTATE_DIRECTION,		// Production
	RANKBATTLE_BATTLESTATE_STAGE_PREPARE,	// Preparation Stage
	RANKBATTLE_BATTLESTATE_STAGE_READY,		// Ready
	RANKBATTLE_BATTLESTATE_STAGE_RUN,		// Progress fight
	RANKBATTLE_BATTLESTATE_STAGE_FINISH,	// Fight finished
	RANKBATTLE_BATTLESTATE_MATCH_FINISH,	// Rank Finished
	RANKBATTLE_BATTLESTATE_BOSS_DIRECTION,	// BOSS Production
	RANKBATTLE_BATTLESTATE_BOSS_READY,		// BOSS Preparations
	RANKBATTLE_BATTLESTATE_BOSS_RUN,		// BOSS Progress
	RANKBATTLE_BATTLESTATE_BOSS_FINISH,		// BOSS End
	RANKBATTLE_BATTLESTATE_END,				// End

	MAX_RANKBATTLE_BATTLESTATE,
	INVALID_RANKBATTLE_BATTLESTATE = INVALID_BYTE
};


// Member state
enum eRANKBATTLE_MEMBER_STATE
{
	RANKBATTLE_MEMBER_STATE_NONE,			// The initial state
	RANKBATTLE_MEMBER_STATE_NORMAL,			// General conditions
	RANKBATTLE_MEMBER_STATE_ATTACKABLE,		// Sets the state attack-able
	RANKBATTLE_MEMBER_STATE_FAINT,			// Death
	RANKBATTLE_MEMBER_STATE_OUTOFAREA,		// Player left arena
	RANKBATTLE_MEMBER_STATE_GIVEUP,			// Abandoned state (disconnection, etc.)
	RANKBATTLE_MEMBER_STATE_NOAPPLY,		// Although PC information does not apply to the game setting and determination.
};


// Match Result
enum eRANKBATTLE_MATCH_RESULT
{
	RANKBATTLE_MATCH_NONE,				// Exception: If everyone left the arena, etc ... a wholly-thereby.
	RANKBATTLE_MATCH_WIN_OWNER,			// Master wins
	RANKBATTLE_MATCH_WIN_CHALLENGER,	// Challenger wins
	RANKBATTLE_MATCH_DRAW,				// Draw

	INVALID_RANKBATTLE_MATCH = INVALID_BYTE,
};



// Update used to query servers in the RankBattle Score
enum eRANKBATTLE_QUERY_UPDATE_TYPE
{
	RANKBATTLE_QUERY_UPDATE_NONE,		// Stay put, (not updated).
	RANKBATTLE_QUERY_UPDATE_ADD,		// And one additional
	RANKBATTLE_QUERY_UPDATE_INIT,		// It initializes to zero.
};


//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//	Constant Defined
//-----------------------------------------------------------------------------------
const DWORD		DBO_RANKBATTLE_MAX_ROOM_COUNT = 100;			// Battle rank maepdang room maximum number

const DWORD		DBO_RANKBATTLE_MAX_ROOM_COUNT_PER_PAGE = 10;	// Maximum number of rooms per page to print (server transfer)

const DWORD		DBO_RANKBATTLE_USER_WAIT_TIME = 120000;			// User standby 2 minutes

const DWORD		DBO_RANKBATTLE_NAME_LENGTH = 128;				// RankBattle name max size

const BYTE		DBO_RANKBATTLE_PARTY_MEMBER_RESTRICTION = 1;	// Party personnel limited

const DWORD		DBO_RANKBATTLE_ROOM_READY_TIME = 10000;			// Start a standby time of up to war after war

const DWORD		DBO_RANKBATTLE_ENDSTAGE_WAIT_TIME = 5000;		// End Stage In the waiting time

const BYTE		DBO_RANKBATTLE_MAX_RANK_BOARD_RECORD_COUNT = 10;	// The maximum record count is output to the Forum Rank

const BYTE		DBO_RANKBATTLE_MAX_COMPARE_DAY = 7;					// The maximum number of days in the comparable Rank Bulletin

const DWORD		DBO_RANKBATTLE_RANK_REQUEST_TERM = 500;			// You may request a list Rank every two seconds.

const BYTE		DBO_RANKBATTLE_RANK_BACKUP_TIME = 6;			// Backup time for a Rank 0 ~ 23, should be updated with the schedule of NB DB. *

const WORD		DBO_RANKBATTLE_MAX_ARENA_TYPE = 10;				// The maximum number of solo World Arena, each party before 10

const DWORD		DBO_RANKBATTLE_MATCH_START_TERM = 60;			// Economic cycle begins

const BYTE		DBO_RANKBATTLE_MAX_MATCHING_LEVEL = 3;			// The maximum difference in level between the PC and each Party to start the game old = 1 / new = 3

const DWORD		DBO_RANKBATTLE_MAX_BGM_NAME = 40;

const DWORD		DBO_RANKBATTLE_MAX_REGISTER = 10;


//-----------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------
//	Data structure definitions
//-----------------------------------------------------------------------------------
#pragma pack(1)

struct sRANKBATTLE_ARENA_INFO
{
	TBLIDX		rankBattleTblidx;
	WORD		wRegisterCount;
};



// Solo room information
struct sRANKBATTLE_ROOMLIST_INDIVIDUAL
{
	ROOMID	roomId;
	WCHAR	wszOwnerPc[NTL_MAX_SIZE_CHAR_NAME + 1];	// Owner
	WORD	wWinCounter;
};



// Party room information
struct sRANKBATTLE_ROOMLIST_PARTY
{
	ROOMID	roomId;
	WCHAR	wszOwnerParty[NTL_MAX_SIZE_PARTY_NAME + 1];	// Owner
	WORD	wWinCounter;
};


//--------------------------------------------------------------------------------
// Room Details

// More room information used for the transport of the Member
struct sRANKBATTLE_ROOMINFO_MEMBER
{
	HOBJECT hPc;
};


// Room details Private room information used to transfer
struct sRANKBATTLE_ROOMINFO_INDIVIDUAL
{
	TBLIDX						rankBattleTblidx;	// RankBattle Tblidx
	ROOMID						roomId;
	sRANKBATTLE_ROOMINFO_MEMBER	sMember;			// Member
};


// Party room details Room information that is used to transfer
struct sRANKBATTLE_ROOMINFO_PARTY
{
	TBLIDX						rankBattleTblidx;	// RankBattle Tblidx
	ROOMID						roomId;
	BYTE						byMemberCount;		// RoomÀÇ MemberCount
	sRANKBATTLE_ROOMINFO_MEMBER	asMember[NTL_MAX_MEMBER_IN_PARTY];		// Member
	HOBJECT						hObject;
};


// 
union uRANKBATTLE_ROOMINFO
{
	sRANKBATTLE_ROOMINFO_INDIVIDUAL	sIndividual;
	sRANKBATTLE_ROOMINFO_PARTY		sParty;
};


//-------------------------------------------------------------------------------


// Rooms used to send the list to the client
union uRANKBATTLE_ROOMINFO_LIST
{
	// The size of the array: the actual data transmission is sent as much by Room Count.
	sRANKBATTLE_ROOMLIST_INDIVIDUAL	asIndividualRoom[DBO_RANKBATTLE_MAX_ROOM_COUNT_PER_PAGE];	// byRoomMode == RANKBATTLE_MODE_INDIVIDUAL
	sRANKBATTLE_ROOMLIST_PARTY		asPartyRoom[DBO_RANKBATTLE_MAX_ROOM_COUNT_PER_PAGE];	// byRoomMode == RANKBATTLE_MODE_PARTY
};

struct sRANKBATTLE_SCORE_INFO
{
	sRANKBATTLE_SCORE_INFO::sRANKBATTLE_SCORE_INFO()
	{
		wMaxStraightWin = 0;
		wStraightWin = 0;
		wMaxStraightKOWin = 0;
		wStraightKOWin = 0;
		dwWin = 0;
		dwDraw = 0;
		dwLose = 0;
		fPoint = 0.0f;
	}

	WORD						wMaxStraightWin;	// Highest winning streak
	WORD						wStraightWin;		// Current winning streak
	WORD						wMaxStraightKOWin;	// Most consecutive KO Count
	WORD						wStraightKOWin;		// KO wins count
	DWORD						dwWin;				// Victory Count
	DWORD						dwDraw;				// Draw Count
	DWORD						dwLose;				// Lose Count
	float						fPoint;				// Point
};


// 
struct sRANKBATTLE_MEMBER_RESULT
{
	HOBJECT	hPc;
	WORD	wResultCode;
};

// Information on their PC to participate Rank Battle
struct sRANKBATTLE_MATCH_MEMBER_INFO
{
	HOBJECT hPc;
	BYTE	byTeam;		// eRANKBATTLE_TEAM_TYPE
	BYTE	byState;	// eRANKBATTLE_MEMBER_STATE
};



// Score: [Battle Score - the result of the match, Stage Score - the results of the Stage] is used for.
struct sRANKBATTLE_SCORE
{
	BYTE	byOwner;
	BYTE	byChallenger;
};



// Point : BattlePoint
struct sRANKBATTLE_POINT
{
	float					fOwner;
	float					fChallenger;
};


// Results
struct sRANKBATTLE_MATCH_FINISH_INFO
{
	BYTE				byMatchWinner;
	bool				bPerfectWin;
	sRANKBATTLE_SCORE	sScore;
	float				fRankPoint;
	DWORD				dwMudosaPoint;
	BYTE				byMatchRating;
};


// 
struct sRANKBATTLE_RANK_DATA
{
	WCHAR				wszCharName[NTL_MAX_SIZE_CHAR_NAME];
	BYTE				byLevel;
	BYTE				byClass;
	WCHAR				wszGuildName[NTL_MAX_SIZE_GUILD_NAME];
	int					nPoint;
	WORD				wStraightWinCount;
	DWORD				adwCompareRank[DBO_RANKBATTLE_MAX_COMPARE_DAY];
};

struct sRANKBATTLE_RANK_INFO
{
	WORD				wCharName;				// WCHAR [NTL_MAX_SIZE_CHAR_NAME]
	BYTE				byLevel;
	BYTE				byClass;
	WORD				wGuildName;				// WCHAR [NTL_MAX_SIZE_GUILD_NAME]
	int					nPoint;
	WORD				wStraightWinCount;
	DWORD				dwCompareRank;
};


float CalculateMileageReward(BYTE byWinnerLevel, BYTE byLoserLevel, BYTE byWins, BYTE byLoses, BYTE byMemberCount, BYTE byEnemyCount);

#pragma pack()
//-------------------------------------------------------------------------------------