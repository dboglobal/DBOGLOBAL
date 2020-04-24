#ifndef __SCRIPT_ALGO_DBOG_ACTION_EVENT_STATUS_SELECT_H__
#define __SCRIPT_ALGO_DBOG_ACTION_EVENT_STATUS_SELECT_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_EventStatusSelect : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_EventStatusSelect(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_EventStatusSelect();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	BYTE						m_byGroup;

	BYTE						m_byIndex;

};

#endif