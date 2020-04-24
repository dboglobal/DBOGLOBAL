#include "stdafx.h"
#include "Monster.h"
#include "NtlRandom.h"

#include "SkillCondition_Give.h"
#include "SkillCondition_LP.h"
#include "SkillCondition_OnlyLP.h"
#include "SkillCondition_RingRange.h"
#include "SkillCondition_Time.h"



CSkillManagerBot::CSkillManagerBot()
{
	Init();
}


CSkillManagerBot::~CSkillManagerBot()
{
	Destroy();
}



bool CSkillManagerBot::Create(CCharacterObject * pOwnerRef, DWORD dwMaxNumberOfSkill)
{
	return CSkillManager::Create(pOwnerRef, dwMaxNumberOfSkill);
}


void CSkillManagerBot::Destroy()
{
	CSkillManager::Destroy();

	for (int j = 0; m_bySkillAllCount > j; j++)
	{
		if (m_apSkillCondition_All[j])
		{
			CSkillCondition* pSkill = m_apSkillCondition_All[j];
			SAFE_DELETE(pSkill);
			m_apSkillCondition_All[j] = NULL;
		}
	}

	Init();
}


void CSkillManagerBot::Init()
{
	m_bySkillAllCount = 0;
	m_bySkillCondition_LP = 0;
	m_bySkillCondition_Give = 0;
	m_bySkillCondition_Time = 0;
	m_bySkillCondition_RingRange = 0;
	m_bySkillCondition_OnlyLP = 0;

	for (int j = 0; j < NTL_MAX_NPC_HAVE_SKILL; j++)
	{
		m_apSkillCondition_All[j] = NULL;
		m_apSkillCondition_LP[j] = NULL;
		m_apSkillCondition_Give[j] = NULL;
		m_apSkillCondition_Time[j] = NULL;
		m_apSkillCondition_RingRange[j] = NULL;
		m_apSkillCondition_OnlyLP[j] = NULL;
	}

	m_curSkillTblidx = INVALID_TBLIDX;
	m_byCurSkillConditionIdx = INVALID_BYTE;

	SetSkillUse_Unlock();
}


bool CSkillManagerBot::RemoveSkillAll()
{
	if (CSkillManager::RemoveSkillAll())
	{
		m_bySkillAllCount = 0;
		m_bySkillCondition_LP = 0;
		m_bySkillCondition_Give = 0;
		m_bySkillCondition_Time = 0;
		m_bySkillCondition_RingRange = 0;
		m_bySkillCondition_OnlyLP = 0;
		for (int j = 0; j < NTL_MAX_NPC_HAVE_SKILL; j++)
		{
			if (m_apSkillCondition_All[j])
			{
				CSkillCondition* pSkill = m_apSkillCondition_All[j];
				SAFE_DELETE(pSkill);
				m_apSkillCondition_All[j] = NULL;
			}
			m_apSkillCondition_LP[j] = NULL;
			m_apSkillCondition_Give[j] = NULL;
			m_apSkillCondition_Time[j] = NULL;
			m_apSkillCondition_RingRange[j] = NULL;
			m_apSkillCondition_OnlyLP[j] = NULL;
		}
		
		return true;
	}

	return false;
}


bool CSkillManagerBot::AddSkill(BYTE byCount, CNpc *pBot, CSkillBot *pSkill, TBLIDX Skill_Tblidx, BYTE byUse_Skill_Basis, WORD wUse_Skill_LP, WORD wUse_Skill_Time)
{
	if (!CSkillManager::AddSkill(pSkill))
		return false;

	CSkillCondition* pSkillCondition;

	if (byUse_Skill_Basis == 3)
	{
		pSkillCondition = new CSkillCondition_LP;
		if (!pSkillCondition)
			return false;

		m_apSkillCondition_LP[m_bySkillCondition_LP++] = pSkillCondition;
		goto LABEL_49;
	}
	else if (byUse_Skill_Basis == 4)
	{
		pSkillCondition = new CSkillCondition_Give;
		if (!pSkillCondition)
			return false;

		m_apSkillCondition_Give[m_bySkillCondition_Give++] = pSkillCondition;
		goto LABEL_49;
	}
	else if (byUse_Skill_Basis == 5)
	{
		pSkillCondition = new CSkillCondition_Time;
		if (!pSkillCondition)
			return false;

		m_apSkillCondition_Time[m_bySkillCondition_Time++] = pSkillCondition;
		goto LABEL_49;
	}
	else if (byUse_Skill_Basis == 6)
	{
		pSkillCondition = new CSkillCondition_RingRange;
		if (!pSkillCondition)
			return false;

		m_apSkillCondition_RingRange[m_bySkillCondition_RingRange++] = pSkillCondition;
		goto LABEL_49;
	}
	else if (byUse_Skill_Basis == 7)
	{
		pSkillCondition = new CSkillCondition_OnlyLP;
		if (!pSkillCondition)
			return false;

		m_apSkillCondition_OnlyLP[m_bySkillCondition_OnlyLP++] = pSkillCondition;
		goto LABEL_49;
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "fail : switch( byUse_Skill_Basis ) : byUse_Skill_Basis(%u)", byUse_Skill_Basis);
	}

	return false;


LABEL_49:
	m_apSkillCondition_All[m_bySkillAllCount] = pSkillCondition;
	pSkillCondition->SetBot(pBot);
	pSkillCondition->SetSkill(pSkill);
	pSkillCondition->SetSkillTblidx(Skill_Tblidx);
	pSkillCondition->SetSkillBasis(byUse_Skill_Basis);
	pSkillCondition->SetSkillLP(wUse_Skill_LP);
	pSkillCondition->SetSkillTime(wUse_Skill_Time);
	pSkillCondition->SetSkillConditionIdx(m_bySkillAllCount);

	m_bySkillAllCount++;
	return true;
}


CSkill* CSkillManagerBot::GetSkill()
{
	for (unsigned int pos = m_skillList.Begin();
		;
		pos = m_skillList.Next(pos))
	{
		if (pos == m_skillList.End())
			break;

		CSkill* pSkill = m_skillList.GetAt(pos);
		if (pSkill && pSkill->GetSkillId() != INVALID_TBLIDX)
		{
			return pSkill;
		}
	}

	return NULL;
}


CCharacter* CSkillManagerBot::GetOwnerCharacter()
{
	return dynamic_cast<CCharacter*>(m_pOwnerRef);
}


CSkillCondition* CSkillManagerBot::GetSkill(CSkillCondition **apSkillCondition, int nSkillConditionCount, DWORD dwTickTime)
{
	if (nSkillConditionCount > 0)
	{
		int nTSkill = 0;
		CSkillCondition* apSkillConditionBuf[NTL_MAX_NPC_HAVE_SKILL];

		for (int j = 0; j < nSkillConditionCount && apSkillCondition[j]; j++)
		{
			if (apSkillCondition[j]->OnUpdate(dwTickTime))
			{
				apSkillConditionBuf[nTSkill++] = apSkillCondition[j];
			}
		}

		if (nTSkill > 0)
		{
			if (nTSkill == 1)
			{
				return apSkillConditionBuf[0];
			}
			else if (nTSkill == 2)
			{
				return apSkillConditionBuf[rand() % nTSkill];
			}
			else
			{
				return apSkillConditionBuf[rand() % nTSkill];
			}
		}
	}

	return NULL;
}


