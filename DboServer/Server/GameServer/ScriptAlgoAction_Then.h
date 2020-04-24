#ifndef __SCRIPT_ALGO_DBOG_ACTION_THEN_H__
#define __SCRIPT_ALGO_DBOG_ACTION_THEN_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_Then : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_Then(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_Then();

public:

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif