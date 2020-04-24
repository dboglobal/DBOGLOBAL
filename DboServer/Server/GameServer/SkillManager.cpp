#include "stdafx.h"
#include "SkillManager.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CSkillManager::CSkillManager()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CSkillManager::~CSkillManager()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CSkillManager::Create(CCharacterObject * pOwnerRef, DWORD dwMaxNumberOfSkill)
{
	if (pOwnerRef == NULL)
	{
		ERR_LOG(LOG_USER, "pOwnerRef is NULL");
		return false;
	}

	m_pOwnerRef = pOwnerRef;
	
	if (m_skillList.Create(dwMaxNumberOfSkill, 0))
	{
		m_ppSkill = new CSkill* [dwMaxNumberOfSkill];

		if (m_ppSkill)
		{
			for (DWORD i = 0; i < dwMaxNumberOfSkill; i++)
				m_ppSkill[i] = NULL;

			m_paTblidxInTransaction = new TBLIDX[dwMaxNumberOfSkill];
			if (m_paTblidxInTransaction)
			{
				for (DWORD j = 0; j < dwMaxNumberOfSkill; j++)
					m_paTblidxInTransaction[j] = INVALID_TBLIDX;

				m_dwMaxNumberOfSkill = dwMaxNumberOfSkill;
				m_pOwnerRef = pOwnerRef;

				return true;
			}
		}
	}

	Destroy();

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CSkillManager::Destroy()
{
	for (unsigned int pos = m_skillList.Begin();
		pos < GetNumberOfSkill();
		pos = m_skillList.Next(pos))
	{
		if (pos == m_skillList.End())
			break;

		CSkill* pSkill = m_skillList.GetAt(pos);
		SAFE_DELETE(pSkill);
	}

	m_skillList.Clear();

	m_dwMaxNumberOfSkill = 0;

	if (m_ppSkill)
		SAFE_DELETE_ARRAY(m_ppSkill);
	if (m_paTblidxInTransaction)
		SAFE_DELETE_ARRAY(m_paTblidxInTransaction);

	m_pOwnerRef = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CSkillManager::TickProcess(DWORD dwTickDiff)
{
	for (unsigned int pos = m_skillList.Begin();
		pos != m_skillList.End();
		pos = m_skillList.Next(pos))
	{
		CSkill* pSkill = m_skillList.GetAt(pos);
		if (pSkill)
			pSkill->TickProcess(dwTickDiff);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CSkillManager::AddSkill(CSkill * pSkill)
{
	if (GetMaxNumberOfSkill() <= m_skillList.GetSize())
		return false;

	if (FindSkill(pSkill->GetSkillId()))
		return false;

	BYTE bySkillIndex = pSkill->GetSkillIndex();
	BYTE finalSkillIndex = INVALID_BYTE;

	if (bySkillIndex == 255)
	{
		for (DWORD j = 0; j < m_dwMaxNumberOfSkill && j != INVALID_BYTE; j++)
		{
			if (!m_ppSkill[j])
			{
				finalSkillIndex = (BYTE)j;
				break;
			}
		}
	}
	else
	{
		if (m_dwMaxNumberOfSkill <= bySkillIndex)
			return false;
		
		if (m_ppSkill[bySkillIndex])
			return false;
		
		finalSkillIndex = bySkillIndex;
	}

	if (finalSkillIndex == INVALID_BYTE)
		return false;
	else
	{
		m_ppSkill[finalSkillIndex] = pSkill;
		
		if (m_skillList.Insert(pSkill) == m_skillList.End())
		{
			m_ppSkill[finalSkillIndex] = NULL;
			
			return false;
		}
		else
		{
			if (bySkillIndex == INVALID_BYTE)
				pSkill->SetSkillIndex(finalSkillIndex);

			return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CSkillManager::RemoveSkill(TBLIDX tblidx)
{
	for (unsigned int pos = m_skillList.Begin();
		pos != m_skillList.End();
		pos = m_skillList.Next(pos))
	{
		CSkill* pSkill = m_skillList.GetAt(pos);
		if (pSkill && pSkill->GetSkillId() == tblidx)
		{
			RemoveSkill_Internal(pos);
			return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CSkillManager::RemoveSkillAll()
{
	while (!m_skillList.IsEmpty())
	{
		unsigned int pos = m_skillList.Begin();

		CSkill* pSkill = m_skillList.GetAt(pos);
		if (pSkill)
		{
			SAFE_DELETE(pSkill);
		}

		m_skillList.Remove(pos);
	}

	for (DWORD j = 0; j < m_dwMaxNumberOfSkill; j++)
		m_ppSkill[j] = NULL;

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	: Check if player already has current skill
//		Return	:
//-----------------------------------------------------------------------------------
CSkill* CSkillManager::FindSkill(TBLIDX tblidx)
{
	for (unsigned int pos = m_skillList.Begin();
		pos != m_skillList.End();
		pos = m_skillList.Next(pos))
	{
		CSkill* pSkill = m_skillList.GetAt(pos);
		if (pSkill && pSkill->GetSkillId() == tblidx)
		{
			return pSkill;
		}
	}

	return NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CSkill* CSkillManager::GetSkillWithSkillIndex(BYTE byIndex)
{
	if (m_dwMaxNumberOfSkill > byIndex)
		return m_ppSkill[byIndex];
	
	return NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CSkillManager::FindSkillWithSystemEffect(TBLIDX systemEffect, std::list<CSkill*> *rListSkill)
{
	rListSkill->clear();

	for (unsigned int pos = m_skillList.Begin();
		pos != m_skillList.End();
		pos = m_skillList.Next(pos))
	{
		CSkill* pSkill = m_skillList.GetAt(pos);
		if (pSkill)
		{
			for (int j = 0; j < NTL_MAX_EFFECT_IN_SKILL; j++)
			{
				sSKILL_TBLDAT* pTbldat = pSkill->GetOriginalTableData();
				if (pTbldat->skill_Effect[j] == systemEffect)
				{
					rListSkill->push_back(pSkill);
					break;
				}
			}
		}
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CSkill* CSkillManager::FindSkillWithSystemEffectCode(eSYSTEM_EFFECT_CODE effectCode)
{
	for (unsigned int pos = m_skillList.Begin();
		pos != m_skillList.End();
		pos = m_skillList.Next(pos))
	{
		CSkill* pSkill = m_skillList.GetAt(pos);
		if (pSkill)
		{
			for (int j = 0; j < NTL_MAX_EFFECT_IN_SKILL; j++)
			{
				if (pSkill->GetEffectCode(j) == effectCode)
				{
					return pSkill;
				}
			}
		}
	}

	return nullptr;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BYTE CSkillManager::AcquireSkillIndex(TBLIDX skillTblidxToGet)
{
	BYTE skillIndexToUse = INVALID_BYTE;

	for (BYTE j = 0; j < m_dwMaxNumberOfSkill; j++)
	{
		if (!m_ppSkill[j])
		{
			if (skillTblidxToGet == m_paTblidxInTransaction[j])
				return INVALID_BYTE;

			if (skillIndexToUse == INVALID_BYTE && m_paTblidxInTransaction[j] == INVALID_TBLIDX)
				skillIndexToUse = j;
		}
	}

	if (skillIndexToUse != INVALID_BYTE)
		m_paTblidxInTransaction[skillIndexToUse] = skillTblidxToGet;

	return skillIndexToUse;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CSkillManager::ReleaseSkillIndex(BYTE skillIndex)
{
	if (skillIndex < m_dwMaxNumberOfSkill)
	{
		if (m_paTblidxInTransaction[skillIndex] == INVALID_TBLIDX)
			ERR_LOG(LOG_USER, "The speficied skillIndex is not locked. skillIndex = %u", skillIndex);
		else
			m_paTblidxInTransaction[skillIndex] = INVALID_TBLIDX;
	}
	else
		ERR_LOG(LOG_USER, "skillIndex has a wrong value. skillIndex[%u] >= m_dwMaxNumberOfSkill[%u]", skillIndex, m_dwMaxNumberOfSkill);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CSkillManager::Init()
{
	this->m_dwMaxNumberOfSkill = 0;
	this->m_ppSkill = NULL;
	this->m_paTblidxInTransaction = NULL;
	this->m_pOwnerRef = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CSkillManager::RemoveSkill_Internal(unsigned int iterList)
{
	CSkill* pSkill = m_skillList.GetAt(iterList);
	if (pSkill)
	{
		BYTE bySkillIndex = pSkill->GetSkillIndex();
		if (bySkillIndex < m_dwMaxNumberOfSkill)
		{
			m_ppSkill[bySkillIndex] = NULL;
		}
		else
		{
			ERR_LOG(LOG_USER, "skillIndex has a wrong value., skillIndex = %u, m_dwMaxNumberOfSkill = %u", bySkillIndex, m_dwMaxNumberOfSkill);
		}

		m_skillList.Remove(iterList);

		SAFE_DELETE(pSkill);
		return true;
	}
	else
	{
		ERR_LOG(LOG_USER, "(NULL == pSkill)");
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CSkillManager::CancelCasting()
{
	SetCurSkillTblidx(INVALID_TBLIDX);
}















