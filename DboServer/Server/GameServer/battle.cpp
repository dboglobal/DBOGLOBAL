#include "stdafx.h"
#include "battle.h"
#include "calcs.h"
#include "Npc.h"
#include "CPlayer.h"
#include "FormulaTable.h"


bool BattleIsCrit(CCharacterAtt* pAttackerAtt, CCharacterAtt* pTargetAtt, bool bIsPhysical)
{
	float fRate = 0.0f;
	float fVar;
	float fVar2;
	float fCritRate;

	if (bIsPhysical == false)
	{
		fCritRate = (float)pAttackerAtt->GetEnergyCriticalRate();

		if (pAttackerAtt->GetFoc() < 170)
			fVar = 37.f;
		else
			fVar = 21.f;
			

		fVar2 = (float)pAttackerAtt->GetFoc();
	}
	else
	{
		fCritRate = (float)pAttackerAtt->GetPhysicalCriticalRate();

		if (pAttackerAtt->GetDex() < 220)
			fVar = 40.f;
		else
			fVar = 28.f;

		fVar2 = (float)pAttackerAtt->GetDex();
	}

	fRate = fCritRate - (fCritRate * (pTargetAtt->GetCriticalBlockSuccessRate() / 100.f)) + fVar2 / fVar;

	//if (0.0f >= fCritBlockRate)
	//{
	//	fRate = fCritRate;
	//}
	//else
	//{
	//	fRate = fCritRate - (fCritRate * (fCritBlockRate / 100.f));
	//}

	//NTL_PRINT(PRINT_APP, "fRate = %f, fVar = %f, fVar2 = %f, fCritRate = %f, CritBlockRate = %f", fRate, fVar, fVar2, fCritRate, pTargetAtt->GetCriticalBlockSuccessRate());

	if (fRate > 90.0f)
		fRate = 90.0f;

	return Dbo_CheckProbabilityF(fRate);
}

//--------------------------------------------------------------------------------------//
//		IS DODGED?
//--------------------------------------------------------------------------------------//
bool BattleIsDodge(bool bTargetPC, WORD hitrate, WORD dodge, BYTE byAttackerLv, BYTE byTargetLv)
{
	float fRate = 100.0f - (CFormulaTable::m_afRate[3700][1] * float((float)hitrate / (float)MAX(hitrate + dodge, 1)) * float(float(byAttackerLv + 1) / float(byAttackerLv + byTargetLv)) * 100.0f);

	if (fRate > 90.f)
		fRate = 90.0f;

	//if(bTargetPC)
	//	NTL_PRINT(PRINT_APP, "BattleIsDodge: dodge percent %f, hitrate %u, dodge %u, byAttackerLv %u, byTargetLv %u \n", fRate, hitrate, dodge, byAttackerLv, byTargetLv);

	return Dbo_CheckProbabilityF(fRate);
}


//--------------------------------------------------------------------------------------//
//		IS RESIST CURSE?
//--------------------------------------------------------------------------------------//
bool BattleIsResist(WORD wSuccessRate, WORD wResistRate, BYTE byAttackerLv, BYTE byTargetLv)
{
	float fRate = 100.0f - (CFormulaTable::m_afRate[3900][1] * float((float)wSuccessRate / (float)MAX(wSuccessRate + wResistRate, 1)) * float(float(byAttackerLv + 1) / float(byAttackerLv + byTargetLv)) * 100.0f);

	if (fRate > 90.f)
		fRate = 90.f;

	//NTL_PRINT(PRINT_APP, "BattleIsResist: curse resist percent %f, wSuccessRate %u, wResistRate %u, byAttackerLv %u, byTargetLv %u \n", fRate, wSuccessRate, wResistRate, byAttackerLv, byTargetLv);

	return Dbo_CheckProbabilityF(fRate);
}

bool BattleIsBlock(WORD wDefenceRate, BYTE byAttackerLv, BYTE byTargetLv)
{
	//float fRate = ((float)wDefenceRate - ((float)byTargetLv / ((float)byAttackerLv * 0.13f)) + 0.02f) / 3.5f;
	float fRate = ((float)wDefenceRate * 2.f + (byTargetLv - byAttackerLv)) / 200.f;
	if (fRate > 20.f)
		fRate = 20.f;

	//NTL_PRINT(PRINT_APP, "fRate:%f, wDefenceRate:%u, byAttackerLv:%u, byTargetLv:%u", fRate, wDefenceRate, byAttackerLv, byTargetLv);

	return Dbo_CheckProbabilityF(fRate);
}


//-----------------------------------------------------------------------------------------------------------//
//											CALCULATE SKILL DAMAGE
//-----------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CalcSkillDamage(CCharacterObject* pCaster, CCharacterObject* victim, sSKILL_TBLDAT* skilltbl, BYTE byEffectNr, float fBaseSkillDmg, float& resultvalue, BYTE& rAttackResult, int& rfReflectDmg, sDBO_LP_EP_RECOVERED* pLpEpRecover, bool bIncreaseDmg/* = false*/, bool bAttackFromBehindBonus/* = false*/)
{
	float  fFinalDamage = 0.0f, min_damage = 0.0f, max_damage = 0.0f, fAttackerPower = 0.0f, fTargetDefensePower = 0.0f, fCritDmgRate = 0.0f, fCritDefRate = 0.0f;

	/* INFO:
		- At "value" damage, we do not add weapon offence. See https://youtu.be/cj5E1dOIYfk?t=155 as proof. With weapon offence added we deal much more dmg. Without we deal exact the same damage. State needs to be figured out.
		- State calculation is / 1.5 instead of 2.0. Watch https://youtu.be/JlBDzAmoNTk?t=317 for more info.
	*/

	CCharacterAtt* pCasterAtt = pCaster->GetCharAtt();

	//printf("skilltbl->bySkill_Type %u, skilltbl->bySkill_Effect_Type[byEffectNr] %u \n", skilltbl->bySkill_Type, skilltbl->bySkill_Effect_Type[byEffectNr]);
	if (skilltbl->bySkill_Effect_Type[byEffectNr] == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
	{
		if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL)
		{
			fAttackerPower = fBaseSkillDmg;

			fTargetDefensePower = (float)victim->GetCharAtt()->GetPhysicalDefence();

			// <armor pen> decrease def
			fTargetDefensePower -= pCaster->GetCharAtt()->GetPhysicalArmorPenRate() * fTargetDefensePower / 100.f;

			// critical dmg def
			fCritDefRate = victim->GetCharAtt()->GetPhysicalCriticalDefenceRate();
		}
		else if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_ENERGY)
		{
			fAttackerPower = fBaseSkillDmg;

			fTargetDefensePower = (float)victim->GetCharAtt()->GetEnergyDefence();

			// <armor pen> decrease def
			fTargetDefensePower -= pCaster->GetCharAtt()->GetEnergyArmorPenRate() * fTargetDefensePower / 100.f;

			// critical dmg def
			fCritDefRate = victim->GetCharAtt()->GetEnergyCriticalDefenceRate();
		}
		else if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_STATE)
		{
			if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
				fAttackerPower = fBaseSkillDmg + float(pCaster->GetCharAtt()->GetSubWeaponPhysicalOffence() + pCaster->GetCharAtt()->GetSubWeaponEnergyOffence() / 1.5f);
			else
				fAttackerPower = fBaseSkillDmg + float(pCaster->GetCharAtt()->GetPhysicalOffence() + pCaster->GetCharAtt()->GetSubWeaponEnergyOffence() / 1.5f);

			fTargetDefensePower = float(victim->GetCharAtt()->GetPhysicalDefence() + victim->GetCharAtt()->GetEnergyOffence() / 1.5f);

			// <armor pen> decrease def
			fTargetDefensePower -= ((pCaster->GetCharAtt()->GetPhysicalArmorPenRate() + pCaster->GetCharAtt()->GetEnergyArmorPenRate()) / 2.f) * fTargetDefensePower / 100.f;

			// critical dmg def
			fCritDefRate = (victim->GetCharAtt()->GetPhysicalCriticalDefenceRate() + victim->GetCharAtt()->GetEnergyCriticalDefenceRate()) / 2.f;
		}
	}
	else if (skilltbl->bySkill_Effect_Type[byEffectNr] == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
	{
		if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL)
		{
			if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
				fAttackerPower = (float)pCaster->GetCharAtt()->GetSubWeaponPhysicalOffence() + (((float)pCaster->GetCharAtt()->GetSubWeaponPhysicalOffence() * fBaseSkillDmg) / 100.f);
			else
				fAttackerPower = (float)pCaster->GetCharAtt()->GetPhysicalOffence() + (((float)pCaster->GetCharAtt()->GetPhysicalOffence() * fBaseSkillDmg) / 100.f);

			fTargetDefensePower = (float)victim->GetCharAtt()->GetPhysicalDefence();

			// <armor pen> decrease def
			fTargetDefensePower -= pCaster->GetCharAtt()->GetPhysicalArmorPenRate() * fTargetDefensePower / 100.f;

			// critical dmg def
			fCritDefRate = victim->GetCharAtt()->GetPhysicalCriticalDefenceRate();
		}
		else if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_ENERGY)
		{
			if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
				fAttackerPower = (float)pCaster->GetCharAtt()->GetSubWeaponEnergyOffence() + (((float)pCaster->GetCharAtt()->GetSubWeaponEnergyOffence() * fBaseSkillDmg) / 100.f);
			else
				fAttackerPower = (float)pCaster->GetCharAtt()->GetEnergyOffence() + (((float)pCaster->GetCharAtt()->GetEnergyOffence() * fBaseSkillDmg) / 100.f);

			fTargetDefensePower = (float)victim->GetCharAtt()->GetEnergyDefence();

			// <armor pen> decrease def
			fTargetDefensePower -= pCaster->GetCharAtt()->GetEnergyArmorPenRate() * fTargetDefensePower / 100.f;

			// critical dmg def
			fCritDefRate = victim->GetCharAtt()->GetEnergyCriticalDefenceRate();
		}
		else if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_STATE)
		{
			float fStateOffence = 0.f;
			if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
				fStateOffence = ((float)pCaster->GetCharAtt()->GetSubWeaponPhysicalOffence() + (float)pCaster->GetCharAtt()->GetSubWeaponEnergyOffence()) / 1.5f;
			else
				fStateOffence = ((float)pCaster->GetCharAtt()->GetPhysicalOffence() + (float)pCaster->GetCharAtt()->GetEnergyOffence()) / 1.5f;

			fAttackerPower = (fStateOffence * fBaseSkillDmg) / 100.f;
			fTargetDefensePower = (float)victim->GetCharAtt()->GetPhysicalDefence() + (float)victim->GetCharAtt()->GetEnergyDefence() / 1.5f;

			// <armor pen> decrease def
			fTargetDefensePower -= ((pCaster->GetCharAtt()->GetPhysicalArmorPenRate() + pCaster->GetCharAtt()->GetEnergyArmorPenRate()) / 2.f) * fTargetDefensePower / 100.f;

			// critical dmg def
			fCritDefRate = (victim->GetCharAtt()->GetPhysicalCriticalDefenceRate() + victim->GetCharAtt()->GetEnergyCriticalDefenceRate()) / 2.f;
		}
	}

	float fDmg0 = fAttackerPower * (1.f - (fTargetDefensePower / (fTargetDefensePower + (float)pCaster->GetLevel() * CFormulaTable::m_afRate[3100][1])));

	min_damage = fDmg0 * (CFormulaTable::m_afRate[3500][1] + ((float)pCaster->GetLevel() * CFormulaTable::m_afRate[3500][2]));
	max_damage = fDmg0 * (CFormulaTable::m_afRate[3500][3] - ((float)pCaster->GetLevel() * CFormulaTable::m_afRate[3500][4]));

	fFinalDamage = RandomRangeF(min_damage, max_damage);

	//printf("fDmg0 %f fDmg1 %f fAttributeBonusRate %f, min_damage %f, max_damage %f\n", fDmg0, fDmg1, fAttributeBonusRate, min_damage, max_damage);
	resultvalue = (fFinalDamage <= 1.f) ? 1.f : fFinalDamage;

	//---------------//

	//check if crit success and add damage
	if (rAttackResult == BATTLE_ATTACK_RESULT_CRITICAL_HIT)		//only add crit dmg once
	{
		float fCritDmgBonus = 0.0f;

		if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL) //check if physical dmg
		{
			fCritDmgRate = pCasterAtt->GetPhysicalCriticalDamageRate();
		}
		else if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_ENERGY) //check if energy dmg
		{
			fCritDmgRate = pCasterAtt->GetEnergyCriticalDamageRate();
		}
		else if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_STATE)
		{
			fCritDmgRate = (pCasterAtt->GetPhysicalCriticalDamageRate() + pCasterAtt->GetEnergyCriticalDamageRate()) / 2.f;
		}

		fCritDmgBonus = ((resultvalue * fCritDmgRate) / 100.f);

		if (bIncreaseDmg) // check if has increase crit dmg effect bonus
		{
			fCritDmgBonus *= (DBO_BATTLE_OFFENCE_BONUS_RATE_BY_CRITICAL / 100.f);
		}

		fCritDmgBonus -= fCritDmgBonus * fCritDefRate / 100.f;

		resultvalue += fCritDmgBonus;
	}



	//---------------//
	//reflect dmg
	rfReflectDmg += (int)GetSkillReflectDamage(resultvalue, skilltbl->bySkill_Type, victim->GetCharAtt()->GetPhysicalReflection(), victim->GetCharAtt()->GetEnergyReflection());

	//---------------//
	if (pLpEpRecover)
	{
		//lp ep recover
		pLpEpRecover->targetLpRecoveredWhenHit = (int)(victim->GetCharAtt()->GetLpRecoveryWhenHit() + (resultvalue * victim->GetCharAtt()->GetLpRecoveryWhenHitInPercent() / 100.0f));
		if (pLpEpRecover->targetLpRecoveredWhenHit > 0)
			pLpEpRecover->bIsLpRecoveredWhenHit = true;

		//printf("resultvalue %f rLpEpRecover.targetLpRecoveredWhenHit %u, GetLpRecoveryWhenHitInPercent %f, GetLpRecoveryWhenHit %u, %f \n", 
		//	resultvalue, rLpEpRecover.targetLpRecoveredWhenHit, victim->GetCharAtt()->GetLpRecoveryWhenHitInPercent(), victim->GetCharAtt()->GetLpRecoveryWhenHit(), resultvalue * victim->GetCharAtt()->GetLpRecoveryWhenHitInPercent() / 100.0f);

		pLpEpRecover->dwTargetEpRecoveredWhenHit = (DWORD)(victim->GetCharAtt()->GetEpRecoveryWhenHit() + (resultvalue * victim->GetCharAtt()->GetEpRecoveryWhenHitInPercent() / 100.0f));
		if (pLpEpRecover->dwTargetEpRecoveredWhenHit > 0)
			pLpEpRecover->bIsEpRecoveredWhenHit = true;
	}

	//ERR_LOG(LOG_USER,"Attacker Lv: %d Victim Lv %d Victim Obj Type %d CalcSkillDamage: %f \n", ch->GetLevel(), victim->GetLevel(), victim->GetObjType(), resultvalue);
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CalcSpecialSkillDamage(CCharacterObject* pCaster, CCharacterObject* victim, sSKILL_TBLDAT* skilltbl, BYTE byEffectNr, float fBaseSkillDmg, float& resultvalue, BYTE& rAttackResult, int& rfReflectDmg, sDBO_LP_EP_RECOVERED& rLpEpRecover)
{
	float  fFinalDamage = 0.0f, min_damage = 0.0f, max_damage = 0.0f, fAttackerPower = 0.0f, fTargetDefensePower = 0.0f, fCritDefRate = 0.0f;
	float fCritDmgRate = 0.0f;

	if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL)
	{
		if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
			fAttackerPower = ((float)pCaster->GetCharAtt()->GetSubWeaponPhysicalOffence() * (fBaseSkillDmg / 2.f)) / 100.f;
		else
			fAttackerPower = ((float)pCaster->GetCharAtt()->GetPhysicalOffence() * (fBaseSkillDmg / 2.f)) / 100.f;

		fTargetDefensePower = (float)victim->GetCharAtt()->GetPhysicalDefence();

		// <armor pen> decrease def
		fTargetDefensePower -= pCaster->GetCharAtt()->GetPhysicalArmorPenRate() * fTargetDefensePower / 100.f;

		// critical dmg def
		fCritDefRate = victim->GetCharAtt()->GetPhysicalCriticalDefenceRate();
	}
	else if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_ENERGY)
	{
		if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
			fAttackerPower = ((float)pCaster->GetCharAtt()->GetSubWeaponEnergyOffence() * (fBaseSkillDmg / 2.f)) / 100.f;
		else
			fAttackerPower = ((float)pCaster->GetCharAtt()->GetEnergyOffence() * (fBaseSkillDmg / 2.f)) / 100.f;

		fTargetDefensePower = (float)victim->GetCharAtt()->GetEnergyDefence();

		// <armor pen> decrease def
		fTargetDefensePower -= pCaster->GetCharAtt()->GetEnergyArmorPenRate() * fTargetDefensePower / 100.f;

		// critical dmg def
		fCritDefRate = victim->GetCharAtt()->GetEnergyCriticalDefenceRate();
	}
	else if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_STATE)
	{
		float fStateOffence = 0.f;
		if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
			fStateOffence = ((float)pCaster->GetCharAtt()->GetSubWeaponPhysicalOffence() + (float)pCaster->GetCharAtt()->GetSubWeaponEnergyOffence()) / 2.f;
		else
			fStateOffence = ((float)pCaster->GetCharAtt()->GetPhysicalOffence() + (float)pCaster->GetCharAtt()->GetEnergyOffence()) / 2.f;

		fAttackerPower = (fStateOffence * fBaseSkillDmg) / 100.f;
		fTargetDefensePower = (float)victim->GetCharAtt()->GetPhysicalDefence() + (float)victim->GetCharAtt()->GetEnergyDefence() / 2.f;

		// <armor pen> decrease def
		fTargetDefensePower -= ((pCaster->GetCharAtt()->GetPhysicalArmorPenRate() + pCaster->GetCharAtt()->GetEnergyArmorPenRate()) / 2.f) * fTargetDefensePower / 100.f;

		// critical dmg def
		fCritDefRate = (victim->GetCharAtt()->GetPhysicalCriticalDefenceRate() + victim->GetCharAtt()->GetEnergyCriticalDefenceRate()) / 2.f;
	}

	float fDmg0 = fAttackerPower * (1.f - (fTargetDefensePower / (fTargetDefensePower + (float)pCaster->GetLevel() * 15.f)));

	min_damage = fDmg0 * (CFormulaTable::m_afRate[3500][1] + ((float)pCaster->GetLevel() * CFormulaTable::m_afRate[3500][2]));
	max_damage = fDmg0 * (CFormulaTable::m_afRate[3500][3] - ((float)pCaster->GetLevel() * CFormulaTable::m_afRate[3500][4]));

	fFinalDamage = RandomRangeF(min_damage, max_damage);

	resultvalue = (fFinalDamage <= 1.f) ? 1.f : fFinalDamage;

	//---------------//

	//check if crit success and add damage
	if (rAttackResult == BATTLE_ATTACK_RESULT_CRITICAL_HIT)		//only add crit dmg once
	{
		float fCritDmgBonus = 0.0f;

		if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL) //check if physical dmg
		{
			fCritDmgRate = pCaster->GetCharAtt()->GetPhysicalCriticalDamageRate();
		}
		else if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_ENERGY) //check if energy dmg
		{
			fCritDmgRate = pCaster->GetCharAtt()->GetEnergyCriticalDamageRate();
		}
		else if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_STATE)
		{
			fCritDmgRate = (pCaster->GetCharAtt()->GetPhysicalCriticalDamageRate() + pCaster->GetCharAtt()->GetEnergyCriticalDamageRate()) / 2.f;
		}

		fCritDmgBonus = ((resultvalue * fCritDmgRate) / 100.f);

		fCritDmgBonus -= fCritDmgBonus * fCritDefRate / 100.f;

		resultvalue += fCritDmgBonus;
	}

	//---------------//
	//reflect dmg
	rfReflectDmg += (int)GetSkillReflectDamage(resultvalue, skilltbl->bySkill_Type, victim->GetCharAtt()->GetPhysicalReflection(), victim->GetCharAtt()->GetEnergyReflection());

	//---------------//
	//lp ep recover
	rLpEpRecover.targetLpRecoveredWhenHit += (int)(victim->GetCharAtt()->GetLpRecoveryWhenHit() + (resultvalue * victim->GetCharAtt()->GetLpRecoveryWhenHitInPercent() / 100.0f));
	if (rLpEpRecover.targetLpRecoveredWhenHit > 0)
		rLpEpRecover.bIsLpRecoveredWhenHit = true;

	rLpEpRecover.dwTargetEpRecoveredWhenHit += (DWORD)(victim->GetCharAtt()->GetEpRecoveryWhenHit() + (resultvalue * victim->GetCharAtt()->GetEpRecoveryWhenHitInPercent() / 100.0f));
	if (rLpEpRecover.dwTargetEpRecoveredWhenHit > 0)
		rLpEpRecover.bIsEpRecoveredWhenHit = true;

	//ERR_LOG(LOG_USER,"Attacker Lv: %d Victim Lv %d Victim Obj Type %d CalcSkillDamage: %f \n", ch->GetLevel(), victim->GetLevel(), victim->GetObjType(), resultvalue);
}


