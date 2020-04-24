#ifndef __SCRIPT_ALGO_DBOG_ACTION_TIME_COUNTDOWN_H__
#define __SCRIPT_ALGO_DBOG_ACTION_TIME_COUNTDOWN_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_TimeCountdown : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_TimeCountdown(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_TimeCountdown();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bCountdown;

};

#endif