#include "stdafx.h"
#include "BotAiAction_Chase.h"
#include "ObjectManager.h"


CBotAiAction_Chase::CBotAiAction_Chase(CNpc* pBot, CBotAiAction_Chase::eATTACKTYPE AttackType, float fSkillAttackRange)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_CHASE, "BOTCONTROL_ACTION_CHASE")
{
	m_dwElapsedTimeSincedNextMovementApplied = 0;
	m_AttackType = AttackType;
	m_fSkillAttackRange = fSkillAttackRange;
	m_bIsPathFindingUsed = false;
	m_fBreakupStartPos = 0.0f;
	m_bIsBreakup = false;
}


CBotAiAction_Chase::~CBotAiAction_Chase()
{
}


void CBotAiAction_Chase::OnEnter()
{
	m_fBreakupStartPos = GetBot()->GetTbldat()->fRun_Speed;
}

void CBotAiAction_Chase::OnContinue()
{
	m_status = COMPLETED;
}

void CBotAiAction_Chase::OnPause()
{
	m_status = COMPLETED;
}

void CBotAiAction_Chase::OnExit()
{
	if (!m_bIsBreakup)
	{
		GetBot()->SendCharStateStanding();
	}

	if (m_bIsPathFindingUsed)
	{
		GetBot()->GetPathFinder()->Reset();
	}
}

int CBotAiAction_Chase::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwElapsedTimeSincedNextMovementApplied = UnsignedSafeIncrease<DWORD>(m_dwElapsedTimeSincedNextMovementApplied, dwTickDiff);

	if(!GetBot()->IsMoveAble()) //not sure
	{
		m_status = COMPLETED;
		return m_status;
	}

	if (!GetBot()->IsChaseAble()) //not sure
	{
		m_status = COMPLETED;
		return m_status;
	}

	CCharacter* pTarget = g_pObjectManager->GetChar(GetBot()->GetTargetHandle());
	if (!pTarget)
	{
		m_status = COMPLETED;
		return m_status;
	}

	bool bIsChasingCheatingTarget = GetBot()->IsChasingCheatingTarget();

	if (m_AttackType == CBotAiAction_Chase::ATTACKTYPE_SKILL)
	{
		if (bIsChasingCheatingTarget)
		{
			if (GetBot()->IsInRange(GetBot()->GetCheatingTargetCollisionLoc(), m_fSkillAttackRange * 70.0f / 100.0f))
			{
				m_status = COMPLETED;
				return m_status;
			}
		}
		else
		{
			bool bCanSeeOnLineOfSight = false;
			bool bKnowCanSeeOnLineOfSight = false;

			if (GetBot()->ConsiderRange(m_fSkillAttackRange, 30.0f / 100.0f))
			{
				if (!bKnowCanSeeOnLineOfSight)
				{
					bCanSeeOnLineOfSight = GetBot()->CanSeeOnLineOfSight(pTarget);
					bKnowCanSeeOnLineOfSight = true;
				}

				if (bCanSeeOnLineOfSight)
				{
					m_status = COMPLETED;
					return m_status;
				}
			}

			if (GetBot()->ConsiderRange(m_fSkillAttackRange, m_fBreakupStartPos))
			{
				if (!bKnowCanSeeOnLineOfSight)
				{
					bCanSeeOnLineOfSight = GetBot()->CanSeeOnLineOfSight(pTarget);
					bKnowCanSeeOnLineOfSight = true;
				}

				if (bCanSeeOnLineOfSight)
				{
				//	BreakupSkill(pTarget);		//disable because this dont work correctly
				//	if (m_bIsBreakup == true)
				//		return m_status;
				}
			}
		}

		if (!Following(pTarget))
			return m_status;
	}
	else
	{
		if (bIsChasingCheatingTarget)
		{
			if (GetBot()->IsInRange(GetBot()->GetCheatingTargetCollisionLoc(), GetBot()->GetAttackRange(pTarget) * 70.0f / 100.0f))
			{
				m_status = COMPLETED;
				return m_status;
			}
		}
		else
		{
			bool bCanSeeOnLineOfSight = false;
			bool bKnowCanSeeOnLineOfSight = false;

			if (GetBot()->ConsiderAttackRange(0.0f))
			{
				if (!bKnowCanSeeOnLineOfSight)
				{
					bCanSeeOnLineOfSight = GetBot()->CanSeeOnLineOfSight(pTarget);
					bKnowCanSeeOnLineOfSight = true;
				}

				if (bCanSeeOnLineOfSight)
				{
					GetBot()->SendCharStateStanding();
					m_status = COMPLETED;
					return m_status;
				}
			}

			if (GetBot()->ConsiderAttackRange(m_fBreakupStartPos))
			{
				if (!bKnowCanSeeOnLineOfSight)
				{
					bCanSeeOnLineOfSight = GetBot()->CanSeeOnLineOfSight(pTarget);
					bKnowCanSeeOnLineOfSight = true;
				}

				if (bCanSeeOnLineOfSight)
				{
				//	BreakupAttack(pTarget);		//disable because this dont work correctly
				//	if (m_bIsBreakup)
				//		return m_status;
				}
			}
		}

		if (!Following(pTarget))
			return m_status;
	}

	return m_status;
}


