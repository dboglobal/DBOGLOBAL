#include "stdafx.h"
#include "BotAiCondition_Flee.h"
#include "NtlRandom.h"
#include "BotAiAction_Look.h"
#include "BotAiState.h"
#include "BotAiAction_Flee.h"


CBotAiCondition_Flee::CBotAiCondition_Flee(CNpc* pBot, float fFleeFactor)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_FLEE, "BOTCONTROL_CONDITION_FLEE")
{
	m_dwTime = 0;
	m_fFleeFactor = fFleeFactor;
}

CBotAiCondition_Flee::~CBotAiCondition_Flee()
{
}


int CBotAiCondition_Flee::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);

	if (m_dwTime >= 1000)
	{
		m_dwTime = 0;

		if(GetBot()->IsMoveAble()) //not sure
		{
			if (GetBot()->ConsiderLPLow(m_fFleeFactor))
			{
				CNtlVector vFleeLoc;
				if (GetBot()->ConsiderFlee(vFleeLoc))
				{
					if (Dbo_CheckProbability(30)) // 30
					{
						CBotAiState* pState = GetBot()->GetBotController()->GetCurrentState();
						if (pState)
						{
							CBotAiAction_Look* pLook = new CBotAiAction_Look(GetBot(), GetBot()->GetTargetHandle(), 1000);
							if (pState->AddSubControlQueue(pLook, true))
							{
								CBotAiAction_Flee* pFlee = new CBotAiAction_Flee(GetBot(), vFleeLoc);
								if (pState->AddSubControlQueue(pFlee, true))
								{
									m_status = COMPLETED;
								}
								else
									m_status = FAILED;
							}
							else
								m_status = FAILED;
						}
					}
				}
			}
		}
	}

	return m_status;
}

