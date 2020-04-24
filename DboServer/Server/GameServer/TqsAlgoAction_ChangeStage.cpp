#include "stdafx.h"
#include "TqsAlgoAction_ChangeStage.h"
#include "TQSNodeAction_ChangeStage.h"


CTqsAlgoAction_ChangeStage::CTqsAlgoAction_ChangeStage(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_CHANGE_STAGE, "TQS_ALGOCONTROLID_ACTION_CHANGE_STAGE")
{
}


CTqsAlgoAction_ChangeStage::~CTqsAlgoAction_ChangeStage()
{
}


bool CTqsAlgoAction_ChangeStage::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_ChangeStage* pAction = dynamic_cast<CTQSNodeAction_ChangeStage*>(pControlScriptNode);
	if (pAction)
	{
		m_byNextStage = pAction->m_byNextStage;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to TQSNodeAction_ChangeStage", pControlScriptNode);
	return false;
}

int CTqsAlgoAction_ChangeStage::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if(m_byNextStage != 255)
		GetOwner()->GetController()->ChangeControlStage_Stage(m_byNextStage, SSD_SCRIPT_TYPE_TQS);
	else
		GetOwner()->GetController()->ChangeControlStage_End();

	return CHANGED;
}
