#ifndef __SCRIPT_ALGO_DBOG_ACTION_EXEC_AT_ONCE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_EXEC_AT_ONCE_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_ExecAtOnce : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_ExecAtOnce(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_ExecAtOnce();

public:

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif