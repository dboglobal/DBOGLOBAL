#include "stdafx.h"
#include "BotAiCondition_AttackTarget.h"


CBotAiCondition_AttackTarget::CBotAiCondition_AttackTarget(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_ATTACK_TARGET, "BOTCONTROL_CONDITION_ATTACK_TARGET")
{
	m_dwTime = 0;
	m_bPartyTargetChecked = false;
}

CBotAiCondition_AttackTarget::~CBotAiCondition_AttackTarget()
{
}

void CBotAiCondition_AttackTarget::OnContinue()
{
	m_bPartyTargetChecked = false;
}

int CBotAiCondition_AttackTarget::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);
	if (m_dwTime >= 500)
	{
		m_dwTime = 0;
		if (CheckAndAttackTarget())
			return CHANGED;
	}

	return m_status;
}


bool CBotAiCondition_AttackTarget::CheckAndAttackTarget()
{
	HOBJECT hTarget = GetBot()->ConsiderAttackTarget(0);
	if (hTarget != INVALID_HOBJECT)
	{
		GetBot()->GetBotController()->AddControlState_Fight(hTarget);
		return true;
	}

	if (!m_bPartyTargetChecked)
	{
		if (GetBot()->GetPartyID() != INVALID_PARTYID)
		{
			CNpcParty* pParty = GetBot()->GetNpcParty();
			if (pParty)
			{
				HOBJECT hNewTarget = pParty->OnShareTargetList(GetBot());
				if (hNewTarget != INVALID_HOBJECT)
				{
					GetBot()->GetBotController()->AddControlState_Fight(hNewTarget);
					return true;
				}
			}
		}
		m_bPartyTargetChecked = true;
	}

	return false;
}