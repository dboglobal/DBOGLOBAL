#ifndef __AI_DBOG_BOTCONDITION_ALLIANCEHELPME_H__
#define __AI_DBOG_BOTCONDITION_ALLIANCEHELPME_H__

#include "BotAiCondition.h"


class CBotAiCondition_AllianceHelpMe : public CBotAiCondition
{

public:
	CBotAiCondition_AllianceHelpMe(CNpc* pBot);
	virtual	~CBotAiCondition_AllianceHelpMe();


public:

	virtual void OnEnter();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif