#include "stdafx.h"
#include "BotAiAction_RangedAttack.h"
#include "BotAiAction_Look.h"
#include "BotAiAction_NavMove.h"


CBotAiAction_RangedAttack::CBotAiAction_RangedAttack(CNpc* pBot, CNtlVector& vDestLoc)
	: CBotAiAction(pBot, BOTCONTROL_RANGED_ATTACK_MOVE, "BOTCONTROL_RANGED_ATTACK_MOVE")
{
	m_dwTime = 0;
	m_vDestLoc.operator=(vDestLoc);
}


CBotAiAction_RangedAttack::~CBotAiAction_RangedAttack()
{
}



void CBotAiAction_RangedAttack::OnEnter()
{
	RemoveAllSubControl();

	CBotAiAction_Look* pLook = new CBotAiAction_Look(GetBot(), GetBot()->GetTargetHandle(), 0);
	if (AddSubControlQueue(pLook, true))
	{
		CBotAiAction_NavMove* pNavMove = new CBotAiAction_NavMove(GetBot(), m_vDestLoc, true, false, CNtlVector::INVALID_XZ, CNtlVector::ZERO, INVALID_FLOAT);
		if (!AddSubControlQueue(pNavMove, true))
			m_status = FAILED;
	}
	else
	{
		m_status = FAILED;
	}
}

void CBotAiAction_RangedAttack::OnExit()
{
}


int CBotAiAction_RangedAttack::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (UpdateSubControlQueue(dwTickDiff, fMultiple) == COMPLETED)
		m_status = COMPLETED;

	return m_status;
}