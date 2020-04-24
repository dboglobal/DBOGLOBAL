#ifndef __SCRIPT_ALGO_DBOG_ACTION_CCBD_H__
#define __SCRIPT_ALGO_DBOG_ACTION_CCBD_H__

#include "ScriptAlgoAction.h"


class CWpsScriptAlgoAction_CCBD : public CScriptAlgoAction
{

public:

	CWpsScriptAlgoAction_CCBD(CWpsAlgoObject* pObject);
	virtual	~CWpsScriptAlgoAction_CCBD();

public:

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif