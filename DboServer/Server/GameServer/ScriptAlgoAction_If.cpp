#include "stdafx.h"
#include "ScriptAlgoAction_If.h"
#include "WPSNodeAction_If.h"
#include "ControlAction.h"


CWpsAlgoAction_If::CWpsAlgoAction_If(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_IF, "SCRIPTCONTROL_ACTION_IF")
{
}


CWpsAlgoAction_If::~CWpsAlgoAction_If()
{
}


bool CWpsAlgoAction_If::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_If* pAction = dynamic_cast<CWPSNodeAction_If*>(pControlScriptNode);
	if (pAction)
	{
		m_bOperationAND = pAction->m_bOperationAND;
		m_bReverse = pAction->m_bReverse;


		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_If", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_If::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	//if m_bReverse == true(default) -> check if condition is true
	//if m_bReverse == false -> check if condition is false

	int nStatus = UpdateSubControlList(dwTickDiff, fMultiple); //UPDATE CONDITION

	int nExecuteControlId = INVALID_SCRIPTCONTROLID;

	//it seems like if m_bReverse is true or not, all is same result.
	if (m_bReverse)
	{
		if (nStatus == COMPLETED)
			nExecuteControlId = SCRIPTCONTROL_ACTION_THEN;
		else
			nExecuteControlId = SCRIPTCONTROL_ACTION_ELSE;
	}
	else
	{
		if(nStatus == COMPLETED)
			nExecuteControlId = SCRIPTCONTROL_ACTION_THEN;
		else
			nExecuteControlId = SCRIPTCONTROL_ACTION_ELSE;
	}

	//printf("nExecuteControlId %u \n", nExecuteControlId);

	CControlState* pLinkObject = (CControlState*)m_subControlQueue.GetFirst(); //get ACTION
	if (pLinkObject == NULL)
	{
		return m_status;
	}

	bool bFound = false;
	CControlState* pNextState = NULL;
	while (pLinkObject)
	{
		pNextState = (CControlState*)pLinkObject->GetNext();

		if (pLinkObject->GetControlStateID() == nExecuteControlId)
		{
			pLinkObject->Start();

			if (pLinkObject->IsStatus(ACTIVATED))
			{
				bFound = true;
				m_status = pLinkObject->OnUpdate(dwTickDiff, fMultiple);
			//	printf("updated nExecuteControlId %u | m_status %u\n", nExecuteControlId, m_status);
			}

			break;
		}

		pLinkObject = pNextState;
	}

	if (!bFound) //IT CAN HAPPEN THAT AS EXAMPLE 'THEN' SHOULD BE EXECUTED BUT 'THEN' DOES NOT EXIST SO JUST COMPLETE IT!
		m_status = COMPLETED;

	return m_status;
}
