#include "stdafx.h"
#include "ActionPattern.h"
#include "ContainerActionPatternUnit.h"
#include "char.h"


CActionPattern::CActionPattern()
{
	this->m_pContainerActionPatternUnit = NULL;
	this->m_actionpatternIdx = 0;
	this->m_pOwnerBot = NULL;
	this->m_bIsSendSameTimePacket = false;
}

CActionPattern::~CActionPattern()
{
	Destroy();
}


bool CActionPattern::Create(CCharacter* pBot, CContainerActionPatternUnit* pContainerActionPatternUnit)
{
	if (pBot)
	{
		if (pContainerActionPatternUnit)
		{
			m_pOwnerBot = pBot;
			m_pContainerActionPatternUnit = pContainerActionPatternUnit;
			return true;
		}
		else
		{
			ERR_LOG(LOG_BOTAI, "fail : NULL == pContainerActionPatternUnit");
		}
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "fail : NULL == pBot");
	}

	return false;
}

void CActionPattern::Destroy()
{

}

eACTIONPATTERNID CActionPattern::GetCurActionID()
{
	if (m_pContainerActionPatternUnit)
	{
		CActionPatternUnit* pUnit = m_pContainerActionPatternUnit->GetActionPattern(m_actionpatternIdx);
		if (pUnit)
		{
			return pUnit->GetActionPatternID();
		}
		else
		{
			ERR_LOG(LOG_BOTAI, "fail : NULL == pUnit : Idx(%d)", m_actionpatternIdx);
		}
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "NULL == m_pContainerActionPatternUnit");
	}

	return INVALID_ACTIONPATTERNID;
}

DWORD CActionPattern::GetWaitTime()
{
	if (m_pContainerActionPatternUnit)
	{
		CActionPatternUnit* pUnit = m_pContainerActionPatternUnit->GetActionPattern(m_actionpatternIdx);
		if (pUnit)
		{
			return pUnit->GetAniTime();
		}
		else
		{
			ERR_LOG(LOG_BOTAI, "fail : NULL == pUnit : Idx(%d)", m_actionpatternIdx);
		}
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "NULL == m_pContainerActionPatternUnit");
	}

	return 0;
}

void CActionPattern::Reset()
{
	this->m_actionpatternIdx = 0;
	this->m_bIsSendSameTimePacket = false;
}

void CActionPattern::ForwardActionPatternIndex()
{
	eACTIONPATTERNID patternId = GetCurActionID();

	if (patternId == BOTAP_MOVE)
		m_bIsSendSameTimePacket = false;

	if (m_pContainerActionPatternUnit)
	{
		if (m_pContainerActionPatternUnit->GetCount() - 1 <= m_actionpatternIdx)
			Reset();
		else
			++m_actionpatternIdx;
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "NULL == m_pContainerActionPatternUnit");
		
	}
}

BYTE CActionPattern::GetActionPatternIndex()
{
	return m_actionpatternIdx;
}

void CActionPattern::SendUnitIdx()
{
	eACTIONPATTERNID patternId = GetCurActionID();
	if (patternId >= BOTAP_MIDLE1 && patternId <= BOTAP_MIDLE2 && !m_bIsSendSameTimePacket)
	{
		m_pOwnerBot->Send_BotNonBattleActionPatternSet_UnitIdx(m_actionpatternIdx);

		m_bIsSendSameTimePacket = true;
	}
}

TBLIDX CActionPattern::GetTableIdx()
{
	if (!m_pContainerActionPatternUnit)
		return INVALID_TBLIDX;

	return m_pContainerActionPatternUnit->GetTblIdx();
}

bool CActionPattern::IsActionMove()
{
	return GetCurActionID() == BOTAP_MOVE;
}

