#include "stdafx.h"
#include "BotAiAction_WanderMove.h"
#include "BotPathFinder.h"
#include "BotAiAction_DestMove.h"

CBotAiAction_WanderMove::CBotAiAction_WanderMove(CNpc* pBot, BYTE actionPatternIndex, CNtlVector& rDestLoc, CNtlVector& rDestDir)
:CBotAiAction(pBot, BOTCONTROL_ACTION_WANDERMOVE, "BOTCONTROL_ACTION_WANDERMOVE")
{
	m_actionPatternIndex = actionPatternIndex;
	m_vDestLoc.operator=(rDestLoc);
	m_vDestDir.operator=(rDestDir);
	m_bWanderFailed = false;
}

CBotAiAction_WanderMove::~CBotAiAction_WanderMove()
{
}

void CBotAiAction_WanderMove::OnEnter()
{
//	printf("CBotAiAction_WanderMove::OnEnter() \n");
	RemoveAllSubControl();

	CNpc* pBot = GetBot();

	if (pBot->GetPathFinder())
	{
		pBot->GetPathFinder()->SetDestLoc(m_vDestLoc);

		CBotPathFinder::ePATH_FINDING_RESULT pathfindres = pBot->GetPathFinder()->WanderPathFind();
		if (pathfindres != CBotPathFinder::PATH_FINDING_RESULT_FAILED && pathfindres != CBotPathFinder::PATH_FINDING_RESULT_SRC_FAILED)
		{
			sVECTOR3 destLoc[DBO_MAX_NEXT_DEST_LOC_COUNT];
			BYTE byDestLocCount = pBot->GetPathFinder()->GetAllNextNavLoc(destLoc);

			CBotAiAction_DestMove* destmove = new CBotAiAction_DestMove(pBot, m_actionPatternIndex, byDestLocCount, destLoc, false, false, CNtlVector::ZERO);

			AddSubControlQueue(destmove, true);
		}
		else
		{
			ERR_LOG(LOG_BOTAI, "WanderPathFind() failed. Result %u", pathfindres);
			m_bWanderFailed = true;
		}
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "fail : CBotPathFinder object couldn't be found.(NULL == GetBot()->GetPathFinder())");
	}
}

void CBotAiAction_WanderMove::OnExit()
{
}

int CBotAiAction_WanderMove::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	//printf("CBotAiAction_WanderMove::OnUpdate() \n");
	if (m_bWanderFailed)
		return m_status;

	if (UpdateSubControlQueue(dwTickDiff, fMultiple) == COMPLETED)
	{
		if (!m_vDestDir.IsZero() && !m_vDestDir.IsInvalid(false))
		{
			GetBot()->SetCurDir(m_vDestDir);
		}
		m_status = COMPLETED;
	}
	
	return m_status;
}
