#ifndef __SCRIPT_ALGO_DBOG_CONDITION_CHILD_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_CHILD_H__

#include "ScriptAlgoCondition.h"


class CWpsAlgoCondition_Child : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_Child(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_Child();

public:

	//virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD				m_scriptnodeID;


};

#endif