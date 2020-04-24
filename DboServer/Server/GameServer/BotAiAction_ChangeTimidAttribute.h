#ifndef __AI_DBOG_BOTACTION_CHANGETIMIDATTRIBUTE_H__
#define __AI_DBOG_BOTACTION_CHANGETIMIDATTRIBUTE_H__

#include "BotAiAction.h"

class CBotAiAction_ChangeTimidAttribute : public CBotAiAction
{

public:

	CBotAiAction_ChangeTimidAttribute(CNpc* pBot);
	virtual	~CBotAiAction_ChangeTimidAttribute();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

private:

	eTIMID_TYPE m_eTimidType;
	float m_fRange;

};

#endif