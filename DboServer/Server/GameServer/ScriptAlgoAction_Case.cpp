#include "stdafx.h"
#include "ScriptAlgoAction_Case.h"
#include "WPSNodeAction_Case.h"


CWpsAlgoAction_Case::CWpsAlgoAction_Case(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CASE, "SCRIPTCONTROL_ACTION_CASE")
{
}


CWpsAlgoAction_Case::~CWpsAlgoAction_Case()
{
}


bool CWpsAlgoAction_Case::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_Case* pAction = dynamic_cast<CWPSNodeAction_Case*>(pControlScriptNode);
	if (pAction)
	{
		m_nCompareValue = pAction->m_nCompareValue;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_Case", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_Case::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_status = UpdateSubControlQueue(dwTickDiff, fMultiple); //UPDATE ACTIONS
	//printf("update case %u \n", m_nCompareValue);
	return m_status;
}
