//***********************************************************************************
//
//	File		:	NtlDojo.h
//
//	Begin		:	2009-01-07
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Chung,DooSup (mailto:john@ntl-inc.com)
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlSharedType.h"
#include "NtlSharedDef.h"
#include "NtlGuild.h"
#include "NtlSharedType.h"
#include "NtlVector.h"

#pragma pack(1)

const DWORD DBO_MAX_COUNT_DOJO_IN_WORLD = 7;
const DWORD DBO_MAX_COUNT_DOJO_BANK_HISTORY = 40;
const DWORD DBO_DOJO_SCRAMBLE_REQUEST_FARE = 1000000;		// The amount of the application for the painting contest (when it is done without rain)
const DWORD DBO_DOJO_SCRAMBLE_REJECT_FARE = 150000000;		// Refusal of dojo contest (when it is done without rain)
const DWORD DBO_DOJO_SCRAMBLE_REQUIRED_REPUTATION = 100000;
const DWORD DBO_DOJO_SCRAMBLE_MAX_REJECT_COUNT = 1;		// How many times allowed to reject a war?

const DWORD DBO_DOJO_SCRAMBLE_MAX_WAIT_TICK = 10 * 60 * 1000; // Notify Leader Notification Wait 10 minutes
const DWORD DBO_DOJO_SCRAMBLE_INIT_WAIT_SEC = 30; // 30 seconds after returning to normal //old 10

const DWORD DBO_DOJO_SCRAMBLE_MAX_MEMBER = 30; // Maximum number of competitors

const DWORD	DBO_DOJO_SCRAMBLE_BROADCAST_TERM = 5 * 1000;	// Competition Score Broadcast Term
const DWORD DBO_MAX_LEVEL_DOJO_IN_WORLD = 3;

//new
const DWORD	DBO_DOJO_SCRAMBLE_PLAYER_CHECK_TICK = 30000;

enum eDOJO_STATE
{
	DOJO_STATE_CLOSE,
	DOJO_STATE_OPEN,
	DOJO_STATE_CLEAR,

	MAX_DOJO_STATE,
	INVALID_DOJO_STATE = 0xff,
};

//---------------------------------------------------------------------------------------
struct sDOJO_STATE_INFO
{
	eDOJO_STATE		eState;		// eDOJO_STATE
	DOJOTIME		tmNextStepTime;
	DOJOTIME		tmRemainTime;
};

enum eDBO_DOJO_STATUS
{
	eDBO_DOJO_STATUS_NORMAL = 0,		// Normal state without contest (other than contest) (Shows time when dojo begin)
	eDBO_DOJO_STATUS_RECEIVE,			// Waiting for a contest (BroadCast starts accepting applications) (Shows that dojo war can be requested)
	eDBO_DOJO_STATUS_REJECT_END,		// Conflicting denial termination status (denyable timeout status) (Shows nothing)
	eDBO_DOJO_STATUS_STANDBY,			// Display of guilds in participating guilds ready for contest (guild-limited notification messages for attack defense participation) (SHOWS "STANDBY: FIGHT STARTING")
	eDBO_DOJO_STATUS_INITIAL,			// Initialization of the battleground seal (Creation of a non-transferable seal in the NPC erasure dojo at the entrance of the caric dojo in the seal)		 (SHOWS NOTHING)
	eDBO_DOJO_STATUS_READY,				// Scramble (Scramble admission is possible.) (SHOWS: "Get Ready")
	eDBO_DOJO_STATUS_START,				// Start a scramble ~ (start scoring with a seal) (SHOWS: "FIGHT STARTED")
	eDBO_DOJO_STATUS_END,				// Competition complete (loser redemption winner compensation) (SHOWS: "FIGHT END")
};	


struct sDOJO_MATCHSTATE_INFO
{
	eDBO_DOJO_STATUS	eState;		// eDBO_DOJO_STATUS
	DOJOTIME			tmNextStepTime;
	DOJOTIME			tmRemainTime;
};

