#include "stdafx.h"
#include "Skill.h"
#include "NtlResultCode.h"
#include "TableContainerManager.h"
#include "SystemEffectTable.h"
#include "FormulaTable.h"

#include "NtlPacketGU.h"
#include "ObjectManager.h"
#include "CPlayer.h"
#include "battle.h"
#include "Monster.h"
#include "SummonPet.h"
#include "NtlNavi.h"


void	PrintAllSkillFunctions(sSKILL_TBLDAT *pSkillDataRef)
{
	for (int i = 0; i < SKILL_FUNCTION_COUNT + 1; i++)
	{
		if (BIT_FLAG_TEST(pSkillDataRef->dwFunction_Bit_Flag, MAKE_BIT_FLAG(i)))
		{
			printf("skill has the function %i \n", i); 
		}
	}
}

bool	CanApplyExplosion(eSYSTEM_EFFECT_CODE effectCode1, eSYSTEM_EFFECT_CODE effectCode2, CCharacter* pTarget)
{
	if (pTarget)
	{
		if (effectCode1 == ACTIVE_EXPLOSION_ALL_DOT || effectCode2 == ACTIVE_EXPLOSION_ALL_DOT)
		{
			if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_BLEEDING) == false && pTarget->GetStateManager()->IsCharCondition(CHARCOND_POISON) == false
				&& pTarget->GetStateManager()->IsCharCondition(CHARCOND_STOMACHACHE) == false && pTarget->GetStateManager()->IsCharCondition(CHARCOND_BURN) == false)
				return false;
		}
		else if (effectCode1 == ACTIVE_EXPLOSION_BLEED || effectCode2 == ACTIVE_EXPLOSION_BLEED)
		{
			if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_BLEEDING) == false)
				return false;
		}
		else if (effectCode1 == ACTIVE_EXPLOSION_POISON || effectCode2 == ACTIVE_EXPLOSION_POISON)
		{
			if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_POISON) == false)
				return false;
		}
		else if (effectCode1 == ACTIVE_EXPLOSION_STOMACHACHE || effectCode2 == ACTIVE_EXPLOSION_STOMACHACHE)
		{
			if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_STOMACHACHE) == false)
				return false;
		}
		else if (effectCode1 == ACTIVE_EXPLOSION_BURN || effectCode2 == ACTIVE_EXPLOSION_BURN)
		{
			if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_BURN) == false)
				return false;
		}
	}

	return true;
}


CSkill::CSkill()
{
	Init();
}

CSkill::~CSkill()
{
	Destroy();
}


bool CSkill::Create(sSKILL_TBLDAT *pSkillDataRef, CCharacter *pOwnerRef, BYTE skillIndex)
{
	if (pSkillDataRef)
	{
		m_pSkillDataRef = pSkillDataRef;
		m_pOwnerRef = pOwnerRef;
		m_skillIndex = skillIndex;

		bool bCreated = false;

		for (int j = 0; j < NTL_MAX_EFFECT_IN_SKILL; j++)
		{
			sSYSTEM_EFFECT_TBLDAT* pEffectTbldat = (sSYSTEM_EFFECT_TBLDAT*)g_pTableContainer->GetSystemEffectTable()->FindData(pSkillDataRef->skill_Effect[j]);
			if (pEffectTbldat)
			{
				m_aeEffectCode[j] = pEffectTbldat->effectCode;
				bCreated = true;
			}
			else if (!bCreated)
			{
				ERR_LOG(LOG_GENERAL, "Could not find system effect tbldat (%u) !!!!", pSkillDataRef->skill_Effect[j]);
				Destroy();
				return false;
			}
		}

		return true;
	}

	return false;
}


void CSkill::Destroy()
{
	m_pSkillDataRef = NULL;
	m_pOwnerRef = NULL;
	m_dwAffectingTimeRemaining = 0;
	m_dwCoolTimeRemaining = 0;

	for (int j = 0; j < NTL_MAX_EFFECT_IN_SKILL; j++)
		m_aeEffectCode[j] = INVALID_SYSTEM_EFFECT_CODE;
}


void CSkill::TickProcess(DWORD dwTickDiff)
{
	DecreaseCoolTimeRemaining(dwTickDiff);
	DecreaseAffectingTimeRemaining(dwTickDiff);
}


bool CSkill::UseSkill(BYTE byRpBonusType, HOBJECT hAppointTargetHandle, CNtlVector& rvFinalSubjectLoc, CNtlVector& rvFinalLoc, BYTE byApplyTargetCount, HOBJECT* ahApplyTarget, WORD & rwResultCode)
{
	rwResultCode = GAME_SUCCESS;

	if (byApplyTargetCount > NTL_MAX_NUMBER_OF_SKILL_TARGET)
	{
		rwResultCode = GAME_FAIL;
		return false;
	}

	CCharacter* pTarget = g_pObjectManager->GetChar(hAppointTargetHandle); //get appointed target
	if (pTarget && pTarget->IsInitialized())
	{
		if (GetCoolTimeRemaining() == 0)
		{
			bool bIsHarmful = Dbo_IsHarmfulEffectType(m_pSkillDataRef->bySkill_Active_Type) && m_pSkillDataRef->byApply_Target != DBO_SKILL_APPLY_TARGET_PARTY;
			bool bValidState = false;
			
			if (bIsHarmful && m_pOwnerRef->IsPC() && pTarget->IsNPC()) //do not allow players using skills on npc
			{
				rwResultCode = GAME_SKILL_INVALID_TARGET_APPOINTED;
			}
			else if (bIsHarmful && m_pSkillDataRef->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_SELF && m_pOwnerRef->GetID() != pTarget->GetID())
			{
				rwResultCode = GAME_SKILL_INVALID_TARGET_APPOINTED;
			}
			else if (bIsHarmful && m_pSkillDataRef->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_TARGET && m_pOwnerRef->GetID() == pTarget->GetID())
			{
				rwResultCode = GAME_SKILL_INVALID_TARGET_APPOINTED;
			}
			else if (bIsHarmful && pTarget->IsValidTarget(0) == false)
			{
				rwResultCode = GAME_SKILL_INVALID_TARGET_APPOINTED;
			}
			else if(pTarget->IsKnockedDown() && (Dbo_IsSystemEffectForBodyCurse(GetEffectCode(0)) || Dbo_IsSystemEffectForBodyCurse(GetEffectCode(1))))
				rwResultCode = GAME_TARGET_IS_WRONG_STATE;

			else if(CanApplyExplosion(GetEffectCode(0), GetEffectCode(1), pTarget) == false)
				rwResultCode = GAME_SKILL_CANT_CAST_NOW;

			else if(m_pSkillDataRef->bDash_Able == false && m_pOwnerRef->IsInRange(pTarget, m_pSkillDataRef->fUse_Range_Max + DBO_DISTANCE_CHECK_TOLERANCE) == false) //15.0f tolerance | DO NOT USE IsInRange3 because of mobs which can spawn by script dont have height
				rwResultCode = GAME_TARGET_TOO_FAR;
			else if (m_pSkillDataRef->bDash_Able == true && m_pOwnerRef->IsInRange(pTarget, m_pSkillDataRef->fUse_Range_Max + NTL_MAX_DASH_DISTANCE_FOR_SKILL + DBO_DISTANCE_CHECK_TOLERANCE) == false) //15.0f tolerance | DO NOT USE IsInRange3 because of mobs which can spawn by script dont have height
				rwResultCode = GAME_TARGET_TOO_FAR;
			else if (m_pSkillDataRef->bDash_Able == true && m_pOwnerRef->GetRunSpeed() <= 0.f && m_pOwnerRef->GetDistance(pTarget->GetCurLoc()) > m_pSkillDataRef->fUse_Range_Max + DBO_DISTANCE_CHECK_TOLERANCE)
			{
				rwResultCode = GAME_SKILL_CANT_CAST_NOW;
			}
			else
				bValidState = Dbo_IsValidStateToUseSkill(m_pSkillDataRef->dwTransform_Use_Info_Bit_Flag, m_pOwnerRef->GetCharStateID(), m_pOwnerRef->GetCharStun().eStunType, m_pOwnerRef->GetAspectStateId(), m_pOwnerRef->GetConditionState(), m_pOwnerRef->GetMoveFlag() == NTL_MOVE_FLAG_SWIM, m_pOwnerRef->GetMoveFlag() == NTL_MOVE_FLAG_JUMP, m_pOwnerRef->GetAirState(), &rwResultCode);

			if (bValidState && rwResultCode == GAME_SUCCESS)
			{
			//	if(m_pOwnerRef->IsPC())
			//		NTL_PRINT(PRINT_APP, "charid = %u, m_pOwnerRef->GetCharStateID() %u, byApplyTargetCount %u", ((CPlayer*)m_pOwnerRef)->GetCharID(), m_pOwnerRef->GetCharStateID(), byApplyTargetCount);
				
				m_byUseRpBonusType = byRpBonusType;
				m_hUseAppointedTarget = hAppointTargetHandle;
				m_byApplyTargetCount = byApplyTargetCount;
				m_vFinalSubjectLoc = rvFinalSubjectLoc;
				m_vFinalLoc = rvFinalLoc;

				for (BYTE i = 0; i < byApplyTargetCount; i++)
				{
					m_ahApplyTarget[i] = ahApplyTarget[i];
				}


				m_pOwnerRef->GetSkillManager()->SetCurSkillTblidx(GetSkillId());

				return true;
			}
		}
		else
		{
			rwResultCode = GAME_SKILL_NOT_READY_TO_BE_CAST;
		}
	}
	else
	{
		rwResultCode = GAME_SKILL_NO_TARGET_APPOINTED;
	}

	return false;
}


bool CSkill::CheckCasting()
{
	if (GetOriginalTableData()->dwCastingTimeInMilliSecs > 0)
	{
		float fCastingTime = (float)GetOriginalTableData()->dwCastingTimeInMilliSecs;

		if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_CASTING_TIME_MINUS)
			fCastingTime -= GetOriginalTableData()->afRpEffectValue[DBO_RP_BONUS_SLOT_CASTING_TIME_MINUS] * 1000;

		if (m_pOwnerRef->GetCharAtt()->GetCastingTimeChangePercent() != 0.0f)
			fCastingTime += fCastingTime * m_pOwnerRef->GetCharAtt()->GetCastingTimeChangePercent() / 100.0f;

		return m_pOwnerRef->SendCharStateCasting((DWORD)fCastingTime, (DWORD)fCastingTime, GetSkillId(), m_hUseAppointedTarget);
	}
	else //if no casting then use immidiently
	{
		CastSkill(m_hUseAppointedTarget, m_byApplyTargetCount, m_ahApplyTarget, false);
	}

	return true;
}


void CSkill::CastSkill(HOBJECT hAppointTargetHandle, BYTE byApplyTargetCount, HOBJECT * ahApplyTarget, bool bSetData/* = true*/)
{
	if (bSetData)
	{
		m_hUseAppointedTarget = hAppointTargetHandle;
		m_byApplyTargetCount = byApplyTargetCount;

		for (BYTE i = 0; i < byApplyTargetCount; i++)
			m_ahApplyTarget[i] = ahApplyTarget[i];
	}
	
	/* INFO:
		- Resist: Deal damage but the curse effect will not be applied;
		- Immune: Damage = 0, and no cc applied; (treated like dodge)
		- Dodge: no dmg nothing
	*/

	// storm strike
	if (m_pSkillDataRef->tblidx >= 710011 && m_pSkillDataRef->tblidx <= 710015)
	{
		sDBO_BUFF_PARAMETER aBuffParameter[NTL_MAX_EFFECT_IN_SKILL];
		sSKILL_TBLDAT* pTempSkillTbldat = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(2385);

		eSYSTEM_EFFECT_CODE aeEffectCode[NTL_MAX_EFFECT_IN_SKILL];
		aeEffectCode[0] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pTempSkillTbldat->skill_Effect[0]);
		aeEffectCode[1] = INVALID_SYSTEM_EFFECT_CODE;

		aBuffParameter[0].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DEFAULT;
		aBuffParameter[0].buffParameter.fParameter = 0;
		aBuffParameter[0].buffParameter.dwRemainValue = 0;

		DWORD dwTemp = 2000;
		if (m_pSkillDataRef->tblidx == 710012)
			dwTemp = 2100;
		else if (m_pSkillDataRef->tblidx == 710013)
			dwTemp = 2200;
		else if (m_pSkillDataRef->tblidx == 710014)
			dwTemp = 2400;
		else if (m_pSkillDataRef->tblidx == 710015)
			dwTemp = 2500;

		m_pOwnerRef->GetBuffManager()->RegisterBuff(dwTemp, aeEffectCode, aBuffParameter, INVALID_HOBJECT, BUFF_TYPE_BLESS, pTempSkillTbldat);
	}

		CNtlPacket packet(sizeof(sGU_CHAR_ACTION_SKILL));
		sGU_CHAR_ACTION_SKILL * res = (sGU_CHAR_ACTION_SKILL *)packet.GetPacketData();
		res->wOpCode = GU_CHAR_ACTION_SKILL;
		res->wResultCode = GAME_SUCCESS;
		res->handle = m_pOwnerRef->GetID();
		res->dwLpEpEventId = g_pObjectManager->AcquireLpEpEventID();
		res->skillId = GetSkillId();
		res->byRpBonusType = m_byUseRpBonusType;
		res->hAppointedTarget = m_hUseAppointedTarget;
		res->bIsSkillHarmful = Dbo_IsHarmfulEffectType(m_pSkillDataRef->bySkill_Active_Type) && m_pSkillDataRef->byApply_Target != DBO_SKILL_APPLY_TARGET_PARTY;
		
		memset(res->aSkillResult, NULL, sizeof(res->aSkillResult));
		memset(&m_actionSkill, NULL, sizeof(m_actionSkill));

		m_actionSkill.bCanTargetRecoverOnHit = true;
		//NTL_PRINT(PRINT_APP,"CastSkill: m_byApplyTargetCount %u, m_hUseAppointedTarget %u, byApplyTargetCount %u, res->bIsSkillHarmful %i, bySkill_Active_Type %u",
		//	m_byApplyTargetCount, m_hUseAppointedTarget, byApplyTargetCount, res->bIsSkillHarmful, m_pSkillDataRef->bySkill_Active_Type);
		for (int i = 0; i < m_byApplyTargetCount; i++)
		{
			CCharacter* pTarget = g_pObjectManager->GetChar(m_ahApplyTarget[i]);
			if (pTarget)
			{
				if (res->bIsSkillHarmful && (m_pOwnerRef->IsAttackable(pTarget) == false))
				{
				//	NTL_PRINT(PRINT_APP,"CONTINUE LOOP 269");
					goto LOOP_CONTINUE;
				}

				bool bGuardCrashFail = false;
				bool bCanCounterAttack = true;
				int nAgro = GetOriginalTableData()->dwAdditional_Aggro_Point + m_pOwnerRef->GetTbldat()->wBasic_Aggro_Point;

				/*DEBUG: Print all skill functions*/
				//PrintAllSkillFunctions(m_pSkillDataRef);

				pTarget->SetLpEpEventID(res->dwLpEpEventId);

				sDBO_BUFF_PARAMETER aBuffParameter[NTL_MAX_EFFECT_IN_SKILL];

				DWORD dwKeepTime = (DWORD)(m_pSkillDataRef->dwKeepTimeInMilliSecs + (float)m_pSkillDataRef->dwKeepTimeInMilliSecs * m_pOwnerRef->GetCharAtt()->GetKeepTimeChangePercent() / 100); //re-set keep time for every target otherwise when hit multiple targets the keep time will be bigger at each target		

				CNtlVector vShift(pTarget->GetCurLoc() - m_pOwnerRef->GetCurLoc());
				vShift.y = 0.0f;
				vShift.SafeNormalize();
				vShift.CopyTo(res->aSkillResult[i].vShift);

				pTarget->GetCurLoc().CopyTo(res->aSkillResult[i].vFinalSubjectLoc);
				res->aSkillResult[i].damageByReflectingCurse = 0;
				res->aSkillResult[i].byBlockedAction = DBO_GUARD_TYPE_INVALID;
				res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_HIT;

				if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_INVINCIBLE))
				{
					res->aSkillResult[i].byBlockedAction = DBO_GUARD_TYPE_DAMAGE_SKILL;
					res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_BLOCK;

					goto RESULT2;
				}

				//NTL_PRINT(PRINT_APP, "USE-SKILL: CurLoc x %f y %f z %f, fSuccess_Rate %f", m_pOwnerRef->GetCurLoc().x, m_pOwnerRef->GetCurLoc().y, m_pOwnerRef->GetCurLoc().z, m_pSkillDataRef->fSuccess_Rate);
				if (res->bIsSkillHarmful)
				{
					/* - CHECK DODGE - */
					if (m_pSkillDataRef->bySkill_Active_Type == SKILL_ACTIVE_TYPE_DD || m_pSkillDataRef->bySkill_Active_Type == SKILL_ACTIVE_TYPE_DOT)
					{
						if (BattleIsDodge(pTarget->IsPC(), WORD((float)m_pOwnerRef->GetCharAtt()->GetAttackRate() * m_pSkillDataRef->fSuccess_Rate), pTarget->GetCharAtt()->GetDodgeRate(), m_pOwnerRef->GetLevel(), pTarget->GetLevel())) //check if dodge damage skill
						{
							res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_DODGE;
							goto RESULT2;
						}
					}

					if (pTarget->GetStateManager()->GetAspectStateID() == ASPECTSTATE_SPINNING_ATTACK || pTarget->GetStateManager()->GetAspectStateID() == ASPECTSTATE_ROLLING_ATTACK)
					{
						res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_DODGE;
						goto RESULT2;
					}

					/* - check IMMUNTE / RESIST - */
					if (m_pSkillDataRef->bySkill_Active_Type == SKILL_ACTIVE_TYPE_DC || m_pSkillDataRef->bySkill_Active_Type == SKILL_ACTIVE_TYPE_CB)
					{
						if (Dbo_IsSystemEffectForBodyCurse(GetEffectCode(0)) || Dbo_IsSystemEffectForBodyCurse(GetEffectCode(1))) //check if stun
						{
							if (pTarget->IsKnockedDown()) //if knocked down then immune
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								goto RESULT2;
							}
						}

						if (BattleIsResist(WORD((float)m_pOwnerRef->GetCharAtt()->GetCurseSuccessRate() * m_pSkillDataRef->fSuccess_Rate), pTarget->GetCharAtt()->GetCurseToleranceRate(), m_pOwnerRef->GetLevel(), pTarget->GetLevel())) //check if resist direct curse
						{
							res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_RESISTED;
							goto RESULT2;
						}

						//make foc decrease duration
						dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, DWORD((float)dwKeepTime * ((float)pTarget->GetCharAtt()->GetFoc() * 0.05f) / 100.f));
					}


					/* - CHECK KNOCKDOWN - */
					bool bCantargetGetKnockedDown = pTarget->GetStateManager()->CanCharStateTransition(CHARSTATE_KNOCKDOWN);

					if (bCantargetGetKnockedDown && m_byUseRpBonusType == DBO_RP_BONUS_TYPE_KNOCKDOWN && pTarget->IsMonster())
					{
						CMonster* pTmpMob = (CMonster*)pTarget;
						if (pTmpMob->GetMobRank() >= MOB_GRADE_ULTRA)
						{
							res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
							bCantargetGetKnockedDown = false;
						}
					}

					if (bCantargetGetKnockedDown && (BIT_FLAG_TEST(m_pSkillDataRef->dwFunction_Bit_Flag, MAKE_BIT_FLAG(SKILL_FUNCTION_FORCED_KNOCKDOWN)) || m_byUseRpBonusType == DBO_RP_BONUS_TYPE_KNOCKDOWN))
					{
						if (pTarget->GetCharStateID() == CHARSTATE_GUARD)
						{
							if ((((CPlayer*)pTarget)->IsGuardSuccess(DBO_GUARD_TYPE_KNOCKDOWN, bCanCounterAttack)))
							{
								res->aSkillResult[i].byBlockedAction = DBO_GUARD_TYPE_KNOCKDOWN;
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_BLOCK;

								goto RESULT2;
							}
						}

						if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_NULLIFIED_DAMAGE) == false)
							res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_KNOCKDOWN;
					}
					else if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_GUARD_CRASH) //check if using guard crash rp effect
					{
						if (pTarget->GetCharStateID() != CHARSTATE_GUARD) //check if target is NOT in guard
						{
							bGuardCrashFail = true;
							goto RESULT3; // deal 0 damage
						}
					}
					else
					{
						if (pTarget->GetCharStateID() == CHARSTATE_GUARD) //check if target is in guard
						{
							if (Dbo_IsCurseType(m_pSkillDataRef->bySkill_Active_Type))
							{
								if (((CPlayer*)pTarget)->IsGuardSuccess(DBO_GUARD_TYPE_CURSE_SKILL, bCanCounterAttack))
								{
									res->aSkillResult[i].byBlockedAction = DBO_GUARD_TYPE_CURSE_SKILL;
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_BLOCK;
									goto RESULT2;
								}
							}
							else if (((CPlayer*)pTarget)->IsGuardSuccess(DBO_GUARD_TYPE_DAMAGE_SKILL, bCanCounterAttack))
							{
								res->aSkillResult[i].byBlockedAction = DBO_GUARD_TYPE_DAMAGE_SKILL;
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_BLOCK;
								goto RESULT2;
							}
						}
					}

					/* - CHECK CRIT - */
					if (res->aSkillResult[i].byAttackResult != BATTLE_ATTACK_RESULT_KNOCKDOWN)
					{
						float fCritRate = 0.0f;
						if (m_pSkillDataRef->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL) //check if physical dmg
						{
							if(BattleIsCrit(m_pOwnerRef->GetCharAtt(), pTarget->GetCharAtt(), true))
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_CRITICAL_HIT;
						}
						else if (m_pSkillDataRef->bySkill_Type == NTL_SKILL_TYPE_ENERGY) //check if energy dmg
						{
							if (BattleIsCrit(m_pOwnerRef->GetCharAtt(), pTarget->GetCharAtt(), false))
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_CRITICAL_HIT;
						}
					}
				}

				for (int e = 0; e < NTL_MAX_EFFECT_IN_SKILL; e++)
				{
					res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_UNKNOWN;
					res->aSkillResult[i].effectResult[e].DD_DOT_lpEpRecovered.Reset();

					eSYSTEM_EFFECT_CODE effectCode = GetEffectCode(e);
					if (effectCode != INVALID_SYSTEM_EFFECT_CODE)
					{
					//	NTL_PRINT(PRINT_APP,"Effect nr %u | effect %s", e, NtlGetSystemEffectString(effectCode));
						switch (effectCode)
						{
						case ACTIVE_DIRECT_DAMAGE:
						case ACTIVE_WARP_BACK_ATTACK:
						case ACTIVE_THROUGH_ATTACK:
						{
							m_actionSkill.bCanWakeUpTarget = true;

							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;

							float fAttBonus = m_pOwnerRef->GetCharAtt()->GetDotTimeChangeAbsolute();

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
								fAttBonus += m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

							//calculate the damage
							CalcSkillDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, (float)m_pSkillDataRef->aSkill_Effect_Value[e] + fAttBonus, res->aSkillResult[i].effectResult[e].DD_DOT_fDamage, res->aSkillResult[i].byAttackResult, res->aSkillResult[i].damageByReflectingCurse, &res->aSkillResult[i].effectResult[e].DD_DOT_lpEpRecovered);
						
							if (effectCode == ACTIVE_WARP_BACK_ATTACK || effectCode == ACTIVE_THROUGH_ATTACK)
							{
								if(m_pOwnerRef->GetStateManager()->IsCharCondition(CHARCOND_NULLIFIED_DAMAGE) == false) //this check is REQUIRED! If we hit multiple targets then we set the state multiple times and in the end it will not be removed!
									m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_NULLIFIED_DAMAGE, NULL, false);
							}

							//aggro
							nAgro += (int)res->aSkillResult[i].effectResult[e].DD_DOT_fDamage;
						}
						break;
						case ACTIVE_BACKSTAB:
						{
							m_actionSkill.bCanWakeUpTarget = true;

							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;

							float fSkillDmg = (float)m_pSkillDataRef->aSkill_Effect_Value[e];
								
							if (pTarget->IsFrontSide(m_pOwnerRef) == false)
							{
								fSkillDmg *= NTL_BATTLE_HIT_BACK_SIDE_BONUS_RATE;
							}

							float fBonusBaseDamage = m_pOwnerRef->GetCharAtt()->GetDotTimeChangeAbsolute();

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
								fBonusBaseDamage += m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];


							//calculate the damage
							CalcSkillDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, fSkillDmg + fBonusBaseDamage, res->aSkillResult[i].effectResult[e].DD_DOT_fDamage, res->aSkillResult[i].byAttackResult, res->aSkillResult[i].damageByReflectingCurse, &res->aSkillResult[i].effectResult[e].DD_DOT_lpEpRecovered, false, pTarget->IsFrontSide(m_pOwnerRef) == false);

							//aggro
							nAgro += (int)(res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * CFormulaTable::m_afRate[8002][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
						}
						break;

						case ACTIVE_CRITICAL_ATTACK: //DD effect
						{
							m_actionSkill.bCanWakeUpTarget = true;

							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;

							float fSkillDmg = (float)m_pSkillDataRef->aSkill_Effect_Value[e];

							if (res->aSkillResult[i].byAttackResult == BATTLE_ATTACK_RESULT_CRITICAL_HIT)
							{
								fSkillDmg *= NTL_BATTLE_CRITICAL_DAMAGE_INCREASE_RATE;
							}

							float fBonusBaseDamage = m_pOwnerRef->GetCharAtt()->GetDotTimeChangeAbsolute();
							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
								fBonusBaseDamage += m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

							//calculate the damage
							CalcSkillDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, fSkillDmg + fBonusBaseDamage, res->aSkillResult[i].effectResult[e].DD_DOT_fDamage, res->aSkillResult[i].byAttackResult, res->aSkillResult[i].damageByReflectingCurse, &res->aSkillResult[i].effectResult[e].DD_DOT_lpEpRecovered, true);

							//aggro
							nAgro += (int)(res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * CFormulaTable::m_afRate[8002][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
						}
						break;

						case ACTIVE_EXPLOSION_BLEED:
						{
							if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_BLEEDING))
							{
								m_actionSkill.bCanWakeUpTarget = true;

								res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;
								res->aSkillResult[i].effectResult[e].DD_DOT_fDamage = (float)m_pSkillDataRef->aSkill_Effect_Value[e];

								if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS)
									res->aSkillResult[i].effectResult[e].DD_DOT_fDamage += m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

								//aggro
								nAgro += (int)(res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * CFormulaTable::m_afRate[8002][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
							}
							else
							{
								goto LOOP_CONTINUE;
							}
						}
						break;
						case ACTIVE_EXPLOSION_POISON:
						{
							if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_POISON))
							{
								m_actionSkill.bCanWakeUpTarget = true;

								res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;
								res->aSkillResult[i].effectResult[e].DD_DOT_fDamage = (float)m_pSkillDataRef->aSkill_Effect_Value[e];

								if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS)
									res->aSkillResult[i].effectResult[e].DD_DOT_fDamage += m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

								//aggro
								nAgro += (int)(res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * CFormulaTable::m_afRate[8002][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
							}
							else
							{
								goto LOOP_CONTINUE;
							}
						}
						break;
						case ACTIVE_EXPLOSION_STOMACHACHE:
						{
							if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_STOMACHACHE))
							{
								m_actionSkill.bCanWakeUpTarget = true;

								res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;
								res->aSkillResult[i].effectResult[e].DD_DOT_fDamage = (float)m_pSkillDataRef->aSkill_Effect_Value[e];

								if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS)
									res->aSkillResult[i].effectResult[e].DD_DOT_fDamage += m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

								//aggro
								nAgro += (int)(res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * CFormulaTable::m_afRate[8002][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
							}
							else
							{
								goto LOOP_CONTINUE;
							}
						}
						break;
						case ACTIVE_EXPLOSION_BURN:
						{
							if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_BURN))
							{
								m_actionSkill.bCanWakeUpTarget = true;

								res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;
								res->aSkillResult[i].effectResult[e].DD_DOT_fDamage = (float)m_pSkillDataRef->aSkill_Effect_Value[e];

								if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS)
									res->aSkillResult[i].effectResult[e].DD_DOT_fDamage += m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

								//aggro
								nAgro += (int)(res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * CFormulaTable::m_afRate[8002][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
							}
							else
							{
								goto LOOP_CONTINUE;
							}
						}
						break;
						case ACTIVE_EXPLOSION_ALL_DOT:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;

							if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_BLEEDING) || pTarget->GetStateManager()->IsCharCondition(CHARCOND_POISON)
								|| pTarget->GetStateManager()->IsCharCondition(CHARCOND_STOMACHACHE) || pTarget->GetStateManager()->IsCharCondition(CHARCOND_BURN))
							{
								m_actionSkill.bCanWakeUpTarget = true;

								float fDOTDMG = 0.0f;
								int nBuffCount = 0;

								float fAttBonus = 0.0f;

								if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS)
									fAttBonus = m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

								for (int a = ACTIVE_BLEED; a <= ACTIVE_BURN; )
								{
									CBuff* pBuff = pTarget->GetBuffManager()->FindCurse((eSYSTEM_EFFECT_CODE)a, (DWORD)(CFormulaTable::m_afRate[8011][1] * 1000));
									if (pBuff)
									{
										if (pBuff->GetBuffInfo()->aBuffParameter[0].buffParameter.DOT_fDamage > 0.f)
											fDOTDMG += pBuff->GetBuffInfo()->aBuffParameter[0].buffParameter.DOT_fDamage * ((float)(pBuff->GetRemainTime(0) / 1000.f / 2.f) / 2.f);
										else
											fDOTDMG += pBuff->GetBuffInfo()->aBuffParameter[1].buffParameter.DOT_fDamage * ((float)(pBuff->GetRemainTime(1) / 1000.f / 2.f) / 2.f);

										//remove the buff
										pTarget->GetBuffManager()->RemoveBuff(pBuff->GetBuffIndex(), pBuff->GetBuffType(), CBuff::BUFF_REMOVAL_REASON_ETC);

										if (++nBuffCount >= 5)
											break;
									}
									else ++a;
								}

								fDOTDMG = (fDOTDMG * (float)m_pSkillDataRef->aSkill_Effect_Value[e] / 100.f) + fAttBonus;

								//crit
								//check if crit success and add damage
								if (res->aSkillResult[i].byAttackResult == BATTLE_ATTACK_RESULT_CRITICAL_HIT)
								{
									float fCritDmgRate = 0.0f;

									if (m_pSkillDataRef->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL) //check if physical dmg
									{
										fCritDmgRate = m_pOwnerRef->GetCharAtt()->GetPhysicalCriticalDamageRate();
									}
									else if (m_pSkillDataRef->bySkill_Type == NTL_SKILL_TYPE_ENERGY) //check if energy dmg
									{
										fCritDmgRate = m_pOwnerRef->GetCharAtt()->GetEnergyCriticalDamageRate();
									}
									else if (m_pSkillDataRef->bySkill_Type == NTL_SKILL_TYPE_STATE)
									{
										fCritDmgRate = (m_pOwnerRef->GetCharAtt()->GetPhysicalCriticalDamageRate() + m_pOwnerRef->GetCharAtt()->GetEnergyCriticalDamageRate()) / 2.f;
									}

									fDOTDMG += fDOTDMG * fCritDmgRate /*+ DBO_BATTLE_OFFENCE_BONUS_RATE_BY_CRITICAL*/ / 100;
								}

								res->aSkillResult[i].effectResult[e].DD_DOT_fDamage = fDOTDMG;
								//printf("\n%f fDOTDMG %f \n", (float)m_pSkillDataRef->aSkill_Effect_Value[e], fDOTDMG);

								//aggro
								nAgro += (int)(res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * CFormulaTable::m_afRate[8002][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
							}
							else
							{
								goto LOOP_CONTINUE;
							}
						}
						break;

						case ACTIVE_BLEED://DOT buff effect // Skills with DOT effect never have 2x DOT effects in a skill. Its mostly 1x DOT and 1x Deal DMG.
						{
							if (pTarget->IsMonster())
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_BLEED)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}

							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;
							aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DOT;

							/* INFO:
							- First DOT is added to the dealt damage. Watch https://youtu.be/JlBDzAmoNTk?t=317
							*/

							float fAttBonus = 0.0f;

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS)
								fAttBonus = m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

							dwKeepTime += (DWORD)m_pOwnerRef->GetCharAtt()->GetDotTimeChangeAbsolute() * 1000; // update seconds to milliseconds

							dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, (DWORD)((float)dwKeepTime * pTarget->GetCharAtt()->GetBleedingKeepTimeDown() / 100.0f));

							//calculate the damage
							CalcSkillDotDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, pTarget->GetCharAtt()->GetBleedDefence(), (float)m_pSkillDataRef->aSkill_Effect_Value[e],
								fAttBonus, aBuffParameter[e].buffParameter.DOT_fDamage, res->aSkillResult[i].byAttackResult, ACTIVE_BLEED);

							if (aBuffParameter[e].buffParameter.DOT_fDamage <= 0.f)
								aBuffParameter[e].buffParameter.DOT_fDamage = 1.0f;

							// crane passive
							aBuffParameter[e].buffParameter.DOT_fDamage += aBuffParameter[e].buffParameter.DOT_fDamage * m_pOwnerRef->GetCharAtt()->GetDotValueChangePercent() / 100.f;

							res->aSkillResult[i].effectResult[e].DD_DOT_fDamage = aBuffParameter[e].buffParameter.fParameter = aBuffParameter[e].buffParameter.DOT_fDamage;

							//aggro
							nAgro += (int)(res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * CFormulaTable::m_afRate[8002][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
						}
						break;
						case ACTIVE_POISON://DOT buff effect // Skills with DOT effect never have 2x DOT effects in a skill. Its mostly 1x DOT and 1x Deal DMG.
						{
							if (pTarget->IsMonster())
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_POISON)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}

							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;
							aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DOT;

							/* INFO:
							- First DOT is added to the dealt damage. Watch https://youtu.be/JlBDzAmoNTk?t=317
							*/

							float fAttBonus = 0.0f;

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS)
								fAttBonus = m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

							dwKeepTime += (DWORD)m_pOwnerRef->GetCharAtt()->GetDotTimeChangeAbsolute() * 1000; // update seconds to milliseconds

							dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, (DWORD)((float)dwKeepTime * pTarget->GetCharAtt()->GetPoisonKeepTimeDown() / 100.0f));

							//calculate the damage
							CalcSkillDotDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, pTarget->GetCharAtt()->GetPoisonDefence(), (float)m_pSkillDataRef->aSkill_Effect_Value[e],
								fAttBonus, aBuffParameter[e].buffParameter.DOT_fDamage, res->aSkillResult[i].byAttackResult, ACTIVE_POISON);

							if (aBuffParameter[e].buffParameter.DOT_fDamage <= 0.f)
								aBuffParameter[e].buffParameter.DOT_fDamage = 1.0f;

							// crane passive
							aBuffParameter[e].buffParameter.DOT_fDamage += aBuffParameter[e].buffParameter.DOT_fDamage * m_pOwnerRef->GetCharAtt()->GetDotValueChangePercent() / 100.f;

							res->aSkillResult[i].effectResult[e].DD_DOT_fDamage = aBuffParameter[e].buffParameter.fParameter = aBuffParameter[e].buffParameter.DOT_fDamage;

							//aggro
							nAgro += (int)(res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * CFormulaTable::m_afRate[8002][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
						}
						break;
						case ACTIVE_STOMACHACHE://DOT buff effect // Skills with DOT effect never have 2x DOT effects in a skill. Its mostly 1x DOT and 1x Deal DMG.
						{
							if (pTarget->IsMonster())
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_ABDOMINAL)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}

							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;
							aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DOT;

							/* INFO:
							- First DOT is added to the dealt damage. Watch https://youtu.be/JlBDzAmoNTk?t=317
							*/

							float fAttBonus = 0.0f;

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS)
								fAttBonus = m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

							dwKeepTime += (DWORD)m_pOwnerRef->GetCharAtt()->GetDotTimeChangeAbsolute() * 1000; // update seconds to milliseconds

							dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, (DWORD)((float)dwKeepTime * pTarget->GetCharAtt()->GetStomachacheKeepTimeDown() / 100.0f));

							//calculate the damage
							CalcSkillDotDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, pTarget->GetCharAtt()->GetStomachacheDefence(), (float)m_pSkillDataRef->aSkill_Effect_Value[e],
								fAttBonus, aBuffParameter[e].buffParameter.DOT_fDamage, res->aSkillResult[i].byAttackResult, ACTIVE_STOMACHACHE);

							if (aBuffParameter[e].buffParameter.DOT_fDamage <= 0.f)
								aBuffParameter[e].buffParameter.DOT_fDamage = 1.0f;

							// crane passive
							aBuffParameter[e].buffParameter.DOT_fDamage += aBuffParameter[e].buffParameter.DOT_fDamage * m_pOwnerRef->GetCharAtt()->GetDotValueChangePercent() / 100.f;

							res->aSkillResult[i].effectResult[e].DD_DOT_fDamage = aBuffParameter[e].buffParameter.fParameter = aBuffParameter[e].buffParameter.DOT_fDamage;

							//aggro
							nAgro += (int)(res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * CFormulaTable::m_afRate[8002][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
						}
						break;
						case ACTIVE_BURN://DOT buff effect // Skills with DOT effect never have 2x DOT effects in a skill. Its mostly 1x DOT and 1x Deal DMG.
						{
							if (pTarget->IsMonster())
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_BURN)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}

							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;
							aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DOT;

							/* INFO:
								- First DOT is added to the dealt damage. Watch https://youtu.be/JlBDzAmoNTk?t=317
							*/

							float fAttBonus = 0.0f;

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS)
								fAttBonus = m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

							dwKeepTime += (DWORD)m_pOwnerRef->GetCharAtt()->GetDotTimeChangeAbsolute() * 1000; // update seconds to milliseconds

							//calculate the damage
							CalcSkillDotDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, pTarget->GetCharAtt()->GetBurnDefence(), (float)m_pSkillDataRef->aSkill_Effect_Value[e],
								fAttBonus, aBuffParameter[e].buffParameter.DOT_fDamage, res->aSkillResult[i].byAttackResult, ACTIVE_BURN);

							if (aBuffParameter[e].buffParameter.DOT_fDamage <= 0.f)
								aBuffParameter[e].buffParameter.DOT_fDamage = 1.0f;

							// crane passive
							aBuffParameter[e].buffParameter.DOT_fDamage += aBuffParameter[e].buffParameter.DOT_fDamage * m_pOwnerRef->GetCharAtt()->GetDotValueChangePercent() / 100.f;

							res->aSkillResult[i].effectResult[e].DD_DOT_fDamage = aBuffParameter[e].buffParameter.fParameter = aBuffParameter[e].buffParameter.DOT_fDamage;

							//aggro
							nAgro += (int)(res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * CFormulaTable::m_afRate[8002][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
						}
						break;

						case ACTIVE_DIRECT_HEAL:
						case ACTIVE_RESCUE: //When use revive skill then it will heal target too!
						{
							if (effectCode == ACTIVE_RESCUE)
							{
								if (pTarget->IsFainting() && pTarget->IsPC()) //Only players can revive
								{
									if (pTarget->GetCurWorld() && pTarget->GetCurWorld()->GetRuleType() != GAMERULE_MINORMATCH) //dont allow revival in prelim
									{
										res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DH_HOT;

										pTarget->Revival(pTarget->GetCurLoc(), pTarget->GetWorldID(), REVIVAL_TYPE_RESCUED, TELEPORT_TYPE_SKILL);
									}
									else goto LOOP_CONTINUE;
								}
								else goto LOOP_CONTINUE;
							}
							else
							{
								if (pTarget->IsFainting())
									goto LOOP_CONTINUE;

								res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DH_HOT;
							}

							CalcDirectHeal(m_pOwnerRef, m_pSkillDataRef, e, res->aSkillResult[i].effectResult[e].fResultValue);

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS)
								res->aSkillResult[i].effectResult[e].fResultValue += m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

							pTarget->UpdateCurLP((int)res->aSkillResult[i].effectResult[e].fResultValue, true, false);

							if (m_pOwnerRef->GetID() != pTarget->GetID())
							{
								//aggro
								nAgro += (int)(res->aSkillResult[i].effectResult[e].fResultValue * CFormulaTable::m_afRate[8003][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);

								IncreaseTargetEnemyAggro(m_pOwnerRef, pTarget, nAgro);
							}
						}
						break;

						case ACTIVE_DIRECT_EP_HEAL:
						{
							if (pTarget->IsFainting())
								goto LOOP_CONTINUE;

							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DH_HOT;

							CalcDirectHeal(m_pOwnerRef, m_pSkillDataRef, e, res->aSkillResult[i].effectResult[e].fResultValue);

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS)
								res->aSkillResult[i].effectResult[e].fResultValue += m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

							pTarget->UpdateCurEP((WORD)res->aSkillResult[i].effectResult[e].fResultValue, true, false);

							if (m_pOwnerRef->GetID() != pTarget->GetID())
							{
								//aggro
								nAgro += (int)(res->aSkillResult[i].effectResult[e].fResultValue * CFormulaTable::m_afRate[8004][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);

								IncreaseTargetEnemyAggro(m_pOwnerRef, pTarget, nAgro);
							}
						}
						break;

						case ACTIVE_HEAL_OVER_TIME:
						case ACTIVE_EP_OVER_TIME:
						{
							if (pTarget->IsFainting())
								goto LOOP_CONTINUE;

							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DH_HOT;

							aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_HOT;

							CalcHealOverTime(m_pOwnerRef, m_pSkillDataRef, e, aBuffParameter[e].buffParameter.fParameter);

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS)
								aBuffParameter[e].buffParameter.fParameter += m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

							if (effectCode == ACTIVE_HEAL_OVER_TIME)
							{
								//aggro
								nAgro += (int)(aBuffParameter[e].buffParameter.fParameter * CFormulaTable::m_afRate[8006][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);

								pTarget->UpdateCurLP((int)aBuffParameter[e].buffParameter.fParameter, true, false);
							}
							else
							{
								//aggro
								nAgro += (int)(aBuffParameter[e].buffParameter.fParameter * CFormulaTable::m_afRate[8007][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);

								pTarget->UpdateCurEP((WORD)aBuffParameter[e].buffParameter.fParameter, true, false);
							}

							res->aSkillResult[i].effectResult[e].fResultValue = aBuffParameter[e].buffParameter.fParameter;

							// increase aggro from targets around
							IncreaseTargetEnemyAggro(m_pOwnerRef, pTarget, nAgro);
						}
						break;

						case ACTIVE_TELEPORT_BIND:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;

							if (pTarget->IsPC())
							{
								if (m_pOwnerRef->GetCurWorld() && m_pOwnerRef->GetCurWorld()->GetRuleType() == GAMERULE_NORMAL)
								{
									CPlayer* pTargetPlayer = (CPlayer*)pTarget;

									if(pTargetPlayer->GetDragonballScrambleBallFlag() == 0)
										pTarget->StartTeleport(CNtlVector(pTargetPlayer->GetBindLoc()), pTarget->GetCurDir(), pTargetPlayer->GetBindWorldID(), TELEPORT_TYPE_SKILL);
									else
										res->wResultCode = GAME_CAN_NOT_TELEPORT;
								}
								else res->wResultCode = GAME_SKILL_CANT_CAST_NOW;
							}
							else res->wResultCode = GAME_SKILL_INVALID_TARGET_APPOINTED;
						}
						break;

						case ACTIVE_VISIT:
						{
							if (m_pOwnerRef->GetCurWorld() && m_pOwnerRef->GetCurWorld()->GetRuleType() == GAMERULE_NORMAL)
							{
								CPlayer* pTargetPC = g_pObjectManager->GetPC(m_pOwnerRef->GetTargetHandle());

								if (pTargetPC && pTargetPC->IsInitialized() && pTargetPC->IsPC() && m_pOwnerRef->IsPC())
								{
									CPlayer* pCasterPc = (CPlayer*)m_pOwnerRef;

									if (pTargetPC->GetID() != m_pOwnerRef->GetID())
									{
										if (pTargetPC->GetCurWorld())
										{
											if (pTargetPC->GetCurWorld()->GetRuleType() == GAMERULE_NORMAL)
											{
												if(pCasterPc->GetDragonballScrambleBallFlag() > 0) //check if caster has a ball
													res->wResultCode = GAME_CAN_NOT_TELEPORT;
												else if(pTargetPC->GetDragonballScrambleBallFlag() > 0 && pCasterPc->GetPartyID() != INVALID_PARTYID && pCasterPc->GetPartyID() != pTargetPC->GetPartyID()) //check if target has a ball and caster is not in same party
													res->wResultCode = GAME_CAN_NOT_TELEPORT;
												else
													pCasterPc->StartTeleport(pTargetPC->GetCurLoc(), pTargetPC->GetCurDir(), pTargetPC->GetWorldID(), TELEPORT_TYPE_SKILL);
											}
											else res->wResultCode = GAME_WORLD_CANT_ENTER_WORLD;
										}
										else res->wResultCode = GAME_WORLD_CANT_FIND_WORLD_AREA;
									}
									else res->wResultCode = GAME_SKILL_CANT_BE_CAST_ON_YOURSELF;
								}
								else res->wResultCode = GAME_SKILL_CAN_BE_CAST_ONLY_ON_PLAYER;
							}
							else res->wResultCode = GAME_SKILL_CANT_CAST_NOW;
						}
						break;

						case ACTIVE_POISON_CURE:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;
							if (!pTarget->IsFainting() && pTarget->GetStateManager()->IsCharCondition(CHARCOND_POISON))
							{
								if (Dbo_CheckProbabilityF((float)m_pSkillDataRef->aSkill_Effect_Value[e]))
									pTarget->GetBuffManager()->EndBuffs(ACTIVE_POISON);
							}
						}break;
						case ACTIVE_STOMACHACHE_CURE:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;
							if (!pTarget->IsFainting() && pTarget->GetStateManager()->IsCharCondition(CHARCOND_STOMACHACHE))
							{
								if (Dbo_CheckProbabilityF((float)m_pSkillDataRef->aSkill_Effect_Value[e]))
									pTarget->GetBuffManager()->EndBuffs(ACTIVE_STOMACHACHE);
							}

						}break;
						case ACTIVE_PARALYZE_CURE:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;
							if (pTarget->GetCharStateID() == CHARSTATE_PARALYZED)
							{
								if (Dbo_CheckProbabilityF((float)m_pSkillDataRef->aSkill_Effect_Value[e]))
									pTarget->GetBuffManager()->EndBuffs(ACTIVE_PARALYZE);
							}
						}break;

						case ACTIVE_CURSE_DEBUFF://remove a curse?
						{
							if (Dbo_CheckProbabilityF((float)m_pSkillDataRef->aSkill_Effect_Value[e]))
							{
								pTarget->GetBuffManager()->RemoveRandomCurseBuff();
							}
						}
						break;

						case ACTIVE_SUPER_SAIYAN:
						case ACTIVE_PURE_MAJIN:
						case ACTIVE_KAIOKEN:
						case ACTIVE_GREAT_NAMEK:
						{
							if (!pTarget->IsPC() || pTarget->GetID() != GetOwner()->GetID())
								goto LOOP_CONTINUE;

							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;
							
							if (((CPlayer*)m_pOwnerRef)->CanTransform(effectCode, (TBLIDX)m_pSkillDataRef->aSkill_Effect_Value[e], true))
							{
								// IF WE TRANSFORM HERE THEN CLIENT OF OTHERS WILL CRASH IF TRANSFORM WHILE CANDY ETC
								if (effectCode != ACTIVE_KAIOKEN)
								{
									if (m_pOwnerRef->GetStateManager()->IsCharCondition(CHARCOND_NULLIFIED_DAMAGE) == false) //this check is REQUIRED! If we hit multiple targets then we set the state multiple times and in the end it will not be removed!
										m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_NULLIFIED_DAMAGE, NULL, false);

									m_pOwnerRef->GetBuffManager()->RemoveAllStun();
								}
								else
								{
									((CPlayer*)m_pOwnerRef)->Transform(GetEffectCode(e), (TBLIDX)m_pSkillDataRef->aSkill_Effect_Value[e], true, GetSkillIndex());
								}
							}
							else res->wResultCode = GAME_SKILL_ALREADY_TRANSFORMED;
						}
						break;

						case ACTIVE_SUMMON:
						{
							if (((CPlayer*)m_pOwnerRef)->GetCurrentPetId() == INVALID_HOBJECT)
							{
								sNPC_TBLDAT* pNpcTbldat = (sNPC_TBLDAT*)g_pTableContainer->GetNpcTable()->FindData((TBLIDX)m_pSkillDataRef->aSkill_Effect_Value[e]);
								if (pNpcTbldat)
								{
									CSummonPet* pPet = (CSummonPet*)g_pObjectManager->CreateCharacter(OBJTYPE_SUMMON_PET);
									if (pPet)
									{
										if (pPet->CreateDataAndSpawn(m_pOwnerRef->GetID(), GetSkillId(), pNpcTbldat, m_pOwnerRef->GetWorldID(), m_pOwnerRef->GetCurLoc(), m_pOwnerRef->GetCurDir()))
										{
											pPet->Spawn((CPlayer*)m_pOwnerRef);

											res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;
											res->aSkillResult[i].effectResult[e].fResultValue = (float)(m_pSkillDataRef->aSkill_Effect_Value[e]);
										}
										else
										{
											g_pObjectManager->DestroyCharacter(pPet);
											res->wResultCode = GAME_FAIL;
										}
									}
									else res->wResultCode = GAME_FAIL;
								}
								else res->wResultCode = GAME_FAIL;
							}
							else res->wResultCode = GAME_PET_CANT_MAKE_MORE_SUMMON_PET;
						}
						break;

						case ACTIVE_EP_UP:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;
							pTarget->UpdateCurEP((WORD)m_pSkillDataRef->aSkill_Effect_Value[e], true, false);
							res->aSkillResult[i].effectResult[e].fResultValue = (float)(m_pSkillDataRef->aSkill_Effect_Value[e]);
						}
						break;

						case ACTIVE_RP_UP:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;
							pTarget->UpdateCurRP((WORD)m_pSkillDataRef->aSkill_Effect_Value[e], true, false);
							res->aSkillResult[i].effectResult[e].fResultValue = (float)(m_pSkillDataRef->aSkill_Effect_Value[e]);
						}
						break;

						case ACTIVE_MONSTER_TRANSFORM:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;
							res->aSkillResult[i].effectResult[e].fResultValue = (float)(m_pSkillDataRef->aSkill_Effect_Value[e]);
						}
						break;

						case ACTIVE_BLEED_CURE:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;
							if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_BLEEDING))
							{
								if (Dbo_CheckProbabilityF((float)m_pSkillDataRef->aSkill_Effect_Value[e]))
									pTarget->GetBuffManager()->EndBuffs(ACTIVE_BLEED);
							}
						}break;
						case ACTIVE_BURN_CURE:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;
							if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_BURN))
							{
								if (Dbo_CheckProbabilityF((float)m_pSkillDataRef->aSkill_Effect_Value[e]))
									pTarget->GetBuffManager()->EndBuffs(ACTIVE_BURN);
							}
						}break;

						case ACTIVE_MIND_CONQUEST:
						{
							if (Dbo_CheckProbabilityF((float)m_pSkillDataRef->aSkill_Effect_Value[e]))
							{
								if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_TERROR))
									pTarget->GetBuffManager()->EndBuffs(ACTIVE_TERROR);
								if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_CONFUSED))
								{
									pTarget->GetBuffManager()->EndBuffs(ACTIVE_CONFUSE);
									pTarget->GetBuffManager()->EndBuffs(ACTIVE_YELL);
								}
							}
						}
						break;
						case ACTIVE_BODY_CONQUEST:
						{
							if (Dbo_CheckProbabilityF((float)m_pSkillDataRef->aSkill_Effect_Value[e]))
							{
								pTarget->GetBuffManager()->EndBuffs(ACTIVE_STUN);
								pTarget->GetBuffManager()->EndBuffs(ACTIVE_PARALYZE);
							}
						}
						break;
						case ACTIVE_CURSE_EAT: //recover lp and remove oldest curse
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DH_HOT;
							res->aSkillResult[i].effectResult[e].fResultValue = (float)(m_pSkillDataRef->aSkill_Effect_Value[e]);

							pTarget->GetBuffManager()->RemoveOldestCurseBuff();
							pTarget->UpdateCurLP((int)res->aSkillResult[i].effectResult[e].fResultValue, true, false);
						}
						break;
						case ACTIVE_RABIES:
						{
							aBuffParameter[e].buffParameter.commonConfigTblidx = (TBLIDX)m_pSkillDataRef->aSkill_Effect_Value[e];
						}
						break;

						case ACTIVE_VIABILITY:
						{
							if (pTarget->GetCurLpInPercent() <= 30.0f)
							{
								aBuffParameter[e].buffParameter.dwRemainValue = (DWORD)m_pSkillDataRef->aSkill_Effect_Value[e];
								aBuffParameter[e].buffParameter.fParameter = res->aSkillResult[i].effectResult[e].fResultValue = (float)m_pSkillDataRef->aSkill_Effect_Value[e];
							}
						}
						break;

						case ACTIVE_ABSORB_CURSE:
						{
							dwKeepTime = 0; // To avoid registering an useless buff

							if (pTarget->GetBuffManager()->RemoveRandomCurseBuff(true))
							{
								sSKILL_TBLDAT* pBonusSkillData = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData((TBLIDX)m_pSkillDataRef->aSkill_Effect_Value[e]);
								if (pBonusSkillData)
								{
									if (BIT_FLAG_TEST(pBonusSkillData->dwFunction_Bit_Flag, MAKE_BIT_FLAG(SKILL_FUNCTION_SUB_BUFF_SYSTEM)))
									{
										sBUFF_INFO buffInfo;
										buffInfo.buffIndex = INVALID_BYTE;
										buffInfo.sourceTblidx = pBonusSkillData->tblidx;
										buffInfo.dwTimeRemaining = GetOriginalTableData()->dwKeepTimeInMilliSecs;
										buffInfo.dwInitialDuration = buffInfo.dwTimeRemaining;
										buffInfo.bySourceType = DBO_OBJECT_SOURCE_SKILL;

										buffInfo.aBuffParameter[0].buffParameter.fParameter = (float)pBonusSkillData->aSkill_Effect_Value[0];
										buffInfo.aBuffParameter[1].buffParameter.fParameter = (float)pBonusSkillData->aSkill_Effect_Value[1];

										eSYSTEM_EFFECT_CODE subBuffEffectCode[NTL_MAX_EFFECT_IN_ITEM];
										subBuffEffectCode[0] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pBonusSkillData->skill_Effect[0]);
										subBuffEffectCode[1] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pBonusSkillData->skill_Effect[1]);

										WORD wTemp;
										pTarget->GetBuffManager()->RegisterSubBuff(&buffInfo, subBuffEffectCode, m_pOwnerRef->GetID(), pBonusSkillData->byBuff_Group, wTemp, pBonusSkillData->bySkill_Effect_Type);
									}
								}
							}
						}
						break;

						case ACTIVE_EP_DOWN:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;
							pTarget->UpdateCurEP((WORD)m_pSkillDataRef->aSkill_Effect_Value[e], false, false);
							res->aSkillResult[i].effectResult[e].fResultValue = (float)m_pSkillDataRef->aSkill_Effect_Value[e];
						}
						break;

						case ACTIVE_RP_DOWN:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;
							pTarget->UpdateCurRP((WORD)m_pSkillDataRef->aSkill_Effect_Value[e], false, false);
							res->aSkillResult[i].effectResult[e].fResultValue = (float)m_pSkillDataRef->aSkill_Effect_Value[e];
						}
						break;

						case ACTIVE_BLESS_DEBUFF: //remove a bless buff
						{
							if (Dbo_CheckProbabilityF((float)m_pSkillDataRef->aSkill_Effect_Value[e]))
							{
								if (pTarget->IsMonster())
								{
									if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_10)))
									{
										res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
										goto RESULT2;
									}
								}

								pTarget->GetBuffManager()->RemoveRandomBlessBuff();
							}
						}
						break;

						case ACTIVE_LP_STEAL:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_LP_EP_STEAL;

							m_actionSkill.bCanTargetRecoverOnHit = false;
							
							//calculate the damage
							if (m_pSkillDataRef->bySkill_Effect_Type[e] == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
							{
								CalcSkillDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, (float)m_pSkillDataRef->aSkill_Effect_Value[e], res->aSkillResult[i].effectResult[e].LP_EP_STEAL_fLpDamage, res->aSkillResult[i].byAttackResult, res->aSkillResult[i].damageByReflectingCurse, &res->aSkillResult[i].effectResult[e].DD_DOT_lpEpRecovered);

								res->aSkillResult[i].effectResult[e].LP_EP_STEAL_fLpGained = (float)m_pSkillDataRef->aSkill_Effect_Value[e];
							}
							else if (m_pSkillDataRef->bySkill_Effect_Type[e] == SYSTEM_EFFECT_APPLY_TYPE_PERCENT) //used for dragons punishment(dark warrior skill)
							{
								float fTmpState = 0.f;

								if (m_pOwnerRef->IsPC() && m_pSkillDataRef->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
									fTmpState = (float)(m_pOwnerRef->GetCharAtt()->GetSubWeaponPhysicalOffence() + m_pOwnerRef->GetCharAtt()->GetSubWeaponEnergyOffence() / 2);
								else
									fTmpState = (float)(m_pOwnerRef->GetCharAtt()->GetPhysicalOffence() + m_pOwnerRef->GetCharAtt()->GetEnergyOffence() / 2);

								res->aSkillResult[i].effectResult[e].LP_EP_STEAL_fLpDamage = 0.0f;
								res->aSkillResult[i].effectResult[e].LP_EP_STEAL_fLpGained = fTmpState * (float)m_pSkillDataRef->aSkill_Effect_Value[e] / 100.f;
							}
						}
						break;

						case ACTIVE_PULL:
						{
							if (pTarget->IsMonster())
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_CHEWING_GUM)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}

							m_actionSkill.bCanWakeUpTarget = true;

							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;

							if (m_vFinalSubjectLoc.IsZero() == false)
							{
								pTarget->SetCurLoc(m_vFinalSubjectLoc);
							}

							pTarget->GetCurLoc().CopyTo(res->aSkillResult[i].vFinalSubjectLoc);
						}
						break;

						case ACTIVE_MAKE_LOWEST_AGGRO:
						{
							if (pTarget->IsPC())
								goto LOOP_CONTINUE;

							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;

							pTarget->ChangeAggro(m_pOwnerRef->GetID(), DBO_AGGRO_CHANGE_TYPE_LOWEST, (DWORD)nAgro);

							nAgro = 0;
						}
						break;

						case ACTIVE_YELL:
						{
							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetMindCurseImmunity()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								goto RESULT2;
							}

							if (Dbo_CheckProbabilityF((float)m_pSkillDataRef->aSkill_Effect_Value[e]))
							{
								dwKeepTime = 1500 * m_pSkillDataRef->bySkill_Grade;
								dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, DWORD((float)dwKeepTime * pTarget->GetCharAtt()->GetConfuseKeepTimeDown() / 100.0f));

								aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DEFAULT;

								pTarget->GetBuffManager()->RegisterBuff(dwKeepTime, m_aeEffectCode, aBuffParameter, m_pOwnerRef->GetID(), BUFF_TYPE_CURSE, GetOriginalTableData());
							}
						}
						break;

						case ACTIVE_TAUNT:
						{
							if (pTarget->IsMonster())
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_TAUNT)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}

							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetMindCurseImmunity()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								goto RESULT2;
							}
						}
						break;

						case ACTIVE_PARALYZE:
						{
							if (pTarget->IsMonster())
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_PARALYZE)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}
							
							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetBodyCurseImmunity()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								goto RESULT2;
							}
							else if (!Dbo_CheckProbability((int)pTarget->GetCharAtt()->GetParalyzeToleranceRate()))
							{
								dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, DWORD((float)dwKeepTime * pTarget->GetCharAtt()->GetParalyzeKeepTimeDown() / 100.0f));
							}
							else
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_RESISTED;
								goto RESULT2;
							}
						}
						break;

						case ACTIVE_CONFUSE:
						{
							if (pTarget->IsMonster())
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_CONFUSION)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}

							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetMindCurseImmunity()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								goto RESULT2;
							}

							if (Dbo_CheckProbability((int)pTarget->GetCharAtt()->GetConfuseToleranceRate()) == false)
							{
								dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, (DWORD)((float)dwKeepTime * pTarget->GetCharAtt()->GetConfuseKeepTimeDown() / 100.0f));
							}
							else
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_RESISTED;
								goto RESULT2;
							}
						}
						break;

						case ACTIVE_TERROR: //fear
						{
							if (pTarget->IsMonster())
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_FEAR)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}

							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetMindCurseImmunity()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								goto RESULT2;
							}

							if (Dbo_CheckProbability((int)pTarget->GetCharAtt()->GetTerrorToleranceRate()) == false)
							{
								dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, (DWORD)((float)dwKeepTime * pTarget->GetCharAtt()->GetTerrorKeepTimeDown() / 100.0f));
							}
							else
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_RESISTED;
								goto RESULT2;
							}
						}
						break;

						case ACTIVE_STUN:
						{
							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetBodyCurseImmunity()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								goto RESULT2;
							}
							else if (pTarget->IsMonster()) //some mobs have immunity against stun etc
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_STUN)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}
						}
						break;

						case ACTIVE_SLEEP:
						{
							if (pTarget->IsMonster())
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_SLEEP)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}
						}
						break;

						case ACTIVE_STONE:
						{
							if (pTarget->IsMonster())
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_PETRIFICATION)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}

							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetChangeCurseImmunity()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
							}
							else if (Dbo_CheckProbability((int)pTarget->GetCharAtt()->GetStoneToleranceRate()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_RESISTED;
								goto RESULT2;
							}
							else
							{
								dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, (DWORD)((float)dwKeepTime * pTarget->GetCharAtt()->GetStoneKeepTimeDown() / 100.0f));
							}
						}
						break;

						case ACTIVE_CANDY:
						{
							if (pTarget->IsMonster())
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_CANDY)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}

							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetChangeCurseImmunity()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
							}
							else if (Dbo_CheckProbability((int)pTarget->GetCharAtt()->GetCandyToleranceRate()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_RESISTED;
								goto RESULT2;
							}
							else
							{
								dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, (DWORD)((float)dwKeepTime * pTarget->GetCharAtt()->GetCandyKeepTimeDown() / 100.0f));
							}
						}
						break;

						case ACTIVE_TIED_UP:
						{
							if (pTarget->IsMonster()) //some mobs have immunity against stun etc
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_STUN)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}

							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetBodyCurseImmunity()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								goto RESULT2;
							}
						}
						break;

						case ACTIVE_FREEZE:
						{
							if (pTarget->IsMonster())
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_FREEZE)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}

							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetBodyCurseImmunity()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								goto RESULT2;
							}
						}
						break;

						case ACTIVE_LP_STEAL_OVER_TIME:
						case ACTIVE_EP_STEAL_OVER_TIME:
						{
							aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_STEAL_OVER_TIME;
							aBuffParameter[e].buffParameter.ACTIVE_LP_EP_STEAL_OVER_TIME_bInPercent = (m_pSkillDataRef->bySkill_Effect_Type[e] == SYSTEM_EFFECT_APPLY_TYPE_PERCENT) ? true : false;
							aBuffParameter[e].buffParameter.ACTIVE_LP_EP_STEAL_OVER_TIME_fDamage = (float)m_pSkillDataRef->aSkill_Effect_Value[e];

							aBuffParameter[e].buffParameter.fParameter = res->aSkillResult[i].effectResult[e].fResultValue = aBuffParameter[e].buffParameter.ACTIVE_LP_EP_STEAL_OVER_TIME_fDamage;
						}
						break;

						case ACTIVE_WARP_STUN:
						{
							if (pTarget->IsMonster()) //some mobs have immunity against stun etc
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_STUN)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}

							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetBodyCurseImmunity()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								goto RESULT2;
							}

							if (m_pOwnerRef->GetStateManager()->IsCharCondition(CHARCOND_NULLIFIED_DAMAGE) == false) //this check is REQUIRED! If we hit multiple targets then we set the state multiple times and in the end it will not be removed!
								m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_NULLIFIED_DAMAGE, NULL, false);
						}
						break;

						case ACTIVE_BATTLE_INABILITY:
						{
							if (pTarget->IsMonster()) //some mobs have immunity against stun etc
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_FIGHT_INABILITY)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}
						}
						break;

						case ACTIVE_SKILL_INABILITY:
						{
							if (pTarget->IsMonster()) //some mobs have immunity against stun etc
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_SKILL_LOCK)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}
						}
						break;

						default: //to avoid having a case for every effect we just use the default
						{
							aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DEFAULT;

							float fEffectVal = (float)m_pSkillDataRef->aSkill_Effect_Value[e];

							if (m_pSkillDataRef->bySkill_Active_Type == SKILL_ACTIVE_TYPE_DOT)
							{
								fEffectVal += (float)m_pSkillDataRef->aSkill_Effect_Value[e] * m_pOwnerRef->GetCharAtt()->GetDotValueChangePercent() / 100.f;

								aBuffParameter[e].buffParameter.DOT_fDamage = fEffectVal;
							}

							aBuffParameter[e].buffParameter.fParameter = res->aSkillResult[i].effectResult[e].fResultValue = fEffectVal;

							aBuffParameter[e].buffParameter.dwRemainValue = (DWORD)m_pSkillDataRef->aSkill_Effect_Value[e];
							aBuffParameter[e].buffParameter.dwRemainTime = dwKeepTime;
						}
						break;
						}

						if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_GUARD_CRASH && pTarget->GetCharStateID() == CHARSTATE_GUARD)
						{
							if (m_pSkillDataRef->bySkill_Active_Type == SKILL_ACTIVE_TYPE_DOT)
							{
								aBuffParameter[e].buffParameter.DOT_fDamage += aBuffParameter[e].buffParameter.DOT_fDamage * 2.5f;
								aBuffParameter[e].buffParameter.fParameter = aBuffParameter[e].buffParameter.DOT_fDamage;
							}
							else
								res->aSkillResult[i].effectResult[e].DD_DOT_fDamage += res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * 2.5f; //increase by 250%
						}
					}
				}

				if (res->aSkillResult[i].byAttackResult != BATTLE_ATTACK_RESULT_BLOCK && pTarget->IsFainting() == false)
				{
					if (BIT_FLAG_TEST(GetOriginalTableData()->dwFunction_Bit_Flag, SKILL_FUNCTION_FLAG_DAMAGE_CASTING_CANCEL))
					{
						if (pTarget->IsCastingSkill())
							pTarget->SendCharStateStanding();
					}


					if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_GUARD_CRASH && pTarget->GetCharStateID() == CHARSTATE_GUARD)
					{
						((CPlayer*)pTarget)->CrashGuard();
					}


				RESULT2:

					if (res->aSkillResult[i].byAttackResult == BATTLE_ATTACK_RESULT_BLOCK && res->bIsSkillHarmful && bCanCounterAttack)
					{
						CSkill* pCountAttack = pTarget->GetSkillManager()->FindSkillWithSystemEffectCode(PASSIVE_GUARD_COUNTERATTACK);
						if (pCountAttack && m_pOwnerRef->IsInRange(pTarget, pCountAttack->GetOriginalTableData()->fUse_Range_Max))
						{
							if (Dbo_CheckProbability(50)) // chance to use passive "counter attack"
							{
								pTarget->SendCharStateStanding();
								WORD wTempCounterAttackRes;
								HOBJECT ahTempCounterAttack = m_pOwnerRef->GetID();
								pCountAttack->UseSkill(INVALID_BYTE, ahTempCounterAttack, m_pOwnerRef->GetCurLoc(), m_pOwnerRef->GetCurLoc(), 1, &ahTempCounterAttack, wTempCounterAttackRes);
							}
						}
					}

					if (res->bIsSkillHarmful && nAgro > 0)
					{
						nAgro += (int)m_pOwnerRef->GetCharAtt()->GetSkillAggroBonus();
						nAgro += (int)(nAgro * m_pOwnerRef->GetCharAtt()->GetSkillAggroBonusInPercent() / 100.f);

						pTarget->ChangeAggro(m_pOwnerRef->GetID(), DBO_AGGRO_CHANGE_TYPE_INCREASE, (DWORD)nAgro);
					}
				}

			RESULT3:

				res->bySkillResultCount += 1;
				res->aSkillResult[i].hTarget = pTarget->GetID();

				if (bGuardCrashFail == false)
				{
					if (res->aSkillResult[i].byAttackResult != BATTLE_ATTACK_RESULT_DODGE && res->aSkillResult[i].byAttackResult != BATTLE_ATTACK_RESULT_IMMUNE && res->aSkillResult[i].byAttackResult != BATTLE_ATTACK_RESULT_BLOCK)
					{
						if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_KEEP_TIME_PLUS)
							dwKeepTime += DWORD(m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_KEEP_TIME_PLUS] * 1000.f);

						if (dwKeepTime > 0 && res->aSkillResult[i].byAttackResult != BATTLE_ATTACK_RESULT_RESISTED)
						{
							m_actionSkill.dwKeepTime[i] = dwKeepTime;
							memcpy(&m_actionSkill.aBuffParameter[i][NTL_SYSTEM_EFFECT_1], &aBuffParameter[NTL_SYSTEM_EFFECT_1], sizeof(aBuffParameter[NTL_SYSTEM_EFFECT_1]));
							memcpy(&m_actionSkill.aBuffParameter[i][NTL_SYSTEM_EFFECT_2], &aBuffParameter[NTL_SYSTEM_EFFECT_2], sizeof(aBuffParameter[NTL_SYSTEM_EFFECT_2]));

							m_actionSkill.bySkillResultBuffCount++;
						}
						
						memcpy(&m_actionSkill.aSkillResult[m_actionSkill.bySkillResultCount], &res->aSkillResult[i], sizeof(res->aSkillResult[i]));

						m_actionSkill.bySkillResultCount++;
					}
				}

			}

		LOOP_CONTINUE:
			continue;
		}

		m_actionSkill.bIsSkillHarmful = res->bIsSkillHarmful;

		if (res->bySkillResultCount == 0 && m_pSkillDataRef->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_TARGET)
			res->wResultCode = GAME_SKILL_NO_TARGET_APPOINTED;

		// set final location. Do at bottom because we might fix the location
		NtlLocationCompress(&res->vFinalLoc, m_vFinalLoc.x, m_vFinalLoc.y, m_vFinalLoc.z);
		
		if (res->wResultCode == GAME_SUCCESS)
		{
			//send packet.
			//NOTE: This packet must be send before SendCharStateSkillAffecting. Otherwise there will be issues with spin attacks etc
			packet.SetPacketLen(sizeof(sGU_CHAR_ACTION_SKILL));
			m_pOwnerRef->Broadcast(&packet);


			if (GetOriginalTableData()->bDash_Able && m_actionSkill.bySkillResultCount > 0)
			{
				CNtlVector vDestLoc = g_pObjectManager->GetChar(m_actionSkill.aSkillResult[0].hTarget)->GetCurLoc();
				vDestLoc.x -= m_pOwnerRef->GetCurDir().x * m_pSkillDataRef->fUse_Range_Max;
				// keep y (height)same
				vDestLoc.z -= m_pOwnerRef->GetCurDir().z * m_pSkillDataRef->fUse_Range_Max;

				m_pOwnerRef->SetDestLoc(vDestLoc);
				m_pOwnerRef->SetMoveDirection(NTL_MOVE_F);
				m_pOwnerRef->SetMoveFlag(NTL_MOVE_FLAG_DASH);
			}
			else if (GetOriginalTableData()->bKeep_Effect && m_actionSkill.bySkillResultBuffCount > 0)
			{
				m_pOwnerRef->SendCharStateKeepingEffect(GetOriginalTableData()->tblidx);
			}

			//deduct ep
			if (GetOriginalTableData()->wRequire_EP > 0)
			{
				float fRequireEP = (float)GetOriginalTableData()->wRequire_EP;

				if (m_pOwnerRef->GetCharAtt()->GetRequiredEpChangePercent() != 0.0f)
					fRequireEP += fRequireEP * m_pOwnerRef->GetCharAtt()->GetRequiredEpChangePercent() / 100.0f;

				if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_EP_MINUS)
					fRequireEP -= fRequireEP * GetOriginalTableData()->afRpEffectValue[DBO_RP_BONUS_SLOT_EP_MINUS] / 100.0f;

				//	printf("fRequireEP %f | Cur EP %u, Max EP %u \n", fRequireEP, cPlayer->GetCurEP(), cPlayer->GetMaxEP());
				if (fRequireEP > 1.0f)
					m_pOwnerRef->UpdateCurEP((WORD)fRequireEP, false, false);
			}

			//deduct LP
			if (GetOriginalTableData()->dwRequire_LP > 0)
				m_pOwnerRef->UpdateCurLP(GetOriginalTableData()->dwRequire_LP, false, false);

			//update rp balls
			if (m_byUseRpBonusType != DBO_RP_BONUS_TYPE_INVALID)
				m_pOwnerRef->UpdateRpBall(1, false, false);

			//start cool down
			if (BIT_FLAG_TEST(GetOriginalTableData()->dwFunction_Bit_Flag, SKILL_FUNCTION_FLAG_SPECIAL_COOL_TIME_RULE) == false)
			{
				float fCoolDown = (float)GetOriginalTableData()->dwCoolTimeInMilliSecs;

				fCoolDown += fCoolDown * m_pOwnerRef->GetCharAtt()->GetCoolTimeChangePercent() / 100.0f;

				if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_COOL_TIME_MINUS)
				{
					float fRpCdr = GetOriginalTableData()->afRpEffectValue[DBO_RP_BONUS_SLOT_COOL_TIME_MINUS] * 1000.0f;
					fCoolDown -= fRpCdr;
				}

				if (fCoolDown < 1.0f)
					fCoolDown = 0.0f;
			//	printf("fCoolDown %f, dwCoolTimeInMilliSecs %u, GetCoolTimeChangePercent %f\n", fCoolDown, GetOriginalTableData()->dwCoolTimeInMilliSecs, m_pOwnerRef->GetCharAtt()->GetCoolTimeChangePercent());
				SetCoolTimeRemaining((DWORD)fCoolDown);
			}
		//	printf("GetOriginalTableData()->bKeep_Effect:%i\n", GetOriginalTableData()->bKeep_Effect);
			if (GetOriginalTableData()->bKeep_Effect && m_actionSkill.bySkillResultBuffCount > 0)
			{
				OnAffected();
			}
			else if (GetOriginalTableData()->bKeep_Effect == false)
			{
				if (m_pOwnerRef->SendCharStateSkillAffecting(GetSkillId(), m_hUseAppointedTarget) == false)
				{
					//init skill
					InitSkillUse();

					m_pOwnerRef->GetSkillManager()->CancelCasting(); //do this or monster keep freezing forever
				}
			}
		}
		else
		{
			m_pOwnerRef->Broadcast(&packet);

			//init skill
			InitSkillUse();

			m_pOwnerRef->GetSkillManager()->CancelCasting(); //do this or monster keep freezing forever

			m_pOwnerRef->SendCharStateStanding();
		}


		//when use this, then some mobs are invincible for ever because they spam skill...
		//if (BIT_FLAG_TEST(m_pSkillDataRef->dwFunction_Bit_Flag, MAKE_BIT_FLAG(SKILL_FUNCTION_SET_INVINCIBLE)))
		//{
		//	printf("LOL m_pSkillDataRef->dwFunction_Bit_Flag %u\n", m_pSkillDataRef->dwFunction_Bit_Flag);
		//	m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_INVINCIBLE, NULL, true);
		//}

		//for (int i = 0; i < SKILL_FUNCTION_COUNT; i++)
		//{
		//	if (BIT_FLAG_TEST(m_pSkillDataRef->dwFunction_Bit_Flag, MAKE_BIT_FLAG(i)))
		//	{
		//		printf("skill has following flags %i \n", i);
		//	}
		//}
}


void CSkill::OnAffected()
{
	//NTL_PRINT(PRINT_APP,"CSkill::OnAffected() bySkillResultCount %u", m_actionSkill.bySkillResultCount);
	int nTotalReflect = 0;

	for (int i = 0; i < m_actionSkill.bySkillResultCount; i++)
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(m_actionSkill.aSkillResult[i].hTarget);
		if (pTarget && pTarget->IsInitialized() && pTarget->IsFainting() == false)
		{
			nTotalReflect += m_actionSkill.aSkillResult[i].damageByReflectingCurse;

			if (m_actionSkill.bIsSkillHarmful && !m_pOwnerRef->IsAttackable(pTarget))
			{
			//	NTL_PRINT(PRINT_APP, "IsAttackable == false");
				continue;
			}
			if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_NULLIFIED_DAMAGE))
			{
			//	NTL_PRINT(PRINT_APP,"CHARCOND_NULLIFIED_DAMAGE");
				continue;
			}

			float fDmg = 0.0f;
			int nRecoverLP = 0;

			if (m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].eResultType == DBO_SYSTEM_EFFECT_RESULT_TYPE_LP_EP_STEAL)
			{
				fDmg += m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].LP_EP_STEAL_fLpDamage;
				nRecoverLP += (int)m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].LP_EP_STEAL_fLpGained;
			}
			if (m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].eResultType == DBO_SYSTEM_EFFECT_RESULT_TYPE_LP_EP_STEAL)
			{
				fDmg += m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].LP_EP_STEAL_fLpDamage;
				nRecoverLP += (int)m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].LP_EP_STEAL_fLpGained;
			}

			if(nRecoverLP > 0)
				m_pOwnerRef->UpdateCurLP(nRecoverLP, true, false);

			if (m_actionSkill.aSkillResult[i].byAttackResult == BATTLE_ATTACK_RESULT_KNOCKDOWN && pTarget->IsFainting() == false)
			{
				CNtlVector vShift(pTarget->GetCurLoc() - m_pOwnerRef->GetCurLoc());
				vShift.y = 0.0f;
				vShift.SafeNormalize();
				vShift.CopyTo(m_actionSkill.aSkillResult[i].vShift);
				pTarget->SendCharStateKnockdown(m_actionSkill.aSkillResult[i].vShift);
			}

			bool bIsTargetFaint = false;

			if (m_actionSkill.bIsSkillHarmful)
			{
				if (m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].eResultType == DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT)
				{
					fDmg += m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].DD_DOT_fDamage;
				}

				if (m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].eResultType == DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT)
				{
					fDmg += m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].DD_DOT_fDamage;
				}

				//printf("fDmg %f bySkill_Active_Type %u \n", fDmg, GetOriginalTableData()->bySkill_Active_Type);
				if (fDmg > 0.0f && m_actionSkill.bCanTargetRecoverOnHit) //deal dmg and check if not dead
				{
					if (pTarget->OnSkillAction(m_pOwnerRef, (int)fDmg, 0, m_actionSkill.aSkillResult[i].byAttackResult, m_actionSkill.bCanWakeUpTarget) == false)
					{
						if (m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].DD_DOT_lpEpRecovered.bIsLpRecoveredWhenHit || m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].DD_DOT_lpEpRecovered.bIsLpRecoveredWhenHit)
							pTarget->UpdateCurLP(m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].DD_DOT_lpEpRecovered.targetLpRecoveredWhenHit + m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].DD_DOT_lpEpRecovered.targetLpRecoveredWhenHit, true, false);

						if (m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].DD_DOT_lpEpRecovered.bIsEpRecoveredWhenHit || m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].DD_DOT_lpEpRecovered.bIsEpRecoveredWhenHit)
							pTarget->UpdateCurEP(WORD(m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].DD_DOT_lpEpRecovered.dwTargetEpRecoveredWhenHit + m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].DD_DOT_lpEpRecovered.dwTargetEpRecoveredWhenHit), true, false);
					}
					else
					{
						bIsTargetFaint = true;
					}
				}
				else if (m_actionSkill.bCanWakeUpTarget)
				{
					pTarget->GetBuffManager()->EndBuff(ACTIVE_SLEEP);
				}
			}

			//add buffs last or it might happen that buff like sleep will be removed at OnSkillAction

			//check if we have to add a buff
			//	printf("m_actionSkill.dwKeepTime[i] %u m_actionSkill.aSkillResult[i].byAttackResult %u\n", m_actionSkill.dwKeepTime[i], m_actionSkill.aSkillResult[i].byAttackResult);
			if (bIsTargetFaint == false && m_actionSkill.dwKeepTime[i] > 0 && m_actionSkill.aSkillResult[i].byAttackResult != BATTLE_ATTACK_RESULT_RESISTED && m_actionSkill.aSkillResult[i].byAttackResult != BATTLE_ATTACK_RESULT_IMMUNE)
			{
				BYTE buffIndex = INVALID_BYTE;

				if (IsForBlessBuff())
				{
					pTarget->GetBuffManager()->RegisterBuff(m_actionSkill.dwKeepTime[i], m_aeEffectCode, m_actionSkill.aBuffParameter[i], m_pOwnerRef->GetID(), BUFF_TYPE_BLESS, GetOriginalTableData(), &buffIndex);
				}
				else if (IsForCurseBuff())
				{
					pTarget->GetBuffManager()->RegisterBuff(m_actionSkill.dwKeepTime[i], m_aeEffectCode, m_actionSkill.aBuffParameter[i], m_pOwnerRef->GetID(), BUFF_TYPE_CURSE, GetOriginalTableData(), &buffIndex);
				}

				if (GetOriginalTableData()->bKeep_Effect) 
					m_pOwnerRef->InsertKeepEffectTarget(pTarget->GetID(), buffIndex);
			}
		}
	}

	//deal reflect damage
	if (nTotalReflect > 0)
	{
		if(m_pOwnerRef->OnAttackAction(NULL, nTotalReflect, BATTLE_ATTACK_RESULT_REFLECTED_DAMAGE))
		{
			//init skill
			InitSkillUse();

			return;
		}
	}

	// Check if has curse which damage me when using skill
	if (m_pOwnerRef->GetStateManager()->IsCharCondition(CHARCOND_INVINCIBLE) == false)
	{
		if (CBuff* pCurseBuff = m_pOwnerRef->GetBuffManager()->FindAnyBuff(ACTIVE_SKILL_LP_DAMAGE))
		{
			sSKILL_TBLDAT* pCurseDebuffSkill = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(pCurseBuff->GetSourceTblidx());
			if (pCurseDebuffSkill)
			{
				unsigned int x = 0;
				float fCurseDmg = 0.0f;

				for (; x < NTL_MAX_EFFECT_IN_SKILL; x++)
				{
					if (pCurseBuff->GetSystemEffectCode(x) == ACTIVE_SKILL_LP_DAMAGE)
					{
						fCurseDmg = pCurseBuff->GetBuffInfo()->aBuffParameter[x].buffParameter.fParameter;
						break;
					}
				}

				if (fCurseDmg > 0.0f)
				{
					if (pCurseDebuffSkill->bySkill_Effect_Type[x] == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
						fCurseDmg = fCurseDmg * (float)m_pOwnerRef->GetCurLP() / 100.f;

					//	printf("fCurseDmg %f, pCurseDebuffSkill->bySkill_Effect_Type[0] %u \n", fCurseDmg, pCurseDebuffSkill->bySkill_Effect_Type[x]);

					m_pOwnerRef->SendEffectAffected(g_pTableContainer->GetSystemEffectTable()->GetEffectTblidx(ACTIVE_SKILL_LP_DAMAGE), pCurseBuff->GetSourceType(), pCurseBuff->GetSourceTblidx(),
						fCurseDmg, INVALID_FLOAT, pCurseBuff->GetCasterHandle());

					if (m_pOwnerRef->OnBuffDamage(pCurseBuff->GetCasterHandle(), fCurseDmg))
					{
						//init skill
						InitSkillUse();

						return;
					}
				}
			}
		}
	}

	if (m_pOwnerRef->IsPC())
		((CPlayer*)m_pOwnerRef)->SetNextAttackTime();

	if (BIT_FLAG_TEST(m_pSkillDataRef->dwFunction_Bit_Flag, SKILL_FUNCTION_FLAG_FAINTING_AFTER_APPLYING) && m_pOwnerRef->IsFainting() == false)
	{
		m_pOwnerRef->Faint(NULL, FAINT_REASON_SELF_FAINTING);
	}

	//do it outside loop. Otherwise if the target is dead and no other target exist, this will never be called.
	for (int e = 0; e < NTL_MAX_EFFECT_IN_SKILL; e++)
	{
		switch (GetEffectCode(e))
		{
			case ACTIVE_WARP_BACK_ATTACK:
			case ACTIVE_THROUGH_ATTACK:
			case ACTIVE_WARP_STUN:
			{
				m_pOwnerRef->SetCurLoc(m_vFinalLoc);

				m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_NULLIFIED_DAMAGE, NULL, false);
			}
			break;

			default: break;
		}
	}

	//init skill
	InitSkillUse();
}

