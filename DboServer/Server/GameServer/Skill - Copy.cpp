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


bool CSkill::UseSkill(BYTE byRpBonusType, HOBJECT hAppointTargetHandle, CNtlVector& rvFinalSubjectLoc, BYTE byApplyTargetCount, HOBJECT* ahApplyTarget, WORD & rwResultCode)
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
			
			if (bIsHarmful && m_pSkillDataRef->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_SELF && m_pOwnerRef->GetID() != pTarget->GetID())
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

			else if(m_pSkillDataRef->bDash_Able == false && m_pOwnerRef->IsInRange3(pTarget, m_pSkillDataRef->fUse_Range_Max + 15.0f) == false) //15.0f tolerance
				rwResultCode = GAME_TARGET_TOO_FAR;
			else if (m_pSkillDataRef->bDash_Able == true && m_pOwnerRef->IsInRange3(pTarget, m_pSkillDataRef->fUse_Range_Max + NTL_MAX_DASH_DISTANCE_FOR_SKILL + 15.0f) == false) //15.0f tolerance
				rwResultCode = GAME_TARGET_TOO_FAR;

			else
				bValidState = Dbo_IsValidStateToUseSkill(m_pSkillDataRef->dwTransform_Use_Info_Bit_Flag, m_pOwnerRef->GetCharStateID(), m_pOwnerRef->GetCharStun().eStunType, m_pOwnerRef->GetAspectStateId(), m_pOwnerRef->GetConditionState(), false, m_pOwnerRef->GetMoveStatus() == NTL_MOVE_STATUS_JUMP, (eAIR_STATE)m_pOwnerRef->GetAirState(), &rwResultCode);

			if (bValidState && rwResultCode == GAME_SUCCESS)
			{
				//if(m_pOwnerRef->IsPC())
				//	NTL_PRINT(PRINT_APP, "charid = %u, m_pOwnerRef->GetCharStateID() %u, byApplyTargetCount %u", ((CPlayer*)m_pOwnerRef)->GetCharID(), m_pOwnerRef->GetCharStateID(), byApplyTargetCount);
				
				m_byUseRpBonusType = byRpBonusType;
				m_hUseAppointedTarget = hAppointTargetHandle;
				m_byApplyTargetCount = byApplyTargetCount;
				m_vFinalSubjectLoc = rvFinalSubjectLoc;

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
			- Immune: Damage = 0, and no cc applied;
		*/


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
		
		memset(res->aSkillResult, 0, sizeof(res->aSkillResult));
		memset(&m_actionSkill, 0, sizeof(m_actionSkill));

		for (int i = 0; i < m_byApplyTargetCount; i++)
		{
			CCharacter* pTarget = g_pObjectManager->GetChar(m_ahApplyTarget[i]);
			if (pTarget)
			{
				if (res->bIsSkillHarmful && !m_pOwnerRef->IsAttackable(pTarget))
					goto LOOP_CONTINUE;

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

				//NTL_PRINT(PRINT_APP, "USE-SKILL: CurLoc x %f y %f z %f, fSuccess_Rate %f", m_pOwnerRef->GetCurLoc().x, m_pOwnerRef->GetCurLoc().y, m_pOwnerRef->GetCurLoc().z, m_pSkillDataRef->fSuccess_Rate);
				if (res->bIsSkillHarmful)
				{
					/* - CHECK DODGE - */
					if (m_pSkillDataRef->bySkill_Active_Type == SKILL_ACTIVE_TYPE_DD || m_pSkillDataRef->bySkill_Active_Type == SKILL_ACTIVE_TYPE_DOT)
					{
						if (BattleIsDodge(pTarget->IsPC(), WORD((float)m_pOwnerRef->GetCharAtt()->GetLastAttackRate() * m_pSkillDataRef->fSuccess_Rate), pTarget->GetCharAtt()->GetLastDodgeRate(), m_pOwnerRef->GetLevel(), pTarget->GetLevel())) //check if dodge damage skill
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
							if (pTarget->IsKnockedDown()) //if knocked down then resist
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								goto RESULT2;
							}
						}

						if (BattleIsResist(WORD((float)m_pOwnerRef->GetCharAtt()->GetLastCurseSuccessRate() * m_pSkillDataRef->fSuccess_Rate), pTarget->GetCharAtt()->GetLastCurseToleranceRate(), m_pOwnerRef->GetLevel(), pTarget->GetLevel())) //check if resist direct curse
						{
							res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_RESISTED;
							goto RESULT2;
						}
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
						res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_KNOCKDOWN;

						if (pTarget->GetCharStateID() == CHARSTATE_GUARD)
						{
							if ((((CPlayer*)pTarget)->IsGuardSuccess(DBO_GUARD_TYPE_KNOCKDOWN, bCanCounterAttack)))
							{
								res->aSkillResult[i].byBlockedAction = DBO_GUARD_TYPE_KNOCKDOWN;
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_BLOCK;

								goto RESULT2;
							}
						}
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
					res->aSkillResult[i].effectResult[e].DD_DOT_lpEpRecovered.Reset();

					eSYSTEM_EFFECT_CODE effectCode = GetEffectCode(e);
					if (effectCode != INVALID_SYSTEM_EFFECT_CODE)
					{
					//	printf("Effect nr %u | effect %s \n", e, NtlGetSystemEffectString(effectCode));
						switch (effectCode)
						{
						case ACTIVE_DIRECT_DAMAGE:
						case ACTIVE_WARP_BACK_ATTACK:
						case ACTIVE_THROUGH_ATTACK:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;

							float fAttBonus = 0.0f;

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
								fAttBonus  = m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

							//calculate the damage
							CalcSkillDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, (float)m_pSkillDataRef->aSkill_Effect_Value[e] + fAttBonus, res->aSkillResult[i].effectResult[e].DD_DOT_fDamage, res->aSkillResult[i].byAttackResult, res->aSkillResult[i].damageByReflectingCurse, &res->aSkillResult[i].effectResult[e].DD_DOT_lpEpRecovered);
						
							if (effectCode == ACTIVE_WARP_BACK_ATTACK || effectCode == ACTIVE_THROUGH_ATTACK)
							{
								if(m_pOwnerRef->GetStateManager()->IsCharCondition(CHARCOND_NULLIFIED_DAMAGE) == false) //this check is REQUIRED! If we hit multiple targets then we set the state multiple times and in the end it will not be removed!
									m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_NULLIFIED_DAMAGE, NULL, false);
							}

							//aggro
							nAgro += (int)(res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * CFormulaTable::m_afRate[8002][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
						}
						break;
						case ACTIVE_BACKSTAB:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;

							float fBonusBaseDamage = 0.0f;

							//check if attack from behind
							if (pTarget->IsFrontSide(m_pOwnerRef) == false)
							{
								fBonusBaseDamage = (float)m_pSkillDataRef->aSkill_Effect_Value[e] * NTL_BATTLE_HIT_BACK_SIDE_BONUS_RATE - (float)m_pSkillDataRef->aSkill_Effect_Value[e]; //increase damage
							}

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
								fBonusBaseDamage += m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

							//calculate the damage
							CalcSkillDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, (float)m_pSkillDataRef->aSkill_Effect_Value[e] + fBonusBaseDamage, res->aSkillResult[i].effectResult[e].DD_DOT_fDamage, res->aSkillResult[i].byAttackResult, res->aSkillResult[i].damageByReflectingCurse, &res->aSkillResult[i].effectResult[e].DD_DOT_lpEpRecovered);

							//aggro
							nAgro += (int)(res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * CFormulaTable::m_afRate[8002][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
						}
						break;

						case ACTIVE_CRITICAL_ATTACK: //DD effect
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;

							float fBonusBaseDamage = 0.0f;
							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
								fBonusBaseDamage += m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

							//calculate the damage
							CalcSkillDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, (float)m_pSkillDataRef->aSkill_Effect_Value[e] + fBonusBaseDamage, res->aSkillResult[i].effectResult[e].DD_DOT_fDamage, res->aSkillResult[i].byAttackResult, res->aSkillResult[i].damageByReflectingCurse, &res->aSkillResult[i].effectResult[e].DD_DOT_lpEpRecovered, true);

							//aggro
							nAgro += (int)(res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * CFormulaTable::m_afRate[8002][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
						}
						break;

						case ACTIVE_EXPLOSION_BLEED:
						{
							if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_BLEEDING))
							{
								res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;
								res->aSkillResult[i].effectResult[e].DD_DOT_fDamage = (float)m_pSkillDataRef->aSkill_Effect_Value[e];

								if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
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
								res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;
								res->aSkillResult[i].effectResult[e].DD_DOT_fDamage = (float)m_pSkillDataRef->aSkill_Effect_Value[e];

								if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
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
								res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;
								res->aSkillResult[i].effectResult[e].DD_DOT_fDamage = (float)m_pSkillDataRef->aSkill_Effect_Value[e];

								if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
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
								res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;
								res->aSkillResult[i].effectResult[e].DD_DOT_fDamage = (float)m_pSkillDataRef->aSkill_Effect_Value[e];

								if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
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
								float fDOTDMG = 0.0f;
								int nBuffCount = 0;

								float fAttBonus = 0.0f;

								if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
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
										fCritDmgRate = m_pOwnerRef->GetCharAtt()->GetLastPhysicalCriticalDamageRate();
									}
									else if (m_pSkillDataRef->bySkill_Type == NTL_SKILL_TYPE_ENERGY) //check if energy dmg
									{
										fCritDmgRate = m_pOwnerRef->GetCharAtt()->GetLastEnergyCriticalDamageRate();
									}
									else if (m_pSkillDataRef->bySkill_Type == NTL_SKILL_TYPE_STATE)
									{
										fCritDmgRate = (m_pOwnerRef->GetCharAtt()->GetLastPhysicalCriticalDamageRate() + m_pOwnerRef->GetCharAtt()->GetLastEnergyCriticalDamageRate()) / 2.f;
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

						case ACTIVE_BLEED:
						case ACTIVE_POISON:
						case ACTIVE_STOMACHACHE:
						case ACTIVE_BURN://DOT buff effect // Skills with DOT effect never have 2x DOT effects in a skill. Its mostly 1x DOT and 1x Deal DMG.
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT;
							aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DOT;

							float fAttBonus = 0.0f;

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
								fAttBonus = m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_RESULT_PLUS];

							dwKeepTime += (DWORD)m_pOwnerRef->GetCharAtt()->GetDotTimeChangeAbsolute() * 1000; // update seconds to milliseconds

							if (effectCode == ACTIVE_BLEED)
							{
								dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, (DWORD)((float)dwKeepTime * pTarget->GetCharAtt()->GetLastBleedingKeepTimeDown() / 100.0f));

								//calculate the damage
								CalcSkillDotDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, pTarget->GetCharAtt()->GetLastBleedDefence(), (float)m_pSkillDataRef->aSkill_Effect_Value[e], fAttBonus, aBuffParameter[e].buffParameter.DOT_fDamage, res->aSkillResult[i].byAttackResult);
							}
							else if (effectCode == ACTIVE_POISON)
							{
								dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, (DWORD)((float)dwKeepTime * pTarget->GetCharAtt()->GetLastPoisonKeepTimeDown() / 100.0f));

								//calculate the damage
								CalcSkillDotDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, pTarget->GetCharAtt()->GetLastPoisonDefence(), (float)m_pSkillDataRef->aSkill_Effect_Value[e], fAttBonus, aBuffParameter[e].buffParameter.DOT_fDamage, res->aSkillResult[i].byAttackResult);
							}
							else if (effectCode == ACTIVE_STOMACHACHE)
							{
								dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, (DWORD)((float)dwKeepTime * pTarget->GetCharAtt()->GetLastStomachacheKeepTimeDown() / 100.0f));

								//calculate the damage
								CalcSkillDotDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, pTarget->GetCharAtt()->GetLastStomachacheDefence(), (float)m_pSkillDataRef->aSkill_Effect_Value[e], fAttBonus, aBuffParameter[e].buffParameter.DOT_fDamage, res->aSkillResult[i].byAttackResult);
							}
							else
							{
								//calculate the damage
								CalcSkillDotDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, pTarget->GetCharAtt()->GetLastBurnDefence(), (float)m_pSkillDataRef->aSkill_Effect_Value[e], fAttBonus, aBuffParameter[e].buffParameter.DOT_fDamage, res->aSkillResult[i].byAttackResult);
							}


							if (aBuffParameter[e].buffParameter.DOT_fDamage <= 0.f)
								aBuffParameter[e].buffParameter.DOT_fDamage = 1.0f;

							aBuffParameter[e].buffParameter.fParameter = aBuffParameter[e].buffParameter.DOT_fDamage;

							//aggro
							nAgro += (int)(aBuffParameter[e].buffParameter.DOT_fDamage * CFormulaTable::m_afRate[8005][1] * max((m_pOwnerRef->GetLevel() - pTarget->GetLevel()), 10) / 100.f);
						}
						break;

						case ACTIVE_DIRECT_HEAL:
						case ACTIVE_RESCUE: //When use revive skill then it will heal target too!
						{
							if (effectCode == ACTIVE_RESCUE)
							{
								res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DH_HOT;
								if (pTarget->IsFainting() && pTarget->IsPC()) //Only players can revive
								{
									pTarget->Revival(pTarget->GetCurLoc(), pTarget->GetWorldID(), REVIVAL_TYPE_RESCUED);
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

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
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

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
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

							if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_RESULT_PLUS && e == NTL_SYSTEM_EFFECT_1)
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
										//	printf("pTargetPC charid %u id %u GetID %u target %u ahApplyTarget[0] %u, m_hUseAppointedTarget %u\n", pTargetPC->GetCharID(), pTargetPC->GetID(), m_pOwnerRef->GetID(), m_pOwnerRef->GetTargetHandle(), ahApplyTarget[0], m_hUseAppointedTarget);
										if (pTargetPC->GetCurWorld())
										{
											if (pTargetPC->GetCurWorld()->GetRuleType() == GAMERULE_NORMAL)
											{
												if(pCasterPc->GetDragonballScrambleBallFlag() > 0 || (pTargetPC->GetPartyID() != INVALID_PARTYID && pTargetPC->GetPartyID() != pCasterPc->GetPartyID()))
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
							if (!pTarget->IsFainting())
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

							int nOldMaxLP = pTarget->GetLastMaxLP();
							WORD wOldMaxEP = pTarget->GetLastMaxEP();

							if (((CPlayer*)pTarget)->CanTransform(effectCode, (TBLIDX)m_pSkillDataRef->aSkill_Effect_Value[e], true))
							{
								((CPlayer*)pTarget)->Transform(effectCode, (TBLIDX)m_pSkillDataRef->aSkill_Effect_Value[e], true, GetSkillIndex());

								if (effectCode != ACTIVE_KAIOKEN)
								{
									if (pTarget->GetLastMaxLP() - nOldMaxLP > 0)
										pTarget->UpdateCurLP(pTarget->GetLastMaxLP() - nOldMaxLP, true, false);

									if (pTarget->GetLastMaxEP() - wOldMaxEP > 0)
										pTarget->UpdateCurEP(pTarget->GetLastMaxEP() - wOldMaxEP, true, false);
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

							if (pTarget->IsMonster() || pTarget->IsNPC())
							{
								if (pTarget->GetTblidx() != (TBLIDX)m_pSkillDataRef->aSkill_Effect_Value[e])
									((CNpc*)pTarget)->DoTransformation((TBLIDX)m_pSkillDataRef->aSkill_Effect_Value[e], ((CNpc*)pTarget)->GetActionPattern()->GetTableIdx(), INVALID_TBLIDX);
							}
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
									pTarget->GetBuffManager()->EndBuffs(ACTIVE_CONFUSE);
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
							if (pTarget->GetBuffManager()->RemoveRandomCurseBuff())
							{
								sSKILL_TBLDAT* pBonusSkillData = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData((TBLIDX)m_pSkillDataRef->aSkill_Effect_Value[e]);
								if (pBonusSkillData)
								{
									if (BIT_FLAG_TEST(pBonusSkillData->dwFunction_Bit_Flag, MAKE_BIT_FLAG(SKILL_FUNCTION_SUB_BUFF_SYSTEM)))
									{
										sBUFF_INFO buffInfo;
										buffInfo.buffIndex = INVALID_BYTE;
										buffInfo.sourceTblidx = pBonusSkillData->tblidx;
										buffInfo.dwTimeRemaining = pBonusSkillData->dwKeepTimeInMilliSecs;
										buffInfo.dwInitialDuration = pBonusSkillData->dwKeepTimeInMilliSecs;
										buffInfo.bySourceType = DBO_OBJECT_SOURCE_SKILL;

										buffInfo.aBuffParameter[0].buffParameter.fParameter = (float)pBonusSkillData->aSkill_Effect_Value[0];
										buffInfo.aBuffParameter[1].buffParameter.fParameter = (float)pBonusSkillData->aSkill_Effect_Value[1];

										eSYSTEM_EFFECT_CODE subBuffEffectCode[NTL_MAX_EFFECT_IN_ITEM];
										subBuffEffectCode[0] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pBonusSkillData->skill_Effect[0]);
										subBuffEffectCode[1] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pBonusSkillData->skill_Effect[1]);

										pTarget->GetBuffManager()->RegisterSubBuff(&buffInfo, subBuffEffectCode, m_pOwnerRef->GetID(), pBonusSkillData->byBuff_Group, res->wResultCode, pBonusSkillData->bySkill_Effect_Type);
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
								pTarget->GetBuffManager()->RemoveRandomBlessBuff();
							}
						}
						break;

						case ACTIVE_LP_STEAL:
						{
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_LP_EP_STEAL;
							
							//calculate the damage
							if (m_pSkillDataRef->bySkill_Effect_Type[e] == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
							{
								CalcSkillDamage(m_pOwnerRef, pTarget, m_pSkillDataRef, e, (float)m_pSkillDataRef->aSkill_Effect_Value[e], res->aSkillResult[i].effectResult[e].LP_EP_STEAL_fLpDamage, res->aSkillResult[i].byAttackResult, res->aSkillResult[i].damageByReflectingCurse, NULL);

								res->aSkillResult[i].effectResult[e].LP_EP_STEAL_fLpGained = (float)m_pSkillDataRef->aSkill_Effect_Value[e];
							}
							else if (m_pSkillDataRef->bySkill_Effect_Type[e] == SYSTEM_EFFECT_APPLY_TYPE_PERCENT) //used for dragons punishment(dark warrior skill)
							{
								float fTmpState = 0.f;

								if (m_pOwnerRef->IsPC() && m_pSkillDataRef->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
									fTmpState = (float)(m_pOwnerRef->GetCharAtt()->GetSubWeaponEnergyOffence() / 2.f) * (float)m_pSkillDataRef->aSkill_Effect_Value[e] / 100.f;
								else
									fTmpState = (float)(m_pOwnerRef->GetCharAtt()->GetLastEnergyOffence() / 2.f) * (float)m_pSkillDataRef->aSkill_Effect_Value[e] / 100.f;

								//res->aSkillResult[i].effectResult[e].LP_EP_STEAL_fLpDamage = 0.0f;
								res->aSkillResult[i].effectResult[e].LP_EP_STEAL_fLpGained = fTmpState;
							}
						}
						break;

						case ACTIVE_PULL:
						{
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
							res->aSkillResult[i].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL;

							pTarget->ChangeAggro(m_pOwnerRef->GetID(), DBO_AGGRO_CHANGE_TYPE_LOWEST, (DWORD)nAgro);

							nAgro = 0;
						}
						break;

						case ACTIVE_YELL:
						{
							if (Dbo_CheckProbabilityF((float)m_pSkillDataRef->aSkill_Effect_Value[e]))
							{
								if (Dbo_CheckProbability((int)pTarget->GetCharAtt()->GetLastConfuseToleranceRate()) == false)
								{
									//TODO
									//pTarget->GetBuffManager()->RegisterBuff(this, dwKeepTime, aBuffParameter, m_pOwnerRef->GetID(), BUFF_TYPE_CURSE);
								}
								else
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_RESISTED;
									goto RESULT2;
								}
							}
						}
						break;

						case ACTIVE_TAUNT:
						{
							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetLastMindCurseImmunity()))
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

							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetLastBodyCurseImmunity()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								goto RESULT2;
							}
							else if (!Dbo_CheckProbability((int)pTarget->GetCharAtt()->GetLastParalyzeToleranceRate()))
							{
								dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, DWORD((float)dwKeepTime * pTarget->GetCharAtt()->GetLastParalyzeKeepTimeDown() / 100.0f));
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
							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetLastMindCurseImmunity()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								goto RESULT2;
							}

							if (Dbo_CheckProbability((int)pTarget->GetCharAtt()->GetLastConfuseToleranceRate()) == false)
							{
								dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, (DWORD)((float)dwKeepTime * pTarget->GetCharAtt()->GetLastConfuseKeepTimeDown() / 100.0f));
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
							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetLastMindCurseImmunity()))
							{
								res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								goto RESULT2;
							}

							if (Dbo_CheckProbability((int)pTarget->GetCharAtt()->GetLastTerrorToleranceRate()) == false)
							{
								dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, (DWORD)((float)dwKeepTime * pTarget->GetCharAtt()->GetLastTerrorKeepTimeDown() / 100.0f));
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
							if (pTarget->IsMonster()) //some mobs have immunity against stun etc
							{
								if (BIT_FLAG_TEST(((CMonster*)pTarget)->GetTbldat()->dwImmunity_Bit_Flag, MAKE_BIT_FLAG(IMMUNITY_FLAG_STUN)))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
									goto RESULT2;
								}
							}
							//no break here
						}
						case ACTIVE_SLEEP:
						case ACTIVE_STONE:
						case ACTIVE_CANDY:
						case ACTIVE_TIED_UP:
						{
							if (effectCode == ACTIVE_STONE || effectCode == ACTIVE_CANDY)
							{
								if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetLastChangeCurseImmunity()))
								{
									res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_IMMUNE;
								}
								else
								{
									if (effectCode == ACTIVE_STONE)
									{
										if (Dbo_CheckProbability((int)pTarget->GetCharAtt()->GetLastStoneToleranceRate()) == false)
										{
											dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, (DWORD)((float)dwKeepTime * pTarget->GetCharAtt()->GetLastStoneKeepTimeDown() / 100.0f));
										}
										else
										{
											res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_RESISTED;
											goto RESULT2;
										}
									}
									else if (effectCode == ACTIVE_CANDY)
									{
										if (Dbo_CheckProbability((int)pTarget->GetCharAtt()->GetLastCandyToleranceRate()) == false)
										{
											dwKeepTime = UnsignedSafeDecrease<DWORD>(dwKeepTime, (DWORD)((float)dwKeepTime * pTarget->GetCharAtt()->GetLastCandyKeepTimeDown() / 100.0f));
										}
										else
										{
											res->aSkillResult[i].byAttackResult = BATTLE_ATTACK_RESULT_RESISTED;
											goto RESULT2;
										}
									}
								}
							}
						}
						break;

						case ACTIVE_FREEZE:
						{
							if (Dbo_CheckProbabilityF(pTarget->GetCharAtt()->GetLastBodyCurseImmunity()))
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

						default: //to avoid having a case for every effect we just use the default
						{
							aBuffParameter[e].buffParameter.fParameter = res->aSkillResult[i].effectResult[e].fResultValue = (float)m_pSkillDataRef->aSkill_Effect_Value[e];

							if(m_pSkillDataRef->bySkill_Active_Type == SKILL_ACTIVE_TYPE_DOT) //apply dot value change percent
								aBuffParameter[e].buffParameter.fParameter += aBuffParameter[e].buffParameter.fParameter * m_pOwnerRef->GetCharAtt()->GetLastDotValueChangePercent() / 100.0f;

							aBuffParameter[e].buffParameter.dwRemainValue = (DWORD)m_pSkillDataRef->aSkill_Effect_Value[e];
							aBuffParameter[e].buffParameter.dwRemainTime = dwKeepTime;
						}
						break;
						}

						if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_GUARD_CRASH && pTarget->GetCharStateID() == CHARSTATE_GUARD)
						{
							res->aSkillResult[i].effectResult[e].DD_DOT_fDamage += res->aSkillResult[i].effectResult[e].DD_DOT_fDamage * 2.5f; //increase by 250%

							if (m_pSkillDataRef->bySkill_Active_Type == SKILL_ACTIVE_TYPE_DOT)
							{
								aBuffParameter[e].buffParameter.fParameter = aBuffParameter[e].buffParameter.DOT_fDamage = res->aSkillResult[i].effectResult[e].DD_DOT_fDamage;
							}
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
						if (pCountAttack)
						{
							if (Dbo_CheckProbabilityF((float)pCountAttack->GetOriginalTableData()->aSkill_Effect_Value[0] / 10.0f)) // chance to use passive "counter attack"
							{
								pTarget->SendCharStateStanding();
								WORD wTempCounterAttackRes;
								HOBJECT ahTempCounterAttack = m_pOwnerRef->GetID();
								pCountAttack->UseSkill(INVALID_BYTE, m_pOwnerRef->GetID(), m_pOwnerRef->GetCurLoc(), 1, &ahTempCounterAttack, wTempCounterAttackRes);
							}
						}
					}

					if (res->bIsSkillHarmful && nAgro > 0)
					{
						nAgro += (int)m_pOwnerRef->GetCharAtt()->GetLastSkillAggroBonus();
						nAgro += (int)(nAgro * m_pOwnerRef->GetCharAtt()->GetLastSkillAggroBonusInPercent() / 100.f);

						pTarget->ChangeAggro(m_pOwnerRef->GetID(), DBO_AGGRO_CHANGE_TYPE_INCREASE, (DWORD)nAgro);
					}
				}

			RESULT3:

				res->bySkillResultCount += 1;
				res->aSkillResult[i].hTarget = pTarget->GetID();

				if (GetOriginalTableData()->bDash_Able)
				{
					m_pOwnerRef->SetDestLoc(pTarget->GetCurLoc());
					m_pOwnerRef->SetMoveDirection(NTL_MOVE_F);
					m_pOwnerRef->SetMoveFlag(NTL_MOVE_FLAG_RUN);
					m_pOwnerRef->SetMoveStatus(NTL_MOVE_STATUS_DASH);
				}

				if (bGuardCrashFail == false)
				{
					if (res->aSkillResult[i].byAttackResult != BATTLE_ATTACK_RESULT_DODGE && res->aSkillResult[i].byAttackResult != BATTLE_ATTACK_RESULT_BLOCK)
					{
						if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_KEEP_TIME_PLUS)
							dwKeepTime += DWORD(m_pSkillDataRef->afRpEffectValue[DBO_RP_BONUS_SLOT_KEEP_TIME_PLUS] * 1000.f);

						if (dwKeepTime > 0 && res->aSkillResult[i].byAttackResult != BATTLE_ATTACK_RESULT_RESISTED && res->aSkillResult[i].byAttackResult != BATTLE_ATTACK_RESULT_IMMUNE)
						{
							m_actionSkill.dwKeepTime[i] = dwKeepTime;
							memcpy(&m_actionSkill.aBuffParameter[i][NTL_SYSTEM_EFFECT_1], &aBuffParameter[NTL_SYSTEM_EFFECT_1], sizeof(aBuffParameter[NTL_SYSTEM_EFFECT_1]));
							memcpy(&m_actionSkill.aBuffParameter[i][NTL_SYSTEM_EFFECT_2], &aBuffParameter[NTL_SYSTEM_EFFECT_2], sizeof(aBuffParameter[NTL_SYSTEM_EFFECT_2]));

							m_actionSkill.bySkillResultBuffCount++;

							if (GetOriginalTableData()->bKeep_Effect)
								m_pOwnerRef->InsertKeepEffectTarget(pTarget->GetID(), GetSkillId());
						}
						//printf("res->aSkillResult[i].byAttackResult %u \n", res->aSkillResult[i].byAttackResult);
						memcpy(&m_actionSkill.aSkillResult[m_actionSkill.bySkillResultCount], &res->aSkillResult[i], sizeof(res->aSkillResult[i]));

						m_actionSkill.bySkillResultCount++;
					}
				}

			}

		LOOP_CONTINUE:
			continue;
		}

		m_actionSkill.bIsSkillHarmful = res->bIsSkillHarmful;

		if (res->wResultCode == GAME_SUCCESS)
		{
			if (GetOriginalTableData()->bDash_Able)	//if we dash, then it will be send before we send packet
				m_pOwnerRef->SendCharStateDash(m_pOwnerRef->GetDestLoc());

			//send packet.
			//NOTE: This packet must be send before sending charstate effect etc. Otherwise there will be issues with spin attacks etc
			m_pOwnerRef->Broadcast(&packet);

			//deduct ep
			if (GetOriginalTableData()->wRequire_EP > 0)
			{
				float fRequireEP = (float)GetOriginalTableData()->wRequire_EP;

				if (m_pOwnerRef->GetCharAtt()->GetRequiredEpChangePercent() != 0.0f)
					fRequireEP += fRequireEP * m_pOwnerRef->GetCharAtt()->GetRequiredEpChangePercent() / 100.0f;

				if (m_byUseRpBonusType == DBO_RP_BONUS_TYPE_EP_MINUS)
					fRequireEP -= fRequireEP * GetOriginalTableData()->afRpEffectValue[DBO_RP_BONUS_SLOT_EP_MINUS] / 100;

				//	printf("fRequireEP %f | Cur EP %u, Max EP %u \n", fRequireEP, cPlayer->GetCurEP(), cPlayer->GetLastMaxEP());
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
				//printf("fCoolDown %f, dwCoolTimeInMilliSecs %u, GetCoolTimeChangePercent %f\n", fCoolDown, GetOriginalTableData()->dwCoolTimeInMilliSecs, m_pOwnerRef->GetCharAtt()->GetCoolTimeChangePercent());
				SetCoolTimeRemaining((DWORD)fCoolDown);
			}

			//- If dont send in this order: keeping effect, add condition, send skill packet then fake death dont work correct
			//- If send packet and then keeping-effect then char fall like tree instead of nice animation
			//printf("GetOriginalTableData()->bKeep_Effect %u \n", GetOriginalTableData()->bKeep_Effect);
			if (GetOriginalTableData()->bKeep_Effect && m_actionSkill.bySkillResultBuffCount > 0)
			{
				m_pOwnerRef->SendCharStateKeepingEffect();

				OnAffected();
			}
			else if (GetOriginalTableData()->bDash_Able)
			{
				OnAffected();
			}
			else if(GetOriginalTableData()->bKeep_Effect == false)
			{
				m_pOwnerRef->SendCharStateSkillAffecting(GetSkillId(), m_hUseAppointedTarget);
			}
		}
		else
		{
			m_pOwnerRef->SendPacket(&packet);

			//init skill
			InitSkillUse();
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
	//NTL_PRINT(PRINT_APP,"CSkill::OnAffected() %u", m_actionSkill.bySkillResultCount);
	int nTotalReflect = 0;
	for (int i = 0; i < m_actionSkill.bySkillResultCount; i++)
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(m_actionSkill.aSkillResult[i].hTarget);
		if (pTarget && pTarget->IsInitialized())
		{
			if (m_actionSkill.bIsSkillHarmful && !m_pOwnerRef->IsAttackable(pTarget))
				continue;
			if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_NULLIFIED_DAMAGE))
				continue;

			nTotalReflect += m_actionSkill.aSkillResult[i].damageByReflectingCurse;

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

			if (m_actionSkill.bIsSkillHarmful)
			{
				if (m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].eResultType == DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT)
				{
					eSYSTEM_EFFECT_CODE effectCode = GetEffectCode(NTL_SYSTEM_EFFECT_1);
					if(effectCode != ACTIVE_BLEED && effectCode != ACTIVE_POISON && effectCode != ACTIVE_STOMACHACHE && effectCode != ACTIVE_BURN)
						fDmg += m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].DD_DOT_fDamage;
				}

				if (m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].eResultType == DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT)
				{
					eSYSTEM_EFFECT_CODE effectCode = GetEffectCode(NTL_SYSTEM_EFFECT_2);
					if (effectCode != ACTIVE_BLEED && effectCode != ACTIVE_POISON && effectCode != ACTIVE_STOMACHACHE && effectCode != ACTIVE_BURN)
						fDmg += m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].DD_DOT_fDamage;
				}

				//printf("fDmg %f bySkill_Active_Type %u \n", fDmg, GetOriginalTableData()->bySkill_Active_Type);
				if (fDmg > 0.0f && pTarget->OnSkillAction(m_pOwnerRef, (int)fDmg, (DWORD)fDmg + GetOriginalTableData()->dwAdditional_Aggro_Point, m_actionSkill.aSkillResult[i].byAttackResult) == false) //deal dmg and check if not dead
				{
					//<pTarget> heal on hit
					printf("bIslpRecoveredWhenHit %i \n", m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].DD_DOT_lpEpRecovered.bIsLpRecoveredWhenHit);
					printf("%u %u\n",
						m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].DD_DOT_lpEpRecovered.targetLpRecoveredWhenHit, m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].DD_DOT_lpEpRecovered.targetLpRecoveredWhenHit);

					if (m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].DD_DOT_lpEpRecovered.bIsLpRecoveredWhenHit || m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].DD_DOT_lpEpRecovered.bIsLpRecoveredWhenHit)
						pTarget->UpdateCurLP(m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].DD_DOT_lpEpRecovered.targetLpRecoveredWhenHit + m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].DD_DOT_lpEpRecovered.targetLpRecoveredWhenHit, true, false);
					
					printf("bIsEpRecoveredWhenHit %i \n", m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].DD_DOT_lpEpRecovered.bIsEpRecoveredWhenHit);
					printf("%u %u\n", 
						m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].DD_DOT_lpEpRecovered.dwTargetEpRecoveredWhenHit, m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].DD_DOT_lpEpRecovered.dwTargetEpRecoveredWhenHit);

					if (m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].DD_DOT_lpEpRecovered.bIsEpRecoveredWhenHit || m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].DD_DOT_lpEpRecovered.bIsEpRecoveredWhenHit)
						pTarget->UpdateCurEP(WORD(m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_1].DD_DOT_lpEpRecovered.dwTargetEpRecoveredWhenHit + m_actionSkill.aSkillResult[i].effectResult[NTL_SYSTEM_EFFECT_2].DD_DOT_lpEpRecovered.dwTargetEpRecoveredWhenHit), true, false);
				}
			}

			//add buffs last or it might happen that buff like sleep will be removed at OnSkillAction

			//check if we have to add a buff
			//	printf("m_actionSkill.dwKeepTime[i] %u m_actionSkill.aSkillResult[i].byAttackResult %u\n", m_actionSkill.dwKeepTime[i], m_actionSkill.aSkillResult[i].byAttackResult);
			if (m_actionSkill.dwKeepTime[i] > 0 &&m_actionSkill.aSkillResult[i].byAttackResult != BATTLE_ATTACK_RESULT_RESISTED && m_actionSkill.aSkillResult[i].byAttackResult != BATTLE_ATTACK_RESULT_IMMUNE)
			{
				if (IsForBlessBuff())
				{
					pTarget->GetBuffManager()->RegisterBuff(m_actionSkill.dwKeepTime[i], m_aeEffectCode, m_actionSkill.aBuffParameter[i], m_pOwnerRef->GetID(), BUFF_TYPE_BLESS, GetOriginalTableData());
				}
				else if (IsForCurseBuff())
				{
					pTarget->GetBuffManager()->RegisterBuff(m_actionSkill.dwKeepTime[i], m_aeEffectCode, m_actionSkill.aBuffParameter[i], m_pOwnerRef->GetID(), BUFF_TYPE_CURSE, GetOriginalTableData());
				}
			}
		}
	}

	//do it outside loop. Otherwise if the target is dead and no other target exist, this will never be called.
	for (int e = 0; e < NTL_MAX_EFFECT_IN_SKILL; e++)
	{
		switch (GetEffectCode(e))
		{
			case ACTIVE_WARP_BACK_ATTACK:
			case ACTIVE_THROUGH_ATTACK:
			{
				m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_NULLIFIED_DAMAGE, NULL, false);
			}
			break;
			default: break;
		}
	}

	//deal reflect damage
	if (nTotalReflect > 0)
	{
		m_pOwnerRef->OnAttackAction(NULL, nTotalReflect, BATTLE_ATTACK_RESULT_REFLECTED_DAMAGE);
	}

	// Check if has curse which damage me when using skill
	if (CBuff* pCurseBuff = m_pOwnerRef->GetBuffManager()->FindAnyBuff(ACTIVE_SKILL_LP_DAMAGE))
	{
		float fCurseDmg = pCurseBuff->GetSystemEffectParameter(ACTIVE_SKILL_LP_DAMAGE);

		m_pOwnerRef->OnBuffDamage(pCurseBuff->GetCasterHandle(), fCurseDmg);

		m_pOwnerRef->SendEffectAffected(g_pTableContainer->GetSystemEffectTable()->GetEffectTblidx(ACTIVE_SKILL_LP_DAMAGE), pCurseBuff->GetSourceType(), pCurseBuff->GetSourceTblidx(),
			fCurseDmg, INVALID_FLOAT, pCurseBuff->GetCasterHandle());
	}

	if(m_pOwnerRef->IsPC())
		((CPlayer*)m_pOwnerRef)->SetNextAttackTime();
	
	if (BIT_FLAG_TEST(m_pSkillDataRef->dwFunction_Bit_Flag, SKILL_FUNCTION_FLAG_FAINTING_AFTER_APPLYING) && m_pOwnerRef->IsFainting() == false)
	{
		m_pOwnerRef->Faint(NULL, FAINT_REASON_SELF_FAINTING);
	}

	//init skill
	InitSkillUse();
}

void CSkill::OnAffectedCanceled()
{
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
			case ACTIVE_WARP_BACK_ATTACK:
			case ACTIVE_THROUGH_ATTACK:
			{
				m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_NULLIFIED_DAMAGE, NULL, false);
			}
			break;
			default: break;
		}
	}

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
	memset(&m_actionSkill, NULL, sizeof(m_actionSkill));

	m_hUseAppointedTarget = INVALID_HOBJECT;
	m_byUseRpBonusType = DBO_RP_BONUS_TYPE_INVALID;
	m_byApplyTargetCount = 0;
	memset(m_ahApplyTarget, INVALID_HOBJECT, sizeof(m_ahApplyTarget));
	m_vFinalSubjectLoc.Reset();
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
				//if (m_pOwnerRef->IsPC())
				//	NTL_PRINT(PRINT_APP, "OnAffected | %f %f %f, state %u", m_pOwnerRef->GetCurLoc().x, m_pOwnerRef->GetCurLoc().y, m_pOwnerRef->GetCurLoc().z, m_pOwnerRef->GetCharStateID());
				OnAffected();
			}

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

