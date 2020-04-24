#include "stdafx.h"
#include "BotState_DirectPlay.h"
#include "char.h"


CBotState_DirectPlay::CBotState_DirectPlay(CCharacter* pParent)
	:CCharState(CHARSTATE_DIRECT_PLAY, pParent)
{
	m_byDirectPlayType = INVALID_BYTE;
	m_directTblidx = INVALID_TBLIDX;
}

CBotState_DirectPlay::~CBotState_DirectPlay()
{
}


void CBotState_DirectPlay::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_DirectPlay::OnExit()
{
	CCharState::OnExit();
}

int CBotState_DirectPlay::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	return 0;
}

int CBotState_DirectPlay::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return NULL;
}

bool CBotState_DirectPlay::CanTransition(BYTE stateID)
{
	if (m_pParent->IsPC())
	{
		if (stateID == CHARSTATE_SPAWNING || stateID == CHARSTATE_DESPAWNING || stateID == CHARSTATE_STANDING)
			return true;
	}
	else return true;

	return false;
}

int CBotState_DirectPlay::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateDirectPlay.byDirectPlayType = m_byDirectPlayType;
	pStateDetail->sCharStateDirectPlay.directTblidx = m_directTblidx;
	return sizeof(sCHARSTATE_DIRECT_PLAY);
}

void CBotState_DirectPlay::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	m_byDirectPlayType = pStateDetail->sCharStateDirectPlay.byDirectPlayType;
	m_directTblidx = pStateDetail->sCharStateDirectPlay.directTblidx;
}

