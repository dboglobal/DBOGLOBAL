#include "stdafx.h"
#include "BotState_Teleporting.h"
#include "char.h"

CBotState_Teleporting::CBotState_Teleporting(CCharacter* pParent)
	:CCharState(CHARSTATE_TELEPORTING, pParent)
{
}

CBotState_Teleporting::~CBotState_Teleporting()
{
}


void CBotState_Teleporting::OnEnter()
{
	m_pParent->Reset(CCharacterObject::NONE);

	CCharState::OnEnter();
}

void CBotState_Teleporting::OnExit()
{
	CCharState::OnExit();
}

int CBotState_Teleporting::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	return 0;
}

int CBotState_Teleporting::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return NULL;
}

bool CBotState_Teleporting::CanTransition(BYTE stateID)
{
	if (m_pParent->IsPC() == false)
		return true;

	if(stateID == CHARSTATE_SPAWNING || stateID == CHARSTATE_STANDING || stateID == CHARSTATE_LEAVING)
		return true;

	return false;
}

int CBotState_Teleporting::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateTeleporting.byTeleportType = m_pParent->GetTeleportType();

	return sizeof(sCHARSTATE_TELEPORTING);
}

void CBotState_Teleporting::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
}

