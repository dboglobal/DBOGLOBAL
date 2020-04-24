#include "stdafx.h"
#include "BotAiState_Despawn.h"


CBotAiState_Despawn::CBotAiState_Despawn(CNpc* pBot)
	:CBotAiState(pBot, BOTCONTROL_DESPAWN, "BOTCONTROL_DESPAWN")
{
	m_dwTime = 0;
}

CBotAiState_Despawn::~CBotAiState_Despawn()
{
}


void CBotAiState_Despawn::OnEnter()
{
}

void CBotAiState_Despawn::OnExit()
{
}

int CBotAiState_Despawn::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);
	if (m_dwTime >= 5000)
	{
		GetBot()->GetBotController()->ChangeControlState_Leave();
		m_status = COMPLETED;
	}

	return m_status;
}
