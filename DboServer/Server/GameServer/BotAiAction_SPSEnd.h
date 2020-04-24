#ifndef __AI_DBOG_BOTACTION_SPSEND_H__
#define __AI_DBOG_BOTACTION_SPSEND_H__

#include "BotAiAction.h"


class CBotAiAction_SPSEnd : public CBotAiAction
{

public:

	CBotAiAction_SPSEnd(CNpc* pBot);
	virtual	~CBotAiAction_SPSEnd();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


};

#endif