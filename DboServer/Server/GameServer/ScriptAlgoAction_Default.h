#ifndef __SCRIPT_ALGO_DBOG_ACTION_DEFAULT_H__
#define __SCRIPT_ALGO_DBOG_ACTION_DEFAULT_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_Default : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_Default(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_Default();

public:

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif