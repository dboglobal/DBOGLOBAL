#ifndef __SCRIPT_ALGO_DBOG_ACTION_LOOP_H__
#define __SCRIPT_ALGO_DBOG_ACTION_LOOP_H__

#include "ScriptAlgoAction.h"

class CWpsAlgoAction_Loop : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_Loop(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_Loop();

public:

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);


};

#endif