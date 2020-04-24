#ifndef __SCRIPT_ALGO_DBOG_ACTION_EVENTHANDLER_H__
#define __SCRIPT_ALGO_DBOG_ACTION_EVENTHANDLER_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_EventHandler : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_EventHandler(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_EventHandler();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eSSD_EVENT_TYPE				m_eEventType;

	TBLIDX						m_tblidx;

};

#endif