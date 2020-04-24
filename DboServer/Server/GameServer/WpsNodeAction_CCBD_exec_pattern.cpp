#include "stdafx.h"
#include "WpsNodeAction_CCBD_exec_pattern.h"
#include "ControlScriptNodeParam_String.h"
#include "SvrScrFormulaToken.h"
#include "SvrScrFormulaExpress.h"



CWpsNodeAction_CCBD_exec_pattern::CWpsNodeAction_CCBD_exec_pattern(const char* lpszName)
	:CControlScriptNodeAction(WPS_NODE_ACTION_CCBD_EXEC_PATTERN, lpszName)
{
}

CWpsNodeAction_CCBD_exec_pattern::~CWpsNodeAction_CCBD_exec_pattern()
{

}


bool CWpsNodeAction_CCBD_exec_pattern::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "pattern list") == 0)
	{
		m_strPatternList = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWpsNodeAction_CCBD_exec_pattern: [%s] not exist \n", name);
	return false;
}


bool CWpsNodeAction_CCBD_exec_pattern::CheckParameter()
{
	CSvrScrFormulaToken cToken;
	if (!cToken.Create())
	{
		ERR_LOG(LOG_SCRIPT, "Cant create CSvrScrFormulaToken");
		return false;
	}

	if (m_strPatternList.size() > 0)
	{
		cToken.AddRemover("(");
		cToken.AddRemover(",");
		cToken.AddRemover("%");
		cToken.AddRemover(")");

		if (!cToken.Separate(m_strPatternList))
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

		bool bPercent = false;
		BYTE byIndex = INVALID_BYTE;

		for (CSvrScrFormulaToken::TOKEN_IT it = cToken.Begin(); it != cToken.End(); it++)
		{
			std::string& rSrc = *it;

			if (bPercent == false)
			{
				byIndex = std::stoi(rSrc);
				bPercent = true;
			}
			else
			{
				m_vecPatterns.push_back(CNtlVector((float)byIndex, std::stof(rSrc), 0.0f));

				byIndex = INVALID_BYTE;
				bPercent = false;
			}
		}
	}

	return true;
}