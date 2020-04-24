#ifndef __SCRIPT_ALGO_DBOG_ACTION_EXEC_WPS_H__
#define __SCRIPT_ALGO_DBOG_ACTION_EXEC_WPS_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_ExecWps : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_ExecWps(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_ExecWps();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_wpsTblidx;

};

#endif