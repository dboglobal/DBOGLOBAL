#ifndef __TQS_ALGO_DBOG_ACTION_CHANGEOBJECTSTATE_H__
#define __TQS_ALGO_DBOG_ACTION_CHANGEOBJECTSTATE_H__

#include "TqsAlgoAction_Base.h"


class CTqsAlgoAction_ChangeObjectState : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_ChangeObjectState(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_ChangeObjectState();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_objectTblidx;

	BYTE						m_mainState;

	BYTE						m_subStateSet;

	BYTE						m_subStateUnset;

};

#endif