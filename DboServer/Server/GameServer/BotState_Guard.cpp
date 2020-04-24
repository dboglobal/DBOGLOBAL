#include "stdafx.h"
#include "BotState_Guard.h"
#include "CPlayer.h"

CBotState_Guard::CBotState_Guard(CCharacter* pParent)
	:CCharState(CHARSTATE_GUARD, pParent)
{
}

CBotState_Guard::~CBotState_Guard()
{
}


void CBotState_Guard::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_Guard::OnExit()
{
	if (m_pParent->IsPC())
		((CPlayer*)m_pParent)->EndGuard();

	CCharState::OnExit();
}

int CBotState_Guard::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	if (m_pParent->IsPC())
		((CPlayer*)m_pParent)->DecreaseGuardTime(dwTickDiff);

	return 0;
}

int CBotState_Guard::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_Guard::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_SITTING || stateID == CHARSTATE_DASH_PASSIVE || stateID == CHARSTATE_RIDEON || stateID == CHARSTATE_AIR_JUMP)
		return false;

	return true;
}

int CBotState_Guard::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	return NULL;
}

void CBotState_Guard::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
}

