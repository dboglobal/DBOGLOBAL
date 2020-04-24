#include "stdafx.h"
#include "WPSNodeAction_MobList.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "SvrScrFormulaToken.h"


CWPSNodeAction_MobList::CWPSNodeAction_MobList(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_MOB_LIST, lpszName)
{
}

CWPSNodeAction_MobList::~CWPSNodeAction_MobList()
{

}

CWPSNodeAction_MobList::sPARAMETER_INFO* CWPSNodeAction_MobList::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_MobList::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "index") == 0)
	{
		m_nIndex = (int)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "auto actual level") == 0)
	{
		m_byActualLevel = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "auto effect level") == 0)
	{
		m_byEffectiveLevel = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "mob1") == 0)
	{
		m_strMobList[0] = std::to_string((double)pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob2") == 0)
	{
		m_strMobList[1] = std::to_string((double)pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob3") == 0)
	{
		m_strMobList[2] = std::to_string((double)pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob4") == 0)
	{
		m_strMobList[3] = std::to_string((double)pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob5") == 0)
	{
		m_strMobList[4] = std::to_string((double)pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob6") == 0)
	{
		m_strMobList[5] = std::to_string((double)pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob7") == 0)
	{
		m_strMobList[6] = std::to_string((double)pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob8") == 0)
	{
		m_strMobList[7] = std::to_string((double)pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob9") == 0)
	{
		m_strMobList[8] = std::to_string((double)pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob10") == 0)
	{
		m_strMobList[9] = std::to_string((double)pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_MobList: [%s] not exist (number)\n", name);
	return false;
}

bool CWPSNodeAction_MobList::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "mob1") == 0)
	{
		m_strMobList[0] = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob2") == 0)
	{
		m_strMobList[1] = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob3") == 0)
	{
		m_strMobList[2] = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob4") == 0)
	{
		m_strMobList[3] = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob5") == 0)
	{
		m_strMobList[4] = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob6") == 0)
	{
		m_strMobList[5] = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob7") == 0)
	{
		m_strMobList[6] = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob8") == 0)
	{
		m_strMobList[7] = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob9") == 0)
	{
		m_strMobList[8] = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob10") == 0)
	{
		m_strMobList[9] = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_MobList: [%s] not exist (string)\n", name);
	return false;
}


bool CWPSNodeAction_MobList::CheckParameter()
{
	CSvrScrFormulaToken cToken;
	if (!cToken.Create())
		return false;
	
	cToken.AddRemover(",");

	for (int i = 0; i < 10; i++)
	{
		if (!cToken.Separate(m_strMobList[i]))
		{
			ERR_LOG(LOG_SCRIPT, "Fail AddSeparator");
			return false;
		}

		for (CStringSeparator::TOKEN_IT it = cToken.Begin(); it != cToken.End(); it++)
		{
			TBLIDX uiTblidx = (TBLIDX)stol(*it);

			m_aMobList[i].push_back(uiTblidx);
		}
	}

	return true;
}