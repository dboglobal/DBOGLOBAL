#ifndef __AI_DBOG_BOTACTION_CHANGECHARATTRIBUTE_H__
#define __AI_DBOG_BOTACTION_CHANGECHARATTRIBUTE_H__

#include "BotAiAction.h"

class CBotAiAction_ChangeCharAttribute : public CBotAiAction
{

public:

	CBotAiAction_ChangeCharAttribute(CNpc* pBot);
	virtual	~CBotAiAction_ChangeCharAttribute();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

private:

	bool m_bUseNickname;
	TBLIDX m_nicknameTblidx;
	BYTE m_bySizeRate;
	BYTE m_byImmortalMode;

};

#endif