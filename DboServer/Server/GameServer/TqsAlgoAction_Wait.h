#ifndef __TQS_ALGO_DBOG_ACTION_WAIT_H__
#define __TQS_ALGO_DBOG_ACTION_WAIT_H__

#include "TqsAlgoAction_Base.h"

class CTqsAlgoAction_Wait : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_Wait(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_Wait();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void		OnEnter();

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	int					m_nCondCount;

	bool				m_bOperationAND;

};

#endif