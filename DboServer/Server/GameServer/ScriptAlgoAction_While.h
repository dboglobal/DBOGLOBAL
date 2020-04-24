#ifndef __SCRIPT_ALGO_DBOG_ACTION_WHILE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_WHILE_H__

#include "ScriptAlgoAction.h"

class CWpsAlgoAction_While : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_While(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_While();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);


private:

	DWORD				m_scriptnodeID;

};

#endif