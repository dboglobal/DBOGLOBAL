#include "stdafx.h"
#include "BotAiAction_ReturnMove.h"
#include "BotAiAction_DestMove.h"


CBotAiAction_ReturnMove::CBotAiAction_ReturnMove(CNpc* pBot, CNtlVector& rDestLoc, CNtlVector& rDestDir)
:CBotAiAction(pBot, BOTCONTROL_ACTION_RETURNMOVE, "BOTCONTROL_ACTION_RETURNMOVE")
{
	m_vDestLoc.operator=(rDestLoc);
	m_vDestDir.operator=(rDestDir);
}

CBotAiAction_ReturnMove::~CBotAiAction_ReturnMove()
{
}

void CBotAiAction_ReturnMove::OnEnter()
{
	RemoveAllSubControl();

	if(GetBot()->GetPathFinder())
	{
		GetBot()->GetPathFinder()->SetDestLoc(m_vDestLoc);

		BYTE byNextLocCount = 0;

		CBotPathFinder::ePATH_FINDING_RESULT eFindResult = GetBot()->GetPathFinder()->PathFind();

		sVECTOR3 avDestLoc[DBO_MAX_NEXT_DEST_LOC_COUNT];
		if (eFindResult)
		{
			byNextLocCount = GetBot()->GetPathFinder()->GetAllNextNavLoc(avDestLoc);
		}
		else
		{
			//ERR_LOG(LOG_BOTAI, "ComeBackMove failed.tblidx[%u] objtype[%u] path idx[%u] worldid[%u] CurLoc[%f, %f, %f] DestLoc[%f, %f, %f]", 
			//	GetBot()->GetTblidx(), GetBot()->GetObjType(), GetBot()->GetPathTblidx(), GetBot()->GetWorldID(), GetBot()->GetCurLoc().x, GetBot()->GetCurLoc().y, GetBot()->GetCurLoc().z, m_vDestLoc.x, m_vDestLoc.y, m_vDestLoc.z);

			byNextLocCount = 1;
			m_vDestLoc.CopyTo(avDestLoc[0]);
		}

		CBotAiAction_DestMove* pDestMove = new CBotAiAction_DestMove(GetBot(), INVALID_BYTE, byNextLocCount, avDestLoc, true, false, CNtlVector::INVALID_XZ);
		AddSubControlQueue(pDestMove, true);
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "fail : CBotPathFinder object couldn't be found.(NULL == GetBot()->GetPathFinder())");
	}
}

void CBotAiAction_ReturnMove::OnExit()
{
}

int CBotAiAction_ReturnMove::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (CComplexState::UpdateSubControlQueue(dwTickDiff, fMultiple) == COMPLETED)
	{
		if (!m_vDestDir.IsZero() && !m_vDestDir.IsInvalid(false))
		{
			GetBot()->SetCurDir(m_vDestDir);
		}

		m_status = COMPLETED;
	}

	return m_status;
}

