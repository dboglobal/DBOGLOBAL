#ifndef __SCRIPT_ALGO_DBOG_CONDITION_CHECK_COUNTDOWN_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_CHECK_COUNTDOWN_H__

#include "ScriptAlgoCondition.h"


class CWpsAlgoCondition_CheckCountDown : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_CheckCountDown(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_CheckCountDown();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD						m_dwWaitTime;

};

#endif