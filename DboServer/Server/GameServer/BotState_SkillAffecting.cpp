#include "stdafx.h"
#include "BotState_SkillAffecting.h"
#include "ObjectManager.h"
#include "char.h"
#include "TableContainerManager.h"
#include "SystemEffectTable.h"
#include "NtlPacketGU.h"
#include "NtlMovement.h"
#include "StateManager.h"


CBotState_SkillAffecting::CBotState_SkillAffecting(CCharacter* pParent)
	:CCharState(CHARSTATE_SKILL_AFFECTING, pParent)
{
	m_hTarget = INVALID_HOBJECT;
	m_skillId = INVALID_TBLIDX;
	m_dwTime = 0;
	m_dwAffectingTimeRemaining = INVALID_DWORD;
}

CBotState_SkillAffecting::~CBotState_SkillAffecting()
{
}


void CBotState_SkillAffecting::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_SkillAffecting::OnExit()
{
	m_pParent->GetSkillManager()->CancelCasting(); //do this or monster keep freezing forever

	if (m_pSkill)
	{
		//printf("m_dwTime: %u, m_pSkill->GetAffectingTimeRemaining(); %u \n", m_dwTime, m_pSkill->GetAffectingTimeRemaining());

		if (m_pSkill->GetAffectingTimeRemaining() > 0)
		{
			m_pSkill->OnAffectedCanceled();
		}
	}

	//NTL_PRINT(PRINT_APP, "LEAVE SKILL AFFECTING STATE");

	m_hTarget = INVALID_HOBJECT;
	m_skillId = INVALID_TBLIDX;
	m_dwTime = INVALID_DWORD;
	m_pSkill = NULL;

	CCharState::OnExit();
}

int CBotState_SkillAffecting::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	if (m_pParent->GetMoveFlag() == NTL_MOVE_FLAG_DASH)
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(m_hTarget);
		if (!pTarget || pTarget->IsInitialized() == false || pTarget->GetCurWorld() == NULL || pTarget->GetWorldID() != m_pParent->GetWorldID())
		{
		//	printf("cancel 1\n");
			// cancel skill
			m_pParent->SendCharStateStanding();

			return 0;
		}

		float fRange = m_pSkill->GetOriginalTableData()->fUse_Range_Max;

		CNtlVector vDestLoc = pTarget->GetCurLoc();
		vDestLoc.x -= m_pParent->GetCurDir().x * fRange;
		// keep y (height)same
		vDestLoc.z -= m_pParent->GetCurDir().z * fRange;

		// set dest location
		m_pParent->SetDestLoc(vDestLoc);

	//	printf("we in dash !\n");
		m_pParent->UpdateMove(dwTickDiff, fMultiple); // update movement

		
		float fDestRange = NtlGetDistance(m_pParent->GetCurLoc(), vDestLoc);
		if(fDestRange >= fRange + NTL_MAX_DASH_DISTANCE_FOR_SKILL + DBO_DISTANCE_CHECK_TOLERANCE + DBO_DISTANCE_CHECK_TOLERANCE)// check if we are TOO far away from target range.. then cancel
		{
	//		printf("cancel 2. fDestRange: %f \n", fDestRange);
			// cancel skill
			m_pParent->SendCharStateStanding();

			return 0;
		}
		else if (fDestRange <= fRange) // check if reached
		{
	//		printf("at destination\n");
			m_pSkill->SetAffectingTimeRemaining(0); // set to 0 to avoid OnAffectedCanceled

			m_pParent->SetMoveFlag(NTL_MOVE_FLAG_INVALID);
			m_pParent->SetDestLoc(CNtlVector::ZERO);
			m_pParent->SetMoveDirection(NTL_MOVE_NONE);

			m_pSkill->OnAffected(); // here we might go faint

			// continue the m_dwTime decrease here
		}
		else
		{
			return 0;
		}
	}

	m_dwTime = UnsignedSafeDecrease<DWORD>(m_dwTime, dwTickDiff);

	if (m_dwTime == 0) //in OnAffected player could go another state
	{
		if (m_pSkill)
			m_pSkill->OnAffectingEnd();

		if(m_pParent->GetCharStateID() == CHARSTATE_SKILL_AFFECTING)
			m_pParent->SendCharStateStanding();
	}

	return 0;
}

int CBotState_SkillAffecting::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_SkillAffecting::CanTransition(BYTE stateID)
{
	if (m_pParent->IsPC())
	{
		if (stateID == CHARSTATE_SITTING
			|| stateID == CHARSTATE_CAMPING
			|| stateID == CHARSTATE_MOVING
			|| stateID == CHARSTATE_DESTMOVE
			|| stateID == CHARSTATE_FOLLOWING
			|| stateID == CHARSTATE_DASH_PASSIVE
			|| stateID == CHARSTATE_FOCUSING
			|| stateID == CHARSTATE_CASTING
			|| stateID == CHARSTATE_CASTING_ITEM
			|| stateID == CHARSTATE_PRIVATESHOP
			|| stateID == CHARSTATE_RIDEON
			|| stateID == CHARSTATE_AIR_JUMP
			|| stateID == CHARSTATE_AIR_DASH_ACCEL
			)
			return false;
	}

	return true;
}

