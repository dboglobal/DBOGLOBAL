#pragma once

#include "NtlSharedType.h"
#include "NtlVector.h"
enum eAIR_STATE;
class CNtlBitFlagManager;


// Skill related enumeration
enum eSKILL_CLASS
{
	NTL_SKILL_CLASS_PASSIVE = 0,
	NTL_SKILL_CLASS_ACTIVE,
	NTL_SKILL_CLASS_HTB,

	NTL_SKILL_CLASS_UNKNOWN = 0xFF,

	NTL_SKILL_CLASS_FIRST = NTL_SKILL_CLASS_PASSIVE,
	NTL_SKILL_CLASS_LAST = NTL_SKILL_CLASS_HTB,
};


// Skill type
enum eSKILL_TYPE
{
	NTL_SKILL_TYPE_NONE = 0,
	NTL_SKILL_TYPE_PHYSICAL,
	NTL_SKILL_TYPE_ENERGY,
	NTL_SKILL_TYPE_STATE,

	NTL_SKILL_TYPE_FIRST = NTL_SKILL_TYPE_NONE,
	NTL_SKILL_TYPE_LAST = NTL_SKILL_TYPE_STATE,

	NTL_SKILL_TYPE_COUNT = NTL_SKILL_TYPE_LAST - NTL_SKILL_TYPE_FIRST + 1
};


// skill active type
enum eSKILL_ACTIVE_TYPE
{
	SKILL_ACTIVE_TYPE_DD, // Direct Damage ( LP Decrease )
	SKILL_ACTIVE_TYPE_DOT, // Damage Over Time
	SKILL_ACTIVE_TYPE_DH, // Direct Heal
	SKILL_ACTIVE_TYPE_HOT, // Heal Over Time
	SKILL_ACTIVE_TYPE_DB, // Direct Bless ( Over the treatment of dokdeung, Teleport, ... )
	SKILL_ACTIVE_TYPE_BB, // Bless Buff
	SKILL_ACTIVE_TYPE_DC, // Direct Curse ( EP / RP such as reduced, ... )
	SKILL_ACTIVE_TYPE_CB, // Curse Buff

	SKILL_ACTIVE_TYPE_UNKNOWN				= 0xFF,

	SKILL_ACTIVE_TYPE_FIRST = SKILL_ACTIVE_TYPE_DD,
	SKILL_ACTIVE_TYPE_LAST = SKILL_ACTIVE_TYPE_CB
};

enum eDBO_SKILL_APPOINT_TARGET
{
	DBO_SKILL_APPOINT_TARGET_SELF = 0,
	DBO_SKILL_APPOINT_TARGET_TARGET,
	DBO_SKILL_APPOINT_TARGET_POINT,

	DBO_SKILL_APPOINT_TARGET_COUNT,

	DBO_SKILL_APPOINT_TARGET_UNKNOWN = 0xFF,

	DBO_SKILL_APPOINT_TARGET_FIRST = DBO_SKILL_APPOINT_TARGET_SELF,
	DBO_SKILL_APPOINT_TARGET_LAST = DBO_SKILL_APPOINT_TARGET_COUNT - 1
};

enum eDBO_SKILL_APPLY_TARGET
{
	DBO_SKILL_APPLY_TARGET_SELF = 0,
	DBO_SKILL_APPLY_TARGET_ENEMY,
	DBO_SKILL_APPLY_TARGET_ALLIANCE,
	DBO_SKILL_APPLY_TARGET_PARTY,
	DBO_SKILL_APPLY_TARGET_MOB_PARTY,
	DBO_SKILL_APPLY_TARGET_ANY,
	DBO_SKILL_APPLY_TARGET_SUMMON, //new
	DBO_SKILL_APPLY_TARGET_ANY_NPC, //new
	DBO_SKILL_APPLY_TARGET_ANY_MOB, //new
	DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE, //new

	DBO_SKILL_APPLY_TARGET_COUNT,

	DBO_SKILL_APPLY_TARGET_UNKNOWN = 0xFF,

	DBO_SKILL_APPLY_TARGET_FIRST = DBO_SKILL_APPLY_TARGET_SELF,
	DBO_SKILL_APPLY_TARGET_LAST = DBO_SKILL_APPLY_TARGET_COUNT - 1
};

enum eDBO_SKILL_APPLY_RANGE
{
	DBO_SKILL_APPLY_RANGE_SINGLE = 0,
	DBO_SKILL_APPLY_RANGE_CIRCLE,
	DBO_SKILL_APPLY_RANGE_RECTANGLE,
	DBO_SKILL_APPLY_RANGE_FORWARD,
	DBO_SKILL_APPLY_RANGE_FLEXIBLE_FORWARD, //new
	DBO_SKILL_APPLY_RANGE_DONUT, //new
	DBO_SKILL_APPLY_RANGE_FAN, //new

