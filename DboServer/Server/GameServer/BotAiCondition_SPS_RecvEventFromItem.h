#ifndef __AI_DBOG_BOTCONDITION_SPS_RECEIVEEVENTFROMITEM_H__
#define __AI_DBOG_BOTCONDITION_SPS_RECEIVEEVENTFROMITEM_H__

#include "BotAiCondition.h"


class CBotAiCondition_SPS_RecvEventFromItem : public CBotAiCondition
{

public:
	CBotAiCondition_SPS_RecvEventFromItem(CNpc* pBot);
	virtual	~CBotAiCondition_SPS_RecvEventFromItem();


public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg* pObjMsg);


private:

	TBLIDX m_itemTblidx;

};

#endif