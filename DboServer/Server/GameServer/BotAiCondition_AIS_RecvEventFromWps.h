#ifndef __AI_DBOG_BOTCONDITION_AIS_RECEIVEEVENTFROMWPS_H__
#define __AI_DBOG_BOTCONDITION_AIS_RECEIVEEVENTFROMWPS_H__

#include "BotAiCondition.h"


class CBotAiCondition_AIS_RecvEventFromWps : public CBotAiCondition
{

public:
	CBotAiCondition_AIS_RecvEventFromWps(CNpc* pBot);
	virtual	~CBotAiCondition_AIS_RecvEventFromWps();


public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);
	virtual void CopyTo(CControlState* pTo);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg* pObjMsg);


private:

	TBLIDX m_wpsTblidx;
	DWORD m_eventId;
	DWORD m_scriptnodeID;

};

#endif