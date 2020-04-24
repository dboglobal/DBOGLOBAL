#include "stdafx.h"
#include "BotAiAction_Flee.h"
#include "BotAiAction_NavMove.h"


CBotAiAction_Flee::CBotAiAction_Flee(CNpc* pBot, CNtlVector& vFleeLoc)
	:CBotAiAction(pBot, BOTCONTROL_ACTION_FLEE, "BOTCONTROL_ACTION_FLEE")
{
	m_vFleeLoc.operator=(vFleeLoc);
}

CBotAiAction_Flee::~CBotAiAction_Flee()
{
}


void CBotAiAction_Flee::OnEnter()
{
	//CPacketSender::Send_SetCharSpeedUp(v7, pBot, 80.0, 80.0);
	if (GetBot()->GetCurLoc().operator!=(m_vFleeLoc))
	{
		CBotAiAction_NavMove* pNavMove = new CBotAiAction_NavMove(GetBot(), m_vFleeLoc, true, false, CNtlVector::INVALID_XZ, CNtlVector::ZERO, INVALID_FLOAT);
		if (!AddSubControlQueue(pNavMove, true))
			m_status = FAILED;
	}
}

void CBotAiAction_Flee::OnExit()
{
	//CPacketSender::Send_SetCharSpeedUp(v7, pBot, 100.0, 100.0);
}

int CBotAiAction_Flee::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return CComplexState::UpdateSubControlQueue(dwTickDiff, fMultiple);
}
