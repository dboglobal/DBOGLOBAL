#include "stdafx.h"
#include "BotState_Fainting.h"
#include "CPlayer.h"
#include "ObjectManager.h"
#include "BudokaiManager.h"

CBotState_Fainting::CBotState_Fainting(CCharacter* pParent)
	:CCharState(CHARSTATE_FAINTING, pParent)
{
}

CBotState_Fainting::~CBotState_Fainting()
{
}


void CBotState_Fainting::OnEnter()
{
	CCharacter* pTarget = g_pObjectManager->GetChar(m_pParent->GetTargetHandle());
	if (pTarget)
	{
		pTarget->GetBotBreakUp()->UnsetBreakUp(m_pParent->GetID());
	}

	m_pParent->Reset(CCharacterObject::FAINT);

	if (m_pParent->GetBotController())
	{
		m_pParent->GetBotController()->ChangeControlState_Faint();
	}

	//if (m_pParent->GetObjType() == OBJTYPE_NPC || m_pParent->GetObjType() == OBJTYPE_MOB) //should we really only clear aggro from npc and mob when they die?
	{
		m_pParent->GetTargetListManager()->ClearAggro();
	}

	CCharState::OnEnter();
}

void CBotState_Fainting::OnExit()
{
	CCharState::OnExit();
}

int CBotState_Fainting::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	//((void(__fastcall *)(CBot *, _QWORD))v7->m_pParent->vfptr[4].__vecDelDtor)(v7->m_pParent, dwTickTime);
	//((void(__fastcall *)(CBot *, _QWORD))v7->m_pParent->vfptr[3].SendPacket)(v7->m_pParent, v8);

	return 0;
}

int CBotState_Fainting::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_Fainting::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_SPAWNING || stateID == CHARSTATE_DESPAWNING || stateID == CHARSTATE_TELEPORTING) //Teleporting is required.. If char die while despawning modus he will be invincible then
		return true;

	return false;
}

int CBotState_Fainting::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	return 0;
}

void CBotState_Fainting::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
}

