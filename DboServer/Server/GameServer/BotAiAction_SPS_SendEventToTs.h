#ifndef __AI_DBOG_BOTACTION_SPSSENDEVENTTOTS_H__
#define __AI_DBOG_BOTACTION_SPSSENDEVENTTOTS_H__

#include "BotAiAction.h"
#include "SPSNodeAction_SendEventToTs.h"

class CBotAiAction_SPS_SendEventToTs : public CBotAiAction
{

public:

	CBotAiAction_SPS_SendEventToTs(CNpc* pBot);
	virtual	~CBotAiAction_SPS_SendEventToTs();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	CSPSNodeAction_SendEventToTs::eTRIGGER_TYPE m_eTriggerType;
	DWORD m_eventId;

};

#endif