void CSkill::OnAffectingEnd()
{
	//NTL_PRINT(PRINT_APP,"CSkill::OnAffectingEnd()");
	for (int e = 0; e < NTL_MAX_EFFECT_IN_SKILL; e++)
	{
		switch (GetEffectCode(e))
		{
			case ACTIVE_SUPER_SAIYAN:
			case ACTIVE_PURE_MAJIN:
			case ACTIVE_GREAT_NAMEK:
			{
				m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_NULLIFIED_DAMAGE, NULL, false);

				int nOldMaxLP = m_pOwnerRef->GetMaxLP();
				WORD wOldMaxEP = m_pOwnerRef->GetMaxEP();

				((CPlayer*)m_pOwnerRef)->Transform(GetEffectCode(e), (TBLIDX)m_pSkillDataRef->aSkill_Effect_Value[e], true, GetSkillIndex());

				if (m_pOwnerRef->GetMaxLP() - nOldMaxLP > 0)
					m_pOwnerRef->UpdateCurLP(m_pOwnerRef->GetMaxLP() - nOldMaxLP, true, false);

				if (m_pOwnerRef->GetMaxEP() - wOldMaxEP > 0)
					m_pOwnerRef->UpdateCurEP(m_pOwnerRef->GetMaxEP() - wOldMaxEP, true, false);
			}
			break;

			case ACTIVE_MONSTER_TRANSFORM:
			{
				if (m_pOwnerRef->IsMonster() || m_pOwnerRef->IsNPC())
				{
					if (m_pOwnerRef->GetTblidx() != (TBLIDX)m_pSkillDataRef->aSkill_Effect_Value[e])
					{
						((CNpc*)m_pOwnerRef)->DoTransformation((TBLIDX)m_pSkillDataRef->aSkill_Effect_Value[e], ((CNpc*)m_pOwnerRef)->GetActionPattern()->GetTableIdx(), INVALID_TBLIDX);

						return; //return because it might be possible, that this skill will be deleted In DoTransformation
					}
				}
			}
			break;

			default: break;
		}
	}
}


