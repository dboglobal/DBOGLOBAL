#ifndef __AI_DBOG_BOTCONDITION_PETFOLLOWTELEPORT_H__
#define __AI_DBOG_BOTCONDITION_PETFOLLOWTELEPORT_H__

#include "BotAiCondition.h"


class CBotAiCondition_PetFollow_Teleport : public CBotAiCondition
{

public:
	CBotAiCondition_PetFollow_Teleport(CNpc* pBot);
	virtual	~CBotAiCondition_PetFollow_Teleport();


public:

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


};

#endif