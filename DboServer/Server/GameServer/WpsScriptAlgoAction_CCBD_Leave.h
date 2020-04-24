#ifndef __SCRIPT_ALGO_DBOG_ACTION_CCBD_LEAVE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_CCBD_LEAVE_H__

#include "ScriptAlgoAction.h"


class CWpsScriptAlgoAction_CCBD_Leave : public CScriptAlgoAction
{

public:

	CWpsScriptAlgoAction_CCBD_Leave(CWpsAlgoObject* pObject);
	virtual	~CWpsScriptAlgoAction_CCBD_Leave();

public:

	virtual void				OnEnter();
	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif