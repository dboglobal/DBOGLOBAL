#ifndef __SCRIPT_ALGO_DBOG_CONDITION_CCBD_CHECK_RUN_STATE_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_CCBD_CHECK_RUN_STATE_H__

#include "ScriptAlgoCondition.h"


class CWpsScriptAlgoCondition_CCBD_check_run_state : public CWpsAlgoCondition
{

public:

	CWpsScriptAlgoCondition_CCBD_check_run_state(CWpsAlgoObject* pObject);
	virtual	~CWpsScriptAlgoCondition_CCBD_check_run_state();

public:

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif