#ifndef __AI_DBOG_BOTACTION_LEAVE_H__
#define __AI_DBOG_BOTACTION_LEAVE_H__

#include "BotAiAction.h"

class CBotAiAction_Leave : public CBotAiAction
{

public:

	CBotAiAction_Leave(CNpc* pBot);
	virtual	~CBotAiAction_Leave();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif