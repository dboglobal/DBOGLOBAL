#ifndef __TQS_ALGO_DBOG_CONDITION_RECV_SPS_EVENT_H__
#define __TQS_ALGO_DBOG_CONDITION_RECV_SPS_EVENT_H__

#include "TqsAlgoCondition_Base.h"


class CTqsAlgoCondition_RecvSpsEvent : public CTqsAlgoCondition_Base
{

public:

	CTqsAlgoCondition_RecvSpsEvent(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoCondition_RecvSpsEvent();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_targetTblidx;

	DWORD						m_eventID;

	DWORD						m_dwTime;

};

#endif