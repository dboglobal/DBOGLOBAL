#include "stdafx.h"
#include "CPlayer.h"
#include "calcs.h"
#include "NtlPacketGU.h"
#include "NtlPacketGQ.h"
#include "NtlResultCode.h"
#include "TableContainerManager.h"
#include "GameServer.h"
#include "HTBSetTable.h"


CSkillManagerPc::CSkillManagerPc()
{
	Init();
}

CSkillManagerPc::~CSkillManagerPc()
{
	Destroy();
}


bool CSkillManagerPc::Create(CPlayer * pOwnerRef, DWORD dwMaxNumberOfSkill)
{
	if (CSkillManager::Create(pOwnerRef, dwMaxNumberOfSkill))
	{
		m_pPlayerRef = pOwnerRef;
		return true;
	}

	return false;
}


void CSkillManagerPc::Destroy()
{
	m_bCastingFinished = false;
	CSkillManager::Destroy();
	m_pPlayerRef = NULL;
}


void CSkillManagerPc::Init()
{
	m_bCastingFinished = false;
	m_pPlayerRef = NULL;
}


void CSkillManagerPc::LearnSkill(TBLIDX skillTblidx, WORD & rwResultCode, bool bRequireSP/* = true*/)
{
	rwResultCode = GAME_FAIL;

	if (GetNumberOfSkill() < GetMaxNumberOfSkill())
	{
		sSKILL_TBLDAT* pSkillData = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(skillTblidx);
		if (pSkillData)
		{
			if (!bRequireSP || (bRequireSP && pSkillData->bSelfTrain)) //only do self-train check when skill point required (require skill point = player train skill by himself) <ANTI-CHEAT ENGINE HACK TO LEARN PASSIVE>
			{
				if (!HasBasicSkillLearned(pSkillData->tblidx)) //check if current skill already learned
				{
					if (m_pPlayerRef->GetLevel() >= pSkillData->byRequire_Train_Level) //check required lvl
					{
						if (Dbo_CheckClass(m_pPlayerRef->GetClass(), pSkillData->dwPC_Class_Bit_Flag) == true) //check class
						{
							if (!bRequireSP || (HasRequiredSkillLearned(pSkillData)))//check if required skill already learned /Only when skill point required (require skill point = player train skill by himself) <ANTI-CHEAT ENGINE HACK TO LEARN SKILLS>
							{
								if (!bRequireSP || (m_pPlayerRef->GetSkillPoints() >= pSkillData->wRequireSP && bRequireSP))
								{
									rwResultCode = GAME_SUCCESS;

									CSkillPc* pSkill = new CSkillPc;

									if (pSkill->Create(pSkillData, m_pPlayerRef, INVALID_BYTE, 0, 0, DBO_RP_BONUS_TYPE_INVALID, false))
									{
										AddSkill(pSkill);
									}
									else
										ERR_LOG(LOG_GENERAL, "fail : skill create fail. Player[%u] skill tblidx[%u]", m_pPlayerRef->GetCharID(), pSkillData->tblidx);

									if (bRequireSP)
										m_pPlayerRef->UpdateCharSP(m_pPlayerRef->GetSkillPoints() - pSkillData->wRequireSP);

									pSkill->OnLearnSkill();
								}
								else rwResultCode = GAME_SKILL_NOT_ENOUGH_SP_POINT;
							}
							else rwResultCode = GAME_SKILL_NO_PREREQUISITE_SKILLS_YOU_HAVE;
						}
						else rwResultCode = GAME_SKILL_TRIED_TO_LEARN_WRONG_CLASS_SKILL;
					}
					else rwResultCode = GAME_SKILL_TOO_HIGH_LEVEL_TO_TRAIN;
				}
				else rwResultCode = GAME_SKILL_ALREADY_MASTERED_SKILL;
			}
			else rwResultCode = GAME_SKILL_CANT_BE_GAINED_BY_YOURSELF;
		}
		else rwResultCode = GAME_SKILL_NOT_EXISTING_SKILL;
	}
	else rwResultCode = GAME_SKILL_NO_EMPTY_SKILL_SLOT;
}


void CSkillManagerPc::UpdateRpSetting(BYTE skillIndex, BYTE byRpBonusType, bool bIsRpBonusAuto)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGU_SKILL_RP_BONUS_SETTING_RES));
	sGU_SKILL_RP_BONUS_SETTING_RES * res = (sGU_SKILL_RP_BONUS_SETTING_RES *)packet.GetPacketData();
	res->wOpCode = GU_SKILL_RP_BONUS_SETTING_RES;
	res->bIsRpBonusAuto = bIsRpBonusAuto;
	res->byRpBonusType = byRpBonusType;
	res->skillIndex = skillIndex;

	CSkillPc* pSkill = (CSkillPc*)GetSkillWithSkillIndex(skillIndex);
	if (pSkill)
	{
		pSkill->UpdateRpSetting(byRpBonusType, bIsRpBonusAuto, res->wResultCode);
		res->skillId = pSkill->GetSkillId();

		if (res->wResultCode == GAME_SUCCESS)
		{
			CNtlPacket pQry(sizeof(sGQ_SKILL_UPDATE_REQ));
			sGQ_SKILL_UPDATE_REQ * rQry = (sGQ_SKILL_UPDATE_REQ *)pQry.GetPacketData();
			rQry->wOpCode = GQ_SKILL_UPDATE_REQ;
			rQry->charId = m_pPlayerRef->GetCharID();
			rQry->handle = m_pPlayerRef->GetID();
			rQry->sSkill.bIsRpBonusAuto = bIsRpBonusAuto;
			rQry->sSkill.byRpBonusType = byRpBonusType;
			rQry->sSkill.skillIndex = skillIndex;
			rQry->sSkill.nRemainSec = (int)pSkill->GetCoolTimeRemaining();
			rQry->sSkill.skillId = pSkill->GetSkillId();
			packet.SetPacketLen(sizeof(sGQ_SKILL_UPDATE_REQ));
			app->SendTo(app->GetQueryServerSession(), &pQry);
		}
	}
	else
		res->wResultCode = GAME_SKILL_YOU_DONT_HAVE_THE_SKILL;


	packet.SetPacketLen(sizeof(sGU_SKILL_RP_BONUS_SETTING_RES));
	m_pPlayerRef->SendPacket(&packet);
}


