#include "stdafx.h"
#include "WPSNodeAction_GetFaintLocation.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlObject.h"
#include "StringSeparator.h"
#include "SvrScrVariableMap.h"

CWPSNodeAction_GetFaintLocation::CWPSNodeAction_GetFaintLocation(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_GET_FAINTLOCATION, lpszName)
{
}

CWPSNodeAction_GetFaintLocation::~CWPSNodeAction_GetFaintLocation()
{

}

CWPSNodeAction_GetFaintLocation::sPARAMETER_INFO* CWPSNodeAction_GetFaintLocation::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_GetFaintLocation::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "target index") == 0)
	{
		m_dwTargetIndex = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_GetFaintLocation: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_GetFaintLocation::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "target type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "mob") == 0)
		{
			m_eTargetType = SSD_TARGET_MOB;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	if (_stricmp(name, "loc var") == 0)
	{
		m_strLoc = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "dir var") == 0)
	{
		m_strDir = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "default") == 0)
	{
		m_strDefault = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_GetFaintLocation: [%s] not exist \n", name);
	return false;
}


bool CWPSNodeAction_GetFaintLocation::CheckParameter()
{
	//set loc/dir default value
	CStringSeparator cToken;
	cToken.AddRemover(" ");
	cToken.AddRemover("	");
	cToken.AddRemover(",");
	cToken.AddRemover("(");
	cToken.AddRemover(")");
	cToken.AddRemover("Loc");
	cToken.AddRemover("Dir");

	if(!cToken.Separate(m_strDefault))
	{
		ERR_LOG(LOG_SCRIPT, "Couldnt Separate.");
		return false;
	}

	int n = 0;

	for (CStringSeparator::TOKEN_IT it = cToken.Begin(); it != cToken.End(); it++)
	{
		std::string& strString = *it;
		if (CStringSeparator::IsFloat(strString))
		{
			switch (n)
			{
				case 0: m_cDefaultLoc.x = (float)atof(strString.c_str()); break;
				case 1: m_cDefaultLoc.y = (float)atof(strString.c_str()); break;
				case 2: m_cDefaultLoc.z = (float)atof(strString.c_str()); break;

				case 3: m_cDefaultDir.x = (float)atof(strString.c_str()); break;
				case 4: m_cDefaultDir.y = (float)atof(strString.c_str()); break;
				case 5: m_cDefaultDir.z = (float)atof(strString.c_str()); break;

				default: printf("not found \n"); break;
			}

			++n;
		}
	}


	sSSD_VARIABLE* pLocVar = GetVariable(m_strLoc);
	if (!pLocVar)
	{
		ERR_LOG(LOG_SCRIPT, "Could not find m_strLoc %s \n", m_strLoc.c_str());
		return false;
	}
	sSSD_VARIABLE* pDirVar = GetVariable(m_strDir);
	if (!pDirVar)
	{
		ERR_LOG(LOG_SCRIPT, "Could not find m_strDir %s \n", m_strDir.c_str());
		return false;
	}

	pLocVar->operator=(m_cDefaultLoc);
	pDirVar->operator=(m_cDefaultDir);

	//printf("loc %f %f %f, dir %f %f %f \n", m_cDefaultLoc.x, m_cDefaultLoc.y, m_cDefaultLoc.z, m_cDefaultDir.x, m_cDefaultDir.y, m_cDefaultDir.z);

	return true;
}