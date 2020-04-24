#include "stdafx.h"
#include "BotAiAction_Wander.h"
#include "Utils.h"
#include "BotAiAction_WanderMove.h"


CBotAiAction_Wander::CBotAiAction_Wander(CNpc* pBot)
:CBotAiAction(pBot, BOTCONTROL_ACTION_WANDER, "BOTCONTROL_ACTION_WANDER")
{
	m_dwTime = 0;
}

CBotAiAction_Wander::~CBotAiAction_Wander()
{
}

void CBotAiAction_Wander::OnEnter()
{
	m_dwTime = 0;
	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_WANDER, true);
}

void CBotAiAction_Wander::OnContinue()
{
	m_dwTime = 0;
}

void CBotAiAction_Wander::OnPause()
{
	RemoveAllSubControlQueue();
}

void CBotAiAction_Wander::OnExit()
{
	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_WANDER, false);
}

int CBotAiAction_Wander::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_subControlQueue.GetCount() > 0)
		UpdateSubControlQueue(dwTickDiff, fMultiple);
	else
		Wander(dwTickDiff);

	return m_status;
}


void CBotAiAction_Wander::Wander(DWORD dwTickTime)
{
	CNpc* pBot = GetBot();
	CActionPattern* pActionPattern = pBot->GetActionPattern();
	
	if (pBot->HasNearbyPlayer(false))
	{
		if (pActionPattern->GetWaitTime())
		{
			pActionPattern->SendUnitIdx();
			m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickTime);

			if (pActionPattern->GetWaitTime() > m_dwTime)
				return;

			m_dwTime = 0;
		}
		else
		{
			if (pActionPattern->IsActionMove())
			{
				if (pBot->IsMoveAble()) // NOT SURE vfptr[7].GetName
				{
					CNtlVector rDestLoc(pBot->GetCurLoc());

					if (pBot->ConsiderWanderLoc(rDestLoc))
					{
						if (pBot->GetCurLoc() != rDestLoc)
						{
							CBotAiAction_WanderMove* pWanderMove = new CBotAiAction_WanderMove(pBot, pActionPattern->GetActionPatternIndex(), rDestLoc, CNtlVector::ZERO);
							if (!AddSubControlQueue(pWanderMove, true))
							{
								this->m_status = FAILED;
								return;
							}
						}
					}
				}
			}
		}
		pActionPattern->ForwardActionPatternIndex();
	}
}