void CSkill::OnAffectedCanceled()
{
	//NTL_PRINT(PRINT_APP, "CSkill::OnAffectedCanceled()");
	CNtlPacket packet(sizeof(sGU_SKILL_AFFECTING_CANCELED_NFY));
	sGU_SKILL_AFFECTING_CANCELED_NFY * res = (sGU_SKILL_AFFECTING_CANCELED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_SKILL_AFFECTING_CANCELED_NFY;
	res->hSubject = m_pOwnerRef->GetID();
	packet.SetPacketLen(sizeof(sGU_SKILL_AFFECTING_CANCELED_NFY));
	m_pOwnerRef->Broadcast(&packet);

	for (int e = 0; e < NTL_MAX_EFFECT_IN_SKILL; e++)
	{
		switch (GetEffectCode(e))
		{
			case ACTIVE_SUPER_SAIYAN:
			case ACTIVE_PURE_MAJIN:
			case ACTIVE_GREAT_NAMEK:
			{
				m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_NULLIFIED_DAMAGE, NULL, false);
			}
			break;

			default: break;
		}
	}

	if (m_pOwnerRef->IsPC())
		((CPlayer*)m_pOwnerRef)->SetNextAttackTime();

	//init skill
	InitSkillUse();
}

bool CSkill::IsForBlessBuff()
{
	if (Dbo_IsForBlessBuff(m_pSkillDataRef->bySkill_Active_Type))
		return true;

	return false;
}


bool CSkill::IsForCurseBuff()
{
	if (Dbo_IsForCurseBuff(m_pSkillDataRef->bySkill_Active_Type) || m_pSkillDataRef->bySkill_Active_Type == SKILL_ACTIVE_TYPE_CB)
		return true;

	return false;
}


void CSkill::Init()
{
	m_pSkillDataRef = NULL;
	m_pOwnerRef = NULL;
	m_dwAffectingTimeRemaining = 0;
	m_dwCoolTimeRemaining = 0;

	for (int j = 0; j < NTL_MAX_EFFECT_IN_SKILL; j++)
		m_aeEffectCode[j] = INVALID_SYSTEM_EFFECT_CODE;

	InitSkillUse();
}

void CSkill::InitSkillUse()
{
	memset(&m_actionSkill, -1, sizeof(m_actionSkill));
	m_actionSkill.bCanTargetRecoverOnHit = false;
	m_actionSkill.bCanWakeUpTarget = false;
	m_actionSkill.bIsSkillHarmful = false;
	m_actionSkill.bySkillResultBuffCount = 0;
	m_actionSkill.bySkillResultCount = 0;


	m_hUseAppointedTarget = INVALID_HOBJECT;
	m_byUseRpBonusType = DBO_RP_BONUS_TYPE_INVALID;
	m_byApplyTargetCount = 0;
	memset(m_ahApplyTarget, INVALID_HOBJECT, sizeof(m_ahApplyTarget));
	m_vFinalSubjectLoc.Reset();
	m_vFinalLoc.Reset();
}


