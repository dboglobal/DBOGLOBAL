#ifndef __AI_DBOG_BOTACTION_ROTATE_H__
#define __AI_DBOG_BOTACTION_ROTATE_H__

#include "BotAiAction.h"


class CBotAiAction_Rotate : public CBotAiAction
{

public:

	CBotAiAction_Rotate(CNpc* pBot, CNtlVector& rRotateDir);
	CBotAiAction_Rotate(CNpc* pBot);
	virtual	~CBotAiAction_Rotate();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	CNtlVector m_vRotateDir;

};

#endif