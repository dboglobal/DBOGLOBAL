#ifndef __TQS_ALGO_DBOG_ACTION_REWARD_H__
#define __TQS_ALGO_DBOG_ACTION_REWARD_H__

#include "TqsAlgoAction_Base.h"


class CTqsAlgoAction_Reward : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_Reward(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_Reward();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void				OnEnter();

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	BYTE						m_byTriggerType;

	WORD						m_teid;

	DWORD						m_dwWaitTime;

};

#endif