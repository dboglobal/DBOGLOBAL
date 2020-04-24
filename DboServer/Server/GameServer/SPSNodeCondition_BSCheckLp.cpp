#include "stdafx.h"
#include "SPSNodeCondition_BSCheckLp.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"



CSPSNodeCondition_BSCheckLp::CSPSNodeCondition_BSCheckLp(const char* lpszName)
:CControlScriptNodeCondition(SPS_NODE_CONDITION_BS_CHECK_LP, lpszName)
{
	m_byMaxPercent = 100;
	m_byMinPercent = 1;
	m_bIsInRange = true;
}

CSPSNodeCondition_BSCheckLp::~CSPSNodeCondition_BSCheckLp()
{
}


bool CSPSNodeCondition_BSCheckLp::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "max percent") == 0)
	{
		m_byMaxPercent = (BYTE)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "min percent") == 0)
	{
		m_byMinPercent = (BYTE)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeCondition_BSCheckLp: [%s] not exist \n", name);
	return false;
}

bool CSPSNodeCondition_BSCheckLp::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "true") == 0)
	{
		const char* sub = pNode->GetName();

		if (_stricmp(sub, "true") == 0)
		{
			m_bIsInRange = true;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "false") == 0)
		{
			m_bIsInRange = false;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeCondition_BSCheckLp: sub [%s] not exist \n", sub);
		return false;
	}

	printf("CSPSNodeCondition_BSCheckLp: [%s] not exist \n", name);
	return false;
}