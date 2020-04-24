#include "stdafx.h"
#include "TQSNodeAction_Wait.h"
#include "ControlScriptNodeParam_String.h"


CTQSNodeAction_Wait::CTQSNodeAction_Wait(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_WAIT, lpszName)
{
	m_bOperationAND = false;
}

CTQSNodeAction_Wait::~CTQSNodeAction_Wait()
{

}

CTQSNodeAction_Wait::sPARAMETER_INFO* CTQSNodeAction_Wait::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_Wait::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();

	if (_stricmp(name, "operation") == 0)
	{
		if (_stricmp(pNode->GetValue(), "and") == 0)
			m_bOperationAND = true;
		else if (_stricmp(pNode->GetValue(), "or") == 0)
			m_bOperationAND = false;

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_Wait: [%s] not exist \n", name);
	return false;
}
