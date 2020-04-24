#include "stdafx.h"
#include "WpsAlgoStagePrepare.h"


CWpsAlgoStagePrepare::CWpsAlgoStagePrepare(CWpsAlgoObject* pOwner) :
CScriptAlgoStage(0, pOwner, SCRIPTCONTROL_PREPARE, "SCRIPTCONTROL_PREPARE")
{
}


CWpsAlgoStagePrepare::~CWpsAlgoStagePrepare()
{
}


int CWpsAlgoStagePrepare::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	printf("CWpsAlgoStagePrepare::OnUpdate \n");
	return m_status;
}

