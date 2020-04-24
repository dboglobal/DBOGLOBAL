#include "stdafx.h"
#include "TqsAlgoCondition_CheckMobGroup.h"
#include "TQSNodeCondition_CheckMobGroup.h"
#include "Monster.h"


CTqsAlgoCondition_CheckMobGroup::CTqsAlgoCondition_CheckMobGroup(CTqsAlgoObject* pObject) :
	CTqsAlgoCondition_Base(pObject, TQS_ALGOCONTROLID_CONDITION_CHECK_MOB_GROUP, "TQS_ALGOCONTROLID_CONDITION_CHECK_MOB_GROUP")
{
}


CTqsAlgoCondition_CheckMobGroup::~CTqsAlgoCondition_CheckMobGroup()
{
}


bool CTqsAlgoCondition_CheckMobGroup::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeCondition_CheckMobGroup* pCond = dynamic_cast<CTQSNodeCondition_CheckMobGroup*>(pControlScriptNode);
	if (pCond)
	{
		m_spawnGroupId = pCond->m_spawnGroupId;
		m_nCheckCount = pCond->m_nCheckCount;

		return true;
	}

	return false;
}

int CTqsAlgoCondition_CheckMobGroup::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_nCheckCount == 0) //if 0 then check if whole spawned mob group has been killed
	{
		if (GetOwner()->GetSpawnGroupCount(m_spawnGroupId) == 0)
		{
		//	NTL_PRINT(PRINT_APP, "m_spawnGroupId %u dwCount %u m_nCheckCount %u nCount %u \n", m_spawnGroupId, dwCount, m_nCheckCount, nCount);
			m_status = COMPLETED;
		}
	}
	else
	{
		if (GetOwner()->GetRemovedSpawnGroupCount(m_spawnGroupId) >= (DWORD)m_nCheckCount)
		{
		//	NTL_PRINT(PRINT_APP, "m_spawnGroupId %u dwCount %u m_nCheckCount %u nCount %u \n", m_spawnGroupId, dwCount, m_nCheckCount, nCount);
			m_status = COMPLETED;
		}
	}

	return m_status;
}
