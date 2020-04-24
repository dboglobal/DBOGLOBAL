#ifndef __AI_DBOG_BOTACTION_SETCHARCONDITION_H__
#define __AI_DBOG_BOTACTION_SETCHARCONDITION_H__

#include "BotAiAction.h"


class CBotAiAction_SetCharCondition : public CBotAiAction
{

public:

	CBotAiAction_SetCharCondition(CNpc* pBot);
	virtual	~CBotAiAction_SetCharCondition();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);
	virtual void CopyTo(CControlState* pTo);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	QWORD m_qwAddCharConditionFlag;
	QWORD m_qwRemoveCharConditionFlag;

};

#endif