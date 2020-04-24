//***********************************************************************************
//
//	File		:	NtlWorld.h
//
//	Begin		:	2006-11-15
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlSharedType.h"
#include "NtlTimeQuest.h"


const DWORD		NUM_WORLD_COUNT = 24;

//-----------------------------------------------------------------------------------
//  
//-----------------------------------------------------------------------------------

// Game World Rules
enum eGAMERULE_TYPE
{
	GAMERULE_NORMAL,		// normal world
	GAMERULE_RANKBATTLE,	// ranking battle
	GAMERULE_MUDOSA,		// 천하제일무도회장
	GAMERULE_DOJO,			// dojo
	GAMERULE_RAID,			// Raid
	GAMERULE_HUNT,			// ultimate dungeon
	GAMERULE_TIMEQUEST,		// time quest
	GAMERULE_TUTORIAL,		// tutorial
	GAMERULE_MINORMATCH,	// budokai minor match
	GAMERULE_MAJORMATCH,	// budokai major match
	GAMERULE_FINALMATCH,	// budokai final match
	GAMERULE_TEINKAICHIBUDOKAI,	// budokai world
	GAMERULE_TLQ,			// tlq
	GAMERULE_DWC,
	GAMERULE_CCBATTLEDUNGEON,	// ccbd
	GAMERULE_SKD,			//sky dungeon

	GAMERULE_TYPE_COUNT,
	INVALID_GAMERULE = 0xFFFFFFFF, // invalid
};

// WorldPlay Executer
enum eWORLDPLAY_EXECUTER_TYPE
{
	WORLDPLAY_EXECUTER_TS,
	WORLDPLAY_EXECUTER_WPS,
	WORLDPLAY_EXECUTER_TLQ,
	WORLDPLAY_EXECUTER_STATIC_WORLD,
	WORLDPLAY_EXECUTER_DYNAMIC_WORLD,
	WORLDPLAY_EXECUTER_OBJECTTRIGGER,
	WORLDPLAY_EXECUTER_ITEM_USE_BY_LOCATION,

	WORLDPLAY_EXECUTER_TYPE_COUNT,
	INVALID_WORLDPLAY_EXECUTER_TYPE = 0xff
};


// World Play Share Type
enum eWORLDPLAY_SHARE_TYPE
{
	WORLDPLAY_SHARE_NONE,
	WORLDPLAY_SHARE_INDIVIDUAL,
	WORLDPLAY_SHARE_PARTY,

	WORLDPLAY_SHARE_TYPE_COUNT,
	INVALID_WORLDPLAY_SHARE_TYPE = 0xff,
};


//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// 월드 관련 정의
//-----------------------------------------------------------------------------------
const float			NTL_DUMMY_AREA_WIDTH_IN_WORLD = 512.0f;		// in meters

const unsigned int	DBO_MAX_WAYPOINT_COUNT = 255; //
//-----------------------------------------------------------------------------------

const unsigned int	DYNAMIC_WORLD_SEPARATOR = 10000;
const int			MAX_DYNAMIC_WORLD_CREATE_COUNT = 1000;

//-----------------------------------------------------------------------------------
// 월드 관련 함수
//-----------------------------------------------------------------------------------
HOBJECT MakeTriggerObjectHandle(HOBJECT hWorldTriggerObjectOffset, DWORD dwTriggerObjectSequence);

const char * NtlGetGameRuleTypeString(BYTE byRuleType);
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// 월드 관련 packet structure
//-----------------------------------------------------------------------------------
#pragma pack(1)

enum eDBO_WORLDZONE_FUNC
{
	eDBO_WORLDZONE_FUNC_LEVEL_0120 = 0x1,
	eDBO_WORLDZONE_FUNC_LEVEL_2130 = 0x2,
	eDBO_WORLDZONE_FUNC_LEVEL_3150 = 0x4,
	eDBO_WORLDZONE_FUNC_LEVEL_5155 = 0x8,
	eDBO_WORLDZONE_FUNC_LEVEL_5660 = 0x10,
};

