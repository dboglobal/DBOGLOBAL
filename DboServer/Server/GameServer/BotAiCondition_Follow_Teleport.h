#ifndef __AI_DBOG_BOTCONDITION_FOLLOWTELEPORT_H__
#define __AI_DBOG_BOTCONDITION_FOLLOWTELEPORT_H__

#include "BotAiCondition.h"

class CNpcParty;

class CBotAiCondition_Follow_Teleport : public CBotAiCondition
{

public:
	CBotAiCondition_Follow_Teleport(CNpc* pBot, CNpcParty *pParty);
	virtual	~CBotAiCondition_Follow_Teleport();


public:

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	CNpcParty * m_pParty;

};

#endif