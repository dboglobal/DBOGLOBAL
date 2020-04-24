#include "stdafx.h"
#include "BotAiAction_Patrol.h"
#include "SPSNodeAction_PathMove.h"
#include "NtlRandom.h"
#include "BotAiAction_NavMove.h"
#include "BotAiCondition_Facing.h"
#include "BotAiAction_DestMove.h"


CBotAiAction_Patrol::CBotAiAction_Patrol(CNpc* pBot, TBLIDX pathTblidx, bool bRunMode, float fPointRadius)
:CBotAiAction(pBot, BOTCONTROL_ACTION_PATROL, "BOTCONTROL_ACTION_PATROL")
{
	m_pathTblidx = pathTblidx;
	m_bRunMode = bRunMode;
	m_fPointRadius = fPointRadius;
	m_bIsScript = false;
}

CBotAiAction_Patrol::CBotAiAction_Patrol(CNpc* pBot)
: CBotAiAction(pBot, BOTCONTROL_ACTION_PATROL, "BOTCONTROL_ACTION_PATROL")
{
	m_pathTblidx = INVALID_TBLIDX;
	m_bRunMode = false;
	m_fPointRadius = 0.0f;
	m_bIsScript = false;
}

CBotAiAction_Patrol::~CBotAiAction_Patrol()
{
}


bool CBotAiAction_Patrol::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_PathMove* pAction = dynamic_cast<CSPSNodeAction_PathMove*>(pControlScriptNode);
	if (pAction)
	{
		m_pathTblidx = pAction->m_pathTblidx;
		m_bRunMode = pAction->m_bRunMode;
		m_fPointRadius = pAction->m_fPointRadius;
		m_bIsScript = true;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_PathMove", pControlScriptNode);
	return false;
}

void CBotAiAction_Patrol::OnEnter()
{
	if (GetBot()->GetCurWorld() == NULL)
	{
		m_status = FAILED;
		return;
	}
	
	CNpc* pBot = GetBot();

	if (m_wayPointList.IsEmpty())
	{
		if (m_wayPointList.Create(pBot->GetWorldTblidx(), m_pathTblidx) == false || m_wayPointList.GetMaxStep() == 0)
		{
			//ERR_LOG(LOG_BOTAI, "Failed to create waypoint list");
			m_status = SYSTEMERROR;
			return;
		}
	}


	RemoveAllSubControl();

	CNtlVector rLoc;
	if (m_wayPointList.GetCurWayPoint(rLoc))
	{
		if (m_fPointRadius == 0.0f)
			m_fPointRadius = m_wayPointList.GetWayPointRadius();

		if (m_fPointRadius != 0.0f)
		{
			rLoc.x += RandomRangeF(-0.0f - m_fPointRadius, m_fPointRadius);
			rLoc.z += RandomRangeF(-0.0f - m_fPointRadius, m_fPointRadius);
		}

		CNtlVector rSecondDestLoc;
		bool bHaveSecondDestLoc = m_wayPointList.GetSecondNextWayPoint(rSecondDestLoc);

		CBotAiAction_NavMove* pNavMove = new CBotAiAction_NavMove(pBot, rLoc, m_bRunMode, bHaveSecondDestLoc, rSecondDestLoc, CNtlVector::ZERO, INVALID_FLOAT);
		if (AddSubControlQueue(pNavMove, true))
		{
			if (GetBot()->HasFunction(NPC_FUNC_FLAG_FACING))
			{
				if(AddSubControlList(new CBotAiCondition_Facing(pBot), false))
					pBot->SetActionStatusFlag(BOT_ACTIONSTATUS_PATROL, true);
				else
					m_status = FAILED;
			}
		}
		else
		{
			m_status = FAILED;
		}
	}
	else
	{
		printf("m_wayPointList.GetCurWayPoint(rLoc) = fail \n");
		m_status = FAILED;
	}
}

void CBotAiAction_Patrol::OnExit()
{
	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_PATROL, false);
}

int CBotAiAction_Patrol::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (UpdateSubControlList(dwTickDiff, fMultiple) == CHANGED)
	{
		m_status = INACTIVATED;
		return m_status;
	}

	if (UpdateSubControlQueue(dwTickDiff, fMultiple) != COMPLETED)
		return m_status;


	CNpc* pBot = GetBot();

	CNtlVector vCurWayPoint(pBot->GetEnterLoc());

	if (m_wayPointList.GetNextWayPoint(vCurWayPoint) == false)
	{
		if (pBot->GetMoveType() == SPAWN_MOVE_PATH_ONCE || m_bIsScript == true)
		{
			m_status = COMPLETED;
			return m_status;
		}

		m_wayPointList.InitStep();
	}
	

	if (m_fPointRadius != 0.0f)
	{
		vCurWayPoint.x += RandomRangeF(-0.0f - m_fPointRadius, m_fPointRadius);
		vCurWayPoint.z += RandomRangeF(-0.0f - m_fPointRadius, m_fPointRadius);
	}

	CNtlVector rSecondDestLoc;
	bool bHaveSecondDestLoc = m_wayPointList.GetSecondNextWayPoint(rSecondDestLoc);

	CBotAiAction_NavMove* pNavMove = new CBotAiAction_NavMove(pBot, vCurWayPoint, m_bRunMode, bHaveSecondDestLoc, rSecondDestLoc, CNtlVector::ZERO, INVALID_FLOAT);
	if (!AddSubControlQueue(pNavMove, true))
	{
		m_status = FAILED;
		return m_status;
	}
		
	return m_status;
}

