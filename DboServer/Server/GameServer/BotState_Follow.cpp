#include "stdafx.h"
#include "BotState_Follow.h"
#include "char.h"
#include "ObjectManager.h"


CBotState_Follow::CBotState_Follow(CCharacter* pParent)
	:CCharState(CHARSTATE_FOLLOWING, pParent)
{
}

CBotState_Follow::~CBotState_Follow()
{
}


void CBotState_Follow::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_Follow::OnExit()
{
	m_pParent->SetMoveDirection(NTL_MOVE_NONE);
	m_pParent->SetMoveFlag(NTL_MOVE_FLAG_INVALID);
	m_pParent->SetFollowTarget(INVALID_HOBJECT);

	CCharState::OnExit();
}

int CBotState_Follow::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	if (!m_pParent->UpdateFollow(dwTickDiff, fMultiple))
	{
		m_pParent->SetMoveDirection(NTL_MOVE_NONE);
		m_pParent->SetMoveFlag(NTL_MOVE_FLAG_INVALID);
		m_pParent->SetFollowTarget(INVALID_HOBJECT);

		SetNextStateID(CHARSTATE_STANDING);
		SetFinish(true);
	}


	return 0;
}

int CBotState_Follow::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_Follow::CanTransition(BYTE stateID)
{
	return true;
}

int CBotState_Follow::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateFollwing.byMoveFlag = m_pParent->GetMoveFlag();
	pStateDetail->sCharStateFollwing.hTarget = m_pParent->GetFollowTarget();
	pStateDetail->sCharStateFollwing.fDistance = m_pParent->GetFollowDistance();
	m_pParent->GetDestLoc().CopyTo(pStateDetail->sCharStateFollwing.vDestLoc);
	pStateDetail->sCharStateFollwing.byMovementReason = m_pParent->GetFollowMoveReason();

	return sizeof(sCHARSTATE_FOLLOWING);
}

void CBotState_Follow::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	m_pParent->SetMoveFlag(pStateDetail->sCharStateFollwing.byMoveFlag);
	m_pParent->SetFollowTarget(pStateDetail->sCharStateFollwing.hTarget);
	m_pParent->SetFollowDistance(pStateDetail->sCharStateFollwing.fDistance);
	m_pParent->SetMoveDirection(NTL_MOVE_FOLLOW_MOVEMENT);
	m_pParent->SetDestLoc(pStateDetail->sCharStateFollwing.vDestLoc);
	m_pParent->SetFollowMoveReason(pStateDetail->sCharStateFollwing.byMovementReason);
}

