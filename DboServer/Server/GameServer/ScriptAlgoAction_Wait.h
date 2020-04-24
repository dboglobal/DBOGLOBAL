#ifndef __SCRIPT_ALGO_DBOG_ACTION_WAIT_H__
#define __SCRIPT_ALGO_DBOG_ACTION_WAIT_H__

#include "ScriptAlgoAction.h"

class CWpsAlgoAction_Wait : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_Wait(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_Wait();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void		OnEnter();

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	int					m_nCondCount;

	bool				m_bOperationAND;

};

#endif