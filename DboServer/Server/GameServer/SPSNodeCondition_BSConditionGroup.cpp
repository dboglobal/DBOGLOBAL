#include "stdafx.h"
#include "SPSNodeCondition_BSConditionGroup.h"
#include "ControlScriptNodeParam_String.h"



CSPSNodeCondition_BSConditionGroup::CSPSNodeCondition_BSConditionGroup(const char* lpszName)
:CControlScriptNodeCondition(SPS_NODE_CONDITION_BS_CONDITION_GROUP, lpszName)
{
	m_bOperatorAnd = true;
}

CSPSNodeCondition_BSConditionGroup::~CSPSNodeCondition_BSConditionGroup()
{
}


bool CSPSNodeCondition_BSConditionGroup::AddParam(CControlScriptNodeParam_String* pNode)
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

		printf("CSPSNodeCondition_BSConditionGroup: sub [%s] not exist \n", sub);
		return false;
	}

	printf("CSPSNodeCondition_BSConditionGroup: [%s] not exist \n", name);
	return false;
}