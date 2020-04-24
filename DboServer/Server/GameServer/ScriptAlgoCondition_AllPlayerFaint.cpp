#include "stdafx.h"
#include "ScriptAlgoCondition_AllPlayerFaint.h"
#include "WPSNodeCondition_AllPlayerFaint.h"


CWpsAlgoCondition_AllPlayerFaint::CWpsAlgoCondition_AllPlayerFaint(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_ALL_PLAYER_FAINT, "SCRIPTCONTROL_CONDITION_ALL_PLAYER_FAINT")
{
}


CWpsAlgoCondition_AllPlayerFaint::~CWpsAlgoCondition_AllPlayerFaint()
{
}


bool CWpsAlgoCondition_AllPlayerFaint::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_AllPlayerFaint* pCond = dynamic_cast<CWPSNodeCondition_AllPlayerFaint*>(pControlScriptNode);
	if (pCond)
	{
		m_bWpsFail = pCond->m_bWpsFail;

		return true;
	}

	return false;
}

int CWpsAlgoCondition_AllPlayerFaint::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
