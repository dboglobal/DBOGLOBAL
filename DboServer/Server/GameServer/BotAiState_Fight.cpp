#include "stdafx.h"
#include "BotAiState_Fight.h"
#include "ObjectManager.h"
#include "char.h"

#include "BotAiCondition_Timid.h"
#include "BotAiCondition_OutOfNest.h"
#include "BotAiCondition_PetFollow_Teleport.h"
#include "BotAiCondition_Follow_Teleport.h"
#include "ApplyAiFunc.h"
#include "BotAiCondition_Speech.h"
#include "BotAiAction_Look.h"
#include "BotAiCondition_SkillUse.h"
#include "BotAiCondition_RangedAttack.h"
#include "BotAiCondition_Terror.h"
#include "PlayScriptManager.h"
#include "BotAiCondition_BSCondition.h"
#include "SPSNodeCondition_RegisterBattleScene.h"
#include "BotAiAction_Attack.h"
#include "BotAiAction_Chase.h"
#include "BotAiCondition_Confusion.h"


CBotAiState_Fight::CBotAiState_Fight(CNpc* pBot, HOBJECT hTarget)
	:CBotAiState(pBot, BOTCONTROL_FIGHT, "BOTCONTROL_FIGHT"),
	m_cFirstProcList(pBot)
{
	m_hTarget = hTarget;
	m_hCompulsionTarget = INVALID_HOBJECT;
	m_dwTime = 0;
	m_dwDelayTime = 0;
	m_bIsOnlySkill = false;
}

CBotAiState_Fight::~CBotAiState_Fight()
{
}


void CBotAiState_Fight::OnEnter()
{
//	printf("CBotAiState_Fight::OnEnter() \n");

	if (m_hTarget != INVALID_HOBJECT)
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(m_hTarget);
		if (pTarget && pTarget->IsInitialized())
		{
			if (GetBot()->HasAIFunction(AI_FUNC_FLAG_TIMID))
			{
				CBotAiCondition_Timid* pCondTimid = new CBotAiCondition_Timid(GetBot());
				if (!AddSubControlList(pCondTimid, false))
				{
					m_status = FAILED;
					return;
				}
			}

			if (GetBot()->GetOnlyOneSkillUse() != INVALID_TBLIDX || GetBot()->HasAIFunction(AI_FUNC_FLAG_ONLYSKILL))
			{
				m_bIsOnlySkill = true;
			}

			if (GetBot()->IsNPC() || GetBot()->IsMonster())
			{
				CBotAiCondition_OutOfNest* pCondOutOfNest = new CBotAiCondition_OutOfNest(GetBot());
				if (!AddSubControlList(pCondOutOfNest, false))
				{
					m_status = FAILED;
					return;
				}
			}

			if (!GetBot()->IsNPC() && !GetBot()->IsMonster())
			{
				if (GetBot()->IsSummonPet())
				{
					CBotAiCondition_PetFollow_Teleport* pCondPetFollowTeleport = new CBotAiCondition_PetFollow_Teleport(GetBot());
					if (!AddSubControlList(pCondPetFollowTeleport, false))
					{
						m_status = FAILED;
						return;
					}
				}
			}
			else
			{
				CNpcParty* pParty = GetBot()->GetNpcParty();
				if (pParty)
				{
					CBotAiCondition_Follow_Teleport* pCondFollowTeleport = new CBotAiCondition_Follow_Teleport(GetBot(), pParty);
					if (!AddSubControlList(pCondFollowTeleport, false))
					{
						m_status = FAILED;
						return;
					}
				}
			}

			if (ApplyAiFunc_FleeType(GetBot(), this))
			{
				sNPC_SPEECH_GROUP* sSpeechGroup = GetBot()->GetSpeechGroup();
				if (!sSpeechGroup || sSpeechGroup->HasSpeechData(NPC_SPEECH_TYPE_FIGHTING) == false || AddSubControlList(new CBotAiCondition_Speech(GetBot(), NPC_SPEECH_TYPE_FIGHTING, 10000, false, true), false))
				{
					if (!GetBot()->IsNPC() && !GetBot()->IsMonster()
						|| GetBot()->HasAIFunction(AI_FUNC_FLAG_OFFENSIVE) != true
						&& GetBot()->HasAIFunction(AI_FUNC_FLAG_SMART_OFFENSIVE) != true
						|| AddSubControlQueue(new CBotAiAction_Look(GetBot(), m_hTarget, 1000), true)
						)
					{
						CSkillManagerBot* pSkillManager = (CSkillManagerBot *)GetBot()->GetSkillManager();
						if (pSkillManager->IsSkill() == false || AddSubControlList(new CBotAiCondition_SkillUse(GetBot()), false))
						{
							if (ApplyAiFunc_HelpType(GetBot(), this))
							{
								if (!GetBot()->IsNPC() && !GetBot()->IsMonster()
									|| GetBot()->GetTbldat() == NULL
									|| GetBot()->CCharacterObject::GetAttackRange() <= 2.0f
									|| AddSubControlList(new CBotAiCondition_RangedAttack(GetBot()), false)
									)
								{
									CBotAiCondition_Terror* pTerror = new CBotAiCondition_Terror(GetBot());
									if (AddSubControlList(pTerror, false))
									{
										CBotAiCondition_Confusion* pConfusion = new CBotAiCondition_Confusion(GetBot());
										if (AddSubControlList(pConfusion, false))
										{
											GetBot()->ChangeTarget(m_hTarget);

											if (!m_bIsOnlySkill)
											{
												CNtlVector vDestDir(pTarget->GetCurLoc().operator-(GetBot()->GetCurLoc()));
												vDestDir.y = 0.0f;
												if (vDestDir.SafeNormalize())
												{
													GetBot()->SetCurDir(vDestDir);
												}

												GetBot()->SendCharStateStanding();

												if (!GetBot()->HasAIFunction(AI_FUNC_FLAG_TIMID))
												{
													GetBot()->ChangeFightMode(true);
												}
											}

											if (GetBot()->GetBattleScript() != INVALID_TBLIDX && GetBot()->GetBattleScriptScene() != INVALID_DWORD)
											{
												for (CSPSNodeCondition_RegisterBattleScene* j = g_pPlayScriptManager->GetFirstRegBattleScene(GetBot()->GetBattleScript(), GetBot()->GetBattleScriptScene());
													j;
													j = g_pPlayScriptManager->GetNextRegBattleScene(j))
												{
													CBotAiCondition_BSCondition* pCondBS = new CBotAiCondition_BSCondition(GetBot());
													pCondBS->SetScript(GetBot()->GetBattleScript(), j->GetNodeIndex(), (eSPS_BS_PLAY_TYPE)j->GetLoopCount()); //not sure about loopcount

													if (!AddSubControlList(pCondBS, false))
													{
														SAFE_DELETE(pCondBS);
														m_status = FAILED;
														return;
													}
												}
											}

											if (AttachAIS_State(GetBot(), this, AIS_NODE_STATE_FIGHT))
											{
												GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_DOING_BATTLE, true);
											}
											else
											{
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
				m_status = FAILED;
			}
		}
	}
}

void CBotAiState_Fight::OnContinue()
{
	GetBot()->ChangeTarget(m_hTarget);

	if (!m_bIsOnlySkill)
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(m_hTarget);
		if (pTarget && pTarget->IsInitialized())
		{
			CNtlVector vDestDir(pTarget->GetCurLoc().operator-(GetBot()->GetCurLoc()));
			vDestDir.y = 0.0f;
			if (vDestDir.SafeNormalize())
			{
				GetBot()->SetCurDir(vDestDir);
			}
		}

		GetBot()->SendCharStateStanding();

		if (!GetBot()->HasAIFunction(AI_FUNC_FLAG_TIMID))
		{
			GetBot()->ChangeFightMode(true);
		}
	}
}

void CBotAiState_Fight::OnPause()
{
	if (!m_bIsOnlySkill)
	{
		if (!GetBot()->HasAIFunction(AI_FUNC_FLAG_TIMID))
		{
			GetBot()->ChangeFightMode(false);
		}
	}
}

void CBotAiState_Fight::OnExit()
{
	if (!m_bIsOnlySkill)
	{
		if (!GetBot()->HasAIFunction(AI_FUNC_FLAG_TIMID))
		{
			GetBot()->ChangeFightMode(false);
		}
	}

	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_DOING_BATTLE, false);
}

int CBotAiState_Fight::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetBot()->GetSkillManager()->IsSkillUseLock())
	{
		m_dwDelayTime += dwTickDiff;
		return m_status;
	}

	int nRes = m_cFirstProcList.OnUpdate(dwTickDiff + m_dwDelayTime, fMultiple);
	
	m_dwDelayTime = 0;

	if (nRes == COMPLETED)
	{
		HOBJECT handle = INVALID_HOBJECT;

		if (m_hCompulsionTarget == INVALID_HOBJECT)
		{
			handle = GetBot()->ConsiderAttackTarget(dwTickDiff);

			if (handle == INVALID_HOBJECT)
			{
				GetBot()->ChangeTarget(INVALID_HOBJECT);
				m_status = COMPLETED;
				return m_status;
			}

			if (m_hTarget != handle || GetBot()->GetTargetHandle() == INVALID_HOBJECT)
			{
				GetBot()->ChangeTarget(handle);
				m_hTarget = handle;
			}
		}
		else
		{
			handle = m_hCompulsionTarget;
		}

		int nSubState = UpdateSubControlList(dwTickDiff, fMultiple);
		if (nSubState == CHANGED || nSubState == CLEARED)
			return nSubState;

		nSubState = UpdateSubControlQueue(dwTickDiff, fMultiple);
		if (nSubState == CHANGED || nSubState == CLEARED)
			return nSubState;

		if (nSubState != COMPLETED && nSubState != FAILED)
			return nSubState;
		
		if (handle == INVALID_HOBJECT)
		{
			if (!GetBot()->GetStateManager()->IsCharCondition(CHARSTATE_TELEPORTING))
			{
				m_status = COMPLETED;
				return m_status;
			}
		}
		else if (nSubState == COMPLETED)
		{
			if (GetBot()->HasAIFunction(AI_FUNC_FLAG_TIMID))
				return m_status;

			CCharacter* pTarget = g_pObjectManager->GetChar(handle);
			if (pTarget && pTarget->IsInitialized())
			{
				int nBotControll = INVALID_BOTCONTROLID;

				if (GetBot()->IsChasingCheatingTarget())
				{
					if (GetBot()->IsInRange(GetBot()->GetCheatingTargetCollisionLoc(), GetBot()->GetAttackRange(pTarget) * 130.0f / 100.0f))
						nBotControll = BOTCONTROL_ACTION_ATTACK;
					else
						nBotControll = BOTCONTROL_ACTION_CHASE;
				}
				else
				{
					if (GetBot()->ConsiderAttackRange(0.0f))
					{
						if (GetBot()->CanSeeOnLineOfSight(pTarget))
						{
							nBotControll = BOTCONTROL_ACTION_ATTACK;
						}
						else
						{
							std::list<CNtlVector> rlistCollisionPos;
							if (GetBot()->IsReachable(pTarget, rlistCollisionPos))
								nBotControll = BOTCONTROL_ACTION_CHASE;
							else
								nBotControll = BOTCONTROL_ACTION_ATTACK;
						}
					}
					else
					{
						nBotControll = BOTCONTROL_ACTION_CHASE;
					}
				}

				if (nBotControll == BOTCONTROL_ACTION_ATTACK)
				{
					if (!m_bIsOnlySkill)
					{
						CBotAiAction_Attack* pAttack = new CBotAiAction_Attack(GetBot(), handle);
						if (!AddSubControlQueue(pAttack, true))
						{
							m_status = FAILED;
							return m_status;
						}
					}
				}
				else if (nBotControll == BOTCONTROL_ACTION_CHASE)
				{
					if (GetBot()->IsMoveAble()) // NOT SURE vfptr[7].GetName
					{
						CBotAiAction_Chase* pChase = new CBotAiAction_Chase(GetBot(), CBotAiAction_Chase::ATTACKTYPE_ATTACK, 0.0f);
						if (!AddSubControlQueue(pChase, true))
						{
							m_status = FAILED;
							return m_status;
						}
					}
				}
			}
		}
	}
	else if (nRes == CHANGED || nRes > SYSTEMERROR && nRes <= CLEARED)
	{
		return nRes;
	}

	return m_status;
}
