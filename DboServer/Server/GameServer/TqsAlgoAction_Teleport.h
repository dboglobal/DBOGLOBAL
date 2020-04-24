#ifndef __TQS_ALGO_DBOG_ACTION_TELEPORT_H__
#define __TQS_ALGO_DBOG_ACTION_TELEPORT_H__

#include "TqsAlgoAction_Base.h"


class CTqsAlgoAction_Teleport : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_Teleport(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_Teleport();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void		OnEnter();

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	CNtlVector					m_vLoc;
	CNtlVector					m_vDir;

};

#endif