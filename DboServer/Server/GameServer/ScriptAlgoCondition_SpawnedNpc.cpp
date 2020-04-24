#include "stdafx.h"
#include "ScriptAlgoCondition_SpawnedNpc.h"
#include "WPSNodeCondition_SpawnedNpc.h"
#include "Npc.h"


CWpsAlgoCondition_SpawnedNpc::CWpsAlgoCondition_SpawnedNpc(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_SPAWNED_NPC, "SCRIPTCONTROL_CONDITION_SPAWNED_NPC")
{
}


CWpsAlgoCondition_SpawnedNpc::~CWpsAlgoCondition_SpawnedNpc()
{
}


bool CWpsAlgoCondition_SpawnedNpc::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_SpawnedNpc* pCond = dynamic_cast<CWPSNodeCondition_SpawnedNpc*>(pControlScriptNode);
	if (pCond)
	{
		m_npcTblidx = pCond->m_npcTblidx;

		return true;
	}

	return false;
}

int CWpsAlgoCondition_SpawnedNpc::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CNpc* pNpc = GetOwner()->FindNpc(m_npcTblidx);

	if (pNpc == NULL && GetOwner()->GetWorld())
	{
		pNpc = GetOwner()->GetWorld()->FindNpc(m_npcTblidx);
	}

	if (pNpc && pNpc->IsInitialized())
		m_status = COMPLETED;

	return m_status;
}
