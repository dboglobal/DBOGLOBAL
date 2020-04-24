#include "stdafx.h"
#include "WpsScriptAlgoAction_CCBD.h"
#include "WpsAlgoStage.h"


CWpsScriptAlgoAction_CCBD::CWpsScriptAlgoAction_CCBD(CWpsAlgoObject* pObject) :
	CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CCBD, "SCRIPTCONTROL_ACTION_CCBD")
{
}


CWpsScriptAlgoAction_CCBD::~CWpsScriptAlgoAction_CCBD()
{
}

int CWpsScriptAlgoAction_CCBD::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CWpsAlgoStage* pStage = dynamic_cast<CWpsAlgoStage*>(m_pParent);
	if (pStage)
	{
		pStage->InitMobList();
	}
	else printf("dynamic_cast m_pParent -> CWpsAlgoStage fail\n");

	m_status = COMPLETED;
	return m_status;
}
