#include "stdafx.h"
#include "ConditionState_AfterEffect.h"
#include "NtlRandom.h"
#include "char.h"

CConditionState_AfterEffect::CConditionState_AfterEffect(CCharacter *pParent)
	:CConditionState(CHARCOND_AFTEREFFECT, pParent)
{
	m_dwDuration = 5000;
}

CConditionState_AfterEffect::~CConditionState_AfterEffect()
{
}



void CConditionState_AfterEffect::OnEnter()
{
	m_pParent->StandUpIfPossible(m_pParent->GetCharStateID());

	m_dwDuration = RandomRangeU(5000, 10000);
}

int CConditionState_AfterEffect::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwDuration = UnsignedSafeDecrease<DWORD>(m_dwDuration, dwTickDiff);
	if (m_dwDuration == 0)
		return 1;

	return 0;
}