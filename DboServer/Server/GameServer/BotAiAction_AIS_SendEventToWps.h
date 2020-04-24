#ifndef __AI_DBOG_BOTACTION_AISSENDEVENTTOWPS_H__
#define __AI_DBOG_BOTACTION_AISSENDEVENTTOWPS_H__

#include "BotAiAction.h"

class CBotAiAction_AIS_SendEventToWps : public CBotAiAction
{

public:

	CBotAiAction_AIS_SendEventToWps(CNpc* pBot);
	virtual	~CBotAiAction_AIS_SendEventToWps();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);
	virtual void CopyTo(CControlState* pTo);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX m_wpsTblidx;
	DWORD m_eventId;

};

#endif