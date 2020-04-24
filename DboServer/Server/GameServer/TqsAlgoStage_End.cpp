#include "stdafx.h"
#include "TqsAlgoStage_End.h"
#include "TimeQuest.h"


CTqsAlgoStage_End::CTqsAlgoStage_End(CTqsAlgoObject* pOwner) :
	CTqsAlgoStage(INVALID_BYTE, pOwner, TQS_ALGOCONTROLID_END, "TQS_ALGOCONTROLID_END")
{
}


CTqsAlgoStage_End::~CTqsAlgoStage_End()
{
}

int CTqsAlgoStage_End::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_subControlQueue.GetCount() == 0)
	{
		GetOwner()->GetTimeQuest()->SetState(TIMEQUEST_GAME_STATE_END);

		m_status = COMPLETED;
		return m_status;
	}

	return CTqsAlgoStage::OnUpdate(dwTickDiff, fMultiple);
}

