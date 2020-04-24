#include "stdafx.h"
#include "BotState_AirJump.h"
#include "char.h"

CBotState_AirJump::CBotState_AirJump(CCharacter* pParent)
	:CCharState(CHARSTATE_AIR_JUMP, pParent)
{
}

CBotState_AirJump::~CBotState_AirJump()
{
}


void CBotState_AirJump::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_AirJump::OnExit()
{
	m_pParent->SetMoveDirection(NTL_MOVE_NONE);
	m_pParent->SetMoveFlag(NTL_MOVE_FLAG_INVALID);

	CCharState::OnExit();
}

int CBotState_AirJump::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	m_pParent->UpdateMove(dwTickDiff, fMultiple);
	return 0;
}

int CBotState_AirJump::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return NULL;
}


bool CBotState_AirJump::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_SITTING || stateID == CHARSTATE_GUARD || stateID == CHARSTATE_DASH_PASSIVE)
		return false;

	return true;
}

int CBotState_AirJump::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateAirJumping.byMoveDirection = m_pParent->GetMoveDirection();
	return sizeof(sCHARSTATE_AIRJUMPING);
}

void CBotState_AirJump::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	m_pParent->SetMoveDirection(pStateDetail->sCharStateAirJumping.byMoveDirection);
	m_pParent->SetMoveFlag(NTL_MOVE_FLAG_FLY_DASH);
}