	DBO_SKILL_APPLY_RANGE_COUNT,

	DBO_SKILL_APPLY_RANGE_UNKNOWN = 0xFF,

	DBO_SKILL_APPLY_RANGE_FIRST = DBO_SKILL_APPLY_RANGE_SINGLE,
	DBO_SKILL_APPLY_RANGE_LAST = DBO_SKILL_APPLY_RANGE_COUNT - 1
};

enum eDBO_SYSTEM_EFFECT_RESULT_TYPE
{
	DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL = 0,
	DBO_SYSTEM_EFFECT_RESULT_TYPE_DH_HOT,
	DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT,
	DBO_SYSTEM_EFFECT_RESULT_TYPE_LP_EP_STEAL,

	DBO_SYSTEM_EFFECT_RESULT_TYPE_COUNT,

	DBO_SYSTEM_EFFECT_RESULT_TYPE_UNKNOWN = 0xFF,

	DBO_SYSTEM_EFFECT_RESULT_TYPE_FIRST = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL,
	DBO_SYSTEM_EFFECT_RESULT_TYPE_LAST = DBO_SYSTEM_EFFECT_RESULT_TYPE_COUNT - 1
};

// WARNING :
// You can add a new RP bonus type but MUST NOT REMOVE one. These index numbers are synchronized with DB.
// by YOSHIKI(2007-11-26)
enum eDBO_RP_BONUS_TYPE
{
	DBO_RP_BONUS_TYPE_KNOCKDOWN = 0,
	DBO_RP_BONUS_TYPE_RESULT_PLUS,				// ABSOLUTE
	DBO_RP_BONUS_TYPE_EP_MINUS,					// PERCENT
	DBO_RP_BONUS_TYPE_KEEP_TIME_PLUS,			// ABSOLUTE
	DBO_RP_BONUS_TYPE_CASTING_TIME_MINUS,		// ABSOLUTE
	DBO_RP_BONUS_TYPE_COOL_TIME_MINUS,			// ABSOLUTE
	DBO_RP_BONUS_TYPE_GUARD_CRASH,

	DBO_RP_BONUS_TYPE_COUNT,
	DBO_RP_BONUS_TYPE_INVALID = 0xFF,

	DBO_RP_BONUS_TYPE_FIRST = DBO_RP_BONUS_TYPE_KNOCKDOWN,
	DBO_RP_BONUS_TYPE_LAST = DBO_RP_BONUS_TYPE_COUNT - 1,
};

enum eDBO_RP_BONUS_SLOT
{
	DBO_RP_BONUS_SLOT_RESULT_PLUS,
	DBO_RP_BONUS_SLOT_EP_MINUS,
	DBO_RP_BONUS_SLOT_COOL_TIME_MINUS,
	DBO_RP_BONUS_SLOT_CASTING_TIME_MINUS,
	DBO_RP_BONUS_SLOT_KEEP_TIME_PLUS,
	DBO_RP_BONUS_SLOT_CHANCE_KNOCK_DOWN_TARGET = 4,
	DBO_RP_BONUS_SLOT_GUARD_CRASH,
};

//January 21, 2009 to change the DBO_BLOCK_MODE_TYPE DBO_GUARD_TYPE. (Occupant)
enum eDBO_GUARD_TYPE
{
	DBO_GUARD_TYPE_DEFENCE = 0,
	DBO_GUARD_TYPE_DAMAGE_SKILL,
	DBO_GUARD_TYPE_CURSE_SKILL,
	DBO_GUARD_TYPE_KNOCKDOWN,
	DBO_GUARD_TYPE_HTB,

	DBO_GUARD_TYPE_COUNT,
	DBO_GUARD_TYPE_INVALID = 0xFF,

	DBO_GUARD_TYPE_FIRST = DBO_GUARD_TYPE_DEFENCE,
	DBO_GUARD_TYPE_LAST  = DBO_GUARD_TYPE_COUNT - 1
};

enum eDBO_STUN_TYPE
{
	DBO_STUN_TYPE_GENERAL = 0,
	DBO_STUN_TYPE_FROZEN,
	DBO_STUN_TYPE_TIED,
	DBO_STUN_TYPE_STONE,
	DBO_STUN_TYPE_CANDY,

	DBO_STUN_TYPE_COUNT,
	DBO_STUN_TYPE_INVALID = 0xFF,

	DBO_STUN_TYPE_FIRST = DBO_STUN_TYPE_GENERAL,
	DBO_STUN_TYPE_LAST = DBO_STUN_TYPE_COUNT - 1
};

