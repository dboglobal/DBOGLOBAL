#ifndef __AI_DBOG_BOTACTION_SKILLUSE_H__
#define __AI_DBOG_BOTACTION_SKILLUSE_H__

#include "BotAiAction.h"


class CBotAiAction_SkillUse : public CBotAiAction
{

public:

	CBotAiAction_SkillUse(CNpc* pBot);
	CBotAiAction_SkillUse(CNpc* pBot, BYTE bySkillIndex);
	virtual	~CBotAiAction_SkillUse();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);
	virtual void CopyTo(CControlState* pTo);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	BYTE m_bySkillIndex;

};

#endif