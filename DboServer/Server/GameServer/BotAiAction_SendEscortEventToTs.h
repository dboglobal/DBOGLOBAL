#ifndef __AI_DBOG_BOTACTION_SENDESCORTEVENTTOTS_H__
#define __AI_DBOG_BOTACTION_SENDESCORTEVENTTOTS_H__

#include "BotAiAction.h"

class CBotAiAction_SendEscortEventToTs : public CBotAiAction
{

public:

	CBotAiAction_SendEscortEventToTs(CNpc* pBot);
	virtual	~CBotAiAction_SendEscortEventToTs();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	int m_teId;

};

#endif