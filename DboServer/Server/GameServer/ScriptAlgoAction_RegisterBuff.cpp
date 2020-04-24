#include "stdafx.h"
#include "ScriptAlgoAction_RegisterBuff.h"
#include "WPSNodeAction_RegisterBuff.h"
#include "Monster.h"
#include "TableContainerManager.h"
#include "SystemEffectTable.h"


CWpsAlgoAction_RegisterBuff::CWpsAlgoAction_RegisterBuff(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_REGISTER_BUFF, "SCRIPTCONTROL_ACTION_REGISTER_BUFF")
{
}


CWpsAlgoAction_RegisterBuff::~CWpsAlgoAction_RegisterBuff()
{
}


bool CWpsAlgoAction_RegisterBuff::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_RegisterBuff* pAction = dynamic_cast<CWPSNodeAction_RegisterBuff*>(pControlScriptNode);
	if (pAction)
	{
		m_eTargetType = pAction->m_eTargetType;
		m_dwTargetIndex = pAction->m_dwTargetIndex;
		m_SkillId = pAction->m_SkillId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_RegisterBuff", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_RegisterBuff::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	sSKILL_TBLDAT* pSkillTbldat = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(m_SkillId);
	if (pSkillTbldat == NULL)
	{
		m_status = COMPLETED;
		return m_status;
	}

	sDBO_BUFF_PARAMETER aBuffParameter[NTL_MAX_EFFECT_IN_SKILL];
	eSYSTEM_EFFECT_CODE aeEffectCode[NTL_MAX_EFFECT_IN_SKILL];

	for (int i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
	{
		aBuffParameter[i].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DEFAULT;
		aBuffParameter[i].buffParameter.fParameter = (float)(pSkillTbldat->aSkill_Effect_Value[i]);
		aBuffParameter[i].buffParameter.dwRemainValue = (DWORD)pSkillTbldat->aSkill_Effect_Value[i];

		aeEffectCode[i] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pSkillTbldat->skill_Effect[i]);
	}
	
	DWORD dwKeepTime = pSkillTbldat->dwKeepTimeInMilliSecs;

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

		if(!pNpc->IsFainting())
			pNpc->GetBuffManager()->RegisterBuff(dwKeepTime, aeEffectCode, aBuffParameter, INVALID_HOBJECT, BUFF_TYPE_BLESS, pSkillTbldat);
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
				pMob->GetBuffManager()->RegisterBuff(dwKeepTime, aeEffectCode, aBuffParameter, INVALID_HOBJECT, BUFF_TYPE_BLESS, pSkillTbldat);
			}

			hMonster = GetOwner()->GetSpawnGroupNext(m_dwTargetIndex);
		}
	}

	m_status = COMPLETED;
	return m_status;
}
