//***********************************************************************************
//
//	File		:	NtlCharacterState.h
//
//	Begin		:	2006-03-31
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlBitFlag.h"
#include "NtlSharedType.h"
#include "NtlVector.h"
#include "NtlSkill.h"
#include "NtlItem.h"
#include "NtlMovement.h"



//-----------------------------------------------------------------------------------
// Character State
//-----------------------------------------------------------------------------------
enum eCHARSTATE
{
	CHARSTATE_SPAWNING,				// Spawn (character when created)
	CHARSTATE_DESPAWNING,			// Despawn (character when deleted)
	CHARSTATE_STANDING,				// STANDING
	CHARSTATE_SITTING,				// SITTING
	CHARSTATE_FAINTING,				// DIE/DEAD
	CHARSTATE_CAMPING,				// IDLE/AFK
	CHARSTATE_LEAVING,				// LEAVING

	CHARSTATE_MOVING,				// Typical fields move (RUNNING, WALKING, JUMPING, including DASHING & FLYING)
	CHARSTATE_DESTMOVE,				// MOVE TO DESTINATION
	CHARSTATE_FOLLOWING,			// FOLLOWING
	CHARSTATE_FALLING,				// FALLING
	CHARSTATE_DASH_PASSIVE,			// Passive dash
	CHARSTATE_TELEPORTING,			// Of teleportation (a character such as moving to Antwerp)
	CHARSTATE_SLIDING,				// SLIDING
	CHARSTATE_KNOCKDOWN,			// KNOCK DOWN

	CHARSTATE_FOCUSING,				// Concentrated state (RP bonus use during the early stages)
	CHARSTATE_CASTING,				// Of casting
	CHARSTATE_SKILL_AFFECTING,		// Application of the skills
	CHARSTATE_KEEPING_EFFECT,		// Skill / item effect keeping state
	CHARSTATE_CASTING_ITEM,			// Cast of items

	CHARSTATE_STUNNED,				// Stun
	CHARSTATE_SLEEPING,				// Sleeping
	CHARSTATE_PARALYZED,			// Paralyzed

	CHARSTATE_HTB,					// HTB를 실행하고 있음
	CHARSTATE_SANDBAG,				// HTB를 당하고 있음
	CHARSTATE_CHARGING,				// CHARGING RP
	CHARSTATE_GUARD,				// Guard mode

	CHARSTATE_PRIVATESHOP,			// PRIVATE SHOP
	CHARSTATE_DIRECT_PLAY,			// Directed character state
	CHARSTATE_OPERATING,			// Object action state
	CHARSTATE_RIDEON,				// USED FOR THE BUS SYSTEM
	CHARSTATE_TURNING,				// TURNING CHARACTER (VEHICLE)

	CHARSTATE_AIR_JUMP,				//START FLYING INTO AIR
	CHARSTATE_AIR_DASH_ACCEL,		//FLYING ACCEL

	CHARSTATE_COUNT,
	INVALID_CHARSTATE = 0xFF

};



//-----------------------------------------------------------------------------------
// Aspect ( 캐릭터 특이 상태 : 변신등 )중복불가 개념
//-----------------------------------------------------------------------------------
enum eASPECTSTATE
{
	ASPECTSTATE_SUPER_SAIYAN,		// 수퍼 사이어인
	ASPECTSTATE_PURE_MAJIN,			// 순수 마인
	ASPECTSTATE_GREAT_NAMEK,		// 그레이트 나메크인
	ASPECTSTATE_KAIOKEN,				// 계왕권
	ASPECTSTATE_SPINNING_ATTACK,		// 회전 공격
	ASPECTSTATE_VEHICLE,			// 탈 것
	ASPECTSTATE_ROLLING_ATTACK,

	ASPECTSTATE_COUNT,
	ASPECTSTATE_INVALID = 0xFF
};


//-----------------------------------------------------------------------------------
// Condition, 중복가능 개념
//-----------------------------------------------------------------------------------
enum eCHARCONDITION
{
	CHARCOND_INVISIBLE,				// Invisible 1
	CHARCOND_HIDING_KI,				// Hide url 2
	CHARCOND_INVINCIBLE,			// invincibility 4
	CHARCOND_TAUNT,					// Fixed target 8
	CHARCOND_ATTACK_DISALLOW,		// No attack 16
	CHARCOND_TRANSPARENT,			// Transparent (for GM) 32
	CHARCOND_CANT_BE_TARGETTED,		// No target 64
	CHARCOND_DIRECT_PLAY,			// Director of the (PC: when directing the client, NPC or MOB: necessary when the production server) 128

