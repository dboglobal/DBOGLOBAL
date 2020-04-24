#include "stdafx.h"
#include "ScriptAlgoAction_ChangeCharAttribute.h"
#include "WPSNodeAction_ChangeCharAttribute.h"
#include "Monster.h"


CWpsAlgoAction_ChangeCharAttribute::CWpsAlgoAction_ChangeCharAttribute(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CHANGE_CHAR_ATTRIBUTE, "SCRIPTCONTROL_ACTION_CHANGE_CHAR_ATTRIBUTE")
{
}


CWpsAlgoAction_ChangeCharAttribute::~CWpsAlgoAction_ChangeCharAttribute()
{
}


bool CWpsAlgoAction_ChangeCharAttribute::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_ChangeCharAttribute* pAction = dynamic_cast<CWPSNodeAction_ChangeCharAttribute*>(pControlScriptNode);
	if (pAction)
	{
		m_eTargetType = pAction->m_eTargetType;
		m_dwTargetIndex = pAction->m_dwTargetIndex;
		m_bUseNickname = pAction->m_bUseNickname;
		m_nicknameTblidx = pAction->m_nicknameTblidx;
		m_bySizeRate = pAction->m_bySizeRate;
		m_byImmortalMode = pAction->m_byImmortalMode;
		m_byLpBlockRate = pAction->m_byLpBlockRate;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_Case", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_ChangeCharAttribute::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	switch (m_eTargetType)
	{
		case OBJTYPE_MOB:
		{
			DWORD dwCount = GetOwner()->GetSpawnGroupCount(m_dwTargetIndex);

			HOBJECT hMob = GetOwner()->GetSpawnGroupFirst(m_dwTargetIndex);

			for (DWORD i = 0; i < dwCount; i++)
			{
				CMonster* pMob = g_pObjectManager->GetMob(hMob);

				if (pMob && pMob->IsInitialized() && !pMob->IsFainting() && pMob->GetCurWorld())
				{
					if (m_bySizeRate != 10)
						pMob->UpdateSizeRate(m_bySizeRate);

					if (m_nicknameTblidx != INVALID_TBLIDX)
						pMob->UpdateNickName(m_nicknameTblidx);
				}

				hMob = GetOwner()->GetSpawnGroupNext(m_dwTargetIndex);
			}
		}
		break;

		default: ERR_LOG(LOG_SCRIPT, "targettype %u not set", m_eTargetType); break;
	}


	m_status = COMPLETED;
	return m_status;
}
