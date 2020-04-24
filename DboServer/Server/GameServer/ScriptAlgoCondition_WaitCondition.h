#ifndef __SCRIPT_ALGO_DBOG_CONDITION_WAIT_CONDITION_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_WAIT_CONDITION_H__

#include "ScriptAlgoCondition.h"


class CWpsAlgoCondition_WaitCondition : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_WaitCondition(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_WaitCondition();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void				OnEnter();

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bOperationAND;

	int							m_nCondCount;

	bool						m_bSetCondCount;

};

#endif