void CalcSkillDotDamage(CCharacterObject * pCaster, CCharacterObject * victim, sSKILL_TBLDAT * skilltbl, BYTE byEffectNr, WORD wDefence, float fBaseSkillDmg, float fBonusDmg, float & resultvalue, BYTE rAttackResult, BYTE byEffectCode)
{
	float  fFinalDamage = 0.0f, min_damage = 0.0f, max_damage = 0.0f, fAttackerPower = 0.0f, fCritDmgRate = 0.0f, fCritDefRate = 0.0f;
	float fTargetDefensePower = (float)wDefence;

	if (skilltbl->bySkill_Effect_Type[byEffectNr] == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
	{
		fAttackerPower = fBaseSkillDmg;
		fFinalDamage = fAttackerPower * (1.f - (fTargetDefensePower / (fTargetDefensePower + (float)pCaster->GetLevel() * 25.f)));
	}
	else if (skilltbl->bySkill_Effect_Type[byEffectNr] == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
	{
		if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL)
		{
			if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
				fAttackerPower = ((float)pCaster->GetCharAtt()->GetSubWeaponPhysicalOffence() * fBaseSkillDmg) / 100.f;
			else
				fAttackerPower = ((float)pCaster->GetCharAtt()->GetPhysicalOffence() * fBaseSkillDmg) / 100.f;

			fTargetDefensePower += (float)victim->GetCharAtt()->GetPhysicalDefence();

			// <armor pen> decrease def
			fTargetDefensePower -= pCaster->GetCharAtt()->GetPhysicalArmorPenRate() * fTargetDefensePower / 100.f;

			// critical dmg def
			fCritDefRate = victim->GetCharAtt()->GetPhysicalCriticalDefenceRate();
		}
		else if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_ENERGY)
		{
			if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
				fAttackerPower = ((float)pCaster->GetCharAtt()->GetSubWeaponEnergyOffence() * fBaseSkillDmg) / 100.f;
			else
				fAttackerPower = ((float)pCaster->GetCharAtt()->GetEnergyOffence() * fBaseSkillDmg) / 100.f;

			fTargetDefensePower += (float)victim->GetCharAtt()->GetEnergyDefence();

			// <armor pen> decrease def
			fTargetDefensePower -= pCaster->GetCharAtt()->GetEnergyArmorPenRate() * fTargetDefensePower / 100.f;

			// critical dmg def
			fCritDefRate = victim->GetCharAtt()->GetEnergyCriticalDefenceRate();
		}
		else if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_STATE)
		{
			float fStateOffence;

			if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
				fStateOffence = ((float)pCaster->GetCharAtt()->GetSubWeaponPhysicalOffence() + (float)pCaster->GetCharAtt()->GetSubWeaponEnergyOffence()) / 2.f;
			else
				fStateOffence = ((float)pCaster->GetCharAtt()->GetPhysicalOffence() + (float)pCaster->GetCharAtt()->GetEnergyOffence()) / 2.f;

			fAttackerPower = (fStateOffence * fBaseSkillDmg) / 100.f;
			fTargetDefensePower += ((float)victim->GetCharAtt()->GetPhysicalDefence() + (float)victim->GetCharAtt()->GetEnergyDefence()) / 2.f;

			// <armor pen> decrease def
			fTargetDefensePower -= ((pCaster->GetCharAtt()->GetPhysicalArmorPenRate() + pCaster->GetCharAtt()->GetEnergyArmorPenRate()) / 2.f) * fTargetDefensePower / 100.f;

			// critical dmg def
			fCritDefRate = (victim->GetCharAtt()->GetPhysicalCriticalDefenceRate() + victim->GetCharAtt()->GetEnergyCriticalDefenceRate()) / 2.f;
		}

		fFinalDamage = fAttackerPower * (1.f - (fTargetDefensePower / (fTargetDefensePower + (float)pCaster->GetLevel() * 35.f)));
	}

	fFinalDamage += fBonusDmg;

	if (victim->IsPC())
	{
		if(byEffectCode == ACTIVE_BLEED || ACTIVE_BURN || wDefence < 1)
			fFinalDamage -= (float)wDefence;
		else
			fFinalDamage -= (float)wDefence / 2.0f;
	}

	resultvalue = (fFinalDamage <= 1.f) ? 1.f : fFinalDamage;

	//---------------//

	//check if crit success and add damage
	if (rAttackResult == BATTLE_ATTACK_RESULT_CRITICAL_HIT)		//only add crit dmg once
	{
		float fCritDmgBonus = 0.0f;

		if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL) //check if physical dmg
		{
			fCritDmgRate = pCaster->GetCharAtt()->GetPhysicalCriticalDamageRate() / 2.f;
		}
		else if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_ENERGY) //check if energy dmg
		{
			fCritDmgRate = pCaster->GetCharAtt()->GetEnergyCriticalDamageRate() / 2.f;
		}
		else if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_STATE)
		{
			fCritDmgRate = (pCaster->GetCharAtt()->GetPhysicalCriticalDamageRate() + pCaster->GetCharAtt()->GetEnergyCriticalDamageRate()) / 4.f;
		}

		fCritDmgBonus = ((resultvalue * fCritDmgRate) / 100.f);

		fCritDmgBonus -= fCritDmgBonus * fCritDefRate / 100.f;

		resultvalue += fCritDmgBonus;
	}
}


void CalcLifeStealDamage(CCharacterObject* pCaster, CCharacterObject* victim, sSKILL_TBLDAT* skilltbl, BYTE byEffectNr, float fBaseSkillDmg, float& resultvalue)
{
	float  fFinalDamage = 0.0f, fAttackerPower = 0.0f, fTargetDefensePower = 0.0f;

	if (skilltbl->bySkill_Type == NTL_SKILL_TYPE_ENERGY)
	{
		if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
			fAttackerPower = fBaseSkillDmg + (float)pCaster->GetCharAtt()->GetSubWeaponEnergyOffence();
		else
			fAttackerPower = fBaseSkillDmg + (float)pCaster->GetCharAtt()->GetEnergyOffence();

		fTargetDefensePower = (float)victim->GetCharAtt()->GetEnergyDefence();

		// <armor pen> decrease def
		fTargetDefensePower -= pCaster->GetCharAtt()->GetEnergyArmorPenRate() * fTargetDefensePower / 100.f;
	}
	else
	{
		if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
			fAttackerPower = fBaseSkillDmg + (float)pCaster->GetCharAtt()->GetSubWeaponPhysicalOffence();
		else
			fAttackerPower = fBaseSkillDmg + (float)pCaster->GetCharAtt()->GetPhysicalOffence();

		fTargetDefensePower = (float)victim->GetCharAtt()->GetPhysicalDefence();

		// <armor pen> decrease def
		fTargetDefensePower -= pCaster->GetCharAtt()->GetPhysicalArmorPenRate() * fTargetDefensePower / 100.f;
	}

	fFinalDamage = fAttackerPower * (1.f - (fTargetDefensePower / (fTargetDefensePower + (float)pCaster->GetLevel() * 35.f)));

	resultvalue = (fFinalDamage <= 1.f) ? 1.f : fFinalDamage;

	//---------------//
}


//--------------------------------------------------------------------------------------//
//		CALCULATE NORMAL ATTACK DAMAGE
//--------------------------------------------------------------------------------------//
float CalcMeleeDamage(CCharacter* pkAttacker, CCharacter* pkVictim)
{
	float  fFinalDamage = 0.0f, min_damage = 0.0f, max_damage = 0.0f, fAttackerPower = 0.0f, fTargetDefensePower = 0.0f;
	
	if (pkAttacker->GetAttackType() == BATTLE_ATTACK_TYPE_ENERGY)
	{
		fAttackerPower = (float)pkAttacker->GetCharAtt()->GetEnergyOffence();
		fTargetDefensePower = (float)pkVictim->GetCharAtt()->GetEnergyDefence();

		// <armor pen> decrease def
		fTargetDefensePower -= pkAttacker->GetCharAtt()->GetEnergyArmorPenRate() * fTargetDefensePower / 100.f;
	}
	else
	{
		fAttackerPower = (float)pkAttacker->GetCharAtt()->GetPhysicalOffence();
		fTargetDefensePower = (float)pkVictim->GetCharAtt()->GetPhysicalDefence();

		// <armor pen> decrease def
		fTargetDefensePower -= pkAttacker->GetCharAtt()->GetPhysicalArmorPenRate() * fTargetDefensePower / 100.f;
	}

	float fDmg0 = fAttackerPower * (1.f - (fTargetDefensePower / (fTargetDefensePower + (float)pkAttacker->GetLevel() * CFormulaTable::m_afRate[3100][1])));

	min_damage = fDmg0 * (CFormulaTable::m_afRate[3500][1] + ((float)pkAttacker->GetLevel() * CFormulaTable::m_afRate[3500][2]));
	max_damage = fDmg0 * (CFormulaTable::m_afRate[3500][3] - ((float)pkAttacker->GetLevel() * CFormulaTable::m_afRate[3500][4]));

	fFinalDamage = RandomRangeF(min_damage, max_damage);

	//if (pkAttacker->IsPC())
	//	printf("fAttackerPower %f, fTargetDefensePower %f, min_damage %f, max_damage %f, fFinalDamage %f, fAttributeBonusRate %f, fDmg0 %f, fDmg1 %f\n", fAttackerPower, fTargetDefensePower, min_damage, max_damage, fFinalDamage, fAttributeBonusRate, fDmg0, fDmg1);

	return (fFinalDamage <= 1.f) ? 1.f : fFinalDamage;
}


