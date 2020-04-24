#ifndef __AI_DBOG_BOTACTION_WAIT_H__
#define __AI_DBOG_BOTACTION_WAIT_H__

#include "BotAiAction.h"


class CBotAiAction_Wait : public CBotAiAction
{

public:

	CBotAiAction_Wait(CNpc* pBot);
	virtual	~CBotAiAction_Wait();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg* pObjMsg);

private:

	DWORD m_dwTime;
	DWORD m_dwWaitTime;
	eSPS_EVENT_TYPE m_eEventType;
	DWORD m_eventID;
	bool m_bSetCondCount;
	bool m_bOperationAND;
	int m_nCondCount;

};

#endif