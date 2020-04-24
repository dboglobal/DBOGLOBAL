#include "stdafx.h"
#include "BotState_Paralyzed.h"
#include "char.h"

CBotState_Paralyzed::CBotState_Paralyzed(CCharacter* pParent)
	:CCharState(CHARSTATE_PARALYZED, pParent)
{
}

CBotState_Paralyzed::~CBotState_Paralyzed()
{
}


void CBotState_Paralyzed::OnEnter()
{
	if (m_pParent->GetBotController())
	{
		m_pParent->GetBotController()->SetControlBlock(true);
	}

	CCharState::OnEnter();
}

void CBotState_Paralyzed::OnExit()
{
	if (m_pParent->GetBotController())
	{
		m_pParent->GetBotController()->SetControlBlock(false);
	}

	CCharState::OnExit();
}

int CBotState_Paralyzed::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	return 0;
}

int CBotState_Paralyzed::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_Paralyzed::CanTransition(BYTE stateID)
{
	/*
		- Can not get knocked down when paralyzed
	*/

	if (stateID == CHARSTATE_KNOCKDOWN)
		return false;

	if (m_pParent->IsPC())
	{
		if (stateID == CHARSTATE_DESPAWNING || stateID == CHARSTATE_STANDING || stateID == CHARSTATE_FAINTING || stateID == CHARSTATE_LEAVING
			|| stateID == CHARSTATE_TELEPORTING || stateID == CHARSTATE_FOCUSING || stateID == CHARSTATE_CASTING || stateID == CHARSTATE_SKILL_AFFECTING 
			|| stateID == CHARSTATE_STUNNED || stateID == CHARSTATE_SLEEPING
			|| stateID == CHARSTATE_SANDBAG || stateID == CHARSTATE_DIRECT_PLAY)
			return true;
	}
	else 
	{ 
		return true;
	}

	return false;
}

int CBotState_Paralyzed::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	return 0;
}

void CBotState_Paralyzed::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
}

