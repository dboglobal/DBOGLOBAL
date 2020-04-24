#ifndef __TQS_ALGO_DBOG_ACTION_SEND_TS_EVENT_H__
#define __TQS_ALGO_DBOG_ACTION_SEND_TS_EVENT_H__

#include "TqsAlgoAction_Base.h"


class CTqsAlgoAction_SendTsEvent : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_SendTsEvent(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_SendTsEvent();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eTS_EVENT_SEND_TYPE			m_eSendType;

	BYTE						m_byTriggerType;

	DWORD						m_teid;

	CNtlVector					m_vOrigin;

	float						m_fEffectRadius;

};

#endif