#include "stdafx.h"
#include "BotAiAction_ChangeAiBitFlag.h"
#include "SPSNodeAction_ChangeAiBitFlag.h"


CBotAiAction_ChangeAiBitFlag::CBotAiAction_ChangeAiBitFlag(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_CHANGEAIBITFLAG, "BOTCONTROL_ACTION_CHANGEAIBITFLAG")
{
	m_scriptnodeID = INVALID_DWORD;
	m_dwAi_Bit_Mask = 0;
	m_dwAi_Bit_Flag = 0;
	m_eScriptType = CSPSNodeAction_ChangeAiBitFlag::INVALID_SCRIPTYPE;
}


CBotAiAction_ChangeAiBitFlag::~CBotAiAction_ChangeAiBitFlag()
{
}


void CBotAiAction_ChangeAiBitFlag::OnEnter()
{
}

void CBotAiAction_ChangeAiBitFlag::OnExit()
{
}

int CBotAiAction_ChangeAiBitFlag::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	switch (m_eScriptType)
	{
		case CSPSNodeAction_ChangeAiBitFlag::eAIS:
		{
			GetBot()->GetBotController()->ChangeControlState_Idle();

			SetBitFlag(AI_FUNC_FLAG_DEFENSIVE);
			SetBitFlag(AI_FUNC_FLAG_TIMID);
			SetBitFlag(AI_FUNC_FLAG_OFFENSIVE);
			SetBitFlag(AI_FUNC_FLAG_SMART_OFFENSIVE);
			SetBitFlag(AI_FUNC_FLAG_ONLYSKILL);
			SetBitFlag(AI_FUNC_FLAG_DESPERATION);
			SetBitFlag(AI_FUNC_FLAG_FLEE);
			SetBitFlag(AI_FUNC_FLAG_RETREAT);
			SetBitFlag(AI_FUNC_FLAG_REVENGE);
			SetBitFlag(AI_FUNC_FLAG_FOCUS);
			SetBitFlag(AI_FUNC_FLAG_MEAN);
			SetBitFlag(AI_FUNC_FLAG_BRAVE);
			SetBitFlag(AI_FUNC_FLAG_ALLIANCE_HELP);
			SetBitFlag(AI_FUNC_FLAG_DEFEND);
			SetBitFlag(AI_FUNC_FLAG_NOT_CHASE);
			SetBitFlag(AI_FUNC_FLAG_NOT_MOVE);

			m_status = COMPLETED;
		}
		break;

		case CSPSNodeAction_ChangeAiBitFlag::eSPS:
		{
			CBotAiState* pAiState = GetBot()->GetBotController()->GetCurrentState();
			if (pAiState)
			{
				if (AttackType(pAiState) == FAILED)
					return m_status;

				else if (FleeType(pAiState) == FAILED)
					return m_status;

				else if (HelpType(pAiState) == FAILED)
					return m_status;

				else if (OnlySkill(pAiState) == FAILED)
					return m_status;

				else if (TargettingType(pAiState) == FAILED)
					return m_status;

				else if (MoveType(pAiState) == FAILED)
					return m_status;

				else
				{
					m_status = COMPLETED;
				}
			}
			else
			{
				return ACTIVATED;
			}
		}
		break;

		default:
		{
			ERR_LOG(LOG_BOTAI, "fail : m_eScriptType(%d)", m_eScriptType);
			m_status = FAILED;
		}
		break;
	}

	return m_status;
}

bool CBotAiAction_ChangeAiBitFlag::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_ChangeAiBitFlag* pAction = dynamic_cast<CSPSNodeAction_ChangeAiBitFlag*>(pControlScriptNode);
	if (pAction)
	{
		m_scriptnodeID = pControlScriptNode->GetNodeIndex();
		m_dwAi_Bit_Mask = pAction->m_dwAi_Bit_Mask;
		m_dwAi_Bit_Flag = pAction->m_dwAi_Bit_Flag;
		m_eScriptType = pAction->m_eScriptType;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_ChangeAiBitFlag", pControlScriptNode);
	return false;
}

void CBotAiAction_ChangeAiBitFlag::CopyTo(CControlState* pTo)
{
	CBotAiAction_ChangeAiBitFlag* pAction = dynamic_cast<CBotAiAction_ChangeAiBitFlag*>(pTo);
	if (pAction)
	{
		pAction->m_scriptnodeID = m_scriptnodeID;
		pAction->m_dwAi_Bit_Mask = m_dwAi_Bit_Mask;
		pAction->m_dwAi_Bit_Flag = m_dwAi_Bit_Flag;
		pAction->m_eScriptType = m_eScriptType;
	}
}


void CBotAiAction_ChangeAiBitFlag::SetBitFlag(DWORD dwFlag)
{
	if (BIT_FLAG_TEST(dwFlag, m_dwAi_Bit_Mask))
	{
		if (BIT_FLAG_TEST(dwFlag, m_dwAi_Bit_Flag))
		{
			BIT_FLAG_SET(*GetBot()->GetAiBitFlag(), dwFlag);
		}
		else
		{
			BIT_FLAG_UNSET(*GetBot()->GetAiBitFlag(), dwFlag);
		}
	}
}


int CBotAiAction_ChangeAiBitFlag::AttackType(CBotAiState * pAiState)
{
	return NULL;
}

int CBotAiAction_ChangeAiBitFlag::FleeType(CBotAiState * pAiState)
{
	return NULL;
}

int CBotAiAction_ChangeAiBitFlag::TargettingType(CBotAiState * pAiState)
{
	return NULL;
}

int CBotAiAction_ChangeAiBitFlag::HelpType(CBotAiState * pAiState)
{
	return NULL;
}

int CBotAiAction_ChangeAiBitFlag::MoveType(CBotAiState * pAiState)
{
	bool bHasChaseMask = BIT_FLAG_TEST(m_dwAi_Bit_Mask, 0x100000);
	bool bHasMoveMask = BIT_FLAG_TEST(m_dwAi_Bit_Mask, 0x200000);

	if (bHasChaseMask || bHasMoveMask)
	{
		SetBitFlag(0x100000);
		SetBitFlag(0x200000);
	}

	return m_status;
}

int CBotAiAction_ChangeAiBitFlag::OnlySkill(CBotAiState * pAiState)
{
	return NULL;
}