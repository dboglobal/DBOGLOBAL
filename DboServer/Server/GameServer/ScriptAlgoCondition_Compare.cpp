#include "stdafx.h"
#include "ScriptAlgoCondition_Compare.h"
#include "WPSNodeCondition_Compare.h"
#include "SvrScrVariableMap.h"


CWpsAlgoCondition_Compare::CWpsAlgoCondition_Compare(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_COMPARE, "SCRIPTCONTROL_CONDITION_COMPARE")
{
}


CWpsAlgoCondition_Compare::~CWpsAlgoCondition_Compare()
{
}


bool CWpsAlgoCondition_Compare::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_Compare* pCond = dynamic_cast<CWPSNodeCondition_Compare*>(pControlScriptNode);
	if (pCond)
	{
		m_strExpress = pCond->m_strExpress;
		m_sLeft = pCond->m_sLeft;
		m_eOperator = pCond->m_eOperator;
		m_sRight = pCond->m_sRight;

		return true;
	}

	return false;
}

int CWpsAlgoCondition_Compare::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	// IN CASE THIS IS CALLED BY "LOOP COND" DO NOT SET m_status. RETURN STATUS DIRECTLY TO AVOID BUG.

	//printf("m_strExpress '%s' \n LEFT-sVarName '%s' m_eOperator '%u' RIGHT-sVarName '%s' \n", m_strExpress.c_str(), m_sLeft.sVarName.str(), m_eOperator, m_sRight.sVarName.str());

	sSSD_VARIABLE* pVarLeft = GetVariable(CNtlString(m_sLeft.sVarName.str()).GetString(), SSD_VARIABLE_INTEGER);
	if (pVarLeft == NULL)
		pVarLeft = &m_sLeft.sConstant;

	sSSD_VARIABLE* pVarRight = GetVariable(CNtlString(m_sRight.sVarName.str()).GetString(), SSD_VARIABLE_INTEGER);
	if (pVarRight == NULL)
		pVarRight = &m_sRight.sConstant;

	//printf("int value left %u, right %u\n", );
	//if (GetOwner()->GetScriptID() == 34007)
	//	printf("left %s %i right %s %i \n", m_sLeft.sVarName.str(), pVarLeft->nInteger, m_sRight.sVarName.str(), pVarRight->nInteger);

	if (sSSD_VARIABLE::Compare(pVarLeft, m_eOperator, pVarRight) == SSD_COMPARE_TRUE)
	{
	//	printf("compare success left %s right %s \n", m_sLeft.sVarName.str(), m_sRight.sVarName.str());
		return COMPLETED;
	}

	return m_status;
}
