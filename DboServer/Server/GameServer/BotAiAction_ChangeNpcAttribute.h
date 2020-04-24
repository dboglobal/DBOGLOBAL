#ifndef __AI_DBOG_BOTACTION_CHANGENPCATTRIBUTE_H__
#define __AI_DBOG_BOTACTION_CHANGENPCATTRIBUTE_H__

#include "BotAiAction.h"

class CBotAiAction_ChangeNpcAttribute : public CBotAiAction
{

public:

	CBotAiAction_ChangeNpcAttribute(CNpc* pBot);
	virtual	~CBotAiAction_ChangeNpcAttribute();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

private:

	bool m_bApplyRelationPc;
	eRELATION_TYPE m_eRelationPc;

};

#endif