#include "stdafx.h"
#include "BotStateManager.h"
#include "NtlRandom.h"
#include "Npc.h"


CBotStateManager::CBotStateManager()
{
	Init();
}

CBotStateManager::~CBotStateManager()
{
}

int CBotStateManager::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	DWORD dwTickDiffa = dwTickDiff;

	if (m_pParentRef->GetObjType() == OBJTYPE_NPC || m_pParentRef->GetObjType() == OBJTYPE_MOB)
	{
		if (/*CNpcServerConfig::m_bDelayUpdateBotState == 1 &&*/ !((CNpc*)m_pParentRef)->HasNearbyPlayer(false))
		{
			m_dwSumDelayTick += dwTickDiffa;

			if (m_dwSumDelayTick < 1000 /*CNpcServerConfig::m_dwDelayUpdateBotStateTime*/)
				return 0;

			dwTickDiffa = m_dwSumDelayTick;
			m_dwSumDelayTick = 0;
		}
	}

	return CStateManager::OnUpdate(dwTickDiffa, fMultiple);
}


void CBotStateManager::Init()
{
	//if (CNpcServerConfig::m_bDelayUpdateBotState == 1)
	//	v4->m_dwSumDelayTick = RandomRange(0, 1000);
	//else
		m_dwSumDelayTick = (DWORD)RandomRange(0,1000);
}