#include "stdafx.h"
#include "AspectState_SuperSaiyan.h"
#include "CPlayer.h"
#include "StatusTransformTable.h"


CAspectState_SuperSaiyan::CAspectState_SuperSaiyan(CCharacter *pParent)
	:CAspectState(ASPECTSTATE_SUPER_SAIYAN, pParent)
{
}

CAspectState_SuperSaiyan::~CAspectState_SuperSaiyan()
{
}



void CAspectState_SuperSaiyan::OnExit()
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

		if(m_pParent->IsSleeping() == false)
			m_pParent->GetStateManager()->AddConditionState(CHARCOND_AFTEREFFECT, NULL, true);
	}

	CAspectState::OnExit();
}

int CAspectState_SuperSaiyan::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwLastTransformEffect += dwTickDiff;

	if (m_dwLastTransformEffect >= NTL_EFFECT_APPLY_INTERVAL)
	{
		CPlayer* pPlayer = (CPlayer*)m_pParent;
		m_dwLastTransformEffect = 0;

		float fMultiplier = pPlayer->GetTransformationTbldat()->fEP_Consume_Rate / 100.0f;

		float fEp = (float)pPlayer->GetMaxEP() * fMultiplier;

		pPlayer->UpdateCurEP((WORD)fEp, false, false);

		if (pPlayer->ConsiderEPLow(10.0f)) //check percent
			pPlayer->CancelTransformation();
	}

	return TRUE;
}
