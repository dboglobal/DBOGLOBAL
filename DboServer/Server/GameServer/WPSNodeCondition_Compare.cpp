#include "stdafx.h"
#include "WPSNodeCondition_Compare.h"
#include "ControlScriptNodeParam_String.h"
#include "SvrScrFormulaToken.h"
#include "SvrScrFormulaExpress.h"


CWPSNodeCondition_Compare::CWPSNodeCondition_Compare(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_COMPARE, lpszName)
{
}

CWPSNodeCondition_Compare::~CWPSNodeCondition_Compare()
{

}


bool CWPSNodeCondition_Compare::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "express") == 0)
	{
		m_strExpress = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_ChangeCharAttribute: [%s] not exist \n", name);
	return false;
}


bool CWPSNodeCondition_Compare::CheckParameter()
{
	CSvrScrFormulaToken cToken;
	if (!cToken.Create())
	{
		ERR_LOG(LOG_SCRIPT, "Cant create CSvrScrFormulaToken");
		return false;
	}

	if (m_strExpress.size() > 0)
	{
		if (!cToken.Separate(m_strExpress))
		{
			ERR_LOG(LOG_SCRIPT, "Fail AddSeparator");
			return false;
		}

		CSvrScrFormulaExpress express;
		express.Create();

		int i = 0;

		for (CSvrScrFormulaToken::TOKEN_IT it = cToken.Begin(); it != cToken.End(); it++)
		{
			std::string& rSrc = *it;

			if (i == 0)
			{
				express.GetEntity(m_sLeft, rSrc);
			}
			else if (i == 1)
			{
				if (_stricmp(rSrc.c_str(), "==") == 0)
					m_eOperator = SSD_VARIABLE_COMPARE_OPERATOR_EQUAL;
				else if (_stricmp(rSrc.c_str(), "!=") == 0)
					m_eOperator = SSD_VARIABLE_COMPARE_OPERATOR_NOT_EQUAL;
				else
				{
					ERR_LOG(LOG_SCRIPT, "Error compare variable '%s' not set !!!", rSrc.c_str());
					return false;
				}
			}
			else if (i == 2)
			{
				express.GetEntity(m_sRight, rSrc);
			}
			else
			{
				ERR_LOG(LOG_SCRIPT, "Error compare got more than 3 values !!!");
				return false;
			}

			i++;
		}
	}

	return true;
}