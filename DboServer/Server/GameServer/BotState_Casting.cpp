#include "stdafx.h"
#include "BotState_Casting.h"
#include "char.h"
#include "NtlPacketGU.h"


CBotState_Casting::CBotState_Casting(CCharacter* pParent)
	:CCharState(CHARSTATE_CASTING, pParent)
{
}

CBotState_Casting::~CBotState_Casting()
{
}


void CBotState_Casting::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_Casting::OnExit()
{
	if (m_dwCastingTimeRemaining > 0)
	{
		m_pParent->GetSkillManager()->CancelCasting();

		CNtlPacket packet(sizeof(sGU_SKILL_CASTING_CANCELED_NFY));
		sGU_SKILL_CASTING_CANCELED_NFY * res = (sGU_SKILL_CASTING_CANCELED_NFY *)packet.GetPacketData();
		res->wOpCode = GU_SKILL_CASTING_CANCELED_NFY;
		res->handle = m_pParent->GetID();
		res->wResultCode = 500; //GAME_SUCCESS
		packet.SetPacketLen(sizeof(sGU_SKILL_CASTING_CANCELED_NFY));
		m_pParent->Broadcast(&packet);
	}

	CCharState::OnExit();
}

int CBotState_Casting::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	if (m_dwCastingTimeRemaining > 0)
	{
		m_dwCastingTimeRemaining = UnsignedSafeDecrease<DWORD>(m_dwCastingTimeRemaining, dwTickDiff);

		if (m_dwCastingTimeRemaining == 0)
		{
			m_pParent->GetSkillManager()->FinishCasting();
		}
	}

	return NULL;
}

int CBotState_Casting::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return NULL;
}

bool CBotState_Casting::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_GUARD || stateID == CHARSTATE_RIDEON || stateID == CHARSTATE_AIR_JUMP)
		return false;

	return true;
}

int CBotState_Casting::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateCasting.skillId = m_skillId;
	pStateDetail->sCharStateCasting.hTarget = m_hTarget;
	pStateDetail->sCharStateCasting.dwCastingTime = m_dwCastingTime;
	pStateDetail->sCharStateCasting.dwCastingTimeRemaining = m_dwCastingTimeRemaining;

	return sizeof(sCHARSTATE_CASTING);
}

void CBotState_Casting::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	m_skillId = pStateDetail->sCharStateCasting.skillId;
	m_hTarget = pStateDetail->sCharStateCasting.hTarget;
	m_dwCastingTime = pStateDetail->sCharStateCasting.dwCastingTime;
	m_dwCastingTimeRemaining = pStateDetail->sCharStateCasting.dwCastingTimeRemaining;
}

