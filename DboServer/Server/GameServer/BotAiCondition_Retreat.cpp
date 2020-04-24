#include "stdafx.h"
#include "BotAiCondition_Retreat.h"
#include "NtlRandom.h"
#include "BotAiState.h"
#include "BotAiAction_Retreat.h"


CBotAiCondition_Retreat::CBotAiCondition_Retreat(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_RETREAT, "BOTCONTROL_CONDITION_RETREAT")
{
	m_dwTime = 0;
}

CBotAiCondition_Retreat::~CBotAiCondition_Retreat()
{
}


int CBotAiCondition_Retreat::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);

	if (m_dwTime < 1000)
		return m_status;

	m_dwTime = 0;

	if (!GetBot()->IsMoveAble()) //not sure vfptr[7].GetName
		return m_status;

	if (!GetBot()->ConsiderLPLow(30.0f))
		return m_status;

	if (!Dbo_CheckProbability(60))
		return m_status;

	CNtlVector vRetreatLoc;
	bool bResultCode = false;

	if (GetBot()->IsInRange(GetBot()->GetEnterLoc(), 4.0f))
	{
		bResultCode = GetBot()->ConsiderRetreatFleeLoc(vRetreatLoc, 20.0f, 40.0f);
	}
	else
	{
		bResultCode = GetBot()->ConsiderRetreatLoc(vRetreatLoc, 20.0f, 40.0f);
	}

	if (bResultCode)
	{
		CBotAiState* pCurState = GetBot()->GetBotController()->GetCurrentState();
		if (pCurState)
		{
			CBotAiAction_Retreat* pRetreat = new CBotAiAction_Retreat(GetBot(), vRetreatLoc);
			if (!pCurState->AddSubControlQueue(pRetreat, true))
			{
				m_status = FAILED;
			}
		}

		m_status = COMPLETED;
	}

	return m_status;
}

