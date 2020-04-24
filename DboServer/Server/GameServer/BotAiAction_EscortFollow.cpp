#include "stdafx.h"
#include "BotAiAction_EscortFollow.h"


CBotAiAction_EscortFollow::CBotAiAction_EscortFollow(CNpc* pBot, CNtlVector& rLoc, float fRadius, bool bRunMode, DWORD eventID)
:CBotAiAction(pBot, BOTCONTROL_ACTION_ESCORT_FOLLOW, "BOTCONTROL_ACTION_ESCORT_FOLLOW")
{
	m_dwArriveWaitTime = 0;
	m_dwStopTime = 0;
	m_hTarget = INVALID_HOBJECT;
	m_vDestLoc = rLoc;
	m_fRadius = fRadius;
	m_bRunMode = bRunMode;
	m_eventID = eventID;
	m_bArrived = false;
}

CBotAiAction_EscortFollow::~CBotAiAction_EscortFollow()
{
}


void CBotAiAction_EscortFollow::OnEnter()
{
	m_dwArriveWaitTime = 0;
	m_dwStopTime = 0;

	if (GetBot()->GetEscortManager()->GetCurrentTarget())
	{
		sVECTOR3 vDest;
		m_vDestLoc.CopyTo(vDest);
		GetBot()->SendCharStateFollowing(m_hTarget, GetBot()->GetAttackFollowRange(), DBO_MOVE_FOLLOW_FRIENDLY, vDest, m_bRunMode);
	}
}

void CBotAiAction_EscortFollow::OnExit()
{
	GetBot()->SendCharStateStanding();
}

int CBotAiAction_EscortFollow::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharacter* pTarget = GetBot()->GetEscortManager()->GetCurrentTarget();
	if (pTarget)
	{
		m_dwStopTime = 0;
		if (m_bArrived)
		{
			m_dwArriveWaitTime = UnsignedSafeIncrease<DWORD>(m_dwArriveWaitTime, dwTickDiff);
			if (GetBot()->GetEscortManager()->GetTargetCount() == 0)
			{
				m_status = COMPLETED;
				return m_status;
			}

			if (m_dwArriveWaitTime >= 60000)
			{
				GetBot()->GetBotController()->ChangeControlState_Leave();
				m_status = FAILED;
				return m_status;
			}

		}
		else
		{
			if (GetBot()->IsInRange(m_vDestLoc, m_fRadius))
			{
				GetBot()->GetEscortManager()->SendEvent(TS_EVENT_SEND_TYPE_PARTY, 0, m_eventID); // 0 = TS_TYPE_QUEST_CS
				GetBot()->SendCharStateStanding();
				m_bArrived = true;
			}
			else if (m_hTarget != pTarget->GetID())
			{
				m_hTarget = pTarget->GetID();
				sVECTOR3 vDest;
				pTarget->GetCurLoc().CopyTo(vDest);
				GetBot()->SendCharStateFollowing(m_hTarget, GetBot()->GetAttackFollowRange(), DBO_MOVE_FOLLOW_FRIENDLY, vDest, m_bRunMode);
			}
		}

		if (UpdateSubControlList(dwTickDiff, fMultiple) == CHANGED)
			m_status = INACTIVATED;
	}
	else
	{
		if (!m_bArrived)
			GetBot()->GetBotController()->ChangeControlState_Leave();

		m_status = COMPLETED;
	}

	return m_status;
}
