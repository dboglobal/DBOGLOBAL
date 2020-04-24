#include "stdafx.h"
#include "ConditionState_Rabies.h"


CConditionState_Rabies::CConditionState_Rabies(CCharacter *pParent)
	:CConditionState(CHARCOND_RABIES, pParent)
{
}

CConditionState_Rabies::~CConditionState_Rabies()
{
}

void CConditionState_Rabies::OnEnter()
{
}

void CConditionState_Rabies::OnExit()
{
}

int CConditionState_Rabies::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return 0;
}

