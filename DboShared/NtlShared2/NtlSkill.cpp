//***********************************************************************************
//
//	File		:	NtlSkill.cpp
//
//	Begin		:	2006-08-31
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
#include "NtlBitFlagManager.h"


//-----------------------------------------------------------------------------------
// 스킬 이펙트 코드 정의
//-----------------------------------------------------------------------------------
#ifdef DEF_SYSTEM_EFFECT_CODE
	#undef DEF_SYSTEM_EFFECT_CODE
#endif
#define DEF_SYSTEM_EFFECT_CODE(c, can_cast_when_being_interrupted) #c,

const char * s_system_effect_string[ MAX_SYSTEM_EFFECT_CODE ] =
{
	#include "NtlSystemEffect.h"
};

#ifdef DEF_SYSTEM_EFFECT_CODE
	#undef DEF_SYSTEM_EFFECT_CODE
#endif
#define DEF_SYSTEM_EFFECT_CODE(c, can_cast_when_being_interrupted) can_cast_when_being_interrupted,

const bool s_system_effect_can_cast_when_being_interrupted[ MAX_SYSTEM_EFFECT_CODE ] =
{
	#include "NtlSystemEffect.h"
};
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
const char * NtlGetSystemEffectString(DWORD dwSystemEffectCode)
{
	if( dwSystemEffectCode >= MAX_SYSTEM_EFFECT_CODE )
	{
		return "NOT DEFINED CODE";
	}

	return s_system_effect_string[ dwSystemEffectCode ];
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
const bool NtlCanCastWhenBeingInterrupted(eSYSTEM_EFFECT_CODE eSystemEffectCode)
{
	if( eSystemEffectCode >= MAX_SYSTEM_EFFECT_CODE )
	{
		return false;
	}

	return s_system_effect_can_cast_when_being_interrupted[ eSystemEffectCode ];
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool Dbo_IsForBlessBuff(BYTE byActiveEffectType)
{
	switch (byActiveEffectType)
	{
	case SKILL_ACTIVE_TYPE_DD:
	case SKILL_ACTIVE_TYPE_DOT:
	case SKILL_ACTIVE_TYPE_DH:
		return false;

	case SKILL_ACTIVE_TYPE_HOT:
		return true;

	case SKILL_ACTIVE_TYPE_DB:
		return false;

	case SKILL_ACTIVE_TYPE_BB:
		return true;

	case SKILL_ACTIVE_TYPE_DC:
	case SKILL_ACTIVE_TYPE_CB:
		return false;

	default:
		return false;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool Dbo_IsForCurseBuff(BYTE byActiveEffectType)
{
	switch (byActiveEffectType)
	{
	case SKILL_ACTIVE_TYPE_DD:
		return false;

	case SKILL_ACTIVE_TYPE_DOT:
		return true;

	case SKILL_ACTIVE_TYPE_DH:
	case SKILL_ACTIVE_TYPE_HOT:
	case SKILL_ACTIVE_TYPE_DB:
	case SKILL_ACTIVE_TYPE_BB:
	case SKILL_ACTIVE_TYPE_DC:
		return false;

	case SKILL_ACTIVE_TYPE_CB:
		return true;

	default:
		return false;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool Dbo_IsHarmfulEffectType(BYTE byActiveEffectType)
{
	switch (byActiveEffectType)
	{
	case SKILL_ACTIVE_TYPE_DD:
	case SKILL_ACTIVE_TYPE_DOT:
		return true;

	case SKILL_ACTIVE_TYPE_DH:
	case SKILL_ACTIVE_TYPE_HOT:
	case SKILL_ACTIVE_TYPE_DB:
	case SKILL_ACTIVE_TYPE_BB:
		return false;

	case SKILL_ACTIVE_TYPE_DC:
	case SKILL_ACTIVE_TYPE_CB:
		return true;

	default:
		return false;
	}
}


bool Dbo_IsCurseType(BYTE byActiveEffectType)
{
	return byActiveEffectType == SKILL_ACTIVE_TYPE_DC || byActiveEffectType == SKILL_ACTIVE_TYPE_CB;
}


DWORD Dbo_GetLevelUpGainSP( BYTE byLevel, BYTE byLevelDiff )
{
	return Dbo_GetLevelSP( byLevel ) - Dbo_GetLevelSP( byLevel-byLevelDiff );
}


DWORD Dbo_GetLevelSP( BYTE byLevel )
{
	if ( byLevel<1 )
	{
		return 0;
	}
	else if ( 1 == byLevel )
	{
		return static_cast<DWORD>( 0 );
	}
	else
	{
		float fLevel( byLevel );
		return static_cast<DWORD>( fLevel-1 );
	}
}


bool Dbo_IsAdvantageSkillType(BYTE byActiveEffectType)
{
	switch (byActiveEffectType)
	{
	case SKILL_ACTIVE_TYPE_DD:
	case SKILL_ACTIVE_TYPE_DOT:
		return false;

	case SKILL_ACTIVE_TYPE_DH:
	case SKILL_ACTIVE_TYPE_HOT:
	case SKILL_ACTIVE_TYPE_DB:
	case SKILL_ACTIVE_TYPE_BB:
		return true;

	case SKILL_ACTIVE_TYPE_DC:
	case SKILL_ACTIVE_TYPE_CB:
		return false;

	default:
		return false;
	}

	//return false;
}


DWORD Dbo_GetRequiredZennyForNpcSkillInit(BYTE byLevel)
{
	if (byLevel == 1)
		return 350;

	return (525 * (byLevel - 1)) + 350;
}


bool Dbo_IsValidStateToUseSkill(DWORD dwTransformUseInfoBitFlag, BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, bool bIsSwimmingState, bool bIsJumpingState, BYTE eAirState, WORD *pwResultCode)
{
	static CNtlBitFlagManager validState;
	static CNtlBitFlagManager validAspectState;
	static bool bInit = false;
	static QWORD qwInvalidCondition = 0;

	if (pwResultCode)
		*pwResultCode = 601; // GAME_SKILL_CANT_CAST_NOW;

	if (bIsSwimmingState || bIsJumpingState)
		return false;
	else if (eAirState == AIR_STATE_ON)
		return false;
	else
	{
		if (!bInit)
		{
			//can only use skills when char is in one of these states
			validState.Create(CHARSTATE_COUNT);
			validState.Set(CHARSTATE_STANDING);
			validState.Set(CHARSTATE_MOVING);
			validState.Set(CHARSTATE_DESTMOVE);
			validState.Set(CHARSTATE_FOLLOWING);
			validState.Set(CHARSTATE_KEEPING_EFFECT);

			validAspectState.Create(ASPECTSTATE_COUNT);

			//cant use skills when char has one of these conditions
			BIT_FLAG_SET(qwInvalidCondition, MAKE_BIT_FLAG64(CHARCOND_ATTACK_DISALLOW));
			BIT_FLAG_SET(qwInvalidCondition, MAKE_BIT_FLAG64(CHARCOND_DIRECT_PLAY));
			BIT_FLAG_SET(qwInvalidCondition, MAKE_BIT_FLAG64(CHARCOND_CONFUSED));
			BIT_FLAG_SET(qwInvalidCondition, MAKE_BIT_FLAG64(CHARCOND_TERROR));
			BIT_FLAG_SET(qwInvalidCondition, MAKE_BIT_FLAG64(CHARCOND_AFTEREFFECT));
			BIT_FLAG_SET(qwInvalidCondition, MAKE_BIT_FLAG64(CHARCOND_FAKE_DEATH));
			BIT_FLAG_SET(qwInvalidCondition, MAKE_BIT_FLAG64(CHARCOND_BATTLE_INABILITY));
			BIT_FLAG_SET(qwInvalidCondition, MAKE_BIT_FLAG64(CHARCOND_SKILL_INABILITY));
			BIT_FLAG_SET(qwInvalidCondition, MAKE_BIT_FLAG64(CHARCOND_RABIES));

			bInit = true;
		}

		CNtlBitFlagManager validStateNew(&validState);
		CNtlBitFlagManager validAspectStateNew(&validAspectState);
		QWORD qwInvalidCharConditionNew = qwInvalidCondition;
		bool bAnyRestrictedState = false;
		
		if (BIT_FLAG_TEST(dwTransformUseInfoBitFlag, SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_GREAT_NAMEK) && Dbo_IsState_GreatNamek(byCurStateId, eStunType, byCurAspectStateId, qwCurCharCondition, &validStateNew, &validAspectStateNew, &qwInvalidCharConditionNew))
		{
			bAnyRestrictedState = true;
			if (pwResultCode)
				*pwResultCode = 638; // GAME_SKILL_CANT_USE_SKILL_WHEN_GREAT_NAMEK;
		}
		
		if (bAnyRestrictedState == false && BIT_FLAG_TEST(dwTransformUseInfoBitFlag, SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_SUPER_SAIYAN) && Dbo_IsState_SuperSaiyan(byCurStateId, eStunType, byCurAspectStateId, qwCurCharCondition, &validStateNew, &validAspectStateNew, &qwInvalidCharConditionNew))
		{
			bAnyRestrictedState = true;
			if (pwResultCode)
				*pwResultCode = 636; // GAME_SKILL_CANT_USE_SKILL_WHEN_SUPER_SAIYAN;
		}
		
		if (bAnyRestrictedState == false && BIT_FLAG_TEST(dwTransformUseInfoBitFlag, SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_PURE_MAJIN) && Dbo_IsState_PureMajin(byCurStateId, eStunType, byCurAspectStateId, qwCurCharCondition, &validStateNew, &validAspectStateNew, &qwInvalidCharConditionNew))
		{
			bAnyRestrictedState = true;
			if (pwResultCode)
				*pwResultCode = 637; // GAME_SKILL_CANT_USE_SKILL_WHEN_PURE_MAJIN;
		}
		
		if (bAnyRestrictedState == false && BIT_FLAG_TEST(dwTransformUseInfoBitFlag, SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_KAIOKEN) && Dbo_IsState_Kaioken(byCurStateId, eStunType, byCurAspectStateId, qwCurCharCondition, &validStateNew, &validAspectStateNew, &qwInvalidCharConditionNew))
		{
			bAnyRestrictedState = true;
			if (pwResultCode)
				*pwResultCode = 639; // GAME_SKILL_CANT_USE_SKILL_WHEN_KAIOKEN;
		}
		
		if (bAnyRestrictedState == false && BIT_FLAG_TEST(dwTransformUseInfoBitFlag, SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_SPINNING_ATTACK) && Dbo_IsState_SpinningAttack(byCurStateId, eStunType, byCurAspectStateId, qwCurCharCondition, &validStateNew, &validAspectStateNew, &qwInvalidCharConditionNew))
		{
			bAnyRestrictedState = true;
			if (pwResultCode)
				*pwResultCode = 640; // GAME_SKILL_CANT_USE_SKILL_WHEN_SPINNING_ATTACK;
		}
		
		if (bAnyRestrictedState == false && BIT_FLAG_TEST(dwTransformUseInfoBitFlag, SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_VEHICLE) && Dbo_IsState_Vehicle(byCurStateId, eStunType, byCurAspectStateId, qwCurCharCondition, &validStateNew, &validAspectStateNew, &qwInvalidCharConditionNew))
		{
			bAnyRestrictedState = true;
			if (pwResultCode)
				*pwResultCode = 641; // GAME_SKILL_CANT_USE_SKILL_WHEN_VEHICLE;
		}
		
		if (bAnyRestrictedState == false && BIT_FLAG_TEST(dwTransformUseInfoBitFlag, SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_MIND_CURSE) && Dbo_IsState_MindCurse(byCurStateId, eStunType, byCurAspectStateId, qwCurCharCondition, &validStateNew, &validAspectStateNew, &qwInvalidCharConditionNew))
		{
			bAnyRestrictedState = true;
		}
		
		if (bAnyRestrictedState == false && BIT_FLAG_TEST(dwTransformUseInfoBitFlag, SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_BODY_CURSE) && Dbo_IsState_BodyCurse(byCurStateId, eStunType, byCurAspectStateId, qwCurCharCondition, &validStateNew, &validAspectStateNew, &qwInvalidCharConditionNew))
			bAnyRestrictedState = true;
		
		if (bAnyRestrictedState == false && BIT_FLAG_TEST(dwTransformUseInfoBitFlag, SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_CHANGE_CURSE) && Dbo_IsState_ChangeCurse(byCurStateId, eStunType, byCurAspectStateId, qwCurCharCondition, &validStateNew, &validAspectStateNew, &qwInvalidCharConditionNew))
			bAnyRestrictedState = true;
		
		if (bAnyRestrictedState == false && BIT_FLAG_TEST(dwTransformUseInfoBitFlag, SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_ROLLING_ATTACK) && Dbo_IsState_RollingAttack(byCurStateId, eStunType, byCurAspectStateId, qwCurCharCondition, &validStateNew, &validAspectStateNew, &qwInvalidCharConditionNew))
		{
			bAnyRestrictedState = true;
			if (pwResultCode)
				*pwResultCode = 1456; // GAME_SKILL_CANT_USE_SKILL_WHEN_ROLLING_ATTACK;
		}
		
		if (bAnyRestrictedState == false && BIT_FLAG_TEST(dwTransformUseInfoBitFlag, SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_RABIES) && Dbo_IsState_Rabies(byCurStateId, eStunType, byCurAspectStateId, qwCurCharCondition, &validStateNew, &validAspectStateNew, &qwInvalidCharConditionNew))
		{
			bAnyRestrictedState = true;
			if (pwResultCode)
				*pwResultCode = 1457; // GAME_SKILL_CANT_USE_SKILL_WHEN_RABIES;
		}
			
		if (bAnyRestrictedState || BIT_FLAG_TEST(dwTransformUseInfoBitFlag, SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_NORMAL))
		{
			if (validStateNew.IsSet(byCurStateId))
			{
				if ( (qwInvalidCharConditionNew & qwCurCharCondition) == 0) //check if forbidden char conditions are not set
				{
					if (byCurAspectStateId == ASPECTSTATE_INVALID || validAspectStateNew.IsSet(byCurAspectStateId))
					{
						if (pwResultCode)
							*pwResultCode = 500; // GAME_SUCCESS;
					//	printf("qwInvalidCharConditionNew %I64u, qwCurCharCondition %I64u, %I64u \n", qwInvalidCharConditionNew, qwCurCharCondition, qwCurCharCondition | qwInvalidCharConditionNew);
						return true;
					}
				}
			}
			else
			{
				if (byCurStateId == CHARSTATE_SKILL_AFFECTING) 
				{
					if (pwResultCode)
						*pwResultCode = 666; // GAME_ANOTHER_SKILL_CANT_CAST_NOW;
				}
			}
		}
	}

	return false;
}


bool Dbo_IsState_GreatNamek(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition)
{
	UNREFERENCED_PARAMETER(eStunType);
	UNREFERENCED_PARAMETER(byCurStateId);
	UNREFERENCED_PARAMETER(qwCurCharCondition);
	UNREFERENCED_PARAMETER(validState);
	UNREFERENCED_PARAMETER(qwValidCharCondition);

	if (byCurAspectStateId == ASPECTSTATE_GREAT_NAMEK)
	{
		if (validAspectState)
			validAspectState->Set(byCurAspectStateId);
		return true;
	}

	return false;
}


bool Dbo_IsState_SuperSaiyan(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition)
{
	UNREFERENCED_PARAMETER(eStunType);
	UNREFERENCED_PARAMETER(byCurStateId);
	UNREFERENCED_PARAMETER(qwCurCharCondition);
	UNREFERENCED_PARAMETER(validState);
	UNREFERENCED_PARAMETER(qwValidCharCondition);

	if (byCurAspectStateId == ASPECTSTATE_SUPER_SAIYAN)
	{
		if (validAspectState)
		{
			validAspectState->Set(byCurAspectStateId);
		}

		return true;
	}

	return false;
}


bool Dbo_IsState_PureMajin(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition)
{
	UNREFERENCED_PARAMETER(eStunType);
	UNREFERENCED_PARAMETER(byCurStateId);
	UNREFERENCED_PARAMETER(qwCurCharCondition);
	UNREFERENCED_PARAMETER(validState);
	UNREFERENCED_PARAMETER(qwValidCharCondition);

	if (byCurAspectStateId == ASPECTSTATE_PURE_MAJIN)
	{
		if (validAspectState)
			validAspectState->Set(byCurAspectStateId);
		return true;
	}

	return false;
}


bool Dbo_IsState_Kaioken(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition)
{
	UNREFERENCED_PARAMETER(eStunType);
	UNREFERENCED_PARAMETER(byCurStateId);
	UNREFERENCED_PARAMETER(qwCurCharCondition);
	UNREFERENCED_PARAMETER(validState);
	UNREFERENCED_PARAMETER(qwValidCharCondition);

	if (byCurAspectStateId == ASPECTSTATE_KAIOKEN)
	{
		if (validAspectState)
			validAspectState->Set(byCurAspectStateId);
		return true;
	}

	return false;
}


bool Dbo_IsState_SpinningAttack(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition)
{
	UNREFERENCED_PARAMETER(eStunType);
	UNREFERENCED_PARAMETER(byCurStateId);
	UNREFERENCED_PARAMETER(qwCurCharCondition);
	UNREFERENCED_PARAMETER(validState);
	UNREFERENCED_PARAMETER(qwValidCharCondition);

	if (byCurAspectStateId == ASPECTSTATE_SPINNING_ATTACK)
	{
		if (validAspectState)
			validAspectState->Set(byCurAspectStateId);
		return true;
	}

	return false;
}


bool Dbo_IsState_Vehicle(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition)
{
	UNREFERENCED_PARAMETER(eStunType);
	UNREFERENCED_PARAMETER(byCurStateId);
	UNREFERENCED_PARAMETER(qwCurCharCondition);
	UNREFERENCED_PARAMETER(validState);
	UNREFERENCED_PARAMETER(qwValidCharCondition);

	if (byCurAspectStateId == ASPECTSTATE_VEHICLE)
	{
		if (validAspectState)
			validAspectState->Set(byCurAspectStateId);
		return true;
	}

	return false;
}


bool Dbo_IsState_MindCurse(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition)
{
	UNREFERENCED_PARAMETER(byCurStateId);
	UNREFERENCED_PARAMETER(eStunType);
	UNREFERENCED_PARAMETER(byCurAspectStateId);
	UNREFERENCED_PARAMETER(validState);
	UNREFERENCED_PARAMETER(validAspectState);

	if (BIT_FLAG_TEST(qwCurCharCondition, MAKE_BIT_FLAG64(CHARCOND_TAUNT))
		|| BIT_FLAG_TEST(qwCurCharCondition, MAKE_BIT_FLAG64(CHARCOND_CONFUSED))
		|| BIT_FLAG_TEST(qwCurCharCondition, MAKE_BIT_FLAG64(CHARCOND_TERROR))
		|| BIT_FLAG_TEST(qwCurCharCondition, MAKE_BIT_FLAG64(CHARCOND_SKILL_INABILITY))
		)
	{
		BIT_FLAG_UNSET(*qwValidCharCondition, MAKE_BIT_FLAG64(CHARCOND_TAUNT));
		BIT_FLAG_UNSET(*qwValidCharCondition, MAKE_BIT_FLAG64(CHARCOND_CONFUSED));
		BIT_FLAG_UNSET(*qwValidCharCondition, MAKE_BIT_FLAG64(CHARCOND_TERROR));
		BIT_FLAG_UNSET(*qwValidCharCondition, MAKE_BIT_FLAG64(CHARCOND_SKILL_INABILITY));

		return true;
	}

	return false;
}


bool Dbo_IsState_BodyCurse(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition)
{
	UNREFERENCED_PARAMETER(byCurAspectStateId);
	UNREFERENCED_PARAMETER(validAspectState);

	if (byCurStateId == CHARSTATE_KNOCKDOWN)
		return false;

	if (byCurStateId == CHARSTATE_PARALYZED)
	{
		if (validState)
			validState->Set(byCurStateId);
		return true;
	}
	else if (byCurStateId != CHARSTATE_STUNNED || (eStunType != DBO_STUN_TYPE_GENERAL && eStunType != DBO_STUN_TYPE_TIED))
	{
		if (BIT_FLAG_TEST(qwCurCharCondition, MAKE_BIT_FLAG64(CHARCOND_SKILL_INABILITY)))
		{
			//BIT_FLAG_SET(*qwValidCharCondition, qwCurCharCondition);
			BIT_FLAG_UNSET(*qwValidCharCondition, MAKE_BIT_FLAG64(CHARCOND_SKILL_INABILITY));
		}

		if (validState) //if we dont do this, then players cant use ssj while debuff like candy
			validState->Set(byCurStateId);

		return true;
	}
	else
	{
		if (validState)
			validState->Set(byCurStateId);

		return true;
	}

	return false;
}


bool Dbo_IsState_ChangeCurse(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition)
{
	UNREFERENCED_PARAMETER(byCurAspectStateId);
	UNREFERENCED_PARAMETER(qwCurCharCondition);
	UNREFERENCED_PARAMETER(validAspectState);
	UNREFERENCED_PARAMETER(qwValidCharCondition);

	if (byCurStateId != CHARSTATE_STUNNED || eStunType != DBO_STUN_TYPE_STONE || eStunType != DBO_STUN_TYPE_CANDY)
		return false;
	else
	{
		if (validState)
			validState->Set(byCurStateId);

		return true;
	}
}


bool Dbo_IsState_RollingAttack(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition)
{
	UNREFERENCED_PARAMETER(byCurStateId);
	UNREFERENCED_PARAMETER(eStunType);
	UNREFERENCED_PARAMETER(qwCurCharCondition);
	UNREFERENCED_PARAMETER(validState);
	UNREFERENCED_PARAMETER(qwValidCharCondition);

	if (byCurAspectStateId == ASPECTSTATE_ROLLING_ATTACK)
	{
		if (validAspectState)
			validAspectState->Set(byCurAspectStateId);
		return true;
	}

	return false;
}


bool Dbo_IsState_Rabies(BYTE byCurStateId, BYTE eStunType, BYTE byCurAspectStateId, QWORD qwCurCharCondition, CNtlBitFlagManager *validState, CNtlBitFlagManager *validAspectState, QWORD *qwValidCharCondition)
{
	UNREFERENCED_PARAMETER(byCurStateId);
	UNREFERENCED_PARAMETER(eStunType);
	UNREFERENCED_PARAMETER(byCurAspectStateId);
	UNREFERENCED_PARAMETER(validState);
	UNREFERENCED_PARAMETER(validAspectState);

	if (BIT_FLAG_TEST(qwCurCharCondition, MAKE_BIT_FLAG64(CHARCOND_RABIES)))
	{
		BIT_FLAG_UNSET(*qwValidCharCondition, MAKE_BIT_FLAG64(CHARCOND_RABIES));

		return true;
	}

	return false;
}


bool Dbo_IsSystemEffectForMindCurse(eSYSTEM_EFFECT_CODE eSystemEffect)
{
	return eSystemEffect == ACTIVE_TAUNT || eSystemEffect == ACTIVE_CONFUSE || eSystemEffect == ACTIVE_TERROR;
}


bool Dbo_IsSystemEffectForBodyCurse(eSYSTEM_EFFECT_CODE eSystemEffect)
{
	return eSystemEffect == ACTIVE_PARALYZE || eSystemEffect == ACTIVE_STUN 
		|| eSystemEffect == ACTIVE_SLEEP 
		|| eSystemEffect == ACTIVE_STONE || eSystemEffect == ACTIVE_CANDY || eSystemEffect == ACTIVE_TIED_UP
		|| eSystemEffect == ACTIVE_FREEZE || eSystemEffect == ACTIVE_WARP_STUN;
}


bool Dbo_IsSystemEffectForStun(WORD wSystemEffect)
{
	return wSystemEffect == ACTIVE_PARALYZE || wSystemEffect == ACTIVE_STUN || wSystemEffect == ACTIVE_WARP_STUN;
}


bool Dbo_IsSystemEffectForChangeCurse(eSYSTEM_EFFECT_CODE eSystemEffect)
{
	return eSystemEffect == ACTIVE_STONE || eSystemEffect == ACTIVE_CANDY;
}


bool Dbo_IsSystemEffectForFly(eSYSTEM_EFFECT_CODE eSystemEffect)
{
	return eSystemEffect == ACTIVE_AIR_MOVE || eSystemEffect == ACTIVE_AIR_MOVE_DASH_ACCEL;
}


bool Dbo_IsSystemEffectForFlyDash(eSYSTEM_EFFECT_CODE eSystemEffect)
{
	return eSystemEffect == ACTIVE_AIR_DASH || eSystemEffect == ACTIVE_AIR_MOVE_DASH_ACCEL;
}


bool Dbo_IsSystemEffectForFlyAccel(eSYSTEM_EFFECT_CODE eSystemEffect)
{
	return eSystemEffect == ACTIVE_AIR_ACCEL || eSystemEffect == ACTIVE_AIR_MOVE_DASH_ACCEL;
}

bool Dbo_IsCcReduction(eSYSTEM_EFFECT_CODE eSystemEffect)
{
	return eSystemEffect == ACTIVE_YELL || eSystemEffect == ACTIVE_TAUNT || eSystemEffect == ACTIVE_PARALYZE
		|| eSystemEffect == ACTIVE_CONFUSE || eSystemEffect == ACTIVE_TERROR || eSystemEffect == ACTIVE_STUN
		|| eSystemEffect == ACTIVE_SLEEP || eSystemEffect == ACTIVE_MOVE_SPEED_DOWN || eSystemEffect == ACTIVE_TIED_UP
		|| eSystemEffect == ACTIVE_FREEZE || eSystemEffect == ACTIVE_WARP_STUN || eSystemEffect == ACTIVE_BATTLE_INABILITY || eSystemEffect == ACTIVE_SKILL_INABILITY;
}

