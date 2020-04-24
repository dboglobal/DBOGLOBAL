#ifndef __AI_DBOG_BOTCONDITION_BS_FLOW_H__
#define __AI_DBOG_BOTCONDITION_BS_FLOW_H__

#include "BotAiCondition.h"


class CBotAiCondition_BSFlow : public CBotAiCondition
{

public:
	CBotAiCondition_BSFlow(CNpc* pBot);
	virtual	~CBotAiCondition_BSFlow();


public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif