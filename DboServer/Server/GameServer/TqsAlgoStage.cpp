#include "stdafx.h"
#include "TqsAlgoStage.h"
#include "TimeQuestControlFactory.h"
#include "TimeQuestScriptManager.h"


CTqsAlgoStage::CTqsAlgoStage(BYTE byStageNumber, CTqsAlgoObject* pOwner, eTQS_ALGOCONTROLID controlID, const char* lpszControlName) :
	m_byStageNumber(byStageNumber),
	CTqsAlgoBase(pOwner, controlID, lpszControlName)
{
}


CTqsAlgoStage::~CTqsAlgoStage()
{
}


void CTqsAlgoStage::OnEnter()
{
	if (CreateVariableMap())
	{
		CTimeQuestControlFactory factory(GetOwner());
		if (!g_pTimeQuestScriptManager->AttachScript(this, &factory, GetOwner()->GetScriptID(), GetStageNumber()))
		{
			ERR_LOG(LOG_SCRIPT, "FAIL : CTqsAlgoStage::OnEnter GetStageNumber %u GetScriptID %u\n", GetStageNumber(), GetOwner()->GetScriptID());
			m_status = SYSTEMERROR;
		}
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "false == CreateVariableMap()");
		m_status = SYSTEMERROR;
	}
}


int CTqsAlgoStage::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	int nStatus = UpdateSubControlList(dwTickDiff, fMultiple); //UPDATE CONDITION

	if (nStatus != CHANGED) //dont execute when condition is CHANGED (tmq failed)
		nStatus = UpdateSubControlQueue(dwTickDiff, fMultiple); //UPDATE ACTTION

	return m_status;
}


