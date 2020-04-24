#include "stdafx.h"
#include "WpsAlgoStageFailed.h"
#include "WorldPlayControlFactory.h"
#include "WorldPlayScriptManager.h"


CWpsAlgoStageFailed::CWpsAlgoStageFailed(CWpsAlgoObject* pOwner) :
CScriptAlgoStage(INVALID_BYTE, pOwner, SCRIPTCONTROL_FAILED, "SCRIPTCONTROL_FAILED")
{
}


CWpsAlgoStageFailed::~CWpsAlgoStageFailed()
{
}


void CWpsAlgoStageFailed::OnEnter()
{
	//printf("CWpsAlgoStageFailed::OnEnter GetStageNumber %u GetScriptID %u\n", GetStageNumber(), GetOwner()->GetScriptID());

	if (CreateVariableMap())
	{
		CWorldPlayControlFactory factory(GetOwner());
		if (!g_pWorldPlayScriptManager->AttachScript(this, &factory, GetOwner()->GetScriptID(), GetStageNumber()))
		{
			m_status = SYSTEMERROR;
		}
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "false == CreateVariableMap()");
		m_status = SYSTEMERROR;
	}
}


int CWpsAlgoStageFailed::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	int nStatus = UpdateSubControlList(dwTickDiff, fMultiple);
	nStatus = UpdateSubControlQueue(dwTickDiff, fMultiple);

	if (nStatus == COMPLETED || nStatus == SYSTEMERROR)//end script
	{
		m_status = COMPLETED;
	//	NTL_PRINT(PRINT_APP, "Stage-Failed status %u", nStatus);

		GetOwner()->QuestFailed();
		GetOwner()->Stop();
	}

	return m_status;
}

