#include "stdafx.h"
#include "SPSNodeAction_Calculate.h"
#include "ControlScriptNodeParam_String.h"
#include "SvrScrFormulaToken.h"
#include "SvrScrFormulaExpress.h"
#include "SvrScrFormulaCalc.h"
#include "SvrScrFormulaDeclare.h"
#include "SvrScrVariableMap.h"
#include "ControlScript.h"



CSPSNodeAction_Calculate::CSPSNodeAction_Calculate(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_CALCULATE, lpszName)
{
}

CSPSNodeAction_Calculate::~CSPSNodeAction_Calculate()
{

}

CControlScriptNode::sPARAMETER_INFO* CSPSNodeAction_Calculate::GetParameterMap()
{
	return NULL;
}


bool CSPSNodeAction_Calculate::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "declare") == 0)
	{
		m_strDeclare = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	
	if (_stricmp(name, "express") == 0)
	{
		m_strExpress = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_Calculate: [%s] not exist \n", name);
	return false;
}


bool CSPSNodeAction_Calculate::CheckParameter()
{
	CSvrScrFormulaToken cToken;
	if (!cToken.Create())
	{
		ERR_LOG(LOG_SCRIPT, "Cant create CSvrScrFormulaToken");
		return false;
	}

	if (m_strDeclare.size() == 0)
	{
	LABEL_33:

		if (m_strExpress.size() > 0)
		{
			if (!cToken.AddSeparator(m_strExpress))
			{
				ERR_LOG(LOG_SCRIPT, "Fail AddSeparator");
				return false;
			}

			CSvrScrFormulaExpress cExpress;
			if (!cExpress.Create())
			{
				ERR_LOG(LOG_SCRIPT, "Cant create CSvrScrFormulaExpress");
				return false;
			}

			if (!cExpress.Analyze(m_sFormula, cToken))
			{
				ERR_LOG(LOG_SCRIPT, "Fail Analyze");
				return false;
			}

			CSvrScrFormulaCalc cCalc(this);
			if (!cCalc.Calculate(m_sFormula))
			{
				ERR_LOG(LOG_SCRIPT, "Cant Calculate CSvrScrFormulaCalc");
				return false;
			}

			return true;
		}
	}


	if (!cToken.AddSeparator(m_strDeclare))
	{
		ERR_LOG(LOG_SCRIPT, "Fail AddSeparator(2)");
		return false;
	}

	CSvrScrFormulaDeclare cDeclare;
	if (!cDeclare.Create())
	{
		ERR_LOG(LOG_SCRIPT, "Fail create CSvrScrFormulaDeclare");
		return false;
	}

	if (!cDeclare.Analyze(m_sDeclareList, cToken))
	{
		ERR_LOG(LOG_SCRIPT, "Fail ccDeclare.Analyze");
		return false;
	}

	CSvrScrVariableMap* pVariableMap = GetValidVariableMap();
	if (!pVariableMap)
	{
		ERR_LOG(LOG_SCRIPT, "Fail pVariableMap is null");
		return false;
	}

	if (m_sDeclareList.size() == 0)
	{
		goto LABEL_33;
	}

	for (std::vector<sSSD_FORMULA_DECLARE>::iterator it = m_sDeclareList.begin(); it != m_sDeclareList.end(); it++)
	{
		sSSD_FORMULA_DECLARE& pRes = *it;

		eSSD_VARIABLE_RESULT eVarResult = pVariableMap->Declare(pRes.sName.str(), pRes.sVariable);

		if (eVarResult != SSD_VARIABLE_RESULT_SUCCESS)
		{
			ERR_LOG(LOG_SCRIPT, "Declare failed. eVarResult = %u, sName = %s, GetFileName() = %s", eVarResult, pRes.sName.str(), GetScript()->GetFileName());
			return false;
		}
	}

	return true;
}