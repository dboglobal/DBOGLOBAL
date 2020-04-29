#include "stdafx.h"
#include "AISNodeFactory.h"

#include "AISNodeState_Comeback.h"
#include "AISNodeState_Fight.h"
#include "AISNodeState_Idle.h"
#include "AISNodeState_NonBattle.h"
#include "AISNodeState_Spawn.h"
#include "AISNodeState_Teleporting.h"
#include "AISNodeState_Wander.h"
#include "AISNodeAction_AddMob.h"
#include "AISNodeAction_ChangeAiBitFlag.h"
#include "AISNodeAction_PointMove.h"
#include "AISNodeAction_Say.h"
#include "AISNodeAction_SendEventToTQS.h"
#include "AISNodeAction_SendEventToWPS.h"
#include "AISNodeAction_SetCharCondition.h"
#include "AISNodeAction_TargetFixAttack.h"
#include "AISNodeAction_UseSkill.h"
#include "AISNodeCondition_HimselfCheck.h"
#include "AISNodeCondition_RecvEventFromTQS.h"
#include "AISNodeCondition_RecvEventFromWPS.h"
#include "AISNodeCondition_Scan.h"

#include <unordered_map>


std::unordered_map<std::string, CControlScriptNodeAction*(*)(const char*)> s_actionMap_0;
std::unordered_map<std::string, CControlScriptNodeCondition*(*)(const char*)> s_conditionMap_0;

bool s_bInitailize_0 = false;


CControlScriptNodeAction* ActionNodeCreator_CAISNodeState_Spawn(const char* lpszNodeName)
{
	return new CAISNodeState_Spawn(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeState_Idle(const char* lpszNodeName)
{
	return new CAISNodeState_Idle(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeState_Fight(const char* lpszNodeName)
{
	return new CAISNodeState_Fight(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeState_NonBattle(const char* lpszNodeName)
{
	return new CAISNodeState_NonBattle(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeState_Teleporting(const char* lpszNodeName)
{
	return new CAISNodeState_Teleporting(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeState_Comeback(const char* lpszNodeName)
{
	return new CAISNodeState_Comeback(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeState_Wander(const char* lpszNodeName)
{
	return new CAISNodeState_Wander(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeAction_AddMob(const char* lpszNodeName)
{
	return new CAISNodeAction_AddMob(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeAction_Say(const char* lpszNodeName)
{
	return new CAISNodeAction_Say(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeAction_PointMove(const char* lpszNodeName)
{
	return new CAISNodeAction_PointMove(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeAction_SetCharCondition(const char* lpszNodeName)
{
	return new CAISNodeAction_SetCharCondition(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeAction_TargetFixAttack(const char* lpszNodeName)
{
	return new CAISNodeAction_TargetFixAttack(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeAction_UseSkill(const char* lpszNodeName)
{
	return new CAISNodeAction_UseSkill(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeAction_ChangeAiBitFlag(const char* lpszNodeName)
{
	return new CAISNodeAction_ChangeAiBitFlag(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeAction_SendEventToWPS(const char* lpszNodeName)
{
	return new CAISNodeAction_SendEventToWPS(lpszNodeName);
}
CControlScriptNodeAction* ActionNodeCreator_CAISNodeAction_SendEventToTQS(const char* lpszNodeName)
{
	return new CAISNodeAction_SendEventToTQS(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CAISNodeCondition_HimselfCheck(const char* lpszNodeName)
{
	return new CAISNodeCondition_HimselfCheck(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CAISNodeCondition_Scan(const char* lpszNodeName)
{
	return new CAISNodeCondition_Scan(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CAISNodeCondition_RecvEventFromWPS(const char* lpszNodeName)
{
	return new CAISNodeCondition_RecvEventFromWPS(lpszNodeName);
}
CControlScriptNodeCondition* ConditionNodeCreator_CAISNodeCondition_RecvEventFromTQS(const char* lpszNodeName)
{
	return new CAISNodeCondition_RecvEventFromTQS(lpszNodeName);
}


inline void Initialize_0()
{
	//---- State
	s_actionMap_0["spawn"] = ActionNodeCreator_CAISNodeState_Spawn;
	s_actionMap_0["idle"] = ActionNodeCreator_CAISNodeState_Idle;
	s_actionMap_0["fight"] = ActionNodeCreator_CAISNodeState_Fight;

	s_actionMap_0["teleporting"] = ActionNodeCreator_CAISNodeState_Teleporting;

	s_actionMap_0["come back"] = ActionNodeCreator_CAISNodeState_Comeback;
	s_actionMap_0["wander"] = ActionNodeCreator_CAISNodeState_Wander;

	//---- Action
	s_actionMap_0["add mob"] = ActionNodeCreator_CAISNodeAction_AddMob;
	s_actionMap_0["say"] = ActionNodeCreator_CAISNodeAction_Say;
	s_actionMap_0["point move"] = ActionNodeCreator_CAISNodeAction_PointMove;

	s_actionMap_0["set condition"] = ActionNodeCreator_CAISNodeAction_SetCharCondition;
	s_actionMap_0["target fix attack"] = ActionNodeCreator_CAISNodeAction_TargetFixAttack;
	s_actionMap_0["use skill"] = ActionNodeCreator_CAISNodeAction_UseSkill;
	s_actionMap_0["change ai bitflag"] = ActionNodeCreator_CAISNodeAction_ChangeAiBitFlag;
	s_actionMap_0["send event to tqs"] = ActionNodeCreator_CAISNodeAction_SendEventToTQS;
	s_actionMap_0["send event to wps"] = ActionNodeCreator_CAISNodeAction_SendEventToWPS;

	//---- Condition
	s_conditionMap_0["himself check"] = ConditionNodeCreator_CAISNodeCondition_HimselfCheck;
	s_conditionMap_0["scan"] = ConditionNodeCreator_CAISNodeCondition_Scan;
	s_conditionMap_0["recv event from wps"] = ConditionNodeCreator_CAISNodeCondition_RecvEventFromWPS;
	s_conditionMap_0["recv event from tqs"] = ConditionNodeCreator_CAISNodeCondition_RecvEventFromTQS;
}




CAISNodeFactory::CAISNodeFactory()
{
}

CAISNodeFactory::~CAISNodeFactory()
{

}

CControlScriptNodeAction* CAISNodeFactory::CreateControlNodeAction(const char* lpszNodeName)
{
	if (!s_bInitailize_0)
	{
		Initialize_0();
		s_bInitailize_0 = true;
	}

	auto it = s_actionMap_0.find(lpszNodeName);
	if (it == s_actionMap_0.end())
	{
		printf("CAISNodeFactory::CreateControlNodeAction: %s not found \n", lpszNodeName);
		return NULL;
	}

	return it->second(lpszNodeName);
}

CControlScriptNodeCondition* CAISNodeFactory::CreateControlNodeCondition(const char* lpszNodeName)
{
	if (!s_bInitailize_0)
	{
		Initialize_0();
		s_bInitailize_0 = true;
	}

	auto it = s_conditionMap_0.find(lpszNodeName);
	if (it == s_conditionMap_0.end())
	{
		printf("CAISNodeFactory::CreateControlNodeCondition: %s not found \n", lpszNodeName);
	}

	return it->second(lpszNodeName);
}

