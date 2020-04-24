#include "stdafx.h"
#include "WPSNodeAction_Calculate.h"
#include "ControlScriptNodeParam_String.h"
#include "SvrScrFormulaToken.h"
#include "SvrScrFormulaExpress.h"
#include "SvrScrFormulaCalc.h"
#include "SvrScrFormulaDeclare.h"
#include "SvrScrVariableMap.h"
#include "ControlScript.h"


CWPSNodeAction_Calculate::CWPSNodeAction_Calculate(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_CALCULATE, lpszName)
{
}

CWPSNodeAction_Calculate::~CWPSNodeAction_Calculate()
{
}

CWPSNodeAction_Calculate::sPARAMETER_INFO* CWPSNodeAction_Calculate::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_Calculate::AddParam(CControlScriptNodeParam_String* pNode)
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

	printf("CWPSNodeAction_Calculate: [%s] not exist \n", name);
	return false;
}


bool CWPSNodeAction_Calculate::CheckParameter()
{
	CSvrScrFormulaToken cToken;
	if (!cToken.Create())
	{
		ERR_LOG(LOG_SCRIPT, "Cant create CSvrScrFormulaToken");
		return false;
	}

	if (m_strDeclare.size() > 0)
	{
		if (!cToken.Separate(m_strDeclare))
		{
			ERR_LOG(LOG_SCRIPT, "Fail AddSeparator(2) m_strDeclare = [%s]", m_strDeclare.c_str());
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
			ERR_LOG(LOG_SCRIPT, "Fail ccDeclare.Analyze. Message = [%s]", cDeclare.GetMessageA());
			return false;
		}

		CSvrScrVariableMap* pVariableMap = GetValidVariableMap();
		if (!pVariableMap)
		{
			ERR_LOG(LOG_SCRIPT, "Fail pVariableMap is null");
			return false;
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
	}

	if (m_strExpress.size() > 0)
	{
		if (!cToken.Separate(m_strExpress))
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
			ERR_LOG(LOG_SCRIPT, "Fail Analyze. Msg = [%s]", cExpress.GetMessageA());
			return false;
		}

		CSvrScrFormulaCalc cCalc(this);
		if (!cCalc.Calculate(m_sFormula))
		{
			ERR_LOG(LOG_SCRIPT, "Cant Calculate fail. Msg = [%s]", cCalc.GetMessageA());
			return false;
		}
	}

	return true;
}