void CSkillManagerPc::UpgradeSkill(BYTE skillIndex)
{
	CNtlPacket packet(sizeof(sGU_SKILL_UPGRADE_RES));
	sGU_SKILL_UPGRADE_RES * res = (sGU_SKILL_UPGRADE_RES *)packet.GetPacketData();
	res->wOpCode = GU_SKILL_UPGRADE_RES;
	res->skillIndex = skillIndex;

	CSkillPc* pSkill = (CSkillPc*)GetSkillWithSkillIndex(skillIndex);
	if (pSkill)
	{
		res->skillId = pSkill->GetOriginalTableData()->dwNextSkillTblidx;
		if (pSkill->UpgradeSkill(res->wResultCode, (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(pSkill->GetSkillId() + 1)))
		{
			CGameServer* app = (CGameServer*)g_pApp;
			//send query packet
			CNtlPacket pQry(sizeof(sGQ_SKILL_UPGRADE_REQ));
			sGQ_SKILL_UPGRADE_REQ* rQry = (sGQ_SKILL_UPGRADE_REQ*)pQry.GetPacketData();
			rQry->wOpCode = GQ_SKILL_UPGRADE_REQ;
			rQry->charId = m_pPlayerRef->GetCharID();
			rQry->bySlot = pSkill->GetSkillIndex();
			rQry->newSkillId = pSkill->GetSkillId();
			rQry->dwSP = m_pPlayerRef->GetSkillPoints();
			pQry.SetPacketLen(sizeof(sGQ_SKILL_UPGRADE_REQ));
			app->SendTo(app->GetQueryServerSession(), &pQry);
		}
	}
	else res->wResultCode = GAME_SKILL_YOU_DONT_HAVE_THE_SKILL;

	packet.SetPacketLen(sizeof(sGU_SKILL_UPGRADE_RES));
	m_pPlayerRef->SendPacket(&packet);
}


void CSkillManagerPc::StartSkillCoolDown(BYTE skillIndex)
{
	CSkillPc * pSkill = (CSkillPc*)GetSkillWithSkillIndex(skillIndex);
	if (pSkill)
	{
		if (BIT_FLAG_TEST(SKILL_FUNCTION_FLAG_SPECIAL_COOL_TIME_RULE, pSkill->GetOriginalTableData()->dwFunction_Bit_Flag))
		{
			CNtlPacket packet(sizeof(sGU_SKILL_COOL_TIME_STARTED_NFY));
			sGU_SKILL_COOL_TIME_STARTED_NFY * res = (sGU_SKILL_COOL_TIME_STARTED_NFY *)packet.GetPacketData();
			res->wOpCode = GU_SKILL_COOL_TIME_STARTED_NFY;
			res->hSubject = m_pPlayerRef->GetID();
			res->skillTblidx = pSkill->GetSkillId();
			res->byRpBonus = pSkill->GetUsedRpBonus();
			packet.SetPacketLen(sizeof(sGU_SKILL_COOL_TIME_STARTED_NFY));
			m_pPlayerRef->SendPacket(&packet);

			float fCoolDown = (float)pSkill->GetOriginalTableData()->dwCoolTimeInMilliSecs;

			fCoolDown += fCoolDown * m_pOwnerRef->GetCharAtt()->GetCoolTimeChangePercent() / 100.0f;

			if (pSkill->GetUsedRpBonus() == DBO_RP_BONUS_TYPE_COOL_TIME_MINUS)
			{
				float fRpCdr = pSkill->GetOriginalTableData()->afRpEffectValue[DBO_RP_BONUS_SLOT_COOL_TIME_MINUS] * 1000.0f;
				fCoolDown -= fRpCdr;
			}

			if (fCoolDown < 1.0f)
				fCoolDown = 0.0f;

			pSkill->SetCoolTimeRemaining((DWORD)fCoolDown);
		}
	}
	else ERR_LOG(LOG_GENERAL, "ERROR Player %u cant find skill index %u to set cooldown.", m_pPlayerRef->GetCharID(), skillIndex);
}


bool CSkillManagerPc::HasSkillLearned(TBLIDX skillTblidx)
{
	for (unsigned int pos = m_skillList.Begin();
		;
		pos = m_skillList.Next(pos))
	{
		if (pos == m_skillList.End())
			break;

		CSkill* pSkill = m_skillList.GetAt(pos);
		if (pSkill && pSkill->GetSkillId() == skillTblidx)
		{
			return true;
		}
	}

	return false;
}

bool CSkillManagerPc::HasBasicSkillLearned(TBLIDX skillTblidx)
{
	for (unsigned int pos = m_skillList.Begin();
		;
		pos = m_skillList.Next(pos))
	{
		if (pos == m_skillList.End())
			break;

		CSkill* pSkill = m_skillList.GetAt(pos);
		if (pSkill && g_pTableContainer->GetSkillTable()->FindBasicSkillTblidx(pSkill->GetSkillId()) == g_pTableContainer->GetSkillTable()->FindBasicSkillTblidx(skillTblidx))
		{
			return true;
		}
	}

	return false;
}


bool CSkillManagerPc::HasRequiredSkillLearned(sSKILL_TBLDAT* pNewSkillTbldat)
{
	if (pNewSkillTbldat->bySkill_Grade > 1) // If we upgrade the skill, check if previous grade already exist
	{
		if (HasSkillLearned(pNewSkillTbldat->tblidx - (pNewSkillTbldat->bySkill_Grade - 1)) == false)
			return false;
	}

	if (pNewSkillTbldat->uiRequire_Skill_Tblidx_Min_1 == INVALID_TBLIDX && pNewSkillTbldat->uiRequire_Skill_Tblidx_Max_1 == INVALID_TBLIDX) //skill dont require check
		return true;

	for (unsigned int pos = m_skillList.Begin();
		pos != m_skillList.End();
		pos = m_skillList.Next(pos))
	{
		CSkill* pSkill = m_skillList.GetAt(pos);
		if (pSkill)
		{
			//check required skill one
			if (pSkill->GetSkillId() >= pNewSkillTbldat->uiRequire_Skill_Tblidx_Min_1)
			{
				if (pSkill->GetSkillId() <= pNewSkillTbldat->uiRequire_Skill_Tblidx_Max_1)
				{
					if (pNewSkillTbldat->uiRequire_Skill_Tblidx_Min_2 == INVALID_TBLIDX) //if no second skill required then can return true
						return true;
				}
			}

			//check required skill two (only second class skills require 2 skills)
			if (pSkill->GetSkillId() >= pNewSkillTbldat->uiRequire_Skill_Tblidx_Min_2)
			{
				if (pSkill->GetSkillId() <= pNewSkillTbldat->uiRequire_Skill_Tblidx_Max_2)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool CSkillManagerPc::HasSkillsInCoolDown()
{
	for (unsigned int pos = m_skillList.Begin();
		pos != m_skillList.End();
		pos = m_skillList.Next(pos))
	{
		CSkill* pSkill = m_skillList.GetAt(pos);
		if (pSkill)
		{
			if (pSkill->GetCoolTimeRemaining() > 30000)
				return true;
		}
	}

	return false;
}


void CSkillManagerPc::InitSkills()
{
	std::vector<TBLIDX> vecLearnSkills;

	while (!m_skillList.IsEmpty())
	{
		unsigned int pos = m_skillList.Begin();

		CSkill* pSkill = m_skillList.GetAt(pos);
		if (pSkill)
		{
			sSKILL_TBLDAT* pTbldat = pSkill->GetOriginalTableData();

			TBLIDX basicTblidx = g_pTableContainer->GetSkillTable()->FindBasicSkillTblidx(pTbldat->tblidx);
			sSKILL_TBLDAT* pBasicTbldat = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(basicTblidx);

			if (pBasicTbldat->wRequireSP == 0)
			{
				vecLearnSkills.push_back(basicTblidx);
			}


			m_ppSkill[pSkill->GetSkillIndex()] = NULL;
			SAFE_DELETE(pSkill);
		}

		m_skillList.Remove(pos);
	}

	WORD wTemp;
	for (std::vector<TBLIDX>::iterator it = vecLearnSkills.begin(); it != vecLearnSkills.end(); it++)
	{
		LearnSkill(*it, wTemp, false);
	}
}


bool CSkillManagerPc::CanResetSkill(CSkillPc* pResetSkill)
{
	if (!pResetSkill->CanResetSkill())
		return false;

	sSKILL_TBLDAT* pResetSkillTbldat = pResetSkill->GetOriginalTableData();

	if (pResetSkillTbldat->uiRequire_Skill_Tblidx_Min_1 == INVALID_TBLIDX && pResetSkillTbldat->uiRequire_Skill_Tblidx_Max_1 == INVALID_TBLIDX) //skill dont require check
		return true;

	//check if other skills requires the skill lvl
	for (unsigned int pos = m_skillList.Begin();
		pos != m_skillList.End();
		pos = m_skillList.Next(pos))
	{
		CSkill* pSkill = m_skillList.GetAt(pos);
		if (pSkill && pSkill->GetSkillIndex() != pResetSkill->GetSkillIndex())
		{
			sSKILL_TBLDAT* pTbldat = pSkill->GetOriginalTableData();

			if (pResetSkill->GetSkillId() == pTbldat->uiRequire_Skill_Tblidx_Min_1) //check if skill is required by another skill
				return false;

			else if (pResetSkill->GetSkillId() == pTbldat->uiRequire_Skill_Tblidx_Min_2)//check if skill is required by another skill
				return false;
		}
	}

	return true;
}


int CSkillManagerPc::CopyToInfo(sSKILL_INFO * pSkillInfo)
{
	int nSize = 0;

	for (unsigned int pos = m_skillList.Begin();
		pos != m_skillList.End();
		pos = m_skillList.Next(pos))
	{
		CSkillPc* pSkill = (CSkillPc*)m_skillList.GetAt(pos);
		if (pSkill)
		{
			{
				nSize += pSkill->CopyToInfo(&pSkillInfo[pos]);
			}
		}
	}

	return nSize;
}


int CSkillManagerPc::CopyToData(sSKILL_DATA * pSkillData)
{
	int nSize = 0;

	for (unsigned int pos = m_skillList.Begin();
		pos != m_skillList.End();
		pos = m_skillList.Next(pos))
	{
		CSkillPc* pSkill = (CSkillPc*)m_skillList.GetAt(pos);
		if (pSkill)
		{
			{
				nSize += pSkill->CopyToData(pSkillData);
				++pSkillData;
			}
		}
	}

	return nSize;
}


void CSkillManagerPc::CopyPassiveAttributesTo(CCharacterAtt* pCharAtt)
{
	for (unsigned int pos = m_skillList.Begin();
		pos != m_skillList.End();
		pos = m_skillList.Next(pos))
	{
		CSkillPc* pSkill = (CSkillPc*)m_skillList.GetAt(pos);
		if (pSkill)
		{
			sSKILL_TBLDAT* pSkillTbldat = pSkill->GetOriginalTableData();

			if (pSkillTbldat->bySkill_Class == NTL_SKILL_CLASS_PASSIVE)
			{
				for (int i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
				{
					if (pSkillTbldat->skill_Effect[i] != INVALID_TBLIDX)
					{
					//	printf("pSkill->GetEffectCode(i) %u, pSkillTbldat->bySkill_Effect_Type[i] %u, value %f \n", pSkill->GetEffectCode(i), pSkillTbldat->bySkill_Effect_Type[i], (float)pSkillTbldat->aSkill_Effect_Value[i]);
						Dbo_SetAvatarAttributeValue(pCharAtt, pSkill->GetEffectCode(i), (float)pSkillTbldat->aSkill_Effect_Value[i], pSkillTbldat->bySkill_Effect_Type[i]);
					}
				}
			}
		}
	}
}


void CSkillManagerPc::_SaveSkillData()
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket pQry(sizeof(sGQ_SAVE_SKILL_DATA_REQ));
	sGQ_SAVE_SKILL_DATA_REQ* rQry = (sGQ_SAVE_SKILL_DATA_REQ*)pQry.GetPacketData();
	rQry->wOpCode = GQ_SAVE_SKILL_DATA_REQ;
	rQry->charId = m_pPlayerRef->GetCharID();
	rQry->handle = m_pPlayerRef->GetID();

	for (unsigned int pos = m_skillList.Begin();
		pos != m_skillList.End();
		pos = m_skillList.Next(pos))
	{
		CSkillPc* pSkill = (CSkillPc*)m_skillList.GetAt(pos);
		if (pSkill && pSkill->GetOriginalTableData()->bySkill_Class != NTL_SKILL_CLASS_PASSIVE)
		{
			rQry->asSkill[rQry->bySkillCount].bIsRpBonusAuto = pSkill->IsRpBonusAuto();
			rQry->asSkill[rQry->bySkillCount].byRpBonusType = pSkill->GetRpBonusType();
			rQry->asSkill[rQry->bySkillCount].nExp = 0;
			rQry->asSkill[rQry->bySkillCount].nRemainSec = (int)pSkill->GetCoolTimeRemaining();
			rQry->asSkill[rQry->bySkillCount].skillId = pSkill->GetSkillId();
			rQry->asSkill[rQry->bySkillCount++].skillIndex = pSkill->GetSkillIndex();
		}
	}
	
	pQry.SetPacketLen(sizeof(sGQ_SAVE_SKILL_DATA_REQ));
	app->SendTo(app->GetQueryServerSession(), &pQry);
}


void CSkillManagerPc::FinishCasting()
{
	m_bCastingFinished = true;

	CNtlPacket packet(sizeof(sGU_SKILL_TARGET_LIST_REQ));
	sGU_SKILL_TARGET_LIST_REQ * res = (sGU_SKILL_TARGET_LIST_REQ *)packet.GetPacketData();
	res->wOpCode = GU_SKILL_TARGET_LIST_REQ;
	res->byAvatarType = DBO_AVATAR_TYPE_AVATAR;
	packet.SetPacketLen(sizeof(sGU_SKILL_TARGET_LIST_REQ));
	m_pPlayerRef->SendPacket(&packet);
}

void CSkillManagerPc::CancelCasting()
{
	m_bCastingFinished = false;
	SetCurSkillTblidx(INVALID_TBLIDX);
}

