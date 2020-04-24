#include "stdafx.h"
#include "TqsAlgoAction_Teleport.h"
#include "TQSNodeAction_Teleport.h"
#include "CPlayer.h"


CTqsAlgoAction_Teleport::CTqsAlgoAction_Teleport(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_TELEPORT, "TQS_ALGOCONTROLID_ACTION_TELEPORT")
{
}


CTqsAlgoAction_Teleport::~CTqsAlgoAction_Teleport()
{
}


bool CTqsAlgoAction_Teleport::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_Teleport* pNode = dynamic_cast<CTQSNodeAction_Teleport*>(pControlScriptNode);
	if (pNode)
	{
		m_vLoc = pNode->m_vLoc;
		m_vDir = pNode->m_vDir;

		return true;
	}
	return false;
}

void CTqsAlgoAction_Teleport::OnEnter()
{
	CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
	while (pPlayer)
	{
		if (pPlayer->GetWorldID() == GetOwner()->GetWorld()->GetID()) //check if player not in world or not in range
		{
			pPlayer->StartTeleport(m_vLoc, m_vDir, GetOwner()->GetWorld()->GetID(), TELEPORT_TYPE_TMQ_PORTAL);
		}

		pPlayer = GetOwner()->GetPlayersNext();
	}
}

int CTqsAlgoAction_Teleport::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	//Complete when everyone finish with teleporting

	CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
	while (pPlayer)
	{
		if (pPlayer->GetWorldID() == GetOwner()->GetWorld()->GetID()) //check if player not in world or not in range
		{
			if (pPlayer->GetCharStateID() == CHARSTATE_DESPAWNING || pPlayer->GetCharStateID() == CHARSTATE_TELEPORTING || pPlayer->GetTeleportWorldID() != INVALID_WORLDID) //check if pPlayer is still teleporting
				return m_status;
		}
		else return m_status;

		pPlayer = GetOwner()->GetPlayersNext();
	}

	m_status = COMPLETED;
	return m_status;
}
