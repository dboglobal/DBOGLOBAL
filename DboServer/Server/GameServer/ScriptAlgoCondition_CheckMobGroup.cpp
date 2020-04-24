#include "stdafx.h"
#include "ScriptAlgoCondition_CheckMobGroup.h"
#include "WPSNodeCondition_CheckMobGroup.h"
#include "Monster.h"


CWpsAlgoCondition_CheckMobGroup::CWpsAlgoCondition_CheckMobGroup(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_CHECK_MOB_GROUP, "SCRIPTCONTROL_CONDITION_CHECK_MOB_GROUP")
{
}


CWpsAlgoCondition_CheckMobGroup::~CWpsAlgoCondition_CheckMobGroup()
{
}


bool CWpsAlgoCondition_CheckMobGroup::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_CheckMobGroup* pCond = dynamic_cast<CWPSNodeCondition_CheckMobGroup*>(pControlScriptNode);
	if (pCond)
	{
		m_spawnGroupId = pCond->m_spawnGroupId;
		m_nCheckCount = pCond->m_nCheckCount;

		return true;
	}

	return false;
}

int CWpsAlgoCondition_CheckMobGroup::OnUpdate(DWORD dwTickDiff, float fMultiple)
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
