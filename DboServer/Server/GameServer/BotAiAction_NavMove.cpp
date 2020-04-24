#include "stdafx.h"
#include "BotAiAction_NavMove.h"
#include "SPSNodeAction_PointMove.h"
#include "SvrScrVariableMap.h"
#include "BotAiAction_DestMove.h"
#include "BotPathFinder.h"
#include "GameServer.h"
#include "GameMain.h"



CBotAiAction_NavMove::CBotAiAction_NavMove(CNpc* pBot, CNtlVector& rDestLoc, bool bRunMode, bool bHaveSecondDestLoc, CNtlVector& rSecondDestLoc, CNtlVector& rDestDir, float fMoveSpeed)
:CBotAiAction(pBot, BOTCONTROL_ACTION_NAVMOVE, "BOTCONTROL_ACTION_NAVMOVE")
{
	m_vDestLoc.operator=(rDestLoc);
	m_vSecondDestLoc.operator=(rSecondDestLoc);
	m_vDestDir.operator=(rDestDir);
	m_bRunMode = bRunMode;
	m_bHaveSecondDestLoc = bHaveSecondDestLoc;
	m_fMoveSpeed = fMoveSpeed;
}

CBotAiAction_NavMove::CBotAiAction_NavMove(CNpc* pBot)
:CBotAiAction(pBot, BOTCONTROL_ACTION_NAVMOVE, "BOTCONTROL_ACTION_NAVMOVE")
{
	m_bRunMode = false;
	m_bHaveSecondDestLoc = false;
	m_fMoveSpeed = INVALID_FLOAT;
}

CBotAiAction_NavMove::~CBotAiAction_NavMove()
{
}


bool CBotAiAction_NavMove::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_PointMove* pPointMove = dynamic_cast<CSPSNodeAction_PointMove*>(pControlScriptNode);
	if (pPointMove)
	{
		m_vDestLoc = pPointMove->m_vDestLoc;
		m_vDestDir = pPointMove->m_vDestDir;
		m_bRunMode = pPointMove->m_bRunMode;
		m_fMoveSpeed = pPointMove->m_fMoveSpeed;

		//get Y loc
		if (m_vDestLoc.y == 0.0f)
		{
			if (GetBot()->GetCurWorld())
			{
				CGameServer* app = (CGameServer*)g_pApp;

				m_vDestLoc.y = app->GetGameMain()->GetWorldManager()->GetAdjustedHeight(GetBot()->GetCurWorld()->GetID(), m_vDestLoc.x, m_vDestLoc.y, m_vDestLoc.z, 5000);

				return true;
			}
			else
			{
				ERR_LOG(LOG_SCRIPT, "The OBJ %u doesn't belong to any world. GetBot()->GetID() = %u, GetBot()->GetTblidx() = %u", GetBot()->GetObjType(), GetBot()->GetID(), GetBot()->GetTblidx());
			}
		}
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "fail : Cant dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_PointMove", pControlScriptNode);
	}

	return false;
}

void CBotAiAction_NavMove::OnEnter()
{
	DoNavMove();
}

void CBotAiAction_NavMove::OnContinue()
{
	RemoveAllSubControl();
	DoNavMove();
}

void CBotAiAction_NavMove::OnPause()
{
}

void CBotAiAction_NavMove::OnExit()
{
//	printf("CBotAiAction_NavMove::OnExit() \n");
}

int CBotAiAction_NavMove::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	//printf("CBotAiAction_NavMove::OnUpdate() \n");
	if (UpdateSubControlQueue(dwTickDiff, fMultiple) == COMPLETED)
	{
		if (!m_vDestDir.IsZero() && !m_vDestDir.IsInvalid(false))
		{
			//here send change heading packet
			GetBot()->SetCurDir(m_vDestDir);
		}
		m_status = COMPLETED;
	}

	return m_status;
}


void CBotAiAction_NavMove::DoNavMove()
{
	//printf("CBotAiAction_NavMove::DoNavMove() \n");
	if (GetBot()->HasFunction(NPC_FUNC_FLAG_BUS))
	{
		sVECTOR3 pavLoc;
		m_vDestLoc.CopyTo(pavLoc);
		CBotAiAction_DestMove* pDestMove = new CBotAiAction_DestMove(GetBot(), BOTAP_MOVE, 1, &pavLoc, m_bRunMode, m_bHaveSecondDestLoc, m_vSecondDestLoc);
		AddSubControlQueue(pDestMove, true);
	}
	else
	{
		CNpc* pBot = GetBot();
		if (pBot->GetPathFinder())
		{
			pBot->GetPathFinder()->SetDestLoc(m_vDestLoc);

			if (m_fMoveSpeed != INVALID_FLOAT)
			{
				printf("update move speed \n");
				/*CBot::SetWalkingSpeed((CBot *)&v19->vfptr, v68->m_fMoveSpeed);
				CBot::SetRunningSpeed((CBot *)&v20->vfptr, v68->m_fMoveSpeed);*/
				//pBot->UpdateMoveSpeed(m_fMoveSpeed);
			}

			if (pBot->GetPathFinder()->PathFind())
			{
				sVECTOR3 destLoc[DBO_MAX_NEXT_DEST_LOC_COUNT];
				BYTE byDestLocCount = pBot->GetPathFinder()->GetAllNextNavLoc(destLoc);

				CBotAiAction_DestMove* destmove = new CBotAiAction_DestMove(pBot, BOTAP_MOVE, byDestLocCount, destLoc, m_bRunMode, m_bHaveSecondDestLoc, m_vSecondDestLoc);
				AddSubControlQueue(destmove, true);
			}
		}
		else
		{
			ERR_LOG(LOG_BOTAI, "fail : CBotPathFinder object couldn't be found.(NULL == GetBot()->GetPathFinder())");
		}
	}
}
