#include "stdafx.h"
#include "TqsAlgoStage_Stage.h"


CTqsAlgoStage_Stage::CTqsAlgoStage_Stage(BYTE byStageNumber, CTqsAlgoObject* pOwner) :
	CTqsAlgoStage(byStageNumber, pOwner, TQS_ALGOCONTROLID_STAGE, "TQS_ALGOCONTROLID_STAGE")
{
}


CTqsAlgoStage_Stage::~CTqsAlgoStage_Stage()
{
}
