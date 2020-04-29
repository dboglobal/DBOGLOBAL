#include "stdafx.h"
#include "TQSNodeFactory.h"

#include "TQSNodeAction_AddMob.h"
#include "TQSNodeAction_AddMobGroup.h"
#include "TQSNodeAction_AddNpc.h"
#include "TQSNodeAction_BroadMessage.h"
#include "TQSNodeAction_ChangeObjectState.h"
#include "TQSNodeAction_ChangeStage.h"
#include "TQSNodeAction_DirectionIndicate.h"
#include "TQSNodeAction_DirectPlay.h"
#include "TQSNodeAction_Information.h"
#include "TQSNodeAction_PlayBgm.h"
#include "TQSNodeAction_PlayJingle.h"
#include "TQSNodeAction_ProgressMessage.h"
#include "TQSNodeAction_RegisterCondition.h"
#include "TQSNodeAction_RemoveMobGroup.h"
#include "TQSNodeAction_RemoveNpc.h"
#include "TQSNodeAction_Reward.h"
#include "TQSNodeAction_SendTqsEvent.h"
#include "TQSNodeAction_SendTsEvent.h"
#include "TQSNodeAction_Teleport.h"
#include "TQSNodeAction_TimeCountdown.h"
#include "TQSNodeAction_Wait.h"
#include "TQSNodeAction_Function.h"

#include "TQSNodeCondition_AllPlayerFaint.h"
#include "TQSNodeCondition_AreaEnter.h"
#include "TQSNodeCondition_CheckMobGroup.h"
#include "TQSNodeCondition_CheckTime.h"
#include "TQSNodeCondition_Child.h"
#include "TQSNodeCondition_NpcFaint.h"
#include "TQSNodeCondition_RecvSpsEvent.h"
#include "TQSNodeCondition_RecvTsEvent.h"

#include <unordered_map>

std::unordered_map<std::string, CControlScriptNodeAction*(*)(const char*)> s_actionMap_tqs;
std::unordered_map<std::string, CControlScriptNodeCondition*(*)(const char*)> s_conditionMap_tqs;

bool s_bInitailize_tqs = false;


/*--------------------------------------------------------------*/

CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_AddNpc(const char* lpszNodeName)
{
	return new CTQSNodeAction_AddNpc(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_RemoveNpc(const char* lpszNodeName)
{
	return new CTQSNodeAction_RemoveNpc(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_AddMob(const char* lpszNodeName)
{
	return new CTQSNodeAction_AddMob(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_AddMobGroup(const char* lpszNodeName)
{
	return new CTQSNodeAction_AddMobGroup(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_RemoveMobGroup(const char* lpszNodeName)
{
	return new CTQSNodeAction_RemoveMobGroup(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_DirectPlay(const char* lpszNodeName)
{
	return new CTQSNodeAction_DirectPlay(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_Wait(const char* lpszNodeName)
{
	return new CTQSNodeAction_Wait(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_ChangeStage(const char* lpszNodeName)
{
	return new CTQSNodeAction_ChangeStage(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_ChangeObjectState(const char* lpszNodeName)
{
	return new CTQSNodeAction_ChangeObjectState(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_TimeCountdown(const char* lpszNodeName)
{
	return new CTQSNodeAction_TimeCountdown(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_SendTsEvent(const char* lpszNodeName)
{
	return new CTQSNodeAction_SendTsEvent(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_SendTqsEvent(const char* lpszNodeName)
{
	return new CTQSNodeAction_SendTqsEvent(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_Reward(const char* lpszNodeName)
{
	return new CTQSNodeAction_Reward(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_Information(const char* lpszNodeName)
{
	return new CTQSNodeAction_Information(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_BroadMessage(const char* lpszNodeName)
{
	return new CTQSNodeAction_BroadMessage(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_ProgressMessage(const char* lpszNodeName)
{
	return new CTQSNodeAction_ProgressMessage(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_DirectionIndicate(const char* lpszNodeName)
{
	return new CTQSNodeAction_DirectionIndicate(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_RegisterCondition(const char* lpszNodeName)
{
	return new CTQSNodeAction_RegisterCondition(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_Teleport(const char* lpszNodeName)
{
	return new CTQSNodeAction_Teleport(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_PlayBgm(const char* lpszNodeName)
{
	return new CTQSNodeAction_PlayBgm(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_PlayJingle(const char* lpszNodeName)
{
	return new CTQSNodeAction_PlayJingle(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CTQSNodeAction_Function(const char* lpszNodeName)
{
	CTQSNodeAction_Function* pNode = new CTQSNodeAction_Function(lpszNodeName);

	if (pNode->Create(true))
		return pNode;
	else
		printf("ActionNodeCreator_CTQNodeAction_Function: Failed to create \n");

	return NULL;
}

CControlScriptNodeCondition* ConditionNodeCreator_CTQSNodeCondition_RecvTsEvent(const char* lpszNodeName)
{
	return new CTQSNodeCondition_RecvTsEvent(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CTQSNodeCondition_RecvSpsEvent(const char* lpszNodeName)
{
	return new CTQSNodeCondition_RecvSpsEvent(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CTQSNodeCondition_CheckTime(const char* lpszNodeName)
{
	return new CTQSNodeCondition_CheckTime(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CTQSNodeCondition_CheckMobGroup(const char* lpszNodeName)
{
	return new CTQSNodeCondition_CheckMobGroup(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CTQSNodeCondition_NpcFaint(const char* lpszNodeName)
{
	return new CTQSNodeCondition_NpcFaint(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CTQSNodeCondition_AllPlayerFaint(const char* lpszNodeName)
{
	return new CTQSNodeCondition_AllPlayerFaint(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CTQSNodeCondition_AreaEnter(const char* lpszNodeName)
{
	return new CTQSNodeCondition_AreaEnter(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CTQSNodeCondition_Child(const char* lpszNodeName)
{
	CControlScriptNodeCondition* cNode = new CTQSNodeCondition_Child(lpszNodeName);

	if (cNode->Create(true))
		return cNode;
	else
		printf("ConditionNodeCreator_CSPSNodeCondition_Child: Failed to create \n");

	return NULL;
}




bool Initialize_tqs()
{
	s_actionMap_tqs["add npc"] = ActionNodeCreator_CTQSNodeAction_AddNpc;
	s_actionMap_tqs["remove npc"] = ActionNodeCreator_CTQSNodeAction_RemoveNpc;
	s_actionMap_tqs["add mob"] = ActionNodeCreator_CTQSNodeAction_AddMob;
	s_actionMap_tqs["add mobgroup"] = ActionNodeCreator_CTQSNodeAction_AddMobGroup;
	s_actionMap_tqs["remove mobgroup"] = ActionNodeCreator_CTQSNodeAction_RemoveMobGroup;
	s_actionMap_tqs["direct play"] = ActionNodeCreator_CTQSNodeAction_DirectPlay;
	s_actionMap_tqs["wait"] = ActionNodeCreator_CTQSNodeAction_Wait;
	s_actionMap_tqs["change stage"] = ActionNodeCreator_CTQSNodeAction_ChangeStage;
	s_actionMap_tqs["change object state"] = ActionNodeCreator_CTQSNodeAction_ChangeObjectState;
	s_actionMap_tqs["time countdown"] = ActionNodeCreator_CTQSNodeAction_TimeCountdown;
	s_actionMap_tqs["send ts event"] = ActionNodeCreator_CTQSNodeAction_SendTsEvent;
	s_actionMap_tqs["send tqs event"] = ActionNodeCreator_CTQSNodeAction_SendTqsEvent;
	s_actionMap_tqs["reward"] = ActionNodeCreator_CTQSNodeAction_Reward;
	s_actionMap_tqs["information"] = ActionNodeCreator_CTQSNodeAction_Information;
	s_actionMap_tqs["broad message"] = ActionNodeCreator_CTQSNodeAction_BroadMessage;
	s_actionMap_tqs["progress message"] = ActionNodeCreator_CTQSNodeAction_ProgressMessage;
	s_actionMap_tqs["direction indicate"] = ActionNodeCreator_CTQSNodeAction_DirectionIndicate;
	s_actionMap_tqs["register condition"] = ActionNodeCreator_CTQSNodeAction_RegisterCondition;
	s_actionMap_tqs["teleport"] = ActionNodeCreator_CTQSNodeAction_Teleport;
	s_actionMap_tqs["play bgm"] = ActionNodeCreator_CTQSNodeAction_PlayBgm;
	s_actionMap_tqs["play jingle"] = ActionNodeCreator_CTQSNodeAction_PlayJingle;
	s_actionMap_tqs["function"] = ActionNodeCreator_CTQSNodeAction_Function;

	s_conditionMap_tqs["recv ts event"] = ConditionNodeCreator_CTQSNodeCondition_RecvTsEvent;
	s_conditionMap_tqs["recv sps event"] = ConditionNodeCreator_CTQSNodeCondition_RecvSpsEvent;
	s_conditionMap_tqs["check time"] = ConditionNodeCreator_CTQSNodeCondition_CheckTime;
	s_conditionMap_tqs["check mobgroup"] = ConditionNodeCreator_CTQSNodeCondition_CheckMobGroup;
	s_conditionMap_tqs["npc faint"] = ConditionNodeCreator_CTQSNodeCondition_NpcFaint;
	s_conditionMap_tqs["all player faint"] = ConditionNodeCreator_CTQSNodeCondition_AllPlayerFaint;
	s_conditionMap_tqs["area enter"] = ConditionNodeCreator_CTQSNodeCondition_AreaEnter;
	s_conditionMap_tqs["child"] = ConditionNodeCreator_CTQSNodeCondition_Child;


	return true;
}



CTQSNodeFactory::CTQSNodeFactory()
{
}

CTQSNodeFactory::~CTQSNodeFactory()
{
}


CControlScriptNodeAction* CTQSNodeFactory::CreateControlNodeAction(const char* lpszNodeName)
{
	if (!s_bInitailize_tqs)
	{
		Initialize_tqs();
		s_bInitailize_tqs = true;
	}

	auto it = s_actionMap_tqs.find(lpszNodeName);
	if (it == s_actionMap_tqs.end())
	{
		printf("CTQSNodeFactory::CreateControlNodeAction: %s not found \n", lpszNodeName);
		return NULL;
	}

	return it->second(lpszNodeName);
}

CControlScriptNodeCondition* CTQSNodeFactory::CreateControlNodeCondition(const char* lpszNodeName)
{
	if (!s_bInitailize_tqs)
	{
		Initialize_tqs();
		s_bInitailize_tqs = true;
	}

	auto it = s_conditionMap_tqs.find(lpszNodeName);
	if (it == s_conditionMap_tqs.end())
	{
		printf("CTQSNodeFactory::CreateControlNodeCondition: %s not found \n", lpszNodeName);
	}

	return it->second(lpszNodeName);
}