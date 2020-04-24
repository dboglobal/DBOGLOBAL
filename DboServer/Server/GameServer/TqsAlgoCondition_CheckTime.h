#ifndef __TQS_ALGO_DBOG_CONDITION_CHECKTIME_H__
#define __TQS_ALGO_DBOG_CONDITION_CHECKTIME_H__

#include "TqsAlgoCondition_Base.h"

class CTqsAlgoCondition_CheckTime : public CTqsAlgoCondition_Base
{

public:

	CTqsAlgoCondition_CheckTime(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoCondition_CheckTime();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD				m_dwWaitTime;

};

#endif