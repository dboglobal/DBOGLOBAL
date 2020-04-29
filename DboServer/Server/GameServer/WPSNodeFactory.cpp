#include "stdafx.h"
#include "WPSNodeFactory.h"

#include "WPSNodeAction_AddMob.h"
#include "WPSNodeAction_AddMobGroup.h"
#include "WPSNodeAction_AddNpc.h"
#include "WPSNodeAction_BroadMessage.h"
#include "WPSNodeAction_Calculate.h"
#include "WPSNodeAction_Case.h"
#include "WPSNodeAction_ChangeCharAttribute.h"
#include "WPSNodeAction_ChangeNpcAttribute.h"
#include "WPSNodeAction_ChangeObjectState.h"
#include "WPSNodeAction_ChangeSpsScene.h"
#include "WPSNodeAction_ChangeStage.h"
#include "WPSNodeAction_ClearEvent.h"
#include "WPSNodeAction_Default.h"
#include "WPSNodeAction_Direction_Indicate.h"
#include "WPSNodeAction_DirectPlay.h"
#include "WPSNodeAction_Else.h"
#include "WPSNodeAction_EventHandler.h"
#include "WPSNodeAction_EventStatusClear.h"
#include "WPSNodeAction_EventStatusRegister.h"
#include "WPSNodeAction_EventStatusSelect.h"
#include "WPSNodeAction_ExecAtOnce.h"
#include "WPSNodeAction_ExecWps.h"
#include "WPSNodeAction_FlashPlay.h"
#include "WPSNodeAction_Function.h"
#include "WPSNodeAction_GetFaintLocation.h"
#include "WPSNodeAction_GetLocation.h"
#include "WPSNodeAction_If.h"
#include "WPSNodeAction_MobList.h"
#include "WPSNodeAction_PlayBgm.h"
#include "WPSNodeAction_PlayJingle.h"
#include "WPSNodeAction_RegisterBuff.h"
#include "WPSNodeAction_RemoveMobGroup.h"
#include "WPSNodeAction_RemoveNpc.h"
#include "WPSNodeAction_SendAISEvent.h"
#include "WPSNodeAction_SendEventToAis.h"
#include "WPSNodeAction_SendEventToSps.h"
#include "WPSNodeAction_SendEventToTqs.h"
#include "WPSNodeAction_SendEventToTs.h"
#include "WPSNodeAction_SendEventToWps.h"
#include "WPSNodeAction_SendSPSEvent.h"
#include "WPSNodeAction_Switch.h"
#include "WPSNodeAction_SystemMessage.h"
#include "WPSNodeAction_TelecastMessage.h"
#include "WPSNodeAction_Then.h"
#include "WPSNodeAction_TimeCountdown.h"
#include "WPSNodeAction_UnregisterBuff.h"
#include "WPSNodeAction_Wait.h"
#include "WPSNodeAction_While.h"
#include "WPSNodeAction_WpsFail.h"
#include "WPSNodeAction_Loop.h"
#include "WpsNodeAction_CCBD.h"
#include "WpsNodeAction_CCBD_register_direct_play.h"
#include "WpsNodeAction_CCBD_wait_user.h"
#include "WpsNodeAction_CCBD_stage.h"
#include "WpsNodeAction_CCBD_exec_pattern.h"
#include "WpsNodeAction_CCBD_stage_clear.h"
#include "WpsNodeAction_CCBD_reward.h"
#include "WpsNodeAction_CCBD_Leave.h"

#include "WPSNodeCondition_AllPlayerFaint.h"
#include "WPSNodeCondition_AreaEnter.h"
#include "WPSNodeCondition_CheckBattle.h"
#include "WPSNodeCondition_CheckCountDown.h"
#include "WPSNodeCondition_CheckLP.h"
#include "WPSNodeCondition_CheckMobGroup.h"
#include "WPSNodeCondition_CheckTime.h"
#include "WPSNodeCondition_Child.h"
#include "WPSNodeCondition_Compare.h"
#include "WPSNodeCondition_LoopCond.h"
#include "WPSNodeCondition_NpcFaint.h"
#include "WPSNodeCondition_RecvEventFromAis.h"
#include "WPSNodeCondition_RecvEventFromItem.h"
#include "WPSNodeCondition_RecvEventFromSps.h"
#include "WPSNodeCondition_RecvEventFromTqs.h"
#include "WPSNodeCondition_RecvEventFromTs.h"
#include "WPSNodeCondition_RecvEventFromWps.h"
#include "WPSNodeCondition_RecvSpsEvent.h"
#include "WPSNodeCondition_RecvTsEvent.h"
#include "WPSNodeCondition_SpawnedMob.h"
#include "WPSNodeCondition_SpawnedNpc.h"
#include "WPSNodeCondition_WaitCondition.h"
#include "WpsNodeCondition_CCBD_check_run_state.h"
#include "WpsNodeCondition_CCBD_check_limit_time.h"
#include "WpsNodeCondition_CCBD_pattern.h"