bool CBotAiAction_Chase::BreakupAttack(CCharacter* pTarget)
{
	int nHaterCount = pTarget->GetMeHaterManager()->GetContainerMeHater()->GetMeHaterCount();
	//printf("BreakupAttack \n");
	if(GetBot()->CCharacterObject::GetAttackRange() > 2.0f)
	{
		if (IsBreakupAIRun_GreatDistance())
		{
			m_bIsBreakup = BreakupAttack_GreatDistance(nHaterCount);
			return m_bIsBreakup;
		}
		else
		{
			m_bIsBreakup = false;
			return true;
		}
	}
	else if (IsBreakupAIRun_ShortDistance())
	{
		m_bIsBreakup = BreakupAttack_ShortDistance(nHaterCount);
		return m_bIsBreakup;
	}
	else
	{
		m_bIsBreakup = false;
		return true;
	}

	return false;
}

bool CBotAiAction_Chase::BreakupAttack_ShortDistance(BYTE byBotCount)
{
	CCharacter* pTarget = g_pObjectManager->GetChar(GetBot()->GetTargetHandle());
	if (pTarget)
	{
		pTarget->GetBotBreakUp()->AddNavMove(GetBot(), GetBot()->GetAttackRange(pTarget), byBotCount, CBotBreakUp::BREAKUP_MOVETYPE_CHASE);
	}

	return false;
}

bool CBotAiAction_Chase::BreakupAttack_GreatDistance(BYTE byBotCount)
{
	CCharacter* pTarget = g_pObjectManager->GetChar(GetBot()->GetTargetHandle());
	if (pTarget)
	{
		pTarget->GetBotBreakUp()->AddNavMove(GetBot(), GetBot()->GetAttackRange(pTarget), byBotCount, CBotBreakUp::BREAKUP_MOVETYPE_CHASE);
	}

	return false;
}


bool CBotAiAction_Chase::BreakupSkill(CCharacter* pTarget)
{
	int nHaterCount = pTarget->GetMeHaterManager()->GetContainerMeHater()->GetMeHaterCount();

	if (m_fSkillAttackRange > 2.0f)
	{
		if (IsBreakupAIRun_GreatDistance())
		{
			m_bIsBreakup = BreakupSkill_GreatDistance(nHaterCount);
			return m_bIsBreakup;
		}
		else
		{
			m_bIsBreakup = 0;
			return true;
		}
	}
	else if (IsBreakupAIRun_ShortDistance())
	{
		m_bIsBreakup = BreakupSkill_ShortDistance(nHaterCount);
		return m_bIsBreakup;
	}
	else
	{
		m_bIsBreakup = 0;
		return true;
	}

	return false;
}

bool CBotAiAction_Chase::BreakupSkill_ShortDistance(BYTE byBotCount)
{
	CCharacter* pTarget = g_pObjectManager->GetChar(GetBot()->GetTargetHandle());
	if (pTarget)
	{
		pTarget->GetBotBreakUp()->AddNavMove(GetBot(), GetBot()->GetTargetAndMeBodySizeRange(pTarget) + m_fSkillAttackRange, byBotCount, CBotBreakUp::BREAKUP_MOVETYPE_CHASE);
	}

	return false;
}

bool CBotAiAction_Chase::BreakupSkill_GreatDistance(BYTE byBotCount)
{
	CCharacter* pTarget = g_pObjectManager->GetChar(GetBot()->GetTargetHandle());
	if (pTarget)
	{
		pTarget->GetBotBreakUp()->AddNavMove(GetBot(), m_fSkillAttackRange, byBotCount, CBotBreakUp::BREAKUP_MOVETYPE_CHASE);
	}

	return false;
}


bool CBotAiAction_Chase::IsBreakupAIRun_ShortDistance()
{
	CCharacter* pTarget = g_pObjectManager->GetChar(GetBot()->GetTargetHandle());
	if (pTarget)
	{
		struct sFOREACH
		{
			CNpc*			pMe;
			CCharacter*		pTarget;
			BYTE			byCount;

			void operator () (std::pair<HOBJECT, HOBJECT> value)
			{
				CCharacter* pAssociate = g_pObjectManager->GetChar(value.first);
				if (pAssociate)
				{
					if (pAssociate->GetID() != pMe->GetID())
					{
						if (pAssociate->CCharacterObject::GetAttackRange() > 2.0f)
						{
							if (pTarget->IsInRange(pAssociate, pAssociate->GetAttackRange(pTarget)))
								++byCount;
						}
						else
						{
							++byCount;
						}
					}
				}
			}
		};

		sFOREACH sForEach;
		sForEach.pMe = GetBot();
		sForEach.pTarget = pTarget;
		sForEach.byCount = 0;

		pTarget->GetMeHaterManager()->GetContainerMeHater()->ForEachCondition(sForEach);
		//printf("sForEach.byCount %u \n", sForEach.byCount);
		return sForEach.byCount > 0;
	}

	return false;
}

bool CBotAiAction_Chase::IsBreakupAIRun_GreatDistance()
{
	CCharacter* pTarget = g_pObjectManager->GetChar(GetBot()->GetTargetHandle());
	if (pTarget)
	{
		struct sFOREACH
		{
			CNpc*			pMe;
			BYTE			byCount;

			void operator () (std::pair<HOBJECT, HOBJECT> value)
			{
				CCharacter* pAssociate = g_pObjectManager->GetChar(value.first);
				if (pAssociate)
				{
					if (pAssociate->GetID() != pMe->GetID())
					{
						if (pAssociate->CCharacterObject::GetAttackRange() > 2.0f)
							++byCount;
					}
				}
			}
		};

		sFOREACH sForEach;
		sForEach.pMe = GetBot();
		sForEach.byCount = 0;

		pTarget->GetMeHaterManager()->GetContainerMeHater()->ForEachCondition(sForEach);

	//	printf("IsBreakupAIRun_GreatDistance: byCount %u \n", sForEach.byCount);

		return sForEach.byCount > 0;
	}

	return false;
}

bool CBotAiAction_Chase::Following(CCharacter* pTarget)
{
	bool bNeedToConsiderNextMovement = false;

	if (GetBot()->IsBotStateMovingMode())
	{
		if (m_bIsPathFindingUsed)
		{
			if (GetBot()->CONSIDERING_NEXT_MOVEMENT_INTERVAL_IN_MILLISECS <= m_dwElapsedTimeSincedNextMovementApplied)
				bNeedToConsiderNextMovement = true;
		}
		else bNeedToConsiderNextMovement = true;
	}
	else bNeedToConsiderNextMovement = true;

	if (bNeedToConsiderNextMovement == false)
		return true;

	bool bIsChasingCheatingTarget = GetBot()->IsChasingCheatingTarget();

	sVECTOR3 avDestLoc[DBO_MAX_NEXT_DEST_LOC_COUNT];

	if (bIsChasingCheatingTarget)
	{
		//GetBot()->SendRunFlagIfNeeded(true);
		GetBot()->GetCheatingTargetCollisionLoc().CopyTo(avDestLoc[0]);
		GetBot()->SendCharStateDestMove(false, CNtlVector::INVALID_XZ, -1, 1, avDestLoc, true);
		m_dwElapsedTimeSincedNextMovementApplied = 0;
		return true;
	}

	int nPathFindingResult = 0;
	BYTE nextStateId = GetBot()->ConsiderPathMovement(pTarget, &nPathFindingResult);

	if (nextStateId == CHARSTATE_STANDING)
	{
		GetBot()->SendCharStateStanding();
		m_bIsPathFindingUsed = false;
		m_dwElapsedTimeSincedNextMovementApplied = 0;
		return true;
	}
	else if (nextStateId == CHARSTATE_DESTMOVE)
	{
		if (nPathFindingResult == 1)
		{
			//CBotNpc::SendRunFlagIfNeeded(v13, 1);
		}
		else
		{
			if (nPathFindingResult != 2)
			{
				return false;
			}

			BYTE byNextLocCount = GetBot()->GetPathFinder()->GetAllNextNavLoc(avDestLoc);
			//CBotNpc::SendRunFlagIfNeeded(v16, 1);
			GetBot()->SendCharStateDestMove(false, CNtlVector::INVALID_XZ, -1, byNextLocCount, avDestLoc, true);
		}

		m_bIsPathFindingUsed = true;
		m_dwElapsedTimeSincedNextMovementApplied = 0;
		return true;
	}
	else if (nextStateId == CHARSTATE_FOLLOWING)
	{
		if (GetBot()->GetCharStateID() != CHARSTATE_FOLLOWING)
		{
			float fTmpAttackRange = m_fSkillAttackRange;
			BYTE followtype = DBO_MOVE_FOLLOW_SKILL_ATTACK;
			if (m_AttackType == ATTACKTYPE_ATTACK)
			{
				fTmpAttackRange = GetBot()->GetAttackFollowRange();
				followtype = DBO_MOVE_FOLLOW_AUTO_ATTACK;
			}

			pTarget->GetCurLoc().CopyTo(avDestLoc[0]);

			GetBot()->SendCharStateFollowing(GetBot()->GetTargetHandle(), fTmpAttackRange, followtype, avDestLoc[0], true);
			m_bIsBreakup = false;
		}

		m_bIsPathFindingUsed = false;
		m_dwElapsedTimeSincedNextMovementApplied = 0;
		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : nextStateId has an invalid value., nextStateId = %d", nextStateId);

	return false;
}