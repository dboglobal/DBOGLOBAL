#include "stdafx.h"
#include "BotAiAction_Attack.h"
#include "char.h"
#include "ObjectManager.h"
#include "GameServer.h"


CBotAiAction_Attack::CBotAiAction_Attack(CNpc* pBot, HOBJECT hTarget)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_ATTACK, "BOTCONTROL_ACTION_ATTACK")
{
	m_hTarget = hTarget;
	m_dwAttackTime = 0;
	m_dwMoveTime = 0;
	m_bAttackCanceled = false;
}


CBotAiAction_Attack::~CBotAiAction_Attack()
{
}



void CBotAiAction_Attack::OnEnter()
{
	m_dwAttackTime = GetBot()->GetTbldat()->wAttack_Speed_Rate;

	GetBot()->ChangeAttackProgress(true);
	GetBot()->SetAttacked(false, GetTickCount());
}

void CBotAiAction_Attack::OnContinue()
{
	m_status = COMPLETED;
}

void CBotAiAction_Attack::OnPause()
{
	if (!m_bAttackCanceled)
	{
		GetBot()->ChangeAttackProgress(false);
		m_bAttackCanceled = true;
	}

	m_status = COMPLETED;
}

void CBotAiAction_Attack::OnExit()
{
	if (!m_bAttackCanceled)
	{
		GetBot()->ChangeAttackProgress(false);
	}
}


int CBotAiAction_Attack::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_hTarget == GetBot()->GetTargetHandle())
	{
		if (!GetBot()->IsAttacked(m_dwAttackTime))
			return m_status;

		if (GetBot()->IsChasingCheatingTarget())
		{
			if (!GetBot()->IsInRange(GetBot()->GetCheatingTargetCollisionLoc(), GetBot()->CCharacterObject::GetAttackRange() * 130.0f / 100.0f))
			{
				m_status = COMPLETED;
				return m_status;
			}
		}
		else
		{
			if (!GetBot()->ConsiderAttackRange(0.0f))
			{
				m_status = COMPLETED;
				return m_status;
			}

			CCharacter* pTarget = g_pObjectManager->GetChar(GetBot()->GetTargetHandle());
			if (pTarget)
			{
				if (!GetBot()->CanSeeOnLineOfSight(pTarget))
				{
					m_status = COMPLETED;
					return m_status;
				}
			}
		}
	}
	else
	{
		if (GetBot()->GetTargetListManager()->GetAggroPoint(m_hTarget))
		{
			if (m_bAttackCanceled)
			{
				if (GetBot()->IsAttacked(m_dwAttackTime))
					m_status = COMPLETED;
			}
			else
			{
				GetBot()->ChangeAttackProgress(false);
				m_bAttackCanceled = true;
			}
		}
		else
		{
			m_status = COMPLETED;
		}
	}

	return m_status;
}


int CBotAiAction_Attack::BreakUp(DWORD dwTickTime, float fMultiple) //currently not used
{
	if (GetBot()->IsMoveAble())
	{
		m_dwMoveTime = UnsignedSafeIncrease<DWORD>(m_dwMoveTime, dwTickTime);
		if (m_dwMoveTime >= 1000)
		{
			m_dwMoveTime = 0;

			CCharacter* pTarget = g_pObjectManager->GetChar(GetBot()->GetTargetHandle());
			if (pTarget && pTarget->IsInitialized())
			{
				if (GetBot()->IsInRange(pTarget, GetBot()->CCharacterObject::GetAttackRange()))
				{
					return ObjTypeBreakUp(pTarget);
				}
				else
				{
					//TO DO

					//CBotAiAction_Attack::BreakUp::__l14::sFOREACH _Func

					/*_Func.pMe = CBotAiBase::GetBot((CBotAiBase *)&v17->vfptr);
					_Func.pTarget = (CBot *)pTarget;
					_Func.pNpcObjectManager = CNtlSingleton<CNpcObjectManager>::GetInstance();
					_Func.fWidth = FLOAT_0_5;
					_Func.pControlAction = v17;
					_Func.pStatus = &v17->m_status;
					v10 = CBot::GetMeHaterManager((CBot *)pTarget);
					v11 = CMeHaterManager::GetContainerMeHater(v10);
					j____ForEachCondition_UsFOREACH__O___BreakUp_CBotAiAction_Attack__QEAAHKM_Z__CContainerMeHater__QEAA_NAEAUsFOREACH__O___BreakUp_CBotAiAction_Attack__QEAAHKM_Z__Z(
						v11,
						&_Func);*/
				}
			}
			else
			{
				m_status = COMPLETED;
			}
		}
	}

	return m_status;
}

int CBotAiAction_Attack::ObjTypeBreakUp(CCharacter* pTarget) //currently not used
{
	if (GetBot()->CCharacterObject::GetAttackRange() > 2.0f)
	{
		pTarget->GetBotBreakUp()->AddNavMove(GetBot(), GetBot()->GetAttackRange(pTarget), 1, CBotBreakUp::BREAKUP_MOVETYPE_LEVEL3);
	}
	else
	{
		pTarget->GetBotBreakUp()->AddNavMove(GetBot(), GetBot()->GetAttackRange(pTarget), INVALID_BYTE, CBotBreakUp::BREAKUP_MOVETYPE_LEVEL4);
	}

	return m_status;
}
