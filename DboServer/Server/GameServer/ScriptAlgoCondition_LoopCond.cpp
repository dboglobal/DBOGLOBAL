#include "stdafx.h"
#include "ScriptAlgoCondition_LoopCond.h"

#include "WPSNodeCondition_LoopCond.h"
#include "ControlScriptNodeParam.h"
#include "SvrScrVariableMap.h"

CWpsAlgoCondition_LoopCond::CWpsAlgoCondition_LoopCond(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_LOOPCOND, "SCRIPTCONTROL_CONDITION_LOOPCOND")
{
}


CWpsAlgoCondition_LoopCond::~CWpsAlgoCondition_LoopCond()
{
}


bool CWpsAlgoCondition_LoopCond::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_LoopCond* pLoopCond = dynamic_cast<CWPSNodeCondition_LoopCond*>(pControlScriptNode);
	if (pLoopCond)
	{
		m_bOperationAND = pLoopCond->m_bOperationAND;
		m_bReverse = pLoopCond->m_bReverse;
		return true;
	}

	return false;
}

int CWpsAlgoCondition_LoopCond::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (!m_bReverse && m_bOperationAND) //ENDLESS LOOP
	{
		GetOwner()->GetController()->ChangeControlStage_Stage(0, SSD_SCRIPT_TYPE_WPS);
		return CHANGED;
	}
	else
	{
		int nStatus = ACTIVATED;

		CControlState* pLinkObject = (CControlState*)m_subControlList.GetFirst(); //get condition
		if (pLinkObject == NULL)
		{
			return nStatus;
		}

		pLinkObject->Start();

		if (pLinkObject->IsStatus(ACTIVATED))
		{
			nStatus = pLinkObject->OnUpdate(dwTickDiff, fMultiple);

			if (m_bReverse)
			{
				if (nStatus == COMPLETED) //check if "fail"
				{
					m_status = ACTIVATED; //set success
				}
				else
				{
					m_status = COMPLETED; //set fail
				}
			}
			else
			{
				m_status = nStatus;
			}

		//	printf("nStatus %u m_status %u\n", nStatus, m_status);
		}
	}

	return m_status;
}
