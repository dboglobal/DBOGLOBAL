//***********************************************************************************
//
//	File		:	NtlObject.h
//
//	Begin		:	2006-04-07
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlCharacter.h"
#include "NtlQuest.h"


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
enum EEntityTypes
{
	ENTITY_CHARACTER, //PC, MOB, NPC, PET
	ENTITY_ITEM,
	ENTITY_OBJECT,

	INVALID_ENTITYTYPE = 0xFF,
};

enum eOBJTYPE
{
	OBJTYPE_PC, // PC
	OBJTYPE_NPC, // NPC
	OBJTYPE_MOB, // MOB
	OBJTYPE_SUMMON_PET, // Summoned pet <skill>
	OBJTYPE_ITEM_PET, // Summoned pet <item>
	OBJTYPE_DROPMONEY, // Money fell to the ground
	OBJTYPE_DROPITEM, // Items dropped on the ground
	OBJTYPE_TOBJECT, // trigger object
	OBJTYPE_DYNAMIC,	// dynamic created object
	OBJTYPE_ITEM,	// ITEM TYPE is not generated in the field. (Has character only)
	
	MAX_OBJTYPE,
	MAX_SPAWNOBJTYPE = OBJTYPE_DYNAMIC,	// 필드에 생성되는 오브젝트 타입의 MAX
	INVALID_OBJTYPE = 0xFF,

	OBJTYPE_FIRST = OBJTYPE_PC,
	OBJTYPE_LAST = MAX_OBJTYPE - 1,
};


// Object를 생성한 source
enum eDBO_OBJECT_SOURCE
{
	DBO_OBJECT_SOURCE_SKILL = 0,		// 스킬에 의해 생긴 object
	DBO_OBJECT_SOURCE_ITEM,				// 아이템에 의해 생긴 object

	DBO_OBJECT_SOURCE_UNKNOWN = 0xFF,

	DBO_OBJECT_SOURCE_FIRST = DBO_OBJECT_SOURCE_SKILL,
	DBO_OBJECT_SOURCE_LAST = DBO_OBJECT_SOURCE_ITEM,
};


// Trigger object의 기능
enum eDBO_TRIGGER_OBJECT_FUNC
{
	eDBO_TRIGGER_OBJECT_FUNC_SELECTION				= 0x1,
	eDBO_TRIGGER_OBJECT_FUNC_PORTAL					= 0x2,
	eDBO_TRIGGER_OBJECT_FUNC_BIND					= 0x4,
	eDBO_TRIGGER_OBJECT_FUNC_SCOUTER				= 0x8,
	eDBO_TRIGGER_OBJECT_FUNC_RANK_BATTLE			= 0x10,
	eDBO_TRIGGER_OBJECT_FUNC_NORMAL_DRAGONBALL		= 0x20,
	eDBO_TRIGGER_OBJECT_FUNC_RAID_DRAGONBALL		= 0x40,
	eDBO_TRIGGER_OBJECT_FUNC_MAIL_BOX				= 0x80,
	eDBO_TRIGGER_OBJECT_FUNC_TIME_MACHINE			= 0x100,
	eDBO_TRIGGER_OBJECT_FUNC_NAMEKAN_SIGN			= 0x200,
	eDBO_TRIGGER_OBJECT_FUNC_PARTY_INSTANCE_DUNGEON = 0x400,
	eDBO_TRIGGER_OBJECT_FUNC_TIME_LEAP_QUEST		= 0x800,
	eDBO_TRIGGER_OBJECT_FUNC_HOIPOIMIX				= 0x1000,
	eDBO_TRIGGER_OBJECT_FUNC_DOJO_WAREHOUSE			= 0x2000,
	eDBO_TRIGGER_OBJECT_FUNC_DOJO_SEAL				= 0x4000,
	eDBO_TRIGGER_OBJECT_FUNC_AUCTION_HOUSE			= 0x8000,
};

// Dynamic Object의 state
enum eDYNAMIC_OBJECT_STATE
{
	eDYNAMIC_OBJECT_STATE_SPAWN,
	eDYNAMIC_OBJECT_STATE_IDLE,
	eDYNAMIC_OBJECT_STATE_DESPAWN,

	eDYNAMIC_OBJECT_STATE_NUMS,
	eDYNAMIC_OBJECT_STATE_INVALID = 0xFF,
};

// Dynamic object function
enum eDYNAMIC_OBJECT_FUNCTION
{
	eDYNAMIC_OBJECT_FUNCTION_HOIPOIMIX = 0x1,

	eDYNAMIC_OBJECT_FUNCTION_NUMS,
	eDYNAMIC_OBJECT_FUNCTION_INVALID = 0xFF,
};

// Dynamic Object의 기능
enum eDBO_DYNAMIC_OBJECT_TYPE
{
	eDBO_DYNAMIC_OBJECT_TYPE_HOIPOI_MIX_MACHINE		= 1,		// 호이포이 믹스 렌탈 머신
	eDBO_DYNAMIC_OBJECT_TYPE_CAPSULE_HOUSE,						// 호이포이 믹스 캐쉬 머신

	eDBO_DYNAMIC_OBJECT_TYPE_NONE = 0xFF,
};

// Dynamic object substate
enum eDYNAMIC_OBJECT_SUBSTATE
{
	eDYNAMIC_OBJECT_SUBSTATE_ALL_SHOW,
	eDYNAMIC_OBJECT_SUBSTATE_ONLY_SPAWN_AVATAR_SHOW,
	eDYNAMIC_OBJECT_SUBSTATE_ENEMY_HIDE,

	eDYNAMIC_OBJECT_SUBSTATE_NUMS,
	eDYNAMIC_OBJECT_SUBSTATE_INVALID = 0xFF,
};

// Dynamic object substate
enum eDYNAMIC_OBJECT_HOIPOI_MACHINE_TYPE
{
	eDYNAMIC_OBJECT_HOIPOI_MACHINE_TYPE_NORMAL	= 1,
	eDYNAMIC_OBJECT_HOIPOI_MACHINE_TYPE_RENTAL,
	eDYNAMIC_OBJECT_HOIPOI_MACHINE_TYPE_NETP,
	eDYNAMIC_OBJECT_HOIPOI_MACHINE_TYPE_CASH,

	eDYNAMIC_OBJECT_HOIPOI_MACHINE_TYPE_NONE = 0xFF,
};


enum eIMMORTAL_MODE
{
	eIMMORTAL_MODE_OFF = 0x0,
	eIMMORTAL_MODE_NORMAL = 0x1,
	eIMMORTAL_MODE_AUTO_REGEN = 0x2,
	eIMMORTAL_MODE_INVALID = 0xFF,
};

enum eRELATION_TYPE
{
	RELATION_TYPE_ALLIENCE = 0x0,
	RELATION_TYPE_ENEMY = 0x1,
	RELATION_TYPE_OTHER = 0x2,
	MAX_RELATION_TYPE_COUNT = 0x3,
	INVALID_RELATION_TYPE = 0xFF,
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const unsigned int MAX_PC = 3000;	// 전체 PC 수

const unsigned int MAX_NPC = 4000;	// 전체 NPC 수

const unsigned int MAX_MOB = 100000;	// 전체 MOB 수

const unsigned int MAX_SUMMON_PET = MAX_PC;	// 전체 Summon Pet 수

const unsigned int MAX_ITEM_PET = MAX_PC;	// 전체 Item Pet 수

const unsigned int MAX_DROP_ITEM = 10000;	// 전체 DROP 되는 ITEM 수

const unsigned int MAX_DROP_MONEY = 10000;	// 전체 DROP 되는 MONEY 수

const unsigned int MAX_PC_HAVE_ITEM = 600;	// PC가 가지는 최대 ITEM 수

const unsigned int MAX_TRIGGER_OBJECT = 60000; // The total number of static objects trigger

const unsigned int MAX_DYNAMIC_OBJECT = MAX_PC; // 전체 동적 오브젝트 수 PC 당 한개만 허용

const unsigned int MAX_BOT = MAX_NPC + MAX_MOB + MAX_SUMMON_PET + MAX_ITEM_PET;		// BOT 수 ( 서버 내부 처리용 )

const unsigned int MAX_ITEM = MAX_PC * MAX_PC_HAVE_ITEM;	// 전체 아이템 수

const unsigned int MAX_GAME_OBJECT = MAX_PC + MAX_BOT + MAX_ITEM + MAX_DROP_ITEM + MAX_DROP_MONEY + MAX_TRIGGER_OBJECT + MAX_DYNAMIC_OBJECT; // The total number of objects

const float		   DBO_MAX_DISTANCE_OF_DYNAMIC_OBJECT = 20.0f;		// DYNAMIC OBJECT 설치 허용 최대 거리

const unsigned int DYNAMIC_OBJECT_CHECK_TERM = 1000;	// 다이나믹오브젝트의 주인과 거리 이격 체크 텀  seconds

const unsigned int DYNAMIC_OBJECT_DESPAWN_TERM = 1000;

const unsigned int DYNAMIC_OBJECT_SPAWN_TERM = 1000;

//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const char*	NtlGetObjectTypeString(eOBJTYPE eObjType);

#pragma pack(1)
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
typedef BYTE DYNAMIC_OBJECT_STATE;

struct sDYNAMIC_OBJECT_BRIEF
{
	BYTE		byObjectType;	// eDBO_DYNAMIC_OBJECT_TYPE
	TBLIDX		idObjectTblidx;	// 해당하는 tblidx(dynamic object가 아닌 hoipoi, capsule house등의 tblidx)
	HOBJECT		hOwner;
};

struct	sDYNAMIC_FIELD_BOSS_INFO
{
	HOBJECT		handle;
	sVECTOR2	vBossPos;
};

struct sDYNAMIC_FIELD_REWARD_INFO
{
	TBLIDX		eventTblidx;
	CHARACTERID	charId;
};


struct sDYNAMIC_OBJECT_STATE
{
	BYTE		byState; // Main State
	sVECTOR3	vCurLoc;
};
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#pragma pack(push, 1)
struct sOBJECT_INFO
{
	BYTE					objType;

	union
	{
		struct // pc
		{
			sPC_BRIEF		pcBrief;
			sCHARSTATE		pcState;
		};
		struct // npc
		{
			sNPC_BRIEF		npcBrief;
			sCHARSTATE		npcState;
		};
		struct // mob
		{
			sMOB_BRIEF		mobBrief;
			sCHARSTATE		mobState;
		};
		struct // summon pet
		{
			sSUMMON_PET_BRIEF		summonPetBrief;
			sCHARSTATE				summonPetState;
		};
		struct // item pet
		{
			sITEM_PET_BRIEF			itemPetBrief;
			sCHARSTATE				itemPetState;
		};
		struct // item
		{
			sITEM_BRIEF			itemBrief;
			sITEM_STATE			itemState;
			sITEM_OPTION_SET	itemOptionSet;
		};
		struct // money
		{
			sMONEY_BRIEF	moneyBrief;
			sMONEY_STATE	moneyState;
		};
		struct // trigger object
		{
			sTOBJECT_BRIEF	tobjectBrief;
			sTOBJECT_STATE	tobjectState;
		};
		struct // dynamic object
		{
			sDYNAMIC_OBJECT_BRIEF	dynamicObjBrief;
			sDYNAMIC_OBJECT_STATE	dynamicObjState;
		};
	};
};



struct UINT_PAIR
{
	UINT_PAIR(unsigned int k1, unsigned int k2) :
		key1(k1),
		key2(k2)
	{
	};

	bool operator < (const UINT_PAIR& pair) const
	{
		return ((pair.key1 < key1) || (pair.key2 < key2));
	}

	bool operator == (const UINT_PAIR& pair) const
	{
		return pair.key1 == key1 && pair.key2 == key2;
	}

	unsigned int	key1;
	unsigned int	key2;
};


#pragma pack()


