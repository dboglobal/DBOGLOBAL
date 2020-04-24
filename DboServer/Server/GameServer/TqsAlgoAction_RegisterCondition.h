#ifndef __TQS_ALGO_DBOG_ACTION_REGISTER_CONDITION_H__
#define __TQS_ALGO_DBOG_ACTION_REGISTER_CONDITION_H__

#include "TqsAlgoAction_Base.h"


class CTqsAlgoAction_RegisterCondition : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_RegisterCondition(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_RegisterCondition();

public:

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);


};

#endif