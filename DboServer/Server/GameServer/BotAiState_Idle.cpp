#include "stdafx.h"
#include "BotAiState_Idle.h"
#include "ApplyAiFunc.h"
#include "BotAiAction_Wander.h"
#include "BotAiAction_FixLoc.h"
#include "BotAiAction_Patrol.h"
#include "BotAiCondition_AttackTarget.h"
#include "BotAiCondition_PetFollow.h"
#include "BotAiCondition_PetFollow_Teleport.h"
#include "BotAiCondition_Follow_Teleport.h"
#include "BotAiAction_PlayScene.h"
#include "BotAiAction_Follow.h"
#include "BotAiCondition_Speech.h"
#include "BotAiCondition_SkillUse.h"


CBotAiState_Idle::CBotAiState_Idle(CNpc* pBot)
:CBotAiState(pBot, BOTCONTROL_IDLE, "BOTCONTROL_IDLE")
{
}

CBotAiState_Idle::~CBotAiState_Idle()
{
}


void CBotAiState_Idle::OnEnter()
{
	CBotAiCondition_AttackTarget* pAttackTarget = new CBotAiCondition_AttackTarget(GetBot());
	if (!AddSubControlList(pAttackTarget, false))
	{
		m_status = CControlStateComposite::SYSTEMERROR;
		return;
	}

	if (ApplyAiFunc_OffenceType(GetBot(), this))
	{
		if (GetBot()->GetSpeechGroup())
		{
			if (GetBot()->GetSpeechGroup()->HasSpeechData(NPC_SPEECH_TYPE_IDLE))
			{
				CBotAiCondition_Speech* pSpeech = new CBotAiCondition_Speech(GetBot(), NPC_SPEECH_TYPE_IDLE, 5000, false, false);
				if (!AddSubControlList(pSpeech, false))
				{
					m_status = FAILED;
					return;
				}
			}
		}

		CNpc* pBot = GetBot();
		//printf("pBot->GetMoveType() %u \n", pBot->GetMoveType());
		switch (pBot->GetMoveType())
		{
			case SPAWN_MOVE_FIX:
			{
				CBotAiAction_FixLoc* pFixLoc = new CBotAiAction_FixLoc(pBot);
				if (!AddSubControlQueue(pFixLoc, true))
				{
					m_status = CControlStateComposite::SYSTEMERROR;
					return;
				}
			}
			break;
			case SPAWN_MOVE_WANDER:
			{
				CBotAiAction_Wander* pWander = new CBotAiAction_Wander(pBot);
				if (!AddSubControlQueue(pWander, true))
				{
					m_status = CControlStateComposite::SYSTEMERROR;
					return;
				}
			}
			break;
			case SPAWN_MOVE_PATROL:
			{
				if (pBot->GetPathTblidx() != INVALID_TBLIDX)
				{
					CBotAiAction_Patrol* pPatrol = new CBotAiAction_Patrol(pBot, pBot->GetPathTblidx(), false, 0.0f);
					if (!AddSubControlQueue(pPatrol, true))
					{
						m_status = CControlStateComposite::SYSTEMERROR;
						return;
					}
				}
			}
			break;
			case SPAWN_MOVE_FOLLOW:
			{
				CNpcParty* pParty = pBot->GetNpcParty();
				if (pParty)
				{
					CSkillManagerBot* pSkillManager = (CSkillManagerBot *)GetBot()->GetSkillManager();
					if (pSkillManager->IsSkill() == true)
					{
						CBotAiCondition_SkillUse* pSkillUse = new CBotAiCondition_SkillUse(GetBot());
						if (!AddSubControlList(pSkillUse, false))
						{
							m_status = CControlStateComposite::SYSTEMERROR;
							return;
						}
					}

					CBotAiAction_Follow* pFollow = new CBotAiAction_Follow(pBot, pParty);
					if (!AddSubControlQueue(pFollow, true))
					{
						m_status = CControlStateComposite::SYSTEMERROR;
						return;
					}
				}

			}
			break;
			case SPAWN_MOVE_PATH_ONCE:
			{
				CBotAiAction_FixLoc* pFixLoc = new CBotAiAction_FixLoc(pBot);
				if (!AddSubControlQueue(pFixLoc, true))
				{
					m_status = CControlStateComposite::SYSTEMERROR;
					return;
				}
				if (pBot->GetPathTblidx() != INVALID_TBLIDX)
				{
					CBotAiAction_Patrol* pPatrol = new CBotAiAction_Patrol(pBot, pBot->GetPathTblidx(), false, 0.0f);
					if (!AddSubControlQueue(pPatrol, true))
					{
						m_status = CControlStateComposite::SYSTEMERROR;
						return;
					}
				}
			}break;
			default: break;
		}

		if (pBot->IsObjType(OBJTYPE_SUMMON_PET))
		{
			CBotAiCondition_PetFollow* pPetFollow = new CBotAiCondition_PetFollow(pBot);
			if (!AddSubControlList(pPetFollow, false))
			{
				m_status = FAILED;
				return;
			}
			CBotAiCondition_PetFollow_Teleport* pPetTeleport = new CBotAiCondition_PetFollow_Teleport(pBot);
			if (!AddSubControlList(pPetTeleport, false))
			{
				m_status = FAILED;
				return;
			}
		}
		
		CNpcParty* pParty = pBot->GetNpcParty();
		if (!pParty || (AddSubControlList(new CBotAiCondition_Follow_Teleport(pBot, pParty), false)))
		{
			if (
				pBot->GetPlayScript() == INVALID_TBLIDX
				|| pBot->GetPlayScriptScene() == INVALID_TBLIDX
				|| AddSubControlQueue(new CBotAiAction_PlayScene(pBot, pBot->GetPlayScript(), pBot->GetPlayScriptScene()), true)
				)
			{
				if (!AttachAIS_State(pBot, this, AIS_NODE_STATE_IDLE))
					m_status = FAILED;
			}
			else
			{
				m_status = CControlStateComposite::SYSTEMERROR;
			}
		}
		else
		{
			m_status = CControlStateComposite::SYSTEMERROR;
		}
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "false == ApplyAiFunc_OffenceType( GetBot(), this ) \n");
		m_status = CControlStateComposite::SYSTEMERROR;
	}
}

void CBotAiState_Idle::OnExit()
{
}

int CBotAiState_Idle::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	int status = UpdateSubControlList(dwTickDiff, fMultiple);
	if (status != CHANGED && status != CLEARED)
	{
		UpdateSubControlQueue(dwTickDiff, fMultiple);
		status = m_status;
	}

	return status;
}
