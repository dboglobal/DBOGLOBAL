#ifndef __TQS_ALGO_DBOG_ACTION_INFORMATION_H__
#define __TQS_ALGO_DBOG_ACTION_INFORMATION_H__

#include "TqsAlgoAction_Base.h"

class CTqsAlgoAction_Information : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_Information(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_Information();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eTIMEQUEST_INFORMATION_TYPE	m_eInfoType;

};

#endif