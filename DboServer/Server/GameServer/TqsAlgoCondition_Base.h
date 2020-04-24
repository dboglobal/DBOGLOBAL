#ifndef __TQS_ALGO_DBOG_CONDITION_H__
#define __TQS_ALGO_DBOG_CONDITION_H__

#include "TqsAlgoBase.h"

class CTqsAlgoCondition_Base : public CTqsAlgoBase
{

public:
	CTqsAlgoCondition_Base(CTqsAlgoObject* pOwner, eTQS_ALGOCONTROLID controlID, const char* lpszControlName) :
		CTqsAlgoBase(pOwner, controlID, lpszControlName)
	{
	}

	virtual	~CTqsAlgoCondition_Base() {}

};

#endif