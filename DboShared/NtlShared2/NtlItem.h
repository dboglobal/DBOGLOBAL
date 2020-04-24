#pragma once
#include "NtlSharedType.h"
#include "NtlSharedDef.h"
#include "NtlVector.h"
#include "NtlQuest.h"
#include "NtlStringHandler.h"


//-----------------------------------------------------------------------------------
// ¾ÆÀÌÅÛ Å¸ÀÔ
//-----------------------------------------------------------------------------------
enum eITEM_TYPE
{
	ITEM_TYPE_GLOVE,
	ITEM_TYPE_STAFF,
	ITEM_TYPE_GUN,
	ITEM_TYPE_DUAL_GUN,
	ITEM_TYPE_CLAW,
	ITEM_TYPE_AXE,
	ITEM_TYPE_SCROLL,
	ITEM_TYPE_GEM,
	ITEM_TYPE_STICK,
	ITEM_TYPE_SWORD,
	ITEM_TYPE_FAN,
	ITEM_TYPE_WAND,
	ITEM_TYPE_BAZOOKA,
	ITEM_TYPE_BACK_PACK,
	ITEM_TYPE_INSTRUMENT,
	ITEM_TYPE_CLUB,
	ITEM_TYPE_DRUM,
	ITEM_TYPE_MASK,

	ITEM_TYPE_JACKET,
	ITEM_TYPE_PANTS,
	ITEM_TYPE_BOOTS,

	ITEM_TYPE_NECKLACE,
	ITEM_TYPE_EARRING,
	ITEM_TYPE_RING,

	ITEM_TYPE_SCOUTER,
	ITEM_TYPE_SCOUTER_PART,
	
	ITEM_TYPE_COSTUME,
	ITEM_TYPE_BAG,
	ITEM_TYPE_QUEST,
	ITEM_TYPE_STONE,

	ITEM_TYPE_RECOVER,
	ITEM_TYPE_FOOD,
	ITEM_TYPE_UTILITY,
	ITEM_TYPE_CHARM,
	ITEM_TYPE_CAPSULE,			// vehicle
	ITEM_TYPE_FUEL,
	ITEM_TYPE_JUNK,
	ITEM_TYPE_COLLECTION,
	ITEM_TYPE_WAREHOUSE,			// Ã¢°í Ãß°¡¿ë °¡¹æ¾ÆÀÌÅÛ¿¡ »ç¿ëÇÒ°ÍÀÓ
	ITEM_TYPE_DRAGONBALL,
	ITEM_TYPE_GAMBLE,
	ITEM_TYPE_MATERIAL,			// Á¶ÇÕÀç·á
	ITEM_TYPE_RECIPE,			
	ITEM_TYPE_HOIPOIROCK,
	ITEM_TYPE_DOGI,
	ITEM_TYPE_PURE_STONE,
	ITEM_TYPE_BLACK_STONE,
	ITEM_TYPE_NETPYSTORE,
	//new
	ITEM_TYPE_QUICK_TELEPORT,
	ITEM_TYPE_CORE_STONE_NONE,
	ITEM_TYPE_CRESCENT_POPO,
	ITEM_TYPE_PARTY_POPO,
	ITEM_TYPE_SKILL_POINT_RESET,
	ITEM_TYPE_ADD_CHARACTER_PLUS,
	ITEM_TYPE_CHARACTER_NAME_CHANGE,
	ITEM_TYPE_GUILD_NAME_CHANGE,
	ITEM_TYPE_DOGI_DYE,
	ITEM_TYPE_MASCOT_BODY,
	ITEM_TYPE_MASCOT_FOOD,
	ITEM_TYPE_MASCOT_RING,
	ITEM_TYPE_STONE_CORE,
	ITEM_TYPE_STONE_CORE_OLD,
	ITEM_TYPE_SKILL_POINT_RESET_PLUS,
	ITEM_TYPE_COMMERCIAL_BUFF,
	ITEM_TYPE_STONE_CORE_PLUS,
	ITEM_TYPE_COSTUME_SET,
	ITEM_TYPE_COSTUME_HAIR_STYLE,
	ITEM_TYPE_COSTUME_MASK,
	ITEM_TYPE_COSTUME_HAIR_ACCESSORY,
	ITEM_TYPE_COSTUME_BACK_ACCESSORY,
	ITEM_TYPE_SKILL_ONE_POINT_RESET,
	ITEM_TYPE_TMQ_LIMIT_COUNT_PLUS,
	ITEM_TYPE_BATTLE_DUNGEON_TICKET,
	ITEM_TYPE_UPGRADE_STONE_WEAPON,
	ITEM_TYPE_UPGRADE_STONE_ARMOR,
	ITEM_TYPE_GREATER_UPGRADE_STONE_WEAPON,
	ITEM_TYPE_GREATER_UPGRADE_STONE_ARMOR,
	ITEM_TYPE_DOWNGRADE_STONE_WEAPON,
	ITEM_TYPE_DOWNGRADE_STONE_ARMOR,
	ITEM_TYPE_NEW_FOOD,
	ITEM_TYPE_CLOSED_BOX,
	ITEM_TYPE_KEY,
	ITEM_TYPE_EVENT_COIN,
	ITEM_TYPE_UPGRADE_COUPON_WEAPON,
	ITEM_TYPE_UPGRADE_COUPON_ARMOR,
	ITEM_TYPE_UPGRADE_COUPON_FULL,
	ITEM_TYPE_UPGRADED_EXCHANGE_POWDER,
	ITEM_TYPE_SEAL,
	ITEM_TYPE_CHANGE_OPTION_KIT = 89,
	ITEM_TYPE_BEAD, //(item: Dogi Property Ball)
	ITEM_TYPE_DESTROY_BEAD,
	ITEM_TYPE_NEW_MASCOT,
	ITEM_TYPE_EVENT_MASCOT_EXP_FOOD,
	ITEM_TYPE_EVENT_MASCOT_FOOD,
	ITEM_TYPE_EVENT_MASCOT_AUTO_FOOD,

	ITEM_TYPE_EVENT_MASCOT_CHANGE_SKILL = 97,
	ITEM_TYPE_EVENT_MASCOT_LEVELUP,

	ITEM_TYPE_EVENT_SEAL = 100,

	ITEM_TYPE_COUNT,
	ITEM_TYPE_UNKNOWN    = 0xFF,

	ITEM_TYPE_WEAPON_FIRST = ITEM_TYPE_GLOVE,
	ITEM_TYPE_WEAPON_LAST  = ITEM_TYPE_MASK,

	ITEM_TYPE_ARMOR_FIRST = ITEM_TYPE_JACKET,
	ITEM_TYPE_ARMOR_LAST  = ITEM_TYPE_BOOTS,

	ITEM_TYPE_GROUP_WEAPON = 0,
	ITEM_TYPE_GROUP_ARMOR  = 1,
	ITEM_TYPE_GROUP_ETC	   = 2,

	ITEM_TYPE_FIRST = ITEM_TYPE_GLOVE,
	ITEM_TYPE_LAST = ITEM_TYPE_COUNT - 1,
};

//-----------------------------------------------------------------------------------
// ÀåÂø ¾ÆÀÌÅÛ Å¸ÀÔ
//-----------------------------------------------------------------------------------
enum eEQUIP_TYPE
{
	EQUIP_TYPE_MAIN_WEAPON,
	EQUIP_TYPE_SUB_WEAPON,
	EQUIP_TYPE_ARMOR,
	EQUIP_TYPE_SCOUTER,					// Scoute Body ( ½ºÄ«¿ìÅÍ °¡¹æ µé¾î°¡´Â°÷ )
	EQUIP_TYPE_QUEST,
	EQUIP_TYPE_ACCESSORY,
	EQUIP_TYPE_COSTUME,

	EQUIP_TYPE_COUNT,
	EQUIP_TYPE_UNKNOWN				= 0xFF,

	EQUIP_TYPE_FIRST = EQUIP_TYPE_MAIN_WEAPON,
	EQUIP_TYPE_LAST = EQUIP_TYPE_COUNT - 1,
};


//-----------------------------------------------------------------------------------
// ¾ÆÀÌÅÛ µî±Þ
//-----------------------------------------------------------------------------------
enum eITEM_RANK
{
	ITEM_RANK_NOTHING,
	ITEM_RANK_NORMAL,
	ITEM_RANK_SUPERIOR,
	ITEM_RANK_EXCELLENT,
	ITEM_RANK_RARE,
	ITEM_RANK_LEGENDARY,

	ITEM_RANK_COUNT,

	ITEM_RANK_FIRST = ITEM_RANK_NOTHING,
	ITEM_RANK_LAST = ITEM_RANK_COUNT - 1,
};

//-----------------------------------------------------------------------------------
// ¾ÆÀÌÅÛ ¸ðµ¨ Å¸ÀÔ
//-----------------------------------------------------------------------------------
enum eITEM_MODEL_TYPE
{
	ITEM_MODEL_TYPE_NONE,
	ITEM_MODEL_TYPE_RACE_GENDER,
	ITEM_MODEL_TYPE_RACE_GENDER_SKIN,

	ITEM_MODEL_TYPE_COUNT,
	ITEM_MODEL_TYPE_UNKNOWN				= 0xFF,

	ITEM_MODEL_TYPE_FIRST = ITEM_MODEL_TYPE_NONE,
	ITEM_MODEL_TYPE_LAST  = ITEM_MODEL_TYPE_COUNT - 1,
};

//-----------------------------------------------------------------------------------
// ¾ÆÀÌÅÛ ¹é »çÀÌÁî Á¤ÀÇ
//-----------------------------------------------------------------------------------
enum eITEM_BAG_SIZE
{
	ITEM_BAG_SIZE_4_BY_1			= 4,
	ITEM_BAG_SIZE_4_BY_2			= 8,
	ITEM_BAG_SIZE_4_BY_3			= 12,
	ITEM_BAG_SIZE_4_BY_4			= 16,
	ITEM_BAG_SIZE_4_BY_5			= 20,
	ITEM_BAG_SIZE_4_BY_6			= 24,
	ITEM_BAG_SIZE_4_BY_7			= 28,
	ITEM_BAG_SIZE_4_BY_8			= 32,
	ITEM_BAG_SIZE_MAX				= 32,
};

//-----------------------------------------------------------------------------------
// ¾ÆÀÌÅÛ UPGRADE GRADEÁ¤ÀÇ
//-----------------------------------------------------------------------------------
enum eITEM_GRADE_LEVEL
{
	ITEM_GRADE_LEVEL_0 = 0,		
	ITEM_GRADE_LEVEL_1,			
	ITEM_GRADE_LEVEL_2,			
	ITEM_GRADE_LEVEL_3,			
	ITEM_GRADE_LEVEL_4,			

	ITEM_GRADE_LEVEL_INVALID	= 0xFF
};
//-----------------------------------------------------------------------------------
// ÄÁÅ×ÀÌ³Ê Å¸ÀÔ
//-----------------------------------------------------------------------------------
enum eCONTAINER_TYPE
{
	//DB Table( Item )
	CONTAINER_TYPE_BAGSLOT,				// °¡¹æÀÌ µé¾î°¥°÷ (0~5) °¢°¢ÀÇ °¡¹æÀÌ È°¼ºÈ­
	CONTAINER_TYPE_BAG1,				// À§ÀÇ BagÀÌ È°¼ºÈ­ µÈ°æ¿ì °¢ °¡¹æ¿¡ µé¾î°¡´Â ÀÎº¥µé...	
	CONTAINER_TYPE_BAG2,
	CONTAINER_TYPE_BAG3,
	CONTAINER_TYPE_BAG4,
	CONTAINER_TYPE_BAG5,

	CONTAINER_TYPE_EQUIP,

	//DB Table(Bank)
	CONTAINER_TYPE_BANKSLOT,
	CONTAINER_TYPE_BANK1,
	CONTAINER_TYPE_BANK2,
	CONTAINER_TYPE_BANK3,
	CONTAINER_TYPE_BANK4,				//bank slot shared with account

	//DB Table(Guild Bank)
	CONTAINER_TYPE_GUILD1,
	CONTAINER_TYPE_GUILD2,
	CONTAINER_TYPE_GUILD3,


	CONTAINER_TYPE_COUNT,
	CONTAINER_TYPE_NONE = 0xFF,

	CONTAINER_TYPE_BAG_FIRST = CONTAINER_TYPE_BAG1,
	CONTAINER_TYPE_BAG_LAST = CONTAINER_TYPE_BAG5,

	CONTAINER_TYPE_INVEN_FIRST = CONTAINER_TYPE_BAGSLOT,
	CONTAINER_TYPE_INVEN_LAST = CONTAINER_TYPE_EQUIP,
	
	CONTAINER_TYPE_BANK_FIRST = CONTAINER_TYPE_BANKSLOT,
	CONTAINER_TYPE_BANK_LAST  = CONTAINER_TYPE_BANK4,

	CONTAINER_TYPE_GUILD_FIRST = CONTAINER_TYPE_GUILD1,
	CONTAINER_TYPE_GUILD_LAST  = CONTAINER_TYPE_GUILD3,

	CONTAINER_TYPE_FIRST = CONTAINER_TYPE_BAGSLOT,
	CONTAINER_TYPE_LAST = CONTAINER_TYPE_BANK4,

};

//-----------------------------------------------------------------------------------
// Bag Slot Position
//-----------------------------------------------------------------------------------
enum eBAGSLOT_POSITION
{
	BAGSLOT_POSITION_BAGSLOT_POSITION_0 = 0,
	BAGSLOT_POSITION_BAGSLOT_POSITION_1,
	BAGSLOT_POSITION_BAGSLOT_POSITION_2,
	BAGSLOT_POSITION_BAGSLOT_POSITION_3,
	BAGSLOT_POSITION_BAGSLOT_POSITION_4,

	BAGSLOT_POSITION_NONE = 0xFF,
};


//-----------------------------------------------------------------------------------
// ÀåÂø ¾ÆÀÌÅÛ ½½·Ô
//-----------------------------------------------------------------------------------
enum eEQUIP_SLOT_TYPE
{
	EQUIP_SLOT_TYPE_HAND, 
	EQUIP_SLOT_TYPE_SUB_WEAPON,
	EQUIP_SLOT_TYPE_JACKET,
	EQUIP_SLOT_TYPE_PANTS,
	EQUIP_SLOT_TYPE_BOOTS,
	EQUIP_SLOT_TYPE_SCOUTER,
	EQUIP_SLOT_TYPE_QUEST,
	EQUIP_SLOT_TYPE_NECKLACE,
	EQUIP_SLOT_TYPE_EARRING_1,
	EQUIP_SLOT_TYPE_EARRING_2,
	EQUIP_SLOT_TYPE_RING_1,
	EQUIP_SLOT_TYPE_RING_2,
	EQUIP_SLOT_TYPE_COSTUME_SET,
	EQUIP_SLOT_TYPE_COSTUME_HAIR_STYLE,
	EQUIP_SLOT_TYPE_COSTUME_MASK,
	EQUIP_SLOT_TYPE_COSTUME_HAIR_ACCESSORY,
	EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY,

	EQUIP_SLOT_TYPE_COUNT,
	EQUIP_SLOT_TYPE_UNKNOWN	= 0xFF,

	EQUIP_SLOT_TYPE_FIRST	= EQUIP_SLOT_TYPE_HAND,
	EQUIP_SLOT_TYPE_LAST	= EQUIP_SLOT_TYPE_COUNT - 1,
};

enum eEQUIP_SLOT_FLAG
{
	EQUIP_SLOT_FLAG_HAND = (0x01 << EQUIP_SLOT_TYPE_HAND),
	EQUIP_SLOT_FLAG_SUB_WEAPON = (0x01 << EQUIP_SLOT_TYPE_SUB_WEAPON),
	EQUIP_SLOT_FLAG_JACKET = (0x01 << EQUIP_SLOT_TYPE_JACKET),
	EQUIP_SLOT_FLAG_PANTS = (0x01 << EQUIP_SLOT_TYPE_PANTS),
	EQUIP_SLOT_FLAG_BOOTS = (0x01 << EQUIP_SLOT_TYPE_BOOTS),
	EQUIP_SLOT_FLAG_SCOUTER = (0x01 << EQUIP_SLOT_TYPE_SCOUTER),
	EQUIP_SLOT_FLAG_QUEST = (0x01 << EQUIP_SLOT_TYPE_QUEST),
	EQUIP_SLOT_FLAG_NECKLACE = (0x01 << EQUIP_SLOT_TYPE_NECKLACE),
	EQUIP_SLOT_FLAG_EARRING_1 = (0x01 << EQUIP_SLOT_TYPE_EARRING_1),
	EQUIP_SLOT_FLAG_EARRING_2 = (0x01 << EQUIP_SLOT_TYPE_EARRING_2),
	EQUIP_SLOT_FLAG_RING_1 = (0x01 << EQUIP_SLOT_TYPE_RING_1),
	EQUIP_SLOT_FLAG_RING_2 = (0x01 << EQUIP_SLOT_TYPE_RING_2),
	EQUIP_SLOT_FLAG_COSTUME_SET = (0x01 << EQUIP_SLOT_TYPE_COSTUME_SET),
	EQUIP_SLOT_FLAG_COSTUME_HAIR_STYLE = (0x01 << EQUIP_SLOT_TYPE_COSTUME_HAIR_STYLE),
	EQUIP_SLOT_FLAG_COSTUME_MASK = (0x01 << EQUIP_SLOT_TYPE_COSTUME_MASK),
	EQUIP_SLOT_FLAG_COSTUME_HAIR_ACCESSORY = (0x01 << EQUIP_SLOT_TYPE_COSTUME_HAIR_ACCESSORY),
	EQUIP_SLOT_FLAG_COSTUME_BACK_ACCESSORY = (0x01 << EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY),
};

//-----------------------------------------------------------------------------------
// CostumeÀ» Âø¿ëÇÏ°Å³ª ¹þÀ»¶§ Ä³¸¯ÅÍ¿¡¼­ °¢°¢ detach, attachÀÌ µÇ¾î¾ß ÇÒ Itemµé¿¡
// ´ëÇØ BitFlagSetÀ» È°¿ëÇÏµµ·Ï ÇÏ¸ç ´ÙÀ½Àº ±âÈ¹ÆÄÆ®¿¡ BitFlagToolÀ» ÀÌ¿ëÇÏ¿© SetÀ»
// ¸¸µé¶§ ÇÊ¿äÇÑ »ó¼ö°ªÀ» Á¤ÀÇÇÏ¸ç ÇÁ·Î±×·¥¿¡¼­ È®ÀÎÇÒ¼ö ÀÖ´Â ½Ç FLAG¿¡ ´ëÇØ¼­´Â 
// 0x01À» SHIFTÇÏ¿© »ç¿ëÇÑ´Ù. °í·Î 32°³±îÁö¸¸ Ãß°¡¼öÁ¤µÉ¼ö ÀÖÀ¸¸ç Ãß°¡¼öÁ¤»çÇ×¿¡
// ´ëÇØ¼­´Â ¸Å¿ì ÁÖÀÇ¸¦ ¿äÇÑ´Ù.
//
// 2008.03.24 ¿ìµð
//-----------------------------------------------------------------------------------
enum eCOSTUME_BITFLAG_SLOT_TYPE
{
	eCBST_JACKET,
	eCBST_PANTS,
	eCBST_BOOTS,
	eCBST_TWO_HAND,
	eCBST_SUB_WEAPON_BACK,
	eCBST_HEAD,
	eCBST_FACE,
	eCBST_SCOUTER_AT_EYE,
};

enum eCOSTUME_BITFLAG_SLOT_TYPE_SHIFT
{
	eCBSTS_JACKET			= 0x01 << eCBST_JACKET,
	eCBSTS_PANTS			= 0x01 << eCBST_PANTS,
	eCBSTS_BOOTS			= 0x01 << eCBST_BOOTS,
	eCBSTS_TWO_HAND			= 0x01 << eCBST_TWO_HAND,
	eCBSTS_SUB_WEAPON_BACK	= 0x01 << eCBST_SUB_WEAPON_BACK,
	eCBSTS_HEAD				= 0x01 << eCBST_HEAD,
	eCBSTS_FACE				= 0x01 << eCBST_FACE,
	eCBSTS_SCOUTER_AT_EYE	= 0x01 << eCBST_SCOUTER_AT_EYE,
};

//-----------------------------------------------------------------------------------
// Items Table Type
//-----------------------------------------------------------------------------------
enum eITEM_TABLE_TYPE
{
	ITEM_TABLE_TYPE_ITEM,
	ITEM_TABLE_TYPE_BANK,
	ITEM_TABLE_TYPE_BANK_PUBLIC,
	ITEM_TABLE_TYPE_GUILD_BANK,

	ITEM_TABLE_TYPE_NONE = 0xFF,

	ITEM_TABLE_TYPE_FIRST = ITEM_TABLE_TYPE_ITEM,
	ITEM_TABLE_TYPE_LAST = ITEM_TABLE_TYPE_NONE - 1,
};

//-----------------------------------------------------------------------------------
// Bank Item Table Type
//-----------------------------------------------------------------------------------
enum eBANK_TABLE_TYPE
{
	BANK_TABLE_TYPE_BASIC,
	BANK_TABLE_TYPE_ADD1,
	BANK_TABLE_TYPE_ADD2,
	BANK_TABLE_TYPE_PUBLIC,

	BANK_TABLE_TYPE_NONE = 0xFF,

	BANK_TABLE_TYPE_FIRST = BANK_TABLE_TYPE_BASIC,
	BANK_TABLE_TYPE_LAST = BANK_TABLE_TYPE_PUBLIC,
};

//-----------------------------------------------------------------------------------
// Item Quality Options
//-----------------------------------------------------------------------------------
enum eOPTION_QUALITY
{
	OPTION_QUALITY_SUPERIOR,
	OPTION_QUALITY_EXCELLENT,
	OPTION_QUALITY_RARE,
	OPTION_QUALITY_LEGENDARY,
	OPTION_QUALITY_SET,

	OPTION_QUALITY_COUNT,
	OPTION_QUALITY_UNKNOWN	= 0xFF,

	OPTION_QUALITY_FIRST	= OPTION_QUALITY_SUPERIOR,
	OPTION_QUALITY_LAST  	= OPTION_QUALITY_COUNT - 1,
};
//-----------------------------------------------------------------------------------
// Function item type
//-----------------------------------------------------------------------------------
enum eITEM_FUNC_TYPE
{
	ITEM_FUNC_TYPE_MAIN_WEAPON_OFF,
	ITEM_FUNC_TYPE_TRADE_FORBIDDEN,
	ITEM_FUNC_TYPE_STORE_FORBIDDEN,
	ITEM_FUNC_TYPE_SELL_FORBIDDEN,
	ITEM_FUNC_TYPE_DELETE_FORBIDDEN,
	ITEM_FUNC_TYPE_SOCKET_ON,
	ITEM_FUNC_TYPE_SELECT_OPTION_ON,
	ITEM_FUNC_TYPE_NEED_LEVEL_CHECK_OFF,
};

enum eITEM_FUNC_BIT_FLAG // wFuncionBitFlag in sITEM_TBLDAT
{
	ITEM_FUNC_BIT_FLAG_MAIN_WEAPON_OFF = 0x01 << ITEM_FUNC_TYPE_MAIN_WEAPON_OFF,
	ITEM_FUNC_BIT_FLAG_TRADE_FORBIDDEN = 0x01 << ITEM_FUNC_TYPE_TRADE_FORBIDDEN,	// trade forbidden
	ITEM_FUNC_BIT_FLAG_STORE_FORBIDDEN = 0x01 << ITEM_FUNC_TYPE_STORE_FORBIDDEN,	// warehouse forbidden
	ITEM_FUNC_BIT_FLAG_SELL_FORBIDDEN = 0x01 << ITEM_FUNC_TYPE_SELL_FORBIDDEN,		// sell forbidden
	ITEM_FUNC_BIT_FLAG_DELETE_FORBIDDEN = 0x01 << ITEM_FUNC_TYPE_DELETE_FORBIDDEN,	// delete forbidden
	ITEM_FUNC_BIT_FLAG_SOCKET_ON = 0x01 << ITEM_FUNC_TYPE_SOCKET_ON,
	ITEM_FUNC_BIT_FLAG_OPTION_ON = 0x01 << ITEM_FUNC_TYPE_SELECT_OPTION_ON,
	ITEM_FUNC_BIT_FLAG_NEED_LEVEL_CHECK_OFF = 0x01 << ITEM_FUNC_TYPE_NEED_LEVEL_CHECK_OFF,
};

//-----------------------------------------------------------------------------------
// °´Ã¼ ¾÷µ¥ÀÌÆ® Å¸ÀÔ (ÇöÀç ¼­¹ö ³»ºÎ¿ë)
//-----------------------------------------------------------------------------------
enum eSTUFF_UPDATE_TYPE
{
	STUFF_UPDATE_INCREASE,
	STUFF_UPDATE_DECREASE,
	STUFF_UPDATE_REPLACE,
	STUFF_UPDATE_INVALID = 0xFF,
};

//-----------------------------------------------------------------------------------
// When use item. CoolTime Group
//-----------------------------------------------------------------------------------
enum eITEM_COOL_TIME_GROUP
{
	ITEM_COOL_TIME_GROUP_DH_LP = 0,
	ITEM_COOL_TIME_GROUP_DH_EP,
	ITEM_COOL_TIME_GROUP_HOT_LP,
	ITEM_COOL_TIME_GROUP_HOT_EP,
	ITEM_COOL_TIME_GROUP_DH_RESCUE,
	ITEM_COOL_TIME_GROUP_DH_POISON,
	ITEM_COOL_TIME_GROUP_DH_STOMACHACHE,
	ITEM_COOL_TIME_GROUP_TELEPORT,
	ITEM_COOL_TIME_GROUP_CB_OFFENCE,
	ITEM_COOL_TIME_GROUP_CB_DEFENCE,
	ITEM_COOL_TIME_GROUP_HOT_FOOD,
	ITEM_COOL_TIME_GROUP_HOT_DRINK,

	ITEM_COOL_TIME_GROUR_MASCOT_FOOT,
	ITEM_COOL_TIME_GROUP_GIFT_BOX,
	ITEM_COOL_TIME_GROUP_TMQ_TIME_PLUS,
	ITEM_COOL_TIME_GROUP_HLS_SKILL,
	ITEM_COOL_TIME_GROUP_EVENT_COIN_PLUS,
	ITEM_COOL_TIME_GROUP_LINGER_LP,
	ITEM_COOL_TIME_GROUP_LINGER_EP,
	ITEM_COOL_TIME_GROUP_UNKNOWN,
	ITEM_COOL_TIME_GROUP_UNKNOWN2,
	ITEM_COOL_TIME_GROUP_UNKNOWN3, //bitter tea

	ITEM_COOL_TIME_GROUP_COUNT,

	ITEM_COOL_TIME_GROUP_FIRST = ITEM_COOL_TIME_GROUP_DH_LP,
	ITEM_COOL_TIME_GROUP_LAST = ITEM_COOL_TIME_GROUP_COUNT - 1
};

enum eITEM_PROCESS_TYPE
{
	 ITEM_PROCESS_USEITEM = 0, // Item Use¿¡ ÀÇÇØ »ç¿ë ReplaceµÊ [7/23/2008 SGpro]
	 ITEM_PROCESS_DISASSEMBLE,

	 ITEM_PROCESS_TYPE_COUNT,

	 ITEM_PROCESS_TYPE_FIRST = ITEM_PROCESS_USEITEM,
	 ITEM_PROCESS_TYPE_LAST = ITEM_PROCESS_TYPE_COUNT - 1,
};

//-----------------------------------------------------------------------------------
// recipe types
//-----------------------------------------------------------------------------------
enum eRECIPE_TYPE
{
	eRECIPE_TYPE_NORMAL,
	eRECIPE_TYPE_MAINWEAPON,
	eRECIPE_TYPE_CLOTH_ARMOR,
	eRECIPE_TYPE_ACCESSORY,
	eRECIPE_TYPE_TECH_ARMOR,
	eRECIPE_TYPE_SUBWEAPON,

	eRECIPE_TYPE_COUNT,

	eRECIPE_INVALID = 0xFF,
};

//-----------------------------------------------------------------------------------
// hoi poi crafting result
//-----------------------------------------------------------------------------------
enum eHOIPOI_MIX_RESULT
{
	HOIPOI_MIX_RESULT_FAIL,
	HOIPOI_MIX_RESULT_SUCCESS,
	HOIPOI_MIX_RESULT_GREAT_SUCCESS,

	HOIPOI_MIX_RESULT_COUNT,

	HOIPOI_MIX_RESULT_INVALID = 0xFF,
};

//-----------------------------------------------------------------------------------
// ¾ÆÀÌÅÛ ±¸¸Å½Ã ÇÊ¿äÇÑ ±ÇÇÑ
//-----------------------------------------------------------------------------------
enum eITEM_NEED_FUNCTION
{
	ITEM_NEED_FUNCTION_FOUNDATION,				// µµÀå¼³¸³
	ITEM_NEED_FUNCTION_MAKING_DOGI,				// µµº¹Á¦ÀÛ
	ITEM_NEED_FUNCTION_BUYING_VEHICLE,			// Å»°Í±¸¸Å	
	ITEM_NEED_FUNCTION_BUYING_MANUAL,			// ±³º»±¸¸Å

	ITEM_NEED_FUNCTION_FIRST = ITEM_NEED_FUNCTION_FOUNDATION ,
	ITEM_NEED_FUNCTION_LAST  = ITEM_NEED_FUNCTION_BUYING_MANUAL
};

enum eCOMMON_POINT_TYPE
{
	eCOMMON_POINT_TYPE_ZENI,
	eCOMMON_POINT_TYPE_NETPY,
	eCOMMON_POINT_TYPE_CASHSHOP
};


//-----------------------------------------------------------------------------------
// ¾ÆÀÌÅÛ °ü·Ã Á¤ÀÇ
//-----------------------------------------------------------------------------------
const DWORD			NTL_DEF_ITEM_ATTACK_SPEEED = 1000; // ITEM ATTACK SPEED

const int			NTL_MAX_SYSTEM_EFFECT_COUNT = 4;

const int			NTL_MAX_OPTION_COUNT = 21; // 옵션아이템의 선별 옵션 최대 갯수 / Maximum number of optional items for option items

const int			NTL_DEF_LUCKY_CHANCE_RATE = 10;  // 럭키 드랍시에 올라가는 드랍 배율 / Drop rate on Lucky Drop City

const int			NTL_MAX_MERCHANT_COUNT = 36;  // 상점탭에 전시하는 최대갯수 / The maximum number to display in the Store tab

const int			NTL_MAX_SUPERIOR_DROP = 18;  // Superior 아이템 드랍 최대수 / Superior Item Drop Maximum

const int			NTL_MAX_LEGENDARY_DROP = 18;  // 유니크 아이템 드랍 최대수 / Unique Item Drop Maximum

const int			NTL_MAX_NORMAL_DROP = 18;  // 노말 아이템 드랍 최대수 / Normal Item Drop Maximum Number

const int			NTL_MAX_EXCELLENT_DROP = 18;  // 엑설런트 아이템 드랍 최대수 / Excellent Item Drop Maximum

const int			NTL_MAX_EACH_DROP = 12;  // 이치 아이템 드랍 최대수 / Ichiba Item Drop Maximum

const int			NTL_MAX_TYPE_DROP = 12;  // 타입 아이템 드랍 최대수 / Maximum number of item drop items

const int			NTL_MAX_DROP_TABLE_SELECT = 8;   // 아이템 드랍 택일 최대수 / Maximum number of item drops

const int			NTL_MAX_DROP_TABLE_RATE_COUNT = 8;

const int			NTL_MAX_IDENTIFY_DROP_RATE = 10;  // Unidentifyed drop probability (%)

const int			NTL_TICK_OF_DROP_OWNNER = 40000; //

const int			NTL_TICK_OF_DROP_DISAPPEAR = 60000;

const int			NTL_TICK_OF_BATTLE_DRAGONBALL_DISAPPEAR = 54;

const int			NTL_MAX_MERCHANT_TAB_COUNT = 6;

const int			NTL_DEF_OPTION_LEVEL_COUNT = 20;   // 100Level 일 경우 5렙으로 20구간 / 20 levels for 5 levels

const DWORD			NTL_MAX_EFFECT_IN_ITEM = 2;		// 아이템이 가질 수 있는 system effect의 최대 갯수 / The maximum number of system effects an item can have

const DWORD			NTL_MAX_OPTION_IN_ITEM = 2;		// Maximum number of items that can have Option

const int			NTL_MAX_RANDOM_OPTION_IN_ITEM = 8;

const float			NTL_MAX_MERCHANT_DISTANCE = 3.0f;	// 아이템 상인간의 허용 최대 거리

const float			NTL_MAX_MERCHANT_DISTANCE_EXTRA = 1.0f;	// 아이템 상인간의 허용 최대 거리의 오차 범위(for sync.) / Tolerance range of maximum allowable distance between item traders (for sync.)

const float			NTL_MAX_NAMEKAN_DISTANCE = 15.0f;	// 아이템 나메칸사인의 허용 최대 거리 / Maximum allowable distance of items or mechansign

const float			NTL_MAX_BUS_DISTANCE = 20.0f;		// PC BUS의 허용 최대 거리

const float			NTL_MAX_LOOTING_DISTANCE = 15.0f;	// pick up distance

const float			NTL_MAX_LOOTING_DISTANCE_EXTRA = 1.0f;	//pick up distance bonus

const float			NTL_MAX_OBJECT_DISTANCE = 5.0f;	// 아이템이나 제니를 주울 때의 허용 최대 거리 / Maximum allowable distance when picking items or jenni

const float			NTL_MAX_OBJECT_DISTANCE_EXTRA = 1.0f;	// 아이템이나 제니를 주울 때의 허용 최대 거리의 오차 범위(for sync.) / The tolerance range of the maximum distance allowed when picking up an item or jenny (for sync.)

const BYTE			NTL_DUR_UNKNOWN = 0xFF;

const int			NTL_UNIDENTIFY_LEGNEDARY_RATE = 50;

const int			NTL_UNIDENTIFY_RARE_RATE = 30;

const int			NTL_UNIDENTIFY_EXCELLENT_RATE = 20;

const int			NTL_UNIDENTIFY_SUPERIOR_RATE = 10;

const int			NTL_UNIDENTIFY_NORMAL_RATE = 5;

const int			NTL_ITEM_MAX_GRADE = 15;

const int			NTL_ITEM_COUNT_GRADE = 5;

const int			NTL_ITEM_UPGRADE_EQUIP_COUNT = 5;	// Cheat 로 올릴 장비의 총 갯수 / Total number of equipments to raise with Cheat

const int			NTL_ITEM_OPTION_BIND_LEVEL = 5;	//  옵션을 붙일때 쓰일 나눌 구간 / Split section to use when attaching option

const DWORD			NTL_ITEM_ALL_USE_FLAG = 0xFFFFFFFF;

const BYTE			NTL_UNSTACKABLE_ITEM_COUNT = 1;

const int			NTL_ITEM_MAX_DRAGONBALL = 7;

const int			NTL_ITEM_MAX_BALL = 9;	// 드롭,정크 볼 포함 / Drop, junk ball included

const float			DBO_ITEM_CLASS_DEFENCE_BONUS_IN_PERCENT = 20.0f;		// 직업별 방어구의 특화 보너스 / Special bonuses for occupation armor

const int			NTL_ITEM_MAX_UPGRADE_POSITION = 14;

const int			NTL_SHOP_ITEM_IDENTIFY_ZENNY = 100;

const int			NTL_ITEM_RECIPE_MAX_COUNT = 200;

const int			NTL_ITEM_RECIPE_NEED_ITEM_COUNT = 20;

const DWORD			DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM = 5;
const DWORD			DBO_MAX_COUNT_RECIPE_CREATE_ITEM = 3;

const BYTE			DBO_HOIPOIMIX_CASH_RATE_MIN = 50;

const BYTE			DBO_HOIPOIMIX_CASH_RATE_MAX = 200;

const BYTE			DBO_HOIPOIMIX_MIX_LEVEL_MAX = 35;

const BYTE			ITEM_DISASSEMBLE_MAX_RESULT = 3;

const BYTE			ITEM_BULK_CREATE_COUNT = 5;
const BYTE			ITEM_BULK_DELETE_COUNT = 5;
const BYTE			ITEM_BULK_UPDATE_COUNT = 10;

//table
const DWORD		DBO_MAX_LENGTH_ITEM_NAME_TEXT = 64;
const DWORD		DBO_MAX_LENGTH_ITEM_ICON_NAME = 32;
const DWORD		DBO_MAX_LENGTH_ITEM_MODEL_NAME = 32;
const DWORD		DBO_MAX_LENGTH_ITEM_SUBWEAPON_ACT_MODEL_NAME = 32;

const DWORD		DBO_MAX_BACK_PACK_COUNT = 4;

const unsigned int		DBO_BLANK_DRAGON_BALL_IDX = 19988;


//-----------------------------------------------------------------------------------
// NEW NEW NEW 4.12.2014
//-----------------------------------------------------------------------------------
#pragma pack(1)

struct sITEM_RANDOM_OPTION
{
	WORD		wOptionIndex;
	int			optionValue;
};

struct sITEM_RANDOM_OPTION_SET
{
	sITEM_RANDOM_OPTION_SET()
	{
		Init();
	}

	void Init()
	{
		for (int i = 0; i < NTL_MAX_RANDOM_OPTION_IN_ITEM; i++)
		{
			aRandomOptions[i].optionValue = -1;
			aRandomOptions[i].wOptionIndex = INVALID_WORD;
		}
	}

	void SetRandomOption(BYTE byPos, int nValue)
	{
		aRandomOptions[byPos].optionValue = nValue;
	}

	void SetRandomOption(BYTE byPos, WORD wOptionIndex, int nValue)
	{
		aRandomOptions[byPos].wOptionIndex = wOptionIndex;
		aRandomOptions[byPos].optionValue = nValue;
	}

	sITEM_RANDOM_OPTION& GetRandomOption(BYTE byPos)
	{
		return aRandomOptions[byPos];
	}

	sITEM_RANDOM_OPTION			aRandomOptions[NTL_MAX_RANDOM_OPTION_IN_ITEM];
};

struct sITEM_OPTION_SET
{
	void Init()
	{
		memset(aOptionTblidx, INVALID_TBLIDX, sizeof(aOptionTblidx));
		for (int i = 0; i < NTL_MAX_RANDOM_OPTION_IN_ITEM; i++)
		{
			aRandomOption[i].optionValue = -1;
			aRandomOption[i].wOptionIndex = INVALID_WORD;
		}
	}
	void SetOption(BYTE byPos, TBLIDX option)
	{
		aOptionTblidx[byPos] = option;
	}
	void SetItemOption(BYTE byPos, WORD wOptionIndex, int nValue)
	{
		aRandomOption[byPos].wOptionIndex = wOptionIndex;
		aRandomOption[byPos].optionValue = nValue;
	}
	sITEM_RANDOM_OPTION& GetRandomOption(BYTE byPos)
	{
		return aRandomOption[byPos];
	}
	WORD GetRandomOptionIndex(BYTE byPos)
	{
		return aRandomOption[byPos].wOptionIndex;
	}
	int GetRandomOptionValue(BYTE byPos)
	{
		return aRandomOption[byPos].optionValue;
	}

	TBLIDX						aOptionTblidx[NTL_MAX_OPTION_IN_ITEM];
	sITEM_RANDOM_OPTION			aRandomOption[NTL_MAX_RANDOM_OPTION_IN_ITEM]; 
};
struct sITEM_RANDOM_OPTION_CHECK_DATA
{
  BYTE		byItemSearchTableType;
  BYTE		byRank;
  TBLIDX	itemId;
  CHARACTERID charId;
  MAILID	mailId;
  TBLIDX	tblidx;
  sITEM_RANDOM_OPTION_SET sItemRandomOptionSet;
};
struct sITEM_DB_RANDOM_OPTION
{
	int		nOptionIndex;
	int		optionValue;
};
struct sITEM_OPTION_DB_SET
{
	TBLIDX							aOptionTblidx[NTL_MAX_OPTION_IN_ITEM];
	sITEM_DB_RANDOM_OPTION			aRandomOption[NTL_MAX_RANDOM_OPTION_IN_ITEM];
};


// marketplace 
struct sTENKAICHIDAISIJYOU_DATA
{
	CHARACTERID			charId;
	QWORD				nItem;
	BYTE				byTabType;
	BYTE				byItemType;
	BYTE				byItemLevel;
	WCHAR				awchItemName[DBO_MAX_LENGTH_ITEM_NAME_TEXT + 1];
	WCHAR				awchSeller[NTL_MAX_SIZE_CHAR_NAME + 1];
	DWORD				dwPrice;
	DBOTIME				nStartSellTime;
	DBOTIME				nEndSellTime;
	ITEMID				itemId;
	TBLIDX				itemNo;
	BYTE				byCount;
	DWORD				dwNeed_Class_Bit_Flag;
	BYTE				byRank;
	BYTE				byGrade;
	BYTE				byCurrentDurability;
	BYTE				byBattleAttribute;
	WCHAR				awchMaker[NTL_MAX_SIZE_CHAR_NAME + 1];
	sITEM_OPTION_SET	sOptionSet;
	DBOTIME				nUseEndTime;
	BYTE				byRestrictState;
};

// marketplace 
struct sTENKAICHIDAISIJYOU_DATA_CLIENT // data for packet
{
	QWORD				nItem;
	BYTE				byTabType;
	BYTE				byItemType;
	BYTE				byItemLevel;
	WCHAR				awchSeller[NTL_MAX_SIZE_CHAR_NAME + 1];
	DWORD				dwPrice;
	DBOTIME				nStartSellTime;
	DBOTIME				nEndSellTime;
	TBLIDX				itemNo;
	BYTE				byCount;
	DWORD				dwNeed_Class_Bit_Flag;
	BYTE				byRank;
	BYTE				byGrade;
	BYTE				byCurrentDurability;
	BYTE				byBattleAttribute;
	WCHAR				awchMaker[NTL_MAX_SIZE_CHAR_NAME + 1];
	sITEM_OPTION_SET	sOptionSet;
	DBOTIME				nUseEndTime;
	BYTE				byRestrictState;
};

#pragma pack()


struct sDROP_ITEM_ETC_INFO
{
	HOBJECT		hInitialOwner;
	sVECTOR3	vMobSpawnLoc;
	PARTYID		partyId;
	TBLIDX		worldId;
};

struct sSELL_ITEM_INFO
{
	TBLIDX	idxItemTbl;
	DWORD	dwPrice;
};

struct sSHOP_BUY_CART
{
	BYTE byMerchantTab;
	BYTE byItemPos;
	BYTE byStack;
};
struct sSHOP_SELL_CART
{
	BYTE byPlace;
	BYTE byPos;
	BYTE byStack;
};

struct sEMPTY_INVEN
{
	BYTE byPlace;
	BYTE byPos;
};


//-----------------------------------------------------------------------------------
// ¼îÇÎÄ«Æ® ±¸Á¶Ã¼
//-----------------------------------------------------------------------------------

#pragma pack(1)

struct sSHOP_BUY_INVEN
{
	BYTE byPlace;
	BYTE byPos;
	BYTE byStack;
	TBLIDX tblItem;
	BYTE byRank;
	BYTE byCurrentDurability;
	BYTE byGrade;
	BYTE byRestrictState;			// eITEM_RESTRICT_STATE_TYPE
	WCHAR awchMaker[NTL_MAX_SIZE_CHAR_NAME + 1]; // Á¦ÀÛÀÚ

	sITEM_OPTION_SET	sOptionSet;

	BYTE byDurationType; //eDURATIONTYPE
	DBOTIME nUseStartTime;// ÀÎº¥Åä¸®¿¡ µé¾î¿Â ³¯Â¥
	DBOTIME nUseEndTime;// »ç¿ë¸¸·á ±â°£
};
#pragma pack()
struct sSHOP_SELL_INVEN
{
	ITEMID itemID;
	BYTE   byStack;
	BYTE   byPlace;
	BYTE   byPos;
};


//-----------------------------------

struct sINVEN_SLOT
{
	BYTE	byPlace;
	BYTE	byPos;
};

struct sITEMID_SLOT : public sINVEN_SLOT
{
	HOBJECT	hItem;
	ITEMID	itemId;
};

struct sHITEM_SLOT : public sINVEN_SLOT
{
	HOBJECT	hItem;
};

// query¿ÍÀÇ Åë½Å¿¡¼­ »ç¿ë
struct sITEM_MOVE_DATA
{
	sITEMID_SLOT	sSrc;
	sITEMID_SLOT	sDest;
};

// client¿ÍÀÇ Åë½Å¿¡¼­ »ç¿ë
struct sITEM_MOVE_INFO
{
	sHITEM_SLOT		sSrc;	// ÀÌµ¿ÇÒ ¾ÆÀÌÅÛ°ú ÀÌµ¿ÇÒ ¾ÆÀÌÅÛÀÌ ÀÖ´Â À§Ä¡.
	sHITEM_SLOT		sDest;	// ¸ñÀûÁö¿¡ ÀÌ¹Ì ÀÖ´Â ¾ÆÀÌÅÛ°ú ¸ñÀûÁöÀÇ À§Ä¡
};


//-----------------------------------------------------------------------------------
// ÀÎº¥Åä¸® °Ë»ç Å¸ÀÔ (¼­¹ö ³»ºÎ¿ë)
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// ÀÎº¥Åä¸® °ü·Ã Á¤ÀÇ
//-----------------------------------------------------------------------------------
const int			NTL_MAX_BAGSLOT_COUNT		= 5; // °¡¹æ ½½·ÔÀÇ °³¼ö

const int			NTL_MAX_BANKSLOT_COUNT		= 4; // Ã¢°í ½½·ÔÀÇ °³¼ö

const int			NTL_MAX_GUILD_BANK_COUNT	= 3; // ±æµå Ã¢°í °³¼ö

const int			NTL_MAX_EQUIP_ITEM_SLOT		= EQUIP_SLOT_TYPE_COUNT;

const int			NTL_MAX_ITEM_SLOT			= 16; // ½½·ÔÀÇ ÃÖ´ë Å©±â

const int			NTL_MAX_BAG_ITEM_SLOT		= 32; // °¡¹æÀÇ ÃÖ´ë Å©±â old 16 / new 32

const int			DBO_ITEM_IN_BAG_COUNT_PER_PACKET = 25; //new

const int			NTL_MAX_SCOUTER_ITEM_SLOT	= 16; // ½ºÄ«¿ìÅÍ °¡¹æÀÇ ÃÖ´ë Å©±â

const int			NTL_MAX_BANK_ITEM_SLOT		= 16; // ÀºÇàÀÇ ÃÖ´ë Å©±â

const int			NTL_MAX_GUILD_ITEM_SLOT		= 32; // ±æµå Ã¢°íÀÇ ÀÎº¥ ÃÖ´ë Å©±â old 16 / new 32

const int			NTL_MAX_NETPYSTORE_ITEM_SLOT = 16; //Netpy StoreÀÇ ÀÎº¥ ÃÖ´ë Å©±â

const int			NTL_MAX_COUNT_GUILD_HAVE_TOTAL_ITEM	= NTL_MAX_GUILD_BANK_COUNT * NTL_MAX_GUILD_ITEM_SLOT; // ±æµå Ã¢°íÀÇ ÃÖ´ë Å©±â

const int			NTL_MAX_BUY_SHOPPING_CART	= 12;  // ¼îÇÎÄ«Æ®ÀÇ »ì¼öÀÖ´Â ÃÖ´ë Å©±â

const int			NTL_MAX_SELL_SHOPPING_CART	= 12;  // ¼îÇÎÄ«Æ®ÀÇ ÆÈ¼öÀÖ´Â ÃÖ´ë Å©±â old 12 new 24

const int			NTL_MAX_SHOPPING_CART		= ( (NTL_MAX_BUY_SHOPPING_CART * sizeof(sSHOP_BUY_CART) ) + (NTL_MAX_SELL_SHOPPING_CART * sizeof(sSHOP_SELL_CART) ) );  // ¼îÇÎÄ«Æ®ÀÇ ÃÖ´ë Å©±â

const int			NTL_MAX_NEWBIE_ITEM			= 10;  // »ý¼º½Ã ºÎ¿©°¡´É ÃÖ´ë ¾ÆÀÌÅÛ old = 8 / new = 10

const int			NTL_MAX_NEWBIE_SKILL		= 7;  // »ý¼º½Ã ºÎ¿©°¡´É ÃÖ´ë ½ºÅ³ old = 3 / new = 7

const int			NTL_MAX_SCOUTER_WATT		= 1000;

const int			NTL_MAX_SET_ITEM_COUNT		= 3;  // jacket pants boots

const int			NTL_SET_ITEM_SOLO_COUNT		= 1;  // solo count

const int			NTL_SET_ITEM_SEMI_COUNT		= 2;  // semi count

const int			NTL_SET_ITEM_FULL_COUNT		= 3;  // full count

const int			NTL_MAX_BUY_HLSSHOP_CART	= 10;  // full count

const int			NTL_MAX_ITEM_COOL_DOWN		= 22;

const int			NTL_AUCTIONHOUSE_REQUIRED_LV = 29;



// Hoi Poi mix level data
struct sHOIPOIMIX_DATA
{
	BYTE			byLevel;	// level
	DWORD			dwExp;		// current exp
};

// È£ÀÌÆ÷ÀÌ ·¹½ÃÇÇ µ¥ÀÌÅÍ
struct sRECIPE_DATA
{
	TBLIDX		recipeTblidx;
	BYTE		byRecipeType;		//eRECIPE_TYPE
};

struct sITEM_BASIC_DATA
{
	ITEMID	nItemID;		// DB¿¡¼­µµ ¾²±â¿¡ intÇüÀ¸·Î
	BYTE	byPlace;
	BYTE	byPos;
	BYTE	byStack;		
};


//-----------------------------------------------------------------------------------
// ¾ÆÀÌÅÛ ¼ö¸® °ü·Ã Á¤ÀÇ
//-----------------------------------------------------------------------------------
struct sITEM_REPAIR
{
	ITEMID itemID;
	BYTE byPlace;
	BYTE byPosition;
	BYTE byDur;			// ¿Ã¸± ¸Æ½º ³»±¸·Â
};

//-----------------------------------------------------------------------------------
// ÀÎº¥Åä¸® °Ë»ç Å¸ÀÔ (¼­¹ö ³»ºÎ¿ë)
//-----------------------------------------------------------------------------------
enum eSTUFF_ENABLE_TYPE
{
	eSTUFF_ENABLE_NONE,			// Pass without inspection
	eSTUFF_ENABLE_BASIC,		// Only basic tests	
	eSTUFF_ENABLE_ALL,			// All inspection

};

//-----------------------------------------------------------------------------------
// ¾ÆÀÌÅÛ »ç¿ë ±â°£
//-----------------------------------------------------------------------------------
enum eDURATIONTYPE
{
	eDURATIONTYPE_NORMAL = 0,
	eDURATIONTYPE_FLATSUM,			//Subscription
	eDURATIONTYPE_METERRATE,		//Decrease duration while moving

	eDURATIONTYPE_COUNT,
	eDURATIONTYPE_INVALID = 0xFF,
};

//-----------------------------------------------------------------------------------
// ±Í¼Ó ¾ÆÀÌÅÛ »óÅÂ
//-----------------------------------------------------------------------------------
enum eITEM_RESTRICT_TYPE
{
	ITEM_RESTRICT_TYPE_NONE = 0,
	ITEM_RESTRICT_TYPE_CHARACTER_GET,
	ITEM_RESTRICT_TYPE_ACCOUNT_GET,
	ITEM_RESTRICT_TYPE_EQUIP,
	ITEM_RESTRICT_TYPE_USE,
	ITEM_RESTRICT_TYPE_DRAGONBALL,
	ITEM_RESTRICT_TYPE_BATTLE_DRAGONBALL,
	ITEM_RESTRICT_TYPE_QUEST,
	ITEM_RESTRICT_TYPE_FOREVER_EQUIP,
	ITEM_RESTRICT_TYPE_FOREVER_CHARACTER_GET,
	ITEM_RESTRICT_TYPE_MAX,
};

enum eITEM_RESTRICT_FUNCTION_FLAG
{
	ITEM_RESTRICT_FUNCTION_FLAG_NONE = 0x0,
	ITEM_RESTRICT_FUNCTION_FLAG_USER_TRADE = 0x1,
	ITEM_RESTRICT_FUNCTION_FLAG_PRIVATE_SHOP = 0x2,
	ITEM_RESTRICT_FUNCTION_FLAG_CHARACTER_STORE = 0x4,
	ITEM_RESTRICT_FUNCTION_FLAG_ACCOUNT_STORE = 0x8,
	ITEM_RESTRICT_FUNCTION_FLAG_DOJO_STORE = 0x10,
	ITEM_RESTRICT_FUNCTION_FLAG_MAIL_ADD = 0x20,
	ITEM_RESTRICT_FUNCTION_FLAG_AUCTION = 0x40,
	ITEM_RESTRICT_FUNCTION_FLAG_BATTLE_DRAGONBALL = 0x80,
	ITEM_RESTRICT_FUNCTION_FLAG_PARTY_TRADE = 0x100,
	ITEM_RESTRICT_FUNCTION_FLAG_MAX = 0x100,
};

//-----------------------------------------------------------------------------------
// restrictions which item can have
//-----------------------------------------------------------------------------------
enum eITEM_RESTRICT_STATE_TYPE
{
	ITEM_RESTRICT_STATE_TYPE_NONE = 0,
	ITEM_RESTRICT_STATE_TYPE_LIMIT,
	ITEM_RESTRICT_STATE_TYPE_CHARACTER_GET,
	ITEM_RESTRICT_STATE_TYPE_ACCOUNT_GET,
	ITEM_RESTRICT_STATE_TYPE_EQUIP,
	ITEM_RESTRICT_STATE_TYPE_USE,
	ITEM_RESTRICT_STATE_TYPE_DRAGONBALL,
	ITEM_RESTRICT_STATE_TYPE_BATTLE_DRAGONBALL,
	ITEM_RESTRICT_STATE_TYPE_QUEST,

	ITEM_RESTRICT_STATE_TYPE_SEAL = 100,
	ITEM_RESTRICT_STATE_TYPE_INSERT_BEAD = 120,
	ITEM_RESTRICT_STATE_TYPE_MAX = 2,
};

enum eDBO_ITEM_CATEGORY
{
	DBO_ITEM_CATEGORY_MAIN_WEAPON = 0x0,
	DBO_ITEM_CATEGORY_SUB_WEAPON = 0x1,
	DBO_ITEM_CATEGORY_JACKET = 0x2,
	DBO_ITEM_CATEGORY_PANTS = 0x3,
	DBO_ITEM_CATEGORY_BOOTS = 0x4,
	DBO_ITEM_CATEGORY_EARRING = 0x5,
	DBO_ITEM_CATEGORY_NECKLACE = 0x6,
	DBO_ITEM_CATEGORY_RING = 0x7,
	DBO_ITEM_CATEGORY_DOGI = 0x8,
	DBO_ITEM_CATEGORY_HAIR = 0x9,
	DBO_ITEM_CATEGORY_MASK = 0xA,
	DBO_ITEM_CATEGORY_RICHNESS = 0xB,
	DBO_ITEM_CATEGORY_BACKPACK = 0xC,
	DBO_ITEM_CATEGORY_BEAD = 0xD,
	DBO_ITEM_CATEGORY_ETC = 0xE,
	DBO_ITEM_CATEGORY_COUNT = 0xF,
	DBO_ITEM_CATEGORY_INVALID = 0xFF,
	DBO_ITEM_CATEGORY_FIRST = 0x0,
	DBO_ITEM_CATEGORY_LAST = 0xE,
};

#pragma pack(1)

// ¾ÆÀÌÅÛ DB Á¤º¸
struct sITEM_DATA
{
	void Init()
	{
		itemId = INVALID_ITEMID;
		itemNo = INVALID_TBLIDX;
		charId = INVALID_CHARACTERID;
		byPlace = INVALID_BYTE;
		byPosition = INVALID_BYTE;
		byStackcount = 0;
		byRank = 0;
		byCurrentDurability = 0;
		bNeedToIdentify = false;
		byGrade = 0;
		byBattleAttribute = 0;
		byRestrictState = 0;
		ZeroMemory(awchMaker, sizeof(awchMaker));
		sOptionSet.Init();
		byDurationType = 0;
		nUseStartTime = 0;
		nUseEndTime = 0;
	}

	ITEMID			itemId;					// Serial Number
	TBLIDX			itemNo;					// Template Number
	CHARACTERID		charId;					// Owner Serial
	BYTE			byPlace;					// eCONTAINER_TYPE
	BYTE			byPosition;
	BYTE			byStackcount;
	BYTE			byRank;
	BYTE			byCurrentDurability;		// ÇöÀç ³»±¸µµ 
	bool			bNeedToIdentify;
	BYTE			byGrade;				// ¾ÆÀÌÅÛ ¾÷±×·¹ÀÌµå µî±Þ
	BYTE			byBattleAttribute;		// Áø±â¸Í¿©¶ô cf) NtlBattle.h eBATTLE_ATTRIBUTE 

	BYTE			byRestrictState;			// eITEM_RESTRICT_STATE_TYPE //current active restriction
	WCHAR			awchMaker[NTL_MAX_SIZE_CHAR_NAME + 1]; // Á¦ÀÛÀÚ

	sITEM_OPTION_SET	sOptionSet;

	BYTE			byDurationType; //eDURATIONTYPE
	DBOTIME			nUseStartTime;// ÀÎº¥Åä¸®¿¡ µé¾î¿Â ³¯Â¥
	DBOTIME			nUseEndTime;// »ç¿ë¸¸·á ±â°£
};

//-----------------------------------------------------------------------------------
// ¾ÆÀÌÅÛ ±â°£ °ü·Ã Á¤ÀÇ
//-----------------------------------------------------------------------------------
struct sITEM_DURATION
{
	ITEMID			itemID;
	BYTE			byPlace;
	BYTE			byPosition;
	DBOTIME			nUseEndTime;// »ç¿ë¸¸·á ±â°£
};


// ¾ÆÀÌÅÛ °£·« Á¤º¸
struct sITEM_BRIEF
{
	TBLIDX			tblidx; // item table index
	BYTE            byGrade; // item effect
	BYTE			byRank;
	BYTE			byBattleAttribute;
};

struct sITEM_SUMMARY
{
	sITEM_SUMMARY():
		tblidx(INVALID_TBLIDX)
	{
	}

	TBLIDX			tblidx; // item table index
	BYTE			byRank;
	BYTE			byGrade;
	BYTE			byBattleAttribute;
};

// ¾ÆÀÌÅÛ »óÅÂ
struct sITEM_STATE
{
	sVECTOR3		vCurLoc; // ÇöÀç À§Ä¡
	bool			bNeedToIdentify;
	bool			bIsNew;
	HOBJECT			hOwner;
	PARTYID			partyID;
	DWORD			dwDropTime;
};


// ¾ÆÀÌÅÛ ÀüÃ¼ Á¤º¸
struct sITEM_PROFILE
{
	sITEM_PROFILE()
	{
		handle = INVALID_HOBJECT;
		tblidx = INVALID_TBLIDX;
	}

	HOBJECT			handle;			// ¾ÆÀÌÅÛ handle
	TBLIDX			tblidx;			// item table index
	BYTE			byPlace;		// eCONTAINER_TYPE
	BYTE			byPos;
	BYTE			byStackcount;
	BYTE			byRank;
	BYTE			byCurDur;		// ³»±¸µµ
	bool			bNeedToIdentify;
	BYTE			byGrade;		// ¾ÆÀÌÅÛ ¾÷±×·¹ÀÌµå µî±Þ
	BYTE			byBattleAttribute;		// Áø±â¸Í¿©¶ô cf) NtlBattle.h eBATTLE_ATTRIBUTE

	BYTE			byRestrictState;		// eITEM_RESTRICT_STATE_TYPE current active restriction
	WCHAR			awchMaker[NTL_MAX_SIZE_CHAR_NAME + 1]; // Á¦ÀÛÀÚ

	sITEM_OPTION_SET	sOptionSet;

	BYTE			byDurationType; //eDURATIONTYPE
	DBOTIME			nUseStartTime;// ÀÎº¥Åä¸®¿¡ µé¾î¿Â ³¯Â¥
	DBOTIME			nUseEndTime;// »ç¿ë¸¸·á ±â°£
	
};


// Zenny °£·« Á¤º¸
struct sMONEY_BRIEF
{
	DWORD			dwZenny;
};


// Zenny »óÅÂ
struct sMONEY_STATE
{
	sVECTOR3		vCurLoc; // ÇöÀç À§Ä¡
	bool			bIsNew;
};


// GameServer¿¡¼­ Query Server·Î ¾ÆÀÌÅÛ »èÁ¦¸¦ ¿äÃ»ÇÒ¶§ »ç¿ë
struct sITEM_DELETE_DATA
{
	BYTE			byPlace;	// Àå¼Ò
	BYTE			byPos;		// À§Ä¡
	HOBJECT			hItem;		// ¾ÆÀÌÅÛ ÇÚµé
	ITEMID			itemId;		// ¾ÆÀÌÅÛ Serial
	TBLIDX			tblidx;     // ¾ÆÀÌÅÛ tblidx		

public:
	sITEM_DELETE_DATA( void )
		: byPlace( INVALID_BYTE )
		, byPos( INVALID_BYTE )
		, hItem( INVALID_HOBJECT )
		, itemId( INVALID_ITEMID )
		, tblidx( INVALID_TBLIDX )
	{
	}

};

//-----------------------------------------------------------------------
// TRADE
//-----------------------------------------------------------------------
struct sTRADE_INVEN
{
	HOBJECT	hItem;			// 
	ITEMID	itemSerial;		// 
	BYTE	byDstPlace;		//
	BYTE	byDstPos;		//
	BYTE	byDstCount;		//
	ITEMID	itemNewSerial;	//
	TBLIDX	itemTblidx; // for logging
};
const int	TRADE_INVEN_MAX_COUNT = 12;
const DWORD NTL_TRADE_UPDATE_INTERVAL = 5000;	//5 Second
const DWORD NTL_TRADE_REPLY_WAIT_TIME = 30000;	// If no response after 30 seconds of time waiting transaction automatically canceled upon request
const float	DBO_TRADE_REQUEST_RANGE	= 10.0f; // Trade range

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//  [11/20/2006 niam]
// ¾ÆÀÌÅÛÀ» ¿©·¯°³ »ý¼º È¤Àº »èÁ¦ ÇÒ ¶§ »ç¿ë
//-----------------------------------------------------------------------


// ¼­ºê µ¥ÀÌÅÍ
//	- ¾ÆÀÌÅÛ »ý¼º Á¤º¸ ÀÌ¿Ü¿¡ °ÔÀÓÀ» ÁøÇàÇÏ´Âµ¥¿¡ ÇÊ¿äÇÑ Ãß°¡ÀûÀÎ Á¤º¸µéÀ» ÀúÀå
//	- ÀÏ¹ÝÀûÀ¸·Î Äõ¸®¼­¹ö¿¡¼­ µ¥ÀÌÅÍ Á¶ÀÛµîÀº ÇÏÁö¾Ê°í ¹ÞÀº ±×´ë·Î ´Ù½Ã °ÔÀÓ ¼­¹ö·Î µÇµ¹·Á ÁØ´Ù.



// ITEM_CREATE_EX ¸Þ½ÃÁö°¡ ÇÑ¹ø¿¡ »ý¼ºÇÒ ¼ö ÀÖ´Â ÃÖ´ë ¾ÆÀÌÅÛÀÇ °¹¼ö
const int	ITEM_CREATE_EX_MAX_COUNT = 10;

// ITEM_CREATE_EX ¸Þ½ÃÁö°¡ ÇÑ¹ø¿¡ »èÁ¦ÇÒ ¼ö ÀÖ´Â ÃÖ´ë ¾ÆÀÌÅÛÀÇ °¹¼ö
const int	ITEM_DELETE_EX_MAX_COUNT = 10;



//-----------------------------------------------------------------------
//	Desc : GQ_CREATE_ITEM_EX_REQ ¿¡¼­ »ç¿ëÇÏ´Â °ø¿ëÃ¼ Á¤ÀÇ
//

struct sITEM_TS_SUB_DATA
{
	BYTE		byTSType;				// eTSM_MANAGER_TYPE     TS_TYPE_QUEST_CS, TS_TYPE_PC_TRIGGER_CS, TS_TYPE_OBJECT_TRIGGER_S
	HOBJECT		hObject;				// Trigger Object ÀÇ ÇÚµé ObjectTrigger¿¡¼­ »ç¿ëÇÑ´Ù
	sTSM_SERIAL	sSerial;
};



// ¾ÆÀÌÅÛ »ý¼º½Ã ÇÁ·ÎÅäÄÝ¿¡ µé¾î°¡´Â ¼­ºê µ¥ÀÌÅÍ¸¦ °¡Áö´Â °ø¿ëÃ¼
union uITEM_CREATE_SUB_DATA
{
	sITEM_TS_SUB_DATA	sTSSubData;		// Trigger System ¿¡¼­ »ç¿ë
};


//-----------------------------------------------------------------------
//	Desc : GQ_DELETE_ITEM_EX_REQ ¿¡¼­ »ç¿ëÇÏ´Â °ø¿ëÃ¼ Á¤ÀÇ
//



// ¾ÆÀÌÅÛ »èÁ¦½Ã ÇÁ·ÎÅäÄÝ¿¡ µé¾î°¡´Â ¼­ºê µ¥ÀÌÅÍ¸¦ °¡Áö´Â °ø¿ëÃ¼
union uITEM_DELETE_SUB_DATA
{
	sITEM_TS_SUB_DATA	sTSSubData;		// Trigger System ¿¡¼­ »ç¿ë
};

struct sITEM_POSITION_DATA
{
	HOBJECT			hItem;		// ¾ÆÀÌÅÛ ÇÚµé
	BYTE			byPlace;	// Àå¼Ò
	BYTE			byPos;		// À§Ä¡
};

struct sITEM_SORT_DATA_MOVE
{
	HOBJECT			hItem;
	BYTE			byPlace;
	BYTE			byPos;
};

struct sITEM_SORT_DATA_DEL
{
	HOBJECT			hItem;
	BYTE			byUnknown; //seems like pos from bag slot?? idk..
};

//-----------------------------------------------------------------------------
// ¾ÆÀÌÅÛ ÀÚµ¿ ÀåÂø

struct sITEM_AUTO_EQUIP_DATA
{
	// ÀÌµ¿ ¾ÆÀÌÅÛ
	ITEMID					srcItemId;
	HOBJECT					hSrcItem;
	BYTE					bySrcEquipPlace;
	BYTE					bySrcEquipPos;

	BYTE					byDestPlace;	// ÀÌµ¿ÇØ °¥ °÷(ºñ¾îÀÖ´Â À§Ä¡ÀÌ¾î¾ß ÇÑ´Ù.)
	BYTE					byDestPos;

	// »ý¼º & ÀåÂø ÇÏ´Â ¾ÆÀÌÅÛ
	sITEM_DATA				sEquipItem;
};


struct sITEM_AUTO_EQUIP_ROLLBACK_DATA
{
	// »èÁ¦ ¾ÆÀÌÅÛ
	ITEMID					delItemId;
	HOBJECT					hDelEquipItem;
};


//-----------------------------------------------------------------------------

WORD Dbo_GetFinalOffence(WORD wBase, BYTE byGrade);
WORD Dbo_GetFinalDefence(WORD wBase, BYTE byGrade);

const BYTE			NTL_MAX_PRIVATESHOP_INVENTORY			= 36;	// °³ÀÎ »óÁ¡¿¡ ÃÖ´ë Å©±â (ÁÖÀÇ : sizeof(BYTE)º¸´Ù Å©¸é ¾ÈµÈ´Ù) (°³ÀÎ»óÁ¡ GUIÀÇ ½½·Ô °³¼öÀÇ ¹è¼ö·Î ¿Ã¶ó°¡´øÁö ³»·Á°¡¾ß ÇÑ´Ù)
const int			NTL_MAX_PRIVATESHOP_BUY_SHOPPING_CART	= 12;	// ¼îÇÎÄ«Æ®ÀÇ »ì¼öÀÖ´Â ÃÖ´ë Å©±â
const BYTE			PRIVATESHOP_ITEMPOS_INVALID				= 0xFF;
const DWORD			NTL_PRIVATESHOP_ITEM_BUY_ZENNY_DEFAULT	= 1;	//°³ÀÎ»óÁ¡ ÆÈ¶§ ±âº» Á¦´Ï°ª

//-- SGpro --------------------------------------------------------------------------
//  Purpose : Game Srv¿Í Query Srv »çÀÌ¿¡¼­ »ç¿ë[7/2/2007 SGpro]
//  Return  : 
//-----------------------------------------------------------------------------------
struct sPRIVATESHOP_ITEM_POS_DATA
{
	bool	bIsSaveItem; //trueÀÌ¸é Shop¿¡ ¾ÆÀÌÅÛÀÌ ÀúÀåµÈ °Í (Buy ¶§ Query Srv°¡ ÇÊ¿äÇÔ)
	BYTE	byPlace;
	BYTE	byPos; //°³ÀÎ»óÁ¡ ÀÎº¥Åä¸® Slot Pos
	BYTE	byInventoryPlace; //ÀÎº¥Åä¸®ÀÇ Place (Buy ¶§ Query Srv°¡ ÇÊ¿äÇÔ)
	BYTE	byInventoryPos;//ÀÎº¥Åä¸®ÀÇ Pos (Buy ¶§ Query Srv°¡ ÇÊ¿äÇÔ)
	ITEMID	itemID;
};

struct sINVEN_ITEM_POS_DATA
{
	BYTE byPlace;
	BYTE byPos;
	ITEMID	itemID;
};

//-- SGpro --------------------------------------------------------------------------
//  Purpose : [7/2/2007 SGpro]
//  Return  :
//-----------------------------------------------------------------------------------
enum ePRIVATESHOP_STATE
{
	 PRIVATESHOP_STATE_CLOSE			= 0 //¿ÀÇÂÀü
	,PRIVATESHOP_STATE_OPEN					//¿ÀÇÂÈÄ
	,PRIVATESHOP_STATE_NONE					//°³ÀÎ»óÁ¡ »óÅÂ°¡ ¾Æ´Ï´Ù
	,PRIVATESHOP_STATE_NULL					//°³ÀÎ»óÁ¡ÀÌ ÇÑ¹øµµ »ý¼ºµÇÁö ¾ÊÀº »óÅÂ
	,PRIVATESHOP_STATE_DATALOADING			//Query Srv¿¡¼­ µ¥ÀÌÅ¸°¡ ¿À±â¸¦ ±â´Ù¸®´Â »óÅÂ
};

// »ý¼ºÀÚ¸¦ ¸¸µéÁö ¸¶¼¼¿ä. 
// ³»ºÎ¿¡¼­ »ç¿ëµË´Ï´Ù. [10/2/2007 SGpro]
struct sSUMMARY_PRIVATESHOP_SHOP_DATA
{
	bool bIsOwnerEmpty; //true if the owner is away
	BYTE byShopState; //Shop State ÀúÀå(ePRIVATESHOP_STATE)
	WCHAR wcPrivateShopName[NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1];
};


//-- server & client ³»ºÎ PrivateShop Data --------------------------------------
struct sPRIVATESHOP_SHOP_DATA
{
	HOBJECT hOwner;								// »óÁ¡ ÁÖÀÎ CharacterID	
	PSHOP_WAITTIME waittimeCashShopStart;		// À¯·á ¼¥ÀÌ µÈ ³¯Â¥
	PSHOP_WAITTIME waittimeCashShopEnd;			// À¯·á ¼¥ ¸¸·áÀÏ
	sSUMMARY_PRIVATESHOP_SHOP_DATA sSummaryPrivateShopData;	
	WCHAR wcNotice[NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1];

public: 
	sPRIVATESHOP_SHOP_DATA()
		: hOwner ( INVALID_HOBJECT )
		, waittimeCashShopStart ( 0 )
		, waittimeCashShopEnd ( 0 )
	{
		sSummaryPrivateShopData.bIsOwnerEmpty = false;
		sSummaryPrivateShopData.byShopState = PRIVATESHOP_STATE_NULL;
		ZeroMemory( sSummaryPrivateShopData.wcPrivateShopName,	sizeof(WCHAR) * ( NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1 ) );
		ZeroMemory( wcNotice,			sizeof(WCHAR) * ( NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1 ) );
	}
};//end of struct sPRIVATESHOP_SHOP_DATA


// -- ÆÐÅ¶¿ë PrivateShop DataÀÓ ------------------------------------------------------
// wcNotice°¡ °¡º¯À¸·Î º¯ÇØ¾ß ÇÏ±â ¶§¹®¿¡ struct sPRIVATESHOP_SHOP_DATA¿Í ºÐ¸®¸¦ Çß´Ù
// Notice´Â °¡º¯ SizeÀÌ±â ¶§¹®¿¡ ¹Ýµå½Ã 
// struct sPACKET_PRIVATESHOP_SHOP_DATA¿¡ °¡Àå ¹Ø¿¡ ÀÖ¾î¾ß µÈ´Ù [10/2/2007 SGpro]
struct sPACKET_PRIVATESHOP_SHOP_DATA
{
	HOBJECT hOwner;// »óÁ¡ ÁÖÀÎ CharacterID	
	PSHOP_WAITTIME waittimeCashShopStart; // À¯·á ¼¥ÀÌ µÈ ³¯Â¥
	PSHOP_WAITTIME waittimeCashShopEnd; // À¯·á ¼¥ ¸¸·áÀÏ
	sSUMMARY_PRIVATESHOP_SHOP_DATA sSummaryPrivateShopData;	
	BYTE byNoticeSize;
	WCHAR wcNotice[NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1];

public: 
	sPACKET_PRIVATESHOP_SHOP_DATA()
		: hOwner ( INVALID_HOBJECT )
		, waittimeCashShopStart ( 0 )
		, waittimeCashShopEnd ( 0 )
		, byNoticeSize ( 0 )
	{
		sSummaryPrivateShopData.bIsOwnerEmpty = false;
		sSummaryPrivateShopData.byShopState = PRIVATESHOP_STATE_NONE;
		ZeroMemory( sSummaryPrivateShopData.wcPrivateShopName,	sizeof(WCHAR) * ( NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1 ) );
		ZeroMemory( wcNotice,			sizeof(WCHAR) * ( NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1 ) );
	}
};// end of struct sPRIVATESHOP_SHOP_DATA

// -- SGpro --------------------------------------------------------------------------
//  Purpose : [7/2/2007 SGpro]
//  Return  :
// -----------------------------------------------------------------------------------
struct sPRIVATESHOP_ITEM_DATA
{
	enum eITEMSTATE
	{
		PRIVATESHOP_GENERAL					= 0,	// General
		PRIVATESHOP_VISITOR_SELECT_ITEM,			// Steamed item
		PRIVATESHOP_VISITOR_UNSELECT_ITEM,			// Unpacking steamed items
		PRIVATESHOP_INVALID					= 0xFF,
	};

	HOBJECT							hItem;
	BYTE							byPrivateShopInventorySlotPos;
	bool							bIsSaveItem; // If true, the item will be saved in the Shop
	sITEM_DATA						sItem;// bIsSaveItemÀÇ °ªÀÌ trueÀÏ °æ¿ì Ã¤¿ö¼­ UserPC¿¡ º¸³½´Ù
	DWORD							dwZenny;
	BYTE							byItemState; // eITEMSTATE

	sPRIVATESHOP_ITEM_DATA()
		: hItem ( INVALID_HOBJECT )
		, byPrivateShopInventorySlotPos ( PRIVATESHOP_ITEMPOS_INVALID )
		, dwZenny ( NTL_PRIVATESHOP_ITEM_BUY_ZENNY_DEFAULT )
		, byItemState ( PRIVATESHOP_INVALID )
		, bIsSaveItem ( false )
	{
		ZeroMemory( &sItem, sizeof( sItem ));
	}
};


// ItemUpgrade
//new
struct sDBO_UPGRADED_ITEM_ABILITY
{
  float afAdditionalAbilityWeapon[NTL_ITEM_MAX_GRADE];
  float afAdditionalAbilityArmor[NTL_ITEM_MAX_GRADE];
};

//-----------------------------------------------------------------------------------
// ¾ÆÀÌÅÛ UPGRADE Scouter Add Rate Á¤ÀÇ
//-----------------------------------------------------------------------------------
enum eITEM_UPGRADE_RATE
{
	ITEM_UPGRADE_RATE_0 = 0,//-10.0f,	
	ITEM_UPGRADE_RATE_1 ,//-5.0f,			
	ITEM_UPGRADE_RATE_2 ,// 0.0f,			
	ITEM_UPGRADE_RATE_3 ,// 5.0f,			
	ITEM_UPGRADE_RATE_4 ,// 10.0f,			
	ITEM_UPGRADE_RATE_5 ,// 20.0f,	
};

enum eITEM_UPGRADE_RESULT
{
	ITEM_UPGRADE_RESULT_SUCCESS,	// upgrade success
	ITEM_UPGRADE_RESULT_FAIL,		// upgrade failes
	ITEM_UPGRADE_RESULT_RESET,		// upgrade failed and item grade set to 0
	ITEM_UPGRADE_RESULT_DESTROY,	// upgrade failed and item destroyed
};


// Obtain the probability of complying with the attribute.
BYTE Dbo_GetHoipoiStoneCount( const BYTE byStoneType, const BYTE byItemType, BYTE byGrade );
// ¾ÆÀÌÅÛ Å¸ÀÔ°ú µî±Þ¿¡ µû¸¥ ¼Ò¿ä È£ÀÌÆ÷ÀÌ °¹¼ö °¡Á®¿À±â
DWORD Dbo_GetRepairPay( DWORD dwCost, BYTE byDur, BYTE byCurDur);
// ¾ÆÀÌÅÛ ¹Í½º EXP
DWORD Dbo_GetHoipoiMixEXP(bool bIsSuccess, BYTE byMadeLevel, BYTE byNeedMixLevel);


// Item Copy
bool Dbo_SetItemData( sITEM_DATA* const pItemData, ITEMID itemId, TBLIDX itemNo, CHARACTERID charId, BYTE byPlace, BYTE byPosition, BYTE byStackcount, BYTE byRank, BYTE byCurrentDurability, bool bNeedToIdentify, BYTE byGrade, BYTE byBattleAttribute, BYTE byRestrictType, const WCHAR* const awchMaker, TBLIDX* const aOptionTblidx, BYTE byDurationType, DBOTIME nUseStartTime, DBOTIME nUseEndTime );
bool Dbo_SetItemData_CheckNeedToIdentify( sITEM_DATA* const pItemData, ITEMID itemId, TBLIDX itemNo, CHARACTERID charId, BYTE byPlace, BYTE byPosition, BYTE byStackcount, BYTE byRank, BYTE byCurrentDurability, bool bNeedToIdentify, BYTE byGrade, BYTE byBattleAttribute, BYTE byRestrictType, WCHAR* const awchMaker, TBLIDX* const aOptionTblidx, BYTE byDurationType, DBOTIME nUseStartTime, DBOTIME nUseEndTime );
// Item Profile
bool Dbo_SetItemProfile( sITEM_PROFILE* const pItemProfile, HOBJECT handle, TBLIDX tblidx, BYTE byPlace, BYTE byPos, BYTE byStackcount, BYTE byRank, BYTE byCurDur, bool bNeedToIdentify, BYTE byGrade, BYTE byBattleAttribute, BYTE byRestrictType, WCHAR* const awchMaker, TBLIDX* const aOptionTblidx, BYTE byDurationType, DBOTIME nUseStartTime, DBOTIME nUseEndTime );
// Bank Item Profile
//bool Dbo_SetBankItemProfile( sBANK_PROFILE* const pItemProfile, HOBJECT handle, TBLIDX tblidx, BYTE byPos, BYTE byStackcount, BYTE byRank, BYTE byCurDur, bool bNeedToIdentify, BYTE byGrade, BYTE byBattleAttribute, BYTE byRestrictType, WCHAR* const awchMaker, TBLIDX* const aOptionTblidx, BYTE byDurationType, DBOTIME nUseStartTime, DBOTIME nUseEndTime );


///////////NEW
bool	Dbo_CheckClass( const BYTE byClass, const DWORD dwNeedClassBitFlag );
DWORD	Dbo_GetAuctionHouseSellFee(DWORD dwCost);


DWORD	Dbo_GetItemRestrictFlagType(BYTE byItemRestrictType, BYTE byItemRestrictState); //byte 1 = restrict type from table. byte 2 = current restrict from item(DB)

float	Dbo_GetRandomOptionSystemEffectRate(BYTE byRvType, WORD wMaxValue, BYTE byValue, float fAttack_Physical_Revision, float fAttack_Energy_Revision, float fDefence_Physical_Revision, float fDefence_Energy_Revision );

bool	IsBagContainer(BYTE byContainr);
bool	IsInvenContainer(BYTE byContainr);
bool	IsEquipContainer(BYTE byContainr);
bool	IsBankContainer(BYTE byContainr);
bool	IsGuildContainer(BYTE byContainr);
BYTE	GetBagSlotPositionByContainerType(eCONTAINER_TYPE container);
BYTE	GetContainerTypeByBagSlotPosition(eBAGSLOT_POSITION eBagSlotPosition);

float	GetDisassembleHigherRankRate(BYTE byItemRank, BYTE byItemNeedMinLevel, BYTE byHigherRank, BYTE byItemNeedMaxLevel);

bool	Dbo_NeedToSaveItemCoolTime(BYTE byItemCoolTimeGroup);

BYTE	Dbo_GetItemCategoryByItemType(BYTE byItemType);
float	Dbo_GetItemWorth(BYTE byItemRank, BYTE byItemLevel);
BYTE	GetRequiredSealItemNum(BYTE byRank, BYTE byGrade);

//DboGetPowderItemCount
DWORD	Dbo_GetChargeItemBattleAttributeChange(BYTE byRank, BYTE byItemLv);
float	Dbo_GetChangeItemBattleAttributeSuccessRate(BYTE byAttributePosNum);
void	Dbo_GetChangeItemBattleAttributeEachRate(BYTE byAdditionalAttribute, int *panEachRate);

bool	IsSealItem(BYTE byRestrictState);
bool	IsSealing(BYTE byRestrictType, BYTE byRestrictState, BYTE byEquipType);
bool	CanChangeOption(BYTE byEquip_Type, BYTE byItemRank, BYTE byRestrictState, BYTE byNeed_Min_Level);

bool	GetInsertBeadByDurationType(BYTE byItem_Type);

BYTE	GetDefaultRestrictState(BYTE byRestrictType, BYTE byItemType, bool bActivate);	//used when creating new item

BYTE	GetItemTypeGroup(BYTE byItemType); //weapon, armor, ETC

bool	IsValidStateToUseItem(TBLIDX itemIdx, WORD wNeed_State_Bit_Flag, BYTE byCurStateId, BYTE byCurAspectStateId, QWORD qwCurCharCondition, bool bIsSwimmingState, bool bIsJumpingState, BYTE byAirState);



struct sDBO_ITEM_COOL_TIME
{
	BYTE			byItemCoolTimeGroupIndex;	//eITEM_COOL_TIME_GROUP
	DWORD			dwInitialItemCoolTime;		//Max cd time
	DWORD			dwItemCoolTimeRemaining;	//CD time left
};

struct sITEM_CREATE_BRIEF
{
	BYTE		byStack;
	TBLIDX		tblidx;
};

struct sITEM_BULK_CREATE_SUB_DATA
{
	TBLIDX		tblidx;
	BYTE		byPlace;
	BYTE		byPos;
	BYTE		byKeyPlace;
	BYTE		byKeyPos;
};

//NEW
enum eITEM_MATERIAL_TYPE
{
  ITEM_MATERIAL_TYPE_ETC = 0,
  ITEM_MATERIAL_TYPE_STONE,
  ITEM_MATERIAL_TYPE_WOOD,
  ITEM_MATERIAL_TYPE_FABRIC,
  ITEM_MATERIAL_TYPE_SKIN,
  ITEM_MATERIAL_TYPE_MATAL,

  ITEM_MATERIAL_TYPE_COUNT,
  ITEM_MATERIAL_TYPE_FIRST = ITEM_MATERIAL_TYPE_ETC,
  ITEM_MATERIAL_TYPE_LAST = ITEM_MATERIAL_TYPE_MATAL,
};

enum eITEM_MATERIAL_TYPE_BIT_FLAG
{
  ITEM_MATERIAL_TYPE_BIT_FLAG_NOTHING = MAKE_BIT_FLAG( ITEM_MATERIAL_TYPE_ETC ),
  ITEM_MATERIAL_TYPE_BIT_FLAG_STONE = MAKE_BIT_FLAG( ITEM_MATERIAL_TYPE_STONE ),
  ITEM_MATERIAL_TYPE_BIT_FLAG_WOOD = MAKE_BIT_FLAG( ITEM_MATERIAL_TYPE_WOOD ),
  ITEM_MATERIAL_TYPE_BIT_FLAG_FABRIC = MAKE_BIT_FLAG( ITEM_MATERIAL_TYPE_FABRIC ),
  ITEM_MATERIAL_TYPE_BIT_FLAG_SKIN = MAKE_BIT_FLAG( ITEM_MATERIAL_TYPE_SKIN ),
  ITEM_MATERIAL_TYPE_BIT_FLAG_MATAL = MAKE_BIT_FLAG( ITEM_MATERIAL_TYPE_MATAL ),

  ITEM_MATERIAL_TYPE_BIT_FLAG_START = ITEM_MATERIAL_TYPE_BIT_FLAG_STONE,
  ITEM_MATERIAL_TYPE_BIT_FLAG_LAST = ITEM_MATERIAL_TYPE_BIT_FLAG_MATAL,
};

enum eITEM_RANDOM_OPTION_TYPE
{
  eITEM_RANDOM_OPTION_TYPE_NORMAL = 0,
  eITEM_RANDOM_OPTION_TYPE_BEAD01 = 6,
  eITEM_RANDOM_OPTION_TYPE_BEAD02 = 7,
  eITEM_RANDOM_OPTION_TYPE_COUNT,

  eITEM_RANDOM_OPTION_TYPE_INVALID = 0xFF,
  eITEM_RANDOM_OPTION_TYPE_MAX_BEAD = 0x2,
};

enum eITEM_CREATE_TYPE
{
	eITEM_CREATE_NORMAL,
	eITEM_CREATE_TRADE,
	eITEM_CREATE_UNSTACK,
	eITEM_CREATE_DROP_PICKUP,
	eITEM_CREATE_EXCHANGE_BUY,
};

enum eITEM_USE_VALID_STATE
{
	ITEM_USE_VALID_STATE_NOT_TRANSFORMED = 0, // item only use-able when player is not transformed
	ITEM_USE_VALID_STATE_STANDING, //item only use-able when player is standing
	ITEM_USE_VALID_STATE_NOT_UNKNOWN2,
	ITEM_USE_VALID_STATE_NOT_UNKNOWN3,
	ITEM_USE_VALID_STATE_NOT_UNKNOWN4,
	ITEM_USE_VALID_STATE_NOT_UNKNOWN5,
	ITEM_USE_VALID_STATE_NOT_UNKNOWN6,
	ITEM_USE_VALID_STATE_NOT_UNKNOWN7,

	ITEM_USE_VALID_STATE_COUNT,
};

#pragma pack()
