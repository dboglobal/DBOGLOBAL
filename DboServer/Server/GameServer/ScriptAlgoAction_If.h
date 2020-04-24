#ifndef __SCRIPT_ALGO_DBOG_ACTION_IF_H__
#define __SCRIPT_ALGO_DBOG_ACTION_IF_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_If : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_If(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_If();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bOperationAND;

	bool						m_bReverse;

};

#endif