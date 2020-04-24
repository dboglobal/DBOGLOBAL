#include "stdafx.h"
#include "BotAiCondition_OutOfNest.h"


CBotAiCondition_OutOfNest::CBotAiCondition_OutOfNest(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_OUT_OF_NEST, "BOTCONTROL_CONDITION_OUT_OF_NEST")
{
	m_byMaximumRangeOfAwayFromNest = 80;
}

CBotAiCondition_OutOfNest::~CBotAiCondition_OutOfNest()
{
}

void CBotAiCondition_OutOfNest::OnEnter()
{
	m_byMaximumRangeOfAwayFromNest = GetBot()->GetNestRange();

	if (GetBot()->GetNestType() == NPC_NEST_TYPE_DEFAULT)
	{
		if (GetBot()->GetSpawnRange() + GetBot()->GetTbldat()->wSight_Range > m_byMaximumRangeOfAwayFromNest)
		{
			m_byMaximumRangeOfAwayFromNest = GetBot()->GetSpawnRange() + GetBot()->GetTbldat()->wSight_Range;
		}
	}

	GetBot()->GetTargetListManager()->SetAggroLastUpdateTime();

	if (GetBot()->IsActionStatusFlag(BOT_ACTIONSTATUS_OUT_OF_NEST))
		GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_OUT_OF_NEST, false);
}

void CBotAiCondition_OutOfNest::OnContinue()
{
	if (GetBot()->IsActionStatusFlag(BOT_ACTIONSTATUS_OUT_OF_NEST))
		GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_OUT_OF_NEST, false);
}

void CBotAiCondition_OutOfNest::OnExit()
{
	if (GetBot()->IsActionStatusFlag(BOT_ACTIONSTATUS_OUT_OF_NEST))
		GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_OUT_OF_NEST, false);
}

int CBotAiCondition_OutOfNest::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CNtlVector vNestLoc(GetBot()->GetBattleNestLoc());

	CNpcParty* pParty = GetBot()->GetNpcParty();
	if (pParty)
	{
		if (GetBot()->IsInRange(vNestLoc, (float)m_byMaximumRangeOfAwayFromNest))
			GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_OUT_OF_NEST, false);
		else
			GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_OUT_OF_NEST, true);

		if (pParty->IsOutOfNest())
		{
			m_status = CLEARED;
		}
	}
	else
	{
		if (!GetBot()->IsInRange(vNestLoc, (float)m_byMaximumRangeOfAwayFromNest))
			m_status = CLEARED;
	}

	return m_status;
}

