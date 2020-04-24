#include "stdafx.h"
#include "SkillPc.h"
#include "CPlayer.h"
#include "Npc.h"
#include "SpellAreaChecker.h"
#include "NtlPacketGU.h"
#include "NtlPacketGQ.h"
#include "NtlResultCode.h"
#include "GameServer.h"
#include "NtlNavi.h"



CSkillPc::CSkillPc()
{
	Init();
}

CSkillPc::~CSkillPc()
{
	Destroy();
}


bool CSkillPc::Create(sSKILL_TBLDAT *pSkillDataRef, CPlayer *pOwnerRef, BYTE skillIndex, DWORD dwTimeRemaining, int nExp, BYTE byRpBonusType, bool bIsRpBonusAuto)
{
	if (CSkill::Create(pSkillDataRef, pOwnerRef, skillIndex))
	{
		SetCoolTimeRemaining(dwTimeRemaining);
		m_nExp = nExp;
		m_byRpBonusType = byRpBonusType;
		m_bIsRpBonusAuto = bIsRpBonusAuto;
		m_pPlayerRef = pOwnerRef;

		return true;
	}

	Destroy();
	return false;
}


void CSkillPc::Destroy()
{
	m_pPlayerRef = NULL;
}


void CSkillPc::Init()
{
	m_nExp = 0;
	m_byRpBonusType = DBO_RP_BONUS_TYPE_INVALID;
	m_bIsRpBonusAuto = false;
	m_pPlayerRef = NULL;
}


void CSkillPc::TickProcess(DWORD dwTickDiff)
{
	CSkill::TickProcess(dwTickDiff);
}


bool CSkillPc::UseSkill(BYTE byRpBonusType, HOBJECT hAppointTargetHandle, CNtlVector & rvFinalSubjectLoc, CNtlVector& rvFinalLoc, BYTE byApplyTargetCount, HOBJECT * ahApplyTarget, WORD & rwResultCode)
{
	if (CSkill::UseSkill(byRpBonusType, hAppointTargetHandle, rvFinalSubjectLoc, rvFinalLoc, byApplyTargetCount, ahApplyTarget, rwResultCode))
	{
		if (BIT_FLAG_TEST(GetOriginalTableData()->dwFunction_Bit_Flag, SKILL_FUNCTION_FLAG_STOP_AUTO_ATTACK))
		{
			m_pPlayerRef->ChangeAttackProgress(false);
			m_pPlayerRef->SetFightMode(false);
			m_pPlayerRef->SetAttackTarget(INVALID_HOBJECT);
		}

		if (byRpBonusType != DBO_RP_BONUS_TYPE_INVALID)
			return m_pOwnerRef->SendCharStateFocusing();
		else
		{
			return CheckCasting();
		}
	}

	return false;
}



