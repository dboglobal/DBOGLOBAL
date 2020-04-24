#include "stdafx.h"
#include "BotAiAction_Retreat.h"
#include "BotAiAction_NavMove.h"
#include "NtlRandom.h"


CBotAiAction_Retreat::CBotAiAction_Retreat(CNpc* pBot, CNtlVector& vReturnLoc)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_RETREAT, "BOTCONTROL_ACTION_RETREAT")
{
	m_vReturnLoc.operator=(vReturnLoc);
	m_fRunSpeed = 80.0f; //this will change run speed down to 80% I think
	m_fWalkSpeed = 80.0f; //this will change walk speed down to 80% I think
}


CBotAiAction_Retreat::~CBotAiAction_Retreat()
{
}



void CBotAiAction_Retreat::OnEnter()
{
	//CPacketSender::Send_SetCharSpeedUp(v7, pBot, v20->m_fRunSpeed, v20->m_fWalkSpeed);

	CBotAiAction_NavMove* pNavMove = new CBotAiAction_NavMove(GetBot(), m_vReturnLoc, true, false, CNtlVector::INVALID_XZ, CNtlVector::ZERO, INVALID_FLOAT);
	if (!AddSubControlQueue(pNavMove, true))
		m_status = FAILED;
}

void CBotAiAction_Retreat::OnExit()
{
	//CPacketSender::Send_SetCharSpeedUp(v7, pBot, 100.0, 100.0); //change move speed back to 100% !?
}


int CBotAiAction_Retreat::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	int nStatus = UpdateSubControlQueue(dwTickDiff, fMultiple);

	if(nStatus == COMPLETED)
	{
		if (Dbo_CheckProbability(30))
			GetBot()->HelpAlliance();

		GetBot()->SetRetreat(true);
		m_status = COMPLETED;
	}

	return m_status;
}
