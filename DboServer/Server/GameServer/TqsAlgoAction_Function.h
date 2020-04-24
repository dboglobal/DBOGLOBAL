#ifndef __TQS_ALGO_DBOG_ACTION_FUNCTION_H__
#define __TQS_ALGO_DBOG_ACTION_FUNCTION_H__

#include "TqsAlgoAction_Base.h"


class CTqsAlgoAction_Function : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_Function(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_Function();

public:

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif