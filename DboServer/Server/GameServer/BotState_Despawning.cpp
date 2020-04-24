#include "stdafx.h"
#include "BotState_Despawning.h"
#include "char.h"

CBotState_Despawning::CBotState_Despawning(CCharacter* pParent)
	:CCharState(CHARSTATE_DESPAWNING, pParent)
{
}

CBotState_Despawning::~CBotState_Despawning()
{
}


void CBotState_Despawning::OnEnter()
{
	m_pParent->Reset(CCharacterObject::NONE);

	if (m_pParent->GetBotController())
	{
		if (m_pParent->GetBotController()->IsCurrentStateID(BOTCONTROL_TELEPORTING))
		{
			/*CEvent_CHAR_TELEPORT::CEvent_CHAR_TELEPORT(&v6);
			v6.handle = CGameObject::GetHandle((CGameObject *)&v8->m_pParent->vfptr);
			((void(__fastcall *)(CBot *, CEvent_CHAR_TELEPORT *))v8->m_pParent->vfptr->OnEvent)(v8->m_pParent, &v6);
			CEvent_CHAR_TELEPORT::~CEvent_CHAR_TELEPORT(&v6);*/
		}
		else
		{
			m_pParent->GetBotController()->ChangeControlState_Despawn();
		}
	}

	CCharState::OnEnter();
}

void CBotState_Despawning::OnExit()
{
	CCharState::OnExit();
}

int CBotState_Despawning::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	return 0;
}

int CBotState_Despawning::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return NULL;
}

bool CBotState_Despawning::CanTransition(BYTE stateID)
{
	if (m_pParent->IsPC() == false)
		return true;

	if (stateID == CHARSTATE_LEAVING || stateID == CHARSTATE_TELEPORTING)
		return true;

	return false;
}

int CBotState_Despawning::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateDespawning.byTeleportType = m_pParent->GetTeleportType();
	return sizeof(sCHARSTATE_DESPAWNING);
}

void CBotState_Despawning::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	BYTE byTeleportType = m_pParent->GetTeleportType();

	//dont do it in OnEnter or state will be reset to 0
	if (m_pParent->IsPC() && byTeleportType != TELEPORT_TYPE_RANKBATTLE
		&& byTeleportType != TELEPORT_TYPE_BUDOKAI && byTeleportType != TELEPORT_TYPE_MATCH && byTeleportType != TELEPORT_TYPE_MINORMATCH && byTeleportType != TELEPORT_TYPE_MAJORMATCH && byTeleportType != TELEPORT_TYPE_FINALMATCH)
	{
		m_pParent->StartInvincibleEvent();
	}
}

