#include "stdafx.h"
#include "BotState_Sleeping.h"
#include "char.h"

CBotState_Sleeping::CBotState_Sleeping(CCharacter* pParent)
	:CCharState(CHARSTATE_SLEEPING, pParent)
{
}

CBotState_Sleeping::~CBotState_Sleeping()
{
}


void CBotState_Sleeping::OnEnter()
{
	if (m_pParent->GetBotController())
	{
		m_pParent->GetBotController()->SetControlBlock(true);
	}

	CCharState::OnEnter();
}

void CBotState_Sleeping::OnExit()
{
	if (m_pParent->GetBotController())
	{
		m_pParent->GetBotController()->SetControlBlock(false);
	}

	CCharState::OnExit();
}

int CBotState_Sleeping::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);
	//printf("CBotState_Sleeping::OnUpdate sleeping \n");

	return 0;
}

int CBotState_Sleeping::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_Sleeping::CanTransition(BYTE stateID)
{
	if (m_pParent->IsPC())
	{
		if (stateID == CHARSTATE_DESPAWNING || stateID == CHARSTATE_STANDING || stateID == CHARSTATE_FAINTING || stateID == CHARSTATE_LEAVING
			|| stateID == CHARSTATE_TELEPORTING || stateID == CHARSTATE_KNOCKDOWN || stateID == CHARSTATE_STUNNED || stateID == CHARSTATE_PARALYZED
			|| stateID == CHARSTATE_SANDBAG || stateID == CHARSTATE_DIRECT_PLAY)
			return true;
	}
	else
		return true;

	return false;
}

int CBotState_Sleeping::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	return NULL;
}

void CBotState_Sleeping::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
}

