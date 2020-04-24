#include "stdafx.h"
#include "AspectState_PureMajin.h"
#include "CPlayer.h"
#include "StatusTransformTable.h"


CAspectState_PureMajin::CAspectState_PureMajin(CCharacter *pParent)
	:CAspectState(ASPECTSTATE_PURE_MAJIN, pParent)
{
}

CAspectState_PureMajin::~CAspectState_PureMajin()
{
}

void CAspectState_PureMajin::OnEnter()
{
	m_pParent->ChangeModelData(DBO_PURE_MAJIN_MODEL_RESOURCE_NAME);

	CAspectState::OnEnter();
}

void CAspectState_PureMajin::OnExit()
{
	CPlayer* pParentPc = (CPlayer*)m_pParent;

	if (pParentPc->IsAdult())
	{
		pParentPc->ChangeModelData(pParentPc->GetTbldat()->szModel_Adult);
	}
	else
	{
		pParentPc->ChangeModelData(pParentPc->GetTbldat()->szModel_Child);
	}

	CAspectState::OnExit();
}


int CAspectState_PureMajin::OnUpdate(DWORD dwTickDiff, float fMultiple)
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
