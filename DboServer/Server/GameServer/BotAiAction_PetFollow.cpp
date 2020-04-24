#include "stdafx.h"
#include "BotAiAction_PetFollow.h"
#include "SummonPet.h"


CBotAiAction_PetFollow::CBotAiAction_PetFollow(CNpc* pBot, HOBJECT hTarget, CNtlVector& vDestLoc)
:CBotAiAction(pBot, BOTCONTROL_ACTION_PET_FOLLOW, "BOTCONTROL_ACTION_PET_FOLLOW")
{
	m_hTarget = hTarget;
	m_vDestLoc.operator=(vDestLoc);
	m_dwElapsedTimeSincedNextMovementApplied = 0;
	m_bIsPathFindingUsed = false;
}

CBotAiAction_PetFollow::~CBotAiAction_PetFollow()
{
}

void CBotAiAction_PetFollow::OnEnter()
{
	if (CCharacter* pChar = g_pObjectManager->GetChar(m_hTarget))
	{
		// GetBot()->SendRunFlagIfNeeded(false);
	}
}

void CBotAiAction_PetFollow::OnExit()
{
	GetBot()->SendCharStateStanding();

	if (m_bIsPathFindingUsed)
	{
		GetBot()->GetPathFinder()->Reset();
	}

	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_PET_FOLLOW, false);
}

int CBotAiAction_PetFollow::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwElapsedTimeSincedNextMovementApplied = UnsignedSafeIncrease<DWORD>(m_dwElapsedTimeSincedNextMovementApplied, dwTickDiff);

	if (UpdateSubControlList(dwTickDiff, fMultiple) == CHANGED)
	{
		m_status = INACTIVATED;
		return m_status;
	}

	CCharacter* pTarget = g_pObjectManager->GetChar(m_hTarget);
	if (pTarget == NULL || pTarget->IsInitialized() == false)
	{
		m_status = COMPLETED;
		return m_status;
	}

	if (CNpc::CONSIDERING_NEXT_MOVEMENT_INTERVAL_IN_MILLISECS > m_dwElapsedTimeSincedNextMovementApplied || pTarget->GetCurWorld() == NULL)
	{
		return m_status;
	}

	if (GetBot()->IsInRange(pTarget, (float)CSummonPet::FOLLOW_END_DISTANCE_IN_METERS))
	{
		m_status = COMPLETED;
		return m_status;
	}

	int nPathFindingResult = 0;
	BYTE nextStateId = GetBot()->ConsiderPathMovement(pTarget, &nPathFindingResult);

	if (nextStateId == CHARSTATE_STANDING)
	{
		if (nPathFindingResult)
		{
			GetBot()->SendCharStateStanding();
		}
		else
		{
			CNtlVector rvNewDir;
			CNtlVector rvNewLoc;
			CNtlVector rvNewDestLoc;
			float rfActualSpeed = 0.0f;
			std::list<sVECTOR3> rlistConsumedDestLoc;

			pTarget->PredictMovement(CNpc::CONSIDERING_NEXT_MOVEMENT_INTERVAL_IN_MILLISECS, rvNewDir, rvNewLoc, rvNewDestLoc, &rlistConsumedDestLoc, &rfActualSpeed);

			sVECTOR3 destLoc;
			rvNewLoc.CopyTo(destLoc);
			GetBot()->SendCharStateDestMove(false, CNtlVector::INVALID_XZ, -1, 1, &destLoc, true);
		}

		m_bIsPathFindingUsed = false;
		m_dwElapsedTimeSincedNextMovementApplied = 0;
		return m_status;
	}
	else if (nextStateId == CHARSTATE_DESTMOVE)
	{
		if (nPathFindingResult == 1)
		{
			//CBotNpc::SendRunFlagIfNeeded(v14, 1);
		}
		else
		{
			if (nPathFindingResult != 2)
			{
				m_status = FAILED;
				return m_status;
			}

			sVECTOR3 avDestLoc[DBO_MAX_NEXT_DEST_LOC_COUNT];
			BYTE byNextLocCount = GetBot()->GetPathFinder()->GetAllNextNavLoc(avDestLoc);

			GetBot()->SendCharStateDestMove(false, CNtlVector::INVALID_XZ, -1, byNextLocCount, avDestLoc, true);
		}

		m_bIsPathFindingUsed = true;

		m_dwElapsedTimeSincedNextMovementApplied = 0;
		return m_status;
	}
	else if (nextStateId == CHARSTATE_FOLLOWING)
	{
		if (GetBot()->GetCharStateID() != CHARSTATE_FOLLOWING)
		{
			sVECTOR3 destLoc;
			pTarget->GetCurLoc().CopyTo(destLoc);
			GetBot()->SendCharStateFollowing(pTarget->GetID(), (float)CSummonPet::FOLLOW_END_DISTANCE_IN_METERS, DBO_MOVE_FOLLOW_FRIENDLY, destLoc, true);
		}

		m_bIsPathFindingUsed = false;
		m_dwElapsedTimeSincedNextMovementApplied = 0;
		return m_status;
	}


	return m_status;
}

