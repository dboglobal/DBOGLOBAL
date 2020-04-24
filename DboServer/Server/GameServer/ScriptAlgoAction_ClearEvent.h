#ifndef __SCRIPT_ALGO_DBOG_ACTION_CLEAREVENT_H__
#define __SCRIPT_ALGO_DBOG_ACTION_CLEAREVENT_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_ClearEvent : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_ClearEvent(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_ClearEvent();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bClearAll;

	DWORD						m_eventId;

};

#endif