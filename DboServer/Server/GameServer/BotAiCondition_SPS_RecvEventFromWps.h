#ifndef __AI_DBOG_BOTCONDITION_SPS_RECEIVEEVENTFROMWPS_H__
#define __AI_DBOG_BOTCONDITION_SPS_RECEIVEEVENTFROMWPS_H__

#include "BotAiCondition.h"


class CBotAiCondition_SPS_RecvEventFromWps : public CBotAiCondition
{

public:
	CBotAiCondition_SPS_RecvEventFromWps(CNpc* pBot);
	virtual	~CBotAiCondition_SPS_RecvEventFromWps();


public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg* pObjMsg);


private:

	bool m_bCreated;
	TBLIDX m_wpsTblidx;
	DWORD m_eventId;

};

#endif