	CHARCOND_BLEEDING,				// Bleeding 256
	CHARCOND_POISON,				// Poisoned 512
	CHARCOND_STOMACHACHE,			// Stomache 1024
	CHARCOND_BURN,					// Burning 2048
	CHARCOND_CONFUSED,				// Confused 4096
	CHARCOND_TERROR,				// Terror 8192
	CHARCOND_BARRIER,				// Neutralize damage 16.384
	CHARCOND_DAMAGE_REFLECTION,		// Damage reflected 32.768

	CHARCOND_AFTEREFFECT,			// Transforming aftermath 65.536
	CHARCOND_CHARGING_BLOCKED,		// No group gathering 131.072
	CHARCOND_FAKE_DEATH,			// Feign Death 262.144
	CHARCOND_NULLIFIED_DAMAGE,		// Damage to invalidate the 524.288

	CHARCOND_CLICK_DISABLE,			// Click prohibited characters 1.048.576
	CHARCOND_CLIENT_UI_DISABLE,		// Characters associated ui output prohibition on the client side (menus, etc.) 2.097.152

	CHARCOND_TAIYOU_KEN, // 4.194.304	//white screen on target
	CHARCOND_BATTLE_INABILITY,// 8.388.608
	CHARCOND_SKILL_INABILITY,// 16.777.216
	CHARCOND_REVIVAL_AFTEREFFECT,	// revival sequelae 33.554.432
	CHARCOND_LP_AUTO_RECOVER,// 67.108.864
	CHARCOND_EP_AUTO_RECOVER,// 134.217.728
	CHARCOND_RABIES,// 268.435.456
	CHARCOND_DRUNK,// 536.870.912
	CHARCOND_EXCITATION_MALE,// 1.073.741.824
	CHARCOND_EXCITATION_FEMALE,// 2.147.483.648

	CHARCONDITION_COUNT,
	INVALID_CHARCONDITION = 0xFF,

	CHARCOND_FIRST = CHARCOND_INVISIBLE,
	CHARCOND_LAST = CHARCONDITION_COUNT - 1,
};

enum eCHARCONDITION_FLAG
{
	CHARCOND_FLAG_INVISIBLE				= (0x01 << CHARCOND_INVISIBLE),
	CHARCOND_FLAG_HIDING_KI				= (0x01 << CHARCOND_HIDING_KI),
	CHARCOND_FLAG_INVINCIBLE			= (0x01 << CHARCOND_INVINCIBLE),
	CHARCOND_FLAG_TAUNT					= (0x01 << CHARCOND_TAUNT),
	CHARCOND_FLAG_ATTACK_DISALLOW		= (0x01 << CHARCOND_ATTACK_DISALLOW),
	CHARCOND_FLAG_TRANSPARENT			= (0x01 << CHARCOND_TRANSPARENT),
	CHARCOND_FLAG_CANT_BE_TARGETTED		= (0x01 << CHARCOND_CANT_BE_TARGETTED),
	CHARCOND_FLAG_DIRECT_PLAY			= (0x01 << CHARCOND_DIRECT_PLAY),

	CHARCOND_FLAG_BLEEDING				= (0x01 << CHARCOND_BLEEDING),
	CHARCOND_FLAG_POISON				= (0x01 << CHARCOND_POISON),
	CHARCOND_FLAG_STOMACHACHE			= (0x01 << CHARCOND_STOMACHACHE),
	CHARCOND_FLAG_BURN					= (0x01 << CHARCOND_BURN),
	CHARCOND_FLAG_CONFUSED				= (0x01 << CHARCOND_CONFUSED),
	CHARCOND_FLAG_TERROR				= (0x01 << CHARCOND_TERROR),
	CHARCOND_FLAG_BARRIER				= (0x01 << CHARCOND_BARRIER),
	CHARCOND_FLAG_DAMAGE_REFLECTION		= (0x01 << CHARCOND_DAMAGE_REFLECTION),

