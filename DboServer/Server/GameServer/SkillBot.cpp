#include "stdafx.h"
#include "SkillBot.h"
#include "Monster.h"


CSkillBot::CSkillBot()
{
	Init();
}

CSkillBot::~CSkillBot()
{
	Destroy();
}


bool CSkillBot::Create(sSKILL_TBLDAT *pSkillDataRef, CNpc *pOwnerRef, BYTE skillIndex)
{
	if (CSkill::Create(pSkillDataRef, pOwnerRef, skillIndex))
	{
		m_pBotRef = pOwnerRef;
		return true;
	}

	Destroy();
	return false;
}


void CSkillBot::Destroy()
{
	m_pBotRef = NULL;
}


void CSkillBot::Init()
{
	m_pBotRef = NULL;
}


void CSkillBot::TickProcess(DWORD dwTickDiff)
{
	CSkill::TickProcess(dwTickDiff);
}

bool CSkillBot::UseSkill(BYTE byRpBonusType, HOBJECT hAppointTargetHandle, CNtlVector & rvFinalSubjectLoc, CNtlVector& rvFinalLoc, BYTE byApplyTargetCount, HOBJECT * ahApplyTarget, WORD & rwResultCode)
{
	if (CSkill::UseSkill(byRpBonusType, hAppointTargetHandle, rvFinalSubjectLoc, rvFinalLoc, byApplyTargetCount, ahApplyTarget, rwResultCode))
	{
		CheckCasting();

		return true;
	}

	return false;
}


void CSkillBot::OnAffected()
{
	CSkill::OnAffected();
}

