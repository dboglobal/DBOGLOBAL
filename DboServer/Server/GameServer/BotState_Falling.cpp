#include "stdafx.h"
#include "BotState_Falling.h"
#include "char.h"

CBotState_Falling::CBotState_Falling(CCharacter* pParent)
	:CCharState(CHARSTATE_FALLING, pParent)
{
}

CBotState_Falling::~CBotState_Falling()
{
}


void CBotState_Falling::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_Falling::OnExit()
{
	CCharState::OnExit();
}

int CBotState_Falling::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);
	//((void(__fastcall *)(CBot *, _QWORD))v7->m_pParent->vfptr[4].__vecDelDtor)(v7->m_pParent, v8);
	//((void(__fastcall *)(CBot *, _QWORD))v7->m_pParent->vfptr[3].SendPacket)(v7->m_pParent, v8);

	return 0;
}

int CBotState_Falling::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return NULL;
}

bool CBotState_Falling::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_SPAWNING || stateID == CHARSTATE_DESPAWNING || stateID == CHARSTATE_STANDING || stateID == CHARSTATE_LEAVING || stateID == CHARSTATE_TELEPORTING)
		return true;

	return false;
}

int CBotState_Falling::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateFalling.byMoveDirection = m_pParent->GetMoveDirection();
	pStateDetail->sCharStateFalling.byMoveFlag = m_pParent->GetMoveFlag();
	return sizeof(sCHARSTATE_FALLING);
}

void CBotState_Falling::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	m_pParent->SetMoveDirection(pStateDetail->sCharStateFalling.byMoveDirection);
	m_pParent->SetMoveFlag(pStateDetail->sCharStateFalling.byMoveFlag);
}