enum eDBO_SKILL_PASSIVE_EFFECT
{
	DBO_SKILL_PASSIVE_EFFECT_CASTING_TIME_DOWN = 0,
	DBO_SKILL_PASSIVE_EFFECT_COOL_TIME_DOWN,
	DBO_SKILL_PASSIVE_EFFECT_SYSTEM_EFFECT_VALUE_UP,
	DBO_SKILL_PASSIVE_EFFECT_KEEP_TIME_UP,
	DBO_SKILL_PASSIVE_EFFECT_REQUIRED_EP_DOWN,
	DBO_SKILL_PASSIVE_EFFECT_APPLY_RANGE_TYPE,
	DBO_SKILL_PASSIVE_EFFECT_APPLY_AREA_SIZE_UP,
	DBO_SKILL_PASSIVE_EFFECT_USE_RANGE_MAX_UP,

	DBO_SKILL_PASSIVE_EFFECT_COUNT,

	DBO_SKILL_PASSIVE_EFFECT_INVALID = 0xFFui8,

	DBO_SKILL_PASSIVE_EFFECT_FIRST = DBO_SKILL_PASSIVE_EFFECT_CASTING_TIME_DOWN,
	DBO_SKILL_PASSIVE_EFFECT_LAST = DBO_SKILL_PASSIVE_EFFECT_COUNT - 1,
};

enum eSKILL_FUNCTION
{
	SKILL_FUNCTION_VOLATILE_BUFF = 0,				// Buff disappears when reconnecting
	SKILL_FUNCTION_ONLY_LOW_LP,						// Available only when LP is less than 50% of maximum LP
	SKILL_FUNCTION_IGNORE_AFFECT_TIME,				// Affecting time unconditionally set to 0 regardless of animation
	SKILL_FUNCTION_OVERWRITE_IF_SAME_GROUP,			// Buff group When the same buff exists, it is unconditionally replaced with the latest buff
	SKILL_FUNCTION_STRONG_CASTING,					// Skill that does not delay casting even if attacked
	SKILL_FUNCTION_SPECIAL_COOL_TIME_RULE,			// Cooling time count directly controlled by the server
	SKILL_FUNCTION_SPECIAL_TARGET,					// Show skill targets to nearby players
	SKILL_FUNCTION_FORCED_KNOCKDOWN,				// Unconditional knockdown regardless of skill ability
	SKILL_FUNCTION_CANT_BE_TARGETTED,				// Can not be targeted
	SKILL_FUNCTION_APPLY_NOT_ME,					// Exclude yourself when choosing targets to which skill effects apply
	SKILL_FUNCTION_STOP_AUTO_ATTACK,				// Turn off automatic attack mode after using skill
	SKILL_FUNCTION_SET_INVINCIBLE,					// Invulnerability while using skill
	SKILL_FUNCTION_FAINTING_AFTER_APPLYING,			// Skill effect after applying the skill to become fainted
	SKILL_FUNCTION_DAMAGE_CASTING_CANCEL,
	SKILL_FUNCTION_COOLTIME_INVINCIBILITY,
	SKILL_FUNCTION_CASTINGTIME_INVINCIBILITY,
	SKILL_FUNCTION_BLESS_DEBUFF_INVINCIBILITY,
	SKILL_FUNCTION_SUB_BUFF_SYSTEM,
	SKILL_FUNCTION_INFINITE_DURATION,
	SKILL_FUNCTION_INVISIBLE_ICON,

	SKILL_FUNCTION_COUNT,

	SKILL_FUNCTION_FIRST = SKILL_FUNCTION_VOLATILE_BUFF,
	SKILL_FUNCTION_LAST = SKILL_FUNCTION_COUNT - 1
};