enum eCONTENTSONOFF_INDEX
{
	CONTENTSONOFF_WESTCITY_PORTAL = 0,
	CONTENTSONOFF_ZONE_2130_TRIGGER,
	CONTENTSONOFF_ZONE_3150_TRIGGER,
	CONTENTSONOFF_ZONE_5155_TRIGGER,
	CONTENTSONOFF_ZONE_5660_TRIGGER,
	CONTENTSONOFF_UD1_TRIGGER,
	CONTENTSONOFF_UD2_TRIGGER,
	CONTENTSONOFF_UD3_TRIGGER,
	CONTENTSONOFF_UD4_TRIGGER,
	CONTENTSONOFF_UD5_TRIGGER,
	CONTENTSONOFF_UD6_TRIGGER,
	CONTENTSONOFF_TMQ1_1_NPC,
	CONTENTSONOFF_TMQ1_2_NPC,
	CONTENTSONOFF_TMQ1_3_NPC,
	CONTENTSONOFF_TMQ1_4_NPC,
	CONTENTSONOFF_TMQ2_1_NPC,
	CONTENTSONOFF_TMQ2_2_NPC,
	CONTENTSONOFF_TMQ2_3_NPC,
	CONTENTSONOFF_TMP,
	CONTENTSONOFF_FREEBATTLE,
	CONTENTSONOFF_RANKBATTLE,
	CONTENTSONOFF_DOJO,
	CONTENTSONOFF_MASCOT,
	CONTENTSONOFF_HOIPOIMIX,
	CONTENTSONOFF_DUNGEON001_WORLD,
	CONTENTSONOFF_DUNGEON001_TRIGGER,
	CONTENTSONOFF_DUNGEON002_NPC,
	CONTENTSONOFF_DUNGEON003_TRIGGER,
	CONTENTSONOFF_DUNGEON004_WORLD,
	CONTENTSONOFF_DUNGEON004_TRIGGER,
	CONTENTSONOFF_DUNGEON006_TRIGGER,
	CONTENTSONOFF_DUNGEON007_TRIGGER,
	CONTENTSONOFF_WZONE1_NPC,
	CONTENTSONOFF_BDUNGEON001_TRIGGER,
	CONTENTSONOFF_DWC,
	CONTENTSONOFF_DB_SCRAMBLE ,
	CONTENTSONOFF_CCBD,
	CONTENTSONOFF_QUESTTYPE_BITFLAG,
	CONTENTSONOFF_CONTENTS_CONDITION,
	CONTENTSONOFF_CHARDATA_HISTORY,
	CONTENTSONOFF_UD1_HELP,
	CONTENTSONOFF_TMQ1_1_HELP,
	CONTENTSONOFF_TMP_HELP,
	CONTENTSONOFF_FREEBATTLE_HELP,
	CONTENTSONOFF_RANKTITLE_HELP,
	CONTENTSONOFF_DOJO_HELP,
	CONTENTSONOFF_MASCOT_HELP,
	CONTENTSONOFF_DWC_HELP,
	CONTENTSONOFF_DB_SCRAMBLE_HELP,
	CONTENTSONOFF_CHARDATA_HISTORY_HELP,

	CONTENTSONOFF_COUNT,
	CONTENTSONOFF_FIRST = CONTENTSONOFF_WESTCITY_PORTAL,
	CONTENTSONOFF_LAST = CONTENTSONOFF_COUNT - 1,
};

enum eQUESTONOFF_FLAG
{
	QUESTONOFF_YAHHOI_WEST = 0x0,
	QUESTONOFF_PORUNGA_ROCKS_NORTH,
	QUESTONOFF_FRAFRAN_DESERT_NORTH,
	QUESTONOFF_KARIN_FOREST,
	QUESTONOFF_WEST_LAND,
	QUESTONOFF_YAHHOI_EAST,
	QUESTONOFF_MUSHROOM_ROCKS_NORTH,
	QUESTONOFF_MUSHROOM_ROCKS_SOUTH,
	QUESTONOFF_OSORO_SHIMA,
	QUESTONOFF_UD01,
	QUESTONOFF_UD02,
	QUESTONOFF_UD03,
	QUESTONOFF_UD04,
	QUESTONOFF_UD05,
	QUESTONOFF_MARKET,
	QUESTONOFF_FREEBATTLE,
	QUESTONOFF_RANKBATTLE,
	QUESTONOFF_MASCOT,
	QUESTONOFF_HOIPOI_MIX,
	QUESTONOFF_CONVERT_CLASS,
	QUESTONOFF_DWC,
	QUESTONOFF_CCBD_MAIL,
	QUESTONOFF_PORUNGA_ROCKS_SOUTH,

	QUESTONOFF_COUNT,
	QUESTONOFF_INVALID = 0xFFFFFFFF,
};

struct sBATTLEDUNGEON_RULE_INFO
{
	bool bLpRegen;
	bool bEpRegen;
	bool bItemDrop;
	bool bItemUse;
	bool bExpAdd;
};

struct sGAME_RULE_INFO
{
	BYTE			byRuleType;	// eGAMERULE_TYPE
	union
	{
		sTIMEQUEST_RULE_INFO		sTimeQuestRuleInfo;
		sBATTLEDUNGEON_RULE_INFO	sBattleDungeonRuleInfo;
	};
};


struct sWORLD_INFO
{
	WORLDID			worldID;	// Worlds instance handle
	TBLIDX			tblidx;		// World Table Index
	HOBJECT			hTriggerObjectOffset; // Start trigger object handle value (will create an object in the object table to trigger the start handle when creating a table code + number)
	sGAME_RULE_INFO	sRuleInfo;
};

struct sWORLD_ZONE_INFO
{
	ZONEID			zoneId;
	bool			bIsDark;
};

#pragma pack()
//-----------------------------------------------------------------------------------
