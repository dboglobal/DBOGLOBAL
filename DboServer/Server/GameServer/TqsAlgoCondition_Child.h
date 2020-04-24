#ifndef __TQS_ALGO_DBOG_CONDITION_CHILD_H__
#define __TQS_ALGO_DBOG_CONDITION_CHILD_H__

#include "TqsAlgoCondition_Base.h"


class CTqsAlgoCondition_Child : public CTqsAlgoCondition_Base
{

public:

	CTqsAlgoCondition_Child(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoCondition_Child();

public:

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);


};

#endif