enum eSKILL_FUNCTION_FLAG
{
	SKILL_FUNCTION_FLAG_VOLATILE_BUFF = 1 << SKILL_FUNCTION_VOLATILE_BUFF,
	SKILL_FUNCTION_FLAG_ONLY_LOW_LP = 1 << SKILL_FUNCTION_ONLY_LOW_LP,
	SKILL_FUNCTION_FLAG_IGNORE_AFFECT_TIME = 1 << SKILL_FUNCTION_IGNORE_AFFECT_TIME,
	SKILL_FUNCTION_FLAG_OVERWRITE_IF_SAME_GROUP = 1 << SKILL_FUNCTION_OVERWRITE_IF_SAME_GROUP,
	SKILL_FUNCTION_FLAG_STRONG_CASTING = 1 << SKILL_FUNCTION_STRONG_CASTING,
	SKILL_FUNCTION_FLAG_SPECIAL_COOL_TIME_RULE = 1 << SKILL_FUNCTION_SPECIAL_COOL_TIME_RULE,
	SKILL_FUNCTION_FLAG_SPECIAL_TARGET = 1 << SKILL_FUNCTION_SPECIAL_TARGET,
	SKILL_FUNCTION_FLAG_FORCED_KNOCKDOWN = 1 << SKILL_FUNCTION_FORCED_KNOCKDOWN,
	SKILL_FUNCTION_FLAG_CANT_BE_TARGETTED = 1 << SKILL_FUNCTION_CANT_BE_TARGETTED,
	SKILL_FUNCTION_FLAG_APPLY_NOT_ME = 1 << SKILL_FUNCTION_APPLY_NOT_ME,
	SKILL_FUNCTION_FLAG_STOP_AUTO_ATTACK = 1 << SKILL_FUNCTION_STOP_AUTO_ATTACK,
	SKILL_FUNCTION_FLAG_SET_INVINCIBLE = 1 << SKILL_FUNCTION_SET_INVINCIBLE,
	SKILL_FUNCTION_FLAG_FAINTING_AFTER_APPLYING = 1 << SKILL_FUNCTION_FAINTING_AFTER_APPLYING,
	SKILL_FUNCTION_FLAG_DAMAGE_CASTING_CANCEL = 1 << SKILL_FUNCTION_DAMAGE_CASTING_CANCEL,
	SKILL_FUNCTION_FLAG_COOLTIME_INVINCIBILITY = 1 << SKILL_FUNCTION_COOLTIME_INVINCIBILITY,
	SKILL_FUNCTION_FLAG_CASTINGTIME_INVINCIBILITY = 1 << SKILL_FUNCTION_CASTINGTIME_INVINCIBILITY,
	SKILL_FUNCTION_FLAG_BLESS_DEBUFF_INVINCIBILITY = 1 << SKILL_FUNCTION_BLESS_DEBUFF_INVINCIBILITY,
	SKILL_FUNCTION_FLAG_SUB_BUFF_SYSTEM = 1 << SKILL_FUNCTION_SUB_BUFF_SYSTEM,
	SKILL_FUNCTION_FLAG_INFINITE_DURATION = 1 << SKILL_FUNCTION_INFINITE_DURATION,
	SKILL_FUNCTION_FLAG_INVISIBLE_ICON = 1 << SKILL_FUNCTION_INVISIBLE_ICON,
};

enum eSKILL_TRANSFORM_USE_INFO
{
	SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_NORMAL = 0,
	SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_GREAT_NAMEK,
	SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_SUPER_SAIYAN,
	SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_PURE_MAJIN,
	SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_KAIOKEN,
	SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_SPINNING_ATTACK,
	SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_VEHICLE,
	SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_MIND_CURSE,
	SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_BODY_CURSE,
	SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_CHANGE_CURSE,
	SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_ROLLING_ATTACK,
	SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_RABIES,

	SKILL_TRANSFORM_USE_INFO_COUNT,

	SKILL_TRANSFORM_USE_INFO_FIRST = SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_NORMAL,
	SKILL_TRANSFORM_USE_INFO_LAST = SKILL_TRANSFORM_USE_INFO_COUNT - 1
};

enum eSKILL_TRANSFORM_USE_INFO_FLAG
{
	SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_NORMAL = 1 << SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_NORMAL,
	SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_GREAT_NAMEK = 1 << SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_GREAT_NAMEK,
	SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_SUPER_SAIYAN = 1 << SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_SUPER_SAIYAN,
	SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_PURE_MAJIN = 1 << SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_PURE_MAJIN,
	SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_KAIOKEN = 1 << SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_KAIOKEN,
	SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_SPINNING_ATTACK = 1 << SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_SPINNING_ATTACK,
	SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_VEHICLE = 1 << SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_VEHICLE,
	SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_MIND_CURSE = 1 << SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_MIND_CURSE,
	SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_BODY_CURSE = 1 << SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_BODY_CURSE,
	SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_CHANGE_CURSE = 1 << SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_CHANGE_CURSE,
	SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_ROLLING_ATTACK = 1 << SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_ROLLING_ATTACK,
	SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_RABIES = 1 << SKILL_TRANSFORM_USE_INFO_ABLE_WHEN_RABIES,
};

//-----------------------------------------------------------------------------------
// 스킬 이펙트 코드 정의
//-----------------------------------------------------------------------------------
#ifdef DEF_SYSTEM_EFFECT_CODE
	#undef DEF_SYSTEM_EFFECT_CODE
#endif
#define DEF_SYSTEM_EFFECT_CODE(c, can_cast_when_being_interrupted) c,

enum eSYSTEM_EFFECT_CODE
{
	#include "NtlSystemEffect.h"

	MAX_SYSTEM_EFFECT_CODE,

	INVALID_SYSTEM_EFFECT_CODE = 0xFFFFFFFF
};
//-----------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------
// 스킬 관련 정의
//-----------------------------------------------------------------------------------
const unsigned int NTL_MAX_PC_HAVE_SKILL = 60; //new 60 old 120

const unsigned int NTL_MAX_NPC_HAVE_SKILL = 7;

const unsigned int NTL_MAX_BLESS_BUFF_CHARACTER_HAS = 16;
const unsigned int NTL_MAX_CURSE_BUFF_CHARACTER_HAS = 16;
const unsigned int NTL_MAX_SUB_BUFF_CHARACTER_HAS = 16;

const unsigned int DBO_MAX_BUFF_CHARACTER_HAS = NTL_MAX_BLESS_BUFF_CHARACTER_HAS + NTL_MAX_CURSE_BUFF_CHARACTER_HAS + NTL_MAX_SUB_BUFF_CHARACTER_HAS;

const unsigned int NTL_MAX_EFFECT_IN_SKILL = 2;
const unsigned int NTL_MAX_ARGUMENT_IN_EFFECT = 3; //new

const int NTL_SYSTEM_EFFECT_1 = 0;

const int NTL_SYSTEM_EFFECT_2 = 1;

const unsigned int NTL_EFFECT_APPLY_INTERVAL = 2000;		// in millisecs.

const unsigned int NTL_MAX_NUMBER_OF_SKILL_TARGET = 32; //old 8 new 32

const BYTE INVALID_BUFF_GROUP = 0xFF;

const float DBO_BACKSTAB_DAMAGE_BONUS_IN_PERCENT = 50.0f;		// in percent

const unsigned int DBO_MAX_RP_BONUS_COUNT_PER_SKILL = 6;

const float DBO_DEFAULT_BLOCK_MODE_SUCCESS_RATE = 70.0f;		// in percent

const float DBO_MAX_BLOCK_MODE_SUCCESS_RATE = 70.0f;		// in percent

const float DBO_MAX_CRITICAL_BLOCK_SUCCESS_RATE = 100.0f;		// in percent

const float DBO_CONFUSE_SCAN_DISTANCE_PC = 20.0f;				// in meters.

const float DBO_MAX_TERROR_MOVEMENT_DISTANCE = 30.0f;			// in meters.

const float DBO_SYSTEM_EFFECT_PULL_DISTANCE = 1.0f;				// in meters.

const DWORD DBO_MIN_POWER_LEVEL_WHEN_HIDING_KI = 1;

const float DBO_KEEPING_EFFECT_VALID_DISTANCE_BUFFER = 10.0f;		// in meters.

const float DBO_STRIKE_RUSH_MOVEMENT_SPEED = 60.0f;					// in m/s
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// HTB 관련 정의
//-----------------------------------------------------------------------------------
const DWORD			NTL_HTB_MAX_PC_HAVE_HTB_SKILL				= 2;	// The maximum number of HTB skills the PC has // 30
const DWORD			NTL_HTB_MAX_SKILL_COUNT_IN_SET				= 10;	// The maximum number of skills that can be included in an HTB skill set
const BYTE			DBO_HTB_PREREQUISITE_RP_BALL				= 1;	// Minimum RP ball required to write HTB
//-----------------------------------------------------------------------------------


// AI skill condition information [2/15/2008 SGpro]
enum eSKILLPRIORITY
{
	SKILLPRIORITY_CALL = 0				// not used [2/15/2008 SGpro]
	, SKILLPRIORITY_START				// not used [2/15/2008 SGpro]
	, SKILLPRIORITY_FAINTING				// not used [2/15/2008 SGpro]
	, SKILLPRIORITY_LP					// If lp is smaller than lp of table, cool time becomes, and it is delayed than table time, [10/29/2008 SGpro]
	, SKILLPRIORITY_GIVE					// If lp is smaller than lp in table, it is cool time, and it is delayed than table time, [10/29/2008 SGpro]
	, SKILLPRIORITY_TIME					// If the time and cool time of the table are appropriate for the condition, [10/29/2008 SGpro]
	, SKILLPRIORITY_RINGRANGE			// Skills are activated if there are enemies in the donuts area. [8/12/2008 SGpro]
	, SKILLPRIORITY_ONLYLP				// If lp is lower than lp of table and it is cool time, it will activate the skill, only once (it will play again when the energy is full and then decreased again) [10/29/2008 SGpro]
};

enum eSKILL_USE_TYPE
{
	SKILL_USE_TYPE_NORMAL = 0x0,
	SKILL_USE_TYPE_SERVICE_SCS,

	SKILL_USE_TYPE_COUNT,

	SKILL_USE_TYPE_FIRST = 0,
	SKILL_USE_TYPE_LAST = SKILL_USE_TYPE_COUNT - 1,
};

enum eBUFF_TYPE
{
	BUFF_TYPE_BLESS = 0x0,
	BUFF_TYPE_CURSE,
	BUFF_TYPE_SUB,

