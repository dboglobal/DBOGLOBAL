#include "stdafx.h"
#include "TimeQuestControlFactory.h"
#include "ScriptAlgoDefine.h"

#include <unordered_map>


#include "TqsAlgoAction_AddMob.h"
#include "TqsAlgoAction_AddMobGroup.h"
#include "TqsAlgoAction_AddNpc.h"
#include "TqsAlgoAction_BroadMessage.h"
#include "TqsAlgoAction_ChangeObjectState.h"
#include "TqsAlgoAction_ChangeStage.h"
#include "TqsAlgoAction_Direction_Indicate.h"
#include "TqsAlgoAction_DirectPlay.h"
#include "TqsAlgoAction_Function.h"
#include "TqsAlgoAction_Information.h"
#include "TqsAlgoAction_PlayBgm.h"
#include "TqsAlgoAction_PlayJingle.h"
#include "TqsAlgoAction_ProgressMessage.h"
#include "TqsAlgoAction_RegisterCondition.h"
#include "TqsAlgoAction_RemoveMobGroup.h"
#include "TqsAlgoAction_RemoveNpc.h"
#include "TqsAlgoAction_Reward.h"
#include "TqsAlgoAction_SendTqsEvent.h"
#include "TqsAlgoAction_SendTsEvent.h"
#include "TqsAlgoAction_Teleport.h"
#include "TqsAlgoAction_TimeCountdown.h"
#include "TqsAlgoAction_Wait.h"
#include "TqsAlgoCondition_AllPlayerFaint.h"
#include "TqsAlgoCondition_AreaEnter.h"
#include "TqsAlgoCondition_CheckMobGroup.h"
#include "TqsAlgoCondition_CheckTime.h"
#include "TqsAlgoCondition_Child.h"
#include "TqsAlgoCondition_NpcFaint.h"
#include "TqsAlgoCondition_RecvSpsEvent.h"
#include "TqsAlgoCondition_RecvTsEvent.h"


std::unordered_map<int, CControlState*(*)(CTqsAlgoObject* pOwner)> s_controlStateMap_tqs;
bool _bInitailize_tqs = false;


CControlState* ControlStateCreator_ScriptAlgoAction_AddNpc(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_AddNpc(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_AddMob(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_AddMob(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Wait(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_Wait(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_RemoveMobGroup(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_RemoveMobGroup(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_AddMobGroup(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_AddMobGroup(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_BroadMessage(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_BroadMessage(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_ChangeObjectState(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_ChangeObjectState(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_ChangeStage(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_ChangeStage(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Direction_Indicate(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_Direction_Indicate(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_DirectPlay(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_DirectPlay(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_PlayBgm(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_PlayBgm(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_PlayJingle(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_PlayJingle(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_RemoveNpc(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_RemoveNpc(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_TimeCountdown(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_TimeCountdown(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Function(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_Function(pOwner);
}

CControlState* ControlStateCreator_ScriptAlgoAction_Information(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_Information(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_ProgressMessage(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_ProgressMessage(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_RegisterCondition(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_RegisterCondition(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Reward(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_Reward(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_SendTqsEvent(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_SendTqsEvent(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_SendTsEvent(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_SendTsEvent(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Teleport(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoAction_Teleport(pOwner);
}


CControlState* ControlStateCreator_ScriptAlgoCondition_CheckTime(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoCondition_CheckTime(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_NpcFaint(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoCondition_NpcFaint(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_AreaEnter(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoCondition_AreaEnter(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_CheckMobGroup(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoCondition_CheckMobGroup(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_Child(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoCondition_Child(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_AllPlayerFaint(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoCondition_AllPlayerFaint(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_RecvSpsEvent(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoCondition_RecvSpsEvent(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_RecvTsEvent(CTqsAlgoObject *pOwner)
{
	return new CTqsAlgoCondition_RecvTsEvent(pOwner);
}




inline void TQSInitialize()
{
	s_controlStateMap_tqs[TQS_NODE_ACTION_ADD_NPC] = ControlStateCreator_ScriptAlgoAction_AddNpc;
	s_controlStateMap_tqs[TQS_NODE_ACTION_ADD_MOB] = ControlStateCreator_ScriptAlgoAction_AddMob;
	s_controlStateMap_tqs[TQS_NODE_ACTION_WAIT] = ControlStateCreator_ScriptAlgoAction_Wait;
	s_controlStateMap_tqs[TQS_NODE_ACTION_REMOVE_MOB_GROUP] = ControlStateCreator_ScriptAlgoAction_RemoveMobGroup;
	s_controlStateMap_tqs[TQS_NODE_ACTION_ADD_MOB_GROUP] = ControlStateCreator_ScriptAlgoAction_AddMobGroup;
	s_controlStateMap_tqs[TQS_NODE_ACTION_BROAD_MESSAGE] = ControlStateCreator_ScriptAlgoAction_BroadMessage;
	s_controlStateMap_tqs[TQS_NODE_ACTION_CHANGE_OBJECT_STATE] = ControlStateCreator_ScriptAlgoAction_ChangeObjectState;
	s_controlStateMap_tqs[TQS_NODE_ACTION_CHANGE_STAGE] = ControlStateCreator_ScriptAlgoAction_ChangeStage;
	s_controlStateMap_tqs[TQS_NODE_ACTION_DIRECTION_INDICATE] = ControlStateCreator_ScriptAlgoAction_Direction_Indicate;
	s_controlStateMap_tqs[TQS_NODE_ACTION_ADD_DIRECT_PLAY] = ControlStateCreator_ScriptAlgoAction_DirectPlay;
	s_controlStateMap_tqs[TQS_NODE_ACTION_PLAY_BGM] = ControlStateCreator_ScriptAlgoAction_PlayBgm;
	s_controlStateMap_tqs[TQS_NODE_ACTION_PLAY_JINGLE] = ControlStateCreator_ScriptAlgoAction_PlayJingle;
	s_controlStateMap_tqs[TQS_NODE_ACTION_REMOVE_NPC] = ControlStateCreator_ScriptAlgoAction_RemoveNpc;
	s_controlStateMap_tqs[TQS_NODE_ACTION_TIME_COUNTDOWN] = ControlStateCreator_ScriptAlgoAction_TimeCountdown;
	s_controlStateMap_tqs[TQS_NODE_ACTION_FUNCTION] = ControlStateCreator_ScriptAlgoAction_Function;
	s_controlStateMap_tqs[TQS_NODE_ACTION_INFORMATION] = ControlStateCreator_ScriptAlgoAction_Information;
	s_controlStateMap_tqs[TQS_NODE_ACTION_PROGRESS_MESSAGE] = ControlStateCreator_ScriptAlgoAction_ProgressMessage;
	s_controlStateMap_tqs[TQS_NODE_ACTION_REGISTER_CONDITION] = ControlStateCreator_ScriptAlgoAction_RegisterCondition;
	s_controlStateMap_tqs[TQS_NODE_ACTION_REWARD] = ControlStateCreator_ScriptAlgoAction_Reward;
	s_controlStateMap_tqs[TQS_NODE_ACTION_SEND_TQS_EVENT] = ControlStateCreator_ScriptAlgoAction_SendTqsEvent;
	s_controlStateMap_tqs[TQS_NODE_ACTION_SEND_TS_EVENT] = ControlStateCreator_ScriptAlgoAction_SendTsEvent;
	s_controlStateMap_tqs[TQS_NODE_ACTION_TELEPORT] = ControlStateCreator_ScriptAlgoAction_Teleport;


	s_controlStateMap_tqs[TQS_NODE_CONDITION_CHECK_TIME] = ControlStateCreator_ScriptAlgoCondition_CheckTime;
	s_controlStateMap_tqs[TQS_NODE_CONDITION_NPC_FAINT] = ControlStateCreator_ScriptAlgoCondition_NpcFaint;
	s_controlStateMap_tqs[TQS_NODE_CONDITION_AREA_ENTER] = ControlStateCreator_ScriptAlgoCondition_AreaEnter;
	s_controlStateMap_tqs[TQS_NODE_CONDITION_CHECK_MOB_GROUP] = ControlStateCreator_ScriptAlgoCondition_CheckMobGroup;
	s_controlStateMap_tqs[TQS_NODE_CONDITION_CHILD] = ControlStateCreator_ScriptAlgoCondition_Child;
	s_controlStateMap_tqs[TQS_NODE_CONDITION_ALL_PLAYER_FAINT] = ControlStateCreator_ScriptAlgoCondition_AllPlayerFaint;
	s_controlStateMap_tqs[TQS_NODE_CONDITION_RECV_SPS_EVENT] = ControlStateCreator_ScriptAlgoCondition_RecvSpsEvent;
	s_controlStateMap_tqs[TQS_NODE_CONDITION_RECV_TS_EVENT] = ControlStateCreator_ScriptAlgoCondition_RecvTsEvent;
}




CTimeQuestControlFactory::CTimeQuestControlFactory(CTqsAlgoObject* pOwner)
{
	m_pOwner = pOwner;
}


CTimeQuestControlFactory::~CTimeQuestControlFactory()
{
}


CControlState* CTimeQuestControlFactory::CreateScriptControlState(int ctrlScrNodeID)
{
	if (!_bInitailize_tqs)
	{
		TQSInitialize();
		_bInitailize_tqs = true;
	}

	//printf("CreateScriptControlState: CAN CREATE %u \n", ctrlScrNodeID);

	if (ctrlScrNodeID < MAX_TQS_NODE)
	{
		return s_controlStateMap_tqs[ctrlScrNodeID](m_pOwner);
	}

	return NULL;
}
