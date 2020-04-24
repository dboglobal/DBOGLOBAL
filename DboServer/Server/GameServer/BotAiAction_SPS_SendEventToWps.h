#ifndef __AI_DBOG_BOTACTION_SPSSENDEVENTTOWPS_H__
#define __AI_DBOG_BOTACTION_SPSSENDEVENTTOWPS_H__

#include "BotAiAction.h"

class CBotAiAction_SPS_SendEventToWps : public CBotAiAction
{

public:

	CBotAiAction_SPS_SendEventToWps(CNpc* pBot);
	virtual	~CBotAiAction_SPS_SendEventToWps();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX m_wpsTblidx;
	DWORD m_eventId;

};

#endif