struct sDBO_DOJO_DATA	
{
public:
	sDBO_DOJO_DATA()
	{
		guildId = INVALID_GUILDID;
		byLevel = 0;
		dojoTblidx = INVALID_TBLIDX;		
		byPeaceStatus = 0;	
		dwPeacePoint = 0;	
		memset( wszName,0x00, sizeof(wszName) );
		challengeGuildId = INVALID_GUILDID;		
		memset( wszSeedCharName,0x00, sizeof(wszSeedCharName) );
		memset( wchLeaderName,0x00, sizeof(wchLeaderName) );
		memset( wchNotice,0x00, sizeof(wchNotice) );
	}
	GUILDID			guildId;
	BYTE			byLevel;											// Dojo level
	TBLIDX			dojoTblidx;											// Dojo table index
	BYTE			byPeaceStatus;										// Security status eDBO_DOJO_PEACE_TYPE
	DWORD			dwPeacePoint;										// Security point
	WCHAR			wszName[NTL_MAX_SIZE_GUILD_NAME + 1];	// Guild Name
	GUILDID			challengeGuildId;									// Challenge guild id
	WCHAR			wszSeedCharName[NTL_MAX_SIZE_CHAR_NAME + 1];		// Seed allocation Carrick

	WCHAR			wchLeaderName[NTL_MAX_SIZE_CHAR_NAME + 1];	// ''	Announcer
	WCHAR			wchNotice[NTL_MAX_LENGTH_OF_DOJO_NOTICE + 1];	//'' Notice	
};
struct sDOJO_SCRAMBLE_POINT
{
	TBLIDX				dojoTblidx;
	BYTE				byDefCount;		// Defenders
	BYTE				byAttCount;		// Attackers
	BYTE				byDefSealCount;	// Defenders Guild Seal Count
	BYTE				byAttSealCount; // Attackers Guild Seal Count
	DWORD				dwDefPoint;	// Goal 100
	DWORD				dwAttPoint;	// Goal 100
	DWORD				dwDefCharge;	// 300 -> 8 Point
	DWORD				dwAttCharge;	// 300 -> 8 Point
	
};
struct sDBO_DOJO_BRIEF
{
	GUILDID			guildId;
	TBLIDX			dojoTblidx;
	BYTE			byLevel;
	sDBO_GUILD_MARK sMark;
};

enum eDBO_DOJO_PEACE_TYPE
{
	eDBO_DOJO_PEACE_TYPE_PANIC = 0,
	eDBO_DOJO_PEACE_TYPE_FEAR,
	eDBO_DOJO_PEACE_TYPE_COMMON,
	eDBO_DOJO_PEACE_TYPE_QUIET,
	eDBO_DOJO_PEACE_TYPE_FESTIVAL,
};	

struct sDBO_DOJO_BANK_HISTORY_DATA
{
	sDBO_TIME		sTime;
	BYTE			byType;			// eDBO_GUILD_ZENNY_UPDATE_TYPE
	DWORD			dwAmount;		
	bool			bIsSave;		//	0: withdraw 1:save
	DWORD			dwBalance;		// balance
};

struct sDBO_DOJO_NPC_INFO
{
	TBLIDX				dojoTblidx;										// Dojo Table index
	WCHAR				wszName[NTL_MAX_SIZE_GUILD_NAME + 1];// Guild name
	BYTE				byLevel;
	DWORD				dwGuildReputation;		// Guild reputation
	DWORD				dwMaxGuildPointEver;	// Maximal dwGuildReputation ever had
	BYTE				byPeaceStatus;	
	WCHAR				wszSeedCharName[NTL_MAX_SIZE_CHAR_NAME + 1];		// Seed allocation Name
	WCHAR				wchLeaderName[NTL_MAX_SIZE_CHAR_NAME + 1];		// Leader Name
	WCHAR				wchNotice[NTL_MAX_LENGTH_OF_DOJO_NOTICE + 1];
	WCHAR				wszChallengeName[NTL_MAX_SIZE_GUILD_NAME + 1]; //challenger guild name
	WCHAR				wszChallengeLeaderName[NTL_MAX_SIZE_CHAR_NAME + 1];		// Challenger guild leader name
};

