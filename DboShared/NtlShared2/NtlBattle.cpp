//***********************************************************************************
//
//	File		:	NtlBattle.cpp
//
//	Begin		:	2006-04-24
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlBattle.h"

#include <crtdbg.h>
#include <stdlib.h>


//-----------------------------------------------------------------------------------
// static variable
//-----------------------------------------------------------------------------------
const char * s_battle_attack_type_string[ BATTLE_ATTACK_TYPE_COUNT ] = 
{
	"BATTLE_ATTACK_TYPE_PHYSICAL",
	"BATTLE_ATTACK_TYPE_ENERGY",
};

const char * s_battle_attack_result_string[ BATTLE_ATTACK_RESULT_COUNT ] = 
{
	"BATTLE_ATTACK_RESULT_HIT", // 일반 공격
	"BATTLE_ATTACK_RESULT_CRITICAL_HIT", // 크리티칼 공격
	"BATTLE_ATTACK_RESULT_DODGE", // 회피
	"BATTLE_ATTACK_RESULT_RESISTED", // 저항
	"BATTLE_ATTACK_RESULT_BLOCK", // 블락
	"BATTLE_ATTACK_RESULT_KNOCKDOWN", // 넉다운
	"BATTLE_ATTACK_RESULT_SLIDING", // 슬라이딩
};

const char * s_freebattle_result_string[ FREEBATTLE_RESULT_COUNT ] = 
{
	"FREEBATTLE_RESULT_WIN", 
	"FREEBATTLE_RESULT_LOSE",
	"FREEBATTLE_RESULT_DRAW",
};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
const char * NtlGetBattleAttackTypeString(BYTE byAttackType)
{
	if( byAttackType >= BATTLE_ATTACK_TYPE_COUNT )
	{
		return "NOT DEFINED";
	}

	return s_battle_attack_type_string[ byAttackType ];
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
const char * DboGetFreeBattleResultString(BYTE byBattleResult)
{
	if( byBattleResult >= FREEBATTLE_RESULT_COUNT )
	{
		return "NOT DEFINED";
	}

	return s_freebattle_result_string[ byBattleResult ];
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
const char * NtlGetBattleAttackResultString(BYTE byAttackResult)
{
	if( byAttackResult >= BATTLE_ATTACK_RESULT_COUNT )
	{
		return "NOT DEFINED";
	}

	return s_battle_attack_result_string[ byAttackResult ];
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
float NtlGetBattleChainAttackBounsRate(BYTE byAttackSequence)
{
	static float s_afChainAttackBonusRate[ NTL_BATTLE_MAX_CHAIN_ATTACK_COUNT ] = { 0.0f, 0.5f, 1.0f, 2.0f, 3.0f, 4.0f };

	if( byAttackSequence <= 0 )
	{
		return 0.0f;
	}

	if( byAttackSequence > NTL_BATTLE_MAX_CHAIN_ATTACK_COUNT )
	{
		_ASSERT( 0 );
		return 0.0f;
	}

	return s_afChainAttackBonusRate[ byAttackSequence - 1 ];
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BYTE NtlGetBattleChainAttackSequence(BYTE byCharLevel)
{
	if ( byCharLevel>=20)
		return 6;
	
	if ( byCharLevel>=16)
		return 5;
	
	if ( byCharLevel>=12)
		return 4;
	
	if ( byCharLevel>=8 )
		return 3;
	
	if ( byCharLevel>=4 )
		return 2;
	
	return 1;
}


////-----------------------------------------------------------------------------------
////		Purpose	:
////		Return	:
////-----------------------------------------------------------------------------------
//float NtlGetBattleAttributeBonusRate(BYTE bySubjectAtt, BYTE byTargetAtt)
//{
//	static float afBattleAttributeBonusRate[BATTLE_ATTRIBUTE_COUNT][BATTLE_ATTRIBUTE_COUNT] = 
//	{
//		// BATTLE_ATTRIBUTE_NONE
//		{ 0.0f, -5.0f, -5.0f, -5.0f, -5.0f, -5.0f },
//
//		// BATTLE_ATTRIBUTE_HONEST
//		{ 5.0f, 0.0f, 5.0f, 10.0f, -10.0f, -5.0f },
//
//		// BATTLE_ATTRIBUTE_STRANGE
//		{ 5.0f, -5.0f, 0.0f, 5.0f, 10.0f, -10.0f },
//
//		// BATTLE_ATTRIBUTE_WILD
//		{ 5.0f, -10.0f, -5.0f, 0.0f, 5.0f, 10.0f },
//
//		// BATTLE_ATTRIBUTE_ELEGANCE
//		{ 5.0f, 10.0f, -10.0f, -5.0f, 0.0f, 5.0f },
//
//		// BATTLE_ATTRIBUTE_FUNNY
//		{ 5.0f, 5.0f, 10.0f, -10.0f, -5.0f, 0.0f },
//
//	};
//
//
//	if( bySubjectAtt >= BATTLE_ATTRIBUTE_COUNT || byTargetAtt >= BATTLE_ATTRIBUTE_COUNT )
//	{
//		_ASSERT( 0 );
//		return 0.0f;
//	}
//
//
//	return afBattleAttributeBonusRate[ bySubjectAtt ][byTargetAtt ];
//}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
eSYSTEM_EFFECT_CODE GetBattleAttributeEffectCode(BYTE byAtt)
{
	static eSYSTEM_EFFECT_CODE awBattleAttribute[BATTLE_ATTRIBUTE_COUNT] =
	{
		INVALID_SYSTEM_EFFECT_CODE, ACTIVE_CRITICAL_BLOCK_UP, ACTIVE_ENERGY_CRITICAL_DEFENSE_UP, ACTIVE_PHYSICAL_ARMOR_PEN_UP, ACTIVE_ENERGY_ARMOR_PEN_UP, ACTIVE_PHYSICAL_CRITICAL_DEFENSE_UP
	};

	if (byAtt >= BATTLE_ATTRIBUTE_COUNT)
	{
		return INVALID_SYSTEM_EFFECT_CODE;
	}

	return awBattleAttribute[byAtt];
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BYTE GetBattleAttributeEffectApplyType(BYTE byAtt)
{
	static BYTE abyBattleAttributeApplyType[BATTLE_ATTRIBUTE_COUNT] =
	{
		SYSTEM_EFFECT_APPLY_TYPE_UNKNOWN, SYSTEM_EFFECT_APPLY_TYPE_VALUE, SYSTEM_EFFECT_APPLY_TYPE_VALUE, SYSTEM_EFFECT_APPLY_TYPE_VALUE, SYSTEM_EFFECT_APPLY_TYPE_VALUE, SYSTEM_EFFECT_APPLY_TYPE_VALUE
	};

	if (byAtt >= BATTLE_ATTRIBUTE_COUNT)
	{
		return SYSTEM_EFFECT_APPLY_TYPE_UNKNOWN;
	}

	return abyBattleAttributeApplyType[byAtt];
}

float GetBattleAttributeEffectApplyValue(BYTE byAtt)
{
	static float afBattleAttributeApplyValue[BATTLE_ATTRIBUTE_COUNT] =
	{
		0.f, 5.f, 5.f, 5.f, 5.f, 5.f
	};

	if (byAtt >= BATTLE_ATTRIBUTE_COUNT)
	{
		return 0.f;
	}

	return afBattleAttributeApplyValue[byAtt];
}


//-----------------------------------------------------------------------------------
//		Purpose	: check if player is in korin arena. This function is no longer required. Use DBO_WORLD_ATTR_BASIC_FREE_PVP_ZONE
//		Return	:
//-----------------------------------------------------------------------------------
bool IsInBattleArena(TBLIDX worldTblidx, CNtlVector& vCurLoc)
{
	if (worldTblidx == 1 /*&& vCurLoc.y >= -97.268f*/ && (vCurLoc.x < 5792 && vCurLoc.z < 788 && vCurLoc.x > 5752 && vCurLoc.z > 748))
		return true;

	return false;
}