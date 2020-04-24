#ifndef __AI_DBOG_BOTACTION_FLEE_H__
#define __AI_DBOG_BOTACTION_FLEE_H__

#include "BotAiAction.h"

class CBotAiAction_Flee : public CBotAiAction
{

public:

	CBotAiAction_Flee(CNpc* pBot, CNtlVector& vFleeLoc);
	virtual	~CBotAiAction_Flee();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


private:

	CNtlVector m_vFleeLoc;

};

#endif