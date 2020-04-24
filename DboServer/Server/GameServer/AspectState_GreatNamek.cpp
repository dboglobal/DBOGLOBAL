#include "stdafx.h"
#include "AspectState_GreatNamek.h"
#include "CPlayer.h"
#include "StatusTransformTable.h"


CAspectState_GreatNamek::CAspectState_GreatNamek(CCharacter *pParent)
	:CAspectState(ASPECTSTATE_GREAT_NAMEK, pParent)
{
}

CAspectState_GreatNamek::~CAspectState_GreatNamek()
{
}


void CAspectState_GreatNamek::OnEnter()
{
	CAspectState::OnEnter();

	CPlayer* pPlayer = (CPlayer*)m_pParent;
	
	if (pPlayer->GetTransformationTbldat())
		m_dwDuration = pPlayer->GetTransformationTbldat()->dwDuration * 1000;
	else
	{
		ERR_LOG(LOG_USER, "Could not find great namek transformation tbldat");
		m_dwDuration = 0;
	}
}


void CAspectState_GreatNamek::OnExit()
{
	CPlayer* pPlayer = (CPlayer*)m_pParent;

	if (pPlayer->IsAdult())
	{
		pPlayer->ChangeModelData(pPlayer->GetTbldat()->szModel_Adult);
	}
	else
	{
		pPlayer->ChangeModelData(pPlayer->GetTbldat()->szModel_Child);
	}

	CAspectState::OnExit();
}


int CAspectState_GreatNamek::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwDuration = UnsignedSafeDecrease<DWORD>(m_dwDuration, dwTickDiff);

	if (m_dwDuration == 0)
	{
		CPlayer* pPlayer = (CPlayer*)m_pParent;
		
		pPlayer->CancelTransformation();
	}

	return TRUE;
}