#include "stdafx.h"
#include "BotState_Sitting.h"
#include "char.h"


CBotState_Sitting::CBotState_Sitting(CCharacter* pParent)
	:CCharState(CHARSTATE_SITTING, pParent)
{
}

CBotState_Sitting::~CBotState_Sitting()
{
}


void CBotState_Sitting::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_Sitting::OnExit()
{
	if (m_pParent->IsPC())
	{
		m_pParent->GetBuffManager()->EndSubBuff(ACTIVE_HEAL_OVER_TIME, INVALID_SYSTEM_EFFECT_CODE);
		m_pParent->GetBuffManager()->EndSubBuff(ACTIVE_EP_OVER_TIME, INVALID_SYSTEM_EFFECT_CODE);
	}

	CCharState::OnExit();
}

int CBotState_Sitting::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	//((void(__fastcall *)(CBot *, _QWORD))v7->m_pParent->vfptr[4].__vecDelDtor)(v7->m_pParent, dwTickTime);
	//((void(__fastcall *)(CBot *, _QWORD))v7->m_pParent->vfptr[3].SendPacket)(v7->m_pParent, v8);

	return 0;
}

int CBotState_Sitting::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_Sitting::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_GUARD || stateID == CHARSTATE_RIDEON || stateID == CHARSTATE_AIR_JUMP)
		return false;

	return true;
}

int CBotState_Sitting::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	return 0;
}

void CBotState_Sitting::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
}