void CSkillPc::CastSkill(HOBJECT hAppointTargetHandle, BYTE byApplyTargetCount, HOBJECT* ahApplyTarget, bool bSetData/* = true*/)
{
	CSpellAreaChecker rSpellAreaChecker;
	rSpellAreaChecker.Create();

	CCharacter *pAppointTarget = (hAppointTargetHandle == INVALID_HOBJECT) ? m_pPlayerRef : g_pObjectManager->GetChar(hAppointTargetHandle);
	if (GetOriginalTableData()->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_SELF)
		pAppointTarget = m_pPlayerRef;

	if (pAppointTarget == NULL)
		return;

	rSpellAreaChecker.PrepareForSelection(m_pOwnerRef, pAppointTarget, GetOriginalTableData()->byApply_Range, GetOriginalTableData()->byApply_Area_Size_1, GetOriginalTableData()->byApply_Area_Size_2);

	CGameServer* app = (CGameServer*)g_pApp;

	sSKILL_TARGET_LIST targetList;
	
	for (BYTE i = 0; i < byApplyTargetCount; i++)
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(ahApplyTarget[i]);
		if (!pTarget || !pTarget->IsInitialized() || pTarget->GetCurWorld() == NULL)
			continue;
		//printf("GetOriginalTableData()->byApply_Target %u \n", GetOriginalTableData()->byApply_Target);
		switch (GetOriginalTableData()->byApply_Target)
		{
		case DBO_SKILL_APPLY_TARGET_SELF:
		{
			if (rSpellAreaChecker.IsObjectInApplyRange(pTarget, NULL))
				targetList.AddTarget(ahApplyTarget[i]);
		}
		break;
		case DBO_SKILL_APPLY_TARGET_ENEMY:
		{
			if (rSpellAreaChecker.IsObjectInApplyRange(pTarget, NULL))
			{
				if (pTarget->IsMonster())
				{
					targetList.AddTarget(ahApplyTarget[i]);
				}
				else if (pTarget->IsPC())
				{
					BYTE byWorldRuleType = m_pPlayerRef->GetCurWorld()->GetTbldat()->byWorldRuleType;
					CPlayer* pTargetPc = (CPlayer*)pTarget;

					if (byWorldRuleType == GAMERULE_NORMAL)
					{
						//check if both players are in platform
						if (m_pPlayerRef->IsPvpZone() && pTargetPc->IsPvpZone())
						{
							targetList.AddTarget(ahApplyTarget[i]);
							continue;
						}
						//check if both players are in freebattle
						if (m_pPlayerRef->GetFreeBattleID() != INVALID_DWORD && m_pPlayerRef->GetFreeBattleID() == pTargetPc->GetFreeBattleID())
						{
							targetList.AddTarget(ahApplyTarget[i]);
							continue;
						}

						if (app->GetGsChannel() == 0) // only in scramble channel
						{
							if (m_pPlayerRef->GetDragonballScramble() && pTargetPc->GetDragonballScramble())
							{
								if (GetNaviEngine()->IsBasicAttributeSet(m_pPlayerRef->GetCurWorld()->GetNaviInstanceHandle(), m_pPlayerRef->GetCurLoc().x, m_pPlayerRef->GetCurLoc().z, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE) == false
									&& GetNaviEngine()->IsBasicAttributeSet(pTargetPc->GetCurWorld()->GetNaviInstanceHandle(), pTargetPc->GetCurLoc().x, pTargetPc->GetCurLoc().z, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE) == false)
								{
									targetList.AddTarget(ahApplyTarget[i]);
									continue;
								}
							}
						}
					}
					else if (byWorldRuleType == GAMERULE_RANKBATTLE)
					{
						if (m_pPlayerRef->GetRankBattleData()->eTeamType == pTargetPc->GetRankBattleData()->eTeamType)
							continue;
					}
					else if (byWorldRuleType == GAMERULE_DOJO)
					{
						if (m_pPlayerRef->GetGuildID() == pTargetPc->GetGuildID()) //if in same guild, then add to targets
							continue;
					}
					else if (byWorldRuleType == GAMERULE_MINORMATCH || byWorldRuleType == GAMERULE_MAJORMATCH || byWorldRuleType == GAMERULE_FINALMATCH)
					{
						if (m_pPlayerRef->GetBudokaiTeamType() == pTargetPc->GetBudokaiTeamType())
							continue;
					}

					targetList.AddTarget(ahApplyTarget[i]);
				}
				else if (pTarget->IsNPC())
				{
					if (((CNpc*)pTarget)->GetPcRelation() == RELATION_TYPE_ENEMY)
						targetList.AddTarget(ahApplyTarget[i]);
				}
				else if(pTarget->IsSummonPet() && pTarget->GetID() != m_pPlayerRef->GetCurrentPetId())
					targetList.AddTarget(ahApplyTarget[i]);
			}
		}
		break;

		case DBO_SKILL_APPLY_TARGET_ALLIANCE: //skills like direct heal, revive, aoe buff "all allies within X meter"
		{
			if (rSpellAreaChecker.IsObjectInApplyRange(pTarget, NULL))
			{
				if (pTarget->IsPC() || pTarget->IsSummonPet() || (pTarget->IsNPC() && ((CNpc*)pTarget)->GetPcRelation() != RELATION_TYPE_ENEMY))
				{
					if (pTarget->IsPC() == false)
						targetList.AddTarget(ahApplyTarget[i]);
					else if (pTarget->IsPC())
					{
						if (m_pPlayerRef->GetID() == ahApplyTarget[i])
						{
							targetList.AddTarget(ahApplyTarget[i]);
							continue;
						}

						BYTE byWorldRuleType = m_pPlayerRef->GetCurWorld()->GetTbldat()->byWorldRuleType;
						CPlayer* pTargetPc = (CPlayer*)pTarget;

						if (byWorldRuleType == GAMERULE_NORMAL)
						{
							//dont allow to buff each others when they are not both inside/outside platform
							if (m_pPlayerRef->IsPvpZone() != pTargetPc->IsPvpZone())
								continue;

							//check if bot inside platform
							if (m_pPlayerRef->IsPvpZone() == true && pTargetPc->IsPvpZone() == true)
							{
								//only allow to use skill if both are in the same party
								if(m_pPlayerRef->GetPartyID() != INVALID_PARTYID && m_pPlayerRef->GetPartyID() != pTargetPc->GetPartyID())
									continue;
							}

							//do not allow to buff when target is in free battle
							if (pTargetPc->GetFreeBattleID() != INVALID_DWORD)
							{
								continue;
							}

							if (app->GetGsChannel() == 0) // only in scramble channel
							{
								//dont allow to use skill on when both are not inside/outside db-scramble
								if (m_pPlayerRef->GetDragonballScramble() != pTargetPc->GetDragonballScramble())
									continue;

								if (m_pPlayerRef->GetDragonballScramble() == true && pTargetPc->GetDragonballScramble() == true)
								{
									if (m_pPlayerRef->GetPartyID() != pTargetPc->GetPartyID() && m_pPlayerRef->GetGuildID() != pTargetPc->GetGuildID()) //if in same guild, then add to targets
										continue;
								}
							}
						}
						else if (byWorldRuleType == GAMERULE_RANKBATTLE)
						{
							if (m_pPlayerRef->GetRankBattleData()->eTeamType != pTargetPc->GetRankBattleData()->eTeamType)
								continue;
						}
						else if (byWorldRuleType == GAMERULE_DOJO)
						{
							if (m_pPlayerRef->GetGuildID() != pTargetPc->GetGuildID()) //if in same guild, then add to targets
								continue;
						}
						else if (byWorldRuleType == GAMERULE_MINORMATCH || byWorldRuleType == GAMERULE_MAJORMATCH || byWorldRuleType == GAMERULE_FINALMATCH)
						{
							if (m_pPlayerRef->GetBudokaiTeamType() != pTargetPc->GetBudokaiTeamType())
								continue;
						}

						targetList.AddTarget(ahApplyTarget[i]);
					}
				}
			}
		}
		break;

		case DBO_SKILL_APPLY_TARGET_PARTY:
		{
			if (rSpellAreaChecker.IsObjectInApplyRange(pTarget, NULL))
			{
				if (m_pPlayerRef->GetID() == pTarget->GetID()
					|| (m_pPlayerRef->GetPartyID() != INVALID_PARTYID && m_pPlayerRef->GetPartyID() == pTarget->GetPartyID())
					|| (pTarget->IsSummonPet() && pTarget->GetID() == m_pPlayerRef->GetCurrentPetId())
					)
				{
					if (pTarget->IsPC() == false)
						targetList.AddTarget(ahApplyTarget[i]);
					else if (pTarget->IsPC())
					{
						if (m_pPlayerRef->GetID() == ahApplyTarget[i])
						{
							targetList.AddTarget(ahApplyTarget[i]);
							continue;
						}

						BYTE byWorldRuleType = m_pPlayerRef->GetCurWorld()->GetTbldat()->byWorldRuleType;
						CPlayer* pTargetPc = (CPlayer*)pTarget;

						if (byWorldRuleType == GAMERULE_NORMAL)
						{
							//dont allow to buff each others when they are not both inside/outside platform
							if (m_pPlayerRef->IsPvpZone() != pTargetPc->IsPvpZone())
								continue;

							//do not allow to buff when target is in free battle
							if (pTargetPc->GetFreeBattleID() != INVALID_DWORD)
							{
								continue;
							}

							if (app->GetGsChannel() == 0) // only in scramble channel
							{
								//dont allow to use skill on when both are not inside/outside db-scramble
								if (m_pPlayerRef->GetDragonballScramble() != pTargetPc->GetDragonballScramble())
									continue;
							}
						}
						else if (byWorldRuleType == GAMERULE_RANKBATTLE)
						{
							if (m_pPlayerRef->GetRankBattleData()->eTeamType != pTargetPc->GetRankBattleData()->eTeamType)
								continue;
						}
						else if (byWorldRuleType == GAMERULE_DOJO)
						{
							if (m_pPlayerRef->GetGuildID() != pTargetPc->GetGuildID()) //if in same guild, then add to targets
								continue;
						}
						else if (byWorldRuleType == GAMERULE_MINORMATCH || byWorldRuleType == GAMERULE_MAJORMATCH || byWorldRuleType == GAMERULE_FINALMATCH)
						{
							if (m_pPlayerRef->GetBudokaiTeamType() != pTargetPc->GetBudokaiTeamType())
								continue;
						}

						targetList.AddTarget(ahApplyTarget[i]);
					}
				}
			}
		}
		break;

		case DBO_SKILL_APPLY_TARGET_MOB_PARTY:
		{
			ERR_LOG(LOG_SYSTEM, "DBO_SKILL_APPLY_TARGET_MOB_PARTY dont know. Skill id %u", GetSkillId());
		}
		break;

		case DBO_SKILL_APPLY_TARGET_ANY:
		{
			if (rSpellAreaChecker.IsObjectInApplyRange(pTarget, NULL))
			{
				if (pTarget->IsPC() == false)
					targetList.AddTarget(ahApplyTarget[i]);
				else if (pTarget->IsPC())
				{
					if (m_pPlayerRef->GetID() == ahApplyTarget[i])
					{
						targetList.AddTarget(ahApplyTarget[i]);
						continue;
					}

					BYTE byWorldRuleType = m_pPlayerRef->GetCurWorld()->GetTbldat()->byWorldRuleType;
					CPlayer* pTargetPc = (CPlayer*)pTarget;

					if (byWorldRuleType == GAMERULE_NORMAL)
					{
						//dont allow to buff each others when they are not both inside/outside platform
						if (m_pPlayerRef->IsPvpZone() != pTargetPc->IsPvpZone())
							continue;

						//do not allow to buff when target is in free battle
						if (pTargetPc->GetFreeBattleID() != m_pPlayerRef->GetFreeBattleID())
						{
							continue;
						}

						if (app->GetGsChannel() == 0) // only in scramble channel
						{
							//dont allow to use skill on when both are not inside/outside db-scramble
							if (m_pPlayerRef->GetDragonballScramble() != pTargetPc->GetDragonballScramble())
								continue;
						}
					}

					targetList.AddTarget(ahApplyTarget[i]);
				}
			}
		}
		break;

		case DBO_SKILL_APPLY_TARGET_SUMMON:
		{
			if (pTarget->IsSummonPet())
				targetList.AddTarget(ahApplyTarget[i]);
		}
		break;

		case DBO_SKILL_APPLY_TARGET_ANY_NPC:
		{
			if (rSpellAreaChecker.IsObjectInApplyRange(pTarget, NULL))
				if (pTarget->IsNPC())
					targetList.AddTarget(ahApplyTarget[i]);
		}
		break;

		case DBO_SKILL_APPLY_TARGET_ANY_MOB:
		{
			if (rSpellAreaChecker.IsObjectInApplyRange(pTarget, NULL))
				if (pTarget->IsMonster())
					targetList.AddTarget(ahApplyTarget[i]);
		}
		break;

		case DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE:
		{
			ERR_LOG(LOG_SYSTEM, "DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE dont know. Skill id %u", GetSkillId());
		}
		break;

		default:
		{
			ERR_LOG(LOG_SYSTEM, "fail : switch( m_pSkill->GetOriginalTableData()->byApply_Target(%u) )", GetOriginalTableData()->byApply_Target);
		}
		break;
		}
	}
	
	if (Dbo_IsHarmfulEffectType(m_pSkillDataRef->bySkill_Active_Type))
	{
		m_pPlayerRef->UpdateBattleCombatMode(true); //Start/Reset combat event
	}

	CSkill::CastSkill(hAppointTargetHandle, targetList.byTargetCount, targetList.ahTarget);
}


