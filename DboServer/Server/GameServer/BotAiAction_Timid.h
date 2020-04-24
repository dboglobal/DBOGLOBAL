#ifndef __AI_DBOG_BOTACTION_TIMID_H__
#define __AI_DBOG_BOTACTION_TIMID_H__

#include "BotAiAction.h"


class CBotAiAction_Timid : public CBotAiAction
{

public:

	CBotAiAction_Timid(CNpc* pBot, CNtlVector& vDestLoc);
	virtual	~CBotAiAction_Timid();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	CNtlVector m_vDestLoc;

};

#endif