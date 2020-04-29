#include "stdafx.h"
#include "SPSNodeFactory.h"

#include "SPSNodeAction_Calculate.h"
#include "SPSNodeAction_ChangeAiBitFlag.h"
#include "SPSNodeAction_ChangeCharAttribute.h"
#include "SPSNodeAction_ChangeNpcAttribute.h"
#include "SPSNodeAction_ChangeTimidAttribute.h"
#include "SPSNodeAction_ClearEvent.h"
#include "SPSNodeAction_DirectPlay.h"
#include "SPSNodeAction_DirectTurning.h"
#include "SPSNodeAction_DoWander.h"
#include "SPSNodeAction_Escort.h"
#include "SPSNodeAction_Function.h"
#include "SPSNodeAction_Leave.h"
#include "SPSNodeAction_PathMove.h"
#include "SPSNodeAction_PointMove.h"
#include "SPSNodeAction_Rotate.h"
#include "SPSNodeAction_Say.h"
#include "SPSNodeAction_SendEscortEventToTs.h"
#include "SPSNodeAction_SendEvent.h"
#include "SPSNodeAction_SendEventToSps.h"
#include "SPSNodeAction_SendEventToTqs.h"
#include "SPSNodeAction_SendEventToTs.h"
#include "SPSNodeAction_SendEventToWps.h"
#include "SPSNodeAction_SendSpsEvent.h"
#include "SPSNodeAction_SetCharCondition.h"
#include "SPSNodeAction_SitDown.h"
#include "SPSNodeAction_SkillUse.h"
#include "SPSNodeAction_SPSEnd.h"
#include "SPSNodeAction_TargetFixAttack.h"
#include "SPSNodeAction_Wait.h"

#include "SPSNodeCondition_BSCheckLp.h"
#include "SPSNodeCondition_BSCheckMobGroup.h"
#include "SPSNodeCondition_BSCheckNpc.h"
#include "SPSNodeCondition_BSCondition.h"
#include "SPSNodeCondition_BSConditionGroup.h"
#include "SPSNodeCondition_BSFlow.h"
#include "SPSNodeCondition_Child.h"
#include "SPSNodeCondition_RecvEventFromItem.h"
#include "SPSNodeCondition_RecvEventFromTs.h"
#include "SPSNodeCondition_RecvEventFromWps.h"
#include "SPSNodeCondition_RegisterBattleScene.h"

#include <unordered_map>


std::unordered_map<std::string, CControlScriptNodeAction*(*)(const char*)> s_actionMap_1;
std::unordered_map<std::string, CControlScriptNodeCondition*(*)(const char*)> s_conditionMap_1;

bool s_bInitailize_1 = false;


CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_PointMove(const char* lpszNodeName)
{
	return new CSPSNodeAction_PointMove(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_PathMove(const char* lpszNodeName)
{
	return new CSPSNodeAction_PathMove(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_Rotate(const char* lpszNodeName)
{
	return new CSPSNodeAction_Rotate(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_Wait(const char* lpszNodeName)
{
	return new CSPSNodeAction_Wait(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_SitDown(const char* lpszNodeName)
{
	return new CSPSNodeAction_SitDown(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_Say(const char* lpszNodeName)
{
	return new CSPSNodeAction_Say(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_DirectPlay(const char* lpszNodeName)
{
	return new CSPSNodeAction_DirectPlay(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_DirectTurning(const char* lpszNodeName)
{
	return new CSPSNodeAction_DirectTurning(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_SetCharCondition(const char* lpszNodeName)
{
	return new CSPSNodeAction_SetCharCondition(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_Leave(const char* lpszNodeName)
{
	return new CSPSNodeAction_Leave(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_Escort(const char* lpszNodeName)
{
	return new CSPSNodeAction_Escort(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_SPSEnd(const char* lpszNodeName)
{
	return new CSPSNodeAction_SPSEnd(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_TargetFixAttack(const char* lpszNodeName)
{
	return new CSPSNodeAction_TargetFixAttack(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_SkillUse(const char* lpszNodeName)
{
	return new CSPSNodeAction_SkillUse(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_ChangeAiBitFlag(const char* lpszNodeName)
{
	return new CSPSNodeAction_ChangeAiBitFlag(lpszNodeName, CSPSNodeAction_ChangeAiBitFlag::eSPS);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_ChangeTimidAttribute(const char* lpszNodeName)
{
	return new CSPSNodeAction_ChangeTimidAttribute(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_ChangeCharAttribute(const char* lpszNodeName)
{
	return new CSPSNodeAction_ChangeCharAttribute(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_ChangeNpcAttribute(const char* lpszNodeName)
{
	return new CSPSNodeAction_ChangeNpcAttribute(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_Function(const char* lpszNodeName)
{
	CSPSNodeAction_Function* pNode = new CSPSNodeAction_Function(lpszNodeName);

	if (pNode->Create(true))
		return pNode;
	else
	{
		SAFE_DELETE(pNode);
		ERR_LOG(LOG_SCRIPT, "ActionNodeCreator_CSPSNodeAction_Function: Failed to create");
	}

	return NULL;
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_SendSpsEvent(const char* lpszNodeName)
{
	return new CSPSNodeAction_SendSpsEvent(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_SendEventToSps(const char* lpszNodeName)
{
	return new CSPSNodeAction_SendEventToSps(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_SendEscortEventToTs(const char* lpszNodeName)
{
	return new CSPSNodeAction_SendEscortEventToTs(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_SendEventToTqs(const char* lpszNodeName)
{
	return new CSPSNodeAction_SendEventToTqs(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_SendEventToWps(const char* lpszNodeName)
{
	return new CSPSNodeAction_SendEventToWps(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_SendEventToTs(const char* lpszNodeName)
{
	return new CSPSNodeAction_SendEventToTs(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_SendEvent(const char* lpszNodeName)
{
	return new CSPSNodeAction_SendEvent(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_ClearEvent(const char* lpszNodeName)
{
	return new CSPSNodeAction_ClearEvent(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_Calculate(const char* lpszNodeName)
{
	return new CSPSNodeAction_Calculate(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CSPSNodeAction_DoWander(const char* lpszNodeName)
{
	return new CSPSNodeAction_DoWander(lpszNodeName);
}

CControlScriptNodeCondition* ConditionNodeCreator_CSPSNodeCondition_Child(const char* lpszNodeName)
{
	CSPSNodeCondition_Child* pNode = new CSPSNodeCondition_Child(lpszNodeName);

	if (pNode->Create(true))
		return pNode;
	else
	{
		SAFE_DELETE(pNode);
		ERR_LOG(LOG_SCRIPT, "ConditionNodeCreator_CSPSNodeCondition_Child: Failed to create");
	}

	return NULL;
}
CControlScriptNodeCondition* ConditionNodeCreator_CSPSNodeCondition_RecvEventFromTs(const char* lpszNodeName)
{
	return new CSPSNodeCondition_RecvEventFromTs(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CSPSNodeCondition_RecvEventFromWps(const char* lpszNodeName)
{
	return new CSPSNodeCondition_RecvEventFromWps(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CSPSNodeCondition_RecvEventFromItem(const char* lpszNodeName)
{
	return new CSPSNodeCondition_RecvEventFromItem(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CSPSNodeCondition_RegisterBattleScene(const char* lpszNodeName)
{
	return new CSPSNodeCondition_RegisterBattleScene(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CSPSNodeCondition_BSCondition(const char* lpszNodeName)
{
	return new CSPSNodeCondition_BSCondition(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CSPSNodeCondition_BSConditionGroup(const char* lpszNodeName)
{
	return new CSPSNodeCondition_BSConditionGroup(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CSPSNodeCondition_BSFlow(const char* lpszNodeName)
{
	return new CSPSNodeCondition_BSFlow(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CSPSNodeCondition_BSCheckLp(const char* lpszNodeName)
{
	return new CSPSNodeCondition_BSCheckLp(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CSPSNodeCondition_BSCheckMobGroup(const char* lpszNodeName)
{
	return new CSPSNodeCondition_BSCheckMobGroup(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CSPSNodeCondition_BSCheckNpc(const char* lpszNodeName)
{
	return new CSPSNodeCondition_BSCheckNpc(lpszNodeName);
}


/*--------------------------------------------------------------*/
inline void Initialize_1()
{
	s_actionMap_1["point move"] = ActionNodeCreator_CSPSNodeAction_PointMove;
	s_actionMap_1["path move"] = ActionNodeCreator_CSPSNodeAction_PathMove;
	s_actionMap_1["rotate"] = ActionNodeCreator_CSPSNodeAction_Rotate;
	s_actionMap_1["wait"] = ActionNodeCreator_CSPSNodeAction_Wait;
	s_actionMap_1["sitdown"] = ActionNodeCreator_CSPSNodeAction_SitDown;
	s_actionMap_1["say"] = ActionNodeCreator_CSPSNodeAction_Say;
	s_actionMap_1["direct play"] = ActionNodeCreator_CSPSNodeAction_DirectPlay;
	s_actionMap_1["direct turning"] = ActionNodeCreator_CSPSNodeAction_DirectTurning;
	s_actionMap_1["char condition"] = ActionNodeCreator_CSPSNodeAction_SetCharCondition;
	s_actionMap_1["leave"] = ActionNodeCreator_CSPSNodeAction_Leave;
	s_actionMap_1["escort"] = ActionNodeCreator_CSPSNodeAction_Escort;
	s_actionMap_1["sps end"] = ActionNodeCreator_CSPSNodeAction_SPSEnd;
	s_actionMap_1["target fix attack"] = ActionNodeCreator_CSPSNodeAction_TargetFixAttack;
	s_actionMap_1["skill use"] = ActionNodeCreator_CSPSNodeAction_SkillUse;
	s_actionMap_1["ai bit flag"] = ActionNodeCreator_CSPSNodeAction_ChangeAiBitFlag;
	s_actionMap_1["timid attribute"] = ActionNodeCreator_CSPSNodeAction_ChangeTimidAttribute;
	s_actionMap_1["char attribute"] = ActionNodeCreator_CSPSNodeAction_ChangeCharAttribute;
	s_actionMap_1["npc attribute"] = ActionNodeCreator_CSPSNodeAction_ChangeNpcAttribute;
	s_actionMap_1["function"] = ActionNodeCreator_CSPSNodeAction_Function;
	s_actionMap_1["send sps event"] = ActionNodeCreator_CSPSNodeAction_SendSpsEvent;
	s_actionMap_1["send event to sps"] = ActionNodeCreator_CSPSNodeAction_SendEventToSps;
	s_actionMap_1["send escort event to ts"] = ActionNodeCreator_CSPSNodeAction_SendEscortEventToTs;
	s_actionMap_1["send event to tqs"] = ActionNodeCreator_CSPSNodeAction_SendEventToTqs;
	s_actionMap_1["send event to wps"] = ActionNodeCreator_CSPSNodeAction_SendEventToWps;
	s_actionMap_1["send event to ts"] = ActionNodeCreator_CSPSNodeAction_SendEventToTs;
	s_actionMap_1["send event"] = ActionNodeCreator_CSPSNodeAction_SendEvent;
	s_actionMap_1["clear event"] = ActionNodeCreator_CSPSNodeAction_ClearEvent;
	s_actionMap_1["calculate"] = ActionNodeCreator_CSPSNodeAction_Calculate;
	s_actionMap_1["do wander"] = ActionNodeCreator_CSPSNodeAction_DoWander;

	s_conditionMap_1["child"] = ConditionNodeCreator_CSPSNodeCondition_Child;
	s_conditionMap_1["recv event from ts"] = ConditionNodeCreator_CSPSNodeCondition_RecvEventFromTs;
	s_conditionMap_1["recv event from wps"] = ConditionNodeCreator_CSPSNodeCondition_RecvEventFromWps;
	s_conditionMap_1["recv event from item"] = ConditionNodeCreator_CSPSNodeCondition_RecvEventFromItem;
	s_conditionMap_1["register battle scene"] = ConditionNodeCreator_CSPSNodeCondition_RegisterBattleScene;
	s_conditionMap_1["condition"] = ConditionNodeCreator_CSPSNodeCondition_BSCondition;
	s_conditionMap_1["condition group"] = ConditionNodeCreator_CSPSNodeCondition_BSConditionGroup;
	s_conditionMap_1["flow"] = ConditionNodeCreator_CSPSNodeCondition_BSFlow;
	s_conditionMap_1["check lp"] = ConditionNodeCreator_CSPSNodeCondition_BSCheckLp;
	s_conditionMap_1["check mobgroup"] = ConditionNodeCreator_CSPSNodeCondition_BSCheckMobGroup;
	s_conditionMap_1["check npc"] = ConditionNodeCreator_CSPSNodeCondition_BSCheckNpc;
}





CSPSNodeFactory::CSPSNodeFactory()
{
}

CSPSNodeFactory::~CSPSNodeFactory()
{
}


CControlScriptNodeAction* CSPSNodeFactory::CreateControlNodeAction(const char* lpszNodeName)
{
	if (!s_bInitailize_1)
	{
		Initialize_1();
		s_bInitailize_1 = true;
	}

	auto it = s_actionMap_1.find(lpszNodeName);
	if (it == s_actionMap_1.end())
	{
		ERR_LOG(LOG_SCRIPT, "CSPSNodeFactory::CreateControlNodeAction: %s not found \n", lpszNodeName);
		return NULL;
	}

	return it->second(lpszNodeName);
}

CControlScriptNodeCondition* CSPSNodeFactory::CreateControlNodeCondition(const char* lpszNodeName)
{
	if (!s_bInitailize_1)
	{
		Initialize_1();
		s_bInitailize_1 = true;
	}

	auto it = s_conditionMap_1.find(lpszNodeName);
	if (it == s_conditionMap_1.end())
	{
		ERR_LOG(LOG_SCRIPT, "CSPSNodeFactory::CreateControlNodeCondition: %s not found \n", lpszNodeName);
	}

	return it->second(lpszNodeName);
}

