#include "stdafx.h"
#include "BotState_RideOn.h"
#include "CPlayer.h"
#include "ObjectManager.h"


CBotState_RideOn::CBotState_RideOn(CCharacter* pParent)
	:CCharState(CHARSTATE_RIDEON, pParent)
{
}

CBotState_RideOn::~CBotState_RideOn()
{
}


void CBotState_RideOn::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_RideOn::OnExit()
{
	m_hTarget = INVALID_HOBJECT;

	CPlayer* pPlayer = (CPlayer*)m_pParent;
	if (pPlayer)
	{
		pPlayer->SetBusID(INVALID_HOBJECT);
	}

	CCharState::OnExit();
}

int CBotState_RideOn::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	CCharacter* pTarget = g_pObjectManager->GetChar(m_hTarget);
	if (pTarget)
		m_pParent->SetCurLoc(pTarget->GetCurLoc(), m_pParent->GetCurWorld());

	return 0;
}

int CBotState_RideOn::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_RideOn::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_STANDING)
		return true;

	return false;
}

int CBotState_RideOn::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateRideOn.hTarget = m_hTarget;

	return 4;
}

void CBotState_RideOn::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	m_hTarget = pStateDetail->sCharStateRideOn.hTarget;
}

