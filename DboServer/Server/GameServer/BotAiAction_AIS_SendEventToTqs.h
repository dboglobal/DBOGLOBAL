#ifndef __AI_DBOG_BOTACTION_AISSENDEVENTTOTQS_H__
#define __AI_DBOG_BOTACTION_AISSENDEVENTTOTQS_H__

#include "BotAiAction.h"

class CBotAiAction_AIS_SendEventToTqs : public CBotAiAction
{

public:

	CBotAiAction_AIS_SendEventToTqs(CNpc* pBot);
	virtual	~CBotAiAction_AIS_SendEventToTqs();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);
	virtual void CopyTo(CControlState* pTo);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD m_eventId;

};

#endif