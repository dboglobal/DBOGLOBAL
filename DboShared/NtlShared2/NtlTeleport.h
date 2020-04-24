#pragma once

#include "NtlVector.h"
#include "NtlSharedDef.h"

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#define NTL_MAX_BIND_RADIUS		(8.0f)
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

// Teleport type
enum eTELEPORT_TYPE
{
	TELEPORT_TYPE_DEFAULT,				// General situation
	TELEPORT_TYPE_GAME_IN,				// When you first enter the game
	TELEPORT_TYPE_GAME_OUT,				// If you leave the game
	TELEPORT_TYPE_TELEPOPO,				// When using a teleoppo item
	TELEPORT_TYPE_SKILL,				// When using skill such as instant movement
	TELEPORT_TYPE_NPC_PORTAL,			// Using NPC portal service
	TELEPORT_TYPE_POPOSTONE,			// When resurrected and summoned to Stone of the Popo
	TELEPORT_TYPE_WORLD_MOVE,			// If you move from one world to another
	TELEPORT_TYPE_TMQ_WORLD_MOVE,		// Entering or leaving the TMQ region
	TELEPORT_TYPE_TMQ_PORTAL,			// Teleported within the TMQ region

	TELEPORT_TYPE_RANKBATTLE,
	TELEPORT_TYPE_TIMEQUEST,
	TELEPORT_TYPE_TUTORIAL,
	TELEPORT_TYPE_BUDOKAI,				// Tianhe First Ball
	TELEPORT_TYPE_MATCH,				// National Ballroom Competition
	TELEPORT_TYPE_MINORMATCH,			// Tianhe First Ball Promotion Teleport
	TELEPORT_TYPE_MAJORMATCH,			// Tianhe First Ball Festival Teleport
	TELEPORT_TYPE_FINALMATCH,			// The first ball of the world

	TELEPORT_TYPE_COMMAND,
	TELEPORT_TYPE_GM,
	TELEPORT_TYPE_DUNGEON,
	TELEPORT_TYPE_DOJO,

	TELEPORT_TYPE_SCS,
	TELEPORT_TYPE_QUICK_TELEPORT,
	TELEPORT_TYPE_PARTY_POPO,
	TELEPORT_TYPE_DWC,

	TELEPORT_TYPE_PARTY_SUMMON,	// SUMMON PARTY WITH MASCOT SKILL
	TELEPORT_TYPE_GUILD_SUMMON,	// SUMMON GUILD WITH MASCOT SKILL

	TELEPORT_TYPE_COUNT,

	TELEPORT_TYPE_INVALID = 0xFF,

	TELEPORT_TYPE_FIRST = TELEPORT_TYPE_GAME_IN,
	TELEPORT_TYPE_LAST = TELEPORT_TYPE_COUNT - 1,
};

enum eDBO_REVIVAL_REQUEST_TYPE
{
	DBO_REVIVAL_REQUEST_TYPE_TELEPORT_SOMEWHERE = 0,
	DBO_REVIVAL_REQUEST_TYPE_CURRENT_POSITION,

	DBO_REVIVAL_REQUEST_TYPE_COUNT,

	DBO_REVIVAL_REQUEST_TYPE_FIRST = DBO_REVIVAL_REQUEST_TYPE_TELEPORT_SOMEWHERE,
	DBO_REVIVAL_REQUEST_TYPE_LAST = DBO_REVIVAL_REQUEST_TYPE_COUNT - 1,
};

// Resurrection Type
enum eREVIVAL_TYPE
{
	REVIVAL_TYPE_CURRENT_POSITION = 0, // Resurrect right from your current location
	REVIVAL_TYPE_BIND_POINT, // Move to bind location
	REVIVAL_TYPE_RESCUED, // Resurrected immediately by skill or other gm command
	REVIVAL_TYPE_SPECIFIED_POSITION, // Resurrected from the specified location

	REVIVAL_TYPE_FIRST = REVIVAL_TYPE_CURRENT_POSITION,
	REVIVAL_TYPE_LAST = REVIVAL_TYPE_RESCUED,
	INVALID_REVIVAL_TYPE = 0xFF
};

// Bind type
enum eDBO_BIND_TYPE
{
	DBO_BIND_TYPE_INITIAL_LOCATION = 0,		// After the character is generated even once the state is not the bind
	DBO_BIND_TYPE_POPO_STONE,				// The state of the bind position in the popostone
	DBO_BIND_TYPE_GM_TOOL,					// The bind position is specified by the GM tool
	DBO_BIND_TYPE_SKILL,					// Bind position specified by skill

	DBO_BIND_TYPE_FIRST = DBO_BIND_TYPE_INITIAL_LOCATION,
	DBO_BIND_TYPE_LAST = DBO_BIND_TYPE_SKILL,
	DBO_BIND_TYPE_INVALID = 0xFF
};

//-----------------------------------------------------------------------------------
// 포탈저장 최대개수
//-----------------------------------------------------------------------------------
const int		NTL_PORTAL_MAX_COUNT	= 100;

const DWORD		DBO_TELEPORT_PROPOSAL_WAIT_TIME		= 120;		// Teleport proposal window maximum wait time

//new
const DWORD		DBO_PARTY_TELEPORT_PROPOSAL_WAIT_TIME = 60;
const int		NTL_QUICK_PORTAL_MAX_COUNT = 10;
const int		NTL_QUICK_PORTAL_FAINTED_SLOT = 0;


//-----------------------------------------------------------------------------------
#pragma pack(1)

// Server Move Information
struct sSERVERTELEPORT_INFO
{
	void Init()
	{
		byTeleportType = INVALID_BYTE;
		worldId = INVALID_WORLDID;
		worldTblidx = INVALID_TBLIDX;
		serverChannelId = INVALID_SERVERCHANNELID;
		serverIndex = INVALID_SERVERINDEX;
	}

	BYTE			byTeleportType;	// eTELEPORT_TYPE
	WORLDID			worldId;
	TBLIDX			worldTblidx;
	sVECTOR3		vLoc;
	sVECTOR3		vDir;
	SERVERCHANNELID	serverChannelId;
	SERVERINDEX		serverIndex;
};

struct sQUICK_TELEPORT_INFO
{
	BYTE			bySlotNum;
	TBLIDX			worldTblidx;
	sVECTOR3		vLoc;
	sDBO_TIME		tSaveTime;
	TBLIDX			mapNameTblidx;
};

#pragma pack()

