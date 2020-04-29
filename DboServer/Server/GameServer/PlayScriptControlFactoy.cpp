#include "stdafx.h"
#include "PlayScriptControlFactoy.h"
#include "ControlStateEx.h"

#include "BotAiAction_Wait.h"
#include "BotAiAction_Say.h"
#include "BotAiAction_NavMove.h"
#include "BotAiAction_Patrol.h"
#include "BotAiAction_Rotate.h"
#include "BotAiAction_SitDown.h"
#include "BotAiAction_DirectPlay.h"
#include "BotAiAction_DirectTurning.h"
#include "BotAiAction_SetCharCondition.h"
#include "BotAiAction_SendSpsEvent.h"
#include "BotAiAction_Leave.h"
#include "BotAiAction_Escort.h"
#include "BotAiAction_SPSEnd.h"
#include "BotAiAction_TargetFixAttack.h"
#include "BotAiAction_SkillUse.h"
#include "BotAiAction_ChangeAiBitFlag.h"
#include "BotAiAction_ChangeTimidAttribute.h"
#include "BotAiAction_ChangeCharAttribute.h"
#include "BotAiAction_ChangeNpcAttribute.h"
#include "BotAiAction_Wander.h"
#include "BotAiAction_Function.h"
#include "BotAiAction_SendEscortEventToTs.h"
#include "BotAiAction_SendEventToTqs.h"
#include "BotAiAction_SPS_SendEventToWps.h"
#include "BotAiAction_SPS_SendEventToTs.h"
#include "BotAiAction_SPS_SendEvent.h"
#include "BotAiAction_ClearSpsEvent.h"
#include "BotAiCondition_Child.h"
#include "BotAiCondition_SPS_RecvEventFromTs.h"
#include "BotAiCondition_SPS_RecvEventFromWps.h"
#include "BotAiCondition_SPS_RecvEventFromItem.h"
#include "BotAiCondition_RegisterBattleScene.h"
#include "BotAiCondition_BSCondition.h"
#include "BotAiCondition_BSConditionGroup.h"
#include "BotAiCondition_BSFlow.h"
#include "BotAiCondition_BSCheckLp.h"

#include <unordered_map>

std::unordered_map<int, CControlState*(*)(CNpc* pBot)> s_controlStateMap_sps;
bool s_bInitailize_sps = false;

CControlState* ControlStateCreator_CBotAiAction_Wait(CNpc *pBot)
{
	return new CBotAiAction_Wait(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_Say(CNpc *pBot)
{
	return new CBotAiAction_Say(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_NavMove(CNpc *pBot)
{
	return new CBotAiAction_NavMove(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_Patrol(CNpc *pBot)
{
	return new CBotAiAction_Patrol(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_Rotate(CNpc *pBot)
{
	return new CBotAiAction_Rotate(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_SitDown(CNpc *pBot)
{
	return new CBotAiAction_SitDown(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_DirectPlay(CNpc *pBot)
{
	return new CBotAiAction_DirectPlay(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_DirectTurning(CNpc *pBot)
{
	return new CBotAiAction_DirectTurning(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_SetCharCondition(CNpc *pBot)
{
	return new CBotAiAction_SetCharCondition(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_SendSpsEvent(CNpc *pBot)
{
	return new CBotAiAction_SendSpsEvent(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_Leave(CNpc *pBot)
{
	return new CBotAiAction_Leave(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_Escort(CNpc *pBot)
{
	return new CBotAiAction_Escort(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_SPSEnd(CNpc *pBot)
{
	return new CBotAiAction_SPSEnd(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_TargetFixAttack(CNpc *pBot)
{
	return new CBotAiAction_TargetFixAttack(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_SkillUse(CNpc *pBot)
{
	return new CBotAiAction_SkillUse(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_ChangeAiBitFlag(CNpc *pBot)
{
	return new CBotAiAction_ChangeAiBitFlag(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_ChangeTimidAttribute(CNpc *pBot)
{
	return new CBotAiAction_ChangeTimidAttribute(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_ChangeCharAttribute(CNpc *pBot)
{
	return new CBotAiAction_ChangeCharAttribute(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_ChangeNpcAttribute(CNpc *pBot)
{
	return new CBotAiAction_ChangeNpcAttribute(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_Wander(CNpc *pBot)
{
	return new CBotAiAction_Wander(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_Function(CNpc *pBot)
{
	return new CBotAiAction_Function(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_SendEscortEventToTs(CNpc *pBot)
{
	return new CBotAiAction_SendEscortEventToTs(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_SendEventToTqs(CNpc *pBot)
{
	return new CBotAiAction_SendEventToTqs(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_SPS_SendEventToWps(CNpc *pBot)
{
	return new CBotAiAction_SPS_SendEventToWps(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_SPS_SendEventToTs(CNpc *pBot)
{
	return new CBotAiAction_SPS_SendEventToTs(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_SPS_SendEvent(CNpc *pBot)
{
	return new CBotAiAction_SPS_SendEvent(pBot);
}
CControlState* ControlStateCreator_CBotAiAction_ClearSpsEvent(CNpc *pBot)
{
	return new CBotAiAction_ClearSpsEvent(pBot);
}
CControlState* ControlStateCreator_CBotAiCondition_Child(CNpc *pBot)
{
	return new CBotAiCondition_Child(pBot);
}
CControlState* ControlStateCreator_CBotAiCondition_SPS_RecvEventFromTs(CNpc *pBot)
{
	return new CBotAiCondition_SPS_RecvEventFromTs(pBot);
}
CControlState* ControlStateCreator_CBotAiCondition_SPS_RecvEventFromWps(CNpc *pBot)
{
	return new CBotAiCondition_SPS_RecvEventFromWps(pBot);
}
CControlState* ControlStateCreator_CBotAiCondition_SPS_RecvEventFromItem(CNpc *pBot)
{
	return new CBotAiCondition_SPS_RecvEventFromItem(pBot);
}
CControlState* ControlStateCreator_CBotAiCondition_RegisterBattleScene(CNpc *pBot)
{
	return new CBotAiCondition_RegisterBattleScene(pBot);
}
CControlState* ControlStateCreator_CBotAiCondition_BSCondition(CNpc *pBot)
{
	return new CBotAiCondition_BSCondition(pBot);
}
CControlState* ControlStateCreator_CBotAiCondition_BSConditionGroup(CNpc *pBot)
{
	return new CBotAiCondition_BSConditionGroup(pBot);
}
CControlState* ControlStateCreator_CBotAiCondition_BSFlow(CNpc *pBot)
{
	return new CBotAiCondition_BSFlow(pBot);
}
CControlState* ControlStateCreator_CBotAiCondition_BSCheckLp(CNpc *pBot)
{
	return new CBotAiCondition_BSCheckLp(pBot);
}

inline void SPSInitialize()
{
	//printf("PlayScriptControlFactoy: SPSInitialize \n");

	s_controlStateMap_sps[SPS_NODE_ACTION_WAIT] = ControlStateCreator_CBotAiAction_Wait;
	s_controlStateMap_sps[SPS_NODE_ACTION_SAY] = ControlStateCreator_CBotAiAction_Say;
	s_controlStateMap_sps[SPS_NODE_ACTION_POINT_MOVE] = ControlStateCreator_CBotAiAction_NavMove;
	s_controlStateMap_sps[SPS_NODE_ACTION_PATH_MOVE] = ControlStateCreator_CBotAiAction_Patrol;
	s_controlStateMap_sps[SPS_NODE_ACTION_ROTATE] = ControlStateCreator_CBotAiAction_Rotate;
	s_controlStateMap_sps[SPS_NODE_ACTION_SITDOWN] = ControlStateCreator_CBotAiAction_SitDown;
	s_controlStateMap_sps[SPS_NODE_ACTION_DIRECT_PLAY] = ControlStateCreator_CBotAiAction_DirectPlay;
	s_controlStateMap_sps[SPS_NODE_ACTION_DIRECT_TURNING] = ControlStateCreator_CBotAiAction_DirectTurning;
	s_controlStateMap_sps[SPS_NODE_ACTION_SET_CHAR_CONDITION] = ControlStateCreator_CBotAiAction_SetCharCondition;
	s_controlStateMap_sps[SPS_NODE_ACTION_SEND_SPS_EVENT] = ControlStateCreator_CBotAiAction_SendSpsEvent;
	s_controlStateMap_sps[SPS_NODE_ACTION_LEAVE] = ControlStateCreator_CBotAiAction_Leave;
	s_controlStateMap_sps[SPS_NODE_ACTION_ESCORT] = ControlStateCreator_CBotAiAction_Escort;
	s_controlStateMap_sps[SPS_NODE_ACTION_SPSEND] = ControlStateCreator_CBotAiAction_SPSEnd;
	s_controlStateMap_sps[SPS_NODE_ACTION_TARGETFIXATTACK] = ControlStateCreator_CBotAiAction_TargetFixAttack;
	s_controlStateMap_sps[SPS_NODE_ACTION_SKILLUSE] = ControlStateCreator_CBotAiAction_SkillUse;
	s_controlStateMap_sps[SPS_NODE_ACTION_CHANGEAIBITFLAG] = ControlStateCreator_CBotAiAction_ChangeAiBitFlag;
	s_controlStateMap_sps[SPS_NODE_ACTION_CHANGE_TIMID_ATTRIBUTE] = ControlStateCreator_CBotAiAction_ChangeTimidAttribute;
	s_controlStateMap_sps[SPS_NODE_ACTION_CHANGE_CHAR_ATTRIBUTE] = ControlStateCreator_CBotAiAction_ChangeCharAttribute;
	s_controlStateMap_sps[SPS_NODE_ACTION_CHANGE_NPC_ATTRIBUTE] = ControlStateCreator_CBotAiAction_ChangeNpcAttribute;
	s_controlStateMap_sps[SPS_NODE_ACTION_DO_WANDER] = ControlStateCreator_CBotAiAction_Wander;
	s_controlStateMap_sps[SPS_NODE_ACTION_FUNCTION] = ControlStateCreator_CBotAiAction_Function;
	s_controlStateMap_sps[SPS_NODE_ACTION_SEND_ESCORT_EVENT_TO_TS] = ControlStateCreator_CBotAiAction_SendEscortEventToTs;
	s_controlStateMap_sps[SPS_NODE_ACTION_SEND_EVENT_TO_TQS] = ControlStateCreator_CBotAiAction_SendEventToTqs;
	s_controlStateMap_sps[SPS_NODE_ACTION_SEND_EVENT_TO_WPS] = ControlStateCreator_CBotAiAction_SPS_SendEventToWps;
	s_controlStateMap_sps[SPS_NODE_ACTION_SEND_EVENT_TO_TS] = ControlStateCreator_CBotAiAction_SPS_SendEventToTs;
	s_controlStateMap_sps[SPS_NODE_ACTION_SEND_EVENT] = ControlStateCreator_CBotAiAction_SPS_SendEvent;
	s_controlStateMap_sps[SPS_NODE_ACTION_CLEAR_EVENT] = ControlStateCreator_CBotAiAction_ClearSpsEvent;
	s_controlStateMap_sps[SPS_NODE_CONDITION_CHILD] = ControlStateCreator_CBotAiCondition_Child;
	s_controlStateMap_sps[SPS_NODE_CONDITION_RECV_EVENT_FROM_TS] = ControlStateCreator_CBotAiCondition_SPS_RecvEventFromTs;
	s_controlStateMap_sps[SPS_NODE_CONDITION_RECV_EVENT_FROM_WPS] = ControlStateCreator_CBotAiCondition_SPS_RecvEventFromWps;
	s_controlStateMap_sps[SPS_NODE_CONDITION_RECV_EVENT_FROM_ITEM] = ControlStateCreator_CBotAiCondition_SPS_RecvEventFromItem;
	s_controlStateMap_sps[SPS_NODE_CONDITION_REGISTER_BATTLE_SCENE] = ControlStateCreator_CBotAiCondition_RegisterBattleScene;
	s_controlStateMap_sps[SPS_NODE_CONDITION_BS_CONDITION] = ControlStateCreator_CBotAiCondition_BSCondition;
	s_controlStateMap_sps[SPS_NODE_CONDITION_BS_CONDITION_GROUP] = ControlStateCreator_CBotAiCondition_BSConditionGroup;
	s_controlStateMap_sps[SPS_NODE_CONDITION_BS_FLOW] = ControlStateCreator_CBotAiCondition_BSFlow;
	s_controlStateMap_sps[SPS_NODE_CONDITION_BS_CHECK_LP] = ControlStateCreator_CBotAiCondition_BSCheckLp;
}




CPlayScriptControlFactoy::CPlayScriptControlFactoy(CNpc* pOwner)
{
	m_pOwner = pOwner;
}

CPlayScriptControlFactoy::~CPlayScriptControlFactoy()
{

}

CControlState* CPlayScriptControlFactoy::CreateScriptControlState(int ctrlScrNodeID)
{
	if (!s_bInitailize_sps)
	{
		SPSInitialize();
		s_bInitailize_sps = true;
	}

	if (ctrlScrNodeID < MAX_SPS_NODE)
	{
		return s_controlStateMap_sps[ctrlScrNodeID](m_pOwner);
	}

	return NULL;
}

bool CPlayScriptControlFactoy::CanCreateControlState(int ctrlScrNodeID)
{
	if (!s_bInitailize_sps)
	{
		SPSInitialize();
		s_bInitailize_sps = true;
	}

	if (ctrlScrNodeID < MAX_SPS_NODE)
	{
		return true;
	}

	return false;
}

