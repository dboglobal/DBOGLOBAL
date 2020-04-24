#include "stdafx.h"
#include "ScriptAlgoAction_Calculate.h"
#include "WPSNodeAction_Calculate.h"
#include "SvrScrVariableMap.h"
#include "SvrScrFormulaCalc.h"


CWpsAlgoAction_Calculate::CWpsAlgoAction_Calculate(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CALCULATE, "SCRIPTCONTROL_ACTION_CALCULATE")
{

}


CWpsAlgoAction_Calculate::~CWpsAlgoAction_Calculate()
{
}


bool CWpsAlgoAction_Calculate::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_Calculate* pAction = dynamic_cast<CWPSNodeAction_Calculate*>(pControlScriptNode);
	if (pAction)
	{
		m_sDeclareList = pAction->m_sDeclareList;
		m_sFormula = pAction->m_sFormula;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_Calculate", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_Calculate::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CSvrScrVariableMap* pValidVariableMap = GetValidVariableMap();
	if (!pValidVariableMap)
	{
		printf("NULL == pVariableMap \n");
		m_status = FAILED;
		return m_status;
	}

	for (std::vector<sSSD_FORMULA_DECLARE>::iterator it = m_sDeclareList.begin(); it != m_sDeclareList.end(); it++)
	{
		sSSD_FORMULA_DECLARE& declare = *it;

		pValidVariableMap->Declare(declare.sName.str(), declare.sVariable);

		//it can happen that it is already declared because some times we reload a function. This is why we dont need to return failed when it is already declared
	}

	if (m_sFormula.vecEntities.size() > 0)
	{
		CSvrScrFormulaCalc cCalc(this);
		if (!cCalc.Calculate(m_sFormula))
		{
			printf("Cant Calculate fail. Msg = [%s]", cCalc.GetMessageA());
			m_status = FAILED;
			return m_status;
		}
	}

	m_status = COMPLETED;
	return m_status;
}
