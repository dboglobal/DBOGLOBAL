#include "stdafx.h"
#include "AspectState_SpinningAttack.h"
#include "CPlayer.h"


CAspectState_SpinningAttack::CAspectState_SpinningAttack(CCharacter *pParent)
	:CAspectState(ASPECTSTATE_SPINNING_ATTACK, pParent)
{
}

CAspectState_SpinningAttack::~CAspectState_SpinningAttack()
{
}


void CAspectState_SpinningAttack::OnExit()
{
	if (m_pParent->IsInitialized())
	{
		if (m_pParent->IsSleeping() == false)
			m_pParent->GetStateManager()->AddConditionState(CHARCOND_AFTEREFFECT, NULL, true);

		m_pParent->ChangeAttackProgress(false);
		m_pParent->SetFightMode(false);

		if (m_pParent->IsPC())
			((CPlayer*)m_pParent)->SetAttackTarget(INVALID_HOBJECT);

		m_pParent->SetSpecialSkillTblidx(INVALID_TBLIDX);
	}

	CAspectState::OnExit();
}

int CAspectState_SpinningAttack::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwLastTransformEffect += dwTickDiff;
	
	float fDif = (m_pParent->GetRunSpeed() / m_pParent->GetWalkingSpeed() * 100.f) - 100.f;
	DWORD dwDelay = NTL_BATTLE_ATTACK_DELAY_TIME - (NTL_BATTLE_ATTACK_DELAY_TIME * (fDif / 100.f));
	if (dwDelay < 300)
		dwDelay = 300;
	else if (dwDelay > 5000)
		dwDelay = 5000;

//	printf("m_pParent->GetRunSpeed() %f, m_pParent->GetWalkingSpeed() %f, dwDelay: %u, fDif %f \n", m_pParent->GetLastRunSpeed(), m_pParent->GetWalkingSpeed(), dwDelay, fDif);
	
	if (m_dwLastTransformEffect >= dwDelay)
	{
		m_dwLastTransformEffect = 0;

		if (m_pParent->IsPC() == false)
			return 0;

		if (m_pParent->GetCurWorldCell())
		{
			((CPlayer*)m_pParent)->UpdateBattleCombatMode(true);

			int nHits = 0;

			int nLoopCount = 0;
			int nLoopCount2 = 0;

			CSkill* pSkill = m_pParent->GetSkillManager()->FindSkill(m_pParent->GetSpecialSkillTblidx());
			if (pSkill)
			{
				CWorldCell::QUADPAGE page = m_pParent->GetCurWorldCell()->GetCellQuadPage(m_pParent->GetCurLoc());
				for (int dir = CWorldCell::QUADDIR_SELF; dir <= CWorldCell::QUADDIR_VERTICAL; dir++)
				{
					CWorldCell* pWorldCellSibling = m_pParent->GetCurWorldCell()->GetQuadSibling(page, (CWorldCell::QUADDIR)dir);
					if (pWorldCellSibling)
					{
						CCharacter* pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetFirst(OBJTYPE_PC);
						while (pTarget && pTarget->IsInitialized())
						{
							++nLoopCount;
							if (nLoopCount > 5000)
							{
								ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
							}

							if (m_pParent->IsInRange(pTarget, m_pParent->GetObjectRadius() + 3.0f))
							{
								if (m_pParent->IsAttackable(pTarget))
								{
									m_pParent->SpecialAttackAction(pTarget, DBO_SPECIAL_ATTACK_TYPE_SPINNING_ATTACK, 1, m_pParent->GetSpecialSkillTblidx(), pSkill);

									if (++nHits >= NTL_MAX_NUMBER_OF_SKILL_TARGET)
									{
										return 0;
									}
								}
							}

							pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetNext(pTarget->GetWorldCellObjectLinker());
						}

						CCharacter* pMob = (CCharacter*)pWorldCellSibling->GetObjectList()->GetFirst(OBJTYPE_MOB);
						while (pMob && pMob->IsInitialized())
						{
							++nLoopCount2;
							if (nLoopCount2 > 5000)
							{
								ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
							}

							if (m_pParent->IsInRange(pMob, m_pParent->GetObjectRadius() + 3.0f))
							{
								if (m_pParent->IsAttackable(pMob))
								{
									m_pParent->SpecialAttackAction(pMob, DBO_SPECIAL_ATTACK_TYPE_SPINNING_ATTACK, 1, m_pParent->GetSpecialSkillTblidx(), pSkill);

									if (++nHits >= NTL_MAX_NUMBER_OF_SKILL_TARGET)
									{
										return 0;
									}
								}
							}

							pMob = (CCharacter*)pWorldCellSibling->GetObjectList()->GetNext(pMob->GetWorldCellObjectLinker());
						}
					}
				}
			}
		}
	}

	return TRUE;
}