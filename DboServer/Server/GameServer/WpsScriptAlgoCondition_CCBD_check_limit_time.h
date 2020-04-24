#ifndef __SCRIPT_ALGO_DBOG_CONDITION_CCBD_CHECK_LIMIT_TIME_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_CCBD_CHECK_LIMIT_TIME_H__

#include "ScriptAlgoCondition.h"


class CWpsScriptAlgoCondition_CCBD_check_limit_time : public CWpsAlgoCondition
{

public:

	CWpsScriptAlgoCondition_CCBD_check_limit_time(CWpsAlgoObject* pObject);
	virtual	~CWpsScriptAlgoCondition_CCBD_check_limit_time();

public:

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif