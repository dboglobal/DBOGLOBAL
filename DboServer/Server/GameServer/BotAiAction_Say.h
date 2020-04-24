#ifndef __AI_DBOG_BOTACTION_SAY_H__
#define __AI_DBOG_BOTACTION_SAY_H__

#include "BotAiAction.h"
#include "NtlChat.h"
#include "NtlStringW.h"

class CBotAiAction_Say : public CBotAiAction
{

public:

	CBotAiAction_Say(CNpc* pBot);
	virtual	~CBotAiAction_Say();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eCHAR_DIALOG_TYPE m_byDialogType;
	CNtlStringW m_strText;
	TBLIDX m_textTblidx;
};

#endif