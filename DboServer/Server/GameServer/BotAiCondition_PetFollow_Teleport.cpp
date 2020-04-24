#include "stdafx.h"
#include "BotAiCondition_PetFollow_Teleport.h"
#include "SummonPet.h"


CBotAiCondition_PetFollow_Teleport::CBotAiCondition_PetFollow_Teleport(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_PET_FOLLOW_TELEPORT, "BOTCONTROL_CONDITION_PET_FOLLOW_TELEPORT")
{
}

CBotAiCondition_PetFollow_Teleport::~CBotAiCondition_PetFollow_Teleport()
{
}


int CBotAiCondition_PetFollow_Teleport::OnUpdate(DWORD dwTickDiff, float fMultiple)
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

	CCharacter* pOwner = (CCharacter*)pPet->GetOwner();
	if (pOwner == NULL || pOwner->IsInitialized() == false)
	{
		m_status = COMPLETED;
		return m_status;
	}

	if (pPet->IsInRange(pOwner, (float)CSummonPet::TELEPORT_BEGIN_DISTANCE_IN_METERS))
		return m_status;

	if (GetBot()->GetBotController()->ChangeControlState_Teleporting(pOwner->GetWorldID(), pOwner->GetWorldTblidx(), pOwner->GetCurLoc(), pOwner->GetCurDir()))
		m_status = CHANGED;
	else
		m_status = FAILED;

	return m_status;
}

