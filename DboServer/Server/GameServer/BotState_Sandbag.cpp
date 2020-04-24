#include "stdafx.h"
#include "BotState_Sandbag.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"

CBotState_Sandbag::CBotState_Sandbag(CCharacter* pParent)
	:CCharState(CHARSTATE_SANDBAG, pParent)
{
}

CBotState_Sandbag::~CBotState_Sandbag()
{
}


void CBotState_Sandbag::OnEnter()
{
	if (m_pParent->GetBotController())
	{
		m_pParent->GetBotController()->SetControlBlock(true);
	}

	if(m_pParent->IsPC())
		((CPlayer*)m_pParent)->SetHtbUseBalls(0);

	CCharState::OnEnter();
}

void CBotState_Sandbag::OnExit()
{
	m_pParent->SetHtbSkillCaster(INVALID_HOBJECT);

	if (m_pParent->GetBotController())
	{
		m_pParent->GetBotController()->SetControlBlock(false);
	}

	CCharState::OnExit();
}

int CBotState_Sandbag::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	CCharacter* pAttacker = g_pObjectManager->GetChar(m_pParent->GetHtbSkillCaster());
	if (pAttacker == NULL || pAttacker->IsInitialized() == false)
	{
		if (m_pParent->IsPC())
		{
			CNtlPacket packet2(sizeof(sGU_HTB_LAST_STEP_CHANGED_NFY));
			sGU_HTB_LAST_STEP_CHANGED_NFY * res2 = (sGU_HTB_LAST_STEP_CHANGED_NFY*)packet2.GetPacketData();
			res2->wOpCode = GU_HTB_LAST_STEP_CHANGED_NFY;
			res2->byLastStep = INVALID_BYTE;
			res2->hSubject = m_pParent->GetID();
			packet2.SetPacketLen(sizeof(sGU_HTB_LAST_STEP_CHANGED_NFY));
			m_pParent->SendPacket(&packet2);
		}

		m_pParent->SendCharStateStanding();
	}

	return 0;
}

int CBotState_Sandbag::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_Sandbag::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_SPAWNING || stateID == CHARSTATE_DESPAWNING || stateID == CHARSTATE_STANDING || stateID == CHARSTATE_FAINTING || stateID == CHARSTATE_LEAVING || stateID == CHARSTATE_KNOCKDOWN)
		return true;

	return false;
}

int CBotState_Sandbag::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	return 0;
}

void CBotState_Sandbag::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
}

