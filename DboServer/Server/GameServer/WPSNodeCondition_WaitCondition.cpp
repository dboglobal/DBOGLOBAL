#include "stdafx.h"
#include "WPSNodeCondition_WaitCondition.h"
#include "ControlScriptNodeParam_String.h"



CWPSNodeCondition_WaitCondition::CWPSNodeCondition_WaitCondition(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_WAIT_CONDITION, lpszName)
{
}

CWPSNodeCondition_WaitCondition::~CWPSNodeCondition_WaitCondition()
{

}


bool CWPSNodeCondition_WaitCondition::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "operation") == 0)
	{
		if (_stricmp(pNode->GetValue(), "and") == 0)
		{
			m_bOperationAND = true;
			return CControlScriptNode::AddParam(pNode);
		}
		if (_stricmp(pNode->GetValue(), "or") == 0)
		{
			m_bOperationAND = false;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CWPSNodeCondition_WaitCondition: [%s] not exist \n", name);
	return false;
}