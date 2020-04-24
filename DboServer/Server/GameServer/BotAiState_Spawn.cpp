#include "stdafx.h"
#include "BotAiState_Spawn.h"


CBotAiState_Spawn::CBotAiState_Spawn(CNpc* pBot)
:CBotAiState(pBot, BOTCONTROL_SPAWN, "BOTCONTROL_SPAWN")
{
	m_dwTime = 0;
	m_dwAniTime = 0;
}

CBotAiState_Spawn::~CBotAiState_Spawn()
{
}


void CBotAiState_Spawn::OnEnter()
{
	if (BIT_FLAG_TEST(GetBot()->GetSpawnFuncFlag(), SPAWN_FUNC_FLAG_NO_SPAWN_WAIT))
	{
		m_dwAniTime = 0;
	}
	else
	{
		m_dwAniTime = (DWORD)GetBot()->GetAniTbldat()->GetAnimationLength(NML_SPAWN);
		if (m_dwAniTime < 3000)
			m_dwAniTime = 3000;
	}
}

void CBotAiState_Spawn::OnExit()
{
}

int CBotAiState_Spawn::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_dwAniTime == 0)
	{
		GetBot()->GetBotController()->ChangeControlState_Idle();
		m_status = COMPLETED;
	}
	else
	{
		m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);
		if (m_dwAniTime <= m_dwTime)
		{
			m_dwTime = 0;
			GetBot()->GetBotController()->ChangeControlState_Idle();
			m_status = COMPLETED;
		}
	}

	if (m_status == COMPLETED)
		GetBot()->GetStateManager()->ChangeCharState(CHARSTATE_STANDING, NULL, false);

	return m_status;
}
