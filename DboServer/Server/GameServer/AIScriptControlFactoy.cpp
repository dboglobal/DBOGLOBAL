#include "stdafx.h"
#include "AIScriptControlFactoy.h"

#include "BotAiAction_AddMob.h"
#include "BotAiAction_AIS_SendEventToWps.h"
#include "BotAiAction_Say.h"
#include "BotAiAction_NavMove.h"
#include "BotAiAction_SetCharCondition.h"
#include "BotAiAction_TargetFixAttack.h"
#include "BotAiAction_SkillUse.h"
#include "BotAiAction_ChangeAiBitFlag.h"
#include "BotAiCondition_HimselfCheck.h"
#include "BotAiCondition_Scan.h"
#include "BotAiCondition_AIS_RecvEventFromWps.h"
#include "BotAiCondition_AIS_RecvEventFromTqs.h"

#include <unordered_map>

std::unordered_map<int, CControlState*(*)(CNpc* pBot, CControlState* pFrom)> s_ScripControlStateMap_ais;
std::unordered_map<int, CControlState*(*)(CNpc* pBot, CControlState* pFrom)> s_NodeControlStateMap_ais;
bool s_bInitailize_ais = false;


CControlState* AISControlStateCreator_CBotAiAction_AddMob(CNpc *pBot, CControlState *pFrom)
{
	CBotAiAction_AddMob* pNode = new CBotAiAction_AddMob(pBot);

	if (pFrom)
		pFrom->CopyTo(pNode);

	return pNode;
}
CControlState* AISControlStateCreator_CBotAiAction_AIS_SendEventToWps(CNpc *pBot, CControlState *pFrom)
{
	CBotAiAction_AIS_SendEventToWps* pNode = new CBotAiAction_AIS_SendEventToWps(pBot);

	if (pFrom)
		pFrom->CopyTo(pNode);

	return pNode;
}
CControlState* AISControlStateCreator_CBotAiAction_Say(CNpc *pBot, CControlState *pFrom)
{
	CBotAiAction_Say* pNode = new CBotAiAction_Say(pBot);

	if (pFrom)
		pFrom->CopyTo(pNode);

	return pNode;
}
CControlState* AISControlStateCreator_CBotAiAction_NavMove(CNpc *pBot, CControlState *pFrom)
{
	CBotAiAction_NavMove* pNode = new CBotAiAction_NavMove(pBot);

	if (pFrom)
		pFrom->CopyTo(pNode);

	return pNode;
}
CControlState* AISControlStateCreator_CBotAiAction_SetCharCondition(CNpc *pBot, CControlState *pFrom)
{
	CBotAiAction_SetCharCondition* pNode = new CBotAiAction_SetCharCondition(pBot);

	if (pFrom)
		pFrom->CopyTo(pNode);

	return pNode;
}
CControlState* AISControlStateCreator_CBotAiAction_TargetFixAttack(CNpc *pBot, CControlState *pFrom)
{
	CBotAiAction_TargetFixAttack* pNode = new CBotAiAction_TargetFixAttack(pBot);

	if (pFrom)
		pFrom->CopyTo(pNode);

	return pNode;
}
CControlState* AISControlStateCreator_CBotAiAction_SkillUse(CNpc *pBot, CControlState *pFrom)
{
	CBotAiAction_SkillUse* pNode = new CBotAiAction_SkillUse(pBot);

	if (pFrom)
		pFrom->CopyTo(pNode);

	return pNode;
}
CControlState* AISControlStateCreator_CBotAiAction_ChangeAiBitFlag(CNpc *pBot, CControlState *pFrom)
{
	CBotAiAction_ChangeAiBitFlag* pNode = new CBotAiAction_ChangeAiBitFlag(pBot);

	if (pFrom)
		pFrom->CopyTo(pNode);

	return pNode;
}
CControlState* AISControlStateCreator_CBotAiCondition_HimselfCheck(CNpc *pBot, CControlState *pFrom)
{
	CBotAiCondition_HimselfCheck* pNode = new CBotAiCondition_HimselfCheck(pBot);

	if (pFrom)
		pFrom->CopyTo(pNode);

	return pNode;
}
CControlState* AISControlStateCreator_CBotAiCondition_Scan(CNpc *pBot, CControlState *pFrom)
{
	CBotAiCondition_Scan* pNode = new CBotAiCondition_Scan(pBot);

	if (pFrom)
		pFrom->CopyTo(pNode);

	return pNode;
}
CControlState* AISControlStateCreator_CBotAiCondition_AIS_RecvEventFromWps(CNpc *pBot, CControlState *pFrom)
{
	CBotAiCondition_AIS_RecvEventFromWps* pNode = new CBotAiCondition_AIS_RecvEventFromWps(pBot);

	if (pFrom)
		pFrom->CopyTo(pNode);

	return pNode;
}
CControlState* AISControlStateCreator_CBotAiCondition_AIS_RecvEventFromTqs(CNpc *pBot, CControlState *pFrom)
{
	CBotAiCondition_AIS_RecvEventFromTqs* pNode = new CBotAiCondition_AIS_RecvEventFromTqs(pBot);

	if (pFrom)
		pFrom->CopyTo(pNode);

	return pNode;
}

