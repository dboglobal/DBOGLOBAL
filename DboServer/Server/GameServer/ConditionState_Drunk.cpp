#include "stdafx.h"
#include "ConditionState_Drunk.h"
#include "CPlayer.h"


CConditionState_Drunk::CConditionState_Drunk(CCharacter *pParent)
	:CConditionState(CHARCOND_DRUNK, pParent)
{
}

CConditionState_Drunk::~CConditionState_Drunk()
{
}

void CConditionState_Drunk::OnExit()
{
	if (m_pParent->IsInitialized())
	{
		if (m_pParent->GetCharStateID() == CHARSTATE_HTB)
		{
			m_pParent->SendCharStateStanding();
		}

		m_pParent->ChangeAttackProgress(false);
		m_pParent->SetFightMode(false);

		if (m_pParent->IsPC())
			((CPlayer*)m_pParent)->SetAttackTarget(INVALID_HOBJECT);

		if (m_pParent->IsSleeping() == false)
			m_pParent->GetStateManager()->AddConditionState(CHARCOND_AFTEREFFECT, NULL, true);
	}
}

