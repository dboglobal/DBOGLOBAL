#include "stdafx.h"
#include "BotAiAction_SkillUse.h"
#include "SPSNodeAction_SkillUse.h"
#include "BotAiState_Fight.h"
#include "ObjectManager.h"
#include "SkillCondition.h"
#include "BotAiAction_Chase.h"



CBotAiAction_SkillUse::CBotAiAction_SkillUse(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_SKILL_USE, "BOTCONTROL_ACTION_SKILL_USE")
{
	m_bySkillIndex = INVALID_BYTE;
}

CBotAiAction_SkillUse::CBotAiAction_SkillUse(CNpc* pBot, BYTE bySkillIndex)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_SKILL_USE, "BOTCONTROL_ACTION_SKILL_USE")
{
	m_bySkillIndex = bySkillIndex;
}

CBotAiAction_SkillUse::~CBotAiAction_SkillUse()
{
}


bool CBotAiAction_SkillUse::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_SkillUse* pAction = dynamic_cast<CSPSNodeAction_SkillUse*>(pControlScriptNode);
	if (pAction)
	{
		m_bySkillIndex = pAction->m_bySkillIndex;

		if (GetBot()->GetSkillManager()->IsSkillUseLock())
			return false;
		
		if (GetBot()->GetSkillManager()->GetNumberOfSkill() == 0)
			return false;

		if (!GetBot()->GetSkillManager()->FindSkillCondition(m_bySkillIndex))
			return false;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_SkillUse", pControlScriptNode);
	return false;
}

void CBotAiAction_SkillUse::CopyTo(CControlState* pTo)
{
	CBotAiAction_SkillUse* pSkillUse = check_cast<CBotAiAction_SkillUse*, CControlState*>(pTo);
	if (pSkillUse)
	{
		pSkillUse->m_bySkillIndex = m_bySkillIndex;
	}
	else
		ERR_LOG(LOG_BOTAI, "fail : NULL == pTo");
}

void CBotAiAction_SkillUse::OnEnter()
{
}

void CBotAiAction_SkillUse::OnExit()
{
	CBotAiState_Fight* pFight = GetBot()->GetBotController()->GetFightState();
	if (pFight) //check if is initialized.. OnExit could be called when destroying BOT.
		pFight->SetCompulsionTarget(INVALID_HOBJECT);
}


int CBotAiAction_SkillUse::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CSkillManagerBot* pSkillManager = (CSkillManagerBot*)GetBot()->GetSkillManager();
	if (!pSkillManager)
	{
		m_status = COMPLETED;
		return m_status;
	}

	CSkillCondition* pSkillCond = pSkillManager->FindSkillCondition(m_bySkillIndex);
	if (!pSkillCond)
	{
		m_status = COMPLETED;
		return m_status;
	}

	if (UpdateSubControlQueue(dwTickDiff, fMultiple) != COMPLETED)
		return m_status;
	
	if (pSkillManager->IsSkillUseLock())
	{
		m_status = COMPLETED;
		return m_status;
	}

	CCharacter* pTarget = g_pObjectManager->GetChar(GetBot()->GetTargetHandle());
	if (pTarget == NULL)
	{
		m_status = COMPLETED;
		return m_status;
	}

	bool bChase = false;
	std::list<CNtlVector> rlistCollisionPos;

	if (pSkillCond->GetSkill()->GetOriginalTableData()->byApply_Target != DBO_SKILL_APPLY_TARGET_SELF) //check if we dont use skill on ourself
	{
		if (GetBot()->ConsiderRange(pSkillCond->GetSkill()->GetOriginalTableData()->fUse_Range_Max, 30.0f / 100.0f) == false)
		{
			if (GetBot()->IsReachable(pTarget, rlistCollisionPos) == false)
			{
				if (rlistCollisionPos.size() > 0)
				{
					CNtlVector rLoc(rlistCollisionPos.back());
					if (GetBot()->IsInRange(rLoc, 1.0f))
						bChase = false;
				}
				else bChase = false;
			}
			else bChase = true;
		}
	}

	if (bChase == false)
	{
		HOBJECT hTarget = INVALID_HOBJECT;
		sSKILL_TARGET_LIST targetList;

		pSkillCond->GetTarget(hTarget, targetList);

		if (hTarget == INVALID_HOBJECT)
		{
			m_status = COMPLETED;
			return m_status;
		}
		else
		{
			GetBot()->GetTargetListManager()->SetAggroLastUpdateTime();

			pSkillManager->SetCurSkillTblidx(pSkillCond->GetSkillTblidx());
			pSkillManager->SetCurSkillConditionIdx(m_bySkillIndex);
			pSkillManager->SetSkillUse_Lock();

			WORD wTemp;
			CNtlVector vFinalSubjectLoc;
			pSkillCond->GetSkill()->UseSkill(INVALID_BYTE, hTarget, vFinalSubjectLoc, GetBot()->GetCurLoc(), targetList.byTargetCount, targetList.ahTarget, wTemp);

			if (wTemp != 500)
			{
				pSkillManager->SetCurSkillTblidx(INVALID_TBLIDX);
				pSkillManager->SetCurSkillConditionIdx(INVALID_BYTE);
				pSkillManager->SetSkillUse_Unlock();
			}

			m_status = COMPLETED;
			return m_status;
		}
	}
	else
	{
		CBotAiAction_Chase* pChase = new CBotAiAction_Chase(GetBot(), CBotAiAction_Chase::ATTACKTYPE_SKILL, pSkillCond->GetSkill()->GetOriginalTableData()->fUse_Range_Max);
		if (!AddSubControlQueue(pChase, true))
		{
			m_status = FAILED;
		}
	}

	return m_status;
}