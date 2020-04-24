#include "stdafx.h"
#include "BotState_Stunned.h"
#include "char.h"

CBotState_Stunned::CBotState_Stunned(CCharacter* pParent)
	:CCharState(CHARSTATE_STUNNED, pParent)
{
}

CBotState_Stunned::~CBotState_Stunned()
{
}


void CBotState_Stunned::OnEnter()
{
	if (m_pParent->GetBotController())
	{
		m_pParent->GetBotController()->SetControlBlock(true);
	}

	CCharState::OnEnter();
}

void CBotState_Stunned::OnExit()
{
	if (m_pParent->GetBotController())
	{
		m_pParent->GetBotController()->SetControlBlock(false);
	}

	m_pParent->GetCharStun().Init();

	CCharState::OnExit();
}

int CBotState_Stunned::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	return 0;
}

int CBotState_Stunned::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_Stunned::CanTransition(BYTE stateID)
{
	if (m_pParent->IsPC())
	{
		if (stateID == CHARSTATE_DESPAWNING || stateID == CHARSTATE_STANDING || stateID == CHARSTATE_FAINTING || stateID == CHARSTATE_LEAVING
			|| stateID == CHARSTATE_KNOCKDOWN || stateID == CHARSTATE_SKILL_AFFECTING 
			|| stateID == CHARSTATE_SLEEPING || stateID == CHARSTATE_PARALYZED
			|| stateID == CHARSTATE_SANDBAG || stateID == CHARSTATE_DIRECT_PLAY)
			return true;
	}
	else
		return true;

	return false;
}

int CBotState_Stunned::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateStunned.byStunType = byStunType;

	return sizeof(sCHARSTATE_STUNNED);
}

void CBotState_Stunned::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	byStunType = pStateDetail->sCharStateStunned.byStunType;
}

