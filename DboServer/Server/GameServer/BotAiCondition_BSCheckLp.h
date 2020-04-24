#ifndef __AI_DBOG_BOTCONDITION_BS_CHECKLP_H__
#define __AI_DBOG_BOTCONDITION_BS_CHECKLP_H__

#include "BotAiCondition.h"


class CBotAiCondition_BSCheckLp : public CBotAiCondition
{

public:
	CBotAiCondition_BSCheckLp(CNpc* pBot);
	virtual	~CBotAiCondition_BSCheckLp();


public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


private:

	BYTE m_byMaxPercent;
	BYTE m_byMinPercent;
	bool m_bIsInRange;

};

#endif