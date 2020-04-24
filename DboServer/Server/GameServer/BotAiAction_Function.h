#ifndef __AI_DBOG_BOTACTION_FUNCTION_H__
#define __AI_DBOG_BOTACTION_FUNCTION_H__

#include "BotAiAction.h"

class CBotAiAction_Function : public CBotAiAction
{

public:

	CBotAiAction_Function(CNpc* pBot);
	virtual	~CBotAiAction_Function();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif