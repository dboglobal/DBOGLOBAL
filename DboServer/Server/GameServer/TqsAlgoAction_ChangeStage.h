#ifndef __TQS_ALGO_DBOG_ACTION_CHANGE_STAGE_H__
#define __TQS_ALGO_DBOG_ACTION_CHANGE_STAGE_H__

#include "TqsAlgoAction_Base.h"


class CTqsAlgoAction_ChangeStage : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_ChangeStage(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_ChangeStage();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	BYTE						m_byNextStage;

};

#endif