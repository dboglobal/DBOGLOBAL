#ifndef __AI_DBOG_BOTCONDITION_SPS_RECEIVEEVENTFROMTS_H__
#define __AI_DBOG_BOTCONDITION_SPS_RECEIVEEVENTFROMTS_H__

#include "BotAiCondition.h"


class CBotAiCondition_SPS_RecvEventFromTs : public CBotAiCondition
{

public:
	CBotAiCondition_SPS_RecvEventFromTs(CNpc* pBot);
	virtual	~CBotAiCondition_SPS_RecvEventFromTs();


public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg* pObjMsg);


private:

	bool m_bCreated;
	BYTE m_byTriggerType;
	int m_teId;

};

#endif