	BUFF_TYPE_COUNT,
	BUFF_TYPE_INVALID = 0xFF,
};


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const char *		NtlGetSystemEffectString(DWORD dwSystemEffectCode);
const bool			NtlCanCastWhenBeingInterrupted(eSYSTEM_EFFECT_CODE eSystemEffectCode);
extern bool			Dbo_IsForBlessBuff(BYTE byActiveEffectType);
extern bool			Dbo_IsForCurseBuff(BYTE byActiveEffectType);
extern bool			Dbo_IsHarmfulEffectType(BYTE byActiveEffectType);
extern bool			Dbo_IsCurseType(BYTE byActiveEffectType);
extern DWORD		Dbo_GetLevelUpGainSP(BYTE byLevel, BYTE byLevelDiff = 1);
extern DWORD		Dbo_GetLevelSP(BYTE byLevel);
extern bool			Dbo_IsAdvantageSkillType(BYTE byActiveEffectType);
extern DWORD		Dbo_GetRequiredZennyForNpcSkillInit(BYTE byLevel);
extern bool			Dbo_IsValidStateToUseSkill(DWORD dwTransformUseInfoBitFlag, BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, bool bIsSwimmingState, bool bIsJumpingState, BYTE eAirState, WORD *pwResultCode);

extern bool			Dbo_IsState_GreatNamek(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition);
extern bool			Dbo_IsState_SuperSaiyan(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition);
extern bool			Dbo_IsState_PureMajin(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition);
extern bool			Dbo_IsState_Kaioken(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition);
extern bool			Dbo_IsState_SpinningAttack(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition);
extern bool			Dbo_IsState_Vehicle(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition);
extern bool			Dbo_IsState_MindCurse(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition);
extern bool			Dbo_IsState_BodyCurse(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition);
extern bool			Dbo_IsState_ChangeCurse(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition);
extern bool			Dbo_IsState_RollingAttack(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition);
extern bool			Dbo_IsState_Rabies(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition);

extern bool			Dbo_IsSystemEffectForMindCurse(eSYSTEM_EFFECT_CODE eSystemEffect);
extern bool			Dbo_IsSystemEffectForBodyCurse(eSYSTEM_EFFECT_CODE eSystemEffect);
extern bool			Dbo_IsSystemEffectForStun(WORD wSystemEffect);
extern bool			Dbo_IsSystemEffectForChangeCurse(eSYSTEM_EFFECT_CODE eSystemEffect);
extern bool			Dbo_IsSystemEffectForFly(eSYSTEM_EFFECT_CODE eSystemEffect);
extern bool			Dbo_IsSystemEffectForFlyDash(eSYSTEM_EFFECT_CODE eSystemEffect);
extern bool			Dbo_IsSystemEffectForFlyAccel(eSYSTEM_EFFECT_CODE eSystemEffect);

extern bool			Dbo_IsCcReduction(eSYSTEM_EFFECT_CODE eSystemEffect);

//-----------------------------------------------------------------------------------

struct sSKILL_TARGET_LIST
{
	sSKILL_TARGET_LIST::sSKILL_TARGET_LIST()
	{
		Init();
	}

	void Init()
	{
		byTargetCount = 0;
		memset(ahTarget, INVALID_HOBJECT, sizeof(ahTarget));
	}

	bool AddTarget(HOBJECT hObject)
	{
		if (byTargetCount < NTL_MAX_NUMBER_OF_SKILL_TARGET)
		{
			ahTarget[byTargetCount++] = hObject;
			return true;
		}
		return false;
	}

	bool IsExist(HOBJECT hObject)
	{
		for (int j = 0; j < byTargetCount; j++)
		{
			if (hObject == ahTarget[j])
				return true;
		}
		return false;
	}

	BYTE			byTargetCount;
	HOBJECT			ahTarget[NTL_MAX_NUMBER_OF_SKILL_TARGET];
};


// Skill related packet structure
#pragma pack(1)
struct sDBO_LP_EP_RECOVERED
{
	void		Reset()
	{
		bIsLpRecoveredWhenHit = false;
		targetLpRecoveredWhenHit = 0;
		bIsEpRecoveredWhenHit = false;
		dwTargetEpRecoveredWhenHit = 0;
	}

	/*void operator= (sDBO_LP_EP_RECOVERED& recover)
	{
		bIsLpRecoveredWhenHit = recover.bIsLpRecoveredWhenHit;
		targetLpRecoveredWhenHit = recover.targetLpRecoveredWhenHit;
		bIsEpRecoveredWhenHit = recover.bIsEpRecoveredWhenHit;
		dwTargetEpRecoveredWhenHit = recover.dwTargetEpRecoveredWhenHit;
	}*/

