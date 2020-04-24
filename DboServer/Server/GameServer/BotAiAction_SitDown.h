#ifndef __AI_DBOG_BOTACTION_SITDOWN_H__
#define __AI_DBOG_BOTACTION_SITDOWN_H__

#include "BotAiAction.h"


class CBotAiAction_SitDown : public CBotAiAction
{

public:

	CBotAiAction_SitDown(CNpc* pBot);
	virtual	~CBotAiAction_SitDown();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool m_bSitDown;

};

#endif