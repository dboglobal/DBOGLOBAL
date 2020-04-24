#include "stdafx.h"
#include "CharState.h"
#include "CPlayer.h"

CCharState::CCharState(BYTE stateID, CCharacter* pParent)
	:CStateBase(stateID)
{
	m_pParent = pParent;
}

CCharState::~CCharState()
{

}

void CCharState::OnEnter()
{
	if (GetPrevStateID() == CHARSTATE_KEEPING_EFFECT)
	{
		m_pParent->RemoveKeepEffectTargets(); //release targets with next tick to avoid endless loop
	}
}

int CCharState::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if(m_pParent->IsPC())
		((CPlayer*)m_pParent)->AfkCheck(dwTickDiff);

	return 0;
}
