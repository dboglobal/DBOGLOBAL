#ifndef __SCRIPT_ALGO_DBOG_CONDITION_RECV_TS_EVENT_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_RECV_TS_EVENT_H__

#include "ScriptAlgoCondition.h"


class CWpsAlgoCondition_RecvTsEvent : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_RecvTsEvent(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_RecvTsEvent();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	BYTE						m_byTriggerType;

	unsigned int				m_teid;

	DWORD						m_dwWaitTime;

	bool						m_bSynchronize;

};

#endif