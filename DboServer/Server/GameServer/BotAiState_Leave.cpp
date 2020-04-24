#include "stdafx.h"
#include "BotAiState_Leave.h"


CBotAiState_Leave::CBotAiState_Leave(CNpc* pBot)
	:CBotAiState(pBot, BOTCONTROL_LEAVE, "BOTCONTROL_LEAVE")
{
}

CBotAiState_Leave::~CBotAiState_Leave()
{
}


void CBotAiState_Leave::OnEnter()
{
	GetBot()->LeaveGame();
}

void CBotAiState_Leave::OnExit()
{
}

int CBotAiState_Leave::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
