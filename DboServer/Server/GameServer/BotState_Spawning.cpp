#include "stdafx.h"
#include "BotState_Spawning.h"
#include "char.h"
#include "NtlPacketGU.h"

CBotState_Spawning::CBotState_Spawning(CCharacter* pParent)
	:CCharState(CHARSTATE_SPAWNING, pParent)
{
}

CBotState_Spawning::~CBotState_Spawning()
{
}


void CBotState_Spawning::OnEnter()
{
	if (m_pParent->GetBotController())
	{
		if (m_pParent->GetBotController()->IsCurrentStateID(BOTCONTROL_TELEPORTING))
		{
			/*CEvent_CHAR_TELEPORT::CEvent_CHAR_TELEPORT(&v5);
			v5.handle = CGameObject::GetHandle((CGameObject *)&v7->m_pParent->vfptr);
			((void(__fastcall *)(CBot *, CEvent_CHAR_TELEPORT *))v7->m_pParent->vfptr->OnEvent)(v7->m_pParent, &v5);
			CEvent_CHAR_TELEPORT::~CEvent_CHAR_TELEPORT(&v5);*/
		}
	}

	if (m_pParent->IsPC() && GetPrevStateID() == CHARSTATE_HTB)
	{
		CNtlPacket packet2(sizeof(sGU_HTB_LAST_STEP_CHANGED_NFY));
		sGU_HTB_LAST_STEP_CHANGED_NFY * res2 = (sGU_HTB_LAST_STEP_CHANGED_NFY*)packet2.GetPacketData();
		res2->wOpCode = GU_HTB_LAST_STEP_CHANGED_NFY;
		res2->byLastStep = INVALID_BYTE;
		res2->hSubject = m_pParent->GetID();
		packet2.SetPacketLen(sizeof(sGU_HTB_LAST_STEP_CHANGED_NFY));
		m_pParent->Broadcast(&packet2);
	}

	CCharState::OnEnter();
}

void CBotState_Spawning::OnExit()
{
	CCharState::OnExit();
}

int CBotState_Spawning::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	return 0;
}

int CBotState_Spawning::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_Spawning::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_RIDEON || stateID == CHARSTATE_AIR_JUMP)
		return false;

	return true;
}

int CBotState_Spawning::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateSpawning.bSpawnDirection = m_bSpawnDirection;
	pStateDetail->sCharStateSpawning.bIsFaint = m_bIsFaint;
	pStateDetail->sCharStateSpawning.byTeleportType = m_byTeleportType;
	return sizeof(sCHARSTATE_SPAWNING);
}

void CBotState_Spawning::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	m_bSpawnDirection = pStateDetail->sCharStateSpawning.bSpawnDirection;
	m_bIsFaint = pStateDetail->sCharStateSpawning.bIsFaint;
	m_byTeleportType = pStateDetail->sCharStateSpawning.byTeleportType;
	
	//dont do it in OnEnter or state will be reset to 0
	if (m_pParent->IsPC() && m_byTeleportType != TELEPORT_TYPE_RANKBATTLE 
		&& m_byTeleportType != TELEPORT_TYPE_BUDOKAI && m_byTeleportType != TELEPORT_TYPE_MATCH && m_byTeleportType != TELEPORT_TYPE_MINORMATCH && m_byTeleportType != TELEPORT_TYPE_MAJORMATCH && m_byTeleportType != TELEPORT_TYPE_FINALMATCH)
	{
		m_pParent->StartInvincibleEvent();
	}
}