CSkillCondition* CSkillManagerBot::GetSkill(DWORD dwTickTime)
{
	CSkillCondition* pSkill = NULL;

	pSkill = GetSkill(m_apSkillCondition_LP, m_bySkillCondition_LP, dwTickTime);
	if (!pSkill)
	{
		pSkill = GetSkill(m_apSkillCondition_OnlyLP, m_bySkillCondition_OnlyLP, dwTickTime);
		if (!pSkill)
		{
			pSkill = GetSkill(m_apSkillCondition_Give, m_bySkillCondition_Give, dwTickTime);
			if (!pSkill)
			{
				pSkill = GetSkill(m_apSkillCondition_RingRange, m_bySkillCondition_RingRange, dwTickTime);
				if (!pSkill)
				{
					pSkill = GetSkill(m_apSkillCondition_Time, m_bySkillCondition_Time, dwTickTime);
					if (!pSkill)
					{
						return NULL;
					}
				}
			}
		}
	}

	return pSkill;
}


CSkillCondition* CSkillManagerBot::FindSkillCondition(BYTE bySkillIdx)
{
	if (bySkillIdx < NTL_MAX_NPC_HAVE_SKILL)
		return m_apSkillCondition_All[bySkillIdx];
	else
		ERR_LOG(LOG_BOTAI, "NTL_MAX_NPC_HAVE_SKILL >= bySkillIdx[%u].", bySkillIdx);


	return NULL;
}

CSkillCondition * CSkillManagerBot::FindSkillCondition(TBLIDX skillTblidx)
{
	for (BYTE i = 0; i < m_bySkillAllCount; i++)
	{
		if (m_apSkillCondition_All[i]->GetSkillTblidx() == skillTblidx)
			return m_apSkillCondition_All[i];
	}

	return nullptr;
}


CSkillCondition* CSkillManagerBot::GetCurSkillCondition()
{
	if (m_byCurSkillConditionIdx < NTL_MAX_NPC_HAVE_SKILL)
		return m_apSkillCondition_All[m_byCurSkillConditionIdx];
	else
		ERR_LOG(LOG_BOTAI, "NTL_MAX_NPC_HAVE_SKILL >= m_byCurSkillConditionIdx[%u].", m_byCurSkillConditionIdx);


	return NULL;
}


void CSkillManagerBot::SetCurSkillConditionIdx(BYTE byCurSkillConditionIdx)
{
	m_byCurSkillConditionIdx = byCurSkillConditionIdx;
}


BYTE CSkillManagerBot::FillPetSkillInfo(sSKILL_INFO_PET * pSkillInfo)
{
	for (BYTE i = 0; i < m_bySkillAllCount; i++)
	{
		pSkillInfo[i].tblidx = m_apSkillCondition_All[i]->GetSkillTblidx();
		pSkillInfo[i].skillIndex = m_apSkillCondition_All[i]->GetSkillConditionIdx();
	}

	return m_bySkillAllCount;
}

void CSkillManagerBot::SetCanUseSkill(TBLIDX skillIdx, bool bFlag)
{
	for (BYTE i = 0; i < m_bySkillAllCount; i++)
	{
		if (m_apSkillCondition_All[i]->GetSkillTblidx() == skillIdx)
		{
			m_apSkillCondition_All[i]->SetCanUseSkill(bFlag);

			return;
		}
	}
}

void CSkillManagerBot::FinishCasting()
{
	CSkillCondition* pSkillCond = GetCurSkillCondition();
	if (pSkillCond)
	{
		HOBJECT hTarget = INVALID_HOBJECT;
		sSKILL_TARGET_LIST targetList;

		pSkillCond->GetTarget(hTarget, targetList); //refetch target because some might moved out/in

		if (hTarget != INVALID_HOBJECT && targetList.byTargetCount > 0)
			pSkillCond->GetSkill()->CastSkill(hTarget, targetList.byTargetCount, targetList.ahTarget);
		else
		{
			CancelCasting();
		}
	}
}

void CSkillManagerBot::CancelCasting()
{
	SetCurSkillConditionIdx(INVALID_BYTE);
	SetSkillUse_Unlock();

	CSkillManager::CancelCasting();
}