int CBotState_SkillAffecting::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateSkillAffecting.hTarget = m_hTarget;
	pStateDetail->sCharStateSkillAffecting.skillId = m_skillId;

	return sizeof(sCHARSTATE_SKILL_AFFECTING);
}

void CBotState_SkillAffecting::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	m_hTarget = pStateDetail->sCharStateSkillAffecting.hTarget;
	m_skillId = pStateDetail->sCharStateSkillAffecting.skillId;

	m_pSkill = m_pParent->GetSkillManager()->FindSkill(m_skillId);
	if (m_pSkill)
	{
		sCHAR_DATA_INFO* pCharInfo = m_pParent->GetAniTbldat()->GetAnim(0, m_pSkill->GetOriginalTableData()->wAction_Animation_Index);
		if (pCharInfo == NULL)
		{
			ERR_LOG(LOG_SYSTEM, "Animation: %u could not be found", m_pSkill->GetOriginalTableData()->wAction_Animation_Index);
			m_dwAffectingTimeRemaining = 1000;
			m_dwTime = 1000;
			m_pSkill->SetAffectingTimeRemaining(m_dwAffectingTimeRemaining);
			return;
		}

		float fAniSpeedModifier = m_pParent->GetCharAtt()->GetSkillAnimationSpeedModifier() / 100.f;
		float fAniSpeed = fAniSpeedModifier / ((float)m_pSkill->GetOriginalTableData()->dwAnimation_Time / 1000.f);
		float fAnimationDuration = m_pParent->GetAniTbldat()->GetSkillAnimationLength(m_pSkill->GetOriginalTableData()->wAction_Animation_Index);

		float fFirstHitTick = pCharInfo->aHitTime[0] * 1000.f;
		//DWORD dwLastHitTick = (pCharInfo->byHitCount > 1) ? DWORD(pCharInfo->aHitTime[pCharInfo->byHitCount - 1] * 1000.f) : fFirstHitTick;

		DWORD dwSkillCancelTime = m_pParent->GetAniTbldat()->GetSkillAnimationSkillCancelTime(m_pSkill->GetOriginalTableData()->wAction_Animation_Index);

		m_dwAffectingTimeRemaining = 0;

	//	if (dwSkillCancelTime > 0)
		{
		//	m_dwTime = DWORD((float)dwSkillCancelTime * (100.f / m_pParent->GetCharAtt()->GetSkillAnimationSpeedModifier()));
			m_dwAffectingTimeRemaining = DWORD(fFirstHitTick / fAniSpeed);
		}
		//else
		//	m_dwTime = DWORD((float)dwLastHitTick * (100.f / m_pParent->GetCharAtt()->GetSkillAnimationSpeedModifier()));

		m_dwTime = DWORD(fAnimationDuration / fAniSpeed);

		//printf("m_dwTime %u, m_dwAffectingTimeRemaining %u, fFirstHitTick %f, dwSkillCancelTime %u, fAnimationDuration %f, fAniSpeedModifier %f, fAniSpeed %f \n", 
		//	m_dwTime, m_dwAffectingTimeRemaining, fFirstHitTick, dwSkillCancelTime, fAnimationDuration, fAniSpeedModifier, fAniSpeed);

		if (BIT_FLAG_TEST(m_pSkill->GetOriginalTableData()->dwFunction_Bit_Flag, SKILL_FUNCTION_FLAG_IGNORE_AFFECT_TIME))
		{
			m_dwAffectingTimeRemaining = 0;
		}
		//else if (m_pSkill->GetEffectCode(0) == ACTIVE_WARP_STUN || m_pSkill->GetEffectCode(1) == ACTIVE_WARP_STUN)
		//{
		//	m_dwAffectingTimeRemaining = DWORD((float)dwLastHitTick * (100.f / m_pParent->GetCharAtt()->GetSkillAnimationSpeedModifier()));
		//}
		//else if (Dbo_IsSystemEffectForBodyCurse(m_pSkill->GetEffectCode(0)) || Dbo_IsSystemEffectForBodyCurse(m_pSkill->GetEffectCode(1))) //if it is a skill like kidney shot which stun target, then it is applied right after usage.
		//{
		//	m_dwAffectingTimeRemaining = 0;
		//}
			
		if (m_dwAffectingTimeRemaining > m_dwTime)
			m_dwTime = m_dwAffectingTimeRemaining;

		//NTL_PRINT(PRINT_APP,"m_dwTime %u, m_dwAffectingTimeRemaining %u byHitCount %u, aHitTime %f, dwSkillCancelTime %u, dwFirstHitTick %u, dwLastHitTick %u, byKnockDownCount %u, fDurationTime %f", 
		//m_dwTime, m_dwAffectingTimeRemaining, pCharInfo->byHitCount, pCharInfo->aHitTime[pCharInfo->byHitCount - 1], dwSkillCancelTime, dwFirstHitTick, dwLastHitTick, pCharInfo->byKnockDownCount, pCharInfo->fDurationTime);

		if (m_pSkill->GetOriginalTableData()->bDash_Able) // if dash skill, we apply effect when hit target
		{
			m_dwAffectingTimeRemaining = INVALID_DWORD;
		}

		m_pSkill->SetAffectingTimeRemaining(m_dwAffectingTimeRemaining);

		if (m_dwAffectingTimeRemaining == 0)
		{
			m_pSkill->OnAffected();
		}
	}
}

