#ifndef __SCRIPT_ALGO_DBOG_ACTION_ELSE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_ELSE_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_Else : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_Else(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_Else();

public:

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif