#include "stdafx.h"
#include "BotState_DashPassive.h"
#include "char.h"

CBotState_DashPassive::CBotState_DashPassive(CCharacter* pParent)
	:CCharState(CHARSTATE_DASH_PASSIVE, pParent)
{
}

CBotState_DashPassive::~CBotState_DashPassive()
{
}


void CBotState_DashPassive::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_DashPassive::OnExit()
{
	m_pParent->SetMoveDirection(NTL_MOVE_NONE);
	m_pParent->SetMoveFlag(NTL_MOVE_FLAG_INVALID);

	CCharState::OnExit();
}

int CBotState_DashPassive::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	m_pParent->UpdateMove(dwTickDiff, fMultiple);

	return 0;
}

int CBotState_DashPassive::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_DashPassive::CanTransition(BYTE stateID)
{
	switch (stateID)
	{
		case CHARSTATE_SITTING:
		case CHARSTATE_MOVING:
		case CHARSTATE_DESTMOVE:
		case CHARSTATE_FOLLOWING:
		case CHARSTATE_DASH_PASSIVE:
		case CHARSTATE_FOCUSING:
		case CHARSTATE_CASTING:
		case CHARSTATE_SKILL_AFFECTING:
		case CHARSTATE_KEEPING_EFFECT:
		case CHARSTATE_CASTING_ITEM:
		case CHARSTATE_HTB:
		case CHARSTATE_CHARGING:
		case CHARSTATE_GUARD:
		case CHARSTATE_PRIVATESHOP:
		case CHARSTATE_OPERATING:
		case CHARSTATE_RIDEON:
		case CHARSTATE_TURNING:
		case CHARSTATE_AIR_JUMP:
		case CHARSTATE_AIR_DASH_ACCEL:
			return false;
	}

	return true;
}

int CBotState_DashPassive::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateDashPassive.byMoveDirection = m_pParent->GetMoveDirection();
	m_pParent->GetDestLoc().CopyTo(pStateDetail->sCharStateDashPassive.vDestLoc);

	return sizeof(sCHARSTATE_DASH_PASSIVE);
}

void CBotState_DashPassive::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	m_pParent->SetMoveDirection(pStateDetail->sCharStateDashPassive.byMoveDirection);
	m_pParent->SetMoveFlag(NTL_MOVE_FLAG_DASH);
	m_pParent->SetDestLoc(pStateDetail->sCharStateDashPassive.vDestLoc);
}

