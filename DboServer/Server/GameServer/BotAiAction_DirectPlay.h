#ifndef __AI_DBOG_BOTACTION_DIRECTPLAY_H__
#define __AI_DBOG_BOTACTION_DIRECTPLAY_H__

#include "BotAiAction.h"

class CBotAiAction_DirectPlay : public CBotAiAction
{

public:

	CBotAiAction_DirectPlay(CNpc* pBot);
	virtual	~CBotAiAction_DirectPlay();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg* pObjMsg);

private:

	TBLIDX m_directionTblidx;
	BYTE m_byDirectPlayType;
	DWORD m_dwPlayTime;
	DWORD m_dwTime;
	eSPS_EVENT_TYPE m_eEventType;
	DWORD m_eventID;
};

#endif