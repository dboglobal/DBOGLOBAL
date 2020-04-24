#ifndef __SCRIPT_ALGO_DBOG_CONDITION_LOOPCOND_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_LOOPCOND_H__

#include "ScriptAlgoCondition.h"

class CWpsAlgoCondition_LoopCond : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_LoopCond(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_LoopCond();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bOperationAND;

	bool						m_bReverse;


};

#endif