void CalcDirectHeal(CCharacterObject * pCaster, sSKILL_TBLDAT * skilltbl, BYTE byEffectNr, float & resultvalue)
{
	resultvalue = (float)skilltbl->aSkill_Effect_Value[byEffectNr];

	if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
	{
		//add weapon offence to healing power
		resultvalue += (float)pCaster->GetCharAtt()->GetSubWeaponEnergyOffence();

		// Add % heal bonus (offence * %)
		resultvalue += (float)pCaster->GetCharAtt()->GetSubWeaponEnergyOffence() * pCaster->GetCharAtt()->GetDirectHealPowerBonusInPercent() / 100.f;
	}
	else
	{
		//add weapon offence to healing power
		resultvalue += (float)pCaster->GetCharAtt()->GetEnergyOffence();

		// Add % heal bonus (offence * %)
		resultvalue += (float)pCaster->GetCharAtt()->GetEnergyOffence() * pCaster->GetCharAtt()->GetDirectHealPowerBonusInPercent() / 100.f;
	}

	// Add Static Bonus
	resultvalue += pCaster->GetCharAtt()->GetDirectHealPowerBonus();

	//NTL_PRINT(PRINT_APP,"resultvalue %f, GetSubWeaponEnergyOffence %u, GetEnergyOffence %u, GetDirectHealPowerBonusInPercent %f, GetDirectHealPowerBonus %f \n", 
	//	resultvalue, pCaster->GetCharAtt()->GetSubWeaponEnergyOffence(), pCaster->GetCharAtt()->GetEnergyOffence(), pCaster->GetCharAtt()->GetDirectHealPowerBonusInPercent(), pCaster->GetCharAtt()->GetDirectHealPowerBonus());
}


void CalcHealOverTime(CCharacterObject * pCaster, sSKILL_TBLDAT * skilltbl, BYTE byEffectNr, float & resultvalue)
{
	resultvalue = (float)skilltbl->aSkill_Effect_Value[byEffectNr];

	if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
	{
		// Add % heal bonus (offence * %)
		resultvalue += (float)pCaster->GetCharAtt()->GetSubWeaponEnergyOffence() * pCaster->GetCharAtt()->GetHotPowerBonusInPercent() / 100.f;
	}
	else
	{
		// Add % heal bonus (offence * %)
		resultvalue += (float)pCaster->GetCharAtt()->GetEnergyOffence() * pCaster->GetCharAtt()->GetHotPowerBonusInPercent() / 100.f;
	}

	// Add Static Bonus
	resultvalue += pCaster->GetCharAtt()->GetHotPowerBonus();
}


//--------------------------------------------------------------------------------------//
//		INCREASES THE AGGRO FROM MONSTER WHICH ARE ATTACKING pTARGET (USED WHEN PCASTER HEAL PTARGET)
//--------------------------------------------------------------------------------------//
void IncreaseTargetEnemyAggro(CCharacter * pCaster, CCharacter * pTarget, DWORD dwDefaultAggro)
{
	int nAgro = (int)(dwDefaultAggro + pCaster->GetCharAtt()->GetSkillAggroBonus());
	nAgro += (int)((float)nAgro * pCaster->GetCharAtt()->GetSkillAggroBonusInPercent() / 100.f);

	auto it = pTarget->GetTargetListManager()->AggroBegin();
	auto itEnd = pTarget->GetTargetListManager()->AggroEnd();

	int nLoopCount = 0;

	while (it != itEnd)
	{
		++nLoopCount;
		if (nLoopCount > 5000)
		{
			ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
		}

		CCharacter* pAttacker = g_pObjectManager->GetChar(it->first);
		if (pAttacker && pAttacker->IsInitialized())
		{
			if (pAttacker->IsNPC() || pAttacker->IsMonster())
			{
				pAttacker->ChangeAggro(pCaster->GetID(), DBO_AGGRO_CHANGE_TYPE_INCREASE, (DWORD)nAgro);
			}
		}

		++it;
	}
}

