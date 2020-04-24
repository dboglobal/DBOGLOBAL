#ifndef __SCRIPT_ALGO_DBOG_ACTION_CASE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_CASE_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_Case : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_Case(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_Case();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

	int					GetCompareValue() { return m_nCompareValue; }

private:

	int					m_nCompareValue;


};

#endif