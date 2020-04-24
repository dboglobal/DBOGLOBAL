#include "stdafx.h"
#include "SPSNodeCondition_BSCondition.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"



CSPSNodeCondition_BSCondition::CSPSNodeCondition_BSCondition(const char* lpszName)
:CControlScriptNodeCondition(SPS_NODE_CONDITION_BS_CONDITION, lpszName)
{
	m_bOperatorAnd = true;
	m_dwPreCoolTime = 0;
	m_dwCoolTime = 0;
}

CSPSNodeCondition_BSCondition::~CSPSNodeCondition_BSCondition()
{
}


bool CSPSNodeCondition_BSCondition::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "cooltime") == 0)
	{
		m_dwCoolTime = (DWORD)floor(pNode->GetValue() * 1000.0f);
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "precooltime") == 0)
	{
		m_dwPreCoolTime = (DWORD)floor(pNode->GetValue() * 1000.0f);
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeCondition_BSCondition: [%s] not exist \n", name);
	return false;
}

bool CSPSNodeCondition_BSCondition::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "operation") == 0)
	{
		const char* sub = pNode->GetName();

		if (_stricmp(sub, "and") == 0)
		{
			m_bOperatorAnd = true;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "or") == 0)
		{
			m_bOperatorAnd = false;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeCondition_BSCondition: sub [%s] not exist \n", sub);
		return false;
	}

	printf("CSPSNodeCondition_BSCondition: [%s] not exist \n", name);
	return false;
}