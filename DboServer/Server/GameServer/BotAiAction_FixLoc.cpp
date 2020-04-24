#include "stdafx.h"
#include "BotAiAction_FixLoc.h"
#include "Utils.h"
#include "BotAiAction_NavMove.h"

CBotAiAction_FixLoc::CBotAiAction_FixLoc(CNpc* pBot)
:CBotAiAction(pBot, BOTCONTROL_ACTION_FIXLOC, "BOTCONTROL_ACTION_FIXLOC")
{
	m_dwTime = 0;
}

CBotAiAction_FixLoc::~CBotAiAction_FixLoc()
{
}


void CBotAiAction_FixLoc::OnEnter()
{
	//printf("CBotAiAction_FixLoc::OnEnter() \n");
	m_dwTime = 0;
	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_FIXLOC, true);
}

void CBotAiAction_FixLoc::OnContinue()
{
	//printf("CBotAiAction_FixLoc::OnContinue() \n");
	m_dwTime = 0;
}

void CBotAiAction_FixLoc::OnPause()
{
//	printf("CBotAiAction_FixLoc::OnPause() \n");
	RemoveAllSubControlQueue();
}

void CBotAiAction_FixLoc::OnExit()
{
//	printf("CBotAiAction_FixLoc::OnExit() \n");
	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_FIXLOC, false);
}

int CBotAiAction_FixLoc::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
//	printf("CBotAiAction_FixLoc::OnUpdate() \n");
	if (m_subControlQueue.GetCount())
		UpdateSubControlQueue(dwTickDiff, fMultiple);
	else
		FixLoc(dwTickDiff);

	return m_status;
}


void CBotAiAction_FixLoc::FixLoc(DWORD dwTickTime)
{
	//printf("CBotAiAction_FixLoc::FixLoc() \n");
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickTime);

	CNpc* pBot = GetBot();
	CActionPattern* pActionPattern = pBot->GetActionPattern();

	if (pActionPattern->GetWaitTime() < m_dwTime)
	{
		m_dwTime = 0;

		if(GetBot()->IsMoveAble()) //not sure
		{
			if (pBot->GetCurLoc().x != pBot->GetEnterLoc().x || pBot->GetCurLoc().z != pBot->GetEnterLoc().z)
			{
				CBotAiAction_NavMove* pNavMove = new CBotAiAction_NavMove(pBot, pBot->GetEnterLoc(), true, false, CNtlVector::INVALID_XZ, pBot->GetEnterDir(), INVALID_FLOAT);
				if (!AddSubControlQueue(pNavMove, true))
				{
					ERR_LOG(LOG_BOTAI, "false == AddSubControlQueue( new CBotAiAction_NavMove() )");
					m_status = 6;
				}
			}
		}
	}
	
}
