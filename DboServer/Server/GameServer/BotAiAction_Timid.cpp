#include "stdafx.h"
#include "BotAiAction_Timid.h"
#include "BotAiAction_NavMove.h"


CBotAiAction_Timid::CBotAiAction_Timid(CNpc* pBot, CNtlVector& vDestLoc)
	:CBotAiAction(pBot, BOTCONTROL_ACTION_TIMID, "BOTCONTROL_ACTION_TIMID")
{
	m_vDestLoc.operator=(vDestLoc);
}

CBotAiAction_Timid::~CBotAiAction_Timid()
{
}



void CBotAiAction_Timid::OnEnter()
{
	CBotAiAction_NavMove* pNavMove = new CBotAiAction_NavMove(GetBot(), m_vDestLoc, true, false, CNtlVector::INVALID_XZ, CNtlVector::ZERO, INVALID_FLOAT);
	if (!AddSubControlQueue(pNavMove, true))
		m_status = FAILED;
}

void CBotAiAction_Timid::OnExit()
{
	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_TIMID_MOVE, false);
}


int CBotAiAction_Timid::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (UpdateSubControlList(dwTickDiff, fMultiple) == CHANGED)
	{
		m_status = INACTIVATED;
	}
	else if (UpdateSubControlQueue(dwTickDiff, fMultiple) == COMPLETED)
	{
		m_status = COMPLETED;
	}

	return m_status;
}
