#ifndef __TQS_SCRIPT_ALGO_DBOG_STAGE_STAGE_H__
#define __TQS_SCRIPT_ALGO_DBOG_STAGE_STAGE_H__


#include "TqsAlgoStage.h"

class CTqsAlgoStage_Stage : public CTqsAlgoStage
{

public:

	CTqsAlgoStage_Stage(BYTE byStageNumber, CTqsAlgoObject* pOwner);
	virtual	~CTqsAlgoStage_Stage();

};

#endif