void CSkill::DecreaseCoolTimeRemaining(DWORD dwTickDiff)
{
	m_dwCoolTimeRemaining = UnsignedSafeDecrease<DWORD>(m_dwCoolTimeRemaining, dwTickDiff);
}


void CSkill::DecreaseAffectingTimeRemaining(DWORD dwTickDiff)
{
	if (m_dwAffectingTimeRemaining > 0)
	{
		m_dwAffectingTimeRemaining = UnsignedSafeDecrease<DWORD>(m_dwAffectingTimeRemaining, dwTickDiff);

		if (m_dwAffectingTimeRemaining == 0)
		{
			if (m_pOwnerRef->GetCharStateID() == CHARSTATE_SKILL_AFFECTING)
			{
			//	if (m_pOwnerRef->IsPC())
			//		NTL_PRINT(PRINT_APP, "OnAffected | %f %f %f, state %u", m_pOwnerRef->GetCurLoc().x, m_pOwnerRef->GetCurLoc().y, m_pOwnerRef->GetCurLoc().z, m_pOwnerRef->GetCharStateID());
				OnAffected();
			}
		//	else NTL_PRINT(PRINT_APP, "lol m_pOwnerRef->GetCharStateID() = %u", m_pOwnerRef->GetCharStateID());
			m_pOwnerRef->GetSkillManager()->CancelCasting();
		}
	}
}


bool CSkill::IsForBuff()
{
	if (IsForBlessBuff())
		return true;

	return IsForCurseBuff();
}


void CSkill::ChangeOriginalTableData(sSKILL_TBLDAT *pSkillDataRef)
{
	m_pSkillDataRef = pSkillDataRef;

	for (int j = 0; j < NTL_MAX_EFFECT_IN_SKILL; j++)
	{
		sSYSTEM_EFFECT_TBLDAT* pEffectTbldat = (sSYSTEM_EFFECT_TBLDAT*)g_pTableContainer->GetSystemEffectTable()->FindData(pSkillDataRef->skill_Effect[j]);
		if (pEffectTbldat)
		{
			m_aeEffectCode[j] = pEffectTbldat->effectCode;
		}
	}
}