void CSkillPc::OnLearnSkill()
{
	CNtlPacket packet(sizeof(sGU_SKILL_LEARNED_NFY));
	sGU_SKILL_LEARNED_NFY * res = (sGU_SKILL_LEARNED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_SKILL_LEARNED_NFY;
	res->skillId = GetSkillId();
	res->skillIndex = GetSkillIndex();
	res->bySkillFrom = 0;
	packet.SetPacketLen(sizeof(sGU_SKILL_LEARNED_NFY));
	m_pPlayerRef->SendPacket(&packet);

	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket pQry(sizeof(sGQ_SKILL_ADD_REQ));
	sGQ_SKILL_ADD_REQ* rQry = (sGQ_SKILL_ADD_REQ*)pQry.GetPacketData();
	rQry->wOpCode = GQ_SKILL_ADD_REQ;
	rQry->charId = m_pPlayerRef->GetCharID();
	rQry->bySlot = GetSkillIndex();
	rQry->skillId = GetSkillId();
	rQry->dwSP = m_pPlayerRef->GetSkillPoints(); // required skill points must be deducted before calling this function
	pQry.SetPacketLen(sizeof(sGQ_SKILL_ADD_REQ));
	g_pApp->SendTo(app->GetQueryServerSession(), &pQry);

	if (GetOriginalTableData()->bySkill_Class == NTL_SKILL_CLASS_PASSIVE)
		m_pPlayerRef->GetCharAtt()->CalculateAll();
}


void CSkillPc::UpdateRpSetting(BYTE byRpBonusType, bool bIsRpBonusAuto, WORD & rwResultCode)
{
	rwResultCode = GAME_SKILL_CANT_SET_THAT_RP_BONUS_IN_SKILL;

	if (bIsRpBonusAuto)
	{
		if (byRpBonusType == DBO_RP_BONUS_TYPE_INVALID)
		{
			rwResultCode = GAME_SUCCESS;
			m_byRpBonusType = byRpBonusType;
			m_bIsRpBonusAuto = bIsRpBonusAuto;
		}
		else
		{
			for (int i = 0; i < DBO_MAX_RP_BONUS_COUNT_PER_SKILL; i++)
			{
				if (GetOriginalTableData()->abyRpEffect[i] == byRpBonusType)
				{
					rwResultCode = GAME_SUCCESS;
					m_byRpBonusType = byRpBonusType;
					m_bIsRpBonusAuto = bIsRpBonusAuto;

					break;
				}
			}
		}
	}
	else
	{
		rwResultCode = GAME_SUCCESS;
		m_byRpBonusType = DBO_RP_BONUS_TYPE_INVALID;
		m_bIsRpBonusAuto = bIsRpBonusAuto;
	}
}


bool CSkillPc::UpgradeSkill(WORD & rwResultCode, sSKILL_TBLDAT* pNewSkillTbldat)
{
	if (pNewSkillTbldat)
	{
		if (GetOwner()->GetLevel() >= pNewSkillTbldat->byRequire_Train_Level)
		{
			if (Dbo_CheckClass(m_pPlayerRef->GetClass(), pNewSkillTbldat->dwPC_Class_Bit_Flag) == true) //check class
			{
				if (m_pPlayerRef->GetSkillPoints() >= pNewSkillTbldat->wRequireSP)
				{
					rwResultCode = GAME_SUCCESS;

					ChangeOriginalTableData(pNewSkillTbldat);

					m_pPlayerRef->UpdateCharSP(m_pPlayerRef->GetSkillPoints() - pNewSkillTbldat->wRequireSP);

					if (pNewSkillTbldat->bySkill_Class == NTL_SKILL_CLASS_PASSIVE) //if passive skill then add new stats - old stats
						m_pPlayerRef->GetCharAtt()->CalculateAll();

					return true;
				}
				else rwResultCode = GAME_SKILL_NOT_ENOUGH_SP_POINT;
			}
			else rwResultCode = GAME_SKILL_TRIED_TO_LEARN_WRONG_CLASS_SKILL;
		}
		else rwResultCode = GAME_SKILL_TOO_HIGH_LEVEL_TO_TRAIN;
	}
	else rwResultCode = GAME_SKILL_TOO_HIGH_LEVEL_TO_TRAIN;

	return false;
}


bool CSkillPc::CanResetSkill()
{
	sSKILL_TBLDAT* pSkillTbldat = GetOriginalTableData();

	if (pSkillTbldat->bySkill_Grade == 1)
	{
		if (GetSkillIndex() == 0 || GetSkillIndex() == 1 || GetSkillIndex() == 2 || pSkillTbldat->byPC_Class_Change != INVALID_BYTE) //dont allow to reset first 3 skills and master class passive when at lv 1
		{
		//	printf("GetSkillIndex() %u pSkillTbldat->byPC_Class_Change %i\n", GetSkillIndex(), pSkillTbldat->byPC_Class_Change);
			return false;
		}
	}

	return true;
}


int CSkillPc::CopyToData(sSKILL_DATA * pSkillData)
{
	if (pSkillData)
	{
		pSkillData->skillIndex = GetSkillIndex();
		pSkillData->skillId = GetSkillId();
		pSkillData->nRemainSec = GetCoolTimeRemaining();
		pSkillData->nExp = m_nExp;
		pSkillData->byRpBonusType = m_byRpBonusType;
		pSkillData->bIsRpBonusAuto = m_bIsRpBonusAuto;

		return sizeof(sSKILL_DATA);
	}

	return 0;
}


int CSkillPc::CopyToInfo(sSKILL_INFO * pSkillInfo)
{
	if (pSkillInfo)
	{
		pSkillInfo->tblidx = GetOriginalTableData()->tblidx;
		pSkillInfo->dwTimeRemaining = GetCoolTimeRemaining();
		pSkillInfo->nExp = m_nExp;
		pSkillInfo->skillIndex = GetSkillIndex();
		pSkillInfo->byRpBonusType = m_byRpBonusType;
		pSkillInfo->bIsRpBonusAuto = m_bIsRpBonusAuto;
		return sizeof(sSKILL_INFO);
	}

	return 0;
}













