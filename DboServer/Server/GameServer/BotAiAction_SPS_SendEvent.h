#ifndef __AI_DBOG_BOTACTION_SPSSENDEVENT_H__
#define __AI_DBOG_BOTACTION_SPSSENDEVENT_H__

#include "BotAiAction.h"
#include "SPSNodeAction_SendEvent.h"

class CBotAiAction_SPS_SendEvent : public CBotAiAction
{

public:

	CBotAiAction_SPS_SendEvent(CNpc* pBot);
	virtual	~CBotAiAction_SPS_SendEvent();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	CSPSNodeAction_SendEvent::eEVENT_TYPE m_eEventType;

};

#endif