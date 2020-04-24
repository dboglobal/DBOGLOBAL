#include "stdafx.h"
#include "calcs.h"
#include "CharacterAtt.h"



void Dbo_SetAvatarAttributeValue(CCharacterAtt * pCharAtt, eSYSTEM_EFFECT_CODE effectcode, float effectvalue, BYTE byApplyType)
{
	sAVATAR_ATTRIBUTE& avt = pCharAtt->GetAvatarAttribute();
	//NTL_PRINT(PRINT_APP,"code: %u, effect: %f, byApplyType: %u", effectcode, effectvalue, byApplyType);
	switch (effectcode)
	{
		//passive
		case PASSIVE_MAX_LP_UP:				pCharAtt->CalculateMaxLP(effectvalue, byApplyType, true);	break;
		case PASSIVE_MAX_EP_UP:				pCharAtt->CalculateMaxEP(effectvalue, byApplyType, true);	break;
		case PASSIVE_MAX_RP_DOWN:			pCharAtt->CalculateMaxRP(effectvalue, byApplyType, false);	break;
		case PASSIVE_PHYSICAL_OFFENCE_UP: 	pCharAtt->CalculatePhysicalOffence(effectvalue, byApplyType, true);	break;
		case PASSIVE_ENERGY_OFFENCE_UP: 	pCharAtt->CalculateEnergyOffence(effectvalue, byApplyType, true);	break;
		case PASSIVE_PHYSICAL_DEFENCE_UP: 	pCharAtt->CalculatePhysicalDefence(effectvalue, byApplyType, true);	break;
		case PASSIVE_ENERGY_DEFENCE_UP: 	pCharAtt->CalculateEnergyDefence(effectvalue, byApplyType, true);	break;
		case PASSIVE_STR_UP:				pCharAtt->CalculateStr(effectvalue, byApplyType, true);	break;
		case PASSIVE_CON_UP:				pCharAtt->CalculateCon(effectvalue, byApplyType, true);	break;
		case PASSIVE_FOC_UP:				pCharAtt->CalculateFoc(effectvalue, byApplyType, true);	break;
		case PASSIVE_DEX_UP:				pCharAtt->CalculateDex(effectvalue, byApplyType, true);	break;
		case PASSIVE_SOL_UP:				pCharAtt->CalculateSol(effectvalue, byApplyType, true);	break;
		case PASSIVE_ENG_UP:				pCharAtt->CalculateEng(effectvalue, byApplyType, true); break;
		case PASSIVE_LP_REGENERATION:		pCharAtt->CalculateLpRegen(effectvalue, byApplyType, true);	break;
		case PASSIVE_EP_REGENERATION:		pCharAtt->CalculateEpRegen(effectvalue, byApplyType, true);	break;
		case PASSIVE_ATTACK_RATE_UP:		pCharAtt->CalculateAttackRate(effectvalue, byApplyType, true);	break;
		case PASSIVE_DODGE_RATE_UP:			pCharAtt->CalculateDodgeRate(effectvalue, byApplyType, true);	break;
		case PASSIVE_BLOCK_RATE_UP:			pCharAtt->CalculateBlockRate(effectvalue, byApplyType, true);	break;
		case PASSIVE_CURSE_SUCCESS_UP:		pCharAtt->CalculateCurseSuccessRate(effectvalue, byApplyType, true);	break;
		case PASSIVE_CURSE_TOLERANCE_UP:	pCharAtt->CalculateCurseToleranceRate(effectvalue, byApplyType, true);	break;
		case PASSIVE_PHYSICAL_CRITICAL_UP:	pCharAtt->CalculatePhysicalCriticalRate(effectvalue, byApplyType, true);	break;
		case PASSIVE_ENERGY_CRITICAL_UP:	pCharAtt->CalculateEnergyCriticalRate(effectvalue, byApplyType, true);	break;
		case PASSIVE_RP_CHARGE_SPEED:		pCharAtt->CalculateRpRegen(effectvalue, byApplyType, true);	break;
		case PASSIVE_MOVE_SPEED:			pCharAtt->CalculateRunSpeed(effectvalue, byApplyType, true);	break;
		case PASSIVE_ATTACK_SPEED_UP:		pCharAtt->CalculateAttackSpeedRate(effectvalue, byApplyType, false);	break;
		case PASSIVE_SKILL_CASTING_TIME_DOWN:pCharAtt->CalculateCastingTimeChangePercent(effectvalue, byApplyType, false);	break;
		case PASSIVE_SKILL_COOL_TIME_DOWN:	pCharAtt->CalculateCoolTimeChangePercent(effectvalue * 0.7f, byApplyType, false);		break;
		case PASSIVE_BLOCK_MODE:			pCharAtt->CalculateGuardRate(effectvalue, byApplyType, true);	break;
		case PASSIVE_DOT_VALUE_UP_ALL:		pCharAtt->CalculateDotValueChangePercent(effectvalue, byApplyType, true);		break;
		case PASSIVE_DOT_TIME_UP_ALL:		pCharAtt->CalculateDotTimeChangeAbsolute(effectvalue, byApplyType, true);		break;
		case PASSIVE_SKILL_KEEP_TIME_UP:	pCharAtt->CalculateKeepTimeChangePercent(effectvalue, byApplyType, true);		break;
		case PASSIVE_REQUIRE_EP_DOWN:		pCharAtt->CalculateRequiredEpChangePercent(effectvalue, byApplyType, false);	break;

			//bless
		case ACTIVE_MAX_LP_UP:				pCharAtt->CalculateMaxLP(effectvalue, byApplyType, true);	break;
		case ACTIVE_MAX_EP_UP:				pCharAtt->CalculateMaxEP(effectvalue, byApplyType, true);	break;
		case ACTIVE_MAX_RP_UP:				pCharAtt->CalculateMaxRP(effectvalue, byApplyType, true);	break;
		case ACTIVE_PHYSICAL_OFFENCE_UP: {	pCharAtt->CalculatePhysicalOffence(effectvalue, byApplyType, true);	}break;
		case ACTIVE_ENERGY_OFFENCE_UP: {	pCharAtt->CalculateEnergyOffence(effectvalue, byApplyType, true);	}break;
		case ACTIVE_PHYSICAL_DEFENCE_UP: {	pCharAtt->CalculatePhysicalDefence(effectvalue, byApplyType, true);	}break;
		case ACTIVE_ENERGY_DEFENCE_UP: {	pCharAtt->CalculateEnergyDefence(effectvalue, byApplyType, true);	}break;
		case ACTIVE_STR_UP:					pCharAtt->CalculateStr(effectvalue, byApplyType, true);	break;
		case ACTIVE_CON_UP:					pCharAtt->CalculateCon(effectvalue, byApplyType, true);	break;
		case ACTIVE_FOC_UP:					pCharAtt->CalculateFoc(effectvalue, byApplyType, true);	break;
		case ACTIVE_DEX_UP:					pCharAtt->CalculateDex(effectvalue, byApplyType, true);	break;
		case ACTIVE_SOL_UP:					pCharAtt->CalculateSol(effectvalue, byApplyType, true);	break;
		case ACTIVE_ENG_UP:					pCharAtt->CalculateEng(effectvalue, byApplyType, true); break;
		case ACTIVE_MOVE_SPEED_UP:			pCharAtt->CalculateRunSpeed(effectvalue, byApplyType, true);	break;
		case ACTIVE_ATTACK_SPEED_UP:		pCharAtt->CalculateAttackSpeedRate(effectvalue, byApplyType, false); break;
		case ACTIVE_ATTACK_RATE_UP:			pCharAtt->CalculateAttackRate(effectvalue, byApplyType, true);	break;
		case ACTIVE_DODGE_RATE_UP:			pCharAtt->CalculateDodgeRate(effectvalue, byApplyType, true);	break;
		case ACTIVE_BLOCK_RATE_UP:			pCharAtt->CalculateBlockRate(effectvalue, byApplyType, true);	break;
		case ACTIVE_ENERGY_REFLECTION:		pCharAtt->CalculateEnergyReflection(effectvalue, byApplyType, true);	break;
		case ACTIVE_LP_REGENERATION:		pCharAtt->CalculateLpRegen(effectvalue, byApplyType, true);	break;
		case ACTIVE_EP_REGENERATION:		pCharAtt->CalculateEpRegen(effectvalue, byApplyType, true);	break;
		case ACTIVE_RP_CHARGE_SPEED:		pCharAtt->CalculateRpRegen(effectvalue, byApplyType, true);	break;
		case ACTIVE_CURSE_SUCCESS:			pCharAtt->CalculateCurseSuccessRate(effectvalue, byApplyType, true);	break;
		case ACTIVE_CURSE_TOLERANCE:		pCharAtt->CalculateCurseToleranceRate(effectvalue, byApplyType, true);	break;
		case ACTIVE_PHYSICAL_CRITICAL:		pCharAtt->CalculatePhysicalCriticalRate(effectvalue, byApplyType, true);	break;
		case ACTIVE_ENERGY_CRITICAL:			pCharAtt->CalculateEnergyCriticalRate(effectvalue, byApplyType, true);	break;
		case ACTIVE_SKILL_CASTING_TIME_DOWN:	pCharAtt->CalculateCastingTimeChangePercent(effectvalue, byApplyType, false);	break;
		case ACTIVE_SKILL_COOL_TIME_DOWN:		pCharAtt->CalculateCoolTimeChangePercent(effectvalue * 0.7f, byApplyType, false);	break;
		case ACTIVE_PARALYZE_TOLERANCE_UP:		pCharAtt->CalculateParalyzeToleranceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break; //add value because the value is "increase by X%. But we have 0 so it wont increase in any way"
		case ACTIVE_TERROR_TOLERANCE_UP:		pCharAtt->CalculateTerrorToleranceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;//add value because the value is "increase by X%. But we have 0 so it wont increase in any way"
		case ACTIVE_CONFUSE_TOLERANCE_UP:		pCharAtt->CalculateConfuseToleranceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;//add value because the value is "increase by X%. But we have 0 so it wont increase in any way"
		case ACTIVE_STONE_TOLERANCE_UP:			pCharAtt->CalculateStoneToleranceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;//add value because the value is "increase by X%. But we have 0 so it wont increase in any way"
		case ACTIVE_CANDY_TOLERANCE_UP:			pCharAtt->CalculateCandyToleranceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;//add value because the value is "increase by X%. But we have 0 so it wont increase in any way"
		case ACTIVE_PARALYZE_KEEPTIME_DOWN:		pCharAtt->CalculateParalyzeKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_TERROR_KEEPTIME_DOWN:		pCharAtt->CalculateTerrorKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_CONFUSE_KEEPTIME_DOWN:		pCharAtt->CalculateConfuseKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_STONE_KEEPTIME_DOWN:		pCharAtt->CalculateStoneKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_CANDY_KEEPTIME_DOWN:		pCharAtt->CalculateCandyKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_BLEEDING_KEEPTIME_DOWN:		pCharAtt->CalculateBleedingKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_POISON_KEEPTIME_DOWN:		pCharAtt->CalculatePoisonKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_STOMACHACHE_KEEPTIME_DOWN:	pCharAtt->CalculateStomachacheKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_CRITICAL_BLOCK_UP:			pCharAtt->CalculateCriticalBlockSuccessRate(effectvalue, byApplyType, true);	break;
		case ACTIVE_NORMAL_SKILL_BLOCK_UP:		pCharAtt->CalculateSkillDamageBlockModeSuccessRate(effectvalue, byApplyType, true);	break;
		case ACTIVE_CURSE_SKILL_BLOCK_UP:		pCharAtt->CalculateCurseBlockModeSuccessRate(effectvalue, byApplyType, true);	break;
		case ACTIVE_KNOCKDOWN_ATTACK_BLOCK_UP:	pCharAtt->CalculateKnockdownBlockModeSuccessRate(effectvalue, byApplyType, true);	break;
		case ACTIVE_HTB_SKILL_BLOCK_UP:			pCharAtt->CalculateHtbBlockModeSuccessRate(effectvalue, byApplyType, true);	break;
		case ACTIVE_SIT_DOWN_LP_REGENERATION_UP:	pCharAtt->CalculateLpSitdownRegen(effectvalue, byApplyType, true);	break;
		case ACTIVE_SIT_DOWN_EP_REGENERATION_UP:	pCharAtt->CalculateEpSitdownRegen(effectvalue, byApplyType, true);	break;
		case ACTIVE_PHYSICAL_CRITICAL_DAMAGE_UP:	pCharAtt->CalculatePhysicalCriticalDamageRate(effectvalue, byApplyType, true);	break;
		case ACTIVE_ENERGY_CRITICAL_DAMAGE_UP:		pCharAtt->CalculateEnergyCriticalDamageRate(effectvalue, byApplyType, true);	break;
		case ACTIVE_ATTACK_RANGE_UP:				pCharAtt->CalculateAttackRange(effectvalue, byApplyType, true);	break;

		case ACTIVE_PHYSICAL_REFLECTION:			pCharAtt->CalculatePhysicalReflection(effectvalue, byApplyType, true);	break;
		case ACTIVE_LP_RECOVERY:					pCharAtt->CalculateLpRecoveryWhenHit(effectvalue, byApplyType, true);	break;
		case ACTIVE_EP_RECOVERY:					pCharAtt->CalculateEpRecoveryWhenHit(effectvalue, byApplyType, true);	break;
		case ACTIVE_LP_RECOVERY_IN_PERCENT:			pCharAtt->CalculateLpRecoveryWhenHitInPercent(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_EP_RECOVERY_IN_PERCENT:			pCharAtt->CalculateEpRecoveryWhenHitInPercent(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_BATTLE_LP_UP:				pCharAtt->CalculateLpBattleRegen(effectvalue, byApplyType, true);	break;
		case ACTIVE_BATTLE_EP_UP:				pCharAtt->CalculateEpBattleRegen(effectvalue, byApplyType, true);	break;
		case ACTIVE_ALL_OFFENCE_UP:			pCharAtt->CalculatePhysicalOffence(effectvalue, byApplyType, true);	pCharAtt->CalculateEnergyOffence(effectvalue, byApplyType, true); break;
		case ACTIVE_ALL_DEFENCE_UP:			pCharAtt->CalculatePhysicalDefence(effectvalue, byApplyType, true);	pCharAtt->CalculateEnergyDefence(effectvalue, byApplyType, true); break;

		case ACTIVE_MIND_IMMUNITY:				pCharAtt->CalculateMindCurseImmunity(effectvalue, byApplyType, true);	break;
		case ACTIVE_BODY_IMMUNITY:				pCharAtt->CalculateBodyCurseImmunity(effectvalue, byApplyType, true);	break;
		case ACTIVE_CHANGE_IMMUNITY:			pCharAtt->CalculateChangeCurseImmunity(effectvalue, byApplyType, true);	break;
		case ACTIVE_SKILL_AGGRO_UP:				pCharAtt->CalculateSkillAggroBonus(effectvalue, byApplyType, true);	break;
		case ACTIVE_DH_POWER_UP:				pCharAtt->CalculateDirectHealPowerBonus(effectvalue, byApplyType, true);	break;
		case ACTIVE_HOT_POWER_UP:				pCharAtt->CalculateHotPowerBonus(effectvalue, byApplyType, true);	break;
		case ACTIVE_SKILL_AGGRO_UP_IN_PERCENT:	pCharAtt->CalculateSkillAggroBonusInPercent(effectvalue, byApplyType, true);	break;
		case ACTIVE_DH_POWER_UP_IN_PERCENT:		pCharAtt->CalculateDirectHealPowerBonusInPercent(effectvalue, byApplyType, true);	break;
		case ACTIVE_HOT_POWER_UP_IN_PERCENT:	pCharAtt->CalculateHotPowerBonusInPercent(effectvalue, byApplyType, true);	break;
		case ACTIVE_MAX_AP_UP:					pCharAtt->CalculateMaxAP(effectvalue, byApplyType, true);	break;
		case ACTIVE_AP_REGENERATION:			pCharAtt->CalculateApRegen(effectvalue * 1000, byApplyType, true);	break;
		case ACTIVE_STOMACHACHE_DEFENCE:		pCharAtt->CalculateStomachacheDefence(effectvalue, byApplyType, true);	break;
		case ACTIVE_POISON_DEFENCE:				pCharAtt->CalculatePoisonDefence(effectvalue, byApplyType, true);	break;
		case ACTIVE_BLEED_DEFENCE:				pCharAtt->CalculateBleedDefence(effectvalue, byApplyType, true);	break;
		case ACTIVE_BURN_DEFENCE:				pCharAtt->CalculateBurnDefence(effectvalue, byApplyType, true);	break;

		case ACTIVE_VIABILITY:					pCharAtt->CalculateBlockRate(effectvalue, byApplyType, true);	break;

			//curse
		case ACTIVE_SKILL_AGGRO_DOWN: {	pCharAtt->CalculateSkillAggroBonus(effectvalue, byApplyType, false);	}break;
		case ACTIVE_SKILL_AGGRO_DOWN_IN_PERCENT: {	pCharAtt->CalculateSkillAggroBonusInPercent(effectvalue, byApplyType, false);	}break;
		case ACTIVE_MAX_LP_DOWN: {	pCharAtt->CalculateMaxLP(effectvalue, byApplyType, false);	}break;
		case ACTIVE_MAX_EP_DOWN: {	pCharAtt->CalculateMaxEP(effectvalue, byApplyType, false);	}break;
		case ACTIVE_MAX_RP_DOWN: {	pCharAtt->CalculateMaxRP(effectvalue, byApplyType, false);	}break;
		case ACTIVE_PHYSICAL_OFFENCE_DOWN: {	pCharAtt->CalculatePhysicalOffence(effectvalue, byApplyType, false);	}break;
		case ACTIVE_ENERGY_OFFENCE_DOWN: {	pCharAtt->CalculateEnergyOffence(effectvalue, byApplyType, false);	}break;
		case ACTIVE_PHYSICAL_DEFENCE_DOWN: { pCharAtt->CalculatePhysicalDefence(effectvalue, byApplyType, false);	}break;
		case ACTIVE_ENERGY_DEFENCE_DOWN: {	pCharAtt->CalculateEnergyDefence(effectvalue, byApplyType, false);	}break;
		case ACTIVE_STR_DOWN: 					pCharAtt->CalculateStr(effectvalue, byApplyType, false);	break;
		case ACTIVE_CON_DOWN: 					pCharAtt->CalculateCon(effectvalue, byApplyType, false);	break;
		case ACTIVE_FOC_DOWN:					pCharAtt->CalculateFoc(effectvalue, byApplyType, false);	break;
		case ACTIVE_DEX_DOWN: 					pCharAtt->CalculateDex(effectvalue, byApplyType, false);	break;
		case ACTIVE_SOL_DOWN: 					pCharAtt->CalculateSol(effectvalue, byApplyType, false);	break;
		case ACTIVE_ENG_DOWN: 					pCharAtt->CalculateEng(effectvalue, byApplyType, false);	break;
		case ACTIVE_MOVE_SPEED_DOWN: {		pCharAtt->CalculateRunSpeed(effectvalue, byApplyType, false);	}break;
		case ACTIVE_ATTACK_SPEED_DOWN:			pCharAtt->CalculateAttackSpeedRate(effectvalue, byApplyType, true); break;
		case ACTIVE_ATTACK_RATE_DOWN: {		pCharAtt->CalculateAttackRate(effectvalue, byApplyType, false);	}break;
		case ACTIVE_DODGE_RATE_DOWN: {		pCharAtt->CalculateDodgeRate(effectvalue, byApplyType, false);	}break;
		case ACTIVE_BLOCK_RATE_DOWN: {		pCharAtt->CalculateBlockRate(effectvalue, byApplyType, false);	}break;
		case ACTIVE_SKILL_CASTING_TIME_UP: {	pCharAtt->CalculateCastingTimeChangePercent(effectvalue, byApplyType, true);	}break;
		case ACTIVE_SKILL_COOL_TIME_UP: {	pCharAtt->CalculateCoolTimeChangePercent(effectvalue * 0.7f, byApplyType, true);	}break;
		case ACTIVE_ATTACK_RANGE_DOWN: {		pCharAtt->CalculateAttackRange(effectvalue, byApplyType, false);	}break;

		case ACTIVE_ALL_OFFENCE_DOWN:		pCharAtt->CalculatePhysicalOffence(effectvalue, byApplyType, false); pCharAtt->CalculateEnergyOffence(effectvalue, byApplyType, false);		break;
		case ACTIVE_ALL_DEFENCE_DOWN:		pCharAtt->CalculatePhysicalDefence(effectvalue, byApplyType, false); pCharAtt->CalculateEnergyDefence(effectvalue, byApplyType, false);		break;

		case ACTIVE_EXP_BOOSTER:	pCharAtt->CalculateExpBooster(effectvalue, byApplyType, true); break;

		case ACTIVE_PHYSICAL_ARMOR_PEN_UP:	pCharAtt->CalculatePhysicalArmorPen(effectvalue, byApplyType, true); break;
		case ACTIVE_ENERGY_ARMOR_PEN_UP:	pCharAtt->CalculateEnergyArmorPen(effectvalue, byApplyType, true); break;
		case ACTIVE_ARMOR_PEN_UP:			pCharAtt->CalculatePhysicalArmorPen(effectvalue, byApplyType, true);
											pCharAtt->CalculateEnergyArmorPen(effectvalue, byApplyType, true); break;
		case ACTIVE_CRITICAL_DEFENSE_UP:	pCharAtt->CalculatePhysicalCriticalDefenceRate(effectvalue, byApplyType, true);
											pCharAtt->CalculateEnergyCriticalDefenceRate(effectvalue, byApplyType, true); break;
		case ACTIVE_CRITICAL_DEFENSE_DOWN:	pCharAtt->CalculatePhysicalCriticalDefenceRate(effectvalue, byApplyType, false);
											pCharAtt->CalculateEnergyCriticalDefenceRate(effectvalue, byApplyType, false); break;

		case ACTIVE_PHYSICAL_CRITICAL_DEFENSE_UP:	pCharAtt->CalculatePhysicalCriticalDefenceRate(effectvalue, byApplyType, true); break;
		case ACTIVE_ENERGY_CRITICAL_DEFENSE_UP:		pCharAtt->CalculateEnergyCriticalDefenceRate(effectvalue, byApplyType, true); break;

		default: /*printf("code %u missing effect %f \n", effectcode, effectvalue); */break;
	}
}





void Dbo_SetRandomOptionValues(CCharacterAtt* pCharAtt, eSYSTEM_EFFECT_CODE effectcode, float effectvalue)
{
	sAVATAR_ATTRIBUTE& avt = pCharAtt->GetAvatarAttribute();
	//NTL_PRINT(PRINT_APP, "code %u effect %f \n", effectcode, effectvalue);
	switch (effectcode)
	{
		case ACTIVE_MAX_LP_UP:				pCharAtt->CalculateMaxLP(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_MAX_EP_UP:				pCharAtt->CalculateMaxEP(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_MAX_RP_UP:				pCharAtt->CalculateMaxRP(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_PHYSICAL_OFFENCE_UP: {	pCharAtt->CalculatePhysicalOffence(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	}break;
		case ACTIVE_ENERGY_OFFENCE_UP: {	pCharAtt->CalculateEnergyOffence(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	}break;
		case ACTIVE_PHYSICAL_DEFENCE_UP: {	pCharAtt->CalculatePhysicalDefence(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	}break;
		case ACTIVE_ENERGY_DEFENCE_UP: {	pCharAtt->CalculateEnergyDefence(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	}break;
		case ACTIVE_STR_UP:					pCharAtt->CalculateStr(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_CON_UP:					pCharAtt->CalculateCon(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_FOC_UP:					pCharAtt->CalculateFoc(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_DEX_UP:					pCharAtt->CalculateDex(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_SOL_UP:					pCharAtt->CalculateSol(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_ENG_UP:					pCharAtt->CalculateEng(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true); break;
		case ACTIVE_MOVE_SPEED_UP:			pCharAtt->CalculateRunSpeed(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_PERCENT, true);	break;
		case ACTIVE_ATTACK_SPEED_UP:		pCharAtt->CalculateAttackSpeedRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_PERCENT, false);
		case ACTIVE_ATTACK_RATE_UP:			pCharAtt->CalculateAttackRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_DODGE_RATE_UP:			pCharAtt->CalculateDodgeRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_BLOCK_RATE_UP:			pCharAtt->CalculateBlockRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_ENERGY_REFLECTION:		pCharAtt->CalculateEnergyReflection(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_LP_REGENERATION:		pCharAtt->CalculateLpRegen(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_EP_REGENERATION:		pCharAtt->CalculateEpRegen(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_RP_CHARGE_SPEED:		pCharAtt->CalculateRpRegen(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_CURSE_SUCCESS:			pCharAtt->CalculateCurseSuccessRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_CURSE_TOLERANCE:		pCharAtt->CalculateCurseToleranceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_PHYSICAL_CRITICAL:		pCharAtt->CalculatePhysicalCriticalRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_ENERGY_CRITICAL:		pCharAtt->CalculateEnergyCriticalRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_SKILL_COOL_TIME_DOWN:		pCharAtt->CalculateCoolTimeChangePercent(effectvalue * 0.7f, SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);	break;
		case ACTIVE_PARALYZE_TOLERANCE_UP:		pCharAtt->CalculateParalyzeToleranceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;//add value because the value is "increase by X%. But we have 0 so it wont increase in any way"
		case ACTIVE_TERROR_TOLERANCE_UP:		pCharAtt->CalculateTerrorToleranceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;//add value because the value is "increase by X%. But we have 0 so it wont increase in any way"
		case ACTIVE_CONFUSE_TOLERANCE_UP:		pCharAtt->CalculateConfuseToleranceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;//add value because the value is "increase by X%. But we have 0 so it wont increase in any way"
		case ACTIVE_STONE_TOLERANCE_UP:			pCharAtt->CalculateStoneToleranceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;//add value because the value is "increase by X%. But we have 0 so it wont increase in any way"
		case ACTIVE_CANDY_TOLERANCE_UP:			pCharAtt->CalculateCandyToleranceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;//add value because the value is "increase by X%. But we have 0 so it wont increase in any way"
		case ACTIVE_PARALYZE_KEEPTIME_DOWN:		pCharAtt->CalculateParalyzeKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_TERROR_KEEPTIME_DOWN:		pCharAtt->CalculateTerrorKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_CONFUSE_KEEPTIME_DOWN:		pCharAtt->CalculateConfuseKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_STONE_KEEPTIME_DOWN:		pCharAtt->CalculateStoneKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_CANDY_KEEPTIME_DOWN:		pCharAtt->CalculateCandyKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_BLEEDING_KEEPTIME_DOWN:		pCharAtt->CalculateBleedingKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_POISON_KEEPTIME_DOWN:		pCharAtt->CalculatePoisonKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_STOMACHACHE_KEEPTIME_DOWN:	pCharAtt->CalculateStomachacheKeepTimeDown(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_CRITICAL_BLOCK_UP:			pCharAtt->CalculateCriticalBlockSuccessRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_CURSE_SKILL_BLOCK_UP:		pCharAtt->CalculateCurseBlockModeSuccessRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_PHYSICAL_CRITICAL_DAMAGE_UP:	pCharAtt->CalculatePhysicalCriticalDamageRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_ENERGY_CRITICAL_DAMAGE_UP:		pCharAtt->CalculateEnergyCriticalDamageRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;

		case ACTIVE_PHYSICAL_REFLECTION:			pCharAtt->CalculatePhysicalReflection(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_LP_RECOVERY:					pCharAtt->CalculateLpRecoveryWhenHit(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_EP_RECOVERY:					pCharAtt->CalculateEpRecoveryWhenHit(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_LP_RECOVERY_IN_PERCENT:			pCharAtt->CalculateLpRecoveryWhenHitInPercent(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_EP_RECOVERY_IN_PERCENT:			pCharAtt->CalculateEpRecoveryWhenHitInPercent(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_STOMACHACHE_DEFENCE:		pCharAtt->CalculateStomachacheDefence(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_POISON_DEFENCE:				pCharAtt->CalculatePoisonDefence(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_BLEED_DEFENCE:				pCharAtt->CalculateBleedDefence(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;
		case ACTIVE_BURN_DEFENCE:				pCharAtt->CalculateBurnDefence(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;

		case ACTIVE_VIABILITY:					pCharAtt->CalculateBlockRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);	break;

		case ACTIVE_MAX_RP_DOWN:				pCharAtt->CalculateMaxRP(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);	break;

		case ACTIVE_PHYSICAL_ARMOR_PEN_UP:	pCharAtt->CalculatePhysicalArmorPen(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true); break;
		case ACTIVE_ENERGY_ARMOR_PEN_UP:	pCharAtt->CalculateEnergyArmorPen(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true); break;
		case ACTIVE_ARMOR_PEN_UP:			pCharAtt->CalculatePhysicalArmorPen(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
											pCharAtt->CalculateEnergyArmorPen(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true); break;
		case ACTIVE_CRITICAL_DEFENSE_UP:	pCharAtt->CalculatePhysicalCriticalDefenceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
											pCharAtt->CalculateEnergyCriticalDefenceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true); break;

		case ACTIVE_PHYSICAL_CRITICAL_DEFENSE_UP:	pCharAtt->CalculatePhysicalCriticalDefenceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true); break;
		case ACTIVE_ENERGY_CRITICAL_DEFENSE_UP:		pCharAtt->CalculateEnergyCriticalDefenceRate(effectvalue, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true); break;

		default: ERR_LOG(LOG_GENERAL, "Dbo_SetRandomOptionValues: effectcode %u not set yet \n", effectcode); break;
	}
}