	bool		bIsLpRecoveredWhenHit;
	int			targetLpRecoveredWhenHit;
	bool		bIsEpRecoveredWhenHit;
	DWORD		dwTargetEpRecoveredWhenHit;
};

// System Effect 결과
struct sDBO_SYSTEM_EFFECT_RESULT
{

	eDBO_SYSTEM_EFFECT_RESULT_TYPE		eResultType;

	union
	{
		float					fResultValue;

		// DH/HOT
		struct
		{
			float						DH_HOT_fHealAmount_Lp;
			float						DH_HOT_fHealAmount_Ep;
		};

		// DD/DOT
		struct
		{
			float						DD_DOT_fDamage;
			float						DD_DOT_fAbsorbedDamage;
			float						DD_DOT_fReflectedDamage;
			sDBO_LP_EP_RECOVERED		DD_DOT_lpEpRecovered; //new
		};

		// ACTIVE_LP_STEAL, ACTIVE_EP_STEAL, ACTIVE_LP_STEAL_OVER_TIME, ACTIVE_EP_STEAL_OVER_TIME
		struct
		{
			float						LP_EP_STEAL_fLpDamage;
			float						LP_EP_STEAL_fEpDamage;
			float						LP_EP_STEAL_fAbsorbedDamage;
			float						LP_EP_STEAL_fReflectedDamage;
			float						LP_EP_STEAL_fLpGained;
			float						LP_EP_STEAL_fEpGained;
		};
	};
};

// SKILL Result value
struct sSKILL_RESULT
{
	HOBJECT						hTarget;
	BYTE						byAttackResult;		// eBATTLE_ATTACK_RESULT
	sDBO_SYSTEM_EFFECT_RESULT	effectResult[NTL_MAX_EFFECT_IN_SKILL];
	BYTE						byBlockedAction;		// eDBO_GUARD_TYPE
	int							damageByReflectingCurse; //new
	sVECTOR3					vShift;
	sVECTOR3					vFinalSubjectLoc;
};

struct sSKILL_DATA
{	// Character's skill storage information
	BYTE				skillIndex; //old bySlot
	TBLIDX				skillId;
	int					nRemainSec;
	int					nExp;
	BYTE				byRpBonusType;
	bool				bIsRpBonusAuto;
};

struct sBUFF_DATA
{
	BYTE					buffIndex; //new
	TBLIDX					sourceTblidx;
	BYTE					bySourceType;		// eDBO_OBJECT_SOURCE
	DWORD					dwInitialDuration;
	DWORD					dwTimeRemaining;		// in Millisecs.
	int						anArgument1[3];
	int						anArgument2[3];
	long double				effectValue1;
	long double				effectValue2;
	BYTE					byBuffGroup;		//
	bool					bIsDBUpdateFiltering;
};


struct sPET_BUFF_DATA
{
	BYTE				byAvatarType;		// eDBO_AVATAR_TYPE
	BYTE				buffIndex; //new
	BYTE				bySourceType;		// eDBO_OBJECT_SOURCE
	TBLIDX				sourceTblidx;
	DWORD				dwInitialDuration;
	DWORD				dwTimeRemaining;		// in Millisecs.
	int					anArgument1[NTL_MAX_ARGUMENT_IN_EFFECT];
	int					anArgument2[NTL_MAX_ARGUMENT_IN_EFFECT];
	double			effectValue1;
	double			effectValue2;
};


// HTB skill data (for DB loading)
struct sHTB_SKILL_DATA
{
	TBLIDX			skillId; // HTB Skill ID
	DWORD			dwSkillTime; // Remaining time(in millisecs.)
};


// Skill information
struct sSKILL_INFO
{
	TBLIDX			tblidx; // skill tblidx
	DWORD			dwTimeRemaining;		// in millisecs.
	int				nExp;
	BYTE			skillIndex; // old bySlotId;
	BYTE			byRpBonusType;			// eDBO_RP_BONUS_TYPE
	bool			bIsRpBonusAuto;
};


// Pet Skill Information
struct sSKILL_INFO_PET
{
	TBLIDX			tblidx; // skill tblidx
	BYTE			skillIndex; // old bySlotIndex;
};


union uDBO_BUFF_PARAMETER
{
	float		fParameter;
	
	struct
	{
		float		DOT_fDamage;
		float		DOT_fBonusDamage;
		float		DOT_fReflecetedDamage;
		sDBO_LP_EP_RECOVERED		DOT_lpEpRecovered;
	};
	struct
	{
		float		HOT_fHealAmount_Lp;
		float		HOT_fHealAmount_Ep;
	};
	struct
	{
		float		ACTIVE_LP_EP_STEAL_OVER_TIME_fDamage;
		bool		ACTIVE_LP_EP_STEAL_OVER_TIME_bInPercent;
		float		ACTIVE_LP_EP_STEAL_OVER_TIME_fRatio;
	};
	struct
	{
		TBLIDX		commonConfigTblidx;
		DWORD		dwRemainTime;
		DWORD		dwRemainValue;
	};
	struct
	{
		float		ACTIVE_VIABILITY_fParameter;
		bool		ACTIVE_VIABILITY_bIsEffectActivated;
	};
};

