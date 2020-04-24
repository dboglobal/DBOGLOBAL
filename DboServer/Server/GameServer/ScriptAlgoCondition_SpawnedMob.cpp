#include "stdafx.h"
#include "ScriptAlgoCondition_SpawnedMob.h"
#include "WPSNodeCondition_SpawnedMob.h"


CWpsAlgoCondition_SpawnedMob::CWpsAlgoCondition_SpawnedMob(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_SPAWNED_MOB, "SCRIPTCONTROL_CONDITION_SPAWNED_MOB")
{
}


CWpsAlgoCondition_SpawnedMob::~CWpsAlgoCondition_SpawnedMob()
{
}


bool CWpsAlgoCondition_SpawnedMob::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_SpawnedMob* pCond = dynamic_cast<CWPSNodeCondition_SpawnedMob*>(pControlScriptNode);
	if (pCond)
	{
		m_mobSpawnGroupId = pCond->m_mobSpawnGroupId;
		m_byMobCount = pCond->m_byMobCount;

		return true;
	}

	return false;
}

int CWpsAlgoCondition_SpawnedMob::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	BYTE spawnCount = (BYTE) GetOwner()->GetSpawnGroupCount(m_mobSpawnGroupId);

	if (spawnCount >= m_byMobCount)
		m_status = COMPLETED;

	return m_status;

}
