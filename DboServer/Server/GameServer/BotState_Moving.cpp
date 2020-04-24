#include "stdafx.h"
#include "BotState_Moving.h"
#include "char.h"

CBotState_Moving::CBotState_Moving(CCharacter* pParent)
	:CCharState(CHARSTATE_MOVING, pParent)
{
}

CBotState_Moving::~CBotState_Moving()
{
}


void CBotState_Moving::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_Moving::OnExit()
{
	m_pParent->SetMoveDirection(NTL_MOVE_NONE);
	m_pParent->SetMoveFlag(NTL_MOVE_FLAG_INVALID);

	CCharState::OnExit();
}

int CBotState_Moving::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	m_pParent->UpdateMove(dwTickDiff, fMultiple);

	return 0;
}

int CBotState_Moving::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_Moving::CanTransition(BYTE stateID)
{
	return true;
}

int CBotState_Moving::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateMoving.byMoveDirection = m_pParent->GetMoveDirection();
	pStateDetail->sCharStateMoving.byMoveFlag = m_pParent->GetMoveFlag();

	return sizeof(sCHARSTATE_MOVING);
}

void CBotState_Moving::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	m_pParent->SetMoveDirection(pStateDetail->sCharStateMoving.byMoveDirection);
	m_pParent->SetMoveFlag(pStateDetail->sCharStateMoving.byMoveFlag);
}

