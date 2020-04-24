#ifndef __SCRIPT_ALGO_DBOG_ACTION_WPSFAIL_H__
#define __SCRIPT_ALGO_DBOG_ACTION_WPSFAIL_H__

#include "ScriptAlgoAction.h"

class CWpsAlgoAction_WpsFail : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_WpsFail(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_WpsFail();

public:

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif