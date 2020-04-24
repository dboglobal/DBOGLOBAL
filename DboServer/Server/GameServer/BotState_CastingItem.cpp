#include "stdafx.h"
#include "BotState_CastingItem.h"
#include "CPlayer.h"
#include "item.h"
#include "NtlPacketGU.h"
#include "GameServer.h"
#include "DojoWar.h"
#include "DojoManager.h"


CBotState_CastingItem::CBotState_CastingItem(CCharacter* pParent)
	:CCharState(CHARSTATE_CASTING_ITEM, pParent)
{
}

CBotState_CastingItem::~CBotState_CastingItem()
{
}


void CBotState_CastingItem::OnEnter()
{
	CGameServer* app = (CGameServer*)g_pApp;
	if (m_itemTblidx == 19451 && app->IsDojoChannel())
	{
		if (CDojo * pDojo = g_pDojoWarManager->GetDojo(((CPlayer*)m_pParent)->GetGuildID()))
		{
			pDojo->SendSealUnderAttack(m_hTarget);
		}
	}

	CCharState::OnEnter();
}

void CBotState_CastingItem::OnExit()
{
	if (m_dwCastingTimeRemaining > 0)
	{
		CPlayer* pPlayer = (CPlayer*)m_pParent;
		CItem* pItem = pPlayer->GetPlayerItemContainer()->GetItem(pPlayer->GetItemCast());
		if (pItem)
		{
			pItem->CancelCasting();
		}

		CGameServer* app = (CGameServer*)g_pApp;
		if (m_itemTblidx == 19451 && app->IsDojoChannel())
		{
			if (CDojo * pDojo = g_pDojoWarManager->GetDojo(((CPlayer*)m_pParent)->GetGuildID()))
			{
				pDojo->SendStopSealUnderAttack(m_hTarget);
			}
		}
	}

	CCharState::OnExit();
}

int CBotState_CastingItem::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	if (m_dwCastingTimeRemaining > dwTickDiff)
		m_dwCastingTimeRemaining -= dwTickDiff;
	else
	{
		m_dwCastingTimeRemaining = 0;

		m_pParent->SendCharStateStanding();

		CPlayer* pPlayer = (CPlayer*)m_pParent;
		CItem* pItem = pPlayer->GetPlayerItemContainer()->GetItem(pPlayer->GetItemCast());
		if (pItem)
		{
			pItem->Cast();
		}
	}

	return NULL;
}

int CBotState_CastingItem::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return NULL;
}

bool CBotState_CastingItem::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_GUARD || stateID == CHARSTATE_RIDEON || stateID == CHARSTATE_AIR_JUMP)
		return false;

	return true;
}

int CBotState_CastingItem::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateCastingItem.itemTblidx = m_itemTblidx;
	pStateDetail->sCharStateCastingItem.hTarget = m_hTarget;
	pStateDetail->sCharStateCastingItem.dwCastingTime = m_dwCastingTime;
	pStateDetail->sCharStateCastingItem.dwCastingTimeRemaining = m_dwCastingTimeRemaining;

	return sizeof(sCHARSTATE_CASTING_ITEM);
}

void CBotState_CastingItem::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	m_itemTblidx = pStateDetail->sCharStateCastingItem.itemTblidx;
	m_hTarget = pStateDetail->sCharStateCastingItem.hTarget;
	m_dwCastingTime = pStateDetail->sCharStateCastingItem.dwCastingTime;
	m_dwCastingTimeRemaining = pStateDetail->sCharStateCastingItem.dwCastingTimeRemaining;
}

