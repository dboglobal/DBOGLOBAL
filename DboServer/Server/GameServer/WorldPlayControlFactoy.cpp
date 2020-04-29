#include "stdafx.h"
#include "WorldPlayControlFactory.h"
#include "ScriptAlgoDefine.h"

#include <unordered_map>

#include "ScriptAlgoAction_Default.h"
#include "ScriptAlgoAction_Case.h"
#include "ScriptAlgoAction_AddNpc.h"
#include "ScriptAlgoAction_AddMob.h"
#include "ScriptAlgoAction_Loop.h"
#include "ScriptAlgoAction_Wait.h"
#include "ScriptAlgoAction_While.h"
#include "ScriptAlgoAction_Calculate.h"
#include "ScriptAlgoAction_GetLocation.h"
#include "ScriptAlgoAction_SendEventToTs.h"
#include "ScriptAlgoAction_RemoveMobGroup.h"
#include "ScriptAlgoAction_WpsFail.h"

#include "ScriptAlgoAction_AddMobGroup.h"
#include "ScriptAlgoAction_BroadMessage.h"
#include "ScriptAlgoAction_ChangeCharAttribute.h"
#include "ScriptAlgoAction_ChangeNpcAttribute.h"
#include "ScriptAlgoAction_ChangeObjectState.h"
#include "ScriptAlgoAction_ChangeSpsScene.h"
#include "ScriptAlgoAction_ChangeStage.h"
#include "ScriptAlgoAction_ClearEvent.h"
#include "ScriptAlgoAction_Direction_Indicate.h"
#include "ScriptAlgoAction_DirectPlay.h"
#include "ScriptAlgoAction_Else.h"
#include "ScriptAlgoAction_EventHandler.h"
#include "ScriptAlgoAction_EventStatusClear.h"
#include "ScriptAlgoAction_EventStatusRegister.h"
#include "ScriptAlgoAction_EventStatusSelect.h"
#include "ScriptAlgoAction_ExecAtOnce.h"
#include "ScriptAlgoAction_ExecWps.h"
#include "ScriptAlgoAction_FlashPlay.h"
#include "ScriptAlgoAction_GetFaintLocation.h"
#include "ScriptAlgoAction_If.h"
#include "ScriptAlgoAction_MobList.h"
#include "ScriptAlgoAction_PlayBgm.h"
#include "ScriptAlgoAction_PlayJingle.h"
#include "ScriptAlgoAction_RegisterBuff.h"
#include "ScriptAlgoAction_RemoveNpc.h"
#include "ScriptAlgoAction_SendAISEvent.h"
#include "ScriptAlgoAction_SendEventToAis.h"
#include "ScriptAlgoAction_SendEventToSps.h"
#include "ScriptAlgoAction_SendEventToTqs.h"
#include "ScriptAlgoAction_SendEventToWps.h"
#include "ScriptAlgoAction_SendSPSEvent.h"
#include "ScriptAlgoAction_Switch.h"
#include "ScriptAlgoAction_SystemMessage.h"
#include "ScriptAlgoAction_TelecastMessage.h"
#include "ScriptAlgoAction_Then.h"
#include "ScriptAlgoAction_TimeCountdown.h"
#include "ScriptAlgoAction_UnregisterBuff.h"
#include "ScriptAlgoAction_Function.h"
#include "WpsScriptAlgoAction_CCBD.h"
#include "WpsScriptAlgoAction_CCBD_exec_pattern.h"
#include "WpsScriptAlgoAction_CCBD_Leave.h"
#include "WpsScriptAlgoAction_CCBD_register_direct_play.h"
#include "WpsScriptAlgoAction_CCBD_reward.h"
#include "WpsScriptAlgoAction_CCBD_stage.h"
#include "WpsScriptAlgoAction_CCBD_stage_clear.h"
#include "WpsScriptAlgoAction_CCBD_wait_user.h"


#include "ScriptAlgoCondition_CheckTime.h"
#include "ScriptAlgoCondition_LoopCond.h"
#include "ScriptAlgoCondition_NpcFaint.h"
#include "ScriptAlgoCondition_AreaEnter.h"
#include "ScriptAlgoCondition_CheckLp.h"
#include "ScriptAlgoCondition_CheckMobGroup.h"
#include "ScriptAlgoCondition_Child.h"

#include "ScriptAlgoCondition_AllPlayerFaint.h"
#include "ScriptAlgoCondition_CheckBattle.h"
#include "ScriptAlgoCondition_CheckCountDown.h"
#include "ScriptAlgoCondition_Compare.h"
#include "ScriptAlgoCondition_NpcFaint.h"
#include "ScriptAlgoCondition_RecvEventFromAis.h"
#include "ScriptAlgoCondition_RecvEventFromItem.h"
#include "ScriptAlgoCondition_RecvEventFromSps.h"
#include "ScriptAlgoCondition_RecvEventFromTqs.h"
#include "ScriptAlgoCondition_RecvEventFromTs.h"
#include "ScriptAlgoCondition_RecvEventFromWps.h"
#include "ScriptAlgoCondition_RecvSpsEvent.h"
#include "ScriptAlgoCondition_RecvTsEvent.h"
#include "ScriptAlgoCondition_SpawnedMob.h"
#include "ScriptAlgoCondition_SpawnedNpc.h"
#include "ScriptAlgoCondition_WaitCondition.h"
#include "WpsScriptAlgoCondition_CCBD_check_limit_time.h"
#include "WpsScriptAlgoCondition_CCBD_check_run_state.h"
#include "WpsScriptAlgoCondition_CCBD_pattern.h"


std::unordered_map<int, CControlState*(*)(CWpsAlgoObject* pOwner)> s_controlStateMap_wps;
bool s_bInitailize_wps = false;


CControlState* ControlStateCreator_ScriptAlgoAction_AddNpc(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_AddNpc(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_AddMob(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_AddMob(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Wait(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_Wait(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_While(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_While(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Loop(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_Loop(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Calculate(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_Calculate(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_GetLocation(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_GetLocation(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_SendEventToTs(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_SendEventToTs(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_RemoveMobGroup(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_RemoveMobGroup(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_WpsFail(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_WpsFail(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Default(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_Default(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Case(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_Case(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_AddMobGroup(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_AddMobGroup(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_BroadMessage(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_BroadMessage(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_ChangeCharAttribute(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_ChangeCharAttribute(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_ChangeNpcAttribute(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_ChangeNpcAttribute(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_ChangeObjectState(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_ChangeObjectState(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_ChangeSpsScene(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_ChangeSpsScene(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_ChangeStage(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_ChangeStage(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_ClearEvent(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_ClearEvent(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Direction_Indicate(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_Direction_Indicate(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_DirectPlay(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_DirectPlay(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Else(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_Else(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_EventHandler(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_EventHandler(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_EventStatusClear(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_EventStatusClear(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_EventStatusRegister(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_EventStatusRegister(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_EventStatusSelect(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_EventStatusSelect(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_ExecAtOnce(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_ExecAtOnce(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_ExecWps(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_ExecWps(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_FlashPlay(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_FlashPlay(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_GetFaintLocation(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_GetFaintLocation(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_If(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_If(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_MobList(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_MobList(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_PlayBgm(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_PlayBgm(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_PlayJingle(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_PlayJingle(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_RegisterBuff(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_RegisterBuff(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_RemoveNpc(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_RemoveNpc(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_SendAISEvent(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_SendAISEvent(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_SendEventToAis(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_SendEventToAis(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_SendEventToSps(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_SendEventToSps(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_SendEventToTqs(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_SendEventToTqs(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_SendEventToWps(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_SendEventToWps(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_SendSPSEvent(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_SendSPSEvent(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Switch(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_Switch(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_SystemMessage(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_SystemMessage(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_TelecastMessage(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_TelecastMessage(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Then(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_Then(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_TimeCountdown(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_TimeCountdown(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_UnregisterBuff(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_UnregisterBuff(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_Function(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoAction_Function(pOwner);
}


CControlState* ControlStateCreator_ScriptAlgoAction_CCBD(CWpsAlgoObject *pOwner)
{
	return new CWpsScriptAlgoAction_CCBD(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_CCBD_register_direct_play(CWpsAlgoObject *pOwner)
{
	return new CWpsScriptAlgoAction_CCBD_register_direct_play(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_CCBD_wait_user(CWpsAlgoObject *pOwner)
{
	return new CWpsScriptAlgoAction_CCBD_wait_user(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_CCBD_stage(CWpsAlgoObject *pOwner)
{
	return new CWpsScriptAlgoAction_CCBD_stage(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_CCBD_exec_pattern(CWpsAlgoObject *pOwner)
{
	return new CWpsScriptAlgoAction_CCBD_exec_pattern(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_CCBD_stage_clear(CWpsAlgoObject *pOwner)
{
	return new CWpsScriptAlgoAction_CCBD_stage_clear(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_CCBD_reward(CWpsAlgoObject *pOwner)
{
	return new CWpsScriptAlgoAction_CCBD_reward(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoAction_CCBD_Leave(CWpsAlgoObject *pOwner)
{
	return new CWpsScriptAlgoAction_CCBD_Leave(pOwner);
}


CControlState* ControlStateCreator_ScriptAlgoCondition_CheckTime(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_CheckTime(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_NpcFaint(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_NpcFaint(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_LoopCond(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_LoopCond(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_AreaEnter(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_AreaEnter(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_CheckLp(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_CheckLp(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_CheckMobGroup(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_CheckMobGroup(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_Child(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_Child(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_AllPlayerFaint(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_AllPlayerFaint(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_CheckBattle(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_CheckBattle(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_CheckCountDown(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_CheckCountDown(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_Compare(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_Compare(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_RecvEventFromAis(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_RecvEventFromAis(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_RecvEventFromItem(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_RecvEventFromItem(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_RecvEventFromSps(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_RecvEventFromSps(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_RecvEventFromTqs(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_RecvEventFromTqs(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_RecvEventFromTs(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_RecvEventFromTs(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_RecvEventFromWps(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_RecvEventFromWps(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_RecvSpsEvent(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_RecvSpsEvent(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_RecvTsEvent(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_RecvTsEvent(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_SpawnedMob(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_SpawnedMob(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_SpawnedNpc(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_SpawnedNpc(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_WaitCondition(CWpsAlgoObject *pOwner)
{
	return new CWpsAlgoCondition_WaitCondition(pOwner);
}

CControlState* ControlStateCreator_ScriptAlgoCondition_CCBD_check_run_state(CWpsAlgoObject *pOwner)
{
	return new CWpsScriptAlgoCondition_CCBD_check_run_state(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_CCBD_check_limit_time(CWpsAlgoObject *pOwner)
{
	return new CWpsScriptAlgoCondition_CCBD_check_limit_time(pOwner);
}
CControlState* ControlStateCreator_ScriptAlgoCondition_CCBD_pattern(CWpsAlgoObject *pOwner)
{
	return new CWpsScriptAlgoCondition_CCBD_pattern(pOwner);
}



inline void WPSInitialize()
{
	s_controlStateMap_wps[WPS_NODE_ACTION_ADD_NPC] = ControlStateCreator_ScriptAlgoAction_AddNpc;
	s_controlStateMap_wps[WPS_NODE_ACTION_ADD_MOB] = ControlStateCreator_ScriptAlgoAction_AddMob;
	s_controlStateMap_wps[WPS_NODE_ACTION_WAIT] = ControlStateCreator_ScriptAlgoAction_Wait;
	s_controlStateMap_wps[WPS_NODE_ACTION_WHILE] = ControlStateCreator_ScriptAlgoAction_While;
	s_controlStateMap_wps[WPS_NODE_ACTION_LOOP] = ControlStateCreator_ScriptAlgoAction_Loop;
	s_controlStateMap_wps[WPS_NODE_ACTION_CALCULATE] = ControlStateCreator_ScriptAlgoAction_Calculate;
	s_controlStateMap_wps[WPS_NODE_ACTION_GET_LOCATION] = ControlStateCreator_ScriptAlgoAction_GetLocation;
	s_controlStateMap_wps[WPS_NODE_ACTION_SEND_EVENT_TO_TS] = ControlStateCreator_ScriptAlgoAction_SendEventToTs;
	s_controlStateMap_wps[WPS_NODE_ACTION_REMOVE_MOB_GROUP] = ControlStateCreator_ScriptAlgoAction_RemoveMobGroup;
	s_controlStateMap_wps[WPS_NODE_ACTION_WPS_FAIL] = ControlStateCreator_ScriptAlgoAction_WpsFail;
	s_controlStateMap_wps[WPS_NODE_ACTION_DEFAULT] = ControlStateCreator_ScriptAlgoAction_Default;
	s_controlStateMap_wps[WPS_NODE_ACTION_CASE] = ControlStateCreator_ScriptAlgoAction_Case;
	s_controlStateMap_wps[WPS_NODE_ACTION_ADD_MOB_GROUP] = ControlStateCreator_ScriptAlgoAction_AddMobGroup;
	s_controlStateMap_wps[WPS_NODE_ACTION_BROAD_MESSAGE] = ControlStateCreator_ScriptAlgoAction_BroadMessage;
	s_controlStateMap_wps[WPS_NODE_ACTION_CHANGE_CHAR_ATTRIBUTE] = ControlStateCreator_ScriptAlgoAction_ChangeCharAttribute;
	s_controlStateMap_wps[WPS_NODE_ACTION_CHANGE_NPC_ATTRIBUTE] = ControlStateCreator_ScriptAlgoAction_ChangeNpcAttribute;
	s_controlStateMap_wps[WPS_NODE_ACTION_CHANGE_SPS_SCENE] = ControlStateCreator_ScriptAlgoAction_ChangeSpsScene;
	s_controlStateMap_wps[WPS_NODE_ACTION_CHANGE_OBJECT_STATE] = ControlStateCreator_ScriptAlgoAction_ChangeObjectState;
	s_controlStateMap_wps[WPS_NODE_ACTION_CHANGE_STAGE] = ControlStateCreator_ScriptAlgoAction_ChangeStage;
	s_controlStateMap_wps[WPS_NODE_ACTION_CLEAR_EVENT] = ControlStateCreator_ScriptAlgoAction_ClearEvent;
	s_controlStateMap_wps[WPS_NODE_ACTION_DIRECTION_INDICATE] = ControlStateCreator_ScriptAlgoAction_Direction_Indicate;
	s_controlStateMap_wps[WPS_NODE_ACTION_ADD_DIRECT_PLAY] = ControlStateCreator_ScriptAlgoAction_DirectPlay;
	s_controlStateMap_wps[WPS_NODE_ACTION_ELSE] = ControlStateCreator_ScriptAlgoAction_Else;
	s_controlStateMap_wps[WPS_NODE_ACTION_EVENT_HANDLER] = ControlStateCreator_ScriptAlgoAction_EventHandler;
	s_controlStateMap_wps[WPS_NODE_ACTION_EVENTSTATUS_CLEAR] = ControlStateCreator_ScriptAlgoAction_EventStatusClear;
	s_controlStateMap_wps[WPS_NODE_ACTION_EVENTSTATUS_REGISTER] = ControlStateCreator_ScriptAlgoAction_EventStatusRegister;
	s_controlStateMap_wps[WPS_NODE_ACTION_EVENTSTATUS_SELECT] = ControlStateCreator_ScriptAlgoAction_EventStatusSelect;
	s_controlStateMap_wps[WPS_NODE_ACTION_EXEC_AT_ONCE] = ControlStateCreator_ScriptAlgoAction_ExecAtOnce;
	s_controlStateMap_wps[WPS_NODE_ACTION_EXEC_WPS] = ControlStateCreator_ScriptAlgoAction_ExecWps;
	s_controlStateMap_wps[WPS_NODE_ACTION_ADD_FLASH_PLAY] = ControlStateCreator_ScriptAlgoAction_FlashPlay;
	s_controlStateMap_wps[WPS_NODE_ACTION_GET_FAINTLOCATION] = ControlStateCreator_ScriptAlgoAction_GetFaintLocation;
	s_controlStateMap_wps[WPS_NODE_ACTION_IF] = ControlStateCreator_ScriptAlgoAction_If;
	s_controlStateMap_wps[WPS_NODE_ACTION_MOB_LIST] = ControlStateCreator_ScriptAlgoAction_MobList;
	s_controlStateMap_wps[WPS_NODE_ACTION_PLAY_BGM] = ControlStateCreator_ScriptAlgoAction_PlayBgm;
	s_controlStateMap_wps[WPS_NODE_ACTION_PLAY_JINGLE] = ControlStateCreator_ScriptAlgoAction_PlayJingle;
	s_controlStateMap_wps[WPS_NODE_ACTION_REGISTER_BUFF] = ControlStateCreator_ScriptAlgoAction_RegisterBuff;
	s_controlStateMap_wps[WPS_NODE_ACTION_REMOVE_NPC] = ControlStateCreator_ScriptAlgoAction_RemoveNpc;
	s_controlStateMap_wps[WPS_NODE_ACTION_SEND_AIS_EVENT] = ControlStateCreator_ScriptAlgoAction_SendAISEvent;
	s_controlStateMap_wps[WPS_NODE_ACTION_SEND_EVENT_TO_AIS] = ControlStateCreator_ScriptAlgoAction_SendEventToAis;
	s_controlStateMap_wps[WPS_NODE_ACTION_SEND_EVENT_TO_SPS] = ControlStateCreator_ScriptAlgoAction_SendEventToSps;
	s_controlStateMap_wps[WPS_NODE_ACTION_SEND_EVENT_TO_TQS] = ControlStateCreator_ScriptAlgoAction_SendEventToTqs;
	s_controlStateMap_wps[WPS_NODE_ACTION_SEND_EVENT_TO_WPS] = ControlStateCreator_ScriptAlgoAction_SendEventToWps;
	s_controlStateMap_wps[WPS_NODE_ACTION_SEND_SPS_EVENT] = ControlStateCreator_ScriptAlgoAction_SendSPSEvent;
	s_controlStateMap_wps[WPS_NODE_ACTION_SWITCH] = ControlStateCreator_ScriptAlgoAction_Switch;
	s_controlStateMap_wps[WPS_NODE_ACTION_SYSTEM_MESSAGE] = ControlStateCreator_ScriptAlgoAction_SystemMessage;
	s_controlStateMap_wps[WPS_NODE_ACTION_TELECAST_MESSAGE] = ControlStateCreator_ScriptAlgoAction_TelecastMessage;
	s_controlStateMap_wps[WPS_NODE_ACTION_THEN] = ControlStateCreator_ScriptAlgoAction_Then;
	s_controlStateMap_wps[WPS_NODE_ACTION_TIME_COUNTDOWN] = ControlStateCreator_ScriptAlgoAction_TimeCountdown;
	s_controlStateMap_wps[WPS_NODE_ACTION_UNREGISTER_BUFF] = ControlStateCreator_ScriptAlgoAction_UnregisterBuff;
	s_controlStateMap_wps[WPS_NODE_ACTION_FUNCTION] = ControlStateCreator_ScriptAlgoAction_Function;

	s_controlStateMap_wps[WPS_NODE_ACTION_CCBD] = ControlStateCreator_ScriptAlgoAction_CCBD;
	s_controlStateMap_wps[WPS_NODE_ACTION_CCBD_REGISTER_DIRECT_PLAY] = ControlStateCreator_ScriptAlgoAction_CCBD_register_direct_play;
	s_controlStateMap_wps[WPS_NODE_ACTION_CCBD_WAIT_USER] = ControlStateCreator_ScriptAlgoAction_CCBD_wait_user;
	s_controlStateMap_wps[WPS_NODE_ACTION_CCBD_STAGE] = ControlStateCreator_ScriptAlgoAction_CCBD_stage;
	s_controlStateMap_wps[WPS_NODE_ACTION_CCBD_REWARD] = ControlStateCreator_ScriptAlgoAction_CCBD_reward;
	s_controlStateMap_wps[WPS_NODE_ACTION_CCBD_EXEC_PATTERN] = ControlStateCreator_ScriptAlgoAction_CCBD_exec_pattern;
	s_controlStateMap_wps[WPS_NODE_ACTION_CCBD_STAGE_CLEAR] = ControlStateCreator_ScriptAlgoAction_CCBD_stage_clear;
	s_controlStateMap_wps[WPS_NODE_ACTION_CCBD_LEAVE] = ControlStateCreator_ScriptAlgoAction_CCBD_Leave;

	//TODO
	//	WPS_NODE_ACTION_DWC,
	//	WPS_NODE_ACTION_DWC_WAIT_USER,
	//	WPS_NODE_ACTION_DWC_REWARD,
	//	WPS_NODE_ACTION_DWC_FAIL,
	//	WPS_NODE_ACTION_DWC_LEAVE,

	s_controlStateMap_wps[WPS_NODE_CONDITION_CHECK_TIME] = ControlStateCreator_ScriptAlgoCondition_CheckTime;
	s_controlStateMap_wps[WPS_NODE_CONDITION_NPC_FAINT] = ControlStateCreator_ScriptAlgoCondition_NpcFaint;
	s_controlStateMap_wps[WPS_NODE_CONDITION_LOOP_COND] = ControlStateCreator_ScriptAlgoCondition_LoopCond;
	s_controlStateMap_wps[WPS_NODE_CONDITION_AREA_ENTER] = ControlStateCreator_ScriptAlgoCondition_AreaEnter;
	s_controlStateMap_wps[WPS_NODE_CONDITION_CHECK_LP] = ControlStateCreator_ScriptAlgoCondition_CheckLp;
	s_controlStateMap_wps[WPS_NODE_CONDITION_CHECK_MOB_GROUP] = ControlStateCreator_ScriptAlgoCondition_CheckMobGroup;
	s_controlStateMap_wps[WPS_NODE_CONDITION_CHILD] = ControlStateCreator_ScriptAlgoCondition_Child;
	s_controlStateMap_wps[WPS_NODE_CONDITION_ALL_PLAYER_FAINT] = ControlStateCreator_ScriptAlgoCondition_AllPlayerFaint;
	s_controlStateMap_wps[WPS_NODE_CONDITION_CHECK_BATTLE] = ControlStateCreator_ScriptAlgoCondition_CheckBattle;
	s_controlStateMap_wps[WPS_NODE_CONDITION_CHECK_COUNTDOWN] = ControlStateCreator_ScriptAlgoCondition_CheckCountDown;
	s_controlStateMap_wps[WPS_NODE_CONDITION_COMPARE] = ControlStateCreator_ScriptAlgoCondition_Compare;
	s_controlStateMap_wps[WPS_NODE_CONDITION_RECV_EVENT_FROM_AIS] = ControlStateCreator_ScriptAlgoCondition_RecvEventFromAis;
	s_controlStateMap_wps[WPS_NODE_CONDITION_RECV_EVENT_FROM_ITEM] = ControlStateCreator_ScriptAlgoCondition_RecvEventFromItem;
	s_controlStateMap_wps[WPS_NODE_CONDITION_RECV_EVENT_FROM_SPS] = ControlStateCreator_ScriptAlgoCondition_RecvEventFromSps;
	s_controlStateMap_wps[WPS_NODE_CONDITION_RECV_EVENT_FROM_TQS] = ControlStateCreator_ScriptAlgoCondition_RecvEventFromTqs;
	s_controlStateMap_wps[WPS_NODE_CONDITION_RECV_EVENT_FROM_TS] = ControlStateCreator_ScriptAlgoCondition_RecvEventFromTs;
	s_controlStateMap_wps[WPS_NODE_CONDITION_RECV_EVENT_FROM_WPS] = ControlStateCreator_ScriptAlgoCondition_RecvEventFromWps;
	s_controlStateMap_wps[WPS_NODE_CONDITION_RECV_SPS_EVENT] = ControlStateCreator_ScriptAlgoCondition_RecvSpsEvent;
	s_controlStateMap_wps[WPS_NODE_CONDITION_RECV_TS_EVENT] = ControlStateCreator_ScriptAlgoCondition_RecvTsEvent;
	s_controlStateMap_wps[WPS_NODE_CONDITION_SPAWNED_MOB] = ControlStateCreator_ScriptAlgoCondition_SpawnedMob;
	s_controlStateMap_wps[WPS_NODE_CONDITION_SPAWNED_NPC] = ControlStateCreator_ScriptAlgoCondition_SpawnedNpc;
	s_controlStateMap_wps[WPS_NODE_CONDITION_WAIT_CONDITION] = ControlStateCreator_ScriptAlgoCondition_WaitCondition;

	s_controlStateMap_wps[WPS_NODE_CONDITION_CCBD_PATTERN] = ControlStateCreator_ScriptAlgoCondition_CCBD_pattern;
	s_controlStateMap_wps[WPS_NODE_CONDITION_CCBD_CHECK_RUN_STATE] = ControlStateCreator_ScriptAlgoCondition_CCBD_check_run_state;
	s_controlStateMap_wps[WPS_NODE_CONDITION_CCBD_CHECK_LIMIT_TIME] = ControlStateCreator_ScriptAlgoCondition_CCBD_check_limit_time;
}




CWorldPlayControlFactory::CWorldPlayControlFactory(CWpsAlgoObject* pOwner)
{
	m_pOwner = pOwner;
}


CWorldPlayControlFactory::~CWorldPlayControlFactory()
{
}


CControlState* CWorldPlayControlFactory::CreateScriptControlState(int ctrlScrNodeID)
{
	if (!s_bInitailize_wps)
	{
		WPSInitialize();
		s_bInitailize_wps = true;
	}

	//printf("CreateScriptControlState: CAN CREATE %u \n", ctrlScrNodeID);

	if (ctrlScrNodeID < MAX_WPS_NODE)
	{
		return s_controlStateMap_wps[ctrlScrNodeID](m_pOwner);
	}

	return NULL;
}