#include <unordered_map>

std::unordered_map<std::string, CControlScriptNodeAction*(*)(const char*)> s_actionMap_2;
std::unordered_map<std::string, CControlScriptNodeCondition*(*)(const char*)> s_conditionMap_2;

bool s_bInitailize_2 = false;

CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_AddMob(const char* lpszNodeName)
{
	return new CWPSNodeAction_AddMob(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_AddMobGroup(const char* lpszNodeName)
{
	return new CWPSNodeAction_AddMobGroup(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_AddNpc(const char* lpszNodeName)
{
	return new CWPSNodeAction_AddNpc(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_BroadMessage(const char* lpszNodeName)
{
	return new CWPSNodeAction_BroadMessage(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_Calculate(const char* lpszNodeName)
{
	return new CWPSNodeAction_Calculate(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_Case(const char* lpszNodeName)
{
	return new CWPSNodeAction_Case(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_ChangeCharAttribute(const char* lpszNodeName)
{
	return new CWPSNodeAction_ChangeCharAttribute(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_ChangeNpcAttribute(const char* lpszNodeName)
{
	return new CWPSNodeAction_ChangeNpcAttribute(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_ChangeObjectState(const char* lpszNodeName)
{
	return new CWPSNodeAction_ChangeObjectState(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_ChangeSpsScene(const char* lpszNodeName)
{
	return new CWPSNodeAction_ChangeSpsScene(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_ChangeStage(const char* lpszNodeName)
{
	return new CWPSNodeAction_ChangeStage(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_ClearEvent(const char* lpszNodeName)
{
	return new CWPSNodeAction_ClearEvent(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_Default(const char* lpszNodeName)
{
	return new CWPSNodeAction_Default(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_Direction_Indicate(const char* lpszNodeName)
{
	return new CWPSNodeAction_Direction_Indicate(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_DirectPlay(const char* lpszNodeName)
{
	return new CWPSNodeAction_DirectPlay(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_Else(const char* lpszNodeName)
{
	return new CWPSNodeAction_Else(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_EventHandler(const char* lpszNodeName)
{
	return new CWPSNodeAction_EventHandler(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_EventStatusClear(const char* lpszNodeName)
{
	return new CWPSNodeAction_EventStatusClear(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_EventStatusRegister(const char* lpszNodeName)
{
	return new CWPSNodeAction_EventStatusRegister(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_EventStatusSelect(const char* lpszNodeName)
{
	return new CWPSNodeAction_EventStatusSelect(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_ExecAtOnce(const char* lpszNodeName)
{
	return new CWPSNodeAction_ExecAtOnce(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_ExecWps(const char* lpszNodeName)
{
	return new CWPSNodeAction_ExecWps(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_FlashPlay(const char* lpszNodeName)
{
	return new CWPSNodeAction_FlashPlay(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_Function(const char* lpszNodeName)
{
	CWPSNodeAction_Function* pNode = new CWPSNodeAction_Function(lpszNodeName);

	if (pNode->Create(true))
		return pNode;
	else
		printf("ActionNodeCreator_CWPSNodeAction_Function: Failed to create \n");

	return NULL;
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_GetFaintLocation(const char* lpszNodeName)
{
	return new CWPSNodeAction_GetFaintLocation(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_GetLocation(const char* lpszNodeName)
{
	return new CWPSNodeAction_GetLocation(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_If(const char* lpszNodeName)
{
	return new CWPSNodeAction_If(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_MobList(const char* lpszNodeName)
{
	return new CWPSNodeAction_MobList(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_PlayBgm(const char* lpszNodeName)
{
	return new CWPSNodeAction_PlayBgm(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_PlayJingle(const char* lpszNodeName)
{
	return new CWPSNodeAction_PlayJingle(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_RegisterBuff(const char* lpszNodeName)
{
	return new CWPSNodeAction_RegisterBuff(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_RemoveMobGroup(const char* lpszNodeName)
{
	return new CWPSNodeAction_RemoveMobGroup(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_RemoveNpc(const char* lpszNodeName)
{
	return new CWPSNodeAction_RemoveNpc(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_SendAISEvent(const char* lpszNodeName)
{
	return new CWPSNodeAction_SendAISEvent(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_SendEventToAis(const char* lpszNodeName)
{
	return new CWPSNodeAction_SendEventToAis(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_SendEventToSps(const char* lpszNodeName)
{
	return new CWPSNodeAction_SendEventToSps(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_SendEventToTqs(const char* lpszNodeName)
{
	return new CWPSNodeAction_SendEventToTqs(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_SendEventToTs(const char* lpszNodeName)
{
	return new CWPSNodeAction_SendEventToTs(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_SendEventToWps(const char* lpszNodeName)
{
	return new CWPSNodeAction_SendEventToWps(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_SendSPSEvent(const char* lpszNodeName)
{
	return new CWPSNodeAction_SendSPSEvent(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_Switch(const char* lpszNodeName)
{
	return new CWPSNodeAction_Switch(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_SystemMessage(const char* lpszNodeName)
{
	return new CWPSNodeAction_SystemMessage(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_TelecastMessage(const char* lpszNodeName)
{
	return new CWPSNodeAction_TelecastMessage(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_Then(const char* lpszNodeName)
{
	return new CWPSNodeAction_Then(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_TimeCountdown(const char* lpszNodeName)
{
	return new CWPSNodeAction_TimeCountdown(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_UnregisterBuff(const char* lpszNodeName)
{
	return new CWPSNodeAction_UnregisterBuff(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_Wait(const char* lpszNodeName)
{
	return new CWPSNodeAction_Wait(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_While(const char* lpszNodeName)
{
	return new CWPSNodeAction_While(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_WpsFail(const char* lpszNodeName)
{
	return new CWPSNodeAction_WpsFail(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_Loop(const char* lpszNodeName)
{
	return new CWPSNodeAction_Loop(lpszNodeName);
}

CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_CCBD(const char* lpszNodeName)
{
	return new CWpsNodeAction_CCBD(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_CCBD_register_direct_play(const char* lpszNodeName)
{
	return new CWpsNodeAction_CCBD_register_direct_play(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_CCBD_wait_user(const char* lpszNodeName)
{
	return new CWpsNodeAction_CCBD_wait_user(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_CCBD_stage(const char* lpszNodeName)
{
	return new CWpsNodeAction_CCBD_stage(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_CCBD_exec_pattern(const char* lpszNodeName)
{
	return new CWpsNodeAction_CCBD_exec_pattern(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_CCBD_stage_clear(const char* lpszNodeName)
{
	return new CWpsNodeAction_CCBD_stage_clear(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_CCBD_reward(const char* lpszNodeName)
{
	return new CWpsNodeAction_CCBD_reward(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CWPSNodeAction_CCBD_Leave(const char* lpszNodeName)
{
	return new CWpsNodeAction_CCBD_Leave(lpszNodeName);
}




CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_AllPlayerFaint(const char* lpszNodeName)
{
	return new CWPSNodeCondition_AllPlayerFaint(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_AreaEnter(const char* lpszNodeName)
{
	return new CWPSNodeCondition_AreaEnter(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_CheckBattle(const char* lpszNodeName)
{
	return new CWPSNodeCondition_CheckBattle(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_CheckCountDown(const char* lpszNodeName)
{
	return new CWPSNodeCondition_CheckCountDown(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_CheckLP(const char* lpszNodeName)
{
	return new CWPSNodeCondition_CheckLP(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_CheckMobGroup(const char* lpszNodeName)
{
	return new CWPSNodeCondition_CheckMobGroup(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_CheckTime(const char* lpszNodeName)
{
	return new CWPSNodeCondition_CheckTime(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_Child(const char* lpszNodeName)
{
	CWPSNodeCondition_Child* pNode = new CWPSNodeCondition_Child(lpszNodeName);

	if (pNode->Create(true))
		return pNode;
	else
		printf("ActionNodeCreator_CWPSNodeAction_Function: Failed to create \n");

	return NULL;
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_Compare(const char* lpszNodeName)
{
	return new CWPSNodeCondition_Compare(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_LoopCond(const char* lpszNodeName)
{
	return new CWPSNodeCondition_LoopCond(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_NpcFaint(const char* lpszNodeName)
{
	return new CWPSNodeCondition_NpcFaint(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_RecvEventFromAis(const char* lpszNodeName)
{
	return new CWPSNodeCondition_RecvEventFromAis(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_RecvEventFromItem(const char* lpszNodeName)
{
	return new CWPSNodeCondition_RecvEventFromItem(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_RecvEventFromSps(const char* lpszNodeName)
{
	return new CWPSNodeCondition_RecvEventFromSps(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_RecvEventFromTqs(const char* lpszNodeName)
{
	return new CWPSNodeCondition_RecvEventFromTqs(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_RecvEventFromTs(const char* lpszNodeName)
{
	return new CWPSNodeCondition_RecvEventFromTs(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_RecvEventFromWps(const char* lpszNodeName)
{
	return new CWPSNodeCondition_RecvEventFromWps(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_RecvSpsEvent(const char* lpszNodeName)
{
	return new CWPSNodeCondition_RecvSpsEvent(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_RecvTsEvent(const char* lpszNodeName)
{
	return new CWPSNodeCondition_RecvTsEvent(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_SpawnedMob(const char* lpszNodeName)
{
	return new CWPSNodeCondition_SpawnedMob(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_SpawnedNpc(const char* lpszNodeName)
{
	return new CWPSNodeCondition_SpawnedNpc(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_WaitCondition(const char* lpszNodeName)
{
	return new CWPSNodeCondition_WaitCondition(lpszNodeName);
}

CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_CCBD_check_run_state(const char* lpszNodeName)
{
	return new CWpsNodeCondition_CCBD_check_run_state(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_CCBD_check_limit_time(const char* lpszNodeName)
{
	return new CWpsNodeCondition_CCBD_check_limit_time(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CWPSNodeCondition_CCBD_pattern(const char* lpszNodeName)
{
	return new CWpsNodeCondition_CCBD_pattern(lpszNodeName);
}


inline void Initialize_2()
{
	s_actionMap_2["add mob"] = ActionNodeCreator_CWPSNodeAction_AddMob;
	s_actionMap_2["add mobgroup"] = ActionNodeCreator_CWPSNodeAction_AddMobGroup;
	s_actionMap_2["add npc"] = ActionNodeCreator_CWPSNodeAction_AddNpc;
	s_actionMap_2["broad message"] = ActionNodeCreator_CWPSNodeAction_BroadMessage;
	s_actionMap_2["calculate"] = ActionNodeCreator_CWPSNodeAction_Calculate;
	s_actionMap_2["case"] = ActionNodeCreator_CWPSNodeAction_Case;
	s_actionMap_2["change char attribute"] = ActionNodeCreator_CWPSNodeAction_ChangeCharAttribute;
	s_actionMap_2["change npc attribute"] = ActionNodeCreator_CWPSNodeAction_ChangeNpcAttribute;
	s_actionMap_2["change object state"] = ActionNodeCreator_CWPSNodeAction_ChangeObjectState;
	s_actionMap_2["change sps scene"] = ActionNodeCreator_CWPSNodeAction_ChangeSpsScene;
	s_actionMap_2["change stage"] = ActionNodeCreator_CWPSNodeAction_ChangeStage;
	s_actionMap_2["clear event"] = ActionNodeCreator_CWPSNodeAction_ClearEvent;
	s_actionMap_2["default"] = ActionNodeCreator_CWPSNodeAction_Default;
	s_actionMap_2["direction indicate"] = ActionNodeCreator_CWPSNodeAction_Direction_Indicate;
	s_actionMap_2["direct play"] = ActionNodeCreator_CWPSNodeAction_DirectPlay;
	s_actionMap_2["else"] = ActionNodeCreator_CWPSNodeAction_Else;
	s_actionMap_2["event handler"] = ActionNodeCreator_CWPSNodeAction_EventHandler;
	s_actionMap_2["event status clear"] = ActionNodeCreator_CWPSNodeAction_EventStatusClear;
	s_actionMap_2["event status register"] = ActionNodeCreator_CWPSNodeAction_EventStatusRegister;
	s_actionMap_2["event status select"] = ActionNodeCreator_CWPSNodeAction_EventStatusSelect;
	s_actionMap_2["exec at once"] = ActionNodeCreator_CWPSNodeAction_ExecAtOnce;
	s_actionMap_2["exec wps"] = ActionNodeCreator_CWPSNodeAction_ExecWps;
	s_actionMap_2["flash play"] = ActionNodeCreator_CWPSNodeAction_FlashPlay;
	s_actionMap_2["function"] = ActionNodeCreator_CWPSNodeAction_Function;
	s_actionMap_2["get faint location"] = ActionNodeCreator_CWPSNodeAction_GetFaintLocation;
	s_actionMap_2["get location"] = ActionNodeCreator_CWPSNodeAction_GetLocation;
	s_actionMap_2["if"] = ActionNodeCreator_CWPSNodeAction_If;
	s_actionMap_2["mob list"] = ActionNodeCreator_CWPSNodeAction_MobList;
	s_actionMap_2["play bgm"] = ActionNodeCreator_CWPSNodeAction_PlayBgm;
	s_actionMap_2["play jingle"] = ActionNodeCreator_CWPSNodeAction_PlayJingle;
	s_actionMap_2["register buff"] = ActionNodeCreator_CWPSNodeAction_RegisterBuff;
	s_actionMap_2["remove mobgroup"] = ActionNodeCreator_CWPSNodeAction_RemoveMobGroup;
	s_actionMap_2["remove npc"] = ActionNodeCreator_CWPSNodeAction_RemoveNpc;
	s_actionMap_2["send ais event"] = ActionNodeCreator_CWPSNodeAction_SendAISEvent;
	s_actionMap_2["send event to ais"] = ActionNodeCreator_CWPSNodeAction_SendEventToAis;
	s_actionMap_2["send event to sps"] = ActionNodeCreator_CWPSNodeAction_SendEventToSps;
	s_actionMap_2["send event to tqs"] = ActionNodeCreator_CWPSNodeAction_SendEventToTqs;
	s_actionMap_2["send event to ts"] = ActionNodeCreator_CWPSNodeAction_SendEventToTs;
	s_actionMap_2["send event to wps"] = ActionNodeCreator_CWPSNodeAction_SendEventToWps;
	s_actionMap_2["send sps event"] = ActionNodeCreator_CWPSNodeAction_SendSPSEvent;
	s_actionMap_2["switch"] = ActionNodeCreator_CWPSNodeAction_Switch;
	s_actionMap_2["system message"] = ActionNodeCreator_CWPSNodeAction_SystemMessage;
	s_actionMap_2["telecast message"] = ActionNodeCreator_CWPSNodeAction_TelecastMessage;
	s_actionMap_2["then"] = ActionNodeCreator_CWPSNodeAction_Then;
	s_actionMap_2["time countdown"] = ActionNodeCreator_CWPSNodeAction_TimeCountdown;
	s_actionMap_2["unregister buff"] = ActionNodeCreator_CWPSNodeAction_UnregisterBuff;
	s_actionMap_2["wait"] = ActionNodeCreator_CWPSNodeAction_Wait;
	s_actionMap_2["while"] = ActionNodeCreator_CWPSNodeAction_While;
	s_actionMap_2["wps fail"] = ActionNodeCreator_CWPSNodeAction_WpsFail;
	s_actionMap_2["loop"] = ActionNodeCreator_CWPSNodeAction_Loop;

	s_actionMap_2["CCBD"] = ActionNodeCreator_CWPSNodeAction_CCBD;
	s_actionMap_2["CCBD register direct play"] = ActionNodeCreator_CWPSNodeAction_CCBD_register_direct_play;
	s_actionMap_2["CCBD wait user"] = ActionNodeCreator_CWPSNodeAction_CCBD_wait_user;
	s_actionMap_2["CCBD stage"] = ActionNodeCreator_CWPSNodeAction_CCBD_stage;
	s_actionMap_2["CCBD exec pattern"] = ActionNodeCreator_CWPSNodeAction_CCBD_exec_pattern;
	s_actionMap_2["CCBD stage clear"] = ActionNodeCreator_CWPSNodeAction_CCBD_stage_clear;
	s_actionMap_2["CCBD reward"] = ActionNodeCreator_CWPSNodeAction_CCBD_reward;
	s_actionMap_2["CCBD leave"] = ActionNodeCreator_CWPSNodeAction_CCBD_Leave;

	s_conditionMap_2["all player faint"] = ConditionNodeCreator_CWPSNodeCondition_AllPlayerFaint;
	s_conditionMap_2["area enter"] = ConditionNodeCreator_CWPSNodeCondition_AreaEnter;
	s_conditionMap_2["check battle"] = ConditionNodeCreator_CWPSNodeCondition_CheckBattle;
	s_conditionMap_2["check countdown"] = ConditionNodeCreator_CWPSNodeCondition_CheckCountDown;
	s_conditionMap_2["check lp"] = ConditionNodeCreator_CWPSNodeCondition_CheckLP;
	s_conditionMap_2["check mobgroup"] = ConditionNodeCreator_CWPSNodeCondition_CheckMobGroup;
	s_conditionMap_2["check time"] = ConditionNodeCreator_CWPSNodeCondition_CheckTime;
	s_conditionMap_2["child"] = ConditionNodeCreator_CWPSNodeCondition_Child;
	s_conditionMap_2["compare"] = ConditionNodeCreator_CWPSNodeCondition_Compare;
	s_conditionMap_2["loop cond"] = ConditionNodeCreator_CWPSNodeCondition_LoopCond;
	s_conditionMap_2["npc faint"] = ConditionNodeCreator_CWPSNodeCondition_NpcFaint;
	s_conditionMap_2["recv event from ais"] = ConditionNodeCreator_CWPSNodeCondition_RecvEventFromAis;
	s_conditionMap_2["recv event from item"] = ConditionNodeCreator_CWPSNodeCondition_RecvEventFromItem;
	s_conditionMap_2["recv event from sps"] = ConditionNodeCreator_CWPSNodeCondition_RecvEventFromSps;
	s_conditionMap_2["recv event from tqs"] = ConditionNodeCreator_CWPSNodeCondition_RecvEventFromTqs;
	s_conditionMap_2["recv event from ts"] = ConditionNodeCreator_CWPSNodeCondition_RecvEventFromTs;
	s_conditionMap_2["recv event from wps"] = ConditionNodeCreator_CWPSNodeCondition_RecvEventFromWps;
	s_conditionMap_2["recv sps event"] = ConditionNodeCreator_CWPSNodeCondition_RecvSpsEvent;
	s_conditionMap_2["recv ts event"] = ConditionNodeCreator_CWPSNodeCondition_RecvTsEvent;
	s_conditionMap_2["spawned mob"] = ConditionNodeCreator_CWPSNodeCondition_SpawnedMob;
	s_conditionMap_2["spawned npc"] = ConditionNodeCreator_CWPSNodeCondition_SpawnedNpc;
	s_conditionMap_2["wait condition"] = ConditionNodeCreator_CWPSNodeCondition_WaitCondition;

	s_conditionMap_2["CCBD check run state"] = ConditionNodeCreator_CWPSNodeCondition_CCBD_check_run_state;
	s_conditionMap_2["CCBD check limit time"] = ConditionNodeCreator_CWPSNodeCondition_CCBD_check_limit_time;
	s_conditionMap_2["CCBD pattern"] = ConditionNodeCreator_CWPSNodeCondition_CCBD_pattern;
}





CWPSNodeFactory::CWPSNodeFactory()
{
}

CWPSNodeFactory::~CWPSNodeFactory()
{
}



CControlScriptNodeAction* CWPSNodeFactory::CreateControlNodeAction(const char* lpszNodeName)
{
	if (!s_bInitailize_2)
	{
		Initialize_2();
		s_bInitailize_2 = true;
	}

	auto it = s_actionMap_2.find(lpszNodeName);
	if (it == s_actionMap_2.end())
	{
		printf("CWPSNodeFactory::CreateControlNodeAction: %s not found \n", lpszNodeName);
		return NULL;
	}

	return it->second(lpszNodeName);
}

CControlScriptNodeCondition* CWPSNodeFactory::CreateControlNodeCondition(const char* lpszNodeName)
{
	if (!s_bInitailize_2)
	{
		Initialize_2();
		s_bInitailize_2 = true;
	}

	auto it = s_conditionMap_2.find(lpszNodeName);
	if (it == s_conditionMap_2.end())
	{
		printf("CWPSNodeFactory::CreateControlNodeCondition: %s not found \n", lpszNodeName);
	}

	return it->second(lpszNodeName);
}

