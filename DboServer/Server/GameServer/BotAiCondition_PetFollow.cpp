#include "stdafx.h"
#include "BotAiCondition_PetFollow.h"
#include "SummonPet.h"
#include "BotAiAction_PetFollow.h"
#include "BotAiState.h"


CBotAiCondition_PetFollow::CBotAiCondition_PetFollow(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_PET_FOLLOW, "BOTCONTROL_CONDITION_PET_FOLLOW")
{
}

CBotAiCondition_PetFollow::~CBotAiCondition_PetFollow()
{
}


int CBotAiCondition_PetFollow::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (!GetBot()->IsMoveAble())
		return m_status;

	if (GetBot()->GetObjType() != OBJTYPE_SUMMON_PET)
	{
		m_status = FAILED;
		return m_status;
	}

	CSummonPet* pPet = dynamic_cast<CSummonPet*>(GetBot());
	if (pPet == NULL)
	{
		ERR_LOG(LOG_SYSTEM, "\"pPet\" is not CSummonPet type., GetBot() = %016x", GetBot());
		m_status = FAILED;
		return m_status;
	}

	if (pPet->IsActionStatusFlag(BOT_ACTIONSTATUS_PET_FOLLOW))
		return m_status;

	CCharacter* pOwner = (CCharacter*)pPet->GetOwner();
	if (pOwner == NULL || pOwner->IsInitialized() == false)
	{
		m_status = COMPLETED;
		return m_status;
	}

	if (pPet->IsInRange(pOwner, (float)CSummonPet::FOLLOW_BEGIN_DISTANCE_IN_METERS))
		return m_status;

	if (CBotAiState* pCurState = pPet->GetBotController()->GetCurrentState())
	{
		if (pCurState->AddSubControlList(new CBotAiAction_PetFollow(GetBot(), pOwner->GetID(), pOwner->GetCurLoc()), false) == false)
		{
			ERR_LOG(LOG_BOTAI, "false == pCurState->AddSubControlList( new CBotAiAction_PetFollow( GetBot(), pMaster->GetHandle(), pMaster->GetCurLoc() ) )");
			m_status = FAILED;
		}
		else
		{
			pPet->SetActionStatusFlag(BOT_ACTIONSTATUS_PET_FOLLOW, true);
		}
	}

	return m_status;
}