struct sDBO_DOJO_TELEPORT_DATA
{
	BYTE								byTeleportType; 
	WORD								wWaitTime;
	WORLDID								destWorldId;
	TBLIDX								destWorldTblidx;
	CNtlVector							vDestDefLoc;
	CNtlVector							vDestAttLoc;
	SERVERCHANNELID						destServerChannelId;
	SERVERINDEX							destServerIndex;
};


//--------------------------
// Ãâ·Â Condition

enum eDOJO_PROGRESS_MESSAGE
{
	DOJO_PROGRESS_MESSAGE_NORMAL,			// Before the contest
	DOJO_PROGRESS_MESSAGE_RECEIVE,			// Start a contest
	DOJO_PROGRESS_MESSAGE_REJECT_END,		// End of contest denial
	DOJO_PROGRESS_MESSAGE_STANDBY,			// Display of guilds in participating guilds ready for contest (guild-limited notification messages for attack defense participation)
	DOJO_PROGRESS_MESSAGE_INITIAL,			// Initialization of the battleground seal (Creation of a non-transferable seal in the NPC erasure paint at the entrance of the caric paint in the seal)
	DOJO_PROGRESS_MESSAGE_READY,			// Scramble (Scramble admission is possible.)
	DOJO_PROGRESS_MESSAGE_START,			// Start a scramble ~ (start scoring with a seal)
	DOJO_PROGRESS_MESSAGE_END,				// Competition complete (loser redemption winner compensation)

	INVALID_DOJO_PROGRESS_MESSAGE = 0xff,
};


enum eDBO_DOJO_REPEAT_TYPE
{
	eDBO_DOJO_REPEAT_TYPE_TIME = 0,
	eDBO_DOJO_REPEAT_TYPE_DAY,
	eDBO_DOJO_REPEAT_TYPE_WEEK,
};	

enum eDBO_DOJO_WEEK_BITFLAG
{
	eDBO_DOJO_WEEK_BITFLAG_MON = 0x01 << 0,
	eDBO_DOJO_WEEK_BITFLAG_TUE = 0x01 << 1,
	eDBO_DOJO_WEEK_BITFLAG_WED = 0x01 << 2,
	eDBO_DOJO_WEEK_BITFLAG_THU = 0x01 << 3,
	eDBO_DOJO_WEEK_BITFLAG_FRI = 0x01 << 4,
	eDBO_DOJO_WEEK_BITFLAG_SAT = 0x01 << 5,
	eDBO_DOJO_WEEK_BITFLAG_SUN = 0x01 << 6,
};

enum eDBO_DOJO_PC_TYPE
{
	eDBO_DOJO_PC_TYPE_OWNNER = 0,
	eDBO_DOJO_PC_TYPE_CHALLENGER,

	eDBO_DOJO_PC_TYPE_NONE = 0xff,
};	

enum eDBO_DOJO_REWARD_TYPE
{
	eDBO_DOJO_REWARD_TYPE_PERFECT_DEF = 0,		// Obtain maximum defense points
	eDBO_DOJO_REWARD_TYPE_WIN_DEF,				// Defense advantage
	eDBO_DOJO_REWARD_TYPE_SAME_DEF,				// Same defensive score
	
	eDBO_DOJO_REWARD_TYPE_PERFECT_ATT,			// Achieve maximum points of attack
	eDBO_DOJO_REWARD_TYPE_WIN_ATT,				// Attack dominance

	eDBO_DOJO_REWARD_TYPE_NONE = 0xff,
};	

enum eDBO_DOJO_COMMAND_TYPE
{
	eDBO_DOJO_COMMAND_TYPE_CLEAR = 0,
	eDBO_DOJO_COMMAND_TYPE_START,
	eDBO_DOJO_COMMAND_TYPE_NEXT,
	eDBO_DOJO_COMMAND_TYPE_RESPONSE,

};	
#pragma pack()