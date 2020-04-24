#include "stdafx.h"
#include "WPSNodeCondition_LoopCond.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeCondition_LoopCond::CWPSNodeCondition_LoopCond(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_LOOP_COND, lpszName)
{
	m_bOperationAND = false;
	m_bReverse = true;
}

CWPSNodeCondition_LoopCond::~CWPSNodeCondition_LoopCond()
{

}


bool CWPSNodeCondition_LoopCond::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "reverse") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
			m_bReverse = true;
		else if (_stricmp(pNode->GetValue(), "false") == 0)
			m_bReverse = false;
		else
		{
			printf("CWPSNodeCondition_LoopCond::AddParam reverse: [%s] not available \n", pNode->GetValue());
			return false;
		}

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "operation") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
			m_bOperationAND = true;
		else if (_stricmp(pNode->GetValue(), "false") == 0)
			m_bOperationAND = false;
		else
		{
			printf("CWPSNodeCondition_LoopCond::AddParam reverse: [%s] not available \n", pNode->GetValue());
			return false;
		}

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeCondition_LoopCond: [%s] not exist \n", name);

	return false;
}