inline void AISInitialize()
{
	s_ScripControlStateMap_ais[AIS_NODE_ACTION_ADD_MOB] = AISControlStateCreator_CBotAiAction_AddMob;
	s_NodeControlStateMap_ais[BOTCONTROL_ACTION_ADDMOB] = AISControlStateCreator_CBotAiAction_AddMob;

	s_ScripControlStateMap_ais[AIS_NODE_ACTION_SENDEVENTTOWPS] = AISControlStateCreator_CBotAiAction_AIS_SendEventToWps;
	s_NodeControlStateMap_ais[BOTCONTROL_ACTION_AIS_SEND_EVENT_TO_WPS] = AISControlStateCreator_CBotAiAction_AIS_SendEventToWps;

	s_ScripControlStateMap_ais[SPS_NODE_ACTION_SAY] = AISControlStateCreator_CBotAiAction_Say;
	s_NodeControlStateMap_ais[BOTCONTROL_ACTION_DIALOG] = AISControlStateCreator_CBotAiAction_Say;

	s_ScripControlStateMap_ais[SPS_NODE_ACTION_POINT_MOVE] = AISControlStateCreator_CBotAiAction_NavMove;
	s_NodeControlStateMap_ais[BOTCONTROL_ACTION_NAVMOVE] = AISControlStateCreator_CBotAiAction_NavMove;

	s_ScripControlStateMap_ais[SPS_NODE_ACTION_SET_CHAR_CONDITION] = AISControlStateCreator_CBotAiAction_SetCharCondition;
	s_NodeControlStateMap_ais[BOTCONTROL_ACTION_SET_CHAR_CONDITION] = AISControlStateCreator_CBotAiAction_SetCharCondition;

	s_ScripControlStateMap_ais[SPS_NODE_ACTION_TARGETFIXATTACK] = AISControlStateCreator_CBotAiAction_TargetFixAttack;
	s_NodeControlStateMap_ais[BOTCONTROL_ACTION_TARGETFIXATTACK] = AISControlStateCreator_CBotAiAction_SetCharCondition;

	s_ScripControlStateMap_ais[SPS_NODE_ACTION_SKILLUSE] = AISControlStateCreator_CBotAiAction_SkillUse;
	s_NodeControlStateMap_ais[BOTCONTROL_ACTION_SKILL_USE] = AISControlStateCreator_CBotAiAction_SkillUse;

	s_ScripControlStateMap_ais[SPS_NODE_ACTION_CHANGEAIBITFLAG] = AISControlStateCreator_CBotAiAction_ChangeAiBitFlag;
	s_NodeControlStateMap_ais[BOTCONTROL_ACTION_CHANGEAIBITFLAG] = AISControlStateCreator_CBotAiAction_ChangeAiBitFlag;

	s_ScripControlStateMap_ais[AIS_NODE_CONDITION_HIMSELF_CHECK] = AISControlStateCreator_CBotAiCondition_HimselfCheck;
	s_NodeControlStateMap_ais[BOTCONTROL_CONDITION_HIMSELFCHECK] = AISControlStateCreator_CBotAiCondition_HimselfCheck;

	s_ScripControlStateMap_ais[AIS_NODE_CONDITION_SCAN] = AISControlStateCreator_CBotAiCondition_Scan;
	s_NodeControlStateMap_ais[BOTCONTROL_CONDITION_SCAN] = AISControlStateCreator_CBotAiCondition_Scan;

	s_ScripControlStateMap_ais[AIS_NODE_CONDITION_RECV_EVENT_FROM_WPS] = AISControlStateCreator_CBotAiCondition_AIS_RecvEventFromWps;
	s_NodeControlStateMap_ais[BOTCONTROL_CONDITION_AIS_RECV_EVENT_FROM_WPS] = AISControlStateCreator_CBotAiCondition_AIS_RecvEventFromWps;

	s_ScripControlStateMap_ais[AIS_NODE_CONDITION_RECV_EVENT_FROM_TQS] = AISControlStateCreator_CBotAiCondition_AIS_RecvEventFromTqs;
	s_NodeControlStateMap_ais[BOTCONTROL_CONDITION_AIS_RECV_EVENT_FROM_TQS] = AISControlStateCreator_CBotAiCondition_AIS_RecvEventFromTqs;
}




