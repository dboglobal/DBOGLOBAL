#ifndef __AI_DBOG_BOTACTION_SENDSPSEVENT_H__
#define __AI_DBOG_BOTACTION_SENDSPSEVENT_H__

#include "BotAiAction.h"

class CBotAiAction_SendSpsEvent : public CBotAiAction
{

public:

	CBotAiAction_SendSpsEvent(CNpc* pBot);
	virtual	~CBotAiAction_SendSpsEvent();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eSPS_EVENT_TYPE m_eEventType;
	BYTE m_byTriggerType;
	DWORD m_eventID;

};

#endif