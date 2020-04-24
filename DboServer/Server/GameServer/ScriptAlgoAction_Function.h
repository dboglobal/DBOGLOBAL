#ifndef __SCRIPT_ALGO_DBOG_ACTION_FUNCTION_H__
#define __SCRIPT_ALGO_DBOG_ACTION_FUNCTION_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_Function : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_Function(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_Function();

public:

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif