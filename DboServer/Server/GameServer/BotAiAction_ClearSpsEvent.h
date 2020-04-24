#ifndef __AI_DBOG_BOTACTION_CLEARSPSEVENT_H__
#define __AI_DBOG_BOTACTION_CLEARSPSEVENT_H__

#include "BotAiAction.h"

class CBotAiAction_ClearSpsEvent : public CBotAiAction
{

public:

	CBotAiAction_ClearSpsEvent(CNpc* pBot);
	virtual	~CBotAiAction_ClearSpsEvent();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);
	virtual void CopyTo(CControlState* pTo);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool m_bClearAll;
	DWORD m_eventId;

};

#endif