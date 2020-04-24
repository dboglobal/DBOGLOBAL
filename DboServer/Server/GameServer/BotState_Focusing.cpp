#include "stdafx.h"
#include "BotState_Focusing.h"
#include "CPlayer.h"


CBotState_Focusing::CBotState_Focusing(CCharacter* pParent)
	:CCharState(CHARSTATE_FOCUSING, pParent)
{
}

CBotState_Focusing::~CBotState_Focusing()
{
}


void CBotState_Focusing::OnEnter()
{
	m_dwTime = 300;

	CCharState::OnEnter();
}

void CBotState_Focusing::OnExit()
{
	CCharState::OnExit();
}

int CBotState_Focusing::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	m_dwTime = UnsignedSafeDecrease<DWORD>(m_dwTime, dwTickDiff);

	if (m_dwTime == 0)
	{
		CSkill* pSkill = (CSkill*)m_pParent->GetSkillManager()->FindSkill(m_pParent->GetSkillManager()->GetCurSkillTblidx());
		if (pSkill)
		{
			pSkill->CheckCasting();
			m_dwTime = INVALID_DWORD;
		}
	}

	return 0;
}

int CBotState_Focusing::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_Focusing::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_GUARD || stateID == CHARSTATE_RIDEON || stateID == CHARSTATE_AIR_JUMP)
		return false;

	return true;
}

int CBotState_Focusing::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	return 0;
}

void CBotState_Focusing::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
}

