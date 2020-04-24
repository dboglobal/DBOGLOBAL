#include "stdafx.h"
#include "ConditionState.h"

CConditionState::CConditionState(BYTE stateID, CCharacter *pParent)
	:CStateBase(stateID)
{
	m_dwReferCount = 0;
	m_pParent = pParent;
}

CConditionState::~CConditionState()
{
}
