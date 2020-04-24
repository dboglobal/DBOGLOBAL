#include "stdafx.h"
#include "ScriptAlgoAction_UnregisterBuff.h"
#include "WPSNodeAction_UnregisterBuff.h"
#include "Monster.h"


CWpsAlgoAction_UnregisterBuff::CWpsAlgoAction_UnregisterBuff(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_UNREGISTER_BUFF, "SCRIPTCONTROL_ACTION_UNREGISTER_BUFF")
{
}


CWpsAlgoAction_UnregisterBuff::~CWpsAlgoAction_UnregisterBuff()
{
}


bool CWpsAlgoAction_UnregisterBuff::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_UnregisterBuff* pAction = dynamic_cast<CWPSNodeAction_UnregisterBuff*>(pControlScriptNode);
	if (pAction)
	{
		m_eTargetType = pAction->m_eTargetType;
		m_dwTargetIndex = pAction->m_dwTargetIndex;
		m_SkillId = pAction->m_SkillId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_TimeCountdown", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_UnregisterBuff::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_eTargetType == OBJTYPE_NPC)
	{
		CNpc* pNpc = GetOwner()->FindNpc(m_dwTargetIndex);

		if (pNpc == NULL && GetOwner()->GetWorld())
		{
			pNpc = GetOwner()->GetWorld()->FindNpc(m_dwTargetIndex);
		}

		if (pNpc == NULL || !pNpc->IsInitialized()) //did we already remove the npc? 
		{
			m_status = COMPLETED;
			return m_status;
		}

		if (!pNpc->IsFainting())
			pNpc->GetBuffManager()->RemoveBlessBuff(m_SkillId);
	}
	else
	{
		DWORD dwCount = GetOwner()->GetSpawnGroupCount(m_dwTargetIndex);
		HOBJECT hMonster = GetOwner()->GetSpawnGroupFirst(m_dwTargetIndex);

		for (DWORD i = 0; i < dwCount; i++)
		{
			CMonster* pMob = g_pObjectManager->GetMob(hMonster);
			if (pMob && pMob->IsInitialized() && !pMob->IsFainting())
			{
				pMob->GetBuffManager()->RemoveBlessBuff(m_SkillId);
			}

			hMonster = GetOwner()->GetSpawnGroupNext(m_dwTargetIndex);
		}
	}

	m_status = COMPLETED;
	return m_status;
}
