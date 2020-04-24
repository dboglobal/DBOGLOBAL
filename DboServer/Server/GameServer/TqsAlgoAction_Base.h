#ifndef __TQS_ALGO_DBOG_ACTION_H__
#define __TQS_ALGO_DBOG_ACTION_H__

#include "TqsAlgoBase.h"

class CTqsAlgoAction_Base : public CTqsAlgoBase
{

public:
	CTqsAlgoAction_Base(CTqsAlgoObject* pOwner, eTQS_ALGOCONTROLID controlID, const char* lpszControlName) :
		CTqsAlgoBase(pOwner, controlID, lpszControlName)
	{
	}

	virtual	~CTqsAlgoAction_Base() {}

};

#endif