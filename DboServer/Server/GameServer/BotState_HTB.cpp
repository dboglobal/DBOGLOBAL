#include "stdafx.h"
#include "BotState_HTB.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CBotState_HTB::CBotState_HTB(CCharacter* pParent)
	:CCharState(CHARSTATE_HTB, pParent)
{
	
}

CBotState_HTB::~CBotState_HTB()
{
}


void CBotState_HTB::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_HTB::OnExit()
{
	if (m_pParent->IsPC() && ((CPlayer*)m_pParent)->GetCurrentHtbSkill() != INVALID_BYTE)
	{
		CHtbSkill* pHtb = ((CPlayer*)m_pParent)->GetHtbSkillManager()->GetHtbSkill(((CPlayer*)m_pParent)->GetCurrentHtbSkill());
		if (pHtb && pHtb->GetTarget() != INVALID_HOBJECT)
		{
			CCharacter* pTarget = g_pObjectManager->GetChar(pHtb->GetTarget());
			if (pTarget && pTarget->GetCharStateID() == CHARSTATE_SANDBAG)
				pTarget->SendCharStateStanding();
		}
	}

	((CPlayer*)m_pParent)->SetCurrentHtbSkill(INVALID_BYTE);
	((CPlayer*)m_pParent)->SetHtbUseBalls(0);

	CCharState::OnExit();
}

int CBotState_HTB::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	return 0;
}

int CBotState_HTB::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_HTB::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_SPAWNING || stateID == CHARSTATE_DESPAWNING || stateID == CHARSTATE_STANDING || stateID == CHARSTATE_FAINTING || stateID == CHARSTATE_LEAVING)
		return true;

	return false;
}

int CBotState_HTB::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	int nSize = sizeof(sCHARSTATE_HTB);

	memcpy(pStateDetail, &m_htb, nSize);

	return nSize;
}

void CBotState_HTB::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	memcpy(&m_htb, pStateDetail, sizeof(sCHARSTATE_HTB));
}

