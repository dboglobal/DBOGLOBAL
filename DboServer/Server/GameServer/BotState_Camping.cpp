#include "stdafx.h"
#include "BotState_Camping.h"

CBotState_Camping::CBotState_Camping(CCharacter* pParent)
	:CCharState(CHARSTATE_CAMPING, pParent)
{
}

CBotState_Camping::~CBotState_Camping()
{
}


void CBotState_Camping::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_Camping::OnExit()
{
	CCharState::OnExit();
}

int CBotState_Camping::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	return 0;
}

int CBotState_Camping::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}


bool CBotState_Camping::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_GUARD || stateID == CHARSTATE_AIR_JUMP)
		return false;

	return true;
}

int CBotState_Camping::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	return 0;
}

void CBotState_Camping::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
}

