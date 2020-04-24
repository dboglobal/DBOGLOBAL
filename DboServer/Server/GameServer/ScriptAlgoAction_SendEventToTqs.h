#ifndef __SCRIPT_ALGO_DBOG_ACTION_SEND_EVENT_TO_TQS_H__
#define __SCRIPT_ALGO_DBOG_ACTION_SEND_EVENT_TO_TQS_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_SendEventToTqs : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_SendEventToTqs(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_SendEventToTqs();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD						m_eventId;

};

#endif