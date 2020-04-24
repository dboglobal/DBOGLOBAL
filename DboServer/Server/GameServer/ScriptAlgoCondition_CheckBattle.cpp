#include "stdafx.h"
#include "ScriptAlgoCondition_CheckBattle.h"
#include "WPSNodeCondition_CheckBattle.h"
#include "ObjectManager.h"
#include "Monster.h"


CWpsAlgoCondition_CheckBattle::CWpsAlgoCondition_CheckBattle(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_CHECK_BATTLE, "SCRIPTCONTROL_CONDITION_CHECK_BATTLE")
{
}


CWpsAlgoCondition_CheckBattle::~CWpsAlgoCondition_CheckBattle()
{
}


bool CWpsAlgoCondition_CheckBattle::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_CheckBattle* pCond = dynamic_cast<CWPSNodeCondition_CheckBattle*>(pControlScriptNode);
	if (pCond)
	{
		m_eObjType = pCond->m_eObjType;
		m_index = pCond->m_index;
		m_group = pCond->m_group;
		m_bIsBattle = pCond->m_bIsBattle;

		return true;
	}

	return false;
}

int CWpsAlgoCondition_CheckBattle::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	DWORD dwCount = GetOwner()->GetSpawnGroupCount(m_group);
	HOBJECT hMonster = GetOwner()->GetSpawnGroupFirst(m_group);

	if (dwCount == 0 || hMonster == INVALID_HOBJECT) //if no mob exist then complete task to avoid freeze in script.
	{
		m_status = COMPLETED;
		return m_status;
	}

	for (DWORD i = 0; i < dwCount; i++)
	{
		CMonster* pMob = g_pObjectManager->GetMob(hMonster);
		if (pMob)
		{
			if (pMob->IsFainting() || !pMob->IsInitialized()) //if we kill the monster with one hit
			{
				m_status = COMPLETED;
				break;
			}

			if (m_bIsBattle) //check if battle began
			{
				if (pMob->IsActionStatusFlag(BOT_ACTIONSTATUS_DOING_BATTLE))
				{
					m_status = COMPLETED;
					break;
				}
			}
		}

		hMonster = GetOwner()->GetSpawnGroupNext(m_group);
	}

	return m_status;
}
