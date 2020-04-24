#include "stdafx.h"
#include "BotState_PrivateShop.h"
#include "CPlayer.h"
#include "privateshop.h"


CBotState_PrivateShop::CBotState_PrivateShop(CCharacter* pParent)
	:CCharState(CHARSTATE_PRIVATESHOP, pParent)
{
}

CBotState_PrivateShop::~CBotState_PrivateShop()
{
}


void CBotState_PrivateShop::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_PrivateShop::OnExit()
{
	CCharState::OnExit();
}

int CBotState_PrivateShop::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);
	//((void(__fastcall *)(CBot *, _QWORD))v7->m_pParent->vfptr[4].__vecDelDtor)(v7->m_pParent, dwTickTime);
	//((void(__fastcall *)(CBot *, _QWORD))v7->m_pParent->vfptr[3].SendPacket)(v7->m_pParent, v8);
	return 0;
}

int CBotState_PrivateShop::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_PrivateShop::CanTransition(BYTE stateID)
{
	if(stateID == CHARSTATE_STANDING)
		return true;

	return false;
}

int CBotState_PrivateShop::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	CPrivateShop* pShop = ((CPlayer*)m_pParent)->GetPrivateShop();
	if (pShop)
	{
		pStateDetail->sCharStatePrivateShop.sSummaryPrivateShopData.bIsOwnerEmpty = pShop->GetShopData()->sSummaryPrivateShopData.bIsOwnerEmpty;
		pStateDetail->sCharStatePrivateShop.sSummaryPrivateShopData.byShopState = pShop->GetShopData()->sSummaryPrivateShopData.byShopState;
		wcscpy_s(pStateDetail->sCharStatePrivateShop.sSummaryPrivateShopData.wcPrivateShopName, NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1, pShop->GetShopData()->sSummaryPrivateShopData.wcPrivateShopName);
	}

	return NULL;
}

void CBotState_PrivateShop::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
}

