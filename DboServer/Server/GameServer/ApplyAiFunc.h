#pragma once

#include "Npc.h"
#include "AIScriptManager.h"
#include "BotAiCondition_ScanTarget.h"
#include "AIScriptControlFactoy.h"
#include "BotAiCondition_AllianceHelpMe.h"
#include "BotAiCondition_Defend.h"
#include "BotAiCondition_Flee.h"
#include "BotAiCondition_Retreat.h"




inline bool ApplyAiFunc_OffenceType(CNpc* pBot, CBotAiState* pAiState)
{
	if (pBot)
	{
		if (pBot->IsSummonPet())
			return true;

		if (pAiState)
		{
			if (pAiState->GetControlStateID() == BOTCONTROL_IDLE)
			{
				if (pBot->HasAIFunction(AI_FUNC_FLAG_OFFENSIVE))
				{
					CBotAiCondition_ScanTarget* pScanTarget = new CBotAiCondition_ScanTarget(pBot, CBotAiCondition_ScanTarget::BOTAI_SMARTSCAN_NORMAL, 0);
					if (!pAiState->AddSubControlList(pScanTarget, false))
					{
						return false;
					}
				}
				else if (pBot->HasAIFunction(AI_FUNC_FLAG_SMART_OFFENSIVE))
				{
					CBotAiCondition_ScanTarget* pScanTarget = new CBotAiCondition_ScanTarget(pBot, CBotAiCondition_ScanTarget::BOTAI_SMARTSCAN_UNDER, 1);
					if (!pAiState->AddSubControlList(pScanTarget, false))
					{
						return false;
					}
				}
			}
			return true;
		}
	}

	return false;
}

inline bool ApplyAiFunc_HelpType(CNpc* pBot, CBotAiState* pAiState)
{
	if (pBot)
	{
		if (pAiState)
		{
			if (pBot->HasAIFunction(AI_FUNC_FLAG_ALLIANCE_HELP) && pBot->GetAllianceID() != INVALID_ALLIANCEID) //not sure if need to do alianceid check.. but some mobs have invalid alianceid
			{
				if (!pAiState->AddSubControlList(new CBotAiCondition_AllianceHelpMe(pBot), false))
					return false;
			}

			if (pBot->HasAIFunction(AI_FUNC_FLAG_DEFEND) && pBot->GetAllianceID() != INVALID_ALLIANCEID) //not sure if need to do alianceid check.. but some mobs have invalid alianceid
			{
				if (!pAiState->AddSubControlList(new CBotAiCondition_Defend(pBot, 5000), false))
					return false;
			}

			return true;
		}
	}

	return false;
}

inline bool ApplyAiFunc_FleeType(CNpc* pBot, CBotAiState* pAiState)
{
	if (pBot)
	{
		if (pAiState)
		{
			if (pBot->IsFleeAble())
			{
				if (!pAiState->AddSubControlList(new CBotAiCondition_Flee(pBot, 30.0f), false))
					return false;
			}

			if (!pBot->IsRetreat() && pBot->HasAIFunction(AI_FUNC_FLAG_RETREAT))
			{
				if (!pAiState->AddSubControlList(new CBotAiCondition_Retreat(pBot), false))
					return false;
			}

			return true;
		}
	}

	return false;
}

inline bool AttachAIS_State(CNpc *pBot, CBotAiState *pBotAiState, eAIS_NODE eAttachAisNode)
{
	bool bRet = false;

	if (pBot)
	{
		if (pBotAiState)
		{
			if (pBot->GetAiScript() != INVALID_TBLIDX)
			{
				CAIScriptControlFactoy factory(pBot);

				if (!g_pAiScriptManager->AttachScript(pBotAiState, &factory, pBot->GetAiScript(), pBot->GetAiScriptScene(), eAttachAisNode))
					return false;
			}

			bRet = true;
		}
		else
		{
			ERR_LOG(LOG_BOTAI, "fail : NULL == pControlState_Bot. eAttachAisNode %u", eAttachAisNode);
			bRet = false;
		}
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "NULL == pBot. eAttachAisNode %u", eAttachAisNode);
		bRet = false;
	}

	return bRet;
}