#ifndef __TQS_ALGO_DBOG_ACTION_TIME_COUNTDOWN_H__
#define __TQS_ALGO_DBOG_ACTION_TIME_COUNTDOWN_H__

#include "TqsAlgoAction_Base.h"


class CTqsAlgoAction_TimeCountdown : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_TimeCountdown(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_TimeCountdown();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bCountdown;

};

#endif