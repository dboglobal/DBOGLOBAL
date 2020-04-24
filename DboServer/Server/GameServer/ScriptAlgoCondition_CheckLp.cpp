#include "stdafx.h"
#include "ScriptAlgoCondition_CheckLp.h"
#include "WPSNodeCondition_CheckLP.h"
#include "Monster.h"


CWpsAlgoCondition_CheckLp::CWpsAlgoCondition_CheckLp(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_CHECK_LP, "SCRIPTCONTROL_CONDITION_CHECK_LP")
{
}


CWpsAlgoCondition_CheckLp::~CWpsAlgoCondition_CheckLp()
{
}


bool CWpsAlgoCondition_CheckLp::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_CheckLP* pCond = dynamic_cast<CWPSNodeCondition_CheckLP*>(pControlScriptNode);
	if (pCond)
	{
		m_eObjType = pCond->m_eObjType;
		m_index = pCond->m_index;
		m_group = pCond->m_group;
		m_byPercent = pCond->m_byPercent;

		return true;
	}

	return false;
}

int CWpsAlgoCondition_CheckLp::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	switch (m_eObjType)
	{
		case OBJTYPE_NPC:
		{
			CNpc* pNpc = GetOwner()->FindNpc(m_index);
			if (pNpc)
			{
				if (!pNpc->IsInitialized() || pNpc->GetCurLpInPercent() <= (float)m_byPercent)
					m_status = COMPLETED;
			}
			else m_status = COMPLETED;
		}
		break;
		case OBJTYPE_MOB:
		{
			DWORD dwCount = GetOwner()->GetSpawnGroupCount(m_group);
			HOBJECT hMonster = GetOwner()->GetSpawnGroupFirst(m_group);

			if(dwCount == 0 || hMonster == INVALID_HOBJECT)
				m_status = COMPLETED;
			else
			{
				for (DWORD i = 0; i < dwCount; i++)
				{
					CMonster* pMob = g_pObjectManager->GetMob(hMonster);
					if (pMob && pMob->IsInitialized())
					{
						if (m_index == INVALID_TBLIDX || pMob->GetTblidx() == m_index)
						{
							if (pMob->GetCurLpInPercent() <= (float)m_byPercent || pMob->IsFainting())
							{
								m_status = COMPLETED;
								break;
							}
						}
					}
					else
					{
						m_status = COMPLETED;
						break;
					}

					hMonster = GetOwner()->GetSpawnGroupNext(m_group);
				}
			}
		}
		break;

		default: ERR_LOG(LOG_SCRIPT, "ObjType '%u' not set ", m_eObjType); break;
	}

	return m_status;
}
