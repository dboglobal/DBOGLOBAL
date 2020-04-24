#include "stdafx.h"
#include "BotState_Standing.h"
#include "char.h"

CBotState_Standing::CBotState_Standing(CCharacter* pParent)
	:CCharState(CHARSTATE_STANDING, pParent)
{
	m_dwWakeUpTime = 0;
}

CBotState_Standing::~CBotState_Standing()
{
}


void CBotState_Standing::OnEnter()
{
	m_dwWakeUpTime = 0;
	if (GetPrevStateID() == CHARSTATE_KNOCKDOWN || GetPrevStateID() == CHARSTATE_SANDBAG)
	{
		m_dwWakeUpTime = (DWORD)m_pParent->GetAniTbldat()->GetSkillAnimationLength(511);
		if (m_dwWakeUpTime > 0)
		{
			if (m_pParent->GetBotController())
				m_pParent->GetBotController()->SetControlBlock(true);
		}
	}

	CCharState::OnEnter();
}

void CBotState_Standing::OnExit()
{
	if (m_pParent->GetBotController())
	{
		if (m_pParent->GetBotController()->IsControlStateBlocked())
			m_pParent->GetBotController()->SetControlBlock(false);
	}

	CCharState::OnExit();
}

int CBotState_Standing::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	if (m_dwWakeUpTime)
	{
		m_dwWakeUpTime = UnsignedSafeDecrease<DWORD>(m_dwWakeUpTime, dwTickDiff);
		if (!m_dwWakeUpTime)
		{
			if (m_pParent->GetBotController())
			{
				m_pParent->GetBotController()->SetControlBlock(false);
			}
		}
	}

	//((void(__fastcall *)(CBot *, _QWORD))v8->m_pParent->vfptr[4].__vecDelDtor)(v8->m_pParent, b);
	//((void(__fastcall *)(CBot *, _QWORD))v8->m_pParent->vfptr[3].SendPacket)(v8->m_pParent, b);
	return 0;
}

int CBotState_Standing::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_Standing::CanTransition(BYTE stateID)
{
	return true;
}

int CBotState_Standing::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	return 0;
}

void CBotState_Standing::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	m_pParent->SetMoveDirection(NTL_MOVE_NONE);
	m_pParent->SetMoveFlag(NTL_MOVE_FLAG_INVALID);
}

