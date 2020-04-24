//***********************************************************************************
//
//	File		:	NtlCharacterState.cpp
//
//	Begin		:	2006-05-22
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************


#include "stdafx.h"
#include "NtlCharacterState.h"


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const char * s_character_state_string[ CHARSTATE_COUNT ] = 
{
	"CHARSTATE_SPAWNING",
	"CHARSTATE_DESPAWNING",
	"CHARSTATE_STANDING",
	"CHARSTATE_SITTING",
	"CHARSTATE_FAINTING",
	"CHARSTATE_CAMPING",
	"CHARSTATE_LEAVING",

	"CHARSTATE_MOVING",
	"CHARSTATE_DESTMOVE",
	"CHARSTATE_FOLLOWING",
	"CHARSTATE_FALLING",
	"CHARSTATE_DASH_PASSIVE",
	"CHARSTATE_TELEPORTING",
	"CHARSTATE_SLIDING",
	"CHARSTATE_KNOCKDOWN",

	"CHARSTATE_FOCUSING",
	"CHARSTATE_CASTING",
	"CHARSTATE_SKILL_AFFECTING",
	"CHARSTATE_KEEPING_EFFECT",
	"CHARSTATE_CASTING_ITEM",

	"CHARSTATE_STUNNED",
	"CHARSTATE_SLEEPING",
	"CHARSTATE_PARALYZED",

	"CHARSTATE_HTB",
	"CHARSTATE_SANDBAG",
	"CHARSTATE_CHARGING",
	"CHARSTATE_GUARD",

	"CHARSTATE_PRIVATESHOP",//  [7/16/2007 SGpro]
	"CHARSTATE_DIRECT_PLAY",
	"CHARSTATE_OPERATING",
	"CHARSTATE_RIDEON",
	"CHARSTATE_TURNING",

	"CHARSTATE_AIR_JUMP",
	"CHARSTATE_AIR_DASH_ACCEL",
};


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const char * s_character_aspect_string[ ASPECTSTATE_COUNT ] = 
{
	"ASPECTSTATE_SUPER_SAIYAN",
	"ASPECTSTATE_PURE_MAJIN",
	"ASPECTSTATE_GREAT_NAMEK",
	"ASPECTSTATE_KAIOKEN",
	"ASPECTSTATE_SPINNING_ATTACK",
	"ASPECTSTATE_VEHICLE",
	"ASPECTSTATE_ROLLING_ATTACK",
};


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const char * s_character_condition_string[ CHARCONDITION_COUNT ] = 
{
	"CHARCOND_INVISIBLE",
	"CHARCOND_HIDING_KI",
	"CHARCOND_INVINCIBLE",
	"CHARCOND_TAUNT",
	"CHARCOND_ATTACK_DISALLOW",
	"CHARCOND_TRANSPARENT",
	"CHARCOND_CANT_BE_TARGETTED",
	"CHARCOND_DIRECT_PLAY",

	"CHARCOND_BLEEDING",
	"CHARCOND_POISON",
	"CHARCOND_STOMACHACHE",
	"CHARCOND_CONFUSED",
	"CHARCOND_TERROR",
	"CHARCOND_BARRIER",
	"CHARCOND_DAMAGE_REFLECTION",

	"CHARCOND_AFTEREFFECT",
	"CHARCOND_CHARGING_BLOCKED",
	"CHARCOND_FAKE_DEATH",
	"CHARCOND_NULLIFIED_DAMAGE",

	"CHARCOND_MOB_ROLE",

	"CHARCOND_CLICK_DISABLE",
	"CHARCOND_CLIENT_UI_DISABLE",
};


//-----------------------------------------------------------------------------------
// 상태 관련 함수 정의 : 
//-----------------------------------------------------------------------------------
const char * NtlGetCharStateString(BYTE byStateID)
{
	if( byStateID >= CHARSTATE_COUNT )
	{
		return "UNDEFINED_CHAR_STATE";
	}

	return s_character_state_string[ byStateID ];
}


//-----------------------------------------------------------------------------------
// 상태 관련 함수 정의 : 
//-----------------------------------------------------------------------------------
const char * NtlGetAspectStateString(BYTE byStateID)
{
	if( byStateID >= ASPECTSTATE_COUNT )
	{
		return "UNDEFINED_ASPECT_STATE";
	}

	return s_character_aspect_string[ byStateID ];
}


//-----------------------------------------------------------------------------------
// 상태 관련 함수 정의 : 
//-----------------------------------------------------------------------------------
const char * NtlGetConditionStateString(BYTE byStateID)
{
	if( byStateID >= CHARCONDITION_COUNT )
	{
		return "UNDEFINED_CONDITION_STATE";
	}

	return s_character_condition_string[ byStateID ];
}