	CHARCOND_FLAG_AFTEREFFECT			= (0x01 << CHARCOND_AFTEREFFECT),
	CHARCOND_FLAG_CHARGING_BLOCKED		= (0x01 << CHARCOND_CHARGING_BLOCKED),
	CHARCOND_FLAG_FAKE_DEATH			= (0x01 << CHARCOND_FAKE_DEATH),
	CHARCOND_FLAG_NULLIFIED_DAMAGE		= (0x01 << CHARCOND_NULLIFIED_DAMAGE),

	CHARCOND_FLAG_CLICK_DISABLE			= (0x01 << CHARCOND_CLICK_DISABLE),
	CHARCOND_FLAG_CLIENT_UI_DISABLE		= (0x01 << CHARCOND_CLIENT_UI_DISABLE),

	CHARCOND_FLAG_TAIYOU_KEN			= (0x01 << CHARCOND_TAIYOU_KEN),
	CHARCOND_FLAG_BATTLE_INABILITY		= (0x01 << CHARCOND_BATTLE_INABILITY),
	CHARCOND_FLAG_SKILL_INABILITY		= (0x01 << CHARCOND_SKILL_INABILITY),
	CHARCOND_FLAG_REVIVAL_AFTEREFFECT	= (0x01 << CHARCOND_REVIVAL_AFTEREFFECT),
	CHARCOND_FLAG_LP_AUTO_RECOVER		= (0x01 << CHARCOND_LP_AUTO_RECOVER),
	CHARCOND_FLAG_EP_AUTO_RECOVER		= (0x01 << CHARCOND_EP_AUTO_RECOVER),
	CHARCOND_FLAG_RABIES				= (0x01 << CHARCOND_RABIES),
	CHARCOND_FLAG_DRUNK					= (0x01 << CHARCOND_DRUNK),
	CHARCOND_FLAG_EXCITATION_MALE		= (0x01 << CHARCOND_EXCITATION_MALE),
	CHARCOND_FLAG_EXCITATION_FEMALE		= (0x01 << CHARCOND_EXCITATION_FEMALE),

};


//-----------------------------------------------------------------------------------
// Condition Flag, Redundancy concept is possible
//-----------------------------------------------------------------------------------
enum eCHARLEAVING_TYPE
{
	CHARLEAVING_DISCONNECT,			// 접속이 끈어짐
	CHARLEAVING_SERVER_CHANGE,		// 다른 게임 서버로의 이동
	CHARLEAVING_CHANNEL_CHANGE,		// 다른 게임 채널로의 이동
	CHARLEAVING_CHARACTER_EXIT,		// 캐릭터 종료 : Lobby로 이동
	CHARLEAVING_GAME_EXIT,			// ACCOUNT 종료 : 게임 종료
	CHARLEAVING_GAME_KICK,			// 시스템에 의한 종료(KICK 등)

	INVALID_CHARLEAVING = INVALID_BYTE,
};


//-----------------------------------------------------------------------------------
// AIR(FLY) STATE
//-----------------------------------------------------------------------------------
enum eAIR_STATE
{
	AIR_STATE_OFF,
	AIR_STATE_ON,
};

//-----------------------------------------------------------------------------------
// status related functions
//-----------------------------------------------------------------------------------
const char *				NtlGetCharStateString(BYTE byStateID);

const char *				NtlGetAspectStateString(BYTE byStateID);

const char *				NtlGetConditionStateString(BYTE byStateID);

//-----------------------------------------------------------------------------------
// 상태 관련 구조체
//-----------------------------------------------------------------------------------
#pragma pack(1)

//-----------------------------------------------------------------------------------
struct sCHARSTATE_SPAWNING
{
	BYTE			byTeleportType;		// eTELEPORT_TYPE

	bool			bSpawnDirection : 1; //if this is true, then it shows spawn effect
	bool			bIsFaint : 1;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_DESPAWNING
{
	BYTE			byTeleportType;		// eTELEPORT_TYPE
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_STANDING
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_SITTING
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_FAINTING
{
	BYTE			byReason; // eFAINT_REASON
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_CAMPING
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_LEAVING
{
	BYTE			byLeavingType;	 //eCHARLEAVING_TYPE
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_MOVING
{
	BYTE			byMoveFlag;
	BYTE			byMoveDirection;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_DESTMOVE
{
	BYTE			byMoveFlag;
	bool			bHaveSecondDestLoc;
	sVECTOR3		vSecondDestLoc;	// Move destination when the destination following coordinates
	BYTE			actionPatternIndex;//new
	BYTE			byDestLocCount;
	sVECTOR3		avDestLoc[DBO_MAX_NEXT_DEST_LOC_COUNT];
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_FOLLOWING
{
	HOBJECT			hTarget; // 타겟 따라가기일때 따라갈 타겟 핸들
	float			fDistance; // 타겟 앞에서 멈출 거리
	BYTE			byMovementReason; // 공격 or 스킬사용 등의 FOLLOW의 이유를 설정
	BYTE			byMoveFlag;//new
	sVECTOR3		vDestLoc;//new
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_FALLING
{
	BYTE			byMoveDirection;		//
	BYTE			byMoveFlag;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_DASH_PASSIVE
{
	BYTE			byMoveDirection;		// NTL_MOVE_DASH_F, NTL_MOVE_DASH_B and so on
	sVECTOR3		vDestLoc;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_TELEPORTING
{
	BYTE	byTeleportType;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_SLIDING
{
	sVECTOR3		vShift;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_KNOCKDOWN
{
	sVECTOR3		vShift;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_FOCUSING
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_CASTING
{
	TBLIDX			skillId;
	HOBJECT			hTarget;
	DWORD			dwCastingTime;
	DWORD			dwCastingTimeRemaining;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_SKILL_AFFECTING
{
	TBLIDX			skillId;
	HOBJECT			hTarget;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_KEEPING_EFFECT
{
	TBLIDX			skillId;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_CASTING_ITEM
{
	TBLIDX			itemTblidx;
	HOBJECT			hTarget;
	DWORD			dwCastingTime;
	DWORD			dwCastingTimeRemaining;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_STUNNED
{
	BYTE			byStunType;		// eDBO_STUN_TYPE
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_SLEEPING
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_PARALYZED
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_HTB
{
	TBLIDX				HTBId;			// HTB skill ID
	HOBJECT				hTarget;		// HTB target handle
	BYTE				byStepCount;	// HTB number of steps
	BYTE				byCurStep;		// The current step
	BYTE				byResultCount;	// HTB Skill number of results
	bool				bIsSuccess;//new
	sHTBSKILL_RESULT	aHTBSkillResult[NTL_HTB_MAX_SKILL_COUNT_IN_SET]; // HTB Skill results
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_SANDBAG
{
	
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_CHARGING
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_GUARD
{
};

//-----------------------------------------------------------------------------------
struct sCHARSTATE_PRIVATESHOP
{
	sSUMMARY_PRIVATESHOP_SHOP_DATA		sSummaryPrivateShopData;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_DIRECT_PLAY
{
	BYTE				byDirectPlayType; // 연출 타입 ( eDIRECT_PLAY_TYPE )
	TBLIDX				directTblidx; // production TBLIDX
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_OPERATING
{
	HOBJECT				hTargetObject; // Handle of the target object
	DWORD				dwOperateTime; // Operating time
	TBLIDX				directTblidx; // production TBLIDX
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_RIDEON
{
	HOBJECT				hTarget; // 타겟 핸들
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_TURNING
{
	TBLIDX				directTblidx; // Production TBLIDX
	sVECTOR3			vDestDir;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_AIRJUMPING
{
	BYTE				byMoveDirection;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_AIRACCEL
{
	BYTE			byMoveFlag;
	BYTE			byMoveDirection;
};
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
struct sASPECTSTATE_SUPER_SAIYAN
{
	BYTE				bySourceGrade;
};
//-----------------------------------------------------------------------------------
struct sASPECTSTATE_PURE_MAJIN
{
	BYTE				bySourceGrade;
};
//-----------------------------------------------------------------------------------
struct sASPECTSTATE_GREAT_NAMEK
{
	BYTE				bySourceGrade;
};
//-----------------------------------------------------------------------------------
struct sASPECTSTATE_KAIOKEN
{
	BYTE				bySourceGrade;
};
//-----------------------------------------------------------------------------------
struct sASPECTSTATE_SPINNING_ATTACK
{
};
//-----------------------------------------------------------------------------------
struct sASPECTSTATE_VEHICLE
{
	TBLIDX		idVehicleTblidx;
	HOBJECT		idVehicleItemHandle;
//	bool		bIsEngineOn; // engine disabled by daneos
};
//-----------------------------------------------------------------------------------
struct sASPECTSTATE_ROLLING_ATTACK
{

};
//-----------------------------------------------------------------------------------
#pragma pack()