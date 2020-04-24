#ifndef __SCRIPT_ALGO_DBOG_ACTION_CHANGEOBJECTSTATE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_CHANGEOBJECTSTATE_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_ChangeObjectState : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_ChangeObjectState(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_ChangeObjectState();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_objectTblidx;

	BYTE						m_mainState;

	BYTE						m_subStateSet;

	BYTE						m_subStateUnset;

};

#endif