CAIScriptControlFactoy::CAIScriptControlFactoy(CNpc* pOwner)
{
	m_pOwner = pOwner;
}

CAIScriptControlFactoy::~CAIScriptControlFactoy()
{
}


CControlState* CAIScriptControlFactoy::CreateScriptControlState(int controlStateID)
{
	if (!s_bInitailize_ais)
	{
		AISInitialize();
		s_bInitailize_ais = true;
	}

	if (controlStateID < MAX_AIS_NODE)
	{
		if (s_ScripControlStateMap_ais[controlStateID])
		{
			return s_ScripControlStateMap_ais[controlStateID](m_pOwner, NULL);
		}
		else
		{
			ERR_LOG(LOG_SCRIPT, "fail : ControlState[%u]", controlStateID);
		}
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "controlStateID == invalid [%u]", controlStateID);
	}

	return NULL;
}

CControlState* CAIScriptControlFactoy::CreateNodeControlState(int controlStateID, CControlState* controlstate)
{
	if (!s_bInitailize_ais)
	{
		AISInitialize();
		s_bInitailize_ais = true;
	}

	if (controlStateID < MAX_BOTCONTROLID)
	{
		if (s_NodeControlStateMap_ais[controlStateID])
		{
			return s_NodeControlStateMap_ais[controlStateID](m_pOwner, controlstate);
		}
		else
		{
			ERR_LOG(LOG_SCRIPT, "fail : ControlState[%u]", controlStateID);
		}
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "controlStateID == invalid [%u]", controlStateID);
	}

	return NULL;
}

bool CAIScriptControlFactoy::CanCreateControlState(int controlStateID)
{
	if (!s_bInitailize_ais)
	{
		AISInitialize();
		s_bInitailize_ais = true;
	}

	if (controlStateID < MAX_AIS_NODE)
	{
		if (s_ScripControlStateMap_ais[controlStateID])
		{
			return true;
		}
		else
		{
			ERR_LOG(LOG_SCRIPT, "fail : ControlState[%u]", controlStateID);
		}
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "fail : ControlState[%u]", controlStateID);
	}

	return false;
}

bool CAIScriptControlFactoy::CanCreateControlStateLoopCountCheck(DWORD dwScriptNodeIndex, DWORD & rdwLoopCount)
{
	if (m_pOwner)
	{
		if (rdwLoopCount == INVALID_DWORD)
		{
			return false;
		}
		else
		{
			DWORD dwState = m_pOwner->GetControlStateManager()->GetState(dwScriptNodeIndex);
			if (dwState == INVALID_DWORD)
			{
				return m_pOwner->GetControlStateManager()->AddState(dwScriptNodeIndex, rdwLoopCount);
			}
			else
			{
				rdwLoopCount = dwState;
				return rdwLoopCount != 0;
			}
		}
	}

	ERR_LOG(LOG_SCRIPT, "fail : NULL == m_pOwner");
	return false;
}

