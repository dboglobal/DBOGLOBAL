#ifndef __AI_DBOG_BOTACTION_TERROR_H__
#define __AI_DBOG_BOTACTION_TERROR_H__

#include "BotAiAction.h"


class CBotAiAction_Terror : public CBotAiAction
{

public:

	CBotAiAction_Terror(CNpc* pBot);
	virtual	~CBotAiAction_Terror();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

protected:

	bool GetNextDest(CNtlVector *pNextDir, CNtlVector *pNextDest);

};

#endif