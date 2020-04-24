#ifndef __SCRIPT_ALGO_DBOG_ACTION_SEND_EVENT_TO_WPS_H__
#define __SCRIPT_ALGO_DBOG_ACTION_SEND_EVENT_TO_WPS_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_SendEventToWps : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_SendEventToWps(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_SendEventToWps();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_wpsTblidx;

	DWORD						m_eventId;

};

#endif