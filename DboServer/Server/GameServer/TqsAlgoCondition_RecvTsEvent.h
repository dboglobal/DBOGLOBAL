#ifndef __TQS_ALGO_DBOG_CONDITION_RECV_TS_EVENT_H__
#define __TQS_ALGO_DBOG_CONDITION_RECV_TS_EVENT_H__

#include "TqsAlgoCondition_Base.h"


class CTqsAlgoCondition_RecvTsEvent : public CTqsAlgoCondition_Base
{

public:

	CTqsAlgoCondition_RecvTsEvent(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoCondition_RecvTsEvent();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	BYTE						m_byTriggerType;

	DWORD						m_teid;

	DWORD						m_dwWaitTime;

	bool						m_bTmqFail;

	bool						m_bSynchronize;

};

#endif