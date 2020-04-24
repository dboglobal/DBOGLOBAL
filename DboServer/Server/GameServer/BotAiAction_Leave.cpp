#include "stdafx.h"
#include "BotAiAction_Leave.h"


CBotAiAction_Leave::CBotAiAction_Leave(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_LEAVE, "BOTCONTROL_ACTION_LEAVE")
{
}


CBotAiAction_Leave::~CBotAiAction_Leave()
{
}


void CBotAiAction_Leave::OnEnter()
{
}

void CBotAiAction_Leave::OnExit()
{
}


int CBotAiAction_Leave::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	GetBot()->GetBotController()->ChangeControlState_Leave();

	m_status = COMPLETED;
	return m_status;
}
