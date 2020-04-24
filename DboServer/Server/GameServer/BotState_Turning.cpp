#include "stdafx.h"
#include "BotState_Turning.h"
#include "char.h"


CBotState_Turning::CBotState_Turning(CCharacter* pParent)
	:CCharState(CHARSTATE_TURNING, pParent)
{
	m_directTblidx = 0;
	m_vDestDir.x = 0.f;
	m_vDestDir.y = 0.f;
	m_vDestDir.z = 0.f;
}

CBotState_Turning::~CBotState_Turning()
{
}


void CBotState_Turning::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_Turning::OnExit()
{
	CObjMsg_CharTurningEnd pMsg;
	pMsg.directTblidx = m_directTblidx;
	m_pParent->SendObjectMsg(&pMsg);

	CCharState::OnExit();
}

int CBotState_Turning::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	return 0;
}

int CBotState_Turning::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_Turning::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_AIR_JUMP)
		return false;

	return true;
}

int CBotState_Turning::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateTurning.directTblidx = m_directTblidx;
	pStateDetail->sCharStateTurning.vDestDir = m_vDestDir;

	return sizeof(sCHARSTATE_TURNING);
}

void CBotState_Turning::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	m_directTblidx = pStateDetail->sCharStateTurning.directTblidx;
	m_vDestDir = pStateDetail->sCharStateTurning.vDestDir;
}

