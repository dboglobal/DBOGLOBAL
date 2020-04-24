#ifndef __AI_DBOG_BOTACTION_DIRECTTURNING_H__
#define __AI_DBOG_BOTACTION_DIRECTTURNING_H__

#include "BotAiAction.h"

class CBotAiAction_DirectTurning : public CBotAiAction
{

public:

	CBotAiAction_DirectTurning(CNpc* pBot);
	virtual	~CBotAiAction_DirectTurning();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg* pObjMsg);

private:

	TBLIDX m_directionTblidx;
};

#endif