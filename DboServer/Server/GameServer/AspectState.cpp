#include "stdafx.h"
#include "AspectState.h"

CAspectState::CAspectState(BYTE stateID, CCharacter *pParent)
	:CStateBase(stateID)
{
	m_pParent = pParent;
	m_dwLastTransformEffect = 0;
}

CAspectState::~CAspectState()
{
}
