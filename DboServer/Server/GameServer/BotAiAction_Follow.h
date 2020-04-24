#ifndef __AI_DBOG_BOTACTION_FOLLOW_H__
#define __AI_DBOG_BOTACTION_FOLLOW_H__

#include "BotAiAction.h"

class CNpcParty;
class CNpc;

class CBotAiAction_Follow : public CBotAiAction
{

public:

	CBotAiAction_Follow(CNpc* pBot, CNpcParty* pParty);
	virtual	~CBotAiAction_Follow();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	int		MarineType(CNpc * pLeader);
	int		FormationType(CNpc * pLeader);

private:

	CNpcParty*	m_pParty;
	bool		m_bIsAddConditionWander;

};

#endif