float GetReflectDamage(float fDmg, BYTE byAttackType, float fPhysicalReflect, float fEnergyReflect)
{
	if (byAttackType == BATTLE_ATTACK_TYPE_PHYSICAL)
	{
		return fDmg * fPhysicalReflect / 100.0f;
	}
	else if (byAttackType == BATTLE_ATTACK_TYPE_ENERGY)
	{
		return fDmg * fEnergyReflect / 100.0f;
	}

	return 0.0f;
}

float GetSkillReflectDamage(float fDmg, BYTE bySkillType, float fPhysicalReflect, float fEnergyReflect)
{
	if (bySkillType == NTL_SKILL_TYPE_PHYSICAL)
	{
		return fDmg * fPhysicalReflect / 100.0f;
	}
	else if (bySkillType == NTL_SKILL_TYPE_ENERGY)
	{
		return fDmg * fEnergyReflect / 100.0f;
	}

	return 0.0f;
}

//float GetAttributeBonusRate(bool bIsPc, bool bSubWeapon, BYTE byOffence, BYTE byDefence, BYTE bySubOffence, sAVATAR_ATTRIBUTE& sOffenceAttribute, sAVATAR_ATTRIBUTE& sDefenceAttribute)
//{
//	float fAttributeBonusRate = 0.0f;
//	BYTE byAttrOffence;
//
//	if (bSubWeapon)
//	{
//		byAttrOffence = bySubOffence;
//		fAttributeBonusRate = NtlGetBattleAttributeBonusRate(bySubOffence, byDefence);
//	}
//	else
//	{
//		byAttrOffence = byOffence;
//		fAttributeBonusRate = NtlGetBattleAttributeBonusRate(byOffence, byDefence);
//	}
//
//	switch (byAttrOffence)
//	{
//		case BATTLE_ATTRIBUTE_HONEST: fAttributeBonusRate += sOffenceAttribute.fHonestOffence - sDefenceAttribute.fHonestDefence; break;
//		case BATTLE_ATTRIBUTE_STRANGE: fAttributeBonusRate += sOffenceAttribute.fStrangeOffence - sDefenceAttribute.fStrangeDefence; break;
//		case BATTLE_ATTRIBUTE_WILD: fAttributeBonusRate += sOffenceAttribute.fWildOffence - sDefenceAttribute.fWildDefence; break;
//		case BATTLE_ATTRIBUTE_ELEGANCE: fAttributeBonusRate += sOffenceAttribute.fEleganceOffence - sDefenceAttribute.fEleganceDefence; break;
//		case BATTLE_ATTRIBUTE_FUNNY: fAttributeBonusRate += sOffenceAttribute.fFunnyOffence - sDefenceAttribute.fFunnyDefence; break;
//
//		default:
//		{
//			switch (byDefence)
//			{
//				case BATTLE_ATTRIBUTE_HONEST: fAttributeBonusRate -= sDefenceAttribute.fHonestDefence; break;
//				case BATTLE_ATTRIBUTE_STRANGE: fAttributeBonusRate -= sDefenceAttribute.fStrangeDefence; break;
//				case BATTLE_ATTRIBUTE_WILD: fAttributeBonusRate -= sDefenceAttribute.fWildDefence; break;
//				case BATTLE_ATTRIBUTE_ELEGANCE: fAttributeBonusRate -= sDefenceAttribute.fEleganceDefence; break;
//				case BATTLE_ATTRIBUTE_FUNNY: fAttributeBonusRate -= sDefenceAttribute.fFunnyDefence; break;
//
//				default: break;
//			}
//		}
//		break;
//	}
//
//	//printf("fAttributeBonusRate %f, byAttrOffence %f, byDefence %f \n", fAttributeBonusRate, byAttrOffence, byDefence);
//	return fAttributeBonusRate / 100.f;
//}
