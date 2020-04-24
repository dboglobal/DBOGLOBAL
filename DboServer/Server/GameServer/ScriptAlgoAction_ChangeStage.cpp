#include "stdafx.h"
#include "ScriptAlgoAction_ChangeStage.h"
#include "WPSNodeAction_ChangeStage.h"


CWpsAlgoAction_ChangeStage::CWpsAlgoAction_ChangeStage(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CHANGE_STAGE, "SCRIPTCONTROL_ACTION_CHANGE_STAGE")
{
}


CWpsAlgoAction_ChangeStage::~CWpsAlgoAction_ChangeStage()
{
}


bool CWpsAlgoAction_ChangeStage::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_ChangeStage* pAction = dynamic_cast<CWPSNodeAction_ChangeStage*>(pControlScriptNode);
	if (pAction)
	{
		m_byNextStage = pAction->m_byNextStage;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_ChangeStage", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_ChangeStage::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
