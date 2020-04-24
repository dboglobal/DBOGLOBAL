#ifndef __SCRIPT_ALGO_DBOG_CONDITION_RECVEVENTFROMTQS_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_RECVEVENTFROMTQS_H__

#include "ScriptAlgoCondition.h"


class CWpsAlgoCondition_RecvEventFromTqs : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_RecvEventFromTqs(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_RecvEventFromTqs();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD						m_eventId;

};

#endif