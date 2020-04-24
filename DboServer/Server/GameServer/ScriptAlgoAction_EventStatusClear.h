#ifndef __SCRIPT_ALGO_DBOG_ACTION_EVENT_STATUS_CLEAR_H__
#define __SCRIPT_ALGO_DBOG_ACTION_EVENT_STATUS_CLEAR_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_EventStatusClear : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_EventStatusClear(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_EventStatusClear();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	BYTE						m_byGroup;

};

#endif