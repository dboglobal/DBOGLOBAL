#ifndef __TQS_SCRIPT_ALGO_DBOG_STAGE_END_H__
#define __TQS_SCRIPT_ALGO_DBOG_STAGE_END_H__

#include "TqsAlgoStage.h"


class CTqsAlgoStage_End : public CTqsAlgoStage
{

public:

	CTqsAlgoStage_End(CTqsAlgoObject* pOwner);
	virtual	~CTqsAlgoStage_End();


	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);


};

#endif