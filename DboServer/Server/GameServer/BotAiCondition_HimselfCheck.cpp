#include "stdafx.h"
#include "BotAiCondition_HimselfCheck.h"
#include "AISNodeCondition_HimselfCheck.h"
#include "AIScriptControlFactoy.h"
#include "EndCondition_HimSelf_LPCheck.h"


CBotAiCondition_HimselfCheck::CBotAiCondition_HimselfCheck(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_HIMSELFCHECK, "BOTCONTROL_CONDITION_HIMSELFCHECK")
{
	m_eCheckType = CAISNodeCondition_HimselfCheck::INVALID_AIS_HIMSELF_CHECKTYPE;
	m_eValueType = CAISNodeCondition_HimselfCheck::INVALID_AIS_HIMSELF_VALUETYPE;
	m_fVal = 0.0f;
	m_dwTime = 0;
	m_scriptnodeID = INVALID_DWORD;
	memset(m_tblidxSkill, INVALID_TBLIDX, sizeof(m_tblidxSkill));
	m_byTblidxSkillCount = 0;
}

CBotAiCondition_HimselfCheck::~CBotAiCondition_HimselfCheck()
{
}

bool CBotAiCondition_HimselfCheck::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CAISNodeCondition_HimselfCheck* pCond = dynamic_cast<CAISNodeCondition_HimselfCheck*>(pControlScriptNode);
	if (pCond)
	{
		m_scriptnodeID = pControlScriptNode->GetNodeIndex();
		m_eCheckType = pCond->m_eCheckType;
		m_eValueType = pCond->m_eValueType;
		memcpy(m_tblidxSkill, pCond->m_tblidxSkill, sizeof(m_tblidxSkill));
		m_eValueType = pCond->m_eValueType;
		m_byTblidxSkillCount = pCond->m_byTblidxSkillCount;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CAISNodeCondition_HimselfCheck", pControlScriptNode);
	return false;
}

void CBotAiCondition_HimselfCheck::CopyTo(CControlState* pTo)
{
	CBotAiCondition_HimselfCheck* pCond = dynamic_cast<CBotAiCondition_HimselfCheck*>(pTo);
	if (pCond)
	{
		pCond->m_eCheckType = m_eCheckType;
		pCond->m_eValueType = m_eValueType;
		pCond->m_fVal = m_fVal;
		pCond->m_dwTime = m_dwTime;
		pCond->m_scriptnodeID = m_scriptnodeID;
		memcpy(pCond->m_tblidxSkill, m_tblidxSkill, sizeof(m_tblidxSkill));
		pCond->m_byTblidxSkillCount = m_byTblidxSkillCount;
	}
}

void CBotAiCondition_HimselfCheck::OnEnter()
{
}

void CBotAiCondition_HimselfCheck::OnHalt()
{
}

void CBotAiCondition_HimselfCheck::OnExit()
{
}

int CBotAiCondition_HimselfCheck::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);
	if (m_dwTime < 1000)
		return m_status;

	m_dwTime = 0;

	switch (m_eCheckType)
	{
		case CAISNodeCondition_HimselfCheck::AIS_EVENT_VALUETYPE_PERCENT:
		{
			return Handle_LP();
		}
		case CAISNodeCondition_HimselfCheck::AIS_EVENT_VALUETYPE_SKILLTBLIDX:
		{
			return Handle_SkillHit();
		}

		default:
		{
			m_status = FAILED;
		}
	}

	return m_status;
}


int CBotAiCondition_HimselfCheck::Handle_LP()
{
	if (GetBot()->GetBotController()->GetCheckupEndCondition()->GetEndCondition(m_scriptnodeID))
		return m_status;

	if (GetBot()->ConsiderLPLow(m_fVal))
	{
		CAIScriptControlFactoy factory(GetBot());

		if (!GetBot()->GetControlStateManager()->CopySubControl(m_scriptnodeID, this, (CComplexState *)GetBot()->GetBotController()->GetCurrentState(), &factory))
		{
			m_status = FAILED;
			return m_status;
		}

		CEndCondition_HimSelf_LPCheck* pEndCondition = new CEndCondition_HimSelf_LPCheck(GetBot(), m_eValueType, m_fVal);
		if (pEndCondition)
		{
			if (!GetBot()->GetBotController()->GetCheckupEndCondition()->AddEndCondition(m_scriptnodeID, pEndCondition))
			{
				ERR_LOG(LOG_BOTAI, "fail : false == AddEndCondition()");
				m_status = FAILED;
			}
		}
		else
		{
			ERR_LOG(LOG_BOTAI, "fail : NULL == pEndCondition");
			m_status = FAILED;
		}
	}

	return m_status;
}

int CBotAiCondition_HimselfCheck::Handle_SkillHit()
{
	if (GetBot()->GetHitBySkillContainer()->Empty())
		return m_status;

	if (m_byTblidxSkillCount == INVALID_BYTE)
	{
		ERR_LOG(LOG_BOTAI, "fail : INVALID_BYTE == m_byTblidxSkillCount");
		m_status = FAILED;
		return m_status;
	}

	TBLIDX skillTblidx = INVALID_TBLIDX;

	int nLoopCount = 0;

	while (true)
	{
		++nLoopCount;
		if (nLoopCount > 1000)
		{
			ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
		}

		skillTblidx = GetBot()->GetHitBySkillContainer()->PopHitBySkill();

		if (skillTblidx == INVALID_TBLIDX)
			break;

		for (int i = 0; i < m_byTblidxSkillCount; i++)
		{
			if (m_tblidxSkill[i] == skillTblidx)
			{
				CAIScriptControlFactoy factory(GetBot());

				if (!GetBot()->GetControlStateManager()->CopySubControl(m_scriptnodeID, this, (CComplexState *)GetBot()->GetBotController()->GetCurrentState(), &factory))
				{
					m_status = FAILED;
				}

				return m_status;
			}
		}
	}

	return m_status;
}
