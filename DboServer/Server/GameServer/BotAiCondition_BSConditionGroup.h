#ifndef __AI_DBOG_BOTCONDITION_BS_CONDITIONGROUP_H__
#define __AI_DBOG_BOTCONDITION_BS_CONDITIONGROUP_H__


#include "BotAiBattleSceneCondition.h"


class CBotAiCondition_BSConditionGroup : public CBotAiBattleSceneCondition
{

public:
	CBotAiCondition_BSConditionGroup(CNpc* pBot);
	virtual	~CBotAiCondition_BSConditionGroup();


public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif