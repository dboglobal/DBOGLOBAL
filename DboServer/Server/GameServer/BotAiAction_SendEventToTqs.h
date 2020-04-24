#ifndef __AI_DBOG_BOTACTION_SENDEVENTTOTQS_H__
#define __AI_DBOG_BOTACTION_SENDEVENTTOTQS_H__

#include "BotAiAction.h"

class CBotAiAction_SendEventToTqs : public CBotAiAction
{

public:

	CBotAiAction_SendEventToTqs(CNpc* pBot);
	virtual	~CBotAiAction_SendEventToTqs();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD m_eventId;

};

#endif