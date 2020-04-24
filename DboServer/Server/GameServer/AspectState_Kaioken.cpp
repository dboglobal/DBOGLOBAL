#include "stdafx.h"
#include "AspectState_Kaioken.h"
#include "CPlayer.h"
#include "StatusTransformTable.h"


CAspectState_Kaioken::CAspectState_Kaioken(CCharacter *pParent)
	:CAspectState(ASPECTSTATE_KAIOKEN, pParent)
{
}

CAspectState_Kaioken::~CAspectState_Kaioken()
{
}


int CAspectState_Kaioken::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwLastTransformEffect += dwTickDiff;

	if (m_dwLastTransformEffect >= NTL_EFFECT_APPLY_INTERVAL)
	{
		CPlayer* pPlayer = (CPlayer*)m_pParent;
		m_dwLastTransformEffect = 0;

		float fMultiplier = ((float)pPlayer->GetTransformGrade() * pPlayer->GetTransformationTbldat()->fLP_Consume_Rate) / 100.0f;

		float fLp = (float)pPlayer->GetMaxLP() * fMultiplier;
		float fEp = (float)pPlayer->GetMaxEP() * fMultiplier;

		pPlayer->UpdateCurLpEp((int)fLp, (WORD)fEp, false, false);

		if (pPlayer->ConsiderLPLow(10.0f)) //check percent
			pPlayer->CancelTransformation();
	}

	return TRUE;
}
