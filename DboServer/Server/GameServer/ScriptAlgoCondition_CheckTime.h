#ifndef __SCRIPT_ALGO_DBOG_CONDITION_CHECKTIME_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_CHECKTIME_H__

#include "ScriptAlgoCondition.h"

class CWpsAlgoCondition_CheckTime : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_CheckTime(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_CheckTime();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD						m_dwWaitTime;

};

#endif