enum eDBO_BUFF_PARAMETER_TYPE
{
  DBO_BUFF_PARAMETER_TYPE_DEFAULT = 0,
  DBO_BUFF_PARAMETER_TYPE_DOT,
  DBO_BUFF_PARAMETER_TYPE_HOT,
  DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_STEAL_OVER_TIME,
  DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_AUTO_RECOVER,

  DBO_BUFF_PARAMETER_TYPE_COUNT,
  DBO_BUFF_PARAMETER_TYPE_FIRST = DBO_BUFF_PARAMETER_TYPE_DEFAULT,
  DBO_BUFF_PARAMETER_TYPE_LAST = DBO_BUFF_PARAMETER_TYPE_COUNT - 1,
};

//new
struct sDBO_BUFF_PARAMETER
{
	sDBO_BUFF_PARAMETER() : byBuffParameterType(DBO_BUFF_PARAMETER_TYPE_DEFAULT)
	{
		::ZeroMemory(&buffParameter, sizeof(uDBO_BUFF_PARAMETER));
	}

	BYTE					byBuffParameterType;
	uDBO_BUFF_PARAMETER		buffParameter;
};

struct sBUFF_INFO
{
	sBUFF_INFO()
	{
		buffIndex = INVALID_BYTE;
		sourceTblidx = INVALID_TBLIDX;
		bySourceType = INVALID_BYTE;
		dwInitialDuration = INVALID_DWORD;
		dwTimeRemaining = INVALID_DWORD;
		bActive = false;
	}

	BYTE					buffIndex; //new
	TBLIDX					sourceTblidx;
	BYTE					bySourceType;			// eDBO_OBJECT_SOURCE
	DWORD					dwInitialDuration;
	DWORD					dwTimeRemaining;		// in millisecs.
	bool					bActive; //set buff active or inactive
	sDBO_BUFF_PARAMETER		aBuffParameter[NTL_MAX_EFFECT_IN_SKILL];
};


// HTB Skill data (for client delivery)
struct sHTB_SKILL_INFO
{
	TBLIDX			skillId; // HTB Skill ID
	BYTE			bySlotId;
	DWORD			dwTimeRemaining; // millisecs
};


// HTB Skill result
struct sHTBSKILL_RESULT
{
	BYTE			byStep; // Current HTB step
	sSKILL_RESULT	sSkillResult;
};

struct sDBO_UPDATED_SKILL_DATA
{
	BYTE			bySkillPassiveEffect;		// eDBO_SKILL_PASSIVE_EFFECT
	union
	{
		// DBO_SKILL_PASSIVE_EFFECT_CASTING_TIME_DOWN
		float		fCasting_Time;

		// DBO_SKILL_PASSIVE_EFFECT_COOL_TIME_DOWN
		WORD		wCool_Time;

		// DBO_SKILL_PASSIVE_EFFECT_SYSTEM_EFFECT_VALUE_UP
		double		aSkill_Effect_Value[NTL_MAX_EFFECT_IN_SKILL]; //change from float to double

		// DBO_SKILL_PASSIVE_EFFECT_KEEP_TIME_UP
		WORD		wKeep_Time;

		// DBO_SKILL_PASSIVE_EFFECT_REQUIRED_EP_DOWN
		WORD		wRequire_EP;

		// DBO_SKILL_PASSIVE_EFFECT_APPLY_RANGE_TYPE
		BYTE		byApply_Range;

		// DBO_SKILL_PASSIVE_EFFECT_APPLY_AREA_SIZE_UP
		struct
		{
			BYTE	byApply_Area_Size_1;
			BYTE	byApply_Area_Size_2;
		};

		// DBO_SKILL_PASSIVE_EFFECT_USE_RANGE_MAX_UP
		BYTE		byUse_Range_Max;
	};
};


struct sBUFF_MAP
{
	BYTE					buffIndex; //new
	TBLIDX					sourceTblidx;
	BYTE					bySourceType;			// eDBO_OBJECT_SOURCE
	DWORD					dwInitialDuration;
	DWORD					dwTimeRemaining;		// in millisecs.
	bool					bActive; //set buff active or inactive
	sDBO_BUFF_PARAMETER		aBuffParameter[NTL_MAX_EFFECT_IN_SKILL];
	HOBJECT					hCaster;
	TBLIDX					auiEffectCode[NTL_MAX_EFFECT_IN_SKILL];
	BYTE					byBuffType;//eBUFF_TYPE
};


#pragma pack()