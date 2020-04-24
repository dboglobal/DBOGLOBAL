#include "stdafx.h"
#include "ScriptAlgoAction_Switch.h"
#include "WPSNodeAction_Switch.h"
#include "SvrScrVariableMap.h"
#include "ScriptAlgoAction_Case.h"


CWpsAlgoAction_Switch::CWpsAlgoAction_Switch(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_SWITCH, "SCRIPTCONTROL_ACTION_SWITCH")
{
}


CWpsAlgoAction_Switch::~CWpsAlgoAction_Switch()
{
}


bool CWpsAlgoAction_Switch::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_Switch* pAction = dynamic_cast<CWPSNodeAction_Switch*>(pControlScriptNode);
	if (pAction)
	{
		m_strCompareVariable = pAction->m_strCompareVariable;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_Switch", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_Switch::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CSvrScrVariableMap* pValidVariableMap = GetValidVariableMap();
	if (!pValidVariableMap)
	{
		printf("NULL == pVariableMap \n");
		m_status = FAILED;
		return m_status;
	}

	int nVal = pValidVariableMap->GetVariable(m_strCompareVariable)->nInteger;

	/*
		UPDATE THE "CASE" ACTION. WHEN CASE FINISH THEN SWITCH FINISH TOO!
		First Loop: stop and delete all cases which not needed.
	*/

	CWpsAlgoAction_Case* pCase = NULL;
	for (pCase = (CWpsAlgoAction_Case *)m_subControlQueue.GetFirst();
		pCase && (pCase->GetCompareValue() != nVal);
		pCase = (CWpsAlgoAction_Case *)m_subControlQueue.GetFirst())
	{
		pCase->Stop();

		m_subControlQueue.Remove(pCase);
		SAFE_DELETE(pCase);
	}

	if (!pCase)
	{
		m_status = COMPLETED;
		return m_status;
	}


	m_status = pCase->OnUpdate(dwTickDiff, fMultiple);

	if (m_status != ACTIVATED)
	{
		pCase->Stop();

		m_subControlQueue.Remove(pCase);
		SAFE_DELETE(pCase);
	}

	//printf("switch case %u, status %u \n", nVal